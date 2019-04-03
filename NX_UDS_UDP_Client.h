#ifndef NX_UDS_UDP_CLIENT_H
#define NX_UDS_UDP_CLIENT_H

/*
 * IPC Client
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

class UDS_Client
{
public:
    UDS_Client();

    ~UDS_Client();

    //-------------------------------------------------------
    // [1] ups socket wrapping functions
    bool start(char* client_path, char* server_path);

    bool stop();

    bool isRunning();

    int32_t read(char* buffer, int length);

	int32_t read(char* buffer, int length, int timeout);

	void write(char* buffer, int length = -1);

private:
    bool m_bRunning;

    // why array size is 108 ?
    //   A) reference - struct sockaddr_un -> char sun_path[] size
    //      it's defined <sys/un.h>
    char m_ClientPath[108];
    char m_ServerPath[108];

    // file descriptor for socket
    int m_nSocket;

    struct sockaddr_un m_ServerAddr;
    struct sockaddr_un m_ClientAddr;
    size_t m_nServerAddrSize;
};

#endif // NX_UDS_UDP_CLIENT_H
