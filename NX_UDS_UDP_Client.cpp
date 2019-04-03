#include "NX_UDS_UDP_Client.h"
#include <poll.h>
#if __dmesg__
#define LOG(X)  do {                \
        fprintf(stderr, "%s\n", X); \
    } while(0)
#else /* !__dmesg__ */
#define LOG(X)  do { } while(0)
#endif

UDS_Client::UDS_Client()
{
    m_bRunning = false;
}

UDS_Client::~UDS_Client()
{
    if (m_bRunning)
        stop();
}

bool UDS_Client::start(char* client_path, char* server_path)
{
    if (m_bRunning) {
        return false;
    }

    // 1. copy for client/sever path
    strcpy(m_ClientPath, client_path);
    strcpy(m_ServerPath, server_path);

    // 2. if client path is exist, remove it.
    if (0 == access(m_ClientPath, F_OK))
        unlink(m_ClientPath);

    // 3. create socket
    //  -. domain: PF_FILE or PF_UNIX (using UDS)
    m_nSocket = socket(PF_FILE, SOCK_DGRAM, 0);
    if (m_nSocket < 0) {
        return false;
    }

    // 4. initialize - struct sockaddr_un variable for client
    memset(&m_ClientAddr, 0, sizeof(m_ClientAddr));
    m_ClientAddr.sun_family = AF_UNIX; // or AF_FILE
    strcpy(m_ClientAddr.sun_path, m_ClientPath);

    // 5. bind
    if (bind(m_nSocket, (struct sockaddr*)&m_ClientAddr, sizeof(m_ClientAddr)) < 0) {
        return false;
    }

    memset(&m_ServerAddr, 0, sizeof(m_ServerAddr));
    m_ServerAddr.sun_family = AF_UNIX;
    strcpy(m_ServerAddr.sun_path, m_ServerPath);
    m_nServerAddrSize = sizeof(m_ServerAddr);

    m_bRunning = true;
    return true;
}

bool UDS_Client::stop()
{
    if (m_bRunning) {
        m_bRunning = close(m_nSocket);
        return !m_bRunning;
    } else {
        return false;
    }
}

bool UDS_Client::isRunning()
{
    return m_bRunning;
}

int32_t UDS_Client::read(char* buffer, int length)
{
#if 1
    // return recvfrom(m_nSocket, buffer, length, 0, NULL, 0);
    return recvfrom(m_nSocket, buffer, length, 0, (struct sockaddr*)&m_ServerAddr, (socklen_t*)&m_nServerAddrSize);
#else
	::read(m_nSocket, buffer, length);
#endif
}

int32_t UDS_Client::read(char* buffer, int length, int timeout)
{
#if 0
	struct pollfd wait;
	wait.fd = m_nSocket;
	wait.events = POLLIN;
	wait.revents = 0;

	if (0 < poll(&wait, 1, timeout)) {
		if (wait.revents & POLLIN) {
            int32_t c = (int32_t)recvfrom(m_nSocket, buffer, length, 0, NULL, 0);
			return c;
		}
	}

    return -1;
#else
    int32_t sec = timeout / 1000;
    int32_t usec = (timeout - (sec*1000)) * 1000;
    struct timeval tv = {sec, usec};
    setsockopt(m_nSocket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    int32_t c = (int32_t)recvfrom(m_nSocket, buffer, length, 0, NULL, 0);
    return c;
#endif
}

void UDS_Client::write(char* buffer, int length/*= -1*/)
{
    // why buffer length + 1 ? +1 is null character.
#if 1
	if (length < 0)
		sendto(m_nSocket, buffer, strlen(buffer)+1, 0, (struct sockaddr*)&m_ServerAddr, m_nServerAddrSize);
	else
		sendto(m_nSocket, buffer, length, 0, (struct sockaddr*)&m_ServerAddr, m_nServerAddrSize);
#else
	::write(m_nSocket, buffer, strlen(buffer)+1);
#endif
}
