/*
                                                                    
                                                                 
                                                              
                                    
  
 */
#ifdef __KERNEL__
#ifndef __ASM_UNINORTH_H__
#define __ASM_UNINORTH_H__

/*
                                             
  
                  
 */

/*                                                                */
/*        */
#define UNI_N_ADDR_SELECT		0x48
#define UNI_N_ADDR_COARSE_MASK		0xffff0000	/*                           */
#define UNI_N_ADDR_FINE_MASK		0x0000ffff	/*                           */

/*               */
/*        */
#define UNI_N_CFG_GART_BASE		0x8c
#define UNI_N_CFG_AGP_BASE		0x90
#define UNI_N_CFG_GART_CTRL		0x94
#define UNI_N_CFG_INTERNAL_STATUS	0x98
#define UNI_N_CFG_GART_DUMMY_PAGE	0xa4

/*                                      */
#define UNI_N_CFG_GART_INVAL		0x00000001
#define UNI_N_CFG_GART_ENABLE		0x00000100
#define UNI_N_CFG_GART_2xRESET		0x00010000
#define UNI_N_CFG_GART_DISSBADET	0x00020000
/*                                                                     */
#define U3_N_CFG_GART_SYNCMODE		0x00040000
#define U3_N_CFG_GART_PERFRD		0x00080000
#define U3_N_CFG_GART_B2BGNT		0x00200000
#define U3_N_CFG_GART_FASTDDR		0x00400000

/*                                                          
                                                  
  
                                                                     
                                                                            
                                                                          
                                                                     
                                  
  
                                                                        
                                                                    
                                                         
  
                                                                      
                                                                          
                                                                       
                                                                    
  
                                                                    
                                                                   
                                                                          
  
                                                                        
                                                                       
                                       
  
                                                 
                          
                                                   
                          
  
                                                                       
                                                 
  
                                                                           
                                              
  
                                                                      
                                                                       
                                                 
  
                                                 
      
                           
      
 */

/*
                                       
  
                                    
  
                                                                            
                                                          
  
 */

/*                              */
#define UNI_N_VERSION			0x0000		/*                           */

#define UNI_N_VERSION_107		0x0003		/*       */
#define UNI_N_VERSION_10A		0x0007		/*        */
#define UNI_N_VERSION_150		0x0011		/*     */
#define UNI_N_VERSION_200		0x0024		/*     */
#define UNI_N_VERSION_PANGEA		0x00C0		/*                   */
#define UNI_N_VERSION_INTREPID		0x00D2		/*                   */
#define UNI_N_VERSION_300		0x0030		/*                */

/*                                                        */
#define UNI_N_CLOCK_CNTL		0x0020
#define UNI_N_CLOCK_CNTL_PCI		0x00000001	/*                    */
#define UNI_N_CLOCK_CNTL_GMAC		0x00000002	/*                    */
#define UNI_N_CLOCK_CNTL_FW		0x00000004	/*                        */
#define UNI_N_CLOCK_CNTL_ATA100		0x00000010	/*                            */

/*                          */
#define UNI_N_POWER_MGT			0x0030
#define UNI_N_POWER_MGT_NORMAL		0x00
#define UNI_N_POWER_MGT_IDLE2		0x01
#define UNI_N_POWER_MGT_SLEEP		0x02

/*                                                            
           
 */
#define UNI_N_ARB_CTRL			0x0040
#define UNI_N_ARB_CTRL_QACK_DELAY_SHIFT	15
#define UNI_N_ARB_CTRL_QACK_DELAY_MASK	0x0e1f8000
#define UNI_N_ARB_CTRL_QACK_DELAY	0x30
#define UNI_N_ARB_CTRL_QACK_DELAY105	0x00

/*                                                              
                                                                     
                                   
 */
#define UNI_N_CPU_NUMBER		0x0050

/*                                                              
                                                        
 */
#define UNI_N_HWINIT_STATE		0x0070
#define UNI_N_HWINIT_STATE_SLEEPING	0x01
#define UNI_N_HWINIT_STATE_RUNNING	0x02
/*                                                                  
                                                           
 */
#define UNI_N_HWINIT_STATE_CPU1_FLAG	0x10000000

/*                                                                          
                        
 */
