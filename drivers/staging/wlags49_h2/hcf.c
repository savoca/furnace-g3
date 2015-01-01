/************************************************************************************************************
 *
 * FILE   :  HCF.C
 *
 * DATE    : $Date: 2004/08/05 11:47:10 $   $Revision: 1.10 $
 * Original: 2004/06/02 10:22:22    Revision: 1.85      Tag: hcf7_t20040602_01
 * Original: 2004/04/15 09:24:41    Revision: 1.63      Tag: hcf7_t7_20040415_01
 * Original: 2004/04/13 14:22:44    Revision: 1.62      Tag: t7_20040413_01
 * Original: 2004/04/01 15:32:55    Revision: 1.59      Tag: t7_20040401_01
 * Original: 2004/03/10 15:39:27    Revision: 1.55      Tag: t20040310_01
 * Original: 2004/03/04 11:03:37    Revision: 1.53      Tag: t20040304_01
 * Original: 2004/03/02 14:51:21    Revision: 1.50      Tag: t20040302_03
 * Original: 2004/02/24 13:00:27    Revision: 1.43      Tag: t20040224_01
 * Original: 2004/02/19 10:57:25    Revision: 1.39      Tag: t20040219_01
 *
 * AUTHOR :  Nico Valster
 *
 * SPECIFICATION: ........
 *
 * DESCRIPTION : HCF Routines for Hermes-II (callable via the Wireless Connection I/F or WCI)
 *               Local Support Routines for above procedures
 *
 *           Customizable via HCFCFG.H, which is included by HCF.H
 *
 *************************************************************************************************************
 *
 *
 * SOFTWARE LICENSE
 *
 * This software is provided subject to the following terms and conditions,
 * which you should read carefully before using the software.  Using this
 * software indicates your acceptance of these terms and conditions.  If you do
 * not agree with these terms and conditions, do not use the software.
 *
 * COPYRIGHT © 1994 - 1995   by AT&T.                All Rights Reserved
 * COPYRIGHT © 1996 - 2000 by Lucent Technologies.   All Rights Reserved
 * COPYRIGHT © 2001 - 2004   by Agere Systems Inc.   All Rights Reserved
 * All rights reserved.
 *
 * Redistribution and use in source or binary forms, with or without
 * modifications, are permitted provided that the following conditions are met:
 *
 * . Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following Disclaimer as comments in the code as
 *    well as in the documentation and/or other materials provided with the
 *    distribution.
 *
 * . Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following Disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * . Neither the name of Agere Systems Inc. nor the names of the contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * Disclaimer
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, INFRINGEMENT AND THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  ANY
 * USE, MODIFICATION OR DISTRIBUTION OF THIS SOFTWARE IS SOLELY AT THE USERS OWN
 * RISK. IN NO EVENT SHALL AGERE SYSTEMS INC. OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, INCLUDING, BUT NOT LIMITED TO, CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 *
 ************************************************************************************************************/


/*                                                                                                           
   
                        
   
                                                                                                      
                                         
                                                                                                      
                                                                                                              
                 
                                                                                                              
                                                                             
                                                                                                              
                                               
  
                                                                                                            */

#include "hcf.h"                //                                
#include "hcfdef.h"             //                          
#include "mmd.h"                //                                      
#include <linux/bug.h>
#include <linux/kernel.h>

#if ! defined offsetof
#define offsetof(s,m)   ((unsigned int)&(((s *)0)->m))
#endif //         


/*                                                                                                         */
/*                                                                                                         */
/*                                                                                                         */
HCF_STATIC int          cmd_exe( IFBP ifbp, hcf_16 cmd_code, hcf_16 par_0 );
HCF_STATIC int          init( IFBP ifbp );
HCF_STATIC int          put_info( IFBP ifbp, LTVP ltvp );
HCF_STATIC int          put_info_mb( IFBP ifbp, CFG_MB_INFO_STRCT FAR * ltvp );
#if (HCF_TYPE) & HCF_TYPE_WPA
HCF_STATIC void         calc_mic( hcf_32* p, hcf_32 M );
void                    calc_mic_rx_frag( IFBP ifbp, wci_bufp p, int len );
void                    calc_mic_tx_frag( IFBP ifbp, wci_bufp p, int len );
HCF_STATIC int          check_mic( IFBP ifbp );
#endif //             

HCF_STATIC void         calibrate( IFBP ifbp );
HCF_STATIC int          cmd_cmpl( IFBP ifbp );
HCF_STATIC hcf_16       get_fid( IFBP ifbp );
HCF_STATIC void         isr_info( IFBP ifbp );
#if HCF_DMA
HCF_STATIC DESC_STRCT*  get_frame_lst(IFBP ifbp, int tx_rx_flag);
#endif //        
HCF_STATIC void         get_frag( IFBP ifbp, wci_bufp bufp, int len BE_PAR( int word_len ) );   //                                
#if HCF_DMA
HCF_STATIC void         put_frame_lst( IFBP ifbp, DESC_STRCT *descp, int tx_rx_flag );
#endif //        
HCF_STATIC void         put_frag( IFBP ifbp, wci_bufp bufp, int len BE_PAR( int word_len ) );
HCF_STATIC void         put_frag_finalize( IFBP ifbp );
HCF_STATIC int          setup_bap( IFBP ifbp, hcf_16 fid, int offset, int type );
#if (HCF_ASSERT) & HCF_ASSERT_PRINTF
static int fw_printf(IFBP ifbp, CFG_FW_PRINTF_STRCT FAR *ltvp);
#endif //                  

HCF_STATIC int          download( IFBP ifbp, CFG_PROG_STRCT FAR *ltvp );
HCF_STATIC hcf_8        hcf_encap( wci_bufp type );
HCF_STATIC hcf_8        null_addr[4] = { 0, 0, 0, 0 };
#if ! defined IN_PORT_WORD          //                                        
extern FILE *log_file;

#define IN_PORT_WORD(port)          in_port_word( (hcf_io)(port) )

static hcf_16 in_port_word( hcf_io port ) {
	hcf_16 i = (hcf_16)_inpw( port );
	if ( log_file ) {
		fprintf( log_file, "\nR %2.2x %4.4x", (port)&0xFF, i);
	}
	return i;
} //             

#define OUT_PORT_WORD(port, value)  out_port_word( (hcf_io)(port), (hcf_16)(value) )

static void out_port_word( hcf_io port, hcf_16 value ) {
	_outpw( port, value );
	if ( log_file ) {
		fprintf( log_file, "\nW %2.02x %4.04x", (port)&0xFF, value );
	}
}

void IN_PORT_STRING_32( hcf_io prt, hcf_32 FAR * dst, int n)    {
	int i = 0;
	hcf_16 FAR * p;
	if ( log_file ) {
		fprintf( log_file, "\nread string_32 length %04x (%04d) at port %02.2x to addr %lp",
			 (hcf_16)n, (hcf_16)n, (hcf_16)(prt)&0xFF, dst);
	}
	while ( n-- ) {
		p = (hcf_16 FAR *)dst;
		*p++ = (hcf_16)_inpw( prt );
		*p   = (hcf_16)_inpw( prt );
		if ( log_file ) {
			fprintf( log_file, "%s%08lx ", i++ % 0x08 ? " " : "\n", *dst);
		}
		dst++;
	}
} //                  

void IN_PORT_STRING_8_16( hcf_io prt, hcf_8 FAR * dst, int n) { //                                    
	hcf_16 FAR * p = (hcf_16 FAR *)dst;                         //                                                   
	int i = 0;
	if ( log_file ) {
		fprintf( log_file, "\nread string_16 length %04x (%04d) at port %02.2x to addr %lp",
			 (hcf_16)n, (hcf_16)n, (hcf_16)(prt)&0xFF, dst );
	}
	while ( n-- ) {
		*p =(hcf_16)_inpw( prt);
		if ( log_file ) {
			if ( i++ % 0x10 ) {
				fprintf( log_file, "%04x ", *p);
			} else {
				fprintf( log_file, "\n%04x ", *p);
			}
		}
		p++;
	}
} //                    

void OUT_PORT_STRING_32( hcf_io prt, hcf_32 FAR * src, int n)   {
	int i = 0;
	hcf_16 FAR * p;
	if ( log_file ) {
		fprintf( log_file, "\nwrite string_32 length %04x (%04d) at port %02.2x",
			 (hcf_16)n, (hcf_16)n, (hcf_16)(prt)&0xFF);
	}
	while ( n-- ) {
		p = (hcf_16 FAR *)src;
		_outpw( prt, *p++ );
		_outpw( prt, *p   );
		if ( log_file ) {
			fprintf( log_file, "%s%08lx ", i++ % 0x08 ? " " : "\n", *src);
		}
		src++;
	}
} //                   

void OUT_PORT_STRING_8_16( hcf_io prt, hcf_8 FAR * src, int n)  {   //                                    
	hcf_16 FAR * p = (hcf_16 FAR *)src;                             //                                                   
	int i = 0;
	if ( log_file ) {
		fprintf( log_file, "\nwrite string_16 length %04x (%04d) at port %04x", n, n, (hcf_16)prt);
	}
	while ( n-- ) {
		(void)_outpw( prt, *p);
		if ( log_file ) {
			if ( i++ % 0x10 ) {
				fprintf( log_file, "%04x ", *p);
			} else {
				fprintf( log_file, "\n%04x ", *p);
			}
		}
		p++;
	}
} //                     

#endif //             

/*                                                                                                           
                                                                                                              
                                                                                                            */

#if HCF_ASSERT
IFBP BASED assert_ifbp = NULL;          //                                             
#endif //           

/*                                                  */
HCF_STATIC  hcf_8 BASED snap_header[] = { 0xAA, 0xAA, 0x03, 0x00, 0x00, //                   
                                          0 };                          //                          

#if (HCF_TYPE) & HCF_TYPE_WPA
HCF_STATIC hcf_8 BASED mic_pad[8] = { 0x5A, 0, 0, 0, 0, 0, 0, 0 };      //                      
#endif //             

#if defined MSF_COMPONENT_ID
CFG_IDENTITY_STRCT BASED cfg_drv_identity = {
	sizeof(cfg_drv_identity)/sizeof(hcf_16) - 1,    //             
	CFG_DRV_IDENTITY,           //         
	MSF_COMPONENT_ID,
	MSF_COMPONENT_VAR,
	MSF_COMPONENT_MAJOR_VER,
	MSF_COMPONENT_MINOR_VER
} ;

CFG_RANGES_STRCT BASED cfg_drv_sup_range = {
	sizeof(cfg_drv_sup_range)/sizeof(hcf_16) - 1,   //             
	CFG_DRV_SUP_RANGE,          //         

	COMP_ROLE_SUPL,
	COMP_ID_DUI,
	{{  DUI_COMPAT_VAR,
	    DUI_COMPAT_BOT,
	    DUI_COMPAT_TOP
	}}
} ;

struct CFG_RANGE3_STRCT BASED cfg_drv_act_ranges_pri = {
	sizeof(cfg_drv_act_ranges_pri)/sizeof(hcf_16) - 1,  //             
	CFG_DRV_ACT_RANGES_PRI,     //         

	COMP_ROLE_ACT,
	COMP_ID_PRI,
	{
		{ 0, 0, 0 },                           //                                     
		{ 0, 0, 0 },                           //                                     
		{  3,                                  //                           
		   CFG_DRV_ACT_RANGES_PRI_3_BOTTOM,        //                             
		   CFG_DRV_ACT_RANGES_PRI_3_TOP            //                          
		}
	}
} ;


struct CFG_RANGE4_STRCT BASED cfg_drv_act_ranges_sta = {
	sizeof(cfg_drv_act_ranges_sta)/sizeof(hcf_16) - 1,  //             
	CFG_DRV_ACT_RANGES_STA,     //         

	COMP_ROLE_ACT,
	COMP_ID_STA,
	{
#if defined HCF_STA_VAR_1
		{  1,                                  //                           
		   CFG_DRV_ACT_RANGES_STA_1_BOTTOM,        //                             
		   CFG_DRV_ACT_RANGES_STA_1_TOP            //                          
		},
#else
		{ 0, 0, 0 },
#endif //              
#if defined HCF_STA_VAR_2
		{  2,                                  //                           
		   CFG_DRV_ACT_RANGES_STA_2_BOTTOM,        //                             
		   CFG_DRV_ACT_RANGES_STA_2_TOP            //                          
		},
#else
		{ 0, 0, 0 },
#endif //              
//                           
#if defined HCF_STA_VAR_3
		{  3,                                  //                           
		   CFG_DRV_ACT_RANGES_STA_3_BOTTOM,        //                             
		   CFG_DRV_ACT_RANGES_STA_3_TOP            //                          
		},
#else
		{ 0, 0, 0 },
#endif //              
//     
#if defined HCF_STA_VAR_4
		{  4,                                  //                           
		   CFG_DRV_ACT_RANGES_STA_4_BOTTOM,        //                                 
		   CFG_DRV_ACT_RANGES_STA_4_TOP            //                              
		}
#else
		{ 0, 0, 0 }
#endif //              
	}
} ;


struct CFG_RANGE6_STRCT BASED cfg_drv_act_ranges_hsi = {
	sizeof(cfg_drv_act_ranges_hsi)/sizeof(hcf_16) - 1,  //             
	CFG_DRV_ACT_RANGES_HSI,     //         
	COMP_ROLE_ACT,
	COMP_ID_HSI,
	{
#if defined HCF_HSI_VAR_0                   //                      
		{  0,                                  //                            
		   CFG_DRV_ACT_RANGES_HSI_0_BOTTOM,        //                                 
		   CFG_DRV_ACT_RANGES_HSI_0_TOP            //                              
		},
#else
		{ 0, 0, 0 },
#endif //              
		{ 0, 0, 0 },                           //                                     
		{ 0, 0, 0 },                           //                                     
		{ 0, 0, 0 },                           //                                     
#if defined HCF_HSI_VAR_4                   //                    
		{  4,                                  //                            
		   CFG_DRV_ACT_RANGES_HSI_4_BOTTOM,        //                                 
		   CFG_DRV_ACT_RANGES_HSI_4_TOP            //                              
		},
#else
		{ 0, 0, 0 },
#endif //              
#if defined HCF_HSI_VAR_5                   //                
		{  5,                                  //                            
		   CFG_DRV_ACT_RANGES_HSI_5_BOTTOM,        //                                 
		   CFG_DRV_ACT_RANGES_HSI_5_TOP            //                              
		}
#else
		{ 0, 0, 0 }
#endif //              
	}
} ;


CFG_RANGE4_STRCT BASED cfg_drv_act_ranges_apf = {
	sizeof(cfg_drv_act_ranges_apf)/sizeof(hcf_16) - 1,  //             
	CFG_DRV_ACT_RANGES_APF,     //         

	COMP_ROLE_ACT,
	COMP_ID_APF,
	{
#if defined HCF_APF_VAR_1               //               
		{  1,                                  //                           
		   CFG_DRV_ACT_RANGES_APF_1_BOTTOM,        //                                 
		   CFG_DRV_ACT_RANGES_APF_1_TOP            //                              
		},
#else
		{ 0, 0, 0 },
#endif //              
#if defined HCF_APF_VAR_2               //         
		{  2,                                  //                            
		   CFG_DRV_ACT_RANGES_APF_2_BOTTOM,        //                                 
		   CFG_DRV_ACT_RANGES_APF_2_TOP            //                              
		},
#else
		{ 0, 0, 0 },
#endif //              
#if defined HCF_APF_VAR_3                       //           
		{  3,                                      //                            
		   CFG_DRV_ACT_RANGES_APF_3_BOTTOM,        //                                                            
		   CFG_DRV_ACT_RANGES_APF_3_TOP            //                              
		},
#else
		{ 0, 0, 0 },
#endif //              
#if defined HCF_APF_VAR_4                       //                
		{  4,                                      //                            
		   CFG_DRV_ACT_RANGES_APF_4_BOTTOM,        //                                                            
		   CFG_DRV_ACT_RANGES_APF_4_TOP            //                              
		}
#else
		{ 0, 0, 0 }
#endif //              
	}
} ;
#define HCF_VERSION  TEXT( "HCF$Revision: 1.10 $" )

static struct /*                 */ {
	hcf_16  len;                    //                            
	hcf_16  typ;                    //           
	hcf_16   v0;                        //                  
	hcf_16   v1;                        //                 
	hcf_16   v2;                        //          
	hcf_16   v3;                        //           
	hcf_16   v4;                        //               
	hcf_16   v5;                        //                         
	hcf_16   v6;                        //        
	hcf_16   v7;                        //          
	hcf_16   v8;                        //        
	hcf_16   v9;                        //           
	hcf_16  v10;                        //       
	hcf_16  v11;                        //           
	hcf_16  v12;                        //            
	hcf_16  v13;                        //              
	hcf_16  v14;                        //          
	hcf_16  v15;                        //            
	hcf_16  v16;                        //         
	hcf_16  v17;                        //                
	hcf_16  v18;                        //                
	hcf_16  v19;                        //               
	char    val[sizeof(HCF_VERSION)];
} BASED cfg_hcf_opt = {
	sizeof(cfg_hcf_opt)/sizeof(hcf_16) -1,
	CFG_HCF_OPT,                //         
	( sizeof(cfg_hcf_opt) - sizeof(HCF_VERSION) - 4 )/sizeof(hcf_16),
#if defined MSF_COMPONENT_ID
	MSF_COMPONENT_ID,
#else
	0,
#endif //                 
	HCF_ALIGN,
	HCF_ASSERT,
	HCF_BIG_ENDIAN,
	0,                                  //                         
	HCF_DMA,
	HCF_ENCAP,
	HCF_EXT,
	HCF_INT_ON,
	HCF_IO,
	HCF_LEGACY,
	HCF_MAX_LTV,
	HCF_PROT_TIME,
	HCF_SLEEP,
	HCF_TALLIES,
	HCF_TYPE,
#if (HCF_TALLIES) & ( HCF_TALLIES_NIC | HCF_TALLIES_HCF )
	HCF_NIC_TAL_CNT,
	HCF_HCF_TAL_CNT,
	offsetof(IFB_STRCT, IFB_TallyLen ),
#else
	0, 0, 0,
#endif //                                  
	HCF_VERSION
}; //            
#endif //                 

