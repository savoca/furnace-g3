/* Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
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

#ifndef __MFD_PM8XXX_REGULATOR_H__
#define __MFD_PM8XXX_REGULATOR_H__

#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/pm8xxx-regulator.h>

/* 
                                                                       
                                                                   
                                                                   
                                                                      
                                         
                                                                  
                                                                    
                    
                                                                       
                                                                         
                                                    
                                                
                                                                       
                              
  
                                                            
 */
enum pm8xxx_regulator_type {
	PM8XXX_REGULATOR_TYPE_PLDO,
	PM8XXX_REGULATOR_TYPE_NLDO,
	PM8XXX_REGULATOR_TYPE_NLDO1200,
	PM8XXX_REGULATOR_TYPE_SMPS,
	PM8XXX_REGULATOR_TYPE_FTSMPS,
	PM8XXX_REGULATOR_TYPE_VS,
	PM8XXX_REGULATOR_TYPE_VS300,
	PM8XXX_REGULATOR_TYPE_NCP,
	PM8XXX_REGULATOR_TYPE_BOOST,
	PM8XXX_REGULATOR_TYPE_MAX,
};

/* 
                                                                          
                           
                                 
                                                                       
                                                         
               
                         
                                                                      
                                 
                                             
                                                                     
                                                                         
                     
                                                                           
                     
                                                                           
                                          
                                                                          
                            
                                                               
                                                            
                                                            
                       
                                                                 
                                    
                                            
                                                           
                                                                     
                                                             
                                                            
                                             
                                        
                                                                     
                                                            
                                       
                                                                          
                                                             
                                                        
               
                                                                         
                                                                         
                                                             
        
                                                                      
                      
                                                                      
                                                                      
             
                                                                        
             
                                                                    
             
                                                              
                           
  
                                                                                
                                                                   
                                                                             
                                                                           
                                                                             
                         
 */
struct pm8xxx_vreg {
	/*                    */
	struct regulator_desc			rdesc;
	struct regulator_desc			rdesc_pc;
	enum pm8xxx_regulator_type		type;
	const int				hpm_min_load;
	const u16				ctrl_addr;
	const u16				test_addr;
	const u16				clk_ctrl_addr;
	const u16				sleep_ctrl_addr;
	const u16				pfm_ctrl_addr;
	const u16				pwr_cnfg_addr;
	/*            */
	struct pm8xxx_regulator_platform_data	pdata;
	struct regulator_dev			*rdev;
	struct regulator_dev			*rdev_pc;
	struct device				*dev;
	struct device				*dev_pc;
	struct mutex				pc_lock;
	int					save_uV;
	int					mode;
	u32					write_count;
	u32					prev_write_count;
	bool					is_enabled;
	bool					is_enabled_pc;
	u8				test_reg[REGULATOR_TEST_BANKS_MAX];
	u8					ctrl_reg;
	u8					clk_ctrl_reg;
	u8					sleep_ctrl_reg;
	u8					pfm_ctrl_reg;
	u8					pwr_cnfg_reg;
};

/* 
                                                                            
                                                                
                                                                  
                                                          
                                                           
                                              
                                                               
                                                                             
                                         
 
                                                                  
*/
struct pm8xxx_regulator_core_platform_data {
	struct pm8xxx_vreg			*vreg;
	struct pm8xxx_regulator_platform_data	*pdata;
	bool					is_pin_controlled;
};

/*               */
#define PLDO(_name, _pc_name, _ctrl_addr, _test_addr, _hpm_min_load) \
	{ \
		.type		= PM8XXX_REGULATOR_TYPE_PLDO, \
		.ctrl_addr	= _ctrl_addr, \
		.test_addr	= _test_addr, \
		.hpm_min_load	= PM8XXX_VREG_##_hpm_min_load##_HPM_MIN_LOAD, \
		.rdesc.name	= _name, \
		.rdesc_pc.name	= _pc_name, \
		.write_count	= 0, \
		.prev_write_count = -1, \
	}

