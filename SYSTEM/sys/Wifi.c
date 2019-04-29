#include "Wifi.h"
#include "SysTimer.h"
#include "M8266WIFIDrv.h"
#include "M8266HostIf.h"
#include "M8266WIFI_ops.h"

#define WIFI_MAX_SCAN_RESULT_NUM  10

static WifiScanResult_cb g_scanResultCb;

void WifiReset(void)
{
	M8266WIFI_Module_Hardware_Reset();
}

const char *WifiGetIPAddress(void)
{
	static char staIp[16];
	if(M8266WIFI_SPI_Get_STA_IP_Addr(staIp, (void *)0))
	{
		return staIp;
	}
	return (void *)0;
}

WifiConnectStatus_t WifiConnectStatus(void)
{
	uint8_t status;
	uint16_t errcode;
	if(M8266WIFI_SPI_Get_STA_Connection_Status(&status, &errcode))
	{
		return (WifiConnectStatus_t)status;
	}
	return WIFI_CONNECT_STATUS_NONE;
}

static struct ScannedSigs scanInfo[WIFI_MAX_SCAN_RESULT_NUM];
static void wifiScanResult(void *arg)
{
	uint8_t i, count = 0;
	for(i = 0; i < 10; i++)
	{
		if(scanInfo[i].rssi != 0)
		{
			count++;
		}
	}
	g_scanResultCb((WifiScanResult_t *)scanInfo, count);
}

int WifiScan(WifiScanResult_cb resultCb)
{
	memset(scanInfo, 0, WIFI_MAX_SCAN_RESULT_NUM * sizeof(struct ScannedSigs));
	if(resultCb != NULL && M8266WIFI_SPI_STA_Scan_Signals(scanInfo, WIFI_MAX_SCAN_RESULT_NUM, 0xff, 2, (void *)0))
	{
		g_scanResultCb = resultCb;
		SysTimerSet(wifiScanResult, 2000, 0, NULL);
		return 0;
	}
	return -1;
}

void WifiSmartConfig(uint8_t start)
{
}

int WifiJoinAp(const char *ssid, const char *pwd)
{
	if(M8266WIFI_SPI_STA_Connect_Ap((uint8_t *)ssid, (uint8_t *)pwd, 1, 20, NULL))
	{
		return 0;
	}
	return -1;
}

void WifiPoll(void)
{
}

int WifiInit(void)
{
	uint16_t status;
	
	M8266HostIf_Init();
	if(M8266WIFI_Module_Init_Via_SPI())
	{
		if(M8266WIFI_SPI_Set_Opmode(1, 1, &status))
		{
			return 0;
		}
	}
	
	return -1;
}