HCF_STATIC LTV_STRCT BASED cfg_null = { 1, CFG_NULL, {0} };

HCF_STATIC hcf_16* BASED xxxx[ ] = {
	&cfg_null.len,                          //                                    
#if defined MSF_COMPONENT_ID
	&cfg_drv_identity.len,                  //                                    
	&cfg_drv_sup_range.len,                 //                                    
	&cfg_drv_act_ranges_pri.len,            //                                    
	&cfg_drv_act_ranges_sta.len,            //                                    
	&cfg_drv_act_ranges_hsi.len,            //                                    
	&cfg_drv_act_ranges_apf.len,            //                                    
	&cfg_hcf_opt.len,                       //                                    
	NULL,                                   //                                    
	NULL,                                   //                                    
#endif //                 
	NULL                                    //           
};
#define xxxx_PRI_IDENTITY_OFFSET    (ARRAY_SIZE(xxxx) - 3)


/*                                                                                                           
                                                                                                              
                                                                                                            */

/*                                                                                                           
  
                                                           
                                                    
                                                
  
            
                                                           
                                                                  
                                                                       
                                                                        
                                                                                                         
                                                                     
                                                                   
                                                                               
                                              
                                                                  
  
          
                                                    
                                                               
                                                                      
  
             
                                                                                                             
                                                                                                          
                                
                                                                                                          
              
  
              
                                                                                      
                                                
  
                                                                      
                        
                      
                    
                     
                       
  
                                                        
                                                                                                               
                                            
  
                                                      
                                                                                           
  
                                                             
                                                                                                        
  
                                                         
                                  
                                                                              
                                                                                                           
                                                     
                                                                                         
                                                                                                            
                                
  
                                                                                                
                                                                                     
                                                                                                     
  
                                                                                                              
                                                                                                  
  
                                                                
                                                                                                             
                                                                                                               
                                                                                                           
                                                                                                               
                                                                                                             
                          
  
                                                   
  
                                                                          
                                                                                            
                                             
                                                                                                              
                                                                                                           
                                          
                                                                                  
                                                                                                             
                                                                              
                                                                                                             
                                                    
                                                                                           
  
                                             
                                                          
                                                                                      
                                                       
  
                                                         
                                                          
                                                         
                                                                                           
  
                                       
                                                                                                          
                                                   
                                                                                           
  
                  
                                                
                                                                        
                                                      
                                                                    
                                                                                                              
                    
  
                                                                                                             
                                                                                                  
  
          
                                                                                                               
                                                                                                            
                                                                                                               
                                                                                                        
                                                                                                           
                                                                                                               
                                                                                                            
                                                                                                           
                                                                                                              
                                                                                                          
                                                          
                                                                                                           
                                                                                                           
                                            
                                                                                                               
                                                                                                              
                                   
                                                                                                       
                                                                                                           
                                                                 
                                                                                                       
                                                                                                             
                                                                                 
                                                                    
                                                                                                          
                                                                                               
                                                                                                       
                                                                                                             
                                                                                                               
                                                                                                             
                                                                                                             
                                         
                                                                                                         
                                                                                                             
                                                          
                                                                                                     
                                                                                                             
                                                                                                      
                                                                                                              
                                                                                                               
                                                                                                         
                                    
                                                                                                         
                                                                                                            
                                                                                                               
                                                                      
                                                               
                                                                        
  
                                          
  
                                                                                                            */
#if ( (HCF_TYPE) & HCF_TYPE_HII5 ) == 0
#if CFG_SCAN != CFG_TALLIES - HCF_ACT_TALLIES + HCF_ACT_SCAN
err: "maintenance" apparently inviolated the underlying assumption about the numerical values of these macros
#endif
#endif //              
#if CFG_PRS_SCAN != CFG_TALLIES - HCF_ACT_TALLIES + HCF_ACT_PRS_SCAN
err: "maintenance" apparently inviolated the underlying assumption about the numerical values of these macros
#endif
int
hcf_action( IFBP ifbp, hcf_16 action )
{
	int rc = HCF_SUCCESS;

	HCFASSERT( ifbp->IFB_Magic == HCF_MAGIC, ifbp->IFB_Magic );
#if HCF_INT_ON
	HCFLOGENTRY( action == HCF_ACT_INT_FORCE_ON ? HCF_TRACE_ACTION_KLUDGE : HCF_TRACE_ACTION, action );                                                      /*   */
#if (HCF_SLEEP)
	HCFASSERT( ifbp->IFB_IntOffCnt != 0xFFFE || action == HCF_ACT_INT_OFF,
		   MERGE_2( action, ifbp->IFB_IntOffCnt ) );
#else
	HCFASSERT( ifbp->IFB_IntOffCnt != 0xFFFE, action );
#endif //          
	HCFASSERT( ifbp->IFB_IntOffCnt != 0xFFFF ||
		   action == HCF_ACT_INT_OFF || action == HCF_ACT_INT_FORCE_ON,  action );
	HCFASSERT( ifbp->IFB_IntOffCnt <= 16 || ifbp->IFB_IntOffCnt >= 0xFFFE,
		   MERGE_2( action, ifbp->IFB_IntOffCnt ) ); //                                            
#endif //           

	switch (action) {
#if HCF_INT_ON
		hcf_16  i;
	case HCF_ACT_INT_OFF:                     //                             
#if HCF_SLEEP
		if ( ifbp->IFB_IntOffCnt == 0xFFFE ) {  //                                                    
			ifbp->IFB_IntOffCnt++;                      //                         
			OPW(HREG_IO, HREG_IO_WAKEUP_ASYNC );        //               
			OPW(HREG_IO, HREG_IO_WAKEUP_ASYNC );        //                                                 
			//                                                
			MSF_WAIT(800);                              //                                             
//                                                                        
//                                                                                                                       
//                                      
//    
//                                                                                               
		}
#endif //          
	/* */   ifbp->IFB_IntOffCnt++;
//          
		i = IPW( HREG_INT_EN );
		OPW( HREG_INT_EN, 0 );
		if ( i & 0x1000 ) {
			rc = HCF_ERR_NO_NIC;
		} else {
			if ( i & IPW( HREG_EV_STAT ) ) {
				rc = HCF_INT_PENDING;
			}
		}
		break;

	case HCF_ACT_INT_FORCE_ON:                //                                    
		ifbp->IFB_IntOffCnt = 0;
		//                              

	case HCF_ACT_INT_ON:                      //                            
	/* */   if ( ifbp->IFB_IntOffCnt-- == 0 && ifbp->IFB_CardStat == 0 ) {
			                          //                              
#if HCF_DMA
			if ( ifbp->IFB_CntlOpt & USE_DMA ) {
				i = HREG_EV_INFO | HREG_EV_RDMAD | HREG_EV_TDMAD | HREG_EV_TX_EXT;  //                    
			} else
#endif //        
			{
				i = HREG_EV_INFO | HREG_EV_RX | HREG_EV_TX_EXT;                     //                        
				if ( ifbp->IFB_RscInd == 0 ) {
					i |= HREG_EV_ALLOC;                                         //                            
				}
			}
#if HCF_SLEEP
			if ( ( IPW(HREG_EV_STAT) & ( i | HREG_EV_SLEEP_REQ ) ) == HREG_EV_SLEEP_REQ ) {
				//                                                        
				//                                                                                         
				ifbp->IFB_IntOffCnt--;          //              
				OPW( HREG_INT_EN, i | HREG_EV_TICK );
				OPW( HREG_EV_ACK, HREG_EV_SLEEP_REQ | HREG_EV_TICK | HREG_EV_ACK_REG_READY );
			} else
#endif //          
			{
				OPW( HREG_INT_EN, i | HREG_EV_SLEEP_REQ );
			}
		}
		break;
#endif //           

#if (HCF_SLEEP) & HCF_DDS
	case HCF_ACT_SLEEP:                       //                  
		hcf_cntl( ifbp, HCF_CNTL_DISABLE );
		cmd_exe( ifbp, HCMD_SLEEP, 0 );
		break;
//                                                                
//                                                                  
//                                                                     
//                                       
//                                                                                         
//                                                                                                        
//                                                                                                       
//                                                                                                      
//                                                                  
//                                                 
//        
#endif //        

	case HCF_ACT_RX_ACK:                      //            
	/* */   if ( ifbp->IFB_RxFID ) {
			DAWA_ACK( HREG_EV_RX );
		}
		ifbp->IFB_RxFID = ifbp->IFB_RxLen = 0;
		break;

  /* */ case  HCF_ACT_PRS_SCAN:                   //                       
		OPW( HREG_PARAM_1, 0x3FFF );
		//                               
	case HCF_ACT_TALLIES:                     //                              
#if ( (HCF_TYPE) & HCF_TYPE_HII5 ) == 0
	case HCF_ACT_SCAN:                        //                           
#endif //              
		/*                                                                                                
                                                                                                  */
		cmd_exe( ifbp, HCMD_INQUIRE, action - HCF_ACT_TALLIES + CFG_TALLIES );
		break;

	default:
		HCFASSERT( DO_ASSERT, action );
		break;
	}
	//                                                                                                         
	HCFLOGEXIT( HCF_TRACE_ACTION );
	return rc;
} //           


/*                                                                                                           
  
                                                      
                                                                             
                                                       
                                                                                           
                                                                                 
                                                                                   
                                                                        
  
            
                                               
                                                                                        
                                               
                                                
                                                 
                                                
                                                   
                                                     
                                                   
                                                          
                                                   
                                                   
                                                   
                                                   
                                                   
                                                   
                                                   
  
          
                
                
                   
                        
                     
  
              
                                                    
                                                                                                     
                                             
                     
                      
                      
                         
                       
  
                                                                              
                                                                                                         
                                                                                            
                                                                   
  
                                                                                                        
                   
  
                                                                                                             
                               
                                          
                                                 
                                                 
                                                                                             
  
                                                                                                   
                
  
                                       
                                                                
                                        
                                         
  
                                      
                                                                
                                                                                                   
                               
                                                                                                          
                                                                                                           
                                            
                                               
                                                                                           
                                                                                  
  
                                       
                                                  
                                          
                                                
  
                                                                                                       
  
                                                                                                                
                                                                                                            
                                                                                        
  
                  
                                                
                                     
                                                                                    
                                                                                              
                                         
                                                                                                               
                    
  
          
                                                       
                                                                                                      
                                                                    
                                                                                                          
                                          
  
                                          
  
                                                                                                            */
int
hcf_cntl( IFBP ifbp, hcf_16 cmd )
{
	int rc = HCF_ERR_INCOMP_FW;
#if HCF_ASSERT
	{   int x = cmd & HCMD_CMD_CODE;
		if ( x == HCF_CNTL_CONTINUE ) x &= ~HCMD_RETRY;
		else if ( (x == HCMD_DISABLE || x == HCMD_ENABLE) && ifbp->IFB_FWIdentity.comp_id == COMP_ID_FW_AP ) {
			x &= ~HFS_TX_CNTL_PORT;
		}
		HCFASSERT( x==HCF_CNTL_ENABLE  || x==HCF_CNTL_DISABLE    || HCF_CNTL_CONTINUE ||
			   x==HCF_CNTL_CONNECT || x==HCF_CNTL_DISCONNECT, cmd );
	}
#endif //           
//                          
//                                                 
//                  
	HCFLOGENTRY( HCF_TRACE_CNTL, cmd );
	if ( ifbp->IFB_CardStat == 0 ) {                                                                 /* */
	/* */   rc = cmd_exe( ifbp, cmd, 0 );
#if (HCF_SLEEP) & HCF_DDS
		ifbp->IFB_TickCnt = 0;              //                                              
#endif //        
	}
#if HCF_DMA
	//                                                                                                       
	//                                                                                                      
	//                                                                                           
	if ( ifbp->IFB_CntlOpt & USE_DMA ) {
		hcf_io io_port = ifbp->IFB_IOBase;
		DESC_STRCT *p;
		if ( cmd == HCF_CNTL_DISABLE || cmd == HCF_CNTL_ENABLE ) {
			OUT_PORT_DWORD( (io_port + HREG_DMA_CTRL), DMA_CTRLSTAT_RESET);                     /* */
			ifbp->IFB_CntlOpt &= ~DMA_ENABLED;
		}
		if ( cmd == HCF_CNTL_ENABLE ) {
			OUT_PORT_DWORD( (io_port + HREG_DMA_CTRL), DMA_CTRLSTAT_GO);
			/*                                                                                                        
                                                                                      
    */
			ifbp->IFB_CntlOpt |= DMA_ENABLED;
			HCFASSERT( NT_ASSERT, NEVER_TESTED );
			//                                                                                  
			p = ifbp->IFB_FirstDesc[DMA_RX];
			if (p != NULL) {   //                                                                  
				if ( 1 )    { //                 
					HCFASSERT( NT_ASSERT, NEVER_TESTED );
					put_frame_lst( ifbp, ifbp->IFB_FirstDesc[DMA_RX], DMA_RX );
					if ( ifbp->IFB_FirstDesc[DMA_RX] ) {
						put_frame_lst( ifbp, ifbp->IFB_FirstDesc[DMA_RX]->next_desc_addr, DMA_RX );
					}
				} else {
					while ( p ) {
						//                                        
						p->BUF_CNT |= DESC_DMA_OWNED;
						p = p->next_desc_addr;
					}
					//                                                          
					p = ifbp->IFB_FirstDesc[DMA_RX];
					OUT_PORT_DWORD( (io_port + HREG_RXDMA_PTR32), p->desc_phys_addr);
				}  //               
			}
		}
	}
#endif //        
	HCFASSERT( rc == HCF_SUCCESS, rc );
	HCFLOGEXIT( HCF_TRACE_CNTL );
	return rc;
} //         


/*                                                                                                           
  
                                                             
                                                            
                                                       
  
            
                                                      
                                                               
                                        
                                                     
  
          
                
                       
                      
                            
                                      
                   
                     
  
                                           
                                                    
                                                      
                                                      
                                                                                 
                                                                                     
                                                                                 
                                                                                         
                                                                                          
                                                                                              
                                                                                                 
                                                                                                      
  
             
                                                                                         
  
                                                                                                      
              
  
              
                                                                                                               
                                                                                                             
                                                                                                              
                                                                                               
                                                                                                           
                                                                                          
  
                                                                                               
                                 
                                                                                                                 
                                                                                            
  
                                                                            
  
                  
                                                                   
                                                                                     
  
          
  
                                                                                                        
                                                                              
    
                                                                 
                                                                                                        
                                                      
                                                                              
                                                                                                           
                                
                                                                                                        
                                                                                                               
                                                                                                           
                                                                                                 
                                 
                                                                                                          
                        
                                                                                                          
                  
                                                                
                                                                                                 
                                                                          
                                                                  
                                                                                                                
                                                                                                                
                                                                                                               
                                                                       
                      
                                               
                                               
                                                                                                            
                                                                                                               
                                                                    
                                                                     
                                                                                                               
                                 
                                                                                                         
                      
                                                                                                       
                                                                                                            
                                                                                                         
                                              
                                                                                                 
                                                                                     
                                                                                                               
                                                                                                               
          
  
         
                                                                                                               
                                                
                                                                                                            
                                                                                                          
                                                                                                        
             
  
         
                                                                                
                                                           
                                                                                                            
                   
                                                                                                             
                          
                                                                 
                                          
  
                                                                                                            */
