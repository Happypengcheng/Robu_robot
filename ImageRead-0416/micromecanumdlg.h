// MicroMecanumDlg.h : header file
//
#include <map>
#include <string>
#include <iomanip.h>  
#include <fstream>
#include <iostream>
#include <math.h> 
#include <WinSock2.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ConnectThread.h"
#include "processthread.h"
using namespace std;


#if !defined(AFX_MICROMECANUMDLG_H__E737454A_05E1_43A2_94BF_E2972390E663__INCLUDED_)
#define AFX_MICROMECANUMDLG_H__E737454A_05E1_43A2_94BF_E2972390E663__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_PROCESS_MESSAGE WM_USER+101 
#define WM_COMMAND_MESSAGE WM_USER+102 
#define WM_CONNECT_SUCCESS_MESSAGE WM_USER+103 
#define WM_LOGIN_SUCCESS_MESSAGE WM_USER+104 


//----------qi---------------------
/*
// ö�ٷ���
enum direction{
	east, north, west, south
};
*/
// �ر����ݽṹ
struct MARK {
	CString barCode;//���
	double x;//��λ����
	double y;//��λ����
	double dir;//��������Y������Ϊ0
};

// У׼�õ����ݽṹ
struct CHECKPOINT {
	double distance;	// ��Line�������۾���
	double deltax;		// ��ر��X���򣨼������߷���ֱ�ķ���ƫ��
	double deltay;		// ��ر��y���򣨼������߷���ֱ�ķ���ƫ��
	double deltaa;		// ��ر�ķ���ƫ����ʵ�ʹ۲�õ�����λ��
	double deltah;		// ���н������ƫ�ƽǣ�ͨ����һ�㼰Ŀ����������õ���ƫ��Ϊ����ƫ��Ϊ������λ����
	double speed;		// �����ٶ�

	double adjustx;		// ���һ��ʵ��У׼step�����x����
	double adjusty;		// ���һ��ʵ��У׼step�����y����
};

//С����Ϣ
struct IGVINFO {
	double curX, curY;	// С����ǰ���꣬��λ�Ǻ���
	double preX, preY;	// С����һ��(ǰ��������ǰ)���꣬��λ�Ǻ���
	double Speed;			// С���н��ٶ�
	double Heading;		// С����ͷ����0��Ϊ������Y�᷽�򣩣�90��Ϊ������X�᷽��
	double Compass;		// �شŸ�Ӧ����0��Ϊ������Y�᷽�򣩣�90��Ϊ������X�᷽��

	double MoveDirection;	// �˶�����

	bool isFree;		// �Ƿ����
	bool isMarked;		// �Ƿ���У׼
	CString MarkID;		// ��У׼�ر�ı��� 
	
};

//--------------------------------------- 

/////////////////////////////////////////////////////////////////////////////
// CMicroMecanumDlg dialog


