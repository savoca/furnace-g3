/*
 * linux/arch/unicore32/include/asm/cacheflush.h
 *
 * Code specific to PKUnity SoC and UniCore ISA
 *
 * Copyright (C) 2001-2010 GUAN Xue-tao
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __UNICORE_CACHEFLUSH_H__
#define __UNICORE_CACHEFLUSH_H__

#include <linux/mm.h>

#include <asm/shmparam.h>

#define CACHE_COLOUR(vaddr)	((vaddr & (SHMLBA - 1)) >> PAGE_SHIFT)

/*
                                                                           
                                                                 
 */
#define PG_dcache_clean PG_arch_1

/*
                      
                      
  
                                                               
  
                                                                 
                                                            
  
                                                       
                                                                 
                                                    
  
                     
  
                                                           
                                                            
                                                      
  
                   
  
                                                          
  
                   
  
                                                     
                                   
  
                                      
  
                                                        
                                                           
                                                          
                                                          
                                 
  
                                  
  
                                                             
                                                             
                                                        
                                    
                                  
  
                                  
  
                                                             
                                                             
                                                        
                                    
                                  
  
                                      
  
                                                      
                           
                          
  
                      
                      
  
                              
  
                                                             
                                    
                                  
 */

extern void __cpuc_flush_icache_all(void);
extern void __cpuc_flush_kern_all(void);
extern void __cpuc_flush_user_all(void);
extern void __cpuc_flush_user_range(unsigned long, unsigned long, unsigned int);
extern void __cpuc_coherent_kern_range(unsigned long, unsigned long);
extern void __cpuc_coherent_user_range(unsigned long, unsigned long);
extern void __cpuc_flush_dcache_area(void *, size_t);
extern void __cpuc_flush_kern_dcache_area(void *addr, size_t size);

/*
                                                                  
                                                              
                                                                
                      
 */
extern void __cpuc_dma_clean_range(unsigned long, unsigned long);
extern void __cpuc_dma_flush_range(unsigned long, unsigned long);

/*
                                                                 
                                                               
                               
 */
extern void copy_to_user_page(struct vm_area_struct *, struct page *,
	unsigned long, void *, const void *, unsigned long);
#define copy_from_user_page(vma, page, vaddr, dst, src, len)	\
	do {							\
		memcpy(dst, src, len);				\
	} while (0)

/*
                                           
 */
/*                    */
static inline void __flush_icache_all(void)
{
	asm("movc	p0.c5, %0, #20;\n"
	    "nop; nop; nop; nop; nop; nop; nop; nop\n"
	    :
	    : "r" (0));
}

#define flush_cache_all()		__cpuc_flush_kern_all()

extern void flush_cache_mm(struct mm_struct *mm);
extern void flush_cache_range(struct vm_area_struct *vma,
		unsigned long start, unsigned long end);
extern void flush_cache_page(struct vm_area_struct *vma,
		unsigned long user_addr, unsigned long pfn);

#define flush_cache_dup_mm(mm) flush_cache_mm(mm)

/*
                                                                 
                                                                    
                                                                   
 */
#define flush_cache_user_range(vma, start, end) \
	__cpuc_coherent_user_range((start) & PAGE_MASK, PAGE_ALIGN(end))

/*
                                                                    
                                                                    
 */
#define flush_icache_range(s, e)	__cpuc_coherent_kern_range(s, e)

/*
                                                                 
                                          
 */
#define clean_dcache_area(start, size)	cpu_dcache_clean_area(start, size)

/*
                                                                    
                                               
  
                                                                    
                                                                    
                                                         
  
                                                                        
                                                                              
                                                
 */
#define ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE 1
extern void flush_dcache_page(struct page *);

#define flush_dcache_mmap_lock(mapping)			\
	spin_lock_irq(&(mapping)->tree_lock)
#define flush_dcache_mmap_unlock(mapping)		\
	spin_unlock_irq(&(mapping)->tree_lock)

#define flush_icache_user_range(vma, page, addr, len)	\
	flush_dcache_page(page)

/*
                                                                         
                                                                       
 */
#define flush_icache_page(vma, page)	do { } while (0)

/*
                                                                   
                                                                
                                                                      
                                                                   
                                                                   
 */
static inline void flush_cache_vmap(unsigned long start, unsigned long end)
{
}

static inline void flush_cache_vunmap(unsigned long start, unsigned long end)
{
}

#endif
