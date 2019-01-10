#ifndef PCI_HOST_H
#define PCI_HOST_H

#include "IODevice.h"
#include "PCIDevice.h"
#include "HardDisk.h"
#include <stdint.h>
#include <pthread.h>
#include "../backend/ConsoleLog.h"

extern ConsoleLog& logg;

#define N_DEVICES 32
#define N_FUNCTIONS 8

class pci_host : public IODevice {

	private:
		// === Registri ===
		uint32_t CAP;	// configuration address port
		uint32_t CDP;	// configuration data port


		// indirizzi registri
		static const io_addr CAP_addr = 0x0CF8;
		static const io_addr CDP_addr = 0x0CFC;

		// === Stato interno ===
		bool enabled;
		bool interrupt_enabled;

		// mutex istanza (vale sia per frontend che backend)
		//pthread_mutex_t mutex;

		void prepare_data();

		uint32_t device_number;
		uint32_t bus_number;			//sempre 0?
		uint32_t function_number;
		uint32_t offset_number;

		PCIDevice* devices[N_DEVICES];
		uint32_t connected;



public:
	pci_host(PCIDevice** connected_PCI_devices);

	void write_reg_long(io_addr addr, uint32_t val);
	void write_reg_word(io_addr addr, uint16_t val);
	void write_reg_byte(io_addr addr, uint8_t val);
	
	uint32_t read_reg_long(io_addr addr);
	uint16_t read_reg_word(io_addr addr);
	uint8_t  read_reg_byte(io_addr addr);

};

#endif