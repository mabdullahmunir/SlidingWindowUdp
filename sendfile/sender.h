#ifndef SENDER_H
#define SENDER_H

#define BUFFER_SIZE 1024
#define TIMEOUT 2

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <vector>
#include <mutex>
#include "UdpClient.h"
#include "packet.h"

class Sender
{
public:
    Sender(char *, int, int);
    ~Sender();

    void openFile(char *);
    void readFile(char *);

    void slider();

    static void *listener(void *);

private:
	FILE * fin;
	int len;
	int lws;
	int windowsize;
	int buffersize;
	bool status;
	UdpClient rclient;
	std::mutex mtx;
	std::vector<Packet> datastorage;
	std::vector<int> acks;
};

#endif