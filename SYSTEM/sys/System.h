#ifndef SYSTEM_H
#define SYSTEM_H

#include "Hal.h"

#define SysLog(...) do{printf("%s: ",__FUNCTION__); printf(__VA_ARGS__); printf("\r\n");}while(0)

#define SysTime HalTimerCount
#define SysTimeHasPast(oldtime, pass) ((SysTime() - (oldtime)) > pass)

void SystemReboot(void);
void SystemInitialize(void);
void SystemPoll(void);
#endif

