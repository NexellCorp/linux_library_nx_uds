#ifndef CNX_LOCALSOCKET_H
#define CNX_LOCALSOCKET_H

#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdint.h>

class CNX_LocalSocket
{
public:
    enum SocketType {
        SocketType_Server,
        SocketType_Client
    };

    CNX_LocalSocket(SocketType eType);

    ~CNX_LocalSocket();

    int32_t Start(const char *pServerPath);

    int32_t Stop();

    int32_t Accept();

    int32_t Connect();

    int32_t Read(int32_t hClient, void *pBuffer, int32_t iSize);

    int32_t Write(int32_t hClient, void *pBuffer, int32_t iSize);

    bool IsRunning();

private:
    SocketType m_eSocketType;

    int32_t m_hSocket;

    struct sockaddr_un m_sAddr;

    int32_t m_iAddrLen;
};

#endif /* CNX_LOCALSOCKET_H */