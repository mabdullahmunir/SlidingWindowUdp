#include "utility.h"

unsigned char checksum(char* data, int len) {
	unsigned long int sum = 0;
	for (int i=0; i<len; i++) {
		sum += data[i];
	}
	sum = sum ^ 0xFF;
	return sum;
}

