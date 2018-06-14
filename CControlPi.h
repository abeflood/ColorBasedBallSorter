
#pragma once

#include "pigpio.h"

using namespace std; 

enum { DIGITAL = 0, ANALOG, SERVO };



#define SERVO_MAX 2400
#define SERVO_MIN 700

//allows for control of Raspberry Pi I/O Pins
class CControlPi
{
private:

public:

	 CControlPi();
	~CControlPi();
	
	void get_data(int channel, int &value);
	
	void set_data(int channel, int value);
	
	void set_servo(int channel, int value);
	
	void get_analog(float &value);

	int get_button(int channel);
};
