//SKQuadrotorGrabber
//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.

#include <string>
#include "CommandHandler.h"

int main()
{
	CommandHandler cmd;
	while (1)
	{
		string str;
		cmd.begin();
		getline(cin, str);
		if (cmd.handle(str))
			break;
	}
	return 0;
}