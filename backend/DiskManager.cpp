<<<<<<< HEAD
=======

/*     TO DO
	- Creare il file nel costruttore inizializzandolo con tutti 0
	- Capire come viene segnalato l'errrore dl file
	- Testare
*/

>>>>>>> 515009e282f598e128b647e9f5fd93a605302eef
#include <iostream>
#include <fstream>

#include "DiskManager.h"

using namespace std;

void DiskManager::build_disk(){
<<<<<<< HEAD
	uint8_t *buffer = new uint8_t[BLOCK_SIZE_BYTE]; // Used to fill disk 
	fstream fs;
	fs.open(FILE_NAME, fstream::in | fstream::out | fstream::binary | fstream::trunc); // Open stream and create file
	for(int i=0;i<disk_size; i+=BLOCK_SIZE_BYTE){ // Move ahead block by block
		fs.write(reinterpret_cast<char*>(buffer),BLOCK_SIZE_BYTE); // Fill file
	}
	fs.close(); // Close stream
	delete[] buffer; // Deallocate buffer
}



DiskManager::DiskManager() : DiskManager(DEFAULT_NUMBER_BLOCKS){};


DiskManager::DiskManager(uint32_t blocks_number){
	disk_size = blocks_number*BLOCK_SIZE_BYTE;
=======
	uint8_t *buffer = new uint8_t[BLOCK_SIZE_BYTE];
	// Fill the buffer with 0	
	for(uint8_t i=0;i<BLOCK_SIZE_BYTE; i++){
		buffer[i] = 0;
	}
	// Build the file disk
	for(uint32_t i=0;i<disk_size; i++){
		write(buffer,i);
	}
}

DiskManager::DiskManager() : DiskManager(DEFAULT_DISK_SIZE_MB*1024*1024){};


DiskManager::DiskManager(uint32_t size){
	disk_size = size;
>>>>>>> 515009e282f598e128b647e9f5fd93a605302eef
	build_disk(); // Build a disk of size 'disk_size'
}

// Read at sector index in file and copy it into buffer
<<<<<<< HEAD
disk_status DiskManager::read(uint8_t *buffer, uint32_t block_index){ 
	uint32_t block_address = block_index * BLOCK_SIZE_BYTE;
	if(block_address < 0 || block_address >= disk_size){ // Error, in of bound
		return OUT_OF_BOUND;
	}
	fstream fs;	
	fs.open(FILE_NAME, fstream::in | fstream::binary); // Open in stream	
	if(!fs.is_open()){ // Check if stream its open
		return FILE_ERROR;
	}
	fs.seekg(block_address); // Set position on disk in input stream
	fs.read(reinterpret_cast<char*>(buffer),BLOCK_SIZE_BYTE); // Read on file and copy the content in buffer
	fs.close();	// Close the stream
	return NO_ERRORS;
}	


disk_status DiskManager::write(uint8_t *buffer, uint32_t block_index){
	uint32_t block_address = block_index * BLOCK_SIZE_BYTE;
	if(block_address < 0 || block_address >= disk_size){ // Error, out of bound
		return OUT_OF_BOUND;
	}
	fstream fs;	
	fs.open(FILE_NAME, fstream::in | fstream::out | fstream::binary); // Open in stream	
	if(!fs.is_open()){ // Check if stream its open
		return FILE_ERROR;
	}
	fs.seekp(block_address); // Move to address of new block
	fs.write(reinterpret_cast<char*>(buffer),BLOCK_SIZE_BYTE); // Write new block in to the file	
	fs.close();	// Close the stream
	return NO_ERRORS; // All good
=======
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
>>>>>>> 515009e282f598e128b647e9f5fd93a605302eef
}

/*Note
	- fstream doesn't work with uint8_t, need to apply reinterpret_cast<char*>(buffer) before using read/write
*/