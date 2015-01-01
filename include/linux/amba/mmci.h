/*
                             
 */
#ifndef AMBA_MMCI_H
#define AMBA_MMCI_H

#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
#include <linux/mmc/sdio_func.h>

struct embedded_sdio_data {
        struct sdio_cis cis;
        struct sdio_cccr cccr;
        struct sdio_embedded_func *funcs;
        int num_funcs;
};


/*
                                                                    
                                                                  
                                                      
 */
#define MCI_ST_DATA2DIREN	(1 << 2)
#define MCI_ST_CMDDIREN		(1 << 3)
#define MCI_ST_DATA0DIREN	(1 << 4)
#define MCI_ST_DATA31DIREN	(1 << 5)
#define MCI_ST_FBCLKEN		(1 << 7)
#define MCI_ST_DATA74DIREN	(1 << 8)

/*                            */
struct dma_chan;

/* 
                                                                  
                               
                                                                  
                                                                     
                                                    
                                                                   
                                                                
             
                                                                   
                                             
                                                                
                                                                 
                                                                   
                                                              
                                                            
                                                   
                                                                     
                                                        
                                                          
                                                                 
                                                            
                                                                
                                                                        
                                             
                                                                
                                                                      
                     
                                                        
                                                          
                                                            
                                          
                                                        
                                                               
                                                             
                        
 */
struct mmci_platform_data {
	unsigned int f_max;
	unsigned int ocr_mask;
	int (*ios_handler)(struct device *, struct mmc_ios *);
	unsigned int (*status)(struct device *);
	int	gpio_wp;
	int	gpio_cd;
	bool	cd_invert;
	unsigned long capabilities;
	unsigned long capabilities2;
	u32 sigdir;
	bool (*dma_filter)(struct dma_chan *chan, void *filter_param);
	void *dma_rx_param;
	void *dma_tx_param;
	unsigned int status_irq;
	struct embedded_sdio_data *embedded_sdio;
	int (*register_status_notify)(void (*callback)(int card_present, void *dev_id), void *dev_id);
};

#endif
