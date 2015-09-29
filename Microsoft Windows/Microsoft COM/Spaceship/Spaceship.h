// Spaceship.h : Haupt-Header-Datei für die DLL SPACESHIP
//

#if !defined(AFX_SPACESHIP_H__482C3AFD_7DC0_4507_B80E_3CD9FB7709CA__INCLUDED_)
#define AFX_SPACESHIP_H__482C3AFD_7DC0_4507_B80E_3CD9FB7709CA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// Hauptsymbole

/////////////////////////////////////////////////////////////////////////////
// CSpaceshipApp
// Siehe Spaceship.cpp für die Implementierung dieser Klasse
//

class CSpaceshipApp : public CWinApp
{
public:
	CSpaceshipApp();

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CSpaceshipApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSpaceshipApp)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

void ITrace(REFIID iid, const char* str);

/////////////////////////////////////////////////////////////////////////////
// Befehlsziel CSpaceship 

class CSpaceship : public CCmdTarget
{
	DECLARE_DYNCREATE(CSpaceship)

private:
	int m_nPosition;
	int m_nAcceleration;
	int m_nColor;

protected:
	CSpaceship();           // Dynamische Erstellung verwendet geschützten Konstruktor

// Attribute
public:

// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSpaceship)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual ~CSpaceship();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSpaceship)
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSpaceship)

	BEGIN_INTERFACE_PART(Motion, IMotion)
		STDMETHOD_(int&, GetPosition) ();
		STDMETHOD_(void, Fly) ();
	END_INTERFACE_PART(Motion)

	BEGIN_INTERFACE_PART(Visual, IVisual)
		STDMETHOD_(void, Display) ();
	END_INTERFACE_PART(Visual)

	// Generierte OLE-Dispatch-Zuordnungsfunktionen
	//{{AFX_DISPATCH(CSpaceship)
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	//}}AFX_DISPATCH
	
	//DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_SPACESHIP_H__482C3AFD_7DC0_4507_B80E_3CD9FB7709CA__INCLUDED_)
