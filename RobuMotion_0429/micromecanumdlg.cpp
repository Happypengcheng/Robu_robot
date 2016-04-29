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

    UINT server_thd(LPVOID p); //ÉùÃ÷Ïß³Ìº¯Êý
	SOCKET listen_sock;//¶¨ÒåÒ»¸öÈ«¾ÖµÄ¼àÌýsoket
	SOCKET sock;//¶¨ÒåÒ»¸ösoket
	CString showeditmsg; //¶¨ÒåÈ«¾ÖµÄÏÔÊ¾ÏûÏ¢µÄ×Ö·û´®


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
	
	if (!OpenProcessToken(GetCurrentProcess(),				// OpenProcessToken() Õâ¸öº¯ÊýµÄ×÷ÓÃÊÇ´ò¿ªÒ»¸ö½ø³ÌµÄ·ÃÎÊÁîÅÆ			
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,&hToken))		// GetCurrentProcess() º¯ÊýµÄ×÷ÓÃÊÇµÃµ½±¾½ø³ÌµÄ¾ä±ú			
		return FALSE;	
	LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);		//LookupPrivilegeValue()µÄ×÷ÓÃÊÇ²é¿´ÏµÍ³È¨ÏÞµÄÌØÈ¨Öµ£¬·µ»ØÐÅÏ¢µ½Ò»¸öLUID½á¹¹ÌåÀï¡£		
	tkp.PrivilegeCount = 1;									// one privilege to set ¸³¸ø±¾½ø³ÌÌØÈ¨		
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;			
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,(PTOKEN_PRIVILEGES)NULL, 0);	// AdjustTokenPrivileges£¨£©µÄ×÷ÓÃÊÇÍ¨ÖªWindows NTÐÞ¸Ä±¾½ø³ÌµÄÈ¨Àû			
	if (GetLastError() != ERROR_SUCCESS)					//Ê§°Ü			
		return FALSE;		
	if (!ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0))		//²ÎÊýÔÚÕâÀïÉèÖÃ¡£Ç¿ÐÐÍË³öWINDOWS£¨EWX_FORCE£©¡£			
		return FALSE;	
	return TRUE;		
}

BOOL CMicroMecanumDlg::OnInitDialog()	//½çÃæ³õÊ¼»¯
{
	CDialog::OnInitDialog();

	FlagPause =0; // 0 ´ú±íÔÝÍ£×´Ì¬ 1´ú±íÕý³£×´Ì¬

	CurLineNo = 0;					// ¼ÇÂ¼µ±Ç°µÄStepÐòºÅ
	IsEmgcBrake = false;			// ¼ÇÂ¼µ±Ç°µÄStepÐòºÅ

m_ofsLog2.open("log-2ËÑË÷µØ±ê¹ý³Ì.txt",ios::app);
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


 	TableName = "Route";			// Éè¶¨²ÎÊý±íÃû
 	InitializationWindows();		// ³õÊ¼»¯½çÃæ	

	//------------------------------------------------
	// qi
	// ³õÊ¼»¯Ð£×¼µã£¨Õë¶ÔÒ»ÌõLine£©£¬×î¶àÖ§³Ö1024¸ö
	nCheckPoints = 0;
	CheckPoints = new CHECKPOINT[1024];
	CheckMarks = new MARK[1024];

	nLineMarks = 0;
	LineMarks = new MARK[1024];

	nMarks = 0;	// ³õÊ¼»¯µØ±ê
	m_CaliCount = 0;	// Ð£×¼´ÎÊýÖÃ0
	ReadMarkFile();					// ¶ÁÈ¡µØ±êÎÄ¼þ qi
	ReadSearchLines();				// ¶ÁÈ¡µØ±êËÑË÷Â·¾¶ÎÄ¼þ qi
	ReadConfigFile();				// ¶ÁÈ¡ÅäÖÃÎÄ¼þ qi

	m_igvInfo.curX = -10000.0;	// Ð¡³µ³õÊ¼Î´¶¨Î»ºÍ¶¨Ïò
	m_igvInfo.curY = -10000.0;	// Ð¡³µ³õÊ¼Î´¶¨Î»ºÍ¶¨Ïò
	m_igvInfo.preX = m_igvInfo.curX;	//  Ð¡³µÉÏÒ»µã(Ç°½ø¡¢ºóÍËÇ°)×ø±ê£¬µ¥Î»ÊÇºÁÃ×
	m_igvInfo.preY = m_igvInfo.curY;	//  Ð¡³µÉÏÒ»µã(Ç°½ø¡¢ºóÍËÇ°)×ø±ê£¬µ¥Î»ÊÇºÁÃ×
	m_igvInfo.isFree = true;
	m_igvInfo.Speed = 0;
	m_igvInfo.MoveDirection = -1;
	m_igvInfo.isMarked = false;
	m_igvInfo.MarkID = "";

	m_igvInfo.Compass = -1.0;	// Ð¡³µ³õÊ¼Î´¶¨Î»ºÍ¶¨Ïò
	
	m_igvInfo.Heading = 270.0;	// Ã»ÓÐ°²×°µØ´Å¸ÐÓ¦Ç°£¬³µÍ··½ÏòÔÝ¶¨Îª90£¨ÊÊÓ¦ÊµÑéÊÒµÄ×ø±êÏµ£©
	//m_Heading = -1.0;	// Ð¡³µ³õÊ¼Î´¶¨Î»ºÍ¶¨Ïò
	m_HeadingCombBxo.SetCurSel(3);
	m_HeadingCombBxo.UpdateData(TRUE);

	m_NotFindMarkTimes370 = 0;	// Ö´ÐÐ370Ö¸ÁîÊ±Î´·¢ÏÖmarkµÄ´ÎÊý
	m_iCurrentSearchingRoute = -1;		// µ±Ç°ÕýÔÚÖ´ÐÐµÄµØ±êËÑË÷Â·¾¶Ë÷Òý, -1±íÊ¾Î´½øÐÐµØ±êËÑË÷
	m_isSearchingMark = false;	// ²»Ö´ÐÐËÑË÷µØ±ê
	m_isSearchingMarkFound = false;	// ÉÐÎ´ËÑµ½µØ±ê
	// end qi 
	//------------------------------------------------	
		
	DeknockEnable = TRUE;

	InitializationMotion();			// Initialization motion control
	InitRs232c();					// ³õÊ¼»¯RS232C
	//InitializationCamera();		// ³õÊ¼»¯Ïà»ú
//	CameraInitional();			// ³õÊ¼»¯Ïà»ú

	//------------------------------------------------
	// pc
	//ÒÔÏÂ³õÊ¼»¯Í¼ÏñÆ¥Åä×é¼þ
	m_Pattern.Load("pattern.bmp");	// ¼ÓÔØÄ£°å
	m_Roi.Attach(&m_Pattern);
	m_Roi.SetPlacement(0,0,268,44);
	m_Match.SetMaxPositions(1);// return only one position
	m_Match.SetMinScore(0.25f);	// Æ¥Åä¶ÈÖÁÉÙ´ïµ½0.25
	ESetAngleUnit(E_ANGLE_UNIT_DEGREES);
	//m_Match.SetMaxAngle(1);
	//m_Match.SetMinAngle(-1);
	//m_Match.SetInterpolate(true);
	//m_Match.SetMaxScale(2);
	//m_Match.SetMinScale(0.5);
	m_Match.LearnPattern(&m_Roi);
	
	Temp_LastBarCodeid = "";	// ³õÊ¼»¯Îª¿Õ
	m_isSocketConnected = false;	
	// GetDlgItem(IDC_STATIC_Connect)->SetWindowText("Î´Á¬½Ó");
	m_ConnectStatus.SetWindowText("Î´Á¬½Ó");

	InitializationSoket();	// ³õÊ¼»¯soketÍ¬Ê±¿ªÆôÏß³Ì½ÓÊÕÏûÏ¢
	m_iTimeCounter = 0;		// ¼ÆÊýÆ÷£¬OnTimerÃ¿Ö´ÐÐ10´Î»Ø´«Ò»´Î×ÔÉíÎ»ÖÃºÍ×´Ì¬ÐÅÏ¢
	
	// end pc 
	//------------------------------------------------ 

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// ¶ÁÈ¡ÅäÖÃÎÄ¼þ
void CMicroMecanumDlg::ReadConfigFile()
{
	ifstream fs;

	fs.open("config.txt", ios::nocreate);//Ä¬ÈÏÒÔ ios::in µÄ·½Ê½´ò¿ªÎÄ¼þ£¬ÎÄ¼þ²»´æÔÚÊ±²Ù×÷Ê§°Ü

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
		RotateFactor = 1/2;	// È±Ê¡Öµ
		RotateLimit = 1;		// È·ÊµÖµ1¶È
		
	}

	fs.close();
	
	//CString str;
	//str.Format("factor = %.4f, limit=%.1f", RotateFactor,RotateLimit);
	//MessageBox(str,"",MB_OK);

}

// ¶ÁÈ¡µØ±êÎÄ¼þ qi
void CMicroMecanumDlg::ReadMarkFile()
{
	ifstream fs;

	fs.open("marks.txt", ios::nocreate);//Ä¬ÈÏÒÔ ios::in µÄ·½Ê½´ò¿ªÎÄ¼þ£¬ÎÄ¼þ²»´æÔÚÊ±²Ù×÷Ê§°Ü

	if(fs)
	{
		fs>>nMarks;
		char* sline = new char[24*nMarks];	// ´Ë´¦ÄÚ´æÓÐÓÅ»¯¿Õ¼ä
		char* pline = sline;	// ¼ÇÂ¼slineÖ¸ÕëÎ»ÖÃ£¬ÒòÎªºóÃæslineÖ¸ÕëÒÆÎ»ÁË
		marks = new MARK[nMarks];
		int i = 0;
		fs.getline(sline,256,'\n');//¶ÁÈ¡»Ø³µ»»ÐÐ
		while(!fs.eof() && i<nMarks)
		{
			fs.getline(sline,256,'\n');//"12345,100,0,90"
			if(strlen(sline) > 0)
			{
				char* buf = strstr(sline, ",");//",100,0,90"
				buf[0] = '\0';//´ËÊ±sline±ä³ÉÁË"12345"
				marks[i].barCode = sline;
				
				sline = buf + 1; //"100,0,90"
				buf = strstr(sline, ",");//",0,90"
				buf[0] = '\0';//´ËÊ±sline±ä³ÉÁË"100"
				marks[i].x = atoi(sline);

				sline = buf + 1; //"0,90"
				buf = strstr(sline, ",");//",90"
				buf[0] = '\0';//´ËÊ±sline±ä³ÉÁË"0"
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

	// Éú³ÉÓ³Éä
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
				str.Format("line mark count: %d¸ö", nLineMarks);
				MessageBox(str);
*/
}

// ¸ù¾ÝÌõÂëµÃµ½µØ±êÐÅÏ¢
MARK* CMicroMecanumDlg::GetMarkInfo(CString barCode)
{
	return mapMarks[barCode];

}

void CMicroMecanumDlg::OnButton1() //½ô¼±Í£Ö¹/¹Ø±ÕµçÔ´
{
Control.mMaxLine = 0;

	CString str;
	GetDlgItem(IDC_BUTTON1)->GetWindowText(str);
	if (str == "¹Ø±ÕµçÔ´")
	{
		m_ofsLog2.close();
		StopMotion();
		SetOutput(&m_Device,ZADDR, 0x00FF);	//P4 ~ P7Êä³ö¸ßµçÆ½1£¬½ûÄÜËÄÖáÇý¶¯Æ÷
		SetOutput(&m_Device,UADDR, 0x00FF);	//P4 ~ P7Êä³ö¸ßµçÆ½1£¬¹Ø±ÕÇý¶¯Æ÷µçÔ´
		pProcessThread.pause();				// Ïß³ÌÔÝÍ£½ÓÊÜÊý¾Ý//¹Ø±ÕÏß³Ì
		pProcessThread.ExitInstance();		// ÍË³öÏß³Ì
		GetDlgItem(IDC_BUTTON1)->SetWindowText("´ò¿ªµçÔ´");
	} 
	else
	{
		m_ofsLog2.open("log-2ËÑË÷µØ±ê¹ý³Ì.txt",ios::app);
		SetOutput(&m_Device,ZADDR, 0x000F);	//P4 ~ P7Êä³öµÍµçÆ½0£¬Ê¹ÄÜËÄÖáÇý¶¯Æ÷
		SetOutput(&m_Device,UADDR, 0x003F);	//P4 ~ P7Êä³öµÍµçÆ½0£¬´ò¿ªÇý¶¯Æ÷µçÔ´

		Control.mLineEnable = 0;	// pc 2016.03.30

		GetDlgItem(IDC_BUTTON1)->SetWindowText("¹Ø±ÕµçÔ´");
	}
}

void CMicroMecanumDlg::OnButton2()	//ÏµÍ³¸´Î»
{
	MotionInit();
//	Init(&m_Device);
//	SetOutput(&m_Device,ZADDR, 0x000F);	//P4 ~ P7Êä³öµÍµçÆ½0£¬Ê¹ÄÜËÄÖáÇý¶¯Æ÷
//	SetOutput(&m_Device,UADDR, 0x000F);	//P4 ~ P7Êä³öµÍµçÆ½0£¬´ò¿ªÇý¶¯Æ÷µçÔ´
}

void CMicroMecanumDlg::OnButton3()	//Ê¹ÓÃËµÃ÷ 
{
	// ²âÊÔ
	//UpdateHeading(361,795);

	CAboutDlg Dlg;
	Dlg.DoModal();	
}

void CMicroMecanumDlg::OnButton4()	//Â·¾¶±à¼­ 
{
	CRoute Dlg; 
	Dlg.DoModal();
}

void CMicroMecanumDlg::OnButton5()	//Í¨Ñ¶Éè¶¨
{
	CCom Dlg;
	Dlg.DoModal();
}

void CMicroMecanumDlg::OnButton6()	//×ø±ê¸´Î»(¾²Ì¬Ð£×¼)  
{
// 	if ((FindBarEnable == FALSE) && (Control.mLineEnable != 0))
// 	{
// 		FindBarEnable = TRUE;	
// 	}
	Line[0].Derection = 370;
	Line[0].MaxSpeed = 500;
	Line[0].ACC = 500;
	Line[0].DACC = 500;
	if (Control.mLineEnable == 0)		// Ö»ÓÐÉÏÒ»ÔË¶¯Íê³É£¬²ÅÄÜ¿ªÊ¼ÐÂµÄÔË¶¯£»ÈôÔË¶¯ÖÐ£¬²»Ö´ÐÐÈÎºÎ¶¯×÷
	{
		Control.mStep = 0;				// ¸´Î»²½ÖèÖ¸Õë
		Control.mExeStep = 0;			// ¸´Î»Î¢²½Ö¸Õë
		Control.mStepEnable = 0;		// ¸´Î»½ûÓÃ×Ô¶¯¿ØÖÆ
		Control.mFixStep = 0;			// ¸´Î»×Ô¶¯Ð£Õý²½Êý
		
		Control.mPause		= 0;		// ¸´Î»ÔÝÍ£Æì±ê
		Control.mGoOn		= 0;		// ¸´Î»¼ÌÐøÂ·¾¶ÔË¶¯Æì±ê
		Control.mRevers		= 0;		// ¸´Î»·´ÏòÆì±ê
		
		Control.mLine		= 0;		// ¸´Î»µ¥¶Î²½ÖèÖ¸Õë
		Control.mExeLine	= 0;		// ¸´Î»µ¥¶ÎÖ´ÐÐÖ¸Õë
		Control.mMaxLine	= 1;		// ¸´Î»µ¥¶ÎÖ´ÐÐÖ¸Õë
		Control.mLineEnable = 0XFF;		// ¿ªÊ¼Ö´ÐÐÎ¢²½³ÌÐò
		FindBarEnable		= TRUE;		// ÔÊÐí×Ô¶¯Ê¶±ðÌõÂë
	}
}

void CMicroMecanumDlg::OnButton7()	//×ø±êÉè¶¨
{
	DisFixInfo = "";
	GetDlgItem(IDC_STATIC2)->SetWindowText(DisFixInfo);

	if (DisplayImage == TRUE)		// ÉèÖÃÒ»¸öÍ¼ÏñÏÔÊ¾¿ª¹Ø
	{
		//½ûÖ¹ÏÔÊ¾Í¼Ïñ
		DisplayImage = FALSE;
		GetDlgItem(IDC_BUTTON7)->SetWindowText("¿ªÊ¼½âÂë");	//×ø±êÉè¶¨ 
	}
	else
	{
		//°Ñ²É¼¯µÄÍ¼ÏñÏÔÊ¾ÔÚ½çÃæÉÏDraw the image
		DisplayImage = TRUE;
		GetDlgItem(IDC_BUTTON7)->SetWindowText("½ûÖ¹½âÂë");	//×ø±êÉè¶¨ 
	}
}

void CMicroMecanumDlg::OnButton8()	//×Ô¶¯ÐÐ×ß 
{
	CString str;
	GetDlgItem(IDC_BUTTON1)->GetWindowText(str);
	if (str == "¹Ø±ÕµçÔ´")
	{
		nCheckPoints = 0;
		StartMotion();				// ¿ªÊ¼ÐÐ×ß
	}
	else
	{
		MessageBox("Çý¶¯Æ÷µçÔ´ÉÐÎ´´ò¿ª£¬Çë´ò¿ªµçÔ´!","¡¾ÑÏÖØ´íÎó¡¿",MB_OK|MB_ICONWARNING|MB_APPLMODAL);
	}
}

void CMicroMecanumDlg::OnCancel()	//¹Ø±Õ³ÌÐò
{
	INT i = MessageBox("ÇëÈ·ÈÏ»úÆ÷ÈË´¦ÓÚ¿ÉÒÔ¹Ø±Õ³ÌÐòµÄ×´Ì¬!","¡¾¼´½«¹Ø±Õ¿ØÖÆ³ÌÐò¡¿",MB_YESNO|MB_ICONWARNING|MB_APPLMODAL);	
	if (i == 6)								//È·ÈÏ¹Ø±Õ
	{
		//---------------qi-----------------------
		// É¾³ýµØ±êÄÚ´æ
		if(!marks)
		{
			delete[] marks;		
		}
		//delete marks;
		//É¾³ýÐ£×¼µã
		if(!CheckPoints)
			delete[] CheckPoints;
		if(!CheckMarks)
			delete[] CheckMarks;
		if(!LineMarks)
			delete[] LineMarks;

		//------------------------------------

		Stop(&m_Device,SDSTP);				// ¼õËÙÍ£Ö¹
		SetOutput(&m_Device,ZADDR, 0x00FF);	//P4 ~ P7Êä³öµÍµçÆ½0£¬½ûÄÜËÄÖáÇý¶¯Æ÷
		SetOutput(&m_Device,UADDR, 0x00FF);	//P4 ~ P7Êä³ö¸ßµçÆ½1£¬¹Ø±ÕÇý¶¯Æ÷µçÔ´
		if(!DeviceClose(&m_Device))			//¹Ø±ÕÔË¶¯¿ØÖÆ¿¨
		{
			MessageBox("ÔË¶¯¿ØÖÆ¿¨¹Ø±ÕÊ§°Ü£¬Close fail!");		
		}
		ZjCom.SetPortOpen(0);				// ¹Ø±Õ´®¿Ú 
		KillTimer(1);						// ¹Ø±Õ¶¨Ê±Æ÷
		Sleep(200);							// µÈ´ý100ms
		pProcessThread.pause();				// Ïß³ÌÔÝÍ£½ÓÊÜÊý¾Ý
		pProcessThread.ExitInstance();		// ÍË³öÏß³Ì
		CameraFree(0);						// ¹Ø±ÕÏà»ú
		CDialog::OnCancel();
	}
}

void CMicroMecanumDlg::OnButton9()	//¹Ø±ÕÏµÍ³ 
{
	INT i = MessageBox("ÇëÈ·ÈÏ»úÆ÷ÈË´¦ÓÚ¿ÉÒÔ¹Ø±ÕµçÔ´µÄ×´Ì¬!","¡¾¼´½«ÇÐ¶Ï»úÆ÷ÈËµçÔ´¡¿",MB_YESNO|MB_ICONWARNING|MB_APPLMODAL);	
	if (i == 6)								//È·ÈÏ¹Ø±Õ
	{
		Stop(&m_Device,SDSTP);				// ¼õËÙÍ£Ö¹
		SetOutput(&m_Device,ZADDR, 0x00FF);	//P4 ~ P7Êä³öµÍµçÆ½0£¬½ûÄÜËÄÖáÇý¶¯Æ÷
		SetOutput(&m_Device,UADDR, 0x00FF);	//P4 ~ P7Êä³ö¸ßµçÆ½1£¬¹Ø±ÕÇý¶¯Æ÷µçÔ´
		if(!DeviceClose(&m_Device))			//¹Ø±ÕÔË¶¯¿ØÖÆ¿¨
		{
			MessageBox("ÔË¶¯¿ØÖÆ¿¨¹Ø±ÕÊ§°Ü£¬Close fail!");		
		}
		pProcessThread.pause();				// Ïß³ÌÔÝÍ£½ÓÊÜÊý¾Ý
		pProcessThread.ExitInstance();		// ÍË³öÏß³Ì//lipan_ÐÞ¸ÄÁË¹Ø±Õ³ÌÐòµÄ²Ù×÷Ë³Ðò
		ZjCom.SetPortOpen(0);				// ¹Ø±Õ´®¿Ú
		KillTimer(1);						// ¹Ø±Õ¶¨Ê±Æ÷
		Sleep(200);							// µÈ´ý100ms
		CameraFree(0);						// ¹Ø±ÕÏà»ú
		CDialog::OnCancel();				// ¹Ø±Õ¿ØÖÆ½çÃæ
		SystemShutdown();					// ¹Ø±ÕµçÄÔ

	}	
}
//system control--------------------------------------------------------------------------------------

#define DataBaseBlock 1
//ADO LIST 4 --------------------------------------------------------------------------------------
BOOL CMicroMecanumDlg::InitializationWindows(void)	// init windows
{	
	//½ø³ÌÓëÖ¸¶¨cpu°ó¶¨
	SetProcessAffinityMask(GetCurrentProcess(), 8);	
	
	// È·ÈÏÏµÍ³ÆÁÄ»ÉèÖÃ
	int W = GetSystemMetrics(SM_CXSCREEN);
	int H = GetSystemMetrics(SM_CYSCREEN);	
	MoveWindow(0,0,W,H-50);
	TRACE("ÏµÍ³Ä¬ÈÏ·Ö±æÂÊ%d*%d!\n",W,H);
	// È·ÈÏ¹¤×÷´°¿Ú
	CRect windows;
	GetClientRect(&windows);
	int x = windows.Width();
	int y = windows.Height();
	TRACE("ÏÔÊ¾´°¿Ú%d*%d!\n",x,y);
	// ÉèÖÃ°´¼ü		
	CRect rect;
	int bw = x / 20;
	int bh = y / 20;
	rect.left	= 19 * bw;
	rect.top	= 0;
	rect.right	= rect.left + bw;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON1)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON1)->SetWindowText("¹Ø±ÕµçÔ´");		//¹Ø±ÕµçÔ´ "½ô¼±Í£Ö¹"
	rect.top	= 1*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON2)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON2)->SetWindowText("ÏµÍ³¸´Î»");		//²âÊÔÔËÐÐ ÏµÍ³¸´Î»
	rect.top	= 2*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON3)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON3)->SetWindowText("Ê¹ÓÃËµÃ÷");
	rect.top	= 3*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON4)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON4)->SetWindowText("Â·¾¶±à¼­");
	rect.top	= 4*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON5)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON5)->SetWindowText("Í¨Ñ¶Éè¶¨");
	rect.top	= 5*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON6)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON6)->SetWindowText("×Ô¶¯¶¨Î»");	//×ø±ê¸´Î» Çå¿ÕÏÔÊ¾
	rect.top	= 6*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON7)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON7)->SetWindowText("½ûÖ¹½âÂë");	//×ø±êÉè¶¨ 
	rect.top	= 7*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON8)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON8)->SetWindowText("¿ªÊ¼ÔË¶¯");	//¿ªÊ¼ÔË¶¯ ×Ô¶¯ÐÐ×ß
	rect.top	= 8*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDCANCEL)->MoveWindow(&rect);	
	GetDlgItem(IDCANCEL)->SetWindowText("¹Ø±Õ³ÌÐò");
	rect.top	= 9*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON9)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON9)->SetWindowText("¹Ø±ÕÏµÍ³");
	TRACE("°´¼ü×óÓÒ%d*%d!\n",rect.left,rect.right);
	// ÉèÖÃEditbox
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
// 	// ÉèÖÃlist control
// 	bw = 19 * bw / 16;
// 	bh = y / 40;
// 	rect.left	= 0;
// 	rect.top	= y - bh-10;
// 	rect.right	= x;
// 	rect.bottom = y - 10;
// 	GetDlgItem(IDC_LIST2)->MoveWindow(&rect);
// 	TRACE("ÁÐ±í´°¿Ú%d*%d!\n",rect.right,rect.bottom);

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


	// ÉèÖÃ¾²Ì¬ÎÄ±¾ÏÔÊ¾´°¿Ú2
	rect.left	= 0;
	rect.top	= y - 6*bh;
	rect.right	= x;
	rect.bottom = y - 4*bh;
	GetDlgItem(IDC_STATIC2)->MoveWindow(&rect);
	// ÉèÖÃ¾²Ì¬ÎÄ±¾ÏÔÊ¾´°¿Ú1
	rect.left	= 0;
	rect.top	= y - 4*bh;
	rect.right	= x;
	rect.bottom = y - 2*bh;
	GetDlgItem(IDC_STATIC1)->MoveWindow(&rect);

	// ÉèÖÃÐ¡³µ³µÍ··½Ïò
	m_HeadingCombBxo.AddString("0");
	m_HeadingCombBxo.AddString("90");
	m_HeadingCombBxo.AddString("180");
	m_HeadingCombBxo.AddString("270");
	
	return TRUE;
}
void CMicroMecanumDlg::InitADOConn(void)	//´´½¨ADOÁ¬½Ó
{
	try
	{
		//´´½¨Á¬½Ó¶ÔÏóÊµÀý
		m_pConnection.CreateInstance("ADODB.Connection");
		//ÉèÖÃÁ¬½Ó×Ö·û´®
		CString strConnect="DRIVER={Microsoft Access Driver (*.mdb)};uid=;pwd=;DBQ=iRoute.mdb;";
		//Ê¹ÓÃOpen·½·¨Á¬½ÓÊý¾Ý¿â
		m_pConnection->Open((_bstr_t)strConnect,"","",adModeUnknown);
	}
	catch(_com_error e)
	{
		//²¶»ñ²¢ÏÔÊ¾´íÎó
		AfxMessageBox(e.Description());
	}
}

