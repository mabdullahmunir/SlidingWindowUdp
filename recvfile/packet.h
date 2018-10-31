#ifndef PACKET_H
#define PACKET_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

class Packet
{
public:
    Packet();
    Packet(char *);
    ~Packet();

    unsigned char getHeader() const;
    int getSeqNum() const;
    int getLen() const;
    unsigned char getChkSum() const;
    char* getData() const;

    bool isValid() const;
    bool getBuffer(char*);

    void setHeader(unsigned char);
    void setSeqNum(int);
    void setData(char *, int);
    
    bool operator<(Packet const&);

    static unsigned char checksum(char* data, int len);

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