/*
                
  
 */

/*
                            
 */
#include <linux/syscalls.h>
#include <linux/i2c-dev.h>

#include "felica_i2c.h"

/*
                        
 */

#define I2C_FELICA_SLAVE_ADDRESS     0x56

/*
                      
 */

static int fd = -1;

/*
                        
 */

/*
               
         
          
*/
int felica_i2c_open (void)
{
  mm_segment_t old_fs = get_fs();

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA_I2C] felica_i2c_open\n");
  #endif

  set_fs(KERNEL_DS);
  fd = sys_open(FELICA_IC2_NAME, O_RDWR|O_NONBLOCK, 0);

  FELICA_DEBUG_MSG("[FELICA] cbal - sys_open fd : %d \n",fd);


  if (fd < 0)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_I2C] ERROR - felica_i2c_open : %d \n", fd);
	#endif
    return fd;
  }

  set_fs(old_fs);

  return 0;
}

/*
               
         
          
*/
int felica_i2c_release (void)
{
  int rc = 0;
  mm_segment_t old_fs = get_fs();

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA_I2C] felica_i2c_release\n");
  #endif

  set_fs(KERNEL_DS);
  rc = sys_close(fd);
  if (rc < 0)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_I2C] ERROR - felica_i2c_release : %d \n", rc);
	#endif
    return rc;
  }
  set_fs(old_fs);

  return 0;
}

/*
               
         
          
*/
int felica_i2c_set_slave_address (unsigned char slave_address)
{
  int rc = -1;

  rc = sys_ioctl(fd, I2C_SLAVE_FORCE, slave_address>>1); //              
//                                                                   

  if (rc < 0)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_I2C] ERROR - sys_ioctl : %d \n",rc);
	#endif
    return rc;
  }

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA_I2C] slave address : 0x%02x \n",slave_address>>1);
  #endif

  return 0;
}

/*
               
         
          
*/
int felica_i2c_read(unsigned char reg, unsigned char *buf, size_t count)
{
  ssize_t rc = 0;
  mm_segment_t old_fs = get_fs();
  int retry = 10;

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA_I2C] felica_i2c_read\n");
  #endif

//                                                                    
  while((I2C_STATUS_FOR_NFC == get_felica_i2c_status())&&(retry > 0))
  {
    usleep(100);
    retry--;
  }

//                                                  
  if(I2C_STATUS_FOR_NFC == get_felica_i2c_status())
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_I2C] ERROR - other device(NFC) use i2c abnormally \n");
	#endif
    return rc;
  }

  set_felica_i2c_status(I2C_STATUS_FOR_FELICA);

  set_fs(KERNEL_DS);

  /*                            */
  rc = felica_i2c_open();
  if (rc)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_I2C] ERROR - felica_i2c_open : %d \n",rc);
	#endif
    goto ERROR;
  }

  /*                   */
  rc = felica_i2c_set_slave_address(I2C_FELICA_SLAVE_ADDRESS);
  if (rc)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_I2C] ERROR - felica_i2c_set_slave_address : %d \n",rc);
	#endif
    goto ERROR;
  }

  /*                      */
  rc = sys_write(fd, &reg, 1);
  if (rc < 0)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_I2C] ERROR - sys_write : %d \n",rc);
	#endif
    goto ERROR;
  }

  /*                    */
  rc = sys_read(fd, buf, count);

  #ifdef FEATURE_DEBUG_MED
  FELICA_DEBUG_MSG("[FELICA_I2C] read data : 0x%02x \n",*buf);
  #endif

  if (rc < 0)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_I2C] ERROR - sys_read : %d \n",rc);
	#endif
    goto ERROR;
  }

  /*             */
  rc = felica_i2c_release();
  if (rc)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_I2C] ERROR - felica_i2c_release : %d \n",rc);
	#endif
    goto ERROR;
  }

  set_fs(old_fs);
  
//                                           
  set_felica_i2c_status(I2C_STATUS_READY);
  
  return 0;

  ERROR:
//                                             
    set_felica_i2c_status(I2C_STATUS_READY);
    return rc;
}

/*
               
         
          
*/
int felica_i2c_write(unsigned char reg, unsigned char *buf, size_t count)
{
  ssize_t rc = 0;
  unsigned char write_buf[2];
  mm_segment_t old_fs = get_fs();
  int retry = 10;

  #ifdef FEATURE_DEBUG_LOW
  FELICA_DEBUG_MSG("[FELICA_I2C] felica_i2c_write\n");
  #endif

 //                                                                   
  while((I2C_STATUS_FOR_NFC == get_felica_i2c_status())&&(retry > 0))  
  {
    usleep(100);
    retry--;
  }

//                                                  
  if(I2C_STATUS_FOR_NFC == get_felica_i2c_status())
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_I2C] ERROR - other device(NFC) use i2c abnormally \n");
	#endif
    return rc;
  }

  set_felica_i2c_status(I2C_STATUS_FOR_FELICA);

  set_fs(KERNEL_DS);

  /*                            */
  rc = felica_i2c_open();
  if (rc)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_I2C] ERROR - felica_i2c_open : %d \n",rc);
	#endif
    goto ERROR;
  }

  /*                   */
  rc = felica_i2c_set_slave_address(I2C_FELICA_SLAVE_ADDRESS);
  if (rc)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_I2C] ERROR - felica_i2c_set_slave_address : %d \n",rc);
	#endif
    goto ERROR;
  }

  /*               */
  memset(write_buf,0x00,2*sizeof(unsigned char));
  write_buf[0] = reg;
  write_buf[1] = *buf;

  #ifdef FEATURE_DEBUG_MED
  FELICA_DEBUG_MSG("[FELICA_I2C] write_buf[0][1] : 0x%02x 0x%02x \n",write_buf[0],write_buf[1]);
  #endif

  /*            */
  rc = sys_write(fd, write_buf, 2);
  if (rc < 0)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_I2C] ERROR - sys_write : %d \n",rc);
	#endif
    goto ERROR;
  }

  /*             */
  rc = felica_i2c_release();
  if (rc)
  {
    #ifdef FEATURE_DEBUG_HIGH
    FELICA_DEBUG_MSG("[FELICA_I2C] ERROR - felica_i2c_release : %d \n",rc);
	#endif
    goto ERROR;
  }

  set_fs(old_fs);

//                                           
  set_felica_i2c_status(I2C_STATUS_READY);
  
  return 0;

  ERROR:
//                                             
    set_felica_i2c_status(I2C_STATUS_READY);    
    return rc;
}
