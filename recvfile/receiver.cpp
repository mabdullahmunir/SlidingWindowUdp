#include "receiver.h"

Receiver::Receiver(int windowsize, int bufferlength, char* ip, int port) : rserver(ip, port) {
	rws = windowsize;
	this->bufferlength = bufferlength;
}

Receiver::~Receiver() {

}

void Receiver::openFile(char * filename) {
	fout = fopen(filename, "ab");
}

void Receiver::writeFile() {
	sort(datastorage.begin(), datastorage.end());
	for (int i=0; i<datastorage.size(); i++) {
		fwrite(datastorage[i].getData(), sizeof(char), datastorage[i].getLen(), fout);
	}
}

void Receiver::closeFile() {
	fclose(fout);
}

void Receiver::sendack(int seq, bool status) {
	char buffer[BUFFER_SIZE+10];
	Packet p;
	status ? p.setHeader(0x06) : p.setHeader(0x07);
	p.setSeqNum(seq);
	p.getBuffer(buffer);
	rserver.send(buffer, 6, recvaddr);
}

void Receiver::listen() {
	uint recvaddrlen = sizeof(sockaddr_in);
	char msgrecv[BUFFER_SIZE+10];
	bool hasEndOfPacketFound = false;

	while (true) {
		while (datastorage.size() < bufferlength) {
			if (rserver.recvfrom(msgrecv, BUFFER_SIZE+10, recvaddr, recvaddrlen) > 0) {
				Packet precv(msgrecv);

				printf("[+] Received new packet\n");

				if (precv.isValid()) {
					if (precv.getLen() == 0) {
						this->sendack(0, true);
						printf("[+] End of packet found\n");
						hasEndOfPacketFound = true;
						break;
					}
					printf("[+] Receive Packet No. %d\n", precv.getSeqNum());
					datastorage.push_back(precv);
					printf("[+] Sending ACK\n");
					this->sendack(precv.getSeqNum(), true);
				} else {
					this->sendack(precv.getSeqNum(), false);
					printf("[-] Sending NACK\n");
				}
			}
		}
		writeFile(); // Write to file since the buffer is full or EOF received
		if (hasEndOfPacketFound) break;
		while(datastorage.size() != 0) datastorage.pop_front(); // Emptying buffer
	}

}