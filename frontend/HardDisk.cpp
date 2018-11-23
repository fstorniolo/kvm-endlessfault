#include "HardDisk.h"
#include <iostream>

using namespace std;

HardDisk::HardDisk() :
 					
					BR(0),SCR(0),SNR(0),CNH(0),CNL(0),HND(0),ERR(0),STS(0),CMD(0),
					DCR(0),DAR(0),ASR(0),enabled(false),interrupt_enabled(false),
					current_position(0){};


void HardDisk::write_reg_byte(io_addr addr, uint8_t val)
{

	switch(addr) {
		case SCR_addr: SCR=val; break;
		case SNR_addr: SNR=val; break;
		case CNH_addr: CNH=val; break;
		case CNL_addr: CNL=val; break;
		case HND_addr: HND=val; break;
		case CMD_addr: CMD=val; break;	//this register will call some other function 
		case DCR_addr: DCR=val; break;


		//Other registers can't be write by the user, so we do not have any case here
	}

}


uint8_t HardDisk::read_reg_byte(io_addr addr)
{

	uint8_t result = 0;

	switch(addr) {
		case ERR_addr: result = ERR; break;
		case SCR_addr: result = SCR; break;
		case SNR_addr: result = SNR; break;
		case CNH_addr: result = CNH; break;
		case CNL_addr: result = CNL; break;
		case STS_addr: result = STS; break;
		case ASR_addr: result = ASR; break;
		case DAR_addr: result = DAR; break;

		//Other registers can't be read by the user, so we do not have any case here

	}


	return result;
}


void HardDisk::write_reg_word(io_addr addr, uint16_t val)
{
	//the only registers on 2 bytes that can be read is BR register
	switch(addr){
		case BR_addr: BR=val; break;
	}

	// here we have to call some other function to update the state of the emulated hard disk


}


uint16_t HardDisk::read_reg_word(io_addr addr)
{
	//the only registers on 2 bytes that can be written is BR register
	uint16_t result = 0;

	switch(addr){
		case BR_addr: result = BR; break;
	}

	return result;
	// here we have to call some other function to update the state of the emulated hard disk

}
