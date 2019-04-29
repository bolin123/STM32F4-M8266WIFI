/********************************************************************
 * M8266WIFI_ops.c
 * .Description
 *     source file of M8266WIFI HAL operations
 * .Copyright(c) Anylinkin Technology 2015.5-
 *     IoT@anylinkin.com
 *     http://www.anylinkin.com
 *     http://anylinkin.taobao.com
 *  Author
 *     wzuo
 *  Date
 *  Version
 ********************************************************************/
#include "stdio.h"
#include "string.h"
#include "brd_cfg.h"
//#include "led.h"
#include "M8266WIFIDrv.h"
#include "M8266HostIf.h"
#include "M8266WIFI_ops.h"

void M8266WIFI_Module_delay_ms(u16 nms)
{
	 u16 i, j;
	 for(i=0; i<nms; i++)
	   for(j=0; j<4; j++)									// delay 1ms. Call 4 times of delay_us(250), as M8266HostIf_delay_us(u8 nus), nus max 255
	      M8266HostIf_delay_us(250);
}

/************************************************************************************************************************
 * M8266WIFI_Module_Hardware_Reset                                                                                      *
 * Description                                                                                                          *
 *    1. To perform a hardware reset to M8266WIFI module via the nReset Pin                                             *
 *       and bring M8266WIFI module to boot up from external SPI flash                                                  *
 *    2. In order to make sure the M8266WIFI module bootup from external                                                *
 *       SPI flash, nCS should be low during Reset out via nRESET pin                                                   *
 * Parameter(s):                                                                                                        *
 *    none                                                                                                              *
 * Return:                                                                                                              *
 *    none                                                                                                              *
 ************************************************************************************************************************/
void M8266WIFI_Module_Hardware_Reset(void) // total 800ms
{
	 M8266HostIf_Set_SPI_nCS_Pin(0);   			// Module nCS==ESP8266 GPIO15 as well, Low during reset in order for a normal reset 
	 M8266WIFI_Module_delay_ms(1); 	    		// delay 1ms, adequate for nCS stable
	
	 M8266HostIf_Set_nRESET_Pin(0);					// Pull low the nReset Pin to bring the module into reset state
	 M8266WIFI_Module_delay_ms(5);      		// delay 1ms, adequate for nRESET stable.
																					// give more time especially for some board not good enough
	
	 M8266HostIf_Set_nRESET_Pin(1);					// Pull high again the nReset Pin to bring the module exiting reset state
	 M8266WIFI_Module_delay_ms(300); 	  		// at least 18ms required for reset-out-boot sampling boottrap pin
																					// Here, we use 300ms for adequate abundance, since some board GPIO,
																					// needs more time for stable(especially for nRESET)
																					// You may shorten the time or give more time here according your board v.s. effiency
 	 M8266HostIf_Set_SPI_nCS_Pin(1);
 //M8266WIFI_Module_delay_ms(1); 	    		// delay 1ms, adequate for nCS stable
	
   M8266WIFI_Module_delay_ms(800-300-5-2); // Delay more than around 500ms for M8266WIFI module bootup and initialization 
																					 // including bootup information print¡£ 
}
/***********************************************************************************
 * M8266WIFI_SPI_wait_sta_connecting_to_ap_and_get_ip                              *
 * Description                                                                     *
 *    Wait M8266WIFI in STA mode connecting to AP/Router and get ip address        *
 *    via SPI API functions in M8266WIFIDrv.lib                                    *
 * Parameter(s):                                                                   *
 *    1. sta_ip: if success, sta_ip return the ip address achieved                 *
 *    2. max_wait_time_in_s: max timeout to wait in seconds                        *
 * Return:                                                                         *
 *       1 = M8266WIFI module connects AP/router and gets a valid IP               *
 *           from AP/router before the timeout succefully                          *
 *       0 = M8266WIFI module fails to connect the AP/router and get               *
 *           a valid IP before the timeout                                         *
 ***********************************************************************************/
