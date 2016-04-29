; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMicroMecanumDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MicroMecanum.h"

ClassCount=7
Class1=CMicroMecanumApp
Class2=CMicroMecanumDlg
Class3=CAboutDlg

ResourceCount=15
Resource1=IDD_CAMDLG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CRoute
Resource4=IDD_MOTIONDLG
Class5=CCom
Resource5=IDD_DATADLG
Class6=CCammera
Resource6=IDD_COMDLG
Class7=CMotion
Resource7=IDD_MICROMECANUM_DIALOG
Resource8=IDD_DIALOG1
Resource9=IDD_MOTIONDLG (Chinese (P.R.C.))
Resource10=IDD_ABOUTBOX (Chinese (P.R.C.))
Resource11=IDD_MICROMECANUM_DIALOG (Chinese (P.R.C.))
Resource12=IDD_DATADLG (Chinese (P.R.C.))
Resource13=IDD_COMDLG (Chinese (P.R.C.))
Resource14=IDD_CAMDLG (Chinese (P.R.C.))
Resource15=IDD_DIALOG1 (Chinese (P.R.C.))

[CLS:CMicroMecanumApp]
Type=0
HeaderFile=MicroMecanum.h
ImplementationFile=MicroMecanum.cpp
Filter=N

[CLS:CMicroMecanumDlg]
Type=0
HeaderFile=MicroMecanumDlg.h
ImplementationFile=MicroMecanumDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_MSCOMM1

[CLS:CAboutDlg]
Type=0
HeaderFile=MicroMecanumDlg.h
ImplementationFile=MicroMecanumDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MICROMECANUM_DIALOG]
Type=1
Class=CMicroMecanumDlg
ControlCount=36
Control1=IDCANCEL,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_BUTTON1,button,1342242816
Control4=IDC_BUTTON2,button,1342242816
Control5=IDC_BUTTON3,button,1342242816
Control6=IDC_BUTTON4,button,1342242816
Control7=IDC_BUTTON5,button,1342242816
Control8=IDC_BUTTON6,button,1342242816
Control9=IDC_BUTTON7,button,1342242816
Control10=IDC_BUTTON8,button,1342242816
Control11=IDC_BUTTON9,button,1342242816
Control12=IDC_EDIT1,edit,1350631552
Control13=IDC_EDIT2,edit,1350631552
Control14=IDC_EDIT3,edit,1350631552
Control15=IDC_EDIT4,edit,1350631552
Control16=IDC_EDIT5,edit,1350631552
Control17=IDC_EDIT6,edit,1350631552
Control18=IDC_EDIT7,edit,1350631552
Control19=IDC_EDIT8,edit,1350631552
Control20=IDC_STATIC1,static,1342308352
Control21=IDC_EDIT9,edit,1350631552
Control22=IDC_EDIT10,edit,1350631552
Control23=IDC_EDIT11,edit,1350631552
Control24=IDC_EDIT13,edit,1350631552
Control25=IDC_EDIT14,edit,1350631552
Control26=IDC_EDIT15,edit,1350631552
Control27=IDC_EDIT16,edit,1350631552
Control28=IDC_EDIT12,edit,1350631552
Control29=IDC_STATIC2,static,1342308352
Control30=IDC_MSCOMM1,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control31=IDC_MULTICAM,combobox,1344340226
Control32=IDC_RESOLUTION,combobox,1344340226
Control33=IDC_STATICP1,static,1342177295
Control34=IDC_COMBO1,combobox,1344339970
Control35=IDC_STATIC,static,1342308352
Control36=IDC_STATIC_Connect,static,1342308352

[DLG:IDD_DATADLG]
Type=1
Class=CRoute
ControlCount=21
Control1=IDCANCEL,button,1342242816
Control2=IDC_LIST1,SysListView32,1350631425
Control3=IDC_EDIT101,edit,1350631552
Control4=IDC_EDIT102,edit,1350631552
Control5=IDC_EDIT103,edit,1350631552
Control6=IDC_EDIT104,edit,1350631552
Control7=IDC_EDIT105,edit,1350631552
Control8=IDC_EDIT106,edit,1350631552
Control9=IDC_EDIT107,edit,1350631552
Control10=IDC_EDIT108,edit,1350631552
Control11=IDC_EDIT109,edit,1350631552
Control12=IDC_EDIT110,edit,1350631552
Control13=IDC_EDIT111,edit,1350631552
Control14=IDC_EDIT112,edit,1350631552
Control15=IDC_EDIT113,edit,1350631552
Control16=IDC_EDIT114,edit,1350631552
Control17=IDC_EDIT115,edit,1350631552
Control18=IDC_EDIT116,edit,1350631552
Control19=IDC_BUTTON1,button,1342242816
Control20=IDC_BUTTON2,button,1342242816
Control21=IDC_BUTTON3,button,1342242816

[CLS:CRoute]
Type=0
HeaderFile=Route.h
ImplementationFile=Route.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CRoute

[DLG:IDD_COMDLG]
Type=1
Class=CCom
ControlCount=5
Control1=IDCANCEL,button,1342242816
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_BUTTON3,button,1342242816
Control5=IDC_BUTTON4,button,1342242816

