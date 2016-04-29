#if !defined(AFX_ROUTE_H__4AA6C990_7269_4DD1_A432_48DBCEECA6F8__INCLUDED_)
#define AFX_ROUTE_H__4AA6C990_7269_4DD1_A432_48DBCEECA6F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Route.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRoute dialog

class CRoute : public CDialog
{
// Construction
public:
	CRoute(CWnd* pParent = NULL);   // standard constructor

//ADO LIST 1 --------------------------------------------------------------------------------------
	#define	MaxStep		256								// 最大缓存步数
	#define	MovementParament		16					// 运动参数
	_ConnectionPtr m_pConnection;						// 数据库连接变量
	_RecordsetPtr m_pRecordset;							// 数据库记录集变量
	CString TableName;									// 数据库表名
	
	BOOL InitializationWindows(void);					// 初始化显示界面		
	void InitADOConn();									// 连接数据库
	void ExitConnect();									// 断开数据库
	void Database2List();								// 把数据库显示在列表控件上
	void InitList();									// 初始化列表
	void SelectRow(INT RowID);							// 选择列表控件某一列并显示	
	void Database2Route(void);							// 把数据库数据转为路径数据
//ADO LIST 1 --------------------------------------------------------------------------------------

// Dialog Data
	//{{AFX_DATA(CRoute)
	enum { IDD = IDD_DATADLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRoute)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRoute)
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROUTE_H__4AA6C990_7269_4DD1_A432_48DBCEECA6F8__INCLUDED_)
