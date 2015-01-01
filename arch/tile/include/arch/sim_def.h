/*
 * Copyright 2010 Tilera Corporation. All Rights Reserved.
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 *   NON INFRINGEMENT.  See the GNU General Public License for
 *   more details.
 */

/* 
        
  
                                        
 */

#ifndef __ARCH_SIM_DEF_H__
#define __ARCH_SIM_DEF_H__


/* 
                                                                 
                                                       
                                                  
 */
#define _SIM_CONTROL_OPERATOR_BITS 8


/*
                                                  
 */

/*                                                  */
#define SIM_CONTROL_PROFILER_DISABLE 0

/*                                                   */
#define SIM_CONTROL_PROFILER_ENABLE 1

/*                                                            */
#define SIM_CONTROL_PROFILER_CLEAR 2

/*                                                            */
#define SIM_CONTROL_CHECKPOINT 3

/* 
                                                                      
                                                                    
 */
#define SIM_CONTROL_SET_TRACING 4

/* 
                                                                      
                                                         
 */
#define SIM_CONTROL_DUMP 5

/*                                                                       */
#define SIM_CONTROL_PROFILER_CHIP_CLEAR 6

/*                                                                */
#define SIM_CONTROL_PROFILER_CHIP_DISABLE 7

/*                                                               */
#define SIM_CONTROL_PROFILER_CHIP_ENABLE 8

/*                                                                    */
#define SIM_CONTROL_ENABLE_FUNCTIONAL 9

/*                                                                      */
#define SIM_CONTROL_DISABLE_FUNCTIONAL 10

/* 
                                                                     
                                                                       
                                                               
                                                             
 */
#define SIM_CONTROL_ENABLE_FUNCTIONAL_BARRIER 11

/* 
                                                                           
                                                                         
                                                                   
                                                                       
 */
/*                                                                    */
#define SIM_CONTROL_PUTC 12

/* 
                                                                        
                                                                  
                                                                          
                                                                   
 */
#define SIM_CONTROL_GRINDER_CLEAR 13

/*                                                           */
#define SIM_CONTROL_SHUTDOWN 14

/* 
                                                                     
                                                                
 */
#define SIM_CONTROL_OS_FORK 15

/* 
                                                                     
                                                                         
 */
#define SIM_CONTROL_OS_EXIT 16

/* 
                                                                     
                                                            
 */
#define SIM_CONTROL_OS_SWITCH 17

/* 
                                                                           
                                                                        
                                                           
 */
#define SIM_CONTROL_OS_EXEC 18

/* 
                                                                           
                                                                         
                                                                         
                                                                          
 */
#define SIM_CONTROL_OS_INTERP 19

/* 
                                                                           
                                                                       
                                                                       
                                                                 
 */
#define SIM_CONTROL_DLOPEN 20

/* 
                                                                           
                                                                         
                                                                  
                              
 */
#define SIM_CONTROL_DLCLOSE 21

/* 
                                                                      
                                                           
                                                                     
                                                 
 */
#define SIM_CONTROL_ALLOW_MULTIPLE_CACHING 22

/*                                                         */
#define SIM_CONTROL_ENABLE_MEM_LOGGING 23

/*                                                          */
#define SIM_CONTROL_DISABLE_MEM_LOGGING 24

/* 
                                                                          
                                                                            
                                               
 */
#define SIM_CONTROL_SHAPING 25

/* 
                                                                         
                                                                           
                                              
 */
#define SIM_CONTROL_COMMAND 26

/* 
                                                                     
                                                         
 */
#define SIM_CONTROL_PANIC 27

/* 
                                                               
                                     
 */
#define SIM_CONTROL_SYSCALL 32

/* 
                                                                     
                                                                     
                                                                          
 */
#define SIM_CONTROL_OS_FORK_PARENT 33

/* 
                                                                   
                                                                      
                                                                       
                                                                     
 */
#define SIM_CONTROL_CLEAR_MPIPE_MAGIC_BYTES 34

/* 
                                                                   
                                                                       
                                                                    
                                                                    
 */
#define SIM_CONTROL_APPEND_MPIPE_MAGIC_BYTE 35

/* 
                                                                   
                                                                   
                                                                     
               
 */
#define SIM_CONTROL_ENABLE_MPIPE_LINK_MAGIC_BYTE 36


/*
                                                
 */

/*                                           */
#define SIM_SYSCALL_ADD_WATCHPOINT 2

/*                                              */
#define SIM_SYSCALL_REMOVE_WATCHPOINT 3

/*                                             */
#define SIM_SYSCALL_QUERY_WATCHPOINT 4

/* 
                                                                   
                                                                
                                                              
                                                         
 */
#define SIM_SYSCALL_VALIDATE_LINES_EVICTED 5

/*                                            */
#define SIM_SYSCALL_QUERY_CPU_SPEED 6


/*
                                                     
                                                           
 */

/* 
                       
     
 */

/*                                                               */
#define SIM_TRACE_CYCLES          0x01

/*                                                                */
#define SIM_TRACE_ROUTER          0x02

/*                                                                         */
#define SIM_TRACE_REGISTER_WRITES 0x04

/*                                                                */
#define SIM_TRACE_DISASM          0x08

/*                                                                    */
#define SIM_TRACE_STALL_INFO      0x10

/*                                                                           */
#define SIM_TRACE_MEMORY_CONTROLLER 0x20

/*                                                            */
#define SIM_TRACE_L2_CACHE 0x40

/*                                                               */
#define SIM_TRACE_LINES 0x80

/*                                                               */
#define SIM_TRACE_NONE 0

/*                                                              */
#define SIM_TRACE_ALL (-1)

/*     */

