/*
 * f_acm.c -- USB CDC serial (ACM) function driver
 *
 * Copyright (C) 2003 Al Borchers (alborchers@steinerpoint.com)
 * Copyright (C) 2008 by David Brownell
 * Copyright (C) 2008 by Nokia Corporation
 * Copyright (C) 2009 by Samsung Electronics
 * Copyright (c) 2011 The Linux Foundation. All rights reserved.
 * Author: Michal Nazarewicz (mina86@mina86.com)
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

/*                       */

#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <mach/usb_gadget_xport.h>

#include "u_serial.h"
#include "gadget_chips.h"


/*
                                                                 
                                                         
  
                                                                     
                                                                  
                                                                  
                                                                   
                                  
  
                                                                     
                                                                     
                                                                       
                                                                   
                                                                     
 */

struct f_acm {
	struct gserial			port;
	u8				ctrl_id, data_id;
	u8				port_num;
	enum transport_type		transport;

	u8				pending;

	/*                                                                
                                                                     
                                                        
  */
	spinlock_t			lock;

	struct usb_ep			*notify;
	struct usb_request		*notify_req;

	struct usb_cdc_line_coding	port_line_coding;	/*           */

	/*                                                               */
	u16				port_handshake_bits;
#define ACM_CTRL_RTS	(1 << 1)	/*                         */
#define ACM_CTRL_DTR	(1 << 0)	/*                            */

	/*                                                            */
	u16				serial_state;
#define ACM_CTRL_OVERRUN	(1 << 6)
#define ACM_CTRL_PARITY		(1 << 5)
#define ACM_CTRL_FRAMING	(1 << 4)
#define ACM_CTRL_RI		(1 << 3)
#define ACM_CTRL_BRK		(1 << 2)
#define ACM_CTRL_DSR		(1 << 1)
#define ACM_CTRL_DCD		(1 << 0)
};

static unsigned int no_acm_tty_ports;
static unsigned int no_acm_sdio_ports;
static unsigned int no_acm_smd_ports;
static unsigned int nr_acm_ports;

static struct acm_port_info {
	enum transport_type	transport;
	unsigned		port_num;
	unsigned		client_port_num;
} gacm_ports[GSERIAL_NO_PORTS];

static inline struct f_acm *func_to_acm(struct usb_function *f)
{
	return container_of(f, struct f_acm, port.func);
}

static inline struct f_acm *port_to_acm(struct gserial *p)
{
	return container_of(p, struct f_acm, port);
}

static int acm_port_setup(struct usb_configuration *c)
{
	int ret = 0;

	pr_debug("%s: no_acm_tty_ports:%u no_acm_sdio_ports: %u nr_acm_ports:%u\n",
			__func__, no_acm_tty_ports, no_acm_sdio_ports,
				nr_acm_ports);

	if (no_acm_tty_ports)
		ret = gserial_setup(c->cdev->gadget, no_acm_tty_ports);
	if (no_acm_sdio_ports)
		ret = gsdio_setup(c->cdev->gadget, no_acm_sdio_ports);
	if (no_acm_smd_ports)
		ret = gsmd_setup(c->cdev->gadget, no_acm_smd_ports);

	return ret;
}

static int acm_port_connect(struct f_acm *acm)
{
	unsigned port_num;

	port_num = gacm_ports[acm->port_num].client_port_num;


	pr_debug("%s: transport:%s f_acm:%p gserial:%p port_num:%d cl_port_no:%d\n",
			__func__, xport_to_str(acm->transport),
			acm, &acm->port, acm->port_num, port_num);

	switch (acm->transport) {
	case USB_GADGET_XPORT_TTY:
		gserial_connect(&acm->port, port_num);
		break;
	case USB_GADGET_XPORT_SDIO:
		gsdio_connect(&acm->port, port_num);
		break;
	case USB_GADGET_XPORT_SMD:
		gsmd_connect(&acm->port, port_num);
		break;
	default:
		pr_err("%s: Un-supported transport: %s\n", __func__,
				xport_to_str(acm->transport));
		return -ENODEV;
	}

	return 0;
}

