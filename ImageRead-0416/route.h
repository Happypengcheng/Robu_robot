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
	#define	MaxStep		256								// ��󻺴沽��
	#define	MovementParament		16					// �˶�����
	_ConnectionPtr m_pConnection;						// ���ݿ����ӱ���
	_RecordsetPtr m_pRecordset;							// ���ݿ��¼������
	CString TableName;									// ���ݿ����
	
	BOOL InitializationWindows(void);					// ��ʼ����ʾ����		
	void InitADOConn();									// �������ݿ�
	void ExitConnect();									// �Ͽ����ݿ�
	void Database2List();								// �����ݿ���ʾ���б�ؼ���
	void InitList();									// ��ʼ���б�
	void SelectRow(INT RowID);							// ѡ���б�ؼ�ĳһ�в���ʾ	
	void Database2Route(void);							// �����ݿ�����תΪ·������
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
