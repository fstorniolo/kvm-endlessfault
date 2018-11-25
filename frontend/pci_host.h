#include "IODevice.h"
#include <stdint.h>
#include <pthread.h>


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

private:
	void process_cmd();


public:
	pci_host();

	void write_reg(io_addr addr, uint32_t val);
	uint32_t read_reg_byte(io_addr addr);

};

#endif