void CMicroMecanumDlg::ExitConnect(void)	//¹Ø±Õ¼ÇÂ¼¼¯ºÍÁ¬½Ó
{
	//¹Ø±Õ¼ÇÂ¼¼¯ºÍÁ¬½Ó
    if(m_pRecordset!=NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
}

void CMicroMecanumDlg::Test(void)	// °ÑÊý¾Ý¿âÊý¾Ý×ªÎªÂ·¾¶Êý¾Ý
{

}
void CMicroMecanumDlg::Database2Route(void)	// °ÑÊý¾Ý¿âÊý¾Ý×ªÎªÂ·¾¶Êý¾Ý
{
	//¶¨ÒåÊý¾Ý¿â±ê×¼ÁÐÃû³ÆºÍÊý¾Ý
	CString	StdData[MovementParament]={"1","0","500","0","1000","1000","1000","0","400","1","2","3","4","100","200","300"};
	CString StdName[MovementParament]={"StepID","CommandID","AidDistance","StartSpeed","MaxSpeed","ACC","DACC","CaliDistance","MictoDistance","PortX","PortY","PortZ","PortU","Data1","Data2","Data3",};	
	//Á¬½ÓÊý¾Ý¿â
	InitADOConn();
	// ¶¨ÒåÒ»¸ö×Ö·û´®
	CString str;
	//¼ÇÂ¼¼¯°´¡°StepID¡±ÉýÐòÅÅÁÐ
	str.Format("select * from %s order by StepID ",TableName);
	_bstr_t bstrSQL = str;
	//´´½¨¼ÇÂ¼¼¯Ö¸Õë¶ÔÏóÊµÀý
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	//´ò¿ª¼ÇÂ¼¼¯
	m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);	
	//¸´Î»Â·¾¶¶ÎÊýÖ¸Õë
	Control.mMaxLine = 0;	
	//½«¼ÇÂ¼¼¯Ö¸ÕëÒÆ¶¯µ½µÚÒ»Ìõ¼ÇÂ¼
	m_pRecordset->MoveFirst();
	//ÏÔÊ¾µ÷ÊÔÐÅÏ¢
	//TRACE("\n"); 
	//TRACE("¿ªÊ¼¶ÁÈ¡Â·¾¶Êý¾Ý!\n"); 
	//¶ÁÈ¡Êý¾Ý¿âÊý¾Ý²¢ÏÔÊ¾ÔÚÁÐ±í¿Ø¼þÉÏ
	while(!m_pRecordset->adoEOF)
	{
		// 1.ÔË¶¯·½Ïò
		Line[Control.mMaxLine].Derection	= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("CommandID"));	
		// 2.Ä¿±ê¾àÀë
		Line[Control.mMaxLine].AidDistance	= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("AidDistance"));	
		// 3.ÔË¶¯ÀàÐÍ
		Line[Control.mMaxLine].MotionType	= LineRips;
		// 4.ËÙ¶È±¶ÂÊ
		Line[Control.mMaxLine].SpeedRate	= sRate;
		// 5.Æô¶¯ËÙ¶È
		Line[Control.mMaxLine].StartSpeed	= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("StartSpeed"));
		// 6.×î¸ßËÙ¶È
		Line[Control.mMaxLine].MaxSpeed		= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("MaxSpeed"));
		// 7.¼Ó¼ÓËÙ¶È
		Line[Control.mMaxLine].ACC			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("ACC"));	
		// 8.¼õ¼ÓËÙ¶È
		Line[Control.mMaxLine].DACC			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("DACC"));
		// 9.¼õËÙ¾àÀë
		Line[Control.mMaxLine].ReducePoint	= 0;
		// 10.Æô¶¯ÀàÐÍ
		Line[Control.mMaxLine].StartType	= STAUD;
		// 11.Ð£Õý¾àÀë
		Line[Control.mMaxLine].CaliDistance	= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("CaliDistance"));
		// 12.Î¢²½²½³¤	
		Line[Control.mMaxLine].MicroDistance =  atoi((char*)(_bstr_t)m_pRecordset->GetCollect("MictoDistance"));
		// 13.X·ÖÖáÄ¿±ê
		Line[Control.mMaxLine].AidX			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("PortX"));
		// 14.Y·ÖÖáÄ¿±ê
		Line[Control.mMaxLine].AidY			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("PortY"));
		// 15.Z·ÖÖáÄ¿±ê
		Line[Control.mMaxLine].AidZ			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("PortZ"));
		// 16.U·ÖÖáÄ¿±ê	
		Line[Control.mMaxLine].AidU			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("PortU"));	
		// Êä³öµ÷ÊÔÐÅÏ¢
		//TRACE("µÚ%d#Â·¶ÎÊý¾Ý×ª»»Íê±Ï!\n",Control.mMaxLine); 
		// ÏÔÊ¾¶ÁÈ¡µÄÔË¶¯²ÎÊý
		//DisplayPar(&Line[Control.mMaxLine]);		
		//µÝÔö¼ÆÊýÆ÷
		Control.mMaxLine++;
		//½«¼ÇÂ¼¼¯Ö¸ÕëÒÆ¶¯µ½ÏÂÒ»Ìõ¼ÇÂ¼
		m_pRecordset->MoveNext();
	}
	//¶Ï¿ªÊý¾Ý¿âÁ¬½Ó
	ExitConnect();
	// Êä³öµ÷ÊÔÐÅÏ¢
	//TRACE("Â·¾¶×Ü¼ÆÉú³É%d¸öÂ·¶Î!\n",Control.mMaxLine); 
	//TRACE("\n"); 

 }
//ADO LIST 4 --------------------------------------------------------------------------------------

#define MotionBlock	2 
// mecanum Ð¡³µÔË¶¯¿ØÖÆÄ£¿é ---------------------------------------------------
CString CMicroMecanumDlg::DisplayPar(MotionParemeter* line)
{
	CString str;
// 	str.Format("·½Ïò%d,¾àÀë%d,ÔË¶¯ÀàÐÍ0X%X,ËÙ¶È±¶ÂÊ%d,Æô¶¯ËÙ¶È%d,×î¸ßËÙ¶È%d,¼Ó¼ÓËÙ¶È%d,¼õ¼ÓËÙ¶È%d,¼õËÙµã%d,Æô¶¯ÀàÐÍ0X%X,Ð£Õý¾àÀë%d,Î¢²½¾àÀë%d,X%d,Y%d,Z%d,U%d,\n",
// 		line->Derection,line->AidDistance,line->MotionType,line->SpeedRate,
// 		line->StartSpeed,line->MaxSpeed,line->ACC,line->DACC,
// 		line->ReducePoint,line->StartType,line->CaliDistance,line->MicroDistance,
// 		line->AidX,line->AidY,line->AidZ,line->AidU);
	str.Format("%d,%d,ÔË¶¯ÀàÐÍ0X%X,%d,%d,%d,%d,%d,¼õËÙµã%d,Æô¶¯ÀàÐÍ0X%X,%d,Î¢²½¾àÀë%d,X%d,Y%d,Z%d,U%d,\n",
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
	// Á¬½Ó·þÎñ¶Ë·¢ËÍ
	pConnectSocketThread.CreateThread();
	pConnectSocketThread.play();
}

BOOL CMicroMecanumDlg::InitializationMotion(void)
{
	// ³õÊ¼»¯ÔË¶¯¿ØÖÆÄ£¿é
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
		MessageBox("Ã»ÓÐ·¢ÏÖÔË¶¯¿ØÖÆ¿¨£¬ÎÞ·¨ÆôÓÃÔË¶¯¿ØÖÆ¹¦ÄÜ!",
			"¡¾ÑÏÖØ´íÎó¡¿",
			MB_OK|MB_ICONWARNING|MB_APPLMODAL);		
	}	
//	Init(&m_Device);
//	SetOutput(&m_Device,ZADDR, 0x000F);			//P4 ~ P7Êä³öµÍµçÆ½0£¬Ê¹ÄÜËÄÖáÇý¶¯Æ÷
//	SetOutput(&m_Device,UADDR, 0x000F);			//P4 ~ P7Êä³öµÍµçÆ½0£¬´ò¿ªÇý¶¯Æ÷µçÔ´
	MotionInit();
	//InitialPar(&Control,Line,Step,CaliMap);		// ³õÊ¼»¯¹ì¼£¿ØÖÆ²ÎÊý
	InitialPar(&Control,Line,Step,CaliMap,pPort,&VportPar);		// ³õÊ¼»¯¹ì¼£¿ØÖÆ²ÎÊý

	Control.PlusUnit = Puls2Distance;
	// ÉèÖÃÔË¶¯¿ØÖÆ×¨ÓÃ¶¨Ê±Æ÷ 	
	SetTimer(1, 50, NULL);						// ³õÊ¼»¯ÉèÖÃ¶¨Ê±Æ÷	
	return TRUE;
}

void CMicroMecanumDlg::MotionInit()	//ÔË¶¯ÏµÍ³¸´Î»
{
	Init(&m_Device);
	SetOutput(&m_Device,ZADDR, 0x000F);	//P4 ~ P7Êä³öµÍµçÆ½0£¬Ê¹ÄÜËÄÖáÇý¶¯Æ÷
	SetOutput(&m_Device,UADDR, 0x003F);	//P4 ~ P7Êä³öµÍµçÆ½0£¬´ò¿ªÇý¶¯Æ÷µçÔ´
	//return TRUE;
}

inline	BOOL CMicroMecanumDlg::Display(void)			// ÔË¶¯×´Ì¬ÏÔÊ¾
{
	// ¼ÆÊ±¾²Ì¬±äÁ¿
	static U32 SecondDis,MotionTime,LastStep;
	// ºÏ³ÉÏÔÊ¾ÓÃ¾Ö²¿±äÁ¿
	CString str,tmp;
	// ×Ô¶¯µÝÔö¼ÆÊ±£¬U32 Ô¼¿ÉÒÔ¼ÆÊ±Ò»¸öÔÂ// SecondDis = 0;
	SecondDis++;
	// Ã¿Ãë¸üÐÂ
	if ((SecondDis%10) == 0)					
	{
		// ÆðÊ¼Ê±¼äµã
		DWORD SartTime =  GetCurrentTime();			
		// ÒÀ¾ÝÔË¶¯Çé¿öÊä³öÐÅÏ¢
		if (Control.mLineEnable == 0)// Èô²»ÔË¶¯Ê±
		{
			// ¸´Î»ÔË¶¯Ê±¼ä
			MotionTime = 0;
			// ¼ýÍ·¿ØÖÆ
			mARROW = 360;
			// Êä³öÔË¶¯ÐÅÏ¢
			str = "ÔË¶¯Í£Ö¹!\n";
		}
		else// ÈôÔË¶¯Ê±
		{	
			// Í³¼ÆÔË¶¯Ê±¼ä
			MotionTime++;
			// ¼ýÍ·¿ØÖÆ
			mARROW = Line[Control.mExeLine].Derection;
			//// ºÏ³É¿ÉÏÔÔË¶¯²ÎÊý
			str = DisplayPar(&Line[Control.mExeLine]);		
		}

		if (CameraReady != TRUE)		// Ã»ÓÐÏà»ú£¬Ö±½ÓÏÔÊ¾
		{
			// »æ»­¼ýÍ·
			DrawArrow(mARROW);
			//TRACE("CameraReady=%d,ÓÃ¶¨Ê±Æ÷ÏÔÊ¾Í¼Ïñ!\n",CameraReady);
		}
		// ¼ÆËãÎ»ÖÃ
		INT32 xLocation = INT32(DOUBLE(GetShiftR(&m_Device,XADDR)) * Puls2Distance);
		// ¼ÆËãÊý¶È
		INT32 xSpeed = INT32(DOUBLE(GetSpeedR(&m_Device,XADDR)) * Puls2Distance);

		// ºÏ³É¿ÉÏÔÊý¾Ý
		tmp.Format(_T("x=%dºÁÃ×,y=%dºÁÃ×£¬ËÙ¶È%dºÁÃ×/Ãë,UÖá¶Ë¿Ú0X%X,"), 
			(int)m_igvInfo.curX,(int)m_igvInfo.curY,xSpeed,GetInput(&m_Device,UADDR));
		// ºÏ³ÉÏÔÊ¾Êý¾Ý
		str = tmp + str;
		SystemInfor = tmp;
		// ½çÃæÏÔÊ¾Êý¾Ý
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);
	}	
	return TRUE;
}
BOOL CMicroMecanumDlg::StartMotion(void)			// ¿ªÊ¼ÔË¶¯
{
	Database2Route();					// ´ÓÊý¾Ý¿âµ¼ÈëÂ·¾¶Êý¾Ý
	if (Control.mLineEnable == 0)		// Ö»ÓÐÉÏÒ»ÔË¶¯Íê³É£¬²ÅÄÜ¿ªÊ¼ÐÂµÄÔË¶¯£»ÈôÔË¶¯ÖÐ£¬²»Ö´ÐÐÈÎºÎ¶¯×÷
	{
		Control.mStep = 0;				// ¸´Î»²½ÖèÖ¸Õë
		Control.mExeStep = 0;			// ¸´Î»Î¢²½Ö¸Õë
		Control.mStepEnable = 0;		// ¸´Î»½ûÓÃ×Ô¶¯¿ØÖÆ
		Control.mFixStep = 0;			// ¸´Î»×Ô¶¯Ð£Õý²½Êý
		
		Control.mPause		= 0;		// ¸´Î»ÔÝÍ£Æì±ê
		Control.mGoOn		= 0;		// ¸´Î»¼ÌÐøÂ·¾¶ÔË¶¯Æì±ê
		Control.mRevers		= 0;		// ¸´Î»·´ÏòÆì±ê
		
		Control.mLine		= 0;		// ¸´Î»µ¥¶Î²½ÖèÖ¸Õë
		Control.mExeLine	= 0;		// ¸´Î»µ¥¶ÎÖ´ÐÐÖ¸Õë
		Control.mLineEnable = 0XFF;		// ¿ªÊ¼Ö´ÐÐÎ¢²½³ÌÐò
		FindBarEnable		= TRUE;		// ÔÊÐí×Ô¶¯Ê¶±ðÌõÂë
	}
	return TRUE;
}
BOOL CMicroMecanumDlg::StopMotion(void)			// Í£Ö¹ÔË¶¯
{
	if (Control.mLineEnable != 0)
	{
		FindBarEnable		= FALSE;			// ½ûÖ¹×Ô¶¯Ê¶±ðÌõÂë
		Control.mStep		= 0;				// ¸´Î»²½ÖèÖ¸Õë
		Control.mExeStep	= 0;				// ¸´Î»Î¢²½Ö¸Õë
		Control.mStepEnable = 0;				// ¸´Î»½ûÓÃ×Ô¶¯¿ØÖÆ
		Control.mFixStep = 0;					// ¸´Î»×Ô¶¯Ð£Õý²½Êý
		
		Control.mPause		= 0;				// ¸´Î»ÔÝÍ£Æì±ê
		Control.mGoOn		= 0;				// ¸´Î»¼ÌÐøÂ·¾¶ÔË¶¯Æì±ê
		Control.mRevers		= 0;				// ¸´Î»·´ÏòÆì±ê
		
		Control.mLine		= 0;				// ¸´Î»µ¥¶Î²½ÖèÖ¸Õë
		Control.mExeLine	= 0;				// ¸´Î»µ¥¶ÎÖ´ÐÐÖ¸Õë
		Control.mLineEnable = 0;				// ¸´Î»µ¥¶ÎÆì±ê
		Stop(&m_Device,STOP);					// STOP / SDSTP
	}
	return TRUE;
}

