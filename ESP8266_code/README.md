
# Receiving data on ESP8266

This script receives data via UART.  

It reads array of 9 elements (uint8_t). It converts the whole package to concrete Values.  
Next step is setting up a webserver on ip 192.168.4.1 hosted by Nodemcu. I can be reached by connecting to WiFi Acces Point created.  
The webpage is simple HTML with elements of JavaScript. It refreshes data of Accelerometer and sound.

.h file is header file with included HTML and JavaScript.