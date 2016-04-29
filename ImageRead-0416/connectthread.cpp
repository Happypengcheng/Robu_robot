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
//////////////////////传递消息用的结构体//////////////////////////////////////

IMPLEMENT_DYNCREATE(CConnectSocketThread, CWinThread)

CConnectSocketThread::CConnectSocketThread()
{
	work=false;
}

CConnectSocketThread::~CConnectSocketThread()
{

}

// 连接服务器
int CConnectSocketThread::Run()
{ 	
	CMicroMecanumDlg pMicroMecanumDlg;	
	CMicroMecanumDlg *hwnd=(CMicroMecanumDlg * )AfxGetMainWnd(); 
	
	//while(!m_isSocketConnected) //通讯线程一直开启，监听
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
				//使用Socket的程序在使用Socket之前必须调用WSAStartup函数。
				//该函数的第一个参数指明程序请求使用的Socket版本，其中高位字节指明副版本、低位字节指明主版本；
				//操作系统利用第二个参数返回请求的Socket的版本信息。
				//当一个应用程序调用WSAStartup函数时，操作系统根据请求的Socket版本来搜索相应的Socket库，
				//然后绑定找到的Socket库到该应用程序中。以后应用程序就可以调用所请求的Socket库中的其它Socket函数了。
				//该函数执行成功后返回0
				
				
				//为server_addr赋值 ，创建soket
				server_addr.sin_addr.s_addr = inet_addr((LPCSTR)"127.0.0.1");
				server_addr.sin_family = AF_INET;
				server_addr.sin_port = htons(5009);	
				
				ofstream m_ofsLog8;				// 记录socket消息
				m_ofsLog8.open("log-8连接Socket是否成功.txt",ios::app);

				if( (m_Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) //判断socket是否创建成功
				{
					// 获取当前系统时间
					time_t t = time( 0 ); 			
					strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A",localtime(&t) );					
					//socket创建失败					
					m_ofsLog8<<tmp<<"-------------socket创建失败\n"; // 接收的消息									
				}
				if( connect(m_Sock, (struct sockaddr *) &server_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)//判断链接是否成功
				{		
					// 获取当前系统时间
					time_t t = time( 0 ); 			
					strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A",localtime(&t) );
					
					//连接失败				
					m_ofsLog8<<tmp<<"-------------连接socket失败--重新连接\n"; // 接收的消息						
				}  
				else // 连接成功
				{
					// 获取当前系统时间
					time_t t = time( 0 ); 			
					strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A",localtime(&t) ); 
					//连接成功				
					m_ofsLog8<<tmp<<"--------连接socket成功\n"; // 接收的消息		
											
					// 发送连接成功消息	
					
					// hwnd->SendMessage(WM_CONNECT_SUCCESS_MESSAGE,0,(LPARAM)0);	//最后一个参数无意义
					m_isSocketConnected = true;	
				}
				m_ofsLog8.close();		
			}
		}

		/*		
		// 接收消息，暂时不用，pc-2016/04/16
		if(m_isSocketConnected) //通讯线程一直开启，监听
		{
			if(work)
			{
				time_t t = time( 0 ); 			
				strftime( tmp, sizeof(tmp), "%X",localtime(&t) );
				
				int res = -1;
				char msg[102400] = "";				
				
				if( (res = recv(m_Sock, msg, 102400, 0)) == -1) // 接收数据，判断是否接收成功
				{			
					// 判断是不是网断了；
					// m_isSocketConnected = false;
				}
				else  
				{	
					//接收成功
					m_ofsLog3.open("log-3Soket读取消息.txt",ios::app);
					m_ofsLog3<<msg; // 接收的消息
					m_ofsLog3<<"\n";
					m_ofsLog3.close();
					
					// 接收到的消息发送给小车
					char* cMsg = new char [102400];
					strcpy(cMsg, msg);
					
					char temp_str1[1024] = "欢迎您！";	
					
					// 如果是初始登录消息
					if(strcmp(temp_str1,msg) == 0)
					{
						m_ofsLog3.open("log-3Soket读取消息.txt",ios::app);
						m_ofsLog3<<tmp<<"--欢迎您，登陆成功\n"; // 接收的消息	
						hwnd->SendMessage(WM_LOGIN_SUCCESS_MESSAGE,0,(LPARAM)0); // 最后一个参数无意义
						m_ofsLog3.close();
					}
					else
					{
						m_ofsLog3.open("log-3Soket读取消息.txt",ios::app);
						m_ofsLog3<<tmp<<"--登陆后，接收到小车下发的指令信息\n"; // 接收的消息		
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
