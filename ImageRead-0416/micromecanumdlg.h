// MicroMecanumDlg.h : header file
//
#include <map>
#include <string>
#include <iomanip.h>  
#include <fstream>
#include <iostream>
#include <math.h> 
#include <WinSock2.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ConnectThread.h"
#include "processthread.h"
using namespace std;


#if !defined(AFX_MICROMECANUMDLG_H__E737454A_05E1_43A2_94BF_E2972390E663__INCLUDED_)
#define AFX_MICROMECANUMDLG_H__E737454A_05E1_43A2_94BF_E2972390E663__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_PROCESS_MESSAGE WM_USER+101 
#define WM_COMMAND_MESSAGE WM_USER+102 
#define WM_CONNECT_SUCCESS_MESSAGE WM_USER+103 
#define WM_LOGIN_SUCCESS_MESSAGE WM_USER+104 


//----------qi---------------------
/*
// 枚举方向
enum direction{
	east, north, west, south
};
*/
// 地标数据结构
struct MARK {
	CString barCode;//编号
	double x;//单位毫米
	double y;//单位毫米
	double dir;//方向，沿着Y轴正向为0
};

// 校准用的数据结构
struct CHECKPOINT {
	double distance;	// 离Line起点的理论距离
	double deltax;		// 与地标的X方向（即与行走方向垂直的方向）偏差
	double deltay;		// 与地标的y方向（即与行走方向垂直的方向）偏差
	double deltaa;		// 与地标的方向偏差，相机实际观测得到，单位度
	double deltah;		// 与行进方向的偏移角，通过上一点及目标点坐标计算得到，偏右为正，偏左为负，单位：度
	double speed;		// 运行速度

	double adjustx;		// 最后一个实际校准step的起点x坐标
	double adjusty;		// 最后一个实际校准step的起点y坐标
};

//小车信息
struct IGVINFO {
	double curX, curY;	// 小车当前坐标，单位是毫米
	double preX, preY;	// 小车上一点(前进、后退前)坐标，单位是毫米
	double Speed;			// 小车行进速度
	double Heading;		// 小车车头朝向，0度为正北（Y轴方向），90度为正东（X轴方向）
	double Compass;		// 地磁感应方向，0度为正北（Y轴方向），90度为正东（X轴方向）

	double MoveDirection;	// 运动方向

	bool isFree;		// 是否空闲
	bool isMarked;		// 是否已校准
	CString MarkID;		// 所校准地标的编码 
	
};

//--------------------------------------- 

/////////////////////////////////////////////////////////////////////////////
// CMicroMecanumDlg dialog


class CMicroMecanumDlg : public CDialog
{
// Construction
public:
	CMicroMecanumDlg(CWnd* pParent = NULL);	// standard constructor
/*

	void Test(void);				// 测试临时使用

// mecanum 小车运动控制模块 ---------------------------------------------------
	#define Puls2Distance 0.09964739028125		// 定义脉冲到距离的换算常数
	#define TurnRound 3180						// 定义mecanum 小车原地旋转一周的距离，单位mm
	//#define RoundUnit 3180						// 定义mecanum 小车原地旋转一周的距离，单位mm

//	#define ARROWRED 0X000000FF
//	#define ARROWGRE 0X0000FF00
//	#define ARROWBLU 0X00FF0000

	#define ARROWBLK 0X00010101
	#define ARROWRED 0X000000FF
	#define ARROWGRE 0X0000FF00
	#define ARROWBLU 0X00FF0000
	#define ARROWWHT 0X00FFFFFF

	#define MotorPower		0X80
	#define PlatformPower	0X40
	#define CameraPower		0X20
	#define uPin			0X10

	#define Pin0	 0X01
	#define Pin1	 0X02
	#define Pin2	 0X04
	#define Pin3	 0X08
	#define Pin4	 0X10
	#define Pin5	 0X20
	#define Pin6	 0X40
	#define Pin7	 0X80

	#define GpioRead 1
	#define GpioWrite 0
	#define GpioHigh 1
	#define GpioLow 0

	PLX_DEVICE_KEY    m_DeviceKey;
	PLX_DEVICE_OBJECT m_Device;	

	ControlParemeter Control;		// 1.定义一个运动控制参数的结构体
	MotionParemeter Line[mTotal];	// 2.定义一组mecanum单段运动参数的结构体
	MotionParemeter Step[mTotal];	// 3.定义一组mecanum单步运动参数的结构体
	MapLine	CaliMap[mTotal];		// 4.定义一个地图校准专用参数的结构体
	VirtualPort pPort[mTotal];		// 虚拟端口专用数据结构
	VirtualPortPar VportPar;		// 虚拟端口操作专用数据结构	
	BOOL DeknockEnable;				// 允许防碰撞检测标志
	BOOL PlatMotionEnable;			// 允许平台运动标志
	//--------qi-----------------------------------------------
	int nMarks;						// 仓库中地标的总数量
	MARK* marks;					// 地标列表，存储地标
	void ReadMarkFile();			// 读取地标文件
	
	// 转向校准系数，1度偏差角对应的校正距离CaliDistance(毫米)占步长的比
	// 例如，如果步长是400mm，校正系数是1/40，则1度偏角的校正距离为10mm
	double RotateFactor;
	double RotateLimit;
	void ReadConfigFile();			// 读取配置文件

	int nCheckPoints;				// 在一条Line中所读取到的地标的数量 
	CHECKPOINT* CheckPoints;		// 在一条Line中所读取到的地标的校准信息
	MARK*	CheckMarks;				// 在一条Line中所读取到的地标的列表

	std::map <CString,MARK*> mapMarks;		// 地标编码与地标信息的映射	
	MARK* GetMarkInfo(CString barCode);		// 根据条码得到地标信息
	
	ofstream m_ofsLog2;						// 日志文件流，暂未用m_ofsLog0和1

	MARK* LineMarks;						// 一条要走的线路上所有的mark
	int   nLineMarks;						// LineMarks的数量
*/
	// 给定四个点的坐标，求小车需要旋转的角度
//	double GetRotateAngle(	double x1, double y1, double x2, double y2, 
//					double x3, double y3, double x4, double y4);