static int acm_port_disconnect(struct f_acm *acm)
{
	unsigned port_num;

	port_num = gacm_ports[acm->port_num].client_port_num;

	pr_debug("%s: transport:%s f_acm:%p gserial:%p port_num:%d cl_pno:%d\n",
			__func__, xport_to_str(acm->transport),
			acm, &acm->port, acm->port_num, port_num);

	switch (acm->transport) {
	case USB_GADGET_XPORT_TTY:
		gserial_disconnect(&acm->port);
		break;
	case USB_GADGET_XPORT_SDIO:
		gsdio_disconnect(&acm->port, port_num);
		break;
	case USB_GADGET_XPORT_SMD:
		gsmd_disconnect(&acm->port, port_num);
		break;
	default:
		pr_err("%s: Un-supported transport:%s\n", __func__,
				xport_to_str(acm->transport));
		return -ENODEV;
	}

	return 0;
}
/*                                                                         */

/*                                                                        */

#define GS_LOG2_NOTIFY_INTERVAL		5	/*                   */
#ifdef CONFIG_USB_G_LGE_ANDROID
#define GS_NOTIFY_MAXPACKET		16	/*                    */
#define GS_DESC_NOTIFY_MAXPACKET	64	/*                        */
#else
#define GS_NOTIFY_MAXPACKET		10	/*                        */
#endif

/*                                  */

static struct usb_interface_assoc_descriptor
acm_iad_descriptor = {
	.bLength =		sizeof acm_iad_descriptor,
	.bDescriptorType =	USB_DT_INTERFACE_ASSOCIATION,

	/*                             */
	.bInterfaceCount = 	2,	//               
	.bFunctionClass =	USB_CLASS_COMM,
	.bFunctionSubClass =	USB_CDC_SUBCLASS_ACM,
	.bFunctionProtocol =	USB_CDC_ACM_PROTO_AT_V25TER,
	/*                       */
};


static struct usb_interface_descriptor acm_control_interface_desc = {
	.bLength =		USB_DT_INTERFACE_SIZE,
	.bDescriptorType =	USB_DT_INTERFACE,
	/*                             */
	.bNumEndpoints =	1,
	.bInterfaceClass =	USB_CLASS_COMM,
	.bInterfaceSubClass =	USB_CDC_SUBCLASS_ACM,
	.bInterfaceProtocol =	USB_CDC_ACM_PROTO_AT_V25TER,
	/*                       */
};

static struct usb_interface_descriptor acm_data_interface_desc = {
	.bLength =		USB_DT_INTERFACE_SIZE,
	.bDescriptorType =	USB_DT_INTERFACE,
	/*                             */
	.bNumEndpoints =	2,
	.bInterfaceClass =	USB_CLASS_CDC_DATA,
	.bInterfaceSubClass =	0,
	.bInterfaceProtocol =	0,
	/*                       */
};

static struct usb_cdc_header_desc acm_header_desc = {
	.bLength =		sizeof(acm_header_desc),
	.bDescriptorType =	USB_DT_CS_INTERFACE,
	.bDescriptorSubType =	USB_CDC_HEADER_TYPE,
	.bcdCDC =		cpu_to_le16(0x0110),
};

static struct usb_cdc_call_mgmt_descriptor
acm_call_mgmt_descriptor = {
	.bLength =		sizeof(acm_call_mgmt_descriptor),
	.bDescriptorType =	USB_DT_CS_INTERFACE,
	.bDescriptorSubType =	USB_CDC_CALL_MANAGEMENT_TYPE,
	.bmCapabilities =	0,
	/*                           */
};

static struct usb_cdc_acm_descriptor acm_descriptor = {
	.bLength =		sizeof(acm_descriptor),
	.bDescriptorType =	USB_DT_CS_INTERFACE,
	.bDescriptorSubType =	USB_CDC_ACM_TYPE,
	.bmCapabilities =	USB_CDC_CAP_LINE,
};

static struct usb_cdc_union_desc acm_union_desc = {
	.bLength =		sizeof(acm_union_desc),
	.bDescriptorType =	USB_DT_CS_INTERFACE,
	.bDescriptorSubType =	USB_CDC_UNION_TYPE,
	/*                              */
	/*                             */
};

/*                     */

static struct usb_endpoint_descriptor acm_fs_notify_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,
	.bEndpointAddress =	USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize =	cpu_to_le16(GS_NOTIFY_MAXPACKET),
	.bInterval =		1 << GS_LOG2_NOTIFY_INTERVAL,
};

