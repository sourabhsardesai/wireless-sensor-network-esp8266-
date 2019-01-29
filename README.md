Abstract
The idea behind the project is to create a wireless sensor/actuator network which gathers the sensor data at a particular time interval and uploads it on the web application giving an access to user from anywhere around the world. The projects demonstrates end to end implementation of iot application from collecting data from transducer to converting it into mqtt protocol and sending it through internet to a cloud application which is hosted on AWS instance and from there pushing it to web application through mqtt client. The project further demonstrate 2 way communication from device to web application and vice versa.


Goal
The major goal of the project is to allow temperature reading and light intensity of the environment to monitor over internet on web application and allows the web application to control the actuator which further turns on/off the fan to maintain the temperature.

Design Choices
Hardware –


Module 1- NodeMcu with Light dependent Resistor
This modules contains following hardware –
1] NodeMcu (esp826612E)
2] Light dependent Resistor ( LDR )
3] Resistor

Node MCU – This is a Wifi System on chip integrated with a Tensilica Xtensa LX106 core it supports Arduino ide for programming which is one of the main reason for using this board for prototyping , Along nodemcu has controller and wifi chip together on board reduces the complexity of interfacing a different controller and wifi chip together. Also it has inbuilt Analog to digital converter which was required for this project to interface an analog sensor.
LDR – It is variable resistor which is used in the voltage divider arrangement, The resistance of the LDR changes with respect to the light. The resistance of the LDR is maximum in the absence of the light and minimum at the presence of the light so based on that the voltage across the resistor connected in voltage divider changes and that voltage is sensed by the nodemcu.

Module 2 –
1] NodeMcu
2] Lm35
3] Capacitor
4] Relay Module
5] DC Fan
Node MCU – This is same wifi SOc chip which is used in the module 1 it has same function here although it is connected to temperature sensor lm 35 in order to sense voltage change with respect to temperature
Lm35 – This is an active transducer which operates from -3 v to 35v it has 3 pins which gives a voltage change with respect to temperature. The typical lm 35 sensor gives output variations of 10mv per degree. A Capacitor is connected across the output pin so as to filter out any noise coming from the sensor.
Relay Module - Relay module is used to actuate the fan, The reason behind using relay module is fan operates on 5v or above and our esp8266 module operates on 3.3v and the sourcing current of the esp gpio is not sufficient to power up the fan, So we have used relay module , The Relay module comes with a transistor as a switch to trigger the relay and relay further makes contact for fan to power up.


Software –

At Arduino –

The Arduino is a platform which allows rapid prototyping and the major reason behind it is that it has vast collection of software libraries, for this project I have used 2 software libraries as follows –
ESP8266wifi – This library is used to enable wifi client on Node mcu, In Short it allows esp module to operate as a wifi client.
PubSubClient – This library allows programmer to send and receive the data through tcp in MQTT protocol, MQTT protocol is used in order to send real time data. Mqtt is a light weight and fast protocol for this purpose we have used PubSubClient library which allows to communicate with the server.

At Backend side –

I have used Amazon EC2 instance for installing my server and hosting my web application. The data coming from the device is received at the MQTT broker as we know that mqtt is a light weight protocol it has to be handled by the broker for this I have used Mosquitto broker which is open source broker this broker is a application which operates on the port number 1883 allows the devices and client to connect it through mqtt client and enables communication. My server here acts a mqtt client which is tuned to the broker and messages coming from it are processed and either stored in the Database or routed to the web client. The Web client is designed in Angular js and it also has a MQTT client which allows showing a real time data on web page.

