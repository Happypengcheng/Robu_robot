// Route.cpp : implementation file
//

#include "stdafx.h"
#include "MicroMecanum.h"
#include "Route.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRoute dialog


CRoute::CRoute(CWnd* pParent /*=NULL*/)
	: CDialog(CRoute::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRoute)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRoute::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRoute)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRoute, CDialog)
	//{{AFX_MSG_MAP(CRoute)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRoute message handlers

//ADO LIST 4 --------------------------------------------------------------------------------------
BOOL CRoute::OnInitDialog()		//��ʼ������
{
	// ��ʼ��������ʾ
 	TableName = "Route";			// �趨��������
 	InitializationWindows();		// ��ʼ������
 	InitList();						// ��ʼ���б�ؼ�

	return TRUE;
}

void CRoute::OnCancel()		//�رձ༭����
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CRoute::InitializationWindows(void)	// init windows
{	
	// ȷ��ϵͳ��Ļ����
	int W = GetSystemMetrics(SM_CXSCREEN);
	int H = GetSystemMetrics(SM_CYSCREEN);	
	MoveWindow(0,0,W,H);
	TRACE("ϵͳĬ�Ϸֱ���%d*%d!\n",W,H);
	// ȷ�Ϲ�������
	CRect windows;
	GetClientRect(&windows);
	int x = windows.Width();
	int y = windows.Height();
	TRACE("��ʾ����%d*%d!\n",x,y);
	// ���ð���		
	CRect rect;
	int bw = x / 20;
	int bh = y / 20;
	rect.left	= 19 * bw;
	rect.top	= 0;
	rect.right	= rect.left + bw;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDCANCEL)->MoveWindow(&rect);	
	GetDlgItem(IDCANCEL)->SetWindowText("�رճ���");
	TRACE("��������%d*%d!\n",rect.left,rect.right);
	rect.top	= 1*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON1)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON1)->SetWindowText("����");
	rect.top	= 2*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON2)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON2)->SetWindowText("ɾ��");
	rect.top	= 3*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON3)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON3)->SetWindowText("�༭");
// 	rect.top	= 4*2*bh;
// 	rect.bottom = rect.top + bh;
// 	GetDlgItem(IDC_BUTTON4)->MoveWindow(&rect);
// 	GetDlgItem(IDC_BUTTON4)->SetWindowText("4");
// 	rect.top	= 5*2*bh;
// 	rect.bottom = rect.top + bh;
// 	GetDlgItem(IDC_BUTTON5)->MoveWindow(&rect);
// 	GetDlgItem(IDC_BUTTON5)->SetWindowText("5");
// 	rect.top	= 6*2*bh;
// 	rect.bottom = rect.top + bh;
// 	GetDlgItem(IDC_BUTTON6)->MoveWindow(&rect);
// 	GetDlgItem(IDC_BUTTON6)->SetWindowText("6");
// 	rect.top	= 7*2*bh;
// 	rect.bottom = rect.top + bh;
// 	GetDlgItem(IDC_BUTTON7)->MoveWindow(&rect);
// 	GetDlgItem(IDC_BUTTON7)->SetWindowText("7");
// 	rect.top	= 8*2*bh;
// 	rect.bottom = rect.top + bh;
// 	GetDlgItem(IDC_BUTTON8)->MoveWindow(&rect);
// 	GetDlgItem(IDC_BUTTON8)->SetWindowText("8");
// 	rect.top	= 9*2*bh;
// 	rect.bottom = rect.top + bh;
// 	GetDlgItem(IDC_BUTTON9)->MoveWindow(&rect);
// 	GetDlgItem(IDC_BUTTON9)->SetWindowText("9");
	// ����Editbox
	bw = 19 * bw / 16;
	bh = y / 40;
	rect.left	= 0;
	rect.top	= 20 + y/2;
	rect.right	= rect.left + bw;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_EDIT101)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT102)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT103)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT104)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT105)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT106)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT107)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT108)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT109)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT110)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT111)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT112)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT113)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT114)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT115)->MoveWindow(&rect);
	rect.left	= rect.left + bw;
	rect.right	= rect.left + bw;
	GetDlgItem(IDC_EDIT116)->MoveWindow(&rect);
	// ����list control
