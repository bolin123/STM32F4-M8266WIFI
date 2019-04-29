#ifndef WIFI_H
#define WIFI_H

#include "HalCtype.h"

typedef struct{
	char ssid[32];
	uint8_t chn;
	uint8_t auth;
	int8_t rssi;
}WifiScanResult_t;

typedef void (*WifiScanResult_cb)(WifiScanResult_t *result, uint8_t resultNum);

typedef enum
{
	WIFI_CONNECT_STATUS_NONE = 0,
	WIFI_CONNECT_STATUS_CONNECTING,
	WIFI_CONNECT_STATUS_PASSWD_ERR,
	WIFI_CONNECT_STATUS_NO_AP,
	WIFI_CONNECT_STATUS_OTHER_ERR,
	WIFI_CONNECT_STATUS_CONNECTED,
}WifiConnectStatus_t;

int WifiScan(WifiScanResult_cb resultCb);
const char *WifiGetIPAddress(void);
void WifiReset(void);
WifiConnectStatus_t WifiConnectStatus(void);
void WifiSmartConfig(uint8_t start);
int WifiJoinAp(const char *ssid, const char *pwd);
void WifiPoll(void);
int WifiInit(void);
#endif
