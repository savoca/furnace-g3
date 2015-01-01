/**
   @copyright
   Copyright (c) 2011 - 2013, INSIDE Secure Oy. All rights reserved.
*/

/*
                                                                      
                                                                   
                                                                 
                                                           

                               

                                                             
                          
                   
                                         
                      
                          
                            
              
                        
                      
                       
               

                                                                      
                                          
*/

#ifndef IMPLEMENTATIONDEFS_H
#define IMPLEMENTATIONDEFS_H

#include "public_defs.h"
#include "implementation_config.h"

/*
        
*/

/*
          
 */
#if !defined( __STDC_VERSION__) || __STDC_VERSION__ < 199901L

#ifdef __GNUC__
#define restrict __restrict__
#else
#define restrict
#endif

#endif

/*         

                                                

        

                                                                 
                                                                     
                                                
*/
#undef MIN
#undef MAX
#define MIN(_x, _y) ((_x) < (_y) ? (_x) : (_y))
#define MAX(_x, _y) ((_x) > (_y) ? (_x) : (_y))

/*
                  

                                          
*/
#define ARRAY_ITEM_COUNT(array) (sizeof (array) / sizeof((array)[0]))

/*      

                                   

                                                 
                                              
*/
#define BIT_0   0x00000001U
#define BIT_1   0x00000002U
#define BIT_2   0x00000004U
#define BIT_3   0x00000008U
#define BIT_4   0x00000010U
#define BIT_5   0x00000020U
#define BIT_6   0x00000040U
#define BIT_7   0x00000080U
#define BIT_8   0x00000100U
#define BIT_9   0x00000200U
#define BIT_10  0x00000400U
#define BIT_11  0x00000800U
#define BIT_12  0x00001000U
#define BIT_13  0x00002000U
#define BIT_14  0x00004000U
#define BIT_15  0x00008000U
#define BIT_16  0x00010000U
#define BIT_17  0x00020000U
#define BIT_18  0x00040000U
#define BIT_19  0x00080000U
#define BIT_20  0x00100000U
#define BIT_21  0x00200000U
#define BIT_22  0x00400000U
#define BIT_23  0x00800000U
#define BIT_24  0x01000000U
#define BIT_25  0x02000000U
#define BIT_26  0x04000000U
#define BIT_27  0x08000000U
#define BIT_28  0x10000000U
#define BIT_29  0x20000000U
#define BIT_30  0x40000000U
#define BIT_31  0x80000000U
#define BIT_ALL 0xffffffffU

/*          

                                               
*/
#define BIT_CLEAR(__bits, __bit)                                \
    do {                                                        \
        uint32_t  *__bit_p = (uint32_t *)(void*) &(__bits);     \
        *__bit_p &= ~(__bit);                                   \
    } while (0)

/*        

                                                
*/
#define BIT_SET(__bits, bit)                                    \
    do {                                                        \
        uint32_t  *__bit_p = (uint32_t *)(void*) &(__bits);     \
        *__bit_p |= (bit);                                      \
    } while (0)

/*           

                                                            
                     
*/
#define BIT_IS_SET(__bits, __bit) (((__bits) & (__bit)) != 0)

/*           

                                                                      
                                      

                                                                    
                                             
*/
#define ALIGNED_TO(Value, Alignment)                                    \
    ((((((Alignment) & (Alignment - 1)) == 0) &&                        \
       ((uintptr_t)(Value) & (Alignment - 1))) == 0) ? true : false)

/*                     

                                                                   
                                                         
*/
#define PARAMETER_NOT_USED(__identifier)        \
    do { if (__identifier) {} } while (0)

/*                      

                                                                
                                         
                                        
*/
#define IDENTIFIER_NOT_USED(__identifier)        \
    do { if (__identifier) {} } while (0)

/*     
 */
#ifndef NULL
#define NULL 0
#endif

/*         
 */
#ifndef offsetof
#define offsetof(type, member) ((size_t) &(((type *) NULL)->member))
#endif

/*            
 */
#define alignmentof(type) (offsetof(struct { char x; type y; }, y))

/*
                                                        

                                     
                                     
                                     
                                     
                                     
                                     

*/

#ifdef INTTYPES_HEADER
#include INTTYPES_HEADER
#endif

/*               
 */
#ifdef STRING_HEADER
#include STRING_HEADER
#endif

/*         
 */
#ifdef SNPRINTF_HEADER
#include SNPRINTF_HEADER
#endif

/*     

                                                             
                                                 

                     
                                                            
*/
#define TODO(__todo) TODO_IMPLEMENTATION(__todo)