u8 M8266WIFI_SPI_wait_sta_connecting_to_ap_and_get_ip(char* sta_ip, u8 max_wait_time_in_s)
{
	 u16  i;
	 u16  status=0;
	 
	 for(i=0; i<1*max_wait_time_in_s; i++)  // max wait
	 {
		   if(   (M8266WIFI_SPI_Get_STA_IP_Addr(sta_ip , &status) ==1)
				   && (strcmp(sta_ip, "0.0.0.0")!=0) )
           break;
     			 
			M8266WIFI_Module_delay_ms(1000);
			continue;
   }
   if(i>=1*max_wait_time_in_s)
		 return 0; // false
	 
	 return 1;  // true
}

/***********************************************************************************
 * M8266WIFI_Module_Init_Via_SPI(void)                                             *
 * Description                                                                     *
 *    To perform a Initialization sequency to M8266WIFI module via SPI I/F         *
 *    (1) Reboot the M8266WIFI module via nRESET pin                               *
 *    (2) Wait and Check if the M8266WIFI module gets an valid IP address          *
 *        if the module in STA or STA+AP mode                                      *
 *    Call this function after Host SPI Interface Initialised                      *
 *    if use SPI interface to config module                                        *
 * Parameter(s):                                                                   *
 *    none                                                                         *
 * Return:                                                                         *
 * Return:                                                                         *
 *       0 = failed                                                                *
 *       1 = success                                                               * 
 ***********************************************************************************/
 u8 M8266WIFI_Module_Init_Via_SPI(void)
 {
	u32  	spi_clk = 40000000;
//	u8   	sta_ap_mode = 0;
//	u8   	connection_status = 0xFF;
//	char 	sta_ip[15+1]={0};
//	char 	ssid[32];
//	s8   	rssi;
	u16  	status = 0;
	
  //////////////////////////////////////////////////////////////////////////////////////////////////////
	//To hardware reset the module (with nCS=0 during reset) and wait up the module bootup
	M8266WIFI_Module_Hardware_Reset();
	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
  // Try SPI clock in a fast one as possible up to 40MHz (M8266WIFI could support only upto 40MHz SPI) 
	/* Notes: 
	     As per STM32, SPI1/4 Clock Divided from APB2 Clock, SPI1/4 CLOCK = APB2_CLOCK/SPI_BaudRatePrescaler
	                SPI2/3 Clock Divided from APB1 Clock, SPI2/3 CLOCK = APB1_CLOCK/SPI_BaudRatePrescaler
	         (1) For STM32F1, , please see system_stm32f10x.c, APB2_CLOCK = PCLK2 = HCLK = SYSCLK, APB1_CLOCK=PCLK1 = HCLK/4 = SYSCLK/2! please see system_stm32f10x.c, 
	                                    SYSCLK  = 72MHz, so, APB2_CLOCK = 72MHz
		       (2) For STM32F2, please see system_stm32f2xx.c, APB2_CLOCK = PCLK2 = HCLK/2 = SYSCLK/2, APB1_CLOCK=PCLK1 = HCLK/4 = SYSCLK/4! please see system_stm32f2xx.c, 
	                 - STM32F207xx:     SYSCLK          = 120MHz, so, APB2_CLOCK = 60MHz, APB1_CLOCK = 30MHz 
	         (3) For STM32F4, APB2_CLOCK = PCLK2 = HCLK/2 = SYSCLK/2, APB1_CLOCK=PCLK1 = HCLK/4 = SYSCLK/4! please see system_stm32f4xx.c, 
	                          Meanwhile, the PCLK2 should <= 90MHz, and SPI_CLK should <= 45MHz
								   - STM32F401xx:     SYSCLK          = 168MHz, so, APB2_CLOCK = 84MHz, APB1=42MHz
	                 - STM32F40_41xxx:  SYSCLK          = 168MHz, so, APB2_CLOCK = 84MHz, APB1=42MHz
	                 - STM32F427_437xx, STM32F429_439xx = 180MHz, so, APB2_CLOCK = 90MHz, APB1=45MHz
	         (4) For STM32F7, please see sys.c, APB2_CLOCK = PCLK2 = HCLK/2 = SYSCLK/2
	                          Meanwhile, the PCLK2 should <= 108MHz, and SPI_CLK should <=54MHz
	                 - STM32F767xx:     SYSCLK          = 216MHz, so, APB2_CLOCK = 108MHz, APB2_CLOCK = 54MHz
	         (5) For STM32L1, please see sys.c, APB1_CLK = APB2_CLOCK = HCLK = SYSCLK = 32MHz				 
	         (6) For STM32L4, please see sys.c, APB1_CLK = APB2_CLOCK = HCLK = SYSCLK = 80MHz
					 
					 (6) For STM32H7, using reset default:
									 SPI 1/2/3 kernel clock = pll1_q_ck = 200MHz
									 SPI 4/5   kernel clock = APB clock = 100MHz

		   For MCU_IS_NXP_LPC17XX
	         (1) SPI Clock =PCLK_SPI/SPI_BaudRatePrescaler
	         (2) PCLK_SPI  =  CPU_CLK = 96MHz, please see system_LPC17xx.c
					 
		   For MMCU_IS_NXP_MK27_28
	         (1) SPI 0/1/2/3 SPI Clock = BUS_CLOCK= (75MHz)/SPI_BaudRateScaler(=2)/SPI_BaudRatePrecaler = 37.5MHz/SPI_BaudRatePrecaler
					 (2) SPI 3       SPI Clock = SYS_CLOCK=(150MHz)/SPI_BaudRateScaler(=2)/SPI_BaudRatePrecaler = 75.0MHz/SPI_BaudRatePrecaler
					 
	     For MCU_IS_HT32F16XX
	         (1) SPI Clock   =  APB_CLK_SPI / SPI_BaudRatePrescaler
	         (2) APB_CLK_SPI =  SYS_CLK /2 = 144MHz /2 = 72MHz, please see system_ht32f165x.c
*/
#if defined(MCU_IS_STM32) && !defined(MCU_IS_STM32H7XX)
#ifndef SPI_BaudRatePrescaler_2
#define SPI_BaudRatePrescaler_2 				((uint32_t)0x00000000U)
#define SPI_BaudRatePrescaler_4         ((uint32_t)0x00000008U)
#define SPI_BaudRatePrescaler_8         ((uint32_t)0x00000010U)
#define SPI_BaudRatePrescaler_16        ((uint32_t)0x00000018U)
#define SPI_BaudRatePrescaler_32        ((uint32_t)0x00000020U)
#define SPI_BaudRatePrescaler_64        ((uint32_t)0x00000028U)
#define SPI_BaudRatePrescaler_128       ((uint32_t)0x00000030U)
#define SPI_BaudRatePrescaler_256       ((uint32_t)0x00000038U)
#endif

#elif defined(MCU_IS_STM32H7XX)
#ifndef SPI_BAUDRATEPRESCALER_2
#define SPI_BAUDRATEPRESCALER_2         (0x00000000U)
#define SPI_BAUDRATEPRESCALER_4         (0x10000000U)
#define SPI_BAUDRATEPRESCALER_8         (0x20000000U)
#define SPI_BAUDRATEPRESCALER_16        (0x30000000U)
#define SPI_BAUDRATEPRESCALER_32        (0x40000000U)
#define SPI_BAUDRATEPRESCALER_64        (0x50000000U)
#define SPI_BAUDRATEPRESCALER_128       (0x60000000U)
#define SPI_BAUDRATEPRESCALER_256       (0x70000000U)
#endif

#elif defined(MCU_IS_NXP_LPC17XX)
#ifndef SPI_BaudRatePrescaler_2
#define SPI_BaudRatePrescaler_2         ((uint32_t)0x00000002U)
#define SPI_BaudRatePrescaler_4         ((uint32_t)0x00000004U)
#define SPI_BaudRatePrescaler_6         ((uint32_t)0x00000006U)
#define SPI_BaudRatePrescaler_8         ((uint32_t)0x00000008U)
#define SPI_BaudRatePrescaler_16        ((uint32_t)0x00000010U)
#define SPI_BaudRatePrescaler_32        ((uint32_t)0x00000020U)
#define SPI_BaudRatePrescaler_64        ((uint32_t)0x00000040U)
#define SPI_BaudRatePrescaler_128       ((uint32_t)0x00000080U)
#define SPI_BaudRatePrescaler_256       ((uint32_t)0x00000100U)
#endif

#elif defined (MCU_IS_NXP_MK27_28)
#ifndef SPI_BaudRatePrescaler_2
#define SPI_BaudRatePrescaler_2         ((uint32_t)0x00000000U)
#define SPI_BaudRatePrescaler_4         ((uint32_t)0x00000001U)
#define SPI_BaudRatePrescaler_6         ((uint32_t)0x00000002U)
#define SPI_BaudRatePrescaler_8         ((uint32_t)0x00000003U)
#define SPI_BaudRatePrescaler_16        ((uint32_t)0x00000004U)
#define SPI_BaudRatePrescaler_32        ((uint32_t)0x00000005U)
#define SPI_BaudRatePrescaler_64        ((uint32_t)0x00000006U)
#define SPI_BaudRatePrescaler_128       ((uint32_t)0x00000007U)
#define SPI_BaudRatePrescaler_256       ((uint32_t)0x00000008U)
#endif
#else
#endif

#if    defined(MCU_IS_STM32F1XX) || defined(MCU_IS_STM32F3XX) // F1 has SPI, SPI2 & SPI3
  #if (M8266WIFI_SPI_INTERFACE_NO == 1)
		M8266HostIf_SPI_SetSpeed(SPI_BaudRatePrescaler_4);				// Setup SPI Clock. Here 72/4 = 18MHz for STM32F2xx SPI1 or SPI4, up to 18MHz, since SPI1/4 clock devided from faster APB2 clock
  #elif (M8266WIFI_SPI_INTERFACE_NO == 2) || (M8266WIFI_SPI_INTERFACE_NO == 3)
		M8266HostIf_SPI_SetSpeed(SPI_BaudRatePrescaler_2);				// Setup SPI Clock. Here 36/2 = 18MHz for STM32F2xx SPI2 or SPI3, up to 18MHz, since SPI2/3 clock devided from lowver APB1 clock
	#endif
 	  spi_clk =  18000000;
		
#elif  defined(MCU_IS_STM32F2XX)  // F2 has SPI1, SPI2 & SPI3
  #if (M8266WIFI_SPI_INTERFACE_NO == 1)
		M8266HostIf_SPI_SetSpeed(SPI_BaudRatePrescaler_4);				// Setup SPI Clock. Here 60/4 = 15MHz for STM32F2xx SPI1 or SPI4, up to 30MHz, since SPI1/4 clock devided from faster APB2 clock
  #elif (M8266WIFI_SPI_INTERFACE_NO == 2) || (M8266WIFI_SPI_INTERFACE_NO == 3)
		M8266HostIf_SPI_SetSpeed(SPI_BaudRatePrescaler_2);				// Setup SPI Clock. Here 30/2 = 15MHz for STM32F2xx SPI2 or SPI3, up to 15MHz, since SPI2/3 clock devided from lowver APB1 clock
	#endif
 	  spi_clk =  15000000;

#elif  defined(MCU_IS_STM32F4XX)  // F4 has SPI, SPI2 & SPI3, and possible SPI4
  #if (M8266WIFI_SPI_INTERFACE_NO == 1) || (M8266WIFI_SPI_INTERFACE_NO == 4)
		M8266HostIf_SPI_SetSpeed(SPI_BaudRatePrescaler_4);				// Setup SPI Clock. Here 84/4 = 21.0MHz for STM32F4xx SPI1 or SPI4, up to 40MHz, since SPI1/4 clock devided from faster APB2 clock
  #elif (M8266WIFI_SPI_INTERFACE_NO == 2) || (M8266WIFI_SPI_INTERFACE_NO == 3)
		M8266HostIf_SPI_SetSpeed(SPI_BaudRatePrescaler_2);				// Setup SPI Clock. Here 42/2 = 21.0MHz for STM32F4xx SPI2 or SPI3, up to 21MHz, since SPI2/3 clock devided from lowver APB1 clock
	#endif
	 spi_clk = 21000000;                                                                       

#elif  defined(MCU_IS_STM32F7XX)
  #if (M8266WIFI_SPI_INTERFACE_NO == 1) || (M8266WIFI_SPI_INTERFACE_NO == 4)
    M8266HostIf_SPI_SetSpeed(SPI_BaudRatePrescaler_4);					// Setup SPI Clock. Here 108/4 = 27MHz for STM32F7xx, up to 40MHz
	#elif (M8266WIFI_SPI_INTERFACE_NO == 2) || (M8266WIFI_SPI_INTERFACE_NO == 3)
    M8266HostIf_SPI_SetSpeed(SPI_BaudRatePrescaler_2);					// Setup SPI Clock. Here 54/2  = 27MHz for STM32F7xx
	#endif
		 spi_clk = 27000000;

#elif  defined(MCU_IS_STM32L1XX)
    M8266HostIf_SPI_SetSpeed(SPI_BaudRatePrescaler_2);				// Setup SPI Clock. Here 32/2  = 16MHz for STM32L1xx
		spi_clk = 16000000;

#elif  defined(MCU_IS_STM32L4XX)
    M8266HostIf_SPI_SetSpeed(SPI_BaudRatePrescaler_4);				// Setup SPI Clock. Here 80/4  = 20MHz for STM32L4xx
		spi_clk = 20000000;
		 
#elif  defined(MCU_IS_STM32H7XX)
  #if (M8266WIFI_SPI_INTERFACE_NO == 1) || (M8266WIFI_SPI_INTERFACE_NO == 2) || (M8266WIFI_SPI_INTERFACE_NO == 3)
    M8266HostIf_SPI_SetSpeed(SPI_BAUDRATEPRESCALER_8);			// Setup SPI Clock. Here 200/8 = 25MHz for STM32F7xx, up to 40MHz
	#elif	(M8266WIFI_SPI_INTERFACE_NO == 4) || (M8266WIFI_SPI_INTERFACE_NO == 5)
    M8266HostIf_SPI_SetSpeed(SPI_BAUDRATEPRESCALER_4);			// Setup SPI Clock. Here 100/4 = 25MHz for STM32F7xx, up to 40MHz
	#endif
		 spi_clk = 25000000;

#elif  defined(MCU_IS_NXP_LPC17XX)
   M8266HostIf_SPI_SetSpeed(SPI_BaudRatePrescaler_4);					// Setup SPI Clock. Here 96/4 = 24MHz for LPC17XX, upto 40MHz
	 spi_clk = 24000000;

#elif  defined(MCU_IS_NXP_MK27_28)
   M8266HostIf_SPI_SetSpeed(SPI_BaudRatePrescaler_2);					// Setup SPI Clock. Here 37.5/2 = 18.75 MHz for for MK27/28.
	 spi_clk = 18750000;

#elif  defined(MCU_IS_NuMicro_M451)
   M8266HostIf_SPI_SetSpeed(4);																// Setup SPI Clock. Here 72/4 = 18MHz for Nuvoton M451, upto 40MHz
	 spi_clk =  18000000;

#elif  defined(MCU_IS_HT32F16XX)
   M8266HostIf_SPI_SetSpeed(4);																// Setup SPI Clock. Here 72/4 = 18MHz for HT32F165x, upto 40MHz
	 spi_clk =  18000000;
#else
#error YOU SHOULD DEFINED MCU_IS_XXX near line 17 in brd_cfg.h
#endif


   // wait clock stable
   M8266WIFI_Module_delay_ms(1);
	 
	 
   //It is very important to call M8266HostIf_SPI_Select() to tell the driver which SPI you used
 	 //and how faster the SPI clock you used. The function must be called before SPI access
	 if(M8266HostIf_SPI_Select((uint32_t)M8266WIFI_INTERFACE_SPI, spi_clk, &status)==0)
   {
		   return 0;
   }
	 
#if 1  // Used to evaluate the high-speed spi communication. Changed to #if 0 to comment it for formal release
{
//	 volatile u32  i, j;
	 u8   byte;

	 if(M8266WIFI_SPI_Interface_Communication_OK(&byte)==0) 	  									//	if SPI logical Communication failed
   {
        return 0;
	 }
#if 0   
	 i = 100000;
	 j = M8266WIFI_SPI_Interface_Communication_Stress_Test(i);
	 if( (j<i)&&(i-j>5)) 		//  if SPI Communication stress test failed
   {
		   while(1)
			 {
#ifdef USE_LED_AND_KEY_FOR_TEST	 // MB LEDs flash in 1Hz uppon errors				 
	          LED_set(0, 0); LED_set(1, 0); M8266WIFI_Module_delay_ms(500);
			      LED_set(0, 1); LED_set(1, 1); M8266WIFI_Module_delay_ms(500);
#endif
		   }
	 }
#endif
 }
#endif
 

#if 0	   // If you hope to reduce the Max Tx power, you could enable it by change to "#if 1"
	//u8 M8266WIFI_SPI_Set_Tx_Max_Power(u8 tx_max_power, u16 *status)
	if(M8266WIFI_SPI_Set_Tx_Max_Power(68, &status)==0)   // tx_max_power=68 to set the max tx power of aroud half of manufacture default, i.e. 50mW or 17dBm
	  return 0;
#endif	



#if 0  // If you expect to config or stop the on-module web server, you could enable it by change to "#if 1" and adjust as required
{
			 //u8 SPI_Set_Web_Server(u8 open_not_shutdown, u16 server_port, u16* status)
			 if(SPI_Set_Web_Server(0, 3128, &status)==0)  // 0 -> to shutdown the web_server
		   {
				  return 0;
		   }
       //u8 SPI_Set_Web_Server(u8 open_not_shutdown, u16 server_port, u16* status)
     	 if(SPI_Set_Web_Server(1, 3128, &status)==0)  // 1-> to (re-)start the web_server with port = 3128
		   {
				  return 0;
		   }
}
#endif

#if 0 // If you expect to change the op_mode overrding the default loaded from flash on bootup, enable it by "#if 1"
{
//u8 M8266WIFI_SPI_Set_Opmode(u8 op_mode, u8 saved, u16* status);
	if(M8266WIFI_SPI_Set_Opmode(2, 0, &status)==0)  // set to AP Only mode, not saved // 1=STA Only, 2=AP Only, 3=STA+AP
    return 0;
}
#endif

#if 0 // If you expect to change the ap info overrding the default loaded from flash on bootup, enable it by "#if 1"
{
// u8 M8266WIFI_SPI_Config_AP(u8 ssid[13+1], u8 password[13+1], u8 enc, u8 channel, u8 saved, u16* status);	
	if(M8266WIFI_SPI_Config_AP("Anylinkin", "1234567890", 4, 6, 0, &status)==0)  // set to 4=WPA_WPA2_PSK, not saved // 0=OPEN, 1=WEP, 2=WPA_PSK, 3=WPA2_PSK, 4=WPA_WPA2_PSK
    return 0;
}

	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get current op_mode, if STA mode or STA+AP mode, then, wait connection to external AP and getting ip

//u8 M8266WIFI_SPI_Get_Opmode(u8* op_mode, u16* status);	
	if(M8266WIFI_SPI_Get_Opmode(&sta_ap_mode, &status)==0)  
    return 0;

	
	if(  (sta_ap_mode == 1)   // if STA mode
	   ||(sta_ap_mode == 3))  // if STA+AP mode
	{
		
#if 0 // If you expect to use smartconfig to config the module, enable here by "#if 1" and prepare to send ssid using your smart devices such as smartphone 
		  // u8 M8266WIFI_SPI_Module_SmartConfig(u8 timeout_in_s, u16* status);
		     u8 smartconfig_type;
				 char smartconfig_phone_ip[15+1]={0};
         if(M8266WIFI_SPI_Module_SmartConfig(30, &smartconfig_type, smartconfig_phone_ip, &status)==0)
				 return 0;
#endif		
		
#if 1 // If you expect to use the SSID and password here overriding the default loaded from on-module SPI flash, enable it by "#if 1", and update the ssid and passowrd with those of your routers connected to.
		//u8 M8266WIFI_SPI_STA_Connect_Ap(u8 ssid[32], u8 password[64], u8 saved, u8 timeout_in_s, u16* status);
 		     if(M8266WIFI_SPI_STA_Connect_Ap("Anylinkin!", "1234567890", 0, 20, &status) == 0) // not saved, timeout=20s
				 return 0;			 			 
#endif
			 
			 // Wait the module got ip address if it works in STA mode
			 //u8 M8266WIFI_SPI_wait_sta_connecting_to_ap_and_get_ip(char* sta_ip, u8 max_wait_time_in_s)
		   if(M8266WIFI_SPI_wait_sta_connecting_to_ap_and_get_ip(sta_ip, 10)==0) // max wait 10s to get sta ip
		   {
				  return 0; 
		   }
#if 1	// If you expect to to know the reason of failure by above M8266WIFI_SPI_wait_sta_connecting_to_ap_and_get_ip(), enable below
			 else
			 {
					if(M8266WIFI_SPI_Get_STA_Connection_Status(&connection_status, &status)==0)  // connection_status will give the status of last connecting
						return 0;
			 }
#endif
			 
#if 1	 // these below two functions are just for examples on how to use them. You may or may not use them during module initialisation		 
		 //u8 M8266WIFI_SPI_STA_Query_Current_SSID_And_RSSI(char* ssid, u8* rssi, u16* status)
			 if(M8266WIFI_SPI_STA_Query_Current_SSID_And_RSSI(ssid, &rssi, &status)==0)
				 return 0;
#endif
			 
	} // end to if(  (sta_ap_mode == 1)
	#endif
  return 1;
 }

