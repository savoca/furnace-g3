/*
 * Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
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

#ifndef __REGULATOR_QPNP_REGULATOR_H__
#define __REGULATOR_QPNP_REGULATOR_H__

#include <linux/regulator/machine.h>

#define QPNP_REGULATOR_DRIVER_NAME "qcom,qpnp-regulator"

/*                                */
#define QPNP_REGULATOR_PIN_CTRL_ENABLE_NONE		0x00
#define QPNP_REGULATOR_PIN_CTRL_ENABLE_EN0		0x01
#define QPNP_REGULATOR_PIN_CTRL_ENABLE_EN1		0x02
#define QPNP_REGULATOR_PIN_CTRL_ENABLE_EN2		0x04
#define QPNP_REGULATOR_PIN_CTRL_ENABLE_EN3		0x08
#define QPNP_REGULATOR_PIN_CTRL_ENABLE_HW_DEFAULT	0x10

/*                                         */
#define QPNP_REGULATOR_PIN_CTRL_HPM_NONE		0x00
#define QPNP_REGULATOR_PIN_CTRL_HPM_EN0			0x01
#define QPNP_REGULATOR_PIN_CTRL_HPM_EN1			0x02
#define QPNP_REGULATOR_PIN_CTRL_HPM_EN2			0x04
#define QPNP_REGULATOR_PIN_CTRL_HPM_EN3			0x08
#define QPNP_REGULATOR_PIN_CTRL_HPM_SLEEP_B		0x10
#define QPNP_REGULATOR_PIN_CTRL_HPM_HW_DEFAULT		0x20

/*
                                                                               
                            
 */
#define QPNP_REGULATOR_DISABLE				0
#define QPNP_REGULATOR_ENABLE				1
#define QPNP_REGULATOR_USE_HW_DEFAULT			2

/*                                                        */
enum qpnp_vs_soft_start_str {
	QPNP_VS_SOFT_START_STR_0P05_UA,
	QPNP_VS_SOFT_START_STR_0P25_UA,
	QPNP_VS_SOFT_START_STR_0P55_UA,
	QPNP_VS_SOFT_START_STR_0P75_UA,
	QPNP_VS_SOFT_START_STR_HW_DEFAULT,
};

/*                                         */
enum qpnp_boost_current_limit {
	QPNP_BOOST_CURRENT_LIMIT_300_MA,
	QPNP_BOOST_CURRENT_LIMIT_600_MA,
	QPNP_BOOST_CURRENT_LIMIT_900_MA,
	QPNP_BOOST_CURRENT_LIMIT_1200_MA,
	QPNP_BOOST_CURRENT_LIMIT_1500_MA,
	QPNP_BOOST_CURRENT_LIMIT_1800_MA,
	QPNP_BOOST_CURRENT_LIMIT_2100_MA,
	QPNP_BOOST_CURRENT_LIMIT_2400_MA,
	QPNP_BOOST_CURRENT_LIMIT_HW_DEFAULT,
};

/* 
                                                                           
                                     
                                                                         
                                  
                                       
                                                      
                            
                                                                             
                                          
                                    
                                                     
                          
                                                  
                                                     
                           
                                                                             
                                                 
                  
                                    
                                                  
                          
                                               
                                                   
                           
                                                                                
                             
                                                                            
                                                                         
                                                   
                                                 
                                                      
                   
                        
                                                      
                      
                                                                         
                                                      
                                                      
                           
                                                                             
                                           
                                      
                                                                      
                                                 
                        
                                                                               
                                             
                                                  
                                                   
                                                     
                     
                                                                            
                                                   
                                            
                               
                                                      
                             
                                                                           
                                                
                                                    
                                                     
                                                      
                             
                                                                                
                                                    
                              
                                        
                                                      
                            
                                                                               
                                                   
                       
                                      
                                                      
                              
                                                                              
                                                    
                                                      
                                                  
                                                      
                                                  
                                               
                                                      
                                   
                              
                                                      
                      
                                                                          
 */
struct qpnp_regulator_platform_data {
	struct regulator_init_data		init_data;
	int					pull_down_enable;
	unsigned				pin_ctrl_enable;
	unsigned				pin_ctrl_hpm;
	int					system_load;
	int					enable_time;
	int					ocp_enable;
	int					ocp_irq;
	int					ocp_max_retries;
	int					ocp_retry_delay_ms;
	enum qpnp_boost_current_limit		boost_current_limit;
	int					soft_start_enable;
	enum qpnp_vs_soft_start_str		vs_soft_start_strength;
	int					auto_mode_enable;
	int					bypass_mode_enable;
	int					hpm_enable;
	u16					base_addr;
};

#ifdef CONFIG_REGULATOR_QPNP

/* 
                                                                  
  
                                                                           
                                                      
 */
int __init qpnp_regulator_init(void);

#else

static inline int __init qpnp_regulator_init(void)
{
	return -ENODEV;
}

#endif /*                       */

#endif
