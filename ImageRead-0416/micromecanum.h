// MicroMecanum.h : main header file for the MICROMECANUM application
//

#if !defined(AFX_MICROMECANUM_H__94B6C0BB_9F8F_4AC4_B433_F40EBBFA63C2__INCLUDED_)
#define AFX_MICROMECANUM_H__94B6C0BB_9F8F_4AC4_B433_F40EBBFA63C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
//#include "MicroMecanum_i.h"

/////////////////////////////////////////////////////////////////////////////
// CMicroMecanumApp:
// See MicroMecanum.cpp for the implementation of this class
//

class CMicroMecanumApp : public CWinApp
{
public:
	CMicroMecanumApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMicroMecanumApp)
	public:
	virtual BOOL InitInstance();
//		virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMicroMecanumApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
// private:
// 	BOOL m_bATLInited;
// private:
// 	BOOL InitATL();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MICROMECANUM_H__94B6C0BB_9F8F_4AC4_B433_F40EBBFA63C2__INCLUDED_)
