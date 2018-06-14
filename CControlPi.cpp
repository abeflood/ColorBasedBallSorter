
#include <iostream>
#include <sstream>
#include "pigpio.h"

using namespace std;


#include "CControlPi.h"

//intialize PIGPIO
 CControlPi::CControlPi()
{
	 if (gpioInitialise() < 0)
	 {
		 cout << "GPIO INTIALIZED FAILED\n\n";
	 }
}

//Terminate PIGPIO
 CControlPi::~CControlPi()
 {
	 gpioTerminate();
 }

//Get digital signal from Pi I/O pin
void CControlPi::get_data(int channel, int &value)
{
	gpioSetMode(channel, PI_INPUT);
	value  = gpioRead(channel);
}

//Set Pi I/O pin to digital signal
void CControlPi::set_data(int channel, int value)
{
	gpioSetMode(channel, PI_OUTPUT);
	gpioWrite(channel, value);

}

//control servo using Pi i/o pin
void CControlPi::set_servo(int channel, int value)
{
	gpioSetMode(channel, PI_OUTPUT);
	gpioServo(channel, value);

}

//gets current status of button
int CControlPi::get_button(int channel)
{
	int value = 0;
	int tmpvalue;
	get_data(channel, tmpvalue);
	if (tmpvalue == 0)
		value = 1;
	return value;
}

