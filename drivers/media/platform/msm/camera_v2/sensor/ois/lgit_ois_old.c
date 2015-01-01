//                                                                              
//                      
//                                                                              
//                          
//                      
//                          

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/leds.h>
#include <linux/errno.h>
#include <linux/i2c.h>
#include <mach/gpio.h>
#include <linux/delay.h>
#include <linux/hrtimer.h>
#include <linux/types.h>
#include <mach/camera2.h>
#include "msm_ois.h"
#include "msm_ois_i2c.h"


#define		OISINI
#include "lgit_ois_old.h"

#define LAST_UPDATE  "13-08-01, 0x0204B_Q2_3" 


//                          
//                
//                          
unsigned char spigyrocheck=0x00;

#define	OIS_FW_POLLING_PASS		OIS_SUCCESS
#define	OIS_FW_POLLING_FAIL		OIS_INIT_TIMEOUT
#define	CLRGYR_POLLING_LIMIT_A	6
#define	ACCWIT_POLLING_LIMIT_A	6
#define	INIDGY_POLLING_LIMIT_A	12
#define INIDGY_POLLING_LIMIT_B	12
#define BSYWIT_POLLING_LIMIT_A  6

#define HALREGTAB	32
#define HALFILTAB	138
#define	GYRFILTAB	125

/*                              */
/*                                      */
/*                                      */
/*          */
/*                   */
/*        */

/*      */
OISINI__ const struct STHALREG	CsHalReg[][HALREGTAB]	= {
	{	//                                       
		{ 0x000E, 0x00},	/*       */
		{ 0x000F, 0x00},	/*           */
		{ 0x0086, 0x00},	/*           */
		{ 0x0087, 0x00},	/*           */
		{ 0x0088, 0x15},	/*            */
		{ 0x0089, 0x00},	/*       */
		{ 0x008A, 0x40},	/*                                     */
		{ 0x008B, 0x54},	/*                                   */
		{ 0x008C, 0x00},	/*                                       */
		{ 0x0090, 0x00},	/*           */
		{ 0x0091, 0x00},	/*           */
		{ 0x0092, 0x15},	/*            */
		{ 0x0093, 0x00},	/*       */
		{ 0x0094, 0x40},	/*                                     */
		{ 0x0095, 0x54},	/*                                   */
		{ 0x0096, 0x00},	/*                                       */
		{ 0x00A0, 0x00},	/*           */
		{ 0x00A1, 0x00},	/*       */
		{ 0x00B4, 0x00},	/*       */
		{ 0x00B5, 0x00},	/*           */
		{ 0x00B6, 0x00},	/*                                 */
		{ 0x00BB, 0x00},	/*       */
		{ 0x00BC, 0x00},	/*           */
		{ 0x00BD, 0x00},	/*                                 */
		{ 0x00C1, 0x00},	/*                                 */
		{ 0x00C5, 0x00},	/*                                 */
		{ 0x00C8, 0x00},	/*       */
		{ 0x0110, 0x01},	/*       */
		{ 0x0111, 0x00},	/*       */
		{ 0x0112, 0x00},	/*       */
		{ 0x017D, 0x01},	/*       */
		{ 0xFFFF , 0xFF }
	}
} ;

/*       */
OISINI__ const struct STHALFIL	CsHalFil[][HALFILTAB]	= {
	{	//                                       
		{ 0x1128, 0x0000},	/*                         */
		{ 0x1168, 0x0000},	/*                         */
		{ 0x11E0, 0x7FFF},	/*                      */
		{ 0x11E1, 0x7FFF},	/*                      */
		{ 0x11E2, 0x7FFF},	/*                      */
		{ 0x11E3, 0x7FFF},	/*                      */
		{ 0x11E4, 0x7FFF},	/*                      */
		{ 0x11E5, 0x7FFF},	/*                      */
		{ 0x12E0, 0x7FFF},	/*                                   */
		{ 0x12E1, 0x0000},	/*                                   */
		{ 0x12E2, 0x0000},	/*                                   */
		{ 0x12E3, 0x7FFF},	/*                                   */
		{ 0x12E4, 0x0000},	/*                                   */
		{ 0x12E5, 0x0000},	/*                                   */
		{ 0x12E6, 0x7FFF},	/*                      */
		{ 0x1301, 0x7FFF},	/*                      */
		{ 0x1302, 0x7FFF},	/*                      */
		{ 0x1305, 0x7FFF},	/*                                   */
		{ 0x1306, 0x0000},	/*                                   */
		{ 0x1307, 0x0000},	/*                                   */
		{ 0x1308, 0x0000},	/*                         */
		{ 0x1309, 0x5A9D},	/*                       */
		{ 0x130A, 0x0000},	/*                         */
		{ 0x130B, 0x0000},	/*                         */
		{ 0x130C, 0x7FFF},	/*                      */
		{ 0x130D, 0x46EB},	/*                                            */
		{ 0x130E, 0xBA1D},	/*                                            */
		{ 0x130F, 0x3679},	/*                                            */
		{ 0x1310, 0x0000},	/*                                            */
		{ 0x1311, 0x0000},	/*                                            */
		{ 0x1312, 0x43B3},	/*                                           */
		{ 0x1313, 0xBCD7},	/*                                           */
		{ 0x1314, 0x3F51},	/*                                           */
		{ 0x1315, 0x0000},	/*                                           */
		{ 0x1316, 0x0000},	/*                                           */
		{ 0x1317, 0x7FFF},	/*                      */
		{ 0x1318, 0x0000},	/*                         */
		{ 0x1319, 0x0000},	/*                         */
		{ 0x131A, 0x002B},	/*                                       */
		{ 0x131B, 0x0055},	/*                                       */
		{ 0x131C, 0x72F9},	/*                                       */
		{ 0x131D, 0x002B},	/*                                       */
		{ 0x131E, 0xCC5D},	/*                                       */
		{ 0x131F, 0x020D},	/*                                      */
		{ 0x1320, 0x020D},	/*                                      */
		{ 0x1321, 0x7FCB},	/*                                      */
		{ 0x1322, 0x0000},	/*                                      */
		{ 0x1323, 0x0000},	/*                                      */
		{ 0x1324, 0x334D},	/*                                         */
		{ 0x1325, 0xCD0A},	/*                                         */
		{ 0x1326, 0x7FA9},	/*                                         */
		{ 0x1327, 0x0000},	/*                                         */
		{ 0x1328, 0x0000},	/*                                         */
		{ 0x1329, 0x7FFF},	/*                      */
		{ 0x132A, 0x7FFF},	/*                      */
		{ 0x132B, 0x0000},	/*                         */
		{ 0x132C, 0x0000},	/*                         */
		{ 0x132D, 0x0000},	/*                         */
		{ 0x132E, 0x0000},	/*                         */
		{ 0x132F, 0x7485},	/*                                           */
		{ 0x1330, 0x8EDF},	/*                                           */
		{ 0x1331, 0x7C9D},	/*                                           */
		{ 0x1332, 0x3853},	/*                                          */
		{ 0x1333, 0x9CAB},	/*                                          */
		{ 0x1334, 0x6355},	/*                                          */
		{ 0x1335, 0x313B},	/*                                          */
		{ 0x1336, 0xD672},	/*                                          */
		{ 0x1337, 0x0D17},	/*                                     */
		{ 0x1338, 0x0D17},	/*                                     */
		{ 0x1339, 0x65D1},	/*                                     */
		{ 0x133A, 0x0000},	/*                                     */
		{ 0x133B, 0x0000},	/*                                     */
		{ 0x133C, 0x7FFF},	/*                      */
		{ 0x133D, 0x0000},	/*                         */
		{ 0x133E, 0x0000},	/*                         */
		{ 0x133F, 0x7FFF},	/*                      */
		{ 0x1341, 0x7FFF},	/*                      */
		{ 0x1342, 0x7FFF},	/*                      */
		{ 0x1345, 0x7FFF},	/*                                   */
		{ 0x1346, 0x0000},	/*                                   */
		{ 0x1347, 0x0000},	/*                                   */
		{ 0x1348, 0x0000},	/*                         */
		{ 0x1349, 0x5A9D},	/*                       */
		{ 0x134A, 0x0000},	/*                         */
		{ 0x134B, 0x0000},	/*                         */
		{ 0x134C, 0x7FFF},	/*                      */
		{ 0x134D, 0x46EB},	/*                                            */
		{ 0x134E, 0xBA1D},	/*                                            */
		{ 0x134F, 0x3679},	/*                                            */
		{ 0x1350, 0x0000},	/*                                            */
		{ 0x1351, 0x0000},	/*                                            */
		{ 0x1352, 0x43B3},	/*                                           */
		{ 0x1353, 0xBCD7},	/*                                           */
		{ 0x1354, 0x3F51},	/*                                           */
		{ 0x1355, 0x0000},	/*                                           */
		{ 0x1356, 0x0000},	/*                                           */
		{ 0x1357, 0x7FFF},	/*                      */
		{ 0x1358, 0x0000},	/*                         */
		{ 0x1359, 0x0000},	/*                         */
		{ 0x135A, 0x002B},	/*                                       */
		{ 0x135B, 0x0055},	/*                                       */
		{ 0x135C, 0x72F9},	/*                                       */
		{ 0x135D, 0x002B},	/*                                       */
		{ 0x135E, 0xCC5D},	/*                                       */
		{ 0x135F, 0x020D},	/*                                      */
		{ 0x1360, 0x020D},	/*                                      */
		{ 0x1361, 0x7FCB},	/*                                      */
		{ 0x1362, 0x0000},	/*                                      */
		{ 0x1363, 0x0000},	/*                                      */
		{ 0x1364, 0x334D},	/*                                         */
		{ 0x1365, 0xCD0A},	/*                                         */
		{ 0x1366, 0x7FA9},	/*                                         */
		{ 0x1367, 0x0000},	/*                                         */
		{ 0x1368, 0x0000},	/*                                         */
		{ 0x1369, 0x7FFF},	/*                      */
		{ 0x136A, 0x7FFF},	/*                      */
		{ 0x136B, 0x0000},	/*                         */
		{ 0x136C, 0x0000},	/*                         */
		{ 0x136D, 0x0000},	/*                         */
		{ 0x136E, 0x0000},	/*                         */
		{ 0x136F, 0x7485},	/*                                           */
		{ 0x1370, 0x8EDF},	/*                                           */
		{ 0x1371, 0x7C9D},	/*                                           */
		{ 0x1372, 0x3853},	/*                                          */
		{ 0x1373, 0x9CAB},	/*                                          */
		{ 0x1374, 0x6355},	/*                                          */
		{ 0x1375, 0x313B},	/*                                          */
		{ 0x1376, 0xD672},	/*                                          */
		{ 0x1377, 0x0D17},	/*                                     */
		{ 0x1378, 0x0D17},	/*                                     */
		{ 0x1379, 0x65D1},	/*                                     */
		{ 0x137A, 0x0000},	/*                                     */
		{ 0x137B, 0x0000},	/*                                     */
		{ 0x137C, 0x7FFF},	/*                      */
		{ 0x137D, 0x0000},	/*                         */
		{ 0x137E, 0x0000},	/*                         */
		{ 0x137F, 0x7FFF},	/*                      */
		{ 0xFFFF , 0xFFFF }
	}
} ;
/*       */
OISINI__ const struct STGYRFIL	CsGyrFil[][GYRFILTAB]	= {
	{	//      
		{ 0x1800, 0x3F800000},	/*                          */
		{ 0x1801, 0x3C3F00A9},	/*                                        */
		{ 0x1802, 0x3F7A07FB},	/*                                        */
		{ 0x1803, 0x38A8A554},	/*                                         */
		{ 0x1804, 0x38A8A554},	/*                                         */
		{ 0x1805, 0x3F7FF576},	/*                                         */
		{ 0x1806, 0x3C3F00A9},	/*                                        */
		{ 0x1807, 0x00000000},	/*                             */
		{ 0x180A, 0x38A8A554},	/*                                         */
		{ 0x180B, 0x38A8A554},	/*                                         */
		{ 0x180C, 0x3F7FF576},	/*                                         */
		{ 0x180D, 0x3F800000},	/*                          */
		{ 0x180E, 0xBF800000},	/*                          */
		{ 0x180F, 0x3FFF64C1},	/*                          */
		{ 0x1810, 0x3F800000},	/*                          */
		{ 0x1811, 0x3F800000},	/*                          */
		{ 0x1812, 0x3B02C2F2},	/*                                */
		{ 0x1813, 0x00000000},	/*                                */
		{ 0x1814, 0x3F7FFD80},	/*                                */
		{ 0x1815, 0x428C7352},	/*                                                */
		{ 0x1816, 0xC28AA79E},	/*                                                */
		{ 0x1817, 0x3DDE3847},	/*                                                */
		{ 0x1818, 0x3F231C22},	/*                                              */
		{ 0x1819, 0xBF1ECB8E},	/*                                              */
		{ 0x181A, 0x3F7A916B},	/*                                              */
		{ 0x181B, 0x00000000},	/*                             */
		{ 0x181C, 0x3F800000},	/*                          */
		{ 0x181D, 0x3F800000},	/*                                       */
		{ 0x181E, 0x00000000},	/*                                       */
		{ 0x181F, 0x00000000},	/*                                       */
		{ 0x1820, 0x3F75C43F},	/*                                               */
		{ 0x1821, 0xBF756FF8},	/*                                               */
		{ 0x1822, 0x3F7FABB9},	/*                                               */
		{ 0x1823, 0x3F800000},	/*                                       */
		{ 0x1824, 0x00000000},	/*                                       */
		{ 0x1825, 0x00000000},	/*                                       */
		{ 0x1826, 0x00000000},	/*                             */
		{ 0x1827, 0x3F800000},	/*                          */
		{ 0x1828, 0x3F800000},	/*                          */
		{ 0x1829, 0x41400000},	/*                                */
		{ 0x182A, 0x3F800000},	/*                          */
		{ 0x182B, 0x3F800000},	/*                          */
		{ 0x182C, 0x00000000},	/*                             */
		{ 0x1830, 0x3D1E56E0},	/*                                         */
		{ 0x1831, 0x3D1E56E0},	/*                                         */
		{ 0x1832, 0x3F6C3524},	/*                                         */
		{ 0x1833, 0x00000000},	/*                                         */
		{ 0x1834, 0x00000000},	/*                                         */
		{ 0x1835, 0x00000000},	/*                             */
		{ 0x1838, 0x3F800000},	/*                          */
		{ 0x1839, 0x3C58B55D},	/*                                         */
		{ 0x183A, 0x3C58B55D},	/*                                         */
		{ 0x183B, 0x3F793A55},	/*                                         */
		{ 0x183C, 0x3C58B55D},	/*                                         */
		{ 0x183D, 0x3C58B55D},	/*                                         */
		{ 0x183E, 0x3F793A55},	/*                                         */
		{ 0x1840, 0x38A8A554},	/*                                         */
		{ 0x1841, 0x38A8A554},	/*                                         */
		{ 0x1842, 0x3F7FF576},	/*                                         */
		{ 0x1850, 0x38A8A554},	/*                                         */
		{ 0x1851, 0x38A8A554},	/*                                         */
		{ 0x1852, 0x3F7FF576},	/*                                         */
		{ 0x1900, 0x3F800000},	/*                          */
		{ 0x1901, 0x3C3F00A9},	/*                                        */
		{ 0x1902, 0x3F7A07FB},	/*                                        */
		{ 0x1903, 0x38A8A554},	/*                                         */
		{ 0x1904, 0x38A8A554},	/*                                         */
		{ 0x1905, 0x3F7FF576},	/*                                         */
		{ 0x1906, 0x3C3F00A9},	/*                                        */
		{ 0x1907, 0x00000000},	/*                             */
		{ 0x190A, 0x38A8A554},	/*                                         */
		{ 0x190B, 0x38A8A554},	/*                                         */
		{ 0x190C, 0x3F7FF576},	/*                                         */
		{ 0x190D, 0x3F800000},	/*                          */
		{ 0x190E, 0xBF800000},	/*                          */
		{ 0x190F, 0x3FFF64C1},	/*                          */
		{ 0x1910, 0x3F800000},	/*                          */
		{ 0x1911, 0x3F800000},	/*                          */
		{ 0x1912, 0x3B02C2F2},	/*                                */
		{ 0x1913, 0x00000000},	/*                                */
		{ 0x1914, 0x3F7FFD80},	/*                                */
		{ 0x1915, 0x428C7352},	/*                                                */
		{ 0x1916, 0xC28AA79E},	/*                                                */
		{ 0x1917, 0x3DDE3847},	/*                                                */
		{ 0x1918, 0x3F231C22},	/*                                              */
		{ 0x1919, 0xBF1ECB8E},	/*                                              */
		{ 0x191A, 0x3F7A916B},	/*                                              */
		{ 0x191B, 0x00000000},	/*                             */
		{ 0x191C, 0x3F800000},	/*                          */
		{ 0x191D, 0x3F800000},	/*                                       */
		{ 0x191E, 0x00000000},	/*                                       */
		{ 0x191F, 0x00000000},	/*                                       */
		{ 0x1920, 0x3F75C43F},	/*                                               */
		{ 0x1921, 0xBF756FF8},	/*                                               */
		{ 0x1922, 0x3F7FABB9},	/*                                               */
		{ 0x1923, 0x3F800000},	/*                                       */
		{ 0x1924, 0x00000000},	/*                                       */
		{ 0x1925, 0x00000000},	/*                                       */
		{ 0x1926, 0x00000000},	/*                             */
		{ 0x1927, 0x3F800000},	/*                          */
		{ 0x1928, 0x3F800000},	/*                          */
		{ 0x1929, 0x41400000},	/*                                */
		{ 0x192A, 0x3F800000},	/*                          */
		{ 0x192B, 0x3F800000},	/*                          */
		{ 0x192C, 0x00000000},	/*                             */
		{ 0x1930, 0x3D1E56E0},	/*                                         */
		{ 0x1931, 0x3D1E56E0},	/*                                         */
		{ 0x1932, 0x3F6C3524},	/*                                         */
		{ 0x1933, 0x00000000},	/*                                         */
		{ 0x1934, 0x00000000},	/*                                         */
		{ 0x1935, 0x00000000},	/*                             */
		{ 0x1938, 0x3F800000},	/*                          */
		{ 0x1939, 0x3C58B55D},	/*                                         */
		{ 0x193A, 0x3C58B55D},	/*                                         */
		{ 0x193B, 0x3F793A55},	/*                                         */
		{ 0x193C, 0x3C58B55D},	/*                                         */
		{ 0x193D, 0x3C58B55D},	/*                                         */
		{ 0x193E, 0x3F793A55},	/*                                         */
		{ 0x1940, 0x38A8A554},	/*                                         */
		{ 0x1941, 0x38A8A554},	/*                                         */
		{ 0x1942, 0x3F7FF576},	/*                                         */
		{ 0x1950, 0x38A8A554},	/*                                         */
		{ 0x1951, 0x38A8A554},	/*                                         */
		{ 0x1952, 0x3F7FF576},	/*                                         */
		{ 0xFFFF , 0xFFFFFFFF }
	}
} ;

