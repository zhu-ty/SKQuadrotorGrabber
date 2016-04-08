//SKDefinition
//Includes every important definition.
//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.
#pragma once
/*从摄像头读取数据并测试帧率，直接写在了主函数中*/
//#define CAMERA_DELAY_MODE

/*绘画ROC曲线时需要的情况，直接从主函数读取*/
//#define ROC_DEBUG_MODE

#ifndef CAMERA_DELAY_MODE
//#define PIC_MODE
#endif
#ifdef PIC_MODE
/*核心衡量阈值*/
#define CROSS_THR 0.7
/*保存中间步骤（仅限处理图片时）*/
#define SAVE_STEP
#ifdef SAVE_STEP
/*保存轮廓筛选结果*/
#define DRAW_SEQ
/*强制重置即时显示框大小*/
//#define RESIZE_PIC
#endif
#else
/*核心衡量阈值*/
#define CROSS_THR 0.63
/*跳帧 每N帧中处理1帧*/
#define SKIP 10
/*帧间距离阈值*/
#define DIFF_THR 300
/*使用ROI，兴趣区域检测算法*/
#define USE_ROI

#define OUTPUT_AVI
#endif

#ifndef USE_ROI
/*使用DoG算法代替自适应二值化算法*/
#define USE_DoG
#endif
/*两类权衡算法占比*/
#define CROSS_THR_BALANCE 0.5
/*轮廓筛选大小阈值*/
#define CROSS_RECT_SIZE 200

#ifndef CAMERA_DELAY_MODE
/*打印中间结果*/
//#define SHOW_POINT
/*复制待处理图片*/
//#define CLONE_IMAGE
/*输出视频结果*/
//#define OUTPUT_AVI
#endif

/*输出结果红色圆圈的大小*/
#define CIRCLE_RADIUS 10


struct Evaluation_Data
{
	Evaluation_Data(double _evaluation_a, double _evaluation_b, double _distance, double _t_value,int _x,int _y)
	{
		evaluation_a = _evaluation_a;
		evaluation_b = _evaluation_b;
		distance = _distance;
		t_value = _t_value;
		point_x = _x;
		point_y = _y;
	}
	double evaluation_a, evaluation_b, distance, t_value;
	int point_x, point_y;
};