//	bw = 19 * bw / 16;
//	bh = y / 40;
	rect.left	= 0;
	rect.top	= 20 + bh + y/2;
	rect.right	= 19 * INT32(x / 20);
	rect.bottom = y - 10;
	GetDlgItem(IDC_LIST1)->MoveWindow(&rect);
	TRACE("�б���%d*%d!\n",rect.right,rect.bottom);
	return TRUE;
}
void CRoute::InitADOConn(void)	//����ADO����
{
	try
	{
		//�������Ӷ���ʵ��
		m_pConnection.CreateInstance("ADODB.Connection");
		//���������ַ���
		CString strConnect="DRIVER={Microsoft Access Driver (*.mdb)};uid=;pwd=;DBQ=iRoute.mdb;";
		//ʹ��Open�����������ݿ�
		m_pConnection->Open((_bstr_t)strConnect,"","",adModeUnknown);
	}
	catch(_com_error e)
	{
		//������ʾ����
		AfxMessageBox(e.Description());
	}
}

void CRoute::ExitConnect(void)	//�رռ�¼��������
{
	//�رռ�¼��������
    if(m_pRecordset!=NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
}

void CRoute::Database2List(void)	//�����ݿ���ʾ���б�ؼ���
{
	//�������ݿ�
	InitADOConn();
	//�ϳɲ�ѯ�ַ���
	//	TableName = "Route";
	CString str;
	str.Format("select * from %s order by StepID desc",TableName);
	_bstr_t bstrSQL = str;
	//������¼��ָ�����ʵ��
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	//�򿪼�¼��
	m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	//��ȡ�б���ָ��
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	//ɾ��ԭ�б����������
	pList ->DeleteAllItems();
	//����¼��ָ���ƶ�����һ����¼
	m_pRecordset->MoveFirst();	
	//��ȡ���ݿ����ݲ���ʾ���б�ؼ���
	while(!m_pRecordset->adoEOF)
	{
		//���б��в���һ�У���ˢ������
		pList ->InsertItem(0,"");
		pList ->SetItemText(0,0,(char*)(_bstr_t)m_pRecordset->GetCollect("StepID"));		// 1
		pList ->SetItemText(0,1,(char*)(_bstr_t)m_pRecordset->GetCollect("CommandID"));		// 2
		pList ->SetItemText(0,2,(char*)(_bstr_t)m_pRecordset->GetCollect("AidDistance"));	// 3
		pList ->SetItemText(0,3,(char*)(_bstr_t)m_pRecordset->GetCollect("StartSpeed"));	// 4
		pList ->SetItemText(0,4,(char*)(_bstr_t)m_pRecordset->GetCollect("MaxSpeed"));		// 5
		pList ->SetItemText(0,5,(char*)(_bstr_t)m_pRecordset->GetCollect("ACC"));			// 6
		pList ->SetItemText(0,6,(char*)(_bstr_t)m_pRecordset->GetCollect("DACC"));			// 7
		pList ->SetItemText(0,7,(char*)(_bstr_t)m_pRecordset->GetCollect("CaliDistance"));	// 8
		pList ->SetItemText(0,8,(char*)(_bstr_t)m_pRecordset->GetCollect("MictoDistance"));	// 9
		pList ->SetItemText(0,9,(char*)(_bstr_t)m_pRecordset->GetCollect("PortX"));			// 10
		pList ->SetItemText(0,10,(char*)(_bstr_t)m_pRecordset->GetCollect("PortY"));		// 11
		pList ->SetItemText(0,11,(char*)(_bstr_t)m_pRecordset->GetCollect("PortZ"));		// 12
		pList ->SetItemText(0,12,(char*)(_bstr_t)m_pRecordset->GetCollect("PortU"));		// 13
		pList ->SetItemText(0,13,(char*)(_bstr_t)m_pRecordset->GetCollect("Data1"));		// 14
		pList ->SetItemText(0,14,(char*)(_bstr_t)m_pRecordset->GetCollect("Data2"));		// 15
		pList ->SetItemText(0,15,(char*)(_bstr_t)m_pRecordset->GetCollect("Data3"));		// 16
		
		//����¼��ָ���ƶ�����һ����¼
		m_pRecordset->MoveNext();
	}
	//�Ͽ����ݿ�����
	ExitConnect();
}

void CRoute::InitList(void)	//��������ʼ���б�
{
	// ���ñ����
	CRect windows;
	GetClientRect(&windows);
	int W = windows.Width();
	W = UINT32(W / 20) * 19/16;	
	//��ȡ�б���ָ��
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	//�����б���ʾ���
	pList ->SetExtendedStyle(LVS_EX_FLATSB							// ��ʾ�����
		|LVS_EX_FULLROWSELECT										// ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
		|LVS_EX_HEADERDRAGDROP										// ֧���Ϸ�
		|LVS_EX_ONECLICKACTIVATE									// ��������
		|LVS_EX_GRIDLINES);											// ��ʾ�����
	//�����б��������
	pList ->InsertColumn(0,"·�����",LVCFMT_LEFT,W,0);				// �������С��������趨�п�
	pList ->InsertColumn(1,"ָ�����",LVCFMT_LEFT,W,1); 			// �������С��������趨�п�
	pList ->InsertColumn(2,"Ŀ�����",LVCFMT_LEFT,W,2); 			// �������С��������趨�п�
	pList ->InsertColumn(3,"�����ٶ�",LVCFMT_CENTER,W,3);			// �������С��������趨�п�	
	pList ->InsertColumn(4,"����ٶ�",LVCFMT_CENTER,W,4);			// �������С��������趨�п�	
	pList ->InsertColumn(5,"����ʱ��",LVCFMT_CENTER,W,5);			// �������С��������趨�п�	
	pList ->InsertColumn(6,"����ʱ��",LVCFMT_CENTER,W,6);			// �������С��������趨�п�	
	pList ->InsertColumn(7,"У������",LVCFMT_CENTER,W,7);			// �������С��������趨�п�
	pList ->InsertColumn(8,"��������",LVCFMT_CENTER,W,8);			// �������С��������趨�п�
	pList ->InsertColumn(9,"X��˿�",LVCFMT_CENTER,W,9);			// �������С��������趨�п�
	pList ->InsertColumn(10,"Y��˿�",LVCFMT_CENTER,W,10);			// �������С��������趨�п�
	pList ->InsertColumn(11,"Z��˿�",LVCFMT_CENTER,W,11);			// �������С��������趨�п�
	pList ->InsertColumn(12,"U��˿�",LVCFMT_CENTER,W,12);			// �������С��������趨�п�
	pList ->InsertColumn(13,"����1",LVCFMT_CENTER,W,13);			// �������С��������趨�п�
	pList ->InsertColumn(14,"����2",LVCFMT_CENTER,W,14);			// �������С��������趨�п�
	pList ->InsertColumn(15,"����3",LVCFMT_CENTER,W,15);			// �������С��������趨�п�
	// �趨���ֱ���ɫ
	pList ->SetTextBkColor(RGB(128,255,128));					
	//�����ݿ���ʾ���б�ؼ���
	Database2List();
}

void CRoute::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//��ȡ16���ı��༭����ָ��
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT101);				
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT102);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT103);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_EDIT104);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_EDIT105);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_EDIT106);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_EDIT107);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit8 = (CEdit*)GetDlgItem(IDC_EDIT108);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit9 = (CEdit*)GetDlgItem(IDC_EDIT109);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit10 = (CEdit*)GetDlgItem(IDC_EDIT110);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit11 = (CEdit*)GetDlgItem(IDC_EDIT111);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit12 = (CEdit*)GetDlgItem(IDC_EDIT112);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit13 = (CEdit*)GetDlgItem(IDC_EDIT113);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit14 = (CEdit*)GetDlgItem(IDC_EDIT114);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit15 = (CEdit*)GetDlgItem(IDC_EDIT115);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit16 = (CEdit*)GetDlgItem(IDC_EDIT116);			// ��ȡ�ı��༭����ָ��
	//��ȡ�б���ָ��
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	//��ȡ�б�����
	int FocusLine = pList ->GetSelectionMark();
	//ȡ���������ݸ���16���ı��༭����ʾ����
	pEdit1->SetWindowText(pList ->GetItemText(FocusLine, 0));	
	pEdit2->SetWindowText(pList ->GetItemText(FocusLine, 1));	// �����ı��༭����ʾ����
	pEdit3->SetWindowText(pList ->GetItemText(FocusLine, 2));	// �����ı��༭����ʾ����
	pEdit4->SetWindowText(pList ->GetItemText(FocusLine, 3));	// �����ı��༭����ʾ����
	pEdit5->SetWindowText(pList ->GetItemText(FocusLine, 4));	// �����ı��༭����ʾ����
	pEdit6->SetWindowText(pList ->GetItemText(FocusLine, 5));	// �����ı��༭����ʾ����
	pEdit7->SetWindowText(pList ->GetItemText(FocusLine, 6));	// �����ı��༭����ʾ����
	pEdit8->SetWindowText(pList ->GetItemText(FocusLine, 7));	// �����ı��༭����ʾ����
	pEdit9->SetWindowText(pList ->GetItemText(FocusLine, 8));	// �����ı��༭����ʾ����
	pEdit10->SetWindowText(pList ->GetItemText(FocusLine, 9));	// �����ı��༭����ʾ����
	pEdit11->SetWindowText(pList ->GetItemText(FocusLine, 10));	// �����ı��༭����ʾ����
	pEdit12->SetWindowText(pList ->GetItemText(FocusLine, 11));	// �����ı��༭����ʾ����
	pEdit13->SetWindowText(pList ->GetItemText(FocusLine, 12));	// �����ı��༭����ʾ����
	pEdit14->SetWindowText(pList ->GetItemText(FocusLine, 13));	// �����ı��༭����ʾ����
	pEdit15->SetWindowText(pList ->GetItemText(FocusLine, 14));	// �����ı��༭����ʾ����
	pEdit16->SetWindowText(pList ->GetItemText(FocusLine, 15));	// �����ı��༭����ʾ����

	//���ش�����
	*pResult = 0;
}
 
