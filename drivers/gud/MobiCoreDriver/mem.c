/*
 * MobiCore Driver Kernel Module.
 *
 * This module is written as a Linux device driver.
 * This driver represents the command proxy on the lowest layer, from the
 * secure world to the non secure world, and vice versa.
 * This driver is located in the non secure world (Linux).
 * This driver offers IOCTL commands, for access to the secure world, and has
 * the interface from the secure world to the normal world.
 * The access to the driver is possible with a file descriptor,
 * which has to be created by the fd = open(/dev/mobicore) command.
 *
 * <-- Copyright Giesecke & Devrient GmbH 2009-2012 -->
 * <-- Copyright Trustonic Limited 2013 -->
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include "main.h"
#include "debug.h"
#include "mem.h"

#include <linux/highmem.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/pagemap.h>
#include <linux/device.h>

#ifdef LPAE_SUPPORT
#define MMU_TYPE_PAGE	(3 << 0)
#define MMU_BUFFERABLE	(1 << 2) /*             */
#define MMU_CACHEABLE	(1 << 3) /*             */
#define MMU_NS		(1 << 5)
#define MMU_AP_RW_ALL	(1 << 6) /*                                     */
#define MMU_EXT_SHARED	(3 << 8) /*                          */
#define MMU_EXT_AF	(1 << 10) /*             */
#define MMU_EXT_NG	(1 << 11)
#define MMU_EXT_XN      (((uint64_t)1) << 54) /*    */
#else
#define MMU_TYPE_EXT	(3 << 0)	/*    */
#define MMU_TYPE_SMALL	(2 << 0)
#define MMU_BUFFERABLE	(1 << 2)
#define MMU_CACHEABLE	(1 << 3)
#define MMU_EXT_AP0	(1 << 4)
#define MMU_EXT_AP1	(2 << 4)
#define MMU_EXT_TEX(x)	((x) << 6)	/*    */
#define MMU_EXT_SHARED	(1 << 10)	/*    */
#define MMU_EXT_NG	(1 << 11)	/*    */
#endif

/*                              */
struct mc_mem_context mem_ctx;

static inline void release_page(struct page *page)
{
	set_bit(PG_dirty, &page->flags);

	page_cache_release(page);
}

static int lock_pages(struct task_struct *task, void *virt_start_page_addr,
	int pages_no, struct page **pages)
{
	int locked_pages;

	/*                                                     */
	down_read(&(task->mm->mmap_sem));
	locked_pages = get_user_pages(
				task,
				task->mm,
				(unsigned long)virt_start_page_addr,
				pages_no,
				1, /*              */
				0,
				pages,
				NULL);
	up_read(&(task->mm->mmap_sem));

	/*                                   */
	if (locked_pages != pages_no) {
		MCDRV_DBG_ERROR(mcd, "get_user_pages() failed, locked_pages=%d",
				locked_pages);
		if (locked_pages > 0) {
			/*                           */
			release_pages(pages, locked_pages, 0);
		}
		return -ENOMEM;
	}

	return 0;
}

/*                                                                      */
static void *get_mmu_table_kernel_virt(struct mc_mmu_table *table)
{
	if (WARN(!table, "Invalid MMU table"))
		return NULL;

	if (WARN(!table->set, "Invalid MMU table set"))
		return NULL;

	if (WARN(!table->set->kernel_virt, "Invalid MMU pointer"))
		return NULL;

	return &(table->set->kernel_virt->table[table->idx]);
}

static inline int in_use(struct mc_mmu_table *table)
{
	return atomic_read(&table->usage) > 0;
}

/*
                                                                         
                                   
 */
struct mc_mmu_table *find_mmu_table(unsigned int handle)
{
	struct mc_mmu_table *table;

	list_for_each_entry(table, &mem_ctx.mmu_tables, list) {
		if (table->handle == handle)
			return table;
	}
	return NULL;
}

/*
                                                                          
                                                                            
 */
