/*
                        
  
                                                                                
                          
                                                                               
                                            
 */

#ifndef _ALCHEMY_GPIO_H_
#define _ALCHEMY_GPIO_H_

#include <asm/mach-au1x00/au1000.h>
#include <asm/mach-au1x00/gpio-au1000.h>
#include <asm/mach-au1x00/gpio-au1300.h>

/*                                                               
                                                                            
                                                
 */
static inline void alchemy_gpio1_input_enable(void)
{
	void __iomem *base = (void __iomem *)KSEG1ADDR(AU1000_SYS_PHYS_ADDR);
	__raw_writel(0, base + 0x110);		/*                     */
	wmb();
}


/*                                  
 
                               
                                        
                                
                                        
                                            
 
                                        
                                                                   
 
                                        
                                                                  
                                                                  
         

                                                                       
                                                                   
                                                                     
                                                                   
                                                                       
               
*/

#ifdef CONFIG_GPIOLIB

/*                                               */
/*                                          */
static inline int __au_irq_to_gpio(unsigned int irq)
{
	switch (alchemy_get_cputype()) {
	case ALCHEMY_CPU_AU1000...ALCHEMY_CPU_AU1200:
		return alchemy_irq_to_gpio(irq);
	case ALCHEMY_CPU_AU1300:
		return au1300_irq_to_gpio(irq);
	}
	return -EINVAL;
}


/*                                                               */
#ifndef CONFIG_ALCHEMY_GPIO_INDIRECT	/*          */

/*                                */
#define gpio_to_irq	__gpio_to_irq
#define gpio_get_value	__gpio_get_value
#define gpio_set_value	__gpio_set_value
#define gpio_cansleep	__gpio_cansleep
#define irq_to_gpio	__au_irq_to_gpio

#include <asm-generic/gpio.h>

#endif	/*                               */


#endif	/*                */

#endif	/*                  */
