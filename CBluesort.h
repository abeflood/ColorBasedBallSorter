#pragma once

#include "CControlPi.h"
#include "CCamera.h"
#include "CServo.h"
#include "Cbase.h"
// OpenCV Include
#include <opencv2/opencv.hpp>
using namespace std;


class CBluesort : public Cbase
{
protected:
	CCamera cam;
	bool pass_flag, fail_flag, on_flag, reset_flag;
	char getkey;
	CServo servo;
	int pass_count, fail_count;
	int mode_button, blue_timer, pbt1, pbtt1; 
	float pbtime1, timebetween1;
	
	int pbt2, pbtt2; 
	float pbtime2, timebetween2;
	
	int pbt3, pbtt3; 
	float pbtime3, timebetween3;
	
	int pbt4, pbtt4; 
	float pbtime4, timebetween4;

public:
	CBluesort();

	void update(char getkey);

	void draw();

	void run();


};
