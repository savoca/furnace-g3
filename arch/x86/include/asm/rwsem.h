/*                                                                 
  
                                                  
  
                                   
  
  
                                                                           
                                                           
  
                                                                         
  
                                                                              
                                                                               
                                                                             
                                                                             
          
  
                                                                             
                                                                              
                 
  
                                                                  
  
                                                                               
                                                                           
                                                                              
                                                                          
                                                                     
 */

#ifndef _ASM_X86_RWSEM_H
#define _ASM_X86_RWSEM_H

#ifndef _LINUX_RWSEM_H
#error "please don't include asm/rwsem.h directly, use linux/rwsem.h instead"
#endif

#ifdef __KERNEL__
#include <asm/asm.h>

/*
                                                            
                                                                
               
 */

#ifdef CONFIG_X86_64
# define RWSEM_ACTIVE_MASK		0xffffffffL
#else
# define RWSEM_ACTIVE_MASK		0x0000ffffL
#endif

#define RWSEM_UNLOCKED_VALUE		0x00000000L
#define RWSEM_ACTIVE_BIAS		0x00000001L
#define RWSEM_WAITING_BIAS		(-RWSEM_ACTIVE_MASK-1)
#define RWSEM_ACTIVE_READ_BIAS		RWSEM_ACTIVE_BIAS
#define RWSEM_ACTIVE_WRITE_BIAS		(RWSEM_WAITING_BIAS + RWSEM_ACTIVE_BIAS)

/*
                   
 */
static inline void __down_read(struct rw_semaphore *sem)
{
	asm volatile("# beginning down_read\n\t"
		     LOCK_PREFIX _ASM_INC "(%1)\n\t"
		     /*                 */
		     "  jns        1f\n"
		     "  call call_rwsem_down_read_failed\n"
		     "1:\n\t"
		     "# ending down_read\n\t"
		     : "+m" (sem->count)
		     : "a" (sem)
		     : "memory", "cc");
}

/*
                                                                  
 */
static inline int __down_read_trylock(struct rw_semaphore *sem)
{
	long result, tmp;
	asm volatile("# beginning __down_read_trylock\n\t"
		     "  mov          %0,%1\n\t"
		     "1:\n\t"
		     "  mov          %1,%2\n\t"
		     "  add          %3,%2\n\t"
		     "  jle	     2f\n\t"
		     LOCK_PREFIX "  cmpxchg  %2,%0\n\t"
		     "  jnz	     1b\n\t"
		     "2:\n\t"
		     "# ending __down_read_trylock\n\t"
		     : "+m" (sem->count), "=&a" (result), "=&r" (tmp)
		     : "i" (RWSEM_ACTIVE_READ_BIAS)
		     : "memory", "cc");
	return result >= 0 ? 1 : 0;
}

/*
                   
 */
static inline void __down_write_nested(struct rw_semaphore *sem, int subclass)
{
	long tmp;
	asm volatile("# beginning down_write\n\t"
		     LOCK_PREFIX "  xadd      %1,(%2)\n\t"
		     /*                                        */
		     "  test      %1,%1\n\t"
		     /*                         */
		     "  jz        1f\n"
		     "  call call_rwsem_down_write_failed\n"
		     "1:\n"
		     "# ending down_write"
		     : "+m" (sem->count), "=d" (tmp)
		     : "a" (sem), "1" (RWSEM_ACTIVE_WRITE_BIAS)
		     : "memory", "cc");
}

static inline void __down_write(struct rw_semaphore *sem)
{
	__down_write_nested(sem, 0);
}

/*
                                                                  
 */
static inline int __down_write_trylock(struct rw_semaphore *sem)
{
	long ret = cmpxchg(&sem->count, RWSEM_UNLOCKED_VALUE,
			   RWSEM_ACTIVE_WRITE_BIAS);
	if (ret == RWSEM_UNLOCKED_VALUE)
		return 1;
	return 0;
}

/*
                       
 */
static inline void __up_read(struct rw_semaphore *sem)
{
	long tmp;
	asm volatile("# beginning __up_read\n\t"
		     LOCK_PREFIX "  xadd      %1,(%2)\n\t"
		     /*                                    */
		     "  jns        1f\n\t"
		     "  call call_rwsem_wake\n" /*                           */
		     "1:\n"
		     "# ending __up_read\n"
		     : "+m" (sem->count), "=d" (tmp)
		     : "a" (sem), "1" (-RWSEM_ACTIVE_READ_BIAS)
		     : "memory", "cc");
}

/*
                       
 */
static inline void __up_write(struct rw_semaphore *sem)
{
	long tmp;
	asm volatile("# beginning __up_write\n\t"
		     LOCK_PREFIX "  xadd      %1,(%2)\n\t"
		     /*                                             */
		     "  jns        1f\n\t"
		     "  call call_rwsem_wake\n" /*                           */
		     "1:\n\t"
		     "# ending __up_write\n"
		     : "+m" (sem->count), "=d" (tmp)
		     : "a" (sem), "1" (-RWSEM_ACTIVE_WRITE_BIAS)
		     : "memory", "cc");
}

/*
                                    
 */
static inline void __downgrade_write(struct rw_semaphore *sem)
{
	asm volatile("# beginning __downgrade_write\n\t"
		     LOCK_PREFIX _ASM_ADD "%2,(%1)\n\t"
		     /*
                                                     
                                                               
        */
		     "  jns       1f\n\t"
		     "  call call_rwsem_downgrade_wake\n"
		     "1:\n\t"
		     "# ending __downgrade_write\n"
		     : "+m" (sem->count)
		     : "a" (sem), "er" (-RWSEM_WAITING_BIAS)
		     : "memory", "cc");
}

/*
                                     
 */
static inline void rwsem_atomic_add(long delta, struct rw_semaphore *sem)
{
	asm volatile(LOCK_PREFIX _ASM_ADD "%1,%0"
		     : "+m" (sem->count)
		     : "er" (delta));
}

/*
                                           
 */
static inline long rwsem_atomic_update(long delta, struct rw_semaphore *sem)
{
	return delta + xadd(&sem->count, delta);
}

#endif /*            */
#endif /*                  */
