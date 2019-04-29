/********************************************************************
 * M8266HostIf.c
 * .Description
 *     Source file of M8266WIFI Host Interface 
 * .Copyright(c) Anylinkin Technology 2015.5-
 *     IoT@anylinkin.com
 *     http://www.anylinkin.com
 *     http://anylinkin.taobao.com
 *  Author
 *     wzuo
 *  Date
 *  Version
 ********************************************************************/
//#include "sys.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"	
#include "brd_cfg.h"
#include "M8266WIFIDrv.h"
#include "M8266HostIf.h"

/***********************************************************************************
 * M8266HostIf_GPIO_SPInCS_nRESET_Pin_Init                                         *
 * Description                                                                     *
 *    To initialise the GPIOs for SPI nCS and nRESET output for M8266WIFI module   *
 *    You may update the macros of GPIO PINs usages for nRESET from brd_cfg.h      *
 *    You are not recommended to modify codes below please                         *
 * Parameter(s):                                                                   *
 *    None                                                                         *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/
void M8266HostIf_GPIO_CS_RESET_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

	//Initial STM32's GPIO for M8266WIFI_SPI_nCS
  M8266WIFI_SPI_nCS_GPIO_RCC_CLOCK_EN;  														// enable nCS GPIO clock
	GPIO_OUTPUT_HIGH(M8266WIFI_SPI_nCS_GPIO,M8266WIFI_SPI_nCS_PIN);		// nCS output high initially	
	GPIO_InitStructure_AS_GPIO_OUTPUT(M8266WIFI_SPI_nCS_GPIO,M8266WIFI_SPI_nCS_PIN);
	
	//Initial STM32's GPIO for M8266WIFI_nRESET
  M8266WIFI_nRESET_GPIO_RCC_CLOCK_EN;  															// enable nRESET GPIO clock
	GPIO_OUTPUT_HIGH(M8266WIFI_nRESET_GPIO,M8266WIFI_nRESET_PIN);		  // nRESET output high initially	
	GPIO_InitStructure_AS_GPIO_OUTPUT(M8266WIFI_nRESET_GPIO,M8266WIFI_nRESET_PIN);
}
/***********************************************************************************
 * M8266HostIf_SPI_Init                                                            *
 * Description                                                                     *
 *    To initialise the SPI Interface for M8266WIFI module                         *
 *    You may update the macros of SPI usages for nRESET from brd_cfg.h            *
 *    You are not recommended to modify codes below please                         *
 * Parameter(s):                                                                   *
 *    None                                                                         *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/
#if defined(MCU_IS_STM32F7XX) || defined(MCU_IS_STM32L1XX) || defined(MCU_IS_STM32L4XX) || defined(MCU_IS_STM32H7XX)
	SPI_HandleTypeDef hspi;
#endif
void M8266HostIf_SPI_Init(void)
{
////////////////////////////////////////////////////////////////////////////////////
///////////////  STM32F1  //////////////////////////////////////////////////////////
#if 	defined(MCU_IS_STM32F1XX)
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;
#if (M8266WIFI_SPI_INTERFACE_NO == 1)  
	#if 1 // if use GPIOA5/6/7 for SPI1 SCK/MISO/MOSI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 						// enable the SPI1 GPIO CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;									// SET GPIOs for SPI1
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);										// no need to Enable GPIO Alternate for SPI1, since GPIOA5/6/7 for SPI1 is the default alternative
	#elif 0 // if use GPIOB3/4/5 for SPI1 SCK/MISO/MOSI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 						// enable the SPI1 GPIO CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;									// SET GPIOs for SPI1
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);											// need to Enable GPIO Alternate for SPI1, since GPIOB3/4/5 for SPI1 is not the default alternative
	#else
	#error You should specify the GPIO pins used on STM32F1 SPI1 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);						  //  Enable SPI1 CLOCK
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);							//  Start to Reset SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);							//  Stop reseting SPI1	
	
	// end of if (M8266WIFI_SPI_INTERFACE_NO == 1) for STM32F1

#elif (M8266WIFI_SPI_INTERFACE_NO == 2)  
	#if 1 // if use GPIOB13/14/15 for SPI2 SCK/MISO/MOSI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 						// enable the SPI2 GPIO CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;									// SET GPIOs for SPI2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//GPIO_PinRemapConfig(GPIO_Remap_SPI2, ENABLE);										//  no need to Enable GPIO Alternate for SPI2, since GPIOB13/14/15 for SPI2 is the default alternative
	#else  // STM32F10x has only GPIOB13/14/15 mapped for SPI2
	#error You should specify the GPIO pins used on STM32F1 SPI2 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);						  //  Enable SPI2 CLOCK
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);							//  Start to Reset SPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);							//  Stop reseting SPI2

	// end of if (M8266WIFI_SPI_INTERFACE_NO == 2) for STM32F1
	
#elif (M8266WIFI_SPI_INTERFACE_NO == 3)  
	#if 1 // if use GPIOB3/4/5 for SPI3 SCK/MISO/MOSI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 						// enable the SPI3 GPIO CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;									// SET GPIOs for SPI2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);														
	//GPIO_PinRemapConfig(GPIO_Remap_SPI3, ENABLE);										//  no need to Enable GPIO Alternate for SPI3, since GPIOB3/4/5 for SPI3 is the default alternative

	#elif 0 // if use GPIOC10/11/12 for SPI3 SCK/MISO/MOSI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 						// enable the SPI2 GPIO CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;									// SET GPIOs for SPI2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_Remap_SPI3, ENABLE);										  //  need to Enable GPIO Alternate for SPI3, since GPIOC10/11/12 for SPI3 is not the default alternative
	#else  // STM32F10x has only GPIOB13/14/15 mapped for SPI2
	#error You should specify the GPIO pins used on STM32F1 SPI3 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);						  //  Enable SPI2 CLOCK
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);							//  Start to Reset SPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, DISABLE);							//  Stop reseting SPI2

	// end of if (M8266WIFI_SPI_INTERFACE_NO == 3) for STM32F1
		
#else
#error Invalid M8266WIFI_SPI_INTERFACE_NO defined in brd_cfg.h 
#endif

	SPI_InitStructure.SPI_Direction 		= SPI_Direction_2Lines_FullDuplex;    
	SPI_InitStructure.SPI_Mode 					= SPI_Mode_Master;									  
	SPI_InitStructure.SPI_DataSize 			= SPI_DataSize_8b;									 
	SPI_InitStructure.SPI_CPOL 					= SPI_CPOL_Low;		                   //  !!!! M8266WIFI SPI Works at CPOL=LOW, CPHA=1Edge
	SPI_InitStructure.SPI_CPHA 					= SPI_CPHA_1Edge;	
	SPI_InitStructure.SPI_NSS 					= SPI_NSS_Soft;									
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit 			= SPI_FirstBit_MSB;									 //  !!!! M8266WIFI SPI Works with MSB First							
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(M8266WIFI_INTERFACE_SPI, &SPI_InitStructure);
	SPI_Cmd(M8266WIFI_INTERFACE_SPI, ENABLE); 															//	Enable SPI Interface
	//M8266HostIf_SPI_ReadWriteByte(0xff);																	//  Start Trasmission	
#ifdef M8266WIFI_SPI_ACCESS_USE_DMA	
	SPI_I2S_DMACmd(M8266WIFI_INTERFACE_SPI, SPI_I2S_DMAReq_Rx, ENABLE);			//  Enable SPI Rx DMA
	SPI_I2S_DMACmd(M8266WIFI_INTERFACE_SPI, SPI_I2S_DMAReq_Tx, ENABLE);			//  Enable SPI Tx DMA
#endif
		
////////////////////////////////////////////////////////////////////////////////////
///////////////  STM32F2 or STM32F4  ///////////////////////////////////////////////
#elif defined(MCU_IS_STM32F2XX) || defined(MCU_IS_STM32F4XX)
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;
#if (M8266WIFI_SPI_INTERFACE_NO == 1)
  #if 1 // if use GPIOA5/6/7 for SPI1 SCK/MISO/MOSI
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 						// enable the SPI1 GPIOA CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;											// SET GPIOs for SPI1
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);							// Enable GPIOA 7/6/5 Alternative for SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
	
  #elif 0 // if use GPIOA5/B4/A7 for SPI1 SCK/MISO/MOSI
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 						// enable the SPI1 GPIOA CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;											// SET GPIOs for SPI1
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);							// Enable GPIOA 7/6/5 Alternative for SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 						// enable the SPI1 GPIOA CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;											// SET GPIOs for SPI1
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);							

  #elif	0 // if use GPIOB3/4/5 for SPI1 SCK/MISO/MOSI
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 						// enable the SPI1 GPIOB CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;											// SET GPIOs for SPI1
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);							// Enable GPIOB 5/4/3 Alternative for SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);

  #elif 0 // if use PB3/PA6/PA7 for SPI1 SCK/MISO/MOSI
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 						// enable the SPI1 GPIOA CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;											// SET GPIOs for SPI1
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);							// Enable GPIOA 7/6 Alternative for SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 						// enable the SPI1 GPIOB CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;											// SET GPIOs for SPI1
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);
	#else
	#error You should specify the GPIO pins used on STM32F4 SPI1 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);						  //  Enable SPI1 CLOCK
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);							//  Start to Reset SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);							//  Stop reseting SPI1	

// end of if (M8266WIFI_SPI_INTERFACE_NO == 1) for STM32F4
	
#elif (M8266WIFI_SPI_INTERFACE_NO == 2)    // if use SPI2 GPIOB13/14/15 for SCK/MISO/MOSI
  #if 1 // if use GPIOB13/14/15 for SPI2 SCK/MISO/MOSI
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 						// enable the SPI2 GPIO CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;											// SET GPIOs for SPI2
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);							// Enable GPIO Alternate for SPI2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);
  #elif 0 // if use GPIOB10/PC2/PC3 for SPI2 SCK/MISO/MOSI
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 						// enable the SPI2 GPIO CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;											// SET GPIOs for SPI2
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_SPI2);						// Enable GPIO Alternate for SPI2
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 						// enable the SPI2 GPIO CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;											// SET GPIOs for SPI2
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource2,GPIO_AF_SPI2);						  // Enable GPIO Alternate for SPI2
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource3,GPIO_AF_SPI2);
  #else
  #error You should specify the GPIO pins used on STM32F4 SPI2 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
  #endif	

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);						  //  Enable SPI2 CLOCK
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);							//  Start to Reset SPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);							//  Stop reseting SPI2
	
// end of if (M8266WIFI_SPI_INTERFACE_NO == 2) for STM32F4	
		
#elif (M8266WIFI_SPI_INTERFACE_NO == 3)
  #if 0  // if use GPIOB3/4/5 for SPI3 SCK/MISO/MOSI
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 						// enable the SPI3 GPIO CLOCK, using PB5/PB4/PB3 for MOSI/MISO/SCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;											// SET GPIOs for SPI3
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI3);							// Enable GPIO Alternate for SPI3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI3);
  #elif 1 // if use GPIOC10/11/12 for SPI3 SCK/MISO/MOSI
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 						// enable the SPI3 GPIO CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;											// SET GPIOs for SPI3
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_SPI3);							// Enable GPIO Alternate for SPI3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_SPI3);
	#else
	#error You should specify the GPIO pins used on STM32F4 SPI3 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);						  //  Enable SPI3 CLOCK
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);							//  Start to Reset SPI3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, DISABLE);							//  Stop reseting SPI3	

// end of if (M8266WIFI_SPI_INTERFACE_NO == 3) for STM32F4	

#else
#error Invalid M8266WIFI_SPI_INTERFACE_NO defined in brd_cfg.h 
#endif

	SPI_InitStructure.SPI_Direction 		= SPI_Direction_2Lines_FullDuplex;    
	SPI_InitStructure.SPI_Mode 					= SPI_Mode_Master;									  
	SPI_InitStructure.SPI_DataSize 			= SPI_DataSize_8b;									 
	SPI_InitStructure.SPI_CPOL 					= SPI_CPOL_Low;		                   //  !!!! M8266WIFI SPI Works at CPOL=LOW, CPHA=1Edge
	SPI_InitStructure.SPI_CPHA 					= SPI_CPHA_1Edge;	
	SPI_InitStructure.SPI_NSS 					= SPI_NSS_Soft;									
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit 			= SPI_FirstBit_MSB;									 //  !!!! M8266WIFI SPI Works with MSB First							
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(M8266WIFI_INTERFACE_SPI, &SPI_InitStructure);
	SPI_Cmd(M8266WIFI_INTERFACE_SPI, ENABLE); 															//	Enable SPI Interface
	//M8266HostIf_SPI_ReadWriteByte(0xff);																	//  Start Trasmission	
#ifdef M8266WIFI_SPI_ACCESS_USE_DMA	
	SPI_I2S_DMACmd(M8266WIFI_INTERFACE_SPI, SPI_I2S_DMAReq_Rx, ENABLE);			//  Enable SPI Rx DMA
	SPI_I2S_DMACmd(M8266WIFI_INTERFACE_SPI, SPI_I2S_DMAReq_Tx, ENABLE);			//  Enable SPI Tx DMA
#endif

////////////////////////////////////////////////////////////////////////////////////
//////////////////////////  STM32F3  ///////////////////////////////////////////////
#elif defined(MCU_IS_STM32F3XX)
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;
#if (M8266WIFI_SPI_INTERFACE_NO == 1)  
	#if 0 // if use SPI1 GPIOA5/6/7 for SCK/MISO/MOSI
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); 						// enable the SPI1 GPIO CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;											// SET GPIOs for SPI1
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_5);							// Enable GPIO Alternate for SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_5);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_5);

  #elif	1 // if use GPIOB3/4/5 for SPI1 SCK/MISO/MOSI
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); 						// enable the SPI1 GPIOB CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;											// SET GPIOs for SPI1
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_5);							// Enable GPIOB 5/4/3 Alternative for SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_5);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_5);
	#else
	#error You should specify the GPIO pins used on STM32F3 SPI1 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);						  //  Enable SPI1 CLOCK
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);							//  Start to Reset SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);							//  Stop reseting SPI1	

// end of if (M8266WIFI_SPI_INTERFACE_NO == 1) for STM32F3	

#elif (M8266WIFI_SPI_INTERFACE_NO == 2)    
  #if 1 // if use SPI2 GPIOB13/14/15 for SCK/MISO/MOSI
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); 						// enable the SPI2 GPIO CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;											// SET GPIOs for SPI2
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_5);							// Enable GPIO Alternate for SPI2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_5);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_5);
	#elif 0
	#else
	#error You should specify the GPIO pins used on STM32F3 SPI1 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);						  //  Enable SPI2 CLOCK
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);							//  Start to Reset SPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);							//  Stop reseting SPI2

// end of if (M8266WIFI_SPI_INTERFACE_NO == 2) for STM32F3	

#elif (M8266WIFI_SPI_INTERFACE_NO == 3)    
  #if 1  // if use GPIOB3/4/5 for SPI3 SCK/MISO/MOSI
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); 						// enable the SPI3 GPIO CLOCK, using PB5/PB4/PB3 for MOSI/MISO/SCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;											// SET GPIOs for SPI3
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_6);							// Enable GPIO Alternate for SPI3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_6);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_6);
	
  #elif 0 // if use GPIOC10/11/12 for SPI3 SCK/MISO/MOSI
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE); 						  // enable the SPI3 GPIO CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;											// SET GPIOs for SPI3
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_6);							// Enable GPIO Alternate for SPI3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_6);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_6);
	#else
	#error You should specify the GPIO pins used on STM32F3 SPI1 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);						  //  Enable SPI3 CLOCK
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);							//  Start to Reset SPI3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, DISABLE);							//  Stop reseting SPI3	
	
	// end of if (M8266WIFI_SPI_INTERFACE_NO == 3) for STM32F3	
	
#else
#error Invalid M8266WIFI_SPI_INTERFACE_NO defined in brd_cfg.h 
#endif

	SPI_InitStructure.SPI_Direction 		= SPI_Direction_2Lines_FullDuplex;    
	SPI_InitStructure.SPI_Mode 					= SPI_Mode_Master;									  
	SPI_InitStructure.SPI_DataSize 			= SPI_DataSize_8b;									 
	SPI_InitStructure.SPI_CPOL 					= SPI_CPOL_Low;		                   //  !!!! M8266WIFI SPI Works at CPOL=LOW, CPHA=1Edge
	SPI_InitStructure.SPI_CPHA 					= SPI_CPHA_1Edge;	
	SPI_InitStructure.SPI_NSS 					= SPI_NSS_Soft;									
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit 			= SPI_FirstBit_MSB;									 //  !!!! M8266WIFI SPI Works with MSB First							
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(M8266WIFI_INTERFACE_SPI, &SPI_InitStructure);
	SPI_Cmd(M8266WIFI_INTERFACE_SPI, ENABLE); 															//	Enable SPI Interface
	//M8266HostIf_SPI_ReadWriteByte(0xff);																	//  Start Trasmission	
#ifdef M8266WIFI_SPI_ACCESS_USE_DMA	
	SPI_I2S_DMACmd(M8266WIFI_INTERFACE_SPI, SPI_I2S_DMAReq_Rx, ENABLE);			//  Enable SPI Rx DMA
	SPI_I2S_DMACmd(M8266WIFI_INTERFACE_SPI, SPI_I2S_DMAReq_Tx, ENABLE);			//  Enable SPI Tx DMA
#endif

////////////////////////////////////////////////////////////////////////////////////
///////////////  STM32F7  //////////////////////////////////////////////////////////
#elif defined(MCU_IS_STM32F7XX)
  GPIO_InitTypeDef  GPIO_InitStructure;
#if (M8266WIFI_SPI_INTERFACE_NO == 1)
	#if 1 // if use GPIOA5/6/7 for SPI1 SCK/MISO/MOSI
	__HAL_RCC_GPIOA_CLK_ENABLE();					// Enable the GPIOE Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI1;	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);  
	#elif 0 // if use GPIOB3/4/5 for SPI1 SCK/MISO/MOSI
	__HAL_RCC_GPIOB_CLK_ENABLE();					// Enable the GPIOB Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI1;	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  
	#else
	#error You should specify the GPIO pins used on STM32F7 SPI1 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	__HAL_RCC_SPI1_CLK_ENABLE();
	hspi.Instance = SPI1;	
// end of if (M8266WIFI_SPI_INTERFACE_NO == 1) for STM32F7

#elif (M8266WIFI_SPI_INTERFACE_NO == 2)
	#if 1 // if use GPIOB13/14/15 for SPI2 SCK/MISO/MOSI
	__HAL_RCC_GPIOB_CLK_ENABLE();					// Enable the GPIOB Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  
  #elif 0 // if use GPIOB10/PC2/PC3 for SPI2 SCK/MISO/MOSI
	__HAL_RCC_GPIOB_CLK_ENABLE();					// Enable the GPIOB Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_10;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  
	__HAL_RCC_GPIOC_CLK_ENABLE();					// Enable the GPIOC Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;	
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);  
	#else
	#error You should specify the GPIO pins used on STM32F7 SPI2 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	__HAL_RCC_SPI2_CLK_ENABLE();
	hspi.Instance = SPI2;	
// end of if (M8266WIFI_SPI_INTERFACE_NO == 2) for STM32F7

#elif (M8266WIFI_SPI_INTERFACE_NO == 3)
	#if 1 // if use GPIOB3/4/5 for SPI3 SCK/MISO/MOSI
	__HAL_RCC_GPIOB_CLK_ENABLE();					// Enable the GPIOB Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF6_SPI3;	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  
  #elif 0 // if use GPIOC10/11/12 for SPI3 SCK/MISO/MOSI
	__HAL_RCC_GPIOC_CLK_ENABLE();					// Enable the GPIOC Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF6_SPI3;	
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);  
	#else
	#error You should specify the GPIO pins used on STM32F7 SPI3 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	__HAL_RCC_SPI3_CLK_ENABLE();
	hspi.Instance = SPI3;	
// end of if (M8266WIFI_SPI_INTERFACE_NO == 3) for STM32F7

#elif (M8266WIFI_SPI_INTERFACE_NO == 4)  
  #if 1 // if use SPI1 GPIOE2/5/6 for SCK/MISO/MOSI
	__HAL_RCC_GPIOE_CLK_ENABLE();					 // Enable the GPIOE Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI4;	
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure); 
  #elif 0 // if use SPI1 GPIOE12/13/14 for SCK/MISO/MOSI
	__HAL_RCC_GPIOE_CLK_ENABLE();					 // Enable the GPIOE Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI4;	
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure); 
	#else
	#error You should specify the GPIO pins used on STM32F7 SPI4 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	__HAL_RCC_SPI4_CLK_ENABLE();
	hspi.Instance = SPI4;
#else
#error Invalid M8266WIFI_SPI_INTERFACE_NO defined in brd_cfg.h 
#endif

  hspi.Init.Mode 									= SPI_MODE_MASTER;
  hspi.Init.Direction 						= SPI_DIRECTION_2LINES;
  hspi.Init.DataSize 							= SPI_DATASIZE_8BIT;
  hspi.Init.CLKPolarity 					= SPI_POLARITY_LOW;
  hspi.Init.CLKPhase 							= SPI_PHASE_1EDGE;
  hspi.Init.NSS 									= SPI_NSS_SOFT;
  hspi.Init.BaudRatePrescaler 		= SPI_BAUDRATEPRESCALER_8;
  hspi.Init.FirstBit 							= SPI_FIRSTBIT_MSB;
  hspi.Init.TIMode 								= SPI_TIMODE_DISABLE;
  hspi.Init.CRCCalculation 				= SPI_CRCCALCULATION_DISABLE;
  hspi.Init.CRCPolynomial 				= 7;
  hspi.Init.CRCLength 						= SPI_CRC_LENGTH_DATASIZE;
  hspi.Init.NSSPMode 							= SPI_NSS_PULSE_DISABLE; //SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi) != HAL_OK)
  {
    while(1);
  }
  __HAL_SPI_ENABLE(&hspi);

////////////////////////////////////////////////////////////////////////////////////
///////////////  STM32L1  //////////////////////////////////////////////////////////
#elif defined(MCU_IS_STM32L1XX)
  GPIO_InitTypeDef  GPIO_InitStructure;
#if (M8266WIFI_SPI_INTERFACE_NO == 1)
	#if 0 // if use GPIOA5/6/7 for SPI1 SCK/MISO/MOSI
	__HAL_RCC_GPIOA_CLK_ENABLE();					// Enable the GPIOE Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI1;	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);  
	#elif 1 // if use GPIOB3/4/5 for SPI1 SCK/MISO/MOSI
	__HAL_RCC_GPIOB_CLK_ENABLE();					// Enable the GPIOB Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI1;	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  
	#else
	#error You should specify the GPIO pins used on STM32F7 SPI1 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	__HAL_RCC_SPI1_CLK_ENABLE();
	hspi.Instance = SPI1;	
// end of if (M8266WIFI_SPI_INTERFACE_NO == 1) for STM32F7

#elif (M8266WIFI_SPI_INTERFACE_NO == 2)
	#if 0 // if use GPIOB13/14/15 for SPI2 SCK/MISO/MOSI
	__HAL_RCC_GPIOB_CLK_ENABLE();					// Enable the GPIOB Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  
  #elif 1 // if use GPIO PD1/PD3/PD4 for SPI2 SCK/MISO/MOSI
	__HAL_RCC_GPIOD_CLK_ENABLE();					// Enable the GPIOD Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;	
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);  
	#else
	#error You should specify the GPIO pins used on STM32F7 SPI2 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	__HAL_RCC_SPI2_CLK_ENABLE();
	hspi.Instance = SPI2;	
// end of if (M8266WIFI_SPI_INTERFACE_NO == 2) for STM32F7

#elif (M8266WIFI_SPI_INTERFACE_NO == 3)   // L15x Serials have only SPI1 and SPI2, no SPI3
	#if 1 // if use GPIOB3/4/5 for SPI3 SCK/MISO/MOSI
	__HAL_RCC_GPIOB_CLK_ENABLE();					// Enable the GPIOB Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF6_SPI3;	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  
  #elif 0 // if use GPIOC10/11/12 for SPI3 SCK/MISO/MOSI
	__HAL_RCC_GPIOC_CLK_ENABLE();					// Enable the GPIOC Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF6_SPI3;	
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);  
	#else
	#error You should specify the GPIO pins used on STM32F7 SPI3 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	__HAL_RCC_SPI3_CLK_ENABLE();
	hspi.Instance = SPI3;	
// end of if (M8266WIFI_SPI_INTERFACE_NO == 3) for STM32F7
#else
#error Invalid M8266WIFI_SPI_INTERFACE_NO defined in brd_cfg.h 
#endif

  hspi.Init.Mode 									= SPI_MODE_MASTER;
  hspi.Init.Direction 						= SPI_DIRECTION_2LINES;
  hspi.Init.DataSize 							= SPI_DATASIZE_8BIT;
  hspi.Init.CLKPolarity 					= SPI_POLARITY_LOW;
  hspi.Init.CLKPhase 							= SPI_PHASE_1EDGE;
  hspi.Init.NSS 									= SPI_NSS_SOFT;
  hspi.Init.BaudRatePrescaler 		= SPI_BAUDRATEPRESCALER_8;
  hspi.Init.FirstBit 							= SPI_FIRSTBIT_MSB;
  hspi.Init.TIMode 								= SPI_TIMODE_DISABLE;
  hspi.Init.CRCCalculation 				= SPI_CRCCALCULATION_DISABLE;
  hspi.Init.CRCPolynomial 				= 7;
  if (HAL_SPI_Init(&hspi) != HAL_OK)
  {
    while(1);
  }
  __HAL_SPI_ENABLE(&hspi); 

////////////////////////////////////////////////////////////////////////////////////
///////////////  STM32L4  //////////////////////////////////////////////////////////
#elif defined(MCU_IS_STM32L4XX)
  GPIO_InitTypeDef  GPIO_InitStructure;
#if (M8266WIFI_SPI_INTERFACE_NO == 1)
	#if 0 // if use GPIOA5/6/7 for SPI1 SCK/MISO/MOSI
	__HAL_RCC_GPIOA_CLK_ENABLE();					// Enable the GPIOE Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI1;	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);  
	#elif 1 // if use GPIOB3/4/5 for SPI1 SCK/MISO/MOSI
	__HAL_RCC_GPIOB_CLK_ENABLE();					// Enable the GPIOB Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI1;	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  
	#else
	#error You should specify the GPIO pins used on STM32F7 SPI1 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	__HAL_RCC_SPI1_CLK_ENABLE();
	hspi.Instance = SPI1;	
// end of if (M8266WIFI_SPI_INTERFACE_NO == 1) for STM32F7

#elif (M8266WIFI_SPI_INTERFACE_NO == 2)
	#if 1 // if use GPIOB13/14/15 for SPI2 SCK/MISO/MOSI
	__HAL_RCC_GPIOB_CLK_ENABLE();					// Enable the GPIOB Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  
  #elif 0 // if use GPIOB10/PC2/PC3 for SPI2 SCK/MISO/MOSI
	__HAL_RCC_GPIOB_CLK_ENABLE();					// Enable the GPIOB Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_10;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  
	__HAL_RCC_GPIOC_CLK_ENABLE();					// Enable the GPIOC Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;	
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);  
	#else
	#error You should specify the GPIO pins used on STM32F7 SPI2 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	__HAL_RCC_SPI2_CLK_ENABLE();
	hspi.Instance = SPI2;	
// end of if (M8266WIFI_SPI_INTERFACE_NO == 2) for STM32F7

#elif (M8266WIFI_SPI_INTERFACE_NO == 3)
	#if 1 // if use GPIOB3/4/5 for SPI3 SCK/MISO/MOSI
	__HAL_RCC_GPIOB_CLK_ENABLE();					// Enable the GPIOB Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF6_SPI3;	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  
  #elif 0 // if use GPIOC10/11/12 for SPI3 SCK/MISO/MOSI
	__HAL_RCC_GPIOC_CLK_ENABLE();					// Enable the GPIOC Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF6_SPI3;	
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);  
	#else
	#error You should specify the GPIO pins used on STM32F7 SPI3 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	__HAL_RCC_SPI3_CLK_ENABLE();
	hspi.Instance = SPI3;	
// end of if (M8266WIFI_SPI_INTERFACE_NO == 3) for STM32F7

#elif (M8266WIFI_SPI_INTERFACE_NO == 4)  
  #if 1 // if use SPI1 GPIOE2/5/6 for SCK/MISO/MOSI
	__HAL_RCC_GPIOE_CLK_ENABLE();					 // Enable the GPIOE Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI4;	
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure); 
  #elif 0 // if use SPI1 GPIOE12/13/14 for SCK/MISO/MOSI
	__HAL_RCC_GPIOE_CLK_ENABLE();					 // Enable the GPIOE Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI4;	
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure); 
	#else
	#error You should specify the GPIO pins used on STM32F7 SPI4 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	__HAL_RCC_SPI4_CLK_ENABLE();
	hspi.Instance = SPI4;
#else
#error Invalid M8266WIFI_SPI_INTERFACE_NO defined in brd_cfg.h 
#endif

  hspi.Init.Mode 									= SPI_MODE_MASTER;
  hspi.Init.Direction 						= SPI_DIRECTION_2LINES;
  hspi.Init.DataSize 							= SPI_DATASIZE_8BIT;
  hspi.Init.CLKPolarity 					= SPI_POLARITY_LOW;
  hspi.Init.CLKPhase 							= SPI_PHASE_1EDGE;
  hspi.Init.NSS 									= SPI_NSS_SOFT;
  hspi.Init.BaudRatePrescaler 		= SPI_BAUDRATEPRESCALER_8;
  hspi.Init.FirstBit 							= SPI_FIRSTBIT_MSB;
  hspi.Init.TIMode 								= SPI_TIMODE_DISABLE;
  hspi.Init.CRCCalculation 				= SPI_CRCCALCULATION_DISABLE;
  hspi.Init.CRCPolynomial 				= 7;
  hspi.Init.CRCLength 						= SPI_CRC_LENGTH_DATASIZE;
  hspi.Init.NSSPMode 							= SPI_NSS_PULSE_DISABLE; //SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi) != HAL_OK)
  {
    while(1);
  }
  __HAL_SPI_ENABLE(&hspi); 
	
////////////////////////////////////////////////////////////////////////////////////
///////////////  STM32H7  //////////////////////////////////////////////////////////
#elif defined(MCU_IS_STM32H7XX)
  GPIO_InitTypeDef  GPIO_InitStructure;
#if (M8266WIFI_SPI_INTERFACE_NO == 1)
	#if 0 // if use GPIOA5/6/7 for SPI1 SCK/MISO/MOSI
	__HAL_RCC_GPIOA_CLK_ENABLE();					// Enable the GPIOE Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI1;	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);  
	#elif 1 // if use GPIOB3/4/5 for SPI1 SCK/MISO/MOSI
	__HAL_RCC_GPIOB_CLK_ENABLE();					// Enable the GPIOB Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI1;	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  
	#else
	#error You should specify the GPIO pins used on STM32F7 SPI1 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	__HAL_RCC_SPI1_CLK_ENABLE();
	hspi.Instance = SPI1;	
// end of if (M8266WIFI_SPI_INTERFACE_NO == 1) for STM32F7

#elif (M8266WIFI_SPI_INTERFACE_NO == 2)
	#if 1 // if use GPIOB13/14/15 for SPI2 SCK/MISO/MOSI
	__HAL_RCC_GPIOB_CLK_ENABLE();					// Enable the GPIOB Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  
  #elif 0 // if use GPIOB10/PC2/PC3 for SPI2 SCK/MISO/MOSI
	__HAL_RCC_GPIOB_CLK_ENABLE();					// Enable the GPIOB Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_10;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  
	__HAL_RCC_GPIOC_CLK_ENABLE();					// Enable the GPIOC Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;	
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);  
	#else
	#error You should specify the GPIO pins used on STM32F7 SPI2 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	__HAL_RCC_SPI2_CLK_ENABLE();
	hspi.Instance = SPI2;	
// end of if (M8266WIFI_SPI_INTERFACE_NO == 2) for STM32F7

#elif (M8266WIFI_SPI_INTERFACE_NO == 3)
	#if 0 // if use GPIOB3/4/5 for SPI3 SCK/MISO/MOSI
	__HAL_RCC_GPIOB_CLK_ENABLE();					// Enable the GPIOB Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_3 | GPIO_PIN_4;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF6_SPI3;	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	GPIO_InitStructure.Pin       = GPIO_PIN_5;	
  GPIO_InitStructure.Alternate = GPIO_AF7_SPI3;		//kindly noted that the SPI3_MOSI @ PB5 -> AF7_SPI3 other than AF6_SPI3	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
  #elif 1 // if use GPIOC10/11/12 for SPI3 SCK/MISO/MOSI
	__HAL_RCC_GPIOC_CLK_ENABLE();					// Enable the GPIOC Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF6_SPI3;	
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);  
	#else
	#error You should specify the GPIO pins used on STM32F7 SPI3 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	__HAL_RCC_SPI3_CLK_ENABLE();
	hspi.Instance = SPI3;	
// end of if (M8266WIFI_SPI_INTERFACE_NO == 3) for STM32H7
#elif (M8266WIFI_SPI_INTERFACE_NO == 4)  
  #if 1 // if use GPIOE2/5/6 for SPI4 SCK/MISO/MOSI
	__HAL_RCC_GPIOE_CLK_ENABLE();					 // Enable the GPIOE Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI4;	
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure); 
  #elif 0 // if use GPIOE12/13/14 for SPI4 SCK/MISO/MOSI
	__HAL_RCC_GPIOE_CLK_ENABLE();					 // Enable the GPIOE Clock
	GPIO_InitStructure.Pin       = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI4;	
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure); 
	#else
	#error You should specify the GPIO pins used on STM32F7 SPI4 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	__HAL_RCC_SPI4_CLK_ENABLE();
	hspi.Instance = SPI4;
#else
#error Invalid M8266WIFI_SPI_INTERFACE_NO defined in brd_cfg.h 
#endif

  hspi.Init.Mode 									= SPI_MODE_MASTER;
  hspi.Init.Direction 						= SPI_DIRECTION_2LINES;
  hspi.Init.DataSize 							= SPI_DATASIZE_8BIT;
  hspi.Init.CLKPolarity 					= SPI_POLARITY_LOW;
  hspi.Init.CLKPhase 							= SPI_PHASE_1EDGE;
  hspi.Init.NSS 									= SPI_NSS_SOFT;
  hspi.Init.BaudRatePrescaler 		= SPI_BAUDRATEPRESCALER_8;
  hspi.Init.FirstBit 							= SPI_FIRSTBIT_MSB;
  hspi.Init.TIMode 								= SPI_TIMODE_DISABLE;
  hspi.Init.CRCCalculation 				= SPI_CRCCALCULATION_DISABLE;
  hspi.Init.CRCPolynomial 				= 7;
  hspi.Init.CRCLength 						= SPI_CRC_LENGTH_DATASIZE;
  hspi.Init.NSSPMode 							= SPI_NSS_PULSE_DISABLE; //SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi) != HAL_OK)
  {
    while(1);
  }
  __HAL_SPI_ENABLE(&hspi); 
	
#else
#error YOU SHOULD DEFINED MCU_IS_STM32FX?XX in brd_cfg.h
#endif
	
}
/***********************************************************************************
 * M8266HostIf_SPI_SetSpeed                                                        *
 * Description                                                                     *
 *    To setup the SPI Clock Speed for M8266WIFI module                            *
 * Parameter(s):                                                                   *
 *    SPI_BaudRatePrescaler: SPI BaudRate Prescaler                                *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/
void M8266HostIf_SPI_SetSpeed(u32 SPI_BaudRatePrescaler)
{
#if  defined(MCU_IS_STM32F1XX) || defined(MCU_IS_STM32F3XX) || defined(MCU_IS_STM32F2XX) || defined(MCU_IS_STM32F4XX)	
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	M8266WIFI_INTERFACE_SPI->CR1&=0XFFC7;
	M8266WIFI_INTERFACE_SPI->CR1|=SPI_BaudRatePrescaler;
	SPI_Cmd(M8266WIFI_INTERFACE_SPI,ENABLE);
#elif defined(MCU_IS_STM32F7XX) || defined(MCU_IS_STM32L1XX) || defined(MCU_IS_STM32L4XX)
	__HAL_SPI_DISABLE(&hspi);
  hspi.Instance->CR1&=0XFFC7;
  hspi.Instance->CR1|=SPI_BaudRatePrescaler;
  __HAL_SPI_ENABLE(&hspi);
#elif defined(MCU_IS_STM32H7XX)
	__HAL_SPI_DISABLE(&hspi);
	hspi.Instance->CFG1 &= ~(7UL<<28);
	hspi.Instance->CFG1 |= SPI_BaudRatePrescaler;
  __HAL_SPI_ENABLE(&hspi);
#else
#error YOU SHOULD DEFINED MCU_IS_STM32F?XX in brd_cfg.h
#endif	
} 

/***********************************************************************************
 * M8266HostIf_Init                                                                *
 * Description                                                                     *
 *    To initialise the Host interface for M8266WIFI module                        *
 * Parameter(s):                                                                   *
 *    baud: baud rate to set                                                       *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/ 
void M8266HostIf_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//	Setup Interrupt Priority Group2
    delay_init(168);  															//  Initialise the Delay Function Parameter
    M8266HostIf_GPIO_CS_RESET_Init();
	
    M8266HostIf_SPI_Init();
//	 M8266HostIf_SPI_SetSpeed(SPI_BaudRatePrescaler_8);
	
}

//////////////////////////////////////////////////////////////////////////////////////
// BELOW FUNCTIONS ARE REQUIRED BY M8266WIFIDRV.LIB. 
// PLEASE IMPLEMENTE THEM ACCORDING TO YOUR HARDWARE
//////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************
 * M8266HostIf_Set_nRESET_Pin                                                      *
 * Description                                                                     *
 *    To Outpout HIGH or LOW onto the GPIO pin for M8266WIFI nRESET                *
 *    You may update the macros of GPIO PIN usages for nRESET from brd_cfg.h       *
 *    You are not recommended to modify codes below please                         *
 * Parameter(s):                                                                   *
 *    1. level: LEVEL output to nRESET pin                                         *
 *              0 = output LOW  onto nRESET                                        *
 *              1 = output HIGH onto nRESET                                        *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/
void M8266HostIf_Set_nRESET_Pin(u8 level)
{
	  if(level!=0)
		//GPIO_SetBits(M8266WIFI_nRESET_GPIO, M8266WIFI_nRESET_PIN);
#if   defined(MCU_IS_STM32F1XX)
		  M8266WIFI_nRESET_GPIO->BSRR  = M8266WIFI_nRESET_PIN;
#elif defined(MCU_IS_STM32F2XX) || defined(MCU_IS_STM32F4XX) || defined(MCU_IS_STM32H7XX)
		  M8266WIFI_nRESET_GPIO->BSRRL = M8266WIFI_nRESET_PIN;
#elif defined(MCU_IS_STM32F3XX) || defined(MCU_IS_STM32F7XX) || defined(MCU_IS_STM32L1XX) || defined(MCU_IS_STM32L4XX)
		  M8266WIFI_nRESET_GPIO->BSRR  = M8266WIFI_nRESET_PIN;
#else
#error YOU SHOULD DEFINED MCU_IS_STM32F?XX in brd_cfg.h
#endif

		else
		//GPIO_ResetBits(M8266WIFI_nRESET_GPIO, M8266WIFI_nRESET_PIN);
#if   defined(MCU_IS_STM32F1XX)
		  M8266WIFI_nRESET_GPIO->BRR   = M8266WIFI_nRESET_PIN;
#elif defined(MCU_IS_STM32F2XX) || defined(MCU_IS_STM32F4XX) || defined(MCU_IS_STM32H7XX)
		  M8266WIFI_nRESET_GPIO->BSRRH = M8266WIFI_nRESET_PIN;
#elif defined(MCU_IS_STM32F3XX) || defined(MCU_IS_STM32F7XX) || defined(MCU_IS_STM32L1XX) || defined(MCU_IS_STM32L4XX)
		  M8266WIFI_nRESET_GPIO->BSRR  = M8266WIFI_nRESET_PIN<<16;
#else
#error YOU SHOULD DEFINED MCU_IS_STM32F?XX in brd_cfg.h
#endif
}
/***********************************************************************************
 * M8266HostIf_Set_SPI_nCS_PIN                                                     *
 * Description                                                                     *
 *    To Outpout HIGH or LOW onto the GPIO pin for M8266WIFI SPI nCS               *
 *    You may update the macros of GPIO PIN usages for SPI nCS from brd_cfg.h      *
 *    You are not recommended to modify codes below please                         *
 * Parameter(s):                                                                   *
 *    1. level: LEVEL output to SPI nCS pin                                        *
 *              0 = output LOW  onto SPI nCS                                       *
 *              1 = output HIGH onto SPI nCS                                       *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/
void M8266HostIf_Set_SPI_nCS_Pin(u8 level)
{
	  if(level!=0)
		//GPIO_SetBits(M8266WIFI_SPI_nCS_GPIO, M8266WIFI_SPI_nCS_PIN);
#if   defined(MCU_IS_STM32F1XX)
		  M8266WIFI_SPI_nCS_GPIO->BSRR  = M8266WIFI_SPI_nCS_PIN;
#elif defined(MCU_IS_STM32F2XX) || defined(MCU_IS_STM32F4XX) || defined(MCU_IS_STM32H7XX)
		  M8266WIFI_SPI_nCS_GPIO->BSRRL = M8266WIFI_SPI_nCS_PIN;
#elif defined(MCU_IS_STM32F3XX) || defined(MCU_IS_STM32F7XX) || defined(MCU_IS_STM32L1XX) || defined(MCU_IS_STM32L4XX) 
		  M8266WIFI_SPI_nCS_GPIO->BSRR  = M8266WIFI_SPI_nCS_PIN;
#else
#error YOU SHOULD DEFINED MCU_IS_STM32F?XX in brd_cfg.h
#endif

		else
		//GPIO_ResetBits(M8266WIFI_SPI_nCS_GPIO, M8266WIFI_SPI_nCS_PIN);
#if   defined(MCU_IS_STM32F1XX)
		  M8266WIFI_SPI_nCS_GPIO->BRR   = M8266WIFI_SPI_nCS_PIN;
#elif defined(MCU_IS_STM32F2XX) || defined(MCU_IS_STM32F4XX) || defined(MCU_IS_STM32H7XX)
		  M8266WIFI_SPI_nCS_GPIO->BSRRH = M8266WIFI_SPI_nCS_PIN;
#elif defined(MCU_IS_STM32F3XX) || defined(MCU_IS_STM32F7XX) || defined(MCU_IS_STM32L1XX) || defined(MCU_IS_STM32L4XX)
      M8266WIFI_SPI_nCS_GPIO->BSRR  = M8266WIFI_SPI_nCS_PIN<<16;		
#else
#error YOU SHOULD DEFINED MCU_IS_STM32F?XX in brd_cfg.h
#endif
}

/***********************************************************************************
 * M8266WIFIHostIf_delay_us                                                        *
 * Description                                                                     *
 *    To loop delay some micro seconds.                                            *
 * Parameter(s):                                                                   *
 *    1. nus: the micro seconds to delay                                           *
 * Return:                                                                         *
 *    none                                                                         *
 ***********************************************************************************/