int
hcf_connect( IFBP ifbp, hcf_io io_base )
{
	int         rc = HCF_SUCCESS;
	hcf_io      io_addr;
	hcf_32      prot_cnt;
	hcf_8       *q;
	LTV_STRCT   x;
#if HCF_ASSERT
	hcf_16 xa = ifbp->IFB_FWIdentity.typ;
	/*                                                                                                    
                                                                               
                                                              
                                                                                        
  */
#endif //           

	if ( io_base == HCF_DISCONNECT ) {                  //          
		io_addr = ifbp->IFB_IOBase;
		OPW( HREG_INT_EN, 0 );      //                                                              
	} else {                                            //                                             
		io_addr = io_base;
	}

#if 0 //                                                                                           
#if HCF_SLEEP
	OUT_PORT_WORD( .....+HREG_IO, HREG_IO_WAKEUP_ASYNC );       //                   
	MSF_WAIT(800);                              //                                             
	note that MSF_WAIT uses not yet defined!!!! IFB_IOBase and IFB_TickIni (via PROT_CNT_INI)
	so be careful if this code is restored
#endif //          
#endif //  

#if ( (HCF_TYPE) & HCF_TYPE_PRELOADED ) == 0    //                                         
	OUT_PORT_WORD( io_addr + HREG_CMD, HCMD_INI );          //                   
	prot_cnt = INI_TICK_INI;
	HCF_WAIT_WHILE( (IN_PORT_WORD( io_addr +  HREG_EV_STAT) & HREG_EV_CMD) == 0 );
	OUT_PORT_WORD( (io_addr + HREG_IO), HREG_IO_SRESET );   //                                             
#endif //                   
	for ( q = (hcf_8*)(&ifbp->IFB_Magic); q > (hcf_8*)ifbp; *--q = 0 ) /*   */;                     /*   */
	ifbp->IFB_Magic     = HCF_MAGIC;
	ifbp->IFB_Version   = IFB_VERSION;
#if defined MSF_COMPONENT_ID //                                                
	xxxx[xxxx_PRI_IDENTITY_OFFSET] = NULL;      //                                    
	xxxx[xxxx_PRI_IDENTITY_OFFSET+1] = NULL;    //                                    
#endif //                 
#if (HCF_TALLIES) & ( HCF_TALLIES_NIC | HCF_TALLIES_HCF )
	ifbp->IFB_TallyLen = 1 + 2 * (HCF_NIC_TAL_CNT + HCF_HCF_TAL_CNT);   //                                       
	ifbp->IFB_TallyTyp = CFG_TALLIES;           //                         
#endif //                                  
	ifbp->IFB_IOBase    = io_addr;              //                                                     
	ifbp->IFB_IORange   = HREG_IO_RANGE;
	ifbp->IFB_CntlOpt   = USE_16BIT;
#if HCF_ASSERT
	assert_ifbp = ifbp;
	ifbp->IFB_AssertLvl = 1;
#if (HCF_ASSERT) & HCF_ASSERT_LNK_MSF_RTN
	if ( io_base != HCF_DISCONNECT ) {
		ifbp->IFB_AssertRtn = (MSF_ASSERT_RTNP)msf_assert;                                          /*   */
	}
#endif //                       
#if (HCF_ASSERT) & HCF_ASSERT_MB                //                                                           
	ifbp->IFB_AssertStrct.len = sizeof(ifbp->IFB_AssertStrct)/sizeof(hcf_16) - 1;
	ifbp->IFB_AssertStrct.typ = CFG_MB_INFO;
	ifbp->IFB_AssertStrct.base_typ = CFG_MB_ASSERT;
	ifbp->IFB_AssertStrct.frag_cnt = 1;
	ifbp->IFB_AssertStrct.frag_buf[0].frag_len =
		( offsetof(IFB_STRCT, IFB_AssertLvl) - offsetof(IFB_STRCT, IFB_AssertLine) ) / sizeof(hcf_16);
	ifbp->IFB_AssertStrct.frag_buf[0].frag_addr = &ifbp->IFB_AssertLine;
#endif //              
#endif //           
	IF_PROT_TIME( prot_cnt = ifbp->IFB_TickIni = INI_TICK_INI );
#if ( (HCF_TYPE) & HCF_TYPE_PRELOADED ) == 0
	//                                                    
	OPW( HREG_IO, 0x0000 );                     //                                                         
	HCF_WAIT_WHILE( (IPW( HREG_EV_STAT) & HREG_EV_CMD) == 0 );
	IF_PROT_TIME( HCFASSERT( prot_cnt, IPW( HREG_EV_STAT) ) );
	IF_PROT_TIME( if ( prot_cnt ) prot_cnt = ifbp->IFB_TickIni );
#endif //                   
	//                                                    
	HCFASSERT( DO_ASSERT, MERGE_2( HCF_ASSERT, 0xCAF0 ) ); //                                                           
	HCFASSERT( xa != CFG_FW_IDENTITY, 0 );       //                                                                    
	HCFASSERT( ((hcf_32)(void*)ifbp & (HCF_ALIGN-1) ) == 0, (hcf_32)(void*)ifbp );
	HCFASSERT( (io_addr & 0x003F) == 0, io_addr );
	                                        //                           
	if (IPW( HREG_CMD ) & HCMD_BUSY ) {                                                             /*   */
		//                                                        
		OPW( HREG_EV_ACK, ~HREG_EV_SLEEP_REQ );
		                                //                                          
		                                //                                          
		HCF_WAIT_WHILE( ( IPW( HREG_CMD ) & HCMD_BUSY ) && (IPW( HREG_EV_STAT) & HREG_EV_CMD) == 0 );
		IF_PROT_TIME( HCFASSERT( prot_cnt, IPW( HREG_EV_STAT) ) ); /*                                                      */
	}
	OPW( HREG_EV_ACK, ~HREG_EV_SLEEP_REQ );
#if ( (HCF_TYPE) & HCF_TYPE_PRELOADED ) == 0                                                        /*  */
	(void)cmd_exe( ifbp, HCMD_INI, 0 );
#endif //                   
	if ( io_base != HCF_DISCONNECT ) {
		rc = init( ifbp );                                                                          /*  */
		if ( rc == HCF_SUCCESS ) {
			x.len = 2;
			x.typ = CFG_NIC_BUS_TYPE;
			(void)hcf_get_info( ifbp, &x );
			ifbp->IFB_BusType = x.val[0];
			//                                                                                           
			if ( x.len == 0 || x.val[0] == 0x0002 || x.val[0] == 0x0003 ) {
#if (HCF_IO) & HCF_IO_32BITS
				ifbp->IFB_CntlOpt &= ~USE_16BIT;            //               
#endif //              
#if HCF_DMA
				ifbp->IFB_CntlOpt |= USE_DMA;               //       
#else
				ifbp->IFB_IORange = 0x40 /*                    */;
#endif //        
			}
		}
	} else HCFASSERT(  ( ifbp->IFB_Magic ^= HCF_MAGIC ) == 0, ifbp->IFB_Magic ) /*   */;
	/*                                                                                   */
	ifbp->IFB_IOBase = io_base;                                                                     /*  */
	return rc;
} //            

#if HCF_DMA
/*                                                                                                           
                         
                                                                                                       
  
                                                                                                        
                                                     
  
                                                                                                           
                                                                                                  
                      
                                                                                                         
                                                                            
                                                                                                         
                                                                                     
                                                                                                           
                                      
  
                                                                                                               
                                                                      
                                                                                                                                                
                                                          
  
                                                            
  
                                                                                                      
                                                      
  
                    
                                                                  
  
                                                                                                            */
HCF_STATIC DESC_STRCT*
get_frame_lst( IFBP ifbp, int tx_rx_flag )
{

	DESC_STRCT *head = ifbp->IFB_FirstDesc[tx_rx_flag];
	DESC_STRCT *copy, *p, *prev;

	HCFASSERT( tx_rx_flag == DMA_RX || tx_rx_flag == DMA_TX, tx_rx_flag );
	                                        //            
	if ( head ) {
		                                //                                                
		p = prev = head;
		while ( ( p->BUF_SIZE & DESC_EOP ) == 0 && p->next_desc_addr ) {
			if ( ( ifbp->IFB_CntlOpt & DMA_ENABLED ) == 0 ) {   //                                
				p->BUF_CNT &= DESC_CNT_MASK;
			}
			prev = p;
			p = p->next_desc_addr;
		}
		                                //                 
		if ( ifbp->IFB_CntlOpt & DMA_ENABLED ) {
			                        //                                                  
			                        //                                                  
			if ( p->BUF_CNT & DESC_DMA_OWNED || head->next_desc_addr == NULL ) {
				                //                                             
				head = NULL;
			}
		}
	}
	                                        //                             
	if ( head ) {
		                                //                                                                   
		if ( head->next_desc_addr == NULL ) {
			                        //                          
			/*                                  */ ifbp->IFB_FirstDesc[tx_rx_flag] = NULL;
			                        //       
		} else {
			                        //                                                      
			                        //                                                         
			copy = head;
			head = head->next_desc_addr;
			                        //                                                                          
			copy->buf_phys_addr = p->buf_phys_addr;
			copy->buf_addr = p->buf_addr;
			copy->BUF_SIZE = p->BUF_SIZE &= DESC_CNT_MASK;  //                                         
			copy->BUF_CNT = p->BUF_CNT &= DESC_CNT_MASK;    //                         
#if (HCF_EXT) & HCF_DESC_STRCT_EXT
			copy->DESC_MSFSup = p->DESC_MSFSup;
#endif //                   
			                        //                                  
			p->buf_addr = NULL;
			                        //                                                                        
			prev->next_desc_addr = copy;
			                        //                                                           
			copy->next_desc_addr = NULL;
			copy->next_desc_phys_addr = 0xDEAD0000; //                                    
			                        //                                                                 
			ifbp->IFB_FirstDesc[tx_rx_flag] = p;
		}
		                                //                                       
		head->BUF_SIZE &= DESC_CNT_MASK;
		//                                                          
		head->next_desc_phys_addr = 0xDEAD0000; //                                    
	}
	                                        //                                           
	return head;
} //              


/*                                                                                                           
                         
  
                
  
                                                            
  
                    
                                                                  
  
                                                                                                              
                  
                       
                            
                                                                   
                                                                                                     
                                                              
                                                              
                                                                             
                                                      

         
                                                                                                             
                            

                                                                                               
                                                                                            
                                                                                              
                                                     
                                                     
                                                                   
                                                                                             */
HCF_STATIC void
put_frame_lst( IFBP ifbp, DESC_STRCT *descp, int tx_rx_flag )
{
	DESC_STRCT  *p = descp;
	hcf_16 port;

	HCFASSERT( ifbp->IFB_CntlOpt & USE_DMA, ifbp->IFB_CntlOpt); //                                            
	HCFASSERT( tx_rx_flag == DMA_RX || tx_rx_flag == DMA_TX, tx_rx_flag );
	HCFASSERT( p , 0 );

	while ( p ) {
		HCFASSERT( ((hcf_32)p & 3 ) == 0, (hcf_32)p );
		HCFASSERT( (p->BUF_CNT & ~DESC_CNT_MASK) == 0, p->BUF_CNT );
		HCFASSERT( (p->BUF_SIZE & ~DESC_CNT_MASK) == 0, p->BUF_SIZE );
		p->BUF_SIZE &= DESC_CNT_MASK;                   //                                                   
		p->BUF_CNT &= tx_rx_flag == DMA_RX ? 0 : DESC_CNT_MASK; //                                                   
		p->BUF_CNT |= DESC_DMA_OWNED;
		if ( p->next_desc_addr ) {
//                                                                                        
			HCFASSERT( p->next_desc_addr->desc_phys_addr, (hcf_32)p->next_desc_addr );
			p->next_desc_phys_addr = p->next_desc_addr->desc_phys_addr;
		} else {                                    //
			p->next_desc_phys_addr = 0;
			if ( p->buf_addr == NULL ) {            //                 
				HCFASSERT( descp == p, (hcf_32)descp );  //                        
				HCFASSERT( ifbp->IFB_FirstDesc[tx_rx_flag] == NULL, (hcf_32)ifbp->IFB_FirstDesc[tx_rx_flag]);
				HCFASSERT( ifbp->IFB_LastDesc[tx_rx_flag] == NULL, (hcf_32)ifbp->IFB_LastDesc[tx_rx_flag]);
				descp->BUF_CNT = 0; //                   
				ifbp->IFB_FirstDesc[tx_rx_flag] = descp;
//                                                                                              
				                //                           
				                //                                              
				                //                                         
//                                                   
			} else {                                //                                                  
				HCFASSERT( ifbp->IFB_FirstDesc[tx_rx_flag], (hcf_32)descp );
				HCFASSERT( ifbp->IFB_LastDesc[tx_rx_flag], (hcf_32)descp );
				HCFASSERT( ifbp->IFB_LastDesc[tx_rx_flag]->next_desc_addr == NULL,
					   (hcf_32)ifbp->IFB_LastDesc[tx_rx_flag]->next_desc_addr);
//                                            
				ifbp->IFB_LastDesc[tx_rx_flag]->next_desc_addr = descp;
				ifbp->IFB_LastDesc[tx_rx_flag]->next_desc_phys_addr = descp->desc_phys_addr;
				port = HREG_RXDMA_PTR32;
				if ( tx_rx_flag ) {
					p->BUF_SIZE |= DESC_EOP;    //                                                                        
					descp->BUF_SIZE |= DESC_SOP;
					port = HREG_TXDMA_PTR32;
				}
				OUT_PORT_DWORD( (ifbp->IFB_IOBase + port), descp->desc_phys_addr );
			}
			ifbp->IFB_LastDesc[tx_rx_flag] = p;
		}
		p = p->next_desc_addr;
	}
} //              


/*                                                                                                           
  
                                                        
                                                                            
                                                                    
  
            
                                               
  
          
                           
  
              
                                                                                                              
                                                                                                           
                                                                                
                                                                                                              
                                                                                                              
                                                                                                       
             
                                                                                                             
                                                                                                     
                                                                                                               
          
                                                                                                           
                            
                                                            
                                                                 
                                                                                                           
                                                                        
                                                                 
  
                                                                                                              
                                                                                                            
                                                                                                     
  
                                                                                       
                                                                                          
                                                                                                            
               
                                                                                                 
  
          
  
                                          
  
                                                                                                            */

DESC_STRCT*
hcf_dma_rx_get (IFBP ifbp)
{
	DESC_STRCT *descp;  //                              

	descp = get_frame_lst( ifbp, DMA_RX );
	if ( descp && descp->buf_addr ) {

		                                //                                         
#if (HCF_ENCAP) == HCF_ENC
		int i;
		DESC_STRCT *p = descp->next_desc_addr;  //                                  
		HCFASSERT(p, 0);
		//                                                                                     
		//                                        
		i = *(wci_recordp)&descp->buf_addr[HFS_STAT] & ( HFS_STAT_MSG_TYPE | HFS_STAT_ERR );
		if ( i == HFS_STAT_TUNNEL ||
		     ( i == HFS_STAT_1042 && hcf_encap( (wci_bufp)&p->buf_addr[HCF_DASA_SIZE] ) != ENC_TUNNEL )) {
			//                                                                             
			HCFASSERT( p->BUF_CNT == (p->buf_addr[5] + (p->buf_addr[4]<<8) + 2*6 + 2 - 8), p->BUF_CNT );
			//                      
			HCFASSERT(p->BUF_SIZE >=8, p->BUF_SIZE);
			//                                                                     
			for ( i=3; i >= 0; i--) p->buf_addr[i+8] = p->buf_addr[i];
			//                                                         
			for ( i=0; i<8; i++) p->buf_addr[i] = descp->buf_addr[HFS_ADDR_DEST + i];
			//                                   
			descp->BUF_CNT = HFS_ADDR_DEST;
		}
	}
#endif //        
	if ( descp == NULL ) ifbp->IFB_DmaPackets &= (hcf_16)~HREG_EV_RDMAD;  //                                        
	HCFLOGEXIT( HCF_TRACE_DMA_RX_GET );
	return descp;
} //               


/*                                                                                                           
  
                                                                    
                                                     
                                                
  
            
                                               
                                            
  
                     
  
              
                                                                                                   
                                                                                       
                                                                                          
                                                                                                             
                                                                                                        
                                                                                                           
  
                  
                                                
                                                                        
                                                  
                                                                                                               
                             
                                                                   
                                                                         
                                                                                                   
                              
                       
                            
                                                                   
                                                                                                     
                                                              
                                                              
                                                                             
                                                        
  
          
  
  
                                          
  
                                                                                                            */
void
hcf_dma_rx_put( IFBP ifbp, DESC_STRCT *descp )
{

	HCFLOGENTRY( HCF_TRACE_DMA_RX_PUT, 0xDA01 );
	HCFASSERT( ifbp->IFB_Magic == HCF_MAGIC, ifbp->IFB_Magic );
	HCFASSERT_INT;

	put_frame_lst( ifbp, descp, DMA_RX );
#if HCF_ASSERT && (HCF_ENCAP) == HCF_ENC
	if ( descp->buf_addr ) {
		HCFASSERT( descp->BUF_SIZE == HCF_DMA_RX_BUF1_SIZE, descp->BUF_SIZE );
		HCFASSERT( descp->next_desc_addr, 0 ); //                                                          
		//                                                                                            
		HCFASSERT( descp->next_desc_addr->BUF_SIZE >= 12, descp->next_desc_addr->BUF_SIZE );
	}
#endif //                    
	HCFLOGEXIT( HCF_TRACE_DMA_RX_PUT );
} //               


/*                                                                                                           
  
                                                        
                                                                                           
                                                                                            
                                                           
  
            
                                               
  
          
                                      
  
              
                                 
                                                                                               
                                                                                          
                           
  
          
  
         
  
                                          
  
                                                                                                            */
DESC_STRCT*
hcf_dma_tx_get( IFBP ifbp )
{
	DESC_STRCT *descp;  //                              

	descp = get_frame_lst( ifbp, DMA_TX );
	if ( descp && descp->buf_addr ) {
		                                //                                         
#if (HCF_ENCAP) == HCF_ENC
		if ( ( descp->BUF_CNT == HFS_TYPE )) {
			//                                
			descp->next_desc_addr->buf_phys_addr -= HCF_DASA_SIZE;
			descp->next_desc_addr->BUF_CNT       += HCF_DASA_SIZE;
		}
#endif //        
	}
	if ( descp == NULL ) {  //                                        
		ifbp->IFB_DmaPackets &= (hcf_16)~HREG_EV_TDMAD;
	}
	HCFLOGEXIT( HCF_TRACE_DMA_TX_GET );
	return descp;
} //               


/*                                                                                                           
  
                                                                                    
                                                                                              
                                                
  
            
                                               
                                                                        
                                                        
  
                     
  
              
                                                                                       
                                                                                          
  
                                                                                                       
                                                                                                             
                                                                                                   
                                                             
                                                          
                                                                                                  
                                                                                                         
                                                                                                              
                                                            
                                                                                                              
                                                                                                     
                 
                                                                                                 
  
                                                                                                     
                                                                                                             
                               
                                                                               
                               
                                                                                  
                          
                                                        
                                                                             
                                                                                                                                                                                                                                                          
  
                                                                                                             
                
  
                                                                                                            
                                                     
                                                                                     
                                                                                                            
                                                                                                           
                                                                                                
  
                                                                           
                                                                                                            
                                                                                               
                                                                            
  
                                                                                                          
          
  
                                                                                                    
                                                                                                                            
  
                  
                                                
                                                   
                                                                        
                                                  
                                                                   
                                                                                                      
                                                                                                       
                                                                    
                                                                                                         
                                                                                                   
                              
                       
                            
                                                                   
                                                                                                     
                                                              
                                                              
                                                                             
                                                        
          
  
         
  
                                          
  
  
                                                                                             
                                                                                                             
                             
                                                                                     
                                            
                                                                                                           
                                                                                                              
                                      
                                
                                                                                                              
                                                   
                                                                                                           
                                                
                                                                                                        
                                                                                                             
                                                               
                                                                                  
                                                                            
                                                                        
                                                                     
                                                                       
                                                    
                                      
                                           
                                                                                                            */
