// COMClient.h : Haupt-Header-Datei für die Anwendung COMCLIENT
//

#if !defined(AFX_COMCLIENT_H__81B77C51_00F7_45C9_99D7_AD4F609958E9__INCLUDED_)
#define AFX_COMCLIENT_H__81B77C51_00F7_45C9_99D7_AD4F609958E9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// Hauptsymbole
#include "exchangedata.h"	// Hinzugefügt von ClassView
#include "ex24c.h"	// Hinzugefügt von ClassView
#include "excel.h"	// Hinzugefügt von ClassView

/////////////////////////////////////////////////////////////////////////////
// CCOMClientApp:
// Siehe COMClient.cpp für die Implementierung dieser Klasse
//

class CCOMClientApp : public CWinApp
{
public:
	Worksheets m_Worksheets;
	Workbooks m_Workbooks;
	_Worksheet m_Worksheet;
	Range m_Range[5];
	_Application m_Excel;
	IEx24c m_Ex24c;
	IAlarm m_Alarm;
	IExchangeDataAuto m_ExchangeData;
	CCOMClientApp();

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CCOMClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementierung

	//{{AFX_MSG(CCOMClientApp)
	afx_msg void OnExchangeDataLoad();
	afx_msg void OnExchangeDataTest();
	afx_msg void OnExchangeDataQuit();
	afx_msg void OnClockAlarm();
	afx_msg void OnClockLoad();
	afx_msg void OnClockTime();
	afx_msg void OnClockQuit();
	afx_msg void OnExcelLoad();
	afx_msg void OnExcelRun();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_COMCLIENT_H__81B77C51_00F7_45C9_99D7_AD4F609958E9__INCLUDED_)
