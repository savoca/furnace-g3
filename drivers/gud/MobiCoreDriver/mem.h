/*
 * MobiCore driver module.(interface to the secure world SWD)
 *
 * <-- Copyright Giesecke & Devrient GmbH 2009-2012 -->
 * <-- Copyright Trustonic Limited 2013 -->
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _MC_MEM_H_
#define _MC_MEM_H_

#ifdef LPAE_SUPPORT
/*
                                                                          
                                                                            
 */
#define MC_ARM_MMU_TABLE_ENTRIES		512

/*                                                    */
struct mmutable {
	uint64_t	table_entries[MC_ARM_MMU_TABLE_ENTRIES];
};

/*                                */
#define MMU_TABLES_PER_PAGE		1
#else
/*
                                                         
                                                                          
                                        
  
                                                                          
                                                                            
 */
#define MC_ARM_MMU_TABLE_ENTRIES		256

/*                                                    */
struct mmutable {
	uint32_t	table_entries[MC_ARM_MMU_TABLE_ENTRIES];
};

/*                                  */
#define MMU_TABLES_PER_PAGE		4
#endif

/*                                          */
struct mc_mmu_table_store {
	struct mmutable table[MMU_TABLES_PER_PAGE];
};

/*                                                            */
struct mc_mmu_tables_set {
	struct list_head		list;
	/*                        */
	struct mc_mmu_table_store	*kernel_virt;
	/*                  */
	phys_addr_t			phys;
	/*                        */
	struct page			*page;
	/*                                       */
	atomic_t			used_tables;
};

/*
                                                                       
          
                                                                       
                                                                            
                                                                     
 */
struct mc_mmu_table {
	struct list_head	list;
	/*            */
	struct mutex		lock;
	/*                                     */
	unsigned int		handle;
	/*                                             */
	atomic_t		usage;
	/*                         */
	struct mc_instance	*owner;
	/*                                              */
	struct mc_mmu_tables_set	*set;
	/*                          */
	unsigned int		idx;
	/*                */
	unsigned int		pages;
	/*                */
	void			*virt;
	/*                  */
	phys_addr_t		phys;
};

/*                                      */
struct mc_mem_context {
	struct mc_instance	*daemon_inst;
	/*                              */
	struct list_head	mmu_tables_sets;
	/*                                 */
	struct list_head	mmu_tables;
	/*                                 */
	struct list_head	free_mmu_tables;
	/*                                                */
	struct mutex		table_lock;
};

/*
                                             
                                            
 */
struct mc_mmu_table *mc_alloc_mmu_table(struct mc_instance *instance,
	struct task_struct *task, void *wsm_buffer, unsigned int wsm_len);

/*                                                       */
void mc_clear_mmu_tables(struct mc_instance *instance);

/*                                 */
void mc_clean_mmu_tables(void);

/*                          */
int mc_free_mmu_table(struct mc_instance *instance, uint32_t handle);

/*
                                                                     
                                                                         
 */
int mc_lock_mmu_table(struct mc_instance *instance, uint32_t handle);

/*                                       */
phys_addr_t mc_find_mmu_table(uint32_t handle, int32_t fd);
/*                                                   */
void mc_release_mmu_tables(void);

/*                                     */
int mc_init_mmu_tables(void);

#endif /*            */
