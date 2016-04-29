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

	// for ������
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


 	InitializationWindows();		// ��ʼ������	

	//------------------------------------------------
	// qi
	// ��ʼ��У׼�㣨���һ��Line�������֧��1024��

	
	//m_Heading = -1.0;	// С����ʼδ��λ�Ͷ���
	m_HeadingCombBxo.SetCurSel(3);
	m_HeadingCombBxo.UpdateData(TRUE);

	// end qi 
	//------------------------------------------------	
		

	//InitializationCamera();		// ��ʼ�����
	CameraInitional();			// ��ʼ�����

	//------------------------------------------------
	// pc
	//���³�ʼ��ͼ��ƥ�����
	m_Pattern.Load("pattern.bmp");	// ����ģ��
	m_Roi.Attach(&m_Pattern);
	m_Roi.SetPlacement(0,0,268,44);
	m_Match.SetMaxPositions(1);// return only one position
	m_Match.SetMinScore(0.25f);	// ƥ������ٴﵽ0.3
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


void CMicroMecanumDlg::OnButton1() //����ֹͣ/�رյ�Դ
{

	CString str;
	GetDlgItem(IDC_BUTTON1)->GetWindowText(str);
	if (str == "�رյ�Դ")
	{
		GetDlgItem(IDC_BUTTON1)->SetWindowText("�򿪵�Դ");
	} 
	else
	{
		GetDlgItem(IDC_BUTTON1)->SetWindowText("�رյ�Դ");
	}
}

void CMicroMecanumDlg::OnButton2()	//ϵͳ��λ
{
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
}

void CMicroMecanumDlg::OnButton5()	//ͨѶ�趨
{
}

void CMicroMecanumDlg::OnButton6()	//���긴λ(��̬У׼)  
{

}

void CMicroMecanumDlg::OnButton7()	//�����趨
{

}

void CMicroMecanumDlg::OnButton8()	//�Զ����� 
{

}

void CMicroMecanumDlg::OnCancel()	//�رճ���
{

		CDialog::OnCancel();
	
}

void CMicroMecanumDlg::OnButton9()	//�ر�ϵͳ 
{

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
	rect.top	= y / 2 - 240;
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


#define MotionBlock	2 
// mecanum С���˶�����ģ�� ---------------------------------------------------


void CMicroMecanumDlg::InitializationSoket()
{
	// ���ӷ���˷���
	pConnectSocketThread.CreateThread();
	pConnectSocketThread.play();
}


void CMicroMecanumDlg::OnTimer(UINT nIDEvent)	//�˶�����ר�ö�ʱ��  
{
								// �Զ�ͨѶ����ʾ��������
	CDialog::OnTimer(nIDEvent);
}
	

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

	return true;
}