	// 给定第3、4点的坐标和车子的偏移角deltaa（针对运动方向的，偏右为正），求小车需要旋转的角度
//	double GetRotateAngle(double x3, double y3,double x4, double y4, double deltaa);

	// 查找sBar地标附近的中心地标(仅在并排的三个中找)，假如自身就是中心，则返回自身barCode.
//	CString FindCentralMark(CString sBar);

	// 根据当前读到的地标(及其偏移量)和中心地标，以及车头朝向，确定车子的移动参数
//	MotionParemeter GetCaliLine(MARK* pMark, MARK* pMarkCentral, double deltax, double deltay, double m_Heading);

	// 根据运动方向和目标距离返回一套运动参数（MotionParameter结构体）
//	MotionParemeter GetMotionParameters(int dir, int dis);

//	IGVINFO m_igvInfo;
//	double m_curX, m_curY;	// 小车当前坐标，单位是毫米
//	double m_preX, m_preY;	// 小车上一点(前进、后退前)坐标，单位是毫米
//	double m_markFoundLocation;	// 搜索地标时找到的地标的行进距离，记录车子走完这一段搜索路径后退回去对准地标

//	double m_Heading;	// 小车车头朝向，0度为正北（Y轴方向），90度为正东（X轴方向）
//	double m_Compass;	// 地磁感应方向，0度为正北（Y轴方向），90度为正东（X轴方向）

	// 根据旋转指令更新车头朝向，361逆时针转，362顺时针，转360度的angle值为3180
//	void UpdateHeading(INT32 cmd, INT32 angle);
	
