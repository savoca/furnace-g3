/* Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _MPQ_DMX_PLUGIN_COMMON_H
#define _MPQ_DMX_PLUGIN_COMMON_H

#include <linux/msm_ion.h>

#include "dvbdev.h"
#include "dmxdev.h"
#include "demux.h"
#include "dvb_demux.h"
#include "dvb_frontend.h"
#include "mpq_adapter.h"
#include "mpq_sdmx.h"

/*                                                       */
#define MPQ_MAX_DMX_FILES				128

/* 
                         
 */
#define TSIF_NAME_LENGTH				20

/* 
                                                    
                                                 
 */
struct ts_packet_header {
#if defined(__BIG_ENDIAN_BITFIELD)
	unsigned sync_byte:8;
	unsigned transport_error_indicator:1;
	unsigned payload_unit_start_indicator:1;
	unsigned transport_priority:1;
	unsigned pid_msb:5;
	unsigned pid_lsb:8;
	unsigned transport_scrambling_control:2;
	unsigned adaptation_field_control:2;
	unsigned continuity_counter:4;
#elif defined(__LITTLE_ENDIAN_BITFIELD)
	unsigned sync_byte:8;
	unsigned pid_msb:5;
	unsigned transport_priority:1;
	unsigned payload_unit_start_indicator:1;
	unsigned transport_error_indicator:1;
	unsigned pid_lsb:8;
	unsigned continuity_counter:4;
	unsigned adaptation_field_control:2;
	unsigned transport_scrambling_control:2;
#else
#error "Please fix <asm/byteorder.h>"
#endif
} __packed;

/* 
                                                       
                                                 
 */
struct ts_adaptation_field {
#if defined(__BIG_ENDIAN_BITFIELD)
	unsigned adaptation_field_length:8;
	unsigned discontinuity_indicator:1;
	unsigned random_access_indicator:1;
	unsigned elementary_stream_priority_indicator:1;
	unsigned PCR_flag:1;
	unsigned OPCR_flag:1;
	unsigned splicing_point_flag:1;
	unsigned transport_private_data_flag:1;
	unsigned adaptation_field_extension_flag:1;
	unsigned program_clock_reference_base_1:8;
	unsigned program_clock_reference_base_2:8;
	unsigned program_clock_reference_base_3:8;
	unsigned program_clock_reference_base_4:8;
	unsigned program_clock_reference_base_5:1;
	unsigned reserved:6;
	unsigned program_clock_reference_ext_1:1;
	unsigned program_clock_reference_ext_2:8;
#elif defined(__LITTLE_ENDIAN_BITFIELD)
	unsigned adaptation_field_length:8;
	unsigned adaptation_field_extension_flag:1;
	unsigned transport_private_data_flag:1;
	unsigned splicing_point_flag:1;
	unsigned OPCR_flag:1;
	unsigned PCR_flag:1;
	unsigned elementary_stream_priority_indicator:1;
	unsigned random_access_indicator:1;
	unsigned discontinuity_indicator:1;
	unsigned program_clock_reference_base_1:8;
	unsigned program_clock_reference_base_2:8;
	unsigned program_clock_reference_base_3:8;
	unsigned program_clock_reference_base_4:8;
	unsigned program_clock_reference_ext_1:1;
	unsigned reserved:6;
	unsigned program_clock_reference_base_5:1;
	unsigned program_clock_reference_ext_2:8;
#else
#error "Please fix <asm/byteorder.h>"
#endif
} __packed;


/*
                                                     
                                                 
 */
