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
BOOL CRoute::OnInitDialog()		//初始化界面
{
	// 初始化界面显示
 	TableName = "Route";			// 设定参数表名
 	InitializationWindows();		// 初始化界面
 	InitList();						// 初始化列表控件

	return TRUE;
}

void CRoute::OnCancel()		//关闭编辑窗口
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CRoute::InitializationWindows(void)	// init windows
{	
	// 确认系统屏幕设置
	int W = GetSystemMetrics(SM_CXSCREEN);
	int H = GetSystemMetrics(SM_CYSCREEN);	
	MoveWindow(0,0,W,H);
	TRACE("系统默认分辨率%d*%d!\n",W,H);
	// 确认工作窗口
	CRect windows;
	GetClientRect(&windows);
	int x = windows.Width();
	int y = windows.Height();
	TRACE("显示窗口%d*%d!\n",x,y);
	// 设置按键		
	CRect rect;
	int bw = x / 20;
	int bh = y / 20;
	rect.left	= 19 * bw;
	rect.top	= 0;
	rect.right	= rect.left + bw;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDCANCEL)->MoveWindow(&rect);	
	GetDlgItem(IDCANCEL)->SetWindowText("关闭程序");
	TRACE("按键左右%d*%d!\n",rect.left,rect.right);
	rect.top	= 1*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON1)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON1)->SetWindowText("插入");
	rect.top	= 2*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON2)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON2)->SetWindowText("删除");
	rect.top	= 3*2*bh;
	rect.bottom = rect.top + bh;
	GetDlgItem(IDC_BUTTON3)->MoveWindow(&rect);
	GetDlgItem(IDC_BUTTON3)->SetWindowText("编辑");
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
	// 设置Editbox
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
	// 设置list control
//	bw = 19 * bw / 16;
//	bh = y / 40;
	rect.left	= 0;
	rect.top	= 20 + bh + y/2;
	rect.right	= 19 * INT32(x / 20);
	rect.bottom = y - 10;
	GetDlgItem(IDC_LIST1)->MoveWindow(&rect);
	TRACE("列表窗口%d*%d!\n",rect.right,rect.bottom);
	return TRUE;
}
void CRoute::InitADOConn(void)	//创建ADO连接
{
	try
	{
		//创建连接对象实例
		m_pConnection.CreateInstance("ADODB.Connection");
		//设置连接字符串
		CString strConnect="DRIVER={Microsoft Access Driver (*.mdb)};uid=;pwd=;DBQ=iRoute.mdb;";
		//使用Open方法连接数据库
		m_pConnection->Open((_bstr_t)strConnect,"","",adModeUnknown);
	}
	catch(_com_error e)
	{
		//捕获并显示错误
		AfxMessageBox(e.Description());
	}
}

