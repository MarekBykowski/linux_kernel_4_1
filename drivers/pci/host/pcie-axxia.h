/*
 * Axxia PCIe host controller driver
 *
 * Copyright (C) 2015 Intel Electronics Co., Ltd.
 *               http://www.intel.com
 *
 * Author: Sangeetha Rao <sangeetha.rao@intel.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _PCIE_AXXIA_H
#define _PCIE_AXXIA_H

/*
 * Maximum number of MSI IRQs can be 256 per controller. But keep
 * it 32 as of now. Probably we will never need more than 32. If needed,
 * then increment it in multiple of 32.
 */
#define MAX_MSI_IRQS			256
#define AXXIA_MSI_IRQL			32
#define AXXIA_MSI_UNCONFIGURED		0
#define AXXIA_MSI_MODE			1
#define AXXIA_MSIX_MODE			2

struct pcie_port {
	struct device		*dev;
	u8			root_bus_nr;
	s8                      pei_nr;
	void __iomem		*dbi_base;
	void __iomem		*cc_gpreg_base;
	void __iomem		*axi_gpreg_base;
	u64			cfg0_base;
	u64			cfg0_mod_base;
	void __iomem		*va_cfg0_base;
	u32			cfg0_size;
	u64			cfg1_base;
	u64			cfg1_mod_base;
	void __iomem		*va_cfg1_base;
	u32			cfg1_size;
	u64			io_base;
	u64			io_mod_base;
	phys_addr_t		io_bus_addr;
	u32			io_size;
	u64			mem_base;
	u64			mem_mod_base;
	phys_addr_t		mem_bus_addr;
	u32			mem_size;
	struct resource		cfg;
	struct resource		io;
	struct resource		mem;
	struct resource		busn;
	u32			lanes;
	struct pcie_host_ops	*ops;
	int			irqs; /* removed 34 not sure why it's there
					 [34]; */
	int			msi_irqs[AXXIA_MSI_IRQL];
	int			num_msi_irqs;
	u32			msi_mode;
	unsigned long		msi_data;
	struct irq_domain	*irq_domain;
	struct mutex		bitmap_lock;
	unsigned long		*bitmap;
	struct msi_controller chip;
	DECLARE_BITMAP(msi_irq_in_use, MAX_MSI_IRQS);
#if 0
	u32			lanes;
	int			msi_irq;
#endif
};

struct pcie_host_ops {
	void (*readl_rc)(struct pcie_port *pp,
			void __iomem *dbi_base, u32 *val);
	void (*writel_rc)(struct pcie_port *pp,
			u32 val, void __iomem *dbi_base);
	int (*rd_own_conf)(struct pcie_port *pp, int where, int size, u32 *val);
	int (*wr_own_conf)(struct pcie_port *pp, int where, int size, u32 val);
	int (*rd_other_conf)(struct pcie_port *pp, struct pci_bus *bus,
			unsigned int devfn, int where, int size, u32 *val);
	int (*wr_other_conf)(struct pcie_port *pp, struct pci_bus *bus,
			unsigned int devfn, int where, int size, u32 val);
	int (*link_up)(struct pcie_port *pp);
	void (*host_init)(struct pcie_port *pp);
	void (*msi_set_irq)(struct pcie_port *pp, int irq);
	void (*msi_clear_irq)(struct pcie_port *pp, int irq);
	u32 (*get_msi_addr)(struct pcie_port *pp);
	u32 (*get_msi_data)(struct pcie_port *pp, int pos);
	void (*scan_bus)(struct pcie_port *pp);
	int (*msi_host_init)(struct pcie_port *pp, struct msi_controller *chip);
};

int axxia_pcie_cfg_read(void __iomem *addr, int where, int size, u32 *val);
int axxia_pcie_cfg_write(void __iomem *addr, int where, int size, u32 val);
int axxia_pcie_link_up(struct pcie_port *pp);
void axxia_pcie_setup_rc(struct pcie_port *pp);
int axxia_pcie_host_init(struct pcie_port *pp);

#endif /* _PCIE_AXXIA_H */
