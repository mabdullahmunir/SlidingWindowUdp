#include "UdpClient.h"
#include <unistd.h>
#include <string.h>
#include <cstdio>

UdpClient::UdpClient(const string& addr, int port) : f_port(port), f_addr(addr) {
    char decimal_port[16];
    snprintf(decimal_port, sizeof(decimal_port), "%d", f_port);
    decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    int r(getaddrinfo(addr.c_str(), decimal_port, &hints, &f_addrinfo));
    if(r != 0 || f_addrinfo == NULL) {
        // throw UdpClient_server_runtime_error(("invalid address or port: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
    f_socket = socket(f_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    if(f_socket == -1) {
        freeaddrinfo(f_addrinfo);
        // throw UdpClient_server_runtime_error(("could not create socket for: \"" + addr + ":" + decimal_port + "\"").c_str());
    }

    dead = false;
}

UdpClient::~UdpClient() {
    freeaddrinfo(f_addrinfo);
    close(f_socket);
}

int UdpClient::getSocket() const {
    return f_socket;
}

int UdpClient::getPort() const {
    return f_port;
}

string UdpClient::getAddr() const {
    return f_addr;
}

bool UdpClient::isDead() const {
    return dead;
}

void UdpClient::setDead(bool tDead) {
    dead = tDead;
}

int UdpClient::send(const char *msg, size_t size) {
    return sendto(f_socket, msg, size, 0, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);
}

int UdpClient::recv(char *msg, size_t max_size) {
    return ::recv(f_socket, msg, max_size, 0);
}
