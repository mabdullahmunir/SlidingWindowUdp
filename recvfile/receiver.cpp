#include "receiver.h"

Receiver::Receiver(int port, int windowsize) : rserver("127.0.0.1", port) {

}

Receiver::~Receiver() {

}

void Receiver::sendack(int nextseq) {
	Packet p;
	p.setHeader(0x06);
	p.setSeqNum(nextseq);

	p.createBuffer();
	char * hmm = p.getBuffer();
	for (int i=0; i<6; i++) {
		printf("%02hhx ", hmm[i]);
	}

}

void Receiver::listen() {
	uint recvaddrlen = sizeof(sockaddr_in);
	char msgrecv[BUFFER_SIZE];

	while (true) {
		if (rserver.recvfrom(msgrecv, BUFFER_SIZE, recvaddr, recvaddrlen) > 0) {
			Packet precv(msgrecv);

			if (precv.isValid()) {
				if (precv.getLen() == 0) {
					break;
				}
				datastorage.push_back(precv);
				this->sendack();
			} else {

			}

		}
	}

}