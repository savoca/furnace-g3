/*
 * Copyright (C) 2004-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Note: We may not need these definitions for AVR32, as we don't
 * support a.out.
 */
#ifndef __ASM_AVR32_USER_H
#define __ASM_AVR32_USER_H

#include <linux/types.h>
#include <asm/ptrace.h>
#include <asm/page.h>

/*
                                                                    
                                                                      
                                                                        
  
                                                            
                                                         
                                                               
                                                                
                                                               
                      
                                                                     
                                                                     
                                                                    
                                                                    
                                                                    
                                               
                                                                     
                                                    
                                                                      
                                       
 */

struct user_fpu_struct {
	/*                      */
};

struct user {
	struct pt_regs	regs;			/*                      */
	size_t		u_tsize;		/*                   */
	size_t		u_dsize;		/*                   */
	size_t		u_ssize;		/*                    */
	unsigned long	start_code;		/*                       */
	unsigned long	start_data;		/*                       */
	unsigned long	start_stack;		/*                        */
	long int	signal;			/*                          */
	unsigned long	u_ar0;			/*                         */
	unsigned long	magic;			/*                        */
	char		u_comm[32];		/*                   */
};

#define NBPG			PAGE_SIZE
#define UPAGES			1
#define HOST_TEXT_START_ADDR	(u.start_code)
#define HOST_DATA_START_ADDR	(u.start_data)
#define HOST_STACK_END_ADDR	(u.start_stack + u.u_ssize * NBPG)

#endif /*                    */
