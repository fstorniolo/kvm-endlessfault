#ifndef PCIDEVICE_H
#define PCIDEVICE_H
#include "IODevice.h"
#include <stdint.h>

//TODO bar is 32 bit register
typedef uint16_t io_addr;

class PCIDevice : public IODevice {

	//TODO: it's needed a struct packed to emulate all the configuration space
	private:
		uint16_t vendorID;
		uint16_t deviceID;
		uint32_t classCode;
		io_addr bar[6];
	
	public:
		PCIDevice(uint16_t vendorID, uint16_t deviceID);
		PCIDevice(uint16_t vendorID, uint16_t deviceID,uint32_t classCode);
		uint16_t getVendorID();
		uint16_t getDeviceID();
		uint32_t getClassCode();
		io_addr getBar(uint8_t index);
		void 	 setBar(io_addr value,uint8_t index);

};

#endif