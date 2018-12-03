#include "PCIDevice.h"
using namespace std;
		
PCIDevice::PCIDevice(uint16_t vendorID, uint16_t deviceID, uint32_t classCode) {
	registers.vendorID = vendorID;
	registers.deviceID = deviceID;
	registers.classCode[0] = (uint8_t)classCode;
	registers.classCode[1] = (uint8_t)(classCode >> 8);
	registers.classCode[2] = (uint8_t)(classCode >> 16);
	registers.BAR0 = 0;
	registers.BAR1 = 0;
	registers.BAR2 = 0;
	registers.BAR3 = 0;
	registers.BAR4 = 0;
	registers.BAR5 = 0;
}

PCIDevice::PCIDevice(uint16_t vendorID, uint16_t deviceID) {
	registers.vendorID = vendorID;
	registers.deviceID = deviceID;
	registers.BAR0 = 0;
	registers.BAR1 = 0;
	registers.BAR2 = 0;
	registers.BAR3 = 0;
	registers.BAR4 = 0;
	registers.BAR5 = 0;
}

uint16_t PCIDevice::getVendorID() {
	return read_reg_word(0); // <- THIS DONE
}

uint16_t PCIDevice::getDeviceID() {
	logg << endl << "dev init " << endl;
	uint16_t tmp = read_reg_word(1); //<- THIS CANT READ IN CORRECT POSITION(CHANGE EVERY TIME) - IL PUNTATORE ALLA STRUTTURA OTTENUTO CON &registers CAMBIA SE INVOCATO PER DEVID O VENDOR ID ?????!?!?!?!?!?!
	logg << "dret tmp devid: " << tmp <<  endl;
	return tmp;
	//return read_reg_word(1);
	//return registers.deviceID;
}

uint32_t PCIDevice::getClassCode() {	
	uint32_t fullClassCode = 0;
	fullClassCode = (registers.classCode[0]) | (registers.classCode[1] << 8) | (registers.classCode[2] << 16);
	return fullClassCode;
}

uint32_t PCIDevice::getBar(uint8_t index) {
	switch (index){
		case 0:
			return registers.BAR0;
		case 1:
			return registers.BAR1;
		case 2:
			return registers.BAR2;
		case 3:
			return registers.BAR3;
		case 4:
			return registers.BAR4;
		case 5:
			return registers.BAR5;
		default:
			return 0; // ITS CORRECT???
	}
}

void PCIDevice::setBar(uint32_t value,uint8_t index) {
		switch (index){
			case 0:
				registers.BAR0 = value;
				break;
			case 1:
				registers.BAR1 = value;
				break;
			case 2:
				registers.BAR2 = value;
				break;
			case 3:
				registers.BAR3 = value;
				break;
			case 4:
				registers.BAR4 = value;
				break;
			case 5:
				registers.BAR5 = value;
				break;
			default:
				return;
		}
}

void PCIDevice::write_reg_byte(uint32_t offset, uint8_t val){ //offset, 1 = 1byte
	if(offset*8 + 8 > 255)
		return; //Out of bound
	deviceRegisters *regs = &registers;
	*((uint8_t*)(regs + offset*8)) = val;
}

void PCIDevice::write_reg_word(uint32_t offset, uint16_t val){
	if(offset*8 + 16 > 255)
		return; //Out of bound
	deviceRegisters *regs = &registers;
	*((uint16_t*)(regs + offset*8)) = val;
}

void PCIDevice::write_reg_long(uint32_t offset, uint32_t val){
	if(offset*8 + 32 > 255)
		return; //Out of bound
	deviceRegisters *regs = &registers;
	*((uint32_t*)(regs + offset*8)) = val;
}

uint8_t PCIDevice::read_reg_byte(uint32_t offset){
	if(offset*8 + 8 > 255)
		return 0; //Out of bound
	deviceRegisters *regs = &registers;
	return *((uint8_t*)(regs + offset*8));
}
uint16_t PCIDevice::read_reg_word(uint32_t offset){
	if(offset*8 + 16 > 255)
		return 0; //Out of bound
	deviceRegisters *regs = &registers;
	logg << "regs: " << regs <<  endl;
	return *((uint16_t*)(regs + offset*8));
}
uint32_t PCIDevice::read_reg_long(uint32_t offset){
	if(offset*8 + 32 > 255)
		return 0; //Out of bound
	deviceRegisters *regs = &registers;
	return *((uint32_t*)(regs + offset*8));
}