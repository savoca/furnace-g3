/* Copyright (c) 2011-2013, The Linux Foundation. All rights reserved.
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

#ifndef __PM8XXX_BMS_H
#define __PM8XXX_BMS_H

#include <linux/errno.h>
#include <linux/batterydata-lib.h>

#define PM8921_BMS_DEV_NAME	"pm8921-bms"


struct pm8xxx_bms_core_data {
	unsigned int	batt_temp_channel;
	unsigned int	vbat_channel;
	unsigned int	ref625mv_channel;
	unsigned int	ref1p25v_channel;
	unsigned int	batt_id_channel;
};

/* 
                                    
                                                              
                                                      
                                                                  
                                               
                                                      
                            
                                                                 
                                                
                                                                  
                        
                                                                  
                                
                                                                            
                         
                                                                            
                                        
                                                                         
                                    
                                                                           
                                                                       
                                                                    
                                                                         
                                                                  
                                                                   
                               
                         
                                                                  
                              
                               
                                                                 
                            
 */
struct pm8921_bms_platform_data {
	struct pm8xxx_bms_core_data	bms_cdata;
	enum battery_type		battery_type;
	int				r_sense_uohm;
	unsigned int			i_test;
	unsigned int			v_cutoff;
	unsigned int			max_voltage_uv;
	unsigned int			rconn_mohm;
	unsigned int			alarm_low_mv;
	unsigned int			alarm_high_mv;
	int				enable_fcc_learning;
	int				min_fcc_learning_soc;
	int				min_fcc_ocv_pc;
	int				min_fcc_learning_samples;
	int				shutdown_soc_valid_limit;
	int				ignore_shutdown_soc;
	int				adjust_soc_low_threshold;
	int				chg_term_ua;
	int				normal_voltage_calc_ms;
	int				low_voltage_calc_ms;
	int				disable_flat_portion_ocv;
	int				ocv_dis_high_soc;
	int				ocv_dis_low_soc;
	int				low_voltage_detect;
	int				vbatt_cutoff_retries;
	int				high_ocv_correction_limit_uv;
	int				low_ocv_correction_limit_uv;
	int				hold_soc_est;
};

#if defined(CONFIG_PM8921_BMS) || defined(CONFIG_PM8921_BMS_MODULE)
/* 
                                                                  
                           
                                                                
                                               
                                         
  
                                                                            
                                                    
  
  
 */
int pm8921_bms_get_vsense_avg(int *result);

/* 
                                                                              
                             
                                                                
                                               
                                         
  
                                                                            
                                                    
  
 */
int pm8921_bms_get_battery_current(int *result);

/* 
                                                                                
  
 */
int pm8921_bms_get_percent_charge(void);

/* 
                                                                              
                    
  
 */
int pm8921_bms_get_fcc(void);

/* 
                                                                              
                                                 
                           
 */
void pm8921_bms_charging_began(void);
/* 
                                                                            
                                                 
                           
 */
void pm8921_bms_charging_end(int is_battery_full);

void pm8921_bms_calibrate_hkadc(void);
/* 
                                                          
                                                            
                                                                  
                                                             
 */
int pm8921_bms_get_simultaneous_battery_voltage_and_current(int *ibat_ua,
								int *vbat_uv);
/* 
                             
                                                           
                                                             
 */
int pm8921_bms_get_current_max(void);
/* 
                                                                              
                                              
                                            
                                               
 */
void pm8921_bms_invalidate_shutdown_soc(void);

/* 
                                                                             
                                                        
                                                            
                                                        
                                  
 */
int pm8921_bms_cc_uah(int *cc_uah);

/* 
                                                                          
                                                        
                                     
 */
void pm8921_bms_battery_removed(void);
/* 
                                                                          
                                                             
                       
 */
void pm8921_bms_battery_inserted(void);
#else
static inline int pm8921_bms_get_vsense_avg(int *result)
{
	return -ENXIO;
}
static inline int pm8921_bms_get_battery_current(int *result)
{
	return -ENXIO;
}
static inline int pm8921_bms_get_percent_charge(void)
{
	return -ENXIO;
}
static inline int pm8921_bms_get_fcc(void)
{
	return -ENXIO;
}
static inline void pm8921_bms_charging_began(void)
{
}
static inline void pm8921_bms_charging_end(int is_battery_full)
{
}
static inline void pm8921_bms_calibrate_hkadc(void)
{
}
static inline int pm8921_bms_get_simultaneous_battery_voltage_and_current(
						int *ibat_ua, int *vbat_uv)
{
	return -ENXIO;
}
static inline int pm8921_bms_get_rbatt(void)
{
	return -EINVAL;
}
static inline void pm8921_bms_invalidate_shutdown_soc(void)
{
}
static inline int pm8921_bms_cc_uah(int *cc_uah)
{
	return -ENXIO;
}
static inline int pm8921_bms_get_current_max(void)
{
	return -ENXIO;
}
static inline void pm8921_bms_battery_removed(void) {}
static inline void pm8921_bms_battery_inserted(void) {}
#endif

#endif
