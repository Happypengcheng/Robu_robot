// MicroMecanumDlg.cpp : implementation file
//

#include "stdafx.h"
#include <sstream>
#include <string>
#include <WinSock2.h>
#include "ConnectThread.h"
#include "MicroMecanum.h"
#include "MicroMecanumDlg.h"
#include "Route.h"
#include "Com.h"
#include "json.h"
#include <time.h> 
#include <stdio.h>
#include <windows.h>
#include <stdio.h> 
#include <stdlib.h>
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMicroMecanumDlg dialog

CMicroMecanumDlg::CMicroMecanumDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMicroMecanumDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMicroMecanumDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMicroMecanumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMicroMecanumDlg)
	DDX_Control(pDX, IDC_STATIC_Connect, m_ConnectStatus);
	DDX_Control(pDX, IDC_COMBO1, m_HeadingCombBxo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMicroMecanumDlg, CDialog)
	//{{AFX_MSG_MAP(CMicroMecanumDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_WM_TIMER()
	ON_MESSAGE(WM_PROCESS_MESSAGE,ProcessImage)
	ON_MESSAGE(WM_CONNECT_SUCCESS_MESSAGE,ProcessConnectSuccess)
	ON_MESSAGE(WM_LOGIN_SUCCESS_MESSAGE,ProcessLoginSuccess)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMicroMecanumDlg message handlers

// motion control--------------------------------------------------------------
void CMicroMecanumDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//DrawArrow(mARROW);
		CDialog::OnPaint();
	}
}

HCURSOR CMicroMecanumDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMicroMecanumDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

#define SystemControlBlock 0
//system control--------------------------------------------------------------------------------------

BOOL SystemShutdown()
{		
	HANDLE hToken;		
	TOKEN_PRIVILEGES tkp;
	
	if (!OpenProcessToken(GetCurrentProcess(),				// OpenProcessToken() 这个函数的作用是打开一个进程的访问令牌			
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,&hToken))		// GetCurrentProcess() 函数的作用是得到本进程的句柄			
		return FALSE;	
	LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);		//LookupPrivilegeValue()的作用是查看系统权限的特权值，返回信息到一个LUID结构体里。		
	tkp.PrivilegeCount = 1;									// one privilege to set 赋给本进程特权		
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;			
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,(PTOKEN_PRIVILEGES)NULL, 0);	// AdjustTokenPrivileges（）的作用是通知Windows NT修改本进程的权利			
	if (GetLastError() != ERROR_SUCCESS)					//失败			
		return FALSE;		
	if (!ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0))		//参数在这里设置。强行退出WINDOWS（EWX_FORCE）。			
		return FALSE;	
	return TRUE;		
}

