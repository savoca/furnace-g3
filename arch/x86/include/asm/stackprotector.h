/*
                               
  
                                                                      
                                                                     
                                                                   
                                                                      
                                                                    
                                                                     
                           
  
                                                                 
                                                                     
                                                         
                                                                      
                                                                  
                                                               
  
                                                                   
                                                                  
                                                                
                                                                     
                                                                
                                         
  
                                                                   
                                                                
                                                                    
                                                                   
                                
 */

#ifndef _ASM_STACKPROTECTOR_H
#define _ASM_STACKPROTECTOR_H 1

#ifdef CONFIG_CC_STACKPROTECTOR

#include <asm/tsc.h>
#include <asm/processor.h>
#include <asm/percpu.h>
#include <asm/desc.h>
#include <linux/random.h>

/*
                                                                  
                                                                 
                                                             
 */
#define GDT_STACK_CANARY_INIT						\
	[GDT_ENTRY_STACK_CANARY] = GDT_ENTRY_INIT(0x4090, 0, 0x18),

/*
                                              
  
                                                                   
                                 
 */
static __always_inline void boot_init_stack_canary(void)
{
	u64 canary;
	u64 tsc;

#ifdef CONFIG_X86_64
	BUILD_BUG_ON(offsetof(union irq_stack_union, stack_canary) != 40);
#endif
	/*
                                                               
                                                            
                                                               
                                                              
  */
	get_random_bytes(&canary, sizeof(canary));
	tsc = __native_read_tsc();
	canary += tsc + (tsc << 32UL);

	current->stack_canary = canary;
#ifdef CONFIG_X86_64
	percpu_write(irq_stack_union.stack_canary, canary);
#else
	percpu_write(stack_canary.canary, canary);
#endif
}

static inline void setup_stack_canary_segment(int cpu)
{
#ifdef CONFIG_X86_32
	unsigned long canary = (unsigned long)&per_cpu(stack_canary, cpu);
	struct desc_struct *gdt_table = get_cpu_gdt_table(cpu);
	struct desc_struct desc;

	desc = gdt_table[GDT_ENTRY_STACK_CANARY];
	set_desc_base(&desc, canary);
	write_gdt_entry(gdt_table, GDT_ENTRY_STACK_CANARY, &desc, DESCTYPE_S);
#endif
}

static inline void load_stack_canary_segment(void)
{
#ifdef CONFIG_X86_32
	asm("mov %0, %%gs" : : "r" (__KERNEL_STACK_CANARY) : "memory");
#endif
}

#else	/*                   */

#define GDT_STACK_CANARY_INIT

/*                                                                     */

static inline void setup_stack_canary_segment(int cpu)
{ }

static inline void load_stack_canary_segment(void)
{
#ifdef CONFIG_X86_32
	asm volatile ("mov %0, %%gs" : : "r" (0));
#endif
}

#endif	/*                   */
#endif	/*                       */
