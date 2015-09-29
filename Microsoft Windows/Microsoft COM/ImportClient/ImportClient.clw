; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CImportClientApp
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ImportClient.h"

ClassCount=2
Class1=CImportClientApp
Class2=CImportClientDlg

ResourceCount=4
Resource2=IDR_MAINFRAME
Resource3=IDR_MENU
Resource4=IDD_IMPORTCLIENT_DIALOG

[CLS:CImportClientApp]
Type=0
HeaderFile=ImportClient.h
ImplementationFile=ImportClient.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=ID_EXCHANGEDATA_QUIT

[CLS:CImportClientDlg]
Type=0
HeaderFile=ImportClientDlg.h
ImplementationFile=ImportClientDlg.cpp
Filter=D
LastObject=CImportClientDlg



[DLG:IDD_IMPORTCLIENT_DIALOG]
Type=1
Class=CImportClientDlg
ControlCount=1
Control1=IDCANCEL,button,1342242816

[MNU:IDR_MENU]
Type=1
Command1=ID_EXCHANGEDATA_LOAD
Command2=ID_EXCHANGEDATA_TEST
Command3=ID_EXCHANGEDATA_QUIT
CommandCount=3

