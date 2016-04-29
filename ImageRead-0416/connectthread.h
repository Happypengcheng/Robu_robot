
// #include "MicroMecanumDlg.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConnectThread.h : header file

//
/////////////////////////////////////////////////////////////////////////////
// CConnectSocketThread thread

class CConnectSocketThread : public CWinThread
{
	DECLARE_DYNCREATE(CConnectSocketThread)
public:
	CConnectSocketThread();           // protected constructor used by dynamic creation

// Attributes
public:
bool IsReceivedMsg;
// Operations
public:
int Run();
void pause();
// static DWORD WINAPI ThreadProc1(LPVOID lpParameter);
// DWORD WINAPI ThreadProc1();
void play();
char tmp[64]; 
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnectSocketThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	~CConnectSocketThread();

	// Generated message map functions
	//{{AFX_MSG(CConnectSocketThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
private:
	bool work;
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

