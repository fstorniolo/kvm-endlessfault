#ifndef HARD_DISK_H
#define HARD_DISK_H
#define HDD_VENDOR_ID 7777
#define HDD_DEVICE_ID 8888

#include "PCIDevice.h"
#include "../backend/DiskManager.h"
#include "../backend/ConsoleLog.h"



extern ConsoleLog& logg;


class HardDisk : public PCIDevice {
private:
	// === Registers ===
	uint16_t BR;	// buffer that contains data to read or to write
	uint8_t SCR;	// registers that contains the numbers of sector to read or to write
	uint8_t SNR;	
	uint8_t CNH;	
	uint8_t CNL;
	uint8_t HND;
	uint8_t ERR;	// contains the type of error that occured
	uint8_t STS;	// contains the disk state 
	uint8_t CMD;	// contains the command

	uint8_t DCR;	// contains the bit that allows interrupts
	uint8_t DAR;	// here for compatibility
	uint8_t ASR;	// here for compatibility
   	// indirizzi registri
	static const io_addr BR_addr = 0x01F0;
	static const io_addr ERR_addr = 0x01F1;
	static const io_addr SCR_addr = 0x01F2;
	static const io_addr SNR_addr = 0x01F3;
	static const io_addr CNL_addr = 0x01F4;
	static const io_addr CNH_addr =	0x01F5;
	static const io_addr HND_addr = 0x01F6;
	static const io_addr CMD_addr = 0x01F7;
	static const io_addr STS_addr = 0x01F7;
	static const io_addr DCR_addr = 0x03F6;
	static const io_addr ASR_addr = 0x03F6;
	static const io_addr DAR_addr = 0x03F7;


	static const uint8_t BUSY_MASK = 0x80;
	static const uint8_t DRQ_MASK = 0x08;

	// === Internal state ===
	bool interrupt_enabled;

	uint8_t internal_buffer[BLOCK_SIZE_BYTE];
	int current_position;
	int sector_numbers_cmd;
	int current_sector_number;
	uint32_t lba;
	uint32_t num_sector_hdd;
	DiskManager disk_manager;

	//bool interrupt_raised;


private:
	void process_cmd();
	void write_BR_register(uint16_t val);
	uint16_t read_BR_register();
	void clean_all_registers();

public:
	HardDisk(uint32_t num_sectors);

	void write_reg_byte(io_addr addr, uint8_t val);
	uint8_t read_reg_byte(io_addr addr);
	void write_reg_word(io_addr addr, uint16_t val);
	uint16_t read_reg_word(io_addr addr);
	uint32_t compute_lba();

	// serve al backend per pushare eventi di inserimento come handler
	//void insert_keycode_event(uint8_t keycode);
};

#endif