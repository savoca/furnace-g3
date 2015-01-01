/*
 * Simple "CDC Subset" USB Networking Links
 * Copyright (C) 2000-2005 by David Brownell
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/module.h>
#include <linux/kmod.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/workqueue.h>
#include <linux/mii.h>
#include <linux/usb.h>
#include <linux/usb/usbnet.h>


/*
                                                                        
                                                                       
                                                                         
                                                   
  
                                                                  
                                                                   
                                                                  
                                                             
  
                                                               
                                                                      
                                                                       
                                                                   
                                                                     
  
                                                                       
                                                                      
                                                                        
                                                                   
  
                                                                        
                                                                       
                                                                       
  
                                                                    
                                                                       
                                                                      
                                                                      
                                                                     
                                                          
 */

#if defined(CONFIG_USB_EPSON2888) || defined(CONFIG_USB_ARMLINUX)
/*                                                   */
static int always_connected (struct usbnet *dev)
{
	return 0;
}
#endif

#ifdef	CONFIG_USB_ALI_M5632
#define	HAVE_HARDWARE

/*                                                                         
  
                                       
  
                                                                    
                                                                      
                                                                     
                                                                       
                                                                        
                          
  
                                                                           */

static const struct driver_info	ali_m5632_info = {
	.description =	"ALi M5632",
	.flags       = FLAG_POINTTOPOINT,
};

#endif


#ifdef	CONFIG_USB_AN2720
#define	HAVE_HARDWARE

/*                                                                         
  
                                                     
  
                                                                
                                                                 
                                                              
                                                               
  
                                                                           */

static const struct driver_info	an2720_info = {
	.description =	"AnchorChips/Cypress 2720",
	.flags       = FLAG_POINTTOPOINT,
	//                    
	//                            

	.in = 2, .out = 2,		//                              
};

#endif	/*                   */


#ifdef	CONFIG_USB_BELKIN
#define	HAVE_HARDWARE

/*                                                                         
  
                                                                         
  
                                                                    
  
                                                                           */

static const struct driver_info	belkin_info = {
	.description =	"Belkin, eTEK, or compatible",
	.flags       = FLAG_POINTTOPOINT,
};

#endif	/*                   */



#ifdef	CONFIG_USB_EPSON2888
#define	HAVE_HARDWARE

/*                                                                         
  
                    
  
                                                                         
                                                                           
                                                                          
                                                             
  
                                                                 
  
                                                                           */

static const struct driver_info	epson2888_info = {
	.description =	"Epson USB Device",
	.check_connect = always_connected,
	.flags = FLAG_POINTTOPOINT,

	.in = 4, .out = 3,
};

#endif	/*                      */


/*                                                                         
  
                                                 
  
                                                                           */
#ifdef CONFIG_USB_KC2190
#define HAVE_HARDWARE
static const struct driver_info kc2190_info = {
	.description =  "KC Technology KC-190",
	.flags = FLAG_POINTTOPOINT,
};
#endif /*                   */


#ifdef	CONFIG_USB_ARMLINUX
#define	HAVE_HARDWARE

/*                                                                         
  
                                                                 
                                                                   
                                                                     
                                          
  
                                                                     
                                                  
  
                                                                        
                                                                     
                                                                    
                                          
  
                                                                           */

static const struct driver_info	linuxdev_info = {
	.description =	"Linux Device",
	.check_connect = always_connected,
	.flags = FLAG_POINTTOPOINT,
};

static const struct driver_info	yopy_info = {
	.description =	"Yopy",
	.check_connect = always_connected,
	.flags = FLAG_POINTTOPOINT,
};

static const struct driver_info	blob_info = {
	.description =	"Boot Loader OBject",
	.check_connect = always_connected,
	.flags = FLAG_POINTTOPOINT,
};

#endif	/*                     */


/*                                                                         */

#ifndef	HAVE_HARDWARE
#warning You need to configure some hardware for this driver
#endif

/*
                                                         
                            
 */

static const struct usb_device_id	products [] = {

#ifdef	CONFIG_USB_ALI_M5632
{
	USB_DEVICE (0x0402, 0x5632),	//             
	.driver_info =	(unsigned long) &ali_m5632_info,
},
{
	USB_DEVICE (0x182d,0x207c),	//               
	.driver_info =	(unsigned long) &ali_m5632_info,
},
#endif

#ifdef	CONFIG_USB_AN2720
{
	USB_DEVICE (0x0547, 0x2720),	//                     
	.driver_info =	(unsigned long) &an2720_info,
}, {
	USB_DEVICE (0x0547, 0x2727),	//              
	.driver_info =	(unsigned long) &an2720_info,
},
#endif

#ifdef	CONFIG_USB_BELKIN
{
	USB_DEVICE (0x050d, 0x0004),	//       
	.driver_info =	(unsigned long) &belkin_info,
}, {
	USB_DEVICE (0x056c, 0x8100),	//     
	.driver_info =	(unsigned long) &belkin_info,
}, {
	USB_DEVICE (0x0525, 0x9901),	//                      
	.driver_info =	(unsigned long) &belkin_info,
},
#endif

#ifdef	CONFIG_USB_EPSON2888
{
	USB_DEVICE (0x0525, 0x2888),	//                 
	.driver_info	= (unsigned long) &epson2888_info,
},
#endif

#ifdef CONFIG_USB_KC2190
{
	USB_DEVICE (0x050f, 0x0190),	//       
	.driver_info =	(unsigned long) &kc2190_info,
},
#endif

#ifdef	CONFIG_USB_ARMLINUX
/*
                                                           
                                                           
                                                          
  
                                                               
                                                                
  
                                                             
                                                               
                                                         
                                     
                                                             
                                                             
 */
{
	//                                        
	//                                
	USB_DEVICE (0x049F, 0x505A),	//                       
	.driver_info =	(unsigned long) &linuxdev_info,
}, {
	USB_DEVICE (0x0E7E, 0x1001),	//              
	.driver_info =	(unsigned long) &yopy_info,
}, {
	USB_DEVICE (0x8086, 0x07d3),	//                  
	.driver_info =	(unsigned long) &blob_info,
}, {
	USB_DEVICE (0x1286, 0x8001),    //                  
	.driver_info =  (unsigned long) &blob_info,
}, {
	//                                                          
	//                                                       
	//                                      
	USB_DEVICE (0x0525, 0xa4a2),
	.driver_info =	(unsigned long) &linuxdev_info,
},
#endif

	{ },		//    
};
MODULE_DEVICE_TABLE(usb, products);

/*                                                                         */

static struct usb_driver cdc_subset_driver = {
	.name =		"cdc_subset",
	.probe =	usbnet_probe,
	.suspend =	usbnet_suspend,
	.resume =	usbnet_resume,
	.disconnect =	usbnet_disconnect,
	.id_table =	products,
};

module_usb_driver(cdc_subset_driver);

MODULE_AUTHOR("David Brownell");
MODULE_DESCRIPTION("Simple 'CDC Subset' USB networking links");
MODULE_LICENSE("GPL");
