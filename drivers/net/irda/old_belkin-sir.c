/*********************************************************************
 *                
 * Filename:      old_belkin.c
 * Version:       1.1
 * Description:   Driver for the Belkin (old) SmartBeam dongle
 * Status:        Experimental...
 * Author:        Jean Tourrilhes <jt@hpl.hp.com>
 * Created at:    22/11/99
 * Modified at:   Fri Dec 17 09:13:32 1999
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 * 
 *     Copyright (c) 1999 Jean Tourrilhes, All Rights Reserved.
 *     
 *     This program is free software; you can redistribute it and/or 
 *     modify it under the terms of the GNU General Public License as 
 *     published by the Free Software Foundation; either version 2 of 
 *     the License, or (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License 
 *     along with this program; if not, write to the Free Software 
 *     Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
 *     MA 02111-1307 USA
 *     
 ********************************************************************/

#include <linux/module.h>
#include <linux/delay.h>
#include <linux/init.h>

#include <net/irda/irda.h>
//                                  

#include "sir-dev.h"

/*
                                                   
                                                                        
                                                        
                                                                 
                           
  
                                                                       
                                                                    
                                                                   
                                                             
  
                                                                       
                                                                      
                                                                    
                                          
  
                                                                     
                                                                   
                                                                            
                                                             
  
                                                                    
                                                             
 */

static int old_belkin_open(struct sir_dev *dev);
static int old_belkin_close(struct sir_dev *dev);
static int old_belkin_change_speed(struct sir_dev *dev, unsigned speed);
static int old_belkin_reset(struct sir_dev *dev);

static struct dongle_driver old_belkin = {
	.owner		= THIS_MODULE,
	.driver_name	= "Old Belkin SmartBeam",
	.type		= IRDA_OLD_BELKIN_DONGLE,
	.open		= old_belkin_open,
	.close		= old_belkin_close,
	.reset		= old_belkin_reset,
	.set_speed	= old_belkin_change_speed,
};

static int __init old_belkin_sir_init(void)
{
	return irda_register_dongle(&old_belkin);
}

static void __exit old_belkin_sir_cleanup(void)
{
	irda_unregister_dongle(&old_belkin);
}

static int old_belkin_open(struct sir_dev *dev)
{
	struct qos_info *qos = &dev->qos;

	IRDA_DEBUG(2, "%s()\n", __func__);

	/*                 */
	sirdev_set_dtr_rts(dev, TRUE, TRUE);

	/*                         */
	qos->baud_rate.bits &= IR_9600;
	/*                                                                   */
	qos->min_turn_time.bits = 0x01;
	irda_qos_bits_to_value(qos);

	/*                                              */

	return 0;
}

static int old_belkin_close(struct sir_dev *dev)
{
	IRDA_DEBUG(2, "%s()\n", __func__);

	/*                  */
	sirdev_set_dtr_rts(dev, FALSE, FALSE);

	return 0;
}

/*
                                          
  
                                                      
 */
static int old_belkin_change_speed(struct sir_dev *dev, unsigned speed)
{
	IRDA_DEBUG(2, "%s()\n", __func__);

	dev->speed = 9600;
	return (speed==dev->speed) ? 0 : -EINVAL;
}

/*
                                   
  
                                         
  
 */
static int old_belkin_reset(struct sir_dev *dev)
{
	IRDA_DEBUG(2, "%s()\n", __func__);

	/*                                               */
	dev->speed = 9600;

	return 0;
}

MODULE_AUTHOR("Jean Tourrilhes <jt@hpl.hp.com>");
MODULE_DESCRIPTION("Belkin (old) SmartBeam dongle driver");	
MODULE_LICENSE("GPL");
MODULE_ALIAS("irda-dongle-7"); /*                        */

module_init(old_belkin_sir_init);
module_exit(old_belkin_sir_cleanup);