// Camera ͼ����ʾ�߳�------------------------------------------------------------------
// After the process thread generate a bmp file,this function load picture and show it 
LRESULT CMicroMecanumDlg::ProcessImage(WPARAM wParam, LPARAM lParam)
{
	struct PicInformation{	
		long width;
		long height;
		unsigned char *m_image;
	}*info;
	// ����ʱ��������
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
	//TRACE("CameraReady=%d,����ͼ����ʾ�߳�!\n",CameraReady);

	//����Evision��������
	if (CameraReady == TRUE)		// ���׼���������Ž�����һ�ν���
	{		
		//��ͼ��������N��һ����ʾͼ��&����
		ImageProcessFrequence++;				// ͳ�ƽ���ͼ�������
		//TRACE("ImageProcessFrequence=%d,ͼ����!\n",ImageProcessFrequence);
		if (ImageProcessFrequence >= ImageProcessSpace)		// �Ƿ񵽴������ֵ					
		{
			ImageProcessFrequence = 0;
			// ����ͼ��ˢ��+���롷��Ƶ��
			// ������ʱ��ֵ
			//TRACE("TimePoint0=%d!\n",TimePoint0);
			DWORD TimePoint1 =  GetCurrentTime();
			//TRACE("TimePoint1=%d!\n",TimePoint1);

			// ������ʱ��	
			FramePerSecond = TimePoint1 - TimePoint0;
			//TRACE("FramePerSecond=%dmS!\n",FramePerSecond);
			// ��������ʱ��ֵ
			TimePoint0 = TimePoint1;
			// ����ͼ��ˢ��Ƶ��
			FramePerSecond = 1000 / (FramePerSecond+0.1);

			//����Evision����,����ȡͼ����Ϣ
			EBW8Image2/*EC24Image1*/.SetSize(info->width,info->height);			// �趨ͼƬ����	
			//���ڴ���ֱ�ӵ���ͼ�� 
			EBW8Image2/*EC24Image1*/.SetImagePtr(info->width,info->height,info->m_image,0);
			// �������ͼ��
			m_Src = EBW8Image2;
					
			//��ʾͼ��			
			CDC *pDC=NULL;											//����һ���յ�Device Concent
			CRect rect;												//�������
			CWnd *pWnd2 = GetDlgItem(IDC_STATICP1);					//��ȡ�滭����		
			if(pWnd2)												//�����ڴ���	
			{
				pWnd2->GetClientRect(&rect);						//�趨�滭����
				pDC = pWnd2->GetDC();								//�趨��ͼĿ���豸				
				BarDecoder();					//����Evision����	
				m_Src.Draw(pDC->m_hDC);			//�Ѳɼ���ͼ����ʾ�ڽ�����Draw the image
				//��ʾͼ��ķֱ�����Ϣ
				CString PictureIfo = "";
				PictureIfo.Format("W%d X H%d",info->width,info->height);
				BarDecodeInfo += PictureIfo;
						
				CPen pen3; 											// �Զ��廭��
				pen3.CreatePen(PS_SOLID,1,RGB(0,255,0));			// ��������
				CPen* pOldPen3= pDC->SelectObject(&pen3);			// Select a green pen

				m_BarCode.Draw(pDC->GetSafeHdc(), INS_DRAW_ACTUAL);	// Draw the found barcode frame
				pDC->TextOut(10, 460, BarDecodeInfo, strlen(BarDecodeInfo));	// ��ʾ������Ϣ
				pDC->SelectObject(&pOldPen3);						// Put back old pen	
		
				ReleaseDC(pDC);										// �ͷŻ�ͼĿ���豸	


			}
			//��ͼ����ͬ����ʾ�ַ���
//			DrawArrow(mARROW);

		}
	}
	delete []info->m_image;
	// info->m_image=NULL;	
	return true;
}

// ���ģ�� -------------------------------------------------------------------------------------------------------

#define EvisionBlock	5 
// �Ӿ�ģ�� -------------------------------------------------------------------------------------------------------