void CMicroMecanumDlg::OnTimer(UINT nIDEvent)	//ÔË¶¯¿ØÖÆ×¨ÓÃ¶¨Ê±Æ÷  
{
	// ¸üÐÂ×ø±ê	
	INT32 xLocation = INT32(DOUBLE(GetShiftR(&m_Device,XADDR)) * Puls2Distance);	// ¼ÆËãÎ»ÖÃ
	UpdateOwnPosition(abs(xLocation));	// ¸üÐÂÐ¡³µµ±Ç°×ø±ê
	ValidatePosition();					// ¼ì²éÐ¡³µÎ»ÖÃÊÇ·ñÆ«ÀëµØ±êÌ«Ô¶
	// ¸üÐÂÐ¡³µËÙ¶È
	INT32 xSpeed = INT32(DOUBLE(GetSpeedR(&m_Device,XADDR)) * Puls2Distance);
	m_igvInfo.Speed = xSpeed;


	if(Control.mLineEnable == 0)
		m_igvInfo.isFree = true;
	else 
	{
		m_igvInfo.MoveDirection = Line[Control.mExeLine].Derection;
		m_igvInfo.isFree = false;

		if (Line[Control.mExeLine].Derection != 370/*Ð£×¼Ö¸Áî*/ && Line[Control.mExeLine].Derection !=360/*µÈ´ýÖ¸Áî*/)
		{
			// Ö»Òª³µ×Ó·¢ÉúÁËÒÆ¶¯£¬¾Í½«×´Ì¬ÉèÎª¡°Î´Ð£×¼¡±
			m_igvInfo.isMarked = false;
			m_igvInfo.MarkID = "";
ofstream ofsLog;				
ofsLog.open("log-4 isFree×´Ì¬µ÷ÊÔ.txt",ios::app);
ofsLog<<"ÒÆ¶¯ÁË,derection="<<Line[Control.mExeLine].Derection<<"mLine="<<Control.mLine<<"\n";
ofsLog.close();

//m_ofsLog2<<"ÒÆ¶¯ÁË,derection="<<Line[Control.mExeLine].Derection<<"mLine="<<Control.mLine<<"\n";

		}
	}

	// Ã¿¸ô1ÃëÖÓÏò·þÎñ¶Ë·¢ËÍ×ÔÉíµÄÎ»ÖÃºÍ×´Ì¬ÐÅÏ¢
	m_iTimeCounter ++;
	if (m_iTimeCounter == 10)
	{
		m_iTimeCounter = 0;
		SendIGVRealTimeInfo();
	}

	// ÒÔÏÂ´úÂëÓÃÓÚµ±×Ô¶¯¶¨Ïò¸Õ¿ªÊ¼Ê±£¬¼ì²éµ±Ç°±êÊÇ·ñÊÇÖÐÐÄ£¬Èç¹û²»ÊÇÖÐÐÄµØ±ê£¬
	// Ôò²åÈëÒ»¸öÏò×ó»òÓÒµÄÒÆ¶¯£¬Ê¹ÆäÕÕ×¼ÖÐÐÄµØ±ê
	if (Control.mLineEnable != 0	&& Control.mLine <= 20 
		&& Line[Control.mExeLine].Derection == 370)	// ¿ªÊ¼Ö´ÐÐÎ¢²½³ÌÐò)
	{
		Control.mPause = 0XFF;	// ½øÈëÔÝÍ£
		Control.mGoOn = 0XFF;	// ½ûÖ¹Á¬Ðø

		// º¯ÊýºóÐø²¿·Ö²»Ö´ÐÐ
		Display();									
		CDialog::OnTimer(nIDEvent);
		return;
	}

	// Èç¹ûÄ³¸öLineµÄ¶¯×÷Íê³ÉÁË
	if (Control.mStepEnable == 0 && Control.mLine == 50)	
	{
		// Èç¹û×ó×ª»òÕßÓÒ×ªÍê³É£¬ÔòÐÞ¸Ä³µÍ·³¯Ïòm_Heading
		if (Line[Control.mExeLine].Derection == 361 || Line[Control.mExeLine].Derection == 362)	// Èç¹ûÖ´ÐÐ×ªÏò
		{
			// ¸üÐÂ³µÍ·³¯Ïò
			UpdateHeading(Line[Control.mExeLine].Derection, 
				int(Line[Control.mExeLine].AidDistance*Puls2Distance+0.5));	// È¡Õû
		}
		// Èç¹ûÕýÔÚËÑË÷µØ±ê£¬Ôò·ÖËÑµ½ºÍÃ»ËÑµ½Á½ÖÖÇé¿ö´¦Àí
		else if ((Line[Control.mExeLine].Derection == 0 || Line[Control.mExeLine].Derection == 90
			|| Line[Control.mExeLine].Derection == 180 || Line[Control.mExeLine].Derection == 270)
			&& m_iCurrentSearchingRoute >= 0)	// ×¢ÒâÕâÀïÃ»ÓÐÓÃm_isSearchingMarkÀ´ÅÐ¶Ï£¬
										// ÒòÎªÓëOnTimer()ÓëBardecoder()º¯Êý²»Ì«Í¬²½£¬ºóÕßÊÇÏß³ÌÇý¶¯µÄ
		{
m_ofsLog2<<"ontimerÔÚÖ´ÐÐµØ±êËÑË÷Â·¾¶ÖÐ£¬m_iCurrentSearchingRoute="<<m_iCurrentSearchingRoute<<"\n";
			// Èç¹ûËÑµ½ÁËµØ±ê
			if (m_isSearchingMarkFound && m_iCurrentSearchingRoute >= 0)
			{
m_ofsLog2<<"ontimerÔÚÖ´ÐÐµØ±êËÑË÷Â·¾¶ÖÐËÑµ½µØ±êÁË£¬×¼±¸Ö´ÐÐÍË»ØµØ±ê¶¯×÷\n";
				// »ñµÃ¸Õ¸ÕÖ´ÐÐµÄËÑË÷Â·¾¶µÄ²ÎÊý
				int dir = Line[Control.mExeLine].Derection;
				int dis = int(Line[Control.mExeLine].AidDistance * Puls2Distance + 0.5);
				// ½«µ±Ç°Ö´ÐÐµÄLineÖ¸Ïò×îºóÒ»ÌõSearchLine
				Control.mExeLine += m_iCountOfSearchRoutes - 1 - m_iCurrentSearchingRoute;
				// ÐÞ¸Ä×îºóÒ»ÌõËÑË÷Â·¾¶£¬ÈÃÐ¡³µÍË»Øµ½¸ÃµØ±ê£¬ÈçÔ­À´ÊÇÇ°½ø0£¬Ôò±ä³ÉºóÍË180,Èç90Ôò±ä³É270...
				int dir_reverse = 180+dir >= 360?dir-180:180+dir;
				MotionParemeter mp = GetMotionParameters(dir_reverse, dis-m_markFoundLocation);
				Line[Control.mExeLine] = mp;

m_ofsLog2<<"ontimerÍË»Ø¶¯×÷£ºexeLine="<<Control.mExeLine<<",dir="<<dir_reverse
        <<",dis="<<dis-m_markFoundLocation<<",mMaxLine="<<Control.mMaxLine<<"\n";
for(int l = 0; l < Control.mMaxLine; l++)
	m_ofsLog2<<"line "<<l<<",dir="<<Line[l].Derection<<",dis="<<Line[l].AidDistance<<"\n";
m_ofsLog2<<"--------------------\n";
				
				// ¼ÌÐøÖ´ÐÐ
				Control.mPause = 0;	// 
				Control.mGoOn = 0;	// 
				Control.mLine = 30;

				m_isSearchingMark = false;			// ²»Ö´ÐÐËÑË÷µØ±ê
				m_isSearchingMarkFound = false;		// ÖØÖÃÎªÎ´ËÑË÷µ½µØ±ê
				m_iCurrentSearchingRoute = -1;				// ÖØÖÃÎª-1

			}
			else if (m_iCurrentSearchingRoute >= 0)
			{
m_ofsLog2<<"µÚ"<<m_iCurrentSearchingRoute<<"ÌõÂ·¾¶ËÑË÷ÁË£¬µ«Ã»ÕÒµ½µØ±ê\n";
				// Íê³ÉÁËÒ»ÌõËÑË÷Â·¾¶£¬Ë÷Òý¼Ó1
				m_iCurrentSearchingRoute++; 

				if (m_iCurrentSearchingRoute >= m_iCountOfSearchRoutes) // ËùÓÐËÑË÷Â·¾¶¶¼×ßÍêÁË
				{
m_ofsLog2<<"ËùÓÐËÑË÷Â·¾¶¶¼×ßÍêÁË£¬Ã»ÕÒµ½µØ±ê\n";
					// ´ËÊ±Ó¦¸ÃÌáÊ¾³öÏÖÁËÒì³£
					;
					// Ð¡³µÔÝÍ££¬Ê¹Æä´¦ÓÚÖ´ÐÐ370ÇÒÔÝÍ£µÄ×´Ì¬
					Control.mPause = 0XFF;	// ½øÈëÔÝÍ£
					Control.mGoOn = 0XFF;	// ½ûÖ¹Á¬Ðø
					Control.mExeLine++;		// Ö¸ÏòÂ·¾¶ËÑË÷ºóÃæµÄ370Ö¸Áî£¨Ò»¶¨ÓÐÒ»ÌõÕâÑùµÄÖ¸Áî£©
					Control.mLine = 60;		// Ö±½ÓÌøµ½¼ì²éÊÇ·ñÔÝÍ£µÄ²½Öè

					//Control.mLineEnable = 0;			// ±¾´ÎÐ£×¼¶¯×÷ÒÑÖ´ÐÐÍê
					
					m_isSearchingMark = false;			// ²»Ö´ÐÐËÑË÷µØ±ê
					m_isSearchingMarkFound = false;		// ÖØÖÃÎªÎ´ËÑË÷µ½µØ±ê
					m_iCurrentSearchingRoute = -1;				// ÖØÖÃÎª-1

					m_NotFindMarkTimes370 = 0;			// ÖØÖÃÖ´ÐÐ¾²Ì¬Ð£×¼Ê±Î´ËÑË÷µ½µØ±êµÄ´ÎÊý
					m_CaliCount = 0;					// Ð£×¼´ÎÊýÇåÁã

					// º¯ÊýºóÐø²¿·Ö²»Ö´ÐÐ
					//Display();									
					//CDialog::OnTimer(nIDEvent);
					//return;
				}
			}
		}
	}

	// ÒÔÏÂ´úÂëÓÃÓÚÔÚ¾²Ì¬Ð£×¼Íê³ÉÊ±ÔÝÍ¦£¬µÈ´ýBarDecoder()¼ì²éÎó²î
	if (Control.mLineEnable != 0)	// Èç¹ûLineÊý×éÖÐµÄ¶¯×÷£¨¿ÉÒÔ¶à¸ö£©Ã»ÓÐÍê³É
	{
		if (Control.mStepEnable == 0 && Control.mLine == 50)	// Èç¹ûÄ³¸öLineµÄ¶¯×÷Íê³ÉÁË
		{
			//CString strTemp;
			//strTemp.Format("µ±Ç°mExeLine:%d,Æä·½Ïò£º%d",Control.mExeLine, Line[Control.mExeLine].Derection);
			//MessageBox(strTemp, "", MB_OK);

			// Èç¹ûÊÇ×Ô¶¯¶¨ÏòÖ¸ÁîÍê³ÉÁË£¬ÔòÈÃmRouteGo()ÔÝÍ££¬µÈ´ýBarDecoder()¶ÁÂëº¯Êý¼ì²éÎó²î
			if (Line[Control.mExeLine].Derection == 370)
			{
				Control.mPause = 0XFF;	// ½øÈëÔÝÍ£
				Control.mGoOn = 0XFF;	// ½ûÖ¹Á¬Ðø
			}
		}
	}

	// ÆðÊ¼Ê±¼äµã
	///DWORD SartTime =  GetCurrentTime();	
	// mRouteGo(&m_Device,&Control,Line,Step,CaliMap);	// ×Ô¶¯Ö´ÐÐÂ·¾¶ÐÐ×ß
	mRouteGo(&m_Device,&Control,Line,Step,CaliMap,pPort,&VportPar);	// ×Ô¶¯Ö´ÐÐÂ·¾¶ÐÐ×ß

	// Deknock(); //	Åö×²¼ì²â

	Display();									// ×Ô¶¯Í¨Ñ¶¼°ÏÔÊ¾ÔËÐÐÊý¾Ý
	CDialog::OnTimer(nIDEvent);
}
	
//mecanum Ð¡³µÔË¶¯¿ØÖÆÄ£¿é ---------------------------------------------------