BOOL CMicroMecanumDlg::OnInitDialog()	//界面初始化
{
	CDialog::OnInitDialog();

	// for 测试用
	//double a = GetRotateAngle(-49,2587,0,4176,0.91,false);
	//UpdateHeading(361,795);
	//CheckBarcode("1234d");
//	CString sss = "123a";
//	LPTSTR ccc = sss.GetBuffer(0);


	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here


 	InitializationWindows();		// 初始化界面	

	//------------------------------------------------
	// qi
	// 初始化校准点（针对一条Line），最多支持1024个

	
	//m_Heading = -1.0;	// 小车初始未定位和定向
	m_HeadingCombBxo.SetCurSel(3);
	m_HeadingCombBxo.UpdateData(TRUE);

	// end qi 
	//------------------------------------------------	
		

	//InitializationCamera();		// 初始化相机
	CameraInitional();			// 初始化相机

	//------------------------------------------------
	// pc
	//以下初始化图像匹配组件
	m_Pattern.Load("pattern.bmp");	// 加载模板
	m_Roi.Attach(&m_Pattern);
	m_Roi.SetPlacement(0,0,268,44);
	m_Match.SetMaxPositions(1);// return only one position
	m_Match.SetMinScore(0.25f);	// 匹配度至少达到0.3
	ESetAngleUnit(E_ANGLE_UNIT_DEGREES);
	//m_Match.SetMaxAngle(1);
	//m_Match.SetMinAngle(-1);
	//m_Match.SetInterpolate(true);
	//m_Match.SetMaxScale(2);
	//m_Match.SetMinScale(0.5);
	m_Match.LearnPattern(&m_Roi);
	
	Temp_LastBarCodeid = "";	// 初始化为空
	m_isSocketConnected = false;	
	// GetDlgItem(IDC_STATIC_Connect)->SetWindowText("未连接");
	m_ConnectStatus.SetWindowText("未连接");

	InitializationSoket();	// 初始化soket同时开启线程接收消息
	m_iTimeCounter = 0;		// 计数器，OnTimer每执行10次回传一次自身位置和状态信息
	
	// end pc 
	//------------------------------------------------ 

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CMicroMecanumDlg::OnButton1() //紧急停止/关闭电源
{

	CString str;
	GetDlgItem(IDC_BUTTON1)->GetWindowText(str);
	if (str == "关闭电源")
	{
		GetDlgItem(IDC_BUTTON1)->SetWindowText("打开电源");
	} 
	else
	{
		GetDlgItem(IDC_BUTTON1)->SetWindowText("关闭电源");
	}
}

void CMicroMecanumDlg::OnButton2()	//系统复位
{
}

void CMicroMecanumDlg::OnButton3()	//使用说明 
{
	// 测试
	//UpdateHeading(361,795);

	CAboutDlg Dlg;
	Dlg.DoModal();	
}

void CMicroMecanumDlg::OnButton4()	//路径编辑 
{
}

void CMicroMecanumDlg::OnButton5()	//通讯设定
{
}

void CMicroMecanumDlg::OnButton6()	//坐标复位(静态校准)  
{

}

void CMicroMecanumDlg::OnButton7()	//坐标设定
{

}

void CMicroMecanumDlg::OnButton8()	//自动行走 
{

}

void CMicroMecanumDlg::OnCancel()	//关闭程序
{

		CDialog::OnCancel();
	
}

void CMicroMecanumDlg::OnButton9()	//关闭系统 
{

}
//system control--------------------------------------------------------------------------------------

#define DataBaseBlock 1
//ADO LIST 4 --------------------------------------------------------------------------------------
BOOL CMicroMecanumDlg::InitializationWindows(void)	// init windows
{	
	//进程与指定cpu绑定
	SetProcessAffinityMask(GetCurrentProcess(), 8);	
	
	// 确认系统屏幕设置
	int W = GetSystemMetrics(SM_CXSCREEN);
	int H = GetSystemMetrics(SM_CYSCREEN);	
	MoveWindow(0,0,W,H-50);
	TRACE("系统默认分辨率%d*%d!\n",W,H);
	// 确认工作窗口
	CRect windows;
	GetClientRect(&windows);
	int x = windows.Width();
	int y = windows.Height();
	TRACE("显示窗口%d*%d!\n",x,y);
	// 设置按键		
	CRect rect;
	int bw = x / 20;
	int bh = y / 20;
	rect.left	= 19 * bw;
	rect.top	= 0;
	rect.right	= rect.left + bw;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON1)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON1)->SetWindowText("关闭电源");		//关闭电源 "紧急停止"
	rect.top	= 1*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON2)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON2)->SetWindowText("系统复位");		//测试运行 系统复位
	rect.top	= 2*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON3)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON3)->SetWindowText("使用说明");
	rect.top	= 3*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON4)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON4)->SetWindowText("路径编辑");
	rect.top	= 4*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON5)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON5)->SetWindowText("通讯设定");
	rect.top	= 5*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON6)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON6)->SetWindowText("自动定位");	//坐标复位 清空显示
	rect.top	= 6*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON7)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON7)->SetWindowText("禁止解码");	//坐标设定 
	rect.top	= 7*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON8)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON8)->SetWindowText("开始运动");	//开始运动 自动行走
	rect.top	= 8*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDCANCEL)->MoveWindow(&rect);	
	GetDlgItem(IDCANCEL)->SetWindowText("关闭程序");
	rect.top	= 9*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON9)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON9)->SetWindowText("关闭系统");
	TRACE("按键左右%d*%d!\n",rect.left,rect.right);
	// 设置Editbox
	bw = x / 16;
	bh = y / 40;
	rect.left	= 0;
//	rect.top	= y-bh;
	rect.top	= y-2*bh;
	rect.right	= rect.left + bw;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_EDIT1)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT2)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT3)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT4)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT5)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT6)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT7)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT8)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT9)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT10)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT11)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT12)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT13)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT14)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT15)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT16)->MoveWindow(&rect);
// 	// 设置list control
// 	bw = 19 * bw / 16;
// 	bh = y / 40;
// 	rect.left	= 0;
// 	rect.top	= y - bh-10;
// 	rect.right	= x;
// 	rect.bottom = y - 10;
// 	GetDlgItem(IDC_LIST2)->MoveWindow(&rect);
// 	TRACE("列表窗口%d*%d!\n",rect.right,rect.bottom);

