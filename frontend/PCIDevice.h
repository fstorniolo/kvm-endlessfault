#ifndef PCIDEVICE_H
#define PCIDEVICE_H
#include "IODevice.h"
#include <stdint.h>
#include <iostream>
#include "../backend/ConsoleLog.h"

extern ConsoleLog& logg;

struct deviceRegisters{
	uint16_t vendorID;
	uint16_t deviceID;

	uint16_t command;
	uint16_t status;

	uint8_t revisionID;
	uint8_t classCode[3];

	uint8_t cacheLineSize;
	uint8_t latencyTimer;
	uint8_t headerType;
	uint8_t BIST;

	uint32_t BAR0;
	uint32_t BAR1;
	uint32_t BAR2;
	uint32_t BAR3;
	uint32_t BAR4;
	uint32_t BAR5;

	uint32_t carbusCISPointer;

	uint16_t subsystemVendorID;
	uint16_t subsystemID;			

	uint32_t expansionROMBaseAddress;

	uint8_t capabilitiesPointer;
	uint8_t reserved[7];

	uint8_t interruptLine;
	uint8_t interruptPIN;
	uint8_t minGrant;
	uint8_t maxLatency;
}__attribute__((packed));

class PCIDevice : public IODevice {
	private:
		deviceRegisters registers;
	public:
		PCIDevice(uint16_t vendorID, uint16_t deviceID);
		PCIDevice(uint16_t vendorID, uint16_t deviceID,uint32_t classCode);
		uint16_t getVendorID();
		uint16_t getDeviceID();
		uint32_t getClassCode();
		uint32_t  getBar(uint8_t index);
		void 	 setBar(uint32_t  value,uint8_t index);

		void write_reg_byte(uint32_t offset, uint8_t val);
		void write_reg_word(uint32_t offset, uint16_t val);
		void write_reg_long(uint32_t offset, uint32_t val);
		uint8_t read_reg_byte(uint32_t offset);
		uint16_t read_reg_word(uint32_t offset);
		uint32_t read_reg_long(uint32_t offset);
};

#endif