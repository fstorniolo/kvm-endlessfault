#ifndef PCI_ATA_H
#define PCI_ATA_H

#include "PCIDevice.h"
#include "HardDisk.h"
#include "../backend/ConsoleLog.h"
#include "../kvm.h"

extern ConsoleLog& logg;

#define ATA_VENDOR_ID 0x7777
#define ATA_DEVICE_ID 0x8888
#define ATA_CLASS_ID  0x010100	

#define BMCMD_OFFSET 	0x0
#define BMSTR_OFFSET 	0x2
#define BMDTPR_OFFSET	0x4


struct buffer_descriptor
{
	uint32_t buffer_addr;

	uint16_t 	buffer_size;
	uint8_t 	reserved;
	uint8_t 	EOT;



}__attribute__((packed));


class PCI_ATA : public PCIDevice {
	private:
		HardDisk *hdd;
		uint32_t BMDTPR;
		uint16_t BMCMD,BMSTR;

		uint8_t *guest_memory;
		void process_cmd();

	public:
		PCI_ATA(HardDisk *hdd_instance);

		void write_reg_byte(io_addr addr, uint8_t val);
		uint8_t read_reg_byte(io_addr addr);
		void write_reg_word(io_addr addr, uint16_t val);
		uint16_t read_reg_word(io_addr addr);
		void write_reg_long(io_addr addr, uint32_t val);
		uint32_t read_reg_long(io_addr addr);
		
		void setVmem(uint8_t* addr);
		void sendInterrupt(uint16_t irq, uint16_t level);

};

#endif