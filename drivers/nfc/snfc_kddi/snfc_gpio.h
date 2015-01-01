/*
             
 
*/


#ifndef __SNFC_GPIO_H__
#define __SNFC_GPIO_H__


/*
                            
 */
#include <linux/gpio.h>
#include "snfc_common.h"
/*      */

enum{
  GPIO_DIRECTION_IN = 0,
  GPIO_DIRECTION_OUT,
};

enum{
  GPIO_LOW_VALUE = 0,
  GPIO_HIGH_VALUE,
};

enum{
  GPIO_CONFIG_ENABLE = 0,
  GPIO_CONFIG_DISABLE,
};

 
struct snfc_gp {
     int gpio_hsel;
     int gpio_pon;
     int gpio_hvdd;
     int gpio_intu;
     int gpio_rfs;
     int gpio_uicc_con;
};

#ifdef CONFIG_LGE_NFC_SONY_KDDI
/*          */
#define GPIO_SNFC_PON		74  	//       

/*          */
#define GPIO_SNFC_RFS   	102		//        

/*          */
#define GPIO_SNFC_INT   	46		//       

/*               */
#define GPIO_SNFC_INTU   	59		//        

/*           */
#define GPIO_SNFC_HSEL		94		//        

/*           */
#define GPIO_SNFC_HVDD		145		//        

#else  //         
/*          */
#define GPIO_SNFC_PON		40  	//       

/*          */
#define GPIO_SNFC_RFS   	94		//        

/*          */
#define GPIO_SNFC_INT   	38		//       

/*               */
#define GPIO_SNFC_INTU   	36		//        

/*           */
#define GPIO_SNFC_HSEL		59		//        

/*           */
#define GPIO_SNFC_HVDD		35		//        

#endif

#define SNFC_GPIO_CFG(gpio, func, dir, pull, drvstr) \
    ((((gpio) & 0x3FF) << 4)        |   \
    ((func) & 0xf)                  |   \
    (((dir) & 0x1) << 14)           |   \
    (((pull) & 0x3) << 15)          |   \
    (((drvstr) & 0xF) << 17))

extern int gpio_rfs;

/*
                     
 */
int snfc_gpio_open(int gpionum, int direction, int value);
void snfc_gpio_write(int gpionum, int value);
int snfc_gpio_read(int gpionum);

#endif  //               
