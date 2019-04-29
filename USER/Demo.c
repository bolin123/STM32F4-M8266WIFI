#include "Demo.h"
#include "Wifi.h"
#include "MSocket.h"

#define DEMO_WIFI_AP_SSID "PandoraBox"
#define DEMO_WIFI_AP_PWD  "a12345678"

#define DEMO_HOST_TCP_PORT 10066
#define DEMO_HOST_UDP_PORT 10067

#define DEMO_NONSENSE_IP "1.1.1.1"

static bool g_wifiConnected = false;
static bool g_tcpServerCreate = false;
static bool g_udpServerCreate = false;

static MSocket_t *g_tcpSocket = NULL;
static MSocket_t *g_udpSocket = NULL;

static void demoTcpDataRecvHandle(const char *ip, uint16_t port, uint8_t *data, uint16_t len)
{
	SysLog("from:[%s:%d], len = %d", ip, port, len);
	MSocketDataSend(g_tcpSocket, data, len);
}

static void demoUdpDataRecvHandle(const char *ip, uint16_t port, uint8_t *data, uint16_t len)
{
    SysLog("from:[%s:%d], len = %d", ip, port, len);
    const char *myIp = WifiGetIPAddress();
    MSocketUdpDataSend(g_udpSocket, ip, port, (uint8_t *)myIp, strlen(myIp));
}

static void demoTcpServerPoll(void)
{
	static uint32_t lastTcpCreateTime = 0;
	
	if(SysTimeHasPast(lastTcpCreateTime, 5000))
	{
		if(g_wifiConnected)
		{
			if(!g_tcpServerCreate)
			{
				g_tcpSocket = MSocketCreate(MSOCKET_TYPE_TCP_SERVER, DEMO_HOST_TCP_PORT, DEMO_NONSENSE_IP, 0, demoTcpDataRecvHandle);
				if(g_tcpSocket != NULL)
				{
					g_tcpServerCreate = true;
					SysLog("tcp server created");
				}
			}

            if(!g_udpServerCreate)
            {
                g_udpSocket = MSocketCreate(MSOCKET_TYPE_UDP, DEMO_HOST_UDP_PORT, DEMO_NONSENSE_IP, 0, demoUdpDataRecvHandle);
                if(g_udpSocket != NULL)
                {
                    g_udpServerCreate = true;
					SysLog("udp server created");
                }
            }
		}
		else
		{
			if(g_tcpServerCreate)
			{
				MSocketRelease(g_tcpSocket);
				g_tcpSocket = NULL;
				g_tcpServerCreate = false;
				SysLog("tcp server released!");
			}

            if(g_udpServerCreate)
            {
                MSocketRelease(g_udpSocket);
                g_udpSocket = NULL;
                g_udpServerCreate = false;
                
				SysLog("udp server released!");
            }
		}
		lastTcpCreateTime = SysTime();
	}
}

static void demoWifiStatusPoll(void)
{
	static uint32_t lastQueryTime = 0;
	if(SysTimeHasPast(lastQueryTime, 2000))
	{
		if(WifiConnectStatus() == WIFI_CONNECT_STATUS_CONNECTED)
		{
			g_wifiConnected = true;
			printf("ap connected\n");
		}
		else
		{
			g_wifiConnected = false;
			printf("ap disconnected\n");
		}
		lastQueryTime = SysTime();
	}
}

void DemoPoll(void)
{
	demoTcpServerPoll();
	demoWifiStatusPoll();
}

void DemoInit(void)
{
	WifiJoinAp(DEMO_WIFI_AP_SSID, DEMO_WIFI_AP_PWD);
}
