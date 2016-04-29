// MotionVision.h : main header file for the MOTIONVISION DLL
//

#if !defined(AFX_MOTIONVISION_H__A5DA1C93_1B57_469D_84C0_83790F9B9EC3__INCLUDED_)
#define AFX_MOTIONVISION_H__A5DA1C93_1B57_469D_84C0_83790F9B9EC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

// #ifdef _MOTIONVISION_H
// #define _MOTIONVISION_H

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMotionVisionApp
// See MotionVision.cpp for the implementation of this class
//

#include "PCL6045BLdef.h"
#include "ConsFunc.h"
#include "PlxApi.h"
#include "PlxInit.h"

// oring
extern "C" bool _declspec(dllexport) DeviceOpen(PLX_DEVICE_KEY* key, PLX_DEVICE_OBJECT* device);
extern "C" bool _declspec(dllexport) DeviceClose(PLX_DEVICE_OBJECT* device);
extern "C" bool _declspec(dllexport) WriteCommand(PLX_DEVICE_OBJECT* device, U32 comm, U32 axisaddr=0x00000000, U32 axissel=0x00000000);
extern "C" bool _declspec(dllexport) ReadBuffer(PLX_DEVICE_OBJECT* device, U32 &buf0, U32 &buf1, U32 axisaddr);
extern "C" bool _declspec(dllexport) WriteBuffer(PLX_DEVICE_OBJECT* device, U32 buf0, U32 buf1, U32 axisaddr);
extern "C" bool _declspec(dllexport) ReadMSTSW(PLX_DEVICE_OBJECT* device, U32 &sts, U32 axisaddr);
extern "C" bool _declspec(dllexport) ReadSSTSW(PLX_DEVICE_OBJECT* device, U32 &sts, U32 axisaddr);
extern "C" bool _declspec(dllexport) WriteOTPW(PLX_DEVICE_OBJECT* device, U32 value, U32 axisaddr);
extern "C" bool _declspec(dllexport) WriteRegister(PLX_DEVICE_OBJECT* device, U32 comm, U32 buf0, U32 buf1, U32 axisaddr, U32 axissel=0x00000000);
extern "C" bool _declspec(dllexport) ReadRegister(PLX_DEVICE_OBJECT* device, U32 comm, U32 &buf0, U32 &buf1, U32 axisaddr);

//old
extern "C" void _declspec(dllexport)  InnitionalAxis(PLX_DEVICE_OBJECT* device);
extern "C" void _declspec(dllexport)  SetIO(PLX_DEVICE_OBJECT* device,U32 axis, U32 val);
extern "C" void _declspec(dllexport)  ClearAxis(PLX_DEVICE_OBJECT* device);
extern "C" void _declspec(dllexport)  StopAxis(PLX_DEVICE_OBJECT* device);
extern "C" void _declspec(dllexport)  RestAxis(PLX_DEVICE_OBJECT* device);
extern "C" void _declspec(dllexport) SetAxis(PLX_DEVICE_OBJECT* device, U32 axis,   U32 m_UintSh,  
                    U32 m_UintSl, U32 m_UintAcc, U32 m_UintDac, U32 m_GoPoSition0, U32 m_GoPoSition1);
extern "C" void _declspec(dllexport) StartMove(PLX_DEVICE_OBJECT* device);
extern "C" void _declspec(dllexport)  ReadAxis(PLX_DEVICE_OBJECT* device ,U32 axis, U32 reg,
											U32* pos0, U32* pos1);
// 张健-2015-08-18-新增修订
//new data type

union Data32to16{
	S32	S32Data;
	U16	U16Data[2];
	};

// mecanum小车运动参数设置专用数据结构
struct MotionParemeter{	
	INT32 Derection;		// 1.运动方向
	INT32 AidDistance;		// 2.目标距离
	INT32 MotionType;		// 3.运动类型
	INT32 SpeedRate;		// 4.速度倍率
	INT32 StartSpeed;		// 5.启动速度
	INT32 MaxSpeed;			// 6.最高速度
	INT32 ACC;				// 7.加加速度
	INT32 DACC;				// 8.减加速度
	INT32 ReducePoint;		// 9.减速距离
	INT32 StartType;		// 10.启动类型
	INT32 CaliDistance;		// 11.校正距离
	INT32 MicroDistance;	// 12.微步步长	
	INT32 AidX;				// 13.X分轴目标
	INT32 AidY;				// 14.Y分轴目标
	INT32 AidZ;				// 15.Z分轴目标
	INT32 AidU;				// 16.U分轴目标	
	};						

