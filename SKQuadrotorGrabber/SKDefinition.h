//SKDefinition
//Includes every important definition.
//Author:ShadowK
//email:zhu.shadowk@gmail.com
//2015.10.31
//Use Ctrl+M,Ctrl+O to fold the code.

//#define RESIZE_PIC
#define PIC_MODE

#ifdef PIC_MODE
#define CROSS_THR 0.3
#else
#define CROSS_THR 0.7
#endif

#define CROSS_THR_BALANCE 0.5
#define CROSS_RECT_SIZE 300

#define DIFF_THR 30

#define CIRCLE_RADIUS 10

//#define SHOW_POINT
#define CLONE_IMAGE
#define USE_DoG

#define SKIP 4

#define SAVE_STEP
#ifdef SAVE_STEP
#define DRAW_SEQ
#endif