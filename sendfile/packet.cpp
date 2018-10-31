#include "packet.h"

Packet::Packet() {
	len = -1;
}

Packet::Packet(char * recvbuf) {
	header = recvbuf[0];
	if (header == 0x01) {
		memcpy(&seqnum, recvbuf+1, 4);
		memcpy(&len, recvbuf+5, 4);
		data = new char[len];
		memcpy(data, recvbuf+9, len);
		chksum = recvbuf[9+len];

		validchksum = (chksum == checksum(recvbuf, len+9));
	} else if (header == 0x06 || header == 0x07) {
		memcpy(&seqnum, recvbuf+1, 4);
		chksum = recvbuf[5];		
		validchksum = (chksum == checksum(recvbuf, 5));
	} else {

	}
}

Packet::~Packet() {

}

unsigned char Packet::getHeader() const {
	return header;
}

int Packet::getSeqNum() const {
	return seqnum;
}

int Packet::getLen() const {
	return len;
}

unsigned char Packet::getChkSum() const {
	return chksum;
}

char* Packet::getData() const {
	return data;
}

bool Packet::isValid() const {
	return validchksum;
}

bool Packet::getBuffer(char * buff) {
	int bLen = len;

	if (getHeader() == 0)
		return false;
	if (getHeader() == 0x01) {
		if (getLen() == -1)
			return false;
		if (getSeqNum() == 0)
			return false;
	}

	switch (getHeader()) {
		case 0x01 :
			bLen += 10;
			break;
		case 0x06 :
			bLen = 6;
			break;
		case 0x07 :
			bLen = 6;
			break;
	}

	memcpy(buff, &header, 1);
	memcpy(buff+1, &seqnum, 4);

	switch (getHeader()) {
		case 0x01 :
			memcpy(buff+5, &len, 4);
			memcpy(buff+9, data, len);
			break;
		case 0x06 :
			break;
		case 0x07 :
			break;
	}

	chksum = checksum(buff, bLen-1);

	memcpy(buff+bLen-1, &chksum, 1);
}

void Packet::setHeader(unsigned char pHeader) {
	header = pHeader;
}

void Packet::setSeqNum(int pSeqNum) {
	seqnum = pSeqNum;
}
void Packet::setData(char * pData, int pLen) {
	len = pLen;
	data = new char[len];
	memcpy(data, pData, len);
}

void Packet::printPacket() {
	printf("------------------\n");
	printf("Packet : %d\n", getSeqNum());
	printf("%d\n", getLen());
	printf("------------------\n");
}

bool Packet::operator<(Packet const& pPacket) {
	return (this->getSeqNum() < pPacket.getSeqNum());
}

unsigned char Packet::checksum(char* data, int len) {
	unsigned long int sum = 0;
	for (int i=0; i<len; i++) {
		sum += data[i];
	}
	sum = sum ^ 0xFF;
	return sum;
}