#include "sender.h"

Sender::Sender(char * ip, int port, int windowsize, int bufferlength) : rclient(ip, port) {
	this->windowsize = windowsize;
	this->bufferlength = bufferlength;
	lws = 0;
	lastbuffer = 0;
	rbuffer = 0;
	status = true;
}

Sender::~Sender() {

}

void Sender::openFile(char * filename) {
	fin = fopen(filename, "rb");

	fseek(fin, 0, SEEK_END);
	unsigned long int size = ftell(fin);
	fseek(fin, 0, SEEK_SET);

	len = (size / BUFFER_SIZE) + 1;
}

void Sender::readFile(char * filename) {
	int i;
	FILE * fin = fopen(filename, "rb");

	fseek(fin, 0, SEEK_END);
	unsigned long int size = ftell(fin);
	fseek(fin, 0, SEEK_SET);

	len = (size / BUFFER_SIZE) + 1;

	for (i = 0; i<len; i++) {
		int nread;
		Packet p;
		char data[BUFFER_SIZE];

		nread = fread(data, sizeof(char), BUFFER_SIZE, fin);
		p.setHeader(0x01);
		p.setSeqNum(i+1);
		p.setData(data, nread);

		datastorage.push_back(p);
		acks.push_back(0);
	}

	fclose(fin);
	char temp[1];
	Packet p1;
	p1.setHeader(0x01);
	p1.setSeqNum(i);
	p1.setData(temp, 0);

	datastorage.push_back(p1);
	acks.push_back(0);
}

void Sender::closeFile() {
	fclose(fin);
}

void Sender::fillBuffer() {

	while (datastorage.size() < bufferlength && lastbuffer < len) {
		int nread;
		Packet p;
		char data[BUFFER_SIZE];
		
		nread = fread(data, sizeof(char), BUFFER_SIZE, fin);
		p.setHeader(0x01);
		p.setSeqNum(++lastbuffer);
		p.setData(data, nread);

		datastorage.push_back(p);
		acks.push_back(0);	
	}

	if (lastbuffer == len && datastorage.size() < bufferlength) {
		char temp[1];
		Packet p1;
		p1.setHeader(0x01);
		p1.setSeqNum(++lastbuffer);
		p1.setData(temp, 0);

		datastorage.push_back(p1);
		acks.push_back(0);
	}
}

void Sender::slider() {
	int i;
	char buffer[BUFFER_SIZE+10];

	while (true) {

		fillBuffer();

		mtx.lock();

		for (i=0; i<windowsize; i++) {
			if (lws+i >= len) {
				continue;
			} else if (lws+i >= lastbuffer) {
				continue;
			} else {
				if (acks[lws+i] == 0) {
					printf("[+] Send Packet No. %d\n", lws+i+1);
					datastorage[lws+i-rbuffer].getBuffer(buffer);
					rclient.send(buffer, datastorage[lws+i-rbuffer].getLen()+10);
					acks[lws+i] = TIMEOUT;
				} else if (acks[lws+i] > 0) {
					acks[lws+i]--;
				}
			}
		}

		mtx.unlock();


		if (lws > lastbuffer - windowsize) {
			while (datastorage.size() > 0) {
				if (datastorage[0].getSeqNum() == lws+1) {
					break;
				} else {
					rbuffer++;
					datastorage.pop_front();
				}
			}
		}

		if (lws >= len)
			break;

		sleep(2);
	}

	while (true) {
		fillBuffer();
		if (acks[lws] == 0) {
			datastorage[lws-rbuffer].getBuffer(buffer);
			rclient.send(buffer, datastorage[lws-rbuffer].getLen()+10);
			acks[lws] = TIMEOUT;
		} else {
			acks[lws]--;
		}

		if (!status)
			break;
	}
}

void* Sender::listener(void * tSender) {
	char msgrecv[BUFFER_SIZE+10];
	Sender* gSender = (Sender *) tSender;

	while (true) {
		if (gSender->rclient.recv(msgrecv, BUFFER_SIZE+10)) {
			Packet precv(msgrecv);

			printf("[+] Received new packet\n");

			gSender->mtx.lock();

			if (precv.isValid()) {

				printf("[+] Get ACK %d\n", precv.getSeqNum());

				if (precv.getSeqNum() == 0) {
					gSender->status = false;
					break;
				}
				
				gSender->acks[precv.getSeqNum()-1] = -1;

			} else {
				printf("[-] Bad Packet Received\n");
			}

			while (gSender->acks[gSender->lws] == -1) {
				gSender->lws++;
			}

			gSender->mtx.unlock();

		}
	}
}