void CRoute::OnButton1()	//  insert	//����
{
	//�������ݿ��׼�����ƺ�����
	CString	StdData[MovementParament]={"1","0","500","0","1000","1000","1000","0","400","1","2","3","4","100","200","300"};
	CString StdName[MovementParament]={"StepID","CommandID","AidDistance","StartSpeed","MaxSpeed","ACC","DACC","CaliDistance","MictoDistance","PortX","PortY","PortZ","PortU","Data1","Data2","Data3",};
	//��������ַ���	
	CString sql,str,ModyStr;	
	//��ȡ�б���ָ��
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);  
	//��ȡѡ����
	POSITION pos= pList ->GetFirstSelectedItemPosition();	
	//ȡ��ѡ���е�����
    int selRol = pList ->GetNextSelectedItem(pos);			
	//��ȡ��ǰ�б��������
	int nStepCounter = pList ->GetItemCount();				
	//���б�Ϊ��
	if (nStepCounter > 0)									
	{	//��ѡ���в�Ϊ��
		if (selRol >= 0)									
		{			
			//�����ݿ�
			InitADOConn();
			//��ѡ���п�ʼ���������к��Զ�����			
			for (INT i=nStepCounter;i>selRol;i--)
			{	
				//��SQL��Update���������к�
				sql.Format("update %s set StepID=%d where StepID=%d ",TableName,i+1,i);
				m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);
				TRACE("���� %d# ��Ϊ %d# ��\n",i,i+1);
			}
			//ԭѡ���б����ߣ���SQL��insert��������ѡ����
			sql.Format("insert into %s (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s) \
				values (%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",TableName,\
				StdName[0],StdName[1],StdName[2],StdName[3],StdName[4],StdName[5],StdName[6],StdName[7],StdName[8],\
				StdName[9],StdName[10],StdName[11],StdName[12],StdName[13],StdName[14],StdName[15],\
				selRol+1,StdData[1],StdData[2],StdData[3],StdData[4],StdData[5],StdData[6],StdData[7],StdData[8],\
				StdData[9],StdData[10],StdData[11],StdData[12],StdData[13],StdData[14],StdData[15]);
			m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);		
			TRACE("���� %d# �� \n",selRol+1);
			//�ر����ݿ�
			m_pConnection->Close();		
		} 		
	}
	else	//���б�Ϊ�գ������к�Ϊ1����													
	{
		//�����ݿ�	
		InitADOConn();
		//��SQL��insert��������һ��ָ���кŵ���
		CString sql;
		INT RowNumber=1;
		sql.Format("insert into %s (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s) \
			values (%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",TableName,\
			StdName[0],StdName[1],StdName[2],StdName[3],StdName[4],StdName[5],StdName[6],StdName[7],StdName[8],\
			StdName[9],StdName[10],StdName[11],StdName[12],StdName[13],StdName[14],StdName[15],\
			selRol+1,StdData[1],StdData[2],StdData[3],StdData[4],StdData[5],StdData[6],StdData[7],StdData[8],\
			StdData[9],StdData[10],StdData[11],StdData[12],StdData[13],StdData[14],StdData[15]);
		m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);
		//�ر����ݿ�
		m_pConnection->Close();		
	}
	//�����ݿ���ʾ���б�ؼ���
	Database2List();
}

