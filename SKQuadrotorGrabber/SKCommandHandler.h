//SKCommandHandler
//Do properthings when input a command.
//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.

#pragma once

#include <iostream>
#include "SKDefinition.h"
#include "SKCommandHandlerVideo.h"
using namespace std;

class SKCommandHandler
{
public:
	void begin();
	bool handle(string s);
	bool handle(char *s);
	SKCommandHandler();
	~SKCommandHandler();
private:
	class SKCommandHandlerImpl *_impl;
};