#define NLDO(_name, _pc_name, _ctrl_addr, _test_addr, _hpm_min_load) \
	{ \
		.type		= PM8XXX_REGULATOR_TYPE_NLDO, \
		.ctrl_addr	= _ctrl_addr, \
		.test_addr	= _test_addr, \
		.hpm_min_load	= PM8XXX_VREG_##_hpm_min_load##_HPM_MIN_LOAD, \
		.rdesc.name	= _name, \
		.rdesc_pc.name	= _pc_name, \
		.write_count	= 0, \
		.prev_write_count = -1, \
	}

#define NLDO1200(_name, _ctrl_addr, _test_addr, _hpm_min_load) \
	{ \
		.type		= PM8XXX_REGULATOR_TYPE_NLDO1200, \
		.ctrl_addr	= _ctrl_addr, \
		.test_addr	= _test_addr, \
		.hpm_min_load	= PM8XXX_VREG_##_hpm_min_load##_HPM_MIN_LOAD, \
		.rdesc.name	= _name, \
		.write_count	= 0, \
		.prev_write_count = -1, \
	}

#define SMPS(_name, _pc_name, _ctrl_addr, _test_addr, _clk_ctrl_addr, \
	     _sleep_ctrl_addr, _hpm_min_load) \
	{ \
		.type		= PM8XXX_REGULATOR_TYPE_SMPS, \
		.ctrl_addr	= _ctrl_addr, \
		.test_addr	= _test_addr, \
		.clk_ctrl_addr	= _clk_ctrl_addr, \
		.sleep_ctrl_addr = _sleep_ctrl_addr, \
		.hpm_min_load	= PM8XXX_VREG_##_hpm_min_load##_HPM_MIN_LOAD, \
		.rdesc.name	= _name, \
		.rdesc_pc.name	= _pc_name, \
		.write_count	= 0, \
		.prev_write_count = -1, \
	}

#define FTSMPS(_name, _pwm_ctrl_addr, _fts_cnfg1_addr, _pfm_ctrl_addr, \
	       _pwr_cnfg_addr, _hpm_min_load) \
	{ \
		.type		= PM8XXX_REGULATOR_TYPE_FTSMPS, \
		.ctrl_addr	= _pwm_ctrl_addr, \
		.test_addr	= _fts_cnfg1_addr, \
		.pfm_ctrl_addr = _pfm_ctrl_addr, \
		.pwr_cnfg_addr = _pwr_cnfg_addr, \
		.hpm_min_load	= PM8XXX_VREG_##_hpm_min_load##_HPM_MIN_LOAD, \
		.rdesc.name	= _name, \
		.write_count	= 0, \
		.prev_write_count = -1, \
	}

#define VS(_name, _pc_name, _ctrl_addr, _test_addr) \
	{ \
		.type		= PM8XXX_REGULATOR_TYPE_VS, \
		.ctrl_addr	= _ctrl_addr, \
		.test_addr	= _test_addr, \
		.rdesc.name	= _name, \
		.rdesc_pc.name	= _pc_name, \
		.write_count	= 0, \
		.prev_write_count = -1, \
	}

#define VS300(_name, _ctrl_addr, _test_addr) \
	{ \
		.type		= PM8XXX_REGULATOR_TYPE_VS300, \
		.ctrl_addr	= _ctrl_addr, \
		.test_addr	= _test_addr, \
		.rdesc.name	= _name, \
		.write_count	= 0, \
		.prev_write_count = -1, \
	}

#define NCP(_name, _ctrl_addr) \
	{ \
		.type		= PM8XXX_REGULATOR_TYPE_NCP, \
		.ctrl_addr	= _ctrl_addr, \
		.rdesc.name	= _name, \
		.write_count	= 0, \
		.prev_write_count = -1, \
	}

#define BOOST(_name, _ctrl_addr) \
	{ \
		.type		= PM8XXX_REGULATOR_TYPE_BOOST, \
		.ctrl_addr	= _ctrl_addr, \
		.rdesc.name	= _name, \
		.write_count	= 0, \
		.prev_write_count = -1, \
	}

#endif
