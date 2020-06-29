/*
  Steering.h - Library for movent of components of the WiFiRC-Car project.
  Created by Kasper van Maasdam, July 2, 2020.
  Released into the public domain.
*/
#ifndef Steering_h
#define Steering_h

#include "Arduino.h"

class Steering
{
  public:
    Steering();//const unsigned short PWMA_GPIO, const unsigned short PWMB_GPIO, const unsigned short DA_GPIO, const unsigned short DB_GPIO);
    void move(String serial_input);
    void stop();
  private:
    const unsigned short _PWMA_GPIO = 5;
		const unsigned short _PWMB_GPIO = 4;
		const unsigned short _DA_GPIO = 0;
		const unsigned short _DB_GPIO = 2;
};
/*
class CameraMovement
{
	public:
		CameraMovement(const unsigned short SERVO_PIN1, const unsigned short SERVO_PIN2, const unsigned short DEFAULTCAMHORIZONTAL, const unsigned short DEFAULTCAMVERTICAL, const unsigned short CAMSTEP)
		void moveCamHorizontal(unsigned short direction);
		void moveCamVertical(unsigned short direction);

	private:
		const unsigned short _SERVO_PIN1;
		const unsigned short _SERVO_PIN2;
		const unsigned short _DEFAULTCAMHORIZONTAL;
		const unsigned short _DEFAULTCAMVERTICAL;
		const unsigned short _CAMSTEP;
		unsigned short horizontalServoPos;
		unsigned short verticalServoPos;
};*/
#endif
