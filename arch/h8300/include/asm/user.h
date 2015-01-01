#ifndef _H8300_USER_H
#define _H8300_USER_H

#include <asm/page.h>

/*                                                                  
                                                                      
                                                                  
                                                                     
                                                                         
                                                                         
                                                                      
                     
                                           
                                                                           
                                                                        
                                                                         
                                                                          
                           
                                                              
                                                                     
                                                                          
                                                                          
                                                                          
                              
                                                                    
                                                      
                                                                          
                                       
                                                         
*/

/*                                                                
                                                                 
                   */
struct user_regs_struct {
	long er1,er2,er3,er4,er5,er6;
	long er0;
	long usp;
	long orig_er0;
	short ccr;
	long pc;
};

	
/*                                                                   
                                                                           
                                                           */
struct user{
/*                                                                        
                                     */
  struct user_regs_struct regs;	/*                                         */
/*                                                */
/*                                                                 */
  unsigned long int u_tsize;	/*                            */
  unsigned long int u_dsize;	/*                            */
  unsigned long int u_ssize;	/*                             */
  unsigned long start_code;     /*                                   */
  unsigned long start_stack;	/*                                        
                                                
                                                  
                      */
  long int signal;     		/*                                   */
  int reserved;			/*                */
  unsigned long u_ar0;		/*                                         */
				/*                */
  unsigned long magic;		/*                                  */
  char u_comm[32];		/*                                   */
};
#define NBPG PAGE_SIZE
#define UPAGES 1
#define HOST_TEXT_START_ADDR (u.start_code)
#define HOST_STACK_END_ADDR (u.start_stack + u.u_ssize * NBPG)

#endif
