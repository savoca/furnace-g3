/*
                                                                     
                                                                 
            
  
                                                                      
                                                                           
                                                                           
                  
  
                             
  
          
                                                      
                                        
                           
                                 
                                                                       
                                                                     
                                                           
                              
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>

#include <asm/io.h>

#include <acpi/acpi_bus.h>
#include <acpi/acpi_drivers.h>

static int pm_tmr_ioport = 0;

/*                                                 */
static u32 read_pmtmr(void)
{
	u32 v1=0,v2=0,v3=0;
	/*                                                    
                                                            
                                                       
                                         
  */
	do {
		v1 = inl(pm_tmr_ioport);
		v2 = inl(pm_tmr_ioport);
		v3 = inl(pm_tmr_ioport);
	} while ((v1 > v2 && v1 < v3) || (v2 > v3 && v2 < v1)
		 || (v3 > v1 && v3 < v2));

	/*                            */
	return (v2 & 0xFFFFFF);
}

static int __init cpufreq_test_tsc(void)
{
	u32 now, then, diff;
	u64 now_tsc, then_tsc, diff_tsc;
	int i;

	/*                                                                      
                      */

	/*                                          */
	if (acpi_gbl_FADT.header.revision >= FADT2_REVISION_ID) {
		/*             */
		if (acpi_gbl_FADT.xpm_timer_block.space_id !=
		    ACPI_ADR_SPACE_SYSTEM_IO)
			return 0;

		pm_tmr_ioport = acpi_gbl_FADT.xpm_timer_block.address;
		/*
                                                            
                                                             
                                   
    */
		if (!pm_tmr_ioport)
			pm_tmr_ioport = acpi_gbl_FADT.pm_timer_block;
	} else {
		/*             */
		pm_tmr_ioport = acpi_gbl_FADT.pm_timer_block;
	}

	printk(KERN_DEBUG "start--> \n");
	then = read_pmtmr();
        rdtscll(then_tsc);
	for (i=0;i<20;i++) {
		mdelay(100);
		now = read_pmtmr();
		rdtscll(now_tsc);
		diff = (now - then) & 0xFFFFFF;
		diff_tsc = now_tsc - then_tsc;
		printk(KERN_DEBUG "t1: %08u t2: %08u diff_pmtmr: %08u diff_tsc: %016llu\n", then, now, diff, diff_tsc);
		then = now;
		then_tsc = now_tsc;
	}
	printk(KERN_DEBUG "<-- end \n");
	return -ENODEV;
}

static void __exit cpufreq_none(void)
{
	return;
}

module_init(cpufreq_test_tsc)
module_exit(cpufreq_none)


MODULE_AUTHOR("Dominik Brodowski");
MODULE_DESCRIPTION("Verify the TSC cpufreq notifier working correctly -- needs ACPI-enabled system");
MODULE_LICENSE ("GPL");