// 	rect.left	= 0;
// 	rect.top	= 0;
// 	rect.right	= 19 * x / 20;
// 	rect.bottom = y - 10 - y / 20;
// 	GetDlgItem(IDC_STATICP1)->MoveWindow(&rect);

	rect.left	= x / 2 - 320;
	rect.top	= y / 2 - 240;
	rect.right	= x / 2 + 320;
	rect.bottom = y / 2 + 240;
	GetDlgItem(IDC_STATICP1)->MoveWindow(&rect);


	// 设置静态文本显示窗口2
	rect.left	= 0;
	rect.top	= y - 6*bh;
	rect.right	= x;
	rect.bottom = y - 4*bh;
	GetDlgItem(IDC_STATIC2)->MoveWindow(&rect);
	// 设置静态文本显示窗口1
	rect.left	= 0;
	rect.top	= y - 4*bh;
	rect.right	= x;
	rect.bottom = y - 2*bh;
	GetDlgItem(IDC_STATIC1)->MoveWindow(&rect);

	// 设置小车车头方向
	m_HeadingCombBxo.AddString("0");
	m_HeadingCombBxo.AddString("90");
	m_HeadingCombBxo.AddString("180");
	m_HeadingCombBxo.AddString("270");
	
	return TRUE;
}


#define MotionBlock	2 
// mecanum 小车运动控制模块 ---------------------------------------------------


void CMicroMecanumDlg::InitializationSoket()
{
	// 连接服务端发送
	pConnectSocketThread.CreateThread();
	pConnectSocketThread.play();
}


void CMicroMecanumDlg::OnTimer(UINT nIDEvent)	//运动控制专用定时器  
{
								// 自动通讯及显示运行数据
	CDialog::OnTimer(nIDEvent);
}
	

#define CammeraBlock	4 
// 相机模块 -------------------------------------------------------------------------------------------------------
// Evision 4 ------------------------------------------------------------------
// 在*Dlg.CPP中，添加Evision的条码识别函数
 //初始化相机参数及Evision，本函数已废弃，2015-12-30 qi
 //初始化相机参数及Evision，本函数已重用，2016-03-30 pc
