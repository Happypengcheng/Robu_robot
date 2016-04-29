#if !defined(AFX_CAMMERA_H__C1C31343_676D_4B5D_9B22_B85BE5E421C5__INCLUDED_)
#define AFX_CAMMERA_H__C1C31343_676D_4B5D_9B22_B85BE5E421C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Cammera.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCammera dialog

class CCammera : public CDialog
{
// Construction
public:
	CCammera(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCammera)
	enum { IDD = IDD_CAMDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCammera)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCammera)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMMERA_H__C1C31343_676D_4B5D_9B22_B85BE5E421C5__INCLUDED_)
