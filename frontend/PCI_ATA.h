#ifndef PCI_ATA_H
#define PCI_ATA_H

#include "PCIDevice.h"
#include "HardDisk.h"
#include "../backend/ConsoleLog.h"

extern ConsoleLog& logg;

#define ATA_VENDOR_ID 0x7777
#define ATA_DEVICE_ID 0x8888
#define ATA_CLASS_ID  0x010100	


class PCI_ATA : public PCIDevice {
	private:
		HardDisk *hdd;

	public:
		PCI_ATA(HardDisk *hdd_instance);

		void write_reg_byte(io_addr addr, uint8_t val);
		uint8_t read_reg_byte(io_addr addr);
		void write_reg_word(io_addr addr, uint16_t val);
		uint16_t read_reg_word(io_addr addr);
		void write_reg_long(io_addr addr, uint32_t val);
		uint32_t read_reg_long(io_addr addr);

};

#endif