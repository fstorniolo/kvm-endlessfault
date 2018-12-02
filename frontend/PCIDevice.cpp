#include "PCIDevice.h"
		
PCIDevice::PCIDevice(uint16_t vendorID, uint16_t deviceID, uint32_t classCode){
	this->vendorID = vendorID;
	this->deviceID = deviceID;
	this->classCode = classCode;

	for(int i=0;i<6;i++)
		bar[i] = 0;

}

PCIDevice::PCIDevice(uint16_t vendorID, uint16_t deviceID){
	this->vendorID = vendorID;
	this->deviceID = deviceID;
}


uint16_t PCIDevice::getVendorID(){
	return vendorID;
}

uint16_t PCIDevice::getDeviceID(){
	return deviceID;
}

uint32_t PCIDevice::getClassCode(){
	return classCode;
}

io_addr PCIDevice::getBar(uint8_t index){
	if(index > 5)
		return -1;

	return bar[index];
}

void PCIDevice::setBar(io_addr value,uint8_t index){
	if(index >5)
		return;

	bar[index] = value;

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