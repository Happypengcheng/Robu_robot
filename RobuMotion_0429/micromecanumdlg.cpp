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

    UINT server_thd(LPVOID p); //�����̺߳���
	SOCKET listen_sock;//����һ��ȫ�ֵļ���soket
	SOCKET sock;//����һ��soket
	CString showeditmsg; //����ȫ�ֵ���ʾ��Ϣ���ַ���


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
	
	if (!OpenProcessToken(GetCurrentProcess(),				// OpenProcessToken() ��������������Ǵ�һ�����̵ķ�������			
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,&hToken))		// GetCurrentProcess() �����������ǵõ������̵ľ��			
		return FALSE;	
	LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);		//LookupPrivilegeValue()�������ǲ鿴ϵͳȨ�޵���Ȩֵ��������Ϣ��һ��LUID�ṹ���		
	tkp.PrivilegeCount = 1;									// one privilege to set ������������Ȩ		
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;			
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,(PTOKEN_PRIVILEGES)NULL, 0);	// AdjustTokenPrivileges������������֪ͨWindows NT�޸ı����̵�Ȩ��			
	if (GetLastError() != ERROR_SUCCESS)					//ʧ��			
		return FALSE;		
	if (!ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0))		//�������������á�ǿ���˳�WINDOWS��EWX_FORCE����			
		return FALSE;	
	return TRUE;		
}

BOOL CMicroMecanumDlg::OnInitDialog()	//�����ʼ��
{
	CDialog::OnInitDialog();

	FlagPause =0; // 0 ������ͣ״̬ 1��������״̬

	CurLineNo = 0;					// ��¼��ǰ��Step���
	IsEmgcBrake = false;			// ��¼��ǰ��Step���

m_ofsLog2.open("log-2�����ر����.txt",ios::app);
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


 	TableName = "Route";			// �趨��������
 	InitializationWindows();		// ��ʼ������	

	//------------------------------------------------
	// qi
	// ��ʼ��У׼�㣨���һ��Line�������֧��1024��
	nCheckPoints = 0;
	CheckPoints = new CHECKPOINT[1024];
	CheckMarks = new MARK[1024];

	nLineMarks = 0;
	LineMarks = new MARK[1024];

	nMarks = 0;	// ��ʼ���ر�
	m_CaliCount = 0;	// У׼������0
	ReadMarkFile();					// ��ȡ�ر��ļ� qi
	ReadSearchLines();				// ��ȡ�ر�����·���ļ� qi
	ReadConfigFile();				// ��ȡ�����ļ� qi

	m_igvInfo.curX = -10000.0;	// С����ʼδ��λ�Ͷ���
	m_igvInfo.curY = -10000.0;	// С����ʼδ��λ�Ͷ���
	m_igvInfo.preX = m_igvInfo.curX;	//  С����һ��(ǰ��������ǰ)���꣬��λ�Ǻ���
	m_igvInfo.preY = m_igvInfo.curY;	//  С����һ��(ǰ��������ǰ)���꣬��λ�Ǻ���
	m_igvInfo.isFree = true;
	m_igvInfo.Speed = 0;
	m_igvInfo.MoveDirection = -1;
	m_igvInfo.isMarked = false;
	m_igvInfo.MarkID = "";

	m_igvInfo.Compass = -1.0;	// С����ʼδ��λ�Ͷ���
	
	m_igvInfo.Heading = 270.0;	// û�а�װ�شŸ�Ӧǰ����ͷ�����ݶ�Ϊ90����Ӧʵ���ҵ�����ϵ��
	//m_Heading = -1.0;	// С����ʼδ��λ�Ͷ���
	m_HeadingCombBxo.SetCurSel(3);
	m_HeadingCombBxo.UpdateData(TRUE);

	m_NotFindMarkTimes370 = 0;	// ִ��370ָ��ʱδ����mark�Ĵ���
	m_iCurrentSearchingRoute = -1;		// ��ǰ����ִ�еĵر�����·������, -1��ʾδ���еر�����
	m_isSearchingMark = false;	// ��ִ�������ر�
	m_isSearchingMarkFound = false;	// ��δ�ѵ��ر�
	// end qi 
	//------------------------------------------------	
		
	DeknockEnable = TRUE;

	InitializationMotion();			// Initialization motion control
	InitRs232c();					// ��ʼ��RS232C
	//InitializationCamera();		// ��ʼ�����
//	CameraInitional();			// ��ʼ�����

	//------------------------------------------------
	// pc
	//���³�ʼ��ͼ��ƥ�����
	m_Pattern.Load("pattern.bmp");	// ����ģ��
	m_Roi.Attach(&m_Pattern);
	m_Roi.SetPlacement(0,0,268,44);
	m_Match.SetMaxPositions(1);// return only one position
	m_Match.SetMinScore(0.25f);	// ƥ������ٴﵽ0.25
	ESetAngleUnit(E_ANGLE_UNIT_DEGREES);
	//m_Match.SetMaxAngle(1);
	//m_Match.SetMinAngle(-1);
	//m_Match.SetInterpolate(true);
	//m_Match.SetMaxScale(2);
	//m_Match.SetMinScale(0.5);
	m_Match.LearnPattern(&m_Roi);
	
	Temp_LastBarCodeid = "";	// ��ʼ��Ϊ��
	m_isSocketConnected = false;	
	// GetDlgItem(IDC_STATIC_Connect)->SetWindowText("δ����");
	m_ConnectStatus.SetWindowText("δ����");

	InitializationSoket();	// ��ʼ��soketͬʱ�����߳̽�����Ϣ
	m_iTimeCounter = 0;		// ��������OnTimerÿִ��10�λش�һ������λ�ú�״̬��Ϣ
	
	// end pc 
	//------------------------------------------------ 

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// ��ȡ�����ļ�
void CMicroMecanumDlg::ReadConfigFile()
{
	ifstream fs;

	fs.open("config.txt", ios::nocreate);//Ĭ���� ios::in �ķ�ʽ���ļ����ļ�������ʱ����ʧ��

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
		RotateFactor = 1/2;	// ȱʡֵ
		RotateLimit = 1;		// ȷʵֵ1��
		
	}

	fs.close();
	
	//CString str;
	//str.Format("factor = %.4f, limit=%.1f", RotateFactor,RotateLimit);
	//MessageBox(str,"",MB_OK);

}

// ��ȡ�ر��ļ� qi
void CMicroMecanumDlg::ReadMarkFile()
{
	ifstream fs;

	fs.open("marks.txt", ios::nocreate);//Ĭ���� ios::in �ķ�ʽ���ļ����ļ�������ʱ����ʧ��

	if(fs)
	{
		fs>>nMarks;
		char* sline = new char[24*nMarks];	// �˴��ڴ����Ż��ռ�
		char* pline = sline;	// ��¼slineָ��λ�ã���Ϊ����slineָ����λ��
		marks = new MARK[nMarks];
		int i = 0;
		fs.getline(sline,256,'\n');//��ȡ�س�����
		while(!fs.eof() && i<nMarks)
		{
			fs.getline(sline,256,'\n');//"12345,100,0,90"
			if(strlen(sline) > 0)
			{
				char* buf = strstr(sline, ",");//",100,0,90"
				buf[0] = '\0';//��ʱsline�����"12345"
				marks[i].barCode = sline;
				
				sline = buf + 1; //"100,0,90"
				buf = strstr(sline, ",");//",0,90"
				buf[0] = '\0';//��ʱsline�����"100"
				marks[i].x = atoi(sline);

				sline = buf + 1; //"0,90"
				buf = strstr(sline, ",");//",90"
				buf[0] = '\0';//��ʱsline�����"0"
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

	// ����ӳ��
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
				str.Format("line mark count: %d��", nLineMarks);
				MessageBox(str);
*/
}

// ��������õ��ر���Ϣ
MARK* CMicroMecanumDlg::GetMarkInfo(CString barCode)
{
	return mapMarks[barCode];

}

void CMicroMecanumDlg::OnButton1() //����ֹͣ/�رյ�Դ
{
Control.mMaxLine = 0;

	CString str;
	GetDlgItem(IDC_BUTTON1)->GetWindowText(str);
	if (str == "�رյ�Դ")
	{
		m_ofsLog2.close();
		StopMotion();
		SetOutput(&m_Device,ZADDR, 0x00FF);	//P4 ~ P7����ߵ�ƽ1����������������
		SetOutput(&m_Device,UADDR, 0x00FF);	//P4 ~ P7����ߵ�ƽ1���ر���������Դ
		pProcessThread.pause();				// �߳���ͣ��������//�ر��߳�
		pProcessThread.ExitInstance();		// �˳��߳�
		GetDlgItem(IDC_BUTTON1)->SetWindowText("�򿪵�Դ");
	} 
	else
	{
		m_ofsLog2.open("log-2�����ر����.txt",ios::app);
		SetOutput(&m_Device,ZADDR, 0x000F);	//P4 ~ P7����͵�ƽ0��ʹ������������
		SetOutput(&m_Device,UADDR, 0x003F);	//P4 ~ P7����͵�ƽ0������������Դ

		Control.mLineEnable = 0;	// pc 2016.03.30

		GetDlgItem(IDC_BUTTON1)->SetWindowText("�رյ�Դ");
	}
}

void CMicroMecanumDlg::OnButton2()	//ϵͳ��λ
{
	MotionInit();
//	Init(&m_Device);
//	SetOutput(&m_Device,ZADDR, 0x000F);	//P4 ~ P7����͵�ƽ0��ʹ������������
//	SetOutput(&m_Device,UADDR, 0x000F);	//P4 ~ P7����͵�ƽ0������������Դ
}

void CMicroMecanumDlg::OnButton3()	//ʹ��˵�� 
{
	// ����
	//UpdateHeading(361,795);

	CAboutDlg Dlg;
	Dlg.DoModal();	
}

void CMicroMecanumDlg::OnButton4()	//·���༭ 
{
	CRoute Dlg; 
	Dlg.DoModal();
}

void CMicroMecanumDlg::OnButton5()	//ͨѶ�趨
{
	CCom Dlg;
	Dlg.DoModal();
}

void CMicroMecanumDlg::OnButton6()	//���긴λ(��̬У׼)  
{
// 	if ((FindBarEnable == FALSE) && (Control.mLineEnable != 0))
// 	{
// 		FindBarEnable = TRUE;	
// 	}
	Line[0].Derection = 370;
	Line[0].MaxSpeed = 500;
	Line[0].ACC = 500;
	Line[0].DACC = 500;
	if (Control.mLineEnable == 0)		// ֻ����һ�˶���ɣ����ܿ�ʼ�µ��˶������˶��У���ִ���κζ���
	{
		Control.mStep = 0;				// ��λ����ָ��
		Control.mExeStep = 0;			// ��λ΢��ָ��
		Control.mStepEnable = 0;		// ��λ�����Զ�����
		Control.mFixStep = 0;			// ��λ�Զ�У������
		
		Control.mPause		= 0;		// ��λ��ͣ���
		Control.mGoOn		= 0;		// ��λ����·���˶����
		Control.mRevers		= 0;		// ��λ�������
		
		Control.mLine		= 0;		// ��λ���β���ָ��
		Control.mExeLine	= 0;		// ��λ����ִ��ָ��
		Control.mMaxLine	= 1;		// ��λ����ִ��ָ��
		Control.mLineEnable = 0XFF;		// ��ʼִ��΢������
		FindBarEnable		= TRUE;		// �����Զ�ʶ������
	}
}

void CMicroMecanumDlg::OnButton7()	//�����趨
{
	DisFixInfo = "";
	GetDlgItem(IDC_STATIC2)->SetWindowText(DisFixInfo);

	if (DisplayImage == TRUE)		// ����һ��ͼ����ʾ����
	{
		//��ֹ��ʾͼ��
		DisplayImage = FALSE;
		GetDlgItem(IDC_BUTTON7)->SetWindowText("��ʼ����");	//�����趨 
	}
	else
	{
		//�Ѳɼ���ͼ����ʾ�ڽ�����Draw the image
		DisplayImage = TRUE;
		GetDlgItem(IDC_BUTTON7)->SetWindowText("��ֹ����");	//�����趨 
	}
}

void CMicroMecanumDlg::OnButton8()	//�Զ����� 
{
	CString str;
	GetDlgItem(IDC_BUTTON1)->GetWindowText(str);
	if (str == "�رյ�Դ")
	{
		nCheckPoints = 0;
		StartMotion();				// ��ʼ����
	}
	else
	{
		MessageBox("��������Դ��δ�򿪣���򿪵�Դ!","�����ش���",MB_OK|MB_ICONWARNING|MB_APPLMODAL);
	}
}

void CMicroMecanumDlg::OnCancel()	//�رճ���
{
	INT i = MessageBox("��ȷ�ϻ����˴��ڿ��Թرճ����״̬!","�������رտ��Ƴ���",MB_YESNO|MB_ICONWARNING|MB_APPLMODAL);	
	if (i == 6)								//ȷ�Ϲر�
	{
		//---------------qi-----------------------
		// ɾ���ر��ڴ�
		if(!marks)
		{
			delete[] marks;		
		}
		//delete marks;
		//ɾ��У׼��
		if(!CheckPoints)
			delete[] CheckPoints;
		if(!CheckMarks)
			delete[] CheckMarks;
		if(!LineMarks)
			delete[] LineMarks;

		//------------------------------------

		Stop(&m_Device,SDSTP);				// ����ֹͣ
		SetOutput(&m_Device,ZADDR, 0x00FF);	//P4 ~ P7����͵�ƽ0����������������
		SetOutput(&m_Device,UADDR, 0x00FF);	//P4 ~ P7����ߵ�ƽ1���ر���������Դ
		if(!DeviceClose(&m_Device))			//�ر��˶����ƿ�
		{
			MessageBox("�˶����ƿ��ر�ʧ�ܣ�Close fail!");		
		}
		ZjCom.SetPortOpen(0);				// �رմ��� 
		KillTimer(1);						// �رն�ʱ��
		Sleep(200);							// �ȴ�100ms
		pProcessThread.pause();				// �߳���ͣ��������
		pProcessThread.ExitInstance();		// �˳��߳�
		CameraFree(0);						// �ر����
		CDialog::OnCancel();
	}
}

void CMicroMecanumDlg::OnButton9()	//�ر�ϵͳ 
{
	INT i = MessageBox("��ȷ�ϻ����˴��ڿ��Թرյ�Դ��״̬!","�������жϻ����˵�Դ��",MB_YESNO|MB_ICONWARNING|MB_APPLMODAL);	
	if (i == 6)								//ȷ�Ϲر�
	{
		Stop(&m_Device,SDSTP);				// ����ֹͣ
		SetOutput(&m_Device,ZADDR, 0x00FF);	//P4 ~ P7����͵�ƽ0����������������
		SetOutput(&m_Device,UADDR, 0x00FF);	//P4 ~ P7����ߵ�ƽ1���ر���������Դ
		if(!DeviceClose(&m_Device))			//�ر��˶����ƿ�
		{
			MessageBox("�˶����ƿ��ر�ʧ�ܣ�Close fail!");		
		}
		pProcessThread.pause();				// �߳���ͣ��������
		pProcessThread.ExitInstance();		// �˳��߳�//lipan_�޸��˹رճ���Ĳ���˳��
		ZjCom.SetPortOpen(0);				// �رմ���
		KillTimer(1);						// �رն�ʱ��
		Sleep(200);							// �ȴ�100ms
		CameraFree(0);						// �ر����
		CDialog::OnCancel();				// �رտ��ƽ���
		SystemShutdown();					// �رյ���

	}	
}
//system control--------------------------------------------------------------------------------------

#define DataBaseBlock 1
//ADO LIST 4 --------------------------------------------------------------------------------------
BOOL CMicroMecanumDlg::InitializationWindows(void)	// init windows
{	
	//������ָ��cpu��
	SetProcessAffinityMask(GetCurrentProcess(), 8);	
	
	// ȷ��ϵͳ��Ļ����
	int W = GetSystemMetrics(SM_CXSCREEN);
	int H = GetSystemMetrics(SM_CYSCREEN);	
	MoveWindow(0,0,W,H-50);
	TRACE("ϵͳĬ�Ϸֱ���%d*%d!\n",W,H);
	// ȷ�Ϲ�������
	CRect windows;
	GetClientRect(&windows);
	int x = windows.Width();
	int y = windows.Height();
	TRACE("��ʾ����%d*%d!\n",x,y);
	// ���ð���		
	CRect rect;
	int bw = x / 20;
	int bh = y / 20;
	rect.left	= 19 * bw;
	rect.top	= 0;
	rect.right	= rect.left + bw;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON1)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON1)->SetWindowText("�رյ�Դ");		//�رյ�Դ "����ֹͣ"
	rect.top	= 1*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON2)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON2)->SetWindowText("ϵͳ��λ");		//�������� ϵͳ��λ
	rect.top	= 2*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON3)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON3)->SetWindowText("ʹ��˵��");
	rect.top	= 3*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON4)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON4)->SetWindowText("·���༭");
	rect.top	= 4*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON5)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON5)->SetWindowText("ͨѶ�趨");
	rect.top	= 5*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON6)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON6)->SetWindowText("�Զ���λ");	//���긴λ �����ʾ
	rect.top	= 6*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON7)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON7)->SetWindowText("��ֹ����");	//�����趨 
	rect.top	= 7*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON8)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON8)->SetWindowText("��ʼ�˶�");	//��ʼ�˶� �Զ�����
	rect.top	= 8*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDCANCEL)->MoveWindow(&rect);	
	GetDlgItem(IDCANCEL)->SetWindowText("�رճ���");
	rect.top	= 9*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON9)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON9)->SetWindowText("�ر�ϵͳ");
	TRACE("��������%d*%d!\n",rect.left,rect.right);
	// ����Editbox
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
// 	// ����list control
// 	bw = 19 * bw / 16;
// 	bh = y / 40;
// 	rect.left	= 0;
// 	rect.top	= y - bh-10;
// 	rect.right	= x;
// 	rect.bottom = y - 10;
// 	GetDlgItem(IDC_LIST2)->MoveWindow(&rect);
// 	TRACE("�б���%d*%d!\n",rect.right,rect.bottom);

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


	// ���þ�̬�ı���ʾ����2
	rect.left	= 0;
	rect.top	= y - 6*bh;
	rect.right	= x;
	rect.bottom = y - 4*bh;
	GetDlgItem(IDC_STATIC2)->MoveWindow(&rect);
	// ���þ�̬�ı���ʾ����1
	rect.left	= 0;
	rect.top	= y - 4*bh;
	rect.right	= x;
	rect.bottom = y - 2*bh;
	GetDlgItem(IDC_STATIC1)->MoveWindow(&rect);

	// ����С����ͷ����
	m_HeadingCombBxo.AddString("0");
	m_HeadingCombBxo.AddString("90");
	m_HeadingCombBxo.AddString("180");
	m_HeadingCombBxo.AddString("270");
	
	return TRUE;
}
void CMicroMecanumDlg::InitADOConn(void)	//����ADO����
{
	try
	{
		//�������Ӷ���ʵ��
		m_pConnection.CreateInstance("ADODB.Connection");
		//���������ַ���
		CString strConnect="DRIVER={Microsoft Access Driver (*.mdb)};uid=;pwd=;DBQ=iRoute.mdb;";
		//ʹ��Open�����������ݿ�
		m_pConnection->Open((_bstr_t)strConnect,"","",adModeUnknown);
	}
	catch(_com_error e)
	{
		//������ʾ����
		AfxMessageBox(e.Description());
	}
}

