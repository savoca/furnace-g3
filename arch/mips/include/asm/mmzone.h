/*
                                                 
                                                                     
 */
#ifndef _ASM_MMZONE_H_
#define _ASM_MMZONE_H_

#include <asm/page.h>
#include <mmzone.h>

#ifdef CONFIG_DISCONTIGMEM

#define pfn_to_nid(pfn)		pa_to_nid((pfn) << PAGE_SHIFT)

#endif /*                     */

#endif /*                */