#define RS232cBlock	3 
// ´®¿ÚÍ¨Ñ¶Ä£¿é -------------------------------------------------------------------------------------------------------
BEGIN_EVENTSINK_MAP(CMicroMecanumDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMicroMecanumDlg)
	ON_EVENT(CMicroMecanumDlg, IDC_MSCOMM1, 1 /* OnComm */, OnOnCommMscomm1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
void CMicroMecanumDlg::InitRs232c(void)	//³õÊ¼»¯´®¿Ú 
{
	//´®¿ÚÉèÖÃ²Ù×÷
	if(!ZjCom.Create(NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,IDC_MSCOMM1))  
	{  
		AfxMessageBox("´´½¨MSComm¿Ø¼þÊ§°Ü!"); 
	}  
	else									//´´½¨³É¹¦ÔòÖ´ÐÐ³õÊ¼»¯¼°´ò¿ª´®¿Ú²Ù×÷  
	{  
		//ZjCom.SetCommPort(1);				//Ö¸¶¨´®¿ÚºÅ		
		ZjCom.SetCommPort(8);				//Ö¸¶¨´®¿ÚºÅ 		
		//ZjCom.SetSettings("9600,N,8,1");	//Í¨ÐÅ²ÎÊýÉèÖÃ
		//ZjCom.SetSettings("19200,N,8,1");	//Í¨ÐÅ²ÎÊýÉèÖÃ		
		ZjCom.SetSettings("115200,N,8,1");//Í¨ÐÅ²ÎÊýÉèÖÃ 		
		ZjCom.SetInBufferSize(1024);		//Ö¸¶¨½ÓÊÕ»º³åÇø´óÐ¡  
		ZjCom.SetInBufferCount(0);			//Çå¿Õ½ÓÊÕ»º³åÇø  
		ZjCom.SetInputMode(1);				//ÉèÖÃÊý¾Ý»ñÈ¡·½Ê½  
		ZjCom.SetInputLen(0);				//ÉèÖÃÃ¿´Î¶ÁÈ¡³¤¶È£¬0ÎªÈ«²¿¶ÁÈ¡  
		ZjCom.SetRThreshold(16);			//ÉèÖÃ½ÓÊÕOnCommÊÂ¼þ×Ö·ûÃÅÏÞÖµÔÚÊÕµ½2×Ö½ÚÊ±ºò´¥·¢ÊÂ¼þ  
		ZjCom.SetPortOpen(1);				//´ò¿ª´®¿Ú 
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
	BYTE rxdata[1024]; //ÉèÖÃBYTEÊý×é An 8-bit integerthat is not signed.
	CString str, strtemp, StrDisp;
		
	switch(ZjCom.GetCommEvent())
	{ 
	case 1: // comEvSend·¢ËÍÊý¾Ý
		TRACE("²úÉúÁË´®¿Ú·¢ËÍÖÐ¶Ï");
		break;
		
	case 2: // comEvReceive¶ÁÈ¡Êý¾Ý
		//MessageBox(_T("¶ÁÈ¡Êý¾ÝÊÂ¼þ"), _T("TRACE"), MB_OK);
		variant_inp=ZjCom.GetInput();		//¶Á»º³åÇø
		safearray_inp=variant_inp;			//VARIANTÐÍ±äÁ¿×ª»»ÎªColeSafeArrayÐÍ±äÁ¿
		len=safearray_inp.GetOneDimSize();	//µÃµ½ÓÐÐ§Êý¾Ý³¤¶È
		if (len>=4)
		{
			strtemp="";
			for(k=0; k<len; k++)
			{
				safearray_inp.GetElement(&k,rxdata+k);	//ColeSafeArrayÐÍ±äÁ¿×ª»»ÎªBYTEÐÍÊý×é
				BYTE bt=*(rxdata+k);					//
				strtemp.Format("0X%X ",bt);				//½«×Ö·ûËÍÈëÁÙÊ±±äÁ¿strtemp´æ·Å
				StrDisp+=strtemp;						//
				//StrDisp += bt;						//
			}			
			//MessageBox(_T(StrDisp), _T("TRACE"), MB_OK);
		}
		if(rxdata[0] == 0X55 && rxdata[15] == 0XAA)		//ÃüÁîÖ¡Í·¡¢Ö¡Î²ÕýÈ·£¬¿ªÊ¼½âÎöÃüÁî
		{
			StrDisp+="\n";
			GetDlgItem(IDC_STATIC2)->SetWindowText(StrDisp);// ÏÔÊ¾½ÓÊÕÊý¾Ý
		}
		break;
		
	default: // ´«ÊäÊÂ¼þ³ö´í
		ZjCom.SetOutBufferCount(0);
		break;
	}	
}
// ´®¿ÚÍ¨Ñ¶Ä£¿é -------------------------------------------------------------------------------------------------------

#define CammeraBlock	4 
// Ïà»úÄ£¿é -------------------------------------------------------------------------------------------------------
// Evision 4 ------------------------------------------------------------------
// ÔÚ*Dlg.CPPÖÐ£¬Ìí¼ÓEvisionµÄÌõÂëÊ¶±ðº¯Êý
 //³õÊ¼»¯Ïà»ú²ÎÊý¼°Evision£¬±¾º¯ÊýÒÑ·ÏÆú£¬2015-12-30 qi
 //³õÊ¼»¯Ïà»ú²ÎÊý¼°Evision£¬±¾º¯ÊýÒÑÖØÓÃ£¬2016-03-30 pc
void CMicroMecanumDlg::CameraInitional()  
{
	DisplayImage = TRUE;						// Í¼ÏñÏÔÊ¾¿ª¹Ø£¬Ä¬ÈÏÏÔÊ¾Í¼Ïñ
	ImageProcessFrequence = 0;					// Í¼Ïñ´¦Àí¼ä¸ô±äÁ¿³õÊ¼»¯ 
	FindBarEnable = FALSE;

	ESetJpegQuality(100);								// ÉèÖÃÍ¼ÏñÖÊÁ¿³£Êý
	ESetTraceMode(E_TRACE_SILENT);						//	ESetTraceMode(E_TRACE_DISPLAY_FULL_MESSAGE);
	ESetAngleUnit(E_ANGLE_UNIT_DEGREES);				// Be sure to work in degrees
	
	// m_BarCode²ÎÊýÉèÖÃ
	m_BarCode.SetStandardSymbologies(BRC_EAN_128);		// Select all standard symbologies	
	m_BarCode.SetKnownLocation(FALSE);					// Éè¶¨²»ÖªµÀÌõÂëÎ»ÖÃ¡¢È«¾ÖÑ°ÕÒÌõÂë

ESetJpegQuality(90);	//2016.03.31 pc
	
	sprintf(m_pszDecodedText, "\0");					// String initialization
	///ImageNull.SetSize(640, 480);						// Éè¶¨Ä¬ÈÏÍ¼Ïñ·Ö±æÂÊ

	BarDecodeInfo = "×¼±¸½âÂë";
//	DecodeTotalTime = 0;
	DecodeSuccesTime = 0;
	DecodeFaileTime = 0;
	TotalDispFrame = 0;
//	FinishedBarDecode = TRUE;							// Íê³É½âÂë
	EnableDecoderBar = TRUE;							// ÔÊÐí¶ÁÂë

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
	CameraGetCount(&CamAllNum);							//»ñÈ¡Ïà»ú¸öÊý
	if (CamAllNum>0)
	{
		CString DisStr;
		DisStr.Format("ÏµÍ³ÓÐ%d¸öÏà»ú! \n",CamAllNum);
		TRACE(DisStr);
		
		char *name=new char[255];
		char *model=new char[255];
		NameComBox=(CComboBox*)GetDlgItem(IDC_MULTICAM);	//¹ØÁª½çÃæÏà»úÃû³ÆÁÐ±í¿ò
		if(CamAllNum>0)
		{
			for(int i=0; i<CamAllNum; i++)
			{
				CameraGetName(i, name, model);				//ÒÀ´Î»ñÈ¡Ïà»úÃû³Æ¡¢ÐÍºÅ
				CString t_model;
				t_model.Format("%s %d#",model,i);			//t_model.Format("%s %d#",name,i);
				NameComBox->InsertString(i,_T(t_model));	//Éú³ÉÁÐ±í
			}
			NameComBox->SetCurSel(0);						//Ñ¡Ôñ0#Ïà»ú
		}													//ÔÚÏà»úÁÐ±í¿òÏÔÊ¾Ïà»úÃû		
		
		m_index = NameComBox->GetCurSel();					//È¡µÃÑ¡ÖÐµÄË÷Òý
		CameraInit(m_index);								//³õÊ¼»¯0#Ïà»úinit camera
		pProcessThread.setIndex(m_index);					//Éè¶¨Ïß³Ì¶ÁÈ¡0#Ïà»úÊý¾Ý				
		CameraLoadParameter(m_index,0);						//ÔØÈë0#Ïà»úµÚÁã×é²ÎÊý
		ResoComBox=(CComboBox*)GetDlgItem(IDC_RESOLUTION);	//¹ØÁª½çÃæÏà»ú·Ö±æÂÊÁÐ±í¿ò
		ResoComBox->ResetContent();							//Çå³ýÉÏ´ÎÑ¡ÔñµÄ·Ö±æÂÊ	
		
		int index=0,width=0,height=0, camera_count=0;
		CameraGetResolutionCount(m_index,&camera_count);	//»ñÈ¡Ïà»ú·Ö±æÂÊ×éÊý
		for(int i=0;i<camera_count;i++)
		{
			CameraGetResolution( m_index,i,&width, &height);//»ñÈ¡µÚN#Ïà»úµÚN×é·Ö±æÂÊ
			CString t_reso;
			t_reso.Format("%d*%d",width,height);
			ResoComBox->InsertString(i, _T(t_reso)); 		//Éú³ÉÁÐ±í
		}
		ResoComBox->SetCurSel(1);							//ÉèÖÃÑ¡ÖÐµÄÏî
		DisStr.Format("¹²ÓÐ%d×é·Ö±æÂÊ! \n",camera_count);
		TRACE(DisStr);

		// ²ÉÓÃµÚÒ»×éÄ¬ÈÏÉèÖÃ640¡Á480·Ö±æÂÊ
		int width1=0,height1=0;
		CameraSetResolution(m_index,1,&width1, &height1);
		
		CameraSetOption(m_index,CAMERA_IMAGE_GRAY8);	// ÉèÎªÓÃ8Î»»Ò¶ÈÍ¼

		// Ïà»úÖ¡É¨ÃèÑÓ³ÙÊ±¼äÉè¶¨£¬2000=18HZ£¬640=30HZ
		//CameraSetDelay(m_index,10);
		//CameraSetDelay(m_index,640);		// 31HZ						
		CameraSetDelay(m_index,2000);		// 18HZ	
		delete []name; 
		delete []model; 

		//Ïà»ú·Ö±æÂÊÉè¶¨
		int Group=1,sWidth,sHigh;
		CameraGetResolution(m_index,Group,&sWidth,&sHigh);
		DisStr.Format("%d#Ïà»ú²ÉÓÃ%d×éW%dXH%d!\n",m_index,Group,sWidth,sHigh);
		TRACE(DisStr);

		// Ïà»ú²ÎÊýÉè¶¨

		// 1.Ù¤ÂêÐ£ÕýÇúÏßÉè¶¨!!!
		double gamma;
		CameraGetGamma(m_index,&gamma);
		DisStr.Format("Ù¤ÂêÐ£ÕýÇúÏß%d!\n",(int)gamma*100);
		TRACE(DisStr);
		// 2.Í¼Ïñ¶Ô±È¶ÈÉè¶¨!!!
		double contrast;
		CameraGetContrast(m_index,&contrast);
		DisStr.Format("Í¼Ïñ¶Ô±È¶È%d!\n",(int)contrast*100);
		TRACE(DisStr);
		// 3.É«²Ê±¥ºÍ¶ÈÉè¶¨£¨ºÚ°×Ïà»úÎÞÒâÒå£©

		// 4.×Ô¶¯ÔöÒæÊ¹ÄÜÉè¶¨


		// 5.Í¼ÏñÔöÒæ£¨ÁÁ¶È£©ÖµÉè¶¨
		int gain;
		CameraGetGain(m_index,&gain);
		DisStr.Format("Í¼ÏñÁÁ¶È%d!\n",gain);
		TRACE(DisStr);		
		// 6.×Ô¶¯¿ìÃÅÊ¹ÄÜÉè¶¨
		
		// 7.¿ìÃÅÆØ¹âÊ±¼äÉè¶¨!!!
		int exposure;
		CameraGetExposure(m_index,&exposure);
		DisStr.Format("ÆØ¹âÊ±¼ä%d!\n",exposure);
		TRACE(DisStr);
 		// 8.Í¼ÏñºÚµçÆ½ãÐÖµÉè¶¨
		int blacklevel;
		CameraGetBlackLevel(m_index,&blacklevel);
		DisStr.Format("ºÚµçÆ½ãÐÖµ%d!\n",blacklevel);
		TRACE(DisStr);
		// 9.×Ô¶¯ÆØ¹âÄ¿±êÖµÉè¶¨ 	
		int AETarget;
		CameraGetAETarget(m_index,&AETarget);
		DisStr.Format("×Ô¶¯ÆØ¹âÄ¿±êÖµ%d!\n",AETarget);
		TRACE(DisStr);						
		// 10.XË®Æ½¾µÏñÉè¶¨	
		bool flag_x;
		CameraGetMirrorX(m_index,&flag_x);
		DisStr.Format("XË®Æ½¾µÏñ%d!\n",flag_x);
		TRACE(DisStr);
		// 11.Y´¹Ö±¾µÏñÉè¶¨	
		bool flag_y;
		CameraGetMirrorY(m_index,&flag_y);
		DisStr.Format("Y´¹Ö±¾µÏñ%d!\n",flag_y);
		TRACE(DisStr);
		// 12.Ïà»úÖ¡É¨ÃèÑÓ³ÙÊ±¼äÉè¶¨
		int delay;
		CameraGetDelay(m_index,&delay);
		DisStr.Format("Ö¡É¨ÃèÑÓ³Ù%d!\n",delay);
		TRACE(DisStr);
		// ÉèÖÃÏà»ú×¼±¸¾ÍÐ÷±êÖ¾
		CameraReady = TRUE;
		// ÆôÓÃ¶ÁÏà»úÊý¾ÝÏß³Ìcreate thread
		pProcessThread.CreateThread();
		// ¿ªÊ¼²¥·ÅÍ¼Ïñ		
		pProcessThread.play();
		TRACE("CameraReady=%d,ÓÃÏß³ÌÏÔÊ¾Í¼Ïñ!\n",CameraReady);
		//Ïß³ÌÓëÖ¸¶¨cpu°ó¶¨
		SetThreadAffinityMask(GetCurrentThread(),0X00000008);
	}
	else
	{
		CameraReady = FALSE;								// Ã»ÓÐÏà»ú
		MessageBox("Ã»ÓÐ·¢ÏÖÏà»ú£¬ÎÞ·¨ÆôÓÃÏà»ú¶¨Î»¹¦ÄÜ!",
			"¡¾ÑÏÖØ´íÎó¡¿",
			MB_OK|MB_ICONWARNING|MB_APPLMODAL);
	}
}

void CMicroMecanumDlg::InitializationCamera()   //³õÊ¼»¯Ïà»ú²ÎÊýnew
{
	m_BarCode.SetStandardSymbologies(BRC_EAN_128);		// ¶ÁÈ¡EAN128Âë	
	m_BarCode.SetKnownLocation(FALSE);					// Éè¶¨²»ÖªµÀÌõÂëÎ»ÖÃ¡¢È«¾ÖÑ°ÕÒÌõÂë

	//-----------------------------------------------------------------------------
	DisplayImage = TRUE;						// Í¼ÏñÏÔÊ¾¿ª¹Ø£¬Ä¬ÈÏÏÔÊ¾Í¼Ïñ
	ImageProcessFrequence = 0;					// Í¼Ïñ´¦Àí¼ä¸ô±äÁ¿³õÊ¼»¯ 
	FindBarEnable = FALSE;

	ESetJpegQuality(100);								// ÉèÖÃÍ¼ÏñÖÊÁ¿³£Êý
	ESetTraceMode(E_TRACE_SILENT);						//	ESetTraceMode(E_TRACE_DISPLAY_FULL_MESSAGE);
	ESetAngleUnit(E_ANGLE_UNIT_DEGREES);				// Be sure to work in degrees
	sprintf(m_pszDecodedText, "\0");					// String initialization
	///ImageNull.SetSize(640, 480);						// Éè¶¨Ä¬ÈÏÍ¼Ïñ·Ö±æÂÊ

	BarDecodeInfo = "×¼±¸½âÂë";
//	DecodeTotalTime = 0;
	DecodeSuccesTime = 0;
	DecodeFaileTime = 0;
	TotalDispFrame = 0;
//	FinishedBarDecode = TRUE;							// Íê³É½âÂë
	EnableDecoderBar = TRUE;							// ÔÊÐí¶ÁÂë

	m_saveFlag=false;
	m_saveBMPFlag=true;
	m_saveJPGFlag=false;

	int CamAllNum=0;
	CameraGetCount(&CamAllNum);							//»ñÈ¡Ïà»ú¸öÊý
	if (CamAllNum>0)
	{

		int index = 0; //camera index
		//int group = 2; //output group (320*240)
		int group = 1; //output group (640*480)
		CameraInit(index);								//³õÊ¼»¯0#Ïà»úinit camera
//		pProcessThread.setIndex(m_index);					//Éè¶¨Ïß³Ì¶ÁÈ¡0#Ïà»úÊý¾Ý				
		CameraLoadParameter(index,0);						//ÔØÈë0#Ïà»úµÚÁã×é²ÎÊý
		
		int width1=0,height1=0;
		CameraSetResolution(index,group,&width1, &height1);	//ÉèÖÃÏà»ú·Ö±æÂÊ

		CameraSetOption(index,CAMERA_IMAGE_GRAY8);

		CameraSetDelay(0,0);						//Ïà»úÖ¡É¨ÃèÑÓ³ÙÊ±¼äÉè¶¨
		
		// ÉèÖÃÏà»ú×¼±¸¾ÍÐ÷±êÖ¾
		CameraReady = TRUE;
		// ÆôÓÃ¶ÁÏà»úÊý¾ÝÏß³Ìcreate thread
		pProcessThread.CreateThread();
		// ¿ªÊ¼²¥·ÅÍ¼Ïñ		
		pProcessThread.play();
		TRACE("CameraReady=%d,ÓÃÏß³ÌÏÔÊ¾Í¼Ïñ!\n",CameraReady);
		//Ïß³ÌÓëÖ¸¶¨cpu°ó¶¨
		SetThreadAffinityMask(GetCurrentThread(),0X00000008);
	}
	else
	{
		CameraReady = FALSE;								// Ã»ÓÐÏà»ú
		MessageBox("Ã»ÓÐ·¢ÏÖÏà»ú£¬ÎÞ·¨ÆôÓÃÏà»ú¶¨Î»¹¦ÄÜ!",
			"¡¾ÑÏÖØ´íÎó¡¿",
			MB_OK|MB_ICONWARNING|MB_APPLMODAL);
	}
}

// SocketÍ¨Ñ¶
// ·þÎñ¶ËÃüÁî´¦Àí
LRESULT CMicroMecanumDlg::ProcessCommand(WPARAM wParam, LPARAM lParam)
{
	char* sMsg=(char *)lParam;	
	Command2Route(sMsg);
	
	// ´¢´æ½ÓÊÕµÄÐÐ×ßÖ¸Áî
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

	if (Control.mLineEnable == 0)		// Ö»ÓÐÉÏÒ»ÔË¶¯Íê³É£¬²ÅÄÜ¿ªÊ¼ÐÂµÄÔË¶¯£»ÈôÔË¶¯ÖÐ£¬²»Ö´ÐÐÈÎºÎ¶¯×÷
	{
		Control.mStep = 0;				// ¸´Î»²½ÖèÖ¸Õë
		Control.mExeStep = 0;			// ¸´Î»Î¢²½Ö¸Õë
		Control.mStepEnable = 0;		// ¸´Î»½ûÓÃ×Ô¶¯¿ØÖÆ
		Control.mFixStep = 0;			// ¸´Î»×Ô¶¯Ð£Õý²½Êý
		
		Control.mPause		= 0;		// ¸´Î»ÔÝÍ£Æì±ê
		Control.mGoOn		= 0;		// ¸´Î»¼ÌÐøÂ·¾¶ÔË¶¯Æì±ê
		Control.mRevers		= 0;		// ¸´Î»·´ÏòÆì±ê
		
		Control.mLine		= 0;		// ¸´Î»µ¥¶Î²½ÖèÖ¸Õë
		Control.mExeLine	= 0;		// ¸´Î»µ¥¶ÎÖ´ÐÐÖ¸Õë
		Control.mLineEnable = 0XFF;		// ¿ªÊ¼Ö´ÐÐÎ¢²½³ÌÐò
		FindBarEnable		= TRUE;		// ÔÊÐí×Ô¶¯Ê¶±ðÌõÂë
	}

	delete sMsg;
	return true;
}


// Camera Í¼ÏñÏÔÊ¾Ïß³Ì------------------------------------------------------------------
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

// Ïà»úÄ£¿é -------------------------------------------------------------------------------------------------------

#define EvisionBlock	5 
// ÊÓ¾õÄ£¿é -------------------------------------------------------------------------------------------------------

CString CMicroMecanumDlg::ReadBar()
{
	CString bar("");
	char sTemp[256];
	sprintf(m_pszDecodedText, "\0");
	//µ÷ÓÃEvisionµÄBarcodeÄ£¿é½âÂë
	if (DisplayImage == TRUE)		
	{
		m_Match.Match(&m_Src);
		
		if(!EFailure())
		{
			double x, y, score;
			int nNum = m_Match.GetNumPositions();
			if (nNum > 0)	// ÕÒµ½ÁËÆ¥Åäµã£¨Æ¥Åä¶È´óÓÚ0.3£©
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
				if (strlen(m_pszDecodedText) < BarCodeNumber)	// ÔÄ¶ÁÊ§°Ü
				{
					BarDecodeInfo += ",Fail1";
					
					// ´Ó×ó²à3/4Í¼ÏñÖÐ¶ÁÌõÂë
					m_Roi.Detach();		
					m_Roi.Attach(&m_Src);
					m_Roi.SetPlacement(0,0,480,480);
					//m_Src.SetPlacement(0,0,480,480);
					sprintf(m_pszDecodedText, "\0");
					m_BarCode.Read(&m_Roi, m_pszDecodedText, sizeof(m_pszDecodedText) - 1);
					if (strlen(m_pszDecodedText) < BarCodeNumber)	// ÔÄ¶ÁÊ§°Ü
					{
						BarDecodeInfo += ",Fail2";
						// ´ÓÓÒ²à3/4Í¼ÏñÖÐ¶ÁÌõÂë
						m_Roi.Detach();		
						m_Roi.Attach(&m_Src);
						m_Roi.SetPlacement(160,0,480,480);
						//m_Src.SetPlacement(160,0,480,480);
						sprintf(m_pszDecodedText, "\0");
						m_BarCode.Read(&m_Roi, m_pszDecodedText, sizeof(m_pszDecodedText) - 1);
						if (strlen(m_pszDecodedText) < BarCodeNumber)	// ÔÄ¶ÁÊ§°Ü
						{
							BarDecodeInfo += ",Fail3";
						}
					}
				}
			}	// end if (nNum > 0)	// ÕÒµ½ÁËÆ¥Åäµã£¨Æ¥Åä¶È´óÓÚ0.3£©
			else
			{
				BarDecodeInfo = "!Score<0.25";
			}
		}	// end if(!EFailure())
		
		//Èô½âÂëºóÎ»Êý´óÓÚµÈÓÚ5
		if (strlen(m_pszDecodedText) >= BarCodeNumber)					
		{
			m_pszDecodedText[BarCodeNumber] = '\0';
			bar = m_pszDecodedText;
			// Èç¹ûÌõÂëÃ¿Ò»Î»¶¼ÊÇ0-9Êý×Ö£¬ÇÒÔÚµØ±ê¿âÖÐ´æÔÚ£¬Ôò½âÂëÐÅÏ¢ÕýÈ·
			if (!CheckBarcode(bar))
				bar = "";
		}
	}
	
	return bar;
}


// º¯Êý¹¦ÄÜ°üÀ¨µ÷ÓÃEvision¿âº¯Êý£¬´ò¿ªÒ»¸öC24Í¼Ïñ×ªÎªBW8¸ñÊ½¡¢½âÂë£¬²¢ÔÚÎÄ±¾¿òÏÔÊ¾
void CMicroMecanumDlg::BarDecoder()						// µ÷ÓÃEvision½âÂë
{	
	BarDecodeInfo = "";

	// Èç¹û½âÂëÐÅÏ¢ÕýÈ·
	if (isDecodeSuccess)
	{
		// -----------¼ÇÂ¼¶Áµ½µÄµØ±ê--pc
		char tmp[64];
		time_t t = time( 0 ); 			
		strftime( tmp, sizeof(tmp), "%X",localtime(&t) );

		ofstream ofsLog7;
		Json::Value msgBody;
						
		ofsLog7.open("log-7¼ÇÂ¼¶Áµ½µÄµØ±ê.txt",ios::app);
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
		// -----------¼ÇÂ¼¶Áµ½µÄµØ±ê½áÊø

		// Í³¼ÆÕýÈ·½âÂë´ÎÊý		
		DecodeSuccesTime++;
		// ¼ÆËãÆÁÄ»ÏñËØµ½Êµ¼Ê¾àÀë»»ËãÏµÊý£¬µ¥Î»£ººÁÃ×/ÏñËØ
		double CaliK = BarLenth/640;
		// ¶ÁÈ¡ÌõÂëX×ø±êÖá³¤¶È
		BarXlenth = 640 * CaliK;
		// ¶ÁÈ¡ÌõÂëY×ø±êÖá³¤¶È
		BarYlenth = 480 * CaliK;

		m_NotFindMarkTimes370 = 0;
		// Èç¹ûÕýÔÚÖ´ÐÐµØ±êËÑË÷£¬ÔòËµÃ÷ÕÒµ½ÁË
		if (m_isSearchingMark)
		{
			m_isSearchingMark = false;		// ²»ÔÙËÑË÷µØ±ê
			m_isSearchingMarkFound = TRUE;	// ±íÃ÷ÒÑ¾­ËÑµ½µØ±êÁË£¬½ÓÏÂÀ´ÔÚOnTimerÀï´¦Àí

			INT32 dis = INT32(DOUBLE(GetShiftR(&m_Device,XADDR)) * Puls2Distance);
			// ¼ÇÂ¼ÏÂ³µ×Ó×ßµÄ¾àÀë£¬ÒÔ±ã³µ×Ó×ßÍêÕâÒ»¶ÎËÑË÷Â·¾¶ºóÍË»ØÈ¥¶Ô×¼µØ±ê
			m_markFoundLocation = fabs(dis);	// È¡¾ø¶ÔÖµ£¬ÒòÎªºóÍËÊ±disÊÇ¸ºÖµ	
m_ofsLog2<<"µØ±êÒÑËÑµ½,m_markFoundLocation="<<m_markFoundLocation<<",m_iCurrentSearchingRoute="<<m_iCurrentSearchingRoute<<"\n";
		}

		// Èç¹û¶ÁÂëÕýÈ·£¬²¢ÔÚ½øÐÐ¸´Î»£¨Ã¿´ÎÐÐ×ßÖ®Ç°¶¼»á½øÐÐ¸´Î»£©
		if (FindBarEnable == TRUE && isDecodeSuccess == TRUE && Control.mMaxLine >= 1)
		{			
ofstream ofsLog1;				
ofsLog1.open("log-1µØ±êÐ£×¼¹ý³Ì.txt",ios::app);
ofsLog1<<"-00 ¶Áµ½µØ±ê£¬Ö¸Áî="<<Line[Control.mExeLine].Derection
	<<",mExeLine="<<Control.mExeLine<<",mMaxLine="<<Control.mMaxLine<<",mLine"<<Control.mLine
	<<",mPause="<<Control.mPause<<"\n";			
			if (Line[Control.mExeLine].Derection == 370 )
			{
				// ¼ÇÂ¼µÚÒ»¸ö¹ì¼£Ð£×¼µã£¬È·±£ÔÚÖ´ÐÐÒ»ÌõLineµÄÆðÊ¼µãÊ±±ØÐë½«nCheckPointsÖÃÎª1
				// ÇÒÎªCheckMarks[0]ºÍCheckPoints[0]¸³Öµ
				nCheckPoints = 1;
				MARK* pMark = GetMarkInfo(BarCodeStr);
				CheckMarks[0].barCode = pMark->barCode;
				CheckMarks[0].dir = pMark->dir;
				CheckMarks[0].x = pMark->x;
				CheckMarks[0].y = pMark->y;

				CheckPoints[0].distance = 0;
				CheckPoints[0].deltax = 0;
				CheckPoints[0].deltay = 0;
				CheckPoints[0].deltaa = -BarAngle;	// Æ«ÒÆ½Ç£¬ÉãÏñ»ú»ñµÃµÄÖµÒª·´¹ýÀ´ÓÃ
				CheckPoints[0].deltah = -BarAngle;	//0;
				CheckPoints[0].speed = 0;
				CheckPoints[0].adjustx = 0;
				CheckPoints[0].adjusty = 0;
//ofstream ofsLog1;				
//ofsLog1.open("log-1µØ±êÐ£×¼¹ý³Ì.txt",ios::app);
ofsLog1<<"-0 ¶Áµ½µØ±ê£¬¿ªÊ¼¾²Ì¬Ð£×¼ mLineEnable="<<Control.mLineEnable<<",mLine="<<Control.mLine<<",mPause="<<Control.mPause<<"\n";

				// Èç¹û¸Ã370Ö¸Áî¸Õ¿ªÊ¼Ö´ÐÐ¶øÇÒ´¦ÓÚÔÝÍ£×´Ì¬£¬Ôò¼ì²éÊÇ·ñÎ»ÓÚÖÐÐÄµØ±ê
				if (Control.mLineEnable !=0 /*== 0XFF*/	&& Control.mLine <= 20 
					&& Control.mPause == 0XFF)	
				{ 
					// ²éÕÒÕâ¸öµØ±êËùÔÚµÄÖÐÐÄµØ±ê
					CString sBarCentral = FindCentralMark(BarCodeStr);
ofsLog1<<"---01 ¶Áµ½µÄµØ±êbar="<<BarCodeStr.GetBuffer(0)<<",ÖÐÐÄµØ±êbarCenter="<<sBarCentral.GetBuffer(0)<<"---------\n"; 					
					if (sBarCentral == BarCodeStr) // ±¾Éí¾ÍÊÇÖÐÐÄµØ±ê£¬Ôò¼ÌÐø
					{
						// mRouteGo()ÖÐÖ±½ÓÌøµ½30£¬¼ÌÐøÖ´ÐÐ
						Control.mLine = 30;
						Control.mPause =0;			// ¼ÌÐøÖ´ÐÐ
						Control.mGoOn =0;			// ¼ÌÐøÖ´ÐÐ		
					}
					else	// ÔÚLineÖÐ²åÈëÒ»¶ÎÆ«ÒÆ
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

								
							// µÝÔö¼ÆÊýÆ÷
							Control.mMaxLine++;
ofsLog1<<"-02 ÔÚLineÖÐÔö¼ÓÒ»¶ÎÆ«ÒÆ£¬²ÎÊýÎªaiddistance="<<MP.AidDistance<<",maxLine="<<Control.mMaxLine<<",exeLine="<<Control.mExeLine<<"\n"; 					
							// ¼ÌÐøÖ´ÐÐ£¬´ËÊ±Ó¦¸ÃÖ´ÐÐÐÂÔö¼ÓµÄÕâ¸öLine
							Control.mPause =0;			// ¼ÌÐøÖ´ÐÐ
							Control.mGoOn =0;			// ¼ÌÐøÖ´ÐÐ		

						}
					}
				}

				// Èç¹û¸Ã370Ö¸ÁîÒÑ¾­Ö´ÐÐÍêÁË¶øÇÒ´¦ÓÚÔÝÍ£×´Ì¬£¬Ôò¼ì²éÆ«ÒÆÁ¿
				if (Control.mStepEnable == 0 && Control.mLine == 60 && Control.mPause == 0XFF)
				{
ofsLog1<<"-1 µ¥´Î370Ö¸ÁîÒÑ¾­Ö´ÐÐÍêÁË£¬¼ì²éÆ«ÒÆÁ¿Îª£ºBarX="<<BarXlocation<<",BarY="<<BarYlocation<<",angle="<<BarAngle<<"\n";
					bool bAcurate = false;	// ¾²Ì¬Ð£×¼ÊÇ·ñ¾«È·
					// ×ø±êÎó²îÐ¡ÓÚ1mm£¬»òÕß½Ç¶ÈÐ¡ÓÚ1.0¶È
					if (fabs(BarXlocation) < X_Y_TOLERANCE_HIGH && fabs(BarYlocation)<X_Y_TOLERANCE_HIGH
						&& fabs(BarAngle) < ANGLE_TOLERANCE_HIGH)
					{
						m_CaliCount ++;	// Ð£×¼´ÎÊý¼Ó1
ofsLog1<<"-2 »ù±¾£¨Ò²¿ÉÄÜÍêÈ«£©¾«È·£¬¾²Ì¬Ð£×¼´ÎÊý¼Ó1£¬µÈÓÚ"<<m_CaliCount<<"\n";
					}
					// ×ø±êÎó²îÐ¡ÓÚ0.5mm£¬»òÕß½Ç¶ÈÐ¡ÓÚ0.5¶È
					if (fabs(BarXlocation) < X_Y_TOLERANCE_LOW && fabs(BarYlocation)<X_Y_TOLERANCE_LOW
																&& fabs(BarAngle) < ANGLE_TOLERANCE_LOW)
					{
						m_CaliCount = 0;
						bAcurate = true;
					}
					if (bAcurate || m_CaliCount == MAX_ReCheckMarkTimes370)	// Èç¹ûÒÑ¾­Ð£×¼¾«È·ÁË£¬»òÕß»ù±¾Ð£×¼³¬¹ýÁËÁ½´Î£¬Ö±½ÓÌøµ½ÏÂÒ»¸öÖ±ÐÐ»òÕß¹ÕÍä»òÕßºóÍËÖ¸Áî
					{
						// ¸üÐÂÊÇ·ñÐ£×¼µÄ×´Ì¬£¬¼ÇÂ¼Ð£×¼µÄµØ±êID
						m_igvInfo.isMarked = true;
						m_igvInfo.MarkID = BarCodeStr;
ofstream ofsLog;				
ofsLog.open("log-4 isFree×´Ì¬µ÷ÊÔ.txt",ios::app);
ofsLog<<"Ð£×¼ÁË£¬MarkID="<<(char*)(_bstr_t)m_igvInfo.MarkID.GetBuffer(0)<<"\n";
ofsLog.close();

//ofsLog1<<"Ð£×¼ÁË£¬MarkID="<<(char*)(_bstr_t)m_igvInfo.MarkID.GetBuffer(0)<<"\n";

						// ¸üÐÂ×ø±ê
						m_igvInfo.curX = pMark->x;
						m_igvInfo.curY = pMark->y;
						m_igvInfo.preX = m_igvInfo.curX;
						m_igvInfo.preY = m_igvInfo.curY;
ofsLog1<<"¾²Ì¬Ð£×¼³É¹¦£¬´Ë´¦¸üÐÂ×ø±ê1£¬curx="<<m_igvInfo.curX<<",cury="<<m_igvInfo.curY<<",prex="<<m_igvInfo.preX<<",prey="<<m_igvInfo.preY<<"\n";
ofsLog1<<"-------------------------------------------------------------\n\n\n";						
						m_CaliCount = 0;	// Ð£×¼´ÎÊýÇåÁã
						if (Control.mRevers == 0)	// ÕýÏòÖ´ÐÐLineÊý×éÖÐÖ¸ÁîÊ±
						{
							int exeLine = Control.mExeLine;
							while (exeLine < Control.mMaxLine)
							{
								exeLine++;	// LineÖ¸ÕëÖ¸ÏòÏÂÒ»¸öLine
								if (exeLine == Control.mMaxLine)	// ºóÃæÃ»ÓÐÆäËüLineÒªÖ´ÐÐÁË
								{
									Control.mExeLine = 0;
									Control.mLine = 80;			// ´Ë¾äÖ´ÐÐºóºóÃæÁ½ÐÐ»áÔÚmRouteGo()ÖÐÖ´ÐÐ£¬Òò´Ë×¢ÊÍµô
									//Control.mGoOn =0;			// ¼ÌÐøÖ´ÐÐ
									//Control.mGoOn =0;			// ¼ÌÐøÖ´ÐÐ

									exeLine = Control.mMaxLine;	// Ïàµ±ÓÚ½áÊøwhileÑ­»·
								}
								else	// Èç¹ûºóÃæ»¹ÓÐÖ±ÐÐ»òÕßÐý×ª£¬Ôò¼ÌÐøÖ´ÐÐ
								{
									int dir = Line[exeLine].Derection;
									if( dir == 0 || dir == 90 || dir == 180 || dir == 270
										|| dir == 361 || dir == 362)
									{
										Control.mExeLine = exeLine;
										Control.mLine = 20;			// 
										Control.mPause =0;			// ¼ÌÐøÖ´ÐÐ
										Control.mGoOn =0;			// ¼ÌÐøÖ´ÐÐ
										exeLine = Control.mMaxLine;	// Ïàµ±ÓÚ½áÊøwhileÑ­»·
									}
								}
							}
						}
					}
					else	// ×Ô¶¯Ð£×¼¾«¶È´ï²»µ½ÒªÇó£¬»¹ÓÐÎó²î£¬Ðè¼ÌÐøÖ´ÐÐ´ËÖ¸Áî
					{
ofsLog1<<"-8 ×Ô¶¯Ð£×¼¾«¶È´ï²»µ½ÒªÇó£¬»¹ÓÐÎó²î£¬Ðè¼ÌÐøÖ´ÐÐ´Ë370Ö¸Áî\n";
						
						Control.mLine = 20;			// mRouteGo()ÖÐÖ±½ÓÌøµ½20£¬²»Ö´ÐÐControl.mExeLine++
						Control.mPause =0;			// ¼ÌÐøÖ´ÐÐ
						Control.mGoOn =0;			// ¼ÌÐøÖ´ÐÐ
					}
				}

ofsLog1.close();

			}		

			// Èç¹û¶ÁÂëÕýÈ·£¬ÇÒÔÚÖ´ÐÐÒ»ÌõLine£¬Ìí¼ÓÒ»¸öÐ£×¼µã
			else if (Line[Control.mExeLine].Derection == 0 || Line[Control.mExeLine].Derection == 180 
				&& nCheckPoints >= 1 /*È·±£µÚÒ»¸öµãÒÑ¾­¶Áµ½*/)
			{
				// Ôö¼ÓÒ»¸ö¹ì¼£Ð£×¼µã
				
				// ¼ÆËãÓëÉÏÒ»¸ö¶Áµ½µÄµØ±êÖ®¼äµÄ¾àÀë
				MARK* pMark = GetMarkInfo(BarCodeStr);
				double disX = pMark->x - CheckMarks[nCheckPoints-1].x;
				double disY = pMark->y - CheckMarks[nCheckPoints-1].y;
				double dis = fabs(disX) + fabs(disY);	// Á½¸öµØ±ê×ø±ê²î

				
				double markDeviation = 0;	// ¶Áµ½µÄµØ±êÔÚ´¹Ö±ÓÚÔË¶¯·½ÏòµÄÆ«ÒÆÁ¿
				double disX0 = pMark->x - CheckMarks[0].x;
				double disY0 = pMark->y - CheckMarks[0].y;

				// ³µÍ·³¯YÖáÕýÏò£¬Ç°½ø
				if (m_igvInfo.Heading ==0 && Line[Control.mExeLine].Derection == 0)
				{
					markDeviation = disX0;
					//BarAngle = BarAngle;	// Ç°½øÎªÕý
				}
				// ³µÍ·³¯YÖáÕýÏò£¬ºóÍË
				else if (m_igvInfo.Heading ==0 && Line[Control.mExeLine].Derection == 180)
				{
					markDeviation = -disX0;
					//BarAngle = -BarAngle;	// ºóÍËÎª¸º
				}
				// ³µÍ·³¯YÖá¸ºÏò£¬Ç°½ø
				else if (m_igvInfo.Heading ==180 && Line[Control.mExeLine].Derection == 0)
				{
					markDeviation = -disX0;
					//BarAngle = BarAngle;	// Ç°½øÎªÕý
				}
				// ³µÍ·³¯YÖá¸ºÏò£¬ºóÍË
				else if (m_igvInfo.Heading ==180 && Line[Control.mExeLine].Derection == 180)
				{
					markDeviation = disX0;
					//BarAngle = -BarAngle;	// ºóÍËÎª¸º
				}				
				// ÓëÇ°ÃæÀàËÆ
				else if (m_igvInfo.Heading ==90 && Line[Control.mExeLine].Derection == 0)
				{
					markDeviation = -disY0;
					//BarAngle = BarAngle;	// Ç°½øÎªÕý
				}
				else if (m_igvInfo.Heading ==90 && Line[Control.mExeLine].Derection == 180)
				{
					markDeviation = disY0;
					//BarAngle = -BarAngle;	// ºóÍËÎª¸º
				}
				else if (m_igvInfo.Heading ==270 && Line[Control.mExeLine].Derection == 0)
				{
					markDeviation = disY0;
					//BarAngle = BarAngle;	// Ç°½øÎªÕý
				}
				else if (m_igvInfo.Heading ==270 && Line[Control.mExeLine].Derection == 180)
				{
					markDeviation = -disY0;
					//BarAngle = -BarAngle;	// ºóÍËÎª¸º
				}

				//Èç¹û¾àÀëÇ°Ò»¸ö±ê¾àÀëÐ¡ÓÚ400mm£¬ÔòºöÂÔ
				if (dis > 400)
				{
					CheckMarks[nCheckPoints].barCode = pMark->barCode;
					CheckMarks[nCheckPoints].dir = pMark->dir;
					CheckMarks[nCheckPoints].x = pMark->x;
					CheckMarks[nCheckPoints].y = pMark->y;
					
					CheckPoints[nCheckPoints].distance = CheckPoints[nCheckPoints-1].distance + dis;
					if (Line[Control.mExeLine].Derection == 0)	// Ç°½ø
						CheckPoints[nCheckPoints].deltax =	-BarXlocation 	// ÉãÏñ»ú»ñµÃµÄÖµÒª·´¹ýÀ´ÓÃ
															+ markDeviation;// ¼ÓÉÏÕâ¸ö±ê±¾ÉíÓëLineµÄÆ«ÒÆÁ¿
					else	// ºóÍË
						CheckPoints[nCheckPoints].deltax =	BarXlocation	// ÉãÏñ»ú»ñµÃµÄÖµÒª·´¹ýÀ´ÓÃ£¬¸º¸ºµÃÕý
															+ markDeviation;// ¼ÓÉÏÕâ¸ö±ê±¾ÉíÓëLineµÄÆ«ÒÆÁ¿

					CheckPoints[nCheckPoints].deltay = -BarYlocation;	// ÉãÏñ»ú»ñµÃµÄÖµÒª·´¹ýÀ´ÓÃ
					CheckPoints[nCheckPoints].deltaa = -BarAngle;	// ÉãÏñ»ú»ñµÃµÄÖµÒª·´¹ýÀ´ÓÃ
					CheckPoints[nCheckPoints].speed = m_igvInfo.Speed;	// ÉãÏñ»ú»ñµÃµÄÖµÒª·´¹ýÀ´ÓÃ

					// adjustx, adjusty, deltaHÁôµ½FixTrack()º¯ÊýÈ¥¸üÐÂ

					nCheckPoints++;
				}
				else
					isDecodeSuccess = false;//ºóÃæ²»Ö´ÐÐÐ£×¼¶¯×÷
			}
		}

		//--------------------------------------------------------------
	
		// Èç¹û¶ÁÂëÕýÈ·£¬¶¯Ì¬Ð£Õý¹ì¼£
		if ((FindBarEnable == TRUE) && isDecodeSuccess/*&& (BarCodeStr == "12345")*/
			&& (Control.mLineEnable != 0) && (Control.mExeStep > 2))
		{

// Êä³öControl.mFixStepºÍControl.mExeStep£»
ofsLog7<<Control.mFixStep<<" "<<Control.mExeStep<<"\n";
ofsLog7.close();

			if (Control.mFixStep != Control.mExeStep)
			{
				TRACE("FindBarEnable=%d,BarCodeStr=%d,mLineEnable=%d,mExeStep=%d,Shift=%d,\n",
					FindBarEnable,atoi(BarCodeStr),Control.mLineEnable,Control.mExeStep,
					INT32(DOUBLE(GetShiftR(&m_Device,XADDR)) * Puls2Distance));
				//StopMotion();	// Èç¹û¶ÁÂëÕýÈ·£¬Á¢¼´Í£³µ
				// ÒÀ¾ÝÌõÂëÐÅÏ¢£¬Ð£×¼ÐÐ³µ¹ì¼£
				DOUBLE dR = 215;
				FixTrack(dR,BarXlocation,BarYlocation,BarAngle,&Control,Step);
				// ¼ÇÒäÐ£×¼Î¢²½Î»ÖÃ
				Control.mFixStep = Control.mExeStep;
			}
		}		
		//¾«¼ò¸ñÊ½Êä³öÌõÂëÐÅÏ¢
		// ¶ÁÈ¡ÌõÂëÖµ
		str = BarCodeStr;									
		// Êä³öÌõÂëÖÐÐÄXÖá×ø±ê
		str1.Format(_T(",%4.1fmm,"), BarXlocation);			// ¸ñÊ½»¯Êä³ö×Ö·û
		str += str1;										
		// Êä³öÌõÂëÖÐÐÄYÖá×ø±ê
		str1.Format(_T("%4.1fmm,"), BarYlocation);			// ¸ñÊ½»¯Êä³ö×Ö·û
		str += str1;										
		// Êä³öÌõÂëÆ«×ª½Ç¶È
		str1.Format(_T("%4.2fdeg,"), BarAngle);				// ¸ñÊ½»¯Êä³ö×Ö·û
		str += str1;										
		// Êä³öÖ¡ÂÊ
		str1.Format(_T("%dFPS,"), FramePerSecond);			// ¸ñÊ½»¯Êä³ö×Ö·û
		str += str1;										
		// °ÑÐèÒªÏÔÊ¾µÄÐÅÏ¢´«µÝµ½ÏÔÊ¾±äÁ¿
		BarDecodeInfo = BarDecodeInfo + ", " + str;							
	} 
	else	// ½âÂë²»ÕýÈ·
	{
		// Èç¹ûÖ´ÐÐ370Ö¸ÁîÎÞ·¨¶Áµ½±ê£¬Ôò¼ÆÊýÆ÷¼Ó1
		// ÉèmLine<60ÊÇÎªÁË·ÀÖ¹ÕâÖÖÇé¿ö£ºËÑË÷Â·¾¶Ö´ÐÐÍêÁËµ«Ã»ÕÒµ½µØ±ê£¨mLine=60,mPause=0XFF£©£¬ÕâÊ±²»ÐèÒªÆô¶¯ËÑË÷Â·¾¶
		if (Line[Control.mExeLine].Derection == 370 && Control.mLine < 60)	
		{
			m_NotFindMarkTimes370++;
CString s;
s.Format("Î´·¢ÏÖµØ±ê´ÎÊý£º%d",m_NotFindMarkTimes370);
GetDlgItem(IDC_STATIC1)->SetWindowText(s);
			// Èç¹û½âÂëÊ§°Ü³¬¹ýÁËÒ»¶¨µÄ´ÎÊý£¬ÔòÆô¶¯¾Ö²¿µØ±êËÑË÷		
			if (m_NotFindMarkTimes370 >= MAX_NotFindMarkTimes370)
			{
				// ¼ÆËãËÑË÷Â·¾¶£¨¹²Num_SearchLinesÌõ£©
				CalculateSearchLines();
				// ½«ËÑË÷Â·¾¶²åÈëµ½ÏÖÓÐÂ·¾¶ÖÐ
				for(int k = (int)Control.mMaxLine-1; k >=(int)Control.mExeLine; k--)
				{
					Line[k+m_iCountOfSearchRoutes] = Line[k];
				}
				for (int i = 0; i<m_iCountOfSearchRoutes;i++)
					Line[Control.mExeLine+i] = SearchLines[i];
					
				// µÝÔö¼ÆÊýÆ÷
				Control.mMaxLine+=m_iCountOfSearchRoutes;
				// ¼ÌÐøÖ´ÐÐ£¬´ËÊ±Ó¦¸ÃÖ´ÐÐÐÂÔö¼ÓµÄÕâ¸öLine
				Control.mPause =0;			// ¼ÌÐøÖ´ÐÐ
				Control.mGoOn =0;			// ¼ÌÐøÖ´ÐÐ
				Control.mLine = 30;

				m_iCurrentSearchingRoute = 0;		// ÉèÖÃµ±Ç°ÕýÔÚÖ´ÐÐµÄµØ±êËÑË÷Â·¾¶Ë÷Òý, -1±íÊ¾Î´½øÐÐµØ±êËÑË÷
				m_isSearchingMark = true;	// Æô¶¯ËÑË÷
				m_isSearchingMarkFound = false;	// ÉÐÎ´ËÑË÷µ½
				m_NotFindMarkTimes370 = 0;

m_ofsLog2<<"Æô¶¯µØ±êËÑË÷\n";
			}
		}
		
		//Í³¼Æ²»ÕýÈ·½âÂë´ÎÊý
		DecodeFaileTime++;
		str = "";											// Çå¿Õ×Ö·û´®
		// Êä³öÖ¡ÂÊ
		str1.Format(_T("%dFPS,"), FramePerSecond);			// ¸ñÊ½»¯Êä³ö×Ö·û
		str += str1;										
		// ½âÂëÊ§°Ü°Ù·Ö±È
		double FailPercent = 100 * float(DecodeFaileTime)/float(DecodeSuccesTime + DecodeFaileTime);
		str1.Format(_T("%2.2fFPC,"), FailPercent);			// ¸ñÊ½»¯Êä³ö×Ö·û
		str += str1;
		// °ÑÐèÒªÏÔÊ¾µÄÐÅÏ¢´«µÝµ½ÏÔÊ¾±äÁ¿
		//BarDecodeInfo = " ¡¾ ½âÂëÊ§°Ü !!! ¡¿," + str + DisplayControl(&Control);
		// °ÑÐèÒªÏÔÊ¾µÄÐÅÏ¢´«µÝµ½ÏÔÊ¾±äÁ¿
		BarDecodeInfo = " ¡¾ ½âÂëÊ§°Ü !!! ¡¿," + BarDecodeInfo + ", " + str;
		//½âÂëÐÅÏ¢²»ÕýÈ·
		isDecodeSuccess = FALSE;	
		//¸´Î»½âÂëÐÅÏ¢
		BarXlocation = 0;		// ÌõÂëÖÐÐÄXÖá×ø±ê
		BarYlocation = 0;		// ÌõÂëÖÐÐÄYÖá×ø±ê
		BarXlenth = 0;			// ÌõÂëX×ø±êÖá³¤¶È
		BarYlenth = 0;			// ÌõÂëY×ø±êÖá³¤¶È
		BarAngle = 0;			// ÌõÂëÆ«×ª½Ç¶È			
	}

	// ÎªÐ£×¼¸³Öµ
	Control.RoundUnit = TurnRound;
	Control.DeltH = BarAngle;
	Control.DeltX = BarXlocation;
	Control.DeltY = BarYlocation;
	FinishedBarDecode = TRUE;								// Íê³É½âÂë

	//ÔÚÍ¼ÏñÉÏÍ¬²½ÏÔÊ¾×Ö·ûµÈ
	DrawArrow(mARROW);

}
// ÊÓ¾õÄ£¿é -------------------------------------------------------------------------------------------------------

