//SKDefinition
//Includes every important definition.
//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.


#define PIC_MODE

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
#define SKIP 30
/*֡�������ֵ*/
#define DIFF_THR 300
/*�����Ƶ���*/
#define OUTPUT_AVI
#endif

/*ʹ��DoG�㷨��������Ӧ��ֵ���㷨*/
//#define USE_DoG
/*����Ȩ���㷨ռ��*/
#define CROSS_THR_BALANCE 0.5
/*����ɸѡ��С��ֵ*/
#define CROSS_RECT_SIZE 100
/*��������ɫԲȦ�Ĵ�С*/
#define CIRCLE_RADIUS 10

/*��ӡ�м���*/
//#define SHOW_POINT
/*���ƴ�����ͼƬ*/
#define CLONE_IMAGE


//TODOList:
// ��ʱ����ϸ����
// ���ִ���֡�Ĵ���֡��ŵ������
// ��Ч�����֡�����
// �DoG�㷨���Ŀհ�
// _SHADOWK