struct pes_packet_header {
#if defined(__BIG_ENDIAN_BITFIELD)
	unsigned packet_start_code_prefix_1:8;
	unsigned packet_start_code_prefix_2:8;
	unsigned packet_start_code_prefix_3:8;
	unsigned stream_id:8;
	unsigned pes_packet_length_msb:8;
	unsigned pes_packet_length_lsb:8;
	unsigned reserved_bits0:2;
	unsigned pes_scrambling_control:2;
	unsigned pes_priority:1;
	unsigned data_alignment_indicator:1;
	unsigned copyright:1;
	unsigned original_or_copy:1;
	unsigned pts_dts_flag:2;
	unsigned escr_flag:1;
	unsigned es_rate_flag:1;
	unsigned dsm_trick_mode_flag:1;
	unsigned additional_copy_info_flag:1;
	unsigned pes_crc_flag:1;
	unsigned pes_extension_flag:1;
	unsigned pes_header_data_length:8;
	unsigned reserved_bits1:4;
	unsigned pts_1:3;
	unsigned marker_bit0:1;
	unsigned pts_2:8;
	unsigned pts_3:7;
	unsigned marker_bit1:1;
	unsigned pts_4:8;
	unsigned pts_5:7;
	unsigned marker_bit2:1;
	unsigned reserved_bits2:4;
	unsigned dts_1:3;
	unsigned marker_bit3:1;
	unsigned dts_2:8;
	unsigned dts_3:7;
	unsigned marker_bit4:1;
	unsigned dts_4:8;
	unsigned dts_5:7;
	unsigned marker_bit5:1;
	unsigned reserved_bits3:4;
#elif defined(__LITTLE_ENDIAN_BITFIELD)
	unsigned packet_start_code_prefix_1:8;
	unsigned packet_start_code_prefix_2:8;
	unsigned packet_start_code_prefix_3:8;
	unsigned stream_id:8;
	unsigned pes_packet_length_lsb:8;
	unsigned pes_packet_length_msb:8;
	unsigned original_or_copy:1;
	unsigned copyright:1;
	unsigned data_alignment_indicator:1;
	unsigned pes_priority:1;
	unsigned pes_scrambling_control:2;
	unsigned reserved_bits0:2;
	unsigned pes_extension_flag:1;
	unsigned pes_crc_flag:1;
	unsigned additional_copy_info_flag:1;
	unsigned dsm_trick_mode_flag:1;
	unsigned es_rate_flag:1;
	unsigned escr_flag:1;
	unsigned pts_dts_flag:2;
	unsigned pes_header_data_length:8;
	unsigned marker_bit0:1;
	unsigned pts_1:3;
	unsigned reserved_bits1:4;
	unsigned pts_2:8;
	unsigned marker_bit1:1;
	unsigned pts_3:7;
	unsigned pts_4:8;
	unsigned marker_bit2:1;
	unsigned pts_5:7;
	unsigned marker_bit3:1;
	unsigned dts_1:3;
	unsigned reserved_bits2:4;
	unsigned dts_2:8;
	unsigned marker_bit4:1;
	unsigned dts_3:7;
	unsigned dts_4:8;
	unsigned marker_bit5:1;
	unsigned dts_5:7;
	unsigned reserved_bits3:4;
#else
#error "Please fix <asm/byteorder.h>"
#endif
} __packed;

/* 
                                                                       
  
                                                                            
                                                                          
                                       
                                                                           
                                                                             
                               
                                                                              
                                               
 */
struct mpq_decoder_buffers_desc {
	struct mpq_streambuffer_buffer_desc desc[DMX_MAX_DECODER_BUFFER_NUM];
	struct ion_handle *ion_handle[DMX_MAX_DECODER_BUFFER_NUM];
	u32 decoder_buffers_num;
};

/*
                                                          
  
                                                       
                                                              
                                                                   
                                                                      
                                                                        
                                               
                                                                            
                                                           
                                             
                                                                      
                                                             
                                                                    
              
                                                                  
                      
                                                                     
                           
                                                          
                                             
                                                                             
                                                                               
                                                          
                                                                      
                           
                                                        
                                                       
                                                                         
                                                          
                                                                   
                                                             
                                                                      
              
                                                                         
                                                                         
                                                                        
                                                                     
                                                                  
                              
                                                                          
                                        
                                                                             
                                                                                
                                                                     
                                                                   
                                                               
                                                                            
                                                                               
                                                                       
                                                                             
                     
                                                               
                                       
                                                                            
                
                                                                               
                                      
                                                          
 */
struct mpq_video_feed_info {
	struct mpq_streambuffer *video_buffer;
	spinlock_t video_buffer_lock;
	struct mpq_decoder_buffers_desc buffer_desc;
	struct pes_packet_header pes_header;
	u32 pes_header_left_bytes;
	u32 pes_header_offset;
	int fullness_wait_cancel;
	enum mpq_adapter_stream_if stream_interface;
	const struct dvb_dmx_video_patterns
		*patterns[DVB_DMX_MAX_SEARCH_PATTERN_NUM];
	int patterns_num;
	char prev_pattern[DVB_DMX_MAX_PATTERN_LEN];
	u32 frame_offset;
	u32 last_pattern_offset;
	u32 pending_pattern_len;
	u64 last_framing_match_type;
	u64 last_framing_match_stc;
	int found_sequence_header_pattern;
	struct dvb_dmx_video_prefix_size_masks prefix_size;
	u32 first_prefix_size;
	struct dmx_pts_dts_info saved_pts_dts_info;
	struct dmx_pts_dts_info new_pts_dts_info;
	int saved_info_used;
	int new_info_exists;
	int first_pts_dts_copy;
	u32 tei_errs;
	int last_continuity;
	u32 continuity_errs;
	u32 ts_packets_num;
	u32 ts_dropped_bytes;
	int last_pkt_index;
	u64 prev_stc;
};

