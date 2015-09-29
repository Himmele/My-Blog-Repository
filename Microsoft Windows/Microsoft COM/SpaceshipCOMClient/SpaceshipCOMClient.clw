; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSpaceshipCOMClientDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SpaceshipCOMClient.h"

ClassCount=4
Class1=CSpaceshipCOMClientApp
Class2=CSpaceshipCOMClientDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SPACESHIPCOMCLIENT_DIALOG

[CLS:CSpaceshipCOMClientApp]
Type=0
HeaderFile=SpaceshipCOMClient.h
ImplementationFile=SpaceshipCOMClient.cpp
Filter=N

[CLS:CSpaceshipCOMClientDlg]
Type=0
HeaderFile=SpaceshipCOMClientDlg.h
ImplementationFile=SpaceshipCOMClientDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=SpaceshipCOMClientDlg.h
ImplementationFile=SpaceshipCOMClientDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SPACESHIPCOMCLIENT_DIALOG]
Type=1
Class=CSpaceshipCOMClientDlg
ControlCount=1
Control1=IDC_COMCLIENT_BUTTON,button,1342242816

