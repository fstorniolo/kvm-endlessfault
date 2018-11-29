#include <iostream>
#include <fstream>

#include "DiskManager.h"

using namespace std;

void DiskManager::build_disk(){
	uint8_t *buffer = new uint8_t[BLOCK_SIZE_BYTE]; // Used to fill disk 
	fstream fs;
	fs.open(FILE_NAME, fstream::out | fstream::binary | fstream::trunc); // Open stream and create file
	for(int i=0;i<disk_size; i+=BLOCK_SIZE_BYTE){ // Move ahead block by block
		fs.write(reinterpret_cast<char*>(buffer),BLOCK_SIZE_BYTE); // Fill file
	}
	fs.close(); // Close stream
	delete[] buffer; // Deallocate buffer
}

bool DiskManager::check_consistency(){ // If true, the file size is correct
	fstream fs;
	fs.open(FILE_NAME, fstream::in | fstream::binary); // Open stream
	fs.seekg (0,fs.end); // Move to the last byte
	uint64_t length = fs.tellg(); // Actual disk size
	fs.close(); // Close the stream
	return length == disk_size; // If true its correct
}


DiskManager::DiskManager() : DiskManager(DEFAULT_NUMBER_BLOCKS){};


DiskManager::DiskManager(uint32_t blocks_number){
	disk_size = blocks_number*BLOCK_SIZE_BYTE;
	if(!check_consistency()) //if file not exists
		build_disk(); // Build a disk of size 'disk_size'
}

// Read at sector index in file and copy it into buffer
disk_status DiskManager::read(uint8_t *buffer, uint32_t block_index){ 
	uint64_t block_address = block_index * BLOCK_SIZE_BYTE; // Extend to 64 bit: in worse case, file byte has = 2^24 index * 2^9 block size -> 2^33 possible value -> 33 bit; 
	if(block_address < 0 || block_address >= disk_size){ // Error, out of bound
		return OUT_OF_BOUND;
	}
	
	if(!check_consistency()){ // Check if file have correct size
		return FILE_ERROR_SIZE;
	}
	
	fstream fs;	
	fs.open(FILE_NAME, fstream::in | fstream::binary); // Open in stream	
	if(!fs.is_open()){ // Check if stream its open
		fs.close(); // Close the stream
		return FILE_ERROR_OPEN;
	}
	fs.seekg(block_address); // Set position on disk in input stream
	fs.read(reinterpret_cast<char*>(buffer),BLOCK_SIZE_BYTE); // Read on file and copy the content in buffer
	fs.close();	// Close the stream
	
	if(!check_consistency()){ // Check if file have correct size
		return FILE_ERROR_SIZE;
	}
	
	return NO_ERRORS; // All good
}	


disk_status DiskManager::write(uint8_t *buffer, uint32_t block_index){
	uint64_t block_address = block_index * BLOCK_SIZE_BYTE; // Extend to 64 bit: in worse case, file byte has = 2^24 index * 2^9 block size -> 2^33 possible value -> 33 bit; 
	if(block_address < 0 || block_address >= disk_size){ // Error, out of bound
		return OUT_OF_BOUND;
	}
	
	if(!check_consistency()){ // Check if file have correct size
		return FILE_ERROR_SIZE;
	}
	
	fstream fs;	
	fs.open(FILE_NAME, fstream::in | fstream::out | fstream::binary); // Open in/out stream	
	if(!fs.is_open()){ // Check if stream its open
		fs.close(); // Close the stream
		return FILE_ERROR_OPEN;
	}
	fs.seekp(block_address); // Move to address of new block
	fs.write(reinterpret_cast<char*>(buffer),BLOCK_SIZE_BYTE); // Write new block in to the file	
	fs.close();	// Close the stream
	
	if(!check_consistency()){ // Check if file have correct size
		return FILE_ERROR_SIZE;
	}
	
	return NO_ERRORS; // All good
}

/*Note
	- fstream doesn't work with uint8_t, need to apply reinterpret_cast<char*>(buffer) before using read/write
*/