/*
 *  Copyright (c) 1997-1998  Mark Lord
 *  Copyright (c) 2007       MontaVista Software, Inc. <source@mvista.com>
 *
 *  May be copied or modified under the terms of the GNU General Public License
 *
 *  June 22, 2004 - get rid of check_region
 *                   - Jesper Juhl
 *
 */

/*
                                                                   
                                                                         
                                                                   
                                                                 
                                                                        
                                        
  
                                                    
  
                                                  
                                                          
  
                                                     
                                                                 
  
                                                            
                                                                   
                                                                    
 */

#define TRM290_NO_DMA_WRITES	/*                                      */

/*
                                   
                                   
                                                                     
                           
  
                                                                         
  
                                                                   
                                     
                    
                                                   
                                      
  
                                                             
                    
                                                              
                                                    
  
                                        
                    
                                          
                                                                          
                                                                
                                                              
                                                              
  
                                                     
                                                             
                                                                        
                                                                        
                                                            
                                                      
                                                       
  
                                                  
                                      
                                                                   
                            
  
                                                         
                                                   
  
                                         
                                   
                                                   
                                                 
                                                           
                                                                
                                                                       
                                                        
                                                     
  
                                                                 
                                     
  
                                                        
                                                        
                                 
                 
                                                          
                 
                 
                                      
  
                                                         
                                                   
  
                                                           
                                                          
  
                                                          
                                                   
                                   
                         
                                                     
                                   
                          
  
                                                                      
                                               
  
                                                   
                                                   
                                                     
  
                                                   
                                                   
                                                     
  
                                                   
                                                   
                                                     
  
 */

#include <linux/types.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/blkdev.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/ide.h>

#include <asm/io.h>

#define DRV_NAME "trm290"

static void trm290_prepare_drive (ide_drive_t *drive, unsigned int use_dma)
{
	ide_hwif_t *hwif = drive->hwif;
	u16 reg = 0;
	unsigned long flags;

	/*                   */
	reg = use_dma ? (0x21 | 0x82) : (0x21 & ~0x82);

	local_irq_save(flags);

	if (reg != hwif->select_data) {
		hwif->select_data = reg;
		/*             */
		outb(0x51 | (hwif->channel << 3), hwif->config_data + 1);
		outw(reg & 0xff, hwif->config_data);
	}

	/*                           */
	if (drive->dev_flags & IDE_DFLAG_PRESENT) {
		reg = inw(hwif->config_data + 3);
		reg &= 0x13;
		reg &= ~(1 << hwif->channel);
		outw(reg, hwif->config_data + 3);
	}

	local_irq_restore(flags);
}

static void trm290_dev_select(ide_drive_t *drive)
{
	trm290_prepare_drive(drive, !!(drive->dev_flags & IDE_DFLAG_USING_DMA));

	outb(drive->select | ATA_DEVICE_OBS, drive->hwif->io_ports.device_addr);
}

static int trm290_dma_check(ide_drive_t *drive, struct ide_cmd *cmd)
{
	if (cmd->tf_flags & IDE_TFLAG_WRITE) {
#ifdef TRM290_NO_DMA_WRITES
		/*                           */
		return 1;
#endif
	}
	return 0;
}

static int trm290_dma_setup(ide_drive_t *drive, struct ide_cmd *cmd)
{
	ide_hwif_t *hwif = drive->hwif;
	unsigned int count, rw = (cmd->tf_flags & IDE_TFLAG_WRITE) ? 1 : 2;

	count = ide_build_dmatable(drive, cmd);
	if (count == 0)
		/*                        */
		return 1;

	outl(hwif->dmatable_dma | rw, hwif->dma_base);
	/*           */
	outw(count * 2 - 1, hwif->dma_base + 2);

	return 0;
}

static void trm290_dma_start(ide_drive_t *drive)
{
	trm290_prepare_drive(drive, 1);
}

static int trm290_dma_end(ide_drive_t *drive)
{
	u16 status = inw(drive->hwif->dma_base + 2);

	trm290_prepare_drive(drive, 0);

	return status != 0x00ff;
}

static int trm290_dma_test_irq(ide_drive_t *drive)
{
	u16 status = inw(drive->hwif->dma_base + 2);

	return status == 0x00ff;
}

static void trm290_dma_host_set(ide_drive_t *drive, int on)
{
}

