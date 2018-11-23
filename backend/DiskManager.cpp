#include <iostream>
#include <fstream>

#include "DiskManager.h"

using namespace std;

void DiskManager::build_disk(){
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
	build_disk(); // Build a disk of size 'disk_size'
}

// Read at sector index in file and copy it into buffer
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
}

/*Note
	- fstream doesn't work with uint8_t, need to apply reinterpret_cast<char*>(buffer) before using read/write
*/