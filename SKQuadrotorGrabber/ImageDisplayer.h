//ImageDisplayer
//Display a image.
//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.

#pragma once

#include <iostream>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#define RESIZE_PIC
using namespace std;


class ImageDisplayer
{
public:
	const static int resize_height = 500;
	const static int resize_width = 500;
	ImageDisplayer();
	~ImageDisplayer();
	void display(IplImage **p);
	void display(IplImage **p, const char *name);
	void hide();
private:
	class ImageDisplayerImpl *_impl;
};