//                          
//                   
//                          
int OnsemiI2CCheck(void)
{
	unsigned char UcLsiVer;
	RegReadA( CVER, &UcLsiVer ); //      
	return (UcLsiVer == 0x43) ? 1 : 0;
}	

int	IniSet( void )
{
	
	WitTim(5);
	RegWriteA( SOFRES1	, 0x00 );	//                
	WitTim(5);
	RegWriteA( SOFRES1	, 0x11 );
	
	//                                  
	E2pDat() ;
	//              
	VerInf() ;
	//              
	IniClk() ;
	//                         
	IniIop() ;
	//                                
	IniMon() ;
	//                      
	IniSrv() ;
	//                            
	IniGyr() ; //       
	
	//                         
	//                            
	if( IniHfl() != OIS_FW_POLLING_PASS ) return OIS_FW_POLLING_FAIL ;
	//                            
	if( IniGfl() != OIS_FW_POLLING_PASS ) return OIS_FW_POLLING_FAIL ;
	//                            
	if( IniDgy() != OIS_FW_POLLING_PASS ) return OIS_FW_POLLING_FAIL ;

	//                         
	IniAdj() ; //      

	return OIS_FW_POLLING_PASS ;
}

//                                                                                
//                        
//                   
//                    
//                                                     
//                                                 
//                                                                                
void	E2pDat( void )
{
	
	MemClr( ( unsigned char * )&StCalDat, sizeof( stCalDat ) ) ;
	
	E2pRed( (unsigned short)ADJ_COMP_FLAG, 2,		( unsigned char * )&StCalDat.UsAdjCompF ) ;
	
	E2pRed( (unsigned short)HALL_OFFSET_X, 2,		( unsigned char * )&StCalDat.StHalAdj.UsHlxOff ) ;
	E2pRed( (unsigned short)HALL_BIAS_X, 2,			( unsigned char * )&StCalDat.StHalAdj.UsHlxGan ) ;
	E2pRed( (unsigned short)HALL_AD_OFFSET_X, 2,	( unsigned char * )&StCalDat.StHalAdj.UsAdxOff ) ;

	E2pRed( (unsigned short)HALL_OFFSET_Y, 2,		( unsigned char * )&StCalDat.StHalAdj.UsHlyOff ) ;
	E2pRed( (unsigned short)HALL_BIAS_Y, 2,			( unsigned char * )&StCalDat.StHalAdj.UsHlyGan ) ;
	E2pRed( (unsigned short)HALL_AD_OFFSET_Y, 2,	( unsigned char * )&StCalDat.StHalAdj.UsAdyOff ) ;

	E2pRed( (unsigned short)LOOP_GAIN_X, 2,			( unsigned char * )&StCalDat.StLopGan.UsLxgVal ) ;
	E2pRed( (unsigned short)LOOP_GAIN_Y, 2,			( unsigned char * )&StCalDat.StLopGan.UsLygVal ) ;

	E2pRed( (unsigned short)LENS_CENTER_FINAL_X, 2,	( unsigned char * )&StCalDat.StLenCen.UsLsxVal ) ;
	E2pRed( (unsigned short)LENS_CENTER_FINAL_Y, 2,	( unsigned char * )&StCalDat.StLenCen.UsLsyVal ) ;

	E2pRed( (unsigned short)GYRO_AD_OFFSET_X, 2,	( unsigned char * )&StCalDat.StGvcOff.UsGxoVal ) ;
	E2pRed( (unsigned short)GYRO_AD_OFFSET_Y, 2,	( unsigned char * )&StCalDat.StGvcOff.UsGyoVal ) ;

	E2pRed( (unsigned short)GYRO_GAIN_X, 4 ,		( unsigned char * )&StCalDat.UlGxgVal ) ;
	E2pRed( (unsigned short)GYRO_GAIN_Y, 4 ,		( unsigned char * )&StCalDat.UlGygVal ) ;

	E2pRed( (unsigned short)OSC_CLK_VAL, 2 ,		( unsigned char * )&StCalDat.UsOscVal ) ;
	
	E2pRed( (unsigned short)SYSTEM_VERSION_INFO, 2,	( unsigned char * )&StCalDat.UsVerDat ) ;

	return;
}

//                                                                                
//                        
//                   
//                    
//                                 
//                                                 
//                                                                                
void	VerInf( void )
{
	UcVerHig = (unsigned char)(StCalDat.UsVerDat >> 8 ) ;		//               
	UcVerLow = (unsigned char)(StCalDat.UsVerDat)  ;			//               

	CDBG("%s : %x, %x \n",__func__, UcVerHig, UcVerLow);

	if( UcVerHig == 0xFF  ){						//                                 
		UcVerHig = 0x00 ;
	}else if( UcVerHig <= (unsigned char)0x02 ){		//                    
		UcVerHig = 0x00 ;
	};

	if( UcVerLow == 0xFF ){							//                                 
		UcVerLow = 0x00 ;
	}else if( UcVerLow <= (unsigned char)0x04 ){		//                    
		UcVerLow = 0x00 ;
	};

	return;
}

//                                                                                
//                        
//                   
//                    
//                             
//                                                     
//                                                  
//                                                                                
void	IniClk( void )
{
	UcOscAdjFlg	= 0 ;					//              
	
	RegWriteA( CLKON,	0x13 ) ;		//                                                                                                      

	return;
}

//                                                                                
//                        
//                   
//                    
//                                        
//                                                     
//                                                  
//                                                                                
void	IniIop( void )
{
	/*                 */
	RegWriteA( IOP0SEL, 0x00 ); 	//                                                
	RegWriteA( IOP3SEL, 0x01 ); 	//                                                    
	//                                     
	RegWriteA( IOP4SEL, 0x00 ); 	//                                                    
	//                                            
	RegWriteA( IOP5SEL, 0x21 ); 	//                                                    
	//                                     
	RegWriteA( IOP6SEL, 0x11 ); 	//                                                    
	//                                      
	RegWriteA( SRMODE, 0x02 );		//                                    
	//                                

	return;
}

int	IniDgy( void )
{
	unsigned char	UcRegBak ;
	unsigned char	UcRamIni0, UcRamIni1, UcCntPla, UcCntPlb ;

	UcCntPla = 0 ;
	UcCntPlb = 0 ;
	
	/*                   */
	RegWriteA( GGADON	, 0x01 );		//                                                             
	RegWriteA( GGADSMPT , 0x0E);		//       
	/*                   */
	RegWriteA( GRSEL	, 0x01 );							//                                                           
	
	/*                          */
	RegWriteA( GRINI	, 0x80 );							//                                                                             
	
	//                                
	RegReadA( GIFDIV, &UcRegBak ) ;
	RegWriteA( GIFDIV, 0x04 ) ;				//                  
	RegWriteA( GRINI, 0x84 ) ;				//                                             
	
	//                   
	RegWriteA( GRADR0	, 0x6B ) ;					//                         
	RegWriteA( GSETDT	, 0x01 ) ;					//                      
	RegWriteA( GRACC	, 0x10 );					//                                                             
	
	if( AccWit( 0x10 ) == OIS_FW_POLLING_FAIL ){ return OIS_FW_POLLING_FAIL; }		/*                            */
	
	UcRamIni0 = 0x02 ;
	do{
		RegWriteA( GRACC,	0x01 ) ;				/*                                    */						
		if( AccWit( 0x01 ) == OIS_FW_POLLING_FAIL) { return OIS_FW_POLLING_FAIL; }	/*                            */
		RegReadA( GRADT0H, &UcRamIni0 ) ;			/*        */
		UcCntPla++ ;
	} while( ( UcRamIni0 & 0x02 ) && ( UcCntPla < INIDGY_POLLING_LIMIT_A ) );
	if( UcCntPla == INIDGY_POLLING_LIMIT_A ) { return OIS_FW_POLLING_FAIL; }
	
	WitTim(2);
	spigyrocheck = UcRamIni0;
	
	RegWriteA( GRADR0,	0x1B ) ;					//                       
	RegWriteA( GSETDT,	( FS_SEL << 3) ) ;			//                                            
	RegWriteA( GRACC,	0x10 ) ;					/*                          */
	if( AccWit( 0x10 ) == OIS_FW_POLLING_FAIL) { return OIS_FW_POLLING_FAIL; }		/*                            */
	/*                     */
	
	/*                     */
	RegWriteA( GRADR0	, 0x6A ) ;					//                        
	RegWriteA( GSETDT	, 0x11 ) ;					//                      
	RegWriteA( GRACC	, 0x10 );					//                                                             
	
						
	if( AccWit( 0x10 ) == OIS_FW_POLLING_FAIL) { return OIS_FW_POLLING_FAIL; }			/*                            */
	
	UcRamIni1 = 0x01 ;
	do{
		RegWriteA( GRACC,	0x01 ) ;				/*                                    */
		if( AccWit( 0x01 ) == OIS_FW_POLLING_FAIL) { return OIS_FW_POLLING_FAIL; }		/*                            */
		RegReadA( GRADT0H, &UcRamIni1 ) ;			/*        */
		UcCntPlb++ ;
	} while( ( UcRamIni1 & 0x01 ) && ( UcCntPlb < INIDGY_POLLING_LIMIT_B ) ) ;
	if( UcCntPlb == INIDGY_POLLING_LIMIT_B ) { return OIS_FW_POLLING_FAIL; }
	
	RegWriteA( GIFDIV, UcRegBak ) ;			//                  
	RegWriteA( GRINI, 0x80 ) ;				//                             
	
	//                          
	RegWriteA( GRADR0,	0x43 ) ;			//                         
	RegWriteA( GRADR1,	0x45 ) ;			//                         
	
	/*                 */
	RegWriteA( GRSEL	, 0x02 );							//                                                           
	
	return OIS_FW_POLLING_PASS;
}

