#include "receiver.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char ip[] = "0.0.0.0";
	Receiver r(atoi(argv[2]), atoi(argv[3]), ip, atoi(argv[4]));
	r.openFile(argv[1]);
	r.listen();
	r.closeFile();
	return 0;
}