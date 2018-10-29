#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>

using namespace std;


class UdpClient
{
public:
    UdpClient(const std::string& addr, int port);
    ~UdpClient();

    int getSocket() const;
    int getPort() const;
    string getAddr() const;
    bool isDead() const;

    void setDead(bool tDead);

    int send(const char *msg, size_t size);
    int recv(char *msg, size_t max_size);


private:
    int f_socket;
    int f_port;
    string f_addr;
    struct addrinfo* f_addrinfo;
    bool dead;
};

#endif