void	IniMon( void )
{
	RegWriteA( PWMMONFC, 0x00 ) ;				//        
	RegWriteA( DAMONFC, 0x81 ) ;				//        
	
	RegWriteA( MONSELA, 0x57 ) ;				//        
	RegWriteA( MONSELB, 0x58 ) ;				//        
	RegWriteA( MONSELC, 0x56 ) ;				//        
	RegWriteA( MONSELD, 0x63 ) ;				//       
	
	return;
}

//                                                                                
//                        
//                   
//                    
//                                     
//                                                     
//                                                                                
void	IniSrv( void )
{
	RegWriteA( LXEQEN 	, 0x00 );				//       
	RegWriteA( LYEQEN 	, 0x00 );				//       
	
	RamWriteA( ADHXOFF,   0x0000 ) ;			//       
	RamWriteA( ADSAD4OFF, 0x0000 ) ;			//       
	RamWriteA( HXINOD,    0x0000 ) ;			//       
	RamWriteA( HXDCIN,    0x0000 ) ;			//       
	RamWriteA( HXSEPT1,    0x0000 ) ;			//       
	RamWriteA( HXSEPT2,    0x0000 ) ;			//       
	RamWriteA( HXSEPT3,    0x0000 ) ;			//       
	RamWriteA( LXDOBZ,     0x0000 ) ;			//       
	RamWriteA( LXFZF,      0x0000 ) ;			//       
	RamWriteA( LXFZB,      0x0000 ) ;			//       
	RamWriteA( LXDX,      0x0000 ) ;			//       
	RamWriteA( LXLMT,     0x7FFF ) ;			//       
	RamWriteA( LXLMT2,    0x7FFF ) ;			//       
	RamWriteA( LXLMTSD,   0x0000 ) ;			//       
	RamWriteA( PLXOFF,    0x0000 ) ;			//       
	RamWriteA( LXDODAT,   0x0000 ) ;			//       
	
	RamWriteA( ADHYOFF,   0x0000 ) ;			//       
	RamWriteA( HYINOD,    0x0000 ) ;			//       
	RamWriteA( HYDCIN,    0x0000 ) ;			//       
	RamWriteA( HYSEPT1,    0x0000 ) ;			//       
	RamWriteA( HYSEPT2,    0x0000 ) ;			//       
	RamWriteA( HYSEPT3,    0x0000 ) ;			//       
	RamWriteA( LYDOBZ,     0x0000 ) ;			//       
	RamWriteA( LYFZF,      0x0000 ) ;			//       
	RamWriteA( LYFZB,      0x0000 ) ;			//       
	RamWriteA( LYDX,      0x0000 ) ;			//       
	RamWriteA( LYLMT,     0x7FFF ) ;			//       
	RamWriteA( LYLMT2,    0x7FFF ) ;			//       
	RamWriteA( LYLMTSD,   0x0000 ) ;			//       
	RamWriteA( PLYOFF,    0x0000 ) ;			//       
	RamWriteA( LYDODAT,   0x0000 ) ;			//       

	RamWriteA( LXOLMT,		0x2C00 );			//                                      
	RamWriteA( LYOLMT,		0x2C00 );			//                                      
	RegWriteA( LXEQFC2 , 0x09 ) ;				//                       
	RegWriteA( LYEQFC2 , 0x09 ) ;				//         

	/*                                             */
	RegWriteA( LCXFC, (unsigned char)0x00 ) ;			//                                              
	RegWriteA( LCYFC, (unsigned char)0x00 ) ;			//                                              

	RegWriteA( LCY1INADD, (unsigned char)LXDOIN ) ;		//                                              
	RegWriteA( LCY1OUTADD, (unsigned char)DLY00 ) ;		//                                               
	RegWriteA( LCX1INADD, (unsigned char)DLY00 ) ;		//                                              
	RegWriteA( LCX1OUTADD, (unsigned char)LXADOIN ) ;	//                                               

	RegWriteA( LCX2INADD, (unsigned char)LYDOIN ) ;		//                                              
	RegWriteA( LCX2OUTADD, (unsigned char)DLY01 ) ;		//                                               
	RegWriteA( LCY2INADD, (unsigned char)DLY01 ) ;		//                                              
	RegWriteA( LCY2OUTADD, (unsigned char)LYADOIN ) ;	//                                               

	/*                                                           */
	RamWriteA( LCY1A0, 0x0000 ) ;			//         
	RamWriteA( LCY1A1, 0x4600 ) ;			//         
	RamWriteA( LCY1A2, 0x0000 ) ;			//         
	RamWriteA( LCY1A3, 0x3000 ) ;			//         
	RamWriteA( LCY1A4, 0x0000 ) ;			//         
	RamWriteA( LCY1A5, 0x0000 ) ;			//         
	RamWriteA( LCY1A6, 0x0000 ) ;			//         

	RamWriteA( LCX1A0, 0x0000 ) ;			//         
	RamWriteA( LCX1A1, 0x4600 ) ;			//         
	RamWriteA( LCX1A2, 0x0000 ) ;			//         
	RamWriteA( LCX1A3, 0x3000 ) ;			//         
	RamWriteA( LCX1A4, 0x0000 ) ;			//         
	RamWriteA( LCX1A5, 0x0000 ) ;			//         
	RamWriteA( LCX1A6, 0x0000 ) ;			//         

	RamWriteA( LCX2A0, 0x0000 ) ;			//         
	RamWriteA( LCX2A1, 0x4600 ) ;			//         
	RamWriteA( LCX2A2, 0x0000 ) ;			//         
	RamWriteA( LCX2A3, 0x3000 ) ;			//         
	RamWriteA( LCX2A4, 0x0000 ) ;			//         
	RamWriteA( LCX2A5, 0x0000 ) ;			//         
	RamWriteA( LCX2A6, 0x0000 ) ;			//         
	
	RamWriteA( LCY2A0, 0x0000 ) ;			//         
	RamWriteA( LCY2A1, 0x4600 ) ;			//         
	RamWriteA( LCY2A2, 0x0000 ) ;			//         
	RamWriteA( LCY2A3, 0x3000 ) ;			//         
	RamWriteA( LCY2A4, 0x0000 ) ;			//         
	RamWriteA( LCY2A5, 0x0000 ) ;			//         
	RamWriteA( LCY2A6, 0x0000 ) ;			//         
	
	RegWriteA( GDPX1INADD,  0x00 ) ;		//                       
	RegWriteA( GDPX1OUTADD, 0x00 ) ;		//                                        
	RegWriteA( GDPX2INADD,  0x00 ) ;		//                       
	RegWriteA( GDPX2OUTADD, 0x00 ) ;		//                                        

	//                      
	RegWriteA( GYINFC, 0x00 ) ;				//                                                                       
	
	//                   
	RegWriteA( SWEN, 0x08 ) ;				//                                                
	RegWriteA( SWFC2, 0x08 ) ;				//                
	
	//                  
	RegWriteA( MDLY1ADD, 0x10 ) ;			//                       
	RegWriteA( MDLY2ADD, 0x11 ) ;			//                       
	
	//                
	//                                                         
	//                                                          
	RegWriteA( DLYCLR, 0xFF ) ;				//                           
	WitTim(1);
	RegWriteA( DLYCLR2, 0xEC ) ;				//                           
	WitTim(1);
	RegWriteA( DLYCLR	, 0x00 );			//                   
	
	//            
	RegWriteA( RTXADD, 0x00 ) ;				//               
	RegWriteA( RTYADD, 0x00 ) ;				//               
	
	//                    
	DrvSw( OFF ) ;							/*                           */
	RegWriteA( DRVFC2	, 0x40 );			//                                               
	
	RegWriteA( PWMFC,   0x5C ) ;			//                                              
	RegWriteA( PWMPERIODX, 0x00 );			//       
	RegWriteA( PWMPERIODY, 0x00 );			//       
	
	RegWriteA( PWMA,    0x00 ) ;			//                       
	RegWriteA( PWMDLY1,  0x03 ) ;			//                             
	RegWriteA( PWMDLY2,  0x03 ) ;			//                             
	
	RegWriteA( LNA		, 0xC0 );			//                             
	RegWriteA( LNFC 	, 0x02 );			//       
	RegWriteA( LNSMTHX	, 0x80 );			//       
	RegWriteA( LNSMTHY	, 0x80 );			//       
	
	//             
	RegWriteA( FLGM, 0xCC ) ;				//                       
	RegWriteA( FLGIST, 0xCC ) ;				//                       
	RegWriteA( FLGIM2, 0xF8 ) ;				//                       
	RegWriteA( FLGIST2, 0xF8 ) ;			//                       
	
	//                 
	RegWriteA( FCSW, 0x00 ) ;				//                                                   
	RegWriteA( FCSW2, 0x00 ) ;				//                                                                  
	
	/*                  */
	RamWriteA( HXSMSTP   , 0x0400 ) ;					/*        */
	RamWriteA( HYSMSTP   , 0x0400 ) ;					/*        */
	
	RegWriteA( SSSFC1, 0x43 ) ;				//                                
	RegWriteA( SSSFC2, 0x03 ) ;				//                           
	RegWriteA( SSSFC3, 0x50 ) ;				//              
	
	RegWriteA( STBB, 0x00 ) ;				//                   

	return;
}

