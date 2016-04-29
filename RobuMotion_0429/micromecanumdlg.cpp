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
#include "MessageHandling.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

    UINT server_thd(LPVOID p); //声明线程函数
	SOCKET listen_sock;//定义一个全局的监听soket
	SOCKET sock;//定义一个soket
	CString showeditmsg; //定义全局的显示消息的字符串


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
	DDX_Control(pDX, IDC_BUTTON14, m_EmergencyBrake);
	DDX_Control(pDX, IDC_EDIT17, showmsg_edit);
	DDX_Control(pDX, IDC_BUTTON13, star_button);
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
	ON_BN_CLICKED(IDC_BUTTON_PauseAndRecovery, OnButton_PauseAndRecovery)
	ON_WM_TIMER()
	ON_MESSAGE(WM_PROCESS_MESSAGE,ProcessImage)
	ON_MESSAGE(WM_COMMAND_MESSAGE,ProcessCommand)
	ON_MESSAGE(WM_CONNECT_SUCCESS_MESSAGE,ProcessConnectSuccess)
	ON_MESSAGE(WM_LOGIN_SUCCESS_MESSAGE,ProcessLoginSuccess)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton_EmergencyBraking)
	ON_BN_CLICKED(IDC_BUTTON_SlowDown, OnButton_SlowDown)
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

	FlagPause =0; // 0 代表暂停状态 1代表正常状态

	CurLineNo = 0;					// 记录当前的Step序号
	IsEmgcBrake = false;			// 记录当前的Step序号

m_ofsLog2.open("log-2搜索地标过程.txt",ios::app);
m_ofsLog2<<"--------------------------------\n";

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


 	TableName = "Route";			// 设定参数表名
 	InitializationWindows();		// 初始化界面	

	//------------------------------------------------
	// qi
	// 初始化校准点（针对一条Line），最多支持1024个
	nCheckPoints = 0;
	CheckPoints = new CHECKPOINT[1024];
	CheckMarks = new MARK[1024];

	nLineMarks = 0;
	LineMarks = new MARK[1024];

	nMarks = 0;	// 初始化地标
	m_CaliCount = 0;	// 校准次数置0
	ReadMarkFile();					// 读取地标文件 qi
	ReadSearchLines();				// 读取地标搜索路径文件 qi
	ReadConfigFile();				// 读取配置文件 qi

	m_igvInfo.curX = -10000.0;	// 小车初始未定位和定向
	m_igvInfo.curY = -10000.0;	// 小车初始未定位和定向
	m_igvInfo.preX = m_igvInfo.curX;	//  小车上一点(前进、后退前)坐标，单位是毫米
	m_igvInfo.preY = m_igvInfo.curY;	//  小车上一点(前进、后退前)坐标，单位是毫米
	m_igvInfo.isFree = true;
	m_igvInfo.Speed = 0;
	m_igvInfo.MoveDirection = -1;
	m_igvInfo.isMarked = false;
	m_igvInfo.MarkID = "";

	m_igvInfo.Compass = -1.0;	// 小车初始未定位和定向
	
	m_igvInfo.Heading = 270.0;	// 没有安装地磁感应前，车头方向暂定为90（适应实验室的坐标系）
	//m_Heading = -1.0;	// 小车初始未定位和定向
	m_HeadingCombBxo.SetCurSel(3);
	m_HeadingCombBxo.UpdateData(TRUE);

	m_NotFindMarkTimes370 = 0;	// 执行370指令时未发现mark的次数
	m_iCurrentSearchingRoute = -1;		// 当前正在执行的地标搜索路径索引, -1表示未进行地标搜索
	m_isSearchingMark = false;	// 不执行搜索地标
	m_isSearchingMarkFound = false;	// 尚未搜到地标
	// end qi 
	//------------------------------------------------	
		
	DeknockEnable = TRUE;

	InitializationMotion();			// Initialization motion control
	InitRs232c();					// 初始化RS232C
	//InitializationCamera();		// 初始化相机
//	CameraInitional();			// 初始化相机

	//------------------------------------------------
	// pc
	//以下初始化图像匹配组件
	m_Pattern.Load("pattern.bmp");	// 加载模板
	m_Roi.Attach(&m_Pattern);
	m_Roi.SetPlacement(0,0,268,44);
	m_Match.SetMaxPositions(1);// return only one position
	m_Match.SetMinScore(0.25f);	// 匹配度至少达到0.25
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

// 读取配置文件
void CMicroMecanumDlg::ReadConfigFile()
{
	ifstream fs;

	fs.open("config.txt", ios::nocreate);//默认以 ios::in 的方式打开文件，文件不存在时操作失败

	if(fs)
	{
		double factor;
		fs >> factor;
		RotateFactor = factor;

		double limit;
		fs >> limit;
		RotateLimit = limit;

		char* temp_ip = new char[512];		
		fs >> temp_ip;
		m_IP = (CString)temp_ip;		

		char* temp_port = new char[100];
		fs >> temp_port;
		m_Port = (CString)temp_port;	
	}
	else
	{
		RotateFactor = 1/2;	// 缺省值
		RotateLimit = 1;		// 确实值1度
		
	}

	fs.close();
	
	//CString str;
	//str.Format("factor = %.4f, limit=%.1f", RotateFactor,RotateLimit);
	//MessageBox(str,"",MB_OK);

}

// 读取地标文件 qi
void CMicroMecanumDlg::ReadMarkFile()
{
	ifstream fs;

	fs.open("marks.txt", ios::nocreate);//默认以 ios::in 的方式打开文件，文件不存在时操作失败

	if(fs)
	{
		fs>>nMarks;
		char* sline = new char[24*nMarks];	// 此处内存有优化空间
		char* pline = sline;	// 记录sline指针位置，因为后面sline指针移位了
		marks = new MARK[nMarks];
		int i = 0;
		fs.getline(sline,256,'\n');//读取回车换行
		while(!fs.eof() && i<nMarks)
		{
			fs.getline(sline,256,'\n');//"12345,100,0,90"
			if(strlen(sline) > 0)
			{
				char* buf = strstr(sline, ",");//",100,0,90"
				buf[0] = '\0';//此时sline变成了"12345"
				marks[i].barCode = sline;
				
				sline = buf + 1; //"100,0,90"
				buf = strstr(sline, ",");//",0,90"
				buf[0] = '\0';//此时sline变成了"100"
				marks[i].x = atoi(sline);

				sline = buf + 1; //"0,90"
				buf = strstr(sline, ",");//",90"
				buf[0] = '\0';//此时sline变成了"0"
				marks[i].y = atoi(sline);

				sline = buf + 1; //"90"
				marks[i].dir = atoi(sline);
				
				switch(sline[0])
				{
				case 'n':
					marks[i].dir = 0;
					break;
				case'e':
					marks[i].dir = 90;
					break;
				case's':
					marks[i].dir = 180;
					break;
				case'w':
					marks[i].dir = 270;
					break;
				}
				
				i++;
			}
		}
		fs.close();
		delete pline;
	}

	// 生成映射
	for (int i = 0; i < nMarks; i++)
	{
		mapMarks.insert(pair<CString,MARK*>(marks[i].barCode,&marks[i]));
	}
	/*
	MARK * pM = GetMarkInfo("10002");
	CString str;
	str.Format("x=%d,y=%d,barcode=%s",pM->x,pM->y,pM->barCode);
	MessageBox(str,"",MB_OK);
	
	int x_origin = 0;
	int y_origin = 0;
	int y_destination = 2103;
	nLineMarks = 0;

				
	std::map <CString,MARK*>::iterator iter = mapMarks.begin();
				
	while(iter!=mapMarks.end())
	{
					int x = iter->second->x;
					int y = iter->second->y;
					if (fabs(x-x_origin) < 20 && y > y_origin -20 && y < y_destination +20)	//2cm
					{
						MARK* m = GetMarkInfo(iter->second->barCode);
						LineMarks[nLineMarks].barCode = m->barCode;
						LineMarks[nLineMarks].x = m->x;
						LineMarks[nLineMarks].y=m->y;
						LineMarks[nLineMarks].dir = m->dir;

						nLineMarks++;
					}
					
					iter++;					
	}

				CString str;
				str.Format("line mark count: %d个", nLineMarks);
				MessageBox(str);
*/
}

// 根据条码得到地标信息
MARK* CMicroMecanumDlg::GetMarkInfo(CString barCode)
{
	return mapMarks[barCode];

}

void CMicroMecanumDlg::OnButton1() //紧急停止/关闭电源
{
Control.mMaxLine = 0;

	CString str;
	GetDlgItem(IDC_BUTTON1)->GetWindowText(str);
	if (str == "关闭电源")
	{
		m_ofsLog2.close();
		StopMotion();
		SetOutput(&m_Device,ZADDR, 0x00FF);	//P4 ~ P7输出高电平1，禁能四轴驱动器
		SetOutput(&m_Device,UADDR, 0x00FF);	//P4 ~ P7输出高电平1，关闭驱动器电源
		pProcessThread.pause();				// 线程暂停接受数据//关闭线程
		pProcessThread.ExitInstance();		// 退出线程
		GetDlgItem(IDC_BUTTON1)->SetWindowText("打开电源");
	} 
	else
	{
		m_ofsLog2.open("log-2搜索地标过程.txt",ios::app);
		SetOutput(&m_Device,ZADDR, 0x000F);	//P4 ~ P7输出低电平0，使能四轴驱动器
		SetOutput(&m_Device,UADDR, 0x003F);	//P4 ~ P7输出低电平0，打开驱动器电源

		Control.mLineEnable = 0;	// pc 2016.03.30

		GetDlgItem(IDC_BUTTON1)->SetWindowText("关闭电源");
	}
}

void CMicroMecanumDlg::OnButton2()	//系统复位
{
	MotionInit();
//	Init(&m_Device);
//	SetOutput(&m_Device,ZADDR, 0x000F);	//P4 ~ P7输出低电平0，使能四轴驱动器
//	SetOutput(&m_Device,UADDR, 0x000F);	//P4 ~ P7输出低电平0，打开驱动器电源
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
	CRoute Dlg; 
	Dlg.DoModal();
}

void CMicroMecanumDlg::OnButton5()	//通讯设定
{
	CCom Dlg;
	Dlg.DoModal();
}

void CMicroMecanumDlg::OnButton6()	//坐标复位(静态校准)  
{
// 	if ((FindBarEnable == FALSE) && (Control.mLineEnable != 0))
// 	{
// 		FindBarEnable = TRUE;	
// 	}
	Line[0].Derection = 370;
	Line[0].MaxSpeed = 500;
	Line[0].ACC = 500;
	Line[0].DACC = 500;
	if (Control.mLineEnable == 0)		// 只有上一运动完成，才能开始新的运动；若运动中，不执行任何动作
	{
		Control.mStep = 0;				// 复位步骤指针
		Control.mExeStep = 0;			// 复位微步指针
		Control.mStepEnable = 0;		// 复位禁用自动控制
		Control.mFixStep = 0;			// 复位自动校正步数
		
		Control.mPause		= 0;		// 复位暂停旗标
		Control.mGoOn		= 0;		// 复位继续路径运动旗标
		Control.mRevers		= 0;		// 复位反向旗标
		
		Control.mLine		= 0;		// 复位单段步骤指针
		Control.mExeLine	= 0;		// 复位单段执行指针
		Control.mMaxLine	= 1;		// 复位单段执行指针
		Control.mLineEnable = 0XFF;		// 开始执行微步程序
		FindBarEnable		= TRUE;		// 允许自动识别条码
	}
}

void CMicroMecanumDlg::OnButton7()	//坐标设定
{
	DisFixInfo = "";
	GetDlgItem(IDC_STATIC2)->SetWindowText(DisFixInfo);

	if (DisplayImage == TRUE)		// 设置一个图像显示开关
	{
		//禁止显示图像
		DisplayImage = FALSE;
		GetDlgItem(IDC_BUTTON7)->SetWindowText("开始解码");	//坐标设定 
	}
	else
	{
		//把采集的图像显示在界面上Draw the image
		DisplayImage = TRUE;
		GetDlgItem(IDC_BUTTON7)->SetWindowText("禁止解码");	//坐标设定 
	}
}

void CMicroMecanumDlg::OnButton8()	//自动行走 
{
	CString str;
	GetDlgItem(IDC_BUTTON1)->GetWindowText(str);
	if (str == "关闭电源")
	{
		nCheckPoints = 0;
		StartMotion();				// 开始行走
	}
	else
	{
		MessageBox("驱动器电源尚未打开，请打开电源!","【严重错误】",MB_OK|MB_ICONWARNING|MB_APPLMODAL);
	}
}

void CMicroMecanumDlg::OnCancel()	//关闭程序
{
	INT i = MessageBox("请确认机器人处于可以关闭程序的状态!","【即将关闭控制程序】",MB_YESNO|MB_ICONWARNING|MB_APPLMODAL);	
	if (i == 6)								//确认关闭
	{
		//---------------qi-----------------------
		// 删除地标内存
		if(!marks)
		{
			delete[] marks;		
		}
		//delete marks;
		//删除校准点
		if(!CheckPoints)
			delete[] CheckPoints;
		if(!CheckMarks)
			delete[] CheckMarks;
		if(!LineMarks)
			delete[] LineMarks;

		//------------------------------------

		Stop(&m_Device,SDSTP);				// 减速停止
		SetOutput(&m_Device,ZADDR, 0x00FF);	//P4 ~ P7输出低电平0，禁能四轴驱动器
		SetOutput(&m_Device,UADDR, 0x00FF);	//P4 ~ P7输出高电平1，关闭驱动器电源
		if(!DeviceClose(&m_Device))			//关闭运动控制卡
		{
			MessageBox("运动控制卡关闭失败，Close fail!");		
		}
		ZjCom.SetPortOpen(0);				// 关闭串口 
		KillTimer(1);						// 关闭定时器
		Sleep(200);							// 等待100ms
		pProcessThread.pause();				// 线程暂停接受数据
		pProcessThread.ExitInstance();		// 退出线程
		CameraFree(0);						// 关闭相机
		CDialog::OnCancel();
	}
}

void CMicroMecanumDlg::OnButton9()	//关闭系统 
{
	INT i = MessageBox("请确认机器人处于可以关闭电源的状态!","【即将切断机器人电源】",MB_YESNO|MB_ICONWARNING|MB_APPLMODAL);	
	if (i == 6)								//确认关闭
	{
		Stop(&m_Device,SDSTP);				// 减速停止
		SetOutput(&m_Device,ZADDR, 0x00FF);	//P4 ~ P7输出低电平0，禁能四轴驱动器
		SetOutput(&m_Device,UADDR, 0x00FF);	//P4 ~ P7输出高电平1，关闭驱动器电源
		if(!DeviceClose(&m_Device))			//关闭运动控制卡
		{
			MessageBox("运动控制卡关闭失败，Close fail!");		
		}
		pProcessThread.pause();				// 线程暂停接受数据
		pProcessThread.ExitInstance();		// 退出线程//lipan_修改了关闭程序的操作顺序
		ZjCom.SetPortOpen(0);				// 关闭串口
		KillTimer(1);						// 关闭定时器
		Sleep(200);							// 等待100ms
		CameraFree(0);						// 关闭相机
		CDialog::OnCancel();				// 关闭控制界面
		SystemShutdown();					// 关闭电脑

	}	
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
	rect.top	= y / 2;// - 240;
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
void CMicroMecanumDlg::InitADOConn(void)	//创建ADO连接
{
	try
	{
		//创建连接对象实例
		m_pConnection.CreateInstance("ADODB.Connection");
		//设置连接字符串
		CString strConnect="DRIVER={Microsoft Access Driver (*.mdb)};uid=;pwd=;DBQ=iRoute.mdb;";
		//使用Open方法连接数据库
		m_pConnection->Open((_bstr_t)strConnect,"","",adModeUnknown);
	}
	catch(_com_error e)
	{
		//捕获并显示错误
		AfxMessageBox(e.Description());
	}
}

void CMicroMecanumDlg::ExitConnect(void)	//关闭记录集和连接
{
	//关闭记录集和连接
    if(m_pRecordset!=NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
}

void CMicroMecanumDlg::Test(void)	// 把数据库数据转为路径数据
{

}
void CMicroMecanumDlg::Database2Route(void)	// 把数据库数据转为路径数据
{
	//定义数据库标准列名称和数据
	CString	StdData[MovementParament]={"1","0","500","0","1000","1000","1000","0","400","1","2","3","4","100","200","300"};
	CString StdName[MovementParament]={"StepID","CommandID","AidDistance","StartSpeed","MaxSpeed","ACC","DACC","CaliDistance","MictoDistance","PortX","PortY","PortZ","PortU","Data1","Data2","Data3",};	
	//连接数据库
	InitADOConn();
	// 定义一个字符串
	CString str;
	//记录集按“StepID”升序排列
	str.Format("select * from %s order by StepID ",TableName);
	_bstr_t bstrSQL = str;
	//创建记录集指针对象实例
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	//打开记录集
	m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);	
	//复位路径段数指针
	Control.mMaxLine = 0;	
	//将记录集指针移动到第一条记录
	m_pRecordset->MoveFirst();
	//显示调试信息
	//TRACE("\n"); 
	//TRACE("开始读取路径数据!\n"); 
	//读取数据库数据并显示在列表控件上
	while(!m_pRecordset->adoEOF)
	{
		// 1.运动方向
		Line[Control.mMaxLine].Derection	= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("CommandID"));	
		// 2.目标距离
		Line[Control.mMaxLine].AidDistance	= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("AidDistance"));	
		// 3.运动类型
		Line[Control.mMaxLine].MotionType	= LineRips;
		// 4.速度倍率
		Line[Control.mMaxLine].SpeedRate	= sRate;
		// 5.启动速度
		Line[Control.mMaxLine].StartSpeed	= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("StartSpeed"));
		// 6.最高速度
		Line[Control.mMaxLine].MaxSpeed		= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("MaxSpeed"));
		// 7.加加速度
		Line[Control.mMaxLine].ACC			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("ACC"));	
		// 8.减加速度
		Line[Control.mMaxLine].DACC			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("DACC"));
		// 9.减速距离
		Line[Control.mMaxLine].ReducePoint	= 0;
		// 10.启动类型
		Line[Control.mMaxLine].StartType	= STAUD;
		// 11.校正距离
		Line[Control.mMaxLine].CaliDistance	= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("CaliDistance"));
		// 12.微步步长	
		Line[Control.mMaxLine].MicroDistance =  atoi((char*)(_bstr_t)m_pRecordset->GetCollect("MictoDistance"));
		// 13.X分轴目标
		Line[Control.mMaxLine].AidX			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("PortX"));
		// 14.Y分轴目标
		Line[Control.mMaxLine].AidY			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("PortY"));
		// 15.Z分轴目标
		Line[Control.mMaxLine].AidZ			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("PortZ"));
		// 16.U分轴目标	
		Line[Control.mMaxLine].AidU			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("PortU"));	
		// 输出调试信息
		//TRACE("第%d#路段数据转换完毕!\n",Control.mMaxLine); 
		// 显示读取的运动参数
		//DisplayPar(&Line[Control.mMaxLine]);		
		//递增计数器
		Control.mMaxLine++;
		//将记录集指针移动到下一条记录
		m_pRecordset->MoveNext();
	}
	//断开数据库连接
	ExitConnect();
	// 输出调试信息
	//TRACE("路径总计生成%d个路段!\n",Control.mMaxLine); 
	//TRACE("\n"); 

 }