	// 更新车辆当前坐标
//	void UpdateOwnPosition(int dis);
	// 检验小车当前位置，例如，是不是离上一个读到的标太远了，或者根据上两个标推算偏移太大了
	// 如果是，则小车紧急停车，且报警
//	void ValidatePosition();	
	// 读取条码,如果读取失败就返回空字符""
	CString ReadBar();
	// 判读一个条码的各位是否是0-9的数字
	BOOL CheckBarcode(CString bar);
/*
	#define MAX_NotFindMarkDistance 2500	//	最远的未发现地标的距离，超过了就紧急停车，小车报警
	#define MAX_NotFindMarkTimes370 5		// 最大的未发现地标的次数，超过了就启动地标搜索过程
	#define MAX_ReCheckMarkTimes370 3		// 确认地标（即在高容差范围内）的次数，达到这个次数了就认为地标已经校准了
	#define X_Y_TOLERANCE_LOW 0.5			// 地标校准时允许的x,y容差（低），单位毫米，小于该值则认为坐标精确
	#define ANGLE_TOLERANCE_LOW 0.5			// 地标校准时允许的角度容差（低），单位毫米，小于该值则认为角度精确
	#define X_Y_TOLERANCE_HIGH 1.0			// 地标校准时允许的x,y容差（高），单位毫米，小于该值则认为坐标基本精确，但需要确认几次
	#define ANGLE_TOLERANCE_HIGH 1.0		// 地标校准时允许的角度容差（高），单位毫米，小于该值则认为角度基本精确，但需要确认几次

	int m_iCountOfSearchRoutes;				// 局部搜索地标线路的条数
	int m_iCurrentSearchingRoute;			// 当前正在执行的地标搜索路径索引, -1表示未进行地标搜索（-1用于Ontimer()中某些操作是否执行，这个跟BarDecoder不太同步）
	bool m_isSearchingMarkFound;			// 执行地标搜索路径过程中地标是否找到
	bool m_isSearchingMark;					// 是否在搜索地标，用于控制BarDecoder()函数中的一些操作是否执行
	int  m_NotFindMarkTimes370;				// 执行370指令时未发现mark的次数
	double m_NotFindMarkDistance;			// 执行前进或后退时未读到校准地标的累计距离，暂未用
	// 执行一条静态校准指令时累计校准的次数（只有当位移偏差小于X_Y_TOLERANCE_HIGH，角度小于ANGLE_TOLERANCE_HIGH时才开始计数），
	// 每次结束静态校准后清零
	int m_CaliCount;
	*/
//	MotionParemeter SearchLines[50]/*[Num_SearchLines]*/;	// 局部搜索地标线路的数组，元素不会超过50

	// 读取地标搜索路径文件
//	void ReadSearchLines();
	
	// 计算搜索路径（共Num_SearchLines条）
//	void CalculateSearchLines();

//	BOOL Deknock(void);
	
	//------------end qi-------------------------------------------
	

	//--------pc---------------------------------------------------
			
//	CString m_IP;					// 定义IP	
//	CString m_Port;					// 定义端口
	char tmp[64];					// 时间存储变量
	std::string Temp_LastBarCodeid; // 记录上一个读到的BarCodeid
	std::string Temp_BarCodeID;		// 记录当前读到的BarCodeid
	CConnectSocketThread pConnectSocketThread;	// socket通讯连接服务器线程指针
	// CRcvMsgThread pRcvMsgThread;	// socket通讯接收消息线程指针

	void SendMessage2Server(char* msg);		// 定义发送消息的函数	
    void InitializationSoket();				// 初始化soket,同时开启线程接收消息
    int GetMacaddress( char* mac);			// 获取本机mac地址
	void byte2Hex(unsigned char bData, char hex[]);

	void SendIGVLoginMessage();		// socket连接成功时发送登录消息给服务端

	// 发送读码数据到后台
	void SendBarCodeInfo(int isSuc, CString barID, double x, double y, double angle);
	int m_iTimeCounter;				// 计数器，OnTimer每执行10次回传一次自身位置和状态信息	

//	void Command2Route(char* sMsg);		// 客户端收到消息后的处理；	

	//------------end pc-------------------------------------------

    
//	BOOL InitializationMotion(void);
//	void MotionInit();	//运动系统复位
//	CString DisplayPar(MotionParemeter* line);
//	CString DisplayControl(ControlParemeter* Control);
//	BOOL StartMotion(void);			// 开始运动
//	BOOL StopMotion(void);			// 停止运动	
//	BOOL Display(void);

//	BOOL MotorPowerOFF(void);		// 关闭电机电源
//	BOOL MotorPowerON(void);		// 打开电机电源
	
	// 箭头控制变量
	U32 mARROW;
	U32 FixedStep;
	CString DisFixInfo;
	BOOL DrawArrow(UINT32 CommandDir);// 绘画运动方向箭头
//	BOOL DeMotionError(DOUBLE *dR,DOUBLE *X,DOUBLE *Y,DOUBLE *L,DOUBLE *dL);	//运动误差修正计算公式
//	BOOL DeMachineError(DOUBLE &R,DOUBLE &D,DOUBLE &dR,DOUBLE &L,DOUBLE &dL);	//机械误差修正计算公式
//	BOOL FixTrack(DOUBLE &dR,DOUBLE &dX,DOUBLE &dY,DOUBLE &dA,ControlParemeter* Control,MotionParemeter* step);		//自动校准误差计算公式
//	BOOL DeAngleError(DOUBLE *dR,DOUBLE *X,DOUBLE *Y,DOUBLE *L,DOUBLE *dL);	//通过角度修正运动误差
//	DWORD RunLife(CString TimeUnit);	//统计程序运行时间
// mecanum 小车运动控制模块 ---------------------------------------------------

// ADO LIST 1 --------------------------------------------------------------------------------------
	#define	MovementParament		16					// 运动参数的个数
	_ConnectionPtr m_pConnection;						// 数据库连接变量
	_RecordsetPtr m_pRecordset;							// 数据库记录集变量
	CString TableName;									// 数据库表名
	
