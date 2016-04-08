//SKDefinition
//Includes every important definition.
//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.
#pragma once
/*������ͷ��ȡ���ݲ�����֡�ʣ�ֱ��д������������*/
//#define CAMERA_DELAY_MODE

/*�滭ROC����ʱ��Ҫ�������ֱ�Ӵ���������ȡ*/
//#define ROC_DEBUG_MODE

#ifndef CAMERA_DELAY_MODE
//#define PIC_MODE
#endif
#ifdef PIC_MODE
/*���ĺ�����ֵ*/
#define CROSS_THR 0.7
/*�����м䲽�裨���޴���ͼƬʱ��*/
#define SAVE_STEP
#ifdef SAVE_STEP
/*��������ɸѡ���*/
#define DRAW_SEQ
/*ǿ�����ü�ʱ��ʾ���С*/
//#define RESIZE_PIC
#endif
#else
/*���ĺ�����ֵ*/
#define CROSS_THR 0.63
/*��֡ ÿN֡�д���1֡*/
#define SKIP 10
/*֡�������ֵ*/
#define DIFF_THR 300
/*ʹ��ROI����Ȥ�������㷨*/
#define USE_ROI

#define OUTPUT_AVI
#endif

#ifndef USE_ROI
/*ʹ��DoG�㷨��������Ӧ��ֵ���㷨*/
#define USE_DoG
#endif
/*����Ȩ���㷨ռ��*/
#define CROSS_THR_BALANCE 0.5
/*����ɸѡ��С��ֵ*/
#define CROSS_RECT_SIZE 200

#ifndef CAMERA_DELAY_MODE
/*��ӡ�м���*/
//#define SHOW_POINT
/*���ƴ�����ͼƬ*/
//#define CLONE_IMAGE
/*�����Ƶ���*/
//#define OUTPUT_AVI
#endif

/*��������ɫԲȦ�Ĵ�С*/
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
