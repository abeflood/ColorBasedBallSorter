#include "CServo.h"

//default servo values
#define SERVO_DEF 1600
#define SERVO_CHAN 17
#define SERVO_PASS 1680
#define SERVO_FAIL 1500
#define SERVO_DEFP 1580

//intialize pins for servo
CServo::CServo()
{
	port.set_servo(SERVO_CHAN, SERVO_DEF);
}

//move servo so ball is passed
void CServo::pass_ball()
{
	port.set_servo(SERVO_CHAN, SERVO_PASS);
	usleep(300000);
	port.set_servo(SERVO_CHAN, SERVO_DEFP);
}

//move servo so ball is rejected
void CServo::fail_ball()
{
	port.set_servo(SERVO_CHAN, SERVO_FAIL);
	usleep(300000);
	port.set_servo(SERVO_CHAN, SERVO_DEF);
}

//Turn on indication LED
void CServo::turn_on_led(int channel)
{
	port.set_data(channel, 1);
}

//Turn off indication Led
void CServo::turn_off_led(int channel)
{
	port.set_data(channel, 0);
}

//Get status of pushbutton
int CServo::get_switch(int channel)
{
	return port.get_button(channel);
}