/*           
             
               
            

                                                              
                                                                      
                                                                      
                                                      

                                       

                                                                        
                                    

       

                                                                 
                                                               
                                        

       

                                                     

         

                                                                 
       

      

                               

                                    
                                                                
                                                     

                                                                 
                                                           
                                                        

                                                                
                             
*/
#define DEBUG_FAIL(flow, ...)   DEBUG_IMPLEMENTATION(FAIL, flow, __VA_ARGS__)
#define DEBUG_HIGH(flow, ...)   DEBUG_IMPLEMENTATION(HIGH, flow, __VA_ARGS__)
#define DEBUG_MEDIUM(flow, ...) DEBUG_IMPLEMENTATION(MEDIUM, flow, __VA_ARGS__)
#define DEBUG_LOW(flow, ...)    DEBUG_IMPLEMENTATION(LOW, flow, __VA_ARGS__)

#define DEBUG_DUMP(flow, dumper, data, len, ...) \
  DEBUG_DUMP_IMPLEMENTATION(flow, dumper, data, len, __VA_ARGS__)

#define DEBUG_DUMP_LINE(context, ...) \
  DEBUG_DUMP_LINE_IMPLEMENTATION(context, __VA_ARGS__)

/*
                  

                                                                     
                                 

                                           
                                      

               

             
                  
                                
                       
                                     
                               
*/
#define DEBUG_STRBUF_GET() DEBUG_STRBUF_GET_IMPLEMENTATION()

/*
                   

                                                                     

               

                                                              
 */
#define DEBUG_STRBUF_TYPE DEBUG_STRBUF_TYPE_IMPLEMENTATION

/*
                            

                                                                      
                                                               
                                                               

                                                                     
                                                                      
                                          

               

                                                                 
   
                                            

                    
       
                                                  

                   
       

         
   
*/
#define DEBUG_STRBUF_ALLOC(dsb, n) DEBUG_STRBUF_ALLOC_IMPLEMENTATION(dsb, n)

/*
                                            
                                      

                                                                  
            

                                                                   

                                                               
                                                                      
                                          

                                                                  
                                                    
                               

               

                                                                 
   
              
            

                                             

                                                     

                                             

               
   
*/
#define DEBUG_STRBUF_BUFFER_GET(dsb, buf_p, len_p)           \
  DEBUG_STRBUF_BUFFER_GET_IMPLEMENTATION(dsb, buf_p, len_p)

#define DEBUG_STRBUF_BUFFER_COMMIT(dsb, len)                 \
  DEBUG_STRBUF_BUFFER_COMMIT_IMPLEMENTATION(dsb, len)

/*
                    

                                                                  
               

               

                                                                 
   
                                            

                    
       
                                  
       

         
   
*/
#define DEBUG_STRBUF_ERROR DEBUG_STRBUF_ERROR_IMPLEMENTATION

#define STR_BOOL(b) STR_BOOL_IMPLEMENTATION(b)

#define STR_HEXBUF(buf, len) STR_HEXBUF_IMPLEMENTATION(buf, len)

/*         

                                               
*/
#define ASSERT(__condition)                   \
    ASSERT_IMPLEMENTATION(                    \
            (__condition),                    \
            "assertion failed")

/*             

                                                                      
                                                                   
                                                             
                                                                     
                                                                     
                                                                    
                              
*/
#define PRECONDITION(__condition)            \
    ASSERT_IMPLEMENTATION(                   \
            (__condition),                   \
            "precondition failed")

/*              

                                                                
                      
*/
#define POSTCONDITION(__condition)           \
    ASSERT_IMPLEMENTATION(                   \
            (__condition),                   \
            "postcondition failed")

/*        

                                                                      
                                                                          
                                                        
*/
#define PANIC(...)                           \
    PANIC_IMPLEMENTATION(__VA_ARGS__)

/*                      

                                                                    
                                                                    
                                                                  
                                

                                                            

                                                           
*/
#define COMPILE_GLOBAL_ASSERT(condition)                \
  COMPILE_GLOBAL_ASSERT_IMPLEMENTATION(condition)

/*                      

                                                                      
                                                                    
                                                                  
                                

                                                            

                                                           
*/
#define COMPILE_STATIC_ASSERT(condition)                \
    COMPILE_STATIC_ASSERT_IMPLEMENTATION(condition)

#ifdef DEBUG_IMPLEMENTATION_HEADER
#include DEBUG_IMPLEMENTATION_HEADER
#endif

#endif				/*                      */
