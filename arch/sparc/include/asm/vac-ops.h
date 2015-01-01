#ifndef _SPARC_VAC_OPS_H
#define _SPARC_VAC_OPS_H

/* vac-ops.h: Inline assembly routines to do operations on the Sparc
 *            VAC (virtual address cache) for the sun4c.
 *
 * Copyright (C) 1994, David S. Miller (davem@caip.rutgers.edu)
 */

#include <asm/sysen.h>
#include <asm/contregs.h>
#include <asm/asi.h>

/*                                                          
         
  
                                                            
                      
  
                                                               
                                                               
                                                               
                                                              
  
                                                               
                             
  
                                                                 
  
                                                                  
  
                                                               
  
                                                   
  
                                  
  
                                                        
  
                                           
                                           
                                           
                                          
  
                         
  
                                                    
  
                                    
  
                                                  
 */

/*                */
#define S4CVACTAG_CID      0x01c00000
#define S4CVACTAG_W        0x00200000
#define S4CVACTAG_P        0x00100000
#define S4CVACTAG_V        0x00080000
#define S4CVACTAG_TID      0x0000fffc

/*                           */
/*                                        */
#if 0
#define S4CVACVA_TID       0x3fff0000
#define S4CVACVA_LINE      0x0000fff0
#define S4CVACVA_BIL       0x0000000f
#endif

/*                                                                 
                                                                 
                                                             
                                                                    
                                                               
                                                                   
                                                               
                                                
 */

#define S4CVAC_BADBITS    0x0000f000

/*                                                       
                 
 */
#define S4CVAC_BADALIAS(vaddr1, vaddr2) \
        ((((unsigned long) (vaddr1)) ^ ((unsigned long) (vaddr2))) & \
	 (S4CVAC_BADBITS))

/*                                                                 
                                                
 */
struct sun4c_vac_props {
	unsigned int num_bytes;     /*                   */
	unsigned int do_hwflushes;  /*                              */
	unsigned int linesize;      /*                            */
	unsigned int log2lsize;     /*                */
	unsigned int on;            /*                */
};

extern struct sun4c_vac_props sun4c_vacinfo;

/*                                                             */
static inline void sun4c_enable_vac(void)
{
	__asm__ __volatile__("lduba [%0] %1, %%g1\n\t"
			     "or    %%g1, %2, %%g1\n\t"
			     "stba  %%g1, [%0] %1\n\t"
			     : /*            */
			     : "r" ((unsigned int) AC_SENABLE),
			     "i" (ASI_CONTROL), "i" (SENABLE_CACHE)
			     : "g1", "memory");
	sun4c_vacinfo.on = 1;
}

/*                                                         */
static inline void sun4c_disable_vac(void)
{
	__asm__ __volatile__("lduba [%0] %1, %%g1\n\t"
			     "andn  %%g1, %2, %%g1\n\t"
			     "stba  %%g1, [%0] %1\n\t"
			     : /*            */
			     : "r" ((unsigned int) AC_SENABLE),
			     "i" (ASI_CONTROL), "i" (SENABLE_CACHE)
			     : "g1", "memory");
	sun4c_vacinfo.on = 0;
}

#endif /*                     */
