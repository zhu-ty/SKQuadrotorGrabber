//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.

#pragma once

#include "SKCommandHandlerVideo.h"
#include "SKImageDisplayer.h"
#include "TimeLogger.h"
#include <vector>
#include <string>

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>


using namespace std;

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.hpp>
using namespace cv;

#define sqr(x) ((x)*(x))

class SKCommandHandlerVideoImpl
{
public:
	/********* Control Consts **********/
	const int thres_N = 10; //Level of adaptive filter
	const int area_min = 10;
	const int area_max = 100;
	/******* End Control Consts ********/

	/*Compare two strings.(Case insensitive)*/
	bool compare(const string& x, const string& y);
	/*For split string.*/
	vector<string> split(string str, string pattern);

	void help();
	void load(string s);
	//void save(string s);
	void exit();

	void calculate();

	static double dot_to_line(double p0x, double p0y, double pax, double pay, double pbx, double pby);
	static double dot_to_dot(double pax, double pay, double pbx, double pby);

	//Bigger the better.
	/*box must have 4 points*/
	static double evaluation_squre(CvPoint2D32f *box);
	/*distance must be 0 ~ 1*/
	static double evaluation_cross(CvRect rect, CvPoint2D32f *box, double distance, IplImage *in);
	static void DoG(Mat& src, Mat_<Vec3b>& dest);
	static void saveimage(const IplImage *p, const string s);

	static CvPoint getquadrotor(IplImage *picc, CvPoint *LastPoint = nullptr,TimeLogger *tl = nullptr,IplImage *space = nullptr);

	static const int ROIsize = 500;
	/*获得ROI左上角的点*/
	static CvPoint getROI(CvSize sz, CvPoint LastPoint);

	CvCapture *input;
	SKImageDisplayer idis;

	struct SKResult
	{
		CvPoint p;
		double trust;
	};
};

SKCommandHandlerVideo::SKCommandHandlerVideo()
{
	_impl = new SKCommandHandlerVideoImpl();
	_impl->input = nullptr;
}
SKCommandHandlerVideo::~SKCommandHandlerVideo()
{
	if (_impl->input != nullptr)	cvReleaseCapture(&_impl->input);
	delete _impl;
}
CvPoint SKCommandHandlerVideo::GetDrone(IplImage *pic)
{
	return SKCommandHandlerVideoImpl::getquadrotor(pic);
}