void CRoute::ExitConnect(void)	//关闭记录集和连接
{
	//关闭记录集和连接
    if(m_pRecordset!=NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
}

void CRoute::Database2List(void)	//把数据库显示在列表控件上
{
	//连接数据库
	InitADOConn();
	//合成查询字符串
	//	TableName = "Route";
	CString str;
	str.Format("select * from %s order by StepID desc",TableName);
	_bstr_t bstrSQL = str;
	//创建记录集指针对象实例
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	//打开记录集
	m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	//获取列表类指针
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	//删除原列表的所有数据
	pList ->DeleteAllItems();
	//将记录集指针移动到第一条记录
	m_pRecordset->MoveFirst();	
	//读取数据库数据并显示在列表控件上
	while(!m_pRecordset->adoEOF)
	{
		//在列表中插入一行，并刷新数据
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
		
		//将记录集指针移动到下一条记录
		m_pRecordset->MoveNext();
	}
	//断开数据库连接
	ExitConnect();
}

void CRoute::InitList(void)	//创建并初始化列表
{
	// 设置表格宽度
	CRect windows;
	GetClientRect(&windows);
	int W = windows.Width();
	W = UINT32(W / 20) * 19/16;	
	//获取列表类指针
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	//设置列表显示风格
	pList ->SetExtendedStyle(LVS_EX_FLATSB							// 显示网格点
		|LVS_EX_FULLROWSELECT										// 选中某行使整行高亮（只适用与report风格的listctrl）
		|LVS_EX_HEADERDRAGDROP										// 支持拖放
		|LVS_EX_ONECLICKACTIVATE									// 单击激活
		|LVS_EX_GRIDLINES);											// 显示网格点
	//设置列表的列内容
	pList ->InsertColumn(0,"路段序号",LVCFMT_LEFT,W,0);				// 增加新列、命名并设定列宽
	pList ->InsertColumn(1,"指令代号",LVCFMT_LEFT,W,1); 			// 增加新列、命名并设定列宽
	pList ->InsertColumn(2,"目标距离",LVCFMT_LEFT,W,2); 			// 增加新列、命名并设定列宽
	pList ->InsertColumn(3,"启动速度",LVCFMT_CENTER,W,3);			// 增加新列、命名并设定列宽	
	pList ->InsertColumn(4,"最高速度",LVCFMT_CENTER,W,4);			// 增加新列、命名并设定列宽	
	pList ->InsertColumn(5,"加速时间",LVCFMT_CENTER,W,5);			// 增加新列、命名并设定列宽	
	pList ->InsertColumn(6,"减速时间",LVCFMT_CENTER,W,6);			// 增加新列、命名并设定列宽	
	pList ->InsertColumn(7,"校正距离",LVCFMT_CENTER,W,7);			// 增加新列、命名并设定列宽
	pList ->InsertColumn(8,"步长距离",LVCFMT_CENTER,W,8);			// 增加新列、命名并设定列宽
	pList ->InsertColumn(9,"X轴端口",LVCFMT_CENTER,W,9);			// 增加新列、命名并设定列宽
	pList ->InsertColumn(10,"Y轴端口",LVCFMT_CENTER,W,10);			// 增加新列、命名并设定列宽
	pList ->InsertColumn(11,"Z轴端口",LVCFMT_CENTER,W,11);			// 增加新列、命名并设定列宽
	pList ->InsertColumn(12,"U轴端口",LVCFMT_CENTER,W,12);			// 增加新列、命名并设定列宽
	pList ->InsertColumn(13,"数据1",LVCFMT_CENTER,W,13);			// 增加新列、命名并设定列宽
	pList ->InsertColumn(14,"数据2",LVCFMT_CENTER,W,14);			// 增加新列、命名并设定列宽
	pList ->InsertColumn(15,"数据3",LVCFMT_CENTER,W,15);			// 增加新列、命名并设定列宽
	// 设定文字背景色
	pList ->SetTextBkColor(RGB(128,255,128));					
	//把数据库显示在列表控件上
	Database2List();
}

void CRoute::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//获取16个文本编辑框类指针
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT101);				
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT102);				// 获取文本编辑框类指针
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT103);				// 获取文本编辑框类指针
	CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_EDIT104);				// 获取文本编辑框类指针
	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_EDIT105);				// 获取文本编辑框类指针
	CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_EDIT106);				// 获取文本编辑框类指针
	CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_EDIT107);				// 获取文本编辑框类指针
	CEdit* pEdit8 = (CEdit*)GetDlgItem(IDC_EDIT108);				// 获取文本编辑框类指针
	CEdit* pEdit9 = (CEdit*)GetDlgItem(IDC_EDIT109);				// 获取文本编辑框类指针
	CEdit* pEdit10 = (CEdit*)GetDlgItem(IDC_EDIT110);			// 获取文本编辑框类指针
	CEdit* pEdit11 = (CEdit*)GetDlgItem(IDC_EDIT111);			// 获取文本编辑框类指针
	CEdit* pEdit12 = (CEdit*)GetDlgItem(IDC_EDIT112);			// 获取文本编辑框类指针
	CEdit* pEdit13 = (CEdit*)GetDlgItem(IDC_EDIT113);			// 获取文本编辑框类指针
	CEdit* pEdit14 = (CEdit*)GetDlgItem(IDC_EDIT114);			// 获取文本编辑框类指针
	CEdit* pEdit15 = (CEdit*)GetDlgItem(IDC_EDIT115);			// 获取文本编辑框类指针
	CEdit* pEdit16 = (CEdit*)GetDlgItem(IDC_EDIT116);			// 获取文本编辑框类指针
	//获取列表类指针
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	//获取列表焦点行
	int FocusLine = pList ->GetSelectionMark();
	//取焦点行数据更新16个文本编辑框显示内容
	pEdit1->SetWindowText(pList ->GetItemText(FocusLine, 0));	
	pEdit2->SetWindowText(pList ->GetItemText(FocusLine, 1));	// 更新文本编辑框显示内容
	pEdit3->SetWindowText(pList ->GetItemText(FocusLine, 2));	// 更新文本编辑框显示内容
	pEdit4->SetWindowText(pList ->GetItemText(FocusLine, 3));	// 更新文本编辑框显示内容
	pEdit5->SetWindowText(pList ->GetItemText(FocusLine, 4));	// 更新文本编辑框显示内容
	pEdit6->SetWindowText(pList ->GetItemText(FocusLine, 5));	// 更新文本编辑框显示内容
	pEdit7->SetWindowText(pList ->GetItemText(FocusLine, 6));	// 更新文本编辑框显示内容
	pEdit8->SetWindowText(pList ->GetItemText(FocusLine, 7));	// 更新文本编辑框显示内容
	pEdit9->SetWindowText(pList ->GetItemText(FocusLine, 8));	// 更新文本编辑框显示内容
	pEdit10->SetWindowText(pList ->GetItemText(FocusLine, 9));	// 更新文本编辑框显示内容
	pEdit11->SetWindowText(pList ->GetItemText(FocusLine, 10));	// 更新文本编辑框显示内容
	pEdit12->SetWindowText(pList ->GetItemText(FocusLine, 11));	// 更新文本编辑框显示内容
	pEdit13->SetWindowText(pList ->GetItemText(FocusLine, 12));	// 更新文本编辑框显示内容
	pEdit14->SetWindowText(pList ->GetItemText(FocusLine, 13));	// 更新文本编辑框显示内容
	pEdit15->SetWindowText(pList ->GetItemText(FocusLine, 14));	// 更新文本编辑框显示内容
	pEdit16->SetWindowText(pList ->GetItemText(FocusLine, 15));	// 更新文本编辑框显示内容

	//返回处理结果
	*pResult = 0;
}
 
