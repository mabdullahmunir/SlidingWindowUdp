#include "sender.h"
#include <stdlib.h>

#define PORT 50000
#define WINDOWSIZE 4

int main(int argc, char *argv[])
{
	Sender s(argv[4], atoi(argv[5]), atoi(argv[2]));
	s.readFile(argv[1]);
	pthread_t th;
	pthread_create(&th, NULL, s.listener, &s);
	s.slider();
	return 0;
}