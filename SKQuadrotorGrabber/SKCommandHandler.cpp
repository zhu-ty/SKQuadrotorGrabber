//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.

#pragma once

#include "SKCommandHandler.h"
#include "SKImageDisplayer.h"
#include <vector>
#include <string>

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


	IplImage *input;
	cv::Mat inputmat;
	SKImageDisplayer idis;
	const int threshold = 230;
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
	//Check input and turn it into gray.
	if (input == nullptr || (input->nChannels != 3 && input->nChannels != 1))
		return;
	if (input->nChannels == 3)
	{
		IplImage *p = cvCreateImage(cvSize(input->width, input->height), input->depth, 1);
		cvCvtColor(input, p, CV_BGR2GRAY);
		IplImage *q;
		q = input;
		input = p;
		cvReleaseImage(&q);
	}

	//Binaryzation
	//cvThreshold(input, input, threshold, 255, CV_THRESH_BINARY);
	cvAdaptiveThreshold(input, input, 255, 0, 0, 25);

	inputmat = cv::Mat(input);
	cv::Mat temp;
	cv::Canny(inputmat, temp, 50, 200);
	
	vector<cv::Vec4i> lines;
	cv::HoughLinesP(temp, lines, 1, CV_PI / 180, 80, 50, 10);
	IplImage *p, *q;
	p = cvCreateImage(cvSize(input->width, input->height), input->depth, 3);
	cvSet(p, CV_RGB(255, 255, 255));
	q = input;
	cv::Mat temp2 = cv::Mat(p);
	for (unsigned int i = 0; i < lines.size(); i++)
		cv::line(temp2, cv::Point(lines[i][0], lines[i][1]), cv::Point(lines[i][2], lines[i][3]), CV_RGB(0, 0, 0));
	input = p;
	cvReleaseImage(&q);

	inputmat = cv::Mat(input);
	printf("\n");
}

