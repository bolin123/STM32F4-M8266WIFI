#include "Hal.h"

#define HAL_LOG_UART_PORT 1

static uint32_t g_timerCount = 0;

//redirect "printf()"
int fputc(int ch, FILE *f)
{
	HalUartWrite(HAL_LOG_UART_PORT, (const uint8_t *)&ch, 1);
	return ch;
}

void HalTimerPast1ms(void)
{
	g_timerCount++;
}

uint32_t HalTimerCount(void)
{
	return g_timerCount;
}

static void halClkInit(void)
{

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		/**/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

}

static void logPrintInit(void)
{
    HalUartConfig_t config;
    config.baudrate = 115200;
    config.flowControl = 0;
    config.parity = 1;
    config.wordLength = USART_WordLength_8b;
    config.recvCb = NULL;
    HalUartConfig(HAL_LOG_UART_PORT, &config);
}

void HalReboot(void)
{
	printf("Sys reboot!!!\n");
	__set_FAULTMASK(1); 
    NVIC_SystemReset();
}

void HalInit(void)
{
	SystemInit();
	halClkInit();
	HalGPIOInitialize();
	HalUartInitialize();
	HalTimerInitialize();
	logPrintInit();
}

void HalPoll(void)
{
}
