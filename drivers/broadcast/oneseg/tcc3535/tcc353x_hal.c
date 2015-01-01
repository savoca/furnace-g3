#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include "tcpal_os.h"
#include "tcc353x_hal.h"

#define ISDB_EN			76 			/*         */
#define ISDB_RESET_N  		75 			/*         */
#define ISDB_INT_N		77 			/*         */

void TchalInit(void)
{
	gpio_request(ISDB_RESET_N, "ISDB_RESET");
	gpio_request(ISDB_EN, "ISDB_EN");
	gpio_request(ISDB_INT_N, "ISDB_INT");
	gpio_direction_output(ISDB_RESET_N, false); 	/*            */
	gpio_direction_output(ISDB_EN, false); 		/*            */
	gpio_direction_input(ISDB_INT_N); 		/*       */
	TcpalPrintStatus((I08S *)"[%s:%d]\n", __func__, __LINE__);
}

void TchalResetDevice(void)
{
	gpio_set_value(ISDB_RESET_N, 1);		/*                   */
	TcpalmSleep(5);
	gpio_set_value(ISDB_RESET_N, 0);		/*                  */
	TcpalmSleep(5);
	gpio_set_value(ISDB_RESET_N, 1);		/*                   */
	TcpalmSleep(5);
	TcpalPrintStatus((I08S *)"[%s:%d]\n", __func__, __LINE__);
}

void TchalPowerOnDevice(void)
{
	gpio_direction_output(ISDB_EN, false); 		/*            */
	gpio_direction_output(ISDB_RESET_N, false); 	/*            */
	gpio_set_value(ISDB_EN, 1);			/*              */
	TcpalmSleep(10);
	TchalResetDevice();
	TchalIrqSetup();
	TcpalPrintStatus((I08S *)"[%s:%d]\n", __func__, __LINE__);
}

void TchalPowerDownDevice(void)
{
	gpio_set_value(ISDB_RESET_N, 0);		/*                  */
	TcpalmSleep(5);
	gpio_set_value(ISDB_EN, 0);			/*             */
	TcpalPrintStatus((I08S *)"[%s:%d]\n", __func__, __LINE__);
}

void TchalIrqSetup(void)
{
	gpio_direction_input(ISDB_INT_N);		/*            */
}

