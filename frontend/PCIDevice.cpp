#include "PCIDevice.h"
using namespace std;
		
PCIDevice::PCIDevice(uint16_t vendorID, uint16_t deviceID, uint32_t classCode) {
	registers.vendorID = vendorID;
	registers.deviceID = deviceID;
	/*logg << "--->classCode 1:  " << classCode <<endl;
	registers.classCode[0] = (uint8_t)(classCode >> 8);
	logg << "--->classCode 2:  " << classCode <<endl;
	registers.classCode[1] = (uint8_t)(classCode >> 16);
	logg << "--->classCode 3:  " << classCode <<endl;
	registers.classCode[2] = (uint8_t)(classCode >> 24);
	logg << "--->classCode 4:  " << classCode <<endl;*/
	registers.classCode = classCode;
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
	return registers.vendorID;
}

uint16_t PCIDevice::getDeviceID() {
	return registers.deviceID;
}

uint32_t PCIDevice::getClassCode() {	
	//uint32_t fullClassCode = 0;
	//fullClassCode = (registers.classCode[0] << 8) | (registers.classCode[1] << 8) | (registers.classCode[2] << 8);
	return registers.classCode;
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
		if(index >5)
			return;

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
		}
}

/*void PCIDevice::write_reg_byte(io_addr addr, uint8_t val){
}
void PCIDevice::write_reg_word(io_addr addr, uint16_t val){
}
void PCIDevice::write_reg_long(io_addr addr, uint32_t val){
}
uint8_t PCIDevice::read_reg_byte(io_addr addr){
}
uint16_t PCIDevice::read_reg_word(io_addr addr){
}
uint32_t PCIDevice::read_reg_long(io_addr addr){
}*/