static struct usb_endpoint_descriptor acm_fs_in_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,
	.bEndpointAddress =	USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
};

static struct usb_endpoint_descriptor acm_fs_out_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,
	.bEndpointAddress =	USB_DIR_OUT,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
};

static struct usb_descriptor_header *acm_fs_function[] = {
	(struct usb_descriptor_header *) &acm_iad_descriptor,
	(struct usb_descriptor_header *) &acm_control_interface_desc,
	(struct usb_descriptor_header *) &acm_header_desc,
	(struct usb_descriptor_header *) &acm_call_mgmt_descriptor,
	(struct usb_descriptor_header *) &acm_descriptor,
	(struct usb_descriptor_header *) &acm_union_desc,
	(struct usb_descriptor_header *) &acm_fs_notify_desc,
	(struct usb_descriptor_header *) &acm_data_interface_desc,
	(struct usb_descriptor_header *) &acm_fs_in_desc,
	(struct usb_descriptor_header *) &acm_fs_out_desc,
	NULL,
};

/*                     */

static struct usb_endpoint_descriptor acm_hs_notify_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,
	.bEndpointAddress =	USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_INT,
#ifdef CONFIG_USB_G_LGE_ANDROID
	.wMaxPacketSize =	cpu_to_le16(GS_DESC_NOTIFY_MAXPACKET),
#else
	.wMaxPacketSize =	cpu_to_le16(GS_NOTIFY_MAXPACKET),
#endif
	.bInterval =		GS_LOG2_NOTIFY_INTERVAL+4,
};

static struct usb_endpoint_descriptor acm_hs_in_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(512),
};

static struct usb_endpoint_descriptor acm_hs_out_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(512),
};

static struct usb_descriptor_header *acm_hs_function[] = {
	(struct usb_descriptor_header *) &acm_iad_descriptor,
	(struct usb_descriptor_header *) &acm_control_interface_desc,
	(struct usb_descriptor_header *) &acm_header_desc,
	(struct usb_descriptor_header *) &acm_call_mgmt_descriptor,
	(struct usb_descriptor_header *) &acm_descriptor,
	(struct usb_descriptor_header *) &acm_union_desc,
	(struct usb_descriptor_header *) &acm_hs_notify_desc,
	(struct usb_descriptor_header *) &acm_data_interface_desc,
	(struct usb_descriptor_header *) &acm_hs_in_desc,
	(struct usb_descriptor_header *) &acm_hs_out_desc,
	NULL,
};

static struct usb_endpoint_descriptor acm_ss_in_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(1024),
};

static struct usb_endpoint_descriptor acm_ss_out_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(1024),
};

static struct usb_ss_ep_comp_descriptor acm_ss_bulk_comp_desc = {
	.bLength =              sizeof acm_ss_bulk_comp_desc,
	.bDescriptorType =      USB_DT_SS_ENDPOINT_COMP,
};

static struct usb_descriptor_header *acm_ss_function[] = {
	(struct usb_descriptor_header *) &acm_iad_descriptor,
	(struct usb_descriptor_header *) &acm_control_interface_desc,
	(struct usb_descriptor_header *) &acm_header_desc,
	(struct usb_descriptor_header *) &acm_call_mgmt_descriptor,
	(struct usb_descriptor_header *) &acm_descriptor,
	(struct usb_descriptor_header *) &acm_union_desc,
	(struct usb_descriptor_header *) &acm_hs_notify_desc,
	(struct usb_descriptor_header *) &acm_ss_bulk_comp_desc,
	(struct usb_descriptor_header *) &acm_data_interface_desc,
	(struct usb_descriptor_header *) &acm_ss_in_desc,
	(struct usb_descriptor_header *) &acm_ss_bulk_comp_desc,
	(struct usb_descriptor_header *) &acm_ss_out_desc,
	(struct usb_descriptor_header *) &acm_ss_bulk_comp_desc,
	NULL,
};

/*                     */

#define ACM_CTRL_IDX	0
#define ACM_DATA_IDX	1
#define ACM_IAD_IDX	2

/*                          */
static struct usb_string acm_string_defs[] = {
	[ACM_CTRL_IDX].s = "CDC Abstract Control Model (ACM)",
	[ACM_DATA_IDX].s = "CDC ACM Data",
	[ACM_IAD_IDX ].s = "CDC Serial",
	{  /*                 */ },
};

