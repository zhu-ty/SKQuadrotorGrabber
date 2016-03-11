//SKCommandHandlerVideo
//Do properthings when input a command.(Specially for video input)
//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.

#pragma once


#include "SKDefinition.h"
#include <iostream>
#include <cv.h>
#include "TimeLogger.h"
using namespace std;

class SKCommandHandlerVideo
{
public:
	void begin();
	bool handle(string s);
	bool handle(char *s);
	SKCommandHandlerVideo();
	~SKCommandHandlerVideo();
	static CvPoint GetDrone(IplImage *pic);
#ifndef ROC_DEBUG_MODE
private:
#endif
	class SKCommandHandlerVideoImpl *_impl;
	static CvPoint getquadrotor(IplImage *picc, CvPoint *LastPoint = nullptr, TimeLogger *tl = nullptr, IplImage *space = nullptr, vector<Evaluation_Data> *vec_data = nullptr);
};
