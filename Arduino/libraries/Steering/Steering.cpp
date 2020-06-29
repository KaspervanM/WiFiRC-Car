/*
  Movement.h - Library for movent of components of the WiFiRC-Car project.
  Created by Kasper van Maasdam, July 2, 2020.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Steering.h"

Steering::Steering()//const unsigned short PWMA_GPIO, const unsigned short PWMB_GPIO, const unsigned short DA_GPIO, const unsigned short DB_GPIO)
{
	pinMode(_PWMA_GPIO, OUTPUT);
  pinMode(_PWMB_GPIO, OUTPUT);
  pinMode(_DA_GPIO, OUTPUT);
  pinMode(_DB_GPIO, OUTPUT);
/*
	_PWMA_GPIO = PWMA_GPIO;
  _PWMB_GPIO = PWMB_GPIO;
  _DA_GPIO = DA_GPIO;
  _DB_GPIO = DB_GPIO;
	*/
}

void Steering::move(String serial_input)
{
	short index[4] = {serial_input.indexOf("l+"), serial_input.indexOf("l-"), serial_input.indexOf("r+"), serial_input.indexOf("r-")};
	unsigned short speeds[4] = {0, 0};
	for (unsigned short i = 0; i < 4; i++) {
		if (index[i] != -1) {
			serial_input.setCharAt(index[i], ',');
			serial_input.remove(index[i]+1);
		}
	}

	{
		unsigned short i = 0;
		unsigned short i2 = 0;
		while (serial_input != "") {
			if (serial_input[i] == ',') {
				short speed = serial_input.substring(0, i).toInt();
				if (speed > 1000) speed = 1000;
				if (speed < 100) speed = 100;
				speeds[i2] = speed;
				i2++;
				serial_input.remove(0, i+1);
				i = -1;
			}
			i++;
		}
	}

	if (speeds[0] == 0) speeds[0] = speeds[1];
	else if (speeds[1] == 0) speeds[1] = speeds[0];

	if (index[0] != -1) {
		analogWrite(_PWMA_GPIO, speeds[0]);
		digitalWrite(_DA_GPIO, LOW);
	} if (index[2] != -1) {
		analogWrite(_PWMB_GPIO, speeds[1]);
		digitalWrite(_DA_GPIO, LOW);
	} if (index[1] != -1) {
		analogWrite(_PWMA_GPIO, speeds[0]);
		digitalWrite(_DA_GPIO, HIGH);
	} if (index[3] != -1) {
		analogWrite(_PWMB_GPIO, speeds[1]);
		digitalWrite(_DB_GPIO, HIGH);
	}
}

void Steering::stop()
{
	analogWrite(_PWMA_GPIO, LOW);
	analogWrite(_PWMB_GPIO, LOW);
	digitalWrite(_DA_GPIO, LOW);
	digitalWrite(_DB_GPIO, LOW);
}