void CMicroMecanumDlg::ExitConnect(void)	//�رռ�¼��������
{
	//�رռ�¼��������
    if(m_pRecordset!=NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
}

void CMicroMecanumDlg::Test(void)	// �����ݿ�����תΪ·������
{

}
void CMicroMecanumDlg::Database2Route(void)	// �����ݿ�����תΪ·������
{
	//�������ݿ��׼�����ƺ�����
	CString	StdData[MovementParament]={"1","0","500","0","1000","1000","1000","0","400","1","2","3","4","100","200","300"};
	CString StdName[MovementParament]={"StepID","CommandID","AidDistance","StartSpeed","MaxSpeed","ACC","DACC","CaliDistance","MictoDistance","PortX","PortY","PortZ","PortU","Data1","Data2","Data3",};	
	//�������ݿ�
	InitADOConn();
	// ����һ���ַ���
	CString str;
	//��¼������StepID����������
	str.Format("select * from %s order by StepID ",TableName);
	_bstr_t bstrSQL = str;
	//������¼��ָ�����ʵ��
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	//�򿪼�¼��
	m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);	
	//��λ·������ָ��
	Control.mMaxLine = 0;	
	//����¼��ָ���ƶ�����һ����¼
	m_pRecordset->MoveFirst();
	//��ʾ������Ϣ
	//TRACE("\n"); 
	//TRACE("��ʼ��ȡ·������!\n"); 
	//��ȡ���ݿ����ݲ���ʾ���б�ؼ���
	while(!m_pRecordset->adoEOF)
	{
		// 1.�˶�����
		Line[Control.mMaxLine].Derection	= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("CommandID"));	
		// 2.Ŀ�����
		Line[Control.mMaxLine].AidDistance	= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("AidDistance"));	
		// 3.�˶�����
		Line[Control.mMaxLine].MotionType	= LineRips;
		// 4.�ٶȱ���
		Line[Control.mMaxLine].SpeedRate	= sRate;
		// 5.�����ٶ�
		Line[Control.mMaxLine].StartSpeed	= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("StartSpeed"));
		// 6.����ٶ�
		Line[Control.mMaxLine].MaxSpeed		= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("MaxSpeed"));
		// 7.�Ӽ��ٶ�
		Line[Control.mMaxLine].ACC			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("ACC"));	
		// 8.�����ٶ�
		Line[Control.mMaxLine].DACC			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("DACC"));
		// 9.���پ���
		Line[Control.mMaxLine].ReducePoint	= 0;
		// 10.��������
		Line[Control.mMaxLine].StartType	= STAUD;
		// 11.У������
		Line[Control.mMaxLine].CaliDistance	= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("CaliDistance"));
		// 12.΢������	
		Line[Control.mMaxLine].MicroDistance =  atoi((char*)(_bstr_t)m_pRecordset->GetCollect("MictoDistance"));
		// 13.X����Ŀ��
		Line[Control.mMaxLine].AidX			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("PortX"));
		// 14.Y����Ŀ��
		Line[Control.mMaxLine].AidY			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("PortY"));
		// 15.Z����Ŀ��
		Line[Control.mMaxLine].AidZ			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("PortZ"));
		// 16.U����Ŀ��	
		Line[Control.mMaxLine].AidU			= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("PortU"));	
		// ���������Ϣ
		//TRACE("��%d#·������ת�����!\n",Control.mMaxLine); 
		// ��ʾ��ȡ���˶�����
		//DisplayPar(&Line[Control.mMaxLine]);		
		//����������
		Control.mMaxLine++;
		//����¼��ָ���ƶ�����һ����¼
		m_pRecordset->MoveNext();
	}
	//�Ͽ����ݿ�����
	ExitConnect();
	// ���������Ϣ
	//TRACE("·���ܼ�����%d��·��!\n",Control.mMaxLine); 
	//TRACE("\n"); 

 }
//ADO LIST 4 --------------------------------------------------------------------------------------

#define MotionBlock	2 
// mecanum С���˶�����ģ�� ---------------------------------------------------
CString CMicroMecanumDlg::DisplayPar(MotionParemeter* line)
{
	CString str;
// 	str.Format("����%d,����%d,�˶�����0X%X,�ٶȱ���%d,�����ٶ�%d,����ٶ�%d,�Ӽ��ٶ�%d,�����ٶ�%d,���ٵ�%d,��������0X%X,У������%d,΢������%d,X%d,Y%d,Z%d,U%d,\n",
// 		line->Derection,line->AidDistance,line->MotionType,line->SpeedRate,
// 		line->StartSpeed,line->MaxSpeed,line->ACC,line->DACC,
// 		line->ReducePoint,line->StartType,line->CaliDistance,line->MicroDistance,
// 		line->AidX,line->AidY,line->AidZ,line->AidU);
	str.Format("%d,%d,�˶�����0X%X,%d,%d,%d,%d,%d,���ٵ�%d,��������0X%X,%d,΢������%d,X%d,Y%d,Z%d,U%d,\n",
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
	// ���ӷ���˷���
	pConnectSocketThread.CreateThread();
	pConnectSocketThread.play();
}

BOOL CMicroMecanumDlg::InitializationMotion(void)
{
	// ��ʼ���˶�����ģ��
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
		MessageBox("û�з����˶����ƿ����޷������˶����ƹ���!",
			"�����ش���",
			MB_OK|MB_ICONWARNING|MB_APPLMODAL);		
	}	
//	Init(&m_Device);
//	SetOutput(&m_Device,ZADDR, 0x000F);			//P4 ~ P7����͵�ƽ0��ʹ������������
//	SetOutput(&m_Device,UADDR, 0x000F);			//P4 ~ P7����͵�ƽ0������������Դ
	MotionInit();
	//InitialPar(&Control,Line,Step,CaliMap);		// ��ʼ���켣���Ʋ���
	InitialPar(&Control,Line,Step,CaliMap,pPort,&VportPar);		// ��ʼ���켣���Ʋ���

	Control.PlusUnit = Puls2Distance;
	// �����˶�����ר�ö�ʱ�� 	
	SetTimer(1, 50, NULL);						// ��ʼ�����ö�ʱ��	
	return TRUE;
}

void CMicroMecanumDlg::MotionInit()	//�˶�ϵͳ��λ
{
	Init(&m_Device);
	SetOutput(&m_Device,ZADDR, 0x000F);	//P4 ~ P7����͵�ƽ0��ʹ������������
	SetOutput(&m_Device,UADDR, 0x003F);	//P4 ~ P7����͵�ƽ0������������Դ
	//return TRUE;
}

inline	BOOL CMicroMecanumDlg::Display(void)			// �˶�״̬��ʾ
{
	// ��ʱ��̬����
	static U32 SecondDis,MotionTime,LastStep;
	// �ϳ���ʾ�þֲ�����
	CString str,tmp;
	// �Զ�������ʱ��U32 Լ���Լ�ʱһ����// SecondDis = 0;
	SecondDis++;
	// ÿ�����
	if ((SecondDis%10) == 0)					
	{
		// ��ʼʱ���
		DWORD SartTime =  GetCurrentTime();			
		// �����˶���������Ϣ
		if (Control.mLineEnable == 0)// �����˶�ʱ
		{
			// ��λ�˶�ʱ��
			MotionTime = 0;
			// ��ͷ����
			mARROW = 360;
			// ����˶���Ϣ
			str = "�˶�ֹͣ!\n";
		}
		else// ���˶�ʱ
		{	
			// ͳ���˶�ʱ��
			MotionTime++;
			// ��ͷ����
			mARROW = Line[Control.mExeLine].Derection;
			//// �ϳɿ����˶�����
			str = DisplayPar(&Line[Control.mExeLine]);		
		}

		if (CameraReady != TRUE)		// û�������ֱ����ʾ
		{
			// �滭��ͷ
			DrawArrow(mARROW);
			//TRACE("CameraReady=%d,�ö�ʱ����ʾͼ��!\n",CameraReady);
		}
		// ����λ��
		INT32 xLocation = INT32(DOUBLE(GetShiftR(&m_Device,XADDR)) * Puls2Distance);
		// ��������
		INT32 xSpeed = INT32(DOUBLE(GetSpeedR(&m_Device,XADDR)) * Puls2Distance);

		// �ϳɿ�������
		tmp.Format(_T("x=%d����,y=%d���ף��ٶ�%d����/��,U��˿�0X%X,"), 
			(int)m_igvInfo.curX,(int)m_igvInfo.curY,xSpeed,GetInput(&m_Device,UADDR));
		// �ϳ���ʾ����
		str = tmp + str;
		SystemInfor = tmp;
		// ������ʾ����
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);
	}	
	return TRUE;
}
BOOL CMicroMecanumDlg::StartMotion(void)			// ��ʼ�˶�
{
	Database2Route();					// �����ݿ⵼��·������
	if (Control.mLineEnable == 0)		// ֻ����һ�˶���ɣ����ܿ�ʼ�µ��˶������˶��У���ִ���κζ���
	{
		Control.mStep = 0;				// ��λ����ָ��
		Control.mExeStep = 0;			// ��λ΢��ָ��
		Control.mStepEnable = 0;		// ��λ�����Զ�����
		Control.mFixStep = 0;			// ��λ�Զ�У������
		
		Control.mPause		= 0;		// ��λ��ͣ���
		Control.mGoOn		= 0;		// ��λ����·���˶����
		Control.mRevers		= 0;		// ��λ�������
		
		Control.mLine		= 0;		// ��λ���β���ָ��
		Control.mExeLine	= 0;		// ��λ����ִ��ָ��
		Control.mLineEnable = 0XFF;		// ��ʼִ��΢������
		FindBarEnable		= TRUE;		// �����Զ�ʶ������
	}
	return TRUE;
}
BOOL CMicroMecanumDlg::StopMotion(void)			// ֹͣ�˶�
{
	if (Control.mLineEnable != 0)
	{
		FindBarEnable		= FALSE;			// ��ֹ�Զ�ʶ������
		Control.mStep		= 0;				// ��λ����ָ��
		Control.mExeStep	= 0;				// ��λ΢��ָ��
		Control.mStepEnable = 0;				// ��λ�����Զ�����
		Control.mFixStep = 0;					// ��λ�Զ�У������
		
		Control.mPause		= 0;				// ��λ��ͣ���
		Control.mGoOn		= 0;				// ��λ����·���˶����
		Control.mRevers		= 0;				// ��λ�������
		
		Control.mLine		= 0;				// ��λ���β���ָ��
		Control.mExeLine	= 0;				// ��λ����ִ��ָ��
		Control.mLineEnable = 0;				// ��λ�������
		Stop(&m_Device,STOP);					// STOP / SDSTP
	}
	return TRUE;
}

void CMicroMecanumDlg::OnTimer(UINT nIDEvent)	//�˶�����ר�ö�ʱ��  
{
	// ��������	
	INT32 xLocation = INT32(DOUBLE(GetShiftR(&m_Device,XADDR)) * Puls2Distance);	// ����λ��
	UpdateOwnPosition(abs(xLocation));	// ����С����ǰ����
	ValidatePosition();					// ���С��λ���Ƿ�ƫ��ر�̫Զ
	// ����С���ٶ�
	INT32 xSpeed = INT32(DOUBLE(GetSpeedR(&m_Device,XADDR)) * Puls2Distance);
	m_igvInfo.Speed = xSpeed;


	if(Control.mLineEnable == 0)
		m_igvInfo.isFree = true;
	else 
	{
		m_igvInfo.MoveDirection = Line[Control.mExeLine].Derection;
		m_igvInfo.isFree = false;

		if (Line[Control.mExeLine].Derection != 370/*У׼ָ��*/ && Line[Control.mExeLine].Derection !=360/*�ȴ�ָ��*/)
		{
			// ֻҪ���ӷ������ƶ����ͽ�״̬��Ϊ��δУ׼��
			m_igvInfo.isMarked = false;
			m_igvInfo.MarkID = "";
ofstream ofsLog;				
ofsLog.open("log-4 isFree״̬����.txt",ios::app);
ofsLog<<"�ƶ���,derection="<<Line[Control.mExeLine].Derection<<"mLine="<<Control.mLine<<"\n";
ofsLog.close();

//m_ofsLog2<<"�ƶ���,derection="<<Line[Control.mExeLine].Derection<<"mLine="<<Control.mLine<<"\n";

		}
	}

	// ÿ��1���������˷��������λ�ú�״̬��Ϣ
	m_iTimeCounter ++;
	if (m_iTimeCounter == 10)
	{
		m_iTimeCounter = 0;
		SendIGVRealTimeInfo();
	}

	// ���´������ڵ��Զ�����տ�ʼʱ����鵱ǰ���Ƿ������ģ�����������ĵر꣬
	// �����һ��������ҵ��ƶ���ʹ����׼���ĵر�
	if (Control.mLineEnable != 0	&& Control.mLine <= 20 
		&& Line[Control.mExeLine].Derection == 370)	// ��ʼִ��΢������)
	{
		Control.mPause = 0XFF;	// ������ͣ
		Control.mGoOn = 0XFF;	// ��ֹ����

		// �����������ֲ�ִ��
		Display();									
		CDialog::OnTimer(nIDEvent);
		return;
	}

	// ���ĳ��Line�Ķ��������
	if (Control.mStepEnable == 0 && Control.mLine == 50)	
	{
		// �����ת������ת��ɣ����޸ĳ�ͷ����m_Heading
		if (Line[Control.mExeLine].Derection == 361 || Line[Control.mExeLine].Derection == 362)	// ���ִ��ת��
		{
			// ���³�ͷ����
			UpdateHeading(Line[Control.mExeLine].Derection, 
				int(Line[Control.mExeLine].AidDistance*Puls2Distance+0.5));	// ȡ��
		}
		// ������������ر꣬����ѵ���û�ѵ������������
		else if ((Line[Control.mExeLine].Derection == 0 || Line[Control.mExeLine].Derection == 90
			|| Line[Control.mExeLine].Derection == 180 || Line[Control.mExeLine].Derection == 270)
			&& m_iCurrentSearchingRoute >= 0)	// ע������û����m_isSearchingMark���жϣ�
										// ��Ϊ��OnTimer()��Bardecoder()������̫ͬ�����������߳�������
		{
m_ofsLog2<<"ontimer��ִ�еر�����·���У�m_iCurrentSearchingRoute="<<m_iCurrentSearchingRoute<<"\n";
			// ����ѵ��˵ر�
			if (m_isSearchingMarkFound && m_iCurrentSearchingRoute >= 0)
			{
m_ofsLog2<<"ontimer��ִ�еر�����·�����ѵ��ر��ˣ�׼��ִ���˻صر궯��\n";
				// ��øո�ִ�е�����·���Ĳ���
				int dir = Line[Control.mExeLine].Derection;
				int dis = int(Line[Control.mExeLine].AidDistance * Puls2Distance + 0.5);
				// ����ǰִ�е�Lineָ�����һ��SearchLine
				Control.mExeLine += m_iCountOfSearchRoutes - 1 - m_iCurrentSearchingRoute;
				// �޸����һ������·������С���˻ص��õر꣬��ԭ����ǰ��0�����ɺ���180,��90����270...
				int dir_reverse = 180+dir >= 360?dir-180:180+dir;
				MotionParemeter mp = GetMotionParameters(dir_reverse, dis-m_markFoundLocation);
				Line[Control.mExeLine] = mp;

m_ofsLog2<<"ontimer�˻ض�����exeLine="<<Control.mExeLine<<",dir="<<dir_reverse
        <<",dis="<<dis-m_markFoundLocation<<",mMaxLine="<<Control.mMaxLine<<"\n";
for(int l = 0; l < Control.mMaxLine; l++)
	m_ofsLog2<<"line "<<l<<",dir="<<Line[l].Derection<<",dis="<<Line[l].AidDistance<<"\n";
m_ofsLog2<<"--------------------\n";
				
				// ����ִ��
				Control.mPause = 0;	// 
				Control.mGoOn = 0;	// 
				Control.mLine = 30;

				m_isSearchingMark = false;			// ��ִ�������ر�
				m_isSearchingMarkFound = false;		// ����Ϊδ�������ر�
				m_iCurrentSearchingRoute = -1;				// ����Ϊ-1

			}
			else if (m_iCurrentSearchingRoute >= 0)
			{
m_ofsLog2<<"��"<<m_iCurrentSearchingRoute<<"��·�������ˣ���û�ҵ��ر�\n";
				// �����һ������·����������1
				m_iCurrentSearchingRoute++; 

				if (m_iCurrentSearchingRoute >= m_iCountOfSearchRoutes) // ��������·����������
				{
m_ofsLog2<<"��������·���������ˣ�û�ҵ��ر�\n";
					// ��ʱӦ����ʾ�������쳣
					;
					// С����ͣ��ʹ�䴦��ִ��370����ͣ��״̬
					Control.mPause = 0XFF;	// ������ͣ
					Control.mGoOn = 0XFF;	// ��ֹ����
					Control.mExeLine++;		// ָ��·�����������370ָ�һ����һ��������ָ�
					Control.mLine = 60;		// ֱ����������Ƿ���ͣ�Ĳ���

					//Control.mLineEnable = 0;			// ����У׼������ִ����
					
					m_isSearchingMark = false;			// ��ִ�������ر�
					m_isSearchingMarkFound = false;		// ����Ϊδ�������ر�
					m_iCurrentSearchingRoute = -1;				// ����Ϊ-1

					m_NotFindMarkTimes370 = 0;			// ����ִ�о�̬У׼ʱδ�������ر�Ĵ���
					m_CaliCount = 0;					// У׼��������

					// �����������ֲ�ִ��
					//Display();									
					//CDialog::OnTimer(nIDEvent);
					//return;
				}
			}
		}
	}

	// ���´��������ھ�̬У׼���ʱ��ͦ���ȴ�BarDecoder()������
	if (Control.mLineEnable != 0)	// ���Line�����еĶ��������Զ����û�����
	{
		if (Control.mStepEnable == 0 && Control.mLine == 50)	// ���ĳ��Line�Ķ��������
		{
			//CString strTemp;
			//strTemp.Format("��ǰmExeLine:%d,�䷽��%d",Control.mExeLine, Line[Control.mExeLine].Derection);
			//MessageBox(strTemp, "", MB_OK);

			// ������Զ�����ָ������ˣ�����mRouteGo()��ͣ���ȴ�BarDecoder()���뺯��������
			if (Line[Control.mExeLine].Derection == 370)
			{
				Control.mPause = 0XFF;	// ������ͣ
				Control.mGoOn = 0XFF;	// ��ֹ����
			}
		}
	}

	// ��ʼʱ���
	///DWORD SartTime =  GetCurrentTime();	
	// mRouteGo(&m_Device,&Control,Line,Step,CaliMap);	// �Զ�ִ��·������
	mRouteGo(&m_Device,&Control,Line,Step,CaliMap,pPort,&VportPar);	// �Զ�ִ��·������

	// Deknock(); //	��ײ���

	Display();									// �Զ�ͨѶ����ʾ��������
	CDialog::OnTimer(nIDEvent);
}
	
