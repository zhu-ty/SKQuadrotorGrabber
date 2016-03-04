/*B衡量参数，体现为相对于中心轴的距离*/
#define B_THR 0.25
/*核心衡量阈值*/
#define CROSS_THR 0.6
/*两类权衡算法占比*/
#define CROSS_THR_BALANCE 0.5
/*轮廓筛选大小阈值*/
#define CROSS_RECT_SIZE 100
/*帧间距离阈值*/
#define DIFF_THR 300
/*ROI大小*/
#define ROI_WIDTH 500
#define ROI_HEIGHT 500

/*主程序宏定义，为单张图片模式*/
//#define IMG_MODE
#ifndef IMG_MODE
/*主程序宏定义，为视频处理模式*/
#define VID_MODE
#endif