void	IniGyr( void )
{
	RegWriteA( GEQON	, 0x00 );				//                                                
	
	/*                   */
	ClrGyr( 0x00 , 0x03 );
	
	/*                   */
	RegWriteA( GGADON	, 0x01 );		//                                                             
	RegWriteA( GGADSMPT , 0x0E);		//       
	
	//        
	RamWrite32A( gxlmt1L, 0x00000000 ) ;	//       
	RamWrite32A( gxlmt1H, 0x3F800000 ) ;	//         
	RamWrite32A( gxlmt2L, 0x3B16BB99 ) ;	//              
	RamWrite32A( gxlmt2H, 0x3F800000 ) ;	//       
	
	RamWrite32A( gylmt1L, 0x00000000 ) ;	//       
	RamWrite32A( gylmt1H, 0x3F800000 ) ;	//         
	RamWrite32A( gylmt2L, 0x3B16BB99 ) ;	//              
	RamWrite32A( gylmt2H, 0x3F800000 ) ;	//       
	
	//         
	RamWrite32A( gxlmt3H0, 0x3E800000 ) ;	//       
	RamWrite32A( gylmt3H0, 0x3E800000 ) ;	//                              
	RamWrite32A( gxlmt3H1, 0x3E800000 ) ;	//       
	RamWrite32A( gylmt3H1, 0x3E800000 ) ;	//       

	RamWrite32A( gxlmt4H0, GYRLMT_S1 ) ;	//                                       
	RamWrite32A( gylmt4H0, GYRLMT_S1 ) ;	//                                       
	
	RamWrite32A( gxlmt4H1, GYRLMT_S2 ) ;	//                                       
	RamWrite32A( gylmt4H1, GYRLMT_S2 ) ;	//                                       
	
	//                
	RegWriteA( GDLYMON10, 0xF5 ) ;			//       
	RegWriteA( GDLYMON11, 0x01 ) ;			//       
	RegWriteA( GDLYMON20, 0xF5 ) ;			//       
	RegWriteA( GDLYMON21, 0x00 ) ;			//       
	RamWrite32A( gdm1g, 0x3F800000 ) ;		//       
	RamWrite32A( gdm2g, 0x3F800000 ) ;		//       
	
	/*                    */
	RegWriteA( GPANADDA, 0x14 ) ;			//                     
	RegWriteA( GPANADDB, 0x0E ) ;			//                   
	
	//         
	RamWrite32A( SttxHis, 	0x00000000 );			//       
	RamWrite32A( SttyHis, 	0x00000000 );			//       
	RamWrite32A( SttxaL, 	0x00000000 );			//       
	RamWrite32A( SttxbL, 	0x00000000 );			//       
	RamWrite32A( Sttx12aM, 	GYRA12_MID_DEG );		//       
	RamWrite32A( Sttx12aH, 	GYRA12_HGH_DEG );		//       
	RamWrite32A( Sttx12bM, 	GYRB12_MID );			//       
	RamWrite32A( Sttx12bH, 	GYRB12_HGH );			//       
	RamWrite32A( Sttx34aM, 	GYRA34_MID_DEG );		//       
	RamWrite32A( Sttx34aH, 	GYRA34_HGH_DEG );		//       
	RamWrite32A( Sttx34bM, 	GYRB34_MID );			//       
	RamWrite32A( Sttx34bH, 	GYRB34_HGH );			//       
	RamWrite32A( SttyaL, 	0x00000000 );			//       
	RamWrite32A( SttybL, 	0x00000000 );			//       
	RamWrite32A( Stty12aM, 	GYRA12_MID_DEG );		//       
	RamWrite32A( Stty12aH, 	GYRA12_HGH_DEG );		//       
	RamWrite32A( Stty12bM, 	GYRB12_MID );			//       
	RamWrite32A( Stty12bH, 	GYRB12_HGH );			//       
	RamWrite32A( Stty34aM, 	GYRA34_MID_DEG );		//       
	RamWrite32A( Stty34aH, 	GYRA34_HGH_DEG );		//       
	RamWrite32A( Stty34bM, 	GYRB34_MID );			//       
	RamWrite32A( Stty34bH, 	GYRB34_HGH );			//       
	
	//                         
	RegWriteA( GPANSTT31JUG0, 	0x01 );		//       
	RegWriteA( GPANSTT31JUG1, 	0x00 );		//       
	RegWriteA( GPANSTT13JUG0, 	0x00 );		//       
	RegWriteA( GPANSTT13JUG1, 	0x07 );		//       
	
	//            
	//                                              
	RegWriteA( GPANSTT1LEVTMR, 	0x00 );		//       
	
	//               
	RegWriteA( GPANTRSON0, 		0x01 );		//                                        
	RegWriteA( GPANTRSON1, 		0x1C );		//                            
	
	//              
	RegWriteA( GPANSTTSETGAIN, 	0x10 );		//       
	RegWriteA( GPANSTTSETISTP, 	0x10 );		//       
	RegWriteA( GPANSTTSETI1FTR,	0x10 );		//       
	RegWriteA( GPANSTTSETI2FTR,	0x10 );		//       
	
	//                           
	RegWriteA( GPANSTT2TMR0,	0xEA );		//       
	RegWriteA( GPANSTT2TMR1,	0x00 );		//       
	RegWriteA( GPANSTT4TMR0,	0x92 );		//       
	RegWriteA( GPANSTT4TMR1,	0x04 );		//       
	
	RegWriteA( GPANSTTXXXTH,	0x0F );		//       

	RegWriteA( GPANSTTSETILHLD,	0x00 );		//       
	
	RamWrite32A( gxlevmid, TRI_LEVEL );					//              
	RamWrite32A( gxlevhgh, TRI_HIGH );					//              
	RamWrite32A( gylevmid, TRI_LEVEL );					//              
	RamWrite32A( gylevhgh, TRI_HIGH );					//              
	RamWrite32A( gxadjmin, XMINGAIN );					//                
	RamWrite32A( gxadjmax, XMAXGAIN );					//                
	RamWrite32A( gxadjdn, XSTEPDN );					//             
	RamWrite32A( gxadjup, XSTEPUP );					//             
	RamWrite32A( gyadjmin, YMINGAIN );					//                
	RamWrite32A( gyadjmax, YMAXGAIN );					//                
	RamWrite32A( gyadjdn, YSTEPDN );					//             
	RamWrite32A( gyadjup, YSTEPUP );					//             
	
	RegWriteA( GLEVGXADD, (unsigned char)XMONADR );		//                    
	RegWriteA( GLEVGYADD, (unsigned char)YMONADR );		//                    
	RegWriteA( GLEVTMR, 		TIMEBSE );				//                 
	RegWriteA( GLEVTMRLOWGX, 	TIMELOW );				//                  
	RegWriteA( GLEVTMRMIDGX, 	TIMEMID );				//                  
	RegWriteA( GLEVTMRHGHGX, 	TIMEHGH );				//                  
	RegWriteA( GLEVTMRLOWGY, 	TIMELOW );				//                  
	RegWriteA( GLEVTMRMIDGY, 	TIMEMID );				//                  
	RegWriteA( GLEVTMRHGHGY, 	TIMEHGH );				//                  
	RegWriteA( GADJGANADD, (unsigned char)GANADR );		//                       
	RegWriteA( GADJGANGO, 		0x00 );					//                  
	
	SetPanTiltMode( OFF ) ;								/*              */

	return;
}

int	IniHfl( void )
{
	unsigned short	UsAryIda, UsAryIdb ;
	
	//                                     
	UsAryIda	= 0 ;
	while( CsHalReg[UcVerLow][ UsAryIda].UsRegAdd != 0xFFFF )
	{
		RegWriteA( CsHalReg[UcVerLow][ UsAryIda].UsRegAdd, CsHalReg[UcVerLow][ UsAryIda].UcRegDat ) ;
		UsAryIda++ ;
		if( UsAryIda >= HALREGTAB ){ return OIS_FW_POLLING_FAIL ; }
	}
	
	//                              
	UsAryIdb	= 0 ;
	while( CsHalFil[UcVerLow][ UsAryIdb].UsRamAdd != 0xFFFF )
	{
		if( CsHalFil[UcVerLow][ UsAryIdb].UsRamAdd < gag ) {
			RamWriteA( CsHalFil[UcVerLow][ UsAryIdb].UsRamAdd, CsHalFil[UcVerLow][ UsAryIdb].UsRamDat ) ;
		}
		UsAryIdb++ ;
		if( UsAryIdb >= HALFILTAB ){ return OIS_FW_POLLING_FAIL ; }
	}
	
	if( (unsigned char)(StCalDat.UsVerDat) <= (unsigned char)0x01 ){		//           
		RamWriteA( plxg, 0x8001 );
	}
	
	return OIS_FW_POLLING_PASS ;
}

int	IniGfl( void )
{
	unsigned short	UsAryIda ;
	
	//                              
	UsAryIda	= 0 ;

	while( CsGyrFil[UcVerLow][ UsAryIda].UsRamAdd != 0xFFFF )
	{
		if( ( CsGyrFil[UcVerLow][ UsAryIda].UsRamAdd & 0xFEFF ) < gxi2a_a ) {
			RamWrite32A( CsGyrFil[UcVerLow][ UsAryIda].UsRamAdd, CsGyrFil[UcVerLow][ UsAryIda].UlRamDat ) ;
		}
		UsAryIda++ ;
		if( UsAryIda >= GYRFILTAB ){ return OIS_FW_POLLING_FAIL ; }
	}

	return OIS_FW_POLLING_PASS ;
}

void	IniAdj( void )
{
//                         
	
	RegWriteA( CMSDAC, BIAS_CUR ) ;				//                        
	RegWriteA( OPGSEL, AMP_GAIN ) ;				//                     
	
	/*                        */
	if( (StCalDat.UsAdjCompF == 0x0000 ) || (StCalDat.UsAdjCompF == 0xFFFF ) || (StCalDat.UsAdjCompF & ( EXE_HXADJ - EXE_END )) ){
		RamWriteA( DAHLXO, DAHLXO_INI ) ;	//       
		RamWriteA( DAHLXB, DAHLXB_INI ) ;	//       
		RamWriteA( ADHXOFF, 0x0000 ) ;		//       
	}else{
		RamWriteA( DAHLXO, StCalDat.StHalAdj.UsHlxOff ) ;	//       
		RamWriteA( DAHLXB, StCalDat.StHalAdj.UsHlxGan ) ;	//       
		RamWriteA( ADHXOFF, StCalDat.StHalAdj.UsAdxOff ) ;	//       
	}
	
	/*                        */
	if( (StCalDat.UsAdjCompF == 0x0000 ) || (StCalDat.UsAdjCompF == 0xFFFF ) || (StCalDat.UsAdjCompF & ( EXE_HYADJ - EXE_END )) ){
		RamWriteA( DAHLYO, DAHLYO_INI ) ;	//       
		RamWriteA( DAHLYB, DAHLYB_INI ) ;	//       
		RamWriteA( ADHYOFF, 0x0000 ) ;		//       
	}else{
		RamWriteA( DAHLYO, StCalDat.StHalAdj.UsHlyOff ) ;	//       
		RamWriteA( DAHLYB, StCalDat.StHalAdj.UsHlyGan ) ;	//       
		RamWriteA( ADHYOFF, StCalDat.StHalAdj.UsAdyOff ) ;	//       
	}
	
	/*                      */
	if( (StCalDat.UsAdjCompF == 0x0000 ) || (StCalDat.UsAdjCompF == 0xFFFF ) || (StCalDat.UsAdjCompF & ( EXE_LXADJ - EXE_END )) ){
		RamWriteA( lxgain, LXGAIN_INI ) ;	//       
	}else{
		RamWriteA( lxgain, StCalDat.StLopGan.UsLxgVal ) ;	//       
	}
	
	/*                      */
	if( (StCalDat.UsAdjCompF == 0x0000 ) || (StCalDat.UsAdjCompF == 0xFFFF ) || (StCalDat.UsAdjCompF & ( EXE_LYADJ - EXE_END )) ){
		RamWriteA( lygain, LYGAIN_INI ) ;	//       
	}else{
		RamWriteA( lygain, StCalDat.StLopGan.UsLygVal ) ;	//       
	}
	
	/*             */
	//                                         
	if( ( StCalDat.StLenCen.UsLsxVal != 0x0000 ) && ( StCalDat.StLenCen.UsLsxVal != 0xffff )){
		UsCntXof = StCalDat.StLenCen.UsLsxVal ;					/*                         */
	} else {
		UsCntXof = MECCEN_X ;						/*                           */
	}
	RamWriteA( HXINOD, UsCntXof ) ;				//       
	
	//                                         
	if( ( StCalDat.StLenCen.UsLsyVal != 0x0000 ) && ( StCalDat.StLenCen.UsLsyVal != 0xffff )){
		UsCntYof = StCalDat.StLenCen.UsLsyVal ;					/*                         */
	} else {
		UsCntYof = MECCEN_Y ;						/*                           */
	}
	RamWriteA( HYINOD, UsCntYof ) ;				//       
	
	/*                   */
	if( ( StCalDat.StGvcOff.UsGxoVal == 0x0000 ) || ( StCalDat.StGvcOff.UsGxoVal == 0xffff )){
		RamWriteA( ADGXOFF, 0x0000 ) ;							//       
		RegWriteA( IZAH, DGYRO_OFST_XH ) ;						//                             
		RegWriteA( IZAL, DGYRO_OFST_XL ) ;						//                            
	}else{
		RamWriteA( ADGXOFF, 0x0000 ) ;							//       
		RegWriteA( IZAH, (unsigned char)(StCalDat.StGvcOff.UsGxoVal >> 8) ) ;	//                             
		RegWriteA( IZAL, (unsigned char)(StCalDat.StGvcOff.UsGxoVal) ) ;			//                            
	}
	
	/*                   */
	if( ( StCalDat.StGvcOff.UsGyoVal == 0x0000 ) || ( StCalDat.StGvcOff.UsGyoVal == 0xffff )){
		RamWriteA( ADGYOFF, 0x0000 ) ;							//       
		RegWriteA( IZBH, DGYRO_OFST_YH ) ;						//                             
		RegWriteA( IZBL, DGYRO_OFST_YL ) ;						//                            
	}else{
		RamWriteA( ADGYOFF, 0x0000 ) ;							//       
		RegWriteA( IZBH, (unsigned char)(StCalDat.StGvcOff.UsGyoVal >> 8) ) ;	//                             
		RegWriteA( IZBL, (unsigned char)(StCalDat.StGvcOff.UsGyoVal) ) ;			//                            
	}
	
	/*                 */
	if( ( StCalDat.UlGxgVal != 0x00000000 ) && ( StCalDat.UlGxgVal != 0xffffffff )){
		RamWrite32A( gxzoom, StCalDat.UlGxgVal ) ;		//                                       
	}else{
		RamWrite32A( gxzoom, GXGAIN_INI ) ;		//                                               
	}
	
	/*                 */
	if( ( StCalDat.UlGygVal != 0x00000000 ) && ( StCalDat.UlGygVal != 0xffffffff )){
		RamWrite32A( gyzoom, StCalDat.UlGygVal ) ;		//                                       
	}else{
		RamWrite32A( gyzoom, GYGAIN_INI ) ;		//                                               
	}
	
	/*                 */
	if( ((unsigned char)StCalDat.UsOscVal != 0x00) && ((unsigned char)StCalDat.UsOscVal != 0xff) ){
		RegWriteA( OSCSET, (unsigned char)( (unsigned char)StCalDat.UsOscVal | 0x01 ) ) ;		//       
	}else{
		RegWriteA( OSCSET, OSC_INI ) ;						//       
	}
	
	RamWriteA( hxinog, 0x8001 ) ;			//                             
	RamWriteA( hyinog, 0x8001 ) ;			//                             
	
	RegWriteA( STBB 	, 0x0F );							//                                                                 
	
	RegWriteA( LXEQEN 	, 0x45 );			//       
	RegWriteA( LYEQEN 	, 0x45 );			//       

	RamWrite32A( gxistp_1, 0x00000000 );
	RamWrite32A( gyistp_1, 0x00000000 );
	

	//                       
	RamWrite32A( gxi1a_1, 0x38A8A554 ) ;		//      
	RamWrite32A( gxi1b_1, 0xB3E6A3C6 ) ;		//     
	RamWrite32A( gxi1c_1, 0x33E6A3C6 ) ;		//   

	RamWrite32A( gxi1a_a, 0x38A8A554 ) ;		//      
	RamWrite32A( gxi1b_a, 0xB3E6A3C6 ) ;		//     
	RamWrite32A( gxi1c_a, 0x33E6A3C6 ) ;		//   

	RamWrite32A( gxi1a_b, 0x3AAF73A1 ) ;		//    
	RamWrite32A( gxi1b_b, 0xB3E6A3C6 ) ;		//     
	RamWrite32A( gxi1c_b, 0x3F800000 ) ;		//   

	RamWrite32A( gxi1a_c, 0x38A8A554 ) ;		//      
	RamWrite32A( gxi1b_c, 0xB3E6A3C6 ) ;		//     
	RamWrite32A( gxi1c_c, 0x33E6A3C6 ) ;		//   

	//           
	RamWrite32A( gyi1a_1, 0x38A8A554 ) ;		//      
	RamWrite32A( gyi1b_1, 0xB3E6A3C6 ) ;		//     
	RamWrite32A( gyi1c_1, 0x33E6A3C6 ) ;		//   

	RamWrite32A( gyi1a_a, 0x38A8A554 ) ;		//      
	RamWrite32A( gyi1b_a, 0xB3E6A3C6 ) ;		//     
	RamWrite32A( gyi1c_a, 0x33E6A3C6 ) ;		//   

	RamWrite32A( gyi1a_b, 0x3AAF73A1 ) ;		//    
	RamWrite32A( gyi1b_b, 0xB3E6A3C6 ) ;		//     
	RamWrite32A( gyi1c_b, 0x3F800000 ) ;		//   

	RamWrite32A( gyi1a_c, 0x38A8A554 ) ;		//      
	RamWrite32A( gyi1b_c, 0xB3E6A3C6 ) ;		//     
	RamWrite32A( gyi1c_c, 0x33E6A3C6 ) ;		//   

	//                      
	RamWrite32A( gxl4a_1, 0x3F800000 ) ;		//  
	RamWrite32A( gxl4b_1, 0xB9DFB23C ) ;		//     
	RamWrite32A( gxl4c_1, 0x39DFB23C ) ;		//   
	RamWrite32A( gxl4a_a, 0x3F800000 ) ;		//  
	RamWrite32A( gxl4b_a, 0xB9DFB23C ) ;		//     
	RamWrite32A( gxl4c_a, 0x39DFB23C ) ;		//   
	RamWrite32A( gxl4a_b, 0x00000000 ) ;		//        
	RamWrite32A( gxl4b_b, 0xBF800000 ) ;		//     
	RamWrite32A( gxl4c_b, 0x39DFB23C ) ;		//   
	RamWrite32A( gxl4a_c, 0x3F800000 ) ;		//  
	RamWrite32A( gxl4b_c, 0xB9DFB23C ) ;		//     
	RamWrite32A( gxl4c_c, 0x39DFB23C ) ;		//   

	//       
	RamWrite32A( gyl4a_1, 0x3F800000 ) ;		//  
	RamWrite32A( gyl4b_1, 0xB9DFB23C ) ;		//     
	RamWrite32A( gyl4c_1, 0x39DFB23C ) ;		//   
	RamWrite32A( gyl4a_a, 0x3F800000 ) ;		//  
	RamWrite32A( gyl4b_a, 0xB9DFB23C ) ;		//     
	RamWrite32A( gyl4c_a, 0x39DFB23C ) ;		//   
	RamWrite32A( gyl4a_b, 0x00000000 ) ;		//        
	RamWrite32A( gyl4b_b, 0xBF800000 ) ;		//     
	RamWrite32A( gyl4c_b, 0x39DFB23C ) ;		//   
	RamWrite32A( gyl4a_c, 0x3F800000 ) ;		//  
	RamWrite32A( gyl4b_c, 0xB9DFB23C ) ;		//     
	RamWrite32A( gyl4c_c, 0x39DFB23C ) ;		//   

	//                      
	RamWrite32A( gxgyro_1, 0x00000000 ) ;		//        
	RamWrite32A( gxgain_1, 0xBF800000 ) ;		//     
	RamWrite32A( gxistp_1, 0x3F800000 ) ;		//   
	RamWrite32A( gxgyro_a, 0x00000000 ) ;		//        
	RamWrite32A( gxgain_a, 0xBF800000 ) ;		//     
	RamWrite32A( gxistp_a, 0x3F800000 ) ;		//   
	RamWrite32A( gxgyro_b, 0x37EC6C50 ) ;		//      
	RamWrite32A( gxgain_b, 0xBF800000 ) ;		//     
	RamWrite32A( gxistp_b, 0x3F800000 ) ;		//   
	RamWrite32A( gxgyro_c, 0x00000000 ) ;		//        
	RamWrite32A( gxgain_c, 0xBF800000 ) ;		//     
	RamWrite32A( gxistp_c, 0x3F800000 ) ;		//   

	//       
	RamWrite32A( gygyro_1, 0x00000000 ) ;		//        
	RamWrite32A( gygain_1, 0xBF800000 ) ;		//     
	RamWrite32A( gyistp_1, 0x3F800000 ) ;		//   
	RamWrite32A( gygyro_a, 0x00000000 ) ;		//        
	RamWrite32A( gygain_a, 0xBF800000 ) ;		//     
	RamWrite32A( gyistp_a, 0x3F800000 ) ;		//   
	RamWrite32A( gygyro_b, 0x37EC6C50 ) ;		//      
	RamWrite32A( gygain_b, 0xBF800000 ) ;		//     
	RamWrite32A( gyistp_b, 0x3F800000 ) ;		//   
	RamWrite32A( gygyro_c, 0x00000000 ) ;		//        
	RamWrite32A( gygain_c, 0xBF800000 ) ;		//     
	RamWrite32A( gyistp_c, 0x3F800000 ) ;		//   

	/*              */
	AutoGainControlSw( OFF ) ;					/*                            */
	
	DrvSw( ON ) ;								/*                             */
	
//                                             
	RegWriteA( GEQON	, 0x01 );				//                                                
	RegWriteA( GPANFILMOD, 0x01 ) ;				//       
	SetPanTiltMode( ON ) ;						/*             */
	RegWriteA( GPANSTTFRCE, 0x44 ) ;			//       
	RegWriteA( GLSEL, 0x04 ) ;					//       
	
	RegWriteA( GHCHR, 0x11 ) ;					//       
	
	RamWrite32A( gxl2a_2, 0x00000000 ) ;	//         
	RamWrite32A( gxl2b_2, 0x3D829952 ) ;	//                
	RamWrite32A( gyl2a_2, 0x00000000 ) ;	//         
	RamWrite32A( gyl2b_2, 0x3D829952 ) ;	//                

	RamWrite32A( gxl2c_2, 0xBA9CD414 ) ;	//                             
	RamWrite32A( gyl2c_2, 0xBA9CD414 ) ;	//                             
	
	//                                              
	//                                              
	RamWrite32A( gxh1c_2, 0x3F7FFD80 ) ;	//       
	RamWrite32A( gyh1c_2, 0x3F7FFD80 ) ;	//       

	RegWriteA( GPANSTTFRCE, 0x11 ) ;									//       

	return;
}

