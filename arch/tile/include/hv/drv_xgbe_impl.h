/*
 * Copyright 2010 Tilera Corporation. All Rights Reserved.
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 *   NON INFRINGEMENT.  See the GNU General Public License for
 *   more details.
 */

/* 
                            
                                                
 */

#ifndef __DRV_XGBE_IMPL_H__
#define __DRV_XGBE_IMPL_H__

#include <hv/netio_errors.h>
#include <hv/netio_intf.h>
#include <hv/drv_xgbe_intf.h>


/*                                  */
#define LOG2_NUM_GROUPS (12)
/*                           */
#define NUM_GROUPS (1 << LOG2_NUM_GROUPS)

/*                                                   */
#define EPP_REQS_PER_TILE (32)

/*                                                               */
#define EDMA_WDS_NO_CSUM      8
/*                                                            */
#define EDMA_WDS_CSUM        10
/*                                                  */
#define EDMA_WDS_TOTAL      128


/*
                                                                     
                                                                       
                                                                     
                                                             
                                                                          
                 
 */
#define SIZE_SMALL (1)       /*                       */
#define SIZE_LARGE (2)       /*                       */
#define SIZE_JUMBO (0)       /*                       */

/*                                   */
#define NETIO_NUM_SIZES 3


/*
                                                                       
                                                  
 */

/*                                       */
#define NETIO_DEFAULT_SMALL_PACKETS 2750
/*                                       */
#define NETIO_DEFAULT_LARGE_PACKETS 2500
/*                                       */
#define NETIO_DEFAULT_JUMBO_PACKETS 250


/*                                      */
#define NETIO_ARENA_SHIFT      24      /*       */
/*                          */
#define NETIO_ARENA_SIZE       (1 << NETIO_ARENA_SHIFT)


/*                     
  
                                                                    
                                                                             
                                                                           
                                                                             
                                                                           
             
  
                                                                       
                                                                           
                                                                          
                                                              
  
                                                                      
                               
  
        
                                          
                                              
                                              
                                              
                                                                   
                                           
                                  
           
  
                                                                              
                                                                         
                                                                          
                                               
 */
typedef struct {
  /*                                                                         
                                                                            
                  */
  volatile uint32_t __packet_write;

  /*                                                                  
                                                                    */
  uint32_t __last_packet_plus_one;
}
__netio_packet_queue_t;


/*                     
  
                                                                            
                                                                           
                                                                            
                                                                              
                                                                           
                                                                              
                                                                              
                                         
  
                                                                          
                                                                             
                                                                              
                                             
  
                                                                           
                                                                             
                                                                            
                                                                           
                                                                              
                                                      
  
                                                                       
                                                                            
                                                                           
                                                                  
                                                                          
                                                                              
                                                                     
                                                                              
                         
  
        
                                                                            
                                   
  
                                          
                                          
                                          
                                          
                                                      
                                           
                                             
                          
           
  
                                                                           
                                                                           
                                                                            
                                                                              
                                                                            
                                               
 */
typedef struct
{
  /*                                                                          
                                                          */
  volatile uint32_t __buffer_write;

  /*                                                                          
                                                   */
  uint32_t __last_buffer;
}
__netio_buffer_queue_t;


/* 
                                                         
 */
typedef struct __netio_queue_impl_t
{
  /*                                               */
  __netio_packet_queue_t __packet_receive_queue;
  /*                                          */
  unsigned int __intr_id;
  /*                                                   */
  uint32_t __buffer_queue[NETIO_NUM_SIZES];
  /*                                                      */
  /*                                             */
  uint32_t __epp_location;
  /*                                           */
  unsigned int __queue_id;
  /*                                       */
  volatile uint32_t __acks_received;
  /*                                                    */
  volatile uint32_t __last_completion_rcv;
  /*                                               */
  uint32_t __max_outstanding;
  /*                                  */
  void* __va_0;
  /*                                                  */
  void* __va_1;
  /*                                                                       */
  uint32_t __padding[3];
  /*                          */
  netio_pkt_t __packets[0];
}
netio_queue_impl_t;


/* 
                                                        
 */
typedef struct __netio_queue_user_impl_t
{
  /*                                       */
  uint32_t __packet_receive_read;
  /*                                                         */
  uint8_t __buffer_read[NETIO_NUM_SIZES];
  /*                                                                        
               */
  uint8_t __buffer_requested_write[NETIO_NUM_SIZES];
  /*                       */
  uint8_t __pcie;
  /*                                                                        */
  uint32_t __receive_credit_remaining;
  /*                                                                   */
  uint32_t __receive_credit_interval;
  /*                                */
  uint32_t __fastio_index;
  /*                                       */
  uint32_t __acks_outstanding;
  /*                                    */
  uint32_t __last_completion_req;
  /*                              */
  int __fd;
}
netio_queue_user_impl_t;


#define NETIO_GROUP_CHUNK_SIZE   64   /*                                   */
#define NETIO_BUCKET_CHUNK_SIZE  64   /*                                    */


/*                                                                  
                                                                    
                                                
 */
typedef struct
{
  uint16_t flags;              /*                                       */
  uint16_t transfer_size;      /*                  */
  uint32_t va;                 /*                         */
  __netio_pkt_handle_t handle; /*                 */
  uint32_t csum0;              /*                       */
  uint32_t csum1;              /*                        */
}
__netio_send_cmd_t;


/*                             
                                                                       
                                          
                                                                            
 */

/*                                                                  
                                                                      */
#define __NETIO_SEND_FLG_ACK    0x1

/*                                                                      
                                                                            
                                                           */
#define __NETIO_SEND_FLG_CSUM   0x2

/*                                                                         */
#define __NETIO_SEND_FLG_COMPLETION 0x4

/*                                                                   
                                        */
#define __NETIO_SEND_FLG_XSEG_SHIFT 3

/*                                                                 */
#define __NETIO_SEND_FLG_XSEG_WIDTH 2

#endif /*                     */
