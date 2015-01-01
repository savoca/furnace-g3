#ifndef _ASM_X86_USER_64_H
#define _ASM_X86_USER_64_H

#include <asm/types.h>
#include <asm/page.h>
/*                                                                  
                                                                
                                                                  
                                                                
                                                            
                                                                      
                                                                
           

                                           
                                                                           
                                                                        
                                                                         
                                                                          
                           
                                                              
                                                                     
                                                                          
                                                                          
                                                                          
                              
                                                                    
                                                      
                                                                          
                                       
                                                           */

/*
                                
                                               
  
                                                                        
                                                                         
                                                                        
                                                                         
                                                                           
                                                     
  
                                
 */

/*                                                                       
                                                                              
                                                           */
struct user_i387_struct {
	unsigned short	cwd;
	unsigned short	swd;
	unsigned short	twd;	/*                             
                               */
	unsigned short	fop;
	__u64	rip;
	__u64	rdp;
	__u32	mxcsr;
	__u32	mxcsr_mask;
	__u32	st_space[32];	/*                                        */
	__u32	xmm_space[64];	/*                                          */
	__u32	padding[24];
};

/*
                                        
 */
struct user_regs_struct {
	unsigned long	r15;
	unsigned long	r14;
	unsigned long	r13;
	unsigned long	r12;
	unsigned long	bp;
	unsigned long	bx;
	unsigned long	r11;
	unsigned long	r10;
	unsigned long	r9;
	unsigned long	r8;
	unsigned long	ax;
	unsigned long	cx;
	unsigned long	dx;
	unsigned long	si;
	unsigned long	di;
	unsigned long	orig_ax;
	unsigned long	ip;
	unsigned long	cs;
	unsigned long	flags;
	unsigned long	sp;
	unsigned long	ss;
	unsigned long	fs_base;
	unsigned long	gs_base;
	unsigned long	ds;
	unsigned long	es;
	unsigned long	fs;
	unsigned long	gs;
};

/*                                                                   
                                                                           
                                                           */

struct user {
/*                                                                        
                                     */
  struct user_regs_struct regs;	/*                                         */
/*                                                */
  int u_fpvalid;		/*                                       */
				/*                              */
  int pad0;
  struct user_i387_struct i387;	/*                              */
/*                                                                 */
  unsigned long int u_tsize;	/*                            */
  unsigned long int u_dsize;	/*                            */
  unsigned long int u_ssize;	/*                             */
  unsigned long start_code;     /*                                   */
  unsigned long start_stack;	/*                                        
                                                
                                                  
                      */
  long int signal;		/*                                   */
  int reserved;			/*                */
  int pad1;
  unsigned long u_ar0;		/*                                         */
				/*                */
  struct user_i387_struct *u_fpstate;	/*                            */
  unsigned long magic;		/*                                  */
  char u_comm[32];		/*                                   */
  unsigned long u_debugreg[8];
  unsigned long error_code; /*                     */
  unsigned long fault_address; /*          */
};
#define NBPG PAGE_SIZE
#define UPAGES 1
#define HOST_TEXT_START_ADDR (u.start_code)
#define HOST_STACK_END_ADDR (u.start_stack + u.u_ssize * NBPG)

#endif /*                    */
