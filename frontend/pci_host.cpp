#include "pci_host.h"
#include <iostream>

using namespace std;

keyboard::keyboard() : CAP(0), CDP(0){
	//pthread_mutex_init(&mutex, NULL);
}

void keyboard::write_reg(io_addr addr, uint32_t val);

{
	//pthread_mutex_lock(&mutex);

	switch(addr) {
		case CAP_addr: CAP=val; break;
		case CDP_addr: break;// diversi tipi di scrittura
	}

	//pthread_mutex_unlock(&mutex);
}

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
}
