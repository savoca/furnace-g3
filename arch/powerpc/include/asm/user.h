#ifndef _ASM_POWERPC_USER_H
#define _ASM_POWERPC_USER_H

#include <asm/ptrace.h>
#include <asm/page.h>

/*
                                  
  
                                                                    
                                                                      
                                                                          
                  
  
                                                            
                                                         
                                                               
                                                                
                                                               
                      
                                                                     
                                                                     
                                                                    
                                                                    
                                                                    
                                               
                                                                     
                                                    
                                                                      
                                       
 */
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
#endif	/*                     */
