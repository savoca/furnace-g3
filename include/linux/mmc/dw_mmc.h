/*
 * Synopsys DesignWare Multimedia Card Interface driver
 *  (Based on NXP driver for lpc 31xx)
 *
 * Copyright (C) 2009 NXP Semiconductors
 * Copyright (C) 2009, 2010 Imagination Technologies Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef LINUX_MMC_DW_MMC_H
#define LINUX_MMC_DW_MMC_H

#include <linux/scatterlist.h>

#define MAX_MCI_SLOTS	2

enum dw_mci_state {
	STATE_IDLE = 0,
	STATE_SENDING_CMD,
	STATE_SENDING_DATA,
	STATE_DATA_BUSY,
	STATE_SENDING_STOP,
	STATE_DATA_ERROR,
};

enum {
	EVENT_CMD_COMPLETE = 0,
	EVENT_XFER_COMPLETE,
	EVENT_DATA_COMPLETE,
	EVENT_DATA_ERROR,
	EVENT_XFER_ERROR
};

struct mmc_data;

/* 
                                                                
                                                            
                                    
                                                                        
                                               
                                                               
                                                            
                                     
                                                               
                                                                  
                           
                                                       
                                                              
                                      
                                          
                                                        
                                                                   
                                                          
                                                                    
                                                        
                               
                                                                    
              
                                              
                                                       
                                               
                                                                      
                                  
                                                                   
             
                         
                                                              
                                                                    
                                 
                                                     
                                         
                             
                                                                    
                                                   
                                                            
                                            
                              
                                       
                                            
                                                      
                                                          
                                             
                                             
                                                                    
                                                     
                                                   
  
          
          
  
                                                                
                                                           
                                        
  
                                                                   
                                                                      
          
  
                                                                      
                                              
  
                                                               
                                                                      
                                                                  
                                                              
                                                                   
                                                                   
                                                                 
                                                                  
                  
 */
struct dw_mci {
	spinlock_t		lock;
	void __iomem		*regs;

	struct scatterlist	*sg;
	struct sg_mapping_iter	sg_miter;

	struct dw_mci_slot	*cur_slot;
	struct mmc_request	*mrq;
	struct mmc_command	*cmd;
	struct mmc_data		*data;

	/*                      */
	int			use_dma;
	int			using_dma;

	dma_addr_t		sg_dma;
	void			*sg_cpu;
	struct dw_mci_dma_ops	*dma_ops;
#ifdef CONFIG_MMC_DW_IDMAC
	unsigned int		ring_size;
#else
	struct dw_mci_dma_data	*dma_data;
#endif
	u32			cmd_status;
	u32			data_status;
	u32			stop_cmdr;
	u32			dir_status;
	struct tasklet_struct	tasklet;
	struct work_struct	card_work;
	unsigned long		pending_events;
	unsigned long		completed_events;
	enum dw_mci_state	state;
	struct list_head	queue;

	u32			bus_hz;
	u32			current_speed;
	u32			num_slots;
	u32			fifoth_val;
	u16			verid;
	u16			data_offset;
	struct device		dev;
	struct dw_mci_board	*pdata;
	struct dw_mci_slot	*slot[MAX_MCI_SLOTS];

	/*                    */
	int			fifo_depth;
	int			data_shift;
	u8			part_buf_start;
	u8			part_buf_count;
	union {
		u16		part_buf16;
		u32		part_buf32;
		u64		part_buf;
	};
	void (*push_data)(struct dw_mci *host, void *buf, int cnt);
	void (*pull_data)(struct dw_mci *host, void *buf, int cnt);

	/*                  */
	u32			quirks;

	struct regulator	*vmmc;	/*                 */
	unsigned long		irq_flags; /*           */
	unsigned int		irq;
};

/*                                              */
struct dw_mci_dma_ops {
	/*         */
	int (*init)(struct dw_mci *host);
	void (*start)(struct dw_mci *host, unsigned int sg_len);
	void (*complete)(struct dw_mci *host);
	void (*stop)(struct dw_mci *host);
	void (*cleanup)(struct dw_mci *host);
	void (*exit)(struct dw_mci *host);
};

/*                  */
/*                                                        */
#define DW_MCI_QUIRK_IDMAC_DTO			BIT(0)
/*                                                            */
#define DW_MCI_QUIRK_RETRY_DELAY		BIT(1)
/*                                                      */
#define DW_MCI_QUIRK_HIGHSPEED			BIT(2)
/*                           */
#define DW_MCI_QUIRK_BROKEN_CARD_DETECTION	BIT(3)


struct dma_pdata;

struct block_settings {
	unsigned short	max_segs;	/*                            */
	unsigned int	max_blk_size;	/*                               */
	unsigned int	max_blk_count;	/*                                    */
	unsigned int	max_req_size;	/*                                   */
	unsigned int	max_seg_size;	/*                                */
};

/*                     */
struct dw_mci_board {
	u32 num_slots;

	u32 quirks; /*                          */
	unsigned int bus_hz; /*           */

	unsigned int caps;	/*              */
	unsigned int caps2;	/*                   */
	/*
                                                                      
                                                                      
       
  */
	unsigned int fifo_depth;

	/*                                                    */
	u32 detect_delay_ms;

	int (*init)(u32 slot_id, irq_handler_t , void *);
	int (*get_ro)(u32 slot_id);
	int (*get_cd)(u32 slot_id);
	int (*get_ocr)(u32 slot_id);
	int (*get_bus_wd)(u32 slot_id);
	/*
                                                                   
                                                                  
                             
  */
	void (*setpower)(u32 slot_id, u32 volt);
	void (*exit)(u32 slot_id);
	void (*select_slot)(u32 slot_id);

	struct dw_mci_dma_ops *dma_ops;
	struct dma_pdata *data;
	struct block_settings *blk_settings;
};

#endif /*                    */