//ADO LIST 4 --------------------------------------------------------------------------------------

#define MotionBlock	2 
// mecanum 小车运动控制模块 ---------------------------------------------------
CString CMicroMecanumDlg::DisplayPar(MotionParemeter* line)
{
	CString str;
// 	str.Format("方向%d,距离%d,运动类型0X%X,速度倍率%d,启动速度%d,最高速度%d,加加速度%d,减加速度%d,减速点%d,启动类型0X%X,校正距离%d,微步距离%d,X%d,Y%d,Z%d,U%d,\n",
// 		line->Derection,line->AidDistance,line->MotionType,line->SpeedRate,
// 		line->StartSpeed,line->MaxSpeed,line->ACC,line->DACC,
// 		line->ReducePoint,line->StartType,line->CaliDistance,line->MicroDistance,
// 		line->AidX,line->AidY,line->AidZ,line->AidU);
	str.Format("%d,%d,运动类型0X%X,%d,%d,%d,%d,%d,减速点%d,启动类型0X%X,%d,微步距离%d,X%d,Y%d,Z%d,U%d,\n",
		line->Derection,line->AidDistance,line->MotionType,line->SpeedRate,
		line->StartSpeed,line->MaxSpeed,line->ACC,line->DACC,
		line->ReducePoint,line->StartType,line->CaliDistance,line->MicroDistance,
		line->AidX,line->AidY,line->AidZ,line->AidU);
	//TRACE(str);
	return str;	
}
CString CMicroMecanumDlg::DisplayControl(ControlParemeter* Control)
{
	CString str;
	str.Format("Step%d,eStep%d,StepE%d,MaxStep%d,Line%d,eLine%d,LineE%d,MaxLine%d,P%d,G%d,R%d,D%d,X%.1f,Y%.1f,Z%.1f,U%.1f,\n",
		Control->mStep,Control->mExeStep,Control->mStepEnable,Control->mMaxStep,
		Control->mLine,Control->mExeLine,Control->mLineEnable,Control->mMaxLine,
		Control->mPause,Control->mGoOn,Control->mRevers,Control->mData,
		Control->PlusUnit,Control->xLocation,Control->yLocation,Control->hAngle);
	//TRACE(str);
	return str;	
}

void CMicroMecanumDlg::InitializationSoket()
{
	// 连接服务端发送
	pConnectSocketThread.CreateThread();
	pConnectSocketThread.play();
}

BOOL CMicroMecanumDlg::InitializationMotion(void)
{
	// 初始化运动控制模块
	m_DeviceKey.bus         = PCI_FIELD_IGNORE;
	m_DeviceKey.slot        = PCI_FIELD_IGNORE;
	m_DeviceKey.function    = PCI_FIELD_IGNORE;
	m_DeviceKey.DeviceId    = 0x5201;
	m_DeviceKey.VendorId    = 0x10B5;
	m_DeviceKey.SubDeviceId = PCI_FIELD_IGNORE;
	m_DeviceKey.SubVendorId = PCI_FIELD_IGNORE;
	m_DeviceKey.Revision    = PCI_FIELD_IGNORE;		
	if(!DeviceOpen(&m_DeviceKey, &m_Device))
	{
		MessageBox("没有发现运动控制卡，无法启用运动控制功能!",
			"【严重错误】",
			MB_OK|MB_ICONWARNING|MB_APPLMODAL);		
	}	
//	Init(&m_Device);
//	SetOutput(&m_Device,ZADDR, 0x000F);			//P4 ~ P7输出低电平0，使能四轴驱动器
//	SetOutput(&m_Device,UADDR, 0x000F);			//P4 ~ P7输出低电平0，打开驱动器电源
	MotionInit();
	//InitialPar(&Control,Line,Step,CaliMap);		// 初始化轨迹控制参数
	InitialPar(&Control,Line,Step,CaliMap,pPort,&VportPar);		// 初始化轨迹控制参数

	Control.PlusUnit = Puls2Distance;
	// 设置运动控制专用定时器 	
	SetTimer(1, 50, NULL);						// 初始化设置定时器	
	return TRUE;
}

void CMicroMecanumDlg::MotionInit()	//运动系统复位
{
	Init(&m_Device);
	SetOutput(&m_Device,ZADDR, 0x000F);	//P4 ~ P7输出低电平0，使能四轴驱动器
	SetOutput(&m_Device,UADDR, 0x003F);	//P4 ~ P7输出低电平0，打开驱动器电源
	//return TRUE;
}

inline	BOOL CMicroMecanumDlg::Display(void)			// 运动状态显示
{
	// 计时静态变量
	static U32 SecondDis,MotionTime,LastStep;
	// 合成显示用局部变量
	CString str,tmp;
	// 自动递增计时，U32 约可以计时一个月// SecondDis = 0;
	SecondDis++;
	// 每秒更新
	if ((SecondDis%10) == 0)					
	{
		// 起始时间点
		DWORD SartTime =  GetCurrentTime();			
		// 依据运动情况输出信息
		if (Control.mLineEnable == 0)// 若不运动时
		{
			// 复位运动时间
			MotionTime = 0;
			// 箭头控制
			mARROW = 360;
			// 输出运动信息
			str = "运动停止!\n";
		}
		else// 若运动时
		{	
			// 统计运动时间
			MotionTime++;
			// 箭头控制
			mARROW = Line[Control.mExeLine].Derection;
			//// 合成可显运动参数
			str = DisplayPar(&Line[Control.mExeLine]);		
		}

		if (CameraReady != TRUE)		// 没有相机，直接显示
		{
			// 绘画箭头
			DrawArrow(mARROW);
			//TRACE("CameraReady=%d,用定时器显示图像!\n",CameraReady);
		}
		// 计算位置
		INT32 xLocation = INT32(DOUBLE(GetShiftR(&m_Device,XADDR)) * Puls2Distance);
		// 计算数度
		INT32 xSpeed = INT32(DOUBLE(GetSpeedR(&m_Device,XADDR)) * Puls2Distance);

		// 合成可显数据
		tmp.Format(_T("x=%d毫米,y=%d毫米，速度%d毫米/秒,U轴端口0X%X,"), 
			(int)m_igvInfo.curX,(int)m_igvInfo.curY,xSpeed,GetInput(&m_Device,UADDR));
		// 合成显示数据
		str = tmp + str;
		SystemInfor = tmp;
		// 界面显示数据
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);
	}	
	return TRUE;
}
BOOL CMicroMecanumDlg::StartMotion(void)			// 开始运动
{
	Database2Route();					// 从数据库导入路径数据
	if (Control.mLineEnable == 0)		// 只有上一运动完成，才能开始新的运动；若运动中，不执行任何动作
	{
		Control.mStep = 0;				// 复位步骤指针
		Control.mExeStep = 0;			// 复位微步指针
		Control.mStepEnable = 0;		// 复位禁用自动控制
		Control.mFixStep = 0;			// 复位自动校正步数
		
		Control.mPause		= 0;		// 复位暂停旗标
		Control.mGoOn		= 0;		// 复位继续路径运动旗标
		Control.mRevers		= 0;		// 复位反向旗标
		
		Control.mLine		= 0;		// 复位单段步骤指针
		Control.mExeLine	= 0;		// 复位单段执行指针
		Control.mLineEnable = 0XFF;		// 开始执行微步程序
		FindBarEnable		= TRUE;		// 允许自动识别条码
	}
	return TRUE;
}
BOOL CMicroMecanumDlg::StopMotion(void)			// 停止运动
{
	if (Control.mLineEnable != 0)
	{
		FindBarEnable		= FALSE;			// 禁止自动识别条码
		Control.mStep		= 0;				// 复位步骤指针
		Control.mExeStep	= 0;				// 复位微步指针
		Control.mStepEnable = 0;				// 复位禁用自动控制
		Control.mFixStep = 0;					// 复位自动校正步数
		
		Control.mPause		= 0;				// 复位暂停旗标
		Control.mGoOn		= 0;				// 复位继续路径运动旗标
		Control.mRevers		= 0;				// 复位反向旗标
		
		Control.mLine		= 0;				// 复位单段步骤指针
		Control.mExeLine	= 0;				// 复位单段执行指针
		Control.mLineEnable = 0;				// 复位单段旗标
		Stop(&m_Device,STOP);					// STOP / SDSTP
	}
	return TRUE;
}

void CMicroMecanumDlg::OnTimer(UINT nIDEvent)	//运动控制专用定时器  
{
	// 更新坐标	
	INT32 xLocation = INT32(DOUBLE(GetShiftR(&m_Device,XADDR)) * Puls2Distance);	// 计算位置
	UpdateOwnPosition(abs(xLocation));	// 更新小车当前坐标
	ValidatePosition();					// 检查小车位置是否偏离地标太远
	// 更新小车速度
	INT32 xSpeed = INT32(DOUBLE(GetSpeedR(&m_Device,XADDR)) * Puls2Distance);
	m_igvInfo.Speed = xSpeed;


	if(Control.mLineEnable == 0)
		m_igvInfo.isFree = true;
	else 
	{
		m_igvInfo.MoveDirection = Line[Control.mExeLine].Derection;
		m_igvInfo.isFree = false;

		if (Line[Control.mExeLine].Derection != 370/*校准指令*/ && Line[Control.mExeLine].Derection !=360/*等待指令*/)
		{
			// 只要车子发生了移动，就将状态设为“未校准”
			m_igvInfo.isMarked = false;
			m_igvInfo.MarkID = "";
ofstream ofsLog;				
ofsLog.open("log-4 isFree状态调试.txt",ios::app);
ofsLog<<"移动了,derection="<<Line[Control.mExeLine].Derection<<"mLine="<<Control.mLine<<"\n";
ofsLog.close();

//m_ofsLog2<<"移动了,derection="<<Line[Control.mExeLine].Derection<<"mLine="<<Control.mLine<<"\n";

		}
	}

	// 每隔1秒钟向服务端发送自身的位置和状态信息
	m_iTimeCounter ++;
	if (m_iTimeCounter == 10)
	{
		m_iTimeCounter = 0;
		SendIGVRealTimeInfo();
	}

	// 以下代码用于当自动定向刚开始时，检查当前标是否是中心，如果不是中心地标，
	// 则插入一个向左或右的移动，使其照准中心地标
	if (Control.mLineEnable != 0	&& Control.mLine <= 20 
		&& Line[Control.mExeLine].Derection == 370)	// 开始执行微步程序)
	{
		Control.mPause = 0XFF;	// 进入暂停
		Control.mGoOn = 0XFF;	// 禁止连续

		// 函数后续部分不执行
		Display();									
		CDialog::OnTimer(nIDEvent);
		return;
	}

	// 如果某个Line的动作完成了
	if (Control.mStepEnable == 0 && Control.mLine == 50)	
	{
		// 如果左转或者右转完成，则修改车头朝向m_Heading
		if (Line[Control.mExeLine].Derection == 361 || Line[Control.mExeLine].Derection == 362)	// 如果执行转向
		{
			// 更新车头朝向
			UpdateHeading(Line[Control.mExeLine].Derection, 
				int(Line[Control.mExeLine].AidDistance*Puls2Distance+0.5));	// 取整
		}
		// 如果正在搜索地标，则分搜到和没搜到两种情况处理
		else if ((Line[Control.mExeLine].Derection == 0 || Line[Control.mExeLine].Derection == 90
			|| Line[Control.mExeLine].Derection == 180 || Line[Control.mExeLine].Derection == 270)
			&& m_iCurrentSearchingRoute >= 0)	// 注意这里没有用m_isSearchingMark来判断，
										// 因为与OnTimer()与Bardecoder()函数不太同步，后者是线程驱动的
		{
m_ofsLog2<<"ontimer在执行地标搜索路径中，m_iCurrentSearchingRoute="<<m_iCurrentSearchingRoute<<"\n";
			// 如果搜到了地标
			if (m_isSearchingMarkFound && m_iCurrentSearchingRoute >= 0)
			{
m_ofsLog2<<"ontimer在执行地标搜索路径中搜到地标了，准备执行退回地标动作\n";
				// 获得刚刚执行的搜索路径的参数
				int dir = Line[Control.mExeLine].Derection;
				int dis = int(Line[Control.mExeLine].AidDistance * Puls2Distance + 0.5);
				// 将当前执行的Line指向最后一条SearchLine
				Control.mExeLine += m_iCountOfSearchRoutes - 1 - m_iCurrentSearchingRoute;
				// 修改最后一条搜索路径，让小车退回到该地标，如原来是前进0，则变成后退180,如90则变成270...
				int dir_reverse = 180+dir >= 360?dir-180:180+dir;
				MotionParemeter mp = GetMotionParameters(dir_reverse, dis-m_markFoundLocation);
				Line[Control.mExeLine] = mp;

m_ofsLog2<<"ontimer退回动作：exeLine="<<Control.mExeLine<<",dir="<<dir_reverse
        <<",dis="<<dis-m_markFoundLocation<<",mMaxLine="<<Control.mMaxLine<<"\n";
for(int l = 0; l < Control.mMaxLine; l++)
	m_ofsLog2<<"line "<<l<<",dir="<<Line[l].Derection<<",dis="<<Line[l].AidDistance<<"\n";
m_ofsLog2<<"--------------------\n";
				
				// 继续执行
				Control.mPause = 0;	// 
				Control.mGoOn = 0;	// 
				Control.mLine = 30;

				m_isSearchingMark = false;			// 不执行搜索地标
				m_isSearchingMarkFound = false;		// 重置为未搜索到地标
				m_iCurrentSearchingRoute = -1;				// 重置为-1

			}
			else if (m_iCurrentSearchingRoute >= 0)
			{
m_ofsLog2<<"第"<<m_iCurrentSearchingRoute<<"条路径搜索了，但没找到地标\n";
				// 完成了一条搜索路径，索引加1
				m_iCurrentSearchingRoute++; 

				if (m_iCurrentSearchingRoute >= m_iCountOfSearchRoutes) // 所有搜索路径都走完了
				{
m_ofsLog2<<"所有搜索路径都走完了，没找到地标\n";
					// 此时应该提示出现了异常
					;
					// 小车暂停，使其处于执行370且暂停的状态
					Control.mPause = 0XFF;	// 进入暂停
					Control.mGoOn = 0XFF;	// 禁止连续
					Control.mExeLine++;		// 指向路径搜索后面的370指令（一定有一条这样的指令）
					Control.mLine = 60;		// 直接跳到检查是否暂停的步骤

					//Control.mLineEnable = 0;			// 本次校准动作已执行完
					
					m_isSearchingMark = false;			// 不执行搜索地标
					m_isSearchingMarkFound = false;		// 重置为未搜索到地标
					m_iCurrentSearchingRoute = -1;				// 重置为-1

					m_NotFindMarkTimes370 = 0;			// 重置执行静态校准时未搜索到地标的次数
					m_CaliCount = 0;					// 校准次数清零

					// 函数后续部分不执行
					//Display();									
					//CDialog::OnTimer(nIDEvent);
					//return;
				}
			}
		}
	}

	// 以下代码用于在静态校准完成时暂挺，等待BarDecoder()检查误差
	if (Control.mLineEnable != 0)	// 如果Line数组中的动作（可以多个）没有完成
	{
		if (Control.mStepEnable == 0 && Control.mLine == 50)	// 如果某个Line的动作完成了
		{
			//CString strTemp;
			//strTemp.Format("当前mExeLine:%d,其方向：%d",Control.mExeLine, Line[Control.mExeLine].Derection);
			//MessageBox(strTemp, "", MB_OK);

			// 如果是自动定向指令完成了，则让mRouteGo()暂停，等待BarDecoder()读码函数检查误差
			if (Line[Control.mExeLine].Derection == 370)
			{
				Control.mPause = 0XFF;	// 进入暂停
				Control.mGoOn = 0XFF;	// 禁止连续
			}
		}
	}

	// 起始时间点
	///DWORD SartTime =  GetCurrentTime();	
	// mRouteGo(&m_Device,&Control,Line,Step,CaliMap);	// 自动执行路径行走
	mRouteGo(&m_Device,&Control,Line,Step,CaliMap,pPort,&VportPar);	// 自动执行路径行走

	// Deknock(); //	碰撞检测

	Display();									// 自动通讯及显示运行数据
	CDialog::OnTimer(nIDEvent);
}
	