void CMicroMecanumDlg::CameraInitional()  
{
	DisplayImage = TRUE;						// 图像显示开关，默认显示图像
	ImageProcessFrequence = 0;					// 图像处理间隔变量初始化 
	FindBarEnable = FALSE;

	ESetJpegQuality(100);								// 设置图像质量常数
	ESetTraceMode(E_TRACE_SILENT);						//	ESetTraceMode(E_TRACE_DISPLAY_FULL_MESSAGE);
	ESetAngleUnit(E_ANGLE_UNIT_DEGREES);				// Be sure to work in degrees
	
	// m_BarCode参数设置
	m_BarCode.SetStandardSymbologies(BRC_EAN_128);		// Select all standard symbologies	
	m_BarCode.SetKnownLocation(FALSE);					// 设定不知道条码位置、全局寻找条码

ESetJpegQuality(90);	//2016.03.31 pc
	
	sprintf(m_pszDecodedText, "\0");					// String initialization
	///ImageNull.SetSize(640, 480);						// 设定默认图像分辨率

	BarDecodeInfo = "准备解码";
//	DecodeTotalTime = 0;
	DecodeSuccesTime = 0;
	DecodeFaileTime = 0;
	TotalDispFrame = 0;
//	FinishedBarDecode = TRUE;							// 完成解码
	EnableDecoderBar = TRUE;							// 允许读码

	m_saveFlag=false;
	m_saveBMPFlag=true;
	m_saveJPGFlag=false;
	
	// 	m_GammaCtrl.SetRange(1,200,true);
	// 	m_ContrastCtrl.SetRange(1,200,true);
	// 	m_ExposureCtrl.SetRange(1,5000,true);
	// 	m_GainCtrl.SetRange(1,255,true);
	// 	m_SaturationCtrl.SetRange(0,200,true);
	// 	m_BlackLevelCtrl.SetRange(0,255,true);
	// 	m_AETargetCtrl.SetRange(0,255,true);
	// 	m_DelayCtrl.SetRange(0,1000,true);
	
	int CamAllNum=0;
	CameraGetCount(&CamAllNum);							//获取相机个数
	if (CamAllNum>0)
	{
		CString DisStr;
		DisStr.Format("系统有%d个相机! \n",CamAllNum);
		TRACE(DisStr);
		
		char *name=new char[255];
		char *model=new char[255];
		NameComBox=(CComboBox*)GetDlgItem(IDC_MULTICAM);	//关联界面相机名称列表框
		if(CamAllNum>0)
		{
			for(int i=0; i<CamAllNum; i++)
			{
				CameraGetName(i, name, model);				//依次获取相机名称、型号
				CString t_model;
				t_model.Format("%s %d#",model,i);			//t_model.Format("%s %d#",name,i);
				NameComBox->InsertString(i,_T(t_model));	//生成列表
			}
			NameComBox->SetCurSel(0);						//选择0#相机
		}													//在相机列表框显示相机名		
		
		m_index = NameComBox->GetCurSel();					//取得选中的索引
		CameraInit(m_index);								//初始化0#相机init camera
		pProcessThread.setIndex(m_index);					//设定线程读取0#相机数据				
		CameraLoadParameter(m_index,0);						//载入0#相机第零组参数
		ResoComBox=(CComboBox*)GetDlgItem(IDC_RESOLUTION);	//关联界面相机分辨率列表框
		ResoComBox->ResetContent();							//清除上次选择的分辨率	
		
		int index=0,width=0,height=0, camera_count=0;
		CameraGetResolutionCount(m_index,&camera_count);	//获取相机分辨率组数
		for(int i=0;i<camera_count;i++)
		{
			CameraGetResolution( m_index,i,&width, &height);//获取第N#相机第N组分辨率
			CString t_reso;
			t_reso.Format("%d*%d",width,height);
			ResoComBox->InsertString(i, _T(t_reso)); 		//生成列表
		}
		ResoComBox->SetCurSel(1);							//设置选中的项
		DisStr.Format("共有%d组分辨率! \n",camera_count);
		TRACE(DisStr);

		// 采用第一组默认设置640×480分辨率
		int width1=0,height1=0;
		CameraSetResolution(m_index,1,&width1, &height1);
		
		CameraSetOption(m_index,CAMERA_IMAGE_GRAY8);	// 设为用8位灰度图

		// 相机帧扫描延迟时间设定，2000=18HZ，640=30HZ
		//CameraSetDelay(m_index,10);
		//CameraSetDelay(m_index,640);		// 31HZ						
		CameraSetDelay(m_index,2000);		// 18HZ	
		delete []name; 
		delete []model; 

		//相机分辨率设定
		int Group=1,sWidth,sHigh;
		CameraGetResolution(m_index,Group,&sWidth,&sHigh);
		DisStr.Format("%d#相机采用%d组W%dXH%d!\n",m_index,Group,sWidth,sHigh);
		TRACE(DisStr);

		// 相机参数设定

		// 1.伽玛校正曲线设定!!!
		double gamma;
		CameraGetGamma(m_index,&gamma);
		DisStr.Format("伽玛校正曲线%d!\n",(int)gamma*100);
		TRACE(DisStr);
		// 2.图像对比度设定!!!
		double contrast;
		CameraGetContrast(m_index,&contrast);
		DisStr.Format("图像对比度%d!\n",(int)contrast*100);
		TRACE(DisStr);
		// 3.色彩饱和度设定（黑白相机无意义）

		// 4.自动增益使能设定


		// 5.图像增益（亮度）值设定
		int gain;
		CameraGetGain(m_index,&gain);
		DisStr.Format("图像亮度%d!\n",gain);
		TRACE(DisStr);		
		// 6.自动快门使能设定
		
		// 7.快门曝光时间设定!!!
		int exposure;
		CameraGetExposure(m_index,&exposure);
		DisStr.Format("曝光时间%d!\n",exposure);
		TRACE(DisStr);
 		// 8.图像黑电平阈值设定
		int blacklevel;
		CameraGetBlackLevel(m_index,&blacklevel);
		DisStr.Format("黑电平阈值%d!\n",blacklevel);
		TRACE(DisStr);
		// 9.自动曝光目标值设定 	
		int AETarget;
		CameraGetAETarget(m_index,&AETarget);
		DisStr.Format("自动曝光目标值%d!\n",AETarget);
		TRACE(DisStr);						
		// 10.X水平镜像设定	
		bool flag_x;
		CameraGetMirrorX(m_index,&flag_x);
		DisStr.Format("X水平镜像%d!\n",flag_x);
		TRACE(DisStr);
		// 11.Y垂直镜像设定	
		bool flag_y;
		CameraGetMirrorY(m_index,&flag_y);
		DisStr.Format("Y垂直镜像%d!\n",flag_y);
		TRACE(DisStr);
		// 12.相机帧扫描延迟时间设定
		int delay;
		CameraGetDelay(m_index,&delay);
		DisStr.Format("帧扫描延迟%d!\n",delay);
		TRACE(DisStr);
		// 设置相机准备就绪标志
		CameraReady = TRUE;
		// 启用读相机数据线程create thread
		pProcessThread.CreateThread();
		// 开始播放图像		
		pProcessThread.play();
		TRACE("CameraReady=%d,用线程显示图像!\n",CameraReady);
		//线程与指定cpu绑定
		SetThreadAffinityMask(GetCurrentThread(),0X00000008);
	}
	else
	{
		CameraReady = FALSE;								// 没有相机
		MessageBox("没有发现相机，无法启用相机定位功能!",
			"【严重错误】",
			MB_OK|MB_ICONWARNING|MB_APPLMODAL);
	}
}

