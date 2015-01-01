/* Copyright (c) 2012-2013, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <mach/board.h>
#include <mach/gpio.h>
#include <mach/gpiomux.h>
#include <mach/socinfo.h>
#include <mach/board_lge.h>

//                          
/*                                
                                          
 */
int g_is_tlmm_spare_reg_value = 0;

#define WLAN_CLK	40
#define WLAN_SET	39
#define WLAN_DATA0	38
#define WLAN_DATA1	37
#define WLAN_DATA2	36

static struct gpiomux_setting ap2mdm_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_LOW,
};
#if !defined(CONFIG_LGE_NFC_SONY_KDDI)
static struct gpiomux_setting mdm2ap_status_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting mdm2ap_errfatal_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_LOW,
};
#endif
static struct gpiomux_setting mdm2ap_pblrdy = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_IN,
};


static struct gpiomux_setting ap2mdm_soft_reset_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting ap2mdm_wakeup = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_LOW,
};

static struct msm_gpiomux_config mdm_configs[] __initdata = {
	/*               */
	{
		.gpio = 105,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_cfg,
		}
	},
#if !defined(CONFIG_LGE_NFC_SONY_KDDI)
	/*               */
	{
		.gpio = 46,
		.settings = {
			[GPIOMUX_SUSPENDED] = &mdm2ap_status_cfg,
		}
	},
#endif
#if !defined(CONFIG_LGE_NFC_SONY_KDDI)
	/*                 */
	{
		.gpio = 82,
		.settings = {
			[GPIOMUX_SUSPENDED] = &mdm2ap_errfatal_cfg,
		}
	},
#endif
	/*                 */
	{
		.gpio = 106,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_cfg,
		}
	},
	/*                                           */
	{
		.gpio = 24,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_soft_reset_cfg,
		}
	},
	/*               */
	{
		.gpio = 104,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_wakeup,
		}
	},
	/*                 */
	{
		.gpio = 80,
		.settings = {
			[GPIOMUX_SUSPENDED] = &mdm2ap_pblrdy,
		}
	},
};

#if defined(CONFIG_LGE_NFC_SONY_KDDI)
static struct gpiomux_setting gpio_uart_felica = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting gpio_uart_felica_active = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting felica_pon_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_LOW,
};
static struct gpiomux_setting felica_int_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting felica_rfs_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting felica_lockcont_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_LOW,
};
static struct gpiomux_setting nfc_hsel_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_LOW,
};
static struct gpiomux_setting snfc_intu_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_IN,
};
static struct gpiomux_setting snfc_hvdd_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_LOW,
};
#endif
static struct gpiomux_setting gpio_uart_config = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_16MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting slimbus = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_KEEPER,
};

#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
/*
                                                 
                         
                        
                           
  
*/
static struct gpiomux_setting gpio_spi_cs2_config = {
	.func = GPIOMUX_FUNC_4,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting gpio_spi_config = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gpio_spi_cs1_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};
/*
                                                      
  
                          
               
                                          
   
   
  
*/
#endif

static struct gpiomux_setting gpio_suspend_config[] = {
	{
		.func = GPIOMUX_FUNC_GPIO,  /*       */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},
	{
		.func = GPIOMUX_FUNC_GPIO,  /*       */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
		.dir = GPIOMUX_OUT_LOW,
	},
};

static struct gpiomux_setting gpio_epm_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

/*                                                                    */
#if defined (CONFIG_BCMDHD) || defined (CONFIG_BCMDHD_MODULE)
#else
static struct gpiomux_setting wcnss_5wire_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting wcnss_5wire_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv  = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#endif
/*                                                                    */
static struct gpiomux_setting wcnss_5gpio_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting wcnss_5gpio_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};