//mecanum 小车运动控制模块 ---------------------------------------------------

#define RS232cBlock	3 
// 串口通讯模块 -------------------------------------------------------------------------------------------------------
BEGIN_EVENTSINK_MAP(CMicroMecanumDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMicroMecanumDlg)
	ON_EVENT(CMicroMecanumDlg, IDC_MSCOMM1, 1 /* OnComm */, OnOnCommMscomm1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
void CMicroMecanumDlg::InitRs232c(void)	//初始化串口 
{
	//串口设置操作
	if(!ZjCom.Create(NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,IDC_MSCOMM1))  
	{  
		AfxMessageBox("创建MSComm控件失败!"); 
	}  
	else									//创建成功则执行初始化及打开串口操作  
	{  
		//ZjCom.SetCommPort(1);				//指定串口号		
		ZjCom.SetCommPort(8);				//指定串口号 		
		//ZjCom.SetSettings("9600,N,8,1");	//通信参数设置
		//ZjCom.SetSettings("19200,N,8,1");	//通信参数设置		
		ZjCom.SetSettings("115200,N,8,1");//通信参数设置 		
		ZjCom.SetInBufferSize(1024);		//指定接收缓冲区大小  
		ZjCom.SetInBufferCount(0);			//清空接收缓冲区  
		ZjCom.SetInputMode(1);				//设置数据获取方式  
		ZjCom.SetInputLen(0);				//设置每次读取长度，0为全部读取  
		ZjCom.SetRThreshold(16);			//设置接收OnComm事件字符门限值在收到2字节时候触发事件  
		ZjCom.SetPortOpen(1);				//打开串口 
	}	
	CString str;
	str.Format(_T("%d"), ZjCom.GetCommPort());			//Port Nuber to str display
	str = " COM" + str + " ," + ZjCom.GetSettings();	//Port par to str display
	GetDlgItem(IDC_STATIC2)->SetWindowText(str);
}
void CMicroMecanumDlg::OnOnCommMscomm1() 
{	
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len,k;
	BYTE rxdata[1024]; //设置BYTE数组 An 8-bit integerthat is not signed.
	CString str, strtemp, StrDisp;
		
	switch(ZjCom.GetCommEvent())
	{ 
	case 1: // comEvSend发送数据
		TRACE("产生了串口发送中断");
		break;
		
	case 2: // comEvReceive读取数据
		//MessageBox(_T("读取数据事件"), _T("TRACE"), MB_OK);
		variant_inp=ZjCom.GetInput();		//读缓冲区
		safearray_inp=variant_inp;			//VARIANT型变量转换为ColeSafeArray型变量
		len=safearray_inp.GetOneDimSize();	//得到有效数据长度
		if (len>=4)
		{
			strtemp="";
			for(k=0; k<len; k++)
			{
				safearray_inp.GetElement(&k,rxdata+k);	//ColeSafeArray型变量转换为BYTE型数组
				BYTE bt=*(rxdata+k);					//
				strtemp.Format("0X%X ",bt);				//将字符送入临时变量strtemp存放
				StrDisp+=strtemp;						//
				//StrDisp += bt;						//
			}			
			//MessageBox(_T(StrDisp), _T("TRACE"), MB_OK);
		}
		if(rxdata[0] == 0X55 && rxdata[15] == 0XAA)		//命令帧头、帧尾正确，开始解析命令
		{
			StrDisp+="\n";
			GetDlgItem(IDC_STATIC2)->SetWindowText(StrDisp);// 显示接收数据
		}
		break;
		
	default: // 传输事件出错
		ZjCom.SetOutBufferCount(0);
		break;
	}	
}
// 串口通讯模块 -------------------------------------------------------------------------------------------------------

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
	char* sMsg=(char *)lParam;	
	Command2Route(sMsg);
	
	// 储存接收的行走指令
	for (int i = 0; i< mTotal; i++)
	{
		Temp_Line[i].ACC = Line[i].ACC;
		Temp_Line[i].AidDistance = Line[i].AidDistance;
		Temp_Line[i].AidU = Line[i].AidU;
		Temp_Line[i].AidX = Line[i].AidX;
		Temp_Line[i].AidY = Line[i].AidY;
		Temp_Line[i].AidZ = Line[i].AidZ;
		Temp_Line[i].CaliDistance = Line[i].CaliDistance;
		Temp_Line[i].DACC = Line[i].DACC;
		Temp_Line[i].Derection = Line[i].Derection;
		Temp_Line[i].MaxSpeed = Line[i].MaxSpeed;
		Temp_Line[i].MicroDistance = Line[i].MicroDistance;
		Temp_Line[i].MotionType = Line[i].MotionType;
		Temp_Line[i].ReducePoint = Line[i].ReducePoint;
		Temp_Line[i].SpeedRate = Line[i].SpeedRate;
		Temp_Line[i].StartSpeed = Line[i].StartSpeed;
		Temp_Line[i].StartType = Line[i].StartType;
	}

	if (Control.mLineEnable == 0)		// 只有上一运动完成，才能开始新的运动；若运动中，不执行任何动作
	{
		Control.mStep = 0;				// 复位步骤指针
		Control.mExeStep = 0;			// 复位微步指针
		Control.mStepEnable = 0;		// 复位禁用自动控制
		Control.mFixStep = 0;			// 复位自动校正步数
		
		Control.mPause		= 0;		// 复位暂停旗标
		Control.mGoOn		= 0;		// 复位继续路径运动旗标
		Control.mRevers		= 0;		// 复位反向旗标
		
		Control.mLine		= 0;		// 复位单段步骤指针
		Control.mExeLine	= 0;		// 复位单段执行指针
		Control.mLineEnable = 0XFF;		// 开始执行微步程序
		FindBarEnable		= TRUE;		// 允许自动识别条码
	}

	delete sMsg;
	return true;
}


// Camera 图像显示线程------------------------------------------------------------------
// After the process thread generate a bmp file,this function load picture and show it 
LRESULT CMicroMecanumDlg::ProcessImage(WPARAM wParam, LPARAM lParam)
{
	struct BARINFO{
		bool isSuc;
		CString barID;
		double x;
		double y;
		double angle;
	} * pBarInfo;
	pBarInfo = (BARINFO *)lParam;

	isDecodeSuccess = pBarInfo->isSuc;
	BarCodeStr = pBarInfo->barID;
	BarXlocation = pBarInfo->x;
	BarYlocation = pBarInfo->y;
	BarAngle = pBarInfo->angle;
	
	BarDecoder();

	return true;
}

// 相机模块 -------------------------------------------------------------------------------------------------------

#define EvisionBlock	5 
// 视觉模块 -------------------------------------------------------------------------------------------------------

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
		}
	}
	
	return bar;
}


