#ifndef PACKET_H
#define PACKET_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utility.h"

class Packet
{
public:
    Packet();
    Packet(char *);
    ~Packet();

    unsigned char getHeader() const;
    int getSeqNum() const;
    char* getData() const;
    int getLen() const;
    unsigned char getChkSum() const;
    char* getBuffer() const;

    bool isValid() const;

    void setHeader(unsigned char);
    void setSeqNum(int);
    void setData(char *, int);
    
    void createBuffer();

    bool operator<(Packet const&);

private:
	unsigned char header;
	int seqnum;
	int len;
	char* data;
	unsigned char chksum;
	char* buffer;
	bool validchksum;
};

#endif