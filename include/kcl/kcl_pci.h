#ifndef AMDKCL_PCI_H
#define AMDKCL_PCI_H

#include <linux/pci.h>
#include <linux/version.h>

#ifndef PCI_EXP_DEVCAP2_ATOMIC_ROUTE
#define PCI_EXP_DEVCAP2_ATOMIC_ROUTE	0x00000040 /* Atomic Op routing */
#endif
#ifndef PCI_EXP_DEVCAP2_ATOMIC_COMP32
#define PCI_EXP_DEVCAP2_ATOMIC_COMP32	0x00000080 /* 32b AtomicOp completion */
#endif
#ifndef PCI_EXP_DEVCAP2_ATOMIC_COMP64
#define PCI_EXP_DEVCAP2_ATOMIC_COMP64	0x00000100 /* 64b AtomicOp completion*/
#endif
#ifndef PCI_EXP_DEVCAP2_ATOMIC_COMP128
#define PCI_EXP_DEVCAP2_ATOMIC_COMP128	0x00000200 /* 128b AtomicOp completion*/
#endif
#ifndef PCI_EXP_DEVCTL2_ATOMIC_REQ
#define PCI_EXP_DEVCTL2_ATOMIC_REQ	0x0040	/* Set Atomic requests */
#endif
#ifndef PCI_EXP_DEVCTL2_ATOMIC_BLOCK
#define PCI_EXP_DEVCTL2_ATOMIC_BLOCK	0x0040	/* Block AtomicOp on egress */
#endif

#if !defined(HAVE_PCIE_ENABLE_ATOMIC_OPS_TO_ROOT)
int pci_enable_atomic_ops_to_root(struct pci_dev *dev, u32 comp_caps);
#endif

#ifndef PCIE_SPEED_16_0GT
#define PCIE_SPEED_16_0GT		0x17
#endif
#ifndef PCI_EXP_LNKCAP2_SLS_16_0GB
#define PCI_EXP_LNKCAP2_SLS_16_0GB	0x00000010	/* Supported Speed 16GT/s */
#endif
#ifndef PCI_EXP_LNKCAP_SLS_16_0GB
#define PCI_EXP_LNKCAP_SLS_16_0GB	0x00000004	/* LNKCAP2 SLS Vector bit 3 */
#endif
#ifndef PCI_EXP_LNKSTA_CLS_16_0GB
#define PCI_EXP_LNKSTA_CLS_16_0GB	0x0004		/* Current Link Speed 16.0GT/s */
#endif

/* PCIe link information */
#ifndef PCIE_SPEED2STR
#define PCIE_SPEED2STR(speed) \
	((speed) == PCIE_SPEED_16_0GT ? "16 GT/s" : \
	(speed) == PCIE_SPEED_8_0GT ? "8 GT/s" : \
	(speed) == PCIE_SPEED_5_0GT ? "5 GT/s" : \
	(speed) == PCIE_SPEED_2_5GT ? "2.5 GT/s" : \
	"Unknown speed")
#endif

/* PCIe speed to Mb/s reduced by encoding overhead */
#ifndef PCIE_SPEED2MBS_ENC
#define PCIE_SPEED2MBS_ENC(speed) \
	((speed) == PCIE_SPEED_16_0GT ? 16000*128/130 : \
	(speed) == PCIE_SPEED_8_0GT  ?  8000*128/130 : \
	(speed) == PCIE_SPEED_5_0GT  ?  5000*8/10 : \
	(speed) == PCIE_SPEED_2_5GT  ?  2500*8/10 : \
	0)
#endif

#ifndef PCI_EXP_LNKCAP_SLS_8_0GB
#define AMDKCL_CREATE_MEASURE_FILE
#define  PCI_EXP_LNKCAP_SLS_8_0GB 0x00000003 /* LNKCAP2 SLS Vector bit 2 */
int  _kcl_pci_create_measure_file(struct pci_dev *pdev);
#endif

#if defined(BUILD_AS_DKMS) && (LINUX_VERSION_CODE < KERNEL_VERSION(4, 11, 0))
void _kcl_pci_configure_extended_tags(struct pci_dev *dev);
#endif

#if !defined(HAVE_PCIE_GET_SPEED_AND_WIDTH_CAP)
extern enum pci_bus_speed (*_kcl_pcie_get_speed_cap)(struct pci_dev *dev);
extern enum pcie_link_width (*_kcl_pcie_get_width_cap)(struct pci_dev *dev);
#endif

#if !defined(HAVE_PCIE_BANDWIDTH_AVAILABLE)
u32 pcie_bandwidth_available(struct pci_dev *dev, struct pci_dev **limiting_dev,
			     enum pci_bus_speed *speed,
			     enum pcie_link_width *width);
#endif

static inline enum pci_bus_speed kcl_pcie_get_speed_cap(struct pci_dev *dev)
{
#if defined(HAVE_PCIE_GET_SPEED_AND_WIDTH_CAP)
		return pcie_get_speed_cap(dev);
#else
		return _kcl_pcie_get_speed_cap(dev);
#endif
}

static inline enum pcie_link_width kcl_pcie_get_width_cap(struct pci_dev *dev)
{
#if defined(HAVE_PCIE_GET_SPEED_AND_WIDTH_CAP)
		return pcie_get_width_cap(dev);
#else
		return _kcl_pcie_get_width_cap(dev);
#endif
}

static inline int kcl_pci_create_measure_file(struct pci_dev *pdev)
{
#ifdef AMDKCL_CREATE_MEASURE_FILE
	return _kcl_pci_create_measure_file(pdev);
#else
	return 0;
#endif
}

static inline void kcl_pci_configure_extended_tags(struct pci_dev *dev)
{
#if defined(BUILD_AS_DKMS) && (LINUX_VERSION_CODE < KERNEL_VERSION(4, 11, 0))
	_kcl_pci_configure_extended_tags(dev);
#endif
}

#if !defined(HAVE_PCI_DEV_ID)
static inline u16 pci_dev_id(struct pci_dev *dev)
{
	return PCI_DEVID(dev->bus->number, dev->devfn);
}
#endif /* HAVE_PCI_DEV_ID */

#if !defined(HAVE_PCI_UPSTREAM_BRIDGE)
static inline struct pci_dev *pci_upstream_bridge(struct pci_dev *dev)
{
	dev = pci_physfn(dev);
	if (pci_is_root_bus(dev->bus))
		return NULL;

	return dev->bus->self;
}
#endif
#endif /* AMDKCL_PCI_H */
