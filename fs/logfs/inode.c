/*
 * fs/logfs/inode.c	- inode handling code
 *
 * As should be obvious for Linux kernel code, license is GPLv2
 *
 * Copyright (c) 2005-2008 Joern Engel <joern@logfs.org>
 */
#include "logfs.h"
#include <linux/slab.h>
#include <linux/writeback.h>
#include <linux/backing-dev.h>

/*
                                                                           
                                                                         
                                                                            
                                                                            
                                                                           
                                                                               
                                                                      
                                                                           
                                                
                                                                        
                                       
  
                                        
 */
#define INOS_PER_WRAP (0x10000)

/*
                                                                            
                                                                             
                                                                          
  
                                                                               
                                                                          
                                                                            
                                                                      
  
                                                                             
                                                                               
                                                                         
                                                                     
  
                                                               
                                                                 
                                     
 */
static struct kmem_cache *logfs_inode_cache;

static DEFINE_SPINLOCK(logfs_inode_lock);

static void logfs_inode_setops(struct inode *inode)
{
	switch (inode->i_mode & S_IFMT) {
	case S_IFDIR:
		inode->i_op = &logfs_dir_iops;
		inode->i_fop = &logfs_dir_fops;
		inode->i_mapping->a_ops = &logfs_reg_aops;
		break;
	case S_IFREG:
		inode->i_op = &logfs_reg_iops;
		inode->i_fop = &logfs_reg_fops;
		inode->i_mapping->a_ops = &logfs_reg_aops;
		break;
	case S_IFLNK:
		inode->i_op = &logfs_symlink_iops;
		inode->i_mapping->a_ops = &logfs_reg_aops;
		break;
	case S_IFSOCK:	/*              */
	case S_IFBLK:	/*              */
	case S_IFCHR:	/*              */
	case S_IFIFO:
		init_special_inode(inode, inode->i_mode, inode->i_rdev);
		break;
	default:
		BUG();
	}
}

static struct inode *__logfs_iget(struct super_block *sb, ino_t ino)
{
	struct inode *inode = iget_locked(sb, ino);
	int err;

	if (!inode)
		return ERR_PTR(-ENOMEM);
	if (!(inode->i_state & I_NEW))
		return inode;

	err = logfs_read_inode(inode);
	if (err || inode->i_nlink == 0) {
		/*                                                 
                     */
		/*                                     */
		clear_nlink(inode);
		logfs_inode(inode)->li_flags |= LOGFS_IF_ZOMBIE;
		iget_failed(inode);
		if (!err)
			err = -ENOENT;
		return ERR_PTR(err);
	}

	logfs_inode_setops(inode);
	unlock_new_inode(inode);
	return inode;
}

struct inode *logfs_iget(struct super_block *sb, ino_t ino)
{
	BUG_ON(ino == LOGFS_INO_MASTER);
	BUG_ON(ino == LOGFS_INO_SEGFILE);
	return __logfs_iget(sb, ino);
}

/*
                                                                    
                                                     
 */
struct inode *logfs_safe_iget(struct super_block *sb, ino_t ino, int *is_cached)
{
	struct logfs_super *super = logfs_super(sb);
	struct logfs_inode *li;

	if (ino == LOGFS_INO_MASTER)
		return super->s_master_inode;
	if (ino == LOGFS_INO_SEGFILE)
		return super->s_segfile_inode;

	spin_lock(&logfs_inode_lock);
	list_for_each_entry(li, &super->s_freeing_list, li_freeing_list)
		if (li->vfs_inode.i_ino == ino) {
			li->li_refcount++;
			spin_unlock(&logfs_inode_lock);
			*is_cached = 1;
			return &li->vfs_inode;
		}
	spin_unlock(&logfs_inode_lock);

	*is_cached = 0;
	return __logfs_iget(sb, ino);
}

static void logfs_i_callback(struct rcu_head *head)
{
	struct inode *inode = container_of(head, struct inode, i_rcu);
	kmem_cache_free(logfs_inode_cache, logfs_inode(inode));
}

static void __logfs_destroy_inode(struct inode *inode)
{
	struct logfs_inode *li = logfs_inode(inode);

	BUG_ON(li->li_block);
	list_del(&li->li_freeing_list);
	call_rcu(&inode->i_rcu, logfs_i_callback);
}

static void logfs_destroy_inode(struct inode *inode)
{
	struct logfs_inode *li = logfs_inode(inode);

	BUG_ON(list_empty(&li->li_freeing_list));
	spin_lock(&logfs_inode_lock);
	li->li_refcount--;
	if (li->li_refcount == 0)
		__logfs_destroy_inode(inode);
	spin_unlock(&logfs_inode_lock);
}

void logfs_safe_iput(struct inode *inode, int is_cached)
{
	if (inode->i_ino == LOGFS_INO_MASTER)
		return;
	if (inode->i_ino == LOGFS_INO_SEGFILE)
		return;

	if (is_cached) {
		logfs_destroy_inode(inode);
		return;
	}

	iput(inode);
}

static void logfs_init_inode(struct super_block *sb, struct inode *inode)
{
	struct logfs_inode *li = logfs_inode(inode);
	int i;

	li->li_flags	= 0;
	li->li_height	= 0;
	li->li_used_bytes = 0;
	li->li_block	= NULL;
	inode->i_uid	= 0;
	inode->i_gid	= 0;
	inode->i_size	= 0;
	inode->i_blocks	= 0;
	inode->i_ctime	= CURRENT_TIME;
	inode->i_mtime	= CURRENT_TIME;
	li->li_refcount = 1;
	INIT_LIST_HEAD(&li->li_freeing_list);

	for (i = 0; i < LOGFS_EMBEDDED_FIELDS; i++)
		li->li_data[i] = 0;

	return;
}