void
hcf_dma_tx_put( IFBP ifbp, DESC_STRCT *descp, hcf_16 tx_cntl )
{
	DESC_STRCT  *p = descp->next_desc_addr;
	int         i;

#if HCF_ASSERT
	int x = ifbp->IFB_FWIdentity.comp_id == COMP_ID_FW_AP ? tx_cntl & ~HFS_TX_CNTL_PORT : tx_cntl;
	HCFASSERT( (x & ~HCF_TX_CNTL_MASK ) == 0, tx_cntl );
#endif //           
	HCFLOGENTRY( HCF_TRACE_DMA_TX_PUT, 0xDA03 );
	HCFASSERT( ifbp->IFB_Magic == HCF_MAGIC, ifbp->IFB_Magic );
	HCFASSERT_INT;
	HCFASSERT( ( ifbp->IFB_CntlOpt & (USE_DMA|DMA_ENABLED) ) == (USE_DMA|DMA_ENABLED), ifbp->IFB_CntlOpt);

	if ( descp->buf_addr ) {
		*(hcf_16*)(descp->buf_addr + HFS_TX_CNTL) = tx_cntl;                                            /* */
#if (HCF_ENCAP) == HCF_ENC
		HCFASSERT( descp->next_desc_addr, 0 );                                   //                     
		HCFASSERT( descp->BUF_CNT == HFS_ADDR_DEST, descp->BUF_CNT );    //                                    
		HCFASSERT( descp->BUF_SIZE >= HCF_DMA_TX_BUF1_SIZE, descp->BUF_SIZE );   //                                 
		HCFASSERT( p->BUF_CNT >= 14, p->BUF_CNT );                  //                                        

		descp->buf_addr[HFS_TYPE-1] = hcf_encap(&descp->next_desc_addr->buf_addr[HCF_DASA_SIZE]);       /* */
		if ( descp->buf_addr[HFS_TYPE-1] != ENC_NONE ) {
			for ( i=0; i < HCF_DASA_SIZE; i++ ) {                                                       /* */
				descp->buf_addr[i + HFS_ADDR_DEST] = descp->next_desc_addr->buf_addr[i];
			}
			i = sizeof(snap_header) + 2 - ( 2*6 + 2 );
			do { i += p->BUF_CNT; } while ( ( p = p->next_desc_addr ) != NULL );
			*(hcf_16*)(&descp->buf_addr[HFS_LEN]) = CNV_END_SHORT(i);   //                                                                       
			for ( i=0; i < sizeof(snap_header) - 1; i++) {
				descp->buf_addr[HFS_TYPE - sizeof(snap_header) + i] = snap_header[i];
			}
			descp->BUF_CNT = HFS_TYPE;                                                                  /* */
			descp->next_desc_addr->buf_phys_addr    += HCF_DASA_SIZE;
			descp->next_desc_addr->BUF_CNT          -= HCF_DASA_SIZE;
		}
#endif //        
	}
	put_frame_lst( ifbp, descp, DMA_TX );
	HCFLOGEXIT( HCF_TRACE_DMA_TX_PUT );
} //               

#endif //        

/*                                                                                                           
  
                                                 
                                                                               
  
            
                                                                                   
  
          
                                                                   
                                                            
                                                                
  
             
                        
  
              
                                                                                                              
                                                                                                              
                                                                                                          
                                                                                                             
                                                    
  
          
  
                                                             
                                                                   
                                                     
                                                                                                              
                                                                           
                            
                                                               
                      
                                           
  
                                          
  
                                                                                                            */
HCF_STATIC hcf_8
hcf_encap( wci_bufp type )
{

	hcf_8   rc = ENC_NONE;                                                                                  /*   */
	hcf_16  t = (hcf_16)(*type<<8) + *(type+1);                                                             /*   */

	if ( t > 1500 ) {                                                                                   /*   */
		if ( t == 0x8137 || t == 0x80F3 ) {
			rc = ENC_TUNNEL;                                                                            /*   */
		} else {
			rc = ENC_1042;
		}
	}
	return rc;
} //          


/*                                                                                                           
  
                                                         
                                                                                                           
  
            
                                               
                                                                                                     
                                                                            
  
          
                                                          
                                
                                           
                                                     
                                                                             
                                                        
                                                                         
  
              
                                                                                                          
                                                                    
                                                                                                            
                                                                            
                                                                                                        
                                                                                                             
                                                                      
                                                                                                        
                                                                                                         
                                                                                                            
                                      
                                                                                                      
                                                                     
  
                                                                                                            
                                                                                                    
                                                                                                          
                                                                                                          
                                                                                                              
                                                              
  
                  
                                                
                                                                                    
                                               
                            
                                                                                                            
                                             
  
          
                                                                                                             
                                                                                                             
                                                                                                       
                                                                                                            
                                                                                                            
                                                                         
  
                                                                                                              
                                            
  
                   
                                                                                                             
                                                  
                                                                                                             
                                                                                                        

         
  
                                
                                                                                           
                                                   
                                                                                 
  
                                                                                                             
                                      
                                                                                                            
                                                                                                            
                                                                                                           
                                                                                                            
                                                                     
  
                                                                                                               
                                                                                                              
                                                                                                         
                          
  
                                                                                           
  
                                                                                                           
                                                                                                           
                                                                                                       
                  
                                                                                                            
                                                                                                     
                                                                                       
                                                             
  
             
                                                                                                           
  
  
                                          
  
                                                                                                            */
int
hcf_get_info( IFBP ifbp, LTVP ltvp )
{

	int         rc = HCF_SUCCESS;
	hcf_16      len = ltvp->len;
	hcf_16      type = ltvp->typ;
	wci_recordp p = &ltvp->len;     //                                        
	hcf_16      *q = NULL;              /*                                                        
                                                                              
          */
	int         i;

	HCFLOGENTRY( HCF_TRACE_GET_INFO, ltvp->typ );
	HCFASSERT( ifbp->IFB_Magic == HCF_MAGIC, ifbp->IFB_Magic );
	HCFASSERT_INT;
	HCFASSERT( ltvp, 0 );
	HCFASSERT( 1 < ltvp->len && ltvp->len <= HCF_MAX_LTV + 1, MERGE_2( ltvp->typ, ltvp->len ) );

	ltvp->len = 0;                              //                             
	//                       
	for ( i = 0; ( q = xxxx[i] ) != NULL && q[1] != type; i++ ) /*   */;

#if HCF_TALLIES
	if ( type == CFG_TALLIES ) {                                                    /* */
		(void)hcf_action( ifbp, HCF_ACT_TALLIES );
		q = (hcf_16*)&ifbp->IFB_TallyLen;
	}
#endif //            

	if ( type == CFG_MB_INFO ) {
		if ( ifbp->IFB_MBInfoLen ) {
			if ( ifbp->IFB_MBp[ifbp->IFB_MBRp] == 0xFFFF ) {
				ifbp->IFB_MBRp = 0; //                      
			}
			q = &ifbp->IFB_MBp[ifbp->IFB_MBRp];
			ifbp->IFB_MBRp += *q + 1;   //                   
			if ( ifbp->IFB_MBp[ifbp->IFB_MBRp] == 0xFFFF ) {
				ifbp->IFB_MBRp = 0;
			}
			ifbp->IFB_MBInfoLen = ifbp->IFB_MBp[ifbp->IFB_MBRp];
		}
	}

	if ( q != NULL ) {                      //                                       
		i = min( len, *q ) + 1;             //                                             
		while ( i-- ) {
			*p++ = *q;
#if (HCF_TALLIES) & HCF_TALLIES_RESET
			if ( q > &ifbp->IFB_TallyTyp && type == CFG_TALLIES ) {
				*q = 0;
			}
#endif //                  
			q++;
		}
	} else {                                //                                              
		if ( type == CFG_CNTL_OPT ) {                                       //                           
			ltvp->len = 2;
			ltvp->val[0] = ifbp->IFB_CntlOpt;
#if (HCF_EXT) & HCF_EXT_NIC_ACCESS
		} else if ( type == CFG_PROD_DATA ) {  //                                                         
			hcf_io      io_port;
			wci_bufp    pt;                 //                                                                                      
			OPW( HREG_AUX_PAGE, (hcf_16)(PLUG_DATA_OFFSET >> 7) );
			OPW( HREG_AUX_OFFSET, (hcf_16)(PLUG_DATA_OFFSET & 0x7E) );
			io_port = ifbp->IFB_IOBase + HREG_AUX_DATA;     //                                                
			p = ltvp->val;                  //                                        
			i = len - 1;
			if (i > 0 ) {
				pt = (wci_bufp)p;   //                                                       
				IN_PORT_STRING_8_16( io_port, pt, i ); //                   
			}
		} else if ( type == CFG_CMD_HCF ) {
#define P ((CFG_CMD_HCF_STRCT FAR *)ltvp)
			HCFASSERT( P->cmd == CFG_CMD_HCF_REG_ACCESS, P->cmd );       //                                     
			if ( P->cmd == CFG_CMD_HCF_REG_ACCESS ) {
				HCFASSERT( P->mode < ifbp->IFB_IOBase, P->mode );        //                    
				ltvp->len = min( len, 4 );                              //                  
				P->add_info = IPW( P->mode );
			}
#undef P
#endif //                   
#if (HCF_ASSERT) & HCF_ASSERT_PRINTF
		} else if (type == CFG_FW_PRINTF) {
			rc = fw_printf(ifbp, (CFG_FW_PRINTF_STRCT*)ltvp);
#endif //                  
		} else if ( type >= CFG_RID_FW_MIN ) {
//                                                                                                    
//                                                                                                  
//                                                                                   
		/*  */  if ( ( rc = cmd_exe( ifbp, HCMD_ACCESS, type ) ) == HCF_SUCCESS &&
				 ( rc = setup_bap( ifbp, type, 0, IO_IN ) ) == HCF_SUCCESS ) {
				get_frag( ifbp, (wci_bufp)&ltvp->len, 2*len+2 BE_PAR(2) );
				if ( IPW( HREG_STAT ) == 0xFFFF ) {                 //                
					ltvp->len = 0;
					HCFASSERT( DO_ASSERT, type );
				}
			}
	/*  */  } else HCFASSERT( DO_ASSERT, type ) /*   */; //                              
	}
	if ( len < ltvp->len ) {
		ltvp->len = len;
		if ( rc == HCF_SUCCESS ) {
			rc = HCF_ERR_LEN;
		}
	}
	HCFASSERT( rc == HCF_SUCCESS || ( rc == HCF_ERR_LEN && ifbp->IFB_AssertTrace & 1<<HCF_TRACE_PUT_INFO ),
		   MERGE_2( type, rc ) );                                                                /*  */
	HCFLOGEXIT( HCF_TRACE_GET_INFO );
	return rc;
} //             


/*                                                                                                           
  
                                                         
                                                                                              
  
            
                                               
                                                                                               
  
          
                
                
                                                          
                                                                 
                        
                                                                      
                                                                           
                 
  
              
                                                                                                          
                                                                                 
                                                                   
  
                                                                                                       
                                                                                          
                                                                                  
                                                                    
                                                                                           
                                                                                              
              
                                                                                     
                                                                                 
                                           
                             
                
                         
                      
                 
                    
                 
                
  
                                                                 
  
                  
                                                
                            
                                                              
                                                                                            
                                                                                    
                                                                                          
                                                                                            
                                       
  
          
  
         
                                                                                                       
                                                                                                               
          
                                                                                                        
               
  
     
  
                                          
  
                                                                                                            */

int
hcf_put_info( IFBP ifbp, LTVP ltvp )
{
	int rc = HCF_SUCCESS;

	HCFLOGENTRY( HCF_TRACE_PUT_INFO, ltvp->typ );
	HCFASSERT( ifbp->IFB_Magic == HCF_MAGIC, ifbp->IFB_Magic );
	HCFASSERT_INT;
	HCFASSERT( ltvp, 0 );
	HCFASSERT( 1 < ltvp->len && ltvp->len <= HCF_MAX_LTV + 1, ltvp->len );

	                                        //                                                        
#if (HCF_TYPE) & HCF_TYPE_WPA
	{
		hcf_16 i;
		hcf_32 FAR * key_p;

		if ( ltvp->typ == CFG_ADD_TKIP_DEFAULT_KEY || ltvp->typ == CFG_ADD_TKIP_MAPPED_KEY ) {
			key_p = (hcf_32*)((CFG_ADD_TKIP_MAPPED_KEY_STRCT FAR *)ltvp)->tx_mic_key;
			i = TX_KEY;     //                                    
			if ( ltvp->typ == CFG_ADD_TKIP_DEFAULT_KEY ) {
				key_p = (hcf_32*)((CFG_ADD_TKIP_DEFAULT_KEY_STRCT FAR *)ltvp)->tx_mic_key;
				i = CNV_LITTLE_TO_SHORT(((CFG_ADD_TKIP_DEFAULT_KEY_STRCT FAR *)ltvp)->tkip_key_id_info);
			}
			if ( i & TX_KEY ) { /*                    
                                                                                             */
				ifbp->IFB_MICTxCntl = (hcf_16)( HFS_TX_CNTL_MIC | (i & KEY_ID )<<8 );
				ifbp->IFB_MICTxKey[0] = CNV_LONGP_TO_LITTLE( key_p );
				ifbp->IFB_MICTxKey[1] = CNV_LONGP_TO_LITTLE( (key_p+1) );
			}
			i = ( i & KEY_ID ) * 2;
			ifbp->IFB_MICRxKey[i]   = CNV_LONGP_TO_LITTLE( (key_p+2) );
			ifbp->IFB_MICRxKey[i+1] = CNV_LONGP_TO_LITTLE( (key_p+3) );
		}
#define P ((CFG_REMOVE_TKIP_DEFAULT_KEY_STRCT FAR *)ltvp)
		if ( ( ltvp->typ == CFG_REMOVE_TKIP_MAPPED_KEY )    ||
		     ( ltvp->typ == CFG_REMOVE_TKIP_DEFAULT_KEY &&
		       ( (ifbp->IFB_MICTxCntl >> 8) & KEY_ID ) == CNV_SHORT_TO_LITTLE(P->tkip_key_id )
			     )
			) { ifbp->IFB_MICTxCntl = 0; }      //                  
#undef P
	}
#endif //             

	if ( ltvp->typ == CFG_PROG ) {
		rc = download( ifbp, (CFG_PROG_STRCT FAR *)ltvp );
	} else switch (ltvp->typ) {
#if (HCF_ASSERT) & HCF_ASSERT_RT_MSF_RTN
		case CFG_REG_ASSERT_RTNP:                                         //                     
#define P ((CFG_REG_ASSERT_RTNP_STRCT FAR *)ltvp)
			ifbp->IFB_AssertRtn = P->rtnp;
//                                                                                                
			HCFASSERT( DO_ASSERT, MERGE_2( HCF_ASSERT, 0xCAF1 ) );   //                                                           
#undef P
			break;
#endif //                      
#if (HCF_EXT) & HCF_EXT_INFO_LOG
		case CFG_REG_INFO_LOG:                                            //                   
			ifbp->IFB_RIDLogp = ((CFG_RID_LOG_STRCT FAR*)ltvp)->recordp;
			break;
#endif //                 
		case CFG_CNTL_OPT:                                                //               
			HCFASSERT( ( ltvp->val[0] & ~(USE_DMA | USE_16BIT) ) == 0, ltvp->val[0] );
			if ( ( ltvp->val[0] & USE_DMA ) == 0 ) ifbp->IFB_CntlOpt &= ~USE_DMA;
			ifbp->IFB_CntlOpt |=  ltvp->val[0] & USE_16BIT;
			break;

		case CFG_REG_MB:                                                  //                
#define P ((CFG_REG_MB_STRCT FAR *)ltvp)
			HCFASSERT( ( (hcf_32)P->mb_addr & 0x0001 ) == 0, (hcf_32)P->mb_addr );
			HCFASSERT( (P)->mb_size >= 60, (P)->mb_size );
			ifbp->IFB_MBp = P->mb_addr;
			/*                                                                                */
			ifbp->IFB_MBSize = ifbp->IFB_MBp == NULL ? 0 : P->mb_size;
			ifbp->IFB_MBWp = ifbp->IFB_MBRp = 0;
			ifbp->IFB_MBp[0] = 0;                                           //                         
			ifbp->IFB_MBInfoLen = 0;
			HCFASSERT( ifbp->IFB_MBSize >= 60 || ifbp->IFB_MBp == NULL, ifbp->IFB_MBSize );
#undef P
			break;
		case CFG_MB_INFO:                                                 //                      
			rc = put_info_mb( ifbp, (CFG_MB_INFO_STRCT FAR *)ltvp );
			break;

#if (HCF_EXT) & HCF_EXT_NIC_ACCESS
		case CFG_CMD_NIC:
#define P ((CFG_CMD_NIC_STRCT FAR *)ltvp)
			OPW( HREG_PARAM_2, P->parm2 );
			OPW( HREG_PARAM_1, P->parm1 );
			rc = cmd_exe( ifbp, P->cmd, P->parm0 );
			P->hcf_stat = (hcf_16)rc;
			P->stat = IPW( HREG_STAT );
			P->resp0 = IPW( HREG_RESP_0 );
			P->resp1 = IPW( HREG_RESP_1 );
			P->resp2 = IPW( HREG_RESP_2 );
			P->ifb_err_cmd = ifbp->IFB_ErrCmd;
			P->ifb_err_qualifier = ifbp->IFB_ErrQualifier;
#undef P
			break;
		case CFG_CMD_HCF:
#define P ((CFG_CMD_HCF_STRCT FAR *)ltvp)
			HCFASSERT( P->cmd == CFG_CMD_HCF_REG_ACCESS, P->cmd );       //                                     
			if ( P->cmd == CFG_CMD_HCF_REG_ACCESS ) {
				HCFASSERT( P->mode < ifbp->IFB_IOBase, P->mode );        //                    
				OPW( P->mode, P->add_info);
			}
#undef P
			break;
#endif //                   

#if (HCF_ASSERT) & HCF_ASSERT_PRINTF
		case CFG_FW_PRINTF_BUFFER_LOCATION:
			ifbp->IFB_FwPfBuff = *(CFG_FW_PRINTF_BUFFER_LOCATION_STRCT*)ltvp;
			break;
#endif //                  

		default:                      //                                                                     
			rc = put_info( ifbp, ltvp );
		}
	//                                                                                                  
	HCFLOGEXIT( HCF_TRACE_PUT_INFO );
	return rc;
} //             