void	MemClr( unsigned char	*NcTgtPtr, unsigned short	UsClrSiz )
{
	unsigned short	UsClrIdx ;
	
	for ( UsClrIdx = 0 ; UsClrIdx < UsClrSiz ; UsClrIdx++ )
	{
		*NcTgtPtr	= 0 ;
		NcTgtPtr++ ;
	}

	return;
}

int	AccWit( unsigned char UcTrgDat )
{
	unsigned char	UcFlgVal ;
	unsigned char	UcCntPla ;
	UcFlgVal	= 1 ;
	UcCntPla	= 0 ;
	
	do{
		RegReadA( GRACC, &UcFlgVal ) ;
		UcFlgVal	&= UcTrgDat ;
		UcCntPla++ ;
	} while( UcFlgVal && ( UcCntPla < ACCWIT_POLLING_LIMIT_A ) ) ;
	if( UcCntPla == ACCWIT_POLLING_LIMIT_A ) { return OIS_FW_POLLING_FAIL; }
	
	return OIS_FW_POLLING_PASS ;
}

void	AutoGainControlSw( unsigned char UcModeSw )
{
	
	if( UcModeSw == OFF )
	{
		RegWriteA( GADJGANGXMOD, 0x00 ) ;				//                 
		RegWriteA( GADJGANGYMOD, 0x00 ) ;				//                 
		RegWriteA( GADJGANGO,	 0x11 ) ;				//                 
	}
	else
	{
		RegWriteA( GADJGANGO,		0x00 ) ;				//               
		RegWriteA( GADJGANGXMOD, 0xA7 ) ;				//                 
		RegWriteA( GADJGANGYMOD, 0xA7 ) ;				//                 
	}
	
	return;
}

int	ClrGyr( unsigned char UcClrFil , unsigned char UcClrMod )
{
	unsigned char	UcRamClr;
	unsigned char	UcCntPla;
	
	UcCntPla = 0 ;
	
	/*                      */
	RegWriteA( GRAMDLYMOD	, UcClrFil ) ;	//                                          
	
	/*            */
	RegWriteA( GRAMINITON	, UcClrMod ) ;	//                                                      
	
	/*                        */
	do{
		RegReadA( GRAMINITON, &UcRamClr );
		UcRamClr &= 0x03;
		UcCntPla++;
	} while( ( UcRamClr != 0x00 ) && ( UcCntPla < CLRGYR_POLLING_LIMIT_A ) );
	if( UcCntPla == CLRGYR_POLLING_LIMIT_A ) { return OIS_FW_POLLING_FAIL; }
	
	return OIS_FW_POLLING_PASS ;
}

void	DrvSw( unsigned char UcDrvSw )
{
	if( UcDrvSw == ON )	{
		RegWriteA( DRVFC	, 0xE3 );					//                                            
	} else {
		RegWriteA( DRVFC	, 0x00 );					//                          
	}

	return;
}
//                                                                              
//                               
//                                                                              

unsigned char	RtnCen( unsigned char	UcCmdPar )
{
	unsigned char	UcCmdSts ;
	
	
	UcCmdSts	= EXE_END ;
	
	if( !UcCmdPar ) {										//              
		StbOnn() ;
	} else if( UcCmdPar == 0x01 ) {							//                 
		
		SrvCon( X_DIR, ON ) ;								//                
		SrvCon( Y_DIR, OFF ) ;
	} else if( UcCmdPar == 0x02 ) {							//                 
		
		SrvCon( X_DIR, OFF ) ;								//                
		SrvCon( Y_DIR, ON ) ;
	}
	
	return( UcCmdSts ) ;
}

void	OisEna( void ) //      
{
	//         
	GyrCon( ON ) ;

	return;
}

void	OisOff( void ) //       
{
	GyrCon( OFF ) ;
	RamWriteA( lxggf, 0x0000 ) ;			//       
	RamWriteA( lyggf, 0x0000 ) ;			//       

	return;
}

void	S2cPro( unsigned char uc_mode )
{
	RamWrite32A( gxh1c, 0x3F7FFD00 ) ;										//       
	RamWrite32A( gyh1c, 0x3F7FFD00 ) ;										//       
	
	if( uc_mode == 1 ) {														//            
		//                     
//                                                  
		RegWriteA( G2NDCEFON0, 0x03 ) ;											//                        
	} else {																	//          
		//            
//                                                  
		RegWriteA( G2NDCEFON0, 0x00 ) ;											//       
	}
	
	return;
}

//                                                                                
//                        
//                   
//                    
//                                                
//                                                      
//                                                                                
void StbOnn( void )
{
	unsigned char	UcRegValx,UcRegValy;					//                

	RegReadA( LXEQEN, &UcRegValx ) ;				/*        */
	RegReadA( LYEQEN, &UcRegValy ) ;				/*        */
	if( (( UcRegValx & 0x80 ) != 0x80 ) && (( UcRegValy & 0x80 ) != 0x80 ))
	{
		RegWriteA( SSSEN,  0x88 ) ;				//                           
		
		SrvCon( X_DIR, ON ) ;
		SrvCon( Y_DIR, ON ) ;
	}
	else
	{
		SrvCon( X_DIR, ON ) ;
		SrvCon( Y_DIR, ON ) ;
	}

	return;
}

void	SrvCon( unsigned char	UcDirSel, unsigned char	UcSwcCon )
{
	if( UcSwcCon ) {
		if( !UcDirSel ) {									//            
			RegWriteA( LXEQEN, 0xC5 ) ;				//                 
		} else {											//            
			RegWriteA( LYEQEN, 0xC5 ) ;				//                 
		}
	} else {
		if( !UcDirSel ) {									//            
			RegWriteA( LXEQEN, 0x45 ) ;				//                  
			RamWriteA( LXDODAT, 0x0000 ) ;			//       
		} else {											//            
			RegWriteA( LYEQEN, 0x45 ) ;				//                  
			RamWriteA( LYDODAT, 0x0000 ) ;			//       
		}
	}

	return;
}

void	GyrCon( unsigned char	UcGyrCon )
{
	//                   
	RegWriteA( GSHTON, 0x00 ) ;												//       
	
	if( UcGyrCon ) {														//        
		RamWriteA( lxggf, 0x7fff ) ;										//       
		RamWriteA( lyggf, 0x7fff ) ;										//       
		RamWrite32A( gxi1b_1, 0xBF800000 ) ;								//     
		RamWrite32A( gyi1b_1, 0xBF800000 ) ;								//     

//                                                    
//                        
//                        
//                                                    
		RamWrite32A( gxi1b_1, 0xB3E6A3C6 ) ;								//     
		RamWrite32A( gyi1b_1, 0xB3E6A3C6 ) ;								//     
		RegWriteA( GPANSTTFRCE, 0x00 ) ;									//       
		AutoGainControlSw( ON ) ;											/*                           */
	} else {																//         
		/*                */
		AutoGainControlSw( OFF ) ;											/*                            */
	}
	
	//                                  
	S2cPro( OFF ) ; //                

	return;
}

