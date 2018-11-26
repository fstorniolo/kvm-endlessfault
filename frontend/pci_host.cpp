#include "pci_host.h"
#include <iostream>


pci_host::pci_host(PCIDevice** connected_PCI_devices) : CAP(0), CDP(0){
	//test connection between two modules
	logg << "pci_host: vendorID "<< connected_PCI_devices[0]->getVendorID() << ", deviceID " << connected_PCI_devices[0]->getDeviceID() << std::endl;

	for(int i=0;i<32;i++){
		for(int j=0;j<8;j++){

			uint8_t *ptr = &(devices[i].functions[j].registers[0]);

			uint16_t *new_ptr = reinterpret_cast<uint16_t*>(ptr);

			if(i==1 && j==0){						//dispositivo 1, funzione 0 bus 0
				//devices[i].functions[j].registers[0] = 0x8086;
				//devices[i].functions[j][1] = 0x8087;
				new_ptr[0] = 0x1234;
				new_ptr[1] = 0x1010;
			}
			else
				new_ptr[0] = -1;

		}
	}

	//pthread_mutex_init(&mutex, NULL);
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

	switch(addr) {
		//case CAP_addr: return 0xFFFFFFFF;  					//non dovrebbe esistere
		case CAP_addr: return CAP;  break;							//solo per debug

		case CDP_addr: 

			uint8_t *ptr = &(devices[device_number].functions[function_number].registers[0]);
			ptr += offset_number;

			uint32_t *new_ptr = reinterpret_cast<uint32_t*>(ptr);

			return *new_ptr;
	}

}

uint8_t pci_host::read_reg_byte(io_addr addr){

	switch(addr) {
		case CAP_addr: return -1;  					//non dovrebbe esistere
		//case CAP_addr: return CAP;  							//solo per debug
		case CDP_addr: 
			return devices[device_number].functions[function_number].registers[offset_number];
	}

}

uint16_t pci_host::read_reg_word(io_addr addr){
	uint8_t *ptr;
	uint16_t *new_ptr;

	switch(addr) {
		case CAP_addr: return -1; break;  		//NON HA SENSO!
		case CDP_addr: 

			ptr = &(devices[device_number].functions[function_number].registers[0]);
			ptr += offset_number;

			new_ptr = reinterpret_cast<uint16_t*>(ptr);
			return *new_ptr;
			break;

		case CDP_addr+2:
			//logg<<"EFFESS"<<endl;

			ptr = &(devices[device_number].functions[function_number].registers[0]);
			ptr += (offset_number + 2);

			new_ptr = reinterpret_cast<uint16_t*>(ptr);

			return *new_ptr;
	}

}

/*uint32_t pci_host::read_from_cdp_long(){

	return (uint32_t)(devices[device_number].functions[function_number][offset_number/2]);
}*/


void pci_host::prepare_data(){

	/*if((CAP & 0x80000000) !=  0x80000000){				//primo bit a uno; configurazione
		return;
	}*/

	//logg<<bitset<32>(CAP)<<endl;

	bus_number = 0;
	device_number = (CAP >> 11) & 0x0000001F;
	function_number = (CAP >> 8) & 0x00000007;
	offset_number = (CAP & 0x000000FF);

	//if(offset_number != 0)
	//	logg<<"HURRA"<<endl;

	//logg<<device_number<<" "<<function_number<<" ->"<<offset_number<<endl;
	//


}

/*
uint8_t keyboard::read_reg_byte(io_addr addr)
{
	//pthread_mutex_lock(&mutex);

	uint8_t result = 0;

	switch(addr) {
		case RBR_addr:
			// chiediamo a RBR e FI di aggiornare il proprio stato:
			// RBR è aggiornato con il valore da leggere,
			next_RBR();
			result = RBR;
			// FI è aggiornato in base a se ci sono ancora keycode
			// da prelevare dal buffer
			update_FI();
			break;
		case STR_addr:
			result= STR;
	}

	pthread_mutex_unlock(&mutex);

	return result;
}

void keyboard::process_cmd()
{
	switch(CMR) {
		// === configurazione tastiera ===
		case 0x60:
			if(TBR & 0x10)	// bit 4: disabilitazione tastiera
				enabled = false;
			else
				enabled = true;

			if(TBR & 0x01)	// bit 0: abilitazione interruzioni
				interrupt_enabled = true;
			else
				interrupt_enabled = false;
		break;
	}
}*/
