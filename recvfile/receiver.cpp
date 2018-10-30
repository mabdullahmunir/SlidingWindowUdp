#include "receiver.h"

Receiver::Receiver(char* ip, int port, int windowsize) : rserver(ip, port) {
	rws = windowsize;
}

Receiver::~Receiver() {

}

void Receiver::sendack(int seq, bool status) {
	Packet p;
	if (status) {
		p.setHeader(0x06);
	} else {
		p.setHeader(0x07);
	}

	p.setSeqNum(seq);

	p.createBuffer();

	rserver.send(p.getBuffer(), 6, recvaddr);
}

void Receiver::saveToFile(char * filename) {
	FILE * fout = fopen(filename, "wb");

	sort(datastorage.begin(), datastorage.end());

	for (int i=0; i<datastorage.size(); i++) {
		fwrite(datastorage[i].getData(), sizeof(char), datastorage[i].getLen(), fout);
	}

	fclose(fout);
}

void Receiver::listen() {
	uint recvaddrlen = sizeof(sockaddr_in);
	char msgrecv[BUFFER_SIZE+10];

	while (true) {
		if (rserver.recvfrom(msgrecv, BUFFER_SIZE+10, recvaddr, recvaddrlen) > 0) {
			Packet precv(msgrecv);

			printf("[+] Received new packet\n");

			if (precv.isValid()) {
				if (precv.getLen() == 0) {
					this->sendack(0, true);
					printf("[+] End of packet found\n");
					break;
				}
				datastorage.push_back(precv);
				this->sendack(precv.getSeqNum(), true);
				printf("[+] Receive Packet No. %d\n", precv.getSeqNum());
				printf("[+] Sending ACK\n");
			} else {
				this->sendack(precv.getSeqNum(), false);
				printf("[-] Sending NACK\n");
			}

		}
	}

}