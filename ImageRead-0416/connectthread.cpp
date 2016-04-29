// ProcessThread.cpp : implementation file
//

#include "stdafx.h"
#include <stdlib.h> 
#include <stdio.h> 
#include <ctime>
#include <fstream>
#include "MicroMecanum.h"
#include "MicroMecanumDlg.h"
#include "ConnectThread.h"
#include <windows.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CConnectSocketThread
//////////////////////������Ϣ�õĽṹ��//////////////////////////////////////

IMPLEMENT_DYNCREATE(CConnectSocketThread, CWinThread)

CConnectSocketThread::CConnectSocketThread()
{
	work=false;
}

CConnectSocketThread::~CConnectSocketThread()
{

}

// ���ӷ�����
int CConnectSocketThread::Run()
{ 	
	CMicroMecanumDlg pMicroMecanumDlg;	
	CMicroMecanumDlg *hwnd=(CMicroMecanumDlg * )AfxGetMainWnd(); 
	
	//while(!m_isSocketConnected) //ͨѶ�߳�һֱ����������
	while(1)
	{
		if(!m_isSocketConnected)
		{			
			if(work)
			{	
				WSADATA wsaData;
				SOCKADDR_IN server_addr;
				
				WORD wVersion;
				wVersion = MAKEWORD(2,2);
				WSAStartup(wVersion,&wsaData);
				//ʹ��Socket�ĳ�����ʹ��Socket֮ǰ�������WSAStartup������
				//�ú����ĵ�һ������ָ����������ʹ�õ�Socket�汾�����и�λ�ֽ�ָ�����汾����λ�ֽ�ָ�����汾��
				//����ϵͳ���õڶ����������������Socket�İ汾��Ϣ��
				//��һ��Ӧ�ó������WSAStartup����ʱ������ϵͳ���������Socket�汾��������Ӧ��Socket�⣬
				//Ȼ����ҵ���Socket�⵽��Ӧ�ó����С��Ժ�Ӧ�ó���Ϳ��Ե����������Socket���е�����Socket�����ˡ�
				//�ú���ִ�гɹ��󷵻�0
				
				
				//Ϊserver_addr��ֵ ������soket
				server_addr.sin_addr.s_addr = inet_addr((LPCSTR)"127.0.0.1");
				server_addr.sin_family = AF_INET;
				server_addr.sin_port = htons(5009);	
				
				ofstream m_ofsLog8;				// ��¼socket��Ϣ
				m_ofsLog8.open("log-8����Socket�Ƿ�ɹ�.txt",ios::app);

				if( (m_Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) //�ж�socket�Ƿ񴴽��ɹ�
				{
					// ��ȡ��ǰϵͳʱ��
					time_t t = time( 0 ); 			
					strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A",localtime(&t) );					
					//socket����ʧ��					
					m_ofsLog8<<tmp<<"-------------socket����ʧ��\n"; // ���յ���Ϣ									
				}
				if( connect(m_Sock, (struct sockaddr *) &server_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)//�ж������Ƿ�ɹ�
				{		
					// ��ȡ��ǰϵͳʱ��
					time_t t = time( 0 ); 			
					strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A",localtime(&t) );
					
					//����ʧ��				
					m_ofsLog8<<tmp<<"-------------����socketʧ��--��������\n"; // ���յ���Ϣ						
				}  
				else // ���ӳɹ�
				{
					// ��ȡ��ǰϵͳʱ��
					time_t t = time( 0 ); 			
					strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A",localtime(&t) ); 
					//���ӳɹ�				
					m_ofsLog8<<tmp<<"--------����socket�ɹ�\n"; // ���յ���Ϣ		
											
					// �������ӳɹ���Ϣ	
					
					// hwnd->SendMessage(WM_CONNECT_SUCCESS_MESSAGE,0,(LPARAM)0);	//���һ������������
					m_isSocketConnected = true;	
				}
				m_ofsLog8.close();		
			}
		}

		/*		
		// ������Ϣ����ʱ���ã�pc-2016/04/16
		if(m_isSocketConnected) //ͨѶ�߳�һֱ����������
		{
			if(work)
			{
				time_t t = time( 0 ); 			
				strftime( tmp, sizeof(tmp), "%X",localtime(&t) );
				
				int res = -1;
				char msg[102400] = "";				
				
				if( (res = recv(m_Sock, msg, 102400, 0)) == -1) // �������ݣ��ж��Ƿ���ճɹ�
				{			
					// �ж��ǲ��������ˣ�
					// m_isSocketConnected = false;
				}
				else  
				{	
					//���ճɹ�
					m_ofsLog3.open("log-3Soket��ȡ��Ϣ.txt",ios::app);
					m_ofsLog3<<msg; // ���յ���Ϣ
					m_ofsLog3<<"\n";
					m_ofsLog3.close();
					
					// ���յ�����Ϣ���͸�С��
					char* cMsg = new char [102400];
					strcpy(cMsg, msg);
					
					char temp_str1[1024] = "��ӭ����";	
					
					// ����ǳ�ʼ��¼��Ϣ
					if(strcmp(temp_str1,msg) == 0)
					{
						m_ofsLog3.open("log-3Soket��ȡ��Ϣ.txt",ios::app);
						m_ofsLog3<<tmp<<"--��ӭ������½�ɹ�\n"; // ���յ���Ϣ	
						hwnd->SendMessage(WM_LOGIN_SUCCESS_MESSAGE,0,(LPARAM)0); // ���һ������������
						m_ofsLog3.close();
					}
					else
					{
						m_ofsLog3.open("log-3Soket��ȡ��Ϣ.txt",ios::app);
						m_ofsLog3<<tmp<<"--��½�󣬽��յ�С���·���ָ����Ϣ\n"; // ���յ���Ϣ		
						m_ofsLog3.close();
						hwnd->SendMessage(WM_COMMAND_MESSAGE,0,(LPARAM)cMsg);
					}
				}			
			}		
		}*/
	}
	return 1;
}

void CConnectSocketThread::pause()
{
    work=false;	
}

void CConnectSocketThread::play()
{
	work=true;	
}

BOOL CConnectSocketThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}
int CConnectSocketThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CConnectSocketThread, CWinThread)
	//{{AFX_MSG_MAP(CConnectSocketThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CConnectSocketThread message handlers