void CMicroMecanumDlg::InitializationCamera()   //初始化相机参数new
{
	m_BarCode.SetStandardSymbologies(BRC_EAN_128);		// 读取EAN128码	
	m_BarCode.SetKnownLocation(FALSE);					// 设定不知道条码位置、全局寻找条码

	//-----------------------------------------------------------------------------
	DisplayImage = TRUE;						// 图像显示开关，默认显示图像
	ImageProcessFrequence = 0;					// 图像处理间隔变量初始化 
	FindBarEnable = FALSE;

	ESetJpegQuality(100);								// 设置图像质量常数
	ESetTraceMode(E_TRACE_SILENT);						//	ESetTraceMode(E_TRACE_DISPLAY_FULL_MESSAGE);
	ESetAngleUnit(E_ANGLE_UNIT_DEGREES);				// Be sure to work in degrees
	sprintf(m_pszDecodedText, "\0");					// String initialization
	///ImageNull.SetSize(640, 480);						// 设定默认图像分辨率

	BarDecodeInfo = "准备解码";
//	DecodeTotalTime = 0;
	DecodeSuccesTime = 0;
	DecodeFaileTime = 0;
	TotalDispFrame = 0;
//	FinishedBarDecode = TRUE;							// 完成解码
	EnableDecoderBar = TRUE;							// 允许读码

	m_saveFlag=false;
	m_saveBMPFlag=true;
	m_saveJPGFlag=false;

	int CamAllNum=0;
	CameraGetCount(&CamAllNum);							//获取相机个数
	if (CamAllNum>0)
	{

		int index = 0; //camera index
		//int group = 2; //output group (320*240)
		int group = 1; //output group (640*480)
		CameraInit(index);								//初始化0#相机init camera
//		pProcessThread.setIndex(m_index);					//设定线程读取0#相机数据				
		CameraLoadParameter(index,0);						//载入0#相机第零组参数
		
		int width1=0,height1=0;
		CameraSetResolution(index,group,&width1, &height1);	//设置相机分辨率

		CameraSetOption(index,CAMERA_IMAGE_GRAY8);

		CameraSetDelay(0,0);						//相机帧扫描延迟时间设定
		
		// 设置相机准备就绪标志
		CameraReady = TRUE;
		// 启用读相机数据线程create thread
		pProcessThread.CreateThread();
		// 开始播放图像		
		pProcessThread.play();
		TRACE("CameraReady=%d,用线程显示图像!\n",CameraReady);
		//线程与指定cpu绑定
		SetThreadAffinityMask(GetCurrentThread(),0X00000008);
	}
	else
	{
		CameraReady = FALSE;								// 没有相机
		MessageBox("没有发现相机，无法启用相机定位功能!",
			"【严重错误】",
			MB_OK|MB_ICONWARNING|MB_APPLMODAL);
	}
}

// Socket通讯
// 服务端命令处理
LRESULT CMicroMecanumDlg::ProcessCommand(WPARAM wParam, LPARAM lParam)
{

	return true;
}


