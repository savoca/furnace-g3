#ifndef __LZ4_H__
#define __LZ4_H__
/*
 * LZ4 Kernel Interface
 *
 * Copyright (C) 2013, LG Electronics, Kyungsik Lee <kyungsik.lee@lge.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#define LZ4_MEM_COMPRESS	(4096 * sizeof(unsigned char *))
#define LZ4HC_MEM_COMPRESS	(65538 * sizeof(unsigned char *))

/*
                      
                                                                           
                                
 */
static inline size_t lz4_compressbound(size_t isize)
{
	return isize + (isize / 255) + 16;
}

/*
                 
                                                
                                      
                                                     
                                                  
                                                                      
                                           
                                                     
                                
                           
                                                                        
                     
 */
int lz4_compress(const unsigned char *src, size_t src_len,
		unsigned char *dst, size_t *dst_len, void *wrkmem);

 /*
                    
                                               
                                        
                                                        
                                                   
                                                                        
                                             
                                                        
                                  
                             
                                                                          
                       
  */
int lz4hc_compress(const unsigned char *src, size_t src_len,
		unsigned char *dst, size_t *dst_len, void *wrkmem);

/*
                   
                                                  
                                                                       
                                                        
                                                                          
                                
                           
                                                        
                                                           
 */
int lz4_decompress(const char *src, size_t *src_len, char *dest,
		size_t actual_dest_len);

/*
                                     
                                                  
                                                             
                                                        
                                                                
                                                         
                    
                                
                           
                                                        
 */
int lz4_decompress_unknownoutputsize(const char *src, size_t src_len,
		char *dest, size_t *dest_len);
#endif