[CLS:CCom]
Type=0
HeaderFile=Com.h
ImplementationFile=Com.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CCom

[DLG:IDD_CAMDLG]
Type=1
Class=CCammera
ControlCount=5
Control1=IDCANCEL,button,1342242816
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_BUTTON3,button,1342242816
Control5=IDC_BUTTON4,button,1342242816

[CLS:CCammera]
Type=0
HeaderFile=Cammera.h
ImplementationFile=Cammera.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CCammera

[DLG:IDD_MOTIONDLG]
Type=1
Class=CMotion
ControlCount=10
Control1=IDCANCEL,button,1342242816
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_BUTTON3,button,1342242816
Control5=IDC_BUTTON4,button,1342242816
Control6=IDC_BUTTON5,button,1342242816
Control7=IDC_BUTTON6,button,1342242816
Control8=IDC_BUTTON7,button,1342242816
Control9=IDC_BUTTON8,button,1342242816
Control10=IDC_BUTTON9,button,1342242816

[CLS:CMotion]
Type=0
HeaderFile=Motion.h
ImplementationFile=Motion.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CMotion

[DLG:IDD_DIALOG1]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:IDD_MICROMECANUM_DIALOG (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=36
Control1=IDCANCEL,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_BUTTON1,button,1342242816
Control4=IDC_BUTTON2,button,1342242816
Control5=IDC_BUTTON3,button,1342242816
Control6=IDC_BUTTON4,button,1342242816
Control7=IDC_BUTTON5,button,1342242816
Control8=IDC_BUTTON6,button,1342242816
Control9=IDC_BUTTON7,button,1342242816
Control10=IDC_BUTTON8,button,1342242816
Control11=IDC_BUTTON9,button,1342242816
Control12=IDC_EDIT1,edit,1350631552
Control13=IDC_EDIT2,edit,1350631552
Control14=IDC_EDIT3,edit,1350631552
Control15=IDC_EDIT4,edit,1350631552
Control16=IDC_EDIT5,edit,1350631552
Control17=IDC_EDIT6,edit,1350631552
Control18=IDC_EDIT7,edit,1350631552
Control19=IDC_EDIT8,edit,1350631552
Control20=IDC_STATIC1,static,1342308352
Control21=IDC_EDIT9,edit,1350631552
Control22=IDC_EDIT10,edit,1350631552
Control23=IDC_EDIT11,edit,1350631552
Control24=IDC_EDIT13,edit,1350631552
Control25=IDC_EDIT14,edit,1350631552
Control26=IDC_EDIT15,edit,1350631552
Control27=IDC_EDIT16,edit,1350631552
Control28=IDC_EDIT12,edit,1350631552
Control29=IDC_STATIC2,static,1342308352
Control30=IDC_MSCOMM1,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control31=IDC_MULTICAM,combobox,1344340226
Control32=IDC_RESOLUTION,combobox,1344340226
Control33=IDC_STATICP1,static,1342177295
Control34=IDC_COMBO1,combobox,1344339970
Control35=IDC_STATIC,static,1342308352
Control36=IDC_STATIC_Connect,static,1342308352

[DLG:IDD_ABOUTBOX (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DATADLG (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=21
Control1=IDCANCEL,button,1342242816
Control2=IDC_LIST1,SysListView32,1350631425
Control3=IDC_EDIT101,edit,1350631552
Control4=IDC_EDIT102,edit,1350631552
Control5=IDC_EDIT103,edit,1350631552
Control6=IDC_EDIT104,edit,1350631552
Control7=IDC_EDIT105,edit,1350631552
Control8=IDC_EDIT106,edit,1350631552
Control9=IDC_EDIT107,edit,1350631552
Control10=IDC_EDIT108,edit,1350631552
Control11=IDC_EDIT109,edit,1350631552
Control12=IDC_EDIT110,edit,1350631552
Control13=IDC_EDIT111,edit,1350631552
Control14=IDC_EDIT112,edit,1350631552
Control15=IDC_EDIT113,edit,1350631552
Control16=IDC_EDIT114,edit,1350631552
Control17=IDC_EDIT115,edit,1350631552
Control18=IDC_EDIT116,edit,1350631552
Control19=IDC_BUTTON1,button,1342242816
Control20=IDC_BUTTON2,button,1342242816
Control21=IDC_BUTTON3,button,1342242816

[DLG:IDD_COMDLG (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=5
Control1=IDCANCEL,button,1342242816
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_BUTTON3,button,1342242816
Control5=IDC_BUTTON4,button,1342242816

[DLG:IDD_CAMDLG (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=5
Control1=IDCANCEL,button,1342242816
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_BUTTON3,button,1342242816
Control5=IDC_BUTTON4,button,1342242816

[DLG:IDD_MOTIONDLG (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=10
Control1=IDCANCEL,button,1342242816
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_BUTTON3,button,1342242816
Control5=IDC_BUTTON4,button,1342242816
Control6=IDC_BUTTON5,button,1342242816
Control7=IDC_BUTTON6,button,1342242816
Control8=IDC_BUTTON7,button,1342242816
Control9=IDC_BUTTON8,button,1342242816
Control10=IDC_BUTTON9,button,1342242816

[DLG:IDD_DIALOG1 (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