static int alloc_mmu_table_store(void)
{
	unsigned long store;
	struct mc_mmu_tables_set *mmutable_set;
	struct mc_mmu_table *mmutable, *mmutable2;
	struct page *page;
	int ret = 0, i;
	/*                                      */
	LIST_HEAD(temp);

	store = get_zeroed_page(GFP_KERNEL);
	if (!store)
		return -ENOMEM;

	/*
                                                      
                                                     
                 
  */
	page = virt_to_page(store);
	set_bit(PG_reserved, &page->flags);

	/*                                                      */
	mmutable_set = kmalloc(sizeof(*mmutable_set), GFP_KERNEL | __GFP_ZERO);
	if (mmutable_set == NULL) {
		ret = -ENOMEM;
		goto free_store;
	}
	/*            */
	mmutable_set->kernel_virt = (void *)store;
	mmutable_set->page = page;
	mmutable_set->phys = virt_to_phys((void *)store);
	/*                         */
	atomic_set(&mmutable_set->used_tables, 0);

	/*                   */
	INIT_LIST_HEAD(&(mmutable_set->list));
	list_add(&mmutable_set->list, &mem_ctx.mmu_tables_sets);

	for (i = 0; i < MMU_TABLES_PER_PAGE; i++) {
		/*                               */
		mmutable  = kmalloc(sizeof(*mmutable), GFP_KERNEL | __GFP_ZERO);
		if (mmutable == NULL) {
			ret = -ENOMEM;
			MCDRV_DBG_ERROR(mcd, "out of memory");
			/*                                                    */
			goto free_temp_list;
		}

		/*                   */
		mmutable->set = mmutable_set;
		mmutable->idx = i;
		mmutable->virt = get_mmu_table_kernel_virt(mmutable);
		mmutable->phys = mmutable_set->phys+i*sizeof(struct mmutable);
		atomic_set(&mmutable->usage, 0);

		/*                   */
		INIT_LIST_HEAD(&mmutable->list);
		list_add_tail(&mmutable->list, &temp);
	}

	/*
                                                                  
             
  */
	list_splice_tail(&temp, &mem_ctx.free_mmu_tables);
	return 0;
free_temp_list:
	list_for_each_entry_safe(mmutable, mmutable2, &temp, list) {
		kfree(mmutable);
	}

	list_del(&mmutable_set->list);

free_store:
	free_page(store);
	return ret;
}
/*
                                                                      
                                                          
 */
static struct mc_mmu_table *alloc_mmu_table(struct mc_instance *instance)
{
	int ret = 0;
	struct mc_mmu_table *table = NULL;

	if (list_empty(&mem_ctx.free_mmu_tables)) {
		ret = alloc_mmu_table_store();
		if (ret) {
			MCDRV_DBG_ERROR(mcd, "Failed to allocate new store!");
			return ERR_PTR(-ENOMEM);
		}
		/*                                                  */
		if (list_empty(&mem_ctx.free_mmu_tables)) {
			MCDRV_DBG_ERROR(mcd,
					"Free list not updated correctly!");
			return ERR_PTR(-EFAULT);
		}
	}

	/*                          */
	table  = list_first_entry(&mem_ctx.free_mmu_tables,
		struct mc_mmu_table, list);
	if (table == NULL) {
		MCDRV_DBG_ERROR(mcd, "out of memory");
		return ERR_PTR(-ENOMEM);
	}
	/*                                     */
	list_move_tail(&table->list, &mem_ctx.mmu_tables);

	table->handle = get_unique_id();
	table->owner = instance;

	atomic_inc(&table->set->used_tables);
	atomic_inc(&table->usage);

	MCDRV_DBG_VERBOSE(mcd,
			  "chunkPhys=0x%llX, idx=%d",
			  (u64)table->set->phys, table->idx);

	return table;
}

/*
                                                                              
                                                                        
                                   
                                           
 */
static void free_mmu_table(struct mc_mmu_table *table)
{
	struct mc_mmu_tables_set *mmutable_set;

	if (WARN(!table, "Invalid table"))
		return;

	mmutable_set = table->set;
	if (WARN(!mmutable_set, "Invalid table set"))
		return;

	list_move_tail(&table->list, &mem_ctx.free_mmu_tables);

	/*                                             */
	if (atomic_dec_and_test(&mmutable_set->used_tables)) {
		struct mc_mmu_table *tmp;

		/*                  */
		list_del(&mmutable_set->list);
		/*
                                                         
                                          
   */
		list_for_each_entry_safe(table, tmp, &mem_ctx.free_mmu_tables,
					 list) {
			if (table->set == mmutable_set) {
				list_del(&table->list);
				kfree(table);
			}
		} /*           */

		/*
                                                          
                      
   */
		BUG_ON(!mmutable_set->page);
		clear_bit(PG_reserved, &(mmutable_set->page)->flags);


		BUG_ON(!mmutable_set->kernel_virt);
		free_page((unsigned long)mmutable_set->kernel_virt);

		kfree(mmutable_set);
	}
}