/* 
                                                       
  
                                                               
                                                 
                                             
                                                                            
                     
                                                                    
              
                                                                     
                                                                  
                                                                          
                                
                                                                           
                                                                            
                                               
 */
struct mpq_feed {
	struct dvb_demux_feed *dvb_demux_feed;
	struct mpq_demux *mpq_demux;
	void *plugin_priv;

	/*                      */
	int sdmx_filter_handle;
	int secondary_feed;
	enum sdmx_filter filter_type;
	struct dvb_ringbuffer metadata_buf;
	struct ion_handle *metadata_buf_handle;

	u8 session_id;
	struct dvb_ringbuffer sdmx_buf;
	struct ion_handle *sdmx_buf_handle;

	struct mpq_video_feed_info video_info;
};

/* 
                                           
                                                   
                                                 
                                                             
                                                     
                                                           
                                       
                                                                  
                                                         
                                      
                                                     
                                                
                                                                            
                       
                                                                     
                                                         
                                                            
                                                                    
                                                                    
                
                                                            
                      
                                                                        
                      
                                                                  
                                                                         
                                                                 
                      
                                                                          
                                       
                                                                          
                             
                                                                              
                                                          
                                                                     
                                                      
                                                             
                                                      
                                                                         
                           
                                                                            
                                      
                                                                     
                                                         
                                                               
                                                       
                                                                        
                                                                              
                                                                          
                                                           
                                                         
 */
struct mpq_demux {
	struct dvb_demux demux;
	struct dmxdev dmxdev;
	struct dmx_frontend fe_memory;
	dmx_source_t source;
	int is_initialized;
	struct ion_client *ion_client;
	struct mutex mutex;
	struct mpq_feed feeds[MPQ_MAX_DMX_FILES];
	void *plugin_priv;

	u32 num_active_feeds;
	u32 num_secure_feeds;
	int sdmx_session_handle;
	int sdmx_session_ref_count;
	int sdmx_filter_count;
	int sdmx_eos;
	struct {
		/*                     */
		struct sdmx_filter_status status[MPQ_MAX_DMX_FILES];

		/*                                             */
		u8 mpq_feed_idx[MPQ_MAX_DMX_FILES];

		/*
                                       
                                               
                                          
                                            
                                                 
                                
   */
		u8 session_id[MPQ_MAX_DMX_FILES];
	} sdmx_filters_state;

	/*          */
	u32 hw_notification_interval;
	u32 hw_notification_min_interval;
	u32 hw_notification_count;
	u32 hw_notification_size;
	u32 hw_notification_min_size;
	u32 decoder_drop_count;
	u32 decoder_out_count;
	u32 decoder_out_interval_sum;
	u32 decoder_out_interval_average;
	u32 decoder_out_interval_max;
	u32 decoder_ts_errors;
	u32 decoder_cc_errors;
	u32 sdmx_process_count;
	u32 sdmx_process_time_sum;
	u32 sdmx_process_time_average;
	u32 sdmx_process_time_max;
	u32 sdmx_process_packets_sum;
	u32 sdmx_process_packets_average;
	u32 sdmx_process_packets_min;
	enum sdmx_log_level sdmx_log_level;

	struct timespec decoder_out_last_time;
	struct timespec last_notification_time;
};

/* 
                                                             
                          
  
                                                 
                                          
  
                                                          
                                                            
                                                            
                                                               
 */
typedef int (*mpq_dmx_init)(
				struct dvb_adapter *mpq_adapter,
				struct mpq_demux *demux);

/* 
                                                                
                           
  
                                                     
                                                    
  
                        
  
                                                           
 */
int mpq_dmx_plugin_init(mpq_dmx_init dmx_init_func);

/* 
                                                   
  
                                                        
 */
void mpq_dmx_plugin_exit(void);

/* 
                                                             
  
                                              
                              
  
                        
  
                                                            
                                
 */
