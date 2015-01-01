/*
 * index.h - Defines for NTFS kernel index handling.  Part of the Linux-NTFS
 *	     project.
 *
 * Copyright (c) 2004 Anton Altaparmakov
 *
 * This program/include file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program/include file is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the main directory of the Linux-NTFS
 * distribution in the file COPYING); if not, write to the Free Software
 * Foundation,Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _LINUX_NTFS_INDEX_H
#define _LINUX_NTFS_INDEX_H

#include <linux/fs.h>

#include "types.h"
#include "layout.h"
#include "inode.h"
#include "attrib.h"
#include "mft.h"
#include "aops.h"

/* 
                                                                       
                                               
                                               
                                      
                                                                      
                                                     
                                                                    
                                                         
                                                                 
                                                           
  
                                                      
  
                                                                            
                                                                         
                                                                               
  
                                                                                
                                                                              
                               
  
                                                                                 
                                                                               
                                                                       
  
                                                 
  
                                                                             
                                                                               
                            
  
                                                                            
                                                   
  
                                                                        
                                                                              
                                                                         
                                               
 */
typedef struct {
	ntfs_inode *idx_ni;
	INDEX_ENTRY *entry;
	void *data;
	u16 data_len;
	bool is_in_root;
	INDEX_ROOT *ir;
	ntfs_attr_search_ctx *actx;
	ntfs_inode *base_ni;
	INDEX_ALLOCATION *ia;
	struct page *page;
} ntfs_index_context;

extern ntfs_index_context *ntfs_index_ctx_get(ntfs_inode *idx_ni);
extern void ntfs_index_ctx_put(ntfs_index_context *ictx);

extern int ntfs_index_lookup(const void *key, const int key_len,
		ntfs_index_context *ictx);

#ifdef NTFS_RW

/* 
                                                                             
                                                       
  
                                                                         
  
                                                                            
                
  
                                                                           
                                                                 
  
                                                                            
                                                                          
 */
static inline void ntfs_index_entry_flush_dcache_page(ntfs_index_context *ictx)
{
	if (ictx->is_in_root)
		flush_dcache_mft_record_page(ictx->actx->ntfs_ino);
	else
		flush_dcache_page(ictx->page);
}

/* 
                                                          
                                                       
  
                                                                         
  
                                                                         
                                                                          
                                                                          
         
  
                                                                            
                                                                           
                                                                             
                                                                            
                                                                               
                                                        
 */
static inline void ntfs_index_entry_mark_dirty(ntfs_index_context *ictx)
{
	if (ictx->is_in_root)
		mark_mft_record_dirty(ictx->actx->ntfs_ino);
	else
		mark_ntfs_record_dirty(ictx->page,
				(u8*)ictx->ia - (u8*)page_address(ictx->page));
}

#endif /*         */

#endif /*                     */