	BOOL InitializationWindows(void);					// 初始化显示界面		
//	void InitADOConn();									// 连接数据库
//	void ExitConnect();									// 断开数据库
//	void Database2List();								// 把数据库显示在列表控件上
//	void InitList();									// 初始化列表
//	void SelectRow(INT RowID);							// 选择列表控件某一列并显示	
//	void Database2Route(void);							// 把数据库数据转为路径数据
// ADO LIST 1 --------------------------------------------------------------------------------------

//------JHSN36BF 3 Evision 2---------------------------------------------------	
	//	BOOL CammeraOK;	
//	int DecoderCounter;
	#define ImageProcessSpace 2
	#define BarLenth 40
	#define BarCodeNumber 5
	CString SystemInfor;

	INT32 ImageProcessFrequence;
	BOOL FindBarEnable;
//	bool DecodeInfoOK;
	bool m_saveFlag;
	bool m_saveBMPFlag;
	bool m_saveJPGFlag;
	int m_index;
	int NumColor;
	int	VirticalCounter;
	int TotalDispFrame;
	int FramePerSecond;
	int DecodeSuccesTime;
	int DecodeFaileTime;
	double	BarXlocation;				// 条码中心X轴坐标
	double  BarYlocation;				// 条码中心Y轴坐标
	double  BarXlenth;					// 条码X坐标轴长度
	double  BarYlenth;					// 条码Y坐标轴长度
	double  BarAngle;					// 条码偏转角度	
	CComboBox *ResoComBox;
	CComboBox *NameComBox;
	CProcessThread pProcessThread;		//图像采集捕捉线程
	void CameraInitional();				//初始化相机参数
	void InitializationCamera();		//初始化相机参数new
	void BarDecoder();					//调用Evision解码
	
	//添加Evision专用变量和函数
	enum IMAGE_FILE_TYPES eFileFormat;  // 枚举的图像文件导入保存的类型；File type for load and save of images
	//EImageC24 EC24Image1;				// C24图像类变量；EImageC24 class instance
	//EImageC24 EC24Image2;				// C24图像类变量；EImageC24 class instance
	//EImageC24 C24Var;					// C24图像类变量；EImageC24 class instance	
	EImageBW8 EBW8Image2;				// BW8图像类变量；EImageBW8 class instance
	EImageBW8 m_Src;					// BW8图像类变量；EImageBW8 class instance
	//EImageBW8 ImageNull;				// BW8图像类变量；EImageBW8 class instance
	EBarCode m_BarCode;					// 条码识别类变量

	// 以下为变量用于模式识别（Match）
	EImageBW8 m_Pattern;				// BW8图像类变量，8位灰度图像；EImageBW8 class instance
	EROIBW8 m_Roi;						// pattern image
	EMatch m_Match;						// Match object for pattern matching
	
	CString	BarDecodeInfo;				// 条码识别部分显示解码信息用
	bool	FinishedBarDecode;			// 解码完成标志
	bool	CameraReady;				// 相机准备就绪标志
	bool	EnableDecoderBar;			// 允许读码
	bool	DisplayImage;				// 允许显示图像
	
	char m_pszDecodedText[256];			// 保存条码字符的一维字符型数组
	CString str,str1,str2;  			// 显示条码识别信息的字符串
	
//------JHSN36BF 3 Evision 2------------------------------------------------------------

// 串口通讯----------------------------------------------------------------------------------------
//	CMSComm ZjCom;
//	void InitRs232c(void);	//初始化串口 
// 串口通讯----------------------------------------------------------------------------------------

// Dialog Data
	//{{AFX_DATA(CMicroMecanumDlg)
	enum { IDD = IDD_MICROMECANUM_DIALOG };
	CStatic	m_ConnectStatus;
	CComboBox	m_HeadingCombBxo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMicroMecanumDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CMicroMecanumDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
//	afx_msg void OnOnCommMscomm1();
	afx_msg LRESULT ProcessImage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ProcessCommand(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ProcessConnectSuccess(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ProcessLoginSuccess(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangeCombo1();
//	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MICROMECANUMDLG_H__E737454A_05E1_43A2_94BF_E2972390E663__INCLUDED_)
