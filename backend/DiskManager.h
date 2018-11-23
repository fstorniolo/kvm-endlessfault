#define BLOCK_SIZE_BYTE 512
#define DEFAULT_NUMBER_BLOCKS 4096
#define FILE_NAME "disk.bin"
enum disk_status{NO_ERRORS,OUT_OF_BOUND,FILE_ERROR};

class DiskManager {
	private:
		uint32_t disk_size; // Size in byte of disk
		void build_disk(); // Build a disk of size 'disk_size'
		
	public:
		DiskManager();
		DiskManager(uint32_t blocks_number); // Constructor, require the number of blocks to create
		disk_status read(uint8_t *buffer, uint32_t block_index); // Read at block index in file and copy it into buffer
		disk_status write(uint8_t *buffer, uint32_t block_index); // Write in file the block at index position the content of buffer
};