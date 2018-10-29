#ifndef RECEIVER_H
#define RECEIVER_H

#define BUFFER_SIZE 10

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include "utility.h"
#include "UdpServer.h"
#include "packet.h"

class Receiver
{
public:
    Receiver(int, int);
    ~Receiver();

    void sendack(int, bool);
    void saveToFile(char *);

    void listen();


private:
	int len;
	int rws;
	int buffersize;
	UdpServer rserver;
	sockaddr_in recvaddr;
	std::vector<Packet> datastorage;
};

#endif