// 函数功能包括调用Evision库函数，打开一个C24图像转为BW8格式、解码，并在文本框显示
void CMicroMecanumDlg::BarDecoder()						// 调用Evision解码
{	
	BarDecodeInfo = "";

	// 如果解码信息正确
	if (isDecodeSuccess)
	{
		// -----------记录读到的地标--pc
		char tmp[64];
		time_t t = time( 0 ); 			
		strftime( tmp, sizeof(tmp), "%X",localtime(&t) );

		ofstream ofsLog7;
		Json::Value msgBody;
						
		ofsLog7.open("log-7记录读到的地标.txt",ios::app);
		MARK* temp_pMark = GetMarkInfo(BarCodeStr);
		msgBody["MarkId1"] = (char*)(_bstr_t)(temp_pMark->barCode).GetBuffer(0);
		Temp_BarCodeID = msgBody["MarkId1"].asString();
		if(Temp_BarCodeID == Temp_LastBarCodeid)
		{
		}
		else
		{
			ofsLog7<<tmp<<"  "<<Temp_BarCodeID<<"\n"; 
			Temp_LastBarCodeid = Temp_BarCodeID;
		} 
		// ofsLog7.close();
		// -----------记录读到的地标结束

		// 统计正确解码次数		
		DecodeSuccesTime++;
		// 计算屏幕像素到实际距离换算系数，单位：毫米/像素
		double CaliK = BarLenth/640;
		// 读取条码X坐标轴长度
		BarXlenth = 640 * CaliK;
		// 读取条码Y坐标轴长度
		BarYlenth = 480 * CaliK;

		m_NotFindMarkTimes370 = 0;
		// 如果正在执行地标搜索，则说明找到了
		if (m_isSearchingMark)
		{
			m_isSearchingMark = false;		// 不再搜索地标
			m_isSearchingMarkFound = TRUE;	// 表明已经搜到地标了，接下来在OnTimer里处理

			INT32 dis = INT32(DOUBLE(GetShiftR(&m_Device,XADDR)) * Puls2Distance);
			// 记录下车子走的距离，以便车子走完这一段搜索路径后退回去对准地标
			m_markFoundLocation = fabs(dis);	// 取绝对值，因为后退时dis是负值	
m_ofsLog2<<"地标已搜到,m_markFoundLocation="<<m_markFoundLocation<<",m_iCurrentSearchingRoute="<<m_iCurrentSearchingRoute<<"\n";
		}

		// 如果读码正确，并在进行复位（每次行走之前都会进行复位）
		if (FindBarEnable == TRUE && isDecodeSuccess == TRUE && Control.mMaxLine >= 1)
		{			
ofstream ofsLog1;				
ofsLog1.open("log-1地标校准过程.txt",ios::app);
ofsLog1<<"-00 读到地标，指令="<<Line[Control.mExeLine].Derection
	<<",mExeLine="<<Control.mExeLine<<",mMaxLine="<<Control.mMaxLine<<",mLine"<<Control.mLine
	<<",mPause="<<Control.mPause<<"\n";			
			if (Line[Control.mExeLine].Derection == 370 )
			{
				// 记录第一个轨迹校准点，确保在执行一条Line的起始点时必须将nCheckPoints置为1
				// 且为CheckMarks[0]和CheckPoints[0]赋值
				nCheckPoints = 1;
				MARK* pMark = GetMarkInfo(BarCodeStr);
				CheckMarks[0].barCode = pMark->barCode;
				CheckMarks[0].dir = pMark->dir;
				CheckMarks[0].x = pMark->x;
				CheckMarks[0].y = pMark->y;

				CheckPoints[0].distance = 0;
				CheckPoints[0].deltax = 0;
				CheckPoints[0].deltay = 0;
				CheckPoints[0].deltaa = -BarAngle;	// 偏移角，摄像机获得的值要反过来用
				CheckPoints[0].deltah = -BarAngle;	//0;
				CheckPoints[0].speed = 0;
				CheckPoints[0].adjustx = 0;
				CheckPoints[0].adjusty = 0;
//ofstream ofsLog1;				
//ofsLog1.open("log-1地标校准过程.txt",ios::app);
ofsLog1<<"-0 读到地标，开始静态校准 mLineEnable="<<Control.mLineEnable<<",mLine="<<Control.mLine<<",mPause="<<Control.mPause<<"\n";

				// 如果该370指令刚开始执行而且处于暂停状态，则检查是否位于中心地标
				if (Control.mLineEnable !=0 /*== 0XFF*/	&& Control.mLine <= 20 
					&& Control.mPause == 0XFF)	
				{ 
					// 查找这个地标所在的中心地标
					CString sBarCentral = FindCentralMark(BarCodeStr);
ofsLog1<<"---01 读到的地标bar="<<BarCodeStr.GetBuffer(0)<<",中心地标barCenter="<<sBarCentral.GetBuffer(0)<<"---------\n"; 					
					if (sBarCentral == BarCodeStr) // 本身就是中心地标，则继续
					{
						// mRouteGo()中直接跳到30，继续执行
						Control.mLine = 30;
						Control.mPause =0;			// 继续执行
						Control.mGoOn =0;			// 继续执行		
					}
					else	// 在Line中插入一段偏移
					{
						MARK* pMarkCentral = GetMarkInfo(sBarCentral);
						if (pMarkCentral != NULL)
						{
							MotionParemeter MP = GetCaliLine(pMark,pMarkCentral,-BarXlocation, -BarYlocation,m_igvInfo.Heading);
							
							for(int k = (int)Control.mMaxLine-1; k >=(int)Control.mExeLine; k--)
							{
								Line[k+1] = Line[k];
							}
							//Line[1] = Line[0];
							Line[Control.mExeLine] = MP;

								
							// 递增计数器
							Control.mMaxLine++;
ofsLog1<<"-02 在Line中增加一段偏移，参数为aiddistance="<<MP.AidDistance<<",maxLine="<<Control.mMaxLine<<",exeLine="<<Control.mExeLine<<"\n"; 					
							// 继续执行，此时应该执行新增加的这个Line
							Control.mPause =0;			// 继续执行
							Control.mGoOn =0;			// 继续执行		

						}
					}
				}

				// 如果该370指令已经执行完了而且处于暂停状态，则检查偏移量
				if (Control.mStepEnable == 0 && Control.mLine == 60 && Control.mPause == 0XFF)
				{
ofsLog1<<"-1 单次370指令已经执行完了，检查偏移量为：BarX="<<BarXlocation<<",BarY="<<BarYlocation<<",angle="<<BarAngle<<"\n";
					bool bAcurate = false;	// 静态校准是否精确
					// 坐标误差小于1mm，或者角度小于1.0度
					if (fabs(BarXlocation) < X_Y_TOLERANCE_HIGH && fabs(BarYlocation)<X_Y_TOLERANCE_HIGH
						&& fabs(BarAngle) < ANGLE_TOLERANCE_HIGH)
					{
						m_CaliCount ++;	// 校准次数加1
ofsLog1<<"-2 基本（也可能完全）精确，静态校准次数加1，等于"<<m_CaliCount<<"\n";
					}
					// 坐标误差小于0.5mm，或者角度小于0.5度
					if (fabs(BarXlocation) < X_Y_TOLERANCE_LOW && fabs(BarYlocation)<X_Y_TOLERANCE_LOW
																&& fabs(BarAngle) < ANGLE_TOLERANCE_LOW)
					{
						m_CaliCount = 0;
						bAcurate = true;
					}
					if (bAcurate || m_CaliCount == MAX_ReCheckMarkTimes370)	// 如果已经校准精确了，或者基本校准超过了两次，直接跳到下一个直行或者拐弯或者后退指令
					{
						// 更新是否校准的状态，记录校准的地标ID
						m_igvInfo.isMarked = true;
						m_igvInfo.MarkID = BarCodeStr;
ofstream ofsLog;				
ofsLog.open("log-4 isFree状态调试.txt",ios::app);
ofsLog<<"校准了，MarkID="<<(char*)(_bstr_t)m_igvInfo.MarkID.GetBuffer(0)<<"\n";
ofsLog.close();

//ofsLog1<<"校准了，MarkID="<<(char*)(_bstr_t)m_igvInfo.MarkID.GetBuffer(0)<<"\n";

						// 更新坐标
						m_igvInfo.curX = pMark->x;
						m_igvInfo.curY = pMark->y;
						m_igvInfo.preX = m_igvInfo.curX;
						m_igvInfo.preY = m_igvInfo.curY;
ofsLog1<<"静态校准成功，此处更新坐标1，curx="<<m_igvInfo.curX<<",cury="<<m_igvInfo.curY<<",prex="<<m_igvInfo.preX<<",prey="<<m_igvInfo.preY<<"\n";
ofsLog1<<"-------------------------------------------------------------\n\n\n";						
						m_CaliCount = 0;	// 校准次数清零
						if (Control.mRevers == 0)	// 正向执行Line数组中指令时
						{
							int exeLine = Control.mExeLine;
							while (exeLine < Control.mMaxLine)
							{
								exeLine++;	// Line指针指向下一个Line
								if (exeLine == Control.mMaxLine)	// 后面没有其它Line要执行了
								{
									Control.mExeLine = 0;
									Control.mLine = 80;			// 此句执行后后面两行会在mRouteGo()中执行，因此注释掉
									//Control.mGoOn =0;			// 继续执行
									//Control.mGoOn =0;			// 继续执行

									exeLine = Control.mMaxLine;	// 相当于结束while循环
								}
								else	// 如果后面还有直行或者旋转，则继续执行
								{
									int dir = Line[exeLine].Derection;
									if( dir == 0 || dir == 90 || dir == 180 || dir == 270
										|| dir == 361 || dir == 362)
									{
										Control.mExeLine = exeLine;
										Control.mLine = 20;			// 
										Control.mPause =0;			// 继续执行
										Control.mGoOn =0;			// 继续执行
										exeLine = Control.mMaxLine;	// 相当于结束while循环
									}
								}
							}
						}
					}
					else	// 自动校准精度达不到要求，还有误差，需继续执行此指令
					{
ofsLog1<<"-8 自动校准精度达不到要求，还有误差，需继续执行此370指令\n";
						
						Control.mLine = 20;			// mRouteGo()中直接跳到20，不执行Control.mExeLine++
						Control.mPause =0;			// 继续执行
						Control.mGoOn =0;			// 继续执行
					}
				}

ofsLog1.close();

			}		

			// 如果读码正确，且在执行一条Line，添加一个校准点
			else if (Line[Control.mExeLine].Derection == 0 || Line[Control.mExeLine].Derection == 180 
				&& nCheckPoints >= 1 /*确保第一个点已经读到*/)
			{
				// 增加一个轨迹校准点
				
				// 计算与上一个读到的地标之间的距离
				MARK* pMark = GetMarkInfo(BarCodeStr);
				double disX = pMark->x - CheckMarks[nCheckPoints-1].x;
				double disY = pMark->y - CheckMarks[nCheckPoints-1].y;
				double dis = fabs(disX) + fabs(disY);	// 两个地标坐标差

				
				double markDeviation = 0;	// 读到的地标在垂直于运动方向的偏移量
				double disX0 = pMark->x - CheckMarks[0].x;
				double disY0 = pMark->y - CheckMarks[0].y;

				// 车头朝Y轴正向，前进
				if (m_igvInfo.Heading ==0 && Line[Control.mExeLine].Derection == 0)
				{
					markDeviation = disX0;
					//BarAngle = BarAngle;	// 前进为正
				}
				// 车头朝Y轴正向，后退
				else if (m_igvInfo.Heading ==0 && Line[Control.mExeLine].Derection == 180)
				{
					markDeviation = -disX0;
					//BarAngle = -BarAngle;	// 后退为负
				}
				// 车头朝Y轴负向，前进
				else if (m_igvInfo.Heading ==180 && Line[Control.mExeLine].Derection == 0)
				{
					markDeviation = -disX0;
					//BarAngle = BarAngle;	// 前进为正
				}
				// 车头朝Y轴负向，后退
				else if (m_igvInfo.Heading ==180 && Line[Control.mExeLine].Derection == 180)
				{
					markDeviation = disX0;
					//BarAngle = -BarAngle;	// 后退为负
				}				
				// 与前面类似
				else if (m_igvInfo.Heading ==90 && Line[Control.mExeLine].Derection == 0)
				{
					markDeviation = -disY0;
					//BarAngle = BarAngle;	// 前进为正
				}
				else if (m_igvInfo.Heading ==90 && Line[Control.mExeLine].Derection == 180)
				{
					markDeviation = disY0;
					//BarAngle = -BarAngle;	// 后退为负
				}
				else if (m_igvInfo.Heading ==270 && Line[Control.mExeLine].Derection == 0)
				{
					markDeviation = disY0;
					//BarAngle = BarAngle;	// 前进为正
				}
				else if (m_igvInfo.Heading ==270 && Line[Control.mExeLine].Derection == 180)
				{
					markDeviation = -disY0;
					//BarAngle = -BarAngle;	// 后退为负
				}

				//如果距离前一个标距离小于400mm，则忽略
				if (dis > 400)
				{
					CheckMarks[nCheckPoints].barCode = pMark->barCode;
					CheckMarks[nCheckPoints].dir = pMark->dir;
					CheckMarks[nCheckPoints].x = pMark->x;
					CheckMarks[nCheckPoints].y = pMark->y;
					
					CheckPoints[nCheckPoints].distance = CheckPoints[nCheckPoints-1].distance + dis;
					if (Line[Control.mExeLine].Derection == 0)	// 前进
						CheckPoints[nCheckPoints].deltax =	-BarXlocation 	// 摄像机获得的值要反过来用
															+ markDeviation;// 加上这个标本身与Line的偏移量
					else	// 后退
						CheckPoints[nCheckPoints].deltax =	BarXlocation	// 摄像机获得的值要反过来用，负负得正
															+ markDeviation;// 加上这个标本身与Line的偏移量

					CheckPoints[nCheckPoints].deltay = -BarYlocation;	// 摄像机获得的值要反过来用
					CheckPoints[nCheckPoints].deltaa = -BarAngle;	// 摄像机获得的值要反过来用
					CheckPoints[nCheckPoints].speed = m_igvInfo.Speed;	// 摄像机获得的值要反过来用

					// adjustx, adjusty, deltaH留到FixTrack()函数去更新

					nCheckPoints++;
				}
				else
					isDecodeSuccess = false;//后面不执行校准动作
			}
		}

		//--------------------------------------------------------------
	
		// 如果读码正确，动态校正轨迹
		if ((FindBarEnable == TRUE) && isDecodeSuccess/*&& (BarCodeStr == "12345")*/
			&& (Control.mLineEnable != 0) && (Control.mExeStep > 2))
		{

// 输出Control.mFixStep和Control.mExeStep；
ofsLog7<<Control.mFixStep<<" "<<Control.mExeStep<<"\n";
ofsLog7.close();

			if (Control.mFixStep != Control.mExeStep)
			{
				TRACE("FindBarEnable=%d,BarCodeStr=%d,mLineEnable=%d,mExeStep=%d,Shift=%d,\n",
					FindBarEnable,atoi(BarCodeStr),Control.mLineEnable,Control.mExeStep,
					INT32(DOUBLE(GetShiftR(&m_Device,XADDR)) * Puls2Distance));
				//StopMotion();	// 如果读码正确，立即停车
				// 依据条码信息，校准行车轨迹
				DOUBLE dR = 215;
				FixTrack(dR,BarXlocation,BarYlocation,BarAngle,&Control,Step);
				// 记忆校准微步位置
				Control.mFixStep = Control.mExeStep;
			}
		}		
		//精简格式输出条码信息
		// 读取条码值
		str = BarCodeStr;									
		// 输出条码中心X轴坐标
		str1.Format(_T(",%4.1fmm,"), BarXlocation);			// 格式化输出字符
		str += str1;										
		// 输出条码中心Y轴坐标
		str1.Format(_T("%4.1fmm,"), BarYlocation);			// 格式化输出字符
		str += str1;										
		// 输出条码偏转角度
		str1.Format(_T("%4.2fdeg,"), BarAngle);				// 格式化输出字符
		str += str1;										
		// 输出帧率
		str1.Format(_T("%dFPS,"), FramePerSecond);			// 格式化输出字符
		str += str1;										
		// 把需要显示的信息传递到显示变量
		BarDecodeInfo = BarDecodeInfo + ", " + str;							
	} 
	else	// 解码不正确
	{
		// 如果执行370指令无法读到标，则计数器加1
		// 设mLine<60是为了防止这种情况：搜索路径执行完了但没找到地标（mLine=60,mPause=0XFF），这时不需要启动搜索路径
		if (Line[Control.mExeLine].Derection == 370 && Control.mLine < 60)	
		{
			m_NotFindMarkTimes370++;
CString s;
s.Format("未发现地标次数：%d",m_NotFindMarkTimes370);
GetDlgItem(IDC_STATIC1)->SetWindowText(s);
			// 如果解码失败超过了一定的次数，则启动局部地标搜索		
			if (m_NotFindMarkTimes370 >= MAX_NotFindMarkTimes370)
			{
				// 计算搜索路径（共Num_SearchLines条）
				CalculateSearchLines();
				// 将搜索路径插入到现有路径中
				for(int k = (int)Control.mMaxLine-1; k >=(int)Control.mExeLine; k--)
				{
					Line[k+m_iCountOfSearchRoutes] = Line[k];
				}
				for (int i = 0; i<m_iCountOfSearchRoutes;i++)
					Line[Control.mExeLine+i] = SearchLines[i];
					
				// 递增计数器
				Control.mMaxLine+=m_iCountOfSearchRoutes;
				// 继续执行，此时应该执行新增加的这个Line
				Control.mPause =0;			// 继续执行
				Control.mGoOn =0;			// 继续执行
				Control.mLine = 30;

				m_iCurrentSearchingRoute = 0;		// 设置当前正在执行的地标搜索路径索引, -1表示未进行地标搜索
				m_isSearchingMark = true;	// 启动搜索
				m_isSearchingMarkFound = false;	// 尚未搜索到
				m_NotFindMarkTimes370 = 0;

m_ofsLog2<<"启动地标搜索\n";
			}
		}
		
		//统计不正确解码次数
		DecodeFaileTime++;
		str = "";											// 清空字符串
		// 输出帧率
		str1.Format(_T("%dFPS,"), FramePerSecond);			// 格式化输出字符
		str += str1;										
		// 解码失败百分比
		double FailPercent = 100 * float(DecodeFaileTime)/float(DecodeSuccesTime + DecodeFaileTime);
		str1.Format(_T("%2.2fFPC,"), FailPercent);			// 格式化输出字符
		str += str1;
		// 把需要显示的信息传递到显示变量
		//BarDecodeInfo = " 【 解码失败 !!! 】," + str + DisplayControl(&Control);
		// 把需要显示的信息传递到显示变量
		BarDecodeInfo = " 【 解码失败 !!! 】," + BarDecodeInfo + ", " + str;
		//解码信息不正确
		isDecodeSuccess = FALSE;	
		//复位解码信息
		BarXlocation = 0;		// 条码中心X轴坐标
		BarYlocation = 0;		// 条码中心Y轴坐标
		BarXlenth = 0;			// 条码X坐标轴长度
		BarYlenth = 0;			// 条码Y坐标轴长度
		BarAngle = 0;			// 条码偏转角度			
	}

	// 为校准赋值
	Control.RoundUnit = TurnRound;
	Control.DeltH = BarAngle;
	Control.DeltX = BarXlocation;
	Control.DeltY = BarYlocation;
	FinishedBarDecode = TRUE;								// 完成解码

	//在图像上同步显示字符等
	DrawArrow(mARROW);

}
// 视觉模块 -------------------------------------------------------------------------------------------------------

#define DrawBlock	6 
// 运动标志绘画模块 ---------------------------------------------------------------------------------------------------
BOOL CMicroMecanumDlg::DrawArrow(UINT32 CommandDir)	// 绘画运动方向箭头
{
	CDC *pDC=NULL;								//定义一个空的Device Concent
	CRect rect;									//定义矩形
	CWnd *pWnd2 = GetDlgItem(IDC_STATICP1);		//获取绘画窗口		
	if(pWnd2)									//若窗口存在	
	{
		pWnd2->GetClientRect(&rect);			//设定绘画矩形
		pDC = pWnd2->GetDC();					//设定绘图目标设备
		//计算绘图参数	
		INT32 w,h,x0,y0;
		x0 = rect.Width()/2;					// 图标X坐标
		y0 = rect.Height()/8;					// 图标Y坐标
		w = rect.Width()/16;					// 图标宽度
		h = rect.Height()/16;					// 图标高度
		if (w >= h)								// 令图标宽度 = 高度 
		{
			w = h;
		}
		else
		{
			h = w;
		}

		POINT pt[3];

		CRect Round;
		CPoint StartP,EndP;		
		Round.left		= x0-w;
		Round.right		= x0+w;
		Round.top		= y0-h;
		Round.bottom	= y0+h;

		if (CameraReady != TRUE)				// 若不显示图像，则设置背景色
		{
			pDC->FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(236,233,216));	//系统默认背景色
		}
		// 画十字线
		CPen pen2; 
		pen2.CreatePen(PS_SOLID,1,RGB(0,0,127));			// 创建蓝色画笔
		CPen* pOldPen2= pDC->SelectObject(&pen2);			// 选择画笔
		// 竖线
		pDC->MoveTo(rect.Width()/2,0);						// 移动到起点
		pDC->LineTo(rect.Width()/2,rect.Height());			// 从起点画线到终点
		// 横线		
		pDC->MoveTo(0,rect.Height()/2);						// 移动到起点
		pDC->LineTo(rect.Width(),rect.Height()/2);			// 从起点画线到终点			
		pDC->SelectObject(&pOldPen2);						// 释放画笔

		// 定义一支画笔
		CPen pen; 											// 定义画笔
		pen.CreatePen(PS_SOLID,1,ARROWRED);					// 创建画笔
		CPen* pOldPen= pDC->SelectObject(&pen);				// 绑定画笔		
		// 定义一个画刷
		CBrush brush;  										// 定义画刷
		brush.CreateSolidBrush(ARROWRED);					// 创建画刷
		CBrush* pOldBrush = pDC->SelectObject(&brush);		// 绑定画刷		
		if (CommandDir <= 360)								// 画刷画实心四边形
		{
			pDC->FillSolidRect(x0-w/2,y0,w,h,ARROWRED);		// 画刷画实心四边形// (x,y,w,h,RGB(200,0,0))
		}
		// 解析方向
		switch(CommandDir)
		{
		case 0:
			pt[0].x = x0-w;
			pt[0].y = y0;
			pt[1].x = x0+w;
			pt[1].y = y0;
			pt[2].x = x0;
			pt[2].y = y0-h;
			pDC->Polygon(pt,3);								// 画刷画实心三边形	
			break;
		case 90:
			pt[0].x = x0+w/2;
			pt[0].y = y0-h/2;
			pt[1].x = x0+w/2;
			pt[1].y = y0+3*h/2;
			pt[2].x = x0+3*w/2;
			pt[2].y = y0+h/2;
			pDC->Polygon(pt,3);								// 画刷画实心三边形	
			break;
		case 180:
			pt[0].x = x0+w;
			pt[0].y = y0+h;
			pt[1].x = x0-w;
			pt[1].y = y0+h;
			pt[2].x = x0;
			pt[2].y = y0+2*h;
			pDC->Polygon(pt,3);								// 画刷画实心三边形	
			break;
		case 270:
			pt[0].x = x0-w/2;
			pt[0].y = y0+3*h/2;
			pt[1].x = x0-w/2;
			pt[1].y = y0-h/2;
			pt[2].x = x0-3*h/2;
			pt[2].y = y0+h/2;
			pDC->Polygon(pt,3);								// 画刷画实心三边形	
			break;
		case 360:
			pDC->FillSolidRect(x0-w,y0-h,2*w,2*h,ARROWBLU);	// 画刷画实心四边形
			break;
		case 361:
			pt[0].x = x0;
			pt[0].y = y0;
			pt[1].x = x0;
			pt[1].y = y0-2*h;
			pt[2].x = x0-w;
			pt[2].y = y0-h;
			pDC->Polygon(pt,3);								// 画刷画实心三边形
// 			StartP.x = x0-w;
// 			StartP.y = y0;			
// 			EndP.x = x0;
// 			EndP.y = y0-h;
// 			pDC->Arc(Round,StartP,EndP);					// 画圆弧			
			break;
		case 362:
			pt[0].x = x0;
			pt[0].y = y0;
			pt[1].x = x0;
			pt[1].y = y0-2*h;
			pt[2].x = x0+w;
			pt[2].y = y0-h;
			pDC->Polygon(pt,3);								// 画刷画实心三边形
// 			StartP.x = x0;
// 			StartP.y = y0-h;			
// 			EndP.x = x0+w;
// 			EndP.y = y0;
// 			pDC->Arc(Round,StartP,EndP);					// 画圆弧			
			break;
		case 370:
			pt[0].x = x0;
			pt[0].y = y0-h;
			pt[1].x = x0;
			pt[1].y = y0+h;
			pt[2].x = x0-w;
			pt[2].y = y0;
			pDC->Polygon(pt,3);								// 画刷画实心三边形	
			pt[0].x = x0;
			pt[0].y = y0-h;
			pt[1].x = x0;
			pt[1].y = y0+h;
			pt[2].x = x0+w;
			pt[2].y = y0;
			pDC->Polygon(pt,3);								// 画刷画实心三边形
			break;
		case 372:
			pDC->Ellipse(x0-w,y0-h,x0+w,y0+h);				// 画刷画实心圆
			break;			
		default:
			break;
		}
		pDC->SelectObject(&pOldPen);						// 释放画笔
		pDC->SelectObject(&pOldBrush);						// 释放画刷	
		
		CPen pen1; 											// 定义画笔
		pen1.CreatePen(PS_SOLID,10,ARROWRED);				// 创建画笔
		pOldPen= pDC->SelectObject(&pen1);					// 绑定画笔	
		if (CommandDir == 361)
		{
			StartP.x = x0-w;
			StartP.y = y0;			
			EndP.x = x0;
			EndP.y = y0-h;
			pDC->Arc(Round,StartP,EndP);					// 画圆弧
		} 		
		if (CommandDir == 362)		// 右旋
		{
			StartP.x = x0;
			StartP.y = y0-h;			
			EndP.x = x0+w;
			EndP.y = y0;
			pDC->Arc(Round,StartP,EndP);					// 画圆弧
		} 		
		pDC->SelectObject(&pOldPen);						// 释放画笔
		
		CTime curtime=CTime::GetCurrentTime();
		CString TimeMark = curtime.Format("%Y-%m-%d-%H:%M:%S ");	//采集并计算系统当前时间
		
		CString MotionIfo;
		if (Control.mLineEnable != 0)				//合成当前运动信息
		{
			MotionIfo.Format(" 路径%d个路段，%d#路段%d个微步，正在执行第%d微步%d#命令!",Control.mMaxLine,Control.mExeLine,Control.mMaxStep,Control.mExeStep,mARROW);
		}
		else
		{
			MotionIfo.Format(" 完成路径总计耗时%d秒!",Control.mRevers);				
		}
		//TimeMark += MotionIfo + DisplayControl(&Control);
		TimeMark += MotionIfo;
	
		CPen pen3; 											// 自定义画笔
		pen3.CreatePen(PS_SOLID,1,RGB(0,255,0));			// 创建画笔
		CPen* pOldPen3= pDC->SelectObject(&pen3);			// Select a green pen
		m_BarCode.Draw(pDC->GetSafeHdc(), INS_DRAW_ACTUAL);	// Draw the found barcode frame
		pDC->TextOut(10, 360, DisplayControl(&Control), strlen(DisplayControl(&Control)));	// 显示小车状态信息
		pDC->TextOut(10, 380, DisplayPar(&Line[Control.mExeLine]), strlen(DisplayPar(&Line[Control.mExeLine])));
		pDC->TextOut(10, 400, DisplayPar(&Line[Control.mExeStep]), strlen(DisplayPar(&Line[Control.mExeStep])));
		pDC->TextOut(10, 420, TimeMark, strlen(TimeMark));	// 显示小车状态信息	
		pDC->TextOut(10, 440, SystemInfor, strlen(SystemInfor));	// 显示小车状态信息	
		pDC->TextOut(10, 460, BarDecodeInfo, strlen(BarDecodeInfo));	// 显示解码信息
		pDC->SelectObject(&pOldPen3);						// Put back old pen	
		
		ReleaseDC(pDC);										// 释放绘图目标设备	
	}
	//RunLife("S");	//统计程序运行时间
	return TRUE;	// 计算匀速运动位移
}
// end 运动标志绘画模块 ---------------------------------------------------------------------------------------------------
// 无用
BOOL CMicroMecanumDlg::DeMotionError(DOUBLE *dR,DOUBLE *X,DOUBLE *Y,DOUBLE *L,DOUBLE *dL)	//运动误差修正计算公式
{
	DOUBLE a,b,k,r,l,rd,ld;	// 定义局部变量

	rd = *dR;	// 输入半径差mm
	a = *X;		// 输入X坐标mm
	b = *Y;		// 输入Y坐标mm

	if (a != 0)
	{
		r = (a*a + b*b)/(2*a);	// 半径mm
		k = asin(b/r);			// 弧度Rad
		l = r*k;				// 弧长mm
		ld = rd*k;				// 弧差mm 
	}
	else
	{
		l = b;					// 弧长mm
		ld = 0;					// 弧差mm 
	}

	*L = l;			// 输出弧长mm
	*dL = ld;		// 输出弧差mm 
	//*dL = -1 * ld;	// 输出弧差mm 
	return TRUE;// 完成功能
}