/*************************************************************************************
 * M8266WIFI_Config_Connection_Via_SPI                                               *
 * Description                                                                       *
 *    To reboot the module, wait wifi connection, and setup udp/tcp connection       *
 *    whether the response is what is expected via SPI                               *
 * Parameter(s):                                                                     *
 *    1. tcp_udp     : the type of connection to set up                              *
 *                     = 0, to set up connection of UDP                              *
 *                     = 1, to set up connection as TCP Client                       *
 *                     = 2, to set up connection as TCP Server                       *
 *    2. local_port  : to specify the local port if any                              *
 *                     if local_port=0, then a random local port will be generated   *
 *    3. remote_addr : the string of ip or dns address of remote                     *
 *                     e.g. "192.168.1.2" or "www.baidu.com"                         *
 *    4. remote_port : the port value of remote server                               *
 *    5. link_no     : the link_number to be used. max 8 links as per mutitiple link *
 * Return:                                                                           *
 *       0 = failed                                                                  *
 *       1 = success                                                                 * 
 *************************************************************************************/
u8 M8266WIFI_Config_Connection_via_SPI(u8 tcp_udp, u16 local_port, char* remote_addr, u16 remote_port, u8 link_no)
{
	u16 status=0;
  //  u8 M8266WIFI_SPI_Setup_Connection(u8 tcp_udp, u16 local_port, char remote_addr, u16 remote_port, u8 link_no, u8 timeout_in_s, u16* status);
	return M8266WIFI_SPI_Setup_Connection(tcp_udp, local_port, remote_addr, remote_port, link_no, 20, &status);
}