// Camera 图像显示线程------------------------------------------------------------------
// After the process thread generate a bmp file,this function load picture and show it 
LRESULT CMicroMecanumDlg::ProcessImage(WPARAM wParam, LPARAM lParam)
{
	struct PicInformation{	
		long width;
		long height;
		unsigned char *m_image;
	}*info;
	// 设置时间记忆变量
	static DWORD TimePoint0,TimePoint00;
	TotalDispFrame++;	
	info=(PicInformation *)lParam; 	

	BITMAPINFO bitInfo;
	BITMAPINFOHEADER * header=&bitInfo.bmiHeader;
	header->biSize=40; 					    	// header size
	header->biWidth=info->width;
	header->biHeight=info->height;
	header->biPlanes=1;
	header->biBitCount=8;				    	// RGB encoded, 24 bit
	header->biCompression=BI_RGB;		    	// no compression
	header->biSizeImage=0;
	header->biXPelsPerMeter=0;
	header->biYPelsPerMeter=0;
	header->biClrUsed=0;
	header->biClrImportant=0;
	//TRACE("CameraReady=%d,进入图像显示线程!\n",CameraReady);

	//调用Evision解析条码
	if (CameraReady == TRUE)		// 相机准备就绪；才进行下一次解码
	{		
		//从图像序列中N抽一，显示图像&解码
		ImageProcessFrequence++;				// 统计接受图像的数量
		//TRACE("ImageProcessFrequence=%d,图像处理!\n",ImageProcessFrequence);
		if (ImageProcessFrequence >= ImageProcessSpace)		// 是否到达解码阈值					
		{
			ImageProcessFrequence = 0;
			// 计算图像《刷新+解码》的频率
			// 读现在时间值
			//TRACE("TimePoint0=%d!\n",TimePoint0);
			DWORD TimePoint1 =  GetCurrentTime();
			//TRACE("TimePoint1=%d!\n",TimePoint1);

			// 计算间隔时间	
			FramePerSecond = TimePoint1 - TimePoint0;
			//TRACE("FramePerSecond=%dmS!\n",FramePerSecond);
			// 记忆现在时间值
			TimePoint0 = TimePoint1;
			// 计算图像刷新频率
			FramePerSecond = 1000 / (FramePerSecond+0.1);

			//调用Evision解码,并获取图像信息
			EBW8Image2/*EC24Image1*/.SetSize(info->width,info->height);			// 设定图片长宽	
			//从内存中直接导入图像 
			EBW8Image2/*EC24Image1*/.SetImagePtr(info->width,info->height,info->m_image,0);
			// 输出解码图像
			m_Src = EBW8Image2;
					
			//显示图像			
			CDC *pDC=NULL;											//定义一个空的Device Concent
			CRect rect;												//定义矩形
			CWnd *pWnd2 = GetDlgItem(IDC_STATICP1);					//获取绘画窗口		
			if(pWnd2)												//若窗口存在	
			{
				pWnd2->GetClientRect(&rect);						//设定绘画矩形
				pDC = pWnd2->GetDC();								//设定绘图目标设备				
				BarDecoder();					//调用Evision解码	
				m_Src.Draw(pDC->m_hDC);			//把采集的图像显示在界面上Draw the image
				//显示图像的分辨率信息
				CString PictureIfo = "";
				PictureIfo.Format("W%d X H%d",info->width,info->height);
				BarDecodeInfo += PictureIfo;
						
				CPen pen3; 											// 自定义画笔
				pen3.CreatePen(PS_SOLID,1,RGB(0,255,0));			// 创建画笔
				CPen* pOldPen3= pDC->SelectObject(&pen3);			// Select a green pen

				m_BarCode.Draw(pDC->GetSafeHdc(), INS_DRAW_ACTUAL);	// Draw the found barcode frame
				pDC->TextOut(10, 460, BarDecodeInfo, strlen(BarDecodeInfo));	// 显示解码信息
				pDC->SelectObject(&pOldPen3);						// Put back old pen	
		
				ReleaseDC(pDC);										// 释放绘图目标设备	


			}
			//在图像上同步显示字符等
//			DrawArrow(mARROW);

		}
	}
	delete []info->m_image;
	// info->m_image=NULL;	
	return true;
}

// 相机模块 -------------------------------------------------------------------------------------------------------

#define EvisionBlock	5 
// 视觉模块 -------------------------------------------------------------------------------------------------------

// 函数功能包括调用Evision库函数，打开一个C24图像转为BW8格式、解码，并在文本框显示
void CMicroMecanumDlg::BarDecoder()						// 调用Evision解码
{	
	BarDecodeInfo = "";
	FinishedBarDecode = FALSE;					// 开始解码						
	DWORD TimePoint0 =  GetCurrentTime();		// 读条码开始时间点

	bool isDecodeSuccess = false;
	CString BarCodeStr = ReadBar();				// 读条码
	if (BarCodeStr.GetLength() == 5)
		isDecodeSuccess = true;
	
	DWORD TimePoint1 =  GetCurrentTime();		// 读条码结束时间点	
	DWORD TimeCost = TimePoint1 - TimePoint0;	// 读条码总计花销时间	
	
	time_t t = time( 0 ); 			
	strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A",localtime(&t) ); 
	ofstream m_ofsLog10;
	m_ofsLog10.open("log-10记录读到的地标.txt",ios::app);
	// 如果解码信息正确
	if (isDecodeSuccess)
	{
		// -----------记录读到的地标--pc	
		if(Temp_LastBarCodeid == BarCodeStr.GetBuffer(0))
		{}
		else
		{
			m_ofsLog10<<tmp<<"  "<<BarCodeStr.GetBuffer(0)<<"\n"; 
			Temp_LastBarCodeid = BarCodeStr.GetBuffer(0);
		}
		// -----------记录读到的地标结束
	}
	else	// 解码不正确
	{
		; 
	}
	m_ofsLog10.close();
								
}

