#include "pci_host.h"
#include <iostream>
#include <bitset>

using namespace std;

pci_host::pci_host(PCIDevice** connected_PCI_devices) : CAP(0), CDP(0),connected(0){
	//test connection between two modules
	//connected = 0;


	for(uint32_t i=0;i<N_DEVICES;i++)
		devices[i] = NULL;

	PCIDevice *temp = connected_PCI_devices[0];

	//caso generale con più dispositii connessi

	/*while(temp != NULL){
		devices[connected++] = temp;
		temp = temp++;
	}*/

	devices[0] = temp;
	connected++;

	//PER FILIPPO, CREDO VADA FATTO ALTROVE
	devices[0]->setBar(0x01F0,0);
	devices[0]->setBar(0x03F6,1);
	//fine


	logg<<"Connected devices: "<<connected<<endl;
	if(connected > 0)
		logg << "pci_host: vendorID "<< hex<<devices[0]->getVendorID() << ", deviceID " << hex<<devices[0]->getDeviceID() << endl;

}

void pci_host::write_reg_long(io_addr addr, uint32_t val)
{
	//pthread_mutex_lock(&mutex);

	switch(addr) {
		case CAP_addr: 
			CAP=val; 
			prepare_data(); 
			break;

		case CDP_addr: break;			//TODO
	}

	//pthread_mutex_unlock(&mutex);
}

void pci_host::write_reg_word(io_addr addr, uint16_t val)
{
	//pthread_mutex_lock(&mutex);

	switch(addr) {
		case CAP_addr: break;
		case CDP_addr: break;			//TODO
	}

	//pthread_mutex_unlock(&mutex);
}

void pci_host::write_reg_byte(io_addr addr, uint8_t val)

{
	//pthread_mutex_lock(&mutex);

	switch(addr) {
		case CAP_addr: break;
		case CDP_addr: break;			//TODO
	}

	//pthread_mutex_unlock(&mutex);
}

uint32_t pci_host::read_reg_long(io_addr addr){

	PCIDevice *tmp;

	switch(addr) {
		//case CAP_addr: return 0xFFFFFFFF;  					//non dovrebbe esistere
		case CAP_addr: return CAP;  break;							//solo per debug

		case CDP_addr:

			tmp = devices[device_number];

			if(tmp == NULL or function_number != 0){
				return 0xFFFFFFFF;
			}

			if(offset_number == 16){
				return tmp->getBar(0);
			}
			else if(offset_number == 20){
				return tmp->getBar(1);
			}

			break;
			
	}

}

uint8_t pci_host::read_reg_byte(io_addr addr){

	switch(addr) {
		case CAP_addr: return -1;  					//non dovrebbe esistere
		case CDP_addr: break;
	}

}

uint16_t pci_host::read_reg_word(io_addr addr){
	uint8_t *ptr;
	uint16_t *new_ptr;
	PCIDevice *tmp;

	switch(addr) {
		case CAP_addr: return -1; break;  		//NON HA SENSO!
		case CDP_addr: 

			tmp = devices[device_number];

			if(tmp == NULL or function_number != 0){			//emulating
				return 0xFFFF;
			}

			if(offset_number == 0)
				return tmp->getDeviceID();

			break;

		case CDP_addr+2:

			tmp = devices[device_number];
			if(tmp == NULL)
				return -1;
			
			if(offset_number == 0)
				return tmp->getVendorID();
			else if(offset_number == 8){
				return tmp->getClassCode();
			}

	}

}

/*uint32_t pci_host::read_from_cdp_long(){

	return (uint32_t)(devices[device_number].functions[function_number][offset_number/2]);
}*/


void pci_host::prepare_data(){

	bus_number = 0;
	device_number = (CAP >> 11) & 0x0000001F;
	function_number = (CAP >> 8) & 0x00000007;
	offset_number = (CAP & 0x000000FF);

	//logg<<bitset<32>(CAP)<<endl;
	//logg<<offset_number<<endl;

}
