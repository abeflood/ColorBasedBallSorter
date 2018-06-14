//#include "stdafx.h"
#include "CBluesort.h"

//defines pins for outputs
#define MODE_LED 16
#define PASS_LED 20
#define FAIL_LED 21
#define MODE_SWITCH 26
#define PASS_SWITCH 12
#define FAIL_SWITCH 6

//returns time since comp on in ms
double GetTickCount(void)		//not my code...from https://stackoverflow.com/questions/7729686/convert-gettickcount-vxworks-to-linux
{
	struct timespec now;
	if (clock_gettime(CLOCK_MONOTONIC, &now))
		return 0;
	return now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0;
}

//sets intial values
CBluesort::CBluesort()
{
	pass_flag = false;
	fail_flag = false;
	on_flag = false;
	reset_flag = false;
	pass_count = 0;
	fail_count = 0;
	
	mode_button = 0;
	pbt1 = 0;
	pbtime1 = 0;
	timebetween1 = 0;
	
	pbt2 = 0;
	pbtime2 = 0;
	timebetween2 = 0;
	
	pbt3 = 0;
	pbtime3 = 0;
	timebetween3 = 0;
	
	blue_timer = 1;
	pbt4 = 0;
	pbtime4 = 0;
	timebetween4 = 0;
}

//sets ball status flags in both manuel and automatic mode
//checks status of pushbuttons and sets pushbutton flags
void CBluesort::update(char getkey)
{
	//checks if auto mode or manual mode is turned on
	if (on_flag == true)
	{
		cout << "Auto mode\n";
		//checks if there is ball waiting to be sorted
		if (cam.ball_check())
		{
			//ensures there is time for ball to be sorted before it is checked again
			if (blue_timer == 1)
			{
				//check if the ball is blue
				if (cam.blue_check())
				{
					cout <<"AUTO pass\n";
					pass_flag = true;
				}
				else
				{
					cout <<"AUTO fail\n";
					fail_flag = true;
				}
				//resets timer
				blue_timer = 0;
				pbt4 = 1; 
				cout << "BALL detected\n";
		}
		}
	}
	else
	{
		cout << "Manual mode\n";
		//passes based on key press
		if (getkey == 'p')
		{
			cout << "KEY pass\n";
			pass_flag = true;
		}
		//rejects based on key press
		if (getkey == 'r')
		{
			fail_flag = true;
			cout <<"KEY fail\n";
		}
	}
	
	// checks pushbutton status and debounces using timer
	pbt1 = servo.get_switch(MODE_SWITCH);
	if (pbt1 == 1)
	{
		pbtime1 = GetTickCount();
		pbt1 = 0;
		pbtt1 = 1;
		
	}
	//sets mode change flag
	if ((((pbtime1 + 100) - GetTickCount()) < 0) && (pbtt1 == 1))
	{
		mode_button = 1;
	
		pbtt1 = 0;
		pbt1 = 0;
	}
	
	// checks pushbuttons (pass and fail) status and debounces using timer, seperate timer for each pushbutton -- only if in manuel mode
	if (on_flag == false)
	{
		pbt2 = servo.get_switch(PASS_SWITCH);
	if (pbt2 == 1)
	{
		pbtime2 = GetTickCount();
		pbt2 = 0;
		pbtt2 = 1;
		
	}
	//sets pass flag
	if ((((pbtime2 + 100) - GetTickCount()) < 0) && (pbtt2 == 1))
	{
		pass_flag = 1;
		cout << "BUT pass\n";
		pbtt2 = 0;
		pbt2 = 0;
	}
	
		pbt3 = servo.get_switch(FAIL_SWITCH);
	if (pbt3 == 1)
	{
		pbtime3 = GetTickCount();
		pbt3 = 0;
		pbtt3 = 1;
		
	}
	//sets fail flag flag
	if ((((pbtime3 + 100) - GetTickCount()) < 0) && (pbtt3 == 1))
	{
		fail_flag = 1;
		cout << "BUT fail\n";
		pbtt3 = 0;
		pbt3 = 0;
	}
	}
	
	if (pbt4 == 1)
	{
		pbtime4 = GetTickCount();
		pbt4 = 0;
		pbtt4 = 1;
		
	}
	
	
	if ((((pbtime4 + 2000) - GetTickCount()) < 0) && (pbtt4 == 1))
	{
		blue_timer = 1;
	//	cout << "timer in hur\n";
		pbtt4 = 0;
		pbt4 = 0;
	}
	//if modde change intiated either with keyboard or pushbutton
	if ((getkey == 'o') || (mode_button == 1))
	{
		//change on flag to off and vice-versa
		on_flag = !on_flag;
		mode_button = 0;
	}
	//resets system
	if (getkey == 'g')
	{
		reset_flag = true;	
	}
}

//sets servos based on flags set above
//displays image of what camera sees
void CBluesort::draw()
{
	std::string modestr, countstr;
	
	//configures system into auto mode
	if (on_flag == true)
		{
		servo.turn_on_led(MODE_LED);
		modestr = "SYSTEM ON";	
		}
		//configures system into manual mode
	else
		{
		servo.turn_off_led(MODE_LED);
		modestr = "SYSTEM OFF";		
		}
		
	//uses servo to pass ball
	if (pass_flag == true)
	{
		pass_count++;
		servo.turn_on_led(PASS_LED);
		servo.turn_off_led(FAIL_LED);
		servo.pass_ball();
		pass_flag = false;
		
	}
	
	//uses servo to fail ball
	if (fail_flag == true)
	{
		fail_count++;
		servo.turn_on_led(FAIL_LED);
		servo.turn_off_led(PASS_LED);
		servo.fail_ball();
		fail_flag = false;
		
	}
	
	//resets system, including pass/fail count
	if (reset_flag == true)
	{
		pass_flag = false;
		fail_flag = false;
		on_flag = false;
		reset_flag = false;
		pass_count = 0;
		fail_count = 0;
	}
	
	//puts pass and fail counts into string
	stringstream ss2;
	ss2 << "Passed: " << pass_count << " Failed: " << fail_count;
	countstr = ss2.str();
	
	//prints image from webcam with pass/fail counts on it
	_canvas = cam.return_frame();
	cv::putText(_canvas, modestr, cv::Point(50, 50), 1, 2, cv::Scalar(255, 255, 255));
	cv::putText(_canvas, countstr, cv::Point(50, 400), 1, 2, cv::Scalar(255, 255, 255));
	cv::imshow("FINAL", _canvas);
}

//runs program in a loop
void CBluesort::run()
{
	char test = 0;

	do
	{
		update(test);
		draw();
		test = cv::waitKey(1);
	} while (test != 'q');
}
