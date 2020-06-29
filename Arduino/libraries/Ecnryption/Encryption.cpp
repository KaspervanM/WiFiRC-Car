/*
  Encryption.h - Library for the encryption and decryption of the passwords saved in the WiFiRC-Car project.
  Created by Kasper van Maasdam, July 3, 2020.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Encryption.h"

Encryption::Encryption(const char* passKey)
 : _passKey(passKey) {}

char* Encryption::encrypt(void* pass)
{
  unsigned short len = 0;
  while (*((char*) (pass + len)) != '\0') len++;
  unsigned short keylen = String(_passKey).length();
  char c, _p;
  short p;
	unsigned short currKeyDigit = 0;

  char* Lpass = new char[len];
  for (unsigned short l = 0; l <= len; l++) Lpass[l] = '\0';

  for (unsigned short j = 0; j < len; j++) {
    c = *((char*) (pass + j));
    p = short(c) + short(_passKey[currKeyDigit % keylen]);
    while (p > 126) p -= 93;
    _p = char(p);
    Lpass[j] = _p;
    currKeyDigit ++;
  }
  return Lpass;
}

char* Encryption::decrypt(void* pass)
{
  unsigned short len = 0;
  while (*((char*) (pass + len)) != '\0') len++;
  unsigned short keylen = String(_passKey).length();
	char c, _p;
  short p;
	unsigned short currKeyDigit = 0;

  char* Lpass = new char[len];
  for (unsigned short l = 0; l <= len; l++) Lpass[l] = '\0';

  for (unsigned short j = 0; j < len; j++) {
    c = *((char*) (pass + j));
    p = short(c) - short(_passKey[currKeyDigit % keylen]);
    while (p < 33) p += 93;
    _p = char(p);
    Lpass[j] = _p;
    currKeyDigit ++;
  }
  return Lpass;
}
