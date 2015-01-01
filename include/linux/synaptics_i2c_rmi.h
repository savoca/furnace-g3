/*
 * include/linux/synaptics_i2c_rmi.h - platform data structure for f75375s sensor
 *
 * Copyright (C) 2008 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _LINUX_SYNAPTICS_I2C_RMI_H
#define _LINUX_SYNAPTICS_I2C_RMI_H

#define SYNAPTICS_I2C_RMI_NAME "synaptics-rmi-ts"

enum {
	SYNAPTICS_FLIP_X = 1UL << 0,
	SYNAPTICS_FLIP_Y = 1UL << 1,
	SYNAPTICS_SWAP_XY = 1UL << 2,
	SYNAPTICS_SNAP_TO_INACTIVE_EDGE = 1UL << 3,
};

struct synaptics_i2c_rmi_platform_data {
	uint32_t version;	/*                                */
				/*                                        */
				/*                                         */
	int (*power)(int on);	/*                                 */
	uint32_t flags;
	unsigned long irqflags;
	uint32_t inactive_left; /*                        */
	uint32_t inactive_right; /*                        */
	uint32_t inactive_top; /*                         */
	uint32_t inactive_bottom; /*                         */
	uint32_t snap_left_on; /*                        */
	uint32_t snap_left_off; /*                        */
	uint32_t snap_right_on; /*                        */
	uint32_t snap_right_off; /*                        */
	uint32_t snap_top_on; /*                         */
	uint32_t snap_top_off; /*                         */
	uint32_t snap_bottom_on; /*                         */
	uint32_t snap_bottom_off; /*                         */
	uint32_t fuzz_x; /*                        */
	uint32_t fuzz_y; /*                         */
	int fuzz_p;
	int fuzz_w;
	int8_t sensitivity_adjust;
};

#endif /*                            */