/*                                                                                                           
  
                                                                                     
                                            
                                            
                                                                                                              
                                                                                     
  
            
                                               
                                                         
                              
                                                                                                               
                           
  
          
                                                                                           
                                                                                                              
                                                     
                                                          
                       
  
              
                                                                                                             
                                                           
  
                                                                                                            
                       
                                                                                  
                                                                                                           
                                                                                 
  
                                                                                                             
                                                                                                             
                                                                                                             
                                                                                                     
                                                                  
                                                                                                             
            
  
          
                                                                                                 
                                                     
  
               
                                                                                              
                                                                                       
                       
                                                                                            
                                                                              
                             
                                                                                               
                                
                                                              
                                                             
                                                                                                    
             
                           
                                          
                                                                              
  
                                                                                                        
                                                                                     
  
                                                                                              
                                                                    
                        
                    
              
                                                                                             
                                                                       
                                                                                            
                                                                                    
                                                                       
                                                                                           
                        
  
                                                                                      
                                                                                           
                                                                                             
                                                 
                                            
                 
                                                                    
  
                  
                                               
                                                    
                                                       
                            
                                                      
                                                                                    
                                               
                            
  
          
  
         
                                                                                      
                                                                                                             
                                                                                                          
                        
  
                                          
  
                                                                                                            */
int
hcf_rcv_msg( IFBP ifbp, DESC_STRCT *descp, unsigned int offset )
{
	int         rc = HCF_SUCCESS;
	wci_bufp    cp;                                     //                             
	hcf_16      i;
	int         tot_len = ifbp->IFB_RxLen - offset;     //            
	wci_bufp    lap = ifbp->IFB_lap + offset;           //                                 
	hcf_16      lal = ifbp->IFB_lal - offset;           //                                      
	hcf_16      j;

	HCFLOGENTRY( HCF_TRACE_RCV_MSG, offset );
	HCFASSERT( ifbp->IFB_Magic == HCF_MAGIC, ifbp->IFB_Magic );
	HCFASSERT_INT;
	HCFASSERT( descp, HCF_TRACE_RCV_MSG );
	HCFASSERT( ifbp->IFB_RxLen, HCF_TRACE_RCV_MSG );
	HCFASSERT( ifbp->IFB_RxLen >= offset, MERGE_2( offset, ifbp->IFB_RxLen ) );
	HCFASSERT( ifbp->IFB_lal >= offset, offset );
	HCFASSERT( (ifbp->IFB_CntlOpt & USE_DMA) == 0, 0xDADA );

	if ( tot_len < 0 ) {
		lal = 0; tot_len = 0;               //                                                   
	}
	do {                                    //                                 
		//                                                                                  
		HCFASSERT( ((hcf_32)descp & 3 ) == 0, (hcf_32)descp );
		cp = descp->buf_addr;
		j = min( (hcf_16)tot_len, descp->BUF_SIZE );    //                                                
		descp->BUF_CNT = j;
		tot_len -= j;                       //                         
		if ( lal ) {                        //                                             
			i = min( lal, j );              //                                                            
			lal -= i;                       //                                          
			j -= i;                         //                                                 
			/*                                                                                         
                            */
			while ( i-- ) *cp++ = *lap++;
		}
		if ( j ) {  //                                                                                     
			get_frag( ifbp, cp, j BE_PAR(0) );
			CALC_RX_MIC( cp, j );
		}
	} while ( ( descp = descp->next_desc_addr ) != NULL );
#if (HCF_TYPE) & HCF_TYPE_WPA
	if ( ifbp->IFB_RxFID ) {
		rc = check_mic( ifbp );             //                                                                 
	}
#endif //             
	(void)hcf_action( ifbp, HCF_ACT_RX_ACK );       //                                                             
	HCFASSERT( rc == HCF_SUCCESS, rc );
	HCFLOGEXIT( HCF_TRACE_RCV_MSG );
	return rc;
} //            


/*                                                                                                           
  
                                                                                 
                                                                  
                                                                                                      
                                                                    
  
            
                                               
                                                      
                                                        
                                            
                                     
                                      
                                      
                                         
                                        
                                                                  
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
  
          
                
                       
                     
  
               
                                                  
                                                                                                                 
                                        
                                                                                   
  
                                                         
                                                                                                             
                                                                                                       
                                                                                                       
                                                                 
  
                                                                                                        
                                                                                                             
                                                                                                                
                                                                            
                                                                                  
                                                                          
                                                                                                  
                                                                                                         
  
                                                                                                  
                                                                
                                                                                                       
                                                                                                                 
                                                                                                               
                                                          
  
                                                                             
                                                                                         
                                                                                                                
                                                                                                              
                                                    
                                                                                                             
                                                                                                           
                                                                                                             
                                                          
                                                                                                              
                                                                                                      
                                                          
                                                  
                                
                                                         
                
  
                                                                                                            
                              
                                                                                                         
                                                                       
  
                                                                                                                
                                                      
                                                                                                          
                                                                                                              
                                       
                                                                                                
                                                                                                                 
                                                                                                              
                                                   
                                                                       
                                                                                                 
                        
                    
              
  
                                                                                                                 
                                     
  
                                                                                                                
                                           
                                                                                                                 
                                                                                                     
  
                  
                                               
                            
                                    
                            
                                                                                    
                                               
  
          
                                                                                                
                                                                                                            
                                                                                     
                                                                                                                
                                                                                                               
                                   
                                                                                                         
                                                 
                                                                                                     
                                                          
                                                                                                          
                                                                                             
  
                                                                                                   
                                                                                                              
                                                                                                              
                                                                                                              
                                                                                                     
  
         
                                                                                                    
                                                                                                         
                                                                             
  
                                                                                                       
                                                                                                             
                                                                                                   
                                                                                         
                                                                                          
  
                                                                                                      
                                                                                                           
                                                                              
  
                                                                                                          
                                                                                                                
                                                                                                                
                                                                
  
                                          
  
                                                                                                            */
int
hcf_send_msg( IFBP ifbp, DESC_STRCT *descp, hcf_16 tx_cntl )
{
	int         rc = HCF_SUCCESS;
	DESC_STRCT  *p /*        */;        //               
	hcf_16      len;                    //                 
	hcf_16      i;

	hcf_16      fid = 0;

	HCFASSERT( ifbp->IFB_RscInd || descp == NULL, ifbp->IFB_RscInd );
	HCFASSERT( (ifbp->IFB_CntlOpt & USE_DMA) == 0, 0xDADB );

	HCFLOGENTRY( HCF_TRACE_SEND_MSG, tx_cntl );
	HCFASSERT( ifbp->IFB_Magic == HCF_MAGIC, ifbp->IFB_Magic );
	HCFASSERT_INT;
	/*                                                                                          
            */
	HCFASSERT( ((hcf_32)descp & 3 ) == 0, (hcf_32)descp );
#if HCF_ASSERT
	{   int x = ifbp->IFB_FWIdentity.comp_id == COMP_ID_FW_AP ? tx_cntl & ~HFS_TX_CNTL_PORT : tx_cntl;
		HCFASSERT( (x & ~HCF_TX_CNTL_MASK ) == 0, tx_cntl );
	}
#endif //           

	if ( descp ) ifbp->IFB_TxFID = 0;               //                        

	/*                                                                     
                                                                      
                    
  */
#if (HCF_TYPE) & HCF_TYPE_WPA
	tx_cntl |= ifbp->IFB_MICTxCntl;
#endif //             
	fid = ifbp->IFB_TxFID;
	if (fid == 0 && ( fid = get_fid( ifbp ) ) != 0 )        /*   */
		/*                                     
                         
                                                                                
  */
	{       //                                                                         
		                                //                                                                 
		len = 0;
		p = descp;
		do len += p->BUF_CNT; while ( ( p = p->next_desc_addr ) != NULL );
		p = descp;
//                                         
	/* */   (void)setup_bap( ifbp, fid, HFS_TX_CNTL, IO_OUT );
#if (HCF_TYPE) & HCF_TYPE_TX_DELAY
		HCFASSERT( ( descp != NULL ) ^ ( tx_cntl & HFS_TX_CNTL_TX_DELAY ), tx_cntl );
		if ( tx_cntl & HFS_TX_CNTL_TX_DELAY ) {
			tx_cntl &= ~HFS_TX_CNTL_TX_DELAY;       //                                          
			ifbp->IFB_TxFID = fid;
			fid = 0;                                //                                                         
		}
#endif //                  
		OPW( HREG_DATA_1, tx_cntl ) ;
		OPW( HREG_DATA_1, 0 );

		HCFASSERT( p->BUF_CNT >= 14, p->BUF_CNT );
		                                /*                                                                  
                                                                                                          
                                                                                   */

		CALC_TX_MIC( NULL, -1 );        //              
	/*  */  put_frag( ifbp, p->buf_addr, HCF_DASA_SIZE BE_PAR(0) ); //                                 
		CALC_TX_MIC( p->buf_addr, HCF_DASA_SIZE );      //               
		CALC_TX_MIC( null_addr, 4 );        //                                 

			                        //                       
#if (HCF_ENCAP) == HCF_ENC
			                        //                                                                          
		if ( ( snap_header[sizeof(snap_header)-1] = hcf_encap( &p->buf_addr[HCF_DASA_SIZE] ) ) != ENC_NONE ) {
			OPW( HREG_DATA_1, CNV_END_SHORT( len + (sizeof(snap_header) + 2) - ( 2*6 + 2 ) ) );
				                //                                 
			put_frag( ifbp, snap_header, sizeof(snap_header) BE_PAR(0) );
			CALC_TX_MIC( snap_header, sizeof(snap_header) );    //                    
			i = HCF_DASA_SIZE;
		} else
#endif //        
		{
			OPW( HREG_DATA_1, *(wci_recordp)&p->buf_addr[HCF_DASA_SIZE] );
			i = 14;
		}
			                        //                                                             
		put_frag( ifbp, &p->buf_addr[i], p->BUF_CNT - i BE_PAR(0) );
		CALC_TX_MIC( &p->buf_addr[i], p->BUF_CNT - i );

		                                //                                               
		while ( ( p = p->next_desc_addr ) != NULL ) {
			/*                                                                                          
              */
			HCFASSERT( ((hcf_32)p & 3 ) == 0, (hcf_32)p );
			put_frag( ifbp, p->buf_addr, p->BUF_CNT BE_PAR(0) );
			CALC_TX_MIC( p->buf_addr, p->BUF_CNT );
		}
		                                //                                                          
		put_frag_finalize( ifbp );
	}
	if ( fid ) {
	/*  */  rc = cmd_exe( ifbp, HCMD_BUSY | HCMD_TX | HCMD_RECL, fid );
		ifbp->IFB_TxFID = 0;
		/*                                                                                               
                                                          
                                                                             
   */
//                                          
	/*  */  if ( ifbp->IFB_RscInd == 0 ) {
			ifbp->IFB_RscInd = get_fid( ifbp );
		}
//                   
	}
//                                                        
	HCFLOGEXIT( HCF_TRACE_SEND_MSG );
	return rc;
} //             


/*                                                                                                           
  
                                                                                  
                                            
                                          
                                             
  
            
                                               
                    
                                                                                                                  
                                                            
                                                                           
  
          
                
                                                                                          
  
              
  
                                        
                                         
                                                             
               
                                                                   
                                                                             
                   
                 
                  
                                                                                                                   
  
                                                                                                      
 
                                                                 
 
                                                                                                        
                                                                                                              
                
                                                                                                       
                                                                                                 
 
                                                                                                         
                                                                                        
                                                                               
                                                                                                  
 
                                                                                                              
                                
                                                                                  
                                                                  
                                                                                                              
                                                                                                             
                                                                                                             
                                                                        
 
                                                                                              
                                                                                          
 
                                                                           
                                                                                                             
                       
                                                                                                             
                                                                                                          
                                                                                                         
                     
                                                                                           
                                                                                                            
 
                          
                                                                
                                             
                                                                               
                      
                   
 
                                                                                                            
                                                                                                   
                                                                                                          
                                                                              
                                                                                                          
                                                                                                     
 
                                                       
                                                      
                                                                                               
                                                                                           
                                                                                           
                    
                                                                                                   
                                                         
                                                                              
                                                                                      
                
                 
                 
                                                                 
                                                                                                             
                                                                                                            
                                                                                     
                                                
                                                                                                        
                                                               
                                                                                           
 
                                                                                                              
                                                                                                              
                                                                                                          
                                                                                                             
                                                                                                           
                                                                                                              
                                            
 
                           
                                                                                                          
                                                                                                        
                                                                                                         
         
                                                         
                               
                                              
                                      
                                                                                                            
                                                                                                          
                                                                                                     
                                                                                                          
                                                                               
                 
                                                          
                                                                  
                           
                                                                                   
                                              
 
 
         
                                                                                                        
                
  
                                                                                                              
                            
                                                                      
                                                                                        
                                                                        
                                                                                                           
          
                                                                                             
                                                                                                          
                                                   
                                                                                                         
                                                                                                            
                                                                                                             
                                                                                                       
                                                                                                             
                                                                                                     
                                        
                                                      
                                                                                   
                                                                                                          
                                   
                                                                     
 
 
                                                                                                       
                               
                                         
                                                                                                        
                                                                                                       
                                                                                                     
                                                                                                         
                                  
                                                            
                                                                                                  
                                                                                                 
                                                       
                                                                            
                                                                                 
                                                                               
                                                                                                       
                                                                                                   
                                                                                                          
                                                                                               
                         
                                                                                                               
                                                      
                                                                                                        
                                          
                                                                                                      
                                                                                               
                                                                                         
                                                                                                         
                                       
                                                                                                        
                                                                                                       
                                                                          
                                                                                                          
                                                                                                        
                                           
                                                                                                        
                                                                                  
                                                                                                    
                                                                                                           
                                        
                                                                                                     
                                                   
                                                                                                     
                                           
                                                                                                        
                       
                                                                                                     
                                                                                                       
                     
                                                                                                           
                                        
                                                                                                      
                                                                                   
                                                                                                          
                                                                                                         
                                                                                                  
                                                 
                                                


                                                                                                        
                                                                               
                                                     
                                                                          
                                                                                           
 
        
                                                                                 
                                                                                  
                                                                  
                     
                                                            
                      
                                   
 
        
                                                 
                                                                                            
                       
                  
                
                  
                       
                                                      
                                                                                                          
                       
                                         
 
                                                                                                           */