/*
                                                                            
                                                                           
                                                             
  
                                   
                                    
                          
                                            
 */
static int map_buffer(struct task_struct *task, void *wsm_buffer,
		      unsigned int wsm_len, struct mc_mmu_table *table)
{
	int		ret = 0;
	unsigned int	i, nr_of_pages;
	/*                                               */
	void		*virt_addr_page;
	struct page	*page;
	struct mmutable	*mmutable;
	struct page	**mmutable_as_array_of_pointers_to_page;
	/*                           */
	unsigned int offset;

	if (WARN(!wsm_buffer, "Invalid WSM buffer pointer"))
		return -EINVAL;

	if (WARN(wsm_len == 0, "Invalid WSM buffer length"))
		return -EINVAL;

	if (WARN(!table, "Invalid mapping table for WSM"))
		return -EINVAL;

	/*                          */
	if (wsm_len > SZ_1M) {
		MCDRV_DBG_ERROR(mcd, "size > 1 MiB");
		return -EINVAL;
	}

	MCDRV_DBG_VERBOSE(mcd, "WSM addr=0x%p, len=0x%08x", wsm_buffer,
			  wsm_len);

	/*                      */
	virt_addr_page = (void *)(((unsigned long)(wsm_buffer)) & PAGE_MASK);
	offset = (unsigned int)	(((unsigned long)(wsm_buffer)) & (~PAGE_MASK));
	nr_of_pages  = PAGE_ALIGN(offset + wsm_len) / PAGE_SIZE;

	MCDRV_DBG_VERBOSE(mcd, "virt addr page start=0x%p, pages=%d",
			  virt_addr_page, nr_of_pages);

	/*                                           */
	if ((nr_of_pages * PAGE_SIZE) > SZ_1M) {
		MCDRV_DBG_ERROR(mcd, "WSM paged exceed 1 MiB");
		return -EINVAL;
	}

	mmutable = table->virt;
	/*
                                                           
                                                           
                           
  */
	mmutable_as_array_of_pointers_to_page = (struct page **)mmutable;

	/*                               */
	if (task != NULL && !is_vmalloc_addr(wsm_buffer)) {
		/*
                                                         
         
                                                             
                                                              
                                        
                                 
                                                          
                                    
   */
		ret = lock_pages(task, virt_addr_page, nr_of_pages,
				 mmutable_as_array_of_pointers_to_page);
		if (ret != 0) {
			MCDRV_DBG_ERROR(mcd, "lock_user_pages() failed");
			return ret;
		}
	}
	/*                                              */
	else if (task == NULL && !is_vmalloc_addr(wsm_buffer)) {
		void *uaddr = wsm_buffer;
		for (i = 0; i < nr_of_pages; i++) {
			page = virt_to_page(uaddr);
			if (!page) {
				MCDRV_DBG_ERROR(mcd, "failed to map address");
				return -EINVAL;
			}
			get_page(page);
			mmutable_as_array_of_pointers_to_page[i] = page;
			uaddr += PAGE_SIZE;
		}
	}
	/*                                                 */
	else {
		void *uaddr = wsm_buffer;
		for (i = 0; i < nr_of_pages; i++) {
			page = vmalloc_to_page(uaddr);
			if (!page) {
				MCDRV_DBG_ERROR(mcd, "failed to map address");
				return -EINVAL;
			}
			get_page(page);
			mmutable_as_array_of_pointers_to_page[i] = page;
			uaddr += PAGE_SIZE;
		}
	}

	table->pages = nr_of_pages;

	/*
                             
                                                               
                                                             
                                                                  
                                                                 
             
                                                   
                                                                     
           
  */
	i = MC_ARM_MMU_TABLE_ENTRIES-1;
	do {
		if (i < nr_of_pages) {
#ifdef LPAE_SUPPORT
			uint64_t pte;
#elif defined(CONFIG_ARM_LPAE) && !defined(LPAE_SUPPORT)
			/*                                              */
			uint64_t pte64;
			uint32_t pte;
#else
			uint32_t pte;
#endif
			page = mmutable_as_array_of_pointers_to_page[i];

			/*
                                                          
                                               
                                      
                                         
                                
                                     
                            
    */
#ifdef LPAE_SUPPORT
			pte = page_to_phys(page);
			pte |=	MMU_EXT_XN
				| MMU_EXT_NG
				| MMU_EXT_AF
				| MMU_AP_RW_ALL
				| MMU_NS
				| MMU_CACHEABLE | MMU_BUFFERABLE
				| MMU_TYPE_PAGE;
#elif defined(CONFIG_ARM_LPAE) && !defined(LPAE_SUPPORT)
			/*
                                                      
                       
                                                
    */
			 pte64 = page_to_phys(page);
			 if ((pte64 >> 32) != 0) {
				MCDRV_DBG_ERROR(mcd,
						"physical addresses bigger than 4GB not supported");
				return -EINVAL;
				}
			pte = (uint32_t)pte64;
			pte |= MMU_EXT_AP1 | MMU_EXT_AP0
				| MMU_CACHEABLE | MMU_BUFFERABLE
				| MMU_TYPE_SMALL | MMU_TYPE_EXT | MMU_EXT_NG;
#else
			pte = page_to_phys(page);
			pte |= MMU_EXT_AP1 | MMU_EXT_AP0
				| MMU_CACHEABLE | MMU_BUFFERABLE
				| MMU_TYPE_SMALL | MMU_TYPE_EXT | MMU_EXT_NG;
#endif /*              */
			/*
                                                       
                                                      
                                                       
                                         
                                          
    */
#ifdef CONFIG_SMP
#ifdef LPAE_SUPPORT
			pte |= MMU_EXT_SHARED;
#else
			pte |= MMU_EXT_SHARED | MMU_EXT_TEX(1);
#endif /*              */
#endif /*            */

			mmutable->table_entries[i] = pte;
			MCDRV_DBG_VERBOSE(mcd, "MMU entry %d:  0x%llx", i,
					  (u64)(pte));
		} else {
			/*                               */
			mmutable->table_entries[i] = 0;
		}
	} while (i-- != 0);

	return ret;
}

