#ifndef PCL6045BL_DEFINE_H
#define PCL6045BL_DEFINE_H

//FOR H8
/******************** 地址 **********************/
#define	BUFW1	0x00000000	//写入缓冲器16-31位
#define	BUFW0	0x00000004	//写入缓冲器0-15位
#define OTPW	0x00000008	//设置通用输出状态
#define COMW	0x0000000C	//写指令到指定轴
#define MSTSW	0x0000000C	//读取主状态
#define SSTSW	0x00000008	//读取子状态及通用输入输出口

#define XADDR	0x00000000	//X轴地址
#define YADDR	0x00000010	//Y轴地址
#define ZADDR	0x00000020	//Z轴地址
#define UADDR	0x00000030	//U轴地址


/******************** 指令 **********************/
#define	XSEL	0x00000100	//选择X轴
#define YSEL	0x00000200	//选择Y轴
#define ZSEL	0x00000400	//选择Z轴
#define USEL	0x00000800	//选择U轴

#define STAFL	0x00000050
#define STAFH	0x00000051
#define STAD	0x00000052
#define STAUD	0x00000053

#define CNTFL	0x00000054
#define CNTFH	0x00000055
#define CNTD	0x00000056
#define CNTUD	0x00000057

#define CMSTA	0x00000006
#define SPSTA	0x0000002A

#define FCHGL	0x00000040
#define FCHGH	0x00000041
#define FSCHL	0x00000042
#define FSCHH	0x00000043

#define STOP	0x00000049
#define SDSTP	0x0000004A

#define CMSTP	0x00000007

#define CMEMG	0x00000005

#define NOP		0x00000000

#define P0RST	0x00000010
#define P1RST	0x00000011
#define P2RST	0x00000012
#define P3RST	0x00000013
#define P4RST	0x00000014
#define P5RST	0x00000015
#define P6RST	0x00000016
#define P7RST	0x00000017
#define P0SET	0x00000018
#define P1SET	0x00000019
#define P2SET	0x0000001A
#define P3SET	0x0000001B
#define P4SET	0x0000001C
#define P5SET	0x0000001D
#define P6SET	0x0000001E
#define P7SET	0x0000001F

#define SRST	0x00000004

#define CUN1R	0x00000020
#define CUN2R	0x00000021
#define CUN3R	0x00000022
#define CUN4R	0x00000023

#define ERCOUT	0x00000024
#define ERCRST	0x00000025

#define PRECAN	0x00000026
#define PCPCAN	0x00000027
#define PRESHF	0x0000002B
#define PCPSHF	0x0000002C
#define PRSET	0x0000004F

#define STAON	0x00000028

#define LTCH	0x00000029

#define RRMV	0x000000D0
#define WRMV	0x00000090
#define RPRMV	0x000000C0
#define WPRMV	0x00000080

#define RRFL	0x000000D1
#define WRFL	0x00000091
#define RPRFL	0x000000C1
#define WPRFL	0x00000081

#define RRFH	0x000000D2
#define WRFH	0x00000092
#define RPRFH	0x000000C2
#define WPRFH	0x00000082

#define RRUR	0x000000D3
#define WRUR	0x00000093
#define RPRUR	0x000000C3
#define WPRUR	0x00000083

#define RRDR	0x000000D4
#define WRDR	0x00000094
#define RPRDR	0x000000C4
#define WPRDR	0x00000084

#define RRMG	0x000000D5
#define WRMG	0x00000095
#define RPRMG	0x000000C5
#define WPRMG	0x00000085

#define RRDP	0x000000D6
#define WRDP	0x00000096
#define RPRDP	0x000000C6
#define WPRDP	0x00000086

#define RRMD	0x000000D7
#define WRMD	0x00000097
#define RPRMD	0x000000C7
#define WPRMD	0x00000087

#define RRIP	0x000000D8
#define WRIP	0x00000098
#define RPRIP	0x000000C8
#define WPRIP	0x00000088

#define RRUS	0x000000D9
#define WRUS	0x00000099
#define RPRUS	0x000000C9
#define WPRUS	0x00000089

#define RRDS	0x000000DA
#define WRDS	0x0000009A
#define RPRDS	0x000000CA
#define WPRDS	0x0000008A

#define RRFA	0x000000DB
#define WRFA	0x0000009B

#define RRENV1	0x000000DC
#define WRENV1	0x0000009C

#define RRENV2	0x000000DD
#define WRENV2	0x0000009D

#define RRENV3	0x000000DE
#define WRENV3	0x0000009E

#define RRENV4	0x000000DF
#define WRENV4	0x0000009F

#define RRENV5	0x000000E0
#define WRENV5	0x000000A0

#define RRENV6	0x000000E1
#define WRENV6	0x000000A1

#define RRENV7	0x000000E2
#define WRENV7	0x000000A2

#define RRCUN1	0x000000E3
#define WRCUN1	0x000000A3

#define RRCUN2	0x000000E4
#define WRCUN2	0x000000A4

#define RRCUN3	0x000000E5
#define WRCUN3	0x000000A5

#define RRCUN4	0x000000E6
#define WRCUN4	0x000000A6

#define RRCMP1	0x000000E7
#define WRCMP1	0x000000A7

#define RRCMP2	0x000000E8
#define WRCMP2	0x000000A8

#define RRCMP3	0x000000E9
#define WRCMP3	0x000000A9

#define RRCMP4	0x000000EA
#define WRCMP4	0x000000AA

#define RRCMP5	0x000000EB
#define WRCMP5	0x000000AB
#define RPRCMP5	0x000000CB
#define WPRCMP5	0x0000008B

#define RRIRQ	0x000000EC
#define WRIRQ	0x000000AC

#define RRLTC1	0x000000ED

#define RRLTC2	0x000000EE

#define RRLTC3	0x000000EF

#define RRLTC4	0x000000F0

#define RRSTS	0x000000F1

#define RREST	0x000000F2

#define RRIST	0x000000F3

#define RRPLS	0x000000F4

#define RRSPD	0x000000F5

#define RRSDC	0x000000F6

#define RRCI	0x000000FC
#define WRCI	0x000000BC
#define RPRCI	0x000000CC
#define WPRCI	0x0000008C

#define RRCIC	0x000000FD

#define RRIPS	0x000000FF


//7.运动的定义
#define CW 			0			//顺时针方向
#define CCW 		1			//逆时针方向
#define CWOring 	0x00001615	//顺时针回原点
#define CCWOring 	0x0000161D	//逆时针回原点
#define NormalMove 	0x00001641	//正常运动
#define LineRips 	0x00008061	//恒速度线性插补1
//#define LineRips 	0x00000061	//恒速度线性插补1
#define sRate 		299			// 速度倍率 X1,
#define ManualReduce  0X00002000	// 手动减速模式
// 即脉冲数 X 距离当量 = 即时速度，最大值65535 X 0.1约6.5米/秒
#endif