/*
            
  
 */

/*
                              
 */
#include <linux/mutex.h>
#include <linux/syscalls.h>
#include "felica.h"
#include "felica_uart.h"

#include "felica_test.h"

/*
          
 */

enum{
UART_STATUS_NOT_OPEN= 0,
UART_STATUS_RX,
UART_STATUS_RX_TX,
};

/*                                            */
#define RECEIVE_BUFFER_MAX_SIZE 4096 /*                                             */
#define TRANSMIT_BUFFER_MAX_SIZE 4096

char receive_buf[RECEIVE_BUFFER_MAX_SIZE + 4];
char transmit_buf[TRANSMIT_BUFFER_MAX_SIZE + 4];

/*
                      
 */
static int uart_status = UART_STATUS_NOT_OPEN;

static DEFINE_MUTEX(felica_mutex);

/*
                        
 */

/*
                        
 */

/*
               
         
          
*/
static int felica_open (struct inode *inode, struct file *fp)
{
  int rc = 0;

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA] felica_open() is called \n");
  #endif

  /*                        */
  if(NULL == fp)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - fp is NULL \n");
	#endif
    return -1;
  }

  /*                                                 
                                     */

  if(UART_STATUS_NOT_OPEN == uart_status)
  {
    uart_status = UART_STATUS_RX;
  }
  else if(UART_STATUS_RX == uart_status)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] /dev/felica RX is already openned. \n");
    #endif

    uart_status = UART_STATUS_RX_TX;

    return 0;
  }
  else
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] /dev/felica RX/TX are already openned. \n");
    #endif

    return 0;
  }

  rc = felica_uart_open();

  //            

  if(rc)
  {
    FELICA_DEBUG_MSG("[FELICA] ERROR - open_hs_uart \n");
    uart_status = UART_STATUS_NOT_OPEN;
    unlock_felica_wake_lock();
    return rc;
  }

  lock_felica_wake_lock();

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA] felica_open - end \n");
  #endif

#ifdef FELICA_FN_DEVICE_TEST
  FELICA_DEBUG_MSG("[FELICA] felica_open - result_open(%d) \n",result_open_uart);
  return result_open_uart;
#else
  return 0;
#endif

}

/*
               
         
          
*/

static ssize_t felica_read(struct file *fp, char *buf, size_t count, loff_t *pos)
{
  int rc = 0;
  int readcount = 0;

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA] felica_read - start \n");
  #endif

  /*                        */
  if(NULL == fp)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - fp is NULL \n");
	#endif
    return -1;
  }

  if(NULL == buf)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - buf is NULL \n");
	#endif
    return -1;
  }

  if(count > RECEIVE_BUFFER_MAX_SIZE)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - count(%d) \n",count);
	#endif
    return -1;
  }

  if(NULL == pos)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - pos is NULL \n");
	#endif
    return -1;
  }

  memset(receive_buf, 0, sizeof(receive_buf));

  /*         
   
                           
                   

                                                 
                                                                             
                 
                                                                 
                    
     

                             
     
                
                            

                       

                                                        

                        

                     
       
                                                         
                                      
         
                                            
                           
           
                                   
           
         
                               
       
     

   */


  /*                                          */
  mutex_lock(&felica_mutex);
  readcount = felica_uart_read(receive_buf,count);
  mutex_unlock(&felica_mutex);

  if(0 >= readcount)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - No data in data buffer \n");
	#endif
    return 0;
  }

  //          

  /*         
   
                           
                   

                                                 
                                                                        
                 
                                                                 
                    
     
   
  */

/*                                */
#ifdef RXTX_LOG_ENABLE
  {
    int i = 0;
    char *ptr = NULL;

    ptr = receive_buf;
    if(NULL != ptr)
    {
      FELICA_DEBUG_MSG("===== READ FELICA RESPONSE ===== \n");
	  FELICA_DEBUG_MSG("FELICA - ");
      for(i=0; i<count; i++)
      {
        FELICA_DEBUG_MSG(" %02x", *ptr++);
        if(0 == (i+1)%10)
        {
          FELICA_DEBUG_MSG("\n");
		  FELICA_DEBUG_MSG("FELICA - ");
        }
      }
      FELICA_DEBUG_MSG("\n");
    }
  }
#endif
  /*                                    */
  rc = copy_to_user(buf, receive_buf, count);

  if (rc) {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - copy_to_user \n");
	#endif
    return rc;
  }

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA] felica_read - end \n");
  #endif

#ifdef FELICA_FN_DEVICE_TEST
  if(result_read_uart != -1)
    result_read_uart = readcount;

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA] felica_read - result_read_uart(%d) \n",result_read_uart);
  #endif

  return result_read_uart;
#else
  return readcount;
#endif
}

/*
               
         
          
*/
static ssize_t felica_write(struct file *fp, const char *buf, size_t count, loff_t *f_pos)
{
  int rc = 0;
  int writecount = 0;

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA] felica_write - start \n");
  #endif

  /*                        */
  if(NULL == fp)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - fp is NULL \n");
	#endif
    return -1;
  }

  if(NULL == buf)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - buf is NULL \n");
	#endif
    return -1;
  }

  if(count > TRANSMIT_BUFFER_MAX_SIZE)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - count(%d) \n",count);
	#endif
    return -1;
  }

  /*                                    */
  memset(transmit_buf, 0, sizeof(transmit_buf));

  /*                                   */
  rc = copy_from_user(transmit_buf, buf, count);
  if (rc) {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - copy_to_user \n");
	#endif
    return rc;
  }

