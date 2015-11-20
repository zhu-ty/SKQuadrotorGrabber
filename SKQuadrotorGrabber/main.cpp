//SKQuadrotorGrabber
//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.

#pragma once

#include "SKDefinition.h"

#ifndef CAMERA_DELAY_MODE
#include <string>
#ifdef PIC_MODE
#include "SKCommandHandler.h"
#else
#include "SKCommandHandlerVideo.h"
#endif
#else
#include<cv.h>
#include<opencv2\opencv.hpp>
#include<iostream>
#include<vector>
#include"SKCommandHandlerVideo.h"
#include"TimeLogger.h"
#include"SKImageDisplayer.h"
#include<conio.h>
#endif


#ifndef CAMERA_DELAY_MODE
int main()
{
#ifdef PIC_MODE
	SKCommandHandler cmd;
#else
	SKCommandHandlerVideo cmd;
#endif
	while (1)
	{
		string str;
		cmd.begin();
		getline(cin, str);
		if (cmd.handle(str))
			break;
	}
	return 0;
}
#else
int main()
{
	cv::VideoCapture input_video(1);
	cv::Mat input_frame;
	TimeLogger tl;
	int timer_num = tl.get_num();
	int n_frame = 0;
	while (1)
	{
		if (n_frame == 10)
		{
			tl.r(timer_num);
			n_frame = 0;
			system("cls");
			printf("fps:%lf",1e3*10.0/tl.get_n_elapsed(timer_num));
			tl.restart(timer_num);
			tl.s(timer_num);
		}
		input_video >> input_frame;
		//cv::imshow("Test", input_frame);
		if (_kbhit() != 0)
			break;
		//cv::waitKey(20);
		n_frame++;
	}
	return 0;
}
#endif