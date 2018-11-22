#define BLOCK_SIZE_BYTE 512
#define DEFAULT_DISK_SIZE_MB 2
#define FILE_NAME "disk.bin"
enum disk_status{NO_ERRORS,OUT_OF_BOUND,FILE_ERROR};

class DiskManager {
	private:
		uint32_t disk_size; // Size in byte of disk
		void build_disk(); // Build a disk of size 'disk_size'
		
	public:
		DiskManager();
		DiskManager(uint32_t size);
		disk_status read(uint8_t *buffer, uint32_t block_address); // Read at sector index in file and copy it into buffer
		disk_status write(uint8_t *buffer, uint32_t block_address); // Write in file at sector position the content of buffer
};