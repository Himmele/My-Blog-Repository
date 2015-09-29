; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCOMClientApp
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "COMClient.h"

ClassCount=2
Class1=CCOMClientApp
Class2=CCOMClientDlg

ResourceCount=4
Resource2=IDR_MAINFRAME
Resource3=IDD_COMCLIENT_DIALOG
Resource4=IDR_MENU

[CLS:CCOMClientApp]
Type=0
HeaderFile=COMClient.h
ImplementationFile=COMClient.cpp
Filter=N
LastObject=ID_EXCEL_RUN
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CCOMClientDlg]
Type=0
HeaderFile=COMClientDlg.h
ImplementationFile=COMClientDlg.cpp
Filter=D
LastObject=CCOMClientDlg



[DLG:IDD_COMCLIENT_DIALOG]
Type=1
Class=CCOMClientDlg
ControlCount=1
Control1=IDCANCEL,button,1342242816

[MNU:IDR_MENU]
Type=1
Command1=ID_EXCHANGEDATA_LOAD
Command2=ID_EXCHANGEDATA_TEST
Command3=ID_EXCHANGEDATA_QUIT
Command4=ID_CLOCK_LOAD
Command5=ID_CLOCK_ALARM
Command6=ID_CLOCK_TIME
Command7=ID_CLOCK_QUIT
Command8=ID_EXCEL_LOAD
Command9=ID_EXCEL_RUN
CommandCount=9