#define UNI_N_AACK_DELAY		0x0100
#define UNI_N_AACK_DELAY_ENABLE		0x00000001

/*                           */
#define UNI_N_CLOCK_STOP_STATUS0	0x0150
#define UNI_N_CLOCK_STOPPED_EXTAGP	0x00200000
#define UNI_N_CLOCK_STOPPED_AGPDEL	0x00100000
#define UNI_N_CLOCK_STOPPED_I2S0_45_49	0x00080000
#define UNI_N_CLOCK_STOPPED_I2S0_18	0x00040000
#define UNI_N_CLOCK_STOPPED_I2S1_45_49	0x00020000
#define UNI_N_CLOCK_STOPPED_I2S1_18	0x00010000
#define UNI_N_CLOCK_STOPPED_TIMER	0x00008000
#define UNI_N_CLOCK_STOPPED_SCC_RTCLK18	0x00004000
#define UNI_N_CLOCK_STOPPED_SCC_RTCLK32	0x00002000
#define UNI_N_CLOCK_STOPPED_SCC_VIA32	0x00001000
#define UNI_N_CLOCK_STOPPED_SCC_SLOT0	0x00000800
#define UNI_N_CLOCK_STOPPED_SCC_SLOT1	0x00000400
#define UNI_N_CLOCK_STOPPED_SCC_SLOT2	0x00000200
#define UNI_N_CLOCK_STOPPED_PCI_FBCLKO	0x00000100
#define UNI_N_CLOCK_STOPPED_VEO0	0x00000080
#define UNI_N_CLOCK_STOPPED_VEO1	0x00000040
#define UNI_N_CLOCK_STOPPED_USB0	0x00000020
#define UNI_N_CLOCK_STOPPED_USB1	0x00000010
#define UNI_N_CLOCK_STOPPED_USB2	0x00000008
#define UNI_N_CLOCK_STOPPED_32		0x00000004
#define UNI_N_CLOCK_STOPPED_45		0x00000002
#define UNI_N_CLOCK_STOPPED_49		0x00000001

#define UNI_N_CLOCK_STOP_STATUS1	0x0160
#define UNI_N_CLOCK_STOPPED_PLL4REF	0x00080000
#define UNI_N_CLOCK_STOPPED_CPUDEL	0x00040000
#define UNI_N_CLOCK_STOPPED_CPU		0x00020000
#define UNI_N_CLOCK_STOPPED_BUF_REFCKO	0x00010000
#define UNI_N_CLOCK_STOPPED_PCI2	0x00008000
#define UNI_N_CLOCK_STOPPED_FW		0x00004000
#define UNI_N_CLOCK_STOPPED_GB		0x00002000
#define UNI_N_CLOCK_STOPPED_ATA66	0x00001000
#define UNI_N_CLOCK_STOPPED_ATA100	0x00000800
#define UNI_N_CLOCK_STOPPED_MAX		0x00000400
#define UNI_N_CLOCK_STOPPED_PCI1	0x00000200
#define UNI_N_CLOCK_STOPPED_KLPCI	0x00000100
#define UNI_N_CLOCK_STOPPED_USB0PCI	0x00000080
#define UNI_N_CLOCK_STOPPED_USB1PCI	0x00000040
#define UNI_N_CLOCK_STOPPED_USB2PCI	0x00000020
#define UNI_N_CLOCK_STOPPED_7PCI1	0x00000008
#define UNI_N_CLOCK_STOPPED_AGP		0x00000004
#define UNI_N_CLOCK_STOPPED_PCI0	0x00000002
#define UNI_N_CLOCK_STOPPED_18		0x00000001

/*                                                   */
#define UNI_N_CLOCK_SPREADING		0x190

/*                                                                         */


/*
                        
 */


/*           */
#define U3_TOGGLE_REG			0x00e0
#define U3_PMC_START_STOP		0x0001
#define U3_MPIC_RESET			0x0002
#define U3_MPIC_OUTPUT_ENABLE		0x0004

/*                     */
#define U3_API_PHY_CONFIG_1		0x23030

/*                             */
#define U3_HT_CONFIG_BASE      		0x70000
#define U3_HT_LINK_COMMAND		0x100
#define U3_HT_LINK_CONFIG		0x110
#define U3_HT_LINK_FREQ			0x120

#endif /*                    */
#endif /*            */
