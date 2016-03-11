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

#ifndef ROC_DEBUG_MODE
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
		IplImage p = IplImage(input_frame);
		IplImage *pp = cvCloneImage(&p);
		SKCommandHandlerVideo::GetDrone(pp);
		//cv::imshow("Test", input_frame);
		if (_kbhit() != 0)
			break;
		//cv::waitKey(20);
		n_frame++;
	}
	return 0;
}
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
//#include"SKCommandHandlerVideo.cpp"
double CROSS_THR = 0.5;
int main()
{
	SKCommandHandlerVideo sk_com;
	CvCapture *capture;
	capture = cvCreateFileCapture("roc.m4v");
	IplImage *space, *frame, *copy_frame;
	frame = cvQueryFrame(capture);
	space = cvCreateImage(cvSize(frame->width, frame->height), frame->depth, 1);
	CvPoint cp;
	cp.x = -1;
	cp.y = -1;
	int skip = 15;
	CROSS_THR = 0.63;
	FILE *out = fopen("info.txt", "w");
	for (int j = 0; j < 1; j++)
	{
		cvReleaseCapture(&capture);
		capture = cvCreateFileCapture("roc.m4v");
		frame = cvQueryFrame(capture);
		//cvSetCaptureProperty(capture, CV_CAP_PROP_POS_AVI_RATIO, 0);
		CROSS_THR += 0.003;
		int f1 = 49, f2 = 51;
		int false_ans = 0, true_ans = 0;
		vector<Evaluation_Data> v_e_d;
		for (int i = 0; i < skip * 100; i++)
		{
			frame = cvQueryFrame(capture);
			if (i % skip == 0)
			{
				v_e_d.clear();
				//copy_frame = cvCloneImage(frame);
				if ((i / skip) % 50 == 0)
					printf("%d%%\n", i / skip);
				cp = SKCommandHandlerVideo::getquadrotor(frame, (cp.x < 0) ? nullptr : &cp, nullptr, space, &v_e_d);
				if (cp.x > 0 && (i / skip < f1))
					false_ans++;
				if (cp.x < 0 && (i / skip >= f1))
					true_ans++;

				if (i / skip < f1)
				{
					for (int ii = 0; ii < v_e_d.size(); ii++)
					{
						fprintf(out, "%5lf  %5lf  %7lf  %5lf 0\n", v_e_d[ii].evaluation_a, v_e_d[ii].evaluation_b, v_e_d[ii].distance, v_e_d[ii].t_value);
					}
				}
				else
				{
					int max_ii = 0;
					for (int ii = 0; ii < v_e_d.size(); ii++)
					{
						if (v_e_d[ii].t_value > v_e_d[max_ii].t_value)
							max_ii = ii;
					}
					for (int ii = 0; ii < v_e_d.size(); ii++)
					{
						fprintf(out, "%5lf  %5lf  %7lf  %5lf %d\n", v_e_d[ii].evaluation_a, v_e_d[ii].evaluation_b, v_e_d[ii].distance, v_e_d[ii].t_value, (ii == max_ii) ? (1) : (0));
					}
				}
				fprintf(out, "\n\n");

				//if (cp.x > 0)
				//cvCircle(copy_frame, cp, 20, CV_RGB(255, 0, 0), 4);
				//char name[50] = { 0 };
				//sprintf(name, "a//%.1lf//%d.jpg", CROSS_THR, i / skip);
				//cvSaveImage(name, copy_frame);
			}
		}
		//printf("%lf%%  %lf%%  %lf\n", ((double)false_ans) / f1 * 100, ((double)true_ans) / f2 * 100, CROSS_THR);
	}
	fclose(out);
	system("pause");
	return 0;
}
#endif