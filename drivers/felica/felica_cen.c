/*
                
  
 */

/*
                              
 */
#include <linux/module.h>
#include <linux/kernel.h>


#include "felica_cen.h"
#include "felica_rfs.h"
#include "felica_test.h"

/*
                         
 */




//                         
#define FELICA_I2C_SLAVE_ADDRESS  0x56
#define FELICA_I2C_REG_ADDRSS_01  0x01
#define FELICA_I2C_REG_ADDRSS_02  0x02

static DEFINE_MUTEX(felica_cen_mutex);

static int isopen = 0; //                     
/*
                         
 */

/*
               
         
          
 */
static int felica_cen_open (struct inode *inode, struct file *fp)
{


  if(1 == isopen)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_CEN] felica_cen_open - already open \n");
    #endif
    return -1;
  }
  else
  {
    #ifdef FEATURE_DEBUG_LOW
    FELICA_DEBUG_MSG("[FELICA_CEN] felica_cen_open - start \n");
    #endif
    isopen = 1;
  }

//                                                                                     

#ifdef FELICA_FN_DEVICE_TEST
	FELICA_DEBUG_MSG("[FELICA_CEN] felica_cen_open - result_open(%d) \n",result_open_cen);
	return result_open_cen;
#else
  return 0;
#endif
}

/*
               
         
          
 */
static int felica_cen_release (struct inode *inode, struct file *fp)
{


  if(0 == isopen)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_CEN] felica_cen_release - not open \n");
    #endif

    return -1;
  }
  else
  {
    #ifdef FEATURE_DEBUG_LOW
    FELICA_DEBUG_MSG("[FELICA_CEN] felica_cen_release - start \n");
    #endif

    isopen = 0;
  }

#ifdef FELICA_FN_DEVICE_TEST
  FELICA_DEBUG_MSG("[FELICA_CEN] felica_cen_release - result_close_cen(%d) \n",result_close_cen);
  return result_close_cen;
#else
	return 0;
#endif
}

/*
               
         
          
 */
static ssize_t felica_cen_read(struct file *fp, char *buf, size_t count, loff_t *pos)
{
  unsigned char read_buf = 0x00;
  int felica_cen = -1, rc = -1;


#ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA_CEN] felica_cen_read - start \n");
#endif

/*             */
  if(NULL == fp)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_CEN] ERROR fp is NULL \n");
	#endif
    return -1;
  }

  if(NULL == buf)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_CEN] ERROR buf is NULL \n");
	#endif
    return -1;
  }

  if(1 != count)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_CEN] ERROR count(%d) \n",count);
	#endif
    return -1;
  }

  if(NULL == pos)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_CEN] ERROR pos is NULL \n");
	#endif
    return -1;
  }

  mutex_lock(&felica_cen_mutex);
  rc = felica_i2c_read(0x02, &read_buf, 1);
  mutex_unlock(&felica_cen_mutex);

  if(rc)
  {
    FELICA_DEBUG_MSG("[FELICA_CEN] ERROR felica_i2c_read(%d) \n",rc);
    return -1;
  }

  //                    
  if(read_buf&0x01)  //       
  {
    #ifdef FEATURE_DEBUG_MED
    FELICA_DEBUG_MSG("[FELICA_CEN] CEN = High (UNLOCK) \n");
    #endif
    felica_cen = GPIO_HIGH_VALUE;
  }
  else  //     
  {
    #ifdef FEATURE_DEBUG_MED
    FELICA_DEBUG_MSG("[FELICA_CEN] CEN = Low (LOCK) \n");
    #endif
    felica_cen = GPIO_LOW_VALUE;
  }

  rc = copy_to_user(buf, &felica_cen, count);
  if(rc)
  {
	#ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_CEN] ERROR - copy_from_user \n");
	#endif
    return -1;
  }

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA_CEN] felica_cen_write - end \n");
  #endif

