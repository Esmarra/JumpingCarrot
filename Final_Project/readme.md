# Inteligent Sensors Final Project
## Ambient Monitoring Coupled with a Mobile Robot

### Getting Started
* [Tutorial Links](https://docs.google.com/document/d/1Y7-AwB2l0Ygj-erC61ZWclCtR6IDCLgGK_R8bSqs204/edit?usp=sharing) - RPI & ESP Tutorials
* [Github commands](https://confluence.atlassian.com/bitbucketserver/basic-git-commands-776639767.html) - Basic comands
* [Report](https://docs.google.com/document/d/10Hp0AI9e4CDaYOMIwZgMZLruwxW1hJ9rVRKjJwoy5Ew/edit?usp=sharing) - Google Docs Report

### Project Dev Boards
* [RaspberryPi](https://www.raspberrypi.org/) - Server Board
* [ESP8266](https://www.banggood.com/Geekcreit-Doit-NodeMcu-Lua-ESP8266-ESP-12F-WIFI-Development-Board-p-985891.html?p=FX25081395033201503Y) - IOT main board
	- [Datasheet](https://github.com/nodemcu/nodemcu-devkit-v1.0/blob/master/NODEMCU_DEVKIT_V1.0.PDF) - ESP Datasheet
* [PIC24](TBD) - Read Photorestior Sensor

### Project Sensor Modules
* [HTU21D](https://www.banggood.com/GY-213V-HTU21D-3_3V-I2C-Temperature-Humidity-Sensor-Module-For-Arduino-p-1184748.html?p=FX25081395033201503Y) - Humidity Sensor
	- [HTU21D  Datasheet](http://www.farnell.com/datasheets/1780639.pdf) - Datasheet
* [LDR](http://www.resistorguide.com/photoresistor/) - Light dependent resistor
<p align="center">
	<img src="/Final_Project/images/si_draft1.png">
</p>

### How it Works
* PIC24 - Reads Data from 4 LDR's(using ADC) and sends it to ESP8266 via Uart0
* ESP8266 - Reads data from HTU21D sensor (Humidity/Temperature) and receives LDR's Light Values via Uart0
* ESP8266 - Sends all Sensor data to MQQT Broker running at Raspbery pi
* Raspbery pi - Is running MQQT server and NodeRed, this then creates a webpage where you can see all Sensor Data