// mecanum小车运动控制专用数据结构
struct ControlParemeter{	
	UINT32 mStep;			// 1.单步运动执行步骤旗标
	UINT32 mExeStep;		// 2.已执行的步数	
	UINT32 mStepEnable;		// 3.允许单步运动旗标
	UINT32 mMaxStep;		// 4.一条路段包含的步数
	UINT32 mFixStep;		// 4A.正在校正的步数
	
	UINT32 mLine;			// 5.路段运动执行步骤旗标
	UINT32 mExeLine;		// 6.已执行的路段数
	UINT32 mLineEnable;		// 7.允许路径运动旗标
	UINT32 mMaxLine;		// 8.一条路径包含的路段数
	
	UINT32 mPause;			// 9.暂停路径运动旗标
	UINT32 mGoOn;			// 10.继续路径运动旗标
	UINT32 mRevers;			// 11.反向路径运动旗标
	UINT32 mData;			// 12.预留数据

	DOUBLE PlusUnit;		// 13,每脉冲换算到距离的值	
	DOUBLE xLocation;		// 14.X坐标
	DOUBLE yLocation;		// 15.Y坐标
	DOUBLE hAngle;			// 16.H航向(Z轴旋转角)
	
	DOUBLE RoundUnit;		// 17,每旋转一周换算到距离的值
	DOUBLE DeltX;			// 18.X坐标误差
	DOUBLE DeltY;			// 19.Y坐标误差
	DOUBLE DeltH;			// 20.H航向(Z轴旋转角)误差
};
							
// 地图校准专用数据结构
struct MapLine{	
	UINT32 mCaliStep;		// 1.单步运动执行步骤旗标
	DOUBLE xLocation;		// 2.X坐标
	DOUBLE yLocation;		// 3.Y坐标
	DOUBLE hAngle;			// 4.H航向(Z轴旋转角)
	
	UINT32 PortX;			// 5.X分轴目标
	UINT32 PortY;			// 6.Y分轴目标
	UINT32 PortZ;			// 7.Z分轴目标
	UINT32 PortU;			// 8.U分轴目标	
};

// 虚拟端口操作专用数据结构
struct VirtualPortPar{	
	UINT32 VportName;		// 1.虚拟端口名称
	UINT32 VpinNane;		// 2.虚拟引脚名称
	UINT32 VpinLevel;		// 3.虚拟引脚电平	
};

// 虚拟端口专用数据结构
struct VirtualPort{	
	INT32 Vpin0;			// 1.虚拟引脚0
	INT32 Vpin1;			// 1.虚拟引脚1
	INT32 Vpin2;			// 1.虚拟引脚2
	INT32 Vpin3;			// 1.虚拟引脚3
	INT32 Vpin4;			// 1.虚拟引脚4
	INT32 Vpin5;			// 1.虚拟引脚5
	INT32 Vpin6;			// 1.虚拟引脚6
	INT32 Vpin7;			// 1.虚拟引脚7

	INT32 Vpin8;			// 1.虚拟引脚8
	INT32 Vpin9;			// 1.虚拟引脚9
	INT32 Vpin10;			// 1.虚拟引脚10
	INT32 Vpin11;			// 1.虚拟引脚11
	INT32 Vpin12;			// 1.虚拟引脚12
	INT32 Vpin13;			// 1.虚拟引脚13
	INT32 Vpin14;			// 1.虚拟引脚14
	INT32 Vpin15;			// 1.虚拟引脚15
	
	INT32 Vpin16;			// 1.虚拟引脚16
	INT32 Vpin17;			// 1.虚拟引脚17
	INT32 Vpin18;			// 1.虚拟引脚18
	INT32 Vpin19;			// 1.虚拟引脚19
	INT32 Vpin20;			// 1.虚拟引脚20
	INT32 Vpin21;			// 1.虚拟引脚21
	INT32 Vpin22;			// 1.虚拟引脚22
	INT32 Vpin23;			// 1.虚拟引脚23
	
	INT32 Vpin24;			// 1.虚拟引脚24
	INT32 Vpin25;			// 1.虚拟引脚25
	INT32 Vpin26;			// 1.虚拟引脚26
	INT32 Vpin27;			// 1.虚拟引脚27
	INT32 Vpin28;			// 1.虚拟引脚28
	INT32 Vpin29;			// 1.虚拟引脚29
	INT32 Vpin30;			// 1.虚拟引脚30
	INT32 Vpin31;			// 1.虚拟引脚31
};								
#define PermiteErr 0.1			// 允许的校正误差
#define mTotal	256				// 定义mecanum运动参数数组的长度
// 	ControlParemeter Control;		// 1.定义一个运动控制参数的结构体
// 	MotionParemeter Line[mTotal];	// 2.定义一组mecanum单段运动参数的结构体
// 	MotionParemeter Step[mTotal];	// 3.定义一组mecanum单步运动参数的结构体
// 	MapLine	CaliMap[mTotal];		// 4.定义一个地图校准专用参数的结构体

