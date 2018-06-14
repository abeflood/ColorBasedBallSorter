//#include "stdafx.h"
#include "CCamera.h"
#include <vector>
#define PASS_AREA 10000

//intialize camera
CCamera::CCamera()
{
	cam.open(0);
}

//check if there is blue ball waiting to be sorted
bool CCamera::blue_check()
{
	bool test = false;

	cam >> intial;

	//convert image to HSV then convert to binary image based on blue
	cv::cvtColor(intial, hsv, CV_BGR2HSV);
	cv::inRange(hsv, cv::Scalar(106, 30, 70), cv::Scalar(109, 255, 255), bluesel);
	
	//clean up binary image
	cv::erode(bluesel, erode, cv::Mat());
	cv::dilate(erode, dilate, cv::Mat());
	
	bounded = intial;

	//find shapes in binary image
	findContours(dilate, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	for (unsigned int i = 0; i < contours.size(); i++)
	{
		cv::Rect r = boundingRect(contours.at(i));
		drawContours(bounded, contours, i, cv::Scalar(255, 255, 255), -1, 8, hierarchy);
		
		//if the shape is larger then threshold..return true
		if (r.area() > PASS_AREA)
		{
			cv::rectangle(bounded, r, cv::Scalar(255, 255, 255));
			test = true;
			cv::putText(bounded, "PASSED", cv::Point(50, 50), 1, 2, cv::Scalar(255, 255, 255));
		}
	}
	
	
	if (!test)
	{
	cv::putText(bounded, "REJECTED", cv::Point(50, 50), 1, 2, cv::Scalar(255, 255, 255));
	}

	//cv::imshow("bounded", bounded);

	return test;
}

//check if there is a ball waiting to be sorted
bool CCamera::ball_check()
{
	bool test = false;

	cam >> intial;
	
	//convert to greyscale then convert to binary based on a threshold value
	cv::cvtColor(intial, grey, CV_BGR2GRAY);
	cv::threshold(grey, threshold, 200, 255, 0);
	
	//clean up binary image
	cv::erode(threshold, between, cv::Mat());
	cv::dilate(between, between2, cv::Mat());

	ballcheck = between2;

	//find shapes in binary image
	findContours(between2, contours2, hierarchy2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	for (unsigned int i = 0; i < contours2.size(); i++)
	{
		cv::Rect r = boundingRect(contours2.at(i));
		drawContours(ballcheck, contours2, i, cv::Scalar(255, 255, 255), -1, 8, hierarchy2);
		//if shapes are is greater than threshold...then return true 
		if (r.area() > (PASS_AREA +PASS_AREA))
		{
			cv::rectangle(ballcheck, r, cv::Scalar(255, 255, 255));
			test = true;
		}
	}
	
	if (!test)
	{
		bounded = intial;
		cv::putText(bounded, "NO BALL", cv::Point(50, 50), 1, 2, cv::Scalar(255, 255, 255));
	}
	return test;
}

//returns one image from camera
cv::Mat CCamera::return_frame()
{
	cv::Mat ret;
	cam >> ret;
	return ret;
}
