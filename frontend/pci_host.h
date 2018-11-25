#ifndef PCI_HOST_H
#define PCI_HOST_H

#include "IODevice.h"
#include <stdint.h>
#include <pthread.h>
#include "../backend/ConsoleLog.h"

extern ConsoleLog& logg;


struct pci_device_function
{

	uint8_t registers[8];

	
};

struct pci_device
{

	pci_device_function functions[8];

	
};

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
		pthread_mutex_t mutex;

		//void process_cmd();
		void prepare_data();
		//uint32_t read_from_cdp();

		uint32_t device_number;
		uint32_t bus_number;			//sempre 0?
		uint32_t function_number;
		uint8_t offset_number;

		pci_device devices[32];			//bus 0;



public:
	pci_host();

	void write_reg_long(io_addr addr, uint32_t val);
	void write_reg_word(io_addr addr, uint16_t val);
	void write_reg_byte(io_addr addr, uint8_t val);
	
	uint32_t read_reg_long(io_addr addr);
	uint16_t read_reg_word(io_addr addr);
	uint8_t  read_reg_byte(io_addr addr);

};

#endif