//mecanum С���˶�����ģ�� ---------------------------------------------------

#define RS232cBlock	3 
// ����ͨѶģ�� -------------------------------------------------------------------------------------------------------
BEGIN_EVENTSINK_MAP(CMicroMecanumDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMicroMecanumDlg)
	ON_EVENT(CMicroMecanumDlg, IDC_MSCOMM1, 1 /* OnComm */, OnOnCommMscomm1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
void CMicroMecanumDlg::InitRs232c(void)	//��ʼ������ 
{
	//�������ò���
	if(!ZjCom.Create(NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,IDC_MSCOMM1))  
	{  
		AfxMessageBox("����MSComm�ؼ�ʧ��!"); 
	}  
	else									//�����ɹ���ִ�г�ʼ�����򿪴��ڲ���  
	{  
		//ZjCom.SetCommPort(1);				//ָ�����ں�		
		ZjCom.SetCommPort(8);				//ָ�����ں� 		
		//ZjCom.SetSettings("9600,N,8,1");	//ͨ�Ų�������
		//ZjCom.SetSettings("19200,N,8,1");	//ͨ�Ų�������		
		ZjCom.SetSettings("115200,N,8,1");//ͨ�Ų������� 		
		ZjCom.SetInBufferSize(1024);		//ָ�����ջ�������С  
		ZjCom.SetInBufferCount(0);			//��ս��ջ�����  
		ZjCom.SetInputMode(1);				//�������ݻ�ȡ��ʽ  
		ZjCom.SetInputLen(0);				//����ÿ�ζ�ȡ���ȣ�0Ϊȫ����ȡ  
		ZjCom.SetRThreshold(16);			//���ý���OnComm�¼��ַ�����ֵ���յ�2�ֽ�ʱ�򴥷��¼�  
		ZjCom.SetPortOpen(1);				//�򿪴��� 
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
	BYTE rxdata[1024]; //����BYTE���� An 8-bit integerthat is not signed.
	CString str, strtemp, StrDisp;
		
	switch(ZjCom.GetCommEvent())
	{ 
	case 1: // comEvSend��������
		TRACE("�����˴��ڷ����ж�");
		break;
		
	case 2: // comEvReceive��ȡ����
		//MessageBox(_T("��ȡ�����¼�"), _T("TRACE"), MB_OK);
		variant_inp=ZjCom.GetInput();		//��������
		safearray_inp=variant_inp;			//VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���
		len=safearray_inp.GetOneDimSize();	//�õ���Ч���ݳ���
		if (len>=4)
		{
			strtemp="";
			for(k=0; k<len; k++)
			{
				safearray_inp.GetElement(&k,rxdata+k);	//ColeSafeArray�ͱ���ת��ΪBYTE������
				BYTE bt=*(rxdata+k);					//
				strtemp.Format("0X%X ",bt);				//���ַ�������ʱ����strtemp���
				StrDisp+=strtemp;						//
				//StrDisp += bt;						//
			}			
			//MessageBox(_T(StrDisp), _T("TRACE"), MB_OK);
		}
		if(rxdata[0] == 0X55 && rxdata[15] == 0XAA)		//����֡ͷ��֡β��ȷ����ʼ��������
		{
			StrDisp+="\n";
			GetDlgItem(IDC_STATIC2)->SetWindowText(StrDisp);// ��ʾ��������
		}
		break;
		
	default: // �����¼�����
		ZjCom.SetOutBufferCount(0);
		break;
	}	
}
// ����ͨѶģ�� -------------------------------------------------------------------------------------------------------

#define CammeraBlock	4 
// ���ģ�� -------------------------------------------------------------------------------------------------------
// Evision 4 ------------------------------------------------------------------
// ��*Dlg.CPP�У����Evision������ʶ����
 //��ʼ�����������Evision���������ѷ�����2015-12-30 qi
 //��ʼ�����������Evision�������������ã�2016-03-30 pc
void CMicroMecanumDlg::CameraInitional()  
{
	DisplayImage = TRUE;						// ͼ����ʾ���أ�Ĭ����ʾͼ��
	ImageProcessFrequence = 0;					// ͼ������������ʼ�� 
	FindBarEnable = FALSE;

	ESetJpegQuality(100);								// ����ͼ����������
	ESetTraceMode(E_TRACE_SILENT);						//	ESetTraceMode(E_TRACE_DISPLAY_FULL_MESSAGE);
	ESetAngleUnit(E_ANGLE_UNIT_DEGREES);				// Be sure to work in degrees
	
	// m_BarCode��������
	m_BarCode.SetStandardSymbologies(BRC_EAN_128);		// Select all standard symbologies	
	m_BarCode.SetKnownLocation(FALSE);					// �趨��֪������λ�á�ȫ��Ѱ������

ESetJpegQuality(90);	//2016.03.31 pc
	
	sprintf(m_pszDecodedText, "\0");					// String initialization
	///ImageNull.SetSize(640, 480);						// �趨Ĭ��ͼ��ֱ���

	BarDecodeInfo = "׼������";
//	DecodeTotalTime = 0;
	DecodeSuccesTime = 0;
	DecodeFaileTime = 0;
	TotalDispFrame = 0;
//	FinishedBarDecode = TRUE;							// ��ɽ���
	EnableDecoderBar = TRUE;							// �������

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
	CameraGetCount(&CamAllNum);							//��ȡ�������
	if (CamAllNum>0)
	{
		CString DisStr;
		DisStr.Format("ϵͳ��%d�����! \n",CamAllNum);
		TRACE(DisStr);
		
		char *name=new char[255];
		char *model=new char[255];
		NameComBox=(CComboBox*)GetDlgItem(IDC_MULTICAM);	//����������������б��
		if(CamAllNum>0)
		{
			for(int i=0; i<CamAllNum; i++)
			{
				CameraGetName(i, name, model);				//���λ�ȡ������ơ��ͺ�
				CString t_model;
				t_model.Format("%s %d#",model,i);			//t_model.Format("%s %d#",name,i);
				NameComBox->InsertString(i,_T(t_model));	//�����б�
			}
			NameComBox->SetCurSel(0);						//ѡ��0#���
		}													//������б����ʾ�����		
		
		m_index = NameComBox->GetCurSel();					//ȡ��ѡ�е�����
		CameraInit(m_index);								//��ʼ��0#���init camera
		pProcessThread.setIndex(m_index);					//�趨�̶߳�ȡ0#�������				
		CameraLoadParameter(m_index,0);						//����0#������������
		ResoComBox=(CComboBox*)GetDlgItem(IDC_RESOLUTION);	//������������ֱ����б��
		ResoComBox->ResetContent();							//����ϴ�ѡ��ķֱ���	
		
		int index=0,width=0,height=0, camera_count=0;
		CameraGetResolutionCount(m_index,&camera_count);	//��ȡ����ֱ�������
		for(int i=0;i<camera_count;i++)
		{
			CameraGetResolution( m_index,i,&width, &height);//��ȡ��N#�����N��ֱ���
			CString t_reso;
			t_reso.Format("%d*%d",width,height);
			ResoComBox->InsertString(i, _T(t_reso)); 		//�����б�
		}
		ResoComBox->SetCurSel(1);							//����ѡ�е���
		DisStr.Format("����%d��ֱ���! \n",camera_count);
		TRACE(DisStr);

		// ���õ�һ��Ĭ������640��480�ֱ���
		int width1=0,height1=0;
		CameraSetResolution(m_index,1,&width1, &height1);
		
		CameraSetOption(m_index,CAMERA_IMAGE_GRAY8);	// ��Ϊ��8λ�Ҷ�ͼ

		// ���֡ɨ���ӳ�ʱ���趨��2000=18HZ��640=30HZ
		//CameraSetDelay(m_index,10);
		//CameraSetDelay(m_index,640);		// 31HZ						
		CameraSetDelay(m_index,2000);		// 18HZ	
		delete []name; 
		delete []model; 

		//����ֱ����趨
		int Group=1,sWidth,sHigh;
		CameraGetResolution(m_index,Group,&sWidth,&sHigh);
		DisStr.Format("%d#�������%d��W%dXH%d!\n",m_index,Group,sWidth,sHigh);
		TRACE(DisStr);

		// ��������趨

		// 1.٤��У�������趨!!!
		double gamma;
		CameraGetGamma(m_index,&gamma);
		DisStr.Format("٤��У������%d!\n",(int)gamma*100);
		TRACE(DisStr);
		// 2.ͼ��Աȶ��趨!!!
		double contrast;
		CameraGetContrast(m_index,&contrast);
		DisStr.Format("ͼ��Աȶ�%d!\n",(int)contrast*100);
		TRACE(DisStr);
		// 3.ɫ�ʱ��Ͷ��趨���ڰ���������壩

		// 4.�Զ�����ʹ���趨


		// 5.ͼ�����棨���ȣ�ֵ�趨
		int gain;
		CameraGetGain(m_index,&gain);
		DisStr.Format("ͼ������%d!\n",gain);
		TRACE(DisStr);		
		// 6.�Զ�����ʹ���趨
		
		// 7.�����ع�ʱ���趨!!!
		int exposure;
		CameraGetExposure(m_index,&exposure);
		DisStr.Format("�ع�ʱ��%d!\n",exposure);
		TRACE(DisStr);
 		// 8.ͼ��ڵ�ƽ��ֵ�趨
		int blacklevel;
		CameraGetBlackLevel(m_index,&blacklevel);
		DisStr.Format("�ڵ�ƽ��ֵ%d!\n",blacklevel);
		TRACE(DisStr);
		// 9.�Զ��ع�Ŀ��ֵ�趨 	
		int AETarget;
		CameraGetAETarget(m_index,&AETarget);
		DisStr.Format("�Զ��ع�Ŀ��ֵ%d!\n",AETarget);
		TRACE(DisStr);						
		// 10.Xˮƽ�����趨	
		bool flag_x;
		CameraGetMirrorX(m_index,&flag_x);
		DisStr.Format("Xˮƽ����%d!\n",flag_x);
		TRACE(DisStr);
		// 11.Y��ֱ�����趨	
		bool flag_y;
		CameraGetMirrorY(m_index,&flag_y);
		DisStr.Format("Y��ֱ����%d!\n",flag_y);
		TRACE(DisStr);
		// 12.���֡ɨ���ӳ�ʱ���趨
		int delay;
		CameraGetDelay(m_index,&delay);
		DisStr.Format("֡ɨ���ӳ�%d!\n",delay);
		TRACE(DisStr);
		// �������׼��������־
		CameraReady = TRUE;
		// ���ö���������߳�create thread
		pProcessThread.CreateThread();
		// ��ʼ����ͼ��		
		pProcessThread.play();
		TRACE("CameraReady=%d,���߳���ʾͼ��!\n",CameraReady);
		//�߳���ָ��cpu��
		SetThreadAffinityMask(GetCurrentThread(),0X00000008);
	}
	else
	{
		CameraReady = FALSE;								// û�����
		MessageBox("û�з���������޷����������λ����!",
			"�����ش���",
			MB_OK|MB_ICONWARNING|MB_APPLMODAL);
	}
}

void CMicroMecanumDlg::InitializationCamera()   //��ʼ���������new
{
	m_BarCode.SetStandardSymbologies(BRC_EAN_128);		// ��ȡEAN128��	
	m_BarCode.SetKnownLocation(FALSE);					// �趨��֪������λ�á�ȫ��Ѱ������

	//-----------------------------------------------------------------------------
	DisplayImage = TRUE;						// ͼ����ʾ���أ�Ĭ����ʾͼ��
	ImageProcessFrequence = 0;					// ͼ������������ʼ�� 
	FindBarEnable = FALSE;

	ESetJpegQuality(100);								// ����ͼ����������
	ESetTraceMode(E_TRACE_SILENT);						//	ESetTraceMode(E_TRACE_DISPLAY_FULL_MESSAGE);
	ESetAngleUnit(E_ANGLE_UNIT_DEGREES);				// Be sure to work in degrees
	sprintf(m_pszDecodedText, "\0");					// String initialization
	///ImageNull.SetSize(640, 480);						// �趨Ĭ��ͼ��ֱ���

	BarDecodeInfo = "׼������";
//	DecodeTotalTime = 0;
	DecodeSuccesTime = 0;
	DecodeFaileTime = 0;
	TotalDispFrame = 0;
//	FinishedBarDecode = TRUE;							// ��ɽ���
	EnableDecoderBar = TRUE;							// �������

	m_saveFlag=false;
	m_saveBMPFlag=true;
	m_saveJPGFlag=false;

	int CamAllNum=0;
	CameraGetCount(&CamAllNum);							//��ȡ�������
	if (CamAllNum>0)
	{

		int index = 0; //camera index
		//int group = 2; //output group (320*240)
		int group = 1; //output group (640*480)
		CameraInit(index);								//��ʼ��0#���init camera
//		pProcessThread.setIndex(m_index);					//�趨�̶߳�ȡ0#�������				
		CameraLoadParameter(index,0);						//����0#������������
		
		int width1=0,height1=0;
		CameraSetResolution(index,group,&width1, &height1);	//��������ֱ���

		CameraSetOption(index,CAMERA_IMAGE_GRAY8);

		CameraSetDelay(0,0);						//���֡ɨ���ӳ�ʱ���趨
		
		// �������׼��������־
		CameraReady = TRUE;
		// ���ö���������߳�create thread
		pProcessThread.CreateThread();
		// ��ʼ����ͼ��		
		pProcessThread.play();
		TRACE("CameraReady=%d,���߳���ʾͼ��!\n",CameraReady);
		//�߳���ָ��cpu��
		SetThreadAffinityMask(GetCurrentThread(),0X00000008);
	}
	else
	{
		CameraReady = FALSE;								// û�����
		MessageBox("û�з���������޷����������λ����!",
			"�����ش���",
			MB_OK|MB_ICONWARNING|MB_APPLMODAL);
	}
}

// SocketͨѶ
// ����������
LRESULT CMicroMecanumDlg::ProcessCommand(WPARAM wParam, LPARAM lParam)
{
	char* sMsg=(char *)lParam;	
	Command2Route(sMsg);
	
	// ������յ�����ָ��
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

	if (Control.mLineEnable == 0)		// ֻ����һ�˶���ɣ����ܿ�ʼ�µ��˶������˶��У���ִ���κζ���
	{
		Control.mStep = 0;				// ��λ����ָ��
		Control.mExeStep = 0;			// ��λ΢��ָ��
		Control.mStepEnable = 0;		// ��λ�����Զ�����
		Control.mFixStep = 0;			// ��λ�Զ�У������
		
		Control.mPause		= 0;		// ��λ��ͣ���
		Control.mGoOn		= 0;		// ��λ����·���˶����
		Control.mRevers		= 0;		// ��λ�������
		
		Control.mLine		= 0;		// ��λ���β���ָ��
		Control.mExeLine	= 0;		// ��λ����ִ��ָ��
		Control.mLineEnable = 0XFF;		// ��ʼִ��΢������
		FindBarEnable		= TRUE;		// �����Զ�ʶ������
	}

	delete sMsg;
	return true;
}


// Camera ͼ����ʾ�߳�------------------------------------------------------------------
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

// ���ģ�� -------------------------------------------------------------------------------------------------------

#define EvisionBlock	5 
// �Ӿ�ģ�� -------------------------------------------------------------------------------------------------------

CString CMicroMecanumDlg::ReadBar()
{
	CString bar("");
	char sTemp[256];
	sprintf(m_pszDecodedText, "\0");
	//����Evision��Barcodeģ�����
	if (DisplayImage == TRUE)		
	{
		m_Match.Match(&m_Src);
		
		if(!EFailure())
		{
			double x, y, score;
			int nNum = m_Match.GetNumPositions();
			if (nNum > 0)	// �ҵ���ƥ��㣨ƥ��ȴ���0.3��
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
				if (strlen(m_pszDecodedText) < BarCodeNumber)	// �Ķ�ʧ��
				{
					BarDecodeInfo += ",Fail1";
					
					// �����3/4ͼ���ж�����
					m_Roi.Detach();		
					m_Roi.Attach(&m_Src);
					m_Roi.SetPlacement(0,0,480,480);
					//m_Src.SetPlacement(0,0,480,480);
					sprintf(m_pszDecodedText, "\0");
					m_BarCode.Read(&m_Roi, m_pszDecodedText, sizeof(m_pszDecodedText) - 1);
					if (strlen(m_pszDecodedText) < BarCodeNumber)	// �Ķ�ʧ��
					{
						BarDecodeInfo += ",Fail2";
						// ���Ҳ�3/4ͼ���ж�����
						m_Roi.Detach();		
						m_Roi.Attach(&m_Src);
						m_Roi.SetPlacement(160,0,480,480);
						//m_Src.SetPlacement(160,0,480,480);
						sprintf(m_pszDecodedText, "\0");
						m_BarCode.Read(&m_Roi, m_pszDecodedText, sizeof(m_pszDecodedText) - 1);
						if (strlen(m_pszDecodedText) < BarCodeNumber)	// �Ķ�ʧ��
						{
							BarDecodeInfo += ",Fail3";
						}
					}
				}
			}	// end if (nNum > 0)	// �ҵ���ƥ��㣨ƥ��ȴ���0.3��
			else
			{
				BarDecodeInfo = "!Score<0.25";
			}
		}	// end if(!EFailure())
		
		//�������λ�����ڵ���5
		if (strlen(m_pszDecodedText) >= BarCodeNumber)					
		{
			m_pszDecodedText[BarCodeNumber] = '\0';
			bar = m_pszDecodedText;
			// �������ÿһλ����0-9���֣����ڵر���д��ڣ��������Ϣ��ȷ
			if (!CheckBarcode(bar))
				bar = "";
		}
	}
	
	return bar;
}


// �������ܰ�������Evision�⺯������һ��C24ͼ��תΪBW8��ʽ�����룬�����ı�����ʾ
void CMicroMecanumDlg::BarDecoder()						// ����Evision����
{	
	BarDecodeInfo = "";

	// ���������Ϣ��ȷ
	if (isDecodeSuccess)
	{
		// -----------��¼�����ĵر�--pc
		char tmp[64];
		time_t t = time( 0 ); 			
		strftime( tmp, sizeof(tmp), "%X",localtime(&t) );

		ofstream ofsLog7;
		Json::Value msgBody;
						
		ofsLog7.open("log-7��¼�����ĵر�.txt",ios::app);
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
		// -----------��¼�����ĵر����

		// ͳ����ȷ�������		
		DecodeSuccesTime++;
		// ������Ļ���ص�ʵ�ʾ��뻻��ϵ������λ������/����
		double CaliK = BarLenth/640;
		// ��ȡ����X�����᳤��
		BarXlenth = 640 * CaliK;
		// ��ȡ����Y�����᳤��
		BarYlenth = 480 * CaliK;

		m_NotFindMarkTimes370 = 0;
		// �������ִ�еر���������˵���ҵ���
		if (m_isSearchingMark)
		{
			m_isSearchingMark = false;		// ���������ر�
			m_isSearchingMarkFound = TRUE;	// �����Ѿ��ѵ��ر��ˣ���������OnTimer�ﴦ��

			INT32 dis = INT32(DOUBLE(GetShiftR(&m_Device,XADDR)) * Puls2Distance);
			// ��¼�³����ߵľ��룬�Ա㳵��������һ������·�����˻�ȥ��׼�ر�
			m_markFoundLocation = fabs(dis);	// ȡ����ֵ����Ϊ����ʱdis�Ǹ�ֵ	
m_ofsLog2<<"�ر����ѵ�,m_markFoundLocation="<<m_markFoundLocation<<",m_iCurrentSearchingRoute="<<m_iCurrentSearchingRoute<<"\n";
		}

		// ���������ȷ�����ڽ��и�λ��ÿ������֮ǰ������и�λ��
		if (FindBarEnable == TRUE && isDecodeSuccess == TRUE && Control.mMaxLine >= 1)
		{			
ofstream ofsLog1;				
ofsLog1.open("log-1�ر�У׼����.txt",ios::app);
ofsLog1<<"-00 �����رָ꣬��="<<Line[Control.mExeLine].Derection
	<<",mExeLine="<<Control.mExeLine<<",mMaxLine="<<Control.mMaxLine<<",mLine"<<Control.mLine
	<<",mPause="<<Control.mPause<<"\n";			
			if (Line[Control.mExeLine].Derection == 370 )
			{
				// ��¼��һ���켣У׼�㣬ȷ����ִ��һ��Line����ʼ��ʱ���뽫nCheckPoints��Ϊ1
				// ��ΪCheckMarks[0]��CheckPoints[0]��ֵ
				nCheckPoints = 1;
				MARK* pMark = GetMarkInfo(BarCodeStr);
				CheckMarks[0].barCode = pMark->barCode;
				CheckMarks[0].dir = pMark->dir;
				CheckMarks[0].x = pMark->x;
				CheckMarks[0].y = pMark->y;

				CheckPoints[0].distance = 0;
				CheckPoints[0].deltax = 0;
				CheckPoints[0].deltay = 0;
				CheckPoints[0].deltaa = -BarAngle;	// ƫ�ƽǣ��������õ�ֵҪ��������
				CheckPoints[0].deltah = -BarAngle;	//0;
				CheckPoints[0].speed = 0;
				CheckPoints[0].adjustx = 0;
				CheckPoints[0].adjusty = 0;
//ofstream ofsLog1;				
//ofsLog1.open("log-1�ر�У׼����.txt",ios::app);
ofsLog1<<"-0 �����ر꣬��ʼ��̬У׼ mLineEnable="<<Control.mLineEnable<<",mLine="<<Control.mLine<<",mPause="<<Control.mPause<<"\n";

				// �����370ָ��տ�ʼִ�ж��Ҵ�����ͣ״̬�������Ƿ�λ�����ĵر�
				if (Control.mLineEnable !=0 /*== 0XFF*/	&& Control.mLine <= 20 
					&& Control.mPause == 0XFF)	
				{ 
					// ��������ر����ڵ����ĵر�
					CString sBarCentral = FindCentralMark(BarCodeStr);
ofsLog1<<"---01 �����ĵر�bar="<<BarCodeStr.GetBuffer(0)<<",���ĵر�barCenter="<<sBarCentral.GetBuffer(0)<<"---------\n"; 					
					if (sBarCentral == BarCodeStr) // ����������ĵر꣬�����
					{
						// mRouteGo()��ֱ������30������ִ��
						Control.mLine = 30;
						Control.mPause =0;			// ����ִ��
						Control.mGoOn =0;			// ����ִ��		
					}
					else	// ��Line�в���һ��ƫ��
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

								
							// ����������
							Control.mMaxLine++;
ofsLog1<<"-02 ��Line������һ��ƫ�ƣ�����Ϊaiddistance="<<MP.AidDistance<<",maxLine="<<Control.mMaxLine<<",exeLine="<<Control.mExeLine<<"\n"; 					
							// ����ִ�У���ʱӦ��ִ�������ӵ����Line
							Control.mPause =0;			// ����ִ��
							Control.mGoOn =0;			// ����ִ��		

						}
					}
				}

				// �����370ָ���Ѿ�ִ�����˶��Ҵ�����ͣ״̬������ƫ����
				if (Control.mStepEnable == 0 && Control.mLine == 60 && Control.mPause == 0XFF)
				{
ofsLog1<<"-1 ����370ָ���Ѿ�ִ�����ˣ����ƫ����Ϊ��BarX="<<BarXlocation<<",BarY="<<BarYlocation<<",angle="<<BarAngle<<"\n";
					bool bAcurate = false;	// ��̬У׼�Ƿ�ȷ
					// �������С��1mm�����߽Ƕ�С��1.0��
					if (fabs(BarXlocation) < X_Y_TOLERANCE_HIGH && fabs(BarYlocation)<X_Y_TOLERANCE_HIGH
						&& fabs(BarAngle) < ANGLE_TOLERANCE_HIGH)
					{
						m_CaliCount ++;	// У׼������1
ofsLog1<<"-2 ������Ҳ������ȫ����ȷ����̬У׼������1������"<<m_CaliCount<<"\n";
					}
					// �������С��0.5mm�����߽Ƕ�С��0.5��
					if (fabs(BarXlocation) < X_Y_TOLERANCE_LOW && fabs(BarYlocation)<X_Y_TOLERANCE_LOW
																&& fabs(BarAngle) < ANGLE_TOLERANCE_LOW)
					{
						m_CaliCount = 0;
						bAcurate = true;
					}
					if (bAcurate || m_CaliCount == MAX_ReCheckMarkTimes370)	// ����Ѿ�У׼��ȷ�ˣ����߻���У׼���������Σ�ֱ��������һ��ֱ�л��߹�����ߺ���ָ��
					{
						// �����Ƿ�У׼��״̬����¼У׼�ĵر�ID
						m_igvInfo.isMarked = true;
						m_igvInfo.MarkID = BarCodeStr;
ofstream ofsLog;				
ofsLog.open("log-4 isFree״̬����.txt",ios::app);
ofsLog<<"У׼�ˣ�MarkID="<<(char*)(_bstr_t)m_igvInfo.MarkID.GetBuffer(0)<<"\n";
ofsLog.close();

//ofsLog1<<"У׼�ˣ�MarkID="<<(char*)(_bstr_t)m_igvInfo.MarkID.GetBuffer(0)<<"\n";

						// ��������
						m_igvInfo.curX = pMark->x;
						m_igvInfo.curY = pMark->y;
						m_igvInfo.preX = m_igvInfo.curX;
						m_igvInfo.preY = m_igvInfo.curY;
ofsLog1<<"��̬У׼�ɹ����˴���������1��curx="<<m_igvInfo.curX<<",cury="<<m_igvInfo.curY<<",prex="<<m_igvInfo.preX<<",prey="<<m_igvInfo.preY<<"\n";
ofsLog1<<"-------------------------------------------------------------\n\n\n";						
						m_CaliCount = 0;	// У׼��������
						if (Control.mRevers == 0)	// ����ִ��Line������ָ��ʱ
						{
							int exeLine = Control.mExeLine;
							while (exeLine < Control.mMaxLine)
							{
								exeLine++;	// Lineָ��ָ����һ��Line
								if (exeLine == Control.mMaxLine)	// ����û������LineҪִ����
								{
									Control.mExeLine = 0;
									Control.mLine = 80;			// �˾�ִ�к�������л���mRouteGo()��ִ�У����ע�͵�
									//Control.mGoOn =0;			// ����ִ��
									//Control.mGoOn =0;			// ����ִ��

									exeLine = Control.mMaxLine;	// �൱�ڽ���whileѭ��
								}
								else	// ������滹��ֱ�л�����ת�������ִ��
								{
									int dir = Line[exeLine].Derection;
									if( dir == 0 || dir == 90 || dir == 180 || dir == 270
										|| dir == 361 || dir == 362)
									{
										Control.mExeLine = exeLine;
										Control.mLine = 20;			// 
										Control.mPause =0;			// ����ִ��
										Control.mGoOn =0;			// ����ִ��
										exeLine = Control.mMaxLine;	// �൱�ڽ���whileѭ��
									}
								}
							}
						}
					}
					else	// �Զ�У׼���ȴﲻ��Ҫ�󣬻����������ִ�д�ָ��
					{
ofsLog1<<"-8 �Զ�У׼���ȴﲻ��Ҫ�󣬻����������ִ�д�370ָ��\n";
						
						Control.mLine = 20;			// mRouteGo()��ֱ������20����ִ��Control.mExeLine++
						Control.mPause =0;			// ����ִ��
						Control.mGoOn =0;			// ����ִ��
					}
				}

ofsLog1.close();

			}		

			// ���������ȷ������ִ��һ��Line�����һ��У׼��
			else if (Line[Control.mExeLine].Derection == 0 || Line[Control.mExeLine].Derection == 180 
				&& nCheckPoints >= 1 /*ȷ����һ�����Ѿ�����*/)
			{
				// ����һ���켣У׼��
				
				// ��������һ�������ĵر�֮��ľ���
				MARK* pMark = GetMarkInfo(BarCodeStr);
				double disX = pMark->x - CheckMarks[nCheckPoints-1].x;
				double disY = pMark->y - CheckMarks[nCheckPoints-1].y;
				double dis = fabs(disX) + fabs(disY);	// �����ر������

				
				double markDeviation = 0;	// �����ĵر��ڴ�ֱ���˶������ƫ����
				double disX0 = pMark->x - CheckMarks[0].x;
				double disY0 = pMark->y - CheckMarks[0].y;

				// ��ͷ��Y������ǰ��
				if (m_igvInfo.Heading ==0 && Line[Control.mExeLine].Derection == 0)
				{
					markDeviation = disX0;
					//BarAngle = BarAngle;	// ǰ��Ϊ��
				}
				// ��ͷ��Y�����򣬺���
				else if (m_igvInfo.Heading ==0 && Line[Control.mExeLine].Derection == 180)
				{
					markDeviation = -disX0;
					//BarAngle = -BarAngle;	// ����Ϊ��
				}
				// ��ͷ��Y�Ḻ��ǰ��
				else if (m_igvInfo.Heading ==180 && Line[Control.mExeLine].Derection == 0)
				{
					markDeviation = -disX0;
					//BarAngle = BarAngle;	// ǰ��Ϊ��
				}
				// ��ͷ��Y�Ḻ�򣬺���
				else if (m_igvInfo.Heading ==180 && Line[Control.mExeLine].Derection == 180)
				{
					markDeviation = disX0;
					//BarAngle = -BarAngle;	// ����Ϊ��
				}				
				// ��ǰ������
				else if (m_igvInfo.Heading ==90 && Line[Control.mExeLine].Derection == 0)
				{
					markDeviation = -disY0;
					//BarAngle = BarAngle;	// ǰ��Ϊ��
				}
				else if (m_igvInfo.Heading ==90 && Line[Control.mExeLine].Derection == 180)
				{
					markDeviation = disY0;
					//BarAngle = -BarAngle;	// ����Ϊ��
				}
				else if (m_igvInfo.Heading ==270 && Line[Control.mExeLine].Derection == 0)
				{
					markDeviation = disY0;
					//BarAngle = BarAngle;	// ǰ��Ϊ��
				}
				else if (m_igvInfo.Heading ==270 && Line[Control.mExeLine].Derection == 180)
				{
					markDeviation = -disY0;
					//BarAngle = -BarAngle;	// ����Ϊ��
				}

				//�������ǰһ�������С��400mm�������
				if (dis > 400)
				{
					CheckMarks[nCheckPoints].barCode = pMark->barCode;
					CheckMarks[nCheckPoints].dir = pMark->dir;
					CheckMarks[nCheckPoints].x = pMark->x;
					CheckMarks[nCheckPoints].y = pMark->y;
					
					CheckPoints[nCheckPoints].distance = CheckPoints[nCheckPoints-1].distance + dis;
					if (Line[Control.mExeLine].Derection == 0)	// ǰ��
						CheckPoints[nCheckPoints].deltax =	-BarXlocation 	// �������õ�ֵҪ��������
															+ markDeviation;// ��������걾����Line��ƫ����
					else	// ����
						CheckPoints[nCheckPoints].deltax =	BarXlocation	// �������õ�ֵҪ�������ã���������
															+ markDeviation;// ��������걾����Line��ƫ����

					CheckPoints[nCheckPoints].deltay = -BarYlocation;	// �������õ�ֵҪ��������
					CheckPoints[nCheckPoints].deltaa = -BarAngle;	// �������õ�ֵҪ��������
					CheckPoints[nCheckPoints].speed = m_igvInfo.Speed;	// �������õ�ֵҪ��������

					// adjustx, adjusty, deltaH����FixTrack()����ȥ����

					nCheckPoints++;
				}
				else
					isDecodeSuccess = false;//���治ִ��У׼����
			}
		}

		//--------------------------------------------------------------
	
		// ���������ȷ����̬У���켣
		if ((FindBarEnable == TRUE) && isDecodeSuccess/*&& (BarCodeStr == "12345")*/
			&& (Control.mLineEnable != 0) && (Control.mExeStep > 2))
		{

// ���Control.mFixStep��Control.mExeStep��
ofsLog7<<Control.mFixStep<<" "<<Control.mExeStep<<"\n";
ofsLog7.close();

			if (Control.mFixStep != Control.mExeStep)
			{
				TRACE("FindBarEnable=%d,BarCodeStr=%d,mLineEnable=%d,mExeStep=%d,Shift=%d,\n",
					FindBarEnable,atoi(BarCodeStr),Control.mLineEnable,Control.mExeStep,
					INT32(DOUBLE(GetShiftR(&m_Device,XADDR)) * Puls2Distance));
				//StopMotion();	// ���������ȷ������ͣ��
				// ����������Ϣ��У׼�г��켣
				DOUBLE dR = 215;
				FixTrack(dR,BarXlocation,BarYlocation,BarAngle,&Control,Step);
				// ����У׼΢��λ��
				Control.mFixStep = Control.mExeStep;
			}
		}		
		//�����ʽ���������Ϣ
		// ��ȡ����ֵ
		str = BarCodeStr;									
		// �����������X������
		str1.Format(_T(",%4.1fmm,"), BarXlocation);			// ��ʽ������ַ�
		str += str1;										
		// �����������Y������
		str1.Format(_T("%4.1fmm,"), BarYlocation);			// ��ʽ������ַ�
		str += str1;										
		// �������ƫת�Ƕ�
		str1.Format(_T("%4.2fdeg,"), BarAngle);				// ��ʽ������ַ�
		str += str1;										
		// ���֡��
		str1.Format(_T("%dFPS,"), FramePerSecond);			// ��ʽ������ַ�
		str += str1;										
		// ����Ҫ��ʾ����Ϣ���ݵ���ʾ����
		BarDecodeInfo = BarDecodeInfo + ", " + str;							
	} 
	else	// ���벻��ȷ
	{
		// ���ִ��370ָ���޷������꣬���������1
		// ��mLine<60��Ϊ�˷�ֹ�������������·��ִ�����˵�û�ҵ��ر꣨mLine=60,mPause=0XFF������ʱ����Ҫ��������·��
		if (Line[Control.mExeLine].Derection == 370 && Control.mLine < 60)	
		{
			m_NotFindMarkTimes370++;
CString s;
s.Format("δ���ֵر������%d",m_NotFindMarkTimes370);
GetDlgItem(IDC_STATIC1)->SetWindowText(s);
			// �������ʧ�ܳ�����һ���Ĵ������������ֲ��ر�����		
			if (m_NotFindMarkTimes370 >= MAX_NotFindMarkTimes370)
			{
				// ��������·������Num_SearchLines����
				CalculateSearchLines();
				// ������·�����뵽����·����
				for(int k = (int)Control.mMaxLine-1; k >=(int)Control.mExeLine; k--)
				{
					Line[k+m_iCountOfSearchRoutes] = Line[k];
				}
				for (int i = 0; i<m_iCountOfSearchRoutes;i++)
					Line[Control.mExeLine+i] = SearchLines[i];
					
				// ����������
				Control.mMaxLine+=m_iCountOfSearchRoutes;
				// ����ִ�У���ʱӦ��ִ�������ӵ����Line
				Control.mPause =0;			// ����ִ��
				Control.mGoOn =0;			// ����ִ��
				Control.mLine = 30;

				m_iCurrentSearchingRoute = 0;		// ���õ�ǰ����ִ�еĵر�����·������, -1��ʾδ���еر�����
				m_isSearchingMark = true;	// ��������
				m_isSearchingMarkFound = false;	// ��δ������
				m_NotFindMarkTimes370 = 0;

m_ofsLog2<<"�����ر�����\n";
			}
		}
		
		//ͳ�Ʋ���ȷ�������
		DecodeFaileTime++;
		str = "";											// ����ַ���
		// ���֡��
		str1.Format(_T("%dFPS,"), FramePerSecond);			// ��ʽ������ַ�
		str += str1;										
		// ����ʧ�ܰٷֱ�
		double FailPercent = 100 * float(DecodeFaileTime)/float(DecodeSuccesTime + DecodeFaileTime);
		str1.Format(_T("%2.2fFPC,"), FailPercent);			// ��ʽ������ַ�
		str += str1;
		// ����Ҫ��ʾ����Ϣ���ݵ���ʾ����
		//BarDecodeInfo = " �� ����ʧ�� !!! ��," + str + DisplayControl(&Control);
		// ����Ҫ��ʾ����Ϣ���ݵ���ʾ����
		BarDecodeInfo = " �� ����ʧ�� !!! ��," + BarDecodeInfo + ", " + str;
		//������Ϣ����ȷ
		isDecodeSuccess = FALSE;	
		//��λ������Ϣ
		BarXlocation = 0;		// ��������X������
		BarYlocation = 0;		// ��������Y������
		BarXlenth = 0;			// ����X�����᳤��
		BarYlenth = 0;			// ����Y�����᳤��
		BarAngle = 0;			// ����ƫת�Ƕ�			
	}

	// ΪУ׼��ֵ
	Control.RoundUnit = TurnRound;
	Control.DeltH = BarAngle;
	Control.DeltX = BarXlocation;
	Control.DeltY = BarYlocation;
	FinishedBarDecode = TRUE;								// ��ɽ���

	//��ͼ����ͬ����ʾ�ַ���
	DrawArrow(mARROW);

}
// �Ӿ�ģ�� -------------------------------------------------------------------------------------------------------

