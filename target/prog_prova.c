#include "lib.h"
#include <stdint.h>

int main()
{
	unsigned int a=2, b=10;
	unsigned int c = a+b;
	uint64_t d = 100*1024*1024; // 100MiB
	

	//ptr = (unsigned long)d;
	char *ptr = (char*)(d);
	char k = *ptr;	


	exit(c);

	// non ci arriviamo mai
	return c;
}
