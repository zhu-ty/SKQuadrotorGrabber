#include "SKNewDefinition.h"
#include <iostream>
#include <cv.h>
using cv::Point;
class DroneCatcher
{
public:
	struct SKResult
	{
		Point point;
		double trust;
		//Ĭ�ϳ�ʼx��y��С����
		SKResult(int _x = -1, int _y = -1, double _trust = 0)
		{
			point.x = _x;
			point.y = _y;
			trust = _trust;
		}
	};
	//��ͼƬ������������������һ�εĵ㣨��û����x��yС���㣩�ͱ�֡ͼƬ��Ϣ
	SKResult GetDrone(Point last_point, cv::Mat image);
private:
	cv::Mat gray;
	cv::Mat binary;
	cv::Mat part;
	Point getROI(CvSize full_size, Point LastPoint);
	double evaluation_squre(cv::Point2f *rotated_rect_points);
	double evaluation_cross(cv::Rect rect, cv::Point2f *rotated_rect_points, double distance = B_THR);
	double dot_to_line(double p0x, double p0y, double pax, double pay, double pbx, double pby);
	double dot_to_dot(double pax, double pay, double pbx, double pby);
};