BOOL CMicroMecanumDlg::DeMachineError(DOUBLE &R,DOUBLE &D,DOUBLE &dR,DOUBLE &L,DOUBLE &dL)	//机械误差修正计算公式
{
	DOUBLE k;		// 定义局部变量

	k = asin(D/R);	// 计算弧度Rad
	L = R * k;		// 输出弧长mm
	dL= dR * k; 	// 输出弧差mm 

	return TRUE;	// 完成功能	
}

// 无用
BOOL CMicroMecanumDlg::DeAngleError(DOUBLE *rL,DOUBLE *X,DOUBLE *Y,DOUBLE *A,DOUBLE *dL)	//通过角度修正运动误差
{
	DOUBLE a,b,c,k,r,l,dl;	// 定义局部变量

	l = *rL;	// 输入圆周长mm
	a = *X;		// 输入X坐标mm
	b = *Y;		// 输入Y坐标mm
	c = *A;		// 输入角度单位度

	// 计算半径
	r = l/(2*3.1415926);	// 半径mm
	// 计算偏航角
	k = (3.1415926 / 2) - acos(a/r);	// 弧度Rad
	dl = r*k;				// 弧差mm 
	*dL = -1 * dl;			// 反向输出弧差mm 
	TRACE("x=%.3f,r=%.3f,K=%.3f,A=%.3f,dl=%.3f,\n",a,r,(k * 180)/3.1415926,c,dl);
	return TRUE;// 完成功能
}

// 动态校准
BOOL CMicroMecanumDlg::FixTrack(DOUBLE &dR,DOUBLE &dX,DOUBLE &dY,DOUBLE &dA,
								  ControlParemeter* Control,MotionParemeter* step)	
{
	// 打开日志文件
ofstream ofsLog;
ofsLog.open("log-0 轨迹校准过程.txt",ios::app);

	// 微步长度
	int MicroDistance = Line[Control->mExeLine].MicroDistance * Puls2Distance;	//单位毫米
	// Line的长度
	int LineLength = Line[Control->mExeLine].AidDistance * Puls2Distance;		// 单位毫米

	// 目前正在执行的微步序号是“mExeStep-3”，“mExeStep-2”和“mExeStep-1”都在堆栈中，
	// 即从“mExeStep”开始可以用于校准
	int StartFixStep = Control->mExeStep;//+1;
	// 最后一个微步序号是“mMaxStep-1”，这一步的长度小于或等于标准步长:
	int EndFixStep = Control->mMaxStep -1 -1;	//即-2，最后一步不校准

	// 判定执行校准的微步是否超出了最后一个可校准的微步
	if (StartFixStep <= EndFixStep)
	{
		double x1, y1;	// 上一个读到的校准点坐标（以出发点为原点，行进方向为Y轴正向的坐标，非地图坐标）
		double x2, y2;	// 当前读到的校准点的坐标（坐标轴同上）
		double x3, y3;	// 开始校准点的坐标（坐标轴同上）
		double x4, y4;	// Line终点的地坐标（坐标轴同上）

		x1 = CheckPoints[nCheckPoints-2].adjustx;
		y1 = CheckPoints[nCheckPoints-2].adjusty;

		x2 = CheckPoints[nCheckPoints-1].deltax;
		y2 = CheckPoints[nCheckPoints-1].distance;

		y3 = StartFixStep * MicroDistance;
		x3 = x1 + (y3-y1)/(y2-y1) * (x2 - x1);		// 根据相似三角形计算
		//x3 = x2 + (y3-y2) * tan (CheckPoints[nCheckPoints-1].deltaa * 3.14/180.0);

		x4 = 0;
		y4 = LineLength;

		
		// 获得需要改正的角度，正的表示需要向左偏，负的表示需要向右偏，单位为度
		double deltaH = GetRotateAngle(x1,y1,x2,y2,x3,y3,x4,y4);
		
		CheckPoints[nCheckPoints-1].deltah = deltaH;	// 记录偏移角

		double deltaH0 = deltaH;	// 备份用于后面的输出
		int TotalFixStep = 0;		// 记录一共纠正了几步

		// 如果偏角过大，则分多步纠偏（最多2步，过了2步会再次读到标），否则，在StartFixStep一步纠偏
		// 分向右侧和向左侧两种情况
		if (deltaH > 0.1)	// 使小车向左偏
		{
			// 为了避免一次纠偏角度过大，如果角度超过限度，则分多次校正
			for (int s = StartFixStep; s <= EndFixStep; s++)
			{
				if (deltaH > RotateLimit)
				{
					// 以单步最大角纠正
					INT32 caliD = RotateLimit * RotateFactor;//RotateFactor * MicroDistance;
					Step[s].CaliDistance = - caliD;	//为负数，往左偏
					TotalFixStep++;

					// 将偏移角减去刚刚纠正的角度，近似作为下一步待纠正的角度
					deltaH -= RotateLimit;

					//如果超过2次就不再继续校准
					if (TotalFixStep == 2)
					{
						deltaH -= 360.0;
						// 更新checkpoint，将实际的变轨点作为最后一个checkpoint
						CheckPoints[nCheckPoints-1].adjustx = x3;
						CheckPoints[nCheckPoints-1].adjusty = y3;
					}
				}
				else if (deltaH >0)
				{
					INT32 caliD = deltaH * RotateFactor;//* RotateFactor * MicroDistance;
					Step[s].CaliDistance = - caliD;//为负数，往左偏						
					TotalFixStep++;

					// 将偏移角减去刚刚纠正的角度，使其为负数，下次循环不执行纠偏
					deltaH -= RotateLimit;

					// 更新checkpoint，将实际的变轨点作为最后一个checkpoint
					CheckPoints[nCheckPoints-1].adjustx = x3;
					CheckPoints[nCheckPoints-1].adjusty = y3;
				}
			}			
		}
		else if (deltaH < -0.1)	// 使小车向右偏
		{
			// 为了避免一次纠偏角度过大，如果角度超过限度，则分多次校正
			for (int s = StartFixStep; s <= EndFixStep; s++)
			{
				if (-deltaH > RotateLimit)
				{
					// 以单步最大角纠正
					INT32 caliD =  RotateLimit * RotateFactor;//* RotateFactor * MicroDistance;
					Step[s].CaliDistance = caliD; //为正数，往右偏
					TotalFixStep++;

					// 近似将偏移角加上刚刚纠正的角度，作为下一步待纠正的角度
					deltaH += RotateLimit;
					
					//如果超过2次就不再继续校准
					if (TotalFixStep == 2)
					{
						deltaH += 360.0;
						// 更新checkpoint，将实际的变轨点作为最后一个checkpoint
						CheckPoints[nCheckPoints-1].adjustx = x3;
						CheckPoints[nCheckPoints-1].adjusty = y3;
					}
				}
				else if (-deltaH > 0)
				{
					INT32 caliD =  deltaH * RotateFactor;//* RotateFactor * MicroDistance;
					Step[s].CaliDistance = - caliD;	//为正数，往右偏
					TotalFixStep++;

					// 将偏移角加上刚刚纠正的角度，使其为正数，下次循环不执行纠偏
					deltaH += RotateLimit;
					
					// 更新checkpoint，将实际的变轨点作为最后一个checkpoint
					CheckPoints[nCheckPoints-1].adjustx = x3;
					CheckPoints[nCheckPoints-1].adjusty = y3;
				}
			}			
		}
		else	// 角度偏移小于0.1度不纠正
		{
ofsLog << "    角度偏移小于0.1度不纠正\n";
		}

		CString Tempstr;
		Tempstr.Format("LL=%d,SS=%d,x1=%.1f,y1=%.1f,x2=%.1f,y2=%.1f,x3=%.1f,y3=%.1f,x4=%.1f,y4=%.1f,偏移角=%.1f,纠偏步数=%d\n",
			Control->mExeLine,Control->mExeStep,x1,y1,x2,y2,x3,y3,x4,y4,deltaH0,TotalFixStep);

		// 写入日志文件中
ofsLog << "    x1="<<x1<<",y1="<<y1<<",x2="<<x2<<",y2="<<y2;
ofsLog << ",x3="<<x3<<",y3="<<y3<<",x4="<<x4<<",y4="<<y4<<"\n";
ofsLog << "    需偏移角="<<deltaH0<<",实测方向角="<<CheckPoints[nCheckPoints-1].deltaa<<"\n";
ofsLog << "    偏移步数="<<TotalFixStep<<",sStep="<<StartFixStep<<",eStep="<<EndFixStep<<"\n";

		//输出CheckPoints[i].adjustx等
		
ofsLog << "    nCheckPoints=" << nCheckPoints << "\n";
		for(int i =0; i< nCheckPoints; i++)
		{	
ofsLog << "   i="<<i<<",CheckPoints[i].adjustx="<<CheckPoints[i].adjustx;
ofsLog <<",CheckPoints[i].adjusty="<<CheckPoints[i].adjusty;
ofsLog <<",CheckPoints[i].distance="<<CheckPoints[i].distance<<"\n";
		}
		for(i =0; i< nCheckPoints; i++)
		{
ofsLog << "   i="<<i<<",CheckMarks[i].x="<<CheckMarks[i].x;
ofsLog <<",CheckMarks[i].y="<<CheckMarks[i].y<<"\n";
		}
ofsLog << "\n";
		
ofsLog.close();
		GetDlgItem(IDC_STATIC2)->SetWindowText(Tempstr);

		// 发送轨迹校准相关的数据到后台 
		SendIGVTrackFixInfo(CheckMarks[nCheckPoints-2].barCode,		// 检测点ID
							CheckMarks[nCheckPoints-2].x,			// 检测点地标x
							CheckMarks[nCheckPoints-2].y,			// 检测点地标y
							CheckPoints[nCheckPoints-2].deltah,		// 检测点方向偏移角	
							CheckPoints[nCheckPoints-2].deltax,		// 检测点x偏移量
							CheckPoints[nCheckPoints-2].deltay,		// 检测点y偏移量
							CheckPoints[nCheckPoints-2].speed,		// 检测点小车速度
							CheckPoints[nCheckPoints-2].adjustx,	// 校准点x坐标(起点为坐标原点，起点指向终点为Y轴正向)
							CheckPoints[nCheckPoints-2].adjusty,	// 校准点y坐标(同上)							
							RotateFactor,							// 校准点校准系数
							CheckMarks[nCheckPoints-1].barCode,		// 检验点ID
							CheckMarks[nCheckPoints-1].x,			// 检验点地标x
							CheckMarks[nCheckPoints-1].y,			// 检验点地标y
							CheckPoints[nCheckPoints-1].deltah,		// 检验点方向偏移角
							CheckPoints[nCheckPoints-1].deltax,		// 检验点x偏移量
							CheckPoints[nCheckPoints-1].deltay,		// 检验点y偏移量
							m_igvInfo.Heading,						// 小车理论上的车头朝向
							m_igvInfo.MoveDirection);				// 小车理论上的运动方向
	}
	return TRUE;	// 完成功能	
}



// 给定四个点的坐标，求小车需要旋转的角度
// x1 y1 出发点
// x2 y2 检测到地标的点
// x3 y3 沿着12点方向延伸到可以实施纠偏的点
// x4 y4 目标点，x4=0
// 返回deltaH: 正的表示向左偏，负的表示向右偏
double CMicroMecanumDlg::GetRotateAngle(double x1, double y1, double x2, double y2, 
										double x3, double y3, double x4, double y4)
{
	double deltaH = 0;
	// 求点1指向点2的向量12
	double x12 = x2-x1;
	double y12 = y2-y1;
	// 向量的模
	double m12 = sqrt(x12*x12 + y12*y12);
	// 求点3指向点4的向量34
	double x34 = x4-x3;
	double y34 = y4-y3;
	// 向量的模
	double m34 = sqrt(x34*x34 + y34*y34);

	double cosinA = (x12*x34 + y12*y34)/(m12*m34);
	deltaH = acos(cosinA);

	// 判断向左偏还是向右偏

	// 如果两个向量都在第一象限,注意y12和y34肯定是>=0
	if (x12 >=0  && x34 >=0 )
	{
		if (x12 ==0 && x34 == 0)
			deltaH = 0;
		else if(x12 == 0 && x34 != 0 )	//需向右偏
			deltaH = -deltaH;
		else if(x12 != 0 && x34 == 0 )	//需向左偏
			deltaH = deltaH;
		else if(y12/x12 > y34/x34)		//需向右偏
			deltaH = -deltaH;
		else							//需向左偏
			deltaH = deltaH;
	}
	// 如果两个都在第二象限
	else if (x12 <= 0 && x34 <= 0)
	{
		if (x12 ==0 && x34 == 0)
			deltaH = 0;
		else if(x12 == 0 && x34 != 0 )	//需向左偏
			deltaH = deltaH;
		else if(x12 != 0 && x34 == 0 )	//需向右偏
			deltaH = -deltaH;
		else if(y12/x12 > y34/x34)		//需向右偏
			deltaH = -deltaH;
		else							//需向左偏
			deltaH = deltaH;
	}
	// 如果12在第一象限，34在第二象限，则需向左偏
	else if (x12 >=0 && x34 <=0)
		deltaH = deltaH;
	else //12在第二象限，34在第一象限，则需向右偏
		deltaH = -deltaH;
		
	return deltaH * 180/3.14;
}