// basic operation
extern "C" BOOL _declspec(dllexport) Init(PLX_DEVICE_OBJECT* device);
extern "C" BOOL _declspec(dllexport) Reset(PLX_DEVICE_OBJECT* device);
extern "C" BOOL _declspec(dllexport) Clean(PLX_DEVICE_OBJECT* device);
extern "C" BOOL _declspec(dllexport) Launch(PLX_DEVICE_OBJECT* device,U32 StartType);
extern "C" BOOL _declspec(dllexport) Stop(PLX_DEVICE_OBJECT* device,U32 StopType);
extern "C" BOOL _declspec(dllexport) SetSingleAxis(PLX_DEVICE_OBJECT* device,MotionParemeter* SinglePar,U32 AxisID);	// 设置单轴运动参数	
// State operation
extern "C" S32 _declspec(dllexport) GetShiftR(PLX_DEVICE_OBJECT* device,U32 axis);
extern "C" S32 _declspec(dllexport) GetSpeedR(PLX_DEVICE_OBJECT* device,U32 axis);
extern "C" U32 _declspec(dllexport) GetStockLevel(PLX_DEVICE_OBJECT* device,U32 AxisID);	// 读取堆栈状态
// I/O operation
#define GpioRead	1
#define GpioWrite	0
#define GpioHigh	1
#define GpioLow		0

#define Pin0	 0X01
#define Pin1	 0X02
#define Pin2	 0X04
#define Pin3	 0X08
#define Pin4	 0X10
#define Pin5	 0X20
#define Pin6	 0X40
#define Pin7	 0X80

//虚拟端口
#define PortMax	 32
#define PinMax	 32
//INT32 VirtualPort[PinMax][PortMax];
//BOOL PlatMotionEnable;
extern "C" U32 _declspec(dllexport) GetInput(PLX_DEVICE_OBJECT* device,U32 axis);			// 每轴的P0～P3为输入端口
extern "C" U32 _declspec(dllexport) SetOutput(PLX_DEVICE_OBJECT* device,U32 axis, U32 val);	// 每轴的P4～P7为输出端口0X00X0,电平/1->输出高电平
extern "C" U32 _declspec(dllexport) GpioOperation(PLX_DEVICE_OBJECT* device,BOOL Operation,U32 Port,U32 Pin,BOOL Level);//通用端口操作
extern "C" BOOL _declspec(dllexport) AutoLift(PLX_DEVICE_OBJECT* device,BOOL PlatMode);		// 平台自动升降 
// mecanum control
//extern "C" CString _declspec(dllexport) ParString(MotionParemeter* SinglePar);		// 生成运动参数字符串

// 减速距离的计算
extern "C" INT32 _declspec(dllexport) AceelerationShift(BOOL Stype,INT32 MinSpeed,INT32 MaxSpeed,INT32 Acc);
// 减速耗时的计算
extern "C" DOUBLE _declspec(dllexport) AceelerationTime(BOOL Stype,INT32 MinSpeed,INT32 MaxSpeed,INT32 Acc);

extern "C" BOOL _declspec(dllexport) SetMecanum(PLX_DEVICE_OBJECT* device,MotionParemeter* SinglePar);	// 设置mecanum各分轴运动参数
extern "C" BOOL _declspec(dllexport) MecanumGo(PLX_DEVICE_OBJECT* device,double Rate,MotionParemeter* SinglePar);	// 设置单步参数，并执行单步运动
extern "C" void _declspec(dllexport) mLineGo(PLX_DEVICE_OBJECT* device,ControlParemeter* pControl,
											 MotionParemeter* pLine,MotionParemeter* pStep,MapLine* pCaliMap);//以微步方式执行路段行走
extern "C" void _declspec(dllexport) mRouteGo(PLX_DEVICE_OBJECT* device,ControlParemeter* pControl,
											  MotionParemeter* pLine,MotionParemeter* pStep,
											  MapLine* pCaliMap,VirtualPort* pPort,VirtualPortPar* pVportPar);// mecanum 以分路段方式执行路径行走
extern "C" BOOL _declspec(dllexport) InitialPar(ControlParemeter* pControl,MotionParemeter* pLine,	
												MotionParemeter* pStep,MapLine* pCaliMap,VirtualPort* pPort,VirtualPortPar* pVportPar);	// 初始化轨迹控制参数

class CMotionVisionApp : public CWinApp
{
public:
	CMotionVisionApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMotionVisionApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMotionVisionApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTIONVISION_H__A5DA1C93_1B57_469D_84C0_83790F9B9EC3__INCLUDED_)
