/****************************************************************************
 * Driver for Solarflare Solarstorm network controllers and boards
 * Copyright 2010-2012 Solarflare Communications Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, incorporated herein by reference.
 */
#ifndef _VFDI_H
#define _VFDI_H

/* 
                                         
  
                                                                
                                                                 
                                                  
  
                                                                    
                                                                      
                                 
  
                                                                    
                                                            
                                                                  
                                                                     
                                                                     
                                                        
                     
  
                                                                     
                                                                   
                                                                     
                                                          
  
                                                           
  
                                                       
                                                        
  
                                                                    
                                                                    
                   
  
                                                                  
                                                         
  
                                                       
                                                        
                                                        
                                                        
                                                        
  
                                                                  
                                                                   
                                                                   
                                                                
  
                                                             
                                                                 
                                                        
  
                                                       
                                                        
  
                                                                  
                             
  
                                                       
                                                        
  
                                                            
                                  
 */
#define VFDI_EV_SEQ_LBN 24
#define VFDI_EV_SEQ_WIDTH 8
#define VFDI_EV_TYPE_LBN 16
#define VFDI_EV_TYPE_WIDTH 8
#define VFDI_EV_TYPE_REQ_WORD0 0
#define VFDI_EV_TYPE_REQ_WORD1 1
#define VFDI_EV_TYPE_REQ_WORD2 2
#define VFDI_EV_TYPE_REQ_WORD3 3
#define VFDI_EV_TYPE_STATUS 4
#define VFDI_EV_TYPE_RESET 5
#define VFDI_EV_DATA_LBN 0
#define VFDI_EV_DATA_WIDTH 16

struct vfdi_endpoint {
	u8 mac_addr[ETH_ALEN];
	__be16 tci;
};

/* 
                                            
                                                          
                                                                    
                                                                    
                                                                   
                                                                        
                             
                                                                        
                                                   
                                                                        
                                        
                                                                    
                   
 */
enum vfdi_op {
	VFDI_OP_RESPONSE = 0,
	VFDI_OP_INIT_EVQ = 1,
	VFDI_OP_INIT_RXQ = 2,
	VFDI_OP_INIT_TXQ = 3,
	VFDI_OP_FINI_ALL_QUEUES = 4,
	VFDI_OP_INSERT_FILTER = 5,
	VFDI_OP_REMOVE_ALL_FILTERS = 6,
	VFDI_OP_SET_STATUS_PAGE = 7,
	VFDI_OP_CLEAR_STATUS_PAGE = 8,
	VFDI_OP_LIMIT,
};

/*                                                                         */
#define VFDI_RC_SUCCESS		0
#define VFDI_RC_ENOMEM		(-12)
#define VFDI_RC_EINVAL		(-22)
#define VFDI_RC_EOPNOTSUPP	(-95)
#define VFDI_RC_ETIMEDOUT	(-110)

/* 
                                                        
                                                                       
                                                                                
                                                     
                                                                   
                                                                         
                                                
                                                       
                                                                     
                                                                        
                                                   
                             
                                                                         
                                                  
                                                        
                                                                      
                                                                         
             
                                                               
                                             
                                                                         
                                                   
                                                                           
                                     
                                         
                                                                         
                                                                 
                                                        
                                                                        
                                                      
                                                                      
                                                               
                                                             
                                                                
                        
 */
struct vfdi_req {
	u32 op;
	u32 reserved1;
	s32 rc;
	u32 reserved2;
	union {
		struct {
			u32 index;
			u32 buf_count;
			u64 addr[];
		} init_evq;
		struct {
			u32 index;
			u32 buf_count;
			u32 evq;
			u32 label;
			u32 flags;
#define VFDI_RXQ_FLAG_SCATTER_EN 1
			u32 reserved;
			u64 addr[];
		} init_rxq;
		struct {
			u32 index;
			u32 buf_count;
			u32 evq;
			u32 label;
			u32 flags;
#define VFDI_TXQ_FLAG_IP_CSUM_DIS 1
#define VFDI_TXQ_FLAG_TCPUDP_CSUM_DIS 2
			u32 reserved;
			u64 addr[];
		} init_txq;
		struct {
			u32 rxq;
			u32 flags;
#define VFDI_MAC_FILTER_FLAG_RSS 1
#define VFDI_MAC_FILTER_FLAG_SCATTER 2
		} mac_filter;
		struct {
			u64 dma_addr;
			u64 peer_page_count;
			u64 peer_page_addr[];
		} set_status_page;
	} u;
};

/* 
                                                                 
                                                                 
                         
                                                              
                         
                                                                  
                                                                   
                                  
                                                                    
                                                                        
                                                 
                                                                          
                                                                         
                     
                                                                          
                                                               
                                                                          
                                                          
                                                                        
                                                                  
                                                        
                                                                        
                                                                       
                                            
 */
struct vfdi_status {
	u32 generation_start;
	u32 generation_end;
	u32 version;
	u32 length;
	u8 vi_scale;
	u8 max_tx_channels;
	u8 rss_rxq_count;
	u8 reserved1;
	u16 peer_count;
	u16 reserved2;
	struct vfdi_endpoint local;
	struct vfdi_endpoint peers[256];

	/*                                                       */
	u32 timer_quantum_ns;
};

#endif
