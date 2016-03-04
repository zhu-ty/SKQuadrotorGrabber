#include <iostream>
#include <string>
#include "SKNewDefinition.h"

#include "../SKQuadrotorGrabber/TimeLogger.h"
#include "DroneCatcher.h"
#include <highgui.h>
#include <cv.h>
#include <vector>
using namespace std;

int main()
{
	using cv::Mat;

	DroneCatcher drone_catcher;
	string str;
	getline(cin, str);
	Mat img;
#ifdef IMG_MODE
	img = cv::imread(str.c_str());
	DroneCatcher::SKResult sk_result = drone_catcher.GetDrone(cv::Point(-1, -1), img);
	printf("%d %d\n", sk_result.point.x, sk_result.point.y);
	img = cv::imread(str.c_str());
	cv::circle(img, sk_result.point, 30, cv::Scalar(0, 0, 0));
	cv::imwrite("F:\\z郑总\\Pic2015年10月31日\\33.jpg", img);
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
			printf("%d\%\n", (current * 100 / total));
		if (current >= total)
			break;
		current++;
	}
#endif
	return 0;
}


//F:\z郑总\Pic2015年10月31日\3.jpg
//F:\z郑总\测试集-2015-11-14-1-11\cut\11-13-下午3点42－涂白－郭嘉辰-1.mov