/*
  Encryption.h - Library for the encryption and decryption of the passwords saved in the WiFiRC-Car project.
  Created by Kasper van Maasdam, July 3, 2020.
  Released into the public domain.
*/

#ifndef Encryption_h
#define Encryption_h

#include "Arduino.h"

class Encryption
{
  public:
    Encryption(const char* passKey);
    char* encrypt(void* pass);
    char* decrypt(void* pass);
  private:
		const char* _passKey;
};
#endif
