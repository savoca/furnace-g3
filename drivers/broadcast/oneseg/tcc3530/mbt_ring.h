/*****************************************************************************

	Copyright(c) 2011 LG Electronics Inc. All Rights Reserved

	File name : mbt_ring.h

	Description :

    Hoistory
	----------------------------------------------------------------------
	Feb. 11. 2011:		hyewon.eum		create

*******************************************************************************/ 
#ifndef _MBT_RING_H_
#define _MBT_RING_H_

/*                                                            
                      
                                                             */
typedef struct _mbt_ring 
{
	char *data;
	int size;
	int pread;
	int pwrite;
	unsigned int mutex;
}MBT_RING;

/*                                                        
                              
                                         
                                                                              
                       

                                                     
                                                         
                                            
                                                         */
static void mbt_ring_init(MBT_RING *rbuf, void *data, int len)
{		
	rbuf->pread=rbuf->pwrite=0;
	rbuf->data=data;
	rbuf->size=len;
}

/*                                                        
                             
                                  
                               
                                                    
 
                                    
                                                          
                                         
                                                         */
static int mbt_ring_empty(MBT_RING *rbuf)
{	
	return (rbuf->pread==rbuf->pwrite);
}

/*                                                        
                            
                                              
                               
                                        
 
                                    
                                                          
                                         
                                                         */
static int mbt_ring_free(MBT_RING *rbuf)
{
	int free;
	
	free = rbuf->pread - rbuf->pwrite;
	if (free <= 0)
	{
		free += rbuf->size;
	}

	return free-1;
}

/*                                                        
                             
                                                   
                               
                                            
 
                                    
                                                          
                                         
                                                         */
static int mbt_ring_avail(MBT_RING *rbuf)
{
	int avail;

	avail = rbuf->pwrite - rbuf->pread;
	if (avail < 0)
	{
		avail += rbuf->size;
	}

	return avail;
}

/*                                                        
                             
                                  
                               
                      
 
                                    
                                                          
                                         
                                                         */
static void mbt_ring_flush(MBT_RING *rbuf)
{
	rbuf->pread = rbuf->pwrite;
}

/*                                                        
                            
                                      
                                                                        
                                       
 
                                    
                                                          
                                         
                                                         */
static int mbt_ring_read(MBT_RING *rbuf, char *buf, int len)
{
	int todo = len;
	int split;
	
	split = (rbuf->pread + len > rbuf->size) ? rbuf->size - rbuf->pread : 0;

	if (split > 0) 
	{
		memcpy(buf,rbuf->data+rbuf->pread,split);

		buf += split;
		todo -= split;
		rbuf->pread = 0;

	}

	memcpy(buf, rbuf->data+rbuf->pread, todo);

	rbuf->pread = (rbuf->pread + todo) % rbuf->size;

	return len;

}

/*                                                        
                             
                                       
                                                                        
                                        
 
                                    
                                                          
                                         
                                                         */
static int mbt_ring_write(MBT_RING *rbuf, const char *buf,int len)
{
	int todo = len;
	int split;

	split = (rbuf->pwrite + len > rbuf->size) ? rbuf->size - rbuf->pwrite : 0;

	if (split > 0) 
	{
		memcpy(rbuf->data+rbuf->pwrite, (void*)buf, split);

		buf += split;
		todo -= split;
		rbuf->pwrite = 0;
	}

	memcpy(rbuf->data+rbuf->pwrite, (void*)buf, todo);	

	rbuf->pwrite = (rbuf->pwrite + todo) % rbuf->size;

	return len;
}

#endif /*               */
