#ifndef RECEIVER_H
#define RECEIVER_H

#define BUFFER_SIZE 1024

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <deque>
#include "UdpServer.h"
#include "packet.h"

class Receiver
{
public:
    Receiver(int, int, char *, int);
    ~Receiver();

	void openFile(char *);
	void writeFile();
	void closeFile();
    void sendack(int, bool);

    void listen();


private:
	FILE * fout;
	int rws;
	int bufferlength;
	UdpServer rserver;
	sockaddr_in recvaddr;
	std::deque<Packet> datastorage;
};

#endif