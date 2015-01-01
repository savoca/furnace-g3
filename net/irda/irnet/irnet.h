/*
 *	IrNET protocol module : Synchronous PPP over an IrDA socket.
 *
 *		Jean II - HPL `00 - <jt@hpl.hp.com>
 *
 * This file contains definitions and declarations global to the IrNET module,
 * all grouped in one place...
 * This file is a *private* header, so other modules don't want to know
 * what's in there...
 *
 * Note : as most part of the Linux kernel, this module is available
 * under the GNU General Public License (GPL).
 */

#ifndef IRNET_H
#define IRNET_H

/*                                                                  */
/*
                
                
                                                                   
                                                                      
                                                                    
                                                                      
                                                                     
                                                   
  
                                                                  
                                                                 
                                                                     
           
  
                                                                 
                                                                      
                                                            
                                                                    
                                                                    
                              
  
                           
                           
                                                                      
                                                               
                                                                      
                                      
  
                                                            
                                     
                                                                   
                                                                 
                                                          
                                        
                                                       
                                                                    
                                                            
                      
                                       
                                               
                            
                                                                  
  
                                                                     
                                                
  
                   
                   
                            
                                                               
                                                         
                                                  
                                                      
                                                                        
                             
  
                                                                   
                                                            
                                                          
  
                                                                     
                                                                    
                  
  
          
  
                                          
                                       
                                    
                                                                    
                                                                    
                                                          
                                                                    
                             
  
                                                                  
                                                                   
                                        
  
                                                                  
                                                                         
                                                                         
                                      
  
                          
                                                                        
                                                                            
                                                                           
  
                                                                   
                                                                       
               
                                                                         
                                  
                                                                          
                                                                         
                                                                      
  
                                                                         
                                                                    
             
                                                                          
                                                                          
                                                  
  
  
            
          
  
                         
                                                                 
                                                     
                                                        
  
                        
                                                                     
                                                        
                                                        
  
                         
                                          
                             
                          
                                                           
                                                      
                                                                 
                                                        
                                                                  
                                   
                                
  
                          
                                                
  
                         
                                                               
                                                                  
                                                           
                                                  
                                                          
  
                          
                                                
                                                                 
                                                                   
                                                       
      
                                                   
                                            
                                                           
                                    
  
                          
                                                                      
                                                      
                                                                       
                                                                  
      
                                                             
                                                                 
                                                                     
                         
                                                                    
                                                                
                                                               
                                                             
                                                                    
                                                         
  
                          
                                                             
                                                                
                                                                 
                                                                  
                                                              
  
                         
                                                               
                                     
                                        
                                                                  
                                        
  
                          
                                                                     
                                                                        
                                     
                                                                     
                                           
  
                        
                                                               
                                                                
                                         
  
                          
                                                                 
                                                  
                                          
                                                                    
  
                          
                                                      
                                                                     
                                                             
  
                          
                            
  
                          
                                                    
                                                             
  
                         
                                                                   
                                                                
                                                                      
 */

/*                                                                  */

#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/tty.h>
#include <linux/proc_fs.h>
#include <linux/netdevice.h>
#include <linux/miscdevice.h>
#include <linux/poll.h>
#include <linux/capability.h>
#include <linux/ctype.h>	/*           */
#include <linux/string.h>	/*               */
#include <asm/uaccess.h>
#include <linux/init.h>

#include <linux/ppp_defs.h>
#include <linux/ppp-ioctl.h>
#include <linux/ppp_channel.h>

#include <net/irda/irda.h>
#include <net/irda/iriap.h>
#include <net/irda/irias_object.h>
#include <net/irda/irlmp.h>
#include <net/irda/irttp.h>
#include <net/irda/discovery.h>

/*                                                                 */
/*
                                                                 
                  
                                                                 
              
 */
