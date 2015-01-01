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

/*
                      
                                                                           
                                
 */
static inline size_t lz4_compressbound(size_t isize)
{
	return isize + (isize / 255) + 16;
}

/*
                   
                                                  
                                                                       
                                                        
                                                                          
                                
                           
                                                        
                                                           
 */
int lz4_decompress(const char *src, size_t *src_len, char *dest,
		size_t actual_dest_len);

/*
                                     
                                                  
                                                             
                                                        
                                                                
                                                         
                    
                                
                           
                                                        
 */
int lz4_decompress_unknownoutputsize(const char *src, size_t src_len,
		char *dest, size_t *dest_len);
#endif