/*
                                                                         
                                                          
 */
static void unmap_buffers(struct mc_mmu_table *table)
{
	struct mmutable *mmutable;
	int i;

	if (WARN_ON(!table))
		return;

	/*                                             */
	MCDRV_DBG_VERBOSE(mcd,
			  "clear MMU table, phys_base=0x%llX,nr_of_pages=%d",
			  (u64)table->phys, table->pages);

	mmutable = table->virt;

	/*                                     */
	for (i = 0; i < table->pages; i++) {
		/*                                                          */
		struct page *page;
		page = phys_to_page(mmutable->table_entries[i]);
		release_page(page);
	}

	/*                                         */
	table->pages = 0;
}

/*                                                                       */
static void unmap_mmu_table(struct mc_mmu_table *table)
{
	/*                                                  */
	if (!atomic_dec_and_test(&table->usage))
		return;

	/*                                                */
	unmap_buffers(table);
	free_mmu_table(table);
}

int mc_free_mmu_table(struct mc_instance *instance, uint32_t handle)
{
	struct mc_mmu_table *table;
	int ret = 0;

	if (WARN(!instance, "No instance data available"))
		return -EFAULT;

	mutex_lock(&mem_ctx.table_lock);
	table = find_mmu_table(handle);

	if (table == NULL) {
		MCDRV_DBG_VERBOSE(mcd, "entry not found");
		ret = -EINVAL;
		goto err_unlock;
	}
	if (instance != table->owner && !is_daemon(instance)) {
		MCDRV_DBG_ERROR(mcd, "instance does no own it");
		ret = -EPERM;
		goto err_unlock;
	}
	/*                                        */
	unmap_mmu_table(table);
err_unlock:
	mutex_unlock(&mem_ctx.table_lock);

	return ret;
}

int mc_lock_mmu_table(struct mc_instance *instance, uint32_t handle)
{
	int ret = 0;
	struct mc_mmu_table *table = NULL;

	if (WARN(!instance, "No instance data available"))
		return -EFAULT;

	mutex_lock(&mem_ctx.table_lock);
	table = find_mmu_table(handle);

	if (table == NULL) {
		MCDRV_DBG_VERBOSE(mcd, "entry not found %u", handle);
		ret = -EINVAL;
		goto table_err;
	}
	if (instance != table->owner && !is_daemon(instance)) {
		MCDRV_DBG_ERROR(mcd, "instance does no own it");
		ret = -EPERM;
		goto table_err;
	}

	/*            */
	atomic_inc(&table->usage);
table_err:
	mutex_unlock(&mem_ctx.table_lock);
	return ret;
}
/*
                                             
                                            
 */