/*                          */
#if defined (CONFIG_LGE_BROADCAST_JFULLSEG)
static struct gpiomux_setting lge_fullseg_i2c_config_active = {
	/*                                                                          */
	.func = GPIOMUX_FUNC_3,
	/*
                                                                  
                                                                     
           
  */
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting lge_fullseg_i2c_config_suspend = {
	/*                                                                          */
	.func = GPIOMUX_FUNC_3,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting lge_fullseg_ctrl_pin_suspend = {
//                                              
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting lge_fullseg_tsif_config_active = {
	/*                                                                          */
	.func = GPIOMUX_FUNC_1,
};

static struct gpiomux_setting lge_fullseg_tsif_config_suspend = {
	/*                                                                         */
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_IN,
};
#endif
/*                          */

/*                          */
#if defined (CONFIG_LGE_BROADCAST_JFULLSEG)
static struct msm_gpiomux_config lge_fullseg_blsp8_i2c_tsif_configs[] __initdata = {
	{
		.gpio		= 47,				/*                          */
		.settings = {
			[GPIOMUX_ACTIVE]	= &lge_fullseg_i2c_config_active,
			[GPIOMUX_SUSPENDED] = &lge_fullseg_i2c_config_suspend,
		},
	},
	{
		.gpio		= 48,				/*                          */
		.settings = {
			[GPIOMUX_ACTIVE]	= &lge_fullseg_i2c_config_active,
			[GPIOMUX_SUSPENDED] = &lge_fullseg_i2c_config_suspend,
		},
	},
	{
		.gpio	   = 75,					/*                 */
		.settings = {
			[GPIOMUX_SUSPENDED] = &lge_fullseg_ctrl_pin_suspend,
		},
	},
	{
		.gpio	   = 76,					/*                */
		.settings = {
			[GPIOMUX_SUSPENDED] = &lge_fullseg_ctrl_pin_suspend,
		},
	},
	/* 
                                     
               
                                                       
    
   */

	{
		.gpio		= 89,				/*                   */
		.settings = {
			[GPIOMUX_ACTIVE]	= &lge_fullseg_tsif_config_active,
			[GPIOMUX_SUSPENDED] = &lge_fullseg_tsif_config_suspend,
		},
	},
	{
		.gpio		= 90,				/*                  */
		.settings = {
			[GPIOMUX_ACTIVE]	= &lge_fullseg_tsif_config_active,
			[GPIOMUX_SUSPENDED] = &lge_fullseg_tsif_config_suspend,
		},
	},
	{
		.gpio		= 91,				/*                    */
		.settings = {
			[GPIOMUX_ACTIVE]	= &lge_fullseg_tsif_config_active,
			[GPIOMUX_SUSPENDED] = &lge_fullseg_tsif_config_suspend,
		},
	},
	{
		.gpio		= 92,				/*                    */
		.settings = {
			[GPIOMUX_ACTIVE]	= &lge_fullseg_tsif_config_active,
			[GPIOMUX_SUSPENDED] = &lge_fullseg_tsif_config_suspend,
		},
	},
};
#endif
/*                          */

static struct gpiomux_setting gpio_i2c_config = {
	.func = GPIOMUX_FUNC_3,
	/*
                                                                  
                                                                     
           
  */
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting lcd_en_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting lcd_en_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_IN,
};

#ifdef CONFIG_MACH_LGE
#else
static struct gpiomux_setting atmel_resout_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting atmel_resout_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting atmel_int_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting atmel_int_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#endif

static struct gpiomux_setting taiko_reset = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting taiko_int = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

/*                                                        */
//                       
#if 0
/*                                                   */
static struct gpiomux_setting hap_lvl_shft_suspended_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting hap_lvl_shft_active_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct msm_gpiomux_config hap_lvl_shft_config[] __initdata = {
	{
		.gpio = 86,
		.settings = {
			[GPIOMUX_SUSPENDED] = &hap_lvl_shft_suspended_config,
			[GPIOMUX_ACTIVE] = &hap_lvl_shft_active_config,
		},
	},
};
#endif

#ifdef CONFIG_LGE_IRDA_KDDI
static struct gpiomux_setting irda_pwdn_suspended_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting irda_pwdn_active_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting gpio_irda_tx_active_config = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting gpio_irda_tx_suspended_config = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct msm_gpiomux_config irda_pwdn_config[] __initdata = {
	{
		.gpio = 29,
		.settings = {
			[GPIOMUX_SUSPENDED] = &irda_pwdn_suspended_config,
			[GPIOMUX_ACTIVE] = &irda_pwdn_active_config,
		},
	},
	{
		.gpio	   = 85, 		/*                */
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_irda_tx_active_config,
			[GPIOMUX_SUSPENDED] = &gpio_irda_tx_suspended_config,
		},
	},
	{
		.gpio	   = 86, 		/*                */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_uart_config,
		},
	},
};

static struct gpiomux_setting irda_rc_pwen_suspended_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting irda_rc_pwen_active_config = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_DOWN,
};

static struct msm_gpiomux_config irda_rc_pwen_config[] __initdata = {
	{
        .gpio      = 69,    /*                      */
        .settings = {
            [GPIOMUX_ACTIVE] = &irda_rc_pwen_active_config,
            [GPIOMUX_SUSPENDED] = &irda_rc_pwen_suspended_config,
        },
    },

};

#endif

#if defined(CONFIG_BACKLIGHT_LM3631)
static struct gpiomux_setting lcd_bl_en_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting lcd_bl_en_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_IN,
};
#endif

#ifdef CONFIG_MACH_LGE
#ifdef CONFIG_MAX17050_FUELGAUGE
static struct gpiomux_setting max17050_i2c_sda_config = {
	/*        */
	.func = GPIOMUX_FUNC_3,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting max17050_i2c_scl_config = {
	/*        */
	.func = GPIOMUX_FUNC_3,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting max17050_int_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.dir = GPIOMUX_IN,
};
#endif

#ifdef CONFIG_MAX17048_FUELGAUGE
static struct gpiomux_setting max17048_i2c_sda_config = {
	/*        */
	.func = GPIOMUX_FUNC_3,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting max17048_i2c_scl_config = {
	/*        */
	.func = GPIOMUX_FUNC_3,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
#endif

static struct gpiomux_setting touch_int_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting touch_int_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting touch_i2c_act_cfg = {
	.func = GPIOMUX_FUNC_3,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting touch_i2c_sus_cfg = {
	.func = GPIOMUX_FUNC_3,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting touch_reset_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting touch_reset_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting touch_ldoen_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting touch_ldoen_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.dir = GPIOMUX_IN,
};
#endif

static struct msm_gpiomux_config msm_touch_configs[] __initdata = {
	{
		.gpio      = 8,		/*             */
		.settings = {
			[GPIOMUX_ACTIVE] = &touch_reset_act_cfg,
			[GPIOMUX_SUSPENDED] = &touch_reset_sus_cfg,
		},
	},
	{
		.gpio      = 73,		/*           */
		.settings = {
			[GPIOMUX_ACTIVE] = &touch_int_act_cfg,
			[GPIOMUX_SUSPENDED] = &touch_int_sus_cfg,
		},
	},

};

#ifdef CONFIG_MACH_LGE
static struct gpiomux_setting hsic_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_OUT_LOW,
};
#else /*              */
static struct gpiomux_setting hsic_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#endif

#if 0
static struct gpiomux_setting hsic_act_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};
#endif
static struct gpiomux_setting hsic_hub_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};

#ifndef CONFIG_MACH_LGE
static struct gpiomux_setting hsic_resume_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting hsic_resume_susp_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};
#endif

static struct msm_gpiomux_config msm_hsic_hub_configs[] = {
	{
		.gpio = 50,               /*                */
		.settings = {
			[GPIOMUX_ACTIVE] = &hsic_hub_act_cfg,
			[GPIOMUX_SUSPENDED] = &hsic_sus_cfg,
		},
	},
};

static struct gpiomux_setting hall_ic_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};

static struct msm_gpiomux_config msm_hall_ic_configs[] = {
	{
		.gpio = 144,
		.settings = {
			[GPIOMUX_ACTIVE] = &hall_ic_act_cfg,
		},
	},
};