void CRoute::OnButton2()	//delete	//ɾ��
{
	//��ȡ�б���ָ�� 
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	// ��ȡѡ����
	POSITION pos= pList ->GetFirstSelectedItemPosition();
	// ȡ��ѡ���е�����	
    int selRol = pList ->GetNextSelectedItem(pos);			
	// ��ȡ��ǰ��������
	int nStepCounter = pList ->GetItemCount();
	//��������ַ���				
	CString sql,ModyStr;
	//ѡ���в�Ϊ��
	if (selRol >= 0)										
	{	//�����ݿ�
		InitADOConn();
		//��SQLɾ��ѡ����
		sql.Format("delete * from %s where StepID=%d ",TableName,selRol+1);
		m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);
		TRACE("ɾ��%d#��\n",selRol+1);
		//˳���ƶ��к�
		for (INT i=selRol;i<nStepCounter-1;i++)
		{	
			//��SQL��Update����copy�����кŵ�����
			sql.Format("update %s set StepID=%d where StepID=%d ",TableName,i+1,i+2);
			m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);
			TRACE("���� %d# ��Ϊ %d# ��\n",i+1,i+2);
		}
		//�ر����ݿ�
		m_pConnection->Close();	
	}
	//�����ݿ���ʾ���б�ؼ���
	Database2List();
}

