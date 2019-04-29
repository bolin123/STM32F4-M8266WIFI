#ifndef MSOCKET_H
#define MSOCKET_H

#include "HalCtype.h"
typedef enum
{
	MSOCKET_TYPE_UDP = 0,
	MSOCKET_TYPE_TCP_CLIENT,
	MSOCKET_TYPE_TCP_SERVER,
}MSocketType_t;

typedef void (*MSocketDataRecv_cb)(const char *ip, uint16_t port, uint8_t *data, uint16_t len);

typedef struct
{
	uint8_t socketId;
	MSocketType_t type;
	MSocketDataRecv_cb dataRecvCb;
}MSocket_t;

int MSocketUdpDataSend(MSocket_t *socket, const char *ip, uint16_t port, uint8_t *data, uint16_t dlen);
int MSocketDataSend(MSocket_t *socket, uint8_t *data, uint16_t dlen);
bool MSocketConnected(MSocket_t *socket);
MSocket_t *MSocketCreate(MSocketType_t type, uint16_t port, const char *dstIP, uint16_t dstPort, MSocketDataRecv_cb recvCb);
void MSocketRelease(MSocket_t *socket);
void MSocketInit(void);
void MSocketPoll(void);
#endif

