//TimeLogger
//Log Time.
//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.11.15
//Use Ctrl+M,Ctrl+O to fold the code.

#pragma once

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <Windows.h>

class TimeLogger
{
private:
	static long gettime()
	{
		//clock_t clockaaa = clock();
		//long time_elapsed = long(clockaaa);
		//return time_elapsed;
		LARGE_INTEGER re;
		QueryPerformanceCounter(&re);
		return (long)re.QuadPart;
	}
public:
	static std::vector<unsigned int> timeelapsed;
	static std::vector<long> start;
	static void restart(int num)
	{
		timeelapsed.clear();
		start.clear();
		for (int i = 0; i < num; i++)
		{
			timeelapsed.push_back(0);
			start.push_back(0);
		}
	}
	static void starttimer(int c)
	{
		if (c >= start.size())
			return;
		start[c] = gettime();
		return;
	}
	static void recordtimer(int c)
	{
		if (c >= start.size())
			return;
		timeelapsed[c] += (gettime() - start[c]);
	}
	static long Frequency()
	{
		LARGE_INTEGER re;
		QueryPerformanceFrequency(&re);
		return re.QuadPart;
	}
};

std::vector<unsigned int> TimeLogger::timeelapsed;
std::vector<long> TimeLogger::start;