static struct gpiomux_setting mhl_suspend_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting mhl_active_1_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting hdmi_suspend_1_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting hdmi_suspend_2_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting hdmi_active_1_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting hdmi_active_2_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_16MA,
	.pull = GPIOMUX_PULL_DOWN,
};

#if defined(CONFIG_MACH_LGE)
#if defined(CONFIG_MAX17050_FUELGAUGE)
static struct msm_gpiomux_config
		msm_fuel_gauge_max17050_configs[] __initdata = {
		{
		.gpio      = 2,		/*                    */
		.settings = {
			[GPIOMUX_ACTIVE]    = &max17050_i2c_sda_config,
			[GPIOMUX_SUSPENDED] = &max17050_i2c_sda_config,
		},
	},
	{
		.gpio      = 3,		/*                    */
		.settings = {
			[GPIOMUX_ACTIVE]    = &max17050_i2c_scl_config,
			[GPIOMUX_SUSPENDED] = &max17050_i2c_scl_config,
		},
	},
	{
		.gpio      = 9,		/*                  */
		.settings = {
			[GPIOMUX_ACTIVE]    = &max17050_int_config,
			[GPIOMUX_SUSPENDED] = &max17050_int_config,
		},
	},
};
#endif

#if defined(CONFIG_MAX17048_FUELGAUGE)
static struct msm_gpiomux_config msm_fuel_gauge_configs[] __initdata = {
		{
		.gpio      = 2,		/*                    */
		.settings = {
			[GPIOMUX_ACTIVE]    = &max17048_i2c_sda_config,
			[GPIOMUX_SUSPENDED] = &max17048_i2c_sda_config,
		},
	},
	{
		.gpio      = 3,		/*                    */
		.settings = {
			[GPIOMUX_ACTIVE]    = &max17048_i2c_scl_config,
			[GPIOMUX_SUSPENDED] = &max17048_i2c_scl_config,
		},
	},
};
#endif
#endif

static struct msm_gpiomux_config msm_mhl_configs[] __initdata = {
	{
		/*                 */
		.gpio = 12,
		.settings = {
			[GPIOMUX_SUSPENDED] = &mhl_suspend_config,
			[GPIOMUX_ACTIVE]    = &mhl_active_1_cfg,
		},
	},
#if !defined(CONFIG_LGE_NFC_SONY_KDDI)
	{
		/*                  */
		.gpio = 82,
		.settings = {
			[GPIOMUX_SUSPENDED] = &mhl_suspend_config,
			[GPIOMUX_ACTIVE]    = &mhl_active_1_cfg,
		},
	},
#endif
};


static struct msm_gpiomux_config msm_hdmi_configs[] __initdata = {
	{
		.gpio = 31,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_1_cfg,
		},
	},
	{
		.gpio = 32,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_1_cfg,
		},
	},
	{
		.gpio = 33,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_1_cfg,
		},
	},
	{
		.gpio = 34,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_2_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_2_cfg,
		},
	},
};

#ifndef CONFIG_MACH_LGE
static struct gpiomux_setting gpio_uart7_active_cfg = {
	.func = GPIOMUX_FUNC_3,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gpio_uart7_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct msm_gpiomux_config msm_blsp2_uart7_configs[] __initdata = {
	{
		.gpio	= 41,	/*                */
		.settings = {
			[GPIOMUX_ACTIVE]    = &gpio_uart7_active_cfg,
			[GPIOMUX_SUSPENDED] = &gpio_uart7_suspend_cfg,
		},
	},
	{
		.gpio	= 42,	/*                */
		.settings = {
			[GPIOMUX_ACTIVE]    = &gpio_uart7_active_cfg,
			[GPIOMUX_SUSPENDED] = &gpio_uart7_suspend_cfg,
		},
	},
	{
		.gpio	= 43,	/*                 */
		.settings = {
			[GPIOMUX_ACTIVE]    = &gpio_uart7_active_cfg,
			[GPIOMUX_SUSPENDED] = &gpio_uart7_suspend_cfg,
		},
	},
	{
		.gpio	= 44,	/*                 */
		.settings = {
			[GPIOMUX_ACTIVE]    = &gpio_uart7_active_cfg,
			[GPIOMUX_SUSPENDED] = &gpio_uart7_suspend_cfg,
		},
	},
};
#endif

static struct msm_gpiomux_config msm_rumi_blsp_configs[] __initdata = {
#if !defined(CONFIG_LGE_NFC_SONY_KDDI)
	{
		.gpio      = 45,	/*                */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_uart_config,
		},
	},
	{
		.gpio      = 46,	/*                */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_uart_config,
		},
	},
#endif
};

static struct msm_gpiomux_config msm_lcd_configs[] __initdata = {
	{
		.gpio = 58,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcd_en_act_cfg,
			[GPIOMUX_SUSPENDED] = &lcd_en_sus_cfg,
		},
	},
#if defined(CONFIG_BACKLIGHT_LM3631)
	{
		.gpio = 60, /*           */
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcd_bl_en_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcd_bl_en_suspend_cfg,
		},
	},
#endif

};

#if defined(CONFIG_LGE_NFC_SONY_KDDI)
static struct msm_gpiomux_config msm8974_nfc_configs_before_revb[] __initdata ={
		/*            */
		{
			.gpio = 74,
			.settings = {
				[GPIOMUX_SUSPENDED] = &felica_pon_cfg,
			},
		},

