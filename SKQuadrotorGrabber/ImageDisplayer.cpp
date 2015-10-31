//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.

#pragma once

#include "ImageDisplayer.h"
#include <mutex>
#include <Windows.h>

using namespace std;
using namespace cv;

class ImageDisplayerImpl
{
public:
	struct Dthread
	{
		IplImage **p;
		bool stop;
		string name;
		mutex lock;
	};

	bool shown;
	HANDLE hThread;
	Dthread dth;
};

namespace thread
{
	DWORD WINAPI Displayimg(LPVOID lpParamter)
	{
		ImageDisplayerImpl::Dthread *dp = (ImageDisplayerImpl::Dthread *)lpParamter;
#ifdef RESIZE_PIC
		IplImage *old;
		IplImage *show = cvCreateImage(cvSize(ImageDisplayer::resize_width, ImageDisplayer::resize_height), (*(dp->p))->depth, (*(dp->p))->nChannels);
		old = nullptr;
#endif
		while (1)
		{
			try
			{
				IplImage *pth = *(dp->p);
#ifdef RESIZE_PIC
				if (old != pth || old->imageData != pth->imageData)
				{
					cvReleaseImage(&show);
					show = cvCreateImage(cvSize(ImageDisplayer::resize_width, ImageDisplayer::resize_height), (*(dp->p))->depth, (*(dp->p))->nChannels);
					cvResize(pth, show);
				}
				cvShowImage(dp->name.c_str(), show);
				cvResizeWindow(dp->name.c_str(),show->width,show->height);
				old = pth;
#else
				cvShowImage(dp->name.c_str(), pth);
				cvResizeWindow(dp->name.c_str(),pth->width,pth->height);
#endif
			}
			catch (...)
			{
			}
			cvWaitKey(100);
			dp->lock.lock();
			if (dp->stop == true)
				break;
			dp->lock.unlock();
		}
		dp->lock.unlock();
#ifdef RESIZE_PIC
		cvReleaseImage(&show);
#endif
		cvDestroyWindow(dp->name.c_str());
		return 0;
	}
}

ImageDisplayer::ImageDisplayer()
{
	_impl = new ImageDisplayerImpl();
	_impl->shown = false;
	_impl->hThread = nullptr;
	_impl->dth.name = "My image";
	_impl->dth.p = nullptr;
	_impl->dth.stop = false;
}

ImageDisplayer::~ImageDisplayer()
{
	delete _impl;
}

void ImageDisplayer::display(IplImage **p)
{
	display(p, "Image");
}

void ImageDisplayer::display(IplImage **p, const char *name)
{
	if (_impl->shown == false)
	{
		string c(name);
		_impl->dth.name = c;
		_impl->dth.p = p;
		_impl->dth.stop = false;
		_impl->hThread = CreateThread(NULL, 0, thread::Displayimg, &(_impl->dth), 0, NULL);
	}
	_impl->shown = true;
}

void ImageDisplayer::hide()
{
	if (_impl->shown == true)
	{
		_impl->dth.lock.lock();
		_impl->dth.stop = true;
		_impl->dth.lock.unlock();
	}
	_impl->shown = false;
}