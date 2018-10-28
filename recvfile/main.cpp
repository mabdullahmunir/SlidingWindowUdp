#include "utility.h"
#include "receiver.h"
#include <stdlib.h>

#define PORT 50000
#define WINDOWSIZE 10

int main(int argc, char const *argv[])
{
	Receiver r(PORT, WINDOWSIZE);
	r.sendack(100);
	return 0;
}