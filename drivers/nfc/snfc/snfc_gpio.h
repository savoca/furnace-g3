/*
             
 
*/


#ifndef __SNFC_GPIO_H__
#define __SNFC_GPIO_H__


/*
                            
 */
#include <linux/gpio.h>
#include "snfc_common.h"
#include <mach/board_lge.h>
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

/*          */
#define GPIO_SNFC_PON           40      //       

/*          */
#define GPIO_SNFC_RFS           94              //        

/*          */
#define GPIO_SNFC_RFS_REV_D     59              //              

/*          */
#define GPIO_SNFC_INT           38              //       

/*          */
#define GPIO_SNFC_INT_REV_D     37              //             

/*               */
#define GPIO_SNFC_INTU          36              //        

/*               */
#define GPIO_SNFC_INTU_REV_D    35              //              

/*           */
#define GPIO_SNFC_HSEL          59              //        

/*           */
#define GPIO_SNFC_HSEL_REV_D            94              //              

/*           */
#define GPIO_SNFC_HVDD          35      //        

/*           */
#define GPIO_SNFC_HVDD_REV_D            36      //              

#define GPIO_SNFC_UICC_CON		39	//        

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
int snfc_get_rfs_gpio_num(void);
int snfc_get_int_gpio_num(void);
int snfc_get_intu_gpio_num(void);
int snfc_get_hsel_gpio_num(void);
int snfc_get_hvdd_gpio_num(void);

#endif  //               