CString CMicroMecanumDlg::ReadBar()
{
	CString bar("");
	char sTemp[256];
	sprintf(m_pszDecodedText, "\0");
	//调用Evision的Barcode模块解码
	if (DisplayImage == TRUE)		
	{
		m_Match.Match(&m_Src);
		
		if(!EFailure())
		{
			double x, y, score;
			int nNum = m_Match.GetNumPositions();
			if (nNum > 0)	// 找到了匹配点（匹配度大于0.3）
			{
				x = m_Match.GetPosition(0)->m_f32CenterX;
				y = m_Match.GetPosition(0)->m_f32CenterY;
				score = m_Match.GetPosition(0)->m_f32Score;
				//double angle = m_Match.GetPosition(0)->m_f32Angle;
				sprintf(sTemp, "s=%5.2f,x=%i", score,(int)x-130);	
				BarDecodeInfo = sTemp;
				m_Roi.Detach();		
				m_Roi.Attach(&m_Src);
				m_Roi.SetPlacement(0,0,640,480);			
				m_BarCode.Read(&m_Roi, m_pszDecodedText, sizeof(m_pszDecodedText) - 1);
				if (strlen(m_pszDecodedText) < BarCodeNumber)	// 阅读失败
				{
					BarDecodeInfo += ",Fail1";
					
					// 从左侧3/4图像中读条码
					m_Roi.Detach();		
					m_Roi.Attach(&m_Src);
					m_Roi.SetPlacement(0,0,480,480);
					//m_Src.SetPlacement(0,0,480,480);
					sprintf(m_pszDecodedText, "\0");
					m_BarCode.Read(&m_Roi, m_pszDecodedText, sizeof(m_pszDecodedText) - 1);
					if (strlen(m_pszDecodedText) < BarCodeNumber)	// 阅读失败
					{
						BarDecodeInfo += ",Fail2";
						// 从右侧3/4图像中读条码
						m_Roi.Detach();		
						m_Roi.Attach(&m_Src);
						m_Roi.SetPlacement(160,0,480,480);
						//m_Src.SetPlacement(160,0,480,480);
						sprintf(m_pszDecodedText, "\0");
						m_BarCode.Read(&m_Roi, m_pszDecodedText, sizeof(m_pszDecodedText) - 1);
						if (strlen(m_pszDecodedText) < BarCodeNumber)	// 阅读失败
						{
							BarDecodeInfo += ",Fail3";
						}
					}
				}
			}	// end if (nNum > 0)	// 找到了匹配点（匹配度大于0.3）
			else
			{
				BarDecodeInfo = "!Score<0.25";
			}
		}	// end if(!EFailure())
		
		//若解码后位数大于等于5
		if (strlen(m_pszDecodedText) >= BarCodeNumber)					
		{
			m_pszDecodedText[BarCodeNumber] = '\0';
			bar = m_pszDecodedText;
			// 如果条码每一位都是0-9数字，且在地标库中存在，则解码信息正确
			if (!CheckBarcode(bar))
				bar = "";
			else // 读码成功
			{
				// 计算屏幕像素到实际距离换算系数，单位：毫米/像素
				double CaliK = BarLenth/m_BarCode.GetSizeX();
				// 读取条码中心X轴坐标
				BarXlocation = (m_BarCode.GetCenterX() - m_Src.GetWidth()/2) * CaliK;
				// 读取条码中心Y轴坐标
				BarYlocation = (m_Src.GetHeight()/2 - m_BarCode.GetCenterY()) * CaliK;
				// 读取条码X坐标轴长度
				BarXlenth = m_BarCode.GetSizeX() * CaliK;
				// 读取条码Y坐标轴长度
				BarYlenth = m_BarCode.GetSizeY() * CaliK;
				// 读取条码偏转角度	
				BarAngle = m_BarCode.GetAngle();

				char s[256];
				sprintf(s, " ,x=%5.2f, y=%5.2f, angle=%6.2f\n", BarXlocation, BarYlocation, BarAngle);
				BarDecodeInfo = "读码成功，barcode=" + bar + s;
				SendBarCodeInfo(1,bar,BarXlocation,BarYlocation,BarAngle);
			}
		}
	}

	if (bar == "")
	{
		SendBarCodeInfo(0,bar,0,0,0);	
	}
	
	return bar;
}


