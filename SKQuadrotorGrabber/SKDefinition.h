//SKDefinition
//Includes every important definition.
//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.



/*������ͷ��ȡ���ݲ�����֡�ʣ�ֱ��д������������*/
//#define CAMERA_DELAY_MODE

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
#define CROSS_THR 0.7
/*��֡ ÿN֡�д���1֡*/
#define SKIP 1
/*֡�������ֵ*/
#define DIFF_THR 300
/**/
#endif

/*ʹ��DoG�㷨��������Ӧ��ֵ���㷨*/
#define USE_DoG
/*����Ȩ���㷨ռ��*/
#define CROSS_THR_BALANCE 0.5
/*����ɸѡ��С��ֵ*/
#define CROSS_RECT_SIZE 100

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


//TODOList:
// ���ִ���֡�Ĵ���֡��ŵ������
// ��Ч�����֡�����
// �DoG�㷨���Ŀհ�
// ֻ�������Ȥ����
// _SHADOWK