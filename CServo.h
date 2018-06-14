#pragma once
#include <unistd.h>
#include "CControlPi.h"
#include "CCamera.h"
#include "Cbase.h"
// OpenCV Include
#include <opencv2/opencv.hpp>



using namespace std;


class CServo
{
protected:
	CControlPi port;

public:
	CServo();
	void pass_ball();
	void fail_ball();
	void turn_on_led(int channel);
	void turn_off_led(int channel);
	int get_switch(int channel);



};
