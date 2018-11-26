#ifndef PCIDEVICE_H
#define PCIDEVICE_H
#include "IODevice.h"
#include <stdint.h>

typedef uint16_t io_addr;

class PCIDevice : public IODevice {

	private:
		uint16_t vendorID;
		uint16_t deviceID;
	
	public:
		PCIDevice(uint16_t vendorID, uint16_t deviceID);
		uint16_t getVendorID();
		uint16_t getDeviceID();
};

#endif