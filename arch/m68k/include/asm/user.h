#ifndef _M68K_USER_H
#define _M68K_USER_H

/*                                                                  
                                                                      
                                                                  
                                                                     
                                                                         
                                                                         
                                                                      
                     
                                           
                                                                           
                                                                        
                                                                         
                                                                          
                           
                                                              
                                                                     
                                                                          
                                                                          
                                                                          
                              
                                                                    
                                                      
                                                                          
                                       
                                                         
*/

struct user_m68kfp_struct {
	unsigned long  fpregs[8*3];	/*                   */
	unsigned long  fpcntl[3];	/*                 */
};

/*                                                                
                                                                 
                   */
struct user_regs_struct {
	long d1,d2,d3,d4,d5,d6,d7;
	long a0,a1,a2,a3,a4,a5,a6;
	long d0;
	long usp;
	long orig_d0;
	short stkadj;
	short sr;
	long pc;
	short fmtvec;
	short __fill;
};


/*                                                                   
                                                                           
                                                           */
struct user{
/*                                                                        
                                     */
  struct user_regs_struct regs;	/*                                         */
/*                                                */
  int u_fpvalid;		/*                                       */
                                /*                              */
  struct user_m68kfp_struct m68kfp; /*                              */
/*                                                                 */
  unsigned long int u_tsize;	/*                            */
  unsigned long int u_dsize;	/*                            */
  unsigned long int u_ssize;	/*                             */
  unsigned long start_code;     /*                                   */
  unsigned long start_stack;	/*                                        
                                                
                                                  
                      */
  long int signal;		/*                                   */
  int reserved;			/*                */
  unsigned long u_ar0;		/*                                         */
				/*                */
  struct user_m68kfp_struct* u_fpstate;	/*                            */
  unsigned long magic;		/*                                  */
  char u_comm[32];		/*                                   */
};
#define NBPG 4096
#define UPAGES 1
#define HOST_TEXT_START_ADDR (u.start_code)
#define HOST_STACK_END_ADDR (u.start_stack + u.u_ssize * NBPG)

#endif