void CRoute::OnButton1()	//  insert	//插入
{
	//定义数据库标准列名称和数据
	CString	StdData[MovementParament]={"1","0","500","0","1000","1000","1000","0","400","1","2","3","4","100","200","300"};
	CString StdName[MovementParament]={"StepID","CommandID","AidDistance","StartSpeed","MaxSpeed","ACC","DACC","CaliDistance","MictoDistance","PortX","PortY","PortZ","PortU","Data1","Data2","Data3",};
	//定义局域字符串	
	CString sql,str,ModyStr;	
	//获取列表类指针
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);  
	//获取选择行
	POSITION pos= pList ->GetFirstSelectedItemPosition();	
	//取得选择行的索引
    int selRol = pList ->GetNextSelectedItem(pos);			
	//获取当前列表的总行数
	int nStepCounter = pList ->GetItemCount();				
	//若列表不为空
	if (nStepCounter > 0)									
	{	//若选定行不为空
		if (selRol >= 0)									
		{			
			//打开数据库
			InitADOConn();
			//从选定行开始到结束，行号自动递增			
			for (INT i=nStepCounter;i>selRol;i--)
			{	
				//用SQL的Update方法递增行号
				sql.Format("update %s set StepID=%d where StepID=%d ",TableName,i+1,i);
				m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);
				TRACE("更改 %d# 行为 %d# 行\n",i,i+1);
			}
			//原选定行被移走，用SQL的insert方法新增选定行
			sql.Format("insert into %s (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s) \
				values (%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",TableName,\
				StdName[0],StdName[1],StdName[2],StdName[3],StdName[4],StdName[5],StdName[6],StdName[7],StdName[8],\
				StdName[9],StdName[10],StdName[11],StdName[12],StdName[13],StdName[14],StdName[15],\
				selRol+1,StdData[1],StdData[2],StdData[3],StdData[4],StdData[5],StdData[6],StdData[7],StdData[8],\
				StdData[9],StdData[10],StdData[11],StdData[12],StdData[13],StdData[14],StdData[15]);
			m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);		
			TRACE("新增 %d# 行 \n",selRol+1);
			//关闭数据库
			m_pConnection->Close();		
		} 		
	}
	else	//若列表为空，新增行号为1的行													
	{
		//打开数据库	
		InitADOConn();
		//用SQL的insert方法新增一个指定行号的行
		CString sql;
		INT RowNumber=1;
		sql.Format("insert into %s (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s) \
			values (%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",TableName,\
			StdName[0],StdName[1],StdName[2],StdName[3],StdName[4],StdName[5],StdName[6],StdName[7],StdName[8],\
			StdName[9],StdName[10],StdName[11],StdName[12],StdName[13],StdName[14],StdName[15],\
			selRol+1,StdData[1],StdData[2],StdData[3],StdData[4],StdData[5],StdData[6],StdData[7],StdData[8],\
			StdData[9],StdData[10],StdData[11],StdData[12],StdData[13],StdData[14],StdData[15]);
		m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);
		//关闭数据库
		m_pConnection->Close();		
	}
	//把数据库显示在列表控件上
	Database2List();
}

