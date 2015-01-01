/*
 * PCIe Port Native Services Support, ACPI-Related Part
 *
 * Copyright (C) 2010 Rafael J. Wysocki <rjw@sisk.pl>, Novell Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License V2.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/acpi.h>
#include <linux/pci-acpi.h>
#include <linux/pcieport_if.h>

#include "aer/aerdrv.h"
#include "../pci.h"

/* 
                                                                               
                                                               
                                                                 
  
                                                                              
                                                                             
                                                                               
                                                          
  
                                                                         
                                                                     
 */
int pcie_port_acpi_setup(struct pci_dev *port, int *srv_mask)
{
	struct acpi_pci_root *root;
	acpi_handle handle;
	u32 flags;

	if (acpi_pci_disabled)
		return 0;

	handle = acpi_find_root_bridge_handle(port);
	if (!handle)
		return -EINVAL;

	root = acpi_pci_find_root(handle);
	if (!root)
		return -ENODEV;

	flags = root->osc_control_set;

	*srv_mask = PCIE_PORT_SERVICE_VC;
	if (flags & OSC_PCI_EXPRESS_NATIVE_HP_CONTROL)
		*srv_mask |= PCIE_PORT_SERVICE_HP;
	if (flags & OSC_PCI_EXPRESS_PME_CONTROL)
		*srv_mask |= PCIE_PORT_SERVICE_PME;
	if (flags & OSC_PCI_EXPRESS_AER_CONTROL)
		*srv_mask |= PCIE_PORT_SERVICE_AER;

	return 0;
}
