#include "PCIDevice.h"
		
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

