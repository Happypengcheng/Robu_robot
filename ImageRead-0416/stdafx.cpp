// stdafx.cpp : source file that includes just the standard includes
//	MicroMecanum.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <winsock2.h>
// 	#ifdef _ATL_STATIC_REGISTRY
// #include <statreg.h>
// #endif
//#include <atlimpl.cpp>


SOCKET m_Sock;  //����ȫ�ֵ�socket
bool m_isSocketConnected =false;		// ����bool�ж������Ƿ�ɹ�
CString m_IP = "localhost";
CString m_Port="5009";
