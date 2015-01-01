/* Copyright (c) 2013, The Linux Foundation. All rights reserved.
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

#include <linux/of.h>
#include <linux/batterydata-lib.h>

#ifdef CONFIG_OF_BATTERYDATA
/* 
                                                                          
                                                                     
                                 
                                                                          
                                      
                                                                        
                                                             
                                                             
                                                       
  
                                                                              
                                                                               
                                        
  
                                                                              
                
 */
int of_batterydata_read_data(struct device_node *container_node,
				struct bms_battery_data *batt_data,
				int batt_id_uv);
#else
static inline int of_batterydata_read_data(struct device_node *container_node,
				struct bms_battery_data *batt_data,
				int batt_id_uv);
{
	return -ENXIO;
}
#endif /*                */
