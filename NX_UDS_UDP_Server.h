#ifndef NX_UDS_UDP_SERVER_H
#define NX_UDS_UDP_SERVER_H

/*
 * IPC Server
 *  - IPC Tools : UDS(Unix Domain Socket)
 *  - Socket : UDP
 */

// for ipc (socket, uds)
#include <sys/types.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// for client list
#include <vector>
#include <string>
using namespace std;

class UDS_Server
{
public:
    UDS_Server();

    ~UDS_Server();

    //-------------------------------------------------------
    // [1] ups socket wrapping functions
    bool start(char* server_path);

    bool stop();

    bool isRunning();

	int32_t read(char* buffer, int length);

	int32_t read(char* buffer, int length, int timeout);

	int32_t read(char* buffer, int length, int timeout, char* client);

    int32_t write(char* buffer, int length = -1);
    
    int32_t write(const char* destination, char* buffer, int length = -1);

    void write_broadcast(char* buffer);

    //-------------------------------------------------------
    // [2] client list functions
    // [2] setter
    void setClientList(std::vector<std::string> clients);

    void pushClient(std::string client);

    // [2] getter
    std::vector<std::string> clientList();

    int indexOfClientList(std::string client);

    // [2] remove
    void removeAllClientList();

    void removeOneClientList(int index);

    void removeOneClientList(std::string client);

private:
    bool m_bRunning;

    // why array size is 108 ?
    //   A) reference - struct sockaddr_un -> char sun_path[] size
    //      it's defined <sys/un.h>
    char m_ServerPath[108];

    // file descriptor for socket
    int m_nSocket;

    struct sockaddr_un m_ServerAddr;
    struct sockaddr_un m_ClientAddr;
    size_t m_nClientAddrSize;

    // client path list
    std::vector<std::string> m_ClientPathList;
};

#endif // NX_UDS_UDP_SERVER_H