#define DrawBlock	6 
// �˶���־�滭ģ�� ---------------------------------------------------------------------------------------------------
BOOL CMicroMecanumDlg::DrawArrow(UINT32 CommandDir)	// �滭�˶������ͷ
{
	CDC *pDC=NULL;								//����һ���յ�Device Concent
	CRect rect;									//�������
	CWnd *pWnd2 = GetDlgItem(IDC_STATICP1);		//��ȡ�滭����		
	if(pWnd2)									//�����ڴ���	
	{
		pWnd2->GetClientRect(&rect);			//�趨�滭����
		pDC = pWnd2->GetDC();					//�趨��ͼĿ���豸
		//�����ͼ����	
		INT32 w,h,x0,y0;
		x0 = rect.Width()/2;					// ͼ��X����
		y0 = rect.Height()/8;					// ͼ��Y����
		w = rect.Width()/16;					// ͼ����
		h = rect.Height()/16;					// ͼ��߶�
		if (w >= h)								// ��ͼ���� = �߶� 
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

		if (CameraReady != TRUE)				// ������ʾͼ�������ñ���ɫ
		{
			pDC->FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(236,233,216));	//ϵͳĬ�ϱ���ɫ
		}
		// ��ʮ����
		CPen pen2; 
		pen2.CreatePen(PS_SOLID,1,RGB(0,0,127));			// ������ɫ����
		CPen* pOldPen2= pDC->SelectObject(&pen2);			// ѡ�񻭱�
		// ����
		pDC->MoveTo(rect.Width()/2,0);						// �ƶ������
		pDC->LineTo(rect.Width()/2,rect.Height());			// ����㻭�ߵ��յ�
		// ����		
		pDC->MoveTo(0,rect.Height()/2);						// �ƶ������
		pDC->LineTo(rect.Width(),rect.Height()/2);			// ����㻭�ߵ��յ�			
		pDC->SelectObject(&pOldPen2);						// �ͷŻ���

		// ����һ֧����
		CPen pen; 											// ���廭��
		pen.CreatePen(PS_SOLID,1,ARROWRED);					// ��������
		CPen* pOldPen= pDC->SelectObject(&pen);				// �󶨻���		
		// ����һ����ˢ
		CBrush brush;  										// ���廭ˢ
		brush.CreateSolidBrush(ARROWRED);					// ������ˢ
		CBrush* pOldBrush = pDC->SelectObject(&brush);		// �󶨻�ˢ		
		if (CommandDir <= 360)								// ��ˢ��ʵ���ı���
		{
			pDC->FillSolidRect(x0-w/2,y0,w,h,ARROWRED);		// ��ˢ��ʵ���ı���// (x,y,w,h,RGB(200,0,0))
		}
		// ��������
		switch(CommandDir)
		{
		case 0:
			pt[0].x = x0-w;
			pt[0].y = y0;
			pt[1].x = x0+w;
			pt[1].y = y0;
			pt[2].x = x0;
			pt[2].y = y0-h;
			pDC->Polygon(pt,3);								// ��ˢ��ʵ��������	
			break;
		case 90:
			pt[0].x = x0+w/2;
			pt[0].y = y0-h/2;
			pt[1].x = x0+w/2;
			pt[1].y = y0+3*h/2;
			pt[2].x = x0+3*w/2;
			pt[2].y = y0+h/2;
			pDC->Polygon(pt,3);								// ��ˢ��ʵ��������	
			break;
		case 180:
			pt[0].x = x0+w;
			pt[0].y = y0+h;
			pt[1].x = x0-w;
			pt[1].y = y0+h;
			pt[2].x = x0;
			pt[2].y = y0+2*h;
			pDC->Polygon(pt,3);								// ��ˢ��ʵ��������	
			break;
		case 270:
			pt[0].x = x0-w/2;
			pt[0].y = y0+3*h/2;
			pt[1].x = x0-w/2;
			pt[1].y = y0-h/2;
			pt[2].x = x0-3*h/2;
			pt[2].y = y0+h/2;
			pDC->Polygon(pt,3);								// ��ˢ��ʵ��������	
			break;
		case 360:
			pDC->FillSolidRect(x0-w,y0-h,2*w,2*h,ARROWBLU);	// ��ˢ��ʵ���ı���
			break;
		case 361:
			pt[0].x = x0;
			pt[0].y = y0;
			pt[1].x = x0;
			pt[1].y = y0-2*h;
			pt[2].x = x0-w;
			pt[2].y = y0-h;
			pDC->Polygon(pt,3);								// ��ˢ��ʵ��������
// 			StartP.x = x0-w;
// 			StartP.y = y0;			
// 			EndP.x = x0;
// 			EndP.y = y0-h;
// 			pDC->Arc(Round,StartP,EndP);					// ��Բ��			
			break;
		case 362:
			pt[0].x = x0;
			pt[0].y = y0;
			pt[1].x = x0;
			pt[1].y = y0-2*h;
			pt[2].x = x0+w;
			pt[2].y = y0-h;
			pDC->Polygon(pt,3);								// ��ˢ��ʵ��������
// 			StartP.x = x0;
// 			StartP.y = y0-h;			
// 			EndP.x = x0+w;
// 			EndP.y = y0;
// 			pDC->Arc(Round,StartP,EndP);					// ��Բ��			
			break;
		case 370:
			pt[0].x = x0;
			pt[0].y = y0-h;
			pt[1].x = x0;
			pt[1].y = y0+h;
			pt[2].x = x0-w;
			pt[2].y = y0;
			pDC->Polygon(pt,3);								// ��ˢ��ʵ��������	
			pt[0].x = x0;
			pt[0].y = y0-h;
			pt[1].x = x0;
			pt[1].y = y0+h;
			pt[2].x = x0+w;
			pt[2].y = y0;
			pDC->Polygon(pt,3);								// ��ˢ��ʵ��������
			break;
		case 372:
			pDC->Ellipse(x0-w,y0-h,x0+w,y0+h);				// ��ˢ��ʵ��Բ
			break;			
		default:
			break;
		}
		pDC->SelectObject(&pOldPen);						// �ͷŻ���
		pDC->SelectObject(&pOldBrush);						// �ͷŻ�ˢ	
		
		CPen pen1; 											// ���廭��
		pen1.CreatePen(PS_SOLID,10,ARROWRED);				// ��������
		pOldPen= pDC->SelectObject(&pen1);					// �󶨻���	
		if (CommandDir == 361)
		{
			StartP.x = x0-w;
			StartP.y = y0;			
			EndP.x = x0;
			EndP.y = y0-h;
			pDC->Arc(Round,StartP,EndP);					// ��Բ��
		} 		
		if (CommandDir == 362)		// ����
		{
			StartP.x = x0;
			StartP.y = y0-h;			
			EndP.x = x0+w;
			EndP.y = y0;
			pDC->Arc(Round,StartP,EndP);					// ��Բ��
		} 		
		pDC->SelectObject(&pOldPen);						// �ͷŻ���
		
		CTime curtime=CTime::GetCurrentTime();
		CString TimeMark = curtime.Format("%Y-%m-%d-%H:%M:%S ");	//�ɼ�������ϵͳ��ǰʱ��
		
		CString MotionIfo;
		if (Control.mLineEnable != 0)				//�ϳɵ�ǰ�˶���Ϣ
		{
			MotionIfo.Format(" ·��%d��·�Σ�%d#·��%d��΢��������ִ�е�%d΢��%d#����!",Control.mMaxLine,Control.mExeLine,Control.mMaxStep,Control.mExeStep,mARROW);
		}
		else
		{
			MotionIfo.Format(" ���·���ܼƺ�ʱ%d��!",Control.mRevers);				
		}
		//TimeMark += MotionIfo + DisplayControl(&Control);
		TimeMark += MotionIfo;
	
		CPen pen3; 											// �Զ��廭��
		pen3.CreatePen(PS_SOLID,1,RGB(0,255,0));			// ��������
		CPen* pOldPen3= pDC->SelectObject(&pen3);			// Select a green pen
		m_BarCode.Draw(pDC->GetSafeHdc(), INS_DRAW_ACTUAL);	// Draw the found barcode frame
		pDC->TextOut(10, 360, DisplayControl(&Control), strlen(DisplayControl(&Control)));	// ��ʾС��״̬��Ϣ
		pDC->TextOut(10, 380, DisplayPar(&Line[Control.mExeLine]), strlen(DisplayPar(&Line[Control.mExeLine])));
		pDC->TextOut(10, 400, DisplayPar(&Line[Control.mExeStep]), strlen(DisplayPar(&Line[Control.mExeStep])));
		pDC->TextOut(10, 420, TimeMark, strlen(TimeMark));	// ��ʾС��״̬��Ϣ	
		pDC->TextOut(10, 440, SystemInfor, strlen(SystemInfor));	// ��ʾС��״̬��Ϣ	
		pDC->TextOut(10, 460, BarDecodeInfo, strlen(BarDecodeInfo));	// ��ʾ������Ϣ
		pDC->SelectObject(&pOldPen3);						// Put back old pen	
		
		ReleaseDC(pDC);										// �ͷŻ�ͼĿ���豸	
	}
	//RunLife("S");	//ͳ�Ƴ�������ʱ��
	return TRUE;	// ���������˶�λ��
}
// end �˶���־�滭ģ�� ---------------------------------------------------------------------------------------------------
// ����
BOOL CMicroMecanumDlg::DeMotionError(DOUBLE *dR,DOUBLE *X,DOUBLE *Y,DOUBLE *L,DOUBLE *dL)	//�˶�����������㹫ʽ
{
	DOUBLE a,b,k,r,l,rd,ld;	// ����ֲ�����

	rd = *dR;	// ����뾶��mm
	a = *X;		// ����X����mm
	b = *Y;		// ����Y����mm

	if (a != 0)
	{
		r = (a*a + b*b)/(2*a);	// �뾶mm
		k = asin(b/r);			// ����Rad
		l = r*k;				// ����mm
		ld = rd*k;				// ����mm 
	}
	else
	{
		l = b;					// ����mm
		ld = 0;					// ����mm 
	}

	*L = l;			// �������mm
	*dL = ld;		// �������mm 
	//*dL = -1 * ld;	// �������mm 
	return TRUE;// ��ɹ���
}