/*                              */
#define DISCOVERY_NOMASK	/*                                */
#define ADVERTISE_HINT		/*                          */
#define ALLOW_SIMULT_CONNECT	/*                                     */
#define DISCOVERY_EVENTS	/*                                        */
#define INITIAL_DISCOVERY	/*                                      */
#undef STREAM_COMPAT		/*                                */
#undef CONNECT_INDIC_KICK	/*                             */
#undef FAIL_SEND_DISCONNECT	/*                             */
#undef PASS_CONNECT_PACKETS	/*                   */
#undef MISSING_PPP_API		/*                         */

/*                          */
#define BLOCK_WHEN_CONNECT	/*                               */
#define CONNECT_IN_SEND		/*                                 */
#undef FLUSH_TO_PPP		/*                                          */
#undef SECURE_DEVIRNET		/*        */

/*                                                                 */

/*
                                                                
                                          
                                                         
 */
/*                 */
#define DEBUG_CTRL_TRACE	0	/*                 */
#define DEBUG_CTRL_INFO		0	/*              */
#define DEBUG_CTRL_ERROR	1	/*          */
#define DEBUG_FS_TRACE		0	/*                      */
#define DEBUG_FS_INFO		0	/*              */
#define DEBUG_FS_ERROR		1	/*          */
#define DEBUG_PPP_TRACE		0	/*                       */
#define DEBUG_PPP_INFO		0	/*              */
#define DEBUG_PPP_ERROR		1	/*          */
#define DEBUG_MODULE_TRACE	0	/*                          */
#define DEBUG_MODULE_ERROR	1	/*          */

/*                  */
#define DEBUG_IRDA_SR_TRACE	0	/*                  */
#define DEBUG_IRDA_SR_INFO	0	/*              */
#define DEBUG_IRDA_SR_ERROR	1	/*          */
#define DEBUG_IRDA_SOCK_TRACE	0	/*                            */
#define DEBUG_IRDA_SOCK_INFO	0	/*              */
#define DEBUG_IRDA_SOCK_ERROR	1	/*          */
#define DEBUG_IRDA_SERV_TRACE	0	/*                  */
#define DEBUG_IRDA_SERV_INFO	0	/*              */
#define DEBUG_IRDA_SERV_ERROR	1	/*          */
#define DEBUG_IRDA_TCB_TRACE	0	/*                      */
#define DEBUG_IRDA_CB_INFO	0	/*              */
#define DEBUG_IRDA_CB_ERROR	1	/*          */
#define DEBUG_IRDA_OCB_TRACE	0	/*                      */
#define DEBUG_IRDA_OCB_INFO	0	/*              */
#define DEBUG_IRDA_OCB_ERROR	1	/*          */

#define DEBUG_ASSERT		0	/*                       */

/*
                                                                
                                             
  
                                                               
                                                     
 */
