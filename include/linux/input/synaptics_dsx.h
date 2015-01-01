/*
 * Synaptics DSX touchscreen driver
 *
 * Copyright (C) 2012 Synaptics Incorporated
 *
 * Copyright (C) 2012 Alexandra Chin <alexandra.chin@tw.synaptics.com>
 * Copyright (C) 2012 Scott Lin <scott.lin@tw.synaptics.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _SYNAPTICS_DSX_H_
#define _SYNAPTICS_DSX_H_

#define PLATFORM_DRIVER_NAME "synaptics_dsx"
#define I2C_DRIVER_NAME "synaptics_dsx_i2c"
#define SPI_DRIVER_NAME "synaptics_dsx_spi"

/*
                                                      
                                  
                                         
 */
struct synaptics_dsx_cap_button_map {
	unsigned char nbuttons;
	unsigned char *map;
};

/*
                                                   
                       
                       
                                      
                                 
                                             
                          
                                      
                        
                                               
                                               
                                                     
                                                  
                                      
                                                           
                                                        
                                                
                                                       
                                            
 */
struct synaptics_dsx_board_data {
	bool x_flip;
	bool y_flip;
	bool regulator_en;
	bool swap_axes;
	int irq_gpio;
	int power_gpio;
	int power_on_state;
	int reset_gpio;
	int reset_on_state;
	unsigned long irq_flags;
	unsigned int panel_x;
	unsigned int panel_y;
	unsigned int power_delay_ms;
	unsigned int reset_delay_ms;
	unsigned int reset_active_ms;
	unsigned int byte_delay_us;
	unsigned int block_delay_us;
	unsigned char *regulator_name;
	int (*gpio_config)(int gpio, bool configure, int dir, int state);
	struct synaptics_dsx_cap_button_map *cap_button_map;
};

#endif