BOOL CMicroMecanumDlg::DeMachineError(DOUBLE &R,DOUBLE &D,DOUBLE &dR,DOUBLE &L,DOUBLE &dL)	//��е����������㹫ʽ
{
	DOUBLE k;		// ����ֲ�����

	k = asin(D/R);	// ���㻡��Rad
	L = R * k;		// �������mm
	dL= dR * k; 	// �������mm 

	return TRUE;	// ��ɹ���	
}

// ����
BOOL CMicroMecanumDlg::DeAngleError(DOUBLE *rL,DOUBLE *X,DOUBLE *Y,DOUBLE *A,DOUBLE *dL)	//ͨ���Ƕ������˶����
{
	DOUBLE a,b,c,k,r,l,dl;	// ����ֲ�����

	l = *rL;	// ����Բ�ܳ�mm
	a = *X;		// ����X����mm
	b = *Y;		// ����Y����mm
	c = *A;		// ����Ƕȵ�λ��

	// ����뾶
	r = l/(2*3.1415926);	// �뾶mm
	// ����ƫ����
	k = (3.1415926 / 2) - acos(a/r);	// ����Rad
	dl = r*k;				// ����mm 
	*dL = -1 * dl;			// �����������mm 
	TRACE("x=%.3f,r=%.3f,K=%.3f,A=%.3f,dl=%.3f,\n",a,r,(k * 180)/3.1415926,c,dl);
	return TRUE;// ��ɹ���
}