void	SetPanTiltMode( unsigned char UcPnTmod )
{
	switch ( UcPnTmod ) {
		case OFF :
			RegWriteA( GPANON, 0x00 ) ;			//                                 
			break ;
		case ON :
			RegWriteA( GPANON, 0x11 ) ;			//                                
			break ;
	}

	return;
}

void	VSModeConvert( unsigned char UcVstmod )
{
	if( UcVstmod ) {								//                 
		RamWrite32A( gxl2a_2, 0x00000000 ) ;	//         
		RamWrite32A( gxl2b_2, 0x3E06FD65 ) ;	//                
		RamWrite32A( gyl2a_2, 0x00000000 ) ;	//         
		RamWrite32A( gyl2b_2, 0x3E06FD65 ) ;	//                

		RamWrite32A( gxl2c_2, 0xBB894028 ) ;	//                             
		RamWrite32A( gyl2c_2, 0xBB894028 ) ;	//                             

		RegWriteA( GPANSTT31JUG0, 	0x00 );		//       
		RegWriteA( GPANSTT31JUG1, 	0x00 );		//       
		RegWriteA( GPANSTT41JUG0, 	0x07 );		//       
		RegWriteA( GPANSTT41JUG1, 	0x00 );		//       
		RegWriteA( GPANSTT13JUG0, 	0x00 );		//       
		RegWriteA( GPANSTT13JUG1, 	0x07 );		//       
		RegWriteA( GPANSTT43JUG0, 	0x00 );		//       
		RegWriteA( GPANSTT43JUG1, 	0x07 );		//       
		RegWriteA( GPANSTT34JUG0, 	0x01 );		//       
		RegWriteA( GPANSTT34JUG1, 	0x00 );		//       

		RegWriteA( GPANSTTXXXTH,	0xF0 );		//       
		RamWrite32A( gxi1a_1, 0x38A8A554 ) ;	//      
		RamWrite32A( gxi1b_1, 0xB3E6A3C6 ) ;	//     
		RamWrite32A( gxi1c_1, 0x33E6A3C6 ) ;	//   
		RamWrite32A( gxi1a_b, 0x38A8A554 ) ;	//      
		RamWrite32A( gxi1b_b, 0xB3E6A3C6 ) ;	//     
		RamWrite32A( gxi1c_b, 0x33E6A3C6 ) ;	//   
		RamWrite32A( gxi1a_c, 0x3AAF73A1 ) ;	//    
		RamWrite32A( gxi1b_c, 0xB3E6A3C6 ) ;	//     
		RamWrite32A( gxi1c_c, 0x3F800000 ) ;	//   
		RamWrite32A( gyi1a_1, 0x38A8A554 ) ;	//      
		RamWrite32A( gyi1b_1, 0xB3E6A3C6 ) ;	//     
		RamWrite32A( gyi1c_1, 0x33E6A3C6 ) ;	//   
		RamWrite32A( gyi1a_b, 0x38A8A554 ) ;	//      
		RamWrite32A( gyi1b_b, 0xB3E6A3C6 ) ;	//     
		RamWrite32A( gyi1c_b, 0x33E6A3C6 ) ;	//   
		RamWrite32A( gyi1a_c, 0x3AAF73A1 ) ;	//    
		RamWrite32A( gyi1b_c, 0xB3E6A3C6 ) ;	//     
		RamWrite32A( gyi1c_c, 0x3F800000 ) ;	//   
	} else {										//                   
		RamWrite32A( gxl2a_2, 0x00000000 ) ;	//         
		RamWrite32A( gxl2b_2, 0x3D829952 ) ;	//                
		RamWrite32A( gyl2a_2, 0x00000000 ) ;	//         
		RamWrite32A( gyl2b_2, 0x3D829952 ) ;	//                

		RamWrite32A( gxl2c_2, 0xBA9CD414 ) ;	//                             
		RamWrite32A( gyl2c_2, 0xBA9CD414 ) ;	//                             

		RegWriteA( GPANSTT31JUG0, 	0x01 );		//       
		RegWriteA( GPANSTT31JUG1, 	0x00 );		//       
		RegWriteA( GPANSTT41JUG0, 	0x00 );		//       
		RegWriteA( GPANSTT41JUG1, 	0x00 );		//       
		RegWriteA( GPANSTT13JUG0, 	0x00 );		//       
		RegWriteA( GPANSTT13JUG1, 	0x07 );		//       
		RegWriteA( GPANSTT43JUG0, 	0x00 );		//       
		RegWriteA( GPANSTT43JUG1, 	0x00 );		//       
		RegWriteA( GPANSTT34JUG0, 	0x00 );		//       
		RegWriteA( GPANSTT34JUG1, 	0x00 );		//       
		RegWriteA( GPANSTTXXXTH,	0x0F );		//       
		RamWrite32A( gxi1a_1, 0x38A8A554 ) ;		//      
		RamWrite32A( gxi1b_1, 0xB3E6A3C6 ) ;		//     
		RamWrite32A( gxi1c_1, 0x33E6A3C6 ) ;		//   
		RamWrite32A( gxi1a_b, 0x3AAF73A1 ) ;		//    
		RamWrite32A( gxi1b_b, 0xB3E6A3C6 ) ;		//     
		RamWrite32A( gxi1c_b, 0x3F800000 ) ;		//   
		RamWrite32A( gxi1a_c, 0x38A8A554 ) ;		//      
		RamWrite32A( gxi1b_c, 0xB3E6A3C6 ) ;		//     
		RamWrite32A( gxi1c_c, 0x33E6A3C6 ) ;		//   
		RamWrite32A( gyi1a_1, 0x38A8A554 ) ;		//      
		RamWrite32A( gyi1b_1, 0xB3E6A3C6 ) ;		//     
		RamWrite32A( gyi1c_1, 0x33E6A3C6 ) ;		//   
		RamWrite32A( gyi1a_b, 0x3AAF73A1 ) ;		//    
		RamWrite32A( gyi1b_b, 0xB3E6A3C6 ) ;		//     
		RamWrite32A( gyi1c_b, 0x3F800000 ) ;		//   
		RamWrite32A( gyi1a_c, 0x38A8A554 ) ;		//      
		RamWrite32A( gyi1b_c, 0xB3E6A3C6 ) ;		//     
		RamWrite32A( gyi1c_c, 0x33E6A3C6 ) ;		//   
	}
	return;
}

void WitTim( unsigned short UsWitTim )
{
   usleep(UsWitTim*1000);
}

//                                                                              
//                                 
//                                                                              

#define		HALL_ADJ		0
#define		LOOPGAIN		1
#define		THROUGH			2
#define		NOISE			3

//                                                                                
//                        
//                   
//                    
//                                        
//                                                      
//                                                                                
#define	INITVAL		0x0000

int BsyWit(unsigned short UsTrgAdr, unsigned char UcTrgDat)
{
	unsigned char UcFlgVal;
	unsigned char UcCntPla;

	RegWriteA(UsTrgAdr, UcTrgDat);

	UcFlgVal = 1;
	UcCntPla = 0;

	do {
		RegReadA(FLGM, &UcFlgVal);
		UcFlgVal &= 0x40;
		UcCntPla ++ ;
	} while ( UcFlgVal && ( UcCntPla < BSYWIT_POLLING_LIMIT_A));
	if (UcCntPla == BSYWIT_POLLING_LIMIT_A) { return OIS_FW_POLLING_FAIL; }
	
	return OIS_FW_POLLING_PASS;
}



short	GenMes( unsigned short	UsRamAdd, unsigned char	UcMesMod )
{
	short	SsMesRlt = 0;

	RegWriteA( MS1INADD, ( unsigned char )( UsRamAdd & 0x00ff ) ) ;	//                           

	if( !UcMesMod ) {
		RegWriteA( MSMPLNSH, 0x03 ) ;								//       
		RegWriteA( MSMPLNSL, 0xFF ) ;								//                          
		BsyWit( MSMA, 0x01 ) ;											//                        
	} else {
		RegWriteA( MSMPLNSH, 0x00 ) ;								//       
		RegWriteA( MSMPLNSL, 0x03 ) ;								//                       
		BsyWit( MSMA, 0x22 ) ;											//                        
	}

	RegWriteA( MSMA, 0x00 ) ;										//                     

	RamReadA( MSAV1, ( unsigned short * )&SsMesRlt ) ;				//       

	return( SsMesRlt ) ;
}

void	MesFil( unsigned char	UcMesMod )
{
	if( !UcMesMod ) {								//                        
		//                        
		RamWriteA( ms1aa, 0x0285 ) ;		//                
		RamWriteA( ms1ab, 0x0285 ) ;		//       
		RamWriteA( ms1ac, 0x7AF5 ) ;		//       
		RamWriteA( ms1ad, 0x0000 ) ;		//       
		RamWriteA( ms1ae, 0x0000 ) ;		//       
		RamWriteA( ms1ba, 0x7FFF ) ;		//               
		RamWriteA( ms1bb, 0x0000 ) ;		//       
		RamWriteA( ms1bc, 0x0000 ) ;		//       
		RamWriteA( ms1bd, 0x0000 ) ;		//       
		RamWriteA( ms1be, 0x0000 ) ;		//       

		RegWriteA( MSF1SOF, 0x00 ) ;		//       
		
		//                        
		RamWriteA( ms2aa, 0x0285 ) ;		//                
		RamWriteA( ms2ab, 0x0285 ) ;		//       
		RamWriteA( ms2ac, 0x7AF5 ) ;		//       
		RamWriteA( ms2ad, 0x0000 ) ;		//       
		RamWriteA( ms2ae, 0x0000 ) ;		//       
		RamWriteA( ms2ba, 0x7FFF ) ;		//               
		RamWriteA( ms2bb, 0x0000 ) ;		//       
		RamWriteA( ms2bc, 0x0000 ) ;		//       
		RamWriteA( ms2bd, 0x0000 ) ;		//       
		RamWriteA( ms2be, 0x0000 ) ;		//       

		RegWriteA( MSF2SOF, 0x00 ) ;		//       
		
	} else if( UcMesMod == LOOPGAIN ) {				//                 
		//                        
		RamWriteA( ms1aa, 0x0F21 ) ;		//                 
		RamWriteA( ms1ab, 0x0F21 ) ;		//       
		RamWriteA( ms1ac, 0x61BD ) ;		//       
		RamWriteA( ms1ad, 0x0000 ) ;		//       
		RamWriteA( ms1ae, 0x0000 ) ;		//       
		RamWriteA( ms1ba, 0x7F7D ) ;		//               
		RamWriteA( ms1bb, 0x8083 ) ;		//       
		RamWriteA( ms1bc, 0x7EF9 ) ;		//       
		RamWriteA( ms1bd, 0x0000 ) ;		//       
		RamWriteA( ms1be, 0x0000 ) ;		//       

		RegWriteA( MSF1SOF, 0x00 ) ;		//       
		
		//                        
		RamWriteA( ms2aa, 0x0F21 ) ;		//                 
		RamWriteA( ms2ab, 0x0F21 ) ;		//       
		RamWriteA( ms2ac, 0x61BD ) ;		//       
		RamWriteA( ms2ad, 0x0000 ) ;		//       
		RamWriteA( ms2ae, 0x0000 ) ;		//       
		RamWriteA( ms2ba, 0x7F7D ) ;		//               
		RamWriteA( ms2bb, 0x8083 ) ;		//       
		RamWriteA( ms2bc, 0x7EF9 ) ;		//       
		RamWriteA( ms2bd, 0x0000 ) ;		//       
		RamWriteA( ms2be, 0x0000 ) ;		//       

		RegWriteA( MSF2SOF, 0x00 ) ;		//       
		
	} else if( UcMesMod == THROUGH ) {				//            
		//                        
		RamWriteA( ms1aa, 0x7FFF ) ;		//               
		RamWriteA( ms1ab, 0x0000 ) ;		//       
		RamWriteA( ms1ac, 0x0000 ) ;		//       
		RamWriteA( ms1ad, 0x0000 ) ;		//       
		RamWriteA( ms1ae, 0x0000 ) ;		//       
		RamWriteA( ms1ba, 0x7FFF ) ;		//               
		RamWriteA( ms1bb, 0x0000 ) ;		//       
		RamWriteA( ms1bc, 0x0000 ) ;		//       
		RamWriteA( ms1bd, 0x0000 ) ;		//       
		RamWriteA( ms1be, 0x0000 ) ;		//       

		RegWriteA( MSF1SOF, 0x00 ) ;		//       
		
		//                        
		RamWriteA( ms2aa, 0x7FFF ) ;		//               
		RamWriteA( ms2ab, 0x0000 ) ;		//       
		RamWriteA( ms2ac, 0x0000 ) ;		//       
		RamWriteA( ms2ad, 0x0000 ) ;		//       
		RamWriteA( ms2ae, 0x0000 ) ;		//       
		RamWriteA( ms2ba, 0x7FFF ) ;		//               
		RamWriteA( ms2bb, 0x0000 ) ;		//       
		RamWriteA( ms2bc, 0x0000 ) ;		//       
		RamWriteA( ms2bd, 0x0000 ) ;		//       
		RamWriteA( ms2be, 0x0000 ) ;		//       

		RegWriteA( MSF2SOF, 0x00 ) ;		//       
		
	} else if( UcMesMod == NOISE ) {				//                         
	
		RamWriteA( ms1aa, 0x04F3 ) ;		//                
		RamWriteA( ms1ab, 0x04F3 ) ;		//       
		RamWriteA( ms1ac, 0x761B ) ;		//       
		RamWriteA( ms1ad, 0x0000 ) ;		//       
		RamWriteA( ms1ae, 0x0000 ) ;		//       
		RamWriteA( ms1ba, 0x04F3 ) ;		//                
		RamWriteA( ms1bb, 0x04F3 ) ;		//       
		RamWriteA( ms1bc, 0x761B ) ;		//       
		RamWriteA( ms1bd, 0x0000 ) ;		//       
		RamWriteA( ms1be, 0x0000 ) ;		//       
		
		RegWriteA( MSF1SOF, 0x00 ) ;		//       

		RamWriteA( ms2aa, 0x04F3 ) ;		//                
		RamWriteA( ms2ab, 0x04F3 ) ;		//       
		RamWriteA( ms2ac, 0x761B ) ;		//       
		RamWriteA( ms2ad, 0x0000 ) ;		//       
		RamWriteA( ms2ae, 0x0000 ) ;		//       
		RamWriteA( ms2ba, 0x04F3 ) ;		//                
		RamWriteA( ms2bb, 0x04F3 ) ;		//       
		RamWriteA( ms2bc, 0x761B ) ;		//       
		RamWriteA( ms2bd, 0x0000 ) ;		//       
		RamWriteA( ms2be, 0x0000 ) ;		//       
		
		RegWriteA( MSF2SOF, 0x00 ) ;		//       
	}
}