/*                                                                       */
#define SIM_TRACE_SPR_ARG(mask) \
  (SIM_CONTROL_SET_TRACING | ((mask) << _SIM_CONTROL_OPERATOR_BITS))


/*
                                                     
                                                    
 */

/* 
                       
     
 */

/*                                      */
#define SIM_DUMP_REGS          0x001

/*                 */
#define SIM_DUMP_SPRS          0x002

/*                 */
#define SIM_DUMP_ITLB          0x004

/*                 */
#define SIM_DUMP_DTLB          0x008

/*                       */
#define SIM_DUMP_L1I           0x010

/*                       */
#define SIM_DUMP_L1D           0x020

/*                     */
#define SIM_DUMP_L2            0x040

/*                             */
#define SIM_DUMP_SNREGS        0x080

/*                        */
#define SIM_DUMP_SNITLB        0x100

/*                              */
#define SIM_DUMP_SNL1I         0x200

/*                              */
#define SIM_DUMP_BACKTRACE     0x400

/*                                   */
#define SIM_DUMP_VALID_LINES   0x800

/*                                    */
#define SIM_DUMP_ALL (-1 & ~SIM_DUMP_VALID_LINES)

/*     */

/*                                                                        */
#define SIM_DUMP_SPR_ARG(mask) \
  (SIM_CONTROL_DUMP | ((mask) << _SIM_CONTROL_OPERATOR_BITS))


/*
                                                     
                                                                 
 */

/* 
                       
     
 */

/*                                                                         */
#define SIM_CHIP_MEMCTL        0x001

/*                                                                     */
#define SIM_CHIP_XAUI          0x002

/*                                                                     */
#define SIM_CHIP_PCIE          0x004

/*                                                                      */
#define SIM_CHIP_MPIPE         0x008

/*                                                                     */
#define SIM_CHIP_TRIO          0x010

/*                              */
#define SIM_CHIP_ALL (-1)

/*     */

/*                                                                           */
#define SIM_PROFILER_CHIP_CLEAR_SPR_ARG(mask) \
  (SIM_CONTROL_PROFILER_CHIP_CLEAR | ((mask) << _SIM_CONTROL_OPERATOR_BITS))

/*                                                                            */
#define SIM_PROFILER_CHIP_DISABLE_SPR_ARG(mask) \
  (SIM_CONTROL_PROFILER_CHIP_DISABLE | ((mask) << _SIM_CONTROL_OPERATOR_BITS))

/*                                                                            */
#define SIM_PROFILER_CHIP_ENABLE_SPR_ARG(mask) \
  (SIM_CONTROL_PROFILER_CHIP_ENABLE | ((mask) << _SIM_CONTROL_OPERATOR_BITS))



/*                        */

/*                                                */
#define SIM_CONTROL_SHAPING_SHIM_ID_BITS 3

/* 
                       
     
 */

/*                            */
#define SIM_CONTROL_SHAPING_GBE_0 0x0

/*                            */
#define SIM_CONTROL_SHAPING_GBE_1 0x1

/*                            */
#define SIM_CONTROL_SHAPING_GBE_2 0x2

/*                            */
#define SIM_CONTROL_SHAPING_GBE_3 0x3

/*                             */
#define SIM_CONTROL_SHAPING_XGBE_0 0x4

/*                             */
#define SIM_CONTROL_SHAPING_XGBE_1 0x5

/*                             */
#define SIM_CONTROL_SHAPING_TYPE_BITS 2

/*                          */
#define SIM_CONTROL_SHAPING_MULTIPLIER 0

/*                   */
#define SIM_CONTROL_SHAPING_PPS 1

/*                   */
#define SIM_CONTROL_SHAPING_BPS 2

/*                                                              */
#define SIM_CONTROL_SHAPING_UNITS_BITS 2

/*                                    */
#define SIM_CONTROL_SHAPING_UNITS_SINGLE 0

/*                                  */
#define SIM_CONTROL_SHAPING_UNITS_KILO 1

/*                                  */
#define SIM_CONTROL_SHAPING_UNITS_MEGA 2

/*                                  */
#define SIM_CONTROL_SHAPING_UNITS_GIGA 3

/*     */

/*                                            */
#define SIM_CONTROL_SHAPING_RATE_BITS \
  (32 - (_SIM_CONTROL_OPERATOR_BITS + \
         SIM_CONTROL_SHAPING_SHIM_ID_BITS + \
         SIM_CONTROL_SHAPING_TYPE_BITS + \
         SIM_CONTROL_SHAPING_UNITS_BITS))

/*                                                                      */
#define SIM_SHAPING_SPR_ARG(shim, type, units, rate) \
  (SIM_CONTROL_SHAPING | \
   ((shim) | \
   ((type) << (SIM_CONTROL_SHAPING_SHIM_ID_BITS)) | \
   ((units) << (SIM_CONTROL_SHAPING_SHIM_ID_BITS + \
                SIM_CONTROL_SHAPING_TYPE_BITS)) | \
   ((rate) << (SIM_CONTROL_SHAPING_SHIM_ID_BITS + \
               SIM_CONTROL_SHAPING_TYPE_BITS + \
               SIM_CONTROL_SHAPING_UNITS_BITS))) << _SIM_CONTROL_OPERATOR_BITS)


/*
                                                
                                                          
 */

/* 
                                                                   
                          
 */
#define SIM_TRACE_FLAG_MASK 0xFFFF

/*                                                                           */
#define SIM_PROFILER_ENABLED_MASK 0x10000


/*
                                            
 */

/* 
                                                        
                                       
 */
#define SIM_PUTC_FLUSH_STRING 0x100

/* 
                                                                   
                                        
 */
#define SIM_PUTC_FLUSH_BINARY 0x101


#endif /*                    */
