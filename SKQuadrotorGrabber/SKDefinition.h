//SKDefinition
//Includes every important definition.
//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.



/*从摄像头读取数据并测试帧率，直接写在了主函数中*/
//#define CAMERA_DELAY_MODE

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
#define CROSS_THR 0.7
/*跳帧 每N帧中处理1帧*/
#define SKIP 1
/*帧间距离阈值*/
#define DIFF_THR 300
/**/
#endif

/*使用DoG算法代替自适应二值化算法*/
#define USE_DoG
/*两类权衡算法占比*/
#define CROSS_THR_BALANCE 0.5
/*轮廓筛选大小阈值*/
#define CROSS_RECT_SIZE 100

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


//TODOList:
// 部分错误帧的处理（帧编号的输出）
// 有效结果的帧内输出
// 填补DoG算法中心空白
// 只处理感兴趣区域
// _SHADOWK