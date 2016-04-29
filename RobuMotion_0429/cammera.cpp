// Cammera.cpp : implementation file
//

#include "stdafx.h"
#include "MicroMecanum.h"
#include "Cammera.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCammera dialog


CCammera::CCammera(CWnd* pParent /*=NULL*/)
	: CDialog(CCammera::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCammera)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCammera::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCammera)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCammera, CDialog)
	//{{AFX_MSG_MAP(CCammera)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCammera message handlers

void CCammera::OnButton1() 
{
	// TODO: Add your control notification handler code here
	
}

void CCammera::OnButton2() 
{
	// TODO: Add your control notification handler code here
	
}

void CCammera::OnButton3() 
{
	// TODO: Add your control notification handler code here
	
}

void CCammera::OnButton4() 
{
	// TODO: Add your control notification handler code here
	
}

void CCammera::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
