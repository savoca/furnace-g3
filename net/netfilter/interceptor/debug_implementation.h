/**
   @copyright
   Copyright (c) 2011 - 2013, INSIDE Secure Oy. All rights reserved.
*/

#ifndef DEBUG_IMPLEMENTATION_H
#define DEBUG_IMPLEMENTATION_H

/*
                                                                 

                                                

                                                            
                                                                
                         

                   

                                                             
                                                

                                                            
                                                 

                                                           
                        

                                               

                                                              
                                                          
                    

                                                           
                                                                  

                           

                                                                   
                             
                    
                                                                

                           

                                                                     
                    
                                                        

                                                 

                                                                     
                                                                        

                                                                   
                                    

                                                 

                                                                     
                                                                      

                                                                   
                                    
*/

#define STR_BOOL_IMPLEMENTATION(b) ((bool)(b) ? "true" : "false")

#define STR_HEXBUF_IMPLEMENTATION(buf, len)             \
  debug_str_hexbuf(DEBUG_STRBUF_GET(), (buf), (len))

/*             

                                                                 
                                                  
                                                           
                                                                
*/
#define __FILELINE__ __FILE__ ":" L_TOSTRING(__LINE__)

#define __DEBUG_CONCAT(x, y) x ## y
#define DEBUG_CONCAT(x, y) __DEBUG_CONCAT(x,y)

#define DEBUG_OUTPUT_FORMAT_WRAP(arg) DEBUG_OUTPUT_FORMAT arg

#define DEBUG_OUTPUT_FORMAT(func, format, ...)           \
    format "%s\n", func, __VA_ARGS__

#define DEBUG_STRINGIFY(x) #x

#define DEBUG_TOSTRING(x) DEBUG_STRINGIFY(x)

#define DEBUG_FULL_FORMAT_STRING(level, flow, module, file, line, func, ...) \
    DEBUG_OUTPUT_FORMAT_WRAP(                                                \
            (                                                                \
                    func,                                                    \
                    DEBUG_STRINGIFY(level) ", "                              \
                    DEBUG_STRINGIFY(flow) ", "                               \
                    DEBUG_STRINGIFY(module) ", "                             \
                    file  ":"                                                \
                    DEBUG_STRINGIFY(line) ": "                               \
                    "%s: "                                                   \
                    __VA_ARGS__, ""                                          \
             )                                                               \
    )

#define DEBUG_STRBUF_TYPE_IMPLEMENTATION \
  struct DebugStrbuf *

#define DEBUG_STRBUF_GET_IMPLEMENTATION() \
  &local_debug_str_buf

#define DEBUG_STRBUF_ALLOC_IMPLEMENTATION(dsb, n) \
  debug_strbuf_get_block((dsb), (n))

#define DEBUG_STRBUF_BUFFER_GET_IMPLEMENTATION(dsb, buf_p, len_p)       \
  debug_strbuf_buffer_get(dsb, buf_p, len_p)

#define DEBUG_STRBUF_BUFFER_COMMIT_IMPLEMENTATION(dsb, len)     \
  debug_strbuf_buffer_commit(dsb, len)

#define DEBUG_STRBUF_ERROR_IMPLEMENTATION \
  debug_strbuf_error

#ifdef DEBUG_LIGHT

#define DEBUG_IMPLEMENTATION(level, flow, ...)                          \
({                                                                      \
  struct DebugStrbuf local_debug_str_buf;                               \
                                                                        \
  debug_strbuf_reset(&local_debug_str_buf);                             \
                                                                        \
  debug_outputf(                                                        \
          DEBUG_STRINGIFY(level),                                       \
          DEBUG_STRINGIFY(flow),                                        \
          DEBUG_TOSTRING(__DEBUG_MODULE__),                             \
          __FILE__,                                                     \
          __LINE__,                                                     \
          __func__,                                                     \
          __VA_ARGS__);                                                 \
 })

#define ASSERT_IMPLEMENTATION(condition, description)                   \
  ((void) ((condition) ? 0 :                                            \
  (assert_outputf(                                                      \
          DEBUG_STRINGIFY(condition),                                   \
          __FILE__,                                                     \
          __LINE__,                                                     \
          DEBUG_TOSTRING(__DEBUG_MODULE__),                             \
          __func__,                                                     \
          description))))

#define DEBUG_DUMP_IMPLEMENTATION(__flow, __dumper, __data, __len, ...) \
({                                                                      \
  struct DebugDumpContext __debug_dump_context;                         \
  __debug_dump_context.level = "DUMP";                                  \
  __debug_dump_context.flow = DEBUG_STRINGIFY(__flow);                  \
  __debug_dump_context.module = DEBUG_TOSTRING(__DEBUG_MODULE__);       \
  __debug_dump_context.file = __FILE__ ;                                \
  __debug_dump_context.line = __LINE__ ;                                \
  __debug_dump_context.func = __func__ ;                                \
                                                                        \
  DEBUG_IMPLEMENTATION(DUMP,__flow, __VA_ARGS__);                       \
  (__dumper)(&__debug_dump_context, __data, __len);                     \
})

#define DEBUG_DUMP_LINE_IMPLEMENTATION(context, ...)                    \
({                                                                      \
  const struct DebugDumpContext *__debug_dump_context_p = context;      \
  struct DebugStrbuf local_debug_str_buf;                               \
                                                                        \
  debug_strbuf_reset(&local_debug_str_buf);                             \
                                                                        \
  debug_outputf(                                                        \
          __debug_dump_context_p->level,                                \
          __debug_dump_context_p->flow,                                 \
          __debug_dump_context_p->module,                               \
          __debug_dump_context_p->file,                                 \
          __debug_dump_context_p->line,                                 \
          __debug_dump_context_p->func,                                 \
          __VA_ARGS__);                                                 \
})

#else

#define DEBUG_IMPLEMENTATION(level, flow, ...)
#define ASSERT_IMPLEMENTATION(condition, description)
#define DEBUG_DUMP_IMPLEMENTATION(flow, dumper, data, len, ...)
#define DEBUG_DUMP_LINE_IMPLEMENTATION(context, ...)

#endif

#define COMPILE_STATIC_ASSERT_IMPLEMENTATION(condition)                 \
    do {                                                                \
        int DEBUG_CONCAT(static_assertion_,__LINE__)                    \
                [1-2*(!(condition))] = { 1 };                           \
        if (DEBUG_CONCAT(static_assertion_,__LINE__)[0])                \
            ;                                                           \
    } while (0)

#define COMPILE_GLOBAL_ASSERT_IMPLEMENTATION(condition)                 \
  extern int DEBUG_CONCAT(global_assert_,__LINE__) [1 - 2*(!(condition))]

#include "debug_strbuf.h"
#include "debug_outputf.h"

struct DebugDumpContext {
	const char *level;
	const char *flow;
	const char *module;
	const char *file;
	int line;
	const char *func;
};

typedef void
DebugDumper(const struct DebugDumpContext *,
	    const void *data, unsigned bytecount);

DebugDumper debug_dump_hex_bytes;

#endif				/*                        */