class CMicroMecanumDlg : public CDialog
{
// Construction
public:
	CMicroMecanumDlg(CWnd* pParent = NULL);	// standard constructor
/*

	void Test(void);				// ������ʱʹ��

// mecanum С���˶�����ģ�� ---------------------------------------------------
	#define Puls2Distance 0.09964739028125		// �������嵽����Ļ��㳣��
	#define TurnRound 3180						// ����mecanum С��ԭ����תһ�ܵľ��룬��λmm
	//#define RoundUnit 3180						// ����mecanum С��ԭ����תһ�ܵľ��룬��λmm

//	#define ARROWRED 0X000000FF
//	#define ARROWGRE 0X0000FF00
//	#define ARROWBLU 0X00FF0000

	#define ARROWBLK 0X00010101
	#define ARROWRED 0X000000FF
	#define ARROWGRE 0X0000FF00
	#define ARROWBLU 0X00FF0000
	#define ARROWWHT 0X00FFFFFF

	#define MotorPower		0X80
	#define PlatformPower	0X40
	#define CameraPower		0X20
	#define uPin			0X10

	#define Pin0	 0X01
	#define Pin1	 0X02
	#define Pin2	 0X04
	#define Pin3	 0X08
	#define Pin4	 0X10
	#define Pin5	 0X20
	#define Pin6	 0X40
	#define Pin7	 0X80

	#define GpioRead 1
	#define GpioWrite 0
	#define GpioHigh 1
	#define GpioLow 0

	PLX_DEVICE_KEY    m_DeviceKey;
	PLX_DEVICE_OBJECT m_Device;	

	ControlParemeter Control;		// 1.����һ���˶����Ʋ����Ľṹ��
	MotionParemeter Line[mTotal];	// 2.����һ��mecanum�����˶������Ľṹ��
	MotionParemeter Step[mTotal];	// 3.����һ��mecanum�����˶������Ľṹ��
	MapLine	CaliMap[mTotal];		// 4.����һ����ͼУ׼ר�ò����Ľṹ��
	VirtualPort pPort[mTotal];		// ����˿�ר�����ݽṹ
	VirtualPortPar VportPar;		// ����˿ڲ���ר�����ݽṹ	
	BOOL DeknockEnable;				// �������ײ����־
	BOOL PlatMotionEnable;			// ����ƽ̨�˶���־
	//--------qi-----------------------------------------------
	int nMarks;						// �ֿ��еر��������
	MARK* marks;					// �ر��б��洢�ر�
	void ReadMarkFile();			// ��ȡ�ر��ļ�
	
	// ת��У׼ϵ����1��ƫ��Ƕ�Ӧ��У������CaliDistance(����)ռ�����ı�
	// ���磬���������400mm��У��ϵ����1/40����1��ƫ�ǵ�У������Ϊ10mm
	double RotateFactor;
	double RotateLimit;
	void ReadConfigFile();			// ��ȡ�����ļ�

	int nCheckPoints;				// ��һ��Line������ȡ���ĵر������ 
	CHECKPOINT* CheckPoints;		// ��һ��Line������ȡ���ĵر��У׼��Ϣ
	MARK*	CheckMarks;				// ��һ��Line������ȡ���ĵر���б�

	std::map <CString,MARK*> mapMarks;		// �ر������ر���Ϣ��ӳ��	
	MARK* GetMarkInfo(CString barCode);		// ��������õ��ر���Ϣ
	
	ofstream m_ofsLog2;						// ��־�ļ�������δ��m_ofsLog0��1

	MARK* LineMarks;						// һ��Ҫ�ߵ���·�����е�mark
	int   nLineMarks;						// LineMarks������
*/
	// �����ĸ�������꣬��С����Ҫ��ת�ĽǶ�
//	double GetRotateAngle(	double x1, double y1, double x2, double y2, 
//					double x3, double y3, double x4, double y4);

	// ������3��4�������ͳ��ӵ�ƫ�ƽ�deltaa������˶�����ģ�ƫ��Ϊ��������С����Ҫ��ת�ĽǶ�
//	double GetRotateAngle(double x3, double y3,double x4, double y4, double deltaa);

	// ����sBar�ر긽�������ĵر�(���ڲ��ŵ���������)����������������ģ��򷵻�����barCode.
//	CString FindCentralMark(CString sBar);

	// ���ݵ�ǰ�����ĵر�(����ƫ����)�����ĵر꣬�Լ���ͷ����ȷ�����ӵ��ƶ�����
//	MotionParemeter GetCaliLine(MARK* pMark, MARK* pMarkCentral, double deltax, double deltay, double m_Heading);

	// �����˶������Ŀ����뷵��һ���˶�������MotionParameter�ṹ�壩
//	MotionParemeter GetMotionParameters(int dir, int dis);

//	IGVINFO m_igvInfo;
//	double m_curX, m_curY;	// С����ǰ���꣬��λ�Ǻ���
//	double m_preX, m_preY;	// С����һ��(ǰ��������ǰ)���꣬��λ�Ǻ���
//	double m_markFoundLocation;	// �����ر�ʱ�ҵ��ĵر���н����룬��¼����������һ������·�����˻�ȥ��׼�ر�

//	double m_Heading;	// С����ͷ����0��Ϊ������Y�᷽�򣩣�90��Ϊ������X�᷽��
//	double m_Compass;	// �شŸ�Ӧ����0��Ϊ������Y�᷽�򣩣�90��Ϊ������X�᷽��

