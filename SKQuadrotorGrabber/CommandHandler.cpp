//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.

#pragma once

#include "CommandHandler.h"
#include "ImageDisplayer.h"
#include <vector>
#include <string>
#define maxbottle 50

class CommandHandlerImpl
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

	void togray();
	void binaryzation();
	void erode();
	void dilate();
	void count();

	bool hasnear(uchar num, int x, int y);
	void setwindow(IplImage *p, uchar num, int x, int y);
	int searchnear(int x, int y, int llable, uchar *lable, int *lower, int *xsum);

	IplImage *input = nullptr;
	ImageDisplayer idis;
	const int threshold = 230;
	const int thresholdwhite = 500;
	const int windowsize = 5;
	const int thresholdlow = 35;
	int whites;
	int fulls1,fulls2;
	uchar emptybottles1[maxbottle];
	uchar emptybottles2[maxbottle];
};

CommandHandler::CommandHandler()
{
	_impl = new CommandHandlerImpl();
	memset(_impl->emptybottles1, 0, sizeof(_impl->emptybottles1));
	memset(_impl->emptybottles2, 0, sizeof(_impl->emptybottles2));
}
CommandHandler::~CommandHandler()
{
	delete _impl;
}
void CommandHandler::begin()
{
	printf("Input command.Input \"help\" for command help.\n$");
}
bool CommandHandler::handle(char *s)
{
	string sr(s);
	return handle(sr);
}
bool CommandHandler::handle(string s)
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
		else if (_impl->compare(splitstr[0], "count"))
		{
			printf("Calculating...\n");
			_impl->calculate();
		}
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

bool CommandHandlerImpl::compare(const string& x, const string& y)
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
vector<string> CommandHandlerImpl::split(string str, string pattern)
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
void CommandHandlerImpl::help()
{
	printf("\"load <FileName>\" Load or reload a image with full location.\n");
	printf("Sample: load C:\\Users\\WIN_20150404_144352.JPG\n");
	printf("\"display\"Display the image.\n");
	printf("Sameple: display\n");
	printf("\"hide\"Stop displaying the image.\n");
	printf("Sameple: hide\n");
	printf("\"save <FileName>\"Save the image.Input the Name without the path to save it at the location of the program.\n");
	printf("Sameple: save C:\\Users\\WIN_20150404_144352.JPG\n");
	printf("\"count\"Count the bottles\n");
	printf("Sameple: count\n");
	printf("\"exit\"Exit the program\n");
	printf("Sameple: exit\n");
	printf("WARNING:Do not use image which has spaces in its FileName.\n");

}
void CommandHandlerImpl::load(string s)
{
	input = cvLoadImage(s.c_str(), 1);
	if(input == nullptr)
		printf("Wrong FileName.\n");
	else
		printf("Load successful.\n");
}
void CommandHandlerImpl::save(string s)
{
	if (input == nullptr)
	{
		printf("You haven't load an image yet!\n");
	}
	cvSaveImage(s.c_str(), input);
	printf("Save successful.\n");
}
void CommandHandlerImpl::display(string s)
{
	if(input != nullptr)
		idis.display(&input,s.c_str());
}
void CommandHandlerImpl::hide()
{
	idis.hide();
}
void CommandHandlerImpl::exit()
{
	hide();
	printf("See you next time!\n");
}
void CommandHandlerImpl::calculate()
{
	if (input == nullptr)
		return;
	//hide();
	//cvWaitKey(1000);
	togray();
	binaryzation();
	erode();
	dilate();
	count();

	printf("Counted by area size:\n");
	printf("Bottles:%d,Full bottles:%d\n", whites, fulls1);
	printf("From left to right, not filled bottles are:(1~%d)\n",whites);
	for (int i = 0; i < maxbottle; i++)
		(emptybottles1[i] == 1) ? printf("%d ", i + 1) : 1;
	printf("\n");
	printf("Counted by area minimum height:\n");
	printf("Bottles:%d,Full bottles:%d\n", whites, fulls2);
	printf("From left to right, not filled bottles are:(1~%d)\n", whites);
	for (int i = 0; i < maxbottle; i++)
		(emptybottles2[i] == 1) ? printf("%d ", i + 1) : 1;
	printf("\n");
}

