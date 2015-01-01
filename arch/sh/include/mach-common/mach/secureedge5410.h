/*
 * include/asm-sh/snapgear.h
 *
 * Modified version of io_se.h for the snapgear-specific functions.
 *
 * May be copied or modified under the terms of the GNU General Public
 * License.  See linux/COPYING for more information.
 *
 * IO functions for a SnapGear
 */

#ifndef _ASM_SH_IO_SNAPGEAR_H
#define _ASM_SH_IO_SNAPGEAR_H

#define __IO_PREFIX	snapgear
#include <asm/io_generic.h>

/*
                                                                  
                                                                    
              
  
                                          
                                                  
                                          
                                             
                                        
                                        
                                            
                                           
                                        
                                                  
                                                      
                                                      
                                             
                                             
                                              
 */

#define SECUREEDGE_IOPORT_ADDR ((volatile short *) 0xb0000000)
extern unsigned short secureedge5410_ioport;

#define SECUREEDGE_WRITE_IOPORT(val, mask) (*SECUREEDGE_IOPORT_ADDR = \
	 (secureedge5410_ioport = \
			((secureedge5410_ioport & ~(mask)) | ((val) & (mask)))))
#define SECUREEDGE_READ_IOPORT() \
	 ((*SECUREEDGE_IOPORT_ADDR&0x0817) | (secureedge5410_ioport&~0x0817))

#endif /*                       */
