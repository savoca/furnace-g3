#ifndef _LINUX_VIRTIO_H
#define _LINUX_VIRTIO_H
/*                                                                    
                   */
#include <linux/types.h>
#include <linux/scatterlist.h>
#include <linux/spinlock.h>
#include <linux/device.h>
#include <linux/mod_devicetable.h>
#include <linux/gfp.h>

/* 
                                                                    
                                                 
                                                                           
                                                           
                                                       
                                                          
                                                            
 */
struct virtqueue {
	struct list_head list;
	void (*callback)(struct virtqueue *vq);
	const char *name;
	struct virtio_device *vdev;
	struct virtqueue_ops *vq_ops;
	void *priv;
};

/* 
                                                             
                                       
                                                
                                        
                                                   
                                                                    
                                          
                                                                         
                              
                           
                                                                       
                                     
                                                
                                          
                                                      
                                 
                                                
                                                                           
                             
                                                  
                                                
                                                                     
                                                                     
                                                  
                                                          
                                                
                                                                 
                                                                      
                                                                     
                                                                       
                          
                                                             
                                                    
                                                 
                                                
                                                     
                                                                    
                                                                      
                                                                  
                                                                
                 
  
                                                                   
                                                                                
                  
  
                                               
 */
struct virtqueue_ops {
	int (*add_buf)(struct virtqueue *vq,
		       struct scatterlist sg[],
		       unsigned int out_num,
		       unsigned int in_num,
		       void *data,
		       gfp_t gfp);

	void (*kick)(struct virtqueue *vq);
	bool (*kick_prepare)(struct virtqueue *vq);
	void (*kick_notify)(struct virtqueue *vq);
	void *(*get_buf)(struct virtqueue *vq, unsigned int *len);
	void (*disable_cb)(struct virtqueue *vq);
	bool (*enable_cb)(struct virtqueue *vq);
	bool (*enable_cb_delayed)(struct virtqueue *vq);
	void *(*detach_unused_buf)(struct virtqueue *vq);
	unsigned int (*get_impl_size)(struct virtqueue *vq);
};

/* 
                                                 
                                                 
                                         
                                                    
                                                                     
                                           
                                                     
  
                                                                        
                                         
  
                                                           
 */
static inline int virtqueue_add_buf(struct virtqueue *vq,
				struct scatterlist sg[],
				unsigned int out_num,
				unsigned int in_num,
				void *data,
				gfp_t gfp)
{
	return vq->vq_ops->add_buf(vq, sg, out_num, in_num, data, gfp);
}
/* 
                                        
                            
  
                                                                 
                  
  
                                                             
                                                    
 */
static inline void virtqueue_kick(struct virtqueue *vq)
{
	vq->vq_ops->kick(vq);
}

/* 
                                                                    
                            
  
                                           
                                  
                              
  
                                                                      
                                                                          
 */
static inline bool virtqueue_kick_prepare(struct virtqueue *vq)
{
	return vq->vq_ops->kick_prepare(vq);
}

/* 
                                                                    
                            
 */
static inline void virtqueue_kick_notify(struct virtqueue *vq)
{
	vq->vq_ops->kick_notify(vq);
}

/* 
                                               
                                                 
                                           
  
                                                                    
                                                                 
                                                                    
          
  
                                                             
                                                    
  
                                                                 
                                 
 */
static inline void *virtqueue_get_buf(struct virtqueue *vq, unsigned int *len)
{
	return vq->vq_ops->get_buf(vq, len);
}

/* 
                                           
                                                 
  
                                                                           
                             
  
                                                        
 */
static inline void virtqueue_disable_cb(struct virtqueue *vq)
{
	vq->vq_ops->disable_cb(vq);
}

/* 
                                                            
                                                 
  
                                                                     
                                                                     
                                                  
  
                                                             
                                                    
 */
static inline bool virtqueue_enable_cb(struct virtqueue *vq)
{
	return vq->vq_ops->enable_cb(vq);
}

/* 
                                                                    
                                                 
  
                                                                 
                                                                      
                                                                     
                                                                       
                          
  
                                                             
                                                    
 */
static inline bool virtqueue_enable_cb_delayed(struct virtqueue *vq)
{
	return vq->vq_ops->enable_cb_delayed(vq);
}

/* 
                                                           
                                                 
  
                                                                  
                                                                     
            
 */
static inline void *virtqueue_detach_unused_buf(struct virtqueue *vq)
{
	return vq->vq_ops->detach_unused_buf(vq);
}

/* 
                                                                              
                                                                       
  
                                                                         
                                                                     
                 
 */
static inline unsigned int virtqueue_get_impl_size(struct virtqueue *vq)
{
	return vq->vq_ops->get_impl_size(vq);
}

/* 
                                                          
                                            
                           
                                                                        
                                                  
                                                
                                                               
                                               
 */
struct virtio_device {
	int index;
	struct device dev;
	struct virtio_device_id id;
	struct virtio_config_ops *config;
	struct list_head vqs;
	/*                                                 */
	unsigned long features[1];
	void *priv;
};

#define dev_to_virtio(dev) container_of(dev, struct virtio_device, dev)
int register_virtio_device(struct virtio_device *dev);
void unregister_virtio_device(struct virtio_device *dev);

/* 
                                                     
                                                               
                                              
                                                                        
                                                                     
                                                                             
                                                          
                                                                           
                                                  
 */
struct virtio_driver {
	struct device_driver driver;
	const struct virtio_device_id *id_table;
	const unsigned int *feature_table;
	unsigned int feature_table_size;
	int (*probe)(struct virtio_device *dev);
	void (*remove)(struct virtio_device *dev);
	void (*config_changed)(struct virtio_device *dev);
#ifdef CONFIG_PM
	int (*freeze)(struct virtio_device *dev);
	int (*restore)(struct virtio_device *dev);
#endif
};

int register_virtio_driver(struct virtio_driver *drv);
void unregister_virtio_driver(struct virtio_driver *drv);
#endif /*                 */