// 本函数经验证后未被采用，因为当初做实验时相对于地标的偏移角测量精度无法保证
// 给定第3、4点的坐标和车子的偏移角deltaa（针对运动方向的，偏右为正），求小车需要旋转的角度
// x3 y3 沿着12点方向延伸到可以实施纠偏的点
// x4 y4 目标点，x4=0
// deltaa 车子的偏移角（针对运动方向的，偏右为正）
// isForward 是否前进（即方向是0度，否则方向是180度）
// 返回deltaA: 正的表示需向左偏（目前偏右了），负的表示向右偏
double CMicroMecanumDlg::GetRotateAngle(double x3, double y3,double x4, double y4, double deltaa)
{
	// 根据deltaa 和 isForward造出x1,y1和x2,y2
	double x1 = 0;
	double y1 = 0;
	double x2;
	double y2 = 1000;
	deltaa = deltaa*3.14/180.0;
	// 求点1指向点2的向量12
	if (deltaa > 0)	// 车头偏右
	{
		x2 = y2*tan(deltaa);
	}
	else if(deltaa<0)
	{
		x2 = -y2*tan(-deltaa);
	}
	else	// deltaa == 0
	{
		x2 = 0;
	}
		
	return GetRotateAngle(x1,y1,x2,y2,x3,y3,x4,y4);
}

// 查找sBar地标附近的中心地标(仅在并排的三个中找)，假如自身就是中心，则返回自身barCode.
CString CMicroMecanumDlg::FindCentralMark(CString sBar)
{
	CString sBarCentral = sBar;
	MARK* pMark = mapMarks[sBar];

	if (!pMark)
		return sBarCentral;

	double x = pMark->x;
	double y = pMark->y;
	CString sBar1, sBar2;
	int count = 0;
	
	for (int i = 0; i < nMarks; i++)
	{
		if (marks[i].barCode != sBar)
		{
			if( fabs(marks[i].x - x) +fabs(marks[i].y-y) <100.1 ) //在一排
			{
				if(count==0)
				{
					sBar1 = marks[i].barCode;
					count++;
				}
				else
				{
					sBar2 = marks[i].barCode;
					count++;
				}
			}
		}
	}

	if(count <2)	// 未找到并排的另两个标
		return sBarCentral;

	double x1 = mapMarks[sBar1]->x;
	double y1 = mapMarks[sBar1]->y;
	double x2 = mapMarks[sBar2]->x;
	double y2 = mapMarks[sBar2]->y;

	if (fabs(y1-y2) < 0.1)	// 横排
	{
		if ( fabs(x+x1-2*x2) < 0.1) // x+x1 == 2*x2
			sBarCentral = sBar2;
		else if (fabs(x+x2-2*x1) < 0.1)
			sBarCentral = sBar1;
		else 
			sBarCentral = sBar;
	}
	else	// 竖排
	{
		if ( fabs(y+y1-2*y2) < 0.1) // y+y1 == 2*y2
			sBarCentral = sBar2;
		else if (fabs(y+y2-2*y1) < 0.1)
			sBarCentral = sBar1;
		else 
			sBarCentral = sBar;
		
	}

	return sBarCentral;
}

// 根据当前读到的地标(及其偏移量)和中心地标，以及车头朝向，确定车子的移动参数
// pMark 当前地标
// pMarkCentral 中心地标
// deltax 当前地标偏移量x
// deltay 当前地标偏移量y
// m_Heading 车头朝向
MotionParemeter CMicroMecanumDlg::GetCaliLine(MARK* pMark, MARK* pMarkCentral, double deltax, double deltay, double m_Heading)
{
	//MotionParemeter MP;
	int dir;
	int dis;
	double x12 = pMarkCentral->x - pMark->x;	// x向量
	double y12 = pMarkCentral->y - pMark->y;	// y向量
	switch((int)m_Heading)
	{
	case 0:		// 车头朝北
		if (x12 > 40 && y12 < 1)		// 向东移动
		{
			dir = 90;
			dis = x12 - deltax;
		}
		else if (x12 < 1 && y12 > 40)	// 向北移动
		{
			dir = 0;
			dis = y12 - deltay;
		}
		else if (x12 < -40 && y12 < 1)	// 向西移动
		{
			dir = 270;
			dis = -x12 + deltax;
		}
		else if (x12 < 1 && y12 < -40)	// 向南移动
		{
			dir = 180;
			dis = -y12 + deltay;
		}
		break;
	case 90:	// 车头朝东
		if (x12 > 40 && y12 < 1)		// 向东移动
		{
			dir = 0;
			dis = x12 - deltay;
		}
		else if (x12 < 1 && y12 > 40)	// 向北移动
		{
			dir = 270;
			dis = y12 + deltax;
		}
		else if (x12 < -40 && y12 < 1)	// 向西移动
		{
			dir = 180;
			dis = -x12 + deltay;
		}
		else if (x12 < 1 && y12 < -40)	// 向南移动
		{
			dir = 90;
			dis = -y12 - deltax;
		}		
		break;
	case 180:	// 车头朝南
		if (x12 > 40 && y12 < 1)		// 向东移动
		{
			dir = 270;
			dis = x12 + deltax;
		}
		else if (x12 < 1 && y12 > 40)	// 向北移动
		{
			dir = 180;
			dis = y12 + deltay;
		}
		else if (x12 < -40 && y12 < 1)	// 向西移动
		{
			dir = 90;
			dis = -x12 - deltax;
		}
		else if (x12 < 1 && y12 < -40)	// 向南移动
		{
			dir = 0;
			dis = -y12 - deltay;
		}		
		break;
	case 270:	// 车头朝西
		if (x12 > 40 && y12 < 1)		// 向东移动
		{
			dir = 180;
			dis = x12 + deltay;
		}
		else if (x12 < 1 && y12 > 40)	// 向北移动
		{
			dir = 90;
			dis = y12 - deltax;
		}
		else if (x12 < -40 && y12 < 1)	// 向西移动
		{
			dir = 0;
			dis = -x12 - deltay;
		}
		else if (x12 < 1 && y12 < -40)	// 向南移动
		{
			dir = 270;
			dis = -y12 + deltax;
		}		
		break;
	}

	return GetMotionParameters(dir, dis);
	/*
		// 1.运动方向
		//MP.Derection	;	
		// 2.目标距离
		//MP.AidDistance	= int(fabs(x12) + fabs(y12));	
		// 3.运动类型
		MP.MotionType	= LineRips;
		// 4.速度倍率
		MP.SpeedRate	= sRate;
		// 5.启动速度
		MP.StartSpeed	= 0;
		// 6.最高速度
		MP.MaxSpeed		= 500;
		// 7.加加速度
		MP.ACC			= 500;	
		// 8.减加速度
		MP.DACC			= 500;
		// 9.减速距离
		MP.ReducePoint	= 0;
		// 10.启动类型
		MP.StartType	= STAUD;
		// 11.校正距离
		MP.CaliDistance	= 0;
		// 12.微步步长	
		MP.MicroDistance =  200;
		// 13.X分轴目标
		MP.AidX			= 1;
		// 14.Y分轴目标
		MP.AidY			= 2;
		// 15.Z分轴目标
		MP.AidZ			= 3;
		// 16.U分轴目标	
		MP.AidU			= 4;	

	return MP;
	*/
}

// 根据运动方向和目标距离返回一套运动参数（MotionParameter结构体）
// dir 方向（度，正前方为0度，顺时针方向计）
// 距离（毫米）
// 前进=0，后退=180，左移=270，右移=180，暂停=372，左转=361，右转=270，下降=366，抬升=367，校准=370；
// AidDistance的取值：前进、后退、左移、右移的距离，单位毫米
// 暂停的单位是0.1秒，如果暂停1秒，AidDistance=10；
// 下降、抬升、校准情况下，AidDistance=0就可以了

MotionParemeter CMicroMecanumDlg::GetMotionParameters(int dir, int dis)
{
	MotionParemeter MP;
		// 1.运动方向
		MP.Derection  = dir	;			
		// 2.目标距离
		MP.AidDistance	= dis;		 	
		// 3.运动类型
		MP.MotionType	= LineRips;
		// 4.速度倍率
		MP.SpeedRate	= sRate;
		// 5.启动速度
		MP.StartSpeed	= 0;
		// 6.最高速度
		MP.MaxSpeed		= 500;
		// 7.加加速度
		MP.ACC			= 500;	
		// 8.减加速度
		MP.DACC			= 500;
		// 9.减速距离
		MP.ReducePoint	= 0;
		// 10.启动类型
		MP.StartType	= STAUD;
		// 11.校正距离
		MP.CaliDistance	= 0;
		// 12.微步步长	
		MP.MicroDistance =  200;
		// 13.X分轴目标
		MP.AidX			= 1;
		// 14.Y分轴目标
		MP.AidY			= 2;
		// 15.Z分轴目标
		MP.AidZ			= 3;
		// 16.U分轴目标	
		MP.AidU			= 4;	

		return MP;
}

DWORD CMicroMecanumDlg::RunLife(CString TimeUnit)	//统计程序运行时间
{
	static DWORD TimePoint0,TimePoint1;
	TimePoint1 =  GetCurrentTime();
	if (TimePoint0 == 0)		// 记忆起始值
	{
		TimePoint0 = TimePoint1;
		TRACE("Time0=%d!\n",TimePoint0);
	}
	if (TimeUnit == "S")
	{
		TimePoint1 = (TimePoint1 - TimePoint0)/1000;
		TRACE("程序运行%d秒!\n",TimePoint1);
	} 
	else
	{
		TimePoint1 = TimePoint1 - TimePoint0;
		TRACE("程序运行0X%X毫秒!\n",TimePoint1);
	}	
	return TimePoint1;// 完成功能
}




void CMicroMecanumDlg::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	CString s;
	m_HeadingCombBxo.GetLBText(m_HeadingCombBxo.GetCurSel(),s);
	m_igvInfo.Heading = atoi(s);
}

// 根据旋转指令更新车头朝向
// cmd: 361逆时针转，362顺时针
// angle: 转360度的angle值为3180 (TurnRound = 3180)
void CMicroMecanumDlg::UpdateHeading(INT32 cmd, INT32 angle)
{
	int heading = m_igvInfo.Heading+0.5;	// 取整
	int turn = int(angle*360/TurnRound + 0.5);	// 整数
	
	if(cmd == 361)
		heading -= turn;
	else if (cmd == 362)
		heading += turn;
		
	if (heading >= 360)
		heading -= 360;
	else if (heading < 0)
		heading += 360;

	switch (heading)
	{
	case 0:
		m_HeadingCombBxo.SetCurSel(0);
		break;
	case 90:
		m_HeadingCombBxo.SetCurSel(1);
		break;
	case 180:
		m_HeadingCombBxo.SetCurSel(2);
		break;
	case 270:
		m_HeadingCombBxo.SetCurSel(3);
		break;
	}

	m_HeadingCombBxo.UpdateData(TRUE);
	
	m_igvInfo.Heading = heading;
}

// 判读一个条码的各位是否是0-9的数字
BOOL CMicroMecanumDlg::CheckBarcode(CString bar)
{
	// 验证长度
	if (bar.GetLength() != BarCodeNumber)
	{
m_ofsLog2 << "读到的地标("<<bar<<")长度不等于"<<BarCodeNumber<<"\n";
		return false;
	}
	// 验证字符
	for (int i =0; i< BarCodeNumber; i++)
	{
		char c = bar.GetAt(i);
		if (c<'0' || c>'9')
		{
			m_ofsLog2 << "读到的地标("<<bar<<")有非0-9字符\n";
			return false;
		}
	}

	// 验证地标
	if(GetMarkInfo(bar) == NULL)
	{
		m_ofsLog2 << "读到的地标("<<bar<<")在地标文件中未查到\n";
		return false;
	}

	return true;
}

// 更新车辆当前坐标
// dis 车辆沿直线行走的距离，注意dis >= 0
void CMicroMecanumDlg::UpdateOwnPosition(int dis)
{
	int iDirection = Line[Control.mExeLine].Derection; // 运动方向
	// 且正在进行前进、后退、左移、右移四种动作之一
	if (iDirection ==0 || iDirection == 90 || iDirection == 180 || iDirection == 270)
	{
		// 如果正在执行line中的某个step，不处于暂停状态
		if (Control.mLineEnable != 0 && Control.mStepEnable != 0 && Control.mPause == 0 && Control.mGoOn == 0)
		{			
			int iHeading = int(m_igvInfo.Heading+0.5); // 车头朝向
			switch(iHeading)
			{
			case 0:	// 车头朝北
				switch(iDirection)
				{
				case 0:
					m_igvInfo.curY = m_igvInfo.preY + dis;
					break;
				case 90:
					m_igvInfo.curX = m_igvInfo.preX + dis;
					break;
				case 180:
					m_igvInfo.curY = m_igvInfo.preY - dis;
					break;
				case 270:
					m_igvInfo.curX = m_igvInfo.preX - dis;
					break;			
				}
				break;
			case 90:	// 车头朝东
				switch(iDirection)
				{
				case 0:
					m_igvInfo.curX = m_igvInfo.preX + dis;
					break;
				case 90:
					m_igvInfo.curY = m_igvInfo.preY - dis;
					break;
				case 180:
					m_igvInfo.curX = m_igvInfo.preX - dis;
					break;
				case 270:
					m_igvInfo.curY = m_igvInfo.preY + dis;
					break;			
				}
				break;
			case 180:	// 车头朝南
				switch(iDirection)
				{
				case 0:
					m_igvInfo.curY = m_igvInfo.preY - dis;
					break;
				case 90:
					m_igvInfo.curX = m_igvInfo.preX - dis;
					break;
				case 180:
					m_igvInfo.curY = m_igvInfo.preY + dis;
					break;
				case 270:
					m_igvInfo.curX = m_igvInfo.preX + dis;
					break;			
				}
				break;
			case 270:
				switch(iDirection)
				{
				case 0:
					m_igvInfo.curX = m_igvInfo.preX - dis;
					break;
				case 90:
					m_igvInfo.curY = m_igvInfo.preY + dis;
					break;
				case 180:
					m_igvInfo.curX = m_igvInfo.preX + dis;
					break;
				case 270:
					m_igvInfo.curY = m_igvInfo.preY - dis;
					break;			
				}
				break;
			}
		}
		// 如果某个Line的动作完成了，则更新preX, preY
		if (Control.mStepEnable == 0 && Control.mLine == 50)
		{
			m_igvInfo.preX = m_igvInfo.curX;
			m_igvInfo.preY = m_igvInfo.curY;
//m_ofsLog2<<"某个Line的动作完成了，此处更新坐标2，curx="<<curX<<",cury="<<curY<<",prex="<<preX<<",prey="<<preY<<"\n";
		}

	}
}

// 检验一下当前位置，例如，是不是离上一个读到的标太远了，或者离Line出发点太远了
// 如果是，则小车紧急停车，且报警
void CMicroMecanumDlg::ValidatePosition()
{
	int iDirection = Line[Control.mExeLine].Derection; // 运动方向
	// 如果正在进行前进、后退、左移、右移四种动作之一
	if (iDirection ==0 || iDirection == 90 || iDirection == 180 || iDirection == 270)
	{
		// 如果正在执行line，且不处于暂停状态
		if (Control.mLineEnable != 0 && Control.mPause == 0 && Control.mGoOn == 0)
		{
			if (nCheckPoints > 0)
			{
				double dis1	= fabs(m_igvInfo.curX-CheckMarks[nCheckPoints-1].x) + fabs(m_igvInfo.curY-CheckMarks[nCheckPoints-1].y);
				double dis2 = fabs(m_igvInfo.curX-m_igvInfo.preX) + fabs(m_igvInfo.curY-m_igvInfo.preY);
				if (dis1 > MAX_NotFindMarkDistance && dis2 > MAX_NotFindMarkDistance)
				{
m_ofsLog2 << "--太远距离未发现地标，离上次发现地标"<<(char*)(_bstr_t)CheckMarks[nCheckPoints-1].barCode.GetBuffer(0)
						<<"的距离是："<<dis1<<"毫米，离上次记录坐标点的距离是："<<dis2<<"毫米，将自动停车！\n";
					
					OnButton1();	// 紧急停止
				}
			}
		}
		
	}
}

