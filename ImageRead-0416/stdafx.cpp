// stdafx.cpp : source file that includes just the standard includes
//	MicroMecanum.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <winsock2.h>
// 	#ifdef _ATL_STATIC_REGISTRY
// #include <statreg.h>
// #endif
//#include <atlimpl.cpp>


SOCKET m_Sock;  //创建全局的socket
bool m_isSocketConnected =false;		// 定义bool判断连接是否成功
CString m_IP = "localhost";
CString m_Port="5009";
