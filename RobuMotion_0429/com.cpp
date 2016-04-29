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
	//�������ò���
	if(!ZjCom.Create(NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,IDC_MSCOMM1))  
	{  
		AfxMessageBox("����MSComm�ؼ�ʧ��!"); 
	}  
	else									//�����ɹ���ִ�г�ʼ�����򿪴��ڲ���  
	{  
		ZjCom.SetCommPort(1);				//ָ�����ں�
		//ZjCom.SetCommPort(7);				//ָ�����ں� 		
		//ZjCom.SetCommPort(8);				//ָ�����ں� 		
		ZjCom.SetSettings("9600,N,8,1");	//ͨ�Ų�������
		//ZjCom.SetSettings("115200,N,8,1");//ͨ�Ų������� 		
		//ZjCom.SetSettings("19200,N,8,1");	//ͨ�Ų�������		
		ZjCom.SetInBufferSize(1024);		//ָ�����ջ�������С  
		ZjCom.SetInBufferCount(0);			//��ս��ջ�����  
		ZjCom.SetInputMode(1);				//�������ݻ�ȡ��ʽ  
		ZjCom.SetInputLen(0);				//����ÿ�ζ�ȡ���ȣ�0Ϊȫ����ȡ  
		ZjCom.SetRThreshold(16);			//���ý���OnComm�¼��ַ�����ֵ���յ�2�ֽ�ʱ�򴥷��¼�  
		ZjCom.SetPortOpen(1);				//�򿪴���  
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
