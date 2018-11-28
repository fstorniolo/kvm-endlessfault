//****************************************************************************************************
// file leggiset.cpp
#include <libce.h>

void cerca_bm(natb& dev, natb& fun)
{	for (dev = 0; dev < 32; dev++)
		for (fun = 0; fun < 8; fun++)
			if (pci_read_confw(0, dev, fun, 10) == 0x0101)
				return; 
}

ioaddr bar0,bar1;

const ioaddr iBR =  0x01F0 - 0x01F0;
const ioaddr iCNL = 0x01F4 - 0x01F0;
const ioaddr iCNH = 0x01F5 - 0x01F0;
const ioaddr iSNR = 0x01F3 - 0x01F0;
const ioaddr iHND = 0x01F6 - 0x01F0;
const ioaddr iSCR = 0x01F2 - 0x01F0;
const ioaddr iERR = 0x01F1 - 0x01F0;
const ioaddr iCMD = 0x01F7 - 0x01F0;
const ioaddr iSTS = 0x01F7 - 0x01F0;

const ioaddr iDCR = 0x03F6 - 0x03F6;

void leggisett(natl lba, natb quanti, natw vetti[])
{	
	natb lba_0 = lba,
		lba_1 = lba >> 8,
		lba_2 = lba >> 16,
		lba_3 = lba >> 24;
	natb s;
	do 
		inputb(iSTS+bar0, s);
	while (s & 0x80);

	outputb(lba_0, iSNR + bar0); 			// indirizzo del settore e selezione drive
	outputb(lba_1, iCNL + bar0);
	outputb(lba_2, iCNH + bar0);
	natb hnd = (lba_3 & 0x0F) | 0xE0;
	outputb(hnd, iHND + bar0);
	outputb(quanti, iSCR + bar0); 			// numero di settori
	outputb(0x0A, iDCR + bar1);			// disabilitazione richieste di interruzione
	outputb(0x20, iCMD + bar0);			// comando di lettura

	for (int i = 0; i < quanti; i++) {
		do 
			inputb(iSTS + bar0, s);
		while ((s & 0x88) != 0x08);
		for (int j = 0; j < 512/2; j++)
			inputw(iBR, vetti[i*512/2 + j]);
	}
}

natb buff[8*512];
int main()
{

	natb dev,fun;
	cerca_bm(dev,fun);

	bar0 = pci_read_confl(0,dev,fun,16);
	bar1 = pci_read_confl(0,dev,fun,20);


	natl lba = 1;
	natb quanti = 2;						// massimo 8
	leggisett(lba, quanti, reinterpret_cast<natw*>(buff));
	for (int i = 0; i < quanti*512; i++)
		char_write(buff[i]);
	char_write('\n');
	pause();
	// ...
	return 0;
}
//****************************************************************************************************