// 计算搜索路径（共Num_SearchLines条）
// 目前搜索450mm*250mm的范围（假设摄像头覆盖150mm*50mm）
// 该函数不用了，改为读文件 2016-3-7 qi
void CMicroMecanumDlg::CalculateSearchLines()
{
	// 设置搜索路径
	SearchLines[0] = GetMotionParameters(0, 80);	// 上移8cm
	SearchLines[1] = GetMotionParameters(180, 160);	// 下18cm
	SearchLines[2] = GetMotionParameters(270, 80);	// 左8	
	SearchLines[3] = GetMotionParameters(0, 160);	// 上16 
	SearchLines[4] = GetMotionParameters(90, 160);	// 右16
	SearchLines[5] = GetMotionParameters(180, 160);	// 下16
}

// 读取地标搜索路径文件
void CMicroMecanumDlg::ReadSearchLines()
{
	ifstream fs;

	fs.open("searchlines.txt", ios::nocreate);//默认以 ios::in 的方式打开文件，文件不存在时操作失败

	if(fs)
	{
		fs>>m_iCountOfSearchRoutes;
		for (int i = 0; i< m_iCountOfSearchRoutes; i++)
		{
			int index;
			int dir=0, dis=0;
			fs>>index >> dir >> dis;
			SearchLines[i] = GetMotionParameters(dir, dis);		
		}
		fs.close();
	}


}

// 定时向服务端发送小车实时信息
void CMicroMecanumDlg::SendIGVRealTimeInfo()
{

	time_t t = time( 0 ); 
	char tmp[64]; 
	strftime( tmp, sizeof(tmp), "%X",localtime(&t) );

	ofstream m_ofsLog3;   //日志文件流
	Json::Value msg;// 构建对象
	Json::Value msgHead;
	Json::Value msgBody;

	char mac_address[1024];
	//GetMacaddress((char*)mac_address);	
	if(GetMacaddress(mac_address)<=0)
    {
        // 写入日志“获取MAC地址失败，无法登录”
		m_ofsLog3.open("log-3连接Socket和接受消息.txt",ios::app);
		m_ofsLog3<<tmp<<"发送小车实时信息时，获取MAC地址失败\n"; 	
		m_ofsLog3.close();
		return;
    }
	std::string mac(mac_address); 
	msgHead["KeyId"] = mac;  // mac地址
	msgHead["MessageType"] = "2";    // 消息类型
	msg["MessageHead"] = msgHead;
	
	msgBody["X"] = int(m_igvInfo.curX);
	msgBody["Y"] = int(m_igvInfo.curY);
	msgBody["Speed"] = (m_igvInfo.Speed);
	msgBody["Direction"] = int(m_igvInfo.Heading);
	msgBody["IsFree"] = m_igvInfo.isFree?"true":"false";
	msgBody["IsMarked"] = m_igvInfo.isMarked?"true":"false";
	msgBody["MarkId"] = (char*)(_bstr_t)m_igvInfo.MarkID.GetBuffer(0);	

//ofstream ofsLog;				
//ofsLog.open("log-4 isFree状态调试.txt",ios::app);
//ofsLog<<"发消息,MarkID: "<<(char*)(_bstr_t)m_igvInfo.MarkID.GetBuffer(0)<<"\n";
//ofsLog.close();

	Json::FastWriter writer;  // 用Json::FastWriter 来处理 json 应该是最快的
	std::string strMsgBody = writer.write(msgBody);
	msg["MessageBody"] = strMsgBody;	

	m_ofsLog3.open("log-3连接Socket和接受消息.txt",ios::app);
	m_ofsLog3<<tmp<<"--准备发送小车实时信息\n"; 	
	m_ofsLog3.close();

	std::string msg2server = writer.write(msg);
	SendMessage2Server((char*)msg2server.c_str());	
}
 
// 发送轨迹校准相关的数据到后台
void CMicroMecanumDlg::SendIGVTrackFixInfo(CString markID1, int x1, int y1, double deltah1, 
		double deltax1, double deltay1, double speed1, double x2, double y2,
		double k2, CString markID3, int x3, int y3, double deltah3, double deltax3, double deltay3,int heading, int motionDirection)
{
	Json::Value msg;// 构建对象
	Json::Value msgHead;
	Json::Value msgBody;
//char* temp_MarkId1;
//char* temp_MarkId3;
	char mac_address[1024];
	//GetMacaddress((char*)mac_address);	
	if(GetMacaddress(mac_address)<=0)
    {
        // 写入日志“获取MAC地址失败，无法登录”
		return;
    }
	std::string mac(mac_address); 
	msgHead["KeyId"] = mac;  // mac地址
	msgHead["MessageType"] = "3";    // 消息类型
	msg["MessageHead"] = msgHead;
	
	msgBody["MarkId1"] = (char*)(_bstr_t)markID1.GetBuffer(0);
	std::string Temp_MarkId1 = msgBody["MarkId1"].asString();
	msgBody["X1"] = x1;
	msgBody["Y1"] = y1;
	msgBody["DelatH1"] = deltah1;
	msgBody["DelatX1"] = deltax1;
	msgBody["DelatY1"] = deltay1;
	msgBody["Speed1"] = speed1;

	msgBody["X2"] = x2;
	msgBody["Y2"] = y2;
	msgBody["K2"] = k2;
	
	msgBody["MarkId3"] = (char*)(_bstr_t)markID3.GetBuffer(0);
	std::string Temp_MarkId3 = msgBody["MarkId3"].asString();
	msgBody["X3"] = x3;
	msgBody["Y3"] = y3;
	msgBody["DelatH3"] = deltah3;
	msgBody["DelatX3"] = deltax3;
	msgBody["DelatY3"] = deltay3;

	msgBody["Heading"] = heading;
	msgBody["MotionDirection"] = motionDirection;

	Json::FastWriter writer;  // 用Json::FastWriter 来处理 json 应该是最快的
	std::string strMsgBody = writer.write(msgBody);
	msg["MessageBody"] = strMsgBody;

	std::string msg2server = writer.write(msg);
	SendMessage2Server((char*)msg2server.c_str());	

ofstream ofsLog;				
ofsLog.open("log-6 轨迹校准记录.txt",ios::app);

/*
ofsLog<<"MarkID="<<m_pszDecodedText;
ofsLog<<",DelatH="<<deltah;
ofsLog<<",DelatX="<<deltax;
ofsLog<<",DeltaY="<<deltay;
ofsLog<<",K="<<k;
ofsLog<<",DeltaH2="<<deltah2;
ofsLog<<",X="<<x;
ofsLog<<",Y="<<y;
ofsLog<<",Heading="<<heading;
ofsLog<<",MotionDirection="<<motionDirection;
ofsLog<<",Speed="<<speed;
ofsLog<<",FixSteps="<<totalFixStep<<"\n";
*/
/*
// 标题栏
ofsLog<<"DelatH1  "<<"DelatH3  "<<"DelatX1  "<<"DelatY1  "<<"Heading  "<<"K2  "<<"MarkId1  ";
ofsLog<<"MarkId3  "<<"MotionDirection  "<<"Speed1  "<<"X1  "<<"X2  "<<"X3  "<<"Y1  "<<"Y2  "<<"Y3"<<"\n";*/


// 内容
ofsLog<<deltah1<<"    "<<deltah3<<"    "<<deltax1<<"    "<<deltay1<<"    "<<deltax3<<"    "<<deltay3<<"    "<<heading<<"    ";
ofsLog<<k2<<"    "<<Temp_MarkId1<<"    "<<Temp_MarkId3<<"    "<<motionDirection<<"    "<<speed1<<"    ";
ofsLog<<x1<<"    "<<x2<<"    "<<x3<<"    "<<y1<<"    "<<y2<<"    "<<y3<<"\n";

//ofsLog<<(char*)strMsgBody.c_str()<<"\n";

ofsLog.close();
	
}

// socket连接成功时发送登录消息给服务端
void CMicroMecanumDlg::SendIGVLoginMessage()
{
	//GetDlgItem(IDC_STATIC_Connect)->SetWindowText("已连接");
	//SetDlgItemText(IDC_STATIC_Connect, "已连接");
	m_ConnectStatus.SetWindowText("已连接");
	ofstream m_ofsLog3;   //日志文件流
	Json::Value msg;// 构建对象
	Json::Value msgHead;
	Json::Value msgBody;

	char mac_address[1024];
	//GetMacaddress((char*)mac_address);	
	if(GetMacaddress(mac_address)>0)
    {
        // 获取MAC地址成功
		m_ofsLog3.open("log-3连接Socket和接受消息.txt",ios::app);
		m_ofsLog3<<"获取MAC地址成功\n"; 		
		m_ofsLog3.close();
    }
	else
    {
        // 写入日志“获取MAC地址失败，无法登录”
		m_ofsLog3.open("log-3连接Socket和接受消息.txt",ios::app);
		m_ofsLog3<<"获取MAC地址失败，无法登录\n"; 	
		m_ofsLog3.close();
		return;
    }
	std::string mac(mac_address); 
	msgHead["KeyId"] = mac;  // mac地址
	msgHead["MessageType"] = "0";    // 消息类型

	msg["MessageHead"] = msgHead;
	msg["MessageBody"] = NULL;

	Json::FastWriter writer;  // 用Json::FastWriter 来处理 json 应该是最快的
	std::string msg2server = writer.write(msg); 
	SendMessage2Server((char*)msg2server.c_str());	
}

