#include"DroneCatcher.h"
#include<vector>
using namespace std;
using cv::Mat;
using cv::Rect;
using cv::RotatedRect;
using cv::Point;
#pragma warning(disable:4018)
DroneCatcher::SKResult DroneCatcher::GetDrone(Point last_point, cv::Mat image, int thr)
{
	bool has_last_point = true;
	Point roi_start = Point(-1, -1);
	//Mat part;
	vector<SKResult> result_list;
	if (last_point.x < 0 || last_point.y < 0)
	{
		has_last_point = false;
		part = image;
	}
	else
	{
		roi_start = getROI(cvSize(image.cols,image.rows), last_point);
		Rect roi_rect(roi_start.x, roi_start.y, ROI_WIDTH, ROI_HEIGHT);
		part = image(roi_rect);
	}
	image.copyTo(output);
	cv::cvtColor(part, gray, CV_BGR2GRAY);
	saveimage(gray, "gray.jpg");
	//cv::adaptiveThreshold(gray, gray, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 31, 5.0);
	cv::threshold(gray, gray, thr, 255, CV_THRESH_BINARY_INV);
	saveimage(gray, "gray_th.jpg");
	cv::dilate(gray, gray, NULL);
	saveimage(gray, "gray_th_dil.jpg");
	gray.copyTo(binary);
	
	vector<vector<Point>> contours;
	//vector<cv::Vec4i> hierarchy;
	cv::findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++)
	{
		double area = fabs(cv::contourArea(contours[i]));
		if (area <= CROSS_RECT_SIZE)
			continue;
		//Í¹°ü¼ì²â
		if (cv::isContourConvex(contours[i]))
			continue;
		Rect rect_A = cv::boundingRect(contours[i]);
		RotatedRect rect_B = cv::minAreaRect(contours[i]);
		cv::Point2f rect_B_p[4];
		rect_B.points(rect_B_p);
		double a_value = evaluation_squre(rect_B_p);
		double b_value = evaluation_cross(rect_A, rect_B_p);
		double t_value = a_value * CROSS_THR_BALANCE + b_value * (1 - CROSS_THR_BALANCE);
		if (has_last_point)
		{
			double distance_dif = dot_to_dot(rect_B.center.x, rect_B.center.y, (last_point.x - roi_start.x), (last_point.y - roi_start.y));
			double weight = 1;
			if (distance_dif > 2 * DIFF_THR)
				weight = 0;
			else if (distance_dif > DIFF_THR)
				weight = ((double)distance_dif - DIFF_THR) / DIFF_THR;
			else if (distance_dif < (double)DIFF_THR / 2)
				weight = 1.5;
			t_value = t_value * weight;
		}
		if (t_value > CROSS_THR)
		{
			SKResult new_skresult;
			new_skresult.point.x = (int)round(rect_B.center.x + roi_start.x);
			new_skresult.point.y = (int)round(rect_B.center.y + roi_start.y);
			new_skresult.trust = t_value;
			result_list.push_back(new_skresult);
#ifdef SAVE_IMG
			for (int z = 0; z < 4; z++)
				for (int j = z + 1; j < 4; j++)
					cv::line(output, rect_B_p[z], rect_B_p[j], cv::Scalar(0, 0, 255));
			cv::drawContours(output, contours, i, cv::Scalar(0, 255, 0));
			cv::rectangle(output, rect_A, cv::Scalar(255, 0, 0));
#endif
		}
		
	}
	saveimage(output, "final.jpg");
	SKResult ret;
	if (result_list.size() == 0)
		return ret;
	else
	{
		int im = 0;
		for (int i = 0; i < result_list.size(); i++)
			if (result_list[i].trust > result_list[im].trust)
				im = i;
		ret = result_list[im];
	}
	return ret;
}
#pragma warning(default:4018)
double DroneCatcher::evaluation_squre(cv::Point2f *box)
{
	double l1 = dot_to_dot(box[0].x, box[0].y, box[1].x, box[1].y);
	double l2 = dot_to_dot(box[0].x, box[0].y, box[3].x, box[3].y);
	return min(l1, l2) / max(l1, l2);
}

double DroneCatcher::evaluation_cross(cv::Rect rect, cv::Point2f *box, double distance)
{
	double diagonal = dot_to_dot(box[0].x, box[0].y, box[2].x, box[2].y);
	double threshold = 1.0 / 4 * diagonal * distance;
	double full = 0;
	for (int i = rect.y; i < min(rect.y + rect.height, binary.rows); i++)
		for (int j = rect.x; j < min(rect.x + rect.width, binary.cols); j++)
		{
			if (*(binary.data + binary.step[0] * i + binary.step[1] * j) == 255)
				//if (((uchar *)in->imageData)[i * in->widthStep / sizeof(uchar) + j] == 255)
			{
				double dis_cross = dot_to_line(j, i, box[0].x, box[0].y, box[2].x, box[2].y);
				dis_cross = min(dis_cross, dot_to_line(j, i, box[1].x, box[1].y, box[3].x, box[3].y));
				full = full + ((dis_cross < threshold) ? 1 : -1);
			}
		}
	full = full / (4 * diagonal * threshold);

	full *= 2;

	if (full > 1) full = 1;
	return (full < 0) ? 0 : full;
}


Point DroneCatcher::getROI(CvSize fullsz, Point LastPoint)
{
	Point ret = cvPoint(0, 0);
	if (fullsz.height < ROI_HEIGHT || fullsz.width < ROI_WIDTH)
		return cvPoint(-1, -1);
	if (LastPoint.x - ROI_WIDTH / 2 < 0)
		ret.x = 0;
	else if (LastPoint.x + ROI_WIDTH / 2 > fullsz.width)
		ret.x = fullsz.width - ROI_WIDTH;
	else
		ret.x = LastPoint.x - ROI_WIDTH / 2;

	if (LastPoint.y - ROI_HEIGHT / 2 < 0)
		ret.y = 0;
	else if (LastPoint.y + ROI_HEIGHT / 2 > fullsz.height)
		ret.y = fullsz.height - ROI_HEIGHT;
	else
		ret.y = LastPoint.y - ROI_HEIGHT / 2;
	return ret;
}

double DroneCatcher::dot_to_line(double p0x, double p0y, double pax, double pay, double pbx, double pby)
{
	double squre = abs(p0x*pay + pax*pby + pbx*p0y - p0x*pby - pax*p0y - pbx*pay);
	return squre / dot_to_dot(pax, pay, pbx, pby);
}
#define sqr(x) ((x)*(x))
double DroneCatcher::dot_to_dot(double pax, double pay, double pbx, double pby)
{
	return sqrt(sqr(pax - pbx) + sqr(pay - pby));
}

void DroneCatcher::saveimage(const cv::Mat p, const std::string s)
{
#ifdef SAVE_IMG
	cv::imwrite(s, p);
#endif
	return;
}