void CRoute::OnButton2()	//delete	//删除
{
	//获取列表类指针 
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	// 获取选择行
	POSITION pos= pList ->GetFirstSelectedItemPosition();
	// 取得选择行的索引	
    int selRol = pList ->GetNextSelectedItem(pos);			
	// 获取当前的总行数
	int nStepCounter = pList ->GetItemCount();
	//定义局域字符串				
	CString sql,ModyStr;
	//选定行不为空
	if (selRol >= 0)										
	{	//打开数据库
		InitADOConn();
		//用SQL删除选定行
		sql.Format("delete * from %s where StepID=%d ",TableName,selRol+1);
		m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);
		TRACE("删除%d#行\n",selRol+1);
		//顺序移动行号
		for (INT i=selRol;i<nStepCounter-1;i++)
		{	
			//用SQL的Update方法copy上行行号到下行
			sql.Format("update %s set StepID=%d where StepID=%d ",TableName,i+1,i+2);
			m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);
			TRACE("更改 %d# 行为 %d# 行\n",i+1,i+2);
		}
		//关闭数据库
		m_pConnection->Close();	
	}
	//把数据库显示在列表控件上
	Database2List();
}

void CRoute::OnButton3()	//edit	//编辑
{
	//定义数据库标准列名称
	CString StdName[MovementParament]={"StepID","CommandID","AidDistance","StartSpeed","MaxSpeed","ACC","DACC","CaliDistance","MictoDistance","PortX","PortY","PortZ","PortU","Data1","Data2","Data3",};
	//获取列表类指针
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	//获取16个文本编辑框类指针
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT101);				
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT102);				// 获取文本编辑框类指针
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT103);				// 获取文本编辑框类指针
	CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_EDIT104);				// 获取文本编辑框类指针
	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_EDIT105);				// 获取文本编辑框类指针
	CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_EDIT106);				// 获取文本编辑框类指针
	CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_EDIT107);				// 获取文本编辑框类指针
	CEdit* pEdit8 = (CEdit*)GetDlgItem(IDC_EDIT108);				// 获取文本编辑框类指针
	CEdit* pEdit9 = (CEdit*)GetDlgItem(IDC_EDIT109);				// 获取文本编辑框类指针
	CEdit* pEdit10 = (CEdit*)GetDlgItem(IDC_EDIT110);			// 获取文本编辑框类指针
	CEdit* pEdit11 = (CEdit*)GetDlgItem(IDC_EDIT111);			// 获取文本编辑框类指针
	CEdit* pEdit12 = (CEdit*)GetDlgItem(IDC_EDIT112);			// 获取文本编辑框类指针
	CEdit* pEdit13 = (CEdit*)GetDlgItem(IDC_EDIT113);			// 获取文本编辑框类指针
	CEdit* pEdit14 = (CEdit*)GetDlgItem(IDC_EDIT114);			// 获取文本编辑框类指针
	CEdit* pEdit15 = (CEdit*)GetDlgItem(IDC_EDIT115);			// 获取文本编辑框类指针
	CEdit* pEdit16 = (CEdit*)GetDlgItem(IDC_EDIT116);			// 获取文本编辑框类指针
	//获取文本编辑框信息，合成SQLupdate字符串
	CString sql,str,ModyStr,ts,StepNumber;





	pEdit1 ->GetWindowText(StepNumber);
	TRACE("编辑%d# 行\n",atoi(StepNumber));
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
	// 调试信息
	TRACE(ModyStr);
	//打开数据库
	InitADOConn();
	//用SQL的update方法更新数据库
	sql.Format("update %s set %s where StepID=%d ",TableName,ModyStr,atoi(StepNumber));
	m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);
	//关闭数据库
	m_pConnection->Close();
	//把数据库显示在列表控件上
	Database2List();
}