#define DrawBlock	6 
// ÔË¶¯±êÖ¾»æ»­Ä£¿é ---------------------------------------------------------------------------------------------------
BOOL CMicroMecanumDlg::DrawArrow(UINT32 CommandDir)	// »æ»­ÔË¶¯·½Ïò¼ýÍ·
{
	CDC *pDC=NULL;								//¶¨ÒåÒ»¸ö¿ÕµÄDevice Concent
	CRect rect;									//¶¨Òå¾ØÐÎ
	CWnd *pWnd2 = GetDlgItem(IDC_STATICP1);		//»ñÈ¡»æ»­´°¿Ú		
	if(pWnd2)									//Èô´°¿Ú´æÔÚ	
	{
		pWnd2->GetClientRect(&rect);			//Éè¶¨»æ»­¾ØÐÎ
		pDC = pWnd2->GetDC();					//Éè¶¨»æÍ¼Ä¿±êÉè±¸
		//¼ÆËã»æÍ¼²ÎÊý	
		INT32 w,h,x0,y0;
		x0 = rect.Width()/2;					// Í¼±êX×ø±ê
		y0 = rect.Height()/8;					// Í¼±êY×ø±ê
		w = rect.Width()/16;					// Í¼±ê¿í¶È
		h = rect.Height()/16;					// Í¼±ê¸ß¶È
		if (w >= h)								// ÁîÍ¼±ê¿í¶È = ¸ß¶È 
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

		if (CameraReady != TRUE)				// Èô²»ÏÔÊ¾Í¼Ïñ£¬ÔòÉèÖÃ±³¾°É«
		{
			pDC->FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(236,233,216));	//ÏµÍ³Ä¬ÈÏ±³¾°É«
		}
		// »­Ê®×ÖÏß
		CPen pen2; 
		pen2.CreatePen(PS_SOLID,1,RGB(0,0,127));			// ´´½¨À¶É«»­±Ê
		CPen* pOldPen2= pDC->SelectObject(&pen2);			// Ñ¡Ôñ»­±Ê
		// ÊúÏß
		pDC->MoveTo(rect.Width()/2,0);						// ÒÆ¶¯µ½Æðµã
		pDC->LineTo(rect.Width()/2,rect.Height());			// ´ÓÆðµã»­Ïßµ½ÖÕµã
		// ºáÏß		
		pDC->MoveTo(0,rect.Height()/2);						// ÒÆ¶¯µ½Æðµã
		pDC->LineTo(rect.Width(),rect.Height()/2);			// ´ÓÆðµã»­Ïßµ½ÖÕµã			
		pDC->SelectObject(&pOldPen2);						// ÊÍ·Å»­±Ê

		// ¶¨ÒåÒ»Ö§»­±Ê
		CPen pen; 											// ¶¨Òå»­±Ê
		pen.CreatePen(PS_SOLID,1,ARROWRED);					// ´´½¨»­±Ê
		CPen* pOldPen= pDC->SelectObject(&pen);				// °ó¶¨»­±Ê		
		// ¶¨ÒåÒ»¸ö»­Ë¢
		CBrush brush;  										// ¶¨Òå»­Ë¢
		brush.CreateSolidBrush(ARROWRED);					// ´´½¨»­Ë¢
		CBrush* pOldBrush = pDC->SelectObject(&brush);		// °ó¶¨»­Ë¢		
		if (CommandDir <= 360)								// »­Ë¢»­ÊµÐÄËÄ±ßÐÎ
		{
			pDC->FillSolidRect(x0-w/2,y0,w,h,ARROWRED);		// »­Ë¢»­ÊµÐÄËÄ±ßÐÎ// (x,y,w,h,RGB(200,0,0))
		}
		// ½âÎö·½Ïò
		switch(CommandDir)
		{
		case 0:
			pt[0].x = x0-w;
			pt[0].y = y0;
			pt[1].x = x0+w;
			pt[1].y = y0;
			pt[2].x = x0;
			pt[2].y = y0-h;
			pDC->Polygon(pt,3);								// »­Ë¢»­ÊµÐÄÈý±ßÐÎ	
			break;
		case 90:
			pt[0].x = x0+w/2;
			pt[0].y = y0-h/2;
			pt[1].x = x0+w/2;
			pt[1].y = y0+3*h/2;
			pt[2].x = x0+3*w/2;
			pt[2].y = y0+h/2;
			pDC->Polygon(pt,3);								// »­Ë¢»­ÊµÐÄÈý±ßÐÎ	
			break;
		case 180:
			pt[0].x = x0+w;
			pt[0].y = y0+h;
			pt[1].x = x0-w;
			pt[1].y = y0+h;
			pt[2].x = x0;
			pt[2].y = y0+2*h;
			pDC->Polygon(pt,3);								// »­Ë¢»­ÊµÐÄÈý±ßÐÎ	
			break;
		case 270:
			pt[0].x = x0-w/2;
			pt[0].y = y0+3*h/2;
			pt[1].x = x0-w/2;
			pt[1].y = y0-h/2;
			pt[2].x = x0-3*h/2;
			pt[2].y = y0+h/2;
			pDC->Polygon(pt,3);								// »­Ë¢»­ÊµÐÄÈý±ßÐÎ	
			break;
		case 360:
			pDC->FillSolidRect(x0-w,y0-h,2*w,2*h,ARROWBLU);	// »­Ë¢»­ÊµÐÄËÄ±ßÐÎ
			break;
		case 361:
			pt[0].x = x0;
			pt[0].y = y0;
			pt[1].x = x0;
			pt[1].y = y0-2*h;
			pt[2].x = x0-w;
			pt[2].y = y0-h;
			pDC->Polygon(pt,3);								// »­Ë¢»­ÊµÐÄÈý±ßÐÎ
// 			StartP.x = x0-w;
// 			StartP.y = y0;			
// 			EndP.x = x0;
// 			EndP.y = y0-h;
// 			pDC->Arc(Round,StartP,EndP);					// »­Ô²»¡			
			break;
		case 362:
			pt[0].x = x0;
			pt[0].y = y0;
			pt[1].x = x0;
			pt[1].y = y0-2*h;
			pt[2].x = x0+w;
			pt[2].y = y0-h;
			pDC->Polygon(pt,3);								// »­Ë¢»­ÊµÐÄÈý±ßÐÎ
// 			StartP.x = x0;
// 			StartP.y = y0-h;			
// 			EndP.x = x0+w;
// 			EndP.y = y0;
// 			pDC->Arc(Round,StartP,EndP);					// »­Ô²»¡			
			break;
		case 370:
			pt[0].x = x0;
			pt[0].y = y0-h;
			pt[1].x = x0;
			pt[1].y = y0+h;
			pt[2].x = x0-w;
			pt[2].y = y0;
			pDC->Polygon(pt,3);								// »­Ë¢»­ÊµÐÄÈý±ßÐÎ	
			pt[0].x = x0;
			pt[0].y = y0-h;
			pt[1].x = x0;
			pt[1].y = y0+h;
			pt[2].x = x0+w;
			pt[2].y = y0;
			pDC->Polygon(pt,3);								// »­Ë¢»­ÊµÐÄÈý±ßÐÎ
			break;
		case 372:
			pDC->Ellipse(x0-w,y0-h,x0+w,y0+h);				// »­Ë¢»­ÊµÐÄÔ²
			break;			
		default:
			break;
		}
		pDC->SelectObject(&pOldPen);						// ÊÍ·Å»­±Ê
		pDC->SelectObject(&pOldBrush);						// ÊÍ·Å»­Ë¢	
		
		CPen pen1; 											// ¶¨Òå»­±Ê
		pen1.CreatePen(PS_SOLID,10,ARROWRED);				// ´´½¨»­±Ê
		pOldPen= pDC->SelectObject(&pen1);					// °ó¶¨»­±Ê	
		if (CommandDir == 361)
		{
			StartP.x = x0-w;
			StartP.y = y0;			
			EndP.x = x0;
			EndP.y = y0-h;
			pDC->Arc(Round,StartP,EndP);					// »­Ô²»¡
		} 		
		if (CommandDir == 362)		// ÓÒÐý
		{
			StartP.x = x0;
			StartP.y = y0-h;			
			EndP.x = x0+w;
			EndP.y = y0;
			pDC->Arc(Round,StartP,EndP);					// »­Ô²»¡
		} 		
		pDC->SelectObject(&pOldPen);						// ÊÍ·Å»­±Ê
		
		CTime curtime=CTime::GetCurrentTime();
		CString TimeMark = curtime.Format("%Y-%m-%d-%H:%M:%S ");	//²É¼¯²¢¼ÆËãÏµÍ³µ±Ç°Ê±¼ä
		
		CString MotionIfo;
		if (Control.mLineEnable != 0)				//ºÏ³Éµ±Ç°ÔË¶¯ÐÅÏ¢
		{
			MotionIfo.Format(" Â·¾¶%d¸öÂ·¶Î£¬%d#Â·¶Î%d¸öÎ¢²½£¬ÕýÔÚÖ´ÐÐµÚ%dÎ¢²½%d#ÃüÁî!",Control.mMaxLine,Control.mExeLine,Control.mMaxStep,Control.mExeStep,mARROW);
		}
		else
		{
			MotionIfo.Format(" Íê³ÉÂ·¾¶×Ü¼ÆºÄÊ±%dÃë!",Control.mRevers);				
		}
		//TimeMark += MotionIfo + DisplayControl(&Control);
		TimeMark += MotionIfo;
	
		CPen pen3; 											// ×Ô¶¨Òå»­±Ê
		pen3.CreatePen(PS_SOLID,1,RGB(0,255,0));			// ´´½¨»­±Ê
		CPen* pOldPen3= pDC->SelectObject(&pen3);			// Select a green pen
		m_BarCode.Draw(pDC->GetSafeHdc(), INS_DRAW_ACTUAL);	// Draw the found barcode frame
		pDC->TextOut(10, 360, DisplayControl(&Control), strlen(DisplayControl(&Control)));	// ÏÔÊ¾Ð¡³µ×´Ì¬ÐÅÏ¢
		pDC->TextOut(10, 380, DisplayPar(&Line[Control.mExeLine]), strlen(DisplayPar(&Line[Control.mExeLine])));
		pDC->TextOut(10, 400, DisplayPar(&Line[Control.mExeStep]), strlen(DisplayPar(&Line[Control.mExeStep])));
		pDC->TextOut(10, 420, TimeMark, strlen(TimeMark));	// ÏÔÊ¾Ð¡³µ×´Ì¬ÐÅÏ¢	
		pDC->TextOut(10, 440, SystemInfor, strlen(SystemInfor));	// ÏÔÊ¾Ð¡³µ×´Ì¬ÐÅÏ¢	
		pDC->TextOut(10, 460, BarDecodeInfo, strlen(BarDecodeInfo));	// ÏÔÊ¾½âÂëÐÅÏ¢
		pDC->SelectObject(&pOldPen3);						// Put back old pen	
		
		ReleaseDC(pDC);										// ÊÍ·Å»æÍ¼Ä¿±êÉè±¸	
	}
	//RunLife("S");	//Í³¼Æ³ÌÐòÔËÐÐÊ±¼ä
	return TRUE;	// ¼ÆËãÔÈËÙÔË¶¯Î»ÒÆ
}
// end ÔË¶¯±êÖ¾»æ»­Ä£¿é ---------------------------------------------------------------------------------------------------
// ÎÞÓÃ
BOOL CMicroMecanumDlg::DeMotionError(DOUBLE *dR,DOUBLE *X,DOUBLE *Y,DOUBLE *L,DOUBLE *dL)	//ÔË¶¯Îó²îÐÞÕý¼ÆËã¹«Ê½
{
	DOUBLE a,b,k,r,l,rd,ld;	// ¶¨Òå¾Ö²¿±äÁ¿

	rd = *dR;	// ÊäÈë°ë¾¶²îmm
	a = *X;		// ÊäÈëX×ø±êmm
	b = *Y;		// ÊäÈëY×ø±êmm

	if (a != 0)
	{
		r = (a*a + b*b)/(2*a);	// °ë¾¶mm
		k = asin(b/r);			// »¡¶ÈRad
		l = r*k;				// »¡³¤mm
		ld = rd*k;				// »¡²îmm 
	}
	else
	{
		l = b;					// »¡³¤mm
		ld = 0;					// »¡²îmm 
	}

	*L = l;			// Êä³ö»¡³¤mm
	*dL = ld;		// Êä³ö»¡²îmm 
	//*dL = -1 * ld;	// Êä³ö»¡²îmm 
	return TRUE;// Íê³É¹¦ÄÜ
}

