/*
 * linux/sound/cs35l32.h -- Platform data for CS35l32
 *
 * Copyright (c) 2012 Cirrus Logic Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __CS35L32_H
#define __CS35L32_H

struct cs35l32_platform_data {
	int gpio_nreset;
	/*           */
	unsigned int mclk_freq;
	/*                       */
	unsigned int batt_thresh;
	/*                      */
	unsigned int batt_recov;
	/*                     */
	unsigned int batt_disable;

	/*                            */
	/*                       */
	unsigned int led_mng;
	/*                   */
	unsigned int audiogain_mng;
	/*                  */
	unsigned int boost_mng;
	
	/*                    */
	unsigned int adsp_drive;
	/*                  */
	unsigned int adsp_clkctl;
	/*                          */
	unsigned int sdout_datacfg;
	/*               */
	unsigned int sdout_share;
	
	/*                                */
	/*                   */
	unsigned int amp_short;
	/*                   */
	unsigned int overtemp_ctl;	
	
};

#endif /*             */
