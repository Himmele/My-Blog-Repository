; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ODLFile=ExchangeData.odl
ClassCount=3
Class1=CExchangeDataApp
LastClass=CExchangeDataAuto
NewFileInclude2=#include "ExchangeData.h"
ResourceCount=1
NewFileInclude1=#include "stdafx.h"
Class2=CPromptDlg
LastTemplate=CCmdTarget
Class3=CExchangeDataAuto
Resource1=IDD_PROMPT_DIALOG

[CLS:CExchangeDataApp]
Type=0
HeaderFile=ExchangeData.h
ImplementationFile=ExchangeData.cpp
Filter=N
LastObject=CExchangeDataApp

[DLG:IDD_PROMPT_DIALOG]
Type=1
Class=CPromptDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_STRING,edit,1350631552
Control4=IDC_EDIT_FIGURE,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[CLS:CPromptDlg]
Type=0
HeaderFile=PromptDlg.h
ImplementationFile=PromptDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CPromptDlg
VirtualFilter=dWC

[CLS:CExchangeDataAuto]
Type=0
HeaderFile=ExchangeDataAuto.h
ImplementationFile=ExchangeDataAuto.cpp
BaseClass=CCmdTarget
Filter=N
VirtualFilter=C
LastObject=CExchangeDataAuto

