#include "utility.h"
#include "receiver.h"
#include <stdlib.h>

#define PORT 50000
#define WINDOWSIZE 4

int main(int argc, char const *argv[])
{
	Receiver r(PORT, WINDOWSIZE);
	r.listen();
	r.saveToFile("hmmm");
	return 0;
}