int mpq_dmx_set_source(struct dmx_demux *demux, const dmx_source_t *src);

/* 
                                                                
  
                            
                                                              
                                    
                                                                          
                                                          
  
                        
  
                                                                     
                                                                    
                                 
 */
int mpq_dmx_map_buffer(struct dmx_demux *demux, struct dmx_buffer *dmx_buffer,
		void **priv_handle, void **kernel_mem);

/* 
                                                                           
  
                            
                                                                           
  
                        
  
                                                                       
                                      
 */
int mpq_dmx_unmap_buffer(struct dmx_demux *demux, void *priv_handle);

/* 
                                                     
                                          
  
                            
  
                         
 */
int mpq_dmx_decoder_fullness_init(struct dvb_demux_feed *feed);

/* 
                                                                
                                                    
  
                            
                                                       
  
                         
 */
int mpq_dmx_decoder_fullness_wait(struct dvb_demux_feed *feed,
		size_t required_space);

/* 
                                                  
                                                      
                                                       
                                      
  
                            
  
                         
 */
int mpq_dmx_decoder_fullness_abort(struct dvb_demux_feed *feed);

/* 
                                              
                                  
  
                            
                                                 
  
                         
 */
int mpq_dmx_decoder_buffer_status(struct dvb_demux_feed *feed,
		struct dmx_buffer_status *dmx_buffer_status);

/* 
                                                                            
                        
  
                             
                                               
  
                        
  
                                                                 
                                                                
 */
int mpq_dmx_reuse_decoder_buffer(struct dvb_demux_feed *feed, int cookie);

/* 
                                                                 
                                                 
  
                                                
                                            
  
                         
  
                                                                
                       
                                                                  
                                                           
                                  
 */
int mpq_dmx_process_video_packet(struct dvb_demux_feed *feed, const u8 *buf);

/* 
                                                          
                      
  
                                              
                                           
  
                         
  
                                                                
                                               
                                                     
  
                    
                                                       
  
                                                                
        
 */
int mpq_dmx_process_pcr_packet(struct dvb_demux_feed *feed, const u8 *buf);

/* 
                                 
                                                                              
                    
  
                                                  
 */
void mpq_dmx_init_debugfs_entries(struct mpq_demux *mpq_demux);

/* 
                                 
                                                            
  
                                              
 */
void mpq_dmx_update_hw_statistics(struct mpq_demux *mpq_demux);

/* 
                                                                
  
                                               
                                           
  
                                                               
                                                                       
                        
  
                    
 */
int mpq_dmx_set_cipher_ops(struct dvb_demux_feed *feed,
		struct dmx_cipher_operations *cipher_ops);

/* 
                                                                    
                   
  
                                    
                                                              
                                               
  
                    
 */
void mpq_dmx_convert_tts(struct dvb_demux_feed *feed,
		const u8 timestamp[TIMESTAMP_LEN],
		u64 *timestampIn27Mhz);

/* 
                                                                           
                                                                       
                                                                         
  
                                 
  
                    
 */
int mpq_sdmx_open_session(struct mpq_demux *mpq_demux);

/* 
                                                                    
                                                                
  
                                 
  
                    
 */
int mpq_sdmx_close_session(struct mpq_demux *mpq_demux);

/* 
                                                        
                                                                         
              
  
                                              
  
                    
 */
int mpq_dmx_init_mpq_feed(struct dvb_demux_feed *feed);

/* 
                                                      
  
                                              
  
                    
 */
int mpq_dmx_terminate_feed(struct dvb_demux_feed *feed);

/* 
                                                         
  
                                                                       
                                                                              
                            
  
                          
                     
                                               
  
                                                 
 */
int mpq_dmx_write(struct dmx_demux *demux, const char *buf, size_t count);

/* 
                                                                            
                               
  
                                 
                                  
                                                                     
                                             
                                      
  
                                            
 */
int mpq_sdmx_process(struct mpq_demux *mpq_demux,
	struct sdmx_buff_descr *input,
	u32 fill_count,
	u32 read_offset,
	size_t tsp_size);

/* 
                                                                     
                                                                        
                                            
 */
int mpq_sdmx_is_loaded(void);

/* 
                                                            
  
                                                      
                                                                            
                                                      
                                                                            
                             
  
                               
                         
  
                                
 */
int mpq_dmx_oob_command(struct dvb_demux_feed *feed,
	struct dmx_oob_command *cmd);

#endif /*                          */