void CRoute::OnButton3()	//edit	//�༭
{
	//�������ݿ��׼������
	CString StdName[MovementParament]={"StepID","CommandID","AidDistance","StartSpeed","MaxSpeed","ACC","DACC","CaliDistance","MictoDistance","PortX","PortY","PortZ","PortU","Data1","Data2","Data3",};
	//��ȡ�б���ָ��
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	//��ȡ16���ı��༭����ָ��
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT101);				
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT102);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT103);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_EDIT104);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_EDIT105);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_EDIT106);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_EDIT107);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit8 = (CEdit*)GetDlgItem(IDC_EDIT108);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit9 = (CEdit*)GetDlgItem(IDC_EDIT109);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit10 = (CEdit*)GetDlgItem(IDC_EDIT110);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit11 = (CEdit*)GetDlgItem(IDC_EDIT111);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit12 = (CEdit*)GetDlgItem(IDC_EDIT112);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit13 = (CEdit*)GetDlgItem(IDC_EDIT113);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit14 = (CEdit*)GetDlgItem(IDC_EDIT114);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit15 = (CEdit*)GetDlgItem(IDC_EDIT115);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit16 = (CEdit*)GetDlgItem(IDC_EDIT116);			// ��ȡ�ı��༭����ָ��
	//��ȡ�ı��༭����Ϣ���ϳ�SQLupdate�ַ���
	CString sql,str,ModyStr,ts,StepNumber;





	pEdit1 ->GetWindowText(StepNumber);
	TRACE("�༭%d# ��\n",atoi(StepNumber));
	ModyStr = "";
	pEdit2 ->GetWindowText(ts);
	str.Format("%s='%s',",StdName[1],ts);
	ModyStr += str;	
	pEdit3 ->GetWindowText(ts);
	str.Format("%s='%s',",StdName[2],ts);
	ModyStr += str;	
	pEdit4 ->GetWindowText(ts);
	str.Format("%s='%s',",StdName[3],ts);
	ModyStr += str;	
	pEdit5 ->GetWindowText(ts);
	str.Format("%s='%s',",StdName[4],ts);
	ModyStr += str;	
	pEdit6 ->GetWindowText(ts);
	str.Format("%s='%s',",StdName[5],ts);
	ModyStr += str;	
	pEdit7 ->GetWindowText(ts);
	str.Format("%s='%s',",StdName[6],ts);
	ModyStr += str;	
	pEdit8 ->GetWindowText(ts);
	str.Format("%s='%s',",StdName[7],ts);
	ModyStr += str;	
	pEdit9 ->GetWindowText(ts);
	str.Format("%s='%s',",StdName[8],ts);
	ModyStr += str;	
	pEdit10 ->GetWindowText(ts);
	str.Format("%s='%s',",StdName[9],ts);
	ModyStr += str;
	pEdit11 ->GetWindowText(ts);
	str.Format("%s='%s',",StdName[10],ts);
	ModyStr += str;
	pEdit12 ->GetWindowText(ts);
	str.Format("%s='%s',",StdName[11],ts);
	ModyStr += str;
	pEdit13 ->GetWindowText(ts);
	str.Format("%s='%s',",StdName[12],ts);
	ModyStr += str;
	pEdit14 ->GetWindowText(ts);
	str.Format("%s='%s',",StdName[13],ts);
	ModyStr += str;
	pEdit15 ->GetWindowText(ts);
	str.Format("%s='%s',",StdName[14],ts);
	ModyStr += str;
	pEdit16 ->GetWindowText(ts);
	str.Format("%s='%s'",StdName[15],ts);
	ModyStr += str;
	// ������Ϣ
	TRACE(ModyStr);
	//�����ݿ�
	InitADOConn();
	//��SQL��update�����������ݿ�
	sql.Format("update %s set %s where StepID=%d ",TableName,ModyStr,atoi(StepNumber));
	m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);
	//�ر����ݿ�
	m_pConnection->Close();
	//�����ݿ���ʾ���б�ؼ���
	Database2List();
}

