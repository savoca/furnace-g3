/*
 * cs35l32.h -- CS35L32 ALSA SoC audio driver
 *
 * Copyright 2012 CirrusLogic, Inc.
 *
 * Author: Brian Austin <brian.austin@cirrus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __CS35L32_H__
#define __CS35L32_H__

#define CS35L32_FIRSTREG	0x01
#define CS35L32_LASTREG		0x1C
#define CS35L32_CHIP_ID		0x00035A32
#define CS35L32_NO_PLACE	0x00	/*              */
#define CS35L32_DEVID_AB	0x01	/*                      */
#define CS35L32_DEVID_CD	0x02    /*                      */
#define CS35L32_DEVID_E		0x03    /*                  */
#define CS35L32_FAB_ID		0x04	/*             */
#define CS35L32_REV_ID		0x05	/*                  */
#define CS35L32_PWRCTL1		0x06    /*             */
#define CS35L32_PWRCTL2		0x07    /*             */
#define CS35L32_CLK_CTL		0x08	/*           */
#define CS35L32_BATT_THRESHOLD	0x09	/*                       */
#define CS35L32_VMON		0x0A	/*                      */
#define CS35L32_BST_CPCP_CTL	0x0B	/*                               */
#define CS35L32_IMON_SCALING	0x0C	/*              */
#define CS35L32_AUDIO_LED_MNGR	0x0D	/*                       */
#define CS35L32_ADSP_CTL	0x0F	/*                     */
#define CS35L32_CLASSD_CTL	0x10	/*                 */
#define CS35L32_PROTECT_CTL	0x11	/*                        */
#define CS35L32_INT_MASK_1	0x12	/*                  */
#define CS35L32_INT_MASK_2	0x13	/*                  */
#define CS35L32_INT_MASK_3	0x14	/*                  */
#define CS35L32_INT_STATUS_1	0x15	/*                         */
#define CS35L32_INT_STATUS_2	0x16	/*                         */
#define CS35L32_INT_STATUS_3	0x17	/*                         */
#define CS35L32_LED_STATUS	0x18	/*                          */
#define CS35L32_FLASH_MODE	0x19	/*                        */
#define CS35L32_MOVIE_MODE	0x1A	/*                        */
#define CS35L32_FLASH_TIMER	0x1B	/*                 */
#define CS35L32_FLASH_INHIBIT	0x1C	/*                           */
#define CS35L32_MAX_REGISTER	0x50
#define CS35L32_REGISTER_COUNT	0x50

#define CS35L32_MCLK_6MHZ	6000000
#define CS35L32_MCLK_6144MHZ   	6144000
#define CS35L32_MCLK_12MHZ	12000000
#define CS35L32_MCLK_12288MHZ	12288000
#define CS35L32_MCLK_DIV2	0x01
#define CS35L32_MCLK_RATIO	0x01
#define CS35L32_MCLKDIS		(1 << 7)
#define CS35L32_PDN_ALL		(1 << 0)
#define CS35L32_PDN_AMP		(1 << 7)
#define CS35L32_PDN_BOOST	(1 << 2)
#define CS35L32_PDN_IMON	(1 << 6)
#define CS35L32_PDN_VMON	(1 << 7)
#define CS35L32_PDN_VPMON	(1 << 5)
#define CS35L32_PDN_ADSP	(1 << 3)

#define CS35L32_MCLK_DIV2_MASK		0x40
#define CS35L32_MCLK_RATIO_MASK		0x01
#define CS35L32_MCLK_MASK		0x41
#define CS35L32_ADSP_MASTER_MASK	0x40
#define CS35L32_BOOST_MASK		0x03
#define CS35L32_ADSP_SHARE_MASK		0x08
#define CS35L32_ADSP_DATACFG_MASK	0x30
#define CS35L32_SDOUT_3ST		0x80

#define CS35L32_RATES (SNDRV_PCM_RATE_48000) 
#define CS35L32_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | \
			SNDRV_PCM_FMTBIT_S24_LE | \
			SNDRV_PCM_FMTBIT_S32_LE)

#endif