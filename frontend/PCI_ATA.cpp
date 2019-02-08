#include "PCI_ATA.h"
#include <iostream>

using namespace std;


PCI_ATA::PCI_ATA(HardDisk *hdd_instance): PCIDevice(ATA_VENDOR_ID, ATA_DEVICE_ID, ATA_CLASS_ID) {
	hdd = hdd_instance;
};

//Since Register are mapped wherever the target chooses, i convert them to normal ATA addresses

void PCI_ATA::write_reg_word(io_addr addr, uint16_t val){
	io_addr bar0,bar1;
	bar0 = this->getBar(0);
	bar1 = this->getBar(1);

	if(addr >= bar0 && addr <= bar0+7)
		addr = addr - bar0 + 0x01F0;
	else if(addr >= bar1 && addr <= bar1+1)
		addr = addr - bar1 + 0x03F6;

	hdd->write_reg_word(addr,val);
}

uint16_t PCI_ATA::read_reg_word(io_addr addr){
	io_addr bar0,bar1;
	bar0 = this->getBar(0);
	bar1 = this->getBar(1);

	if(addr >= bar0 && addr <= bar0+7)
		addr = addr - bar0 + 0x01F0;
	else if(addr >= bar1 && addr <= bar1+1)
		addr = addr - bar1 + 0x03F6;

	return hdd->read_reg_word(addr);
}

void PCI_ATA::write_reg_byte(io_addr addr, uint8_t val){
	io_addr bar0,bar1;
	bar0 = this->getBar(0);
	bar1 = this->getBar(1);

	if(addr >= bar0 && addr <= bar0+7)
		addr = addr - bar0 + 0x01F0;
	else if(addr >= bar1 && addr <= bar1+1)
		addr = addr - bar1 + 0x03F6;

	hdd->write_reg_byte(addr,val);
}

uint8_t PCI_ATA::read_reg_byte(io_addr addr){
	io_addr bar0,bar1;
	bar0 = this->getBar(0);
	bar1 = this->getBar(1);

	if(addr >= bar0 && addr <= bar0+7)
		addr = addr - bar0 + 0x01F0;
	else if(addr >= bar1 && addr <= bar1+1)
		addr = addr - bar1 + 0x03F6;

	return hdd->read_reg_byte(addr);
}

void PCI_ATA::write_reg_long(io_addr addr, uint32_t val){
	io_addr bar0,bar1;
	bar0 = this->getBar(0);
	bar1 = this->getBar(1);

	if(addr >= bar0 && addr <= bar0+7)
		addr = addr - bar0 + 0x01F0;
	else if(addr >= bar1 && addr <= bar1+1)
		addr = addr - bar1 + 0x03F6;

	hdd->write_reg_long(addr,val);
}

uint32_t PCI_ATA::read_reg_long(io_addr addr){
	io_addr bar0,bar1;
	bar0 = this->getBar(0);
	bar1 = this->getBar(1);

	if(addr >= bar0 && addr <= bar0+7)
		addr = addr - bar0 + 0x01F0;
	else if(addr >= bar1 && addr <= bar1+1)
		addr = addr - bar1 + 0x03F6;

	return hdd->read_reg_long(addr);
}



