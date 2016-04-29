# Microsoft Developer Studio Project File - Name="MicroMecanum" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MicroMecanum - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MicroMecanum.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MicroMecanum.mak" CFG="MicroMecanum - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MicroMecanum - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MicroMecanum - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Robusys", DAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MicroMecanum - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"Release/Robusys5.5-0427.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "MicroMecanum - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MicroMecanum - Win32 Release"
# Name "MicroMecanum - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "jsoncpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\json\json_reader.cpp
# End Source File
# Begin Source File

SOURCE=.\json\json_value.cpp
# End Source File
# Begin Source File

SOURCE=.\json\json_writer.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Cammera.cpp
# End Source File
# Begin Source File

SOURCE=.\Com.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectThread.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageHandling.cpp
# End Source File
# Begin Source File

SOURCE=.\MicroMecanum.cpp
# End Source File
# Begin Source File

SOURCE=.\MicroMecanum.rc
# End Source File
# Begin Source File

SOURCE=.\MicroMecanumDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Motion.cpp
# End Source File
# Begin Source File

SOURCE=.\mscomm.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessThread.cpp
# End Source File
# Begin Source File

SOURCE=.\Route.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "jsonh"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\json\autolink.h
# End Source File
# Begin Source File

SOURCE=.\json\config.h
# End Source File
# Begin Source File

SOURCE=.\json\features.h
# End Source File
# Begin Source File

SOURCE=.\json\forwards.h
# End Source File
# Begin Source File

SOURCE=.\json\json.h
# End Source File
# Begin Source File

SOURCE=.\json\json_batchallocator.h
# End Source File
# Begin Source File

SOURCE=.\json\json_internalarray.inl
# End Source File
# Begin Source File

SOURCE=.\json\json_internalmap.inl
# End Source File
# Begin Source File

SOURCE=.\json\json_valueiterator.inl
# End Source File
# Begin Source File

SOURCE=.\json\reader.h
# End Source File
# Begin Source File

SOURCE=.\json\value.h
# End Source File
# Begin Source File

SOURCE=.\json\writer.h
# End Source File
# End Group
# Begin Group "MAC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MAC\IPEXPORT.H
# End Source File
# Begin Source File

SOURCE=.\MAC\Iphlpapi.h
# End Source File
# Begin Source File

SOURCE=.\MAC\IPRTRMIB.H
# End Source File
# Begin Source File

SOURCE=.\MAC\IPTYPES.H
# End Source File
# End Group
# Begin Source File

SOURCE=.\Cammera.h
# End Source File
# Begin Source File

SOURCE=.\Com.h
# End Source File
# Begin Source File

SOURCE=.\ConnectThread.h
# End Source File
# Begin Source File

SOURCE=.\ConsFunc.h
# End Source File
# Begin Source File

SOURCE=.\JHCap.h
# End Source File
# Begin Source File

SOURCE=.\MessageHandling.h
# End Source File
# Begin Source File

SOURCE=.\MicroMecanum.h
# End Source File
# Begin Source File

SOURCE=.\MicroMecanum_i.h
# End Source File
# Begin Source File

SOURCE=.\MicroMecanumDlg.h
# End Source File
# Begin Source File

SOURCE=.\Motion.h
# End Source File
# Begin Source File

SOURCE=.\MotionVision.h
# End Source File
# Begin Source File

SOURCE=.\mscomm.h
# End Source File
# Begin Source File

SOURCE=.\PciTypes.h
# End Source File
# Begin Source File

SOURCE=.\PCL6045BLdef.h
# End Source File
# Begin Source File

SOURCE=.\PexApi.h
# End Source File
# Begin Source File

SOURCE=.\Plx.h
# End Source File
# Begin Source File

SOURCE=.\PlxApi.h
# End Source File
# Begin Source File

SOURCE=.\PlxDefCk.h
# End Source File
# Begin Source File

SOURCE=.\PlxError.h
# End Source File
# Begin Source File

SOURCE=.\PlxInit.h
# End Source File
# Begin Source File

SOURCE=.\PlxLegcy.h
# End Source File
# Begin Source File

SOURCE=.\PlxTypes.h
# End Source File
# Begin Source File

SOURCE=.\ProcessThread.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Route.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\MicroMecanum.ico
# End Source File
# Begin Source File

SOURCE=.\res\MicroMecanum.rc2
# End Source File
# Begin Source File

SOURCE=.\MicroMecanum.rgs
# End Source File
# End Group
# Begin Source File

SOURCE=.\MicroMecanum.clw
# End Source File
# Begin Source File

SOURCE=.\MotionVision.dll
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\MotionVision.lib
# End Source File
# End Target
# End Project
# Section MicroMecanum : {648A5600-2C6E-101B-82B6-000000000014}
# 	2:21:DefaultSinkHeaderFile:mscomm.h
# 	2:16:DefaultSinkClass:CMSComm
# End Section
# Section MicroMecanum : {E6E17E90-DF38-11CF-8E74-00A0C90F26F8}
# 	2:5:Class:CMSComm
# 	2:10:HeaderFile:mscomm.h
# 	2:8:ImplFile:mscomm.cpp
# End Section
# Section MicroMecanum : {00000001-0001-0000-0000-000000000000}
# 	1:16:IDR_MICROMECANUM:104
# End Section