// ��̬У׼
BOOL CMicroMecanumDlg::FixTrack(DOUBLE &dR,DOUBLE &dX,DOUBLE &dY,DOUBLE &dA,
								  ControlParemeter* Control,MotionParemeter* step)	
{
	// ����־�ļ�
ofstream ofsLog;
ofsLog.open("log-0 �켣У׼����.txt",ios::app);

	// ΢������
	int MicroDistance = Line[Control->mExeLine].MicroDistance * Puls2Distance;	//��λ����
	// Line�ĳ���
	int LineLength = Line[Control->mExeLine].AidDistance * Puls2Distance;		// ��λ����

	// Ŀǰ����ִ�е�΢������ǡ�mExeStep-3������mExeStep-2���͡�mExeStep-1�����ڶ�ջ�У�
	// ���ӡ�mExeStep����ʼ��������У׼
	int StartFixStep = Control->mExeStep;//+1;
	// ���һ��΢������ǡ�mMaxStep-1������һ���ĳ���С�ڻ���ڱ�׼����:
	int EndFixStep = Control->mMaxStep -1 -1;	//��-2�����һ����У׼

	// �ж�ִ��У׼��΢���Ƿ񳬳������һ����У׼��΢��
	if (StartFixStep <= EndFixStep)
	{
		double x1, y1;	// ��һ��������У׼�����꣨�Գ�����Ϊԭ�㣬�н�����ΪY����������꣬�ǵ�ͼ���꣩
		double x2, y2;	// ��ǰ������У׼������꣨������ͬ�ϣ�
		double x3, y3;	// ��ʼУ׼������꣨������ͬ�ϣ�
		double x4, y4;	// Line�յ�ĵ����꣨������ͬ�ϣ�

		x1 = CheckPoints[nCheckPoints-2].adjustx;
		y1 = CheckPoints[nCheckPoints-2].adjusty;

		x2 = CheckPoints[nCheckPoints-1].deltax;
		y2 = CheckPoints[nCheckPoints-1].distance;

		y3 = StartFixStep * MicroDistance;
		x3 = x1 + (y3-y1)/(y2-y1) * (x2 - x1);		// �������������μ���
		//x3 = x2 + (y3-y2) * tan (CheckPoints[nCheckPoints-1].deltaa * 3.14/180.0);

		x4 = 0;
		y4 = LineLength;

		
		// �����Ҫ�����ĽǶȣ����ı�ʾ��Ҫ����ƫ�����ı�ʾ��Ҫ����ƫ����λΪ��
		double deltaH = GetRotateAngle(x1,y1,x2,y2,x3,y3,x4,y4);
		
		CheckPoints[nCheckPoints-1].deltah = deltaH;	// ��¼ƫ�ƽ�

		double deltaH0 = deltaH;	// �������ں�������
		int TotalFixStep = 0;		// ��¼һ�������˼���

		// ���ƫ�ǹ�����ֶಽ��ƫ�����2��������2�����ٴζ����꣩��������StartFixStepһ����ƫ
		// �����Ҳ��������������
		if (deltaH > 0.1)	// ʹС������ƫ
		{
			// Ϊ�˱���һ�ξ�ƫ�Ƕȹ�������Ƕȳ����޶ȣ���ֶ��У��
			for (int s = StartFixStep; s <= EndFixStep; s++)
			{
				if (deltaH > RotateLimit)
				{
					// �Ե������Ǿ���
					INT32 caliD = RotateLimit * RotateFactor;//RotateFactor * MicroDistance;
					Step[s].CaliDistance = - caliD;	//Ϊ����������ƫ
					TotalFixStep++;

					// ��ƫ�ƽǼ�ȥ�ով����ĽǶȣ�������Ϊ��һ���������ĽǶ�
					deltaH -= RotateLimit;

					//�������2�ξͲ��ټ���У׼
					if (TotalFixStep == 2)
					{
						deltaH -= 360.0;
						// ����checkpoint����ʵ�ʵı�����Ϊ���һ��checkpoint
						CheckPoints[nCheckPoints-1].adjustx = x3;
						CheckPoints[nCheckPoints-1].adjusty = y3;
					}
				}
				else if (deltaH >0)
				{
					INT32 caliD = deltaH * RotateFactor;//* RotateFactor * MicroDistance;
					Step[s].CaliDistance = - caliD;//Ϊ����������ƫ						
					TotalFixStep++;

					// ��ƫ�ƽǼ�ȥ�ով����ĽǶȣ�ʹ��Ϊ�������´�ѭ����ִ�о�ƫ
					deltaH -= RotateLimit;

					// ����checkpoint����ʵ�ʵı�����Ϊ���һ��checkpoint
					CheckPoints[nCheckPoints-1].adjustx = x3;
					CheckPoints[nCheckPoints-1].adjusty = y3;
				}
			}			
		}
		else if (deltaH < -0.1)	// ʹС������ƫ
		{
			// Ϊ�˱���һ�ξ�ƫ�Ƕȹ�������Ƕȳ����޶ȣ���ֶ��У��
			for (int s = StartFixStep; s <= EndFixStep; s++)
			{
				if (-deltaH > RotateLimit)
				{
					// �Ե������Ǿ���
					INT32 caliD =  RotateLimit * RotateFactor;//* RotateFactor * MicroDistance;
					Step[s].CaliDistance = caliD; //Ϊ����������ƫ
					TotalFixStep++;

					// ���ƽ�ƫ�ƽǼ��ϸով����ĽǶȣ���Ϊ��һ���������ĽǶ�
					deltaH += RotateLimit;
					
					//�������2�ξͲ��ټ���У׼
					if (TotalFixStep == 2)
					{
						deltaH += 360.0;
						// ����checkpoint����ʵ�ʵı�����Ϊ���һ��checkpoint
						CheckPoints[nCheckPoints-1].adjustx = x3;
						CheckPoints[nCheckPoints-1].adjusty = y3;
					}
				}
				else if (-deltaH > 0)
				{
					INT32 caliD =  deltaH * RotateFactor;//* RotateFactor * MicroDistance;
					Step[s].CaliDistance = - caliD;	//Ϊ����������ƫ
					TotalFixStep++;

					// ��ƫ�ƽǼ��ϸով����ĽǶȣ�ʹ��Ϊ�������´�ѭ����ִ�о�ƫ
					deltaH += RotateLimit;
					
					// ����checkpoint����ʵ�ʵı�����Ϊ���һ��checkpoint
					CheckPoints[nCheckPoints-1].adjustx = x3;
					CheckPoints[nCheckPoints-1].adjusty = y3;
				}
			}			
		}
		else	// �Ƕ�ƫ��С��0.1�Ȳ�����
		{
ofsLog << "    �Ƕ�ƫ��С��0.1�Ȳ�����\n";
		}

		CString Tempstr;
		Tempstr.Format("LL=%d,SS=%d,x1=%.1f,y1=%.1f,x2=%.1f,y2=%.1f,x3=%.1f,y3=%.1f,x4=%.1f,y4=%.1f,ƫ�ƽ�=%.1f,��ƫ����=%d\n",
			Control->mExeLine,Control->mExeStep,x1,y1,x2,y2,x3,y3,x4,y4,deltaH0,TotalFixStep);

		// д����־�ļ���
ofsLog << "    x1="<<x1<<",y1="<<y1<<",x2="<<x2<<",y2="<<y2;
ofsLog << ",x3="<<x3<<",y3="<<y3<<",x4="<<x4<<",y4="<<y4<<"\n";
ofsLog << "    ��ƫ�ƽ�="<<deltaH0<<",ʵ�ⷽ���="<<CheckPoints[nCheckPoints-1].deltaa<<"\n";
ofsLog << "    ƫ�Ʋ���="<<TotalFixStep<<",sStep="<<StartFixStep<<",eStep="<<EndFixStep<<"\n";

		//���CheckPoints[i].adjustx��
		
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

		// ���͹켣У׼��ص����ݵ���̨ 
		SendIGVTrackFixInfo(CheckMarks[nCheckPoints-2].barCode,		// ����ID
							CheckMarks[nCheckPoints-2].x,			// ����ر�x
							CheckMarks[nCheckPoints-2].y,			// ����ر�y
							CheckPoints[nCheckPoints-2].deltah,		// ���㷽��ƫ�ƽ�	
							CheckPoints[nCheckPoints-2].deltax,		// ����xƫ����
							CheckPoints[nCheckPoints-2].deltay,		// ����yƫ����
							CheckPoints[nCheckPoints-2].speed,		// ����С���ٶ�
							CheckPoints[nCheckPoints-2].adjustx,	// У׼��x����(���Ϊ����ԭ�㣬���ָ���յ�ΪY������)
							CheckPoints[nCheckPoints-2].adjusty,	// У׼��y����(ͬ��)							
							RotateFactor,							// У׼��У׼ϵ��
							CheckMarks[nCheckPoints-1].barCode,		// �����ID
							CheckMarks[nCheckPoints-1].x,			// �����ر�x
							CheckMarks[nCheckPoints-1].y,			// �����ر�y
							CheckPoints[nCheckPoints-1].deltah,		// ����㷽��ƫ�ƽ�
							CheckPoints[nCheckPoints-1].deltax,		// �����xƫ����
							CheckPoints[nCheckPoints-1].deltay,		// �����yƫ����
							m_igvInfo.Heading,						// С�������ϵĳ�ͷ����
							m_igvInfo.MoveDirection);				// С�������ϵ��˶�����
	}
	return TRUE;	// ��ɹ���	
}



// �����ĸ�������꣬��С����Ҫ��ת�ĽǶ�
// x1 y1 ������
// x2 y2 ��⵽�ر�ĵ�
// x3 y3 ����12�㷽�����쵽����ʵʩ��ƫ�ĵ�
// x4 y4 Ŀ��㣬x4=0
// ����deltaH: ���ı�ʾ����ƫ�����ı�ʾ����ƫ
double CMicroMecanumDlg::GetRotateAngle(double x1, double y1, double x2, double y2, 
										double x3, double y3, double x4, double y4)
{
	double deltaH = 0;
	// ���1ָ���2������12
	double x12 = x2-x1;
	double y12 = y2-y1;
	// ������ģ
	double m12 = sqrt(x12*x12 + y12*y12);
	// ���3ָ���4������34
	double x34 = x4-x3;
	double y34 = y4-y3;
	// ������ģ
	double m34 = sqrt(x34*x34 + y34*y34);

	double cosinA = (x12*x34 + y12*y34)/(m12*m34);
	deltaH = acos(cosinA);

	// �ж�����ƫ��������ƫ

	// ��������������ڵ�һ����,ע��y12��y34�϶���>=0
	if (x12 >=0  && x34 >=0 )
	{
		if (x12 ==0 && x34 == 0)
			deltaH = 0;
		else if(x12 == 0 && x34 != 0 )	//������ƫ
			deltaH = -deltaH;
		else if(x12 != 0 && x34 == 0 )	//������ƫ
			deltaH = deltaH;
		else if(y12/x12 > y34/x34)		//������ƫ
			deltaH = -deltaH;
		else							//������ƫ
			deltaH = deltaH;
	}
	// ����������ڵڶ�����
	else if (x12 <= 0 && x34 <= 0)
	{
		if (x12 ==0 && x34 == 0)
			deltaH = 0;
		else if(x12 == 0 && x34 != 0 )	//������ƫ
			deltaH = deltaH;
		else if(x12 != 0 && x34 == 0 )	//������ƫ
			deltaH = -deltaH;
		else if(y12/x12 > y34/x34)		//������ƫ
			deltaH = -deltaH;
		else							//������ƫ
			deltaH = deltaH;
	}
	// ���12�ڵ�һ���ޣ�34�ڵڶ����ޣ���������ƫ
	else if (x12 >=0 && x34 <=0)
		deltaH = deltaH;
	else //12�ڵڶ����ޣ�34�ڵ�һ���ޣ���������ƫ
		deltaH = -deltaH;
		
	return deltaH * 180/3.14;
}