BOOL CMicroMecanumDlg::DeMachineError(DOUBLE &R,DOUBLE &D,DOUBLE &dR,DOUBLE &L,DOUBLE &dL)	//»úÐµÎó²îÐÞÕý¼ÆËã¹«Ê½
{
	DOUBLE k;		// ¶¨Òå¾Ö²¿±äÁ¿

	k = asin(D/R);	// ¼ÆËã»¡¶ÈRad
	L = R * k;		// Êä³ö»¡³¤mm
	dL= dR * k; 	// Êä³ö»¡²îmm 

	return TRUE;	// Íê³É¹¦ÄÜ	
}

// ÎÞÓÃ
BOOL CMicroMecanumDlg::DeAngleError(DOUBLE *rL,DOUBLE *X,DOUBLE *Y,DOUBLE *A,DOUBLE *dL)	//Í¨¹ý½Ç¶ÈÐÞÕýÔË¶¯Îó²î
{
	DOUBLE a,b,c,k,r,l,dl;	// ¶¨Òå¾Ö²¿±äÁ¿

	l = *rL;	// ÊäÈëÔ²ÖÜ³¤mm
	a = *X;		// ÊäÈëX×ø±êmm
	b = *Y;		// ÊäÈëY×ø±êmm
	c = *A;		// ÊäÈë½Ç¶Èµ¥Î»¶È

	// ¼ÆËã°ë¾¶
	r = l/(2*3.1415926);	// °ë¾¶mm
	// ¼ÆËãÆ«º½½Ç
	k = (3.1415926 / 2) - acos(a/r);	// »¡¶ÈRad
	dl = r*k;				// »¡²îmm 
	*dL = -1 * dl;			// ·´ÏòÊä³ö»¡²îmm 
	TRACE("x=%.3f,r=%.3f,K=%.3f,A=%.3f,dl=%.3f,\n",a,r,(k * 180)/3.1415926,c,dl);
	return TRUE;// Íê³É¹¦ÄÜ
}

// ¶¯Ì¬Ð£×¼
BOOL CMicroMecanumDlg::FixTrack(DOUBLE &dR,DOUBLE &dX,DOUBLE &dY,DOUBLE &dA,
								  ControlParemeter* Control,MotionParemeter* step)	
{
	// ´ò¿ªÈÕÖ¾ÎÄ¼þ
ofstream ofsLog;
ofsLog.open("log-0 ¹ì¼£Ð£×¼¹ý³Ì.txt",ios::app);

	// Î¢²½³¤¶È
	int MicroDistance = Line[Control->mExeLine].MicroDistance * Puls2Distance;	//µ¥Î»ºÁÃ×
	// LineµÄ³¤¶È
	int LineLength = Line[Control->mExeLine].AidDistance * Puls2Distance;		// µ¥Î»ºÁÃ×

	// Ä¿Ç°ÕýÔÚÖ´ÐÐµÄÎ¢²½ÐòºÅÊÇ¡°mExeStep-3¡±£¬¡°mExeStep-2¡±ºÍ¡°mExeStep-1¡±¶¼ÔÚ¶ÑÕ»ÖÐ£¬
	// ¼´´Ó¡°mExeStep¡±¿ªÊ¼¿ÉÒÔÓÃÓÚÐ£×¼
	int StartFixStep = Control->mExeStep;//+1;
	// ×îºóÒ»¸öÎ¢²½ÐòºÅÊÇ¡°mMaxStep-1¡±£¬ÕâÒ»²½µÄ³¤¶ÈÐ¡ÓÚ»òµÈÓÚ±ê×¼²½³¤:
	int EndFixStep = Control->mMaxStep -1 -1;	//¼´-2£¬×îºóÒ»²½²»Ð£×¼

	// ÅÐ¶¨Ö´ÐÐÐ£×¼µÄÎ¢²½ÊÇ·ñ³¬³öÁË×îºóÒ»¸ö¿ÉÐ£×¼µÄÎ¢²½
	if (StartFixStep <= EndFixStep)
	{
		double x1, y1;	// ÉÏÒ»¸ö¶Áµ½µÄÐ£×¼µã×ø±ê£¨ÒÔ³ö·¢µãÎªÔ­µã£¬ÐÐ½ø·½ÏòÎªYÖáÕýÏòµÄ×ø±ê£¬·ÇµØÍ¼×ø±ê£©
		double x2, y2;	// µ±Ç°¶Áµ½µÄÐ£×¼µãµÄ×ø±ê£¨×ø±êÖáÍ¬ÉÏ£©
		double x3, y3;	// ¿ªÊ¼Ð£×¼µãµÄ×ø±ê£¨×ø±êÖáÍ¬ÉÏ£©
		double x4, y4;	// LineÖÕµãµÄµØ×ø±ê£¨×ø±êÖáÍ¬ÉÏ£©

		x1 = CheckPoints[nCheckPoints-2].adjustx;
		y1 = CheckPoints[nCheckPoints-2].adjusty;

		x2 = CheckPoints[nCheckPoints-1].deltax;
		y2 = CheckPoints[nCheckPoints-1].distance;

		y3 = StartFixStep * MicroDistance;
		x3 = x1 + (y3-y1)/(y2-y1) * (x2 - x1);		// ¸ù¾ÝÏàËÆÈý½ÇÐÎ¼ÆËã
		//x3 = x2 + (y3-y2) * tan (CheckPoints[nCheckPoints-1].deltaa * 3.14/180.0);

		x4 = 0;
		y4 = LineLength;

		
		// »ñµÃÐèÒª¸ÄÕýµÄ½Ç¶È£¬ÕýµÄ±íÊ¾ÐèÒªÏò×óÆ«£¬¸ºµÄ±íÊ¾ÐèÒªÏòÓÒÆ«£¬µ¥Î»Îª¶È
		double deltaH = GetRotateAngle(x1,y1,x2,y2,x3,y3,x4,y4);
		
		CheckPoints[nCheckPoints-1].deltah = deltaH;	// ¼ÇÂ¼Æ«ÒÆ½Ç

		double deltaH0 = deltaH;	// ±¸·ÝÓÃÓÚºóÃæµÄÊä³ö
		int TotalFixStep = 0;		// ¼ÇÂ¼Ò»¹²¾ÀÕýÁË¼¸²½

		// Èç¹ûÆ«½Ç¹ý´ó£¬Ôò·Ö¶à²½¾ÀÆ«£¨×î¶à2²½£¬¹ýÁË2²½»áÔÙ´Î¶Áµ½±ê£©£¬·ñÔò£¬ÔÚStartFixStepÒ»²½¾ÀÆ«
		// ·ÖÏòÓÒ²àºÍÏò×ó²àÁ½ÖÖÇé¿ö
		if (deltaH > 0.1)	// Ê¹Ð¡³µÏò×óÆ«
		{
			// ÎªÁË±ÜÃâÒ»´Î¾ÀÆ«½Ç¶È¹ý´ó£¬Èç¹û½Ç¶È³¬¹ýÏÞ¶È£¬Ôò·Ö¶à´ÎÐ£Õý
			for (int s = StartFixStep; s <= EndFixStep; s++)
			{
				if (deltaH > RotateLimit)
				{
					// ÒÔµ¥²½×î´ó½Ç¾ÀÕý
					INT32 caliD = RotateLimit * RotateFactor;//RotateFactor * MicroDistance;
					Step[s].CaliDistance = - caliD;	//Îª¸ºÊý£¬Íù×óÆ«
					TotalFixStep++;

					// ½«Æ«ÒÆ½Ç¼õÈ¥¸Õ¸Õ¾ÀÕýµÄ½Ç¶È£¬½üËÆ×÷ÎªÏÂÒ»²½´ý¾ÀÕýµÄ½Ç¶È
					deltaH -= RotateLimit;

					//Èç¹û³¬¹ý2´Î¾Í²»ÔÙ¼ÌÐøÐ£×¼
					if (TotalFixStep == 2)
					{
						deltaH -= 360.0;
						// ¸üÐÂcheckpoint£¬½«Êµ¼ÊµÄ±ä¹ìµã×÷Îª×îºóÒ»¸öcheckpoint
						CheckPoints[nCheckPoints-1].adjustx = x3;
						CheckPoints[nCheckPoints-1].adjusty = y3;
					}
				}
				else if (deltaH >0)
				{
					INT32 caliD = deltaH * RotateFactor;//* RotateFactor * MicroDistance;
					Step[s].CaliDistance = - caliD;//Îª¸ºÊý£¬Íù×óÆ«						
					TotalFixStep++;

					// ½«Æ«ÒÆ½Ç¼õÈ¥¸Õ¸Õ¾ÀÕýµÄ½Ç¶È£¬Ê¹ÆäÎª¸ºÊý£¬ÏÂ´ÎÑ­»·²»Ö´ÐÐ¾ÀÆ«
					deltaH -= RotateLimit;

					// ¸üÐÂcheckpoint£¬½«Êµ¼ÊµÄ±ä¹ìµã×÷Îª×îºóÒ»¸öcheckpoint
					CheckPoints[nCheckPoints-1].adjustx = x3;
					CheckPoints[nCheckPoints-1].adjusty = y3;
				}
			}			
		}
		else if (deltaH < -0.1)	// Ê¹Ð¡³µÏòÓÒÆ«
		{
			// ÎªÁË±ÜÃâÒ»´Î¾ÀÆ«½Ç¶È¹ý´ó£¬Èç¹û½Ç¶È³¬¹ýÏÞ¶È£¬Ôò·Ö¶à´ÎÐ£Õý
			for (int s = StartFixStep; s <= EndFixStep; s++)
			{
				if (-deltaH > RotateLimit)
				{
					// ÒÔµ¥²½×î´ó½Ç¾ÀÕý
					INT32 caliD =  RotateLimit * RotateFactor;//* RotateFactor * MicroDistance;
					Step[s].CaliDistance = caliD; //ÎªÕýÊý£¬ÍùÓÒÆ«
					TotalFixStep++;

					// ½üËÆ½«Æ«ÒÆ½Ç¼ÓÉÏ¸Õ¸Õ¾ÀÕýµÄ½Ç¶È£¬×÷ÎªÏÂÒ»²½´ý¾ÀÕýµÄ½Ç¶È
					deltaH += RotateLimit;
					
					//Èç¹û³¬¹ý2´Î¾Í²»ÔÙ¼ÌÐøÐ£×¼
					if (TotalFixStep == 2)
					{
						deltaH += 360.0;
						// ¸üÐÂcheckpoint£¬½«Êµ¼ÊµÄ±ä¹ìµã×÷Îª×îºóÒ»¸öcheckpoint
						CheckPoints[nCheckPoints-1].adjustx = x3;
						CheckPoints[nCheckPoints-1].adjusty = y3;
					}
				}
				else if (-deltaH > 0)
				{
					INT32 caliD =  deltaH * RotateFactor;//* RotateFactor * MicroDistance;
					Step[s].CaliDistance = - caliD;	//ÎªÕýÊý£¬ÍùÓÒÆ«
					TotalFixStep++;

					// ½«Æ«ÒÆ½Ç¼ÓÉÏ¸Õ¸Õ¾ÀÕýµÄ½Ç¶È£¬Ê¹ÆäÎªÕýÊý£¬ÏÂ´ÎÑ­»·²»Ö´ÐÐ¾ÀÆ«
					deltaH += RotateLimit;
					
					// ¸üÐÂcheckpoint£¬½«Êµ¼ÊµÄ±ä¹ìµã×÷Îª×îºóÒ»¸öcheckpoint
					CheckPoints[nCheckPoints-1].adjustx = x3;
					CheckPoints[nCheckPoints-1].adjusty = y3;
				}
			}			
		}
		else	// ½Ç¶ÈÆ«ÒÆÐ¡ÓÚ0.1¶È²»¾ÀÕý
		{
ofsLog << "    ½Ç¶ÈÆ«ÒÆÐ¡ÓÚ0.1¶È²»¾ÀÕý\n";
		}

		CString Tempstr;
		Tempstr.Format("LL=%d,SS=%d,x1=%.1f,y1=%.1f,x2=%.1f,y2=%.1f,x3=%.1f,y3=%.1f,x4=%.1f,y4=%.1f,Æ«ÒÆ½Ç=%.1f,¾ÀÆ«²½Êý=%d\n",
			Control->mExeLine,Control->mExeStep,x1,y1,x2,y2,x3,y3,x4,y4,deltaH0,TotalFixStep);

		// Ð´ÈëÈÕÖ¾ÎÄ¼þÖÐ
ofsLog << "    x1="<<x1<<",y1="<<y1<<",x2="<<x2<<",y2="<<y2;
ofsLog << ",x3="<<x3<<",y3="<<y3<<",x4="<<x4<<",y4="<<y4<<"\n";
ofsLog << "    ÐèÆ«ÒÆ½Ç="<<deltaH0<<",Êµ²â·½Ïò½Ç="<<CheckPoints[nCheckPoints-1].deltaa<<"\n";
ofsLog << "    Æ«ÒÆ²½Êý="<<TotalFixStep<<",sStep="<<StartFixStep<<",eStep="<<EndFixStep<<"\n";

		//Êä³öCheckPoints[i].adjustxµÈ
		
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

		// ·¢ËÍ¹ì¼£Ð£×¼Ïà¹ØµÄÊý¾Ýµ½ºóÌ¨ 
		SendIGVTrackFixInfo(CheckMarks[nCheckPoints-2].barCode,		// ¼ì²âµãID
							CheckMarks[nCheckPoints-2].x,			// ¼ì²âµãµØ±êx
							CheckMarks[nCheckPoints-2].y,			// ¼ì²âµãµØ±êy
							CheckPoints[nCheckPoints-2].deltah,		// ¼ì²âµã·½ÏòÆ«ÒÆ½Ç	
							CheckPoints[nCheckPoints-2].deltax,		// ¼ì²âµãxÆ«ÒÆÁ¿
							CheckPoints[nCheckPoints-2].deltay,		// ¼ì²âµãyÆ«ÒÆÁ¿
							CheckPoints[nCheckPoints-2].speed,		// ¼ì²âµãÐ¡³µËÙ¶È
							CheckPoints[nCheckPoints-2].adjustx,	// Ð£×¼µãx×ø±ê(ÆðµãÎª×ø±êÔ­µã£¬ÆðµãÖ¸ÏòÖÕµãÎªYÖáÕýÏò)
							CheckPoints[nCheckPoints-2].adjusty,	// Ð£×¼µãy×ø±ê(Í¬ÉÏ)							
							RotateFactor,							// Ð£×¼µãÐ£×¼ÏµÊý
							CheckMarks[nCheckPoints-1].barCode,		// ¼ìÑéµãID
							CheckMarks[nCheckPoints-1].x,			// ¼ìÑéµãµØ±êx
							CheckMarks[nCheckPoints-1].y,			// ¼ìÑéµãµØ±êy
							CheckPoints[nCheckPoints-1].deltah,		// ¼ìÑéµã·½ÏòÆ«ÒÆ½Ç
							CheckPoints[nCheckPoints-1].deltax,		// ¼ìÑéµãxÆ«ÒÆÁ¿
							CheckPoints[nCheckPoints-1].deltay,		// ¼ìÑéµãyÆ«ÒÆÁ¿
							m_igvInfo.Heading,						// Ð¡³µÀíÂÛÉÏµÄ³µÍ·³¯Ïò
							m_igvInfo.MoveDirection);				// Ð¡³µÀíÂÛÉÏµÄÔË¶¯·½Ïò
	}
	return TRUE;	// Íê³É¹¦ÄÜ	
}



// ¸ø¶¨ËÄ¸öµãµÄ×ø±ê£¬ÇóÐ¡³µÐèÒªÐý×ªµÄ½Ç¶È
// x1 y1 ³ö·¢µã
// x2 y2 ¼ì²âµ½µØ±êµÄµã
// x3 y3 ÑØ×Å12µã·½ÏòÑÓÉìµ½¿ÉÒÔÊµÊ©¾ÀÆ«µÄµã
// x4 y4 Ä¿±êµã£¬x4=0
// ·µ»ØdeltaH: ÕýµÄ±íÊ¾Ïò×óÆ«£¬¸ºµÄ±íÊ¾ÏòÓÒÆ«
double CMicroMecanumDlg::GetRotateAngle(double x1, double y1, double x2, double y2, 
										double x3, double y3, double x4, double y4)
{
	double deltaH = 0;
	// Çóµã1Ö¸Ïòµã2µÄÏòÁ¿12
	double x12 = x2-x1;
	double y12 = y2-y1;
	// ÏòÁ¿µÄÄ£
	double m12 = sqrt(x12*x12 + y12*y12);
	// Çóµã3Ö¸Ïòµã4µÄÏòÁ¿34
	double x34 = x4-x3;
	double y34 = y4-y3;
	// ÏòÁ¿µÄÄ£
	double m34 = sqrt(x34*x34 + y34*y34);

	double cosinA = (x12*x34 + y12*y34)/(m12*m34);
	deltaH = acos(cosinA);

	// ÅÐ¶ÏÏò×óÆ«»¹ÊÇÏòÓÒÆ«

	// Èç¹ûÁ½¸öÏòÁ¿¶¼ÔÚµÚÒ»ÏóÏÞ,×¢Òây12ºÍy34¿Ï¶¨ÊÇ>=0
	if (x12 >=0  && x34 >=0 )
	{
		if (x12 ==0 && x34 == 0)
			deltaH = 0;
		else if(x12 == 0 && x34 != 0 )	//ÐèÏòÓÒÆ«
			deltaH = -deltaH;
		else if(x12 != 0 && x34 == 0 )	//ÐèÏò×óÆ«
			deltaH = deltaH;
		else if(y12/x12 > y34/x34)		//ÐèÏòÓÒÆ«
			deltaH = -deltaH;
		else							//ÐèÏò×óÆ«
			deltaH = deltaH;
	}
	// Èç¹ûÁ½¸ö¶¼ÔÚµÚ¶þÏóÏÞ
	else if (x12 <= 0 && x34 <= 0)
	{
		if (x12 ==0 && x34 == 0)
			deltaH = 0;
		else if(x12 == 0 && x34 != 0 )	//ÐèÏò×óÆ«
			deltaH = deltaH;
		else if(x12 != 0 && x34 == 0 )	//ÐèÏòÓÒÆ«
			deltaH = -deltaH;
		else if(y12/x12 > y34/x34)		//ÐèÏòÓÒÆ«
			deltaH = -deltaH;
		else							//ÐèÏò×óÆ«
			deltaH = deltaH;
	}
	// Èç¹û12ÔÚµÚÒ»ÏóÏÞ£¬34ÔÚµÚ¶þÏóÏÞ£¬ÔòÐèÏò×óÆ«
	else if (x12 >=0 && x34 <=0)
		deltaH = deltaH;
	else //12ÔÚµÚ¶þÏóÏÞ£¬34ÔÚµÚÒ»ÏóÏÞ£¬ÔòÐèÏòÓÒÆ«
		deltaH = -deltaH;
		
	return deltaH * 180/3.14;
}

