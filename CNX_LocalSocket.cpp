#include "CNX_LocalSocket.h"
#include <poll.h>
#define LOG_TAG "[libnxsocket]"
#include <NX_Log.h>

CNX_LocalSocket::CNX_LocalSocket(SocketType eType)
{
    m_eSocketType = eType;
    m_hSocket = 0;
}

CNX_LocalSocket::~CNX_LocalSocket()
{

}

int32_t CNX_LocalSocket::Start(const char *pServerPath)
{
    int iNoDelay = 1;

    if (m_eSocketType == SocketType_Server)
    {
        if (0 == access(pServerPath, F_OK))
            unlink(pServerPath);
    }

    // socket 
    m_hSocket = socket(PF_FILE, SOCK_STREAM, 0);
    if (m_hSocket < 0)
    {
        goto error_proc;
    }

    setsockopt(m_hSocket, IPPROTO_TCP, TCP_NODELAY, &iNoDelay, sizeof(iNoDelay));

    // reset 
    memset(&m_sAddr, 0, sizeof(m_sAddr));
    m_sAddr.sun_family = AF_UNIX;
    strcpy(m_sAddr.sun_path, pServerPath);

    m_iAddrLen = sizeof(m_sAddr);

    if (m_eSocketType == SocketType_Server)
    {
    // bind
        if (bind(m_hSocket, (struct sockaddr*)&m_sAddr, m_iAddrLen) < 0)
        {
            goto error_proc;
        }

        // listen
        if (listen(m_hSocket, 10) < 0)
        {
            goto error_proc;
        }
    }

    return m_hSocket;

error_proc:
    if (0 < m_hSocket)
    {
        close(m_hSocket);
        m_hSocket = 0;
    }

    return -1;
}

int32_t CNX_LocalSocket::Stop()
{
    if (IsRunning())
    {
        return close(m_hSocket);
    }

    return -1;
}

int32_t CNX_LocalSocket::Accept()
{
    return accept(m_hSocket, (struct sockaddr*)&m_sAddr, (socklen_t*)&m_iAddrLen);
}

int32_t CNX_LocalSocket::Connect()
{
    return connect(m_hSocket, (struct sockaddr*)&m_sAddr, m_iAddrLen);
}

int32_t CNX_LocalSocket::Read(int32_t hClient, void *pBuffer, int32_t iSize)
{
    struct pollfd hPoll;

    hPoll.fd = hClient;
    hPoll.events = POLLIN;
    hPoll.revents = 0;

    if (0 < poll((struct pollfd *)&hPoll, 1, 1000))
    {
#if 1
        return read(hClient, pBuffer, iSize);
#else
        return recv(hClient, pBuffer, iSize, 0);
#endif
    }
    
    return -1;
}

int32_t CNX_LocalSocket::Write(int32_t hClient, void *pBuffer, int32_t iSize)
{
#if 1
    int32_t iWritten = write(hClient, pBuffer, iSize);
    NXLOGI("[%s] iWritten = %d", __FUNCTION__, iWritten);
    if (0 < iWritten)
    {
        FILE *fp = fdopen(hClient, "rw");
        fflush(fp);
        //fclose(fp);
    }
    NXLOGI("[%s] fflush done", __FUNCTION__);

    return iWritten;
#else
    return send(hClient, pBuffer, iSize, 0);
#endif
}

bool CNX_LocalSocket::IsRunning()
{
    return (m_hSocket > 0);
}