void CMicroMecanumDlg::SendMessage2Server(char* msg)
{	
	//msg = "00000";
	ofstream m_ofsLog3;   //日志文件流
	if(send(m_Sock, msg, strlen(msg), 0) == SOCKET_ERROR) //判断发送是否成功
	{
		 //MessageBox("发送失败");
		m_ofsLog3.open("log-3连接Socket和接受消息.txt",ios::app);
		m_ofsLog3<<"发送消息失败\n"; 
		m_isSocketConnected = false;
		// GetDlgItem(IDC_STATIC_Connect)->SetWindowText("未连接");
		m_ConnectStatus.SetWindowText("未连接");
		m_ofsLog3.close();
	}
	else if(msg == "")   //判断发送的数据是否为空
	{
		 //MessageBox("请输入信息");
		m_ofsLog3.open("log-3连接Socket和接受消息.txt",ios::app);
		m_ofsLog3<<"发送消息为空\n"; 		
		m_ofsLog3.close();
	}
	else //发送成功
	{
		m_ofsLog3.open("log-3连接Socket和接受消息.txt",ios::app);
		m_ofsLog3<<"发送消息成功\n"; 	
		m_ofsLog3.close();
	}		
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


// 收到消息后的处理函数
void CMicroMecanumDlg::Command2Route(char* sMsg)	
{
	MotionParemeter m_motionPar;

	// 复位路径段数指针
	Control.mMaxLine = 0;	
	
	// 读取收到的消息
	Json::Reader *reader = new Json::Reader(Json::Features::strictMode());  
	Json::Value root;
	Json::Value arraymember1;
	if (reader->parse(sMsg, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素
	{	
		int route_size = root.size();
		for(int i = 0; i<route_size; i++)
		{
			arraymember1 = root[i];
			
			int upload_dir = arraymember1["MessageHead"]["CommandType"].asInt(); 
			std::string upload_aidDisStr = arraymember1["MessageBody"].asString();	
			int upload_aidDis;
			stringstream ss(upload_aidDisStr);
			ss>>upload_aidDis;
			if(upload_aidDis<-1000)
			{
				upload_aidDis = 0;
			}
			
			if(upload_dir == 361 || upload_dir == 362)
			{
				upload_aidDis = (int)(upload_aidDis * TurnRound/360.0);
			}

			m_motionPar = GetMotionParameters(upload_dir,upload_aidDis);

			// 1.运动方向
			Line[Control.mMaxLine].Derection	= m_motionPar.Derection;			
			// 2.目标距离
			Line[Control.mMaxLine].AidDistance	= m_motionPar.AidDistance;	
			// 3.运动类型
			Line[Control.mMaxLine].MotionType	= LineRips;
			// 4.速度倍率
			Line[Control.mMaxLine].SpeedRate	= sRate;
			// 5.启动速度
			Line[Control.mMaxLine].StartSpeed	= m_motionPar.StartSpeed;
			// 6.最高速度
			Line[Control.mMaxLine].MaxSpeed		= m_motionPar.MaxSpeed;
			// 7.加加速度
			Line[Control.mMaxLine].ACC			= m_motionPar.ACC;	
			// 8.减加速度
			Line[Control.mMaxLine].DACC			= m_motionPar.DACC;
			// 9.减速距离
			Line[Control.mMaxLine].ReducePoint	= 0;
			// 10.启动类型
			Line[Control.mMaxLine].StartType	= STAUD;
			// 11.校正距离
			Line[Control.mMaxLine].CaliDistance	= m_motionPar.CaliDistance;
			// 12.微步步长	
			Line[Control.mMaxLine].MicroDistance = m_motionPar.MicroDistance;
			// 13.X分轴目标
			Line[Control.mMaxLine].AidX			= m_motionPar.AidX;
			// 14.Y分轴目标
			Line[Control.mMaxLine].AidY			= m_motionPar.AidY;
			// 15.Z分轴目标
			Line[Control.mMaxLine].AidZ			= m_motionPar.AidZ;
			// 16.U分轴目标	
			Line[Control.mMaxLine].AidU			= m_motionPar.AidU;	
					
			//递增计数器
			Control.mMaxLine++;		
		}	  
	}
	else
	{
	}
	delete reader;
 }

BOOL CMicroMecanumDlg::Deknock(void)			// 防碰撞检测 
{
	static UINT32 MechineSwitchDown = 1;
	if (pPort[0].Vpin0 == 0)
	{
		DeknockEnable = TRUE;
	} 
	else
	{
		DeknockEnable = FALSE;
	}
	if (DeknockEnable == TRUE)
	{
		// 反射型防碰撞检测
		if (GpioOperation(&m_Device,GpioRead,XADDR,Pin0,1) == GpioLow)
		{
			// 若端口电平拉低,表明运动方向有障碍物,立刻减速停车
			Stop(&m_Device,SDSTP);	// STOP / SDSTP 紧急停止;
			StopMotion();			// 停止微步运动

			MechineSwitchDown = 0;
			// 关闭电机电源
			MotorPowerOFF();
		}
		else
		{
			if (MechineSwitchDown == 0)
			{
				MechineSwitchDown = 1;
				// 恢复电机供电
				MotorPowerON();
			} 
		}

	}
/*	
	// 机械开关防碰撞检测
	if (GpioOperation(&m_Device,GpioRead,XADDR,Pin1,1) == GpioLow)
	{
		MechineSwitchDown = 0;
		// 关闭电机电源
		MotorPowerOFF();
	}
	else
	{
		if (MechineSwitchDown == 0)
		{
			MechineSwitchDown = 1;
			// 恢复电机供电
			MotorPowerON();
		} 
	}

	// 是否操作平台运动	
	if (PlatMotionEnable == TRUE)
	{
		// 要求平台在最低位
		if (GpioOperation(GpioRead,ZADDR,PlatformPower,GpioHigh) == GpioLow)
		{
			//1#电机执行动作
			if ((Dlg->GpioOperation(GpioRead,YADDR,Pin1,GpioHigh) == GpioHigh)		//到达最低点
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin5,GpioHigh) == GpioLow))	//1#电机正在运动
			{
				// 关闭1#电机电源
				Dlg->GpioOperation(GpioWrite,UADDR,Pin5,GpioHigh);
			}
			//2#电机执行动作
			if ((Dlg->GpioOperation(GpioRead,XADDR,Pin2,GpioHigh) == GpioHigh)		//到达最低点
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin4,GpioHigh) == GpioLow))	//2#电机正在运动
			{
				// 关闭2#电机电源
				Dlg->GpioOperation(GpioWrite,UADDR,Pin4,GpioHigh);
			}
			//完成下降动作，到达最低点
			if ((Dlg->GpioOperation(GpioRead,YADDR,Pin1,GpioHigh) == GpioHigh)		//1#到达最低点
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin5,GpioHigh) == GpioHigh)	//1#电机停止运动
				&& (Dlg->GpioOperation(GpioRead,XADDR,Pin2,GpioHigh) == GpioHigh)	//2#到达最低点
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin4,GpioHigh) == GpioHigh))	//2#电机停止运动
			{
				// 禁止平台运动
				PlatMotionEnable = FALSE;
			}
		} 
		else	// 要求平台在最高位
		{
			//1#电机执行动作
			if ((Dlg->GpioOperation(GpioRead,YADDR,Pin0,GpioHigh) == GpioHigh)		//到达最低点
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin5,GpioHigh) == GpioLow))	//1#电机正在运动
			{
				// 关闭1#电机电源
				Dlg->GpioOperation(GpioWrite,UADDR,Pin5,GpioHigh);
			}
			//2#电机执行动作
			if ((Dlg->GpioOperation(GpioRead,YADDR,Pin2,GpioHigh) == GpioHigh)		//到达最低点
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin4,GpioHigh) == GpioLow))	//2#电机正在运动
			{
				// 关闭2#电机电源
				Dlg->GpioOperation(GpioWrite,UADDR,Pin4,GpioHigh);
			}
			//完成下降动作，到达最低点
			if ((Dlg->GpioOperation(GpioRead,YADDR,Pin0,GpioHigh) == GpioHigh)		//1#到达最低点
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin5,GpioHigh) == GpioHigh)	//1#电机停止运动
				&& (Dlg->GpioOperation(GpioRead,YADDR,Pin2,GpioHigh) == GpioHigh)	//2#到达最低点
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin4,GpioHigh) == GpioHigh))	//2#电机停止运动
			{
				// 禁止平台运动
				PlatMotionEnable = FALSE;
			}		
		}
	}
	*/
	return TRUE;
}

BOOL CMicroMecanumDlg::MotorPowerON(void)		// 打开电机电源
{
	GpioOperation(&m_Device,GpioWrite,ZADDR,Pin7,GpioLow);
	return TRUE;
}
BOOL CMicroMecanumDlg::MotorPowerOFF(void)		// 关闭电机电源
{
	GpioOperation(&m_Device,GpioWrite,ZADDR,Pin7,GpioHigh);
	return TRUE;
}

LRESULT CMicroMecanumDlg::ProcessConnectSuccess(WPARAM wParam, LPARAM lParam)
{
	SendIGVLoginMessage();
	return true;
}

LRESULT CMicroMecanumDlg::ProcessLoginSuccess(WPARAM wParam, LPARAM lParam)
{
	m_ConnectStatus.SetWindowText("已登录");
	return true;
}



//添加线程函数server_thd()：SSS
UINT server_thd(LPVOID p)
{
	 WSADATA wsaData;
	 WORD wVersion; 
	 wVersion = MAKEWORD(2,2);
	 WSAStartup(wVersion,&wsaData);
	 
	 SOCKADDR_IN local_addr;
	 SOCKADDR_IN client_addr;
	 int iaddrSize = sizeof(SOCKADDR_IN);
	 int res;
	 char msg_rcv[1024];
	 memset(msg_rcv,0,1024*sizeof(char)); //lipan 初始化字符串
	 CString port;
	 CMicroMecanumDlg * dlg = (CMicroMecanumDlg *)AfxGetApp()->GetMainWnd(); //得到应用程序活动主窗口的指针
	 //dlg->port_edit->GetWindowText(port);//获得端口地址
	 port="5009";
	 if(port=="") 
	 {
		 showeditmsg = "请输入端口号\r\n";
		 dlg->showmsg_edit.SetWindowText(showeditmsg);
		 return 0;
	 }
	 //为local_addr赋值，创建soket
	 local_addr.sin_family = AF_INET;
	 local_addr.sin_port = htons(atoi(port)); //atoi， 把字符串转换成整型数
	 local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	 //创建监听listen_sock
	 if( (listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET )
	 {
		 showeditmsg = "创建监听失败\r\n";
		 dlg->showmsg_edit.SetWindowText(showeditmsg);

		 return 0;
	 }
	 //绑定
	 if( bind(listen_sock, (struct sockaddr*) &local_addr, sizeof(SOCKADDR_IN)) )
	 {
		 showeditmsg = "绑定错误,换一个端口试试\r\n";
		 dlg->showmsg_edit.SetWindowText(showeditmsg);
		 return 0;
	 }
	 
	 listen(listen_sock, 1); //开始监听，允许最大监听数为1
	 dlg->star_button.EnableWindow(FALSE);  //开启服务器按钮灰化
	 dlg->showmsg_edit.ShowWindow(SW_SHOW); //绑定消息edit显示
	 showeditmsg = "已成功开启....\r\n";
	 dlg->showmsg_edit.SetWindowText(showeditmsg);
	 
	 //接受连接请求
	 if( (sock = accept(listen_sock, (struct sockaddr *)&client_addr, &iaddrSize)) == INVALID_SOCKET)
	 {
		 showeditmsg = "接受连接请求失败\r\n";
		 dlg->showmsg_edit.SetWindowText(showeditmsg);
		 return 0;
	 }
	 else
	 {
		 CString port;
		 port.Format("%d", int(ntohs(client_addr.sin_port)));
		 showeditmsg = "已连接来自：" + CString(inet_ntoa(client_addr.sin_addr)) + "  端口:5009\r\n";
		 dlg->showmsg_edit.SetWindowText(showeditmsg);
	 }
			 
	 ////////////-------接收数据
	 while(1)
	 {
		 MsgHandling(msg_rcv);				//lipan_对返回信息进行规范处理，只保留最新的一条消息
		 if( (res = recv(sock, msg_rcv, 1024, 0)) == -1 ) //接收数据，判断是否接收成功
		 {
			showeditmsg = "-------失去连接\r\n";
			dlg->showmsg_edit.SetWindowText(showeditmsg);
			dlg->star_button.EnableWindow(TRUE);
			break;
		 }
		 else
		 {
			showeditmsg = "客户端:" + ((CString)(msg_rcv)).Mid(0,res) + "\r\n";
			dlg->showmsg_edit.SetWindowText(showeditmsg);

			// 读取收到的消息
			Json::Reader *reader = new Json::Reader(Json::Features::strictMode());  
			Json::Value root;
			if (reader->parse(msg_rcv, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素
			{
				struct BARINFO{
					bool isSuc;
					CString barID;
					double x;
					double y;
					double angle;
				};
				BARINFO barInfo;
				barInfo.isSuc = root["isSuccess"].asInt()>0?true:false;	
				barInfo.barID = root["BarCodeID"].asCString(); 
				barInfo.x = root["CenterX"].asDouble();
				barInfo.y = root["CenterY"].asDouble();
				barInfo.angle = root["Angle"].asDouble();

				dlg->SendMessage(WM_PROCESS_MESSAGE,0,(LPARAM)&barInfo);
			}
			else
			{

			}
			delete reader;
		}	 
	}
	return 0;
}

void CMicroMecanumDlg::OnButton13() 
{
	// TODO: 在此添加控件通知处理程序代码
	WSADATA wsData;  
	WSAStartup(MAKEWORD(2,2), &wsData); 

	char name[80];
	CString IP;
	CString P;
	hostent* pHost; 
	gethostname(name,sizeof(name));//获得主机名 
	pHost = gethostbyname(name);//获得主机结构	 
	IP = inet_ntoa(*(struct in_addr *)pHost->h_addr_list);//获取主机ip地址
	showmsg_edit.SetWindowText("绑定IP地址：" + IP);
	AfxBeginThread(&server_thd,0); //开启新线程处理

}

void CMicroMecanumDlg::OnButton_PauseAndRecovery()
{
	// TODO: 暂停功能测试
	ofstream m_ofsLog8;
	m_ofsLog8.open("log_8 小车暂停与恢复功能测试.txt",ios::app);
	if (FlagPause == 0)
	{
		Control.mPause = 0XFF;	// 进入暂停
		Control.mGoOn = 0XFF;	// 禁止连续
		GetDlgItem(IDC_BUTTON_PauseAndRecovery)->SetWindowText("Recovery");
		m_ofsLog8<<"IDC:    "<<IDC_BUTTON_PauseAndRecovery<<" 输出Line的个数 "<<Control.mMaxLine<<endl;
		m_ofsLog8<<"进入暂停状态      FlagPause = "<<FlagPause;
		FlagPause=1;
		m_ofsLog8<<"lipan"<<endl;
		m_ofsLog8.close();
		return;
	} 
	if (FlagPause==1)
	{
		Control.mPause = 0XFF;
		Control.mGoOn = 0;
		GetDlgItem(IDC_BUTTON_PauseAndRecovery)->SetWindowText("Pause");
		m_ofsLog8<<"IDC:    "<<IDC_BUTTON_PauseAndRecovery<<" 输出Line的个数 "<<Control.mMaxLine<<endl;
		m_ofsLog8<<"进入恢复状态    Flagpause = "<<FlagPause;
		FlagPause=0;
		Control.mPause = 0;
		Control.mGoOn = 0;
		m_ofsLog8<<"lipan"<<endl;
		m_ofsLog8.close();
		return;
	}
}

void CMicroMecanumDlg::OnButton_EmergencyBraking() 
{
	// TODO: Add your control notification handler code here
	ofstream m_ofsLog9;
	m_ofsLog9.open("log_9 小车EmergencyBreak测试.txt",ios::app);

	CString str;
	m_EmergencyBrake.GetWindowText(str);
	if (str == "紧急制动")
	{	
		CurLineNo  = Control.mExeLine;

		FindBarEnable		= FALSE;			// 禁止自动识别条码
		Control.mStep		= 0;				// 复位步骤指针
		Control.mExeStep	= 0;				// 复位微步指针
		Control.mStepEnable = 0;				// 复位禁用自动控制
		Control.mFixStep = 0;					// 复位自动校正步数
		
		Control.mPause		= 0;				// 复位暂停旗标
		Control.mGoOn		= 0;				// 复位继续路径运动旗标
		Control.mRevers		= 0;				// 复位反向旗标
		
		Control.mLine		= 0;				// 复位单段步骤指针
		Control.mExeLine	= 0;				// 复位单段执行指针
		Control.mLineEnable = 0;				// 复位单段旗标
		Stop(&m_Device,STOP);

		m_ofsLog9<<"当前Line    mExeLine = "<<Control.mExeLine<<" ";
		m_ofsLog9<<"最大Line    mMaxLine = "<<Control.mMaxLine<<"\n";

		m_EmergencyBrake.SetWindowText("恢复");
	} 
	else
	{
		Control.mMaxLine = Control.mMaxLine - CurLineNo;				    // 更新最大Line数值
	
		m_ofsLog9<<"恢复后的最大Line    mMaxLine = "<<Control.mMaxLine<<"\n";		
		
		// 记录行走的距离
		INT32 xLocation = INT32(DOUBLE(GetShiftR(&m_Device,XADDR)) * Puls2Distance);	// 计算已经行走的距离
		Temp_Line[CurLineNo].AidDistance = Temp_Line[CurLineNo].AidDistance - ((INT32)abs(xLocation));
		
		memset(Line, 0, sizeof(struct MotionParemeter)*mTotal); // 清空
		
		//更新Line
		for (int i = 0; i<Control.mMaxLine; i++)
		{
			Line[i] = Temp_Line[CurLineNo];
			CurLineNo++;
		}

		// 开始运动
		Control.mStep = 0;				// 复位步骤指针
		Control.mExeStep = 0;			// 复位微步指针
		Control.mStepEnable = 0;		// 复位禁用自动控制
		Control.mFixStep = 0;			// 复位自动校正步数
		
		Control.mPause		= 0;		// 复位暂停旗标
		Control.mGoOn		= 0;		// 复位继续路径运动旗标
		Control.mRevers		= 0;		// 复位反向旗标
		
		Control.mLine		= 0;		// 复位单段步骤指针
		Control.mExeLine	= 0;		// 复位单段执行指针
		Control.mLineEnable = 0XFF;		// 开始执行微步程序
		FindBarEnable		= TRUE;		// 允许自动识别条码

		m_EmergencyBrake.SetWindowText("紧急制动");
	}
	m_ofsLog9.close();
}

void CMicroMecanumDlg::OnButton_SlowDown(){

	ofstream m_log8;
	m_log8.open("log_8 SlowDown.txt",ios::app);
	m_log8<<"Control.mMaxStep : "<<Control.mMaxStep<<endl<<"Control.mExeStep : "<< Control.mExeStep<< endl;


	if(Control.mMaxStep > Control.mExeStep + 1){
/*
		MotionParemeter temp;
		INT32 xLocation = INT32(DOUBLE(GetShiftR(&m_Device,XADDR)) * Puls2Distance);
		temp = GetMotionParameters(Line[Control.mExeLine].Derection, 3*200 +(200 - (INT32)abs(xLocation%200)));
		memset(Line, 0, sizeof(MotionParemeter)*mTotal);
		Line[0] = GetMotionParameters(temp.Derection, temp.AidDistance);
		Step[Control.mExeStep + 3] = Step [Control.mMaxStep - 1];
		Step[Control.mExeStep + 3].ACC = Step [Control.mMaxStep - 1].ACC;
		Step[Control.mExeStep + 3].AidDistance = 200;
		Step[Control.mExeStep + 3].AidU = Step [Control.mMaxStep - 1].AidU;
		Step[Control.mExeStep + 3].AidX = Step [Control.mMaxStep - 1].AidX;
		Step[Control.mExeStep + 3].AidY = Step [Control.mMaxStep - 1].AidY;
		Step[Control.mExeStep + 3].AidZ = Step [Control.mMaxStep - 1].AidZ;
		Step[Control.mExeStep + 3].CaliDistance = Step [Control.mMaxStep - 1].CaliDistance;
		Step[Control.mExeStep + 3].DACC = 625;
		Step[Control.mExeStep + 3].Derection = Step [Control.mMaxStep - 1].Derection;
		Step[Control.mExeStep + 3].MaxSpeed = Step [Control.mMaxStep - 1].MaxSpeed;
		Step[Control.mExeStep + 3].MicroDistance = 200;
		Step[Control.mExeStep + 3].MotionType = Step [Control.mMaxStep - 1].MotionType;
		Step[Control.mExeStep + 3].ReducePoint = 200;//Step [Control.mMaxStep - 1].MicroDistance;
		Step[Control.mExeStep + 3].SpeedRate = Step [Control.mMaxStep - 1].SpeedRate;
		Step[Control.mExeStep + 3].StartSpeed = Step [Control.mMaxStep - 1].StartSpeed;
		Step[Control.mExeStep + 3].StartType = Step [Control.mMaxStep - 1].StartType;
		Control.mMaxStep = Control.mExeStep + 4;
*/
//		Step[Control.mExeStep].CaliDistance =150;
		Control.mMaxStep = Control.mExeStep + 3;
		Step[Control.mExeStep +2].AidDistance = 10000; // Config AidDistance
		Step[Control.mExeStep +2].DACC = 5000; //decrease rate
		Step[Control.mExeStep +2].ACC = 5000;	//increase rate
		Step[Control.mExeStep +2].MaxSpeed = 5000;	//Max Speed
		Step[Control.mExeStep +2].StartSpeed = 500; // Start speed
		Step[Control.mExeStep +2].ReducePoint = 2000;

/*		for(int i = Control.mExeStep + 4; i<Control.mMaxStep; i++){
			Step[i].AidDistance = 0;
		}

		for(int j = Control.mExeLine + 1 ; j<Control.mMaxLine; j++){
			Line[j].AidDistance = 0;
			Line[j].Derection = 0;
		}
		INT32 xLocation = INT32(DOUBLE(GetShiftR(&m_Device,XADDR)) * Puls2Distance);
		Line[Control.mExeLine].AidDistance = 3*200 +(200 - (INT32)abs(xLocation%200));

		Control.mMaxLine = Control.mExeLine + 1;

  */
		m_log8<<"After modified : Control.mMaxStep : "<<Control.mMaxStep<<endl<<"Control.mExeStep : "<< Control.mExeStep<< endl;
		m_log8<<" far to the distination"<<endl<<" Step[Control.mExeStep+2].DACC = "<< Step[Control.mExeStep+2].DACC<<endl;
		m_log8<<" Step[Control.mExeStep+2].ReducePoint  "<<Step[Control.mExeStep+2].ReducePoint<<endl;
		m_log8<<" Line[Control.mExeLine].AidDistance : "<< Line[Control.mExeLine].AidDistance<< endl;
		m_log8.close();
	}else{
		FindBarEnable		= FALSE;			// 禁止自动识别条码
		Control.mStep		= 0;				// 复位步骤指针
		Control.mExeStep	= 0;				// 复位微步指针
		Control.mStepEnable = 0;				// 复位禁用自动控制
		Control.mFixStep = 0;					// 复位自动校正步数
		
		Control.mPause		= 0;				// 复位暂停旗标
		Control.mGoOn		= 0;				// 复位继续路径运动旗标
		Control.mRevers		= 0;				// 复位反向旗标
		
		Control.mLine		= 0;				// 复位单段步骤指针
		Control.mExeLine	= 0;				// 复位单段执行指针
		Control.mLineEnable = 0;				// 复位单�
	}
}