		/*            */
		{
			.gpio = 102,
                     .settings = {
                         [GPIOMUX_SUSPENDED] = &felica_rfs_cfg,
                     },
               },
               {
                     .gpio = 46,
                     .settings = {
                         [GPIOMUX_SUSPENDED] = &felica_int_cfg,
                     },
               },
               {
                     .gpio = 45,
                     .settings = {
                         [GPIOMUX_SUSPENDED] = &felica_lockcont_cfg,
                     },
               },
               {
                     .gpio = 94,
                     .settings = {
                         [GPIOMUX_SUSPENDED] = &nfc_hsel_cfg,
                     },
               },
               {
                     .gpio = 59,
                     .settings = {
                         [GPIOMUX_SUSPENDED] = &snfc_intu_cfg,
                     },
               },
               {
                     .gpio = 145,
                     .settings = {
                         [GPIOMUX_SUSPENDED] = &snfc_hvdd_cfg,
                     },
               },
};
static struct msm_gpiomux_config msm8974_nfc_configs_after_revb[] __initdata ={
        {
            .gpio = 74,
            .settings = {
                [GPIOMUX_SUSPENDED] = &felica_pon_cfg,
            },
        },
        {
            .gpio = 82,
            .settings = {
                [GPIOMUX_SUSPENDED] = &felica_rfs_cfg,
            },
        },
        /*            */
        {
            .gpio = 46,
            .settings = {
                [GPIOMUX_SUSPENDED] = &felica_int_cfg,
            },
        },
        /*            */
        {
            .gpio = 45,
            .settings = {
                [GPIOMUX_SUSPENDED] = &felica_lockcont_cfg,
            },
         },
         /*          */
         {
            .gpio = 94,
            .settings = {
                [GPIOMUX_SUSPENDED] = &nfc_hsel_cfg,
            },
         },
         /*           */
         {
            .gpio = 59,
            .settings = {
                [GPIOMUX_SUSPENDED] = &snfc_intu_cfg,
            },
         },
         /*          */
         {
            .gpio = 145,
            .settings = {
                [GPIOMUX_SUSPENDED] = &snfc_hvdd_cfg,
            },
         },
};
#endif

static struct msm_gpiomux_config msm_blsp_configs[] __initdata = {
#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
	{
		.gpio      = 0,		/*                         */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_spi_config,
		},
	},
	{
		.gpio      = 1,		/*                         */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_spi_config,
		},
	},
	{
		.gpio      = 3,		/*                   */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_spi_config,
		},
	},
	{
		.gpio      = 9,		/*                      */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_spi_cs2_config,
		},
	},
	{
		.gpio      = 8,		/*                     */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_spi_cs1_config,
		},
	},
#endif

	{
		.gpio      = 6,		/*                    */
		.settings = {
			[GPIOMUX_SUSPENDED] = &touch_i2c_sus_cfg,
			[GPIOMUX_ACTIVE] = &touch_i2c_act_cfg,
		},
	},
	{
		.gpio      = 7,		/*                    */
		.settings = {
			[GPIOMUX_SUSPENDED] = &touch_i2c_sus_cfg,
			[GPIOMUX_ACTIVE] = &touch_i2c_act_cfg,
		},
	},
	{
		.gpio      = 83,		/*                    */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},
	{
		.gpio      = 84,		/*                    */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},

#ifdef CONFIG_MACH_LGE
#else
	{
		.gpio      = 5,			/*               */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_uart_config,
		},
	},
#endif
#ifdef CONFIG_MACH_LGE
	{
		.gpio      = 0,			/*               */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_uart_config,
		},
	},
	{
		.gpio      = 1,			/*               */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_uart_config,
		},
	},
#endif
/*                                                        */
//                      
#if 0
	{
		.gpio	   = 85, 		/*               */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_uart_config,
		},
	},
	{
		.gpio	   = 86, 		/*               */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_uart_config,
		},
	},
#endif

/*                                                        */
//                            
#if 0
	{
		.gpio      = 53,		/*                          */
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_spi_config,
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[1],
		},
	},
	{
		.gpio      = 54,		/*                          */
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_spi_config,
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[1],
		},
	},
	{
		.gpio      = 56,		/*                    */
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_spi_config,
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[0],
		},
	},
	{
		.gpio      = 55,		/*                      */
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_spi_config,
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[0],
		},
	},
#endif /*                      */
/*                                                        */
	{
		.gpio      = 81,		/*            */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_epm_config,
		},
	},
};

static struct msm_gpiomux_config msm8974_slimbus_config[] __initdata = {
	{
		.gpio	= 70,		/*             */
		.settings = {
			[GPIOMUX_SUSPENDED] = &slimbus,
		},
	},
	{
		.gpio	= 71,		/*              */
		.settings = {
			[GPIOMUX_SUSPENDED] = &slimbus,
		},
	},
};

static struct gpiomux_setting cam_settings[] = {
	{
		.func = GPIOMUX_FUNC_1, /*        */ /*   */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_1, /*       */ /*   */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},

	{
		.func = GPIOMUX_FUNC_1, /*           */ /*   */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_KEEPER,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*        */ /*   */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*         */ /*   */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},
};

static struct gpiomux_setting sd_card_det_active_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting sd_card_det_sleep_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};
static struct msm_gpiomux_config sd_card_det __initdata = {
#ifdef CONFIG_MACH_LGE
	.gpio = 95,
#else
	.gpio = 62,
#endif
	.settings = {
		[GPIOMUX_ACTIVE]    = &sd_card_det_active_config,
		[GPIOMUX_SUSPENDED] = &sd_card_det_sleep_config,
	},
};

#if defined(CONFIG_LGE_NFC_SONY_KDDI)
static struct msm_gpiomux_config msm8974_felica_uart_blsp1_configs[] __initdata = {
	{
		.gpio      = 53,		/*                */
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_uart_felica_active,
			[GPIOMUX_SUSPENDED] = &gpio_uart_felica,
		},
	},
	{
		.gpio      = 54,		/*                */
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_uart_felica_active,
			[GPIOMUX_SUSPENDED] = &gpio_uart_felica,
		},
	},
};
#endif
static struct msm_gpiomux_config msm_sensor_configs[] __initdata = {
//           
	{
		.gpio = 15, /*           */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[0],
			[GPIOMUX_SUSPENDED] = &cam_settings[1],
		},
	},
	{
		.gpio = 4, /*            */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[4],
		},
	},
	{
		.gpio = 16, /*                        */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[4],
		},
	},
	{
		.gpio = 19, /*              */
		.settings = {
			[GPIOMUX_ACTIVE]	= &cam_settings[0],
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[0],
		},
	},
	{
		.gpio = 20, /*              */
		.settings = {
			[GPIOMUX_ACTIVE]	= &cam_settings[0],
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[0],
		},
	},
	{
		.gpio = 57, /*            */
		.settings = {
			[GPIOMUX_ACTIVE]	= &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[4],
		},
	},
	{
		.gpio = 96, /*         */
		.settings = {
			[GPIOMUX_ACTIVE]	= &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[4],
		},
	},

