/*
               
 
*/

#ifndef __SNFC_COMMON_H__
#define __SNFC_COMMON_H__

/*
                      
*/
#include <linux/kernel.h>/*          */
#include <linux/fs.h>/*               */
#include <asm/uaccess.h>/*              */
#include <linux/delay.h>/*      */
#include <linux/types.h>/*        */
#include <linux/miscdevice.h>/*                              */
#include <linux/module.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>

/*
          
 */

/*               */
//                         
//                            

#define SNFC_DEBUG_MSG printk
//                                

#ifdef FEATURE_DEBUG_LOW
#define SNFC_DEBUG_MSG_LOW printk
#else
#define SNFC_DEBUG_MSG_LOW(...) ((void)0)
#endif

#ifdef FEATURE_DEBUG_MIDDLE
#define SNFC_DEBUG_MSG_MIDDLE printk
#else
#define SNFC_DEBUG_MSG_MIDDLE(...) ((void)0)
#endif

extern void snfc_avali_poll_felica_status(void);
extern int snfc_hvdd_wait_rfs_low(void);
#endif

