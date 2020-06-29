# WiFiRC-Car
WiFiRC-Car is an Arduino remote controlled car over a WiFi connection to a server.
It is still a work in progress, so if you have any thoughts on how to improve or if you find bugs, please contact me at kaspervanm@gmail.com.

## How it works
The car gets its commands from a webserver by continuously reading the contents to check if something has changed. I tried to make the webserver as seperate as possible from the car itself. This way dependancy issues can be easily solved. Because the car communicates to the server over the internet, it needs to be connected to WiFi. The car is able to store up to eight service set identifiers (SSID's) and their corresponding passwords. Each with a max of 32 characters. The passwords are encrypted with a key before being stored on the ESP8266's EEPROM. This way, no passwords can be recovered, even if the EEPROM gets compromised. In order to instantiate a connection between the arduino and the server, you must enter the SSID and password of the WiFi available in the area where the car will be driving. This can be done by a captive portal that will remain active until it connects to Wifi and reaches the server. 

## Future updates
There will be a webcom stream to the server. Because it would be unrealistic to expect the ESP8266 to handle everything, I planned to connect a Raspberry Pi to the Arduino over serial. I am not yet certain of how to securely transfer the password corresponding to the SSID over a serial connection and have the Raspberry Pi remain connected to the WiFi without the possibility of acquiring the credentials even when the micro-sd chip is taken out. I you have any suggestions on this, please contact me.
