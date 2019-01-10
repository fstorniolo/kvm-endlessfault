#include "PCI_ATA.h"
#include <iostream>
#include <bitset>

using namespace std;


PCI_ATA::PCI_ATA(HardDisk *hdd_instance): PCIDevice(ATA_VENDOR_ID, ATA_DEVICE_ID, ATA_CLASS_ID) {
	hdd = hdd_instance;
	BMCMD = BMSTR = BMDTPR = 0;

};

//Since Register are mapped wherever the target chooses, I convert them to normal ATA addresses

void PCI_ATA::write_reg_word(io_addr addr, uint16_t val){
	io_addr bar0,bar1,bar4;
	bar0 = this->getBar(0);
	bar1 = this->getBar(1);
	bar4 = this->getBar(4);

	if(addr >= bar4 && addr <= bar4+4){				//bus mastering
		if(addr == bar4){
			BMCMD = (uint16_t)val;
			//process_cmd();
		}
		else if(addr == bar4+BMSTR_OFFSET)
			BMSTR = (uint16_t)val;

		return;
	}

	if(addr >= bar0 && addr <= bar0+7)
		addr = addr - bar0 + 0x01F0;
	else if(addr >= bar1 && addr <= bar1+1)
		addr = addr - bar1 + 0x03F6;

	hdd->write_reg_word(addr,val);
}

uint16_t PCI_ATA::read_reg_word(io_addr addr){
	io_addr bar0,bar1,bar4;
	bar0 = this->getBar(0);
	bar1 = this->getBar(1);
	bar4 = this->getBar(4);

	if(addr >= bar4 && addr <= bar4+4){				//bus mastering
		if(addr == bar4)
			return BMCMD;
		else if(addr == bar4+BMSTR_OFFSET)
			return BMSTR;
		//else if(addr == bar4+BMDTPR_OFFSET)
		//	return BMDTPR;
	}

	if(addr >= bar0 && addr <= bar0+7)
		addr = addr - bar0 + 0x01F0;
	else if(addr >= bar1 && addr <= bar1+1)
		addr = addr - bar1 + 0x03F6;

	return hdd->read_reg_word(addr);
}

void PCI_ATA::write_reg_byte(io_addr addr, uint8_t val){

	io_addr bar0,bar1,bar4;
	bar0 = this->getBar(0);
	bar1 = this->getBar(1);
	bar4 = this->getBar(4);

	if(addr >= bar4 && addr <= bar4+4){				//bus mastering
		if(addr == bar4){
			BMCMD = (uint16_t)val;
			process_cmd();
		}
		else if(addr == bar4+BMSTR_OFFSET)
			BMSTR = (uint16_t)val;

		return;
	}

	if(addr >= bar0 && addr <= bar0+7)
		addr = addr - bar0 + 0x01F0;
	else if(addr >= bar1 && addr <= bar1+1)
		addr = addr - bar1 + 0x03F6;

	hdd->write_reg_byte(addr,val);
}

uint8_t PCI_ATA::read_reg_byte(io_addr addr){

	io_addr bar0,bar1,bar4;
	bar0 = this->getBar(0);
	bar1 = this->getBar(1);
	bar4 = this->getBar(4);

	if(addr >= bar4 && addr <= bar4+4){				//bus mastering
		if(addr == bar4)
			return (uint8_t)BMCMD;
		else if(addr == bar4+BMSTR_OFFSET)
			return (uint8_t)BMSTR;
		//else if(addr == bar4+BMDTPR_OFFSET)
		//	return BMDTPR;
	}

	if(addr >= bar0 && addr <= bar0+7)
		addr = addr - bar0 + 0x01F0;
	else if(addr >= bar1 && addr <= bar1+1)
		addr = addr - bar1 + 0x03F6;

	return hdd->read_reg_byte(addr);
}

void PCI_ATA::write_reg_long(io_addr addr, uint32_t val){
	io_addr bar0,bar1,bar4;
	bar0 = this->getBar(0);
	bar1 = this->getBar(1);
	bar4 = this->getBar(4);

	if(addr >= bar4 && addr <= bar4+4){				//bus mastering
		if(addr == bar4){
		//	BMCMD = val;			
		//	process_cmd();
		}
		else if(addr == bar4 + BMSTR_OFFSET)
			BMSTR = val;
		else if(addr ==bar4 + BMDTPR_OFFSET){
			BMDTPR = val;
			//logg<<"scritto ptr "<<val<<endl;

/*
			uint8_t *ptr = &guest_memory[val];
			buffer_descriptor *struttura = reinterpret_cast<buffer_descriptor*>(ptr);


			logg<<"buffer size "<<endl;
			logg<<(uint32_t)(struttura->buffer_size)<<endl;

			logg<<"buffer addr "<<endl;
			logg<<(uint32_t)(struttura->buffer_addr)<<endl;

			uint8_t *carattere = &guest_memory[struttura->buffer_addr];
			logg<<"carattere "<<*carattere<<endl;
*/


		}

		return;
	}


	if(addr >= bar0 && addr <= bar0+7)
		addr = addr - bar0 + 0x01F0;
	else if(addr >= bar1 && addr <= bar1+1)
		addr = addr - bar1 + 0x03F6;

	hdd->write_reg_long(addr,val);
}

uint32_t PCI_ATA::read_reg_long(io_addr addr){

	io_addr bar0,bar1,bar4;
	bar0 = this->getBar(0);
	bar1 = this->getBar(1);
	bar4 = this->getBar(4);

	if(addr >= bar4 && addr <= bar4+4){				//bus mastering
		if(addr == bar4)
			return BMCMD;
		else if(addr == bar4+BMSTR_OFFSET)
			return BMSTR;
		else if(addr == bar4+BMDTPR_OFFSET)
			return BMDTPR;
	}

	if(addr >= bar0 && addr <= bar0+7)
		addr = addr - bar0 + 0x01F0;
	else if(addr >= bar1 && addr <= bar1+1)
		addr = addr - bar1 + 0x03F6;

	return hdd->read_reg_long(addr);
}


void PCI_ATA::process_cmd(){
	logg<<"SCRITTO CMD HOST "<<endl;
	logg<<bitset<16>(BMCMD)<<endl;
	
	if(BMCMD & 0x1)
		logg<<"bus mastering start"<<endl;
	else{
		logg<<"bus mastering stops"<<endl;
	}

}

void PCI_ATA::setVmem(uint8_t* addr){

	guest_memory = addr;

}

void PCI_ATA::sendInterrupt(uint16_t irq, uint16_t level){

	logg<<"effess uaglio"<<endl;

	//set_IRQline(irq,level);

}



