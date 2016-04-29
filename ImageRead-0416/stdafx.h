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

// 彭成定义区----------------------------------------------------------------------------------------------------------
#include <Iphlpapi.h>
#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib,"Iphlpapi.lib")
extern SOCKET m_Sock;  //创建全局的socket
extern bool m_isSocketConnected;		// 定义bool判断连接是否成功
extern CString m_IP;
extern CString m_Port;

// 彭成定义区----------------------------------------------------------------------------------------------------------

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3B0E600C_BA96_4113_99C7_A7A53A3FA8EF__INCLUDED_)

// 张健定义区----------------------------------------------------------------------------------------------------------

// 小车模板建立步骤
// 1. 用VC++6.0模板建立一个空白DLG工程（必须首先安装Access2007）。
// 2. 删除确定，保留取消，另加9个预备按钮IDC_BUTTON1～9。
// 3. 新建一个名为LIST的列表控件,设置为report风格
// 4. 新建十六个EDIT1~16文本输入框，EDIT1设为只读
// 5. 在工程目录下添加开发用数据库文件，即copy iRoute.mdb到工程目录下。
// 6. 在stdafx.h中，添加Motion的头文件及库文件,即把本文件直接copy到stdafx.h空白处。
// #import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace \
// rename("EOF","adoEOF")rename("BOF","adoBOF")	//隐式引用静态库
#import "msado15.dll" no_namespace \
rename("EOF","adoEOF")rename("BOF","adoBOF")	//隐式引用静态库
#include "MotionVision.h"						//运动控制卡函数
#pragma comment(lib, "MotionVision.lib")		//隐式引用静态库

// 7. 在*Dlg.h中，class *Dlg : public CDialog的public中添加文件	2.+DLG.h.txt 的内容

// 8. 在*.cpp中，	AfxEnableControlContainer();语句后拷入以下文件。
/*
//ADO LIST 2
::CoInitialize(NULL);
*/
// 9. 在*.cpp中，	return FALSE;语句前拷入以下文件。
/*
//ADO LIST 3
::CoUninitialize();
*/

// 10. 在DLG图形编辑界面，双击IDC_BUTTON1～9按钮，生成 IDC_BUTTON1～9()9个函数。
// 11. 在DLG图形编辑界面，双击list control，生成OnClickList(NMHDR* pNMHDR, LRESULT* pResult)函数。
// 12. 在*Dlg.cpp中注释掉以上函数，并添加文件3.+DLG.cpp.txt内容，记得替换*Dlg。

// 13. Ctrl+s/F7/F5。

// 在stdafx.h中，添加Evision功能模块对应的头文件及静态库

#include"Windows.h"
#include <afxdlgs.h>
#include "ProcessThread.h"
#include "JHCap.h"
#pragma comment(lib,"JHCap2.lib")
#include "Easy.h"						// Evision基础头文件;EasyImage Header
#include "EImage.h"						// 图像处理头文件;EasyImage Header
#include "EColor.h"						// 色彩变换头文件;EasyColor Header
#include "EInspect.h"					// 图像检测头文件;EasyBar Header
#include "EBarCode.h"					// 条码识别头文件;EasyBar Header
#include "EMatch.h"					// 条码识别头文件;EasyBar Header
#pragma comment(lib, "EasyMs60d.lib")	//隐式引用静态库
#pragma comment(lib, "EImgMs60d.lib")	//隐式引用静态库
#pragma comment(lib, "EClrMs60d.lib")	//隐式引用静态库
#pragma comment(lib, "EBrcMS60d.lib")	//隐式引用静态库
#pragma comment(lib, "EInsMs60d.lib")	//隐式引用静态库
#pragma comment(lib, "EMchMs60d.lib")	//隐式引用静态库

// 张健定义区----------------------------------------------------------------------------------------------------------

