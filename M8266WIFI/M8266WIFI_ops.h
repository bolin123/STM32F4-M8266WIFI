/********************************************************************
 * M8266WIFI_ops.h
 * .Description
 *     header file of M8266WIFI HAL operations
 * .Copyright(c) Anylinkin Technology 2015.5-
 *     IoT@anylinkin.com
 *     http://www.anylinkin.com
 *     http://anylinkin.taobao.com
 *  Author
 *     wzuo
 *  Date
 *  Version
 ********************************************************************/
 
#ifndef _M8266WIFI_OPS_H_ 
#define _M8266WIFI_OPS_H_

void M8266WIFI_Module_delay_ms(u16 nms);
void M8266WIFI_Module_Hardware_Reset(void);
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
/////// BELOW FUNCTIONS ARE QUERYING OR CONFIGURATION VIA UART
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
u8 M8266WIFI_wait_response_ok(u8* ack, u16 wait_time_in_10ms);
u8 M8266WIFI_running_ok(void);
u8 M8266WIFI_Reboot_And_Config_Connection(u8 udp_tcp, u16 local_port, u8* remote_ip, u16 remote_port, u8 link_no);
u8 M8266WIFI_get_sta_ap_mode(void);
u8 M8266WIFI_SPI_wait_sta_connecting_to_ap_and_get_ip(char* sta_ip, u8 max_wait_time_in_s);
u8 M8266WIFI_Module_Init_Via_USART(void);
u8 M8266WIFI_Config_Connection_via_USART(u8 tcp_udp, u16 local_port, u8* remote_ip, u16 remote_port, u8 link_no);
u8 M8266WIFI_SmartConfig(void);
u8 M8266WIFI_WpsConfig(void);
u8 M8266WIFI_set_led(u8 led_no, u8 on);
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
/////// BELOW FUNCTIONS ARE QUERYING OR CONFIGURATION VIA SPI
/////// MORE TO BE FOUND IN M8266WIFIDrv.h
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
u8 M8266WIFI_Module_Init_Via_SPI(void);
u8 M8266WIFI_Config_Connection_via_SPI(u8 tcp_udp, u16 local_port, char* remote_addr, u16 remote_port, u8 link_no);

u8 M8266WIFI_Sleep_module_for_some_time_and_then_wakeup_automatically(u16 time_to_wakeup_in_ms);
u8 M8266WIFI_Sleep_Module(void);
u8 M8266WIFI_Wakeup_Module(void);

#endif
