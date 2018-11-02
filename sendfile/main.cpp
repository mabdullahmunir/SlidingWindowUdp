#include "sender.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
	Sender s(atoi(argv[2]), atoi(argv[3]), argv[4], atoi(argv[5]));
	s.openFile(argv[1]);
	pthread_t th;
	pthread_create(&th, NULL, s.listener, &s);
	s.slider();
	s.closeFile();
	return 0;
}