void CRoute::SelectRow(INT RowID)	//select
{
	//获取列表类指针
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	//获取16个文本编辑框类指针
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT101);				
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT102);				// 获取文本编辑框类指针
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT103);				// 获取文本编辑框类指针
	CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_EDIT104);				// 获取文本编辑框类指针
	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_EDIT105);				// 获取文本编辑框类指针
	CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_EDIT106);				// 获取文本编辑框类指针
	CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_EDIT107);				// 获取文本编辑框类指针
	CEdit* pEdit8 = (CEdit*)GetDlgItem(IDC_EDIT108);				// 获取文本编辑框类指针
	CEdit* pEdit9 = (CEdit*)GetDlgItem(IDC_EDIT109);				// 获取文本编辑框类指针
	CEdit* pEdit10 = (CEdit*)GetDlgItem(IDC_EDIT110);			// 获取文本编辑框类指针
	CEdit* pEdit11 = (CEdit*)GetDlgItem(IDC_EDIT111);			// 获取文本编辑框类指针
	CEdit* pEdit12 = (CEdit*)GetDlgItem(IDC_EDIT112);			// 获取文本编辑框类指针
	CEdit* pEdit13 = (CEdit*)GetDlgItem(IDC_EDIT113);			// 获取文本编辑框类指针
	CEdit* pEdit14 = (CEdit*)GetDlgItem(IDC_EDIT114);			// 获取文本编辑框类指针
	CEdit* pEdit15 = (CEdit*)GetDlgItem(IDC_EDIT115);			// 获取文本编辑框类指针
	CEdit* pEdit16 = (CEdit*)GetDlgItem(IDC_EDIT116);			// 获取文本编辑框类指针
	// 获取选择行
	POSITION pos= pList ->GetFirstSelectedItemPosition();
	// 取得选择行的索引	
    INT selRol = pList ->GetNextSelectedItem(pos);			
	// 调试信息显示
	TRACE("改变之前选定了%d行!\n",selRol+1);	
	// 获取当前的总行数
	INT  nListTotalRow = pList ->GetItemCount();
	// 读取要选择的行
	CString str;
	pEdit2->GetWindowText(str);
	INT SelecedtRow = atoi(str);
	// 取消所有行的高亮焦点状态
	for (INT i=0;i<nListTotalRow;i++)
	{
		pList ->SetFocus();
		pList ->SetItemState(i,0,LVIS_SELECTED|LVIS_FOCUSED);
	}
	//设置某行为高亮焦点状态
	pList ->SetFocus();
	pList ->SetItemState(RowID-1,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
	//获取选择行
	pos= pList ->GetFirstSelectedItemPosition();	
	//取得选择行的索引
    selRol = pList ->GetNextSelectedItem(pos);
	// 调试信息显示
	TRACE("现在选定了%d行!\n",selRol+1);
	//更新文本编辑框显示内容
	pEdit1->SetWindowText(pList ->GetItemText(RowID-1, 0));	
	pEdit2->SetWindowText(pList ->GetItemText(RowID-1, 1));	// 更新文本编辑框显示内容
	pEdit3->SetWindowText(pList ->GetItemText(RowID-1, 2));	// 更新文本编辑框显示内容
	pEdit4->SetWindowText(pList ->GetItemText(RowID-1, 3));	// 更新文本编辑框显示内容
	pEdit5->SetWindowText(pList ->GetItemText(RowID-1, 4));	// 更新文本编辑框显示内容
	pEdit6->SetWindowText(pList ->GetItemText(RowID-1, 5));	// 更新文本编辑框显示内容
	pEdit7->SetWindowText(pList ->GetItemText(RowID-1, 6));	// 更新文本编辑框显示内容
	pEdit8->SetWindowText(pList ->GetItemText(RowID-1, 7));	// 更新文本编辑框显示内容
	pEdit9->SetWindowText(pList ->GetItemText(RowID-1, 8));	// 更新文本编辑框显示内容
	pEdit10->SetWindowText(pList ->GetItemText(RowID-1, 9));// 更新文本编辑框显示内容
	pEdit11->SetWindowText(pList ->GetItemText(RowID-1, 10));// 更新文本编辑框显示内容
	pEdit12->SetWindowText(pList ->GetItemText(RowID-1, 11));// 更新文本编辑框显示内容	
	pEdit13->SetWindowText(pList ->GetItemText(RowID-1, 12));// 更新文本编辑框显示内容
	pEdit14->SetWindowText(pList ->GetItemText(RowID-1, 13));// 更新文本编辑框显示内容
	pEdit15->SetWindowText(pList ->GetItemText(RowID-1, 14));// 更新文本编辑框显示内容
	pEdit16->SetWindowText(pList ->GetItemText(RowID-1, 15));// 更新文本编辑框显示内容

// 	MotionParemeter MotionPar;
// 	
// 	// 1.单步序列号
// 	MotionPar.StepNuber		= 0;
// 	// 2.单步工作状态，0没有执行/其他已执行
// 	MotionPar.StepStaut		= atoi(pList->GetItemText(RowID-1, 0));
// 	// 3.运动方向
// 	MotionPar.Derection		= atoi(pList->GetItemText(RowID-1, 1));	
// 	// 4.目标距离
// 	MotionPar.AidDistance	= atoi(pList->GetItemText(RowID-1, 2));	
// 	// 5.启动速度
// 	MotionPar.StartSpeed	= atoi(pList->GetItemText(RowID-1, 3));	
// 	// 6.最高速度
// 	MotionPar.MaxSpeed		= atoi(pList->GetItemText(RowID-1, 4));	
// 	// 7.加加速度
// 	MotionPar.ACC			= atoi(pList->GetItemText(RowID-1, 5));	
// 	// 8.减加速度
// 	MotionPar.DACC			= atoi(pList->GetItemText(RowID-1, 6));	
// 	// 9.校正距离
// 	MotionPar.CaliDistance	= atoi(pList->GetItemText(RowID-1, 7));
// 	// 10.分步距离
// 	MotionPar.Step			= atoi(pList->GetItemText(RowID-1, 8));
// 	// 11.X分轴目标
// 	MotionPar.AidX			= atoi(pList->GetItemText(RowID-1, 9));
// 	// 12.Y分轴目标
// 	MotionPar.AidY			= atoi(pList->GetItemText(RowID-1, 10));
// 	// 13.Z分轴目标
// 	MotionPar.AidZ			= atoi(pList->GetItemText(RowID-1, 11));
// 	// 14.U分轴目标
// 	MotionPar.AidU			= atoi(pList->GetItemText(RowID-1, 12));
// 	// 15.预留数据1
// 	MotionPar.Pdata1		= atoi(pList->GetItemText(RowID-1, 13));
// 	// 16.预留数据2
// 	MotionPar.Pdata2		= atoi(pList->GetItemText(RowID-1, 14));
// 	// 绘制曲线
// 	DrawCurve(MotionPar);
}

//ADO LIST 4 --------------------------------------------------------------------------------------