int
hcf_service_nic( IFBP ifbp, wci_bufp bufp, unsigned int len )
{

	int         rc = HCF_SUCCESS;
	hcf_16      stat;
	wci_bufp    buf_addr;
	hcf_16      i;

	HCFLOGENTRY( HCF_TRACE_SERVICE_NIC, ifbp->IFB_IntOffCnt );
	HCFASSERT( ifbp->IFB_Magic == HCF_MAGIC, ifbp->IFB_Magic );
	HCFASSERT_INT;

	ifbp->IFB_LinkStat = 0; //                                                                            
	ifbp->IFB_DSLinkStat &= ~CFG_LINK_STAT_CHANGE;                                                  /*  */
	(void)hcf_action( ifbp, HCF_ACT_RX_ACK );                                                       /*  */
	if ( ifbp->IFB_CardStat == 0 && ( stat = IPW( HREG_EV_STAT ) ) != 0xFFFF ) {                    /*  */
/*                                                                
                                                                      
                                                                                             
 */
		                                                                                        /*  */
		if ( ifbp->IFB_RscInd == 0 && stat & HREG_EV_ALLOC ) { //                                    
			ifbp->IFB_RscInd = 1;
		}
		IF_TALLY( if ( stat & HREG_EV_INFO_DROP ) { ifbp->IFB_HCF_Tallies.NoBufInfo++; } );
#if (HCF_EXT) & HCF_EXT_INT_TICK
		if ( stat & HREG_EV_TICK ) {
			ifbp->IFB_TickCnt++;
		}
#if 0 //                      
		if ( ifbp->IFB_TickCnt == 3 && ( ifbp->IFB_DSLinkStat & CFG_LINK_STAT_CONNECTED ) == 0 ) {
			CFG_DDS_TICK_TIME_STRCT ltv;
			//                                                                                 
			hcf_action( ifbp, HCF_ACT_SLEEP );
			ifbp->IFB_DSLinkStat |= CFG_LINK_STAT_DS_OOR; //              
			ltv.len = 2;
			ltv.typ = CFG_DDS_TICK_TIME;
			ltv.tick_time = ( ( ifbp->IFB_DSLinkStat & CFG_LINK_STAT_TIMER ) + 0x10 ) *64; //                
			hcf_put_info( ifbp, (LTVP)&ltv );
			printk( "<5>Preparing for sleep, link_status: %04X, timer : %d\n",
				ifbp->IFB_DSLinkStat, ltv.tick_time );//                 
			ifbp->IFB_TickCnt++; //                                                            
			if ( ltv.tick_time < 300 * 125 ) ifbp->IFB_DSLinkStat += 0x0010;

		}
#endif //        
#endif //                 
		if ( stat & HREG_EV_INFO ) {
			isr_info( ifbp );
		}
#if (HCF_EXT) & HCF_EXT_INT_TX_EX
		if ( stat & HREG_EV_TX_EXT && ( i = IPW( HREG_TX_COMPL_FID ) ) != 0 /*    */ ) {
			DAWA_ZERO_FID( HREG_TX_COMPL_FID );
			(void)setup_bap( ifbp, i, 0, IO_IN );
			get_frag( ifbp, &ifbp->IFB_TxFsStat, HFS_SWSUP BE_PAR(1) );
		}
#endif //                  
//                                                         
#if HCF_DMA
		if ( !( ifbp->IFB_CntlOpt & USE_DMA ) ) //                                       
#endif //        
		/*  */  if ( stat & HREG_EV_RX && ( ifbp->IFB_RxFID = IPW( HREG_RX_FID ) ) != 0 ) { //                  
				HCFASSERT( bufp, len );
				HCFASSERT( len >= HFS_DAT + 2, len );
				DAWA_ZERO_FID( HREG_RX_FID );
				HCFASSERT( ifbp->IFB_RxFID < CFG_PROD_DATA, ifbp->IFB_RxFID);
				(void)setup_bap( ifbp, ifbp->IFB_RxFID, 0, IO_IN );
				get_frag( ifbp, bufp, HFS_ADDR_DEST BE_PAR(1) );
				ifbp->IFB_lap = buf_addr = bufp + HFS_ADDR_DEST;
				ifbp->IFB_RxLen = (hcf_16)(bufp[HFS_DAT_LEN] + (bufp[HFS_DAT_LEN+1]<<8) + 2*6 + 2);
			/*  */  if ( ifbp->IFB_RxLen >= 22 ) {  //                                                       
								//                                                     
				/*  */  get_frag( ifbp, buf_addr, 22 BE_PAR(0) );
				/*  */  CALC_RX_MIC( bufp, -1 );        //                 
					CALC_RX_MIC( buf_addr, HCF_DASA_SIZE ); //                  
					CALC_RX_MIC( null_addr, 4 );    //                                    
					CALC_RX_MIC( buf_addr+14, 8 );  //                                                
					buf_addr += 22;
#if (HCF_ENCAP) == HCF_ENC
					HCFASSERT( len >= HFS_DAT + 2 + sizeof(snap_header), len );
				/*  */  i = *(wci_recordp)&bufp[HFS_STAT] & ( HFS_STAT_MSG_TYPE | HFS_STAT_ERR );
					if ( i == HFS_STAT_TUNNEL ||
					     ( i == HFS_STAT_1042 && hcf_encap( (wci_bufp)&bufp[HFS_TYPE] ) != ENC_TUNNEL ) ) {
							                //                                    
				/*  */  bufp[HFS_LEN  ] = bufp[HFS_TYPE  ];
						bufp[HFS_LEN+1] = bufp[HFS_TYPE+1];
							                //                                        
						ifbp->IFB_RxLen -= (HFS_TYPE - HFS_LEN);
						buf_addr -= ( HFS_TYPE - HFS_LEN ); //                                                
					}
#endif //        
				}
			/*  */  ifbp->IFB_lal = min( (hcf_16)(len - HFS_ADDR_DEST), ifbp->IFB_RxLen );
				i = ifbp->IFB_lal - ( buf_addr - ( bufp + HFS_ADDR_DEST ) );
				get_frag( ifbp, buf_addr, i BE_PAR(0) );
				CALC_RX_MIC( buf_addr, i );
#if (HCF_TYPE) & HCF_TYPE_WPA
				if ( ifbp->IFB_lal == ifbp->IFB_RxLen ) {
					rc = check_mic( ifbp );
				}
#endif //             
			/*  */  if ( len - HFS_ADDR_DEST >= ifbp->IFB_RxLen ) {
					ifbp->IFB_RxFID = 0;
				} else { /*                                                                                          */
					stat &= (hcf_16)~HREG_EV_RX;    //                                            
				}
			}
		//                                                                   
		IF_USE_DMA( ifbp->IFB_DmaPackets |= stat & ( HREG_EV_RDMAD | HREG_EV_TDMAD ) );
		//                                                                            
	/*  */  stat &= (hcf_16)~( HREG_EV_SLEEP_REQ | HREG_EV_CMD | HREG_EV_ACK_REG_READY | HREG_EV_ALLOC | HREG_EV_FW_DMA );
//                                                                                                                                                                    
		IF_USE_DMA( stat &= (hcf_16)~HREG_EV_RX );
		if ( stat ) {
			DAWA_ACK( stat );   /*    */
		}
	}
	HCFLOGEXIT( HCF_TRACE_SERVICE_NIC );
	return rc;
} //                


/*                                                                                                           
                                                                                                              
                                                                                                            */


/*                                                                                                           
  
                                                     
                                              
  
            
                                   
                                                                           
  
                     
  
              
                                                                                            
                        
                              
  
  
          
  
         
                                          
  
                                                                                                            */

#if (HCF_TYPE) & HCF_TYPE_WPA

#define ROL32( A, n ) ( ((A) << (n)) | ( ((A)>>(32-(n)))  & ( (1UL << (n)) - 1 ) ) )
#define ROR32( A, n ) ROL32( (A), 32-(n) )

#define L   *p
#define R   *(p+1)

void
calc_mic( hcf_32* p, hcf_32 m )
{
#if HCF_BIG_ENDIAN
	m = (m >> 16) | (m << 16);
#endif //               
	L ^= m;
	R ^= ROL32( L, 17 );
	L += R;
	R ^= ((L & 0xff00ff00) >> 8) | ((L & 0x00ff00ff) << 8);
	L += R;
	R ^= ROL32( L, 3 );
	L += R;
	R ^= ROR32( L, 2 );
	L += R;
} //         
#undef R
#undef L
#endif //             



#if (HCF_TYPE) & HCF_TYPE_WPA
/*                                                                                                           
  
                                                                        
                                                    
  
            
                                               
                                         
                                                            
  
                     
  
              
                            
  
                                 
                                                                                                     
               
  
  
          
  
         
                                          
  
                                                                                                            */
void
calc_mic_rx_frag( IFBP ifbp, wci_bufp p, int len )
{
	static union { hcf_32 x32; hcf_16 x16[2]; hcf_8 x8[4]; } x; //                                                 
	int i;

	if ( len == -1 ) {                              //                           
		i = *(wci_recordp)&p[HFS_STAT];
		/*                                                                                           
                                                                                     
                                 
   */

		if ( ( i & HFS_STAT_MIC ) == 0 ) {
			ifbp->IFB_MICRxCarry = 0xFFFF;          //                        
		} else {
			ifbp->IFB_MICRxCarry = 0;
//                                                               
//                                                     
			i = ( i & HFS_STAT_MIC_KEY_ID ) >> 10;  /*                                             */
			ifbp->IFB_MICRx[0] = CNV_LONG_TO_LITTLE(ifbp->IFB_MICRxKey[i  ]);
			ifbp->IFB_MICRx[1] = CNV_LONG_TO_LITTLE(ifbp->IFB_MICRxKey[i+1]);
		}
	} else {
		if ( ifbp->IFB_MICRxCarry == 0 ) {
			x.x32 = CNV_LONGP_TO_LITTLE(p);
			p += 4;
			if ( len < 4 ) {
				ifbp->IFB_MICRxCarry = (hcf_16)len;
			} else {
				ifbp->IFB_MICRxCarry = 4;
				len -= 4;
			}
		} else while ( ifbp->IFB_MICRxCarry < 4 && len ) {      //                                   
				x.x8[ifbp->IFB_MICRxCarry++] = *p++;
				len--;
			}
		while ( ifbp->IFB_MICRxCarry == 4 ) {   //                                                                         
			calc_mic( ifbp->IFB_MICRx, x.x32 );
			x.x32 = CNV_LONGP_TO_LITTLE(p);
			p += 4;
			if ( len < 4 ) {
				ifbp->IFB_MICRxCarry = (hcf_16)len;
			}
			len -= 4;
		}
	}
} //                 
#endif //             


#if (HCF_TYPE) & HCF_TYPE_WPA
/*                                                                                                           
  
                                                                        
                                                    
  
            
                                               
                                         
                                                            
  
                     
  
              
                            
  
                                 
                                                                                                     
               
  
  
          
  
         
                                          
  
                                                                                                            */
void
calc_mic_tx_frag( IFBP ifbp, wci_bufp p, int len )
{
	static union { hcf_32 x32; hcf_16 x16[2]; hcf_8 x8[4]; } x; //                                                 

	                                        //                         
	if ( len == -1 ) {
		                                //                                   
		ifbp->IFB_MICTxCarry = 0xFFFF;
		                                //                             
		if ( ifbp->IFB_MICTxCntl ) {
			                        //                     
			ifbp->IFB_MICTxCarry = 0;
			                        //                           
			ifbp->IFB_MICTx[0] = CNV_LONG_TO_LITTLE(ifbp->IFB_MICTxKey[0]); /*                     */
			ifbp->IFB_MICTx[1] = CNV_LONG_TO_LITTLE(ifbp->IFB_MICTxKey[1]);
		}
		                                //    
	} else {
		                                //                                            
		                                //                                           
		if ( ifbp->IFB_MICTxCarry == 0 ) {
			                        //                                          
			x.x32 = CNV_LONGP_TO_LITTLE(p);
			                        //                                
			p += 4;
			                        //                                           
			if ( len < 4 ) {
				                //                                             
				                //                                                    
				ifbp->IFB_MICTxCarry = (hcf_16)len;
				                //          
			} else {
				                //                                                  
				ifbp->IFB_MICTxCarry = 4;
				                //                                    
				len -= 4;
			}
			                        //                      
			                        //                                                    
			                        //                                           
		} else while ( ifbp->IFB_MICTxCarry < 4 && len ) {      /*                                     */
				x.x8[ifbp->IFB_MICTxCarry++] = *p++;
				len--;
			}
		                                //                                           
		                                //                  
		while ( ifbp->IFB_MICTxCarry == 4 ) {
			                        //                             
			calc_mic( ifbp->IFB_MICTx, x.x32 );
			                        //                                           
			x.x32 = CNV_LONGP_TO_LITTLE(p);
			                        //                           
			p += 4;
			                        //                                           
			                        //                                             
			                        //                                                    
			if ( len < 4 ) {
				ifbp->IFB_MICTxCarry = (hcf_16)len;
			}
			                        //                                    
			len -= 4;
		}
	}
} //                 
#endif //             


#if HCF_PROT_TIME
/*                                                                                                           
  
                                            
                                                                                     
  
            
                                               
  
                     
  
              
                                                                      
                                                                                                         
                                                                                                         
                                                                                                      
                      
  
  
          
  
                                                                                                     
                                                                                                           
                                                      
                                                                                                       
                                                                                                            
                                                                                                               
                                                                                                        
                                                                                                        
             
                                      
                                                                                                 
                                                                                                               
                                                                                                            
                                                                                                     
                  
                                                                                                            
                                                                                             
                                                                                                               
                                                                                                               
  
         
                                                                                                         
                                                         
                                                                                                         
                                                                                                              
                             
                                                                                                             
                                                                                                              
                     
                                           
                                                                                                       
                                                                                                        
                                                                                               
                                                                        
                                                                                              
                                                                                                         
                 
                                                                                                              
                                          
                                                                                                               
                                                                                                           
                               
  
                                          
  
                                                                                                            */
HCF_STATIC void
calibrate( IFBP ifbp )
{
	int     cnt = HCF_PROT_TIME_CNT;
	hcf_32  prot_cnt;

	HCFTRACE( ifbp, HCF_TRACE_CALIBRATE );
	if ( ifbp->IFB_TickIni == INI_TICK_INI ) {                                                  /* */
		ifbp->IFB_TickIni = 0;                                                                  /* */
		while ( cnt-- ) {
			prot_cnt = INI_TICK_INI;
			OPW( HREG_EV_ACK, HREG_EV_TICK );                                               /* */
			while ( (IPW( HREG_EV_STAT ) & HREG_EV_TICK) == 0 && --prot_cnt ) {
				ifbp->IFB_TickIni++;
			}
			if ( prot_cnt == 0 || prot_cnt == INI_TICK_INI ) {                              /* */
				ifbp->IFB_TickIni = INI_TICK_INI;
				ifbp->IFB_DefunctStat = HCF_ERR_DEFUNCT_TIMER;
				ifbp->IFB_CardStat |= CARD_STAT_DEFUNCT;
				HCFASSERT( DO_ASSERT, prot_cnt );
			}
		}
		ifbp->IFB_TickIni <<= HCF_PROT_TIME_SHFT;                                               /* */
	}
	HCFTRACE( ifbp, HCF_TRACE_CALIBRATE | HCF_TRACE_EXIT );
} //          
#endif //              


#if (HCF_TYPE) & HCF_TYPE_WPA
/*                                                                                                           
  
                                           
                                                              
  
            
                                               
  
          
                
                
  
              
  
  
          
  
                                                        
                                                                                                             
  
         
                                          
  
                                                                                                            */
int
check_mic( IFBP ifbp )
{
	int     rc = HCF_SUCCESS;
	hcf_32 x32[2];              //                               

	                                                    //                      
	if ( *(wci_recordp)&ifbp->IFB_lap[-HFS_ADDR_DEST] & HFS_STAT_MIC ) {
		//                                        
		CALC_RX_MIC( mic_pad, 8 );                  //                                                                                                
		get_frag( ifbp, (wci_bufp)x32, 8 BE_PAR(0));//                          
		                                            //                                              
		                                            //                               
	/*  */  if ( x32[0] != CNV_LITTLE_TO_LONG(ifbp->IFB_MICRx[0]) ||
		     x32[1] != CNV_LITTLE_TO_LONG(ifbp->IFB_MICRx[1])     ) {
			rc = HCF_ERR_MIC;
		}
	}
	                                                    //             
	return rc;
} //          
#endif //             


/*                                                                                                           
  
                                          
                                                     
  
            
                                               
  
          
                    
                     
                            
                
  
              
  
  
          
  
                                                                            
                                                            
                                                            
                                                        
                                                                       
                                                                                                      
                                                                                                        
                                                                                                              
                                                                                                      
                                                                                                              
                                                    
                                                                                                               
                                                                                                               
                     
                                                                                                            
                                                 
                               
                                          
                                                                
                            
                                                                      
  
  
                                          
  
                                                                                                            */
HCF_STATIC int
cmd_cmpl( IFBP ifbp )
{

	PROT_CNT_INI;
	int     rc = HCF_SUCCESS;
	hcf_16  stat;

	HCFLOGENTRY( HCF_TRACE_CMD_CPL, ifbp->IFB_Cmd );
	ifbp->IFB_Cmd &= ~HCMD_BUSY;                                                /*   */
	HCF_WAIT_WHILE( (IPW( HREG_EV_STAT) & HREG_EV_CMD) == 0 );                  /*   */
	stat = IPW( HREG_STAT );
#if HCF_PROT_TIME
	if ( prot_cnt == 0 ) {
		IF_TALLY( ifbp->IFB_HCF_Tallies.MiscErr++ );
		rc = HCF_ERR_TIME_OUT;
		HCFASSERT( DO_ASSERT, ifbp->IFB_Cmd );
	} else
#endif //              
	{
		DAWA_ACK( HREG_EV_CMD );
	/* */   if ( stat != (ifbp->IFB_Cmd & HCMD_CMD_CODE) ) {
		/* */   if ( ( (stat ^ ifbp->IFB_Cmd ) & HCMD_CMD_CODE) != 0 ) {
				rc = ifbp->IFB_DefunctStat = HCF_ERR_DEFUNCT_CMD_SEQ;
				ifbp->IFB_CardStat |= CARD_STAT_DEFUNCT;
			}
			IF_TALLY( ifbp->IFB_HCF_Tallies.MiscErr++ );
			ifbp->IFB_ErrCmd = stat;
			ifbp->IFB_ErrQualifier = IPW( HREG_RESP_0 );
			HCFASSERT( DO_ASSERT, MERGE_2( IPW( HREG_PARAM_0 ), ifbp->IFB_Cmd ) );
			HCFASSERT( DO_ASSERT, MERGE_2( ifbp->IFB_ErrQualifier, ifbp->IFB_ErrCmd ) );
		}
	}
	HCFASSERT( rc == HCF_SUCCESS, rc);
	HCFLOGEXIT( HCF_TRACE_CMD_CPL );
	return rc;
} //         


/*                                                                                                           
  
                                                                  
                                                                                                             
  
            
                                               
             
          
  
          
                    
                            
                
                                                            
  
              
                                                                       
  
                                                                                
                                                                                
                                                                                                       
                                                       
                                                                                                          
                                                                                                   
                                                                                                         
          
  
  
  
          
  
                                                                                                        
                                                 
                                                                                                               
                                                                                                         
                                                                                                            
                                                                                                               
                                                                                                            
                                                                                                               
                                                                                                           
                                              
                                                                                                      
  
         
                                          
  
                                                                                                            */

HCF_STATIC int
cmd_exe( IFBP ifbp, hcf_16 cmd_code, hcf_16 par_0 ) //                                                             
{
	int rc;

	HCFLOGENTRY( HCF_TRACE_CMD_EXE, cmd_code );
	HCFASSERT( (cmd_code & HCMD_CMD_CODE) != HCMD_TX || cmd_code & HCMD_BUSY, cmd_code ); //                         
	OPW( HREG_SW_0, HCF_MAGIC );
	if ( IPW( HREG_SW_0 ) == HCF_MAGIC ) {                                                      /*   */
		rc = ifbp->IFB_DefunctStat;
	}
	else rc = HCF_ERR_NO_NIC;
	if ( rc == HCF_SUCCESS ) {
		//                                                       
	/* */   if ( ifbp->IFB_Cmd & HCMD_BUSY ) {
			rc = cmd_cmpl( ifbp );
		}
		OPW( HREG_PARAM_0, par_0 );
		OPW( HREG_CMD, cmd_code &~HCMD_BUSY );
		ifbp->IFB_Cmd = cmd_code;
		if ( (cmd_code & HCMD_BUSY) == 0 ) {    //                                                       
			rc = cmd_cmpl( ifbp );
		}
	}
	HCFASSERT( rc == HCF_SUCCESS, MERGE_2( rc, cmd_code ) );
	HCFLOGEXIT( HCF_TRACE_CMD_EXE );
	return rc;
} //        


/*                                                                                                           
  
                                                                    
                                                                                            
  
            
                                               
                                                             
  
          
  
              
  
  
          
                                                                        
                                                                                                               
            
                                                                                                         
                      
  
  
                                          
  
                                                                                                            */
