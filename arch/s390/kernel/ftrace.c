/*
 * Dynamic function tracer architecture backend.
 *
 * Copyright IBM Corp. 2009
 *
 *   Author(s): Heiko Carstens <heiko.carstens@de.ibm.com>,
 *		Martin Schwidefsky <schwidefsky@de.ibm.com>
 */

#include <linux/hardirq.h>
#include <linux/uaccess.h>
#include <linux/ftrace.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kprobes.h>
#include <trace/syscall.h>
#include <asm/asm-offsets.h>

#ifdef CONFIG_64BIT
#define MCOUNT_OFFSET_RET 12
#else
#define MCOUNT_OFFSET_RET 22
#endif

#ifdef CONFIG_DYNAMIC_FTRACE

void ftrace_disable_code(void);
void ftrace_enable_insn(void);

#ifdef CONFIG_64BIT
/*
                                          
                               
                                    
                                    
                               
                                                                      
                                                               
                                                        
                               
                                       
                              
                               
                              
                                                                           
                                                        
                               
                         
                              
                               
                              
                                                                        
               
 */
asm(
	"	.align	4\n"
	"ftrace_disable_code:\n"
	"	jg	0f\n"
	"	lgr	%r0,%r0\n"
	"	basr	%r14,%r1\n"
	"0:\n"
	"	.align	4\n"
	"ftrace_enable_insn:\n"
	"	lg	%r1,"__stringify(__LC_FTRACE_FUNC)"\n");

#define FTRACE_INSN_SIZE	6

#else /*              */
/*
                                          
                              
                             
                               
                                  
                                  
                              
                              
                              
                                                                    
                                                                      
                                                        
                              
                                       
                      
                                 
                                   
                              
                                                                           
                                                         
                              
                        
                      
                                 
                                   
                              
                                                                       
               
 */
asm(
	"	.align	4\n"
	"ftrace_disable_code:\n"
	"	j	1f\n"
	"	j	0f\n"
	"	.fill	12,1,0x07\n"
	"0:	basr	%r14,%r14\n"
	"1:\n"
	"	.align	4\n"
	"ftrace_enable_insn:\n"
	"	l	%r14,"__stringify(__LC_FTRACE_FUNC)"\n");

#define FTRACE_INSN_SIZE	4

#endif /*              */


int ftrace_make_nop(struct module *mod, struct dyn_ftrace *rec,
		    unsigned long addr)
{
	if (probe_kernel_write((void *) rec->ip, ftrace_disable_code,
			       MCOUNT_INSN_SIZE))
		return -EPERM;
	return 0;
}

int ftrace_make_call(struct dyn_ftrace *rec, unsigned long addr)
{
	if (probe_kernel_write((void *) rec->ip, ftrace_enable_insn,
			       FTRACE_INSN_SIZE))
		return -EPERM;
	return 0;
}

int ftrace_update_ftrace_func(ftrace_func_t func)
{
	return 0;
}

int __init ftrace_dyn_arch_init(void *data)
{
	*(unsigned long *) data = 0;
	return 0;
}

#endif /*                       */

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
/*
                                                                       
                          
 */
unsigned long __kprobes prepare_ftrace_return(unsigned long parent,
					      unsigned long ip)
{
	struct ftrace_graph_ent trace;

	if (unlikely(atomic_read(&current->tracing_graph_pause)))
		goto out;
	if (ftrace_push_return_trace(parent, ip, &trace.depth, 0) == -EBUSY)
		goto out;
	trace.func = (ip & PSW_ADDR_INSN) - MCOUNT_OFFSET_RET;
	/*                                                */
	if (!ftrace_graph_entry(&trace)) {
		current->curr_ret_stack--;
		goto out;
	}
	parent = (unsigned long) return_to_handler;
out:
	return parent;
}

#ifdef CONFIG_DYNAMIC_FTRACE
/*
                                                                         
                                                                         
                                                                      
                                                                   
                                                                
 */
int ftrace_enable_ftrace_graph_caller(void)
{
	unsigned short offset;

	offset = ((void *) prepare_ftrace_return -
		  (void *) ftrace_graph_caller) / 2;
	return probe_kernel_write(ftrace_graph_caller + 2,
				  &offset, sizeof(offset));
}

int ftrace_disable_ftrace_graph_caller(void)
{
	static unsigned short offset = 0x0002;

	return probe_kernel_write(ftrace_graph_caller + 2,
				  &offset, sizeof(offset));
}

#endif /*                       */
#endif /*                              */