// 视觉模块 -------------------------------------------------------------------------------------------------------



void CMicroMecanumDlg::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	CString s;
	m_HeadingCombBxo.GetLBText(m_HeadingCombBxo.GetCurSel(),s);
}


// 判读一个条码的各位是否是0-9的数字
BOOL CMicroMecanumDlg::CheckBarcode(CString bar)
{
	// 验证长度
	if (bar.GetLength() != BarCodeNumber)
	{
		return false;
	}
	// 验证字符
	for (int i =0; i< BarCodeNumber; i++)
	{
		char c = bar.GetAt(i);
		if (c<'0' || c>'9')
		{
			return false;
		}
	}

	return true;
}
 
// 发送轨迹校准相关的数据到后台
void CMicroMecanumDlg::SendBarCodeInfo(int isSuc, CString barID, double x, double y, double angle)
{
	Json::Value msgBody;
	msgBody["isSuccess"] = isSuc;
	msgBody["BarCodeID"] = (char*)(_bstr_t)barID.GetBuffer(0);
	msgBody["CenterX"] = x;
	msgBody["CenterY"] = y;
	msgBody["Angle"] = angle;

	Json::FastWriter writer;  // 用Json::FastWriter 来处理 json 应该是最快的
	std::string strMsgBody = writer.write(msgBody);
	SendMessage2Server((char*)strMsgBody.c_str());		
}

// socket连接成功时发送登录消息给服务端，暂时未用,
void CMicroMecanumDlg::SendIGVLoginMessage()
{
	
}

void CMicroMecanumDlg::SendMessage2Server(char* msg)
{	
	//msg = "00000";	
	// 获取当前系统时间
	time_t t = time( 0 ); 			
	strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A",localtime(&t) ); 

	ofstream m_ofsLog11;
	m_ofsLog11.open("log-11发送消息给Motion.txt",ios::app);
	if(send(m_Sock, msg, strlen(msg), 0) == SOCKET_ERROR) //判断发送是否成功
	{		
		m_ofsLog11<<tmp<<"------------发送消息失败\n"; 
		m_isSocketConnected = false;	
		m_ConnectStatus.SetWindowText("未与Motion连接");		
	}
	else if(msg == "")   //判断发送的数据是否为空
	{
		m_ofsLog11<<tmp<<"------------发送消息为空\n"; 	
	}
	else //发送成功
	{	
		m_ofsLog11<<tmp<<"------------发送消息成功\n"; 	
	}	
	m_ofsLog11.close();
}
 
int CMicroMecanumDlg::GetMacaddress(char* mac) //获取本机MAC址 
{
	ULONG ulSize=0;
	PIP_ADAPTER_INFO pInfo=NULL;
	int temp=0;
	temp = GetAdaptersInfo(pInfo,&ulSize);//第一处调用，获取缓冲区大小
	pInfo=(PIP_ADAPTER_INFO)malloc(ulSize);
	temp = GetAdaptersInfo(pInfo,&ulSize);

	int iCount=0;
	//while(pInfo)//遍历每一张网卡
	{
		//  pInfo->Address MAC址
		for(int i=0;i<(int)pInfo->AddressLength;i++)
		{
			byte2Hex(pInfo->Address[i],&mac[iCount]);
			iCount+=2;
			if(i<(int)pInfo->AddressLength)
			{
				if (i<(int)pInfo->AddressLength-1)
					mac[iCount++] = '-';
			}
			else
			{
				//mac[iCount++] = '#';
			}
		}
		//pInfo = pInfo->Next;
	}

	if(iCount >0)
	{
		mac[iCount]='\0';
		return iCount;
	}
	else return -1;
}	

void CMicroMecanumDlg::byte2Hex(unsigned char bData, char hex[])
{
	int high=bData/16,low =bData %16;
	hex[0] = (high <10)?('0'+high):('A'+high-10);
	hex[1] = (low <10)?('0'+low):('A'+low-10);
}


LRESULT CMicroMecanumDlg::ProcessConnectSuccess(WPARAM wParam, LPARAM lParam)
{
	m_ConnectStatus.SetWindowText("已连接");
	return true;
}

LRESULT CMicroMecanumDlg::ProcessLoginSuccess(WPARAM wParam, LPARAM lParam)
{
	m_ConnectStatus.SetWindowText("已登录");
	return true;
}