	// ������תָ����³�ͷ����361��ʱ��ת��362˳ʱ�룬ת360�ȵ�angleֵΪ3180
//	void UpdateHeading(INT32 cmd, INT32 angle);
	
	// ���³�����ǰ����
//	void UpdateOwnPosition(int dis);
	// ����С����ǰλ�ã����磬�ǲ�������һ�������ı�̫Զ�ˣ����߸���������������ƫ��̫����
	// ����ǣ���С������ͣ�����ұ���
//	void ValidatePosition();	
	// ��ȡ����,�����ȡʧ�ܾͷ��ؿ��ַ�""
	CString ReadBar();
	// �ж�һ������ĸ�λ�Ƿ���0-9������
	BOOL CheckBarcode(CString bar);
/*
	#define MAX_NotFindMarkDistance 2500	//	��Զ��δ���ֵر�ľ��룬�����˾ͽ���ͣ����С������
	#define MAX_NotFindMarkTimes370 5		// ����δ���ֵر�Ĵ����������˾������ر���������
	#define MAX_ReCheckMarkTimes370 3		// ȷ�ϵر꣨���ڸ��ݲΧ�ڣ��Ĵ������ﵽ��������˾���Ϊ�ر��Ѿ�У׼��
	#define X_Y_TOLERANCE_LOW 0.5			// �ر�У׼ʱ�����x,y�ݲ�ͣ�����λ���ף�С�ڸ�ֵ����Ϊ���꾫ȷ
	#define ANGLE_TOLERANCE_LOW 0.5			// �ر�У׼ʱ����ĽǶ��ݲ�ͣ�����λ���ף�С�ڸ�ֵ����Ϊ�ǶȾ�ȷ
	#define X_Y_TOLERANCE_HIGH 1.0			// �ر�У׼ʱ�����x,y�ݲ�ߣ�����λ���ף�С�ڸ�ֵ����Ϊ���������ȷ������Ҫȷ�ϼ���
	#define ANGLE_TOLERANCE_HIGH 1.0		// �ر�У׼ʱ����ĽǶ��ݲ�ߣ�����λ���ף�С�ڸ�ֵ����Ϊ�ǶȻ�����ȷ������Ҫȷ�ϼ���

	int m_iCountOfSearchRoutes;				// �ֲ������ر���·������
	int m_iCurrentSearchingRoute;			// ��ǰ����ִ�еĵر�����·������, -1��ʾδ���еر�������-1����Ontimer()��ĳЩ�����Ƿ�ִ�У������BarDecoder��̫ͬ����
	bool m_isSearchingMarkFound;			// ִ�еر�����·�������еر��Ƿ��ҵ�
	bool m_isSearchingMark;					// �Ƿ��������ر꣬���ڿ���BarDecoder()�����е�һЩ�����Ƿ�ִ��
	int  m_NotFindMarkTimes370;				// ִ��370ָ��ʱδ����mark�Ĵ���
	double m_NotFindMarkDistance;			// ִ��ǰ�������ʱδ����У׼�ر���ۼƾ��룬��δ��
	// ִ��һ����̬У׼ָ��ʱ�ۼ�У׼�Ĵ�����ֻ�е�λ��ƫ��С��X_Y_TOLERANCE_HIGH���Ƕ�С��ANGLE_TOLERANCE_HIGHʱ�ſ�ʼ��������
	// ÿ�ν�����̬У׼������
	int m_CaliCount;
	*/
//	MotionParemeter SearchLines[50]/*[Num_SearchLines]*/;	// �ֲ������ر���·�����飬Ԫ�ز��ᳬ��50

	// ��ȡ�ر�����·���ļ�
//	void ReadSearchLines();
	