// ±¾º¯Êý¾­ÑéÖ¤ºóÎ´±»²ÉÓÃ£¬ÒòÎªµ±³õ×öÊµÑéÊ±Ïà¶ÔÓÚµØ±êµÄÆ«ÒÆ½Ç²âÁ¿¾«¶ÈÎÞ·¨±£Ö¤
// ¸ø¶¨µÚ3¡¢4µãµÄ×ø±êºÍ³µ×ÓµÄÆ«ÒÆ½Çdeltaa£¨Õë¶ÔÔË¶¯·½ÏòµÄ£¬Æ«ÓÒÎªÕý£©£¬ÇóÐ¡³µÐèÒªÐý×ªµÄ½Ç¶È
// x3 y3 ÑØ×Å12µã·½ÏòÑÓÉìµ½¿ÉÒÔÊµÊ©¾ÀÆ«µÄµã
// x4 y4 Ä¿±êµã£¬x4=0
// deltaa ³µ×ÓµÄÆ«ÒÆ½Ç£¨Õë¶ÔÔË¶¯·½ÏòµÄ£¬Æ«ÓÒÎªÕý£©
// isForward ÊÇ·ñÇ°½ø£¨¼´·½ÏòÊÇ0¶È£¬·ñÔò·½ÏòÊÇ180¶È£©
// ·µ»ØdeltaA: ÕýµÄ±íÊ¾ÐèÏò×óÆ«£¨Ä¿Ç°Æ«ÓÒÁË£©£¬¸ºµÄ±íÊ¾ÏòÓÒÆ«
double CMicroMecanumDlg::GetRotateAngle(double x3, double y3,double x4, double y4, double deltaa)
{
	// ¸ù¾Ýdeltaa ºÍ isForwardÔì³öx1,y1ºÍx2,y2
	double x1 = 0;
	double y1 = 0;
	double x2;
	double y2 = 1000;
	deltaa = deltaa*3.14/180.0;
	// Çóµã1Ö¸Ïòµã2µÄÏòÁ¿12
	if (deltaa > 0)	// ³µÍ·Æ«ÓÒ
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

// ²éÕÒsBarµØ±ê¸½½üµÄÖÐÐÄµØ±ê(½öÔÚ²¢ÅÅµÄÈý¸öÖÐÕÒ)£¬¼ÙÈç×ÔÉí¾ÍÊÇÖÐÐÄ£¬Ôò·µ»Ø×ÔÉíbarCode.
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
			if( fabs(marks[i].x - x) +fabs(marks[i].y-y) <100.1 ) //ÔÚÒ»ÅÅ
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

	if(count <2)	// Î´ÕÒµ½²¢ÅÅµÄÁíÁ½¸ö±ê
		return sBarCentral;

	double x1 = mapMarks[sBar1]->x;
	double y1 = mapMarks[sBar1]->y;
	double x2 = mapMarks[sBar2]->x;
	double y2 = mapMarks[sBar2]->y;

	if (fabs(y1-y2) < 0.1)	// ºáÅÅ
	{
		if ( fabs(x+x1-2*x2) < 0.1) // x+x1 == 2*x2
			sBarCentral = sBar2;
		else if (fabs(x+x2-2*x1) < 0.1)
			sBarCentral = sBar1;
		else 
			sBarCentral = sBar;
	}
	else	// ÊúÅÅ
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

// ¸ù¾Ýµ±Ç°¶Áµ½µÄµØ±ê(¼°ÆäÆ«ÒÆÁ¿)ºÍÖÐÐÄµØ±ê£¬ÒÔ¼°³µÍ·³¯Ïò£¬È·¶¨³µ×ÓµÄÒÆ¶¯²ÎÊý
// pMark µ±Ç°µØ±ê
// pMarkCentral ÖÐÐÄµØ±ê
// deltax µ±Ç°µØ±êÆ«ÒÆÁ¿x
// deltay µ±Ç°µØ±êÆ«ÒÆÁ¿y
// m_Heading ³µÍ·³¯Ïò
MotionParemeter CMicroMecanumDlg::GetCaliLine(MARK* pMark, MARK* pMarkCentral, double deltax, double deltay, double m_Heading)
{
	//MotionParemeter MP;
	int dir;
	int dis;
	double x12 = pMarkCentral->x - pMark->x;	// xÏòÁ¿
	double y12 = pMarkCentral->y - pMark->y;	// yÏòÁ¿
	switch((int)m_Heading)
	{
	case 0:		// ³µÍ·³¯±±
		if (x12 > 40 && y12 < 1)		// Ïò¶«ÒÆ¶¯
		{
			dir = 90;
			dis = x12 - deltax;
		}
		else if (x12 < 1 && y12 > 40)	// Ïò±±ÒÆ¶¯
		{
			dir = 0;
			dis = y12 - deltay;
		}
		else if (x12 < -40 && y12 < 1)	// ÏòÎ÷ÒÆ¶¯
		{
			dir = 270;
			dis = -x12 + deltax;
		}
		else if (x12 < 1 && y12 < -40)	// ÏòÄÏÒÆ¶¯
		{
			dir = 180;
			dis = -y12 + deltay;
		}
		break;
	case 90:	// ³µÍ·³¯¶«
		if (x12 > 40 && y12 < 1)		// Ïò¶«ÒÆ¶¯
		{
			dir = 0;
			dis = x12 - deltay;
		}
		else if (x12 < 1 && y12 > 40)	// Ïò±±ÒÆ¶¯
		{
			dir = 270;
			dis = y12 + deltax;
		}
		else if (x12 < -40 && y12 < 1)	// ÏòÎ÷ÒÆ¶¯
		{
			dir = 180;
			dis = -x12 + deltay;
		}
		else if (x12 < 1 && y12 < -40)	// ÏòÄÏÒÆ¶¯
		{
			dir = 90;
			dis = -y12 - deltax;
		}		
		break;
	case 180:	// ³µÍ·³¯ÄÏ
		if (x12 > 40 && y12 < 1)		// Ïò¶«ÒÆ¶¯
		{
			dir = 270;
			dis = x12 + deltax;
		}
		else if (x12 < 1 && y12 > 40)	// Ïò±±ÒÆ¶¯
		{
			dir = 180;
			dis = y12 + deltay;
		}
		else if (x12 < -40 && y12 < 1)	// ÏòÎ÷ÒÆ¶¯
		{
			dir = 90;
			dis = -x12 - deltax;
		}
		else if (x12 < 1 && y12 < -40)	// ÏòÄÏÒÆ¶¯
		{
			dir = 0;
			dis = -y12 - deltay;
		}		
		break;
	case 270:	// ³µÍ·³¯Î÷
		if (x12 > 40 && y12 < 1)		// Ïò¶«ÒÆ¶¯
		{
			dir = 180;
			dis = x12 + deltay;
		}
		else if (x12 < 1 && y12 > 40)	// Ïò±±ÒÆ¶¯
		{
			dir = 90;
			dis = y12 - deltax;
		}
		else if (x12 < -40 && y12 < 1)	// ÏòÎ÷ÒÆ¶¯
		{
			dir = 0;
			dis = -x12 - deltay;
		}
		else if (x12 < 1 && y12 < -40)	// ÏòÄÏÒÆ¶¯
		{
			dir = 270;
			dis = -y12 + deltax;
		}		
		break;
	}

	return GetMotionParameters(dir, dis);
	/*
		// 1.ÔË¶¯·½Ïò
		//MP.Derection	;	
		// 2.Ä¿±ê¾àÀë
		//MP.AidDistance	= int(fabs(x12) + fabs(y12));	
		// 3.ÔË¶¯ÀàÐÍ
		MP.MotionType	= LineRips;
		// 4.ËÙ¶È±¶ÂÊ
		MP.SpeedRate	= sRate;
		// 5.Æô¶¯ËÙ¶È
		MP.StartSpeed	= 0;
		// 6.×î¸ßËÙ¶È
		MP.MaxSpeed		= 500;
		// 7.¼Ó¼ÓËÙ¶È
		MP.ACC			= 500;	
		// 8.¼õ¼ÓËÙ¶È
		MP.DACC			= 500;
		// 9.¼õËÙ¾àÀë
		MP.ReducePoint	= 0;
		// 10.Æô¶¯ÀàÐÍ
		MP.StartType	= STAUD;
		// 11.Ð£Õý¾àÀë
		MP.CaliDistance	= 0;
		// 12.Î¢²½²½³¤	
		MP.MicroDistance =  200;
		// 13.X·ÖÖáÄ¿±ê
		MP.AidX			= 1;
		// 14.Y·ÖÖáÄ¿±ê
		MP.AidY			= 2;
		// 15.Z·ÖÖáÄ¿±ê
		MP.AidZ			= 3;
		// 16.U·ÖÖáÄ¿±ê	
		MP.AidU			= 4;	

	return MP;
	*/
}

// ¸ù¾ÝÔË¶¯·½ÏòºÍÄ¿±ê¾àÀë·µ»ØÒ»Ì×ÔË¶¯²ÎÊý£¨MotionParameter½á¹¹Ìå£©
// dir ·½Ïò£¨¶È£¬ÕýÇ°·½Îª0¶È£¬Ë³Ê±Õë·½Ïò¼Æ£©
// ¾àÀë£¨ºÁÃ×£©
// Ç°½ø=0£¬ºóÍË=180£¬×óÒÆ=270£¬ÓÒÒÆ=180£¬ÔÝÍ£=372£¬×ó×ª=361£¬ÓÒ×ª=270£¬ÏÂ½µ=366£¬Ì§Éý=367£¬Ð£×¼=370£»
// AidDistanceµÄÈ¡Öµ£ºÇ°½ø¡¢ºóÍË¡¢×óÒÆ¡¢ÓÒÒÆµÄ¾àÀë£¬µ¥Î»ºÁÃ×
// ÔÝÍ£µÄµ¥Î»ÊÇ0.1Ãë£¬Èç¹ûÔÝÍ£1Ãë£¬AidDistance=10£»
// ÏÂ½µ¡¢Ì§Éý¡¢Ð£×¼Çé¿öÏÂ£¬AidDistance=0¾Í¿ÉÒÔÁË

MotionParemeter CMicroMecanumDlg::GetMotionParameters(int dir, int dis)
{
	MotionParemeter MP;
		// 1.ÔË¶¯·½Ïò
		MP.Derection  = dir	;			
		// 2.Ä¿±ê¾àÀë
		MP.AidDistance	= dis;		 	
		// 3.ÔË¶¯ÀàÐÍ
		MP.MotionType	= LineRips;
		// 4.ËÙ¶È±¶ÂÊ
		MP.SpeedRate	= sRate;
		// 5.Æô¶¯ËÙ¶È
		MP.StartSpeed	= 0;
		// 6.×î¸ßËÙ¶È
		MP.MaxSpeed		= 500;
		// 7.¼Ó¼ÓËÙ¶È
		MP.ACC			= 500;	
		// 8.¼õ¼ÓËÙ¶È
		MP.DACC			= 500;
		// 9.¼õËÙ¾àÀë
		MP.ReducePoint	= 0;
		// 10.Æô¶¯ÀàÐÍ
		MP.StartType	= STAUD;
		// 11.Ð£Õý¾àÀë
		MP.CaliDistance	= 0;
		// 12.Î¢²½²½³¤	
		MP.MicroDistance =  200;
		// 13.X·ÖÖáÄ¿±ê
		MP.AidX			= 1;
		// 14.Y·ÖÖáÄ¿±ê
		MP.AidY			= 2;
		// 15.Z·ÖÖáÄ¿±ê
		MP.AidZ			= 3;
		// 16.U·ÖÖáÄ¿±ê	
		MP.AidU			= 4;	

		return MP;
}

DWORD CMicroMecanumDlg::RunLife(CString TimeUnit)	//Í³¼Æ³ÌÐòÔËÐÐÊ±¼ä
{
	static DWORD TimePoint0,TimePoint1;
	TimePoint1 =  GetCurrentTime();
	if (TimePoint0 == 0)		// ¼ÇÒäÆðÊ¼Öµ
	{
		TimePoint0 = TimePoint1;
		TRACE("Time0=%d!\n",TimePoint0);
	}
	if (TimeUnit == "S")
	{
		TimePoint1 = (TimePoint1 - TimePoint0)/1000;
		TRACE("³ÌÐòÔËÐÐ%dÃë!\n",TimePoint1);
	} 
	else
	{
		TimePoint1 = TimePoint1 - TimePoint0;
		TRACE("³ÌÐòÔËÐÐ0X%XºÁÃë!\n",TimePoint1);
	}	
	return TimePoint1;// Íê³É¹¦ÄÜ
}




void CMicroMecanumDlg::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	CString s;
	m_HeadingCombBxo.GetLBText(m_HeadingCombBxo.GetCurSel(),s);
	m_igvInfo.Heading = atoi(s);
}