int16_t lgit_convert_float32(uint32_t float32, uint8_t len);


unsigned char	TneGvc( uint8_t flag )
{
	unsigned char	UcGvcSts ;
	unsigned short UsGxoVal, UsGyoVal;
	int16_t oldx, oldy;

	UcGvcSts	= EXE_END ;

	{
		uint32_t gyrox, gyroy = 0;
		RamRead32A(GXADZ, &gyrox);
		RamRead32A(GYADZ, &gyroy);
		oldx = abs(lgit_convert_float32(gyrox,18));
		oldy = abs(lgit_convert_float32(gyroy,18));
		CDBG("%s old gxadz %x, %x \n",__func__,oldx, oldy);
		
		if ( oldx < 262*2 && oldy < 262*2)
		{
			CDBG("%s no need to adjust \n",__func__);		
			return UcGvcSts;
		}
	}

	
	//                                        
	//                 
	RamWriteA( ADGXOFF, 0x0000 ) ;	//       
	RamWriteA( ADGYOFF, 0x0000 ) ;	//       
	RegWriteA( IZAH,	(unsigned char)(INITVAL >> 8) ) ;	//                             
	RegWriteA( IZAL,	(unsigned char)INITVAL ) ;			//                            
	RegWriteA( IZBH,	(unsigned char)(INITVAL >> 8) ) ;	//                             
	RegWriteA( IZBL,	(unsigned char)INITVAL ) ;			//                            
	
	RegWriteA( GDLYMON10, 0xF5 ) ;		//                        
	RegWriteA( GDLYMON11, 0x01 ) ;		//                        
	RegWriteA( GDLYMON20, 0xF5 ) ;		//                        
	RegWriteA( GDLYMON21, 0x00 ) ;		//                        
	MesFil( THROUGH ) ;
	RegWriteA( MSF1EN, 0x01 ) ;			//                                     
	//        
	//  
	//        
	if (BsyWit( DLYCLR2, 0x80 ) != OIS_FW_POLLING_PASS) {return OIS_FW_POLLING_FAIL ;}			//                                       
	UsGxoVal = (unsigned short)GenMes( GYRMON1, 0 );		//                                 
	RegWriteA( IZAH, (unsigned char)(UsGxoVal >> 8) ) ;	//                             
	RegWriteA( IZAL, (unsigned char)(UsGxoVal) ) ;		//                            
	//        
	//  
	//        
	if (BsyWit( DLYCLR2, 0x80 ) != OIS_FW_POLLING_PASS) {return OIS_FW_POLLING_FAIL;}			//                                       
	UsGyoVal = (unsigned short)GenMes( GYRMON2, 0 );		//                                 

	RegWriteA( IZBH, (unsigned char)(UsGyoVal >> 8) ) ;	//                             
	RegWriteA( IZBL, (unsigned char)(UsGyoVal) ) ;		//                            

	
	RegWriteA( MSF1EN, 0x00 ) ;			//                                      

	if( ( ( short )UsGxoVal > ( short )GYROFF_HIGH ) ||
		( ( short )UsGxoVal < ( short )GYROFF_LOW ) ) {
		UcGvcSts	= EXE_GYRADJ ;
	}

	if( ( ( short )UsGyoVal > ( short )GYROFF_HIGH ) ||
		( ( short )UsGyoVal < ( short )GYROFF_LOW ) ) {
		UcGvcSts	= EXE_GYRADJ ;
	}

	{
	uint32_t gyrox, gyroy = 0;
	int16_t newx, newy;
	
	RamRead32A(GXADZ, &gyrox);
	RamRead32A(GYADZ, &gyroy);
	newx = abs(lgit_convert_float32(gyrox,18));
	newy = abs(lgit_convert_float32(gyroy,18));
	CDBG("%s new gxadz %x, %x \n",__func__,newx, newy);

	if ( newx > 262*2 || newy > 262*2 || newx > oldx || newy > oldy )
	{ UcGvcSts = EXE_GYRADJ; }
	}

	if (UcGvcSts != EXE_GYRADJ)
	{
		CDBG("%s : gyro original : %x, %x \n",__func__, StCalDat.StGvcOff.UsGxoVal, StCalDat.StGvcOff.UsGyoVal);
		CDBG("%s : gyro result : %x, %x \n",__func__, UsGxoVal, UsGyoVal);
		if (flag == OIS_VER_CALIBRATION)
		{
			ois_i2c_e2p_write(GYRO_AD_OFFSET_X, 0xFFFF & UsGxoVal, 2);
			usleep(10000);
			ois_i2c_e2p_write(GYRO_AD_OFFSET_Y, 0xFFFF & UsGyoVal, 2);
		}
	}
	
	return( UcGvcSts ) ;
}
//                                                                                   
//                                     
//                                                                                   
#define     __MEASURE_LOOPGAIN      0x00

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
void SetSineWave( unsigned char UcJikuSel , unsigned char UcMeasMode )
{
	unsigned char   UcSWFC1[]   = { 0x7D/*     */ , 0x0F/*    */ } ,            //                                           
                    UcSWFC2[]   = { 0x00/*     */ , 0x00/*    */ } ;            //                                           

    unsigned char   UcSwSel[2][2] = { { 0x80 , 0x40 } ,                         //                  
                                      { 0x00 , 0x00 }                           //                    
                                    };

    UcMeasMode &= 0x01;
    UcJikuSel  &= 0x01;

    /*                     */
    RamWriteA( WAVXO , 0x0000 );                                                //       
    RamWriteA( WAVYO , 0x0000 );                                                //       

    /*                        */
    //                                                                                     
    //                                                                                     
	RamWriteA( wavxg , 0x3000 );                                                //       
    RamWriteA( wavyg , 0x3000 );                                                //       

    /*               */
    //                                                    
    //                                                    
    //                                                    
    RegWriteA( SWFC1 , UcSWFC1[UcMeasMode] );                                   //                                   
    RegWriteA( SWFC2 , UcSWFC2[UcMeasMode] );                                   //                                                         
    RegWriteA( SWFC3 , 0x00 );                                                  //                                   
    RegWriteA( SWFC4 , 0x00 );                                                  //                                   
    RegWriteA( SWFC5 , 0x00 );                                                  //                        

    /*                         */
    RegWriteA( SWSEL , UcSwSel[UcMeasMode][UcJikuSel] );                        //                                                                                        

    /*                                        */
    if( !UcMeasMode )       //               
    {
        RegWriteA( SINXADD , 0x00 );                                            //                         
        RegWriteA( SINYADD , 0x00 );                                            //                         
    }
    else if( !UcJikuSel )   //                        
    {
        RegWriteA( SINXADD , (unsigned char)LXDODAT );                          //                         
        RegWriteA( SINYADD , 0x00 );                                            //                         
    }
    else                    //                        
    {
        RegWriteA( SINXADD , 0x00 );                                            //                         
        RegWriteA( SINYADD , (unsigned char)LYDODAT );                          //                         
    }
}

//                                                                              
//                                 
//                      
//                      
//                                  
//                                                                              
//                                                                              
void StartSineWave( void )
{
    /*                 */
    RegWriteA( SWEN , 0x80 );                                                   //                                                               
}

//                                                                              
//                                
//                      
//                      
//                                 
//                                                                              
//                                                                              
void StopSineWave( void )
{
    /*                         */
    RegWriteA( SWSEL   , 0x00 );                                                //                                                                                        
    RegWriteA( SINXADD , 0x00 );                                                //                         
    RegWriteA( SINYADD , 0x00 );                                                //                         

    /*                */
    RegWriteA( SWEN  , 0x00 );                                                  //                                                               
}


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
void SetMeasFil( unsigned char UcJikuSel , unsigned char UcMeasMode , unsigned char UcFilSel )
{
    unsigned short  UsIn1Add[2][2] = { { LXC1   , LYC1   } ,                    //                  
                                       { ADHXI0 , ADHYI0 }                      //                    
                                     } ,
                    UsIn2Add[2][2] = { { LXC2   , LYC2   } ,                    //                  
                                       { 0x0000 , 0x0000 }                      //                    
                                     } ;

    /*                                */
    UcJikuSel  &= 0x01;
    UcMeasMode &= 0x01;
    if( UcFilSel > NOISE ) UcFilSel = THROUGH;
	
	MesFil( UcFilSel ) ;					/*                    */

    RegWriteA( MS1INADD , (unsigned char)UsIn1Add[UcMeasMode][UcJikuSel] );     //       
    RegWriteA( MS1OUTADD, 0x00 );                                               //       


    RegWriteA( MS2INADD , (unsigned char)UsIn2Add[UcMeasMode][UcJikuSel] );     //       
    RegWriteA( MS2OUTADD, 0x00 );                                               //       

    /*                                  */
    //                                                    
    //                                             
    //                                                    
    RegWriteA( MSFDS , 0x00 );                                                  //       
}

//                                                                              
//                                
//                      
//                      
//               
//                                                                              
//                                                                              
void StartMeasFil( void )
{
    /*                        */
    RegWriteA( MSF1EN , 0x01 );                                                 //                                                          
    RegWriteA( MSF2EN , 0x01 );                                                 //                                                          
}

//                                                                              
//                               
//                      
//                      
//               
//                                                                              
//                                                                              
void StopMeasFil( void )
{
    /*                        */
    RegWriteA( MSF1EN , 0x00 );                                                 //                                                          
    RegWriteA( MSF2EN , 0x00 );                                                 //                                                          
}

//                                                                                
//                             
//                    
//                                          
//                                                 
//                                                     
//                                                                                
void	IntegralMes( unsigned char UcSinFrq )
{
	RegWriteA( MSMA, 0x03 ) ;							//                                    

	if ( UcSinFrq == 0 ){
		WitTim( 200 ) ;
	}else if ( UcSinFrq == 1 ){
		WitTim( 100 ) ;
	}else{
		WitTim( 60 ) ;
	}

	RegWriteA( MSMA, 0x00 ) ;							//                   

}

//                                                                                
//                             
//                              
//                                 
//                                                     
//                                                     
//                                                                                
unsigned short	GetIntegral( unsigned char	UcDirSel )
{
	unsigned short	UsAmpVal ;

	if( !UcDirSel ) {
		RamReadA( MSCAP, &UsAmpVal ) ;				//       
	} else {
		RamReadA( MSCAP2, &UsAmpVal ) ;				//       
	}

	return( UsAmpVal ) ;
}

//                                                                                
//                             
//                                     
//                                               
//                                                              
//                                                     
//                                                                                
short	CalAmpRatio( unsigned short	UsSource1, unsigned short	UsSource2 )
{
	short	SsAmpRatio ;

	SsAmpRatio	= ( short )( ( ( unsigned long )UsSource1 * 100 ) / UsSource2 ) ;

	return( SsAmpRatio ) ;
}