//            
	{
		.gpio = 17, /*           */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[0],
			[GPIOMUX_SUSPENDED] = &cam_settings[1],
		},
	},
	{
		.gpio = 18, /*                             */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[4],
		},
	},
	{
		.gpio = 21, /*              */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[0],
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[0],
		},
	},
	{
		.gpio = 22, /*              */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[0],
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[0],
		},
	},
	{
		.gpio = 30, /*           */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[4],
		},
	},

	{
		.gpio = 23, /*              */
		.settings = {
			[GPIOMUX_ACTIVE]    = &touch_ldoen_act_cfg,
			[GPIOMUX_SUSPENDED] = &touch_ldoen_sus_cfg,
		},
	},
	{
		.gpio = 24, /*               */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[0],
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[1],
		},
	},

#if 0 //           
	{
		.gpio = 25, /*                 */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[0],
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[1],
		},
	},
	{
		.gpio = 26, /*         */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[0],
			[GPIOMUX_SUSPENDED] = &cam_settings[1],
		},
	},
#endif
};

#ifndef CONFIG_MACH_LGE
static struct gpiomux_setting pri_auxpcm_act_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};


static struct gpiomux_setting pri_auxpcm_sus_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct msm_gpiomux_config msm8974_pri_auxpcm_configs[] __initdata = {
	{
		.gpio = 65,
		.settings = {
			[GPIOMUX_SUSPENDED] = &pri_auxpcm_sus_cfg,
			[GPIOMUX_ACTIVE] = &pri_auxpcm_act_cfg,
		},
	},
	{
		.gpio = 66,
		.settings = {
			[GPIOMUX_SUSPENDED] = &pri_auxpcm_sus_cfg,
			[GPIOMUX_ACTIVE] = &pri_auxpcm_act_cfg,
		},
	},
	{
		.gpio = 67,
		.settings = {
			[GPIOMUX_SUSPENDED] = &pri_auxpcm_sus_cfg,
			[GPIOMUX_ACTIVE] = &pri_auxpcm_act_cfg,
		},
	},
	{
		.gpio = 68,
		.settings = {
			[GPIOMUX_SUSPENDED] = &pri_auxpcm_sus_cfg,
			[GPIOMUX_ACTIVE] = &pri_auxpcm_act_cfg,
		},
	},
};
#endif

/*                                                                    */
#if defined (CONFIG_BCMDHD) || defined (CONFIG_BCMDHD_MODULE)
#else
static struct msm_gpiomux_config wcnss_5wire_interface[] = {
	{
		.gpio = 36,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 37,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},

	{
		.gpio = 38,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 39,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 40,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
};
#endif
static struct msm_gpiomux_config wcnss_5gpio_interface[] = {
	{
		.gpio = 36,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5gpio_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5gpio_suspend_cfg,
		},
	},
	{
		.gpio = 37,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5gpio_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5gpio_suspend_cfg,
		},
	},
	{
		.gpio = 38,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5gpio_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5gpio_suspend_cfg,
		},
	},
	{
		.gpio = 39,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5gpio_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5gpio_suspend_cfg,
		},
	},
	{
		.gpio = 40,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5gpio_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5gpio_suspend_cfg,
		},
	},
};

static struct msm_gpiomux_config msm_taiko_config[] __initdata = {
	{
		.gpio	= 63,		/*           */
		.settings = {
			[GPIOMUX_SUSPENDED] = &taiko_reset,
		},
	},
	{
		.gpio	= 72,		/*         */
		.settings = {
			[GPIOMUX_SUSPENDED] = &taiko_int,
		},
	},
};
#if defined(CONFIG_SLIMPORT_ANX7808) || defined(CONFIG_SLIMPORT_ANX7816)
static struct gpiomux_setting slimport_int_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};

static struct msm_gpiomux_config slimport_configs[] __initdata = {
	{
		.gpio      = 28,		/*              */
		.settings = {
			[GPIOMUX_SUSPENDED] = &slimport_int_sus_cfg,
		},
	},

};
#endif

#ifdef CONFIG_MACH_LGE
static struct gpiomux_setting headset_active_cfg_gpio65 ={
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir  = GPIOMUX_IN,
};

static struct gpiomux_setting headset_active_cfg_gpio64 ={
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir  = GPIOMUX_IN,
};

static struct msm_gpiomux_config headset_configs[] ={
	{
		.gpio = 64,
		.settings = {
			[GPIOMUX_ACTIVE]    = &headset_active_cfg_gpio64,
		},
	},

	{
		.gpio = 65,
		.settings = {
			[GPIOMUX_ACTIVE]    = &headset_active_cfg_gpio65,
		},
	},
};

/*                                                              */
#if 0  /*               */
static struct gpiomux_setting sensor_int_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir  = GPIOMUX_IN,
};

static struct gpiomux_setting sensor_en_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir  = GPIOMUX_OUT_HIGH,
};

