#ifndef HAL_H
#define HAL_H

#include "HalCtype.h"
#include "HalGPIO.h"
#include "HalTimer.h"
#include "HalUart.h"

uint32_t HalTimerCount(void);
void HalReboot(void);
void HalInit(void);
void HalPoll(void);
#endif