// ����������֤��δ�����ã���Ϊ������ʵ��ʱ����ڵر��ƫ�ƽǲ��������޷���֤
// ������3��4�������ͳ��ӵ�ƫ�ƽ�deltaa������˶�����ģ�ƫ��Ϊ��������С����Ҫ��ת�ĽǶ�
// x3 y3 ����12�㷽�����쵽����ʵʩ��ƫ�ĵ�
// x4 y4 Ŀ��㣬x4=0
// deltaa ���ӵ�ƫ�ƽǣ�����˶�����ģ�ƫ��Ϊ����
// isForward �Ƿ�ǰ������������0�ȣ���������180�ȣ�
// ����deltaA: ���ı�ʾ������ƫ��Ŀǰƫ���ˣ������ı�ʾ����ƫ
double CMicroMecanumDlg::GetRotateAngle(double x3, double y3,double x4, double y4, double deltaa)
{
	// ����deltaa �� isForward���x1,y1��x2,y2
	double x1 = 0;
	double y1 = 0;
	double x2;
	double y2 = 1000;
	deltaa = deltaa*3.14/180.0;
	// ���1ָ���2������12
	if (deltaa > 0)	// ��ͷƫ��
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

// ����sBar�ر긽�������ĵر�(���ڲ��ŵ���������)����������������ģ��򷵻�����barCode.
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
			if( fabs(marks[i].x - x) +fabs(marks[i].y-y) <100.1 ) //��һ��
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

	if(count <2)	// δ�ҵ����ŵ���������
		return sBarCentral;

	double x1 = mapMarks[sBar1]->x;
	double y1 = mapMarks[sBar1]->y;
	double x2 = mapMarks[sBar2]->x;
	double y2 = mapMarks[sBar2]->y;

	if (fabs(y1-y2) < 0.1)	// ����
	{
		if ( fabs(x+x1-2*x2) < 0.1) // x+x1 == 2*x2
			sBarCentral = sBar2;
		else if (fabs(x+x2-2*x1) < 0.1)
			sBarCentral = sBar1;
		else 
			sBarCentral = sBar;
	}
	else	// ����
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

// ���ݵ�ǰ�����ĵر�(����ƫ����)�����ĵر꣬�Լ���ͷ����ȷ�����ӵ��ƶ�����
// pMark ��ǰ�ر�
// pMarkCentral ���ĵر�
// deltax ��ǰ�ر�ƫ����x
// deltay ��ǰ�ر�ƫ����y
// m_Heading ��ͷ����
MotionParemeter CMicroMecanumDlg::GetCaliLine(MARK* pMark, MARK* pMarkCentral, double deltax, double deltay, double m_Heading)
{
	//MotionParemeter MP;
	int dir;
	int dis;
	double x12 = pMarkCentral->x - pMark->x;	// x����
	double y12 = pMarkCentral->y - pMark->y;	// y����
	switch((int)m_Heading)
	{
	case 0:		// ��ͷ����
		if (x12 > 40 && y12 < 1)		// ���ƶ�
		{
			dir = 90;
			dis = x12 - deltax;
		}
		else if (x12 < 1 && y12 > 40)	// ���ƶ�
		{
			dir = 0;
			dis = y12 - deltay;
		}
		else if (x12 < -40 && y12 < 1)	// �����ƶ�
		{
			dir = 270;
			dis = -x12 + deltax;
		}
		else if (x12 < 1 && y12 < -40)	// �����ƶ�
		{
			dir = 180;
			dis = -y12 + deltay;
		}
		break;
	case 90:	// ��ͷ����
		if (x12 > 40 && y12 < 1)		// ���ƶ�
		{
			dir = 0;
			dis = x12 - deltay;
		}
		else if (x12 < 1 && y12 > 40)	// ���ƶ�
		{
			dir = 270;
			dis = y12 + deltax;
		}
		else if (x12 < -40 && y12 < 1)	// �����ƶ�
		{
			dir = 180;
			dis = -x12 + deltay;
		}
		else if (x12 < 1 && y12 < -40)	// �����ƶ�
		{
			dir = 90;
			dis = -y12 - deltax;
		}		
		break;
	case 180:	// ��ͷ����
		if (x12 > 40 && y12 < 1)		// ���ƶ�
		{
			dir = 270;
			dis = x12 + deltax;
		}
		else if (x12 < 1 && y12 > 40)	// ���ƶ�
		{
			dir = 180;
			dis = y12 + deltay;
		}
		else if (x12 < -40 && y12 < 1)	// �����ƶ�
		{
			dir = 90;
			dis = -x12 - deltax;
		}
		else if (x12 < 1 && y12 < -40)	// �����ƶ�
		{
			dir = 0;
			dis = -y12 - deltay;
		}		
		break;
	case 270:	// ��ͷ����
		if (x12 > 40 && y12 < 1)		// ���ƶ�
		{
			dir = 180;
			dis = x12 + deltay;
		}
		else if (x12 < 1 && y12 > 40)	// ���ƶ�
		{
			dir = 90;
			dis = y12 - deltax;
		}
		else if (x12 < -40 && y12 < 1)	// �����ƶ�
		{
			dir = 0;
			dis = -x12 - deltay;
		}
		else if (x12 < 1 && y12 < -40)	// �����ƶ�
		{
			dir = 270;
			dis = -y12 + deltax;
		}		
		break;
	}

	return GetMotionParameters(dir, dis);
	/*
		// 1.�˶�����
		//MP.Derection	;	
		// 2.Ŀ�����
		//MP.AidDistance	= int(fabs(x12) + fabs(y12));	
		// 3.�˶�����
		MP.MotionType	= LineRips;
		// 4.�ٶȱ���
		MP.SpeedRate	= sRate;
		// 5.�����ٶ�
		MP.StartSpeed	= 0;
		// 6.����ٶ�
		MP.MaxSpeed		= 500;
		// 7.�Ӽ��ٶ�
		MP.ACC			= 500;	
		// 8.�����ٶ�
		MP.DACC			= 500;
		// 9.���پ���
		MP.ReducePoint	= 0;
		// 10.��������
		MP.StartType	= STAUD;
		// 11.У������
		MP.CaliDistance	= 0;
		// 12.΢������	
		MP.MicroDistance =  200;
		// 13.X����Ŀ��
		MP.AidX			= 1;
		// 14.Y����Ŀ��
		MP.AidY			= 2;
		// 15.Z����Ŀ��
		MP.AidZ			= 3;
		// 16.U����Ŀ��	
		MP.AidU			= 4;	

	return MP;
	*/
}

// �����˶������Ŀ����뷵��һ���˶�������MotionParameter�ṹ�壩
// dir ���򣨶ȣ���ǰ��Ϊ0�ȣ�˳ʱ�뷽��ƣ�
// ���루���ף�
// ǰ��=0������=180������=270������=180����ͣ=372����ת=361����ת=270���½�=366��̧��=367��У׼=370��
// AidDistance��ȡֵ��ǰ�������ˡ����ơ����Ƶľ��룬��λ����
// ��ͣ�ĵ�λ��0.1�룬�����ͣ1�룬AidDistance=10��
// �½���̧����У׼����£�AidDistance=0�Ϳ�����

MotionParemeter CMicroMecanumDlg::GetMotionParameters(int dir, int dis)
{
	MotionParemeter MP;
		// 1.�˶�����
		MP.Derection  = dir	;			
		// 2.Ŀ�����
		MP.AidDistance	= dis;		 	
		// 3.�˶�����
		MP.MotionType	= LineRips;
		// 4.�ٶȱ���
		MP.SpeedRate	= sRate;
		// 5.�����ٶ�
		MP.StartSpeed	= 0;
		// 6.����ٶ�
		MP.MaxSpeed		= 500;
		// 7.�Ӽ��ٶ�
		MP.ACC			= 500;	
		// 8.�����ٶ�
		MP.DACC			= 500;
		// 9.���پ���
		MP.ReducePoint	= 0;
		// 10.��������
		MP.StartType	= STAUD;
		// 11.У������
		MP.CaliDistance	= 0;
		// 12.΢������	
		MP.MicroDistance =  200;
		// 13.X����Ŀ��
		MP.AidX			= 1;
		// 14.Y����Ŀ��
		MP.AidY			= 2;
		// 15.Z����Ŀ��
		MP.AidZ			= 3;
		// 16.U����Ŀ��	
		MP.AidU			= 4;	

		return MP;
}

DWORD CMicroMecanumDlg::RunLife(CString TimeUnit)	//ͳ�Ƴ�������ʱ��
{
	static DWORD TimePoint0,TimePoint1;
	TimePoint1 =  GetCurrentTime();
	if (TimePoint0 == 0)		// ������ʼֵ
	{
		TimePoint0 = TimePoint1;
		TRACE("Time0=%d!\n",TimePoint0);
	}
	if (TimeUnit == "S")
	{
		TimePoint1 = (TimePoint1 - TimePoint0)/1000;
		TRACE("��������%d��!\n",TimePoint1);
	} 
	else
	{
		TimePoint1 = TimePoint1 - TimePoint0;
		TRACE("��������0X%X����!\n",TimePoint1);
	}	
	return TimePoint1;// ��ɹ���
}




void CMicroMecanumDlg::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	CString s;
	m_HeadingCombBxo.GetLBText(m_HeadingCombBxo.GetCurSel(),s);
	m_igvInfo.Heading = atoi(s);
}