/*                                */
 #ifdef RXTX_LOG_ENABLE
 {
    int i = 0;
    char *ptr = NULL;

    ptr = transmit_buf;

    if(NULL != ptr)
    {
      FELICA_DEBUG_MSG("===== WRITE FELICA COMMAND =====\n");
      for(i=0; i<count; i++)
      {
        FELICA_DEBUG_MSG(" %02x", *ptr++);
        if(0 == (i+1)%10)
        {
          FELICA_DEBUG_MSG("\n");
        }
      }
      FELICA_DEBUG_MSG("\n");
    }
  }
#endif

  /*         
   
                           
                   

                                                 
                                                                        
                 
                                                                 
                    
     
   
    */

  /*                                            */
  mutex_lock(&felica_mutex);
  writecount = felica_uart_write(transmit_buf,count);
  mutex_unlock(&felica_mutex);

  //           

  #ifdef FEATURE_DEBUG_MED
  FELICA_DEBUG_MSG("[FELICA] writecount : %d \n",writecount);
  #endif

#ifdef FELICA_FN_DEVICE_TEST
  FELICA_DEBUG_MSG("[FELICA] felica_write - result_write_uart(%d) \n",result_write_uart);

  if(result_write_uart != -1) {
    if (writecount == 0)
      result_write_uart = -1;
    else
      result_write_uart = writecount;
	}
    return result_write_uart;
#else
  return writecount;
#endif
}

/*
               
         
          
*/
static int felica_release (struct inode *inode, struct file *fp)
{
  int rc = 0;

  /*                        */
  if(NULL == fp)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - fp is NULL \n");
	#endif
    return -1;
  }
  /*                                                  
                                     */

  if(UART_STATUS_RX_TX == uart_status)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] /dev/felica RX is closed. \n");
    #endif

    uart_status = UART_STATUS_RX;

    return 0;
  }
  else if(UART_STATUS_RX == uart_status)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] /dev/felica TX is closed. \n");
    #endif

    uart_status = UART_STATUS_NOT_OPEN;
  }
  else
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] /dev/felica RX/TX are already closed. \n");
    #endif

    return 0;
  }

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA] felica_release - start \n");
  #endif

  rc = felica_uart_close();

  unlock_felica_wake_lock();

  if(rc)
  {
	#ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - open_hs_uart \n");
	#endif
    return rc;
  }

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA] felica_release - end \n");
  #endif

#ifdef FELICA_FN_DEVICE_TEST
  FELICA_DEBUG_MSG("[FELICA] felica_close - result_close(%d) \n",result_close_uart);
  return result_close_uart;
#else
  return 0;
#endif
}

/*
                                                                                                         
              
                                                      
*/
static long felica_ioctl (struct file *fp, unsigned int cmd, unsigned long arg)
{
  int numofreceiveddata = 0;
  int rc = 0;
  int *uarg = (int *)arg;

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA] felica_ioctl - start \n");
  #endif

  /*                        */
  if(NULL == fp)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - fp is NULL \n");
	#endif
    return -1;
  }

  if(IOCTL_FELICA_MAGIC != _IOC_TYPE(cmd))
  {
	#ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - IO cmd type(%d) \n",_IOC_TYPE(cmd));
	#endif
    return -1;
  }

  if(IOCTL_FELICA_CMD_AVAILABLE != _IOC_NR(cmd))
  {
  	#ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - IO cmd number(%d) \n",_IOC_NR(cmd));
	#endif
    return -1;
  }

  if(0 != _IOC_SIZE(cmd))
  {
  	#ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - IO cmd size(%d) \n",_IOC_SIZE(cmd));
	#endif
    return -1;
  }

  mutex_lock(&felica_mutex);
  rc = felica_uart_ioctrl(&numofreceiveddata);
  mutex_unlock(&felica_mutex);

  if (rc) {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - felica_uart_ioctrl \n");
	#endif
    return rc;
  }

   //           

  rc = copy_to_user(uarg, &numofreceiveddata, sizeof(int));
  if(rc)
  {
  	#ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - open_hs_uart \n");
	#endif
    return rc;
  }

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA] felica_ioctl - end \n");
  #endif

#ifdef FELICA_FN_DEVICE_TEST
  return result_available_uart;
#else
  return rc;
#endif
}

/*
               
         
          
*/
static int felica_fsync(struct file *fp, loff_t param1, loff_t param2, int datasync)
{
  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA] felica_fsync \n");
  #endif

  //                                   

  return 0;
}

static struct file_operations felica_fops =
{
  .owner    = THIS_MODULE,
  .open      = felica_open,
  .read      = felica_read,
  .write    = felica_write,
  .unlocked_ioctl    = felica_ioctl,
  .fsync    = felica_fsync,
  .release  = felica_release,
};

static struct miscdevice felica_device = {
  .minor = MINOR_NUM_FELICA,
  .name = FELICA_NAME,
  .fops = &felica_fops,
};

/*
               
         
          
*/
static int felica_init(void)
{
  int rc = 0;

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA] felica_init - start \n");
  #endif

  /*                          */
  rc = misc_register(&felica_device);
  if(rc)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA] ERROR - felica_init can not register(%d)\n", rc);
	#endif
    return rc;
  }

  init_felica_wake_lock();

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA] felica_init - end \n");
  #endif

  return 0;
}

/*
               
         
          
*/
static void felica_exit(void)
{
  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA] felica_exit - start \n");
  #endif

  destroy_felica_wake_lock();

  /*                            */
  misc_deregister(&felica_device);

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA] felica_exit - end \n");
  #endif
}

module_init(felica_init);
module_exit(felica_exit);

MODULE_LICENSE("Dual BSD/GPL");
