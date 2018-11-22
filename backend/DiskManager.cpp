
/*     TO DO
	- Creare il file nel costruttore inizializzandolo con tutti 0
	- Capire come viene segnalato l'errrore dl file
	- Testare
*/

#include <iostream>
#include <fstream>

#include "DiskManager.h"

using namespace std;

void DiskManager::build_disk(){
	// To do
}

DiskManager::DiskManager() : DiskManager(DEFAULT_DISK_SIZE_MB*1024*1024){};


DiskManager::DiskManager(uint32_t size){
	disk_size = size;
	build_disk(); // Build a disk of size 'disk_size'
}

// Read at sector index in file and copy it into buffer
disk_status DiskManager::read(uint8_t *buffer, uint32_t block_address){ 
	if(block_address < 0 || block_address >= disk_size){ // Error, out of bound
		return OUT_OF_BOUND;
	}
	ifstream input (FILE_NAME, ios::in | ios::binary); // Open output stream
	//int index = generate_index(head, sector, trace); // Generate the index for access disk in right position
	input.seekg(block_address); // Set position on disk in input stream
	input.read(reinterpret_cast<char*>(buffer),BLOCK_SIZE_BYTE); // Read on file and copy the content in buffer
	input.close();	// Close the stream
	return NO_ERRORS;
}	

// Write in file at sector position the content of buffer
disk_status DiskManager::write(uint8_t *buffer, uint32_t block_address){
	if(block_address < 0 || block_address >= disk_size){ // Error, out of bound
		return OUT_OF_BOUND;
	}
	ofstream output (FILE_NAME, ios::out | ios::binary);
	output.seekp(block_address); // Set position on disk in output stream
	output.write(reinterpret_cast<char*>(buffer),BLOCK_SIZE_BYTE); // Write on file the content of buffer
	output.close();	// Close the stream
	return NO_ERRORS;
}

/*Note
	- fstream doesn't work with uint8_t, need to apply reinterpret_cast<char*>(buffer) before using read/write
*/