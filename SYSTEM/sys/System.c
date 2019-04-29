#include "System.h"
#include "Wifi.h"
#include "MSocket.h"
#include "Hal.h"

void SystemReboot(void)
{
	HalReboot();
}

void SystemInitialize(void)
{
	HalInit();
	if(WifiInit() < 0)
	{
		SysLog("Wifi init error!");
		HalReboot();
		while(1);
	}
	MSocketInit();
}

void SystemPoll(void)
{
	WifiPoll();
	MSocketPoll();
}
