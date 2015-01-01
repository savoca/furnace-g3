/*                                                                          */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*   Copyright (c) Telechips, Inc.                                          */
/*                                                                          */
/*                                                                          */
/*                                                                          */

#include "tcc353x_common.h"
#include "tcc353x_api.h"
#include "tcc353x_monitoring.h"
#include "tcc353x_user_defines.h"

#define __USER_GPIO9_STRENGTH_MAX__

/*                               */
Tcc353xRegisterConfig_t Tcc353xSingle[3] = {
	{
	 /*              */
	 /*    */0x06,			/*                                              */
	 /*            */
	 0x10,
	 /*                                           */
	 TCC353X_REMAP_TYPE, TCC353X_INIT_PC_H, TCC353X_INIT_PC_L,
	 /*                                                            */
	 0x00, 0x00, 0x00,
	 /*                                                         */
	 0x00, 0x00, 0x00,
	 /*                                                         */
	 0x00, 0x00, 0x00,
	 /*                                                            */
#if defined (__USER_GPIO9_STRENGTH_MAX__)
	 TCC353X_DRV_STR_GPIO_0x13_07_00, 0x02, 0x00,
#else
	 TCC353X_DRV_STR_GPIO_0x13_07_00, 0x00, 0x00,
#endif
	 /*                                                         */
	 0x00, 0x00, 0x00,
	 /*                                                               */
	 0x00, 0x00, 0x00,
	 /*            */
	 0x10,
	 /*                                              */
	 0x00, TCC353X_STREAM_THRESHOLD_SPISLV_WH,
	 /*                                               */
	 TCC353X_STREAM_THRESHOLD_SPISLV_WL, 0x10,
	 /*                                  */
	 0, 0,
	 /*                                  */
	 0, 0,
	 /*                                     */
	 0x11, 0x0F,
	 /*                                     */
	 TCC353X_STREAM_THRESHOLD_SPISLV_WH,
	 TCC353X_STREAM_THRESHOLD_SPISLV_WL,
	 /*                                     */
	 ((TCC353X_BUFF_A_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_START >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_A_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_END >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_B_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_START >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_B_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_END >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_C_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_START >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_C_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_END >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_D_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_START >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_D_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_END >> 2) & 0xFF),
	 }
	,
	{
	 /*              */
	 0x00,			/*                   */
	 /*            */
	 0x00,
	 /*                                           */
	 TCC353X_REMAP_TYPE, TCC353X_INIT_PC_H, TCC353X_INIT_PC_L,
	 /*                                                            */
	 0xF0, 0x00, 0x00,
	 /*                                                         */
	 0x00, 0x00, 0x00,
	 /*                                                         */
	 0x00, 0x00, 0x00,
	 /*                                                            */
#if defined (__USER_GPIO9_STRENGTH_MAX__)
	 TCC353X_DRV_STR_GPIO_0x13_07_00, 0x02, 0x00,
#else
	 TCC353X_DRV_STR_GPIO_0x13_07_00, 0x00, 0x00,
#endif
	 /*                                                         */
	 0x00, 0x00, 0x00,
	 /*                                                               */
	 0x00, 0x00, 0x00,
	 /*            */
	 0x10,
	 /*                                              */
	 0x0F, TCC353X_STREAM_THRESHOLD_WH,
	 /*                                               */
	 TCC353X_STREAM_THRESHOLD_WL, 0x90,
	 /*                                  */
	 0x21, 0x10 | TCC353X_DLR,
	 /*                                  */
	 STS_POLARITY | 0x12, 0x40,
	 /*                                     */
	 0x11, 0x0F,
	 /*                                     */
	 TCC353X_STREAM_THRESHOLD_WH, TCC353X_STREAM_THRESHOLD_WL,
	 /*                                     */
	 ((TCC353X_BUFF_A_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_START >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_A_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_END >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_B_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_START >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_B_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_END >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_C_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_START >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_C_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_END >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_D_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_START >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_D_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_END >> 2) & 0xFF),
	 }
	,
	{
	 /*              */
	 0x00,			/*                   */
	 /*            */
	 0x00,
	 /*                                           */
	 TCC353X_REMAP_TYPE, TCC353X_INIT_PC_H, TCC353X_INIT_PC_L,
	 /*                                                            */
	 0xF0, 0x00, 0x00,
	 /*                                                         */
	 0x00, 0x00, 0x00,
	 /*                                                         */
	 0x00, 0x00, 0x00,
	 /*                                                            */
#if defined (__USER_GPIO9_STRENGTH_MAX__)
	 TCC353X_DRV_STR_GPIO_0x13_07_00, 0x02, 0x00,
#else
	 TCC353X_DRV_STR_GPIO_0x13_07_00, 0x00, 0x00,
#endif
	 /*                                                         */
	 0x00, 0x00, 0x00,
	 /*                                                               */
	 0x00, 0x00, 0x00,
	 /*            */
	 0x10,
	 /*                                              */
	 0x0F, TCC353X_STREAM_THRESHOLD_WH,
	 /*                                               */
	 TCC353X_STREAM_THRESHOLD_WL, 0x90,
	 /*                                  */
	 0x11, TCC353X_DLR << 2,
	 /*                                  */
	 0x10, 0x00,
	 /*                                     */
	 0x11, 0x0F,
	 /*                                     */
	 TCC353X_STREAM_THRESHOLD_WH, TCC353X_STREAM_THRESHOLD_WL,
	 /*                                     */
	 ((TCC353X_BUFF_A_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_START >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_A_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_END >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_B_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_START >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_B_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_END >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_C_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_START >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_C_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_END >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_D_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_START >> 2) & 0xFF),
	 /*                                     */
	 ((TCC353X_BUFF_D_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_END >> 2) & 0xFF),
	 }
	,
};

#ifdef _MODEL_F9J_ /*              */
Tcc353xOption_t Tcc353xOptionSingle = {
	/*                          */
	BB_TCC3530,

	/*                          */
	TCC353X_BOARD_SINGLE,

	/*                          */
	TCC353X_IF_TCCSPI,

	/*                          */
	TCC353X_STREAM_IO_MAINIO,

	/*                          */
	/*                          */
	/*                          */
	/*                          */
	/*                          */
	0xA8,

	/*                          */
	0x00, /*                 */ /*        */

	/*                          */
	19200,

	/*                           */
	TCC353X_DIVERSITY_NONE,

	/*                                      */
	1,

	/*                                      */
	/*                                      */
	/*                                      */
	(-1),

	/*                          */
	&Tcc353xSingle[0]
};

#else

/*                        */
Tcc353xOption_t Tcc353xOptionSingle = {
	/*                          */
	BB_TCC3530,

	/*                          */
	TCC353X_BOARD_SINGLE,

	/*                          */
	TCC353X_IF_TCCSPI,

	/*                          */
	TCC353X_STREAM_IO_MAINIO,

	/*                          */
	/*                          */
	/*                          */
	/*                          */
	/*                          */
	0xA8,

	/*                          */
	0x00, /*                 */ /*        */

	/*                          */
	38400,

	/*                           */
	TCC353X_DIVERSITY_NONE,

	/*                                      */
	1,

	/*                                      */
	/*                                      */
	/*                                      */
	GPIO_NUM_RF_SWITCHING_TCC3530,

	/*                          */
	&Tcc353xSingle[0]
};
#endif