	// ��������·������Num_SearchLines����
//	void CalculateSearchLines();

//	BOOL Deknock(void);
	
	//------------end qi-------------------------------------------
	

	//--------pc---------------------------------------------------
			
//	CString m_IP;					// ����IP	
//	CString m_Port;					// ����˿�
	char tmp[64];					// ʱ��洢����
	std::string Temp_LastBarCodeid; // ��¼��һ��������BarCodeid
	std::string Temp_BarCodeID;		// ��¼��ǰ������BarCodeid
	CConnectSocketThread pConnectSocketThread;	// socketͨѶ���ӷ������߳�ָ��
	// CRcvMsgThread pRcvMsgThread;	// socketͨѶ������Ϣ�߳�ָ��

	void SendMessage2Server(char* msg);		// ���巢����Ϣ�ĺ���	
    void InitializationSoket();				// ��ʼ��soket,ͬʱ�����߳̽�����Ϣ
    int GetMacaddress( char* mac);			// ��ȡ����mac��ַ
	void byte2Hex(unsigned char bData, char hex[]);

	void SendIGVLoginMessage();		// socket���ӳɹ�ʱ���͵�¼��Ϣ�������

	// ���Ͷ������ݵ���̨
	void SendBarCodeInfo(int isSuc, CString barID, double x, double y, double angle);
	int m_iTimeCounter;				// ��������OnTimerÿִ��10�λش�һ������λ�ú�״̬��Ϣ	

//	void Command2Route(char* sMsg);		// �ͻ����յ���Ϣ��Ĵ���	

	//------------end pc-------------------------------------------

    
//	BOOL InitializationMotion(void);
//	void MotionInit();	//�˶�ϵͳ��λ
//	CString DisplayPar(MotionParemeter* line);
//	CString DisplayControl(ControlParemeter* Control);
//	BOOL StartMotion(void);			// ��ʼ�˶�
//	BOOL StopMotion(void);			// ֹͣ�˶�	
//	BOOL Display(void);

//	BOOL MotorPowerOFF(void);		// �رյ����Դ
//	BOOL MotorPowerON(void);		// �򿪵����Դ
	
	// ��ͷ���Ʊ���
	U32 mARROW;
	U32 FixedStep;
	CString DisFixInfo;
	BOOL DrawArrow(UINT32 CommandDir);// �滭�˶������ͷ
//	BOOL DeMotionError(DOUBLE *dR,DOUBLE *X,DOUBLE *Y,DOUBLE *L,DOUBLE *dL);	//�˶�����������㹫ʽ
//	BOOL DeMachineError(DOUBLE &R,DOUBLE &D,DOUBLE &dR,DOUBLE &L,DOUBLE &dL);	//��е����������㹫ʽ
//	BOOL FixTrack(DOUBLE &dR,DOUBLE &dX,DOUBLE &dY,DOUBLE &dA,ControlParemeter* Control,MotionParemeter* step);		//�Զ�У׼�����㹫ʽ
//	BOOL DeAngleError(DOUBLE *dR,DOUBLE *X,DOUBLE *Y,DOUBLE *L,DOUBLE *dL);	//ͨ���Ƕ������˶����
//	DWORD RunLife(CString TimeUnit);	//ͳ�Ƴ�������ʱ��
// mecanum С���˶�����ģ�� ---------------------------------------------------

// ADO LIST 1 --------------------------------------------------------------------------------------
	#define	MovementParament		16					// �˶������ĸ���
	_ConnectionPtr m_pConnection;						// ���ݿ����ӱ���
	_RecordsetPtr m_pRecordset;							// ���ݿ��¼������
	CString TableName;									// ���ݿ����
	
