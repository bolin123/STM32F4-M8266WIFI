#include "MSocket.h"
#include "M8266WIFIDrv.h"

#define MSOCKET_MAX_NUM 2 // <= 4
#define MSOCKET_INVALID_ID 0xff
#define MSOCKET_RECV_BUFF_LEN 1024

static MSocket_t g_socket[MSOCKET_MAX_NUM];
static uint8_t g_recvBuff[MSOCKET_RECV_BUFF_LEN];

int MSocketUdpDataSend(MSocket_t *socket, const char *ip, uint16_t port, uint8_t *data, uint16_t dlen)
{
	uint16_t status;
	uint16_t len = 0;
    
	if(socket != NULL)
    {
        do
        {
            len += M8266WIFI_SPI_Send_Udp_Data(data, dlen, socket->socketId, (char *)ip, port, &status);
            if((status&0x00ff) >= 0x10)
			{
				return -1;
			}
        }while(len < dlen);
    }   
    return len;
}

int MSocketDataSend(MSocket_t *socket, uint8_t *data, uint16_t dlen)
{
	uint16_t status;
	uint16_t len = 0;
	if(socket != NULL)
	{
		do
		{
			len += M8266WIFI_SPI_Send_Data(data, dlen, socket->socketId, &status);
			if((status&0x00ff) >= 0x10)
			{
				return -1;
			}
		}
		while(len < dlen);
	}
	return len;
}

bool MSocketConnected(MSocket_t *socket)
{
	uint8_t state;
	if(socket != NULL)
	{
		if(M8266WIFI_SPI_Query_Connection(socket->socketId, NULL, &state, NULL, NULL, NULL, NULL))
		{
			if(state == 3)
			{
				return true;
			}
		}
	}
	
	return false;
}

MSocket_t *MSocketCreate(MSocketType_t type, uint16_t port, const char *dstIP, uint16_t dstPort, MSocketDataRecv_cb recvCb)
{
	uint8_t i;
	uint16_t status;
	
	for(i = 0; i < MSOCKET_MAX_NUM; i++)
	{
		if(g_socket[i].socketId == MSOCKET_INVALID_ID)
		{
			if(M8266WIFI_SPI_Setup_Connection((uint8_t)type, port, (char *)dstIP, dstPort, i, 0, &status))
			{
				g_socket[i].type = type;
				g_socket[i].socketId = i;
				g_socket[i].dataRecvCb = recvCb;
				if(type == MSOCKET_TYPE_TCP_SERVER)
				{
					M8266WIFI_SPI_Set_TcpServer_Auto_Discon_Timeout(i, 180, &status);//timeout 60s
					//M8266WIFI_SPI_Config_Max_Clients_Allowed_To_A_Tcp_Server(i, 3, &status); //最大连接数
				}
				return &g_socket[i];
			}
		}
	}
	return NULL;
}

void MSocketRelease(MSocket_t *socket)
{
	uint16_t status;
	if(socket != NULL)
	{
		M8266WIFI_SPI_Delete_Connection(socket->socketId, &status);
		socket->socketId = MSOCKET_INVALID_ID;
	}
}

static void msocketDataRecv(void)
{
	uint8_t errcode;
	uint16_t len;
	uint16_t status;
	uint8_t socketId;
    uint8_t remoteIp[4];
    uint16_t remotePort;
    char ipaddr[16] = "";
	
	if(M8266WIFI_SPI_Has_DataReceived())
	{
		do
		{
			len = M8266WIFI_SPI_RecvData_ex(g_recvBuff, MSOCKET_RECV_BUFF_LEN, 100, &socketId, remoteIp, &remotePort, &status);
			errcode = (uint8_t)(status & 0x00ff);
			if(len > 0 && g_socket[socketId].dataRecvCb != NULL)
			{
			    sprintf(ipaddr, "%d.%d.%d.%d", remoteIp[0], remoteIp[1], remoteIp[2], remoteIp[3]);
				g_socket[socketId].dataRecvCb(ipaddr, remotePort, g_recvBuff, len);
			}
			else
			{
				break;
			}
		}while(errcode == 0x24 || errcode == 0x23);
	}
}

void MSocketInit(void)
{
	memset(g_socket, MSOCKET_INVALID_ID, sizeof(MSocket_t) * MSOCKET_MAX_NUM);
}

void MSocketPoll(void)
{
	msocketDataRecv();
}
