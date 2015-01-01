/*
 * Memory barrier definitions.  This is based on information published
 * in the Processor Abstraction Layer and the System Abstraction Layer
 * manual.
 *
 * Copyright (C) 1998-2003 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 * Copyright (C) 1999 Asit Mallick <asit.k.mallick@intel.com>
 * Copyright (C) 1999 Don Dugger <don.dugger@intel.com>
 */
#ifndef _ASM_IA64_BARRIER_H
#define _ASM_IA64_BARRIER_H

#include <linux/compiler.h>

/*
                                                                      
                                                                    
                                                                   
                                                                  
  
                                                           
                                                  
                                                
                                           
                                      
                                                       
                                              
                                                   
              
  
                                                                   
                                                                    
                                                                  
                                                                      
                                
 */
#define mb()	ia64_mf()
#define rmb()	mb()
#define wmb()	mb()
#define read_barrier_depends()	do { } while(0)

#ifdef CONFIG_SMP
# define smp_mb()	mb()
# define smp_rmb()	rmb()
# define smp_wmb()	wmb()
# define smp_read_barrier_depends()	read_barrier_depends()
#else
# define smp_mb()	barrier()
# define smp_rmb()	barrier()
# define smp_wmb()	barrier()
# define smp_read_barrier_depends()	do { } while(0)
#endif

/*
                                                                 
                                                                    
                           
 */
#define set_mb(var, value)	do { (var) = (value); mb(); } while (0)

/*
                                                                      
                                                               
                           
 */

#endif /*                     */