/*************************************************************************************
 * M8266WIFI_Sleep_module_for_some_time_and_then_wakeup_automatically                *
 * Description                                                                       *
 *    - To (deep) sleep the module for some time via SPI                             *
 *    - All connections will get lost when entering sleep and after wakeup           *
 *      And should be re-established by the host                                     *
 *      e.g.                                                                         *
 *          	if(M8266WIFI_Sleep_M8266WIFI(5000)==0) // wakeup in 5000ms             *
 *		              return fail; // do somework here for failure                     *
 *            M8266WIFI_Module_Init_Via_SPI();       // re-initial the module        *
 *         or M8266WIFI_Config_Connection_via_SPI(); // re-establish the server      *
 * Parameter(s):                                                                     *
  *     1. time_to_wakeup_in_ms : time to wakeup from sleep, unit in ms              *
 *        - max value 4294967 ms (about 1.19hour).                                   *
 *          if a value exceed this provided, then 4294967ms will be used             *
 *        - use 0 if expect to sleep for ever without automatic wakeup               *
* Return:                                                                            *
 *       0 = failed                                                                  *
 *       1 = success                                                                 * 
 *************************************************************************************/
u8 M8266WIFI_Sleep_module_for_some_time_and_then_wakeup_automatically(u16 time_to_wakeup_in_ms)
{
	u16 status;
  //u8 M8266WIFI_SPI_Sleep_Module(u8 sleep_type, u32 time_to_wakeup_in_ms, u16 *status)
	if(M8266WIFI_SPI_Sleep_Module(3,time_to_wakeup_in_ms, &status)==0) //sleep_type=3=deep_sleep
		 return 0;

	if(time_to_wakeup_in_ms>0) // not a forever sleep
  {
		 M8266HostIf_Set_SPI_nCS_Pin(0);   // set nCS low to enaure the module will reboot from flash after wakeup
	   M8266WIFI_Module_delay_ms(time_to_wakeup_in_ms+1000);  // additional 1000ms for bootup time
	   M8266HostIf_Set_SPI_nCS_Pin(1);
	}
	
	return 1;
}

