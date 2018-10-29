#ifndef SENDER_H
#define SENDER_H

#define BUFFER_SIZE 10
#define TIMEOUT 5

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <vector>
#include "utility.h"
#include "UdpClient.h"
#include "packet.h"

class Sender
{
public:
    Sender(int, int);
    ~Sender();

    void readFile(char *);

    void slider();

    static void *listener(void *);

private:
	int len;
	int lws;
	int windowsize;
	int buffersize;
	bool status;
	UdpClient rclient;
	std::vector<Packet> datastorage;
	std::vector<int> acks;
};

#endif