void CRoute::SelectRow(INT RowID)	//select
{
	//��ȡ�б���ָ��
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	//��ȡ16���ı��༭����ָ��
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT101);				
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT102);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT103);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_EDIT104);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_EDIT105);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_EDIT106);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_EDIT107);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit8 = (CEdit*)GetDlgItem(IDC_EDIT108);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit9 = (CEdit*)GetDlgItem(IDC_EDIT109);				// ��ȡ�ı��༭����ָ��
	CEdit* pEdit10 = (CEdit*)GetDlgItem(IDC_EDIT110);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit11 = (CEdit*)GetDlgItem(IDC_EDIT111);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit12 = (CEdit*)GetDlgItem(IDC_EDIT112);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit13 = (CEdit*)GetDlgItem(IDC_EDIT113);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit14 = (CEdit*)GetDlgItem(IDC_EDIT114);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit15 = (CEdit*)GetDlgItem(IDC_EDIT115);			// ��ȡ�ı��༭����ָ��
	CEdit* pEdit16 = (CEdit*)GetDlgItem(IDC_EDIT116);			// ��ȡ�ı��༭����ָ��
	// ��ȡѡ����
	POSITION pos= pList ->GetFirstSelectedItemPosition();
	// ȡ��ѡ���е�����	
    INT selRol = pList ->GetNextSelectedItem(pos);			
	// ������Ϣ��ʾ
	TRACE("�ı�֮ǰѡ����%d��!\n",selRol+1);	
	// ��ȡ��ǰ��������
	INT  nListTotalRow = pList ->GetItemCount();
	// ��ȡҪѡ�����
	CString str;
	pEdit2->GetWindowText(str);
	INT SelecedtRow = atoi(str);
	// ȡ�������еĸ�������״̬
	for (INT i=0;i<nListTotalRow;i++)
	{
		pList ->SetFocus();
		pList ->SetItemState(i,0,LVIS_SELECTED|LVIS_FOCUSED);
	}
	//����ĳ��Ϊ��������״̬
	pList ->SetFocus();
	pList ->SetItemState(RowID-1,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
	//��ȡѡ����
	pos= pList ->GetFirstSelectedItemPosition();	
	//ȡ��ѡ���е�����
    selRol = pList ->GetNextSelectedItem(pos);
	// ������Ϣ��ʾ
	TRACE("����ѡ����%d��!\n",selRol+1);
	//�����ı��༭����ʾ����
	pEdit1->SetWindowText(pList ->GetItemText(RowID-1, 0));	
	pEdit2->SetWindowText(pList ->GetItemText(RowID-1, 1));	// �����ı��༭����ʾ����
	pEdit3->SetWindowText(pList ->GetItemText(RowID-1, 2));	// �����ı��༭����ʾ����
	pEdit4->SetWindowText(pList ->GetItemText(RowID-1, 3));	// �����ı��༭����ʾ����
	pEdit5->SetWindowText(pList ->GetItemText(RowID-1, 4));	// �����ı��༭����ʾ����
	pEdit6->SetWindowText(pList ->GetItemText(RowID-1, 5));	// �����ı��༭����ʾ����
	pEdit7->SetWindowText(pList ->GetItemText(RowID-1, 6));	// �����ı��༭����ʾ����
	pEdit8->SetWindowText(pList ->GetItemText(RowID-1, 7));	// �����ı��༭����ʾ����
	pEdit9->SetWindowText(pList ->GetItemText(RowID-1, 8));	// �����ı��༭����ʾ����
	pEdit10->SetWindowText(pList ->GetItemText(RowID-1, 9));// �����ı��༭����ʾ����
	pEdit11->SetWindowText(pList ->GetItemText(RowID-1, 10));// �����ı��༭����ʾ����
	pEdit12->SetWindowText(pList ->GetItemText(RowID-1, 11));// �����ı��༭����ʾ����	
	pEdit13->SetWindowText(pList ->GetItemText(RowID-1, 12));// �����ı��༭����ʾ����
	pEdit14->SetWindowText(pList ->GetItemText(RowID-1, 13));// �����ı��༭����ʾ����
	pEdit15->SetWindowText(pList ->GetItemText(RowID-1, 14));// �����ı��༭����ʾ����
	pEdit16->SetWindowText(pList ->GetItemText(RowID-1, 15));// �����ı��༭����ʾ����

// 	MotionParemeter MotionPar;
// 	
// 	// 1.�������к�
// 	MotionPar.StepNuber		= 0;
// 	// 2.��������״̬��0û��ִ��/������ִ��
// 	MotionPar.StepStaut		= atoi(pList->GetItemText(RowID-1, 0));
// 	// 3.�˶�����
// 	MotionPar.Derection		= atoi(pList->GetItemText(RowID-1, 1));	
// 	// 4.Ŀ�����
// 	MotionPar.AidDistance	= atoi(pList->GetItemText(RowID-1, 2));	
// 	// 5.�����ٶ�
// 	MotionPar.StartSpeed	= atoi(pList->GetItemText(RowID-1, 3));	
// 	// 6.����ٶ�
// 	MotionPar.MaxSpeed		= atoi(pList->GetItemText(RowID-1, 4));	
// 	// 7.�Ӽ��ٶ�
// 	MotionPar.ACC			= atoi(pList->GetItemText(RowID-1, 5));	
// 	// 8.�����ٶ�
// 	MotionPar.DACC			= atoi(pList->GetItemText(RowID-1, 6));	
// 	// 9.У������
// 	MotionPar.CaliDistance	= atoi(pList->GetItemText(RowID-1, 7));
// 	// 10.�ֲ�����
// 	MotionPar.Step			= atoi(pList->GetItemText(RowID-1, 8));
// 	// 11.X����Ŀ��
// 	MotionPar.AidX			= atoi(pList->GetItemText(RowID-1, 9));
// 	// 12.Y����Ŀ��
// 	MotionPar.AidY			= atoi(pList->GetItemText(RowID-1, 10));
// 	// 13.Z����Ŀ��
// 	MotionPar.AidZ			= atoi(pList->GetItemText(RowID-1, 11));
// 	// 14.U����Ŀ��
// 	MotionPar.AidU			= atoi(pList->GetItemText(RowID-1, 12));
// 	// 15.Ԥ������1
// 	MotionPar.Pdata1		= atoi(pList->GetItemText(RowID-1, 13));
// 	// 16.Ԥ������2
// 	MotionPar.Pdata2		= atoi(pList->GetItemText(RowID-1, 14));
// 	// ��������
// 	DrawCurve(MotionPar);
}

//ADO LIST 4 --------------------------------------------------------------------------------------

