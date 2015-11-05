//SKQuadrotorGrabber
//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.

#include <string>
#include "SKDefinition.h"

#ifdef PIC_MODE
#include "SKCommandHandler.h"
#else
#include "SKCommandHandlerVideo.h"
#endif



int main()
{
#ifdef PIC_MODE
	SKCommandHandler cmd;
#else
	SKCommandHandlerVideo cmd;
#endif
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