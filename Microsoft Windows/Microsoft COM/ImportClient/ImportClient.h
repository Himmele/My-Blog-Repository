// ImportClient.h : Haupt-Header-Datei für die Anwendung IMPORTCLIENT
//

#if !defined(AFX_IMPORTCLIENT_H__78012BD8_CFA5_4396_9F29_1EEDA2B72A14__INCLUDED_)
#define AFX_IMPORTCLIENT_H__78012BD8_CFA5_4396_9F29_1EEDA2B72A14__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// Hauptsymbole

/////////////////////////////////////////////////////////////////////////////
// CImportClientApp:
// Siehe ImportClient.cpp für die Implementierung dieser Klasse
//

class CImportClientApp : public CWinApp
{
public:
	CImportClientApp();
	IExchangeDataAutoPtr m_ExchangeData;

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CImportClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementierung

	//{{AFX_MSG(CImportClientApp)
	afx_msg void OnExchangeDataLoad();
	afx_msg void OnExchangeDataTest();
	afx_msg void OnExchangeDataQuit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_IMPORTCLIENT_H__78012BD8_CFA5_4396_9F29_1EEDA2B72A14__INCLUDED_)