#ifdef FELICA_FN_DEVICE_TEST
	FELICA_DEBUG_MSG("[FELICA_CEN] felica_cen_read - result_read_cen = %d,(%d) \n",felica_cen,result_read_cen);
	return result_read_cen;
#else
  return 1;
#endif
}

/*
               
         
          
 */
static ssize_t felica_cen_write(struct file *fp, const char *buf, size_t count, loff_t *pos)
{
  unsigned char write_buf = 0x00;
  int rc = -1;


  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA_CEN] felica_cen_write - start \n");
  #endif

/*             */
  if(NULL == fp)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_CEN] ERROR fp is NULL \n");
	#endif
    return -1;
  }

  if(NULL == buf)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_CEN] ERROR buf is NULL \n");
	#endif
    return -1;
  }

  if(1 != count)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_CEN]ERROR count(%d) \n",count);
	#endif
    return -1;
  }

  if(NULL == pos)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_CEN] ERROR pos is NULL \n");
	#endif
    return -1;
  }

  /*                     */
  rc = copy_from_user(&write_buf, buf, count);
  if(rc)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_CEN] ERROR - copy_from_user \n");
	#endif
    return -1;
  }

    #ifdef FEATURE_DEBUG_MED
	FELICA_DEBUG_MSG("[FELICA_CEN] copy_from_user(%d) \n",*buf);
    #endif

  /*                 */
  if(*buf == 1)
  {
    #ifdef FEATURE_DEBUG_MED
    FELICA_DEBUG_MSG("[FELICA_CEN] CEN = High (UNLOCK) \n");
    #endif
    write_buf = 0x81; //           
  }
  else
  {
    #ifdef FEATURE_DEBUG_MED
    FELICA_DEBUG_MSG("[FELICA_CEN] CEN = Low (LOCK) \n");
    #endif
    write_buf = 0x80; //         
  }

  /*                 */
  mutex_lock(&felica_cen_mutex);
  rc = felica_i2c_write(0x02, &write_buf, 1);
  mutex_unlock(&felica_cen_mutex);
  mdelay(2);

//                               
#if 0
	if(*buf == 1)
	{
		rc= request_irq(gpio_to_irq(felica_get_rfs_gpio_num()), felica_rfs_detect_interrupt, IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING|IRQF_NO_SUSPEND , FELICA_RFS_NAME, NULL);
		if (rc)
		{
			FELICA_DEBUG_MSG("[FELICA_RFS] FAIL!! can not request_irq %d\n", rc);
			return rc;
		}
	}
	else
	{
		free_irq(gpio_to_irq(felica_get_rfs_gpio_num()), NULL);
	}
#endif

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA_CEN] felica_cen_write - end \n");
  #endif
  return 1;
}

/*
                       
 */

static struct file_operations felica_cen_fops =
{
  .owner    = THIS_MODULE,
  .open    = felica_cen_open,
  .read    = felica_cen_read,
  .write    = felica_cen_write,
  .release  = felica_cen_release,
};

static struct miscdevice felica_cen_device =
{
  .minor = MINOR_NUM_FELICA_CEN,
  .name = FELICA_CEN_NAME,
  .fops = &felica_cen_fops
};

/*
               
         
          
 */
static int felica_cen_init(void)
{
  int rc = -1;

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA_CEN] felica_cen_init - start \n");
  #endif

  /*                          */
  rc = misc_register(&felica_cen_device);
  if (rc < 0)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_CEN] ERROR can not register felica_cen \n");
	#endif
    return rc;
  }

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA_CEN] felica_cen_init - end \n");
  #endif

  return 0;
}

/*
               
         
          
 */
static void felica_cen_exit(void)
{
  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA_CEN] felica_cen_exit - start \n");
  #endif

  /*                            */
  misc_deregister(&felica_cen_device);

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA_CEN] felica_cen_exit - end \n");
  #endif
}

module_init(felica_cen_init);
module_exit(felica_cen_exit);

MODULE_LICENSE("Dual BSD/GPL");