struct mc_mmu_table *mc_alloc_mmu_table(struct mc_instance *instance,
	struct task_struct *task, void *wsm_buffer, unsigned int wsm_len)
{
	int ret = 0;
	struct mc_mmu_table *table;

	if (WARN(!instance, "No instance data available"))
		return ERR_PTR(-EFAULT);

	mutex_lock(&mem_ctx.table_lock);
	table = alloc_mmu_table(instance);
	if (IS_ERR(table)) {
		MCDRV_DBG_ERROR(mcd, "alloc_mmu_table() failed");
		ret = -ENOMEM;
		goto err_no_mem;
	}

	/*                                 */
	ret = map_buffer(task, wsm_buffer, wsm_len, table);

	if (ret != 0) {
		MCDRV_DBG_ERROR(mcd, "map_buffer() failed");
		unmap_mmu_table(table);
		goto err_no_mem;
	}
	MCDRV_DBG_VERBOSE(mcd,
			  "mapped buffer %p to table with handle %d @ 0x%llX",
			  wsm_buffer, table->handle, (u64)table->phys);

	mutex_unlock(&mem_ctx.table_lock);
	return table;
err_no_mem:
	mutex_unlock(&mem_ctx.table_lock);
	return ERR_PTR(ret);
}

phys_addr_t mc_find_mmu_table(uint32_t handle, int32_t fd)
{
	phys_addr_t ret = 0;
	struct mc_mmu_table *table = NULL;

	mutex_lock(&mem_ctx.table_lock);
	table = find_mmu_table(handle);

	if (table == NULL) {
		MCDRV_DBG_ERROR(mcd, "entry not found %u", handle);
		ret = 0;
		goto table_err;
	}

	/*                                                           
                                                             */
	if (!mc_check_owner_fd(table->owner, fd)) {
		MCDRV_DBG_ERROR(mcd, "not valid owner %u", handle);
		ret = 0;
		goto table_err;
	}

	ret = table->phys;
table_err:
	mutex_unlock(&mem_ctx.table_lock);
	return ret;
}

void mc_clean_mmu_tables(void)
{
	struct mc_mmu_table *table, *tmp;

	mutex_lock(&mem_ctx.table_lock);
	/*                                */
	list_for_each_entry_safe(table, tmp, &mem_ctx.mmu_tables, list) {
		if (table->owner == NULL) {
			MCDRV_DBG(mcd,
				  "cleariM MMU: p=0x%llX pages=%d",
				  (u64)table->phys,
				  table->pages);
			unmap_mmu_table(table);
		}
	}
	mutex_unlock(&mem_ctx.table_lock);
}

void mc_clear_mmu_tables(struct mc_instance *instance)
{
	struct mc_mmu_table *table, *tmp;

	mutex_lock(&mem_ctx.table_lock);
	/*                                    */
	list_for_each_entry_safe(table, tmp, &mem_ctx.mmu_tables, list) {
		if (table->owner == instance) {
			MCDRV_DBG(mcd, "release WSM MMU: p=0x%llX pages=%d",
				  (u64)table->phys,
				  table->pages);
			/*                                                */
			table->owner = NULL;
			unmap_mmu_table(table);
		}
	}
	mutex_unlock(&mem_ctx.table_lock);
}

int mc_init_mmu_tables(void)
{
	/*                               */
	INIT_LIST_HEAD(&mem_ctx.mmu_tables_sets);

	/*                            */
	INIT_LIST_HEAD(&mem_ctx.mmu_tables);

	/*                                 */
	INIT_LIST_HEAD(&mem_ctx.free_mmu_tables);

	mutex_init(&mem_ctx.table_lock);

	return 0;
}

void mc_release_mmu_tables(void)
{
	struct mc_mmu_table *table;
	/*                                    */
	list_for_each_entry(table, &mem_ctx.mmu_tables, list) {
		WARN(1, "WSM MMU still in use: phys=0x%llX ,nr_of_pages=%d",
		     (u64)table->phys, table->pages);
	}
}
