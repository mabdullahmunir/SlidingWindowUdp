#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>

using namespace std;


class udp_server
{
public:
    udp_server(const std::string& addr, int port);
    ~udp_server();

    int getSocket() const;
    int getPort() const;
    string getAddr() const;

    int recv(char *msg, size_t max_size);
    int recvfrom(char *msg, size_t max_size, sockaddr_in& sender, uint& senderaddrsize);
    int send(const char *msg, size_t size, sockaddr_in dest);

private:
    int f_socket;
    int f_port;
    string f_addr;
    struct addrinfo* f_addrinfo;
};

#endif