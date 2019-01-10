//***************************************************************************************************
// file tas_int.cpp
#include <libce.h>
char buff_tas[80];
extern "C" void leggi_linea(int& nn, char vv[]);
int main()
{
	char c;
	int quanti = 80;
	leggi_linea(quanti, buff_tas);
	// ...
	for (int i = 0; i < quanti; i++)
		char_write(buff_tas[i]);
	pause();
	return 0;
}
