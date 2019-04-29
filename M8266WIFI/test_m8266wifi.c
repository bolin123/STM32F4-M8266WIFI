/********************************************************************
 * test_m8266wifi.c
 * .Description
 *     Source file of M8266WIFI testing application 
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
#include "brd_cfg.h"
#include "M8266WIFIDrv.h"
#include "M8266HostIf.h"
#include "M8266WIFI_ops.h"
#include "led.h"

void M8266WIFI_Test(void)
{
	 u16 i;
	 u16 status = 0;
	 u8  link_no=0;

#ifdef USE_LED_AND_KEY_FOR_TEST	
	 for(i=0; i<3; i++)  // Flash the Two LEDs 4 times in the Main Board to indicate we start test
	 {
         LED_set(0, 0); LED_set(1, 1);  M8266WIFI_Module_delay_ms(100);
		     LED_set(0, 1); LED_set(1, 0);  M8266WIFI_Module_delay_ms(100);
		     LED_set(0, 0); LED_set(1, 1);  M8266WIFI_Module_delay_ms(100);
		     LED_set(0, 1); LED_set(1, 0);  M8266WIFI_Module_delay_ms(100);
	 }
	 LED_set(0, 1); LED_set(1, 1);
#endif	 


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	 SETUP  SOCKET CONNECTIONS 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	 
	 
   // Test Type
	 #define TEST_M8266WIFI_TYPE    1	     // 1 = Repeative Sending, 2 = Repeative Reception, 3 = Echo
	 
	 // Set TCP/UDP Connections  
	 #define TEST_CONNECTION_TYPE   2	    // 0=WIFI module as UDP, 1=WIFI module as TCP Client, 2=WIFI module as TCP Server

#if (TEST_CONNECTION_TYPE==1)  // TCP Client
	 #define TEST_LOCAL_PORT  			0			// local port=0 then a random port will be generated each time. To avoid the rejection by some TCP server due to repeative same ip:port
#else	 
   #define TEST_LOCAL_PORT  			4321
#endif

#if(TEST_CONNECTION_TYPE==2)  // TCP Server DO NOT need remote ip and port. below data has no usage, any value could be used for initialization. Will automatically changed to remote ip and port when connected by remote
   #define TEST_REMOTE_ADDR       "1.1.1.1"
   #define TEST_REMOTE_PORT  	    1234
#else
   #define TEST_REMOTE_ADDR    		"192.168.4.1"   // "www.baidu.com"
   #define TEST_REMOTE_PORT  	    4321						// 80
#endif			 


  //  u8 M8266WIFI_SPI_Setup_Connection(u8 tcp_udp, u16 local_port, char remote_addr, u16 remote_port, u8 link_no, u8 timeout_in_s, u16* status);
	if(M8266WIFI_SPI_Setup_Connection(TEST_CONNECTION_TYPE, TEST_LOCAL_PORT, TEST_REMOTE_ADDR, TEST_REMOTE_PORT, link_no, 20, &status)==0)
	{		
		 while(1)
		 {
#ifdef USE_LED_AND_KEY_FOR_TEST	 // led flash in 1Hz uppon errors
	      LED_set(0, 0); LED_set(1, 0); M8266WIFI_Module_delay_ms(500);
	      LED_set(0, 1); LED_set(1, 1); M8266WIFI_Module_delay_ms(500);
#endif
		 }
	}
	else
	{
#if (TEST_CONNECTION_TYPE == 0)  		// If config the wifi module as UDP, then the module could join a multicust group
#if 0	//changed to "#if 1" if you hope to test multicust with UDP
			//u8 M8266WIFI_SPI_Set_Multicuast_Group(u8 join_not_leave, char multicust_group_ip[15+1], u16* status)
		 if(M8266WIFI_SPI_Op_Multicuast_Group(0, "224.6.6.6", &status)==0)
     {
		   while(1)
		   {
#ifdef USE_LED_AND_KEY_FOR_TEST   // led flash in 1Hz uppon errors
				  LED_set(0, 0); LED_set(1, 0); M8266WIFI_Module_delay_ms(1000);
	        LED_set(0, 1); LED_set(1, 1); M8266WIFI_Module_delay_ms(1000);
#endif				 
		   }
     }
     else
#endif	//end of 	test multicust with UDP
#elif (TEST_CONNECTION_TYPE == 2)  // If config the wifi module as TCP server, then tcp server auto disconnection timeout could be set		
		//u8 M8266WIFI_SPI_Set_TcpServer_Auto_Discon_Timeout(u8 link_no, u16 timeout_in_s, u16* status)	
		 if( M8266WIFI_SPI_Set_TcpServer_Auto_Discon_Timeout(link_no, 25, &status) == 0)
     {
#ifdef USE_LED_AND_KEY_FOR_TEST			 
		   // led flash in 1Hz when error
		   while(1)
		   {
	        LED_set(0, 0); LED_set(1, 0); M8266WIFI_Module_delay_ms(1000);
	        LED_set(0, 1); LED_set(1, 1); M8266WIFI_Module_delay_ms(1000);
		   }
#endif		 
     }
     else
#endif	// end of #if (TEST_CONNECTION_TYPE == 0) or #elif (TEST_CONNECTION_TYPE == 2)		 
     {
#ifdef USE_LED_AND_KEY_FOR_TEST			 
		   // led 1 flash 4 times upon success 
		   LED_set(1, 0); M8266WIFI_Module_delay_ms(50); LED_set(1, 1); M8266WIFI_Module_delay_ms(50);
		   LED_set(1, 0); M8266WIFI_Module_delay_ms(50); LED_set(1, 1); M8266WIFI_Module_delay_ms(50);
		   LED_set(1, 0); M8266WIFI_Module_delay_ms(50); LED_set(1, 1); M8266WIFI_Module_delay_ms(50);
		   LED_set(1, 0); M8266WIFI_Module_delay_ms(50); LED_set(1, 1); M8266WIFI_Module_delay_ms(50);
#endif			 
		 }
	}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#if (TEST_M8266WIFI_TYPE==1) // Transmission Test: to test send data to remote constantly
{
#define TEST_SEND_DATA_SIZE   1024 //2048 // 1472 //1460

   u8  snd_data[TEST_SEND_DATA_SIZE];
	 u16 sent = 0;
   u32 total_sent = 0, MBytes = 0; 
	 u16 batch;

 	 for(i=0; i<TEST_SEND_DATA_SIZE; i++) snd_data[i]=i; 
	 
   link_no=0;
	
 	 for(batch=0; ; batch++)
	 {
		  snd_data[0]= batch>>8;    							// first two bytes using bacth to differentiate the continous packets 
			snd_data[1]= batch&0xFF;  
		  snd_data[TEST_SEND_DATA_SIZE-2]=0xFF;		// last two bytes using customised to lable end of a packet for the sake of searching for data verification. Here FF FF as an example

#if 1
		  if(total_sent> 1024*1024)
			{
				 MBytes++;
				 total_sent -= 1024*1024;
			}
#else		 
		  if( (batch%1024)==0)
				LED_set(0, (batch/1024)&0x01);	
#endif	

#if 0  // if use simple but less-efficiency transmission, changed to "#if 1"
			//u32 M8266WIFI_SPI_Send_Data_Block(u8 Data[], u32 Data_len, u16 tcp_packet_size, u8 link_no, u16* status); 
		  sent = M8266WIFI_SPI_Send_Data_Block(snd_data, TEST_SEND_DATA_SIZE, 1460, link_no, &status);
		  total_sent += sent;
			  
			if( (sent!=TEST_SEND_DATA_SIZE) || ( (status&0xFF)!= 0 ) ) 
			{
				if(  ((status&0xFF)==0x14)      // 0x14 = connection of link_no not present
				   ||((status&0xFF)==0x15) )    // 0x15 = connection of link_no closed
				{
					// do some work here, including re-establish the closed connect or link here and re-send the packet if any
					// additional work here
					  M8266HostIf_delay_us(249);	
				}
				else if	( (status&0xFF)==0x18)       // 0x18 = TCP server in listening states and no tcp clients connecting to so far
				{
					  M8266HostIf_delay_us(250);	
				}
				else if	( (status&0xFF)==0x1E)       // 0x1E: too many errors ecountered during sending can not fixed
				{
					  M8266HostIf_delay_us(251);	
				}
				else                                 // 0x10, 0x11 here may due to spi failure during sending, and normally packet has been sent partially, i.e. sent!=0
				{
						M8266HostIf_delay_us(200);
					// do some work here					  
					  if(sent<TEST_SEND_DATA_SIZE)
							 total_sent += M8266WIFI_SPI_Send_Data(snd_data+sent, TEST_SEND_DATA_SIZE-sent, link_no, &status);  // try to resend the left packet
				}
			}

#elif 0 // complex but high-efficiency transsmission test
			//u16 M8266WIFI_SPI_Send_Data(u8 data[], u16 data_len, u8 link_no, u16* status)
      sent = M8266WIFI_SPI_Send_Data(snd_data, TEST_SEND_DATA_SIZE, link_no, &status);
			total_sent += sent;
			
			//M8266WIFI_Module_delay_ms(10);
			
			if( (sent!=TEST_SEND_DATA_SIZE) || ( (status&0xFF)!= 0 ) ) 
			{
				if( (status&0xFF)==0x12 )  				   // 0x12 = Module send buffer full, and normally sent return with 0, i.e. this packet has not been sent anyway
				{                              
			      M8266HostIf_delay_us(250);       // if 500Kbytes/s, need 2ms for 1Kbytes packet to be sent. If use 250us here, may repeat query 8- times, but in a small interval
					  //M8266WIFI_Module_delay_ms(1);	 // if 500Kbytes/s, need 2ms for 1Kbytes packet to be sent. If use 1ms here,   may repeat query 2- times, but in a large interval
					  if(sent<TEST_SEND_DATA_SIZE)
					     total_sent += M8266WIFI_SPI_Send_Data(snd_data+sent, TEST_SEND_DATA_SIZE-sent, link_no, &status);  // try to resend it					
				}
				else if(  ((status&0xFF)==0x14)      // 0x14 = connection of link_no not present
					      ||((status&0xFF)==0x15) )    // 0x15 = connection of link_no closed
				{
					// do some work here, including re-establish the closed connect or link here and re-send the packet if any
					// additional work here
					  M8266HostIf_delay_us(249);	
				}
				else if	( (status&0xFF)==0x18)       // 0x18 = TCP server in listening states and no tcp clients connecting to so far
				{
					  M8266HostIf_delay_us(251);	
				}				
				else                                 // 0x10, 0x11 here may due to spi failure during sending, and normally packet has been sent partially, i.e. sent!=0
				{
						M8266HostIf_delay_us(200);
					// do some work here					  
					  if(sent<TEST_SEND_DATA_SIZE)
							 total_sent += M8266WIFI_SPI_Send_Data(snd_data+sent, TEST_SEND_DATA_SIZE-sent, link_no, &status);  // try to resend the left packet
				}
				
			}
#elif 0 // transsmission test, avoid skip
			{
			   #define M8266WIFI_SPI_SEND_DATA_BLOCK_MAX_LOOPS   1000
         #define tcp_packet_size 1024
         u16 loops     = 0;
			   u32 len       = TEST_SEND_DATA_SIZE;

         for(sent=0, loops=0; (sent<len)&&(loops<=M8266WIFI_SPI_SEND_DATA_BLOCK_MAX_LOOPS); loops++)
         {		
    	      sent += M8266WIFI_SPI_Send_Data_Block(snd_data+sent, len, tcp_packet_size, link_no, &status);
            if(sent>=len)  break;
			      if((status&0xFF) == 0x00)
			      {
				 loops = 0;
			      }
		        else
			      {
				      if(   ((status&0xFF) == 0x14)      // 0x14 = connection of link_no not present
                 || ((status&0xFF) == 0x15)    // 0x15 = connection of link_no closed
                 || ((status&0xFF) == 0x18) )  // 0x18 = TCP server in listening states and no tcp clients connecting to so far
	            {
                 // add some failure process here
	            }
	            else
	            {
	               M8266HostIf_delay_us(100);
	            }
			      }
          } // end of for(...
			
			    total_sent += sent;
			} 
#elif 1 // efficient transsmission test, avoid skip
      {			
			   #define M8266WIFI_SPI_SEND_DATA_BLOCK_MAX_LOOPS   1000
         #define tcp_packet_size 1024
         u16 loops     = 0;
			   u32 len       = TEST_SEND_DATA_SIZE; 

         for(sent=0, loops=0; (sent<len)&&(loops<=M8266WIFI_SPI_SEND_DATA_BLOCK_MAX_LOOPS); loops++)
         {		
				    sent += M8266WIFI_SPI_Send_Data(snd_data+sent, ((len-sent)>tcp_packet_size)?tcp_packet_size:(len-sent), link_no, &status);
            if(sent>=len)  break;
			      if((status&0xFF) == 0x00)
			      {
							 loops = 0;
			      }
		        else
			      {
				      if(   ((status&0xFF) == 0x14)      // 0x14 = connection of link_no not present
                 || ((status&0xFF) == 0x15)    // 0x15 = connection of link_no closed
                 || ((status&0xFF) == 0x18) )  // 0x18 = TCP server in listening states and no tcp clients connecting to so far
	            {
                 // add some failure process here
	            }
	            else
	            {
	               M8266HostIf_delay_us(100);
	            }
			     }
         } // end of for(...
			
			   total_sent += sent;
		  }
#endif			
			
	 } // end of for(batch=0; ; batch++)
 }
	
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#elif (TEST_M8266WIFI_TYPE==2)  // to test constant reception from remote
{
#define  RECV_DATA_MAX_SIZE  2048

   u8  RecvData[RECV_DATA_MAX_SIZE];
   u16 received = 0;
	 u32 total_received = 0;
   u32 MBytes = 0;
	
	 for(i=0; i<RECV_DATA_MAX_SIZE; i++) RecvData[i]=0xFF-i; 	
   M8266WIFI_SPI_Send_Data(RecvData, 1024, link_no, &status);  // just send to check the ip address on TCP tester. 
   	
	 while(1) // TCP工具连续循环发TCP包时，可能会出现粘包拆包现象。例如，两个1024字节的包，可能会被重新打包成1460+588两个包发送
	 {
			if(M8266WIFI_SPI_Has_DataReceived())
			{
				//u16 M8266WIFI_SPI_RecvData(u8 data[], u16 max_len, uint16_t max_wait_in_ms, u8* link_no, u16* status);
	      received = M8266WIFI_SPI_RecvData(RecvData, RECV_DATA_MAX_SIZE, 5*1000, &link_no, &status);

			  if(  (status&0xFF)!= 0 )  
			  {
				    if( (status&0xFF)==0x22 )      // 0x22 = Module buffer has no data received
				    {  
			         M8266HostIf_delay_us(250); 
					   //M8266WIFI_Module_delay_ms(1);
				    }
				    else if( (status&0xFF)==0x23 )   
				    { // the packet had not been received completed in the last call of M8266WIFI_SPI_RecvData()
							// and has continued to be received in this call of M8266WIFI_SPI_RecvData()
							// do some work here if necessary
				    }
				    else if(  (status&0xFF)==0x24)   
				    { 
	            // the packet is large in size than max_len specified and received only the max_len // TCP上的粘包和拆包现象
							// normally caused by the burst transmision by the routers after some block. 
							// Suggest to stop the TCP transmission for some time
					    // do some work here if necessary
				    }
						else
						{
					    // do some work here if necessary
						}
			  }
				
#if 0 // commented it if to avoid delay by printf
  			 if((status&0xFF)!=0)
				 {					 
					 printf("\r\n Received data from from M8266 with an error, status = 0x%04X\r\n", status);			 
				 }
				 else
				 {					 
					 printf("\r\n Received data from from M8266 with no error\r\n");			 
				 }
		     printf(" Received Data[%d] from M8266 = ", received);
         for(i=0; i<received; i++)
				 {
					  if(i%8==0) printf("\r\n  %03d :", i);
		        printf(" %02X", RecvData[i]);
				 }
		     printf("\r\n");
#endif
#define  TEST_RECV_UNIT (1024*1024)
				 total_received += received;
				 if( total_received >= (TEST_RECV_UNIT) )
				 {
					   LED_set(0, MBytes&0x01);	
					   total_received = total_received%(TEST_RECV_UNIT);
				     MBytes++;
         }
			} // end of if(M8266WIFI_SPI_Has_DataReceived())				 
	 } // end of while(1)
 }	 

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#elif (TEST_M8266WIFI_TYPE==3)  // Echo test: to receive data from remote and then echo back to remote
{
#define  RECV_DATA_MAX_SIZE  2048    

   u8  RecvData[RECV_DATA_MAX_SIZE];   // make sure the stack size is more than RECV_DATA_MAX_SIZE
   u16 received = 0;
	 u16 sent;
	
	 for(i=0; i<RECV_DATA_MAX_SIZE; i++) RecvData[i]=i; 

   link_no = 0;
   sent = M8266WIFI_SPI_Send_Data(RecvData, 1024, link_no, &status);

   while(1)
	 {
			if(M8266WIFI_SPI_Has_DataReceived())
			{
				//u16 M8266WIFI_SPI_RecvData(u8 data[], u16 max_len, uint16_t max_wait_in_ms, u8* link_no, u16* status);
	       received = M8266WIFI_SPI_RecvData(RecvData, RECV_DATA_MAX_SIZE, 5*1000, &link_no, &status);
				
#if 0 // commented it if to avoid delay by printf
  			 if((status&0xFF)!=0)
				 {					 
					 printf("\r\n Received data from from M8266 with an error, status = 0x%04X\r\n", status);			 
				 }
				 else
				 {					 
					 printf("\r\n Received data from from M8266 with no error\r\n");			 
				 }
		     printf(" Received Data[%d] from M8266 = ", received);
         for(i=0; i<received; i++)
				 {
					  if(i%8==0) printf("\r\n  %03d :", i);
		        printf(" %02X", data[i]);
				 }
		     printf("\r\n");
#endif

				// Step 2: echo the data
				if(received!=0)
				{
					//u16 M8266WIFI_SPI_Send_Data(u8 data[], u16 data_len, u8 link_no, u16* status);
          sent = M8266WIFI_SPI_Send_Data(RecvData, received, link_no, &status);

					if( (sent!=received) || ( (status&0xFF)!= 0 ) ) 
					{
						if(  ((status&0xFF)==0x14)      // 0x14 = connection of link_no not present
					      ||((status&0xFF)==0x15) )    // 0x15 = connection of link_no closed
						{
							// do some work here, including re-establish the closed connect or link here and re-send the packet if any
							// additional work here
							M8266HostIf_delay_us(249);	
						}
						else if	( (status&0xFF)==0x18)       // 0x18 = TCP server in listening states and no tcp clients connecting to so far
						{
							M8266HostIf_delay_us(251);	
						}				
						else 
						{
							if( (status&0xFF)==0x12 )  				   // 0x12 = Module send buffer full, and normally sent return with 0, i.e. this packet has not been sent anyway
							{                              
								M8266HostIf_delay_us(250);       // if 500Kbytes/s, need 2ms for 1Kbytes packet to be sent. If use 250us here, may repeat query 8- times, but in a small interval
							}
							while(sent<received)
							{
								sent += M8266WIFI_SPI_Send_Data(RecvData+sent, received-sent, link_no, &status);  // try to resend it					
								M8266HostIf_delay_us(10);
							}
						}
					}
#if 0	// to avoid print bring about delay	during test
					if( (sent!=received) || ( (status&0xFF)!= 0 ) )
					{
						printf("\r\n Send Data[len=%d] to M8266 failed. Only %d bytes has been sent(0x%04X).\r\n", received, sent, status);
						break;
					}
					else
					{
						printf("\r\n Send Data[len=%d] to M8266 successfully(%04X).\r\n", received, status);
					}
#endif					
			  }
			} // end of if(M8266WIFI_SPI_Has_DataReceived())
		} // end of while(1)
}	 
 
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#else
#error NOT Supported Test Type! should be 1~3!		
#endif

} // end of M8266WIFI_Test
