#include "PCIDevice.h"
using namespace std;
		
PCIDevice::PCIDevice(uint16_t vendorID, uint16_t deviceID, uint32_t classCode) {
	registers.vendorID = vendorID;
	registers.deviceID = deviceID;
	registers.classCode[0] = (uint8_t)classCode;
	registers.classCode[1] = (uint8_t)(classCode >> 8);
	registers.classCode[2] = (uint8_t)(classCode >> 16);
	for(int i = 0; i<6; i++)
		registers.BAR[i] = 0;	
}

PCIDevice::PCIDevice(uint16_t vendorID, uint16_t deviceID) {
	registers.vendorID = vendorID;
	registers.deviceID = deviceID;
	for(int i = 0; i<6; i++)
		registers.BAR[i] = 0;
}

uint16_t PCIDevice::getVendorID() {
	return registers.vendorID;
}

uint16_t PCIDevice::getDeviceID() {
	return registers.deviceID;
}

uint32_t PCIDevice::getClassCode() {	
	uint32_t fullClassCode = 0;
	fullClassCode = (registers.classCode[0]) | (registers.classCode[1] << 8) | (registers.classCode[2] << 16);
	return fullClassCode;
}

uint32_t PCIDevice::getBar(uint8_t index) {	
	if(index < 0 || index > 5){
		logg << "Out of bound PCIDevice::getBar with index: " << index << endl;
		return -1; //Out of bound
	}
	return registers.BAR[index];
}

void PCIDevice::setBar(uint32_t value,uint8_t index) {
	if(index < 0 || index > 5){
		logg << "Out of bound PCIDevice::setBar with index: " << index << endl;
		return; //Out of bound
	}
	registers.BAR[index] = value;
}

// next function can read/write in a certain offset inside device registers, offset in byte

void PCIDevice::write_reg_byte_PCI(uint32_t offset, uint8_t val) { 
	if(offset < 0 || offset + 1 > sizeof(deviceRegisters)){
		logg << "Out of bound PCIDevice::write_reg_byte_PCI with offset: " << offset << endl;
		return; //Out of bound
	}
	uint8_t *regs = (uint8_t*)&registers;
	*((uint8_t*)(regs + offset)) = val;
}

void PCIDevice::write_reg_word_PCI(uint32_t offset, uint16_t val) {
	if(offset < 0 || offset + 2 > sizeof(deviceRegisters)){
		logg << "Out of bound PCIDevice::write_reg_word_PCI with offset: " << offset << endl;
		return; //Out of bound
	}
	uint8_t *regs = (uint8_t*)&registers;
	*((uint16_t*)(regs + offset)) = val;
}

void PCIDevice::write_reg_long_PCI(uint32_t offset, uint32_t val) {
	if(offset < 0 || offset + 4 > sizeof(deviceRegisters)){
		logg << "Out of bound PCIDevice::write_reg_long_PCI with offset: " << offset << endl;
		return; //Out of bound
	}
	uint8_t *regs = (uint8_t*)&registers;
	*((uint32_t*)(regs + offset)) = val;
}

uint8_t PCIDevice::read_reg_byte_PCI(uint32_t offset) { 
	if(offset < 0 || offset + 1 > sizeof(deviceRegisters)){
		logg << "Out of bound PCIDevice::read_reg_byte_PCI with offset: " << offset << endl;
		return 0; //Out of bound
	}
	uint8_t *regs = (uint8_t*)&registers;
	return *(regs + offset);
}
uint16_t PCIDevice::read_reg_word_PCI(uint32_t offset) { // CAMPIONE SU CUI FACCIO TEST
	if(offset < 0 || offset + 2 > sizeof(deviceRegisters)){
		logg << "Out of bound PCIDevice::read_reg_word_PCI with offset: " << offset << endl;
		return 0; //Out of bound
	}
	uint8_t *regs = (uint8_t*)&registers;
	uint16_t tmp = *((uint16_t*)(regs + offset));
	return tmp;
}
uint32_t PCIDevice::read_reg_long_PCI(uint32_t offset) {
	if(offset < 0 || offset + 4 > sizeof(deviceRegisters)){
		logg << "Out of bound read_reg_long_PCI with offset: " << offset << endl;
		return 0; //Out of bound
	}
	uint8_t *regs = (uint8_t*)&registers;
	return *((uint32_t*)(regs + offset));
}