static struct msm_gpiomux_config sensor_configs[] __initdata = {
	{
		.gpio      = 87,    /*                    */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},
	{
		.gpio      = 88,    /*                    */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},
	{
		.gpio      = 10,    /*                   */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},
	{
		.gpio      = 11,    /*                   */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},
	{
		.gpio      = 3,    /*              */
		.settings = {
			[GPIOMUX_ACTIVE] = &sensor_en_config,
			[GPIOMUX_SUSPENDED] = &sensor_en_config,
		},
	},
	{
		.gpio      = 65,    /*          */
		.settings = {
			[GPIOMUX_ACTIVE] = &sensor_int_config,
			[GPIOMUX_SUSPENDED] = &sensor_int_config,
		},
	},
	{
		.gpio      = 66,    /*          */
		.settings = {
			[GPIOMUX_ACTIVE] = &sensor_int_config,
			[GPIOMUX_SUSPENDED] = &sensor_int_config,
		},
	},
/*
  
                                           
               
                                         
                                            
    
   
*/
	{
		.gpio      = 73,    /*          */
		.settings = {
			[GPIOMUX_ACTIVE] = &sensor_int_config,
			[GPIOMUX_SUSPENDED] = &sensor_int_config,
		},
	},
#if !defined(CONFIG_LGE_NFC_SONY_KDDI)
	{
		.gpio      = 74,    /*               */
		.settings = {
			[GPIOMUX_ACTIVE] = &sensor_int_config,
			[GPIOMUX_SUSPENDED] = &sensor_int_config,
		},
	},
	{
		.gpio      = 102,    /*                 */
		.settings = {
			[GPIOMUX_ACTIVE] = &sensor_int_config,
			[GPIOMUX_SUSPENDED] = &sensor_int_config,
		},
	},
#endif

};
#endif
#endif

#if defined(CONFIG_LGE_SM100) || defined(CONFIG_TSPDRV)
static struct gpiomux_setting vibrator_suspend_cfg = {
       .func = GPIOMUX_FUNC_GPIO,
       .drv = GPIOMUX_DRV_2MA,
       .pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting vibrator_active_cfg_gpio27 = {
       .func = GPIOMUX_FUNC_6,
       .drv = GPIOMUX_DRV_2MA,
       .pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting vibrator_active_cfg_gpio60 = {
       .func = GPIOMUX_FUNC_GPIO,
       .drv = GPIOMUX_DRV_2MA,
       .pull = GPIOMUX_PULL_NONE,
};

static struct msm_gpiomux_config vibrator_configs[] = {
	{
		.gpio = 27,
		.settings = {
			[GPIOMUX_ACTIVE]    = &vibrator_active_cfg_gpio27,
			[GPIOMUX_SUSPENDED] = &vibrator_suspend_cfg,
		},
	},
	{
		.gpio = 60,
		.settings = {
			[GPIOMUX_ACTIVE]    = &vibrator_active_cfg_gpio60,
			[GPIOMUX_SUSPENDED] = &vibrator_suspend_cfg,
		},
	},
};
#endif


/*                                      
                                                     
                                                                    
*/
/*
                                                         
                           
                        
                         
  
*/


#ifdef CONFIG_MMC_MSM_SDC4_SUPPORT
static struct gpiomux_setting sdc4_clk_actv_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting sdc4_cmd_data_0_3_actv_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting sdc4_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting sdc4_data_1_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct msm_gpiomux_config msm8974_sdc4_configs[] __initdata = {

/*                          */
#if !defined (CONFIG_LGE_BROADCAST_JFULLSEG)
	{
		/*      */
		.gpio      = 92,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdc4_cmd_data_0_3_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdc4_suspend_cfg,
		},
	},
#endif
/*                          */
#if !defined(CONFIG_LGE_NFC_SONY_KDDI)
	{
		/*      */
		.gpio      = 94,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdc4_cmd_data_0_3_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdc4_suspend_cfg,
		},
	},
#endif
#if 0
	{
		/*      */
		.gpio      = 95,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdc4_cmd_data_0_3_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdc4_data_1_suspend_cfg,
		},
	},
#endif
/*                                        */
/*                          */
#if !defined (CONFIG_LGE_BROADCAST_JFULLSEG)
	{
		/*     */
		.gpio      = 91,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdc4_cmd_data_0_3_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdc4_suspend_cfg,
		},
	},
#endif
/*                          */
	{
		/*     */
		.gpio      = 93,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdc4_clk_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdc4_suspend_cfg,
		},
	},
};

static void msm_gpiomux_sdc4_install(void)
{
	msm_gpiomux_install(msm8974_sdc4_configs,
			    ARRAY_SIZE(msm8974_sdc4_configs));
}
#else
static void msm_gpiomux_sdc4_install(void) {}
#endif /*                             */

/*                                                        */
#ifdef CONFIG_LGE_BLUETOOTH
static struct gpiomux_setting bt_gpio_uart_active_config = {
    .func = GPIOMUX_FUNC_2,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_NONE, /*                     */
};

static struct gpiomux_setting bt_gpio_uart_suspend_config = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE, /*                    */
};

static struct gpiomux_setting bt_rfkill_active_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting bt_rfkill_suspend_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting bt_host_wakeup_active_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting bt_host_wakeup_suspend_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting bt_wakeup_active_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting bt_wakeup_suspend_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting bt_pcm_active_config = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting bt_pcm_suspend_config = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct msm_gpiomux_config bt_msm_blsp_configs[] __initdata = {
#if !defined(CONFIG_LGE_NFC_SONY_KDDI)
	{
		.gpio = 53, /*                 */
		.settings = {
			[GPIOMUX_ACTIVE] = &bt_gpio_uart_active_config ,
			[GPIOMUX_SUSPENDED] = &bt_gpio_uart_suspend_config ,
		},
	},
	{
		.gpio = 54, /*                 */
		.settings = {
			[GPIOMUX_ACTIVE] = &bt_gpio_uart_active_config ,
			[GPIOMUX_SUSPENDED] = &bt_gpio_uart_suspend_config ,
		},
	},
	{
		.gpio = 55, /*                  */
		.settings = {
			[GPIOMUX_ACTIVE] = &bt_gpio_uart_active_config ,
			[GPIOMUX_SUSPENDED] = &bt_gpio_uart_suspend_config ,
		},
	},
#endif
	{
		.gpio = 56, /*                  */
		.settings = {
			[GPIOMUX_ACTIVE] = &bt_gpio_uart_active_config ,
			[GPIOMUX_SUSPENDED] = &bt_gpio_uart_suspend_config ,
		},
	},
};

