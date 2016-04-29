// ProcessThread.cpp : implementation file
//

#include "stdafx.h"
#include "MicroMecanum.h"
#include "MicroMecanumDlg.h"
//#include "Demo.h"
//#include"DemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CAMERA_IMAGE_RAW8    0x1
#define CAMERA_IMAGE_GRAY8   0x2
//#define CAMERA_IMAGE_RGB24   0x3

/////////////////////////////////////////////////////////////////////////////
// CProcessThread
//////////////////////传递消息用的结构体//////////////////////////////////////
struct PicInformation{
	long width;
	long height;
	unsigned char *m_image;
};
IMPLEMENT_DYNCREATE(CProcessThread, CWinThread)

CProcessThread::CProcessThread()
{
	work=false;
	m_ProcessIndex=0;
}

CProcessThread::~CProcessThread()
{

}

int CProcessThread::Run()
{
	while(1)
	{	
		ofstream m_ofsLog8;   //日志文件流，用于储存接收的消息
		m_ofsLog8.open("log-8读取图像消息.txt",ios::app);
		if(work)
		{  
			int m_width=0, m_height=0, len=0;
			CameraGetImageSize(m_ProcessIndex,&m_width, &m_height);
			CameraGetImageBufferSize(m_ProcessIndex,&len, CAMERA_IMAGE_GRAY8/*CAMERA_IMAGE_BMP*/);	
			unsigned char *m_inBuf = new unsigned char[len];
		
		    if(CameraQueryImage(m_ProcessIndex,m_inBuf, &len, CAMERA_IMAGE_GRAY8/*CAMERA_IMAGE_BMP*/)==API_OK)			
			{	
				// 获取当前系统时间
				time_t t = time( 0 ); 			
				strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A",localtime(&t) );	
				m_ofsLog8<<tmp<<"--------------读取图像成功\n"; // 接收的消息

		    	//CameraVertFlipBuf(m_inBuf,m_width*3,m_height);
				// draw lines 
			/*	for (int i= 100; i<=120; i++)
				{
					for (int j=0; j<m_width; j++)
					{
						*(m_inBuf+(i*m_width+j)*3+0) = 0x00;
						*(m_inBuf+(i*m_width+j)*3+1) = 0x00;
						*(m_inBuf+(i*m_width+j)*3+2) = 0xff;
					}
				}*/

				PicInformation tempInformation;
				tempInformation.m_image=m_inBuf;
				tempInformation.width=m_width;
				tempInformation.height=m_height;
				CMicroMecanumDlg *hwnd=(CMicroMecanumDlg * )AfxGetMainWnd();  
				hwnd->SendMessage(WM_PROCESS_MESSAGE,0,(LPARAM)&tempInformation);
				
			}
			else
			{
				// 获取当前系统时间
				time_t t = time( 0 ); 			
				strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A",localtime(&t) );				
				m_ofsLog8<<tmp<<"--------------读取图像失败\n"; // 接收的消息
				delete []m_inBuf;
				Sleep(10);
			}			
		}
		m_ofsLog8.close();
    }
	return 1;
}

void CProcessThread::pause()
{
    work=false;	

}
void CProcessThread::play()
{
	work=true;
}
BOOL CProcessThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}
void CProcessThread::setIndex(int index)
{
	m_ProcessIndex=index;
}
int CProcessThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CProcessThread, CWinThread)
	//{{AFX_MSG_MAP(CProcessThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessThread message handlers
