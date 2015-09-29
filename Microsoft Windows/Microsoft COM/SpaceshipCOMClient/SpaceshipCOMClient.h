// SpaceshipCOMClient.h : Haupt-Header-Datei für die Anwendung SPACESHIPCOMCLIENT
//

#if !defined(AFX_SPACESHIPCOMCLIENT_H__A2F606CE_5874_46DF_B9FF_FE081FD88551__INCLUDED_)
#define AFX_SPACESHIPCOMCLIENT_H__A2F606CE_5874_46DF_B9FF_FE081FD88551__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// Hauptsymbole

/////////////////////////////////////////////////////////////////////////////
// CSpaceshipCOMClientApp:
// Siehe SpaceshipCOMClient.cpp für die Implementierung dieser Klasse
//

class CSpaceshipCOMClientApp : public CWinApp
{
public:
	CSpaceshipCOMClientApp();

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CSpaceshipCOMClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementierung

	//{{AFX_MSG(CSpaceshipCOMClientApp)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_SPACESHIPCOMCLIENT_H__A2F606CE_5874_46DF_B9FF_FE081FD88551__INCLUDED_)
