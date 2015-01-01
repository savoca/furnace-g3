#ifndef __UAPI_USB_CCID_BRIDGE_H
#define __UAPI_USB_CCID_BRIDGE_H

#include <linux/ioctl.h>

/* 
                                                                    
                                                             
                                     
                                                                        
                                                            
 */
struct usb_ccid_data {
	uint8_t length;
	void *data;
};

/* 
                                                                 
                                            
                                                           
 */
struct usb_ccid_abort {
	uint8_t seq;
	uint8_t slot;
};

#define USB_CCID_NOTIFY_SLOT_CHANGE_EVENT 1
#define USB_CCID_HARDWARE_ERROR_EVENT 2
#define USB_CCID_RESUME_EVENT 3
/* 
                                                                         
                      
                                                                             
                                                            
                                                                   
                                                         
 */
struct usb_ccid_event {
	uint8_t event;
	union {
		struct {
			uint8_t slot_icc_state;
		} notify;

		struct {
			uint8_t slot;
			uint8_t seq;
			uint8_t error_code;
		} error;
	} u;
};

#define USB_CCID_GET_CLASS_DESC _IOWR('C', 0, struct usb_ccid_data)

#define USB_CCID_GET_CLOCK_FREQUENCIES _IOWR('C', 1, struct usb_ccid_data)

#define USB_CCID_GET_DATA_RATES _IOWR('C', 2, struct usb_ccid_data)

#define USB_CCID_ABORT _IOW('C', 3, struct usb_ccid_abort)

#define USB_CCID_GET_EVENT _IOR('C', 4, struct usb_ccid_event)

#endif /*                          */