/*************************************************************************************
 * M8266WIFI_Sleep_Module                                                            *
 * Description                                                                       *
 *    To (deep) sleep the module forever via SPI and should be wakeup by host        *
 *    After calling of this function, all connection will get lost                   *
 * Parameter(s):                                                                     *
 *    None                                                                           *
* Return:                                                                            *
 *       0 = failed                                                                  *
 *       1 = success                                                                 * 
 *************************************************************************************/
u8 M8266WIFI_Sleep_Module(void)
{
	u16 status;
  //u8 M8266WIFI_SPI_Sleep_Module(u8 sleep_type, u32 time_to_wakeup_in_ms, u16 *status)
	if(M8266WIFI_SPI_Sleep_Module(3, 0, &status)==0) //sleep_type=3=deep_sleep
		 return 0;
	
	return 1;
}
/*************************************************************************************
 * M8266WIFI_Wakeup_Module                                                           *
 * Description                                                                       *
 *    To wakeup the module                                                           *
 *    - All connections will get lost when entering sleep and after wakeup           *
 *      And should be re-established by the host after wakeup                        *
 *      e.g.                                                                         *
 *          	if(M8266WIFI_Wakeup_Module()==0)                                       *
 *		        {      ; // do somework here for failure                               *
 *            }                                                                      *
 *            else                                                                   *
 *            {                                                                      *
 *               M8266WIFI_Config_Connection_via_SPI(); // re-establish the server   *
 *            }                                                                      *
 *                                                                                   *
 * Parameter(s):                                                                     *
 *    None                                                                           *
* Return:                                                                            *
 *       0 = failed                                                                  *
 *       1 = success                                                                 * 
 *************************************************************************************/
u8 M8266WIFI_Wakeup_Module(void)
{
	 return M8266WIFI_Module_Init_Via_SPI();
}
