//SKCommandHandlerVideo
//Do properthings when input a command.(Specially for video input)
//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.

#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>
#include "SKDefinition.h"
using namespace std;

class SKCommandHandlerVideo
{
public:
	void begin();
	bool handle(string s);
	bool handle(char *s);
	SKCommandHandlerVideo();
	~SKCommandHandlerVideo();
	static CvPoint GetDrone(const IplImage *pic);
private:
	class SKCommandHandlerVideoImpl *_impl;
};