	BOOL InitializationWindows(void);					// ��ʼ����ʾ����		
//	void InitADOConn();									// �������ݿ�
//	void ExitConnect();									// �Ͽ����ݿ�
//	void Database2List();								// �����ݿ���ʾ���б�ؼ���
//	void InitList();									// ��ʼ���б�
//	void SelectRow(INT RowID);							// ѡ���б�ؼ�ĳһ�в���ʾ	
//	void Database2Route(void);							// �����ݿ�����תΪ·������
// ADO LIST 1 --------------------------------------------------------------------------------------

//------JHSN36BF 3 Evision 2---------------------------------------------------	
	//	BOOL CammeraOK;	
//	int DecoderCounter;
	#define ImageProcessSpace 2
	#define BarLenth 40
	#define BarCodeNumber 5
	CString SystemInfor;

	INT32 ImageProcessFrequence;
	BOOL FindBarEnable;
//	bool DecodeInfoOK;
	bool m_saveFlag;
	bool m_saveBMPFlag;
	bool m_saveJPGFlag;
	int m_index;
	int NumColor;
	int	VirticalCounter;
	int TotalDispFrame;
	int FramePerSecond;
	int DecodeSuccesTime;
	int DecodeFaileTime;
	double	BarXlocation;				// ��������X������
	double  BarYlocation;				// ��������Y������
	double  BarXlenth;					// ����X�����᳤��
	double  BarYlenth;					// ����Y�����᳤��
	double  BarAngle;					// ����ƫת�Ƕ�	
	CComboBox *ResoComBox;
	CComboBox *NameComBox;
	CProcessThread pProcessThread;		//ͼ��ɼ���׽�߳�
	void CameraInitional();				//��ʼ���������
	void InitializationCamera();		//��ʼ���������new
	void BarDecoder();					//����Evision����
	
	//���Evisionר�ñ����ͺ���
	enum IMAGE_FILE_TYPES eFileFormat;  // ö�ٵ�ͼ���ļ����뱣������ͣ�File type for load and save of images
	//EImageC24 EC24Image1;				// C24ͼ���������EImageC24 class instance
	//EImageC24 EC24Image2;				// C24ͼ���������EImageC24 class instance
	//EImageC24 C24Var;					// C24ͼ���������EImageC24 class instance	
	EImageBW8 EBW8Image2;				// BW8ͼ���������EImageBW8 class instance
	EImageBW8 m_Src;					// BW8ͼ���������EImageBW8 class instance
	//EImageBW8 ImageNull;				// BW8ͼ���������EImageBW8 class instance
	EBarCode m_BarCode;					// ����ʶ�������

	// ����Ϊ��������ģʽʶ��Match��
	EImageBW8 m_Pattern;				// BW8ͼ���������8λ�Ҷ�ͼ��EImageBW8 class instance
	EROIBW8 m_Roi;						// pattern image
	EMatch m_Match;						// Match object for pattern matching
	
	CString	BarDecodeInfo;				// ����ʶ�𲿷���ʾ������Ϣ��
	bool	FinishedBarDecode;			// ������ɱ�־
	bool	CameraReady;				// ���׼��������־
	bool	EnableDecoderBar;			// �������
	bool	DisplayImage;				// ������ʾͼ��
	
	char m_pszDecodedText[256];			// ���������ַ���һά�ַ�������
	CString str,str1,str2;  			// ��ʾ����ʶ����Ϣ���ַ���
	
//------JHSN36BF 3 Evision 2------------------------------------------------------------

// ����ͨѶ----------------------------------------------------------------------------------------
//	CMSComm ZjCom;
//	void InitRs232c(void);	//��ʼ������ 
// ����ͨѶ----------------------------------------------------------------------------------------

// Dialog Data
	//{{AFX_DATA(CMicroMecanumDlg)
	enum { IDD = IDD_MICROMECANUM_DIALOG };
	CStatic	m_ConnectStatus;
	CComboBox	m_HeadingCombBxo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMicroMecanumDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CMicroMecanumDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
//	afx_msg void OnOnCommMscomm1();
	afx_msg LRESULT ProcessImage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ProcessCommand(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ProcessConnectSuccess(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ProcessLoginSuccess(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangeCombo1();
//	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MICROMECANUMDLG_H__E737454A_05E1_43A2_94BF_E2972390E663__INCLUDED_)
