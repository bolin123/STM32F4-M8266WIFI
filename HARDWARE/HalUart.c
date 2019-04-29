#include "HalUart.h"

static HalUartConfig_t g_uartConfig[4];

void HalUartConfig(uint8_t uart, HalUartConfig_t *config)
{   
    USART_InitTypeDef uartcfg;
    USART_TypeDef *uartNo;
    
    USART_StructInit(&uartcfg);
    if(0 == uart)
    {
        uartNo = USART1;
    }
    else if(1 == uart)
    {
        uartNo = USART2;
    }
    else
    {
        return;
    }
    g_uartConfig[uart] = *config;

    USART_Cmd(uartNo, DISABLE);
    USART_ITConfig(uartNo, USART_IT_RXNE, DISABLE);
    
    uartcfg.USART_BaudRate = config->baudrate;
    uartcfg.USART_WordLength = config->wordLength;
    uartcfg.USART_Parity = config->parity;
    uartcfg.USART_StopBits = USART_StopBits_1;
    uartcfg.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    uartcfg.USART_HardwareFlowControl = config->flowControl;

    USART_Init(uartNo, &uartcfg);

    USART_ClearITPendingBit(uartNo, USART_IT_RXNE);
    USART_ITConfig(uartNo, USART_IT_RXNE, ENABLE);
    
    USART_Cmd(uartNo, ENABLE);
}

void HalUartInitialize(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
#if 1
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#if 0
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
	
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); //GPIOA9???USART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //GPIOA10???USART1
	
    GPIO_InitTypeDef GPIO_InitStructure;
#if 1
    //uart1 io
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;  //TX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //??????
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //??
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
     
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;  //RX
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //??????
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //??
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //GPIOA9???USART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //GPIOA10???USART1
    //uart2 io
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;  //TX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //??????
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //??
    GPIO_Init(GPIOA, &GPIO_InitStructure);
     
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;  //RX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //??????
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //??
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#if 0
    //uart3 io
    GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;  //TX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
     
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;  //RX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif
}

void HalUartPoll(void)
{
}

void HalUartWrite(uint8_t uart, const uint8_t *data, uint16_t len)
{
    uint16_t i;
    USART_TypeDef *uartNo;
    if(0 == uart)
    {
        uartNo = USART1;
    }
    else if(1 == uart)
    {
        uartNo = USART2;
    }

    for(i = 0; i < len; i++)
    {
        USART_SendData(uartNo, (uint16_t)data[i]);
        while(USART_GetFlagStatus(uartNo, USART_FLAG_TC) == RESET);
    }
    
}

#if 1
void USART1_IRQHandler(void)
{
    uint8_t data;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        data = USART_ReceiveData(USART1);
        if(g_uartConfig[0].recvCb != NULL)
        {
            g_uartConfig[0].recvCb(&data, 1);
        }
    }
}
#endif

void USART2_IRQHandler(void)
{
    uint8_t data;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        data = USART_ReceiveData(USART2);
        if(g_uartConfig[1].recvCb != NULL)
        {
            g_uartConfig[1].recvCb(&data, 1);
        }
    }
}
#if 0
void USART3_IRQHandler(void)
{
    uint8_t data;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        data = USART_ReceiveData(USART3);
        if(g_uartConfig[2].recvCb != NULL)
        {
            g_uartConfig[2].recvCb(&data, 1);
        }
    }
}
#endif