// ¸ù¾ÝÐý×ªÖ¸Áî¸üÐÂ³µÍ·³¯Ïò
// cmd: 361ÄæÊ±Õë×ª£¬362Ë³Ê±Õë
// angle: ×ª360¶ÈµÄangleÖµÎª3180 (TurnRound = 3180)
void CMicroMecanumDlg::UpdateHeading(INT32 cmd, INT32 angle)
{
	int heading = m_igvInfo.Heading+0.5;	// È¡Õû
	int turn = int(angle*360/TurnRound + 0.5);	// ÕûÊý
	
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

// ÅÐ¶ÁÒ»¸öÌõÂëµÄ¸÷Î»ÊÇ·ñÊÇ0-9µÄÊý×Ö
BOOL CMicroMecanumDlg::CheckBarcode(CString bar)
{
	// ÑéÖ¤³¤¶È
	if (bar.GetLength() != BarCodeNumber)
	{
m_ofsLog2 << "¶Áµ½µÄµØ±ê("<<bar<<")³¤¶È²»µÈÓÚ"<<BarCodeNumber<<"\n";
		return false;
	}
	// ÑéÖ¤×Ö·û
	for (int i =0; i< BarCodeNumber; i++)
	{
		char c = bar.GetAt(i);
		if (c<'0' || c>'9')
		{
			m_ofsLog2 << "¶Áµ½µÄµØ±ê("<<bar<<")ÓÐ·Ç0-9×Ö·û\n";
			return false;
		}
	}

	// ÑéÖ¤µØ±ê
	if(GetMarkInfo(bar) == NULL)
	{
		m_ofsLog2 << "¶Áµ½µÄµØ±ê("<<bar<<")ÔÚµØ±êÎÄ¼þÖÐÎ´²éµ½\n";
		return false;
	}

	return true;
}

// ¸üÐÂ³µÁ¾µ±Ç°×ø±ê
// dis ³µÁ¾ÑØÖ±ÏßÐÐ×ßµÄ¾àÀë£¬×¢Òâdis >= 0
void CMicroMecanumDlg::UpdateOwnPosition(int dis)
{
	int iDirection = Line[Control.mExeLine].Derection; // ÔË¶¯·½Ïò
	// ÇÒÕýÔÚ½øÐÐÇ°½ø¡¢ºóÍË¡¢×óÒÆ¡¢ÓÒÒÆËÄÖÖ¶¯×÷Ö®Ò»
	if (iDirection ==0 || iDirection == 90 || iDirection == 180 || iDirection == 270)
	{
		// Èç¹ûÕýÔÚÖ´ÐÐlineÖÐµÄÄ³¸östep£¬²»´¦ÓÚÔÝÍ£×´Ì¬
		if (Control.mLineEnable != 0 && Control.mStepEnable != 0 && Control.mPause == 0 && Control.mGoOn == 0)
		{			
			int iHeading = int(m_igvInfo.Heading+0.5); // ³µÍ·³¯Ïò
			switch(iHeading)
			{
			case 0:	// ³µÍ·³¯±±
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
			case 90:	// ³µÍ·³¯¶«
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
			case 180:	// ³µÍ·³¯ÄÏ
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
		// Èç¹ûÄ³¸öLineµÄ¶¯×÷Íê³ÉÁË£¬Ôò¸üÐÂpreX, preY
		if (Control.mStepEnable == 0 && Control.mLine == 50)
		{
			m_igvInfo.preX = m_igvInfo.curX;
			m_igvInfo.preY = m_igvInfo.curY;
//m_ofsLog2<<"Ä³¸öLineµÄ¶¯×÷Íê³ÉÁË£¬´Ë´¦¸üÐÂ×ø±ê2£¬curx="<<curX<<",cury="<<curY<<",prex="<<preX<<",prey="<<preY<<"\n";
		}

	}
}

// ¼ìÑéÒ»ÏÂµ±Ç°Î»ÖÃ£¬ÀýÈç£¬ÊÇ²»ÊÇÀëÉÏÒ»¸ö¶Áµ½µÄ±êÌ«Ô¶ÁË£¬»òÕßÀëLine³ö·¢µãÌ«Ô¶ÁË
// Èç¹ûÊÇ£¬ÔòÐ¡³µ½ô¼±Í£³µ£¬ÇÒ±¨¾¯
void CMicroMecanumDlg::ValidatePosition()
{
	int iDirection = Line[Control.mExeLine].Derection; // ÔË¶¯·½Ïò
	// Èç¹ûÕýÔÚ½øÐÐÇ°½ø¡¢ºóÍË¡¢×óÒÆ¡¢ÓÒÒÆËÄÖÖ¶¯×÷Ö®Ò»
	if (iDirection ==0 || iDirection == 90 || iDirection == 180 || iDirection == 270)
	{
		// Èç¹ûÕýÔÚÖ´ÐÐline£¬ÇÒ²»´¦ÓÚÔÝÍ£×´Ì¬
		if (Control.mLineEnable != 0 && Control.mPause == 0 && Control.mGoOn == 0)
		{
			if (nCheckPoints > 0)
			{
				double dis1	= fabs(m_igvInfo.curX-CheckMarks[nCheckPoints-1].x) + fabs(m_igvInfo.curY-CheckMarks[nCheckPoints-1].y);
				double dis2 = fabs(m_igvInfo.curX-m_igvInfo.preX) + fabs(m_igvInfo.curY-m_igvInfo.preY);
				if (dis1 > MAX_NotFindMarkDistance && dis2 > MAX_NotFindMarkDistance)
				{
m_ofsLog2 << "--Ì«Ô¶¾àÀëÎ´·¢ÏÖµØ±ê£¬ÀëÉÏ´Î·¢ÏÖµØ±ê"<<(char*)(_bstr_t)CheckMarks[nCheckPoints-1].barCode.GetBuffer(0)
						<<"µÄ¾àÀëÊÇ£º"<<dis1<<"ºÁÃ×£¬ÀëÉÏ´Î¼ÇÂ¼×ø±êµãµÄ¾àÀëÊÇ£º"<<dis2<<"ºÁÃ×£¬½«×Ô¶¯Í£³µ£¡\n";
					
					OnButton1();	// ½ô¼±Í£Ö¹
				}
			}
		}
		
	}
}

// ¼ÆËãËÑË÷Â·¾¶£¨¹²Num_SearchLinesÌõ£©
// Ä¿Ç°ËÑË÷450mm*250mmµÄ·¶Î§£¨¼ÙÉèÉãÏñÍ·¸²¸Ç150mm*50mm£©
// ¸Ãº¯Êý²»ÓÃÁË£¬¸ÄÎª¶ÁÎÄ¼þ 2016-3-7 qi
void CMicroMecanumDlg::CalculateSearchLines()
{
	// ÉèÖÃËÑË÷Â·¾¶
	SearchLines[0] = GetMotionParameters(0, 80);	// ÉÏÒÆ8cm
	SearchLines[1] = GetMotionParameters(180, 160);	// ÏÂ18cm
	SearchLines[2] = GetMotionParameters(270, 80);	// ×ó8	
	SearchLines[3] = GetMotionParameters(0, 160);	// ÉÏ16 
	SearchLines[4] = GetMotionParameters(90, 160);	// ÓÒ16
	SearchLines[5] = GetMotionParameters(180, 160);	// ÏÂ16
}

// ¶ÁÈ¡µØ±êËÑË÷Â·¾¶ÎÄ¼þ
void CMicroMecanumDlg::ReadSearchLines()
{
	ifstream fs;

	fs.open("searchlines.txt", ios::nocreate);//Ä¬ÈÏÒÔ ios::in µÄ·½Ê½´ò¿ªÎÄ¼þ£¬ÎÄ¼þ²»´æÔÚÊ±²Ù×÷Ê§°Ü

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

// ¶¨Ê±Ïò·þÎñ¶Ë·¢ËÍÐ¡³µÊµÊ±ÐÅÏ¢
void CMicroMecanumDlg::SendIGVRealTimeInfo()
{

	time_t t = time( 0 ); 
	char tmp[64]; 
	strftime( tmp, sizeof(tmp), "%X",localtime(&t) );

	ofstream m_ofsLog3;   //ÈÕÖ¾ÎÄ¼þÁ÷
	Json::Value msg;// ¹¹½¨¶ÔÏó
	Json::Value msgHead;
	Json::Value msgBody;

	char mac_address[1024];
	//GetMacaddress((char*)mac_address);	
	if(GetMacaddress(mac_address)<=0)
    {
        // Ð´ÈëÈÕÖ¾¡°»ñÈ¡MACµØÖ·Ê§°Ü£¬ÎÞ·¨µÇÂ¼¡±
		m_ofsLog3.open("log-3Á¬½ÓSocketºÍ½ÓÊÜÏûÏ¢.txt",ios::app);
		m_ofsLog3<<tmp<<"·¢ËÍÐ¡³µÊµÊ±ÐÅÏ¢Ê±£¬»ñÈ¡MACµØÖ·Ê§°Ü\n"; 	
		m_ofsLog3.close();
		return;
    }
	std::string mac(mac_address); 
	msgHead["KeyId"] = mac;  // macµØÖ·
	msgHead["MessageType"] = "2";    // ÏûÏ¢ÀàÐÍ
	msg["MessageHead"] = msgHead;
	
	msgBody["X"] = int(m_igvInfo.curX);
	msgBody["Y"] = int(m_igvInfo.curY);
	msgBody["Speed"] = (m_igvInfo.Speed);
	msgBody["Direction"] = int(m_igvInfo.Heading);
	msgBody["IsFree"] = m_igvInfo.isFree?"true":"false";
	msgBody["IsMarked"] = m_igvInfo.isMarked?"true":"false";
	msgBody["MarkId"] = (char*)(_bstr_t)m_igvInfo.MarkID.GetBuffer(0);	

//ofstream ofsLog;				
//ofsLog.open("log-4 isFree×´Ì¬µ÷ÊÔ.txt",ios::app);
//ofsLog<<"·¢ÏûÏ¢,MarkID: "<<(char*)(_bstr_t)m_igvInfo.MarkID.GetBuffer(0)<<"\n";
//ofsLog.close();

	Json::FastWriter writer;  // ÓÃJson::FastWriter À´´¦Àí json Ó¦¸ÃÊÇ×î¿ìµÄ
	std::string strMsgBody = writer.write(msgBody);
	msg["MessageBody"] = strMsgBody;	

	m_ofsLog3.open("log-3Á¬½ÓSocketºÍ½ÓÊÜÏûÏ¢.txt",ios::app);
	m_ofsLog3<<tmp<<"--×¼±¸·¢ËÍÐ¡³µÊµÊ±ÐÅÏ¢\n"; 	
	m_ofsLog3.close();

	std::string msg2server = writer.write(msg);
	SendMessage2Server((char*)msg2server.c_str());	
}
 
// ·¢ËÍ¹ì¼£Ð£×¼Ïà¹ØµÄÊý¾Ýµ½ºóÌ¨
void CMicroMecanumDlg::SendIGVTrackFixInfo(CString markID1, int x1, int y1, double deltah1, 
		double deltax1, double deltay1, double speed1, double x2, double y2,
		double k2, CString markID3, int x3, int y3, double deltah3, double deltax3, double deltay3,int heading, int motionDirection)
{
	Json::Value msg;// ¹¹½¨¶ÔÏó
	Json::Value msgHead;
	Json::Value msgBody;
//char* temp_MarkId1;
//char* temp_MarkId3;
	char mac_address[1024];
	//GetMacaddress((char*)mac_address);	
	if(GetMacaddress(mac_address)<=0)
    {
        // Ð´ÈëÈÕÖ¾¡°»ñÈ¡MACµØÖ·Ê§°Ü£¬ÎÞ·¨µÇÂ¼¡±
		return;
    }
	std::string mac(mac_address); 
	msgHead["KeyId"] = mac;  // macµØÖ·
	msgHead["MessageType"] = "3";    // ÏûÏ¢ÀàÐÍ
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

	Json::FastWriter writer;  // ÓÃJson::FastWriter À´´¦Àí json Ó¦¸ÃÊÇ×î¿ìµÄ
	std::string strMsgBody = writer.write(msgBody);
	msg["MessageBody"] = strMsgBody;

	std::string msg2server = writer.write(msg);
	SendMessage2Server((char*)msg2server.c_str());	

ofstream ofsLog;				
ofsLog.open("log-6 ¹ì¼£Ð£×¼¼ÇÂ¼.txt",ios::app);

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
// ±êÌâÀ¸
ofsLog<<"DelatH1  "<<"DelatH3  "<<"DelatX1  "<<"DelatY1  "<<"Heading  "<<"K2  "<<"MarkId1  ";
ofsLog<<"MarkId3  "<<"MotionDirection  "<<"Speed1  "<<"X1  "<<"X2  "<<"X3  "<<"Y1  "<<"Y2  "<<"Y3"<<"\n";*/


// ÄÚÈÝ
ofsLog<<deltah1<<"    "<<deltah3<<"    "<<deltax1<<"    "<<deltay1<<"    "<<deltax3<<"    "<<deltay3<<"    "<<heading<<"    ";
ofsLog<<k2<<"    "<<Temp_MarkId1<<"    "<<Temp_MarkId3<<"    "<<motionDirection<<"    "<<speed1<<"    ";
ofsLog<<x1<<"    "<<x2<<"    "<<x3<<"    "<<y1<<"    "<<y2<<"    "<<y3<<"\n";

//ofsLog<<(char*)strMsgBody.c_str()<<"\n";

ofsLog.close();
	
}

// socketÁ¬½Ó³É¹¦Ê±·¢ËÍµÇÂ¼ÏûÏ¢¸ø·þÎñ¶Ë
void CMicroMecanumDlg::SendIGVLoginMessage()
{
	//GetDlgItem(IDC_STATIC_Connect)->SetWindowText("ÒÑÁ¬½Ó");
	//SetDlgItemText(IDC_STATIC_Connect, "ÒÑÁ¬½Ó");
	m_ConnectStatus.SetWindowText("ÒÑÁ¬½Ó");
	ofstream m_ofsLog3;   //ÈÕÖ¾ÎÄ¼þÁ÷
	Json::Value msg;// ¹¹½¨¶ÔÏó
	Json::Value msgHead;
	Json::Value msgBody;

	char mac_address[1024];
	//GetMacaddress((char*)mac_address);	
	if(GetMacaddress(mac_address)>0)
    {
        // »ñÈ¡MACµØÖ·³É¹¦
		m_ofsLog3.open("log-3Á¬½ÓSocketºÍ½ÓÊÜÏûÏ¢.txt",ios::app);
		m_ofsLog3<<"»ñÈ¡MACµØÖ·³É¹¦\n"; 		
		m_ofsLog3.close();
    }
	else
    {
        // Ð´ÈëÈÕÖ¾¡°»ñÈ¡MACµØÖ·Ê§°Ü£¬ÎÞ·¨µÇÂ¼¡±
		m_ofsLog3.open("log-3Á¬½ÓSocketºÍ½ÓÊÜÏûÏ¢.txt",ios::app);
		m_ofsLog3<<"»ñÈ¡MACµØÖ·Ê§°Ü£¬ÎÞ·¨µÇÂ¼\n"; 	
		m_ofsLog3.close();
		return;
    }
	std::string mac(mac_address); 
	msgHead["KeyId"] = mac;  // macµØÖ·
	msgHead["MessageType"] = "0";    // ÏûÏ¢ÀàÐÍ

	msg["MessageHead"] = msgHead;
	msg["MessageBody"] = NULL;

	Json::FastWriter writer;  // ÓÃJson::FastWriter À´´¦Àí json Ó¦¸ÃÊÇ×î¿ìµÄ
	std::string msg2server = writer.write(msg); 
	SendMessage2Server((char*)msg2server.c_str());	
}

void CMicroMecanumDlg::SendMessage2Server(char* msg)
{	
	//msg = "00000";
	ofstream m_ofsLog3;   //ÈÕÖ¾ÎÄ¼þÁ÷
	if(send(m_Sock, msg, strlen(msg), 0) == SOCKET_ERROR) //ÅÐ¶Ï·¢ËÍÊÇ·ñ³É¹¦
	{
		 //MessageBox("·¢ËÍÊ§°Ü");
		m_ofsLog3.open("log-3Á¬½ÓSocketºÍ½ÓÊÜÏûÏ¢.txt",ios::app);
		m_ofsLog3<<"·¢ËÍÏûÏ¢Ê§°Ü\n"; 
		m_isSocketConnected = false;
		// GetDlgItem(IDC_STATIC_Connect)->SetWindowText("Î´Á¬½Ó");
		m_ConnectStatus.SetWindowText("Î´Á¬½Ó");
		m_ofsLog3.close();
	}
	else if(msg == "")   //ÅÐ¶Ï·¢ËÍµÄÊý¾ÝÊÇ·ñÎª¿Õ
	{
		 //MessageBox("ÇëÊäÈëÐÅÏ¢");
		m_ofsLog3.open("log-3Á¬½ÓSocketºÍ½ÓÊÜÏûÏ¢.txt",ios::app);
		m_ofsLog3<<"·¢ËÍÏûÏ¢Îª¿Õ\n"; 		
		m_ofsLog3.close();
	}
	else //·¢ËÍ³É¹¦
	{
		m_ofsLog3.open("log-3Á¬½ÓSocketºÍ½ÓÊÜÏûÏ¢.txt",ios::app);
		m_ofsLog3<<"·¢ËÍÏûÏ¢³É¹¦\n"; 	
		m_ofsLog3.close();
	}		
}
 
int CMicroMecanumDlg::GetMacaddress(char* mac) //»ñÈ¡±¾»úMACÖ· 
{
	ULONG ulSize=0;
	PIP_ADAPTER_INFO pInfo=NULL;
	int temp=0;
	temp = GetAdaptersInfo(pInfo,&ulSize);//µÚÒ»´¦µ÷ÓÃ£¬»ñÈ¡»º³åÇø´óÐ¡
	pInfo=(PIP_ADAPTER_INFO)malloc(ulSize);
	temp = GetAdaptersInfo(pInfo,&ulSize);

	int iCount=0;
	//while(pInfo)//±éÀúÃ¿Ò»ÕÅÍø¿¨
	{
		//  pInfo->Address MACÖ·
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


// ÊÕµ½ÏûÏ¢ºóµÄ´¦Àíº¯Êý
void CMicroMecanumDlg::Command2Route(char* sMsg)	
{
	MotionParemeter m_motionPar;

	// ¸´Î»Â·¾¶¶ÎÊýÖ¸Õë
	Control.mMaxLine = 0;	
	
	// ¶ÁÈ¡ÊÕµ½µÄÏûÏ¢
	Json::Reader *reader = new Json::Reader(Json::Features::strictMode());  
	Json::Value root;
	Json::Value arraymember1;
	if (reader->parse(sMsg, root))  // reader½«Json×Ö·û´®½âÎöµ½root£¬root½«°üº¬JsonÀïËùÓÐ×ÓÔªËØ
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

			// 1.ÔË¶¯·½Ïò
			Line[Control.mMaxLine].Derection	= m_motionPar.Derection;			
			// 2.Ä¿±ê¾àÀë
			Line[Control.mMaxLine].AidDistance	= m_motionPar.AidDistance;	
			// 3.ÔË¶¯ÀàÐÍ
			Line[Control.mMaxLine].MotionType	= LineRips;
			// 4.ËÙ¶È±¶ÂÊ
			Line[Control.mMaxLine].SpeedRate	= sRate;
			// 5.Æô¶¯ËÙ¶È
			Line[Control.mMaxLine].StartSpeed	= m_motionPar.StartSpeed;
			// 6.×î¸ßËÙ¶È
			Line[Control.mMaxLine].MaxSpeed		= m_motionPar.MaxSpeed;
			// 7.¼Ó¼ÓËÙ¶È
			Line[Control.mMaxLine].ACC			= m_motionPar.ACC;	
			// 8.¼õ¼ÓËÙ¶È
			Line[Control.mMaxLine].DACC			= m_motionPar.DACC;
			// 9.¼õËÙ¾àÀë
			Line[Control.mMaxLine].ReducePoint	= 0;
			// 10.Æô¶¯ÀàÐÍ
			Line[Control.mMaxLine].StartType	= STAUD;
			// 11.Ð£Õý¾àÀë
			Line[Control.mMaxLine].CaliDistance	= m_motionPar.CaliDistance;
			// 12.Î¢²½²½³¤	
			Line[Control.mMaxLine].MicroDistance = m_motionPar.MicroDistance;
			// 13.X·ÖÖáÄ¿±ê
			Line[Control.mMaxLine].AidX			= m_motionPar.AidX;
			// 14.Y·ÖÖáÄ¿±ê
			Line[Control.mMaxLine].AidY			= m_motionPar.AidY;
			// 15.Z·ÖÖáÄ¿±ê
			Line[Control.mMaxLine].AidZ			= m_motionPar.AidZ;
			// 16.U·ÖÖáÄ¿±ê	
			Line[Control.mMaxLine].AidU			= m_motionPar.AidU;	
					
			//µÝÔö¼ÆÊýÆ÷
			Control.mMaxLine++;		
		}	  
	}
	else
	{
	}
	delete reader;
 }

BOOL CMicroMecanumDlg::Deknock(void)			// ·ÀÅö×²¼ì²â 
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
		// ·´ÉäÐÍ·ÀÅö×²¼ì²â
		if (GpioOperation(&m_Device,GpioRead,XADDR,Pin0,1) == GpioLow)
		{
			// Èô¶Ë¿ÚµçÆ½À­µÍ,±íÃ÷ÔË¶¯·½ÏòÓÐÕÏ°­Îï,Á¢¿Ì¼õËÙÍ£³µ
			Stop(&m_Device,SDSTP);	// STOP / SDSTP ½ô¼±Í£Ö¹;
			StopMotion();			// Í£Ö¹Î¢²½ÔË¶¯

			MechineSwitchDown = 0;
			// ¹Ø±Õµç»úµçÔ´
			MotorPowerOFF();
		}
		else
		{
			if (MechineSwitchDown == 0)
			{
				MechineSwitchDown = 1;
				// »Ö¸´µç»ú¹©µç
				MotorPowerON();
			} 
		}

	}
/*	
	// »úÐµ¿ª¹Ø·ÀÅö×²¼ì²â
	if (GpioOperation(&m_Device,GpioRead,XADDR,Pin1,1) == GpioLow)
	{
		MechineSwitchDown = 0;
		// ¹Ø±Õµç»úµçÔ´
		MotorPowerOFF();
	}
	else
	{
		if (MechineSwitchDown == 0)
		{
			MechineSwitchDown = 1;
			// »Ö¸´µç»ú¹©µç
			MotorPowerON();
		} 
	}

	// ÊÇ·ñ²Ù×÷Æ½Ì¨ÔË¶¯	
	if (PlatMotionEnable == TRUE)
	{
		// ÒªÇóÆ½Ì¨ÔÚ×îµÍÎ»
		if (GpioOperation(GpioRead,ZADDR,PlatformPower,GpioHigh) == GpioLow)
		{
			//1#µç»úÖ´ÐÐ¶¯×÷
			if ((Dlg->GpioOperation(GpioRead,YADDR,Pin1,GpioHigh) == GpioHigh)		//µ½´ï×îµÍµã
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin5,GpioHigh) == GpioLow))	//1#µç»úÕýÔÚÔË¶¯
			{
				// ¹Ø±Õ1#µç»úµçÔ´
				Dlg->GpioOperation(GpioWrite,UADDR,Pin5,GpioHigh);
			}
			//2#µç»úÖ´ÐÐ¶¯×÷
			if ((Dlg->GpioOperation(GpioRead,XADDR,Pin2,GpioHigh) == GpioHigh)		//µ½´ï×îµÍµã
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin4,GpioHigh) == GpioLow))	//2#µç»úÕýÔÚÔË¶¯
			{
				// ¹Ø±Õ2#µç»úµçÔ´
				Dlg->GpioOperation(GpioWrite,UADDR,Pin4,GpioHigh);
			}
			//Íê³ÉÏÂ½µ¶¯×÷£¬µ½´ï×îµÍµã
			if ((Dlg->GpioOperation(GpioRead,YADDR,Pin1,GpioHigh) == GpioHigh)		//1#µ½´ï×îµÍµã
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin5,GpioHigh) == GpioHigh)	//1#µç»úÍ£Ö¹ÔË¶¯
				&& (Dlg->GpioOperation(GpioRead,XADDR,Pin2,GpioHigh) == GpioHigh)	//2#µ½´ï×îµÍµã
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin4,GpioHigh) == GpioHigh))	//2#µç»úÍ£Ö¹ÔË¶¯
			{
				// ½ûÖ¹Æ½Ì¨ÔË¶¯
				PlatMotionEnable = FALSE;
			}
		} 
		else	// ÒªÇóÆ½Ì¨ÔÚ×î¸ßÎ»
		{
			//1#µç»úÖ´ÐÐ¶¯×÷
			if ((Dlg->GpioOperation(GpioRead,YADDR,Pin0,GpioHigh) == GpioHigh)		//µ½´ï×îµÍµã
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin5,GpioHigh) == GpioLow))	//1#µç»úÕýÔÚÔË¶¯
			{
				// ¹Ø±Õ1#µç»úµçÔ´
				Dlg->GpioOperation(GpioWrite,UADDR,Pin5,GpioHigh);
			}
			//2#µç»úÖ´ÐÐ¶¯×÷
			if ((Dlg->GpioOperation(GpioRead,YADDR,Pin2,GpioHigh) == GpioHigh)		//µ½´ï×îµÍµã
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin4,GpioHigh) == GpioLow))	//2#µç»úÕýÔÚÔË¶¯
			{
				// ¹Ø±Õ2#µç»úµçÔ´
				Dlg->GpioOperation(GpioWrite,UADDR,Pin4,GpioHigh);
			}
			//Íê³ÉÏÂ½µ¶¯×÷£¬µ½´ï×îµÍµã
			if ((Dlg->GpioOperation(GpioRead,YADDR,Pin0,GpioHigh) == GpioHigh)		//1#µ½´ï×îµÍµã
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin5,GpioHigh) == GpioHigh)	//1#µç»úÍ£Ö¹ÔË¶¯
				&& (Dlg->GpioOperation(GpioRead,YADDR,Pin2,GpioHigh) == GpioHigh)	//2#µ½´ï×îµÍµã
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin4,GpioHigh) == GpioHigh))	//2#µç»úÍ£Ö¹ÔË¶¯
			{
				// ½ûÖ¹Æ½Ì¨ÔË¶¯
				PlatMotionEnable = FALSE;
			}		
		}
	}
	*/
	return TRUE;
}

BOOL CMicroMecanumDlg::MotorPowerON(void)		// ´ò¿ªµç»úµçÔ´
{
	GpioOperation(&m_Device,GpioWrite,ZADDR,Pin7,GpioLow);
	return TRUE;
}
BOOL CMicroMecanumDlg::MotorPowerOFF(void)		// ¹Ø±Õµç»úµçÔ´
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
	m_ConnectStatus.SetWindowText("ÒÑµÇÂ¼");
	return true;
}



//Ìí¼ÓÏß³Ìº¯Êýserver_thd()£ºSSS
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
	 memset(msg_rcv,0,1024*sizeof(char)); //lipan ³õÊ¼»¯×Ö·û´®
	 CString port;
	 CMicroMecanumDlg * dlg = (CMicroMecanumDlg *)AfxGetApp()->GetMainWnd(); //µÃµ½Ó¦ÓÃ³ÌÐò»î¶¯Ö÷´°¿ÚµÄÖ¸Õë
	 //dlg->port_edit->GetWindowText(port);//»ñµÃ¶Ë¿ÚµØÖ·
	 port="5009";
	 if(port=="") 
	 {
		 showeditmsg = "ÇëÊäÈë¶Ë¿ÚºÅ\r\n";
		 dlg->showmsg_edit.SetWindowText(showeditmsg);
		 return 0;
	 }
	 //Îªlocal_addr¸³Öµ£¬´´½¨soket
	 local_addr.sin_family = AF_INET;
	 local_addr.sin_port = htons(atoi(port)); //atoi£¬ °Ñ×Ö·û´®×ª»»³ÉÕûÐÍÊý
	 local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	 //´´½¨¼àÌýlisten_sock
	 if( (listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET )
	 {
		 showeditmsg = "´´½¨¼àÌýÊ§°Ü\r\n";
		 dlg->showmsg_edit.SetWindowText(showeditmsg);

		 return 0;
	 }
	 //°ó¶¨
	 if( bind(listen_sock, (struct sockaddr*) &local_addr, sizeof(SOCKADDR_IN)) )
	 {
		 showeditmsg = "°ó¶¨´íÎó,»»Ò»¸ö¶Ë¿ÚÊÔÊÔ\r\n";
		 dlg->showmsg_edit.SetWindowText(showeditmsg);
		 return 0;
	 }
	 
	 listen(listen_sock, 1); //¿ªÊ¼¼àÌý£¬ÔÊÐí×î´ó¼àÌýÊýÎª1
	 dlg->star_button.EnableWindow(FALSE);  //¿ªÆô·þÎñÆ÷°´Å¥»Ò»¯
	 dlg->showmsg_edit.ShowWindow(SW_SHOW); //°ó¶¨ÏûÏ¢editÏÔÊ¾
	 showeditmsg = "ÒÑ³É¹¦¿ªÆô....\r\n";
	 dlg->showmsg_edit.SetWindowText(showeditmsg);
	 
	 //½ÓÊÜÁ¬½ÓÇëÇó
	 if( (sock = accept(listen_sock, (struct sockaddr *)&client_addr, &iaddrSize)) == INVALID_SOCKET)
	 {
		 showeditmsg = "½ÓÊÜÁ¬½ÓÇëÇóÊ§°Ü\r\n";
		 dlg->showmsg_edit.SetWindowText(showeditmsg);
		 return 0;
	 }
	 else
	 {
		 CString port;
		 port.Format("%d", int(ntohs(client_addr.sin_port)));
		 showeditmsg = "ÒÑÁ¬½ÓÀ´×Ô£º" + CString(inet_ntoa(client_addr.sin_addr)) + "  ¶Ë¿Ú:5009\r\n";
		 dlg->showmsg_edit.SetWindowText(showeditmsg);
	 }
			 
	 ////////////-------½ÓÊÕÊý¾Ý
	 while(1)
	 {
		 MsgHandling(msg_rcv);				//lipan_¶Ô·µ»ØÐÅÏ¢½øÐÐ¹æ·¶´¦Àí£¬Ö»±£Áô×îÐÂµÄÒ»ÌõÏûÏ¢
		 if( (res = recv(sock, msg_rcv, 1024, 0)) == -1 ) //½ÓÊÕÊý¾Ý£¬ÅÐ¶ÏÊÇ·ñ½ÓÊÕ³É¹¦
		 {
			showeditmsg = "-------Ê§È¥Á¬½Ó\r\n";
			dlg->showmsg_edit.SetWindowText(showeditmsg);
			dlg->star_button.EnableWindow(TRUE);
			break;
		 }
		 else
		 {
			showeditmsg = "¿Í»§¶Ë:" + ((CString)(msg_rcv)).Mid(0,res) + "\r\n";
			dlg->showmsg_edit.SetWindowText(showeditmsg);

			// ¶ÁÈ¡ÊÕµ½µÄÏûÏ¢
			Json::Reader *reader = new Json::Reader(Json::Features::strictMode());  
			Json::Value root;
			if (reader->parse(msg_rcv, root))  // reader½«Json×Ö·û´®½âÎöµ½root£¬root½«°üº¬JsonÀïËùÓÐ×ÓÔªËØ
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
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	WSADATA wsData;  
	WSAStartup(MAKEWORD(2,2), &wsData); 

	char name[80];
	CString IP;
	CString P;
	hostent* pHost; 
	gethostname(name,sizeof(name));//»ñµÃÖ÷»úÃû 
	pHost = gethostbyname(name);//»ñµÃÖ÷»ú½á¹¹	 
	IP = inet_ntoa(*(struct in_addr *)pHost->h_addr_list);//»ñÈ¡Ö÷»úipµØÖ·
	showmsg_edit.SetWindowText("°ó¶¨IPµØÖ·£º" + IP);
	AfxBeginThread(&server_thd,0); //¿ªÆôÐÂÏß³Ì´¦Àí

}

void CMicroMecanumDlg::OnButton_PauseAndRecovery()
{
	// TODO: ÔÝÍ£¹¦ÄÜ²âÊÔ
	ofstream m_ofsLog8;
	m_ofsLog8.open("log_8 Ð¡³µÔÝÍ£Óë»Ö¸´¹¦ÄÜ²âÊÔ.txt",ios::app);
	if (FlagPause == 0)
	{
		Control.mPause = 0XFF;	// ½øÈëÔÝÍ£
		Control.mGoOn = 0XFF;	// ½ûÖ¹Á¬Ðø
		GetDlgItem(IDC_BUTTON_PauseAndRecovery)->SetWindowText("Recovery");
		m_ofsLog8<<"IDC:    "<<IDC_BUTTON_PauseAndRecovery<<" Êä³öLineµÄ¸öÊý "<<Control.mMaxLine<<endl;
		m_ofsLog8<<"½øÈëÔÝÍ£×´Ì¬      FlagPause = "<<FlagPause;
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
		m_ofsLog8<<"IDC:    "<<IDC_BUTTON_PauseAndRecovery<<" Êä³öLineµÄ¸öÊý "<<Control.mMaxLine<<endl;
		m_ofsLog8<<"½øÈë»Ö¸´×´Ì¬    Flagpause = "<<FlagPause;
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
	m_ofsLog9.open("log_9 Ð¡³µEmergencyBreak²âÊÔ.txt",ios::app);

	CString str;
	m_EmergencyBrake.GetWindowText(str);
	if (str == "½ô¼±ÖÆ¶¯")
	{	
		CurLineNo  = Control.mExeLine;

		FindBarEnable		= FALSE;			// ½ûÖ¹×Ô¶¯Ê¶±ðÌõÂë
		Control.mStep		= 0;				// ¸´Î»²½ÖèÖ¸Õë
		Control.mExeStep	= 0;				// ¸´Î»Î¢²½Ö¸Õë
		Control.mStepEnable = 0;				// ¸´Î»½ûÓÃ×Ô¶¯¿ØÖÆ
		Control.mFixStep = 0;					// ¸´Î»×Ô¶¯Ð£Õý²½Êý
		
		Control.mPause		= 0;				// ¸´Î»ÔÝÍ£Æì±ê
		Control.mGoOn		= 0;				// ¸´Î»¼ÌÐøÂ·¾¶ÔË¶¯Æì±ê
		Control.mRevers		= 0;				// ¸´Î»·´ÏòÆì±ê
		
		Control.mLine		= 0;				// ¸´Î»µ¥¶Î²½ÖèÖ¸Õë
		Control.mExeLine	= 0;				// ¸´Î»µ¥¶ÎÖ´ÐÐÖ¸Õë
		Control.mLineEnable = 0;				// ¸´Î»µ¥¶ÎÆì±ê
		Stop(&m_Device,STOP);

		m_ofsLog9<<"µ±Ç°Line    mExeLine = "<<Control.mExeLine<<" ";
		m_ofsLog9<<"×î´óLine    mMaxLine = "<<Control.mMaxLine<<"\n";

		m_EmergencyBrake.SetWindowText("»Ö¸´");
	} 
	else
	{
		Control.mMaxLine = Control.mMaxLine - CurLineNo;				    // ¸üÐÂ×î´óLineÊýÖµ
	
		m_ofsLog9<<"»Ö¸´ºóµÄ×î´óLine    mMaxLine = "<<Control.mMaxLine<<"\n";		
		
		// ¼ÇÂ¼ÐÐ×ßµÄ¾àÀë
		INT32 xLocation = INT32(DOUBLE(GetShiftR(&m_Device,XADDR)) * Puls2Distance);	// ¼ÆËãÒÑ¾­ÐÐ×ßµÄ¾àÀë
		Temp_Line[CurLineNo].AidDistance = Temp_Line[CurLineNo].AidDistance - ((INT32)abs(xLocation));
		
		memset(Line, 0, sizeof(struct MotionParemeter)*mTotal); // Çå¿Õ
		
		//¸üÐÂLine
		for (int i = 0; i<Control.mMaxLine; i++)
		{
			Line[i] = Temp_Line[CurLineNo];
			CurLineNo++;
		}

		// ¿ªÊ¼ÔË¶¯
		Control.mStep = 0;				// ¸´Î»²½ÖèÖ¸Õë
		Control.mExeStep = 0;			// ¸´Î»Î¢²½Ö¸Õë
		Control.mStepEnable = 0;		// ¸´Î»½ûÓÃ×Ô¶¯¿ØÖÆ
		Control.mFixStep = 0;			// ¸´Î»×Ô¶¯Ð£Õý²½Êý
		
		Control.mPause		= 0;		// ¸´Î»ÔÝÍ£Æì±ê
		Control.mGoOn		= 0;		// ¸´Î»¼ÌÐøÂ·¾¶ÔË¶¯Æì±ê
		Control.mRevers		= 0;		// ¸´Î»·´ÏòÆì±ê
		
		Control.mLine		= 0;		// ¸´Î»µ¥¶Î²½ÖèÖ¸Õë
		Control.mExeLine	= 0;		// ¸´Î»µ¥¶ÎÖ´ÐÐÖ¸Õë
		Control.mLineEnable = 0XFF;		// ¿ªÊ¼Ö´ÐÐÎ¢²½³ÌÐò
		FindBarEnable		= TRUE;		// ÔÊÐí×Ô¶¯Ê¶±ðÌõÂë

		m_EmergencyBrake.SetWindowText("½ô¼±ÖÆ¶¯");
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
		FindBarEnable		= FALSE;			// ½ûÖ¹×Ô¶¯Ê¶±ðÌõÂë
		Control.mStep		= 0;				// ¸´Î»²½ÖèÖ¸Õë
		Control.mExeStep	= 0;				// ¸´Î»Î¢²½Ö¸Õë
		Control.mStepEnable = 0;				// ¸´Î»½ûÓÃ×Ô¶¯¿ØÖÆ
		Control.mFixStep = 0;					// ¸´Î»×Ô¶¯Ð£Õý²½Êý
		
		Control.mPause		= 0;				// ¸´Î»ÔÝÍ£Æì±ê
		Control.mGoOn		= 0;				// ¸´Î»¼ÌÐøÂ·¾¶ÔË¶¯Æì±ê
		Control.mRevers		= 0;				// ¸´Î»·´ÏòÆì±ê
		
		Control.mLine		= 0;				// ¸´Î»µ¥¶Î²½ÖèÖ¸Õë
		Control.mExeLine	= 0;				// ¸´Î»µ¥¶ÎÖ´ÐÐÖ¸Õë
		Control.mLineEnable = 0;				// ¸´Î»µ¥¶
	}
}
