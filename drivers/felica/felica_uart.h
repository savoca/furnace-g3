/*
                
  
 */

#ifndef __FELICA_UART_H__
#define __FELICA_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
                            
 */

#include <linux/list.h>

#include "felica_common.h"

/*
          
 */
int felica_uart_open(void);
int felica_uart_close(void);
int felica_uart_write(char *buf, size_t count);
int felica_uart_read(char *buf, size_t count);
int felica_uart_ioctrl(int *count);

#ifdef __cplusplus
}
#endif

#endif //                  
