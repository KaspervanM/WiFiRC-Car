/*
  ConnectToEEPROM.h - Library for saving and loading of the passwords and SSID's in the EEPROM.
  Created by Kasper van Maasdam, July 3, 2020.
  Released into the public domain.
*/

#ifndef ConnectToEEPROM_h
#define ConnectToEEPROM_h

#include "Arduino.h"
#include <EEPROM.h>

class ConnectToEEPROM
{
  public:
    ConnectToEEPROM(const unsigned short SSID_SIZE, const unsigned short WIFI_MEMORY);
		struct Credentials{char** ssid; char** password;};
    void save(char ssid[][32], char password[][32]);
    Credentials load();
		unsigned short getNextIndex(char ssid[][32]);
  private:
    const unsigned short _SSID_SIZE;
		const unsigned short _WIFI_MEMORY;
};
#endif
