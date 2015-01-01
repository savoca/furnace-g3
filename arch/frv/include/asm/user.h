/* user.h: FR-V core file format stuff
 *
 * Copyright (C) 2003 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#ifndef _ASM_USER_H
#define _ASM_USER_H

#include <asm/page.h>
#include <asm/registers.h>

/*                                                                  
                                                                      
                                                                  
                                                                     
                                                                     
                                                                     
                                                               
                                    
  
                                           
         
                                                                      
                                                                    
                                                                     
                                                                
                                                      
  
        
                                                          
                                                                
                                                               
                                                               
                                                                      
                                                              
             
  
         
                                                               
                                                        
                                                                    
                                                                   
                                          
 */

/*                                                                   
                                                                           
                                                           
 */
struct user {
	/*                                                                        
                                      */
	struct user_context	regs;

	/*                                                                 */
	unsigned long		u_tsize;	/*                            */
	unsigned long		u_dsize;	/*                            */
	unsigned long		u_ssize;	/*                             */
	unsigned long		start_code;     /*                                   */
	unsigned long		start_stack;	/*                                        
                                                  
                                                    
                        */
	long int		signal;		/*                                   */

	unsigned long		magic;		/*                                  */
	char			u_comm[32];	/*                                   */
};

#define NBPG			PAGE_SIZE
#define UPAGES			1
#define HOST_TEXT_START_ADDR	(u.start_code)
#define HOST_STACK_END_ADDR	(u.start_stack + u.u_ssize * NBPG)

#endif