HCF_STATIC int
download( IFBP ifbp, CFG_PROG_STRCT FAR *ltvp )                     //                                  
{
	hcf_16              i;
	int                 rc = HCF_SUCCESS;
	wci_bufp            cp;
	hcf_io              io_port = ifbp->IFB_IOBase + HREG_AUX_DATA;

	HCFLOGENTRY( HCF_TRACE_DL, ltvp->typ );
#if (HCF_TYPE) & HCF_TYPE_PRELOADED
	HCFASSERT( DO_ASSERT, ltvp->mode );
#else
	                                        //                        
	if ( ifbp->IFB_DLMode == CFG_PROG_STOP && ltvp->mode == CFG_PROG_VOLATILE) {
		                                //                                
	/* */   OPW( HREG_EV_ACK, ~HREG_EV_SLEEP_REQ );
		rc = cmd_exe( ifbp, HCMD_INI, 0 );  /*                                                             
                                     */
		rc = init( ifbp );
	}
	                                        //                      
	if ( ltvp->mode == CFG_PROG_STOP && ifbp->IFB_DLMode == CFG_PROG_VOLATILE) {
		                                //                                
		OPW( HREG_PARAM_1, (hcf_16)(ltvp->nic_addr >> 16) );
		rc = cmd_exe( ifbp, HCMD_EXECUTE, (hcf_16) ltvp->nic_addr );
		if (rc == HCF_SUCCESS) {
			rc = init( ifbp );  /*                                                         
                                                        */
		}
		                                //                
	} else {
		                                //                              
#if 0   //                                                                                               
		if ( 0 /*                                    */ && ltvp->mode == CFG_PROG_SEEPROM_READBACK ) {
			OPW( HREG_PARAM_1, (hcf_16)(ltvp->nic_addr >> 16) );
			OPW( HREG_PARAM_2, (hcf_16)((ltvp->len - 4) << 1) );
			                        //                                                        
			rc = cmd_exe( ifbp, HCMD_PROGRAM | ltvp->mode, (hcf_16)ltvp->nic_addr );
			                        //                                                     
			OPW( HREG_AUX_PAGE,   IPW( HREG_RESP_1) );
			OPW( HREG_AUX_OFFSET, IPW( HREG_RESP_0) );
			                        //                                           
			i = ( IPW( HREG_RESP_2 ) + 1 ) / 2;  //                                                                                        
/*                                                             
                                             
                          
*/
/*                                                                                    
                                                                
                             
                     
                      
    
*/
			                        //                                            
			cp = (wci_bufp)ltvp->host_addr;                     /*                                                   */
			i = ltvp->len - 4;
			IN_PORT_STRING_8_16( io_port, cp, i );      //                                                    
			                        //                               
		} else
#endif //                                                                                                
		{                               //                                    
			HCFASSERT( ltvp->segment_size, *ltvp->host_addr );
			i = ltvp->segment_size/2;
			                        //                                                    
			cp = (wci_bufp)ltvp->host_addr;                     //                                                    
			                        //                                     
			if ( ltvp->mode == CFG_PROG_VOLATILE ) {
				                //                                                   
				OPW( HREG_AUX_PAGE, (hcf_16)(ltvp->nic_addr >> 16 << 9 | (ltvp->nic_addr & 0xFFFF) >> 7 ) );
				OPW( HREG_AUX_OFFSET, (hcf_16)(ltvp->nic_addr & 0x007E) );
				OUT_PORT_STRING_8_16( io_port, cp, i );     //                                         
			}
		}
	}
	ifbp->IFB_DLMode = ltvp->mode;                  //                        
#endif //                   
	HCFASSERT( rc == HCF_SUCCESS, rc );
	HCFLOGEXIT( HCF_TRACE_DL );
	return rc;
} //         


#if (HCF_ASSERT) & HCF_ASSERT_PRINTF
/*                                                 
                                                   
                                               
                                                    
                    
                                                  */
HCF_STATIC int
fw_printf(IFBP ifbp, CFG_FW_PRINTF_STRCT FAR *ltvp)
{
	int rc = HCF_SUCCESS;
	hcf_16 fw_cnt;
//                                                     
//                             
	hcf_32 DbMsgBuffer;
	CFG_FW_PRINTF_BUFFER_LOCATION_STRCT *p = &ifbp->IFB_FwPfBuff;
	ltvp->len = 1;
	if ( p->DbMsgSize != 0 ) {
		//                                                                                          
		OPW( HREG_AUX_PAGE, (hcf_16)(p->DbMsgCount >> 7) );
		OPW( HREG_AUX_OFFSET, (hcf_16)(p->DbMsgCount & 0x7E) );
		fw_cnt = ((IPW( HREG_AUX_DATA) >>1 ) & ((hcf_16)p->DbMsgSize - 1));
		if ( fw_cnt != ifbp->IFB_DbgPrintF_Cnt ) {
//                                                                                 
			DbMsgBuffer = p->DbMsgBuffer + ifbp->IFB_DbgPrintF_Cnt * 6; //                      
			OPW( HREG_AUX_PAGE, (hcf_16)(DbMsgBuffer >> 7) );
			OPW( HREG_AUX_OFFSET, (hcf_16)(DbMsgBuffer & 0x7E) );
			ltvp->msg_id     = IPW(HREG_AUX_DATA);
			ltvp->msg_par    = IPW(HREG_AUX_DATA);
			ltvp->msg_tstamp = IPW(HREG_AUX_DATA);
			ltvp->len = 4;
			ifbp->IFB_DbgPrintF_Cnt++;
			ifbp->IFB_DbgPrintF_Cnt &= (p->DbMsgSize - 1);
		}
	}
	return rc;
};
#endif //                  


/*                                                                                                           
  
                                            
                                                                 
  
            
                                               
  
          
                         
                   
  
              
  
  
          
                                                                                                              
             
                                                                                      
                                                                                      
                                                                                                            
                                                                                          
  
                                                                                                              
                                                                                                               
                                                                                                               
                                                                        
                                          
  
                                                                                                            */
HCF_STATIC hcf_16
get_fid( IFBP ifbp )
{

	hcf_16 fid = 0;
#if ( (HCF_TYPE) & HCF_TYPE_HII5 ) == 0
	PROT_CNT_INI;
#endif //              

	IF_DMA( HCFASSERT(!(ifbp->IFB_CntlOpt & USE_DMA), ifbp->IFB_CntlOpt) );

	if ( IPW( HREG_EV_STAT) & HREG_EV_ALLOC) {
		fid = IPW( HREG_ALLOC_FID );
		HCFASSERT( fid, ifbp->IFB_RscInd );
		DAWA_ZERO_FID( HREG_ALLOC_FID );
#if ( (HCF_TYPE) & HCF_TYPE_HII5 ) == 0
		HCF_WAIT_WHILE( ( IPW( HREG_EV_STAT ) & HREG_EV_ACK_REG_READY ) == 0 );
		HCFASSERT( prot_cnt, IPW( HREG_EV_STAT ) );
#endif //              
		DAWA_ACK( HREG_EV_ALLOC );          //                                              
//                                          
		if ( ifbp->IFB_RscInd == 1 ) {
			ifbp->IFB_RscInd = 0;
		}
//                  
	} else {
//                                          
		fid = ifbp->IFB_RscInd;
//                  
		ifbp->IFB_RscInd = 0;
	}
	return fid;
} //        


/*                                                                                                           
  
                                                                                          
                                                                                    
  
            
                                               
                                         
                                                            
                                                                          
  
                     
  
              
                                                                                                           
               
                                                                                                           
                                
  
  
          
                                                                                                         
                                                                                                      
                                                                                        
                                                                                                           
                                                                                                           
                                        
  
         
                                                                                                             
                              
  
                                          
  
                                                                                                            */
HCF_STATIC void
get_frag( IFBP ifbp, wci_bufp bufp, int len BE_PAR( int word_len ) )
{
	hcf_io      io_port = ifbp->IFB_IOBase + HREG_DATA_1;   //                 
	wci_bufp    p = bufp;                                   //               
	int         i;                                          //                               
	int         j;

	HCFASSERT( ((hcf_32)bufp & (HCF_ALIGN-1) ) == 0, (hcf_32)bufp );

/*                                                                                                           
                                  
                               
 */

	i = len;
	                                        //                                                     
	if ( i && ifbp->IFB_CarryIn ) {
		                                //                       
		                                //                                               
		*p++ = (hcf_8)(ifbp->IFB_CarryIn>>8);
		i--;
		                                //                   
		ifbp->IFB_CarryIn = 0;
	}
#if (HCF_IO) & HCF_IO_32BITS
	//                                                                                                 
	                                        //                                             
	if ( !(ifbp->IFB_CntlOpt & USE_16BIT) && i >= 6 ) {
		hcf_32 FAR  *p4; //                               
		if ( ( (hcf_32)p & 0x1 ) == 0 ) {           //                                  
			if ( (hcf_32)p & 0x2 ) {            //                                       
							    //                                                    
				*(wci_recordp)p = IN_PORT_WORD( io_port );
				                            //                                                     
				p += 2;
				i -= 2;
			}
			                                    //                                          
			p4 = (hcf_32 FAR *)p;
			j = i/4;
			IN_PORT_STRING_32( io_port, p4, j );
			                                    //                                                  
			p += i & ~0x0003;
			i &= 0x0003;
		}
	}
#endif //              
	                                        //                                                      
	if ( i ) {
		                                //                                                        
		j = i/2;
		IN_PORT_STRING_8_16( io_port, p, j );
		                                //                 
		if ( i & 0x0001 ) {
			                        //                 
			ifbp->IFB_CarryIn = IN_PORT_WORD( io_port );
			                        //                                      
			bufp[len-1] = (hcf_8)ifbp->IFB_CarryIn;
			                        //                                       
			ifbp->IFB_CarryIn |= 0x1;
		}
	}
#if HCF_BIG_ENDIAN
	HCFASSERT( word_len == 0 || word_len == 2 || word_len == 4, word_len );
	HCFASSERT( word_len == 0 || ((hcf_32)bufp & 1 ) == 0, (hcf_32)bufp );
	HCFASSERT( word_len <= len, MERGE2( word_len, len ) );
	//                                                                                                
	//        
	if ( word_len ) {                               //                                  
		hcf_8 c;
		c = bufp[1];                                //                            
		bufp[1] = bufp[0];
		bufp[0] = c;
		if ( word_len > 1 ) {                       //                                                       
			c = bufp[3];                            //                               
			bufp[3] = bufp[2];
			bufp[2] = c;
		}
	}
#endif //               
} //         

/*                                                                                                           
  
                                      
                                                                                                         
  
            
                                               
  
          
                       
                      
                     
                   
                       
                    
  
              
                          
                                                         
                                                                
                                                                   
                                                                                                
                                                         
                                                                                                
                                                                                               
  
  
          
                                                                                           
                                                                                                  
                                                                                                            
                         
                                                                                                       
                                                                                                      
                          
                                                                                                 
                         
                                                                                           
                    
                    
                    
                     
                    
                    
                                      
                                                                                             
               
                                                                                                           
                                                                                                            
                                                                          
                                                                                                
                                                                                                 
                                                                                         
                                                                                                         
                                                                          
                                                                                                  
                                                                                                         
                                                                                                           
                                                                                                            
                                                                                                              
                                                                                                            
                                                                                                     
                                                                                           
                                                                                                          
                                                                                                             
                                                                           
                                                                                                               
                                                                                                          
                                       
                                                                                                              
                                                                                                            
              
                                                                                                            
                                                                                         
                                                                                                      
                                                                                                      
                                                      
                                                            
                                                                 
                                                                         
                                    
                                                                      
                                                                                            
                                                                      
  
                                          
  
                                                                                                            */
HCF_STATIC int
init( IFBP ifbp )
{

	int rc = HCF_SUCCESS;

	HCFLOGENTRY( HCF_TRACE_INIT, 0 );

	ifbp->IFB_CardStat = 0;                                                                         /*  */
	OPW( HREG_EV_ACK, ~HREG_EV_SLEEP_REQ );                                             /*  */
	IF_PROT_TIME( calibrate( ifbp ) );                                                  /*  */
#if 0 //    
	ifbp->IFB_FWIdentity.len = 2;                           //                              
	ifbp->IFB_FWIdentity.typ = CFG_TICK_TIME;
	ifbp->IFB_FWIdentity.comp_id = (1000*1000)/1024 + 1;    //                
	hcf_put_info( ifbp, (LTVP)&ifbp->IFB_FWIdentity.len );
#endif //    
	ifbp->IFB_FWIdentity.len = sizeof(CFG_FW_IDENTITY_STRCT)/sizeof(hcf_16) - 1;
	ifbp->IFB_FWIdentity.typ = CFG_FW_IDENTITY;
	rc = hcf_get_info( ifbp, (LTVP)&ifbp->IFB_FWIdentity.len );
/*                                                       */
#if HCF_BIG_ENDIAN
	ifbp->IFB_FWIdentity.comp_id       = CNV_LITTLE_TO_SHORT( ifbp->IFB_FWIdentity.comp_id );
	ifbp->IFB_FWIdentity.variant       = CNV_LITTLE_TO_SHORT( ifbp->IFB_FWIdentity.variant );
	ifbp->IFB_FWIdentity.version_major = CNV_LITTLE_TO_SHORT( ifbp->IFB_FWIdentity.version_major );
	ifbp->IFB_FWIdentity.version_minor = CNV_LITTLE_TO_SHORT( ifbp->IFB_FWIdentity.version_minor );
#endif //               
#if defined MSF_COMPONENT_ID                                                                        /*  */
	if ( rc == HCF_SUCCESS ) {                                                                      /*  */
		ifbp->IFB_HSISup.len = sizeof(CFG_SUP_RANGE_STRCT)/sizeof(hcf_16) - 1;
		ifbp->IFB_HSISup.typ = CFG_NIC_HSI_SUP_RANGE;
		rc = hcf_get_info( ifbp, (LTVP)&ifbp->IFB_HSISup.len );
/*                                                                                                     
                                         
                                                                                         
                                */
#if HCF_BIG_ENDIAN
		ifbp->IFB_HSISup.role    = CNV_LITTLE_TO_SHORT( ifbp->IFB_HSISup.role );
		ifbp->IFB_HSISup.id      = CNV_LITTLE_TO_SHORT( ifbp->IFB_HSISup.id );
		ifbp->IFB_HSISup.variant = CNV_LITTLE_TO_SHORT( ifbp->IFB_HSISup.variant );
		ifbp->IFB_HSISup.bottom  = CNV_LITTLE_TO_SHORT( ifbp->IFB_HSISup.bottom );
		ifbp->IFB_HSISup.top     = CNV_LITTLE_TO_SHORT( ifbp->IFB_HSISup.top );
#endif //               
		ifbp->IFB_FWSup.len = sizeof(CFG_SUP_RANGE_STRCT)/sizeof(hcf_16) - 1;
		ifbp->IFB_FWSup.typ = CFG_FW_SUP_RANGE;
		(void)hcf_get_info( ifbp, (LTVP)&ifbp->IFB_FWSup.len );
/*                                                       */
#if HCF_BIG_ENDIAN
		ifbp->IFB_FWSup.role    = CNV_LITTLE_TO_SHORT( ifbp->IFB_FWSup.role );
		ifbp->IFB_FWSup.id      = CNV_LITTLE_TO_SHORT( ifbp->IFB_FWSup.id );
		ifbp->IFB_FWSup.variant = CNV_LITTLE_TO_SHORT( ifbp->IFB_FWSup.variant );
		ifbp->IFB_FWSup.bottom  = CNV_LITTLE_TO_SHORT( ifbp->IFB_FWSup.bottom );
		ifbp->IFB_FWSup.top     = CNV_LITTLE_TO_SHORT( ifbp->IFB_FWSup.top );
#endif //               

		if ( ifbp->IFB_FWSup.id == COMP_ID_PRI ) {                                              /*   */
			int i = sizeof( CFG_FW_IDENTITY_STRCT) + sizeof(CFG_SUP_RANGE_STRCT );
			while ( i-- ) ((hcf_8*)(&ifbp->IFB_PRIIdentity))[i] = ((hcf_8*)(&ifbp->IFB_FWIdentity))[i];
			ifbp->IFB_PRIIdentity.typ = CFG_PRI_IDENTITY;
			ifbp->IFB_PRISup.typ = CFG_PRI_SUP_RANGE;
			xxxx[xxxx_PRI_IDENTITY_OFFSET] = &ifbp->IFB_PRIIdentity.len;
			xxxx[xxxx_PRI_IDENTITY_OFFSET+1] = &ifbp->IFB_PRISup.len;
		}
		if ( !mmd_check_comp( (void*)&cfg_drv_act_ranges_hsi, &ifbp->IFB_HSISup)                 /*   */
#if ( (HCF_TYPE) & HCF_TYPE_PRELOADED ) == 0
//                                                       
		     || !mmd_check_comp( (void*)&cfg_drv_act_ranges_pri, &ifbp->IFB_PRISup)
#endif //                   
			) {
			ifbp->IFB_CardStat = CARD_STAT_INCOMP_PRI;
			rc = HCF_ERR_INCOMP_PRI;
		}
		if ( ( ifbp->IFB_FWSup.id == COMP_ID_STA && !mmd_check_comp( (void*)&cfg_drv_act_ranges_sta, &ifbp->IFB_FWSup) ) ||
		     ( ifbp->IFB_FWSup.id == COMP_ID_APF && !mmd_check_comp( (void*)&cfg_drv_act_ranges_apf, &ifbp->IFB_FWSup) )
			) {                                                                                  /*    */
			ifbp->IFB_CardStat |= CARD_STAT_INCOMP_FW;
			rc = HCF_ERR_INCOMP_FW;
		}
	}
#endif //                 

	if ( rc == HCF_SUCCESS && ifbp->IFB_FWIdentity.comp_id >= COMP_ID_FW_STA ) {
		PROT_CNT_INI;
		/*                                                                                     
                                                                                            
                                                                                
                                                                                        */
		rc = cmd_exe( ifbp, HCMD_ALLOC, 0 );
//                                          
//                                                                                                    
//     
		if ( rc == HCF_SUCCESS ) {
			HCF_WAIT_WHILE( (IPW( HREG_EV_STAT ) & HREG_EV_ALLOC) == 0 );
			IF_PROT_TIME( HCFASSERT(prot_cnt, IPW( HREG_EV_STAT )) );
#if HCF_DMA
			if ( ! ( ifbp->IFB_CntlOpt & USE_DMA ) )
#endif //        
			{
				ifbp->IFB_RscInd = get_fid( ifbp );
				HCFASSERT( ifbp->IFB_RscInd, 0 );
				cmd_exe( ifbp, HCMD_ALLOC, 0 );
				IF_PROT_TIME( if ( prot_cnt == 0 ) rc = HCF_ERR_TIME_OUT );
			}
		}
//                  
	}

	HCFASSERT( rc == HCF_SUCCESS, rc );
	HCFLOGEXIT( HCF_TRACE_INIT );
	return rc;
} //     

