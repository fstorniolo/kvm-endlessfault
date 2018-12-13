#include "pci_host.h"
#include <iostream>
#include <bitset>

using namespace std;

pci_host::pci_host(PCIDevice** connected_PCI_devices) : CAP(0), CDP(0),connected(0){
	//test connection between two modules
	//connected = 0;
	uint32_t i;

	for(i=0;i<N_DEVICES;i++)
		devices[i] = NULL;

	for(i=0;i<32;i++){
		if(connected_PCI_devices[i] == nullptr)
			continue;


		devices[connected] = connected_PCI_devices[i];
		connected++;
	}

	
	//After implementation move this code in kvm.cpp after constructor of PCI Host

	for(i=0;i<connected;i++){
		if(devices[i]->getClassCode() == 0x010100){				//PCI-ATA
			devices[i]->setBar(0x01F0,0);
			devices[i]->setBar(0x03F6,1);
		}
	}

	logg<<"Connected devices: "<<connected<<endl;
	if(connected > 0){
		logg << "pci_host: vendorID "<< hex<<devices[0]->getVendorID() << ", deviceID " << hex<<devices[0]->getDeviceID() << endl;
		logg << "pci_apic: vendorID "<< hex<<devices[1]->getVendorID() << ", deviceID " << hex<<devices[1]->getDeviceID() << endl;

	}

}

void pci_host::write_reg_long(io_addr addr, uint32_t val)
{

	PCIDevice *tmp;

	if(addr == CAP_addr){
		CAP=val; 
		prepare_data(); 
		return;	
	}

	tmp = devices[device_number];
	if(tmp == NULL or function_number != 0){							//just one function
		return;
	}

	tmp->write_reg_long_PCI(offset_number + addr-CDP_addr,val);


/*	switch(addr) {
		case CAP_addr: 
			CAP=val; 
			prepare_data(); 
			break;

		case CDP_addr: break;			//TODO
	}*/

}

void pci_host::write_reg_word(io_addr addr, uint16_t val)
{

	PCIDevice *tmp;

	if(addr == CAP_addr){
		return;	
	}

	tmp = devices[device_number];
	if(tmp == NULL or function_number != 0){							//just one function
		return;
	}

	tmp->write_reg_word_PCI(offset_number + addr-CDP_addr,val);

	/*switch(addr) {
		case CAP_addr: break;
		case CDP_addr: break;			//TODO
	}*/

}

void pci_host::write_reg_byte(io_addr addr, uint8_t val)

{

	PCIDevice *tmp;

	if(addr == CAP_addr){
		return;	
	}

	tmp = devices[device_number];
	if(tmp == NULL or function_number != 0){							//just one function
		return;
	}

	tmp->write_reg_byte_PCI(offset_number + addr-CDP_addr,val);
}


uint32_t pci_host::read_reg_long(io_addr addr){

	PCIDevice *tmp;

	if(addr == CAP_addr){
		return CAP;
	}

	tmp = devices[device_number];
	if(tmp == NULL or function_number != 0){							//just one function
		logg << "print tmp in pci host: "<<tmp<<endl;
		return 0xFFFFFFFF;
	}

	return tmp->read_reg_long_PCI(offset_number + addr-CDP_addr);


}

uint8_t pci_host::read_reg_byte(io_addr addr){

	PCIDevice *tmp;

	if(addr == CAP_addr){
		return -1;
	}

	//CASE CDP

	tmp = devices[device_number];
	if(tmp == NULL or function_number != 0){							//just one function
		logg << "print tmp in pci host: "<<tmp<<endl;
		return 0xFF;
	}

	return tmp->read_reg_byte_PCI(offset_number + addr-CDP_addr);

}


uint16_t pci_host::read_reg_word(io_addr addr){

	PCIDevice *tmp;

	if(addr == CAP_addr){
			return -1;
	}

	tmp = devices[device_number];
	
	if(bus_number != 0 or tmp == NULL or function_number != 0){			//emulating
		return 0xFFFF;
	}

	return tmp->read_reg_word_PCI(offset_number + addr-CDP_addr);

}



void pci_host::prepare_data(){

	bus_number = (CAP >> 16) & 0x000000FF;
	device_number = (CAP >> 11) & 0x0000001F;
	function_number = (CAP >> 8) & 0x00000007;
	offset_number = (CAP & 0x000000FF);

}
