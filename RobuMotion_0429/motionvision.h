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
// �Ž�-2015-08-18-�����޶�
//new data type

union Data32to16{
	S32	S32Data;
	U16	U16Data[2];
	};

// mecanumС���˶���������ר�����ݽṹ
struct MotionParemeter{	
	INT32 Derection;		// 1.�˶�����
	INT32 AidDistance;		// 2.Ŀ�����
	INT32 MotionType;		// 3.�˶�����
	INT32 SpeedRate;		// 4.�ٶȱ���
	INT32 StartSpeed;		// 5.�����ٶ�
	INT32 MaxSpeed;			// 6.����ٶ�
	INT32 ACC;				// 7.�Ӽ��ٶ�
	INT32 DACC;				// 8.�����ٶ�
	INT32 ReducePoint;		// 9.���پ���
	INT32 StartType;		// 10.��������
	INT32 CaliDistance;		// 11.У������
	INT32 MicroDistance;	// 12.΢������	
	INT32 AidX;				// 13.X����Ŀ��
	INT32 AidY;				// 14.Y����Ŀ��
	INT32 AidZ;				// 15.Z����Ŀ��
	INT32 AidU;				// 16.U����Ŀ��	
	};						

// mecanumС���˶�����ר�����ݽṹ
struct ControlParemeter{	
	UINT32 mStep;			// 1.�����˶�ִ�в������
	UINT32 mExeStep;		// 2.��ִ�еĲ���	
	UINT32 mStepEnable;		// 3.�������˶����
	UINT32 mMaxStep;		// 4.һ��·�ΰ����Ĳ���
	UINT32 mFixStep;		// 4A.����У���Ĳ���
	
	UINT32 mLine;			// 5.·���˶�ִ�в������
	UINT32 mExeLine;		// 6.��ִ�е�·����
	UINT32 mLineEnable;		// 7.����·���˶����
	UINT32 mMaxLine;		// 8.һ��·��������·����
	
	UINT32 mPause;			// 9.��ͣ·���˶����
	UINT32 mGoOn;			// 10.����·���˶����
	UINT32 mRevers;			// 11.����·���˶����
	UINT32 mData;			// 12.Ԥ������

	DOUBLE PlusUnit;		// 13,ÿ���廻�㵽�����ֵ	
	DOUBLE xLocation;		// 14.X����
	DOUBLE yLocation;		// 15.Y����
	DOUBLE hAngle;			// 16.H����(Z����ת��)
	
	DOUBLE RoundUnit;		// 17,ÿ��תһ�ܻ��㵽�����ֵ
	DOUBLE DeltX;			// 18.X�������
	DOUBLE DeltY;			// 19.Y�������
	DOUBLE DeltH;			// 20.H����(Z����ת��)���
};
							
// ��ͼУ׼ר�����ݽṹ
struct MapLine{	
	UINT32 mCaliStep;		// 1.�����˶�ִ�в������
	DOUBLE xLocation;		// 2.X����
	DOUBLE yLocation;		// 3.Y����
	DOUBLE hAngle;			// 4.H����(Z����ת��)
	
	UINT32 PortX;			// 5.X����Ŀ��
	UINT32 PortY;			// 6.Y����Ŀ��
	UINT32 PortZ;			// 7.Z����Ŀ��
	UINT32 PortU;			// 8.U����Ŀ��	
};

// ����˿ڲ���ר�����ݽṹ
struct VirtualPortPar{	
	UINT32 VportName;		// 1.����˿�����
	UINT32 VpinNane;		// 2.������������
	UINT32 VpinLevel;		// 3.�������ŵ�ƽ	
};

// ����˿�ר�����ݽṹ
struct VirtualPort{	
	INT32 Vpin0;			// 1.��������0
	INT32 Vpin1;			// 1.��������1
	INT32 Vpin2;			// 1.��������2
	INT32 Vpin3;			// 1.��������3
	INT32 Vpin4;			// 1.��������4
	INT32 Vpin5;			// 1.��������5
	INT32 Vpin6;			// 1.��������6
	INT32 Vpin7;			// 1.��������7

	INT32 Vpin8;			// 1.��������8
	INT32 Vpin9;			// 1.��������9
	INT32 Vpin10;			// 1.��������10
	INT32 Vpin11;			// 1.��������11
	INT32 Vpin12;			// 1.��������12
	INT32 Vpin13;			// 1.��������13
	INT32 Vpin14;			// 1.��������14
	INT32 Vpin15;			// 1.��������15
	
	INT32 Vpin16;			// 1.��������16
	INT32 Vpin17;			// 1.��������17
	INT32 Vpin18;			// 1.��������18
	INT32 Vpin19;			// 1.��������19
	INT32 Vpin20;			// 1.��������20
	INT32 Vpin21;			// 1.��������21
	INT32 Vpin22;			// 1.��������22
	INT32 Vpin23;			// 1.��������23
	
