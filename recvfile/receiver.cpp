#include "receiver.h"

Receiver::Receiver(int port, int windowsize) : rserver("127.0.0.1", port) {
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
	char * data = p.getBuffer();

	rserver.send(data, 6, recvaddr);
}

void Receiver::listen() {
	uint recvaddrlen = sizeof(sockaddr_in);
	char msgrecv[BUFFER_SIZE];

	while (true) {
		if (rserver.recvfrom(msgrecv, BUFFER_SIZE, recvaddr, recvaddrlen) > 0) {
			Packet precv(msgrecv);

			printf("[+] Received new packet\n");

			if (precv.isValid()) {
				if (precv.getLen() == 0) {
					printf("[+] End of packet found\n");
					break;
				}
				datastorage.push_back(precv);
				this->sendack(++rws, true);
				printf("[+] Sending ACK\n");
			} else {
				this->sendack(precv.getSeqNum(), false);
				printf("[-] Sending NACK\n");
			}

		}
	}

}