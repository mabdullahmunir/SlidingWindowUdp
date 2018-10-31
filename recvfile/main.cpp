#include "receiver.h"
#include <stdlib.h>

#define PORT 50000
#define WINDOWSIZE 4

int main(int argc, char *argv[])
{
	char ip[] = "0.0.0.0";
	Receiver r(ip, atoi(argv[4]), atoi(argv[2]));
	r.listen();
	r.saveToFile(argv[1]);
	return 0;
}