// �������ܰ�������Evision�⺯������һ��C24ͼ��תΪBW8��ʽ�����룬�����ı�����ʾ
void CMicroMecanumDlg::BarDecoder()						// ����Evision����
{	
	BarDecodeInfo = "";
	FinishedBarDecode = FALSE;					// ��ʼ����						
	DWORD TimePoint0 =  GetCurrentTime();		// �����뿪ʼʱ���

	bool isDecodeSuccess = false;
	CString BarCodeStr = ReadBar();				// ������
	if (BarCodeStr.GetLength() == 5)
		isDecodeSuccess = true;
	
	DWORD TimePoint1 =  GetCurrentTime();		// ���������ʱ���	
	DWORD TimeCost = TimePoint1 - TimePoint0;	// �������ܼƻ���ʱ��	
	
	time_t t = time( 0 ); 			
	strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A",localtime(&t) ); 
	ofstream m_ofsLog10;
	m_ofsLog10.open("log-10��¼�����ĵر�.txt",ios::app);
	// ���������Ϣ��ȷ
	if (isDecodeSuccess)
	{
		// -----------��¼�����ĵر�--pc	
		if(Temp_LastBarCodeid == BarCodeStr.GetBuffer(0))
		{}
		else
		{
			m_ofsLog10<<tmp<<"  "<<BarCodeStr.GetBuffer(0)<<"\n"; 
			Temp_LastBarCodeid = BarCodeStr.GetBuffer(0);
		}
		// -----------��¼�����ĵر����
	}
	else	// ���벻��ȷ
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
			else // ����ɹ�
			{
				// ������Ļ���ص�ʵ�ʾ��뻻��ϵ������λ������/����
				double CaliK = BarLenth/m_BarCode.GetSizeX();
				// ��ȡ��������X������
				BarXlocation = (m_BarCode.GetCenterX() - m_Src.GetWidth()/2) * CaliK;
				// ��ȡ��������Y������
				BarYlocation = (m_Src.GetHeight()/2 - m_BarCode.GetCenterY()) * CaliK;
				// ��ȡ����X�����᳤��
				BarXlenth = m_BarCode.GetSizeX() * CaliK;
				// ��ȡ����Y�����᳤��
				BarYlenth = m_BarCode.GetSizeY() * CaliK;
				// ��ȡ����ƫת�Ƕ�	
				BarAngle = m_BarCode.GetAngle();

				char s[256];
				sprintf(s, " ,x=%5.2f, y=%5.2f, angle=%6.2f\n", BarXlocation, BarYlocation, BarAngle);
				BarDecodeInfo = "����ɹ���barcode=" + bar + s;
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


// �Ӿ�ģ�� -------------------------------------------------------------------------------------------------------



void CMicroMecanumDlg::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	CString s;
	m_HeadingCombBxo.GetLBText(m_HeadingCombBxo.GetCurSel(),s);
}


// �ж�һ������ĸ�λ�Ƿ���0-9������
BOOL CMicroMecanumDlg::CheckBarcode(CString bar)
{
	// ��֤����
	if (bar.GetLength() != BarCodeNumber)
	{
		return false;
	}
	// ��֤�ַ�
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
 
// ���͹켣У׼��ص����ݵ���̨
void CMicroMecanumDlg::SendBarCodeInfo(int isSuc, CString barID, double x, double y, double angle)
{
	Json::Value msgBody;
	msgBody["isSuccess"] = isSuc;
	msgBody["BarCodeID"] = (char*)(_bstr_t)barID.GetBuffer(0);
	msgBody["CenterX"] = x;
	msgBody["CenterY"] = y;
	msgBody["Angle"] = angle;

	Json::FastWriter writer;  // ��Json::FastWriter ������ json Ӧ��������
	std::string strMsgBody = writer.write(msgBody);
	SendMessage2Server((char*)strMsgBody.c_str());		
}

// socket���ӳɹ�ʱ���͵�¼��Ϣ������ˣ���ʱδ��,
void CMicroMecanumDlg::SendIGVLoginMessage()
{
	
}

void CMicroMecanumDlg::SendMessage2Server(char* msg)
{	
	//msg = "00000";	
	// ��ȡ��ǰϵͳʱ��
	time_t t = time( 0 ); 			
	strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A",localtime(&t) ); 

	ofstream m_ofsLog11;
	m_ofsLog11.open("log-11������Ϣ��Motion.txt",ios::app);
	if(send(m_Sock, msg, strlen(msg), 0) == SOCKET_ERROR) //�жϷ����Ƿ�ɹ�
	{		
		m_ofsLog11<<tmp<<"------------������Ϣʧ��\n"; 
		m_isSocketConnected = false;	
		m_ConnectStatus.SetWindowText("δ��Motion����");		
	}
	else if(msg == "")   //�жϷ��͵������Ƿ�Ϊ��
	{
		m_ofsLog11<<tmp<<"------------������ϢΪ��\n"; 	
	}
	else //���ͳɹ�
	{	
		m_ofsLog11<<tmp<<"------------������Ϣ�ɹ�\n"; 	
	}	
	m_ofsLog11.close();
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


LRESULT CMicroMecanumDlg::ProcessConnectSuccess(WPARAM wParam, LPARAM lParam)
{
	m_ConnectStatus.SetWindowText("������");
	return true;
}

LRESULT CMicroMecanumDlg::ProcessLoginSuccess(WPARAM wParam, LPARAM lParam)
{
	m_ConnectStatus.SetWindowText("�ѵ�¼");
	return true;
}