static struct inode *logfs_alloc_inode(struct super_block *sb)
{
	struct logfs_inode *li;

	li = kmem_cache_alloc(logfs_inode_cache, GFP_NOFS);
	if (!li)
		return NULL;
	logfs_init_inode(sb, &li->vfs_inode);
	return &li->vfs_inode;
}

/*
                                                                          
                                                                           
                                                                             
                                                                     
  
                                                                             
                                                                               
                                                                             
                                                                           
        
  
                                                                             
                                                                             
                                                                           
                                                                            
                                                        
  
                                                                         
                                                                             
                                                                      
                           
  
                                          
 */
struct inode *logfs_new_meta_inode(struct super_block *sb, u64 ino)
{
	struct inode *inode;

	inode = new_inode(sb);
	if (!inode)
		return ERR_PTR(-ENOMEM);

	inode->i_mode = S_IFREG;
	inode->i_ino = ino;
	inode->i_data.a_ops = &logfs_reg_aops;
	mapping_set_gfp_mask(&inode->i_data, GFP_NOFS);

	return inode;
}

struct inode *logfs_read_meta_inode(struct super_block *sb, u64 ino)
{
	struct inode *inode;
	int err;

	inode = logfs_new_meta_inode(sb, ino);
	if (IS_ERR(inode))
		return inode;

	err = logfs_read_inode(inode);
	if (err) {
		iput(inode);
		return ERR_PTR(err);
	}
	logfs_inode_setops(inode);
	return inode;
}

static int logfs_write_inode(struct inode *inode, struct writeback_control *wbc)
{
	int ret;
	long flags = WF_LOCK;

	/*                                                   */
	if (logfs_inode(inode)->li_flags & LOGFS_IF_STILLBORN)
		return 0;

	ret = __logfs_write_inode(inode, NULL, flags);
	LOGFS_BUG_ON(ret, inode->i_sb);
	return ret;
}

/*                                */
static int logfs_drop_inode(struct inode *inode)
{
	struct logfs_super *super = logfs_super(inode->i_sb);
	struct logfs_inode *li = logfs_inode(inode);

	spin_lock(&logfs_inode_lock);
	list_move(&li->li_freeing_list, &super->s_freeing_list);
	spin_unlock(&logfs_inode_lock);
	return generic_drop_inode(inode);
}

static void logfs_set_ino_generation(struct super_block *sb,
		struct inode *inode)
{
	struct logfs_super *super = logfs_super(sb);
	u64 ino;

	mutex_lock(&super->s_journal_mutex);
	ino = logfs_seek_hole(super->s_master_inode, super->s_last_ino + 1);
	super->s_last_ino = ino;
	super->s_inos_till_wrap--;
	if (super->s_inos_till_wrap < 0) {
		super->s_last_ino = LOGFS_RESERVED_INOS;
		super->s_generation++;
		super->s_inos_till_wrap = INOS_PER_WRAP;
	}
	inode->i_ino = ino;
	inode->i_generation = super->s_generation;
	mutex_unlock(&super->s_journal_mutex);
}

struct inode *logfs_new_inode(struct inode *dir, umode_t mode)
{
	struct super_block *sb = dir->i_sb;
	struct inode *inode;

	inode = new_inode(sb);
	if (!inode)
		return ERR_PTR(-ENOMEM);

	logfs_init_inode(sb, inode);

	/*                      */
	logfs_inode(inode)->li_flags |=
		logfs_inode(dir)->li_flags & LOGFS_FL_INHERITED;

	inode->i_mode = mode;
	logfs_set_ino_generation(sb, inode);

	inode_init_owner(inode, dir, mode);
	logfs_inode_setops(inode);
	insert_inode_hash(inode);

	return inode;
}

static void logfs_init_once(void *_li)
{
	struct logfs_inode *li = _li;
	int i;

	li->li_flags = 0;
	li->li_used_bytes = 0;
	li->li_refcount = 1;
	for (i = 0; i < LOGFS_EMBEDDED_FIELDS; i++)
		li->li_data[i] = 0;
	inode_init_once(&li->vfs_inode);
}

static int logfs_sync_fs(struct super_block *sb, int wait)
{
	logfs_get_wblocks(sb, NULL, WF_LOCK);
	logfs_write_anchor(sb);
	logfs_put_wblocks(sb, NULL, WF_LOCK);
	return 0;
}

static void logfs_put_super(struct super_block *sb)
{
	struct logfs_super *super = logfs_super(sb);
	/*                      */
	iput(super->s_master_inode);
	iput(super->s_segfile_inode);
	iput(super->s_mapping_inode);
}

const struct super_operations logfs_super_operations = {
	.alloc_inode	= logfs_alloc_inode,
	.destroy_inode	= logfs_destroy_inode,
	.evict_inode	= logfs_evict_inode,
	.drop_inode	= logfs_drop_inode,
	.put_super	= logfs_put_super,
	.write_inode	= logfs_write_inode,
	.statfs		= logfs_statfs,
	.sync_fs	= logfs_sync_fs,
};

int logfs_init_inode_cache(void)
{
	logfs_inode_cache = kmem_cache_create("logfs_inode_cache",
			sizeof(struct logfs_inode), 0, SLAB_RECLAIM_ACCOUNT,
			logfs_init_once);
	if (!logfs_inode_cache)
		return -ENOMEM;
	return 0;
}

void logfs_destroy_inode_cache(void)
{
	/*
                                                               
                  
  */
	rcu_barrier();
	kmem_cache_destroy(logfs_inode_cache);
}
