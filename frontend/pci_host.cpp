#include "pci_host.h"
#include <iostream>

using namespace std;

pci_host::pci_host() : CAP(0), CDP(0){

	for(int i=0;i<32;i++){
		for(int j=0;j<8;j++){
			if(i==1 && j==0){						//dispositivo 1, funzione 0 bus 0
				devices[i].functions[j][0] = 0x8086;
				devices[i].functions[j][1] = 0x8087;
			}
			else
				devices[i].functions[j][0] = -1;

		}
	}

	//pthread_mutex_init(&mutex, NULL);
}

void pci_host::write_reg(io_addr addr, uint32_t val);

{
	//pthread_mutex_lock(&mutex);

	switch(addr) {
		case CAP_addr: CAP=val; prepare_data(); break;
		case CDP_addr: break;// diversi tipi di scrittura
	}

	//pthread_mutex_unlock(&mutex);
}

uint32_t pci_host::read_reg(io_addr addr){

	switch(addr) {
		//case CAP_addr: return 0xFFFFFFFF;  					//non dovrebbe esistere
		case CAP_addr: return CAP;  							//solo per debug
		case CDP_addr: return read_from_cdp();
	}

}

uint32_t pci_host::read_from_cdp(){

	return devices[device_number].functions[function_number][offset_number/2];
}


void pci_host::prepare_data(){

	/*if((CAP & 0x80000000) !=  0x80000000){				//primo bit a uno; configurazione
		return;
	}*/

	bus_number = 0;
	device_number = (CAP >> 11) & 0x0000001F;
	function_number = (CAP >> 8) & 0x00000007;
	offset_number = CAP & 0x000000FF;


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
