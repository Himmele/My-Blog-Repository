// ElectronicCash.h : Haupt-Header-Datei für die Anwendung ELECTRONICCASH
//

#if !defined(AFX_ELECTRONICCASH_H__75BED93B_7B1C_4AA9_BBDC_F394DCEBAE3B__INCLUDED_)
#define AFX_ELECTRONICCASH_H__75BED93B_7B1C_4AA9_BBDC_F394DCEBAE3B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// Hauptsymbole

#define IMPLEMENT_OLECREATE2(class_name, external_name, \
	l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	AFX_DATADEF COleObjectFactory class_name::factory(class_name::guid, \
	RUNTIME_CLASS(class_name), TRUE, _T(external_name)); \
	const AFX_DATADEF GUID class_name::guid = \
	{l, w1, w2, {b1, b2, b3, b4, b5, b6, b7, b8}};

/////////////////////////////////////////////////////////////////////////////
// CElectronicCashApp:
// Siehe ElectronicCash.cpp für die Implementierung dieser Klasse
//

class CElectronicCashApp : public CWinApp
{
public:
	CElectronicCashApp();

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CElectronicCashApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementierung

	//{{AFX_MSG(CElectronicCashApp)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// Befehlsziel CElectronicCash 

class CElectronicCash : public CCmdTarget
{
	DECLARE_DYNCREATE(CElectronicCash)

	CElectronicCash();           // Dynamische Erstellung verwendet geschützten Konstruktor

// Attribute
public:

// Operationen
public:
	double m_dBalance;

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CElectronicCash)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual ~CElectronicCash();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CElectronicCash)
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CElectronicCash)

	// Generierte OLE-Dispatch-Zuordnungsfunktionen
	//{{AFX_DISPATCH(CElectronicCash)
	afx_msg double GetBalance();
	afx_msg void SetBalance(double newValue);
	afx_msg double Withdrawal(double dAmount);
	afx_msg void Deposit(double dAmount);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_ELECTRONICCASH_H__75BED93B_7B1C_4AA9_BBDC_F394DCEBAE3B__INCLUDED_)