static struct msm_gpiomux_config bt_rfkill_configs[] = {
	{
		.gpio = 41,
		.settings = {
			[GPIOMUX_ACTIVE]    = &bt_rfkill_active_config,
			[GPIOMUX_SUSPENDED] = &bt_rfkill_suspend_config,
		},
	},
};

static struct msm_gpiomux_config bt_host_wakeup_configs[] __initdata = {
	{
		.gpio = 42,
		.settings = {
			[GPIOMUX_ACTIVE]    = &bt_host_wakeup_active_config,
			[GPIOMUX_SUSPENDED] = &bt_host_wakeup_suspend_config,
		},
	},
};
/*
                                                                   
  
             
               
                                                  
                                                   
    
   
  
*/
static struct msm_gpiomux_config bt_pcm_configs[] __initdata = {
	{
		.gpio	   = 79,	/*            */
		.settings = {
			[GPIOMUX_ACTIVE]	= &bt_pcm_active_config,
			[GPIOMUX_SUSPENDED] = &bt_pcm_suspend_config,
		},
	},
	{
		.gpio	   = 80,	/*             */
		.settings = {
			[GPIOMUX_ACTIVE]	= &bt_pcm_active_config,
			[GPIOMUX_SUSPENDED] = &bt_pcm_suspend_config,
		},
	},
	{
		.gpio	   = 81,	/*            */
		.settings = {
			[GPIOMUX_ACTIVE]	= &bt_pcm_active_config,
			[GPIOMUX_SUSPENDED] = &bt_pcm_suspend_config,
		},
	},
#if !defined(CONFIG_LGE_NFC_SONY_KDDI)
	{
		.gpio	   = 82,	/*             */
		.settings = {
			[GPIOMUX_ACTIVE]	= &bt_pcm_active_config,
			[GPIOMUX_SUSPENDED] = &bt_pcm_suspend_config,
		},
	}
#endif
};

static void bluetooth_msm_gpiomux_install(void)
{
    /*      */
    msm_gpiomux_install(bt_msm_blsp_configs, ARRAY_SIZE(bt_msm_blsp_configs));

    /*        */
    msm_gpiomux_install(bt_rfkill_configs, ARRAY_SIZE(bt_rfkill_configs));

    /*              */
    msm_gpiomux_install(bt_host_wakeup_configs, ARRAY_SIZE(bt_host_wakeup_configs));

    /*            */
    msm_gpiomux_install(bt_wakeup_configs, ARRAY_SIZE(bt_wakeup_configs));

    /*         */
    msm_gpiomux_install(bt_pcm_configs, ARRAY_SIZE(bt_pcm_configs));
}
#endif /*                      */
/*                                                        */


static struct msm_gpiomux_config apq8074_dragonboard_ts_config[] __initdata = {
	{
		/*                    */
		.gpio      = 2,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},
	{
		/*                   */
		.gpio      = 3,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},
};

void __init msm_8974_init_gpiomux(void)
{
	int rc;

	rc = msm_gpiomux_init_dt();
	if (rc) {
		pr_err("%s failed %d\n", __func__, rc);
		return;
	}

/*                          */
#if defined (CONFIG_LGE_BROADCAST_JFULLSEG)
	    msm_gpiomux_install(lge_fullseg_blsp8_i2c_tsif_configs, ARRAY_SIZE(lge_fullseg_blsp8_i2c_tsif_configs));
#endif
/*                          */

#if defined(CONFIG_LGE_NFC_SONY_KDDI)
       if(lge_get_board_revno() >= HW_REV_B){
           msm_gpiomux_install(msm8974_nfc_configs_after_revb, ARRAY_SIZE(msm8974_nfc_configs_after_revb));
       }else {
           msm_gpiomux_install(msm8974_nfc_configs_before_revb, ARRAY_SIZE(msm8974_nfc_configs_before_revb));
    }
		msm_gpiomux_install(msm8974_felica_uart_blsp1_configs,
				ARRAY_SIZE(msm8974_felica_uart_blsp1_configs));
#endif

#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
//                                                                   
#endif
	msm_gpiomux_install(msm_blsp_configs, ARRAY_SIZE(msm_blsp_configs));

#ifndef CONFIG_MACH_LGE
	msm_gpiomux_install(msm_blsp2_uart7_configs,
			 ARRAY_SIZE(msm_blsp2_uart7_configs));
#endif

#if defined(CONFIG_MACH_LGE)
#if defined(CONFIG_MAX17048_FUELGAUGE)
	msm_gpiomux_install(msm_fuel_gauge_configs,
			ARRAY_SIZE(msm_fuel_gauge_configs));
#endif
#endif
/*                                                                    */
#if defined (CONFIG_BCMDHD) || defined (CONFIG_BCMDHD_MODULE)
#else
	msm_gpiomux_install(wcnss_5wire_interface,
				ARRAY_SIZE(wcnss_5wire_interface));
#endif
/*                                                                    */

	msm_gpiomux_install(msm8974_slimbus_config,
			ARRAY_SIZE(msm8974_slimbus_config));

	msm_gpiomux_install(msm_touch_configs, ARRAY_SIZE(msm_touch_configs));

/*                                                        */
//                       
#if 0
	msm_gpiomux_install(hap_lvl_shft_config,
			ARRAY_SIZE(hap_lvl_shft_config));
#endif
#ifdef CONFIG_LGE_IRDA_KDDI
	msm_gpiomux_install(irda_pwdn_config,
			ARRAY_SIZE(irda_pwdn_config));

	if(lge_get_board_revno() >= HW_REV_B) {
		msm_gpiomux_install(irda_rc_pwen_config,
            ARRAY_SIZE(irda_rc_pwen_config));
	}
#endif
	msm_gpiomux_install(msm_sensor_configs, ARRAY_SIZE(msm_sensor_configs));

	msm_gpiomux_install(&sd_card_det, 1);
	msm_gpiomux_sdc4_install();

	msm_gpiomux_install(msm_taiko_config, ARRAY_SIZE(msm_taiko_config));

	//                                                                    
	msm_gpiomux_install(msm_hsic_hub_configs,
				ARRAY_SIZE(msm_hsic_hub_configs));

	msm_gpiomux_install(msm_hdmi_configs, ARRAY_SIZE(msm_hdmi_configs));
	if (of_board_is_fluid())
		msm_gpiomux_install(msm_mhl_configs,
				    ARRAY_SIZE(msm_mhl_configs));
#ifndef CONFIG_MACH_LGE
	msm_gpiomux_install(msm8974_pri_auxpcm_configs,
				 ARRAY_SIZE(msm8974_pri_auxpcm_configs));
#endif
	msm_gpiomux_install_nowrite(msm_lcd_configs,
			ARRAY_SIZE(msm_lcd_configs));

	if (of_board_is_rumi())
		msm_gpiomux_install(msm_rumi_blsp_configs,
				    ARRAY_SIZE(msm_rumi_blsp_configs));

	if (socinfo_get_platform_subtype() == PLATFORM_SUBTYPE_MDM)
		msm_gpiomux_install(mdm_configs,
			ARRAY_SIZE(mdm_configs));

#if defined(CONFIG_SLIMPORT_ANX7808) || defined(CONFIG_SLIMPORT_ANX7816)
		msm_gpiomux_install(slimport_configs,
					ARRAY_SIZE(slimport_configs));
#endif
#if defined(CONFIG_MACH_LGE)
//                                                                          
	msm_gpiomux_install(msm_hall_ic_configs, ARRAY_SIZE(msm_hall_ic_configs));

   	msm_gpiomux_install(headset_configs,ARRAY_SIZE(headset_configs));
#endif
#if 0 /*                                          */
	msm_gpiomux_install(sensor_configs, ARRAY_SIZE(sensor_configs));
#endif
#if defined(CONFIG_LGE_SM100) || defined(CONFIG_TSPDRV)
       msm_gpiomux_install(vibrator_configs, ARRAY_SIZE(vibrator_configs));
#endif
/*                                                        */
#ifdef CONFIG_LGE_BLUETOOTH
    bluetooth_msm_gpiomux_install();
#endif /*                      */
/*                                                        */

	if (of_board_is_dragonboard() && machine_is_apq8074())
		msm_gpiomux_install(apq8074_dragonboard_ts_config,
			ARRAY_SIZE(apq8074_dragonboard_ts_config));
}

