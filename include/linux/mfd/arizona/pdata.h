/*
 * Platform data for Arizona devices
 *
 * Copyright 2012 Wolfson Microelectronics. PLC.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _ARIZONA_PDATA_H
#define _ARIZONA_PDATA_H

#define ARIZONA_GPN_DIR                          0x8000  /*         */
#define ARIZONA_GPN_DIR_MASK                     0x8000  /*         */
#define ARIZONA_GPN_DIR_SHIFT                        15  /*         */
#define ARIZONA_GPN_DIR_WIDTH                         1  /*         */
#define ARIZONA_GPN_PU                           0x4000  /*        */
#define ARIZONA_GPN_PU_MASK                      0x4000  /*        */
#define ARIZONA_GPN_PU_SHIFT                         14  /*        */
#define ARIZONA_GPN_PU_WIDTH                          1  /*        */
#define ARIZONA_GPN_PD                           0x2000  /*        */
#define ARIZONA_GPN_PD_MASK                      0x2000  /*        */
#define ARIZONA_GPN_PD_SHIFT                         13  /*        */
#define ARIZONA_GPN_PD_WIDTH                          1  /*        */
#define ARIZONA_GPN_LVL                          0x0800  /*         */
#define ARIZONA_GPN_LVL_MASK                     0x0800  /*         */
#define ARIZONA_GPN_LVL_SHIFT                        11  /*         */
#define ARIZONA_GPN_LVL_WIDTH                         1  /*         */
#define ARIZONA_GPN_POL                          0x0400  /*         */
#define ARIZONA_GPN_POL_MASK                     0x0400  /*         */
#define ARIZONA_GPN_POL_SHIFT                        10  /*         */
#define ARIZONA_GPN_POL_WIDTH                         1  /*         */
#define ARIZONA_GPN_OP_CFG                       0x0200  /*            */
#define ARIZONA_GPN_OP_CFG_MASK                  0x0200  /*            */
#define ARIZONA_GPN_OP_CFG_SHIFT                      9  /*            */
#define ARIZONA_GPN_OP_CFG_WIDTH                      1  /*            */
#define ARIZONA_GPN_DB                           0x0100  /*        */
#define ARIZONA_GPN_DB_MASK                      0x0100  /*        */
#define ARIZONA_GPN_DB_SHIFT                          8  /*        */
#define ARIZONA_GPN_DB_WIDTH                          1  /*        */
#define ARIZONA_GPN_FN_MASK                      0x007F  /*                */
#define ARIZONA_GPN_FN_SHIFT                          0  /*                */
#define ARIZONA_GPN_FN_WIDTH                          7  /*                */

#define ARIZONA_MAX_GPIO 5

#define ARIZONA_32KZ_MCLK1 1
#define ARIZONA_32KZ_MCLK2 2
#define ARIZONA_32KZ_NONE  3

#define ARIZONA_MIC_CLAMP_SPKLN 1
#define ARIZONA_MIC_CLAMP_SPKLP 2
#define ARIZONA_MIC_CLAMP_SPKRN 3
#define ARIZONA_MIC_CLAMP_SPKRP 4

#define ARIZONA_MAX_INPUT 4

#define ARIZONA_DMIC_MICVDD   0
#define ARIZONA_DMIC_MICBIAS1 1
#define ARIZONA_DMIC_MICBIAS2 2
#define ARIZONA_DMIC_MICBIAS3 3

#define ARIZONA_MAX_MICBIAS 3

#define ARIZONA_INMODE_DIFF 0
#define ARIZONA_INMODE_SE   1
#define ARIZONA_INMODE_DMIC 2

#define ARIZONA_MAX_OUTPUT 6

#define ARIZONA_HAP_ACT_ERM 0
#define ARIZONA_HAP_ACT_LRA 2

#define ARIZONA_MAX_PDM_SPK 2

#define ARIZONA_NUM_MICD_LEVEL 4

#define ARIZONA_MAX_AIF 3

struct regulator_init_data;

struct arizona_micbias {
	int mV;                    /*                    */
	unsigned int ext_cap:1;    /*                            */
	unsigned int discharge:1;  /*                     */
	unsigned int soft_start:1; /*                                       */
	unsigned int bypass:1;     /*                  */
};

struct arizona_micd_config {
	unsigned int src;
	unsigned int bias;
	bool gpio;
};

struct arizona_micd_range {
	int max;  /*       */
	int key;  /*                               */
};

struct arizona_pdata {
	int reset;      /*                                  */
	int ldoena;     /*                                  */

	/*                                     */
	struct regulator_init_data *micvdd;

	/*                                   */
	struct regulator_init_data *ldo1;

	/*                                         */
	int control_init_time;

	/*                                                                 */
	int clk32k_src;

	/*                                                */
	unsigned int irq_flags;

	/*           */
	int gpio_base;

	/*                          */
	int gpio_defaults[ARIZONA_MAX_GPIO];

	/*                                   */
	bool jd_gpio5;

	/*                                                                  */
	bool jd_gpio5_nopull;

	/*                                                             */
	bool hpdet_acc_id;

	/*                                          */
	bool hpdet_acc_id_line;

	/*                                         */
	int hpdet_id_gpio;

	/*                                  */
	void (*hpdet_cb)(unsigned int measurement);

	/*                                                                */
	int micd_detect_debounce;

	/*                                  */
	int micd_pol_gpio;

	/*                       */
	int micd_bias_start_time;

	/*                         */
	int micd_rate;

	/*                            */
	int micd_dbtime;

	/*                          */
	int micd_timeout;

	/*                                  */
	bool micd_force_micbias;

	/*                              */
	const struct arizona_micd_range *micd_ranges;
	int num_micd_ranges;

	/*                                  */
	struct arizona_micd_config *micd_configs;
	int num_micd_configs;

	/*                                    */
	int dmic_ref[ARIZONA_MAX_INPUT];

	/*                         */
	struct arizona_micbias micbias[ARIZONA_MAX_MICBIAS];

	/*                           */
	int inmode[ARIZONA_MAX_INPUT];

	/*                   */
	bool out_mono[ARIZONA_MAX_OUTPUT];

	/*                           */
	unsigned int spk_mute[ARIZONA_MAX_PDM_SPK];

	/*                     */
	unsigned int spk_fmt[ARIZONA_MAX_PDM_SPK];

	/*           */
	int irq_base;

	/*                       */
	unsigned int hap_act;

	/*                                                       */
	unsigned int mic_spk_clamp;

	/* 
                                                            
                                                           
                      
  */
	int max_channels_clocked[ARIZONA_MAX_AIF];

	/*                                         */
	void (*init_done)(void);

	/*                                                           */
	int irq_gpio;

	/*                       */
	unsigned int wm5102t_output_pwr;
};

#endif
