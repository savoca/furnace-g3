/*
                       
*/

#ifndef __SNFC_AVAILABLE_POLL__
#define __SNFC_AVAILABLE_POLL__

/*
                        
 */
#include "snfc_common.h"
#include "snfc_i2c.h"
#include "snfc_gpio.h"
#include "snfc_uart_collision_control.h"

int snfc_avail_poll_probe(struct device_node *np);
void snfc_avail_poll_remove(void);

/*
           
 */

#endif //                       