void CommandHandlerImpl::togray()
{
	if (input->nChannels == 3)
	{
		IplImage *p, *q;
		q = input;
		p = cvCreateImage(cvGetSize(input), input->depth, 1);
		for (int i = 0; i < input->height; i++)
		{
			uchar *pixel = (uchar *)(input->imageData + i * input->widthStep);
			uchar *pixelp = (uchar *)(p->imageData + i * p->widthStep);
			for (int j = 0; j < input->width; j++)
				pixelp[j] = (uchar)(0.072169 * pixel[3 * j] + 0.715160 * pixel[3 * j + 1] + 0.212671 * pixel[3 * j + 2]);
		}
		input = p;
		cvReleaseImage(&q);
	}
}
void CommandHandlerImpl::binaryzation()
{
	for (int i = 0; i < input->height; i++)
	{
		uchar *pixel = (uchar *)(input->imageData + i * input->widthStep);
		for (int j = 0; j < input->width; j++)
			pixel[j] = (uchar)(pixel[j] > threshold ? 255 : 0);
	}
}
bool CommandHandlerImpl::hasnear(uchar num,int x,int y)
{
	if (x > 0 && (((uchar *)(input->imageData + y * input->widthStep))[x - 1] == num))
		return true;
	else if (x < input->width - 1 && (((uchar *)(input->imageData + y * input->widthStep))[x + 1] == num))
		return true;
	else if (y > 0 && (((uchar *)(input->imageData + (y - 1) * input->widthStep))[x] == num))
		return true;
	else if (y < input->height - 1 && (((uchar *)(input->imageData + (y + 1) * input->widthStep))[x] == num))
		return true;
	return false;
}
void CommandHandlerImpl::setwindow(IplImage *p,uchar num, int x, int y)
{
	int c = windowsize / 2;
	for (int i = y - c; i < y + c + 1; i++)
		for (int j = x - c; j < x + c + 1; j++)
			if (i >= 0 && i < p->height && j >= 0 && j < p->width)
				((uchar *)(p->imageData + i * p->widthStep))[j] = num;
}
void CommandHandlerImpl::erode()
{
	IplImage *p, *q;
	p = cvCloneImage(input);
	q = input;
	for (int i = 0; i < input->height; i++)
	{
		uchar *pixel = (uchar *)(input->imageData + i * input->widthStep);
		uchar *pixelp = (uchar *)(p->imageData + i * p->widthStep);
		for (int j = 0; j < input->width; j++)
		{
			if (pixel[j] == 0 && hasnear(255, j, i))
				setwindow(p, 0, j, i);
		}
	}
	input = p;
	cvReleaseImage(&q);
}
void CommandHandlerImpl::dilate()
{
	IplImage *p, *q;
	p = cvCloneImage(input);
	q = input;
	for (int i = 0; i < input->height; i++)
	{
		uchar *pixel = (uchar *)(input->imageData + i * input->widthStep);
		uchar *pixelp = (uchar *)(p->imageData + i * p->widthStep);
		for (int j = 0; j < input->width; j++)
		{
			if (pixel[j] == 255 && hasnear(0, j, i))
				setwindow(p, 255, j, i);
		}
	}
	input = p;
	cvReleaseImage(&q);
}
void CommandHandlerImpl::count()
{
	int ext = input->width * input->height;
	int llable = 1;
	uchar *lable = new uchar[ext];
	int *size = new int[maxbottle];
	int *low = new int[maxbottle];
	int *xsum = new int[maxbottle];
	memset(size, 0, maxbottle * sizeof(int));
	memset(low, 0, maxbottle * sizeof(int));
	memset(xsum, 0, maxbottle * sizeof(int));
	memset(lable, 0, ext * sizeof(uchar));
	for (int i = 0; i < input->height; i++)
	{
		uchar *pixel = (uchar *)(input->imageData + i * input->widthStep);
		for (int j = 0; j < input->width; j++)
		{
			if (lable[i * input->height + j] == 0 && pixel[j] == 255)
			{
				size[llable - 1] += searchnear(j, i, llable, lable, &low[llable - 1], &xsum[llable - 1]);
				llable++;
			}
		}
	}
	whites = llable - 1;
	fulls1 = whites;
	fulls2 = whites;
	double xaverage[maxbottle];
	for (int i = 0; i < llable; i++)
		xaverage[i] = (double)xsum[i] / size[i];
	for (int i = 0; i < llable; i++)
		if (size[i] > thresholdwhite)
		{
			int k = 0;
			for (int j = 0; j < llable; j++)
				if (xaverage[i] > xaverage[j]) k++;
			emptybottles1[k] = 1;
			fulls1--;
		}
	for (int i = 0; i < llable; i++)
		if (low[i] > thresholdlow)
		{
			int k = 0;
			for (int j = 0; j < llable; j++)
				if (xaverage[i] > xaverage[j]) k++;
			emptybottles2[k] = 1;
			fulls2--;
		}
	delete low;
	delete lable;
	delete size;
	delete xsum;
}
int CommandHandlerImpl::searchnear(int x, int y, int llable, uchar *lable, int *lower, int *xsum)
{
	int sum = 0;
	if (y >= 0 && y < input->height && x >= 0 && x < input->width && ((uchar *)(input->imageData + y * input->widthStep))[x] == 255 && lable[y*input->height + x] == 0)
	{
		if (y > *lower)	*lower = y;
		lable[y*input->height + x] = llable;
		*xsum += x;
		sum++;
		sum += searchnear(x - 1, y - 1, llable, lable, lower, xsum);
		sum += searchnear(x + 0, y - 1, llable, lable, lower, xsum);
		sum += searchnear(x + 1, y - 1, llable, lable, lower, xsum);
		sum += searchnear(x - 1, y + 0, llable, lable, lower, xsum);
		sum += searchnear(x + 1, y + 0, llable, lable, lower, xsum);
		sum += searchnear(x - 1, y + 1, llable, lable, lower, xsum);
		sum += searchnear(x + 0, y + 1, llable, lable, lower, xsum);
		sum += searchnear(x + 1, y + 1, llable, lable, lower, xsum);
	}
	return sum;
}

#undef maxbottle