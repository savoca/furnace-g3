/*
 * segment.h - NILFS Segment constructor prototypes and definitions
 *
 * Copyright (C) 2005-2008 Nippon Telegraph and Telephone Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Written by Ryusuke Konishi <ryusuke@osrg.net>
 *
 */
#ifndef _NILFS_SEGMENT_H
#define _NILFS_SEGMENT_H

#include <linux/types.h>
#include <linux/fs.h>
#include <linux/buffer_head.h>
#include <linux/nilfs2_fs.h>
#include "nilfs.h"

struct nilfs_root;

/* 
                                                    
                                     
                                                      
                                            
                                                                   
                                                               
                                                                      
                                                        
                                                           
                                                       
                                                         
                                                               
 */
struct nilfs_recovery_info {
	int			ri_need_recovery;
	sector_t		ri_super_root;
	__u64			ri_cno;

	sector_t		ri_lsegs_start;
	sector_t		ri_lsegs_end;
	u64			ri_lsegs_start_seq;
	struct list_head	ri_used_segments;
	sector_t		ri_pseg_start;
	u64			ri_seq;
	__u64			ri_segnum;
	__u64			ri_nextnum;
};

/*                  */
#define NILFS_RECOVERY_SR_UPDATED	 1  /*                            */
#define NILFS_RECOVERY_ROLLFORWARD_DONE	 2  /*                             */

/* 
                                                    
                     
                      
                                                                          
                                                  
 */
struct nilfs_cstage {
	int			scnt;
	unsigned		flags;
	struct nilfs_inode_info *dirty_file_ptr;
	struct nilfs_inode_info *gc_inode_ptr;
};

struct nilfs_segment_buffer;

struct nilfs_segsum_pointer {
	struct buffer_head     *bh;
	unsigned		offset; /*                 */
};

/* 
                                                         
                                                
                                                       
                                                  
                                               
                                                               
                                                     
                                                    
                                                                           
                                                   
                                                           
                                       
                                                                     
                                                                     
                                     
                              
                                                                            
                                                                            
                                     
                                              
                                                                              
                               
                                            
                            
                                                  
                                  
                                                                  
                                         
                                     
                                                               
                                   
                                         
                                   
                                               
                                                         
                                                   
                                                           
                                                           
                                
                                       
 */
struct nilfs_sc_info {
	struct super_block     *sc_super;
	struct nilfs_root      *sc_root;

	unsigned long		sc_nblk_inc;

	struct list_head	sc_dirty_files;
	struct list_head	sc_gc_inodes;

	__u64		       *sc_freesegs;
	size_t			sc_nfreesegs;

	struct nilfs_inode_info *sc_dsync_inode;
	loff_t			sc_dsync_start;
	loff_t			sc_dsync_end;

	/*                 */
	struct list_head	sc_segbufs;
	struct list_head	sc_write_logs;
	unsigned long		sc_segbuf_nblocks;
	struct nilfs_segment_buffer *sc_curseg;

	struct nilfs_cstage	sc_stage;

	struct nilfs_segsum_pointer sc_finfo_ptr;
	struct nilfs_segsum_pointer sc_binfo_ptr;
	unsigned long		sc_blk_cnt;
	unsigned long		sc_datablk_cnt;
	unsigned long		sc_nblk_this_inc;
	time_t			sc_seg_ctime;
	__u64			sc_cno;
	unsigned long		sc_flags;

	spinlock_t		sc_state_lock;
	unsigned long		sc_state;
	unsigned long		sc_flush_request;

	wait_queue_head_t	sc_wait_request;
	wait_queue_head_t	sc_wait_daemon;
	wait_queue_head_t	sc_wait_task;

	__u32			sc_seq_request;
	__u32			sc_seq_accepted;
	__u32			sc_seq_done;

	int			sc_sync;
	unsigned long		sc_interval;
	unsigned long		sc_mjcp_freq;
	unsigned long		sc_lseg_stime;	/*                 */
	unsigned long		sc_watermark;

	struct timer_list	sc_timer;
	struct task_struct     *sc_task;
};

/*          */
enum {
	NILFS_SC_DIRTY,		/*                                          */
	NILFS_SC_UNCLOSED,	/*                               */
	NILFS_SC_SUPER_ROOT,	/*                                     */
	NILFS_SC_PRIOR_FLUSH,	/*                                            
                  */
	NILFS_SC_HAVE_DELTA,	/*                                          
                                               
                   */
};

/*          */
#define NILFS_SEGCTOR_QUIT	    0x0001  /*                             */
#define NILFS_SEGCTOR_COMMIT	    0x0004  /*                              */

/*
                      
 */
#define NILFS_SC_CLEANUP_RETRY	    3  /*                                 
                           */

/*
                                         
 */
#define NILFS_SC_DEFAULT_TIMEOUT    5   /*                               
                                     
                                          */
#define NILFS_SC_DEFAULT_SR_FREQ    30  /*                                
                 */

/*
                                                         
 */
#define NILFS_SC_DEFAULT_WATERMARK  3600

/*         */
extern struct kmem_cache *nilfs_transaction_cachep;

/*           */
extern void nilfs_relax_pressure_in_lock(struct super_block *);

extern int nilfs_construct_segment(struct super_block *);
extern int nilfs_construct_dsync_segment(struct super_block *, struct inode *,
					 loff_t, loff_t);
extern void nilfs_flush_segment(struct super_block *, ino_t);
extern int nilfs_clean_segments(struct super_block *, struct nilfs_argv *,
				void **);

int nilfs_attach_log_writer(struct super_block *sb, struct nilfs_root *root);
void nilfs_detach_log_writer(struct super_block *sb);

/*            */
extern int nilfs_read_super_root_block(struct the_nilfs *, sector_t,
				       struct buffer_head **, int);
extern int nilfs_search_super_root(struct the_nilfs *,
				   struct nilfs_recovery_info *);
int nilfs_salvage_orphan_logs(struct the_nilfs *nilfs, struct super_block *sb,
			      struct nilfs_recovery_info *ri);
extern void nilfs_dispose_segment_list(struct list_head *);

#endif /*                  */