static void __devinit init_hwif_trm290(ide_hwif_t *hwif)
{
	struct pci_dev *dev	= to_pci_dev(hwif->dev);
	unsigned int  cfg_base	= pci_resource_start(dev, 4);
	unsigned long flags;
	u8 reg = 0;

	if ((dev->class & 5) && cfg_base)
		printk(KERN_INFO DRV_NAME " %s: chip", pci_name(dev));
	else {
		cfg_base = 0x3df0;
		printk(KERN_INFO DRV_NAME " %s: using default", pci_name(dev));
	}
	printk(KERN_CONT " config base at 0x%04x\n", cfg_base);
	hwif->config_data = cfg_base;
	hwif->dma_base = (cfg_base + 4) ^ (hwif->channel ? 0x80 : 0);

	printk(KERN_INFO "    %s: BM-DMA at 0x%04lx-0x%04lx\n",
	       hwif->name, hwif->dma_base, hwif->dma_base + 3);

	if (ide_allocate_dma_engine(hwif))
		return;

	local_irq_save(flags);
	/*                                                     */
	outb(0x51 | (hwif->channel << 3), hwif->config_data + 1);
	/*                       */
	hwif->select_data = 0x21;
	outb(hwif->select_data, hwif->config_data);
	/*              */
	reg = inb(hwif->config_data + 3);
	/*                          */
	reg = (reg & 0x10) | 0x03;
	outb(reg, hwif->config_data + 3);
	local_irq_restore(flags);

	if (reg & 0x10)
		/*             */
		hwif->irq = hwif->channel ? 15 : 14;

#if 1
	{
	/*
                                                                      
                                                                    
                                             
  */
		u16 new, old, compat = hwif->channel ? 0x374 : 0x3f4;
		static u16 next_offset = 0;
		u8 old_mask;

		outb(0x54 | (hwif->channel << 3), hwif->config_data + 1);
		old = inw(hwif->config_data);
		old &= ~1;
		old_mask = inb(old + 2);
		if (old != compat && old_mask == 0xff) {
			/*                               */
			compat += (next_offset += 0x400);
			hwif->io_ports.ctl_addr = compat + 2;
			outw(compat | 1, hwif->config_data);
			new = inw(hwif->config_data);
			printk(KERN_INFO "%s: control basereg workaround: "
				"old=0x%04x, new=0x%04x\n",
				hwif->name, old, new & ~1);
		}
	}
#endif
}

static const struct ide_tp_ops trm290_tp_ops = {
	.exec_command		= ide_exec_command,
	.read_status		= ide_read_status,
	.read_altstatus		= ide_read_altstatus,
	.write_devctl		= ide_write_devctl,

	.dev_select		= trm290_dev_select,
	.tf_load		= ide_tf_load,
	.tf_read		= ide_tf_read,

	.input_data		= ide_input_data,
	.output_data		= ide_output_data,
};

static struct ide_dma_ops trm290_dma_ops = {
	.dma_host_set		= trm290_dma_host_set,
	.dma_setup 		= trm290_dma_setup,
	.dma_start 		= trm290_dma_start,
	.dma_end		= trm290_dma_end,
	.dma_test_irq		= trm290_dma_test_irq,
	.dma_lost_irq		= ide_dma_lost_irq,
	.dma_check		= trm290_dma_check,
};

static const struct ide_port_info trm290_chipset __devinitdata = {
	.name		= DRV_NAME,
	.init_hwif	= init_hwif_trm290,
	.tp_ops 	= &trm290_tp_ops,
	.dma_ops	= &trm290_dma_ops,
	.host_flags	= IDE_HFLAG_TRM290 |
			  IDE_HFLAG_NO_ATAPI_DMA |
#if 0 /*                      */
			  IDE_HFLAG_TRUST_BIOS_FOR_DMA |
#endif
			  IDE_HFLAG_NO_AUTODMA |
			  IDE_HFLAG_NO_LBA48,
};

static int __devinit trm290_init_one(struct pci_dev *dev, const struct pci_device_id *id)
{
	return ide_pci_init_one(dev, &trm290_chipset, NULL);
}

static const struct pci_device_id trm290_pci_tbl[] = {
	{ PCI_VDEVICE(TEKRAM, PCI_DEVICE_ID_TEKRAM_DC290), 0 },
	{ 0, },
};
MODULE_DEVICE_TABLE(pci, trm290_pci_tbl);

static struct pci_driver trm290_pci_driver = {
	.name		= "TRM290_IDE",
	.id_table	= trm290_pci_tbl,
	.probe		= trm290_init_one,
	.remove		= ide_pci_remove,
};

static int __init trm290_ide_init(void)
{
	return ide_pci_register_driver(&trm290_pci_driver);
}

static void __exit trm290_ide_exit(void)
{
	pci_unregister_driver(&trm290_pci_driver);
}

module_init(trm290_ide_init);
module_exit(trm290_ide_exit);

MODULE_AUTHOR("Mark Lord");
MODULE_DESCRIPTION("PCI driver module for Tekram TRM290 IDE");
MODULE_LICENSE("GPL");