/*                                                                                                           
  
                                           
                                     
  
            
                                               
  
                     
  
              
  
  
          
                                                                         
                                                                         
                                                                           
                                                                                                          
                                                                                                  
                                                                                                        
                                                                                                       
                                                    
                                                                                            
                                                                                                       
                                                                                                          
                                                                                                  
                                                                                                             
                                                                                               
  
                                          
  
                                                                                                            */
HCF_STATIC void
isr_info( IFBP ifbp )
{
	hcf_16  info[2], fid;
#if (HCF_EXT) & HCF_EXT_INFO_LOG
	RID_LOGP    ridp = ifbp->IFB_RIDLogp;   //                                                                       
#endif //                 

	HCFTRACE( ifbp, HCF_TRACE_ISR_INFO );                                                               /*   */
	fid = IPW( HREG_INFO_FID );
	DAWA_ZERO_FID( HREG_INFO_FID );
	if ( fid ) {
		(void)setup_bap( ifbp, fid, 0, IO_IN );
		get_frag( ifbp, (wci_bufp)info, 4 BE_PAR(2) );
		HCFASSERT( info[0] <= HCF_MAX_LTV + 1, MERGE_2( info[1], info[0] ) );  //                                   
#if (HCF_TALLIES) & HCF_TALLIES_NIC     //                    
		if ( info[1] == CFG_TALLIES ) {
			hcf_32  *p;
		/* */   if ( info[0] > HCF_NIC_TAL_CNT ) {
				info[0] = HCF_NIC_TAL_CNT + 1;
			}
			p = (hcf_32*)&ifbp->IFB_NIC_Tallies;
			while ( info[0]-- >1 ) *p++ += IPW( HREG_DATA_1 );  //                              
		}
		else
#endif //                
		{
		/* */   if ( info[1] == CFG_LINK_STAT ) {
				ifbp->IFB_LinkStat = IPW( HREG_DATA_1 );
			}
#if (HCF_EXT) & HCF_EXT_INFO_LOG
		/* */   while ( 1 ) {
				if ( ridp->typ == 0 || ridp->typ == info[1] ) {
					if ( ridp->bufp ) {
						HCFASSERT( ridp->len >= 2, ridp->typ );
						ridp->bufp[0] = min((hcf_16)(ridp->len - 1), info[0] );     //      
						ridp->bufp[1] = info[1];                        //      
						get_frag( ifbp, (wci_bufp)&ridp->bufp[2], (ridp->bufp[0] - 1)*2 BE_PAR(0) );
					}
					break;
				}
				ridp++;
			}
#endif //                 
		}
		HCFTRACE( ifbp, HCF_TRACE_ISR_INFO | HCF_TRACE_EXIT );
	}
	return;
} //         

//
//
//                          
//                                           
//                                                                                                  
//                                                                                            
//                                                                                             
//                          
//                                                                                                                 
//                                                                                                  
//                                                                                                                 
//           
//                  
//       
//                                 
//                       
//                                                         
//                                 
//                                                         
//                                                                                                                                             
//                                                                          
//                                                                                        
//               
//                                                                                                          
//           
//                 
//       
//                           
//   
//                                                        
//
//
//
//
//         
//             


/*                                                                                                           
  
                                                                                 
                                                                                               
  
            
                                               
                                                                
                                                                                          
  
                     
  
              
  
  
          
  
         
                                                                                                               
                                                                                                              
       
                                                                                                     
                                                                          
                                                                                                    
                                                                                                       
  
                                                                                                              
                                          
  
                                          
  
                                                                                                            */
#if HCF_ASSERT
void
mdd_assert( IFBP ifbp, unsigned int line_number, hcf_32 q )
{
	hcf_16  run_time_flag = ifbp->IFB_AssertLvl;

	if ( run_time_flag /*           */ ) { //                                                                   
		ifbp->IFB_AssertQualifier = q;
		ifbp->IFB_AssertLine = (hcf_16)line_number;
#if (HCF_ASSERT) & ( HCF_ASSERT_LNK_MSF_RTN | HCF_ASSERT_RT_MSF_RTN )
		if ( ifbp->IFB_AssertRtn ) {
			ifbp->IFB_AssertRtn( line_number, ifbp->IFB_AssertTrace, q );
		}
#endif //                                               
#if (HCF_ASSERT) & HCF_ASSERT_SW_SUP
		OPW( HREG_SW_2, line_number );
		OPW( HREG_SW_2, ifbp->IFB_AssertTrace );
		OPW( HREG_SW_2, (hcf_16)q );
		OPW( HREG_SW_2, (hcf_16)(q >> 16 ) );
#endif //                  

#if (HCF_ASSERT) & HCF_ASSERT_MB
		ifbp->IFB_AssertLvl = 0;                                    //                           
		hcf_put_info( ifbp, (LTVP)&ifbp->IFB_AssertStrct );
		ifbp->IFB_AssertLvl = run_time_flag;                        //                                 
#endif //              
	}
} //           
#endif //           


/*                                                                                                           
  
                                                                                          
                                                                                     
  
            
                                               
                                         
                                                            
                                                                          
  
                     
  
              
                                                                                                
                                     
  
  
          
  
         
                                                                                                             
                              
  
                                          
  
                                                                                                            */
HCF_STATIC void
put_frag( IFBP ifbp, wci_bufp bufp, int len BE_PAR( int word_len ) )
{
	hcf_io      io_port = ifbp->IFB_IOBase + HREG_DATA_1;   //                 
	int         i;                                          //                               
	hcf_16      j;
	HCFASSERT( ((hcf_32)bufp & (HCF_ALIGN-1) ) == 0, (hcf_32)bufp );
#if HCF_BIG_ENDIAN
	HCFASSERT( word_len == 0 || word_len == 2 || word_len == 4, word_len );
	HCFASSERT( word_len == 0 || ((hcf_32)bufp & 1 ) == 0, (hcf_32)bufp );
	HCFASSERT( word_len <= len, MERGE_2( word_len, len ) );

	if ( word_len ) {                                   //                               
		                                //                                   
		j = bufp[1] | bufp[0]<<8;
		OUT_PORT_WORD( io_port, j );
		                                //                                         
		len -= 2;
		bufp += 2;
		if ( word_len > 1 ) {           //                                                    
			                        //                                      
			j = bufp[1] | bufp[0]<<8;   /*                                */
			OUT_PORT_WORD( io_port, j );
			                        //                                            
			len -= 2;
			bufp += 2;
		}
	}
#endif //               
	i = len;
	if ( i && ifbp->IFB_CarryOut ) {                    //                
		j = ((*bufp)<<8) + ( ifbp->IFB_CarryOut & 0xFF );
		OUT_PORT_WORD( io_port, j );
		bufp++; i--;
		ifbp->IFB_CarryOut = 0;
	}
#if (HCF_IO) & HCF_IO_32BITS
	//                                                                                                 
	                                                        //                                             
	if ( !(ifbp->IFB_CntlOpt & USE_16BIT) && i >= 6 ) {
		hcf_32 FAR  *p4; //                               
		if ( ( (hcf_32)bufp & 0x1 ) == 0 ) {            //                                  
			if ( (hcf_32)bufp & 0x2 ) {             //                                       
								//                                                       
				j = *(wci_recordp)bufp;     //                                                       
				OUT_PORT_WORD( io_port, j );
				                                //                                                     
				bufp += 2; i -= 2;
			}
			                                        //                                           
			p4 = (hcf_32 FAR *)bufp;
			j = (hcf_16)i/4;
			OUT_PORT_STRING_32( io_port, p4, j );
			                                        //                                                  
			bufp += i & ~0x0003;
			i &= 0x0003;
		}
	}
#endif //              
	                                        //                                                   
	if ( i ) {
		                                //                              
		if ( i & 0x0001 ) {
			                        //                                                                             
			ifbp->IFB_CarryOut = (hcf_16)bufp[i-1] | 0x0100;    //                                                                                
		}
		                                //                                                         
		j = (hcf_16)i/2;
		OUT_PORT_STRING_8_16( io_port, bufp, j );
	}
} //         


/*                                                                                                           
  
                                                    
                                                                                      
  
            
                                               
  
                     
  
              
                                                                                              
                                         
  
  
          
                                                                                                        
                                                                                                            
                                                                                                     
                                                                                                            
                                                                           
                                                                        
                                                                   
  
         
  
                                          
  
                                                                                                            */
HCF_STATIC void
put_frag_finalize( IFBP ifbp )
{
#if (HCF_TYPE) & HCF_TYPE_WPA
	if ( ifbp->IFB_MICTxCarry != 0xFFFF) {      //                         
		CALC_TX_MIC( mic_pad, 8);               //                                                        
		                                        //                                                 
		put_frag( ifbp, (wci_bufp)ifbp->IFB_MICTx, 8 BE_PAR(0) );
	}
#endif //             
	put_frag( ifbp, null_addr, 1 BE_PAR(0) );   //                                          
} //                  


/*                                                                                                           
  
                                                     
                                                                                                    
  
            
                                               
                                                                 
  
          
                
               
               
  
              
  
  
          
                                                                
                                                            
                                                                                                              
                                                                                              
  
         
                                                                                                           
                                                                                                      
                                                                     
                                                                                                            
                                                                                                             
                                                                                                    
                                                                                                           
                   
  
                                          
  
                                                                                                            */
HCF_STATIC int
put_info( IFBP ifbp, LTVP ltvp  )
{

	int rc = HCF_SUCCESS;

	HCFASSERT( ifbp->IFB_CardStat == 0, MERGE_2( ltvp->typ, ifbp->IFB_CardStat ) );
	HCFASSERT( CFG_RID_CFG_MIN <= ltvp->typ && ltvp->typ <= CFG_RID_CFG_MAX, ltvp->typ );

	if ( ifbp->IFB_CardStat == 0 &&                                                             /*   */
	     ( ( CFG_RID_CFG_MIN <= ltvp->typ    && ltvp->typ <= CFG_RID_CFG_MAX ) ||
	       ( CFG_RID_ENG_MIN <= ltvp->typ /*                        */       )     ) ) {
#if HCF_ASSERT //                                                                      
		{
			hcf_16     t = ltvp->typ;
			LTV_STRCT  x = { 2, t, {0} };                                                          /*  */
			hcf_get_info( ifbp, (LTVP)&x );
			if ( x.len == 0 &&
			     ( t != CFG_DEFAULT_KEYS && t != CFG_ADD_TKIP_DEFAULT_KEY && t != CFG_REMOVE_TKIP_DEFAULT_KEY &&
			       t != CFG_ADD_TKIP_MAPPED_KEY && t != CFG_REMOVE_TKIP_MAPPED_KEY &&
			       t != CFG_HANDOVER_ADDR && t != CFG_DISASSOCIATE_ADDR &&
			       t != CFG_FCBC && t != CFG_FCBD && t != CFG_FCBE && t != CFG_FCBF &&
			       t != CFG_DEAUTHENTICATE_ADDR
				     )
				) {
				HCFASSERT( DO_ASSERT, ltvp->typ );
			}
		}
#endif //           

		rc = setup_bap( ifbp, ltvp->typ, 0, IO_OUT );
		put_frag( ifbp, (wci_bufp)ltvp, 2*ltvp->len + 2 BE_PAR(2) );
	/*  */  if ( rc == HCF_SUCCESS ) {
			rc = cmd_exe( ifbp, HCMD_ACCESS + HCMD_ACCESS_WRITE, ltvp->typ );
		}
	}
	return rc;
} //         


/*                                                                                                           
  
                                                                           
                                                                                              
  
            
                                               
                                                                                                                  
                                           
  
          
  
              
                                                                                                      
                            
                             
                                                                                                   
  
                                                                             
                    
                                                                                                      
                                                                                                        
                                                                        
  
  
          
                                                                                                    
                                                                                                         
                                                                                                              
                                                                                                               
                                                                                               
                                        
                                
                                                                              
  
                                                                                                       
                                                                                                          
                                                                                                     
                                                                                                        
                                                                        
                                                                                                         
                          
                                                                                                           
                                                                                                          
                                                                                                            
              
  
         
                                                                                                            
                                                         
  
                                          
  
                                                                                                            */

HCF_STATIC int
put_info_mb( IFBP ifbp, CFG_MB_INFO_STRCT FAR * ltvp )
{

	int         rc = HCF_SUCCESS;
	hcf_16      i;                      //            
	hcf_16      *dp;                    //                                
	wci_recordp sp;                     //              
	hcf_16      len;                    //                               
	hcf_16      tlen;                   //                                              

	if ( ifbp->IFB_MBp == NULL ) return rc;  //                
	HCFASSERT( ifbp->IFB_MBp != NULL, 0 );                   //                                                  
	HCFASSERT( ifbp->IFB_MBSize, 0 );

	len = 1;                                                                                            /*   */
	for ( i = 0; i < ltvp->frag_cnt; i++ ) {
		len += ltvp->frag_buf[i].frag_len;
	}
	if ( ifbp->IFB_MBRp > ifbp->IFB_MBWp ) {
		tlen = ifbp->IFB_MBRp - ifbp->IFB_MBWp;                                                         /*   */
	} else {
		if ( ifbp->IFB_MBRp == ifbp->IFB_MBWp ) {
			ifbp->IFB_MBRp = ifbp->IFB_MBWp = 0;    //                  
		}
		tlen = ifbp->IFB_MBSize - ifbp->IFB_MBWp;                                                       /*   */
		if ( ( tlen <= len + 2 ) && ( len + 2 < ifbp->IFB_MBRp ) ) {    //                                  
			                                                        //                                      
			ifbp->IFB_MBp[ifbp->IFB_MBWp] = 0xFFFF;                 //                                         
			ifbp->IFB_MBWp = 0;                                     //                                      
			tlen = ifbp->IFB_MBRp;                                  //                            
		}
	}
	dp = &ifbp->IFB_MBp[ifbp->IFB_MBWp];
	if ( len == 0 ) {
		tlen = 0; //                        
	}
	if ( len + 2 >= tlen ){                                                                             /*   */
		//                                         
		IF_TALLY( ifbp->IFB_HCF_Tallies.NoBufMB++ );
		rc = HCF_ERR_LEN;
	} else {
		*dp++ = len;                                    //                                                  
		*dp++ = ltvp->base_typ;                         //                           
		ifbp->IFB_MBWp += len + 1;                      //                              
		for ( i = 0; i < ltvp->frag_cnt; i++ ) {                //                              
			sp = ltvp->frag_buf[i].frag_addr;
			len = ltvp->frag_buf[i].frag_len;
			while ( len-- ) *dp++ = *sp++;
		}
		ifbp->IFB_MBp[ifbp->IFB_MBWp] = 0;              //                                        
		ifbp->IFB_MBInfoLen = ifbp->IFB_MBp[ifbp->IFB_MBRp];                                            /*   */
	}
	return rc;
} //            


/*                                                                                                           
  
                                                                             
                                                         
  
            
                                         
                            
                                               
                                  
  
          
                                    
                                                
                                                           
                                                                    
  
              
  
                                      
                                                                                                     
                                            
                                                               
                                          
                                                                                                         
                                                                                                           
                                                                                 
  
                                                                                                          
                                                         
                                                                                                          
  
                                                                                                             
                                                                                               
                                                                                                         
                                                                                                     
                                                                                                             
                                                                                                           
  
                                                                                                         
                                                                                                             
                                                                                                            
                                                                                                            
           
  
  
          
  
                                                                                                          
                             
                                                    
                                                                                                       
                                            
                                                                                                            
                         
                                                        
                                                 
                                        
                                                                                                           
                                                                                                              
                                                                                                               
                                                         
                                                                                                        
                                                                                                           
                                                                                                           
                                          
                                                                                                          
                                                                                              
  
                                                    
  
                                                                                                            */
HCF_STATIC int
setup_bap( IFBP ifbp, hcf_16 fid, int offset, int type )
{
	PROT_CNT_INI;
	int rc;

	HCFTRACE( ifbp, HCF_TRACE_STRIO );
	rc = ifbp->IFB_DefunctStat;
	if (rc == HCF_SUCCESS) {                                        /* */
		OPW( HREG_SELECT_1, fid );                                                              /* */
		OPW( HREG_OFFSET_1, offset );
		if ( type == IO_IN ) {
			ifbp->IFB_CarryIn = 0;
		}
		else ifbp->IFB_CarryOut = 0;
		HCF_WAIT_WHILE( IPW( HREG_OFFSET_1) & HCMD_BUSY );
		HCFASSERT( !( IPW( HREG_OFFSET_1) & HREG_OFFSET_ERR ), MERGE_2( fid, offset ) );         /* */
		if ( prot_cnt == 0 ) {
			HCFASSERT( DO_ASSERT, MERGE_2( fid, offset ) );
			rc = ifbp->IFB_DefunctStat = HCF_ERR_DEFUNCT_TIME_OUT;
			ifbp->IFB_CardStat |= CARD_STAT_DEFUNCT;
		}
	}
	HCFTRACE( ifbp, HCF_TRACE_STRIO | HCF_TRACE_EXIT );
	return rc;
} //          

