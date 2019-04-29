¡¾Files decriptions¡¿

----------------------------HARDWARE INTERFACE------------------------------------------
- M8266WIFIHostIf.c -- example source of MCU inteface to M8266WIFI Module              -
- M8266WIFIHostIf.h -- example header of MCU inteface to M8266WIFI Module              -
Notes:                                                                                 -
  1. Hardware Layer fundamental control/query operations including GPIO/UART/SPI       -
     Operations                                                                        -
  2. Should be adjust according to the specific hardware logic                         -
----------------------------------------------------------------------------------------

----------------------------DRIVER------------------------------------------------------
- M8266WIFIDRV.lib  -- Driver.lib of MCU to perform SPI-WIFI transparent transception  -
- M8266WIFIHostIf.h -- example header of MCU inteface to M8266WIFI Module              -
Notes:                                                                                 -
  1. Driver Layer operations mainly for High-Speed SPI-WIFI Operations,                -
     including High-Speed SPI-WIFI transparent transception, timing, hands-shaking etc -
  2. PLEASE DO NOT MODIFY THEM. Just embed them into your project work bentch          -
----------------------------------------------------------------------------------------


----------------------------HAL OPERATIONS----------------------------------------------
- M8266WIFI_ops.c   -- example source of MCU HAL operations to M8266WIFI module        -
- M8266WIFI_ops.h   -- example header of MCU HAL operations to M8266WIFI module        -
Notes:                                                                                 -
  1. Hardare Abtraction Layer operations for MCU to config and communication with      -
     M8266WIFI module                                                                  -
  2. You may please extend the operations following these example functions            -
----------------------------------------------------------------------------------------

----------------------------APPLICATIONS__----------------------------------------------
- test_m8266wifi.c  -- example application source of test M8266WIFI module             -
Notes:                                                                                 -
  1. example testing application to test speed of MCU-WIFI transparent transception    -
  2. You may please modify or reference it for your own application
----------------------------------------------------------------------------------------
 
¡¾Architecture¡¿

  ------------
 |APPLICATIONS|
  ------------
        \
         \
          \
       ----------------
      | HAL OPERATIONS |
       ----------------
             \
              \
               \
           ---------
          | DRIVER  |
           ---------
                 \
                  \
                   \
            --------------------
           | HARDWARE INTERFACE |
            --------------------

¡¾Usage¡¿
       
 1. Implement the sources of hardware interface according to your hardware design

    Maily including 
     - GPIO initialization and operations for nCS and nRESER
     - SPI  initialization and read/write operations
     - UART initialization and read/write operations
    You are recommended to uses the manner of macros following brd_cfg.h

 2. Add Driver Lib into your project

 3. Copy sources of HAL OPERATION to your project

    If necessary, you may modify or extend your own HAL operations according to 
    AT commands and other related protocols 

 4. Compose your applications referencing to APPLICATION SOURCE





AnyLinkin!
Link anytime, anywhere, and anyhow!
IoT@anylinkin.com
http://www.anylinkin.com
htpp://anylinkin.taobao.com
              