/*                                                      */
#define DERROR(dbg, format, args...) \
	{if(DEBUG_##dbg) \
		printk(KERN_INFO "irnet: %s(): " format, __func__ , ##args);}

/*                                                       */
#define DEBUG(dbg, format, args...) \
	{if(DEBUG_##dbg) \
		printk(KERN_DEBUG "irnet: %s(): " format, __func__ , ##args);}

/*                             */
#define DENTER(dbg, format, args...) \
	{if(DEBUG_##dbg) \
		printk(KERN_DEBUG "irnet: -> %s" format, __func__ , ##args);}

/*                                                   */
#define DPASS(dbg, format, args...) \
	{if(DEBUG_##dbg) \
		printk(KERN_DEBUG "irnet: <>%s" format, __func__ , ##args);}

/*                            */
#define DEXIT(dbg, format, args...) \
	{if(DEBUG_##dbg) \
		printk(KERN_DEBUG "irnet: <-%s()" format, __func__ , ##args);}

/*                            */
#define DRETURN(ret, dbg, args...) \
	{DEXIT(dbg, ": " args);\
	return ret; }

/*                                     */
#define DABORT(cond, ret, dbg, args...) \
	{if(cond) {\
		DERROR(dbg, args);\
		return ret; }}

/*                                                   */
#define DASSERT(cond, ret, dbg, args...) \
	{if((DEBUG_ASSERT) && !(cond)) {\
		DERROR(dbg, "Invalid assertion: " args);\
		return ret; }}

/*                                                                  */

/*          */
#define IRNET_MAGIC	0xB00754

/*                                                           */
#define IRNET_MAX_EVENTS	8	/*                               */

/*                                                                 */

/*
                                                                       
                         
                                                                        
                                                                       
                                                                       
                                                             
 */
typedef struct irnet_socket
{
  /*                                                             */
  /*                                                   */
  irda_queue_t		q;		/*                            */
  int			magic;		/*          */

  /*                                                             */
  /*                                                  */
  struct file *		file;		/*                                  */
  /*                                  */
  struct ktermios	termios;	/*                   */
  /*                               */
  int			event_index;	/*                            */

  /*                                                              */
  /*                                                               */
  int			ppp_open;	/*                             */
  struct ppp_channel	chan;		/*                                */

  int			mru;		/*                         */
  u32			xaccm[8];	/*                                  */
  u32			raccm;		/*                         */
  unsigned int		flags;		/*                              */
  unsigned int		rbits;		/*                          */
  struct work_struct disconnect_work;   /*                               */
  /*                                                              */
  /*                                                    */
  unsigned long		ttp_open;	/*                         */
  unsigned long		ttp_connect;	/*                              */
  struct tsap_cb *	tsap;		/*                                 */

  char			rname[NICKNAME_MAX_LEN + 1];
					/*                              */
  __u32			rdaddr;		/*                             */
  __u32			rsaddr;		/*                              */
  __u32			daddr;		/*                          */
  __u32			saddr;		/*                       */
  __u8			dtsap_sel;	/*                      */
  __u8			stsap_sel;	/*                     */

  __u32			max_sdu_size_rx;/*                                  */
  __u32			max_sdu_size_tx;
  __u32			max_data_size;
  __u8			max_header_size;
  LOCAL_FLOW		tx_flow;	/*                               */

  /*                                                              */
  /*                                                    */
  void *		ckey;		/*                     */
  __u16			mask;		/*                                */
  int			nslots;		/*                               */

  struct iriap_cb *	iriap;		/*                          */
  int			errno;		/*                         */

  /*                                                              */
  /*                                                 
                                                */
  struct irda_device_info *discoveries;	/*                           */
  int			disco_index;	/*                                */
  int			disco_number;	/*                           */

  struct mutex		lock;

} irnet_socket;

/*
                                                                         
 */
typedef enum irnet_event
{
  IRNET_DISCOVER,		/*                           */
  IRNET_EXPIRE,			/*                    */
  IRNET_CONNECT_TO,		/*                                          */
  IRNET_CONNECT_FROM,		/*                                          */
  IRNET_REQUEST_FROM,		/*                                  */
  IRNET_NOANSWER_FROM,		/*                           */
  IRNET_BLOCKED_LINK,		/*                                  */
  IRNET_DISCONNECT_FROM,	/*                               */
  IRNET_DISCONNECT_TO		/*                      */
} irnet_event;

/*
                                                     
 */
typedef struct irnet_log
{
  irnet_event	event;
  int		unit;
  __u32		saddr;
  __u32		daddr;
  char		name[NICKNAME_MAX_LEN + 1];	/*        */
  __u16_host_order hints;			/*                     */
} irnet_log;

/*
                                                          
 */
typedef struct irnet_ctrl_channel
{
  irnet_log	log[IRNET_MAX_EVENTS];	/*           */
  int		index;		/*                      */
  spinlock_t	spinlock;	/*                                   */
  wait_queue_head_t	rwait;	/*                                     */
} irnet_ctrl_channel;

/*                                                                  */
/*
                                       
                                                                        
 */

/*                                                                 */
extern int
	irda_irnet_create(irnet_socket *);	/*                           */
extern int
	irda_irnet_connect(irnet_socket *);	/*                          */
extern void
	irda_irnet_destroy(irnet_socket *);	/*                          */
extern int
	irda_irnet_init(void);		/*                               */
extern void
	irda_irnet_cleanup(void);	/*                             */

/*                                                                 */

/*                                                   */
extern struct irnet_ctrl_channel	irnet_events;

#endif /*         */
