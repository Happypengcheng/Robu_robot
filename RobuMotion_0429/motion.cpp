// Motion.cpp : implementation file
//

#include "stdafx.h"
#include "MicroMecanum.h"
#include "Motion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMotion dialog


CMotion::CMotion(CWnd* pParent /*=NULL*/)
	: CDialog(CMotion::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMotion)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMotion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMotion)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMotion, CDialog)
	//{{AFX_MSG_MAP(CMotion)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotion message handlers

void CMotion::OnButton1() 
{
	// TODO: Add your control notification handler code here
	
}

void CMotion::OnButton2() 
{
	// TODO: Add your control notification handler code here
	
}

void CMotion::OnButton3() 
{
	// TODO: Add your control notification handler code here
	
}

void CMotion::OnButton4() 
{
	// TODO: Add your control notification handler code here
	
}

void CMotion::OnButton5() 
{
	// TODO: Add your control notification handler code here
	
}

void CMotion::OnButton6() 
{
	// TODO: Add your control notification handler code here
	
}

void CMotion::OnButton7() 
{
	// TODO: Add your control notification handler code here
	
}

void CMotion::OnButton8() 
{
	// TODO: Add your control notification handler code here
	
}

void CMotion::OnButton9() 
{
	// TODO: Add your control notification handler code here
	
}

void CMotion::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