//                                                                              
//                              
//                                                  
//                                   
//               
//                                                                             
//                                                                              
unsigned char	 SrvGainMes( unsigned char	UcJikuSel )
{
	unsigned short  UsSineAdd[]	= { lxxg   , lyxg   } ;
	unsigned short  UsSineGan[]	= { 0x2AF5 , 0x2AF5 } ;
	unsigned char	UcSWFC1[]	= { 0x0B, 0x7D, 0x42 } ;	//                   
	unsigned char	UcSWFC2[]	= { 0x00, 0x00, 0x00 } ;	//              
	short			SsRatioSh[]	= { 345, 141, 50 } ;
	unsigned char	UcJudgeSts1, UcJudgeSts2, UcJudgeSts3 ;
	unsigned char	UcJudgeSts = SUCCESS ;
	unsigned char	UcSinFrq ;
	unsigned short	UsMSCAP1, UsMSCAP2 ;
	short			SsCalRatio ;

	UcJikuSel	&= 0x01 ;
	UcJudgeSts1	= 0x00 ;
	UcJudgeSts2	= 0x00 ;
	UcJudgeSts3	= 0x00 ;

	/*               */
	SetSineWave( UcJikuSel, __MEASURE_LOOPGAIN ) ;
	RamWriteA( wavxg , 0x7FFF );                                                //       
    RamWriteA( wavyg , 0x7FFF );                                                //       
    
	/*                  */
	RamWriteA( UsSineAdd[ UcJikuSel ], UsSineGan[ UcJikuSel ] ) ;				//                              

	/*                    */
	SetMeasFil( UcJikuSel, __MEASURE_LOOPGAIN, THROUGH ) ;

	/*                       */
	StartMeasFil() ;

	/*                 */
	StartSineWave() ;
	WitTim( 100 ) ;
	for( UcSinFrq = 0 ; UcSinFrq < 3 ; UcSinFrq++ )
	{
		RegWriteA( SWFC1, UcSWFC1[ UcSinFrq ] ) ;								//                                
		RegWriteA( SWFC2, UcSWFC2[ UcSinFrq ] ) ;								//       

		RegWriteA( DLYCLR2, 0xC0 ) ;
		WitTim( 50 ) ;
		IntegralMes( UcSinFrq ) ;
		UsMSCAP1	= GetIntegral( 0 ) ;
		UsMSCAP2	= GetIntegral( 1 ) ;
		SsCalRatio	= CalAmpRatio( UsMSCAP1, UsMSCAP2 ) ;
		CDBG("%s, %d=> %x, %x, ratio=%x (%x) \n", 
			__func__, UcSinFrq, UsMSCAP1, UsMSCAP2, SsCalRatio, SsRatioSh[ UcSinFrq ]);
		
		if( !UcSinFrq ) {
			if( SsCalRatio < SsRatioSh[ UcSinFrq ] ) {
				if( UcJikuSel == X_DIR){
					UcJudgeSts1	= EXE_XFRQ1 ;
				}else if( UcJikuSel == Y_DIR ){
					UcJudgeSts1	= EXE_YFRQ1 ;
				}
			}
		} else if( UcSinFrq == 1 ) {
			SsCalRatio	&= 0x7FFF ;
			if( SsCalRatio > SsRatioSh[ UcSinFrq ] ) {
				if( UcJikuSel == X_DIR){
					UcJudgeSts2	= EXE_XFRQ2 ;
				}else if( UcJikuSel == Y_DIR ){
					UcJudgeSts2	= EXE_YFRQ2 ;
				}
			}
		} else {
			if( SsCalRatio > SsRatioSh[ UcSinFrq ] ) {
				if( UcJikuSel == X_DIR){
					UcJudgeSts3	= EXE_XFRQ3 ;
				}else if( UcJikuSel == Y_DIR ){
					UcJudgeSts3	= EXE_YFRQ3 ;
				}
			}
		}
	}

	/*                */
	RamWriteA( UsSineAdd[UcJikuSel] , 0x0000 );                                 //                              

	/*                */
	StopSineWave();

	/*                     */
	StopMeasFil();

	UcJudgeSts	= UcJudgeSts1 | UcJudgeSts2 | UcJudgeSts3 ;

	return( UcJudgeSts ) ;
}

//                                                                     

void EnsureWrite(uint16_t addr, int16_t data, int8_t trial)
{
	int i = 0;
	int16_t written;
	do
	{
		RamWriteA(addr, data & 0xFFFF);
		RamReadA(addr, &written);
	} while ((data !=  written) && ( i < trial ));
}

static struct msm_ois_fn_t lgit_ois_func_tbl;

int32_t	lgit_ois_on( enum ois_ver_t ver )
{
	int32_t rc = OIS_SUCCESS;
	CDBG("%s, enter %s\n", __func__,LAST_UPDATE);
	switch(ver)
	{
		case OIS_VER_RELEASE:
		{
			usleep(20000);
			rc = IniSet();
			if (rc < 0) { return rc;}
			usleep(5000);
			RtnCen(0);
			usleep(100000);
			
		}
		break;	
		case OIS_VER_DEBUG:
		case OIS_VER_CALIBRATION:
		{
			int i=0;
			usleep(20000);
			rc = IniSet();
			if (rc < 0) { return rc;}
			usleep(5000);
			rc = OIS_INIT_GYRO_ADJ_FAIL;	
			do
			{
				if (TneGvc(ver) == EXE_END) 
				{
					rc = OIS_SUCCESS;  
					break; 
				}
			} while (i++ < 5);
			
			usleep(5000);
			RtnCen(0);

			if ( (SrvGainMes(X_DIR) | SrvGainMes(Y_DIR)) != 0)
			{
				rc |= OIS_INIT_SRV_GAIN_FAIL;
			}
			
			OisOff();

			EnsureWrite(HXSEPT1, 0x0000, 5);
			EnsureWrite(HYSEPT1, 0x0000, 5);

			usleep(50000);
		}
		break;
	}	
	lgit_ois_func_tbl.ois_cur_mode = OIS_MODE_CENTERING_ONLY;

	if (StCalDat.UsVerDat != 0x0204)
	{
		CDBG("%s, old module %x \n", __func__,StCalDat.UsVerDat);
		rc |= OIS_INIT_OLD_MODULE;
	}

	CDBG("%s, exit \n", __func__);
	return rc;
}

int32_t	lgit_ois_off( void )
{
	int16_t i;
	int16_t hallx,hally;

	CDBG("%s\n", __func__);
	
	OisOff();
	usleep(1000);

	RamWriteA( LXOLMT, 0x1200 );
	usleep(2000);
	RamReadA(HXTMP, &hallx);
	RamWriteA(HXSEPT1, -hallx);

	RamWriteA( LYOLMT, 0x1200 );	
	usleep(2000);
	RamReadA(HYTMP, &hally);
	RamWriteA(HYSEPT1, -hally);

	for (i=0x1100; i>=0x0000;  i-=0x100)
	{
		RamWriteA( LXOLMT,	i );	
		RamWriteA( LYOLMT,	i );
		usleep(2000);
	}

	SrvCon( X_DIR, OFF ) ;
	SrvCon( Y_DIR, OFF ) ;

	CDBG("%s, exit\n", __func__);

	return OIS_SUCCESS;
}

int32_t lgit_ois_mode(enum ois_mode_t data)
{
	int cur_mode = lgit_ois_func_tbl.ois_cur_mode;
	CDBG("%s:%d\n", __func__,data);

	if (cur_mode == data)
	{
		return OIS_SUCCESS;
	}
	
	switch(cur_mode)
	{
		case OIS_MODE_PREVIEW_CAPTURE :
		case OIS_MODE_CAPTURE : 
		case OIS_MODE_VIDEO :
			OisOff();
			break;
		case OIS_MODE_CENTERING_ONLY :
			break;
		case OIS_MODE_CENTERING_OFF:
			RtnCen(0); 
			break;
	}
	
	switch(data)
	{
		case OIS_MODE_PREVIEW_CAPTURE :
		case OIS_MODE_CAPTURE : 	
			VSModeConvert(OFF);
			OisEna() ;
			break;			
		case OIS_MODE_VIDEO :
			VSModeConvert(ON);
			OisEna() ;
			break;
		case OIS_MODE_CENTERING_ONLY :
			break;
		case OIS_MODE_CENTERING_OFF:
			SrvCon( X_DIR, OFF ) ;
    		SrvCon( Y_DIR, OFF ) ;
			break;
	}

	lgit_ois_func_tbl.ois_cur_mode = data;

	return OIS_SUCCESS;
}

int16_t lgit_convert_float32(uint32_t float32, uint8_t len)
{
	uint8_t sgn = float32 >> 31;
	uint16_t exp = 0xFF & (float32 >> 23);
	uint32_t frc = (0x7FFFFF & (float32)) | 0x800000;

	if (exp > 127) {
		frc = frc << (exp-127);
	} else {
		frc = frc >> (127-exp);
	}

	frc = frc >> (24-len);
	if (frc > 0x007FFF) { frc = 0x7FFF; }
	if (sgn) { frc = (~frc) + 1; }
	
	return 0xFFFF & frc;
}

int16_t lgit_convert_int32(int32_t in)
{
	if (in > 32767) return 32767;
	if (in < -32767) return -32767;
	return 0xFFFF & in;
}

/*
                   
                                              
                                      
                                
                                    

                                                               
                                                                              

                                             
                                                             
                                             

                                                                  
                            
*/

#define GYRO_SCALE_FACTOR 262
#define HALL_SCALE_FACTOR 176
#define STABLE_THRESHOLD  600 //                                             

int32_t lgit_ois_stat(struct msm_sensor_ois_info_t *ois_stat)
{
	uint32_t gyro = 0;
	int16_t hall = 0;
	int16_t target = 0;
	
	snprintf(ois_stat->ois_provider, ARRAY_SIZE(ois_stat->ois_provider), "LGIT_ONSEMI");
	
	RamRead32A(GXADZ, &gyro);
	RamReadA(LXGZF, &target);	
	RamReadA(HXIN, &hall);

	ois_stat->gyro[0] =  lgit_convert_int32((int32_t)(lgit_convert_float32(gyro,18))*256/GYRO_SCALE_FACTOR);
	ois_stat->target[0] = lgit_convert_int32(-1*(((int32_t)target)*256/HALL_SCALE_FACTOR));
	ois_stat->hall[0] = lgit_convert_int32(((int32_t)hall)*256/HALL_SCALE_FACTOR);
	
	CDBG("%s gyrox %x, %x | targetx %x, %x | hallx  %x, %x \n",__func__, 
		gyro, ois_stat->gyro[0], target, ois_stat->target[0], hall, ois_stat->hall[0]);

	RamRead32A(GYADZ, &gyro);
	RamReadA(LYGZF, &target);
	RamReadA(HYIN, &hall);

	ois_stat->gyro[1] =  lgit_convert_int32((int32_t)(lgit_convert_float32(gyro,18))*256/GYRO_SCALE_FACTOR);
	ois_stat->target[1] = lgit_convert_int32(-1*(((int32_t)target)*256/HALL_SCALE_FACTOR));
	ois_stat->hall[1] = lgit_convert_int32(((int32_t)hall)*256/HALL_SCALE_FACTOR);

	CDBG("%s gyroy %x, %x | targety %x, %x | hally  %x, %x \n",__func__, 
		gyro, ois_stat->gyro[1], target, ois_stat->target[1], hall, ois_stat->hall[1]);

	ois_stat->is_stable = 1; 
	
	//                                     
	if (ois_stat->hall[0] == 0 && ois_stat->hall[1] == 0 && ois_stat->gyro[0] == 0 && ois_stat->gyro[1] == 0)
	{
		ois_stat->is_stable = 0; 
	}

	//                                                          
	RamReadA(LXCFIN, &target);
	if (abs(target) > STABLE_THRESHOLD)
	{
		ois_stat->is_stable = 0;
	}

	RamReadA(LYCFIN, &hall);
	if (abs(hall) > STABLE_THRESHOLD)
	{
		ois_stat->is_stable = 0;	
	}	
	CDBG("%s stable %x, %x, %d", __func__, target,hall, ois_stat->is_stable);
	
	return OIS_SUCCESS;
}

#define LENS_MOVE_POLLING_LIMIT (10)
#define LENS_MOVE_THRESHOLD     (5) //   

int32_t lgit_ois_move_lens(int16_t target_x, int16_t target_y)
{
	int32_t rc = OIS_SUCCESS;
	int16_t offset_x, offset_y;
	int16_t oldx, oldy;
	int16_t hallx1, hally1;
	int16_t hallx2, hally2;

	//                         
    //                                     
	offset_x = -1 * target_x * HALL_SCALE_FACTOR / 256;
	offset_y = -1 * target_y * HALL_SCALE_FACTOR / 256;

    //                           
	RamReadA(HXSEPT1, &oldx);	
	RamReadA(HYSEPT1, &oldy);	

	CDBG("%s offset %d,%d -> %d, %d \n",__func__, oldx, oldy, offset_x, offset_y);

	RamReadA(HXIN, &hallx1);	
	RamReadA(HYIN, &hally1);
	
	hallx1 -= oldx;
	hally1 -= oldy;
	
	CDBG("%s : hall %d, %d \n",__func__, hallx1, hally1);

	//                                                 
	EnsureWrite(HXSEPT1, offset_x, 5);
	EnsureWrite(HYSEPT1, offset_y, 5);

	usleep(30000);

	//                         
	RamReadA(HXIN, &hallx2); 
	RamReadA(HYIN, &hally2);

	hallx2 -= offset_x;
	hally2 -= offset_y;
	
	CDBG("%s : hall %d, %d \n",__func__, hallx2, hally2);

    //                 
	rc = OIS_FAIL;
	if (abs(hallx2+offset_x) < LENS_MOVE_THRESHOLD*HALL_SCALE_FACTOR ||
		abs(hally2+offset_y) < LENS_MOVE_THRESHOLD*HALL_SCALE_FACTOR )
	{
		rc = OIS_SUCCESS;
	}
#if 1	
	else if 
	(((abs(hallx1-hallx2) < LENS_MOVE_THRESHOLD*HALL_SCALE_FACTOR) && (oldx != offset_x))
	||((abs(hally1-hally2) < LENS_MOVE_THRESHOLD*HALL_SCALE_FACTOR) && (oldy != offset_y)))
	{
		rc = OIS_FAIL;	
		printk("%s : fail \n",__func__);
	}
	else
	{
		rc = OIS_SUCCESS;
	}
#endif

	return rc;

}


void lgit_ois_init(struct msm_ois_ctrl_t *msm_ois_t)
{
	lgit_ois_func_tbl.ois_on = lgit_ois_on;
	lgit_ois_func_tbl.ois_off = lgit_ois_off;
	lgit_ois_func_tbl.ois_mode = lgit_ois_mode;
	lgit_ois_func_tbl.ois_stat = lgit_ois_stat;
	lgit_ois_func_tbl.ois_move_lens = lgit_ois_move_lens;
	lgit_ois_func_tbl.ois_cur_mode = OIS_MODE_CENTERING_ONLY;
	
	msm_ois_t->sid_ois = 0x48 >> 1;
	msm_ois_t->ois_func_tbl = &lgit_ois_func_tbl;
}
