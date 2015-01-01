/*****************************************************************************

	Copyright(c) 2008 LG Electronics Inc. All Rights Reserved

	File name : mbt_ringbuffer.c

	Description :

    Hoistory
	----------------------------------------------------------------------
	Mar. 16, 2009:		inb612		create

*******************************************************************************/
//                  
//                   
//                   

//                    

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
//                        
#include <linux/string.h>
//                        

#include <mbt_ring.h>
//                    

static DEFINE_MUTEX(ringbuffer_lock);

/*                                                            
                      
                                                             */
#define USE_STATIC_DATA_BUFFER	//                                                         
#define RING_DATA_BUFFER_SIZE 188*128*30

/*                                                            
                             
                                                             */

/*                                                            
                             
                                                             */

/*                                                            
                                   
                                                             */

/*                                                            
                        
                                                             */

/*                                                            
                           
                                                             */

/*                                                            
                           
                                                             */ 
#ifdef USE_STATIC_DATA_BUFFER
static char data_buf[RING_DATA_BUFFER_SIZE] = {0,}; 
#endif

/*                                                            
                                   
                                                             */


/*                                                        
                                    
                                         
                                                          
                       

                                                     
                                                         
                                            
                                                         */
void mbt_dataring_create(unsigned int *buffer_id, int len)
{
	void *data = NULL;
	MBT_RING *rbuf = NULL;
	//                 

	if((buffer_id == NULL) || (len <= 0))
	{
		return;
	}
	
	//                                        

	//         
	{
		rbuf = (MBT_RING*)kmalloc(sizeof(MBT_RING), GFP_KERNEL);
		if(rbuf == NULL)
		{
			//                                      
			//                                       
			printk("mbt_dataring_create fail no memory !!! \n");
			return;
		}

		memset(rbuf, 0x00, sizeof(MBT_RING));
		//                               
		#ifdef USE_STATIC_DATA_BUFFER
		data = &data_buf[0];
		#else
		data = kmalloc(len, GFP_KERNEL);
		#endif
		if(data)
		{
			#ifdef USE_STATIC_DATA_BUFFER
			memset(data, 0x00, RING_DATA_BUFFER_SIZE);
			#else
			memset(data, 0x00, len);
			#endif
		}
		#ifdef USE_STATIC_DATA_BUFFER
		mbt_ring_init(rbuf, data, RING_DATA_BUFFER_SIZE);
		#else
		mbt_ring_init(rbuf, data, len);
		#endif
		*buffer_id = (unsigned int)rbuf;
		//                                       
		//                               
	}
}
EXPORT_SYMBOL(mbt_dataring_create);


/*                                                        
                                     
                                            
                                  
                       

                                                     
                                                         
                                            
                                                         */
void mbt_dataring_destroy(unsigned int * buffer_id)
{
	//                  
	MBT_RING *rbuf = NULL;
	//                 
	
	if(buffer_id == NULL)
	{
		return;
	}

	if(*buffer_id == 0)
	{
		return;
	}

	rbuf = (MBT_RING*)(*buffer_id);	

	//                    
	// 
	//        
	// 

	//                    
	
	//                                      
	mutex_lock(&ringbuffer_lock);
	#ifndef USE_STATIC_DATA_BUFFER
	if(rbuf->data != NULL)
	{
		kfree(rbuf->data);
	}
	#endif
	
	kfree((void*)rbuf);
	*buffer_id = 0;
	//                                      
	//                                       

	mutex_unlock(&ringbuffer_lock);
}
EXPORT_SYMBOL(mbt_dataring_destroy);

/*                                                        
                                 
                                  
                               
                                                    
 
                                    
                                                          
                                         
                                                         */
int mbt_dataring_empty(unsigned int buffer_id)
{
	if(buffer_id == 0)
	{
		return -1;
	}
	
	return mbt_ring_empty((MBT_RING*)buffer_id);
}
EXPORT_SYMBOL(mbt_dataring_empty);

/*                                                        
                                
                                              
                               
                                        
 
                                    
                                                          
                                         
                                                         */
int mbt_dataring_free(unsigned int buffer_id)
{
	if(buffer_id == 0)
	{
		return -1;
	}

	return mbt_ring_free((MBT_RING*)buffer_id);
}
EXPORT_SYMBOL(mbt_dataring_free);

/*                                                        
                                 
                                                   
                               
                                            
 
                                    
                                                          
                                         
                                                         */
int mbt_dataring_avail(unsigned int buffer_id)
{	
	if(buffer_id == 0)
	{
		return -1;
	}

	return mbt_ring_avail((MBT_RING*)buffer_id);
}
EXPORT_SYMBOL(mbt_dataring_avail);

/*                                                        
                                 
                                  
                               
                      
 
                                    
                                                          
                                         
                                                         */
void mbt_dataring_flush(unsigned int buffer_id)
{
	MBT_RING *rbuf;
	
	if(buffer_id == 0)
	{
		return;
	}

	rbuf = (MBT_RING*)buffer_id;

	//                    
	// 
	//        
	// 

	//                                            
	mutex_lock(&ringbuffer_lock);
	mbt_ring_flush(rbuf);
	//                                            
	mutex_unlock(&ringbuffer_lock);
}
EXPORT_SYMBOL(mbt_dataring_flush);

/*                                                        
                                
                                      
                                                                        
                                       
 
                                    
                                                          
                                         
                                                         */
int mbt_dataring_read(unsigned int buffer_id, char* buf, int len)
{
	MBT_RING *rbuf;
	
	if((buffer_id == 0)||(buf == NULL)||(len<=0))
	{
		return -1;
	}

	rbuf = (MBT_RING*)buffer_id;

	if(/*                     */ (rbuf->data == NULL))
	{
		return -1;
	}

	//                                            
	mutex_lock(&ringbuffer_lock);
	mbt_ring_read(rbuf, (char*)buf, (int)len);
	mutex_unlock(&ringbuffer_lock);
	//                                            
	return len;
}
EXPORT_SYMBOL(mbt_dataring_read);

/*                                                        
                                 
                                       
                                                                        
                                        
 
                                    
                                                          
                                         
                                                         */
int mbt_dataring_write(unsigned int buffer_id, const char * buf, int len)
{
	MBT_RING *rbuf;
	
	if((buffer_id == 0)||(buf == NULL) ||(len <= 0))
	{
		return -1;
	}

	rbuf = (MBT_RING*)buffer_id;

	if(/*                     */ (rbuf->data == NULL))
	{
		return -1;
	}

	//                                            
	mutex_lock(&ringbuffer_lock);
	mbt_ring_write(rbuf, (char*)buf, (int)len);
	mutex_unlock(&ringbuffer_lock);
	//                                            
	return len;
}
EXPORT_SYMBOL(mbt_dataring_write);

