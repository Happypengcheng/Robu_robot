// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__3B0E600C_BA96_4113_99C7_A7A53A3FA8EF__INCLUDED_)
#define AFX_STDAFX_H__3B0E600C_BA96_4113_99C7_A7A53A3FA8EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <winsock2.h>
#include <afxsock.h>		// MFC socket extensions
#include "mscomm.h"			// MFC RS232C
#include "math.h"			// MFC RS232C

// ��ɶ�����----------------------------------------------------------------------------------------------------------
#include <Iphlpapi.h>
#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib,"Iphlpapi.lib")
extern SOCKET m_Sock;  //����ȫ�ֵ�socket
extern bool m_isSocketConnected;		// ����bool�ж������Ƿ�ɹ�
extern CString m_IP;
extern CString m_Port;

// ��ɶ�����----------------------------------------------------------------------------------------------------------

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3B0E600C_BA96_4113_99C7_A7A53A3FA8EF__INCLUDED_)

// �Ž�������----------------------------------------------------------------------------------------------------------

// С��ģ�彨������
// 1. ��VC++6.0ģ�彨��һ���հ�DLG���̣��������Ȱ�װAccess2007����
// 2. ɾ��ȷ��������ȡ�������9��Ԥ����ťIDC_BUTTON1��9��
// 3. �½�һ����ΪLIST���б�ؼ�,����Ϊreport���
// 4. �½�ʮ����EDIT1~16�ı������EDIT1��Ϊֻ��
// 5. �ڹ���Ŀ¼����ӿ��������ݿ��ļ�����copy iRoute.mdb������Ŀ¼�¡�
// 6. ��stdafx.h�У����Motion��ͷ�ļ������ļ�,���ѱ��ļ�ֱ��copy��stdafx.h�հ״���
// #import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace \
// rename("EOF","adoEOF")rename("BOF","adoBOF")	//��ʽ���þ�̬��
#import "msado15.dll" no_namespace \
rename("EOF","adoEOF")rename("BOF","adoBOF")	//��ʽ���þ�̬��
#include "MotionVision.h"						//�˶����ƿ�����
#pragma comment(lib, "MotionVision.lib")		//��ʽ���þ�̬��

// 7. ��*Dlg.h�У�class *Dlg : public CDialog��public������ļ�	2.+DLG.h.txt ������

// 8. ��*.cpp�У�	AfxEnableControlContainer();�����������ļ���
/*
//ADO LIST 2
::CoInitialize(NULL);
*/
// 9. ��*.cpp�У�	return FALSE;���ǰ���������ļ���
/*
//ADO LIST 3
::CoUninitialize();
*/

// 10. ��DLGͼ�α༭���棬˫��IDC_BUTTON1��9��ť������ IDC_BUTTON1��9()9��������
// 11. ��DLGͼ�α༭���棬˫��list control������OnClickList(NMHDR* pNMHDR, LRESULT* pResult)������
// 12. ��*Dlg.cpp��ע�͵����Ϻ�����������ļ�3.+DLG.cpp.txt���ݣ��ǵ��滻*Dlg��

// 13. Ctrl+s/F7/F5��

// ��stdafx.h�У����Evision����ģ���Ӧ��ͷ�ļ�����̬��

#include"Windows.h"
#include <afxdlgs.h>
#include "ProcessThread.h"
#include "JHCap.h"
#pragma comment(lib,"JHCap2.lib")
#include "Easy.h"						// Evision����ͷ�ļ�;EasyImage Header
#include "EImage.h"						// ͼ����ͷ�ļ�;EasyImage Header
#include "EColor.h"						// ɫ�ʱ任ͷ�ļ�;EasyColor Header
#include "EInspect.h"					// ͼ����ͷ�ļ�;EasyBar Header
#include "EBarCode.h"					// ����ʶ��ͷ�ļ�;EasyBar Header
#include "EMatch.h"					// ����ʶ��ͷ�ļ�;EasyBar Header
#pragma comment(lib, "EasyMs60d.lib")	//��ʽ���þ�̬��
#pragma comment(lib, "EImgMs60d.lib")	//��ʽ���þ�̬��
#pragma comment(lib, "EClrMs60d.lib")	//��ʽ���þ�̬��
#pragma comment(lib, "EBrcMS60d.lib")	//��ʽ���þ�̬��
#pragma comment(lib, "EInsMs60d.lib")	//��ʽ���þ�̬��
#pragma comment(lib, "EMchMs60d.lib")	//��ʽ���þ�̬��

// �Ž�������----------------------------------------------------------------------------------------------------------

