//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.

#pragma once

#include "SKCommandHandler.h"
#include "SKImageDisplayer.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>

#define sqr(x) ((x)*(x))

class SKCommandHandlerImpl
{
public:
	//Compare two strings.(Case insensitive)
	bool compare(const string& x, const string& y);
	//For split string.
	vector<string> split(string str, string pattern);

	void help();
	void load(string s);
	void save(string s);
	void display(string name);
	void hide();
	void exit();

	void calculate();

	//double dot_to_line(double p0x, double p0y, double pax, double pay, double pbx, double pby);
	//double dot_to_dot(double pax, double pay, double pbx, double pby);

	////Bigger the better.
	///*box must have 4 points*/
	//double evaluation_squre(CvPoint2D32f *box);
	///*distance must be 0 ~ 1*/
	//double evaluation_cross(CvRect rect, CvPoint2D32f *box, double distance);
	IplImage *input;
	cv::Mat inputmat;
	SKImageDisplayer idis;
};

SKCommandHandler::SKCommandHandler()
{
	_impl = new SKCommandHandlerImpl();
	_impl->input = nullptr;
}
SKCommandHandler::~SKCommandHandler()
{
	if (_impl->input != nullptr)	cvReleaseImage(&(_impl->input));
	delete _impl;
}
void SKCommandHandler::begin()
{
	printf("Input command.Input \"help\" for command help.\n$");
}
bool SKCommandHandler::handle(char *s)
{
	string sr(s);
	return handle(sr);
}
bool SKCommandHandler::handle(string s)
{
	try
	{
		vector<string> splitstr = _impl->split(s, " ");
		if (_impl->compare(splitstr[0], "help"))
			_impl->help();
		else if (_impl->compare(splitstr[0], "load"))
		{
			if (splitstr.size() < 2)
				printf("Error\n");
			else
				_impl->load(splitstr[1]);
		}
		else if (_impl->compare(splitstr[0], "save"))
		{
			if (splitstr.size() < 2)
				printf("Error\n");
			else
				_impl->save(splitstr[1]);
		}
		else if (_impl->compare(splitstr[0], "display"))
		{
			_impl->display("ImageNow");
		}
		else if (_impl->compare(splitstr[0], "hide"))
		{
			_impl->hide();
		}
		else if (_impl->compare(splitstr[0], "exit"))
		{
			printf("See you next time!\n");
			return true;
		}
		else if (_impl->compare(splitstr[0], "calculate"))
		{
			printf("Calculating...\n");
			_impl->calculate();
		}
#ifdef _DEBUG
		else if (_impl->compare(splitstr[0], "loadd"))
		{
			_impl->load("F:\\360Downloads\\3.jpg");
		}
		else if (_impl->compare(splitstr[0], "cad"))
		{
			printf("Calculating...\n");
			_impl->calculate();
		}
		else if (_impl->compare(splitstr[0], "savd"))
		{
			_impl->save("C:\\Users\\ShadowK\\Desktop\\123.jpg");
		}
#endif
		else
		{
			printf("Error\n");
		}
	}
	catch (...)
	{
		printf("Some error just happened. You may re-input your command if you wish.\n");
	}
	printf("\n");
	return false;
}

bool SKCommandHandlerImpl::compare(const string& x, const string& y)
{
	string::const_iterator p = x.begin();
	string::const_iterator q = y.begin();
	while (p != x.end() && q != y.end() && toupper(*p) == toupper(*q))
	{
		++p;
		++q;
	}
	if (p == x.end())
	{
		return (q == y.end());
	}
	if (q == y.end())
	{
		return false;
	}
	return false;
}
vector<string> SKCommandHandlerImpl::split(string str, string pattern)
{
	int pos;
	std::vector<std::string> result;
	if (str[str.length() - 1] == '\n')
		str.pop_back();
	str += pattern;
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}
void SKCommandHandlerImpl::help()
{
	printf("\"load <FileName>\" Load or reload a image with full location.\n");
	printf("Sample: load C:\\Users\\WIN_20150404_144352.JPG\n");
	printf("\"display\"Display the image.\n");
	printf("Sameple: display\n");
	printf("\"hide\"Stop displaying the image.\n");
	printf("Sameple: hide\n");
	printf("\"save <FileName>\"Save the image.Input the Name without the path to save it at the location of the program.\n");
	printf("Sameple: save C:\\Users\\WIN_20150404_144352.JPG\n");
	printf("\"calculate\"Do the main work of the program.\n");
	printf("Sameple: calculate\n");
	printf("\"exit\"Exit the program\n");
	printf("Sameple: exit\n");
	printf("WARNING:Do not use image which has spaces in its FileName.\n");

}
void SKCommandHandlerImpl::load(string s)
{
	input = cvLoadImage(s.c_str(), 1);
	if(input == nullptr)
		printf("Wrong FileName.\n");
	else
	{
		printf("Load successful.\n");
		inputmat = cv::Mat(input, false);
	}
}
void SKCommandHandlerImpl::save(string s)
{
	if (input == nullptr)
	{
		printf("You haven't load an image yet!\n");
	}
	cvSaveImage(s.c_str(), input);
	printf("Save successful.\n");
}
void SKCommandHandlerImpl::display(string s)
{
	if(input != nullptr)
		idis.display(&input,s.c_str());
}
void SKCommandHandlerImpl::hide()
{
	idis.hide();
}
void SKCommandHandlerImpl::exit()
{
	hide();
	printf("See you next time!\n");
}

