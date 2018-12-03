#ifndef PCI_APIC_H
#define PCI_APIC_H

#include "PCIDevice.h"
#include "../backend/ConsoleLog.h"

extern ConsoleLog& logg;

#define APIC_VENDOR_ID 0x8086
#define APIC_DEVICE_ID 0x7000
//#define ATA_CLASS_ID  0x0101	


class PCI_APIC : public PCIDevice {
	private:

	public:
		PCI_APIC();
};

#endif