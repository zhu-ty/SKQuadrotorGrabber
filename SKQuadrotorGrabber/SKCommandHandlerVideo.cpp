//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.

#pragma once

#include "SKCommandHandlerVideo.h"
#include "SKImageDisplayer.h"
#include <vector>
#include <string>

#define sqr(x) ((x)*(x))

class SKCommandHandlerVideoImpl
{
public:
	//Compare two strings.(Case insensitive)
	bool compare(const string& x, const string& y);
	//For split string.
	vector<string> split(string str, string pattern);

	void help();
	void load(string s);
	//void save(string s);
	void exit();

	void calculate();

	double dot_to_line(double p0x, double p0y, double pax, double pay, double pbx, double pby);
	double dot_to_dot(double pax, double pay, double pbx, double pby);

	//Bigger the better.
	/*box must have 4 points*/
	double evaluation_squre(CvPoint2D32f *box);
	/*distance must be 0 ~ 1*/
	double evaluation_cross(CvRect rect, CvPoint2D32f *box, double distance, IplImage *in);

	CvPoint getquadrotor(const IplImage *picc, CvPoint *LastPoint = NULL);

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
			_impl->load("F:\\360Downloads\\3.jpg");
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
	printf("See you next time!\n");
}

void SKCommandHandlerVideoImpl::calculate()
{
	if (input == nullptr)
		return;
	int frame_count = (int)cvGetCaptureProperty(input, CV_CAP_PROP_FRAME_COUNT);
	double fps = cvGetCaptureProperty(input, CV_CAP_PROP_FPS);
	if (frame_count == 0)	return;
	int frame_count_now = 0;
	IplImage *frame = cvQueryFrame(input);
	CvPoint ans = cvPoint(-1, -1);

	//idis.display(&frame);
	CvVideoWriter* wrVideo1 = cvCreateVideoWriter("TEST.avi", CV_FOURCC('M', 'J', 'P', 'G'), fps, cvGetSize(frame));
	while (1)
	{
		frame = cvQueryFrame(input);
		if (frame == NULL)
			break;
		if (ans.x < 0 || ans.y < 0)
			ans = getquadrotor(frame);
		else
			ans = getquadrotor(frame, &ans);
		if (ans.x > 0 && ans.y > 0)
			cvCircle(frame, ans, CIRCLE_RADIUS, CV_RGB(255, 0, 0),2);
		cvWriteFrame(wrVideo1, frame);
		//cvWaitKey((double)1000 / fps);
	}
	cvReleaseVideoWriter(&wrVideo1);
	//idis.hide();
	printf("\n");
}

CvPoint SKCommandHandlerVideoImpl::getquadrotor(const IplImage *picc, CvPoint *LastPoint)
{
	
	CvPoint ret = cvPoint(-1, -1);
	if (picc == nullptr || (picc->nChannels != 3 && picc->nChannels != 1))
		return ret;
	IplImage *pic = cvCloneImage(picc);
	if (pic->nChannels == 3)
	{
		IplImage *p = cvCreateImage(cvSize(pic->width, pic->height), pic->depth, 1);
		cvCvtColor(pic, p, CV_BGR2GRAY);
		cvReleaseImage(&pic);
		pic = p;
	}
	cvAdaptiveThreshold(pic, pic, 255, 0, CV_THRESH_BINARY_INV, 25);
	cvDilate(pic, pic);
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = NULL;
	cvFindContours(pic, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	CvSeq *pCurSeq = contours;
	CvSeq *pOldSeq = NULL;
	vector<SKResult> results;
	while (pCurSeq)
	{
		double tmparea = fabs(cvContourArea(pCurSeq));
		if (tmparea < CROSS_RECT_SIZE)//删除掉面积比较小的轮廓
		{
			pCurSeq = pCurSeq->h_next;
			continue;
		}
		if (cvCheckContourConvexity(pCurSeq) == 1)//凸轮廓
		{
			pCurSeq = pCurSeq->h_next;
			continue;
		}
		CvRect rect1 = cvBoundingRect(pCurSeq, 0);
		CvBox2D rect2d = cvMinAreaRect2(pCurSeq);
		CvPoint2D32f rect4p[4];
		cvBoxPoints(rect2d, rect4p);
		double a = evaluation_squre(rect4p);
		double b = evaluation_cross(rect1, rect4p, 0.25, pic);
		double t = a * CROSS_THR_BALANCE + b * (1 - CROSS_THR_BALANCE);
		printf("SEQ_VALUE:%lf Center:(%d,%d)\n", t, (int)rect2d.center.x, (int)rect2d.center.y);
		if (LastPoint != NULL)
		{
			double distance_dif = dot_to_dot(rect2d.center.x, rect2d.center.y, LastPoint->x, LastPoint->y);
			double weight = 1;
			if (distance_dif > 2 * DIFF_THR)
				weight = 0;
			else if (distance_dif > DIFF_THR)
				weight = ((double)distance_dif - DIFF_THR) / DIFF_THR;
			else if (distance_dif < (double)DIFF_THR / 2)
				weight = 1.5;
			t = t * weight;
		}
		if (t > CROSS_THR)
		{
			SKResult r;
			r.p = cvPoint(static_cast<int>(rect2d.center.x), static_cast<int>(rect2d.center.y));
			r.trust = t;
			results.push_back(r);
		}
		pCurSeq = pCurSeq->h_next;
	}
	cvReleaseImage(&pic);
	if (results.size() == 0)
		return ret;
	int im = 0;
	for (int i = 0; i < results.size(); i++)
	{
		if (results[i].trust > results[im].trust)
			im = i;
	}
	ret = results[im].p;
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