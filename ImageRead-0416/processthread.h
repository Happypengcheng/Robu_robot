#if !defined(AFX_PROCESSTHREAD_H__51436803_BA1A_4313_92F4_E96B0CBFE0E0__INCLUDED_)
#define AFX_PROCESSTHREAD_H__51436803_BA1A_4313_92F4_E96B0CBFE0E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProcessThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CProcessThread thread

class CProcessThread : public CWinThread
{
	DECLARE_DYNCREATE(CProcessThread)
public:
	CProcessThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
int Run();
void pause();
void play();
void setIndex(int index);
char tmp[64];
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	~CProcessThread();

	// Generated message map functions
	//{{AFX_MSG(CProcessThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
private:
	bool work;
	int m_ProcessIndex;
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSTHREAD_H__51436803_BA1A_4313_92F4_E96B0CBFE0E0__INCLUDED_)
