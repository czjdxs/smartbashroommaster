#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "mc32p7311.h"

#define TRUE  1
#define FAULT 0
#define ON 	  1
#define OFF   0
#define HIGH  1
#define LOW   0

#define FramHead		0XB4FD			//帧头

#define ZMKEY			0XFA			//照明
#define HQKEY			0XFB			//换气
#define SMKEY			0XFC			//智能关机 
#define QNKEY			0XFD			//取暖
#define SYSOFF			0XFF			//关闭
#define ZMON			0X9F			//照明开指令
#define SHZON			0XFE			//取暖，自然风，换气，智能开关开
#define	QNON			0XFD			//强暖
#define XYDON			0XF5			//小夜灯指令


#define DATAIN			P02D
#define DIG1			P04D
#define DIG2			P13D
#define	DIG3			P01D
#define SegA			P15D = TRUE
#define SegB			P14D = TRUE
#define SegC			P17D = TRUE
#define SegD			P11D = TRUE
#define SegE			P12D = TRUE
#define SegF			P03D = TRUE
#define SegG			P16D = TRUE
#define SegOFF()	{IOP0 &= 0XF7;IOP1 &= 0X09;}
#define DigOFF()	{IOP0 |= 0X12;IOP1 |= 0X08;}

#define Seg0 {SegA;SegB;SegC;SegD;SegE;SegF;}
#define Seg1 {SegB;SegC;}
#define Seg2 {SegA;SegB;SegG;SegE;SegD;}
#define Seg3 {SegA;SegB;SegC;SegD;SegG;}
#define Seg4 {SegB;SegC;SegF;SegG;}
#define Seg5 {SegA;SegC;SegD;SegF;SegG;}
#define Seg6 {SegA;SegC;SegD;SegF;SegG;SegE;}
#define Seg7 {SegA;SegB;SegC;}
#define Seg8 {SegA;SegB;SegC;SegD;SegE;SegF;SegG;}
#define Seg9 {SegA;SegB;SegC;SegD;SegF;SegG;}


typedef union
{
   	struct
   	{
   	   	uchar flg0:1;
   	   	uchar flg1:1;
   	   	uchar flg2:1;
   	   	uchar flg3:1;
   	   	uchar flg4:1;
   	   	uchar flg5:1;
   	   	uchar flg6:1;
   	   	uchar flg7:1;
   	}bits;
}flag;

flag flg;

#define sys_10ms_flag 		flg.bits.flg0   	   	//10ms标志位 	   	   	   	   	   	
#define AD_Convert_flg		flg.bits.flg1			//AD转换开始标志
#define sys_Colling_flg		flg.bits.flg3			//冷却标志
#define sys_waitting_flg	flg.bits.flg4			//待机状态标志
#define sys_start_flg		flg.bits.flg5

flag status;

#define sys_status_light			status.bits.flg0		//照明开启标志
#define sys_status_airchange		status.bits.flg1		//换气标志
#define sys_status_smartkey			status.bits.flg2		//智能关机
#define sys_status_warmkey			status.bits.flg3		//取暖标志
#define sys_status_natureair		status.bits.flg4		//自然风标志
#define sys_status_nithtflg			status.bits.flg5		//夜灯按键标志
#define sys_status_lithtflg			status.bits.flg6		//照明按键标志

void BoardInit();

#endif