void SKCommandHandlerImpl::calculate()
{
	if (input == nullptr || (input->nChannels != 3 && input->nChannels != 1))
		return;
	CvPoint ans;
	ans = SKCommandHandlerVideo::GetDrone(input);
	if (ans.x > 0 && ans.y > 0)
#ifdef CIRCLE_RADIUS
		cvCircle(input, ans, CIRCLE_RADIUS, CV_RGB(255, 0, 0), 2);
#else
		cvCircle(input, ans, 10, CV_RGB(255, 0, 0), 2);
#endif
	#pragma region OLD
	//if (input->nChannels == 3)
	//{
	//	IplImage *p = cvCreateImage(cvSize(input->width, input->height), input->depth, 1);
	//	cvCvtColor(input, p, CV_BGR2GRAY);
	//	IplImage *q;
	//	q = input;
	//	input = p;
	//	inputmat = cv::Mat(input);
	//	cvReleaseImage(&q);
	//}
//	cvAdaptiveThreshold(input, input, 255, 0, CV_THRESH_BINARY_INV, 25);
//
//	cvDilate(input, input);
//	//cvDilate(input, input);
//	//cvErode(input, input);
//
//	CvMemStorage* storage = cvCreateMemStorage(0);
//	CvSeq* contours = NULL;
//
//	IplImage *tmp = cvCloneImage(input);
//	printf("FindSEQ:%d\n", cvFindContours(tmp, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE));
//	cvReleaseImage(&tmp);
//
//	IplImage* outlineImg = cvCreateImage(cvGetSize(input), IPL_DEPTH_32F, 3);
//	int index = 0;
//	CvSeq *pCurSeq = contours;
//	CvSeq *pOldSeq = NULL;
//	while (pCurSeq)
//	{
//		double tmparea = fabs(cvContourArea(pCurSeq));
//		if (tmparea < CROSS_RECT_SIZE)//删除掉面积比较小的轮廓
//		{
//			pCurSeq = pCurSeq->h_next;
//			continue;
//		}
//		if (cvCheckContourConvexity(pCurSeq) == 1)//凸轮廓
//		{
//			pCurSeq = pCurSeq->h_next;
//			continue;
//		}
//		CvRect rect1 = cvBoundingRect(pCurSeq, 0);
//		CvBox2D rect2d = cvMinAreaRect2(pCurSeq);
//		CvPoint2D32f rect4p[4];
//		cvBoxPoints(rect2d, rect4p);
//		double a = evaluation_squre(rect4p);
//		double b = evaluation_cross(rect1, rect4p, 0.25);
//		printf("SEQ_VALUE:%lf Center:(%d,%d)\n", a * CROSS_THR_BALANCE + b * (1 - CROSS_THR_BALANCE), (int)rect2d.center.x, (int)rect2d.center.y);
//		if (a * CROSS_THR_BALANCE + b * (1 - CROSS_THR_BALANCE) > CROSS_THR)
//		{
//#ifdef DRAW_SEQ
//			for (int i = 0; i < 4; i++)
//				for (int j = i + 1; j < 4; j++)
//					cvLine(outlineImg, cvPoint(rect4p[i].x, rect4p[i].y), cvPoint(rect4p[j].x, rect4p[j].y), CV_RGB(0, 0, 255));
//			cvDrawContours(outlineImg, pCurSeq, CV_RGB(0, 255, 0), CV_RGB(0, 255, 0), 0, 2, CV_FILLED, cvPoint(0, 0));
//			cvRectangleR(outlineImg, rect1, CV_RGB(255, 0, 0), 1, 8, 0);
//#endif
//		}
//		pCurSeq = pCurSeq->h_next;
//	}
//	IplImage *q;
//	q = input;
//	input = outlineImg;
//	inputmat = cv::Mat(input);
//	cvReleaseImage(&q);
	#pragma endregion
	printf("\n");
}


#pragma region OLD

//double SKCommandHandlerImpl::dot_to_line(double p0x, double p0y, double pax, double pay, double pbx, double pby)
//{
//	double squre = abs(p0x*pay + pax*pby + pbx*p0y - p0x*pby - pax*p0y - pbx*pay);
//	return squre / dot_to_dot(pax,pay,pbx,pby);
//}
//double SKCommandHandlerImpl::dot_to_dot(double pax, double pay, double pbx, double pby)
//{
//	return sqrt(sqr(pax - pbx) + sqr(pay - pby));
//}
//
//double SKCommandHandlerImpl::evaluation_squre(CvPoint2D32f *box)
//{
//	try
//	{
//		box[3];
//	}
//	catch (...)
//	{
//		return .0;
//	}
//	double l1 = dot_to_dot(box[0].x, box[0].y, box[1].x, box[1].y);
//	double l2 = dot_to_dot(box[0].x, box[0].y, box[3].x, box[3].y);
//	return min(l1, l2) / max(l1, l2);
//}
//double SKCommandHandlerImpl::evaluation_cross(CvRect rect, CvPoint2D32f *box, double distance)
//{
//	double diagonal = dot_to_dot(box[0].x, box[0].y, box[2].x, box[2].y);
//	double threshold = 1.0 / 4 * diagonal * distance;
//	double full = 0;
//	for (int i = rect.y; i < min(rect.y + rect.height, input->height); i++)
//		for (int j = rect.x; j < min(rect.x + rect.width, input->width); j++)
//		{
//			if (((uchar *)input->imageData)[i * input->widthStep / sizeof(uchar) + j] == 255)
//			{
//				double dis_cross = dot_to_line(j, i, box[0].x, box[0].y, box[2].x, box[2].y);
//				dis_cross = min(dis_cross, dot_to_line(j, i, box[1].x, box[1].y, box[3].x, box[3].y));
//				full = full + ((dis_cross < threshold) ? 1 : -1);
//			}
//		}
//	full = full / (4 * diagonal * threshold);
//
//	full *= 2;
//
//	if (full > 1) full = 1;
//	return (full < 0) ? 0 : full;
//}

#pragma endregion
