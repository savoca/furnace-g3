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

#ifndef __TCC353X_MAILBOX_H__
#define __TCC353X_MAILBOX_H__

#include "tcc353x_common.h"
#include "tcc353x_core.h"

#define MAX_MAILBOX_RETRY	(2)	/*              */

#define MB_HOSTMAIL		0x47
#define MB_SLAVEMAIL		0x74

#define MB_ERR_OK		0x00
#define MB_ERR_CMD		0x01
#define MB_ERR_PARA		0x02

#define MBCMD_SYS		(0x00<<11)
#define MBPARA_SYS_START	(MBCMD_SYS | 0x00)
#define MBPARA_SYS_RF_FREQ	(MBCMD_SYS | 0x04)
#define MBPARA_SYS_PAUSE	(MBCMD_SYS | 0x07)
#define MBPARA_SYS_ASM_VER	(MBCMD_SYS | 0xff)

#define MB_TMCC			(0x05<<11)
#define MBPARA_TMCC_RESULT	(MB_TMCC | 0x00)

/*
                     
               
                
       
                     
               
                 
 */

#define MB_CMD_READ	0
#define MB_CMD_WRITE	1

I32S Tcc353xMailboxTxOnly(Tcc353xHandle_t * _handle, I32U cmd,
			  I32U * data_array, I32S word_cnt);
I32S Tcc353xMailboxTxRx(Tcc353xHandle_t * _handle, mailbox_t * p_mailbox,
			I32U cmd, I32U * data_array, I32S word_cnt);
I32U Tcc353xGetAccessMail(Tcc353xHandle_t * _handle);

#endif