// ������תָ����³�ͷ����
// cmd: 361��ʱ��ת��362˳ʱ��
// angle: ת360�ȵ�angleֵΪ3180 (TurnRound = 3180)
void CMicroMecanumDlg::UpdateHeading(INT32 cmd, INT32 angle)
{
	int heading = m_igvInfo.Heading+0.5;	// ȡ��
	int turn = int(angle*360/TurnRound + 0.5);	// ����
	
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

// �ж�һ������ĸ�λ�Ƿ���0-9������
BOOL CMicroMecanumDlg::CheckBarcode(CString bar)
{
	// ��֤����
	if (bar.GetLength() != BarCodeNumber)
	{
m_ofsLog2 << "�����ĵر�("<<bar<<")���Ȳ�����"<<BarCodeNumber<<"\n";
		return false;
	}
	// ��֤�ַ�
	for (int i =0; i< BarCodeNumber; i++)
	{
		char c = bar.GetAt(i);
		if (c<'0' || c>'9')
		{
			m_ofsLog2 << "�����ĵر�("<<bar<<")�з�0-9�ַ�\n";
			return false;
		}
	}

	// ��֤�ر�
	if(GetMarkInfo(bar) == NULL)
	{
		m_ofsLog2 << "�����ĵر�("<<bar<<")�ڵر��ļ���δ�鵽\n";
		return false;
	}

	return true;
}

// ���³�����ǰ����
// dis ������ֱ�����ߵľ��룬ע��dis >= 0
void CMicroMecanumDlg::UpdateOwnPosition(int dis)
{
	int iDirection = Line[Control.mExeLine].Derection; // �˶�����
	// �����ڽ���ǰ�������ˡ����ơ��������ֶ���֮һ
	if (iDirection ==0 || iDirection == 90 || iDirection == 180 || iDirection == 270)
	{
		// �������ִ��line�е�ĳ��step����������ͣ״̬
		if (Control.mLineEnable != 0 && Control.mStepEnable != 0 && Control.mPause == 0 && Control.mGoOn == 0)
		{			
			int iHeading = int(m_igvInfo.Heading+0.5); // ��ͷ����
			switch(iHeading)
			{
			case 0:	// ��ͷ����
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
			case 90:	// ��ͷ����
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
			case 180:	// ��ͷ����
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
		// ���ĳ��Line�Ķ�������ˣ������preX, preY
		if (Control.mStepEnable == 0 && Control.mLine == 50)
		{
			m_igvInfo.preX = m_igvInfo.curX;
			m_igvInfo.preY = m_igvInfo.curY;
//m_ofsLog2<<"ĳ��Line�Ķ�������ˣ��˴���������2��curx="<<curX<<",cury="<<curY<<",prex="<<preX<<",prey="<<preY<<"\n";
		}

	}
}

// ����һ�µ�ǰλ�ã����磬�ǲ�������һ�������ı�̫Զ�ˣ�������Line������̫Զ��
// ����ǣ���С������ͣ�����ұ���
void CMicroMecanumDlg::ValidatePosition()
{
	int iDirection = Line[Control.mExeLine].Derection; // �˶�����
	// ������ڽ���ǰ�������ˡ����ơ��������ֶ���֮һ
	if (iDirection ==0 || iDirection == 90 || iDirection == 180 || iDirection == 270)
	{
		// �������ִ��line���Ҳ�������ͣ״̬
		if (Control.mLineEnable != 0 && Control.mPause == 0 && Control.mGoOn == 0)
		{
			if (nCheckPoints > 0)
			{
				double dis1	= fabs(m_igvInfo.curX-CheckMarks[nCheckPoints-1].x) + fabs(m_igvInfo.curY-CheckMarks[nCheckPoints-1].y);
				double dis2 = fabs(m_igvInfo.curX-m_igvInfo.preX) + fabs(m_igvInfo.curY-m_igvInfo.preY);
				if (dis1 > MAX_NotFindMarkDistance && dis2 > MAX_NotFindMarkDistance)
				{
m_ofsLog2 << "--̫Զ����δ���ֵر꣬���ϴη��ֵر�"<<(char*)(_bstr_t)CheckMarks[nCheckPoints-1].barCode.GetBuffer(0)
						<<"�ľ����ǣ�"<<dis1<<"���ף����ϴμ�¼�����ľ����ǣ�"<<dis2<<"���ף����Զ�ͣ����\n";
					
					OnButton1();	// ����ֹͣ
				}
			}
		}
		
	}
}

// ��������·������Num_SearchLines����
// Ŀǰ����450mm*250mm�ķ�Χ����������ͷ����150mm*50mm��
// �ú��������ˣ���Ϊ���ļ� 2016-3-7 qi
void CMicroMecanumDlg::CalculateSearchLines()
{
	// ��������·��
	SearchLines[0] = GetMotionParameters(0, 80);	// ����8cm
	SearchLines[1] = GetMotionParameters(180, 160);	// ��18cm
	SearchLines[2] = GetMotionParameters(270, 80);	// ��8	
	SearchLines[3] = GetMotionParameters(0, 160);	// ��16 
	SearchLines[4] = GetMotionParameters(90, 160);	// ��16
	SearchLines[5] = GetMotionParameters(180, 160);	// ��16
}

// ��ȡ�ر�����·���ļ�
void CMicroMecanumDlg::ReadSearchLines()
{
	ifstream fs;

	fs.open("searchlines.txt", ios::nocreate);//Ĭ���� ios::in �ķ�ʽ���ļ����ļ�������ʱ����ʧ��

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

// ��ʱ�����˷���С��ʵʱ��Ϣ
void CMicroMecanumDlg::SendIGVRealTimeInfo()
{

	time_t t = time( 0 ); 
	char tmp[64]; 
	strftime( tmp, sizeof(tmp), "%X",localtime(&t) );

	ofstream m_ofsLog3;   //��־�ļ���
	Json::Value msg;// ��������
	Json::Value msgHead;
	Json::Value msgBody;

	char mac_address[1024];
	//GetMacaddress((char*)mac_address);	
	if(GetMacaddress(mac_address)<=0)
    {
        // д����־����ȡMAC��ַʧ�ܣ��޷���¼��
		m_ofsLog3.open("log-3����Socket�ͽ�����Ϣ.txt",ios::app);
		m_ofsLog3<<tmp<<"����С��ʵʱ��Ϣʱ����ȡMAC��ַʧ��\n"; 	
		m_ofsLog3.close();
		return;
    }
	std::string mac(mac_address); 
	msgHead["KeyId"] = mac;  // mac��ַ
	msgHead["MessageType"] = "2";    // ��Ϣ����
	msg["MessageHead"] = msgHead;
	
	msgBody["X"] = int(m_igvInfo.curX);
	msgBody["Y"] = int(m_igvInfo.curY);
	msgBody["Speed"] = (m_igvInfo.Speed);
	msgBody["Direction"] = int(m_igvInfo.Heading);
	msgBody["IsFree"] = m_igvInfo.isFree?"true":"false";
	msgBody["IsMarked"] = m_igvInfo.isMarked?"true":"false";
	msgBody["MarkId"] = (char*)(_bstr_t)m_igvInfo.MarkID.GetBuffer(0);	

//ofstream ofsLog;				
//ofsLog.open("log-4 isFree״̬����.txt",ios::app);
//ofsLog<<"����Ϣ,MarkID: "<<(char*)(_bstr_t)m_igvInfo.MarkID.GetBuffer(0)<<"\n";
//ofsLog.close();

	Json::FastWriter writer;  // ��Json::FastWriter ������ json Ӧ��������
	std::string strMsgBody = writer.write(msgBody);
	msg["MessageBody"] = strMsgBody;	

	m_ofsLog3.open("log-3����Socket�ͽ�����Ϣ.txt",ios::app);
	m_ofsLog3<<tmp<<"--׼������С��ʵʱ��Ϣ\n"; 	
	m_ofsLog3.close();

	std::string msg2server = writer.write(msg);
	SendMessage2Server((char*)msg2server.c_str());	
}
 
// ���͹켣У׼��ص����ݵ���̨
void CMicroMecanumDlg::SendIGVTrackFixInfo(CString markID1, int x1, int y1, double deltah1, 
		double deltax1, double deltay1, double speed1, double x2, double y2,
		double k2, CString markID3, int x3, int y3, double deltah3, double deltax3, double deltay3,int heading, int motionDirection)
{
	Json::Value msg;// ��������
	Json::Value msgHead;
	Json::Value msgBody;
//char* temp_MarkId1;
//char* temp_MarkId3;
	char mac_address[1024];
	//GetMacaddress((char*)mac_address);	
	if(GetMacaddress(mac_address)<=0)
    {
        // д����־����ȡMAC��ַʧ�ܣ��޷���¼��
		return;
    }
	std::string mac(mac_address); 
	msgHead["KeyId"] = mac;  // mac��ַ
	msgHead["MessageType"] = "3";    // ��Ϣ����
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

	Json::FastWriter writer;  // ��Json::FastWriter ������ json Ӧ��������
	std::string strMsgBody = writer.write(msgBody);
	msg["MessageBody"] = strMsgBody;

	std::string msg2server = writer.write(msg);
	SendMessage2Server((char*)msg2server.c_str());	

ofstream ofsLog;				
ofsLog.open("log-6 �켣У׼��¼.txt",ios::app);

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
// ������
ofsLog<<"DelatH1  "<<"DelatH3  "<<"DelatX1  "<<"DelatY1  "<<"Heading  "<<"K2  "<<"MarkId1  ";
ofsLog<<"MarkId3  "<<"MotionDirection  "<<"Speed1  "<<"X1  "<<"X2  "<<"X3  "<<"Y1  "<<"Y2  "<<"Y3"<<"\n";*/


// ����
ofsLog<<deltah1<<"    "<<deltah3<<"    "<<deltax1<<"    "<<deltay1<<"    "<<deltax3<<"    "<<deltay3<<"    "<<heading<<"    ";
ofsLog<<k2<<"    "<<Temp_MarkId1<<"    "<<Temp_MarkId3<<"    "<<motionDirection<<"    "<<speed1<<"    ";
ofsLog<<x1<<"    "<<x2<<"    "<<x3<<"    "<<y1<<"    "<<y2<<"    "<<y3<<"\n";

//ofsLog<<(char*)strMsgBody.c_str()<<"\n";

ofsLog.close();
	
}

// socket���ӳɹ�ʱ���͵�¼��Ϣ�������
void CMicroMecanumDlg::SendIGVLoginMessage()
{
	//GetDlgItem(IDC_STATIC_Connect)->SetWindowText("������");
	//SetDlgItemText(IDC_STATIC_Connect, "������");
	m_ConnectStatus.SetWindowText("������");
	ofstream m_ofsLog3;   //��־�ļ���
	Json::Value msg;// ��������
	Json::Value msgHead;
	Json::Value msgBody;

	char mac_address[1024];
	//GetMacaddress((char*)mac_address);	
	if(GetMacaddress(mac_address)>0)
    {
        // ��ȡMAC��ַ�ɹ�
		m_ofsLog3.open("log-3����Socket�ͽ�����Ϣ.txt",ios::app);
		m_ofsLog3<<"��ȡMAC��ַ�ɹ�\n"; 		
		m_ofsLog3.close();
    }
	else
    {
        // д����־����ȡMAC��ַʧ�ܣ��޷���¼��
		m_ofsLog3.open("log-3����Socket�ͽ�����Ϣ.txt",ios::app);
		m_ofsLog3<<"��ȡMAC��ַʧ�ܣ��޷���¼\n"; 	
		m_ofsLog3.close();
		return;
    }
	std::string mac(mac_address); 
	msgHead["KeyId"] = mac;  // mac��ַ
	msgHead["MessageType"] = "0";    // ��Ϣ����

	msg["MessageHead"] = msgHead;
	msg["MessageBody"] = NULL;

	Json::FastWriter writer;  // ��Json::FastWriter ������ json Ӧ��������
	std::string msg2server = writer.write(msg); 
	SendMessage2Server((char*)msg2server.c_str());	
}

void CMicroMecanumDlg::SendMessage2Server(char* msg)
{	
	//msg = "00000";
	ofstream m_ofsLog3;   //��־�ļ���
	if(send(m_Sock, msg, strlen(msg), 0) == SOCKET_ERROR) //�жϷ����Ƿ�ɹ�
	{
		 //MessageBox("����ʧ��");
		m_ofsLog3.open("log-3����Socket�ͽ�����Ϣ.txt",ios::app);
		m_ofsLog3<<"������Ϣʧ��\n"; 
		m_isSocketConnected = false;
		// GetDlgItem(IDC_STATIC_Connect)->SetWindowText("δ����");
		m_ConnectStatus.SetWindowText("δ����");
		m_ofsLog3.close();
	}
	else if(msg == "")   //�жϷ��͵������Ƿ�Ϊ��
	{
		 //MessageBox("��������Ϣ");
		m_ofsLog3.open("log-3����Socket�ͽ�����Ϣ.txt",ios::app);
		m_ofsLog3<<"������ϢΪ��\n"; 		
		m_ofsLog3.close();
	}
	else //���ͳɹ�
	{
		m_ofsLog3.open("log-3����Socket�ͽ�����Ϣ.txt",ios::app);
		m_ofsLog3<<"������Ϣ�ɹ�\n"; 	
		m_ofsLog3.close();
	}		
}
 
int CMicroMecanumDlg::GetMacaddress(char* mac) //��ȡ����MACַ 
{
	ULONG ulSize=0;
	PIP_ADAPTER_INFO pInfo=NULL;
	int temp=0;
	temp = GetAdaptersInfo(pInfo,&ulSize);//��һ�����ã���ȡ��������С
	pInfo=(PIP_ADAPTER_INFO)malloc(ulSize);
	temp = GetAdaptersInfo(pInfo,&ulSize);

	int iCount=0;
	//while(pInfo)//����ÿһ������
	{
		//  pInfo->Address MACַ
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


// �յ���Ϣ��Ĵ�����
void CMicroMecanumDlg::Command2Route(char* sMsg)	
{
	MotionParemeter m_motionPar;

	// ��λ·������ָ��
	Control.mMaxLine = 0;	
	
	// ��ȡ�յ�����Ϣ
	Json::Reader *reader = new Json::Reader(Json::Features::strictMode());  
	Json::Value root;
	Json::Value arraymember1;
	if (reader->parse(sMsg, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��
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

			// 1.�˶�����
			Line[Control.mMaxLine].Derection	= m_motionPar.Derection;			
			// 2.Ŀ�����
			Line[Control.mMaxLine].AidDistance	= m_motionPar.AidDistance;	
			// 3.�˶�����
			Line[Control.mMaxLine].MotionType	= LineRips;
			// 4.�ٶȱ���
			Line[Control.mMaxLine].SpeedRate	= sRate;
			// 5.�����ٶ�
			Line[Control.mMaxLine].StartSpeed	= m_motionPar.StartSpeed;
			// 6.����ٶ�
			Line[Control.mMaxLine].MaxSpeed		= m_motionPar.MaxSpeed;
			// 7.�Ӽ��ٶ�
			Line[Control.mMaxLine].ACC			= m_motionPar.ACC;	
			// 8.�����ٶ�
			Line[Control.mMaxLine].DACC			= m_motionPar.DACC;
			// 9.���پ���
			Line[Control.mMaxLine].ReducePoint	= 0;
			// 10.��������
			Line[Control.mMaxLine].StartType	= STAUD;
			// 11.У������
			Line[Control.mMaxLine].CaliDistance	= m_motionPar.CaliDistance;
			// 12.΢������	
			Line[Control.mMaxLine].MicroDistance = m_motionPar.MicroDistance;
			// 13.X����Ŀ��
			Line[Control.mMaxLine].AidX			= m_motionPar.AidX;
			// 14.Y����Ŀ��
			Line[Control.mMaxLine].AidY			= m_motionPar.AidY;
			// 15.Z����Ŀ��
			Line[Control.mMaxLine].AidZ			= m_motionPar.AidZ;
			// 16.U����Ŀ��	
			Line[Control.mMaxLine].AidU			= m_motionPar.AidU;	
					
			//����������
			Control.mMaxLine++;		
		}	  
	}
	else
	{
	}
	delete reader;
 }

BOOL CMicroMecanumDlg::Deknock(void)			// ����ײ��� 
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
		// �����ͷ���ײ���
		if (GpioOperation(&m_Device,GpioRead,XADDR,Pin0,1) == GpioLow)
		{
			// ���˿ڵ�ƽ����,�����˶��������ϰ���,���̼���ͣ��
			Stop(&m_Device,SDSTP);	// STOP / SDSTP ����ֹͣ;
			StopMotion();			// ֹͣ΢���˶�

			MechineSwitchDown = 0;
			// �رյ����Դ
			MotorPowerOFF();
		}
		else
		{
			if (MechineSwitchDown == 0)
			{
				MechineSwitchDown = 1;
				// �ָ��������
				MotorPowerON();
			} 
		}

	}
/*	
	// ��е���ط���ײ���
	if (GpioOperation(&m_Device,GpioRead,XADDR,Pin1,1) == GpioLow)
	{
		MechineSwitchDown = 0;
		// �رյ����Դ
		MotorPowerOFF();
	}
	else
	{
		if (MechineSwitchDown == 0)
		{
			MechineSwitchDown = 1;
			// �ָ��������
			MotorPowerON();
		} 
	}

	// �Ƿ����ƽ̨�˶�	
	if (PlatMotionEnable == TRUE)
	{
		// Ҫ��ƽ̨�����λ
		if (GpioOperation(GpioRead,ZADDR,PlatformPower,GpioHigh) == GpioLow)
		{
			//1#���ִ�ж���
			if ((Dlg->GpioOperation(GpioRead,YADDR,Pin1,GpioHigh) == GpioHigh)		//������͵�
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin5,GpioHigh) == GpioLow))	//1#��������˶�
			{
				// �ر�1#�����Դ
				Dlg->GpioOperation(GpioWrite,UADDR,Pin5,GpioHigh);
			}
			//2#���ִ�ж���
			if ((Dlg->GpioOperation(GpioRead,XADDR,Pin2,GpioHigh) == GpioHigh)		//������͵�
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin4,GpioHigh) == GpioLow))	//2#��������˶�
			{
				// �ر�2#�����Դ
				Dlg->GpioOperation(GpioWrite,UADDR,Pin4,GpioHigh);
			}
			//����½�������������͵�
			if ((Dlg->GpioOperation(GpioRead,YADDR,Pin1,GpioHigh) == GpioHigh)		//1#������͵�
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin5,GpioHigh) == GpioHigh)	//1#���ֹͣ�˶�
				&& (Dlg->GpioOperation(GpioRead,XADDR,Pin2,GpioHigh) == GpioHigh)	//2#������͵�
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin4,GpioHigh) == GpioHigh))	//2#���ֹͣ�˶�
			{
				// ��ֹƽ̨�˶�
				PlatMotionEnable = FALSE;
			}
		} 
		else	// Ҫ��ƽ̨�����λ
		{
			//1#���ִ�ж���
			if ((Dlg->GpioOperation(GpioRead,YADDR,Pin0,GpioHigh) == GpioHigh)		//������͵�
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin5,GpioHigh) == GpioLow))	//1#��������˶�
			{
				// �ر�1#�����Դ
				Dlg->GpioOperation(GpioWrite,UADDR,Pin5,GpioHigh);
			}
			//2#���ִ�ж���
			if ((Dlg->GpioOperation(GpioRead,YADDR,Pin2,GpioHigh) == GpioHigh)		//������͵�
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin4,GpioHigh) == GpioLow))	//2#��������˶�
			{
				// �ر�2#�����Դ
				Dlg->GpioOperation(GpioWrite,UADDR,Pin4,GpioHigh);
			}
			//����½�������������͵�
			if ((Dlg->GpioOperation(GpioRead,YADDR,Pin0,GpioHigh) == GpioHigh)		//1#������͵�
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin5,GpioHigh) == GpioHigh)	//1#���ֹͣ�˶�
				&& (Dlg->GpioOperation(GpioRead,YADDR,Pin2,GpioHigh) == GpioHigh)	//2#������͵�
				&& (Dlg->GpioOperation(GpioRead,UADDR,Pin4,GpioHigh) == GpioHigh))	//2#���ֹͣ�˶�
			{
				// ��ֹƽ̨�˶�
				PlatMotionEnable = FALSE;
			}		
		}
	}
	*/
	return TRUE;
}

BOOL CMicroMecanumDlg::MotorPowerON(void)		// �򿪵����Դ
{
	GpioOperation(&m_Device,GpioWrite,ZADDR,Pin7,GpioLow);
	return TRUE;
}
BOOL CMicroMecanumDlg::MotorPowerOFF(void)		// �رյ����Դ
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
	m_ConnectStatus.SetWindowText("�ѵ�¼");
	return true;
}



//����̺߳���server_thd()��SSS
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
	 memset(msg_rcv,0,1024*sizeof(char)); //lipan ��ʼ���ַ���
	 CString port;
	 CMicroMecanumDlg * dlg = (CMicroMecanumDlg *)AfxGetApp()->GetMainWnd(); //�õ�Ӧ�ó��������ڵ�ָ��
	 //dlg->port_edit->GetWindowText(port);//��ö˿ڵ�ַ
	 port="5009";
	 if(port=="") 
	 {
		 showeditmsg = "������˿ں�\r\n";
		 dlg->showmsg_edit.SetWindowText(showeditmsg);
		 return 0;
	 }
	 //Ϊlocal_addr��ֵ������soket
	 local_addr.sin_family = AF_INET;
	 local_addr.sin_port = htons(atoi(port)); //atoi�� ���ַ���ת����������
	 local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	 //��������listen_sock
	 if( (listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET )
	 {
		 showeditmsg = "��������ʧ��\r\n";
		 dlg->showmsg_edit.SetWindowText(showeditmsg);

		 return 0;
	 }
	 //��
	 if( bind(listen_sock, (struct sockaddr*) &local_addr, sizeof(SOCKADDR_IN)) )
	 {
		 showeditmsg = "�󶨴���,��һ���˿�����\r\n";
		 dlg->showmsg_edit.SetWindowText(showeditmsg);
		 return 0;
	 }
	 
	 listen(listen_sock, 1); //��ʼ������������������Ϊ1
	 dlg->star_button.EnableWindow(FALSE);  //������������ť�һ�
	 dlg->showmsg_edit.ShowWindow(SW_SHOW); //����Ϣedit��ʾ
	 showeditmsg = "�ѳɹ�����....\r\n";
	 dlg->showmsg_edit.SetWindowText(showeditmsg);
	 
	 //������������
	 if( (sock = accept(listen_sock, (struct sockaddr *)&client_addr, &iaddrSize)) == INVALID_SOCKET)
	 {
		 showeditmsg = "������������ʧ��\r\n";
		 dlg->showmsg_edit.SetWindowText(showeditmsg);
		 return 0;
	 }
	 else
	 {
		 CString port;
		 port.Format("%d", int(ntohs(client_addr.sin_port)));
		 showeditmsg = "���������ԣ�" + CString(inet_ntoa(client_addr.sin_addr)) + "  �˿�:5009\r\n";
		 dlg->showmsg_edit.SetWindowText(showeditmsg);
	 }
			 
	 ////////////-------��������
	 while(1)
	 {
		 MsgHandling(msg_rcv);				//lipan_�Է�����Ϣ���й淶����ֻ�������µ�һ����Ϣ
		 if( (res = recv(sock, msg_rcv, 1024, 0)) == -1 ) //�������ݣ��ж��Ƿ���ճɹ�
		 {
			showeditmsg = "-------ʧȥ����\r\n";
			dlg->showmsg_edit.SetWindowText(showeditmsg);
			dlg->star_button.EnableWindow(TRUE);
			break;
		 }
		 else
		 {
			showeditmsg = "�ͻ���:" + ((CString)(msg_rcv)).Mid(0,res) + "\r\n";
			dlg->showmsg_edit.SetWindowText(showeditmsg);

			// ��ȡ�յ�����Ϣ
			Json::Reader *reader = new Json::Reader(Json::Features::strictMode());  
			Json::Value root;
			if (reader->parse(msg_rcv, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	WSADATA wsData;  
	WSAStartup(MAKEWORD(2,2), &wsData); 

	char name[80];
	CString IP;
	CString P;
	hostent* pHost; 
	gethostname(name,sizeof(name));//��������� 
	pHost = gethostbyname(name);//��������ṹ	 
	IP = inet_ntoa(*(struct in_addr *)pHost->h_addr_list);//��ȡ����ip��ַ
	showmsg_edit.SetWindowText("��IP��ַ��" + IP);
	AfxBeginThread(&server_thd,0); //�������̴߳���

}

void CMicroMecanumDlg::OnButton_PauseAndRecovery()
{
	// TODO: ��ͣ���ܲ���
	ofstream m_ofsLog8;
	m_ofsLog8.open("log_8 С����ͣ��ָ����ܲ���.txt",ios::app);
	if (FlagPause == 0)
	{
		Control.mPause = 0XFF;	// ������ͣ
		Control.mGoOn = 0XFF;	// ��ֹ����
		GetDlgItem(IDC_BUTTON_PauseAndRecovery)->SetWindowText("Recovery");
		m_ofsLog8<<"IDC:    "<<IDC_BUTTON_PauseAndRecovery<<" ���Line�ĸ��� "<<Control.mMaxLine<<endl;
		m_ofsLog8<<"������ͣ״̬      FlagPause = "<<FlagPause;
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
		m_ofsLog8<<"IDC:    "<<IDC_BUTTON_PauseAndRecovery<<" ���Line�ĸ��� "<<Control.mMaxLine<<endl;
		m_ofsLog8<<"����ָ�״̬    Flagpause = "<<FlagPause;
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
	m_ofsLog9.open("log_9 С��EmergencyBreak����.txt",ios::app);

	CString str;
	m_EmergencyBrake.GetWindowText(str);
	if (str == "�����ƶ�")
	{	
		CurLineNo  = Control.mExeLine;

		FindBarEnable		= FALSE;			// ��ֹ�Զ�ʶ������
		Control.mStep		= 0;				// ��λ����ָ��
		Control.mExeStep	= 0;				// ��λ΢��ָ��
		Control.mStepEnable = 0;				// ��λ�����Զ�����
		Control.mFixStep = 0;					// ��λ�Զ�У������
		
		Control.mPause		= 0;				// ��λ��ͣ���
		Control.mGoOn		= 0;				// ��λ����·���˶����
		Control.mRevers		= 0;				// ��λ�������
		
		Control.mLine		= 0;				// ��λ���β���ָ��
		Control.mExeLine	= 0;				// ��λ����ִ��ָ��
		Control.mLineEnable = 0;				// ��λ�������
		Stop(&m_Device,STOP);

		m_ofsLog9<<"��ǰLine    mExeLine = "<<Control.mExeLine<<" ";
		m_ofsLog9<<"���Line    mMaxLine = "<<Control.mMaxLine<<"\n";

		m_EmergencyBrake.SetWindowText("�ָ�");
	} 
	else
	{
		Control.mMaxLine = Control.mMaxLine - CurLineNo;				    // �������Line��ֵ
	
		m_ofsLog9<<"�ָ�������Line    mMaxLine = "<<Control.mMaxLine<<"\n";		
		
		// ��¼���ߵľ���
		INT32 xLocation = INT32(DOUBLE(GetShiftR(&m_Device,XADDR)) * Puls2Distance);	// �����Ѿ����ߵľ���
		Temp_Line[CurLineNo].AidDistance = Temp_Line[CurLineNo].AidDistance - ((INT32)abs(xLocation));
		
		memset(Line, 0, sizeof(struct MotionParemeter)*mTotal); // ���
		
		//����Line
		for (int i = 0; i<Control.mMaxLine; i++)
		{
			Line[i] = Temp_Line[CurLineNo];
			CurLineNo++;
		}

		// ��ʼ�˶�
		Control.mStep = 0;				// ��λ����ָ��
		Control.mExeStep = 0;			// ��λ΢��ָ��
		Control.mStepEnable = 0;		// ��λ�����Զ�����
		Control.mFixStep = 0;			// ��λ�Զ�У������
		
		Control.mPause		= 0;		// ��λ��ͣ���
		Control.mGoOn		= 0;		// ��λ����·���˶����
		Control.mRevers		= 0;		// ��λ�������
		
		Control.mLine		= 0;		// ��λ���β���ָ��
		Control.mExeLine	= 0;		// ��λ����ִ��ָ��
		Control.mLineEnable = 0XFF;		// ��ʼִ��΢������
		FindBarEnable		= TRUE;		// �����Զ�ʶ������

		m_EmergencyBrake.SetWindowText("�����ƶ�");
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
		FindBarEnable		= FALSE;			// ��ֹ�Զ�ʶ������
		Control.mStep		= 0;				// ��λ����ָ��
		Control.mExeStep	= 0;				// ��λ΢��ָ��
		Control.mStepEnable = 0;				// ��λ�����Զ�����
		Control.mFixStep = 0;					// ��λ�Զ�У������
		
		Control.mPause		= 0;				// ��λ��ͣ���
		Control.mGoOn		= 0;				// ��λ����·���˶����
		Control.mRevers		= 0;				// ��λ�������
		
		Control.mLine		= 0;				// ��λ���β���ָ��
		Control.mExeLine	= 0;				// ��λ����ִ��ָ��
		Control.mLineEnable = 0;				// ��λ���
	}
}
