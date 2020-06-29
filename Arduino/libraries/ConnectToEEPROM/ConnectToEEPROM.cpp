/*
  ConnectToEEPROM.h - Library for saving and loading of the passwords and SSID's in the EEPROM.
  Created by Kasper van Maasdam, July 3, 2020.
  Released into the public domain.
*/

#include "Arduino.h"
#include "ConnectToEEPROM.h"
#include <EEPROM.h>

ConnectToEEPROM::ConnectToEEPROM(const unsigned short SSID_SIZE, const unsigned short WIFI_MEMORY)
 : _SSID_SIZE(SSID_SIZE), _WIFI_MEMORY(WIFI_MEMORY) {}

void ConnectToEEPROM::save(char ssid[][32], char password[][32])
{
  /** Empty out the previous save */
  EEPROM.begin(2 * _SSID_SIZE * _WIFI_MEMORY);
  for (int i = 0 ; i < 2 * _SSID_SIZE * _WIFI_MEMORY ; i++) {
    EEPROM.put(i, 0);
  }
  for (int i = 0 ; i < _WIFI_MEMORY ; i++) {
		char* ssid_temp = ssid[i];
		char* pass_temp = password[i];
		for (unsigned short s = 0; s < _SSID_SIZE; s++) {
    	EEPROM.write(i * _SSID_SIZE + s, (int) ssid_temp[s]);
			if (String(ssid_temp[s]).length() == 0) break;
		}
		for (unsigned short p = 0; p < _SSID_SIZE; p++) {
    	EEPROM.write(_SSID_SIZE * _WIFI_MEMORY + i * _SSID_SIZE + p, (int) pass_temp[p]);
			if (String(pass_temp[p]).length() == 0) break;
		}
    EEPROM.put((_SSID_SIZE * _WIFI_MEMORY + i * _SSID_SIZE), password[i]);
  }
  EEPROM.commit();
  EEPROM.end();
}

ConnectToEEPROM::Credentials ConnectToEEPROM::load()
{
	char** ssid = new char*[_SSID_SIZE];
	char** password = new char*[_SSID_SIZE];
	char* ssid_temp = new char[_SSID_SIZE];
	char* pass_temp = new char[_SSID_SIZE];

	EEPROM.begin(2 * _SSID_SIZE * _WIFI_MEMORY);
  for (unsigned short i = 0 ; i < _WIFI_MEMORY ; i++) {
		for (unsigned short s = 0; s < _SSID_SIZE; s++) {
    	ssid_temp[s] = (char) EEPROM.read(i * _SSID_SIZE + s);
			if (String(ssid_temp[s]).length() == 0) break;
		}
		ssid[i] = ssid_temp;
    ssid_temp = "";
		for (unsigned short p = 0; p < _SSID_SIZE; p++) {
    	pass_temp[p] = (char) EEPROM.read(_SSID_SIZE * _WIFI_MEMORY + i * _SSID_SIZE + p) ;
			if (String(pass_temp[p]).length() == 0) break;
		}
		password[i] = pass_temp;
    pass_temp = "";
    if (String(ssid[i]).length() == 0) break;
  }
  EEPROM.end();

	ConnectToEEPROM::Credentials creds;
	creds = {ssid, password};
	return creds;
}

unsigned short ConnectToEEPROM::getNextIndex(char ssid[][32])
{
	for (unsigned short i = 0 ; i < _WIFI_MEMORY ; i++) {
    if (i == _WIFI_MEMORY - 1) return _WIFI_MEMORY - 1;
		if (String(ssid[i]).length() == 0) return i;
  }
}
