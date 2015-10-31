//CommandHandler
//Do properthings when input a command.
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
using namespace std;

class CommandHandler
{
public:
	void begin();
	bool handle(string s);
	bool handle(char *s);
	CommandHandler();
	~CommandHandler();
private:
	class CommandHandlerImpl *_impl;
};