static void wcnss_switch_to_gpio(void)
{
	/*                    */
	msm_gpiomux_install(wcnss_5gpio_interface,
			ARRAY_SIZE(wcnss_5gpio_interface));

	/*                    */
	gpio_direction_input(WLAN_DATA2);
	gpio_direction_input(WLAN_DATA1);
	gpio_direction_input(WLAN_DATA0);
	gpio_direction_output(WLAN_SET, 0);
	gpio_direction_output(WLAN_CLK, 0);
}

static void wcnss_switch_to_5wire(void)
{
	msm_gpiomux_install(wcnss_5wire_interface,
			ARRAY_SIZE(wcnss_5wire_interface));
}

u32 wcnss_rf_read_reg(u32 rf_reg_addr)
{
	int count = 0;
	u32 rf_cmd_and_addr = 0;
	u32 rf_data_received = 0;
	u32 rf_bit = 0;

	wcnss_switch_to_gpio();

	/*                                               */
	gpio_set_value(WLAN_SET, 0);
	gpio_set_value(WLAN_CLK, 0);

	/*                                          */
	gpio_set_value(WLAN_SET, 1);

	gpio_direction_output(WLAN_DATA0, 1);
	gpio_direction_output(WLAN_DATA1, 1);
	gpio_direction_output(WLAN_DATA2, 1);

	gpio_set_value(WLAN_DATA0, 0);
	gpio_set_value(WLAN_DATA1, 0);
	gpio_set_value(WLAN_DATA2, 0);

	/*                                                               
                                                 
  */
	rf_cmd_and_addr  = (((WLAN_RF_READ_REG_CMD) |
		(rf_reg_addr << WLAN_RF_REG_ADDR_START_OFFSET)) &
		WLAN_RF_READ_CMD_MASK);

	for (count = 0; count < 5; count++) {
		gpio_set_value(WLAN_CLK, 0);

		rf_bit = (rf_cmd_and_addr & 0x1);
		gpio_set_value(WLAN_DATA0, rf_bit ? 1 : 0);
		rf_cmd_and_addr = (rf_cmd_and_addr >> 1);

		rf_bit = (rf_cmd_and_addr & 0x1);
		gpio_set_value(WLAN_DATA1, rf_bit ? 1 : 0);
		rf_cmd_and_addr = (rf_cmd_and_addr >> 1);

		rf_bit = (rf_cmd_and_addr & 0x1);
		gpio_set_value(WLAN_DATA2, rf_bit ? 1 : 0);
		rf_cmd_and_addr = (rf_cmd_and_addr >> 1);

		/*                                */
		gpio_set_value(WLAN_CLK, 1);
	}

	/*                            */
	gpio_set_value(WLAN_CLK, 0);

	/*                                      */
	gpio_direction_input(WLAN_DATA0);
	gpio_direction_input(WLAN_DATA1);
	gpio_direction_input(WLAN_DATA2);

	for (count = 0; count < 2; count++) {
		gpio_set_value(WLAN_CLK, 1);
		gpio_set_value(WLAN_CLK, 0);
	}

	rf_bit = 0;
	for (count = 0; count < 6; count++) {
		gpio_set_value(WLAN_CLK, 1);
		gpio_set_value(WLAN_CLK, 0);

		rf_bit = gpio_get_value(WLAN_DATA0);
		rf_data_received |= (rf_bit << (count * 3 + 0));

		if (count != 5) {
			rf_bit = gpio_get_value(WLAN_DATA1);
			rf_data_received |= (rf_bit << (count * 3 + 1));

			rf_bit = gpio_get_value(WLAN_DATA2);
			rf_data_received |= (rf_bit << (count * 3 + 2));
		}
	}

	gpio_set_value(WLAN_SET, 0);
	wcnss_switch_to_5wire();

	return rf_data_received;
}
