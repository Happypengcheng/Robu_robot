// Com.cpp : implementation file
//

#include "stdafx.h"
#include "MicroMecanum.h"
#include "Com.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCom dialog


CCom::CCom(CWnd* pParent /*=NULL*/)
	: CDialog(CCom::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCom)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCom)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCom, CDialog)
	//{{AFX_MSG_MAP(CCom)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCom message handlers

void CCom::OnButton1() 
{
	//串口设置操作
	if(!ZjCom.Create(NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,IDC_MSCOMM1))  
	{  
		AfxMessageBox("创建MSComm控件失败!"); 
	}  
	else									//创建成功则执行初始化及打开串口操作  
	{  
		ZjCom.SetCommPort(1);				//指定串口号
		//ZjCom.SetCommPort(7);				//指定串口号 		
		//ZjCom.SetCommPort(8);				//指定串口号 		
		ZjCom.SetSettings("9600,N,8,1");	//通信参数设置
		//ZjCom.SetSettings("115200,N,8,1");//通信参数设置 		
		//ZjCom.SetSettings("19200,N,8,1");	//通信参数设置		
		ZjCom.SetInBufferSize(1024);		//指定接收缓冲区大小  
		ZjCom.SetInBufferCount(0);			//清空接收缓冲区  
		ZjCom.SetInputMode(1);				//设置数据获取方式  
		ZjCom.SetInputLen(0);				//设置每次读取长度，0为全部读取  
		ZjCom.SetRThreshold(16);			//设置接收OnComm事件字符门限值在收到2字节时候触发事件  
		ZjCom.SetPortOpen(1);				//打开串口  
	}
	
	CString str,str1,str2;
	str.Format(_T("%d"), ZjCom.GetCommPort());			//Port Nuber to str display
	str = " COM" + str + " ," + ZjCom.GetSettings();	//Port par to str display	
}

void CCom::OnButton2() 
{
	// TODO: Add your control notification handler code here
	
}

void CCom::OnButton3() 
{
	// TODO: Add your control notification handler code here
	
}

void CCom::OnButton4() 
{
	// TODO: Add your control notification handler code here
	
}

void CCom::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