static struct usb_gadget_strings acm_string_table = {
	.language =		0x0409,	/*       */
	.strings =		acm_string_defs,
};

static struct usb_gadget_strings *acm_strings[] = {
	&acm_string_table,
	NULL,
};

/*                                                                         */

/*                                                                
                                                               
                                                              
                                                              
 */

static void acm_complete_set_line_coding(struct usb_ep *ep,
		struct usb_request *req)
{
	struct f_acm	*acm = ep->driver_data;
	struct usb_composite_dev *cdev = acm->port.func.config->cdev;

	if (req->status != 0) {
		DBG(cdev, "acm ttyGS%d completion, err %d\n",
				acm->port_num, req->status);
		return;
	}

	/*                   */
	if (req->actual != sizeof(acm->port_line_coding)) {
		DBG(cdev, "acm ttyGS%d short resp, len %d\n",
				acm->port_num, req->actual);
		usb_ep_set_halt(ep);
	} else {
		struct usb_cdc_line_coding	*value = req->buf;

		/*                                                
                                                   
                                                 
                                                     
                                                    
                                                 
   */
		acm->port_line_coding = *value;
	}
}

static int acm_setup(struct usb_function *f, const struct usb_ctrlrequest *ctrl)
{
	struct f_acm		*acm = func_to_acm(f);
	struct usb_composite_dev *cdev = f->config->cdev;
	struct usb_request	*req = cdev->req;
	int			value = -EOPNOTSUPP;
	u16			w_index = le16_to_cpu(ctrl->wIndex);
	u16			w_value = le16_to_cpu(ctrl->wValue);
	u16			w_length = le16_to_cpu(ctrl->wLength);

	/*                                                          
                                                            
   
                                                                     
                                                                     
                                                                     
                                     
  */
	switch ((ctrl->bRequestType << 8) | ctrl->bRequest) {

	/*                                                            */
	case ((USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
			| USB_CDC_REQ_SET_LINE_CODING:
		if (w_length != sizeof(struct usb_cdc_line_coding))
			goto invalid;

		value = w_length;
		cdev->gadget->ep0->driver_data = acm;
		req->complete = acm_complete_set_line_coding;
		break;

	/*                                                             */
	case ((USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
			| USB_CDC_REQ_GET_LINE_CODING:

		value = min_t(unsigned, w_length,
				sizeof(struct usb_cdc_line_coding));
		memcpy(req->buf, &acm->port_line_coding, value);
		break;

	/*                                                    */
	case ((USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
			| USB_CDC_REQ_SET_CONTROL_LINE_STATE:
		value = 0;

		/*                                                 
                                                       
                                                      
   */
		acm->port_handshake_bits = w_value;
		if (acm->port.notify_modem) {
			unsigned port_num =
				gacm_ports[acm->port_num].client_port_num;

			acm->port.notify_modem(&acm->port, port_num, w_value);
		}
		break;

	default:
invalid:
		VDBG(cdev, "invalid control req%02x.%02x v%04x i%04x l%d\n",
			ctrl->bRequestType, ctrl->bRequest,
			w_value, w_index, w_length);
	}

	/*                                             */
	if (value >= 0) {
		DBG(cdev, "acm ttyGS%d req%02x.%02x v%04x i%04x l%d\n",
			acm->port_num, ctrl->bRequestType, ctrl->bRequest,
			w_value, w_index, w_length);
		req->zero = 0;
		req->length = value;
		value = usb_ep_queue(cdev->gadget->ep0, req, GFP_ATOMIC);
		if (value < 0)
			ERROR(cdev, "acm response on ttyGS%d, err %d\n",
					acm->port_num, value);
	}

	/*                                                     */
	return value;
}

static int acm_set_alt(struct usb_function *f, unsigned intf, unsigned alt)
{
	struct f_acm		*acm = func_to_acm(f);
	struct usb_composite_dev *cdev = f->config->cdev;

	/*                                                       */

	if (intf == acm->ctrl_id) {
		if (acm->notify->driver_data) {
			VDBG(cdev, "reset acm control interface %d\n", intf);
			usb_ep_disable(acm->notify);
		} else {
			VDBG(cdev, "init acm ctrl interface %d\n", intf);
		}
		if (config_ep_by_speed(cdev->gadget, f, acm->notify))
			return -EINVAL;

		usb_ep_enable(acm->notify);
		acm->notify->driver_data = acm;

	} else if (intf == acm->data_id) {
		if (acm->port.in->driver_data) {
			DBG(cdev, "reset acm ttyGS%d\n", acm->port_num);
			acm_port_disconnect(acm);
		}
		if (!acm->port.in->desc || !acm->port.out->desc) {
			DBG(cdev, "activate acm ttyGS%d\n", acm->port_num);
			if (config_ep_by_speed(cdev->gadget, f,
					       acm->port.in) ||
			    config_ep_by_speed(cdev->gadget, f,
					       acm->port.out)) {
				acm->port.in->desc = NULL;
				acm->port.out->desc = NULL;
				return -EINVAL;
			}
		}
		if (config_ep_by_speed(cdev->gadget, f,
				acm->port.in) ||
			config_ep_by_speed(cdev->gadget, f,
				acm->port.out)) {
			acm->port.in->desc = NULL;
			acm->port.out->desc = NULL;
			return -EINVAL;
		}

		acm_port_connect(acm);

	} else
		return -EINVAL;

	return 0;
}

static void acm_disable(struct usb_function *f)
{
	struct f_acm	*acm = func_to_acm(f);
	struct usb_composite_dev *cdev = f->config->cdev;

	DBG(cdev, "acm ttyGS%d deactivated\n", acm->port_num);
	acm_port_disconnect(acm);
	usb_ep_disable(acm->notify);
	acm->notify->driver_data = NULL;
}

/*                                                                         */

/* 
                                                  
                                  
                           
                                            
                         
                        
                                                                 
  
                                               
  
                                                                 
                                                             
 */
static int acm_cdc_notify(struct f_acm *acm, u8 type, u16 value,
		void *data, unsigned length)
{
	struct usb_ep			*ep = acm->notify;
	struct usb_request		*req;
	struct usb_cdc_notification	*notify;
#ifndef CONFIG_USB_G_LGE_ANDROID
	const unsigned			len = sizeof(*notify) + length;
#endif
	void				*buf;
	int				status;

#ifdef CONFIG_USB_G_LGE_ANDROID
	unsigned char noti_buf[GS_NOTIFY_MAXPACKET];

	memset(noti_buf, 0, GS_NOTIFY_MAXPACKET);
#endif
	req = acm->notify_req;
	acm->notify_req = NULL;
	acm->pending = false;

#ifdef CONFIG_USB_G_LGE_ANDROID
	req->length = GS_NOTIFY_MAXPACKET;
#else
	req->length = len;
#endif
	notify = req->buf;
	buf = notify + 1;

	notify->bmRequestType = USB_DIR_IN | USB_TYPE_CLASS
			| USB_RECIP_INTERFACE;
	notify->bNotificationType = type;
	notify->wValue = cpu_to_le16(value);
	notify->wIndex = cpu_to_le16(acm->ctrl_id);
	notify->wLength = cpu_to_le16(length);
#ifdef CONFIG_USB_G_LGE_ANDROID
	memcpy(noti_buf, data, length);
	memcpy(buf, noti_buf, GS_NOTIFY_MAXPACKET);
#else
	memcpy(buf, data, length);
#endif

	/*                                                             */
	spin_unlock(&acm->lock);
	status = usb_ep_queue(ep, req, GFP_ATOMIC);
	spin_lock(&acm->lock);

	if (status < 0) {
		ERROR(acm->port.func.config->cdev,
				"acm ttyGS%d can't notify serial state, %d\n",
				acm->port_num, status);
		acm->notify_req = req;
	}

	return status;
}

static int acm_notify_serial_state(struct f_acm *acm)
{
	struct usb_composite_dev *cdev = acm->port.func.config->cdev;
	int			status;

	spin_lock(&acm->lock);
	if (acm->notify_req) {
		DBG(cdev, "acm ttyGS%d serial state %04x\n",
				acm->port_num, acm->serial_state);
		status = acm_cdc_notify(acm, USB_CDC_NOTIFY_SERIAL_STATE,
				0, &acm->serial_state, sizeof(acm->serial_state));
	} else {
		acm->pending = true;
		status = 0;
	}
	spin_unlock(&acm->lock);
	return status;
}

static void acm_cdc_notify_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct f_acm		*acm = req->context;
	u8			doit = false;

	/*                                                    
                                           
  */
	spin_lock(&acm->lock);
	if (req->status != -ESHUTDOWN)
		doit = acm->pending;
	acm->notify_req = req;
	spin_unlock(&acm->lock);

	if (doit)
		acm_notify_serial_state(acm);
}

/*                                 */

static void acm_connect(struct gserial *port)
{
	struct f_acm		*acm = port_to_acm(port);

	acm->serial_state |= ACM_CTRL_DSR | ACM_CTRL_DCD;
	acm_notify_serial_state(acm);
}

unsigned int acm_get_dtr(struct gserial *port)
{
	struct f_acm		*acm = port_to_acm(port);

	if (acm->port_handshake_bits & ACM_CTRL_DTR)
		return 1;
	else
		return 0;
}

unsigned int acm_get_rts(struct gserial *port)
{
	struct f_acm		*acm = port_to_acm(port);

	if (acm->port_handshake_bits & ACM_CTRL_RTS)
		return 1;
	else
		return 0;
}

unsigned int acm_send_carrier_detect(struct gserial *port, unsigned int yes)
{
	struct f_acm		*acm = port_to_acm(port);
	u16			state;

	pr_info("%s : ACM_CTRL_DCD is %s\n", __func__, (yes ? "yes" : "no"));
	state = acm->serial_state;
	state &= ~ACM_CTRL_DCD;
	if (yes)
		state |= ACM_CTRL_DCD;

	acm->serial_state = state;
	return acm_notify_serial_state(acm);

}

unsigned int acm_send_ring_indicator(struct gserial *port, unsigned int yes)
{
	struct f_acm		*acm = port_to_acm(port);
	u16			state;

	state = acm->serial_state;
	state &= ~ACM_CTRL_RI;
	if (yes)
		state |= ACM_CTRL_RI;

	acm->serial_state = state;
	return acm_notify_serial_state(acm);

}

static void acm_disconnect(struct gserial *port)
{
	struct f_acm		*acm = port_to_acm(port);

	acm->serial_state &= ~(ACM_CTRL_DSR | ACM_CTRL_DCD);
	acm_notify_serial_state(acm);
}

static int acm_send_break(struct gserial *port, int duration)
{
	struct f_acm		*acm = port_to_acm(port);
	u16			state;

	state = acm->serial_state;
	state &= ~ACM_CTRL_BRK;
	if (duration)
		state |= ACM_CTRL_BRK;

	acm->serial_state = state;
	return acm_notify_serial_state(acm);
}

static int acm_send_modem_ctrl_bits(struct gserial *port, int ctrl_bits)
{
	struct f_acm *acm = port_to_acm(port);

	acm->serial_state = ctrl_bits;

	return acm_notify_serial_state(acm);
}

/*                                                                         */

/*                                   */
static int
acm_bind(struct usb_configuration *c, struct usb_function *f)
{
	struct usb_composite_dev *cdev = c->cdev;
	struct f_acm		*acm = func_to_acm(f);
	int			status;
	struct usb_ep		*ep;

	/*                                                                 */
	status = usb_interface_id(c, f);
	if (status < 0)
		goto fail;
	acm->ctrl_id = status;
	acm_iad_descriptor.bFirstInterface = status;

	acm_control_interface_desc.bInterfaceNumber = status;
	acm_union_desc .bMasterInterface0 = status;

	status = usb_interface_id(c, f);
	if (status < 0)
		goto fail;
	acm->data_id = status;

	acm_data_interface_desc.bInterfaceNumber = status;
	acm_union_desc.bSlaveInterface0 = status;
	acm_call_mgmt_descriptor.bDataInterface = status;

	status = -ENODEV;

	/*                                      */
	ep = usb_ep_autoconfig(cdev->gadget, &acm_fs_in_desc);
	if (!ep)
		goto fail;
	acm->port.in = ep;
	ep->driver_data = cdev;	/*       */

	ep = usb_ep_autoconfig(cdev->gadget, &acm_fs_out_desc);
	if (!ep)
		goto fail;
	acm->port.out = ep;
	ep->driver_data = cdev;	/*       */

	ep = usb_ep_autoconfig(cdev->gadget, &acm_fs_notify_desc);
	if (!ep)
		goto fail;
	acm->notify = ep;
	ep->driver_data = cdev;	/*       */

	/*                       */
	acm->notify_req = gs_alloc_req(ep,
			sizeof(struct usb_cdc_notification) + 2,
			GFP_KERNEL);
	if (!acm->notify_req)
		goto fail;

	acm->notify_req->complete = acm_cdc_notify_complete;
	acm->notify_req->context = acm;

	/*                  */
	f->descriptors = usb_copy_descriptors(acm_fs_function);
	if (!f->descriptors)
		goto fail;

	/*                                                            
                                                              
               
  */
	if (gadget_is_dualspeed(c->cdev->gadget)) {
		acm_hs_in_desc.bEndpointAddress =
				acm_fs_in_desc.bEndpointAddress;
		acm_hs_out_desc.bEndpointAddress =
				acm_fs_out_desc.bEndpointAddress;
		acm_hs_notify_desc.bEndpointAddress =
				acm_fs_notify_desc.bEndpointAddress;

		/*                  */
		f->hs_descriptors = usb_copy_descriptors(acm_hs_function);
		if (!f->hs_descriptors)
			goto fail;
	}
	if (gadget_is_superspeed(c->cdev->gadget)) {
		acm_ss_in_desc.bEndpointAddress =
			acm_fs_in_desc.bEndpointAddress;
		acm_ss_out_desc.bEndpointAddress =
			acm_fs_out_desc.bEndpointAddress;

		/*                                             */
		f->ss_descriptors = usb_copy_descriptors(acm_ss_function);
		if (!f->ss_descriptors)
			goto fail;
	}

	DBG(cdev, "acm ttyGS%d: %s speed IN/%s OUT/%s NOTIFY/%s\n",
			acm->port_num,
			gadget_is_superspeed(c->cdev->gadget) ? "super" :
			gadget_is_dualspeed(c->cdev->gadget) ? "dual" : "full",
			acm->port.in->name, acm->port.out->name,
			acm->notify->name);
	return 0;

fail:
	if (f->hs_descriptors)
		usb_free_descriptors(f->hs_descriptors);
	if (f->descriptors)
		usb_free_descriptors(f->descriptors);

	if (acm->notify_req)
		gs_free_req(acm->notify, acm->notify_req);

	/*                                                  */
	if (acm->notify)
		acm->notify->driver_data = NULL;
	if (acm->port.out)
		acm->port.out->driver_data = NULL;
	if (acm->port.in)
		acm->port.in->driver_data = NULL;

	ERROR(cdev, "%s/%p: can't bind, err %d\n", f->name, f, status);

	return status;
}

static void
acm_unbind(struct usb_configuration *c, struct usb_function *f)
{
	struct f_acm		*acm = func_to_acm(f);

	if (gadget_is_dualspeed(c->cdev->gadget))
		usb_free_descriptors(f->hs_descriptors);
	if (gadget_is_superspeed(c->cdev->gadget))
		usb_free_descriptors(f->ss_descriptors);
	usb_free_descriptors(f->descriptors);
	gs_free_req(acm->notify, acm->notify_req);
	kfree(acm->port.func.name);
	kfree(acm);
}

/*                                            */
static inline bool can_support_cdc(struct usb_configuration *c)
{
	/*                                        */
	return true;
}

#ifdef CONFIG_USB_G_LGE_MULTICONFIG_ATF_WA
/*
                     
                                     
                                                        
                                                              
 */
static int lge_acm_desc_change(struct usb_function *f, bool is_mac)
{
	if (is_mac == true) {
		((struct usb_interface_descriptor *)f->ss_descriptors[1])->bInterfaceClass = USB_CLASS_VENDOR_SPEC;
		((struct usb_interface_descriptor *)f->hs_descriptors[1])->bInterfaceClass = USB_CLASS_VENDOR_SPEC;
		((struct usb_interface_descriptor *)f->descriptors[1])->bInterfaceClass = USB_CLASS_VENDOR_SPEC;
		pr_info("MAC ACM bInterfaceClass change to %u \n", ((struct usb_interface_descriptor *)f->ss_descriptors[1])->bInterfaceClass);
	} else {
		((struct usb_interface_descriptor *)f->ss_descriptors[1])->bInterfaceClass = USB_CLASS_COMM;
		((struct usb_interface_descriptor *)f->hs_descriptors[1])->bInterfaceClass = USB_CLASS_COMM;
		((struct usb_interface_descriptor *)f->descriptors[1])->bInterfaceClass = USB_CLASS_COMM;
		pr_info("WIN/LINUX ACM bInterfaceClass change to %u \n", ((struct usb_interface_descriptor *)f->ss_descriptors[1])->bInterfaceClass);
	}
	return 0;
}
#endif

/* 
                                                              
                                                        
                                                      
                                               
  
                                                
  
                                                                
                                                            
                                                       
 */
int acm_bind_config(struct usb_configuration *c, u8 port_num)
{
	struct f_acm	*acm;
	int		status;

	if (!can_support_cdc(c))
		return -EINVAL;

	/*                                                     
                             
  */

	/*                                                                */
	if (acm_string_defs[ACM_CTRL_IDX].id == 0) {
		status = usb_string_id(c->cdev);
		if (status < 0)
			return status;
		acm_string_defs[ACM_CTRL_IDX].id = status;

		acm_control_interface_desc.iInterface = status;

		status = usb_string_id(c->cdev);
		if (status < 0)
			return status;
		acm_string_defs[ACM_DATA_IDX].id = status;

		acm_data_interface_desc.iInterface = status;

		status = usb_string_id(c->cdev);
		if (status < 0)
			return status;
		acm_string_defs[ACM_IAD_IDX].id = status;

		acm_iad_descriptor.iFunction = status;
	}

	/*                                          */
	acm = kzalloc(sizeof *acm, GFP_KERNEL);
	if (!acm)
		return -ENOMEM;

	spin_lock_init(&acm->lock);

	acm->port_num = port_num;
	acm->transport = gacm_ports[port_num].transport;

	acm->port.connect = acm_connect;
	acm->port.get_dtr = acm_get_dtr;
	acm->port.get_rts = acm_get_rts;
	acm->port.send_carrier_detect = acm_send_carrier_detect;
	acm->port.send_ring_indicator = acm_send_ring_indicator;
	acm->port.disconnect = acm_disconnect;
	acm->port.send_break = acm_send_break;
	acm->port.send_modem_ctrl_bits = acm_send_modem_ctrl_bits;

	acm->port.func.name = kasprintf(GFP_KERNEL, "acm%u", port_num + 1);
	if (!acm->port.func.name) {
		kfree(acm);
		return -ENOMEM;
	}
	acm->port.func.strings = acm_strings;
	/*                                     */
	acm->port.func.bind = acm_bind;
	acm->port.func.unbind = acm_unbind;
	acm->port.func.set_alt = acm_set_alt;
	acm->port.func.setup = acm_setup;
	acm->port.func.disable = acm_disable;
#ifdef CONFIG_USB_G_LGE_MULTICONFIG_ATF_WA
	acm->port.func.desc_change = lge_acm_desc_change;
#endif
	status = usb_add_function(c, &acm->port.func);
	if (status)
		kfree(acm);
	return status;
}

/* 
                                                   
 */
static int acm_init_port(int port_num, const char *name)
{
	enum transport_type transport;

	if (port_num >= GSERIAL_NO_PORTS)
		return -ENODEV;

	transport = str_to_xport(name);
	pr_debug("%s, port:%d, transport:%s\n", __func__,
			port_num, xport_to_str(transport));

	gacm_ports[port_num].transport = transport;
	gacm_ports[port_num].port_num = port_num;

	switch (transport) {
	case USB_GADGET_XPORT_TTY:
		gacm_ports[port_num].client_port_num = no_acm_tty_ports;
		no_acm_tty_ports++;
		break;
	case USB_GADGET_XPORT_SDIO:
		gacm_ports[port_num].client_port_num = no_acm_sdio_ports;
		no_acm_sdio_ports++;
		break;
	case USB_GADGET_XPORT_SMD:
		gacm_ports[port_num].client_port_num = no_acm_smd_ports;
		no_acm_smd_ports++;
		break;
	default:
		pr_err("%s: Un-supported transport transport: %u\n",
				__func__, gacm_ports[port_num].transport);
		return -ENODEV;
	}

	nr_acm_ports++;

	return 0;
}