	INT32 Vpin24;			// 1.��������24
	INT32 Vpin25;			// 1.��������25
	INT32 Vpin26;			// 1.��������26
	INT32 Vpin27;			// 1.��������27
	INT32 Vpin28;			// 1.��������28
	INT32 Vpin29;			// 1.��������29
	INT32 Vpin30;			// 1.��������30
	INT32 Vpin31;			// 1.��������31
};								
#define PermiteErr 0.1			// �����У�����
#define mTotal	256				// ����mecanum�˶���������ĳ���
// 	ControlParemeter Control;		// 1.����һ���˶����Ʋ����Ľṹ��
// 	MotionParemeter Line[mTotal];	// 2.����һ��mecanum�����˶������Ľṹ��
// 	MotionParemeter Step[mTotal];	// 3.����һ��mecanum�����˶������Ľṹ��
// 	MapLine	CaliMap[mTotal];		// 4.����һ����ͼУ׼ר�ò����Ľṹ��

// basic operation
extern "C" BOOL _declspec(dllexport) Init(PLX_DEVICE_OBJECT* device);
extern "C" BOOL _declspec(dllexport) Reset(PLX_DEVICE_OBJECT* device);
extern "C" BOOL _declspec(dllexport) Clean(PLX_DEVICE_OBJECT* device);
extern "C" BOOL _declspec(dllexport) Launch(PLX_DEVICE_OBJECT* device,U32 StartType);
extern "C" BOOL _declspec(dllexport) Stop(PLX_DEVICE_OBJECT* device,U32 StopType);
extern "C" BOOL _declspec(dllexport) SetSingleAxis(PLX_DEVICE_OBJECT* device,MotionParemeter* SinglePar,U32 AxisID);	// ���õ����˶�����	
// State operation
extern "C" S32 _declspec(dllexport) GetShiftR(PLX_DEVICE_OBJECT* device,U32 axis);
extern "C" S32 _declspec(dllexport) GetSpeedR(PLX_DEVICE_OBJECT* device,U32 axis);
extern "C" U32 _declspec(dllexport) GetStockLevel(PLX_DEVICE_OBJECT* device,U32 AxisID);	// ��ȡ��ջ״̬
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

//����˿�
#define PortMax	 32
#define PinMax	 32
//INT32 VirtualPort[PinMax][PortMax];
//BOOL PlatMotionEnable;
extern "C" U32 _declspec(dllexport) GetInput(PLX_DEVICE_OBJECT* device,U32 axis);			// ÿ���P0��P3Ϊ����˿�
extern "C" U32 _declspec(dllexport) SetOutput(PLX_DEVICE_OBJECT* device,U32 axis, U32 val);	// ÿ���P4��P7Ϊ����˿�0X00X0,��ƽ/1->����ߵ�ƽ
extern "C" U32 _declspec(dllexport) GpioOperation(PLX_DEVICE_OBJECT* device,BOOL Operation,U32 Port,U32 Pin,BOOL Level);//ͨ�ö˿ڲ���
extern "C" BOOL _declspec(dllexport) AutoLift(PLX_DEVICE_OBJECT* device,BOOL PlatMode);		// ƽ̨�Զ����� 
// mecanum control
//extern "C" CString _declspec(dllexport) ParString(MotionParemeter* SinglePar);		// �����˶������ַ���

// ���پ���ļ���
extern "C" INT32 _declspec(dllexport) AceelerationShift(BOOL Stype,INT32 MinSpeed,INT32 MaxSpeed,INT32 Acc);
// ���ٺ�ʱ�ļ���
extern "C" DOUBLE _declspec(dllexport) AceelerationTime(BOOL Stype,INT32 MinSpeed,INT32 MaxSpeed,INT32 Acc);

extern "C" BOOL _declspec(dllexport) SetMecanum(PLX_DEVICE_OBJECT* device,MotionParemeter* SinglePar);	// ����mecanum�������˶�����
extern "C" BOOL _declspec(dllexport) MecanumGo(PLX_DEVICE_OBJECT* device,double Rate,MotionParemeter* SinglePar);	// ���õ�����������ִ�е����˶�
extern "C" void _declspec(dllexport) mLineGo(PLX_DEVICE_OBJECT* device,ControlParemeter* pControl,
											 MotionParemeter* pLine,MotionParemeter* pStep,MapLine* pCaliMap);//��΢����ʽִ��·������
extern "C" void _declspec(dllexport) mRouteGo(PLX_DEVICE_OBJECT* device,ControlParemeter* pControl,
											  MotionParemeter* pLine,MotionParemeter* pStep,
											  MapLine* pCaliMap,VirtualPort* pPort,VirtualPortPar* pVportPar);// mecanum �Է�·�η�ʽִ��·������
extern "C" BOOL _declspec(dllexport) InitialPar(ControlParemeter* pControl,MotionParemeter* pLine,	
												MotionParemeter* pStep,MapLine* pCaliMap,VirtualPort* pPort,VirtualPortPar* pVportPar);	// ��ʼ���켣���Ʋ���

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