void M8266HostIf_delay_us(u8 nus)
{
   delay_us(nus);
}

/***********************************************************************************
 * M8266HostIf_SPI_ReadWriteByte                                                   *
 * Description                                                                     *
 *    To write a byte onto the SPI bus from MCU MOSI to the M8266WIFI module       *
 *    and read back a byte from the SPI bus MISO meanwhile                         *
 *    You may update the macros of SPI usage from brd_cfg.h                        *
 * Parameter(s):                                                                   *
 *    1. TxdByte: the byte to be sent over MOSI                                    *
 * Return:                                                                         *
 *    1. The byte read back from MOSI meanwhile                                    *                                                                         *
 ***********************************************************************************/
u8 M8266HostIf_SPI_ReadWriteByte(u8 TxdByte)
{
#if  defined(MCU_IS_STM32F1XX) || defined(MCU_IS_STM32F2XX) || defined(MCU_IS_STM32F4XX)	
	while(SPI_I2S_GetFlagStatus(M8266WIFI_INTERFACE_SPI, SPI_I2S_FLAG_TXE) == RESET){} 	//wait SPI TXD Buffer Empty
	SPI_I2S_SendData(M8266WIFI_INTERFACE_SPI, TxdByte); 			//Write the byte to the TXD buffer and then shift out via MOSI
	while(SPI_I2S_GetFlagStatus(M8266WIFI_INTERFACE_SPI, SPI_I2S_FLAG_RXNE) == RESET){} 	//wait the SPI RXD buffer has received the data from MISO
	return SPI_I2S_ReceiveData(M8266WIFI_INTERFACE_SPI);  		//return the data received from MISO
#elif defined(MCU_IS_STM32F3XX)	
	while(SPI_I2S_GetFlagStatus(M8266WIFI_INTERFACE_SPI, SPI_I2S_FLAG_TXE) == RESET){} 	//wait SPI TXD Buffer Empty
	SPI_SendData8(M8266WIFI_INTERFACE_SPI, TxdByte); 			//Write the byte to the TXD buffer and then shift out via MOSI
	while(SPI_I2S_GetFlagStatus(M8266WIFI_INTERFACE_SPI, SPI_I2S_FLAG_RXNE) == RESET){} 	//wait the SPI RXD buffer has received the data from MISO
	return SPI_ReceiveData8(M8266WIFI_INTERFACE_SPI);  		//return the data received from MISO
#elif defined(MCU_IS_STM32F7XX) || defined(MCU_IS_STM32L1XX) || defined(MCU_IS_STM32L4XX) || defined(MCU_IS_STM32H7XX)
  u8 Rxdata;
  HAL_SPI_TransmitReceive(&hspi,&TxdByte,&Rxdata,1, 1000);       
 	return Rxdata; 
#else
#error YOU SHOULD DEFINED MCU_IS_STM32F?XX in brd_cfg.h
#endif	
}