void SKCommandHandlerVideo::begin()
{
	printf("Input command.Input \"help\" for command help.\n$");
}
bool SKCommandHandlerVideo::handle(char *s)
{
	string sr(s);
	return handle(sr);
}
bool SKCommandHandlerVideo::handle(string s)
{
	try
	{
		if (s == "")
			return false;
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
		//else if (_impl->compare(splitstr[0], "save"))
		//{
		//	if (splitstr.size() < 2)
		//		printf("Error\n");
		//	else
		//		_impl->save(splitstr[1]);
		//}
		else if (_impl->compare(splitstr[0], "exit"))
		{
			printf("See you next time!\n");
			return true;
		}
		else if (_impl->compare(splitstr[0], "calculate"))
		{
			printf("Calculating...\n");
			_impl->calculate();

			return true;
		}
#ifdef _DEBUG
		else if (_impl->compare(splitstr[0], "loadd"))
		{
			//_impl->load("F:\\360Downloads\\3.jpg");
		}
		else if (_impl->compare(splitstr[0], "cad"))
		{
			printf("Calculating...\n");
			_impl->calculate();
		}
		//else if (_impl->compare(splitstr[0], "savd"))
		//{
		//	_impl->save("C:\\Users\\ShadowK\\Desktop\\123.jpg");
		//}
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

bool SKCommandHandlerVideoImpl::compare(const string& x, const string& y)
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
vector<string> SKCommandHandlerVideoImpl::split(string str, string pattern)
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
void SKCommandHandlerVideoImpl::help()
{
	printf("\"load <FileName>\" Load or reload a video with full location.\n");
	printf("Sample: load C:\\Users\\WIN_20150404_144352.mp4\n");
	//printf("\"save <FileName>\"Save the video.Input the Name without the path to save it at the location of the program.\n");
	//printf("Sameple: save C:\\Users\\WIN_20150404_144352.mp4\n");
	printf("\"calculate\"Do the main work of the program.\n");
	printf("Sameple: calculate\n");
	printf("\"exit\"Exit the program\n");
	printf("Sameple: exit\n");
	printf("WARNING:Do not use image which has spaces in its FileName.\n");

}
void SKCommandHandlerVideoImpl::load(string s)
{
	//input = cvLoadImage(s.c_str(), 1);
	if (input != nullptr)
		cvReleaseCapture(&input);
	input = cvCreateFileCapture(s.c_str());
	if(input == nullptr)
		printf("Wrong FileName.\n");
	else
	{
		printf("Load successful.\n");
	}
}
//void SKCommandHandlerVideoImpl::save(string s)
//{
//	if (input == nullptr)
//	{
//		printf("You haven't load an image yet!\n");
//	}
//	cvSaveImage(s.c_str(), input);
//	printf("Save successful.\n");
//}
void SKCommandHandlerVideoImpl::exit()
{
	std::printf("See you next time!\n");
}

void SKCommandHandlerVideoImpl::calculate()
{
	if (input == nullptr)
		return;
	TimeLogger tl;
	tl.s(0);
#ifndef PIC_MODE
	time_t timer = time(NULL);
	int frame_count = (int)cvGetCaptureProperty(input, CV_CAP_PROP_FRAME_COUNT);
	double fps = cvGetCaptureProperty(input, CV_CAP_PROP_FPS);
	if (frame_count == 0)	return;
	int frame_count_now = 0;
	IplImage *frame = cvQueryFrame(input);
	IplImage *myspace = cvCreateImage(cvSize(frame->width, frame->height), frame->depth, 1);
	CvPoint ans = cvPoint(-1, -1);
	int x = 0;
	//idis.display(&frame);
#ifdef OUTPUT_AVI
	CvVideoWriter* wrVideo1 = cvCreateVideoWriter("TEST.avi", CV_FOURCC('M', 'J', 'P', 'G'), fps, cvGetSize(frame));
#endif
	tl.r(0);
	while (1)
	{
		//获取每帧图像
		tl.s(1);
		frame = cvQueryFrame(input);
		if (frame == NULL)
			break;
#ifdef SKIP
		x = (x + 1) % SKIP;
		if (x != 0)
		{
			frame_count_now++;
			continue;
		}
#endif
		tl.r(1);
		//若上一帧有输出则使用带帧间信息的判断方式
		if (ans.x < 0 || ans.y < 0)
			ans = getquadrotor(frame, nullptr, &tl, myspace);
		else
			ans = getquadrotor(frame, &ans, &tl,myspace);
#ifdef OUTPUT_AVI
		if (ans.x > 0 && ans.y > 0)
			cvCircle(frame, ans, CIRCLE_RADIUS, CV_RGB(255, 0, 0),2);
		cvWriteFrame(wrVideo1, frame);
#endif
		frame_count_now++;
#ifdef SKIP
		if (frame_count_now * 100 / frame_count != (frame_count_now - SKIP) * 100 / frame_count)
			printf("%d%%\n", frame_count_now * 100 / frame_count);
#endif
	}
#ifdef OUTPUT_AVI
	cvReleaseVideoWriter(&wrVideo1);
#endif
#ifdef SKIP
	printf("%d seconds are used, total frames processed : ", (time(NULL) - timer));
	printf("%d\n", frame_count / SKIP);
#endif
	for (int i = 0; i < tl.get_num(); i++)
		printf("%lf\n", tl.get_n_elapsed(i));
#endif
	printf("\n");
}

CvPoint SKCommandHandlerVideoImpl::getquadrotor(IplImage *picc, CvPoint *LastPoint,TimeLogger *tl,IplImage *space)
{
	if (tl != nullptr)
		tl->s(2);
	CvPoint ret = cvPoint(-1, -1);
	if (picc == nullptr || (picc->nChannels != 3 && picc->nChannels != 1))
		return ret;
	//保护原图（如需要显示则应当保护原图）
#ifdef CLONE_IMAGE
	IplImage *pic = cvCloneImage(picc);
#else
	IplImage *pica = picc;
	IplImage *pic;
#endif
#ifndef USE_DoG
	//Step 1:灰度化
#ifndef CLONE_IMAGE
	if (pica->nChannels == 3)
	{
		IplImage *p;
		if (space == nullptr)
			p = cvCreateImage(cvSize(pica->width, pica->height), pica->depth, 1);
#else
	if (pic->nChannels == 3)
	{
		IplImage *p;
		if (space == nullptr)
			p = cvCreateImage(cvSize(pic->width, pic->height), pic->depth, 1);
#endif
		else
			p = space;
#ifndef CLONE_IMAGE
		cvCvtColor(pica, p, CV_BGR2GRAY);
#else
		cvCvtColor(pic, p, CV_BGR2GRAY);
#endif
#ifdef CLONE_IMAGE
		cvReleaseImage(&pic);
#endif
		pic = p;
	}
#ifndef CLONE_IMAGE
	else
		pic = pica;
#endif
#ifdef USE_ROI
	CvPoint Roi;
	if (LastPoint != nullptr)
	{
		Roi = getROI(cvSize(pic->width, pic->height), *LastPoint);
		if (Roi.x > -1 && Roi.y > -1)
		{
			IplImage *q = pic;
			//Mat tmpmat(pic);
			//Mat tmpmatroi(tmpmat, Rect(Roi.x, Roi.y, ROIsize, ROIsize));
			//IplImage tmpipl = ((IplImage)tmpmatroi);
			//pic = cvCloneImage(&tmpipl);
			cvSetImageROI(pic, cvRect(Roi.x, Roi.y, ROIsize, ROIsize));
			IplImage *pp = cvCreateImage(cvSize(ROIsize, ROIsize), pic->depth, pic->nChannels);
			cvCopy(pic, pp);
			pic = pp;
			if (space == nullptr)
				cvReleaseImage(&q);
			else
				cvResetImageROI(q);
		}
	}
#endif
	saveimage(pic, "S1.jpg");

	//Step 2:自适应二值化
	//TODO(_SHADOWK):应当改进二值化算法
	cvAdaptiveThreshold(pic, pic, 255, 0, CV_THRESH_BINARY_INV, 25);
	//Mat tmpm(pic);
	//medianBlur(tmpm, tmpm, 5);
	saveimage(pic, "S2.jpg");
	//Step 3:腐蚀膨胀一次
	//cvErode(pic, pic);
	//cvDilate(pic, pic);
	cvDilate(pic, pic);
#else
	pic = pica;
	Mat tmpm(pic);
	Mat_<Vec3b> tmpmre;
	DoG(tmpm, tmpmre);
#ifdef CLONE_IMAGE
	cvReleaseImage(&pic);
#endif
	IplImage pp = (Mat)tmpmre;
	pic = &pp;
	cvThreshold(pic, pic, 10, 255, CV_THRESH_BINARY);
	saveimage(pic, "S2.jpg");
	cvDilate(pic, pic);
#endif
	if (tl != nullptr)
		tl->r(2);
	if (tl != nullptr)
		tl->s(3);
	saveimage(pic, "S3.jpg");
	//Step 4:找出图像所有轮廓
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = NULL;
	IplImage *tmp = cvCloneImage(pic);//为了不破坏二值化的pic，需要做保护
	cvFindContours(tmp, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	cvReleaseImage(&tmp);
	CvSeq *pCurSeq = contours;
	CvSeq *pOldSeq = NULL;
	vector<SKResult> results;
#ifdef DRAW_SEQ
	IplImage* outlineImg = cvCreateImage(cvGetSize(pic), IPL_DEPTH_32F, 3);

	

#endif
	if (tl != nullptr)
		tl->r(3);
	//枚举每个轮廓
	while (pCurSeq)
	{
		if (tl != nullptr)
			tl->s(4);
		//删除掉面积比较小的轮廓
		double tmparea = fabs(cvContourArea(pCurSeq));
		if (tmparea < CROSS_RECT_SIZE)
		{
			pCurSeq = pCurSeq->h_next;
			continue;
		}
		//删除凸轮廓
		if (cvCheckContourConvexity(pCurSeq) == 1)
		{
			pCurSeq = pCurSeq->h_next;
			continue;
		}
		//Step 5:寻找小矩形与大矩形
		CvRect rect1 = cvBoundingRect(pCurSeq, 0);//大矩形
		CvBox2D rect2d = cvMinAreaRect2(pCurSeq);//小矩形（最小邻接）
		CvPoint2D32f rect4p[4];//最小邻接矩形的四个点
		cvBoxPoints(rect2d, rect4p);
		if (tl != nullptr)
			tl->r(4);
		if (tl != nullptr)
			tl->s(5);
		//Step 6: EvaluationA:小矩形的宽长比
		double a = evaluation_squre(rect4p);

		//Step 7: EvaluationB:轮廓内像素点在小矩形对角线上的集中程度
		double b = evaluation_cross(rect1, rect4p, 0.25, pic);

		//总的Evaluation
		double t = a * CROSS_THR_BALANCE + b * (1 - CROSS_THR_BALANCE);
#ifdef SHOW_POINT
		printf("SEQ_VALUE:%lf Center:(%d,%d)\n", t, (int)rect2d.center.x, (int)rect2d.center.y);
#endif

		//Step 8: 利用帧间信息（超过两倍DIFF_THR则为零，否则从0增益到1.5）
#ifndef PIC_MODE
		if (LastPoint != NULL)
		{
#ifdef USE_DoG
			double xt = (LastPoint->x) / 2.0;
			double yt = (LastPoint->y) / 2.0;
			double distance_dif = dot_to_dot(rect2d.center.x, rect2d.center.y, xt, yt);
#else
#ifdef USE_ROI
			double xt = (LastPoint->x) - Roi.x;
			double yt = (LastPoint->y) - Roi.y;
			double distance_dif = dot_to_dot(rect2d.center.x, rect2d.center.y, xt, yt);
#else
			double distance_dif = dot_to_dot(rect2d.center.x, rect2d.center.y, LastPoint->x, LastPoint->y);
#endif
#endif
			double weight = 1;
			if (distance_dif > 2 * DIFF_THR)
				weight = 0;
			else if (distance_dif > DIFF_THR)
				weight = ((double)distance_dif - DIFF_THR) / DIFF_THR;
			else if (distance_dif < (double)DIFF_THR / 2)
				weight = 1.5;
			t = t * weight;
		}
#endif
		//Step 9:超过阈值则置入Vector中
		if (t > CROSS_THR)
		{
			SKResult r;
			r.p = cvPoint(static_cast<int>(rect2d.center.x), static_cast<int>(rect2d.center.y));
			r.trust = t;
			results.push_back(r);
#ifdef DRAW_SEQ
			for (int i = 0; i < 4; i++)
				for (int j = i + 1; j < 4; j++)
					cvLine(outlineImg, cvPoint(rect4p[i].x, rect4p[i].y), cvPoint(rect4p[j].x, rect4p[j].y), CV_RGB(0, 0, 255));
			cvDrawContours(outlineImg, pCurSeq, CV_RGB(0, 255, 0), CV_RGB(0, 255, 0), 0, 2, CV_FILLED, cvPoint(0, 0));
			cvRectangleR(outlineImg, rect1, CV_RGB(255, 0, 0), 1, 8, 0);
#endif
		}
		pCurSeq = pCurSeq->h_next;
		if (tl != nullptr)
			tl->r(5);
	}
	cvReleaseMemStorage(&storage);
#ifndef USE_DoG
#ifndef USE_ROI
	if(space == nullptr)
		cvReleaseImage(&pic);
#else
#ifdef CLONE_IMAGE
	if (LastPoint != nullptr)
		cvReleaseImage(&pic);
#endif
#endif
#endif
	if (tl != nullptr)
		tl->s(6);
#ifdef DRAW_SEQ
	saveimage(outlineImg, "S10.jpg");
	cvReleaseImage(&outlineImg);
#endif
	//Step 10:找出Evaluation的最大值并输出
	if (results.size() == 0)
		return ret;
	int im = 0;
	for (unsigned int i = 0; i < results.size(); i++)
	{
		if (results[i].trust > results[im].trust)
			im = i;
	}
	ret = results[im].p;
#ifdef USE_DoG
	ret.x *= 2;
	ret.y *= 2;
#endif
#ifdef USE_ROI
	if (LastPoint != nullptr)
	{
		ret.x += Roi.x;
		ret.y += Roi.y;
	}
#endif
	if (tl != nullptr)
		tl->r(6);
	return ret;
}

CvPoint SKCommandHandlerVideoImpl::getROI(CvSize fullsz, CvPoint LastPoint)
{
	CvPoint ret = cvPoint(0,0);
	if (fullsz.height < ROIsize || fullsz.width < ROIsize)
		return cvPoint(-1, -1);
	if (LastPoint.x - ROIsize / 2 < 0)
		ret.x = 0;
	else if (LastPoint.x + ROIsize / 2 > fullsz.width)
		ret.x = fullsz.width - ROIsize;
	else
		ret.x = LastPoint.x - ROIsize / 2;

	if (LastPoint.y - ROIsize / 2 < 0)
		ret.y = 0;
	else if (LastPoint.y + ROIsize / 2 > fullsz.height)
		ret.y = fullsz.height - ROIsize;
	else
		ret.y = LastPoint.y - ROIsize / 2;
	return ret;
}

double SKCommandHandlerVideoImpl::dot_to_line(double p0x, double p0y, double pax, double pay, double pbx, double pby)
{
	double squre = abs(p0x*pay + pax*pby + pbx*p0y - p0x*pby - pax*p0y - pbx*pay);
	return squre / dot_to_dot(pax,pay,pbx,pby);
}
double SKCommandHandlerVideoImpl::dot_to_dot(double pax, double pay, double pbx, double pby)
{
	return sqrt(sqr(pax - pbx) + sqr(pay - pby));
}

double SKCommandHandlerVideoImpl::evaluation_squre(CvPoint2D32f *box)
{
	try
	{
		box[3];
	}
	catch (...)
	{
		return .0;
	}
	double l1 = dot_to_dot(box[0].x, box[0].y, box[1].x, box[1].y);
	double l2 = dot_to_dot(box[0].x, box[0].y, box[3].x, box[3].y);
	return min(l1, l2) / max(l1, l2);
}
double SKCommandHandlerVideoImpl::evaluation_cross(CvRect rect, CvPoint2D32f *box, double distance, IplImage *in)
{
	double diagonal = dot_to_dot(box[0].x, box[0].y, box[2].x, box[2].y);
	double threshold = 1.0 / 4 * diagonal * distance;
	double full = 0;
	for (int i = rect.y; i < min(rect.y + rect.height, in->height); i++)
		for (int j = rect.x; j < min(rect.x + rect.width, in->width); j++)
		{
			if (((uchar *)in->imageData)[i * in->widthStep / sizeof(uchar) + j] == 255)
			{
				double dis_cross = dot_to_line(j, i, box[0].x, box[0].y, box[2].x, box[2].y);
				dis_cross = min(dis_cross, dot_to_line(j, i, box[1].x, box[1].y, box[3].x, box[3].y));
				full = full + ((dis_cross < threshold) ? 1 : -1);
			}
		}
	full = full / (4 * diagonal * threshold);

	full *= 2;

	if (full > 1) full = 1;
	return (full < 0) ? 0 : full;
}
void SKCommandHandlerVideoImpl::DoG(Mat& src, Mat_<Vec3b>& dest)
{
	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);
	if (gray.cols > 1000)
		pyrDown(gray, gray, Size(gray.cols / 2, gray.rows / 2));

	Mat img_G0, img_G1;
	GaussianBlur(gray, img_G0, Size(3, 3), 0);
	GaussianBlur(img_G0, img_G1, Size(3, 3), 0);
	dest = img_G0 - img_G1;
	normalize(dest, dest, 0, 255, CV_MINMAX);
	medianBlur(dest, dest, 3);
	medianBlur(dest, dest, 3);
	medianBlur(dest, dest, 3);
}

void SKCommandHandlerVideoImpl::saveimage(const IplImage *p, const string s)
{
	if (p == nullptr)
		return;
#ifdef SAVE_STEP
	cvSaveImage(s.c_str(), p);
#endif
	return;
}
