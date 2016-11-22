#include <iostream>
#include <string>
#include "SKNewDefinition.h"

#include "../SKQuadrotorGrabber/TimeLogger.h"
#include "DroneCatcher.h"
#include <highgui.h>
#include <cv.h>
#include <vector>
using namespace std;

int main(int argc, char** argv)
{
	using cv::Mat;

	DroneCatcher drone_catcher;
	string str;
	if (argc < 2)
		getline(cin, str);
	else
		str = argv[1];
	//str = "F:/zÖ£×Ü/2016-11-20/ÓãÑÛÍ¼Ïñ/ÓãÑÛÍ¼Ïñ/1.jpg";
	Mat img;
#ifdef IMG_MODE
	img = cv::imread(str.c_str());
	DroneCatcher::SKResult sk_result;
	if(argc > 2)
		sk_result = drone_catcher.GetDrone(cv::Point(-1, -1), img, atoi(argv[2]));
	else
		sk_result = drone_catcher.GetDrone(cv::Point(-1, -1), img);
	printf("%d %d\n", sk_result.point.x, sk_result.point.y);
	img = cv::imread(str.c_str());
	cv::circle(img, sk_result.point, 30, cv::Scalar(0, 0, 0));
	str = "out" + str;
	cv::imwrite(str, img);
	if(argc < 2)
		system("pause");
#endif
#ifdef VID_MODE
	cv::VideoCapture video_capture(str);
	long current = 0;
	long total = video_capture.get(CV_CAP_PROP_FRAME_COUNT);
	vector<cv::Point> ans;
	ans.reserve(total);
	DroneCatcher::SKResult last_result;
	while (true)
	{
		video_capture.read(img);
		last_result = drone_catcher.GetDrone(last_result.point, img);
		ans.push_back(last_result.point);
		if (current % 100 == 0)
			printf("%d%%\n", (current * 100 / total));
		if (current >= total)
			break;
		current++;
	}
#endif
	return 0;
}


//F:\zÖ£×Ü\Pic2015Äê10ÔÂ31ÈÕ\3.jpg
//F:\zÖ£×Ü\²âÊÔ¼¯-2015-11-14-1-11\cut\11-13-ÏÂÎç3µã42£­Í¿°×£­¹ù¼Î³½-1.mov