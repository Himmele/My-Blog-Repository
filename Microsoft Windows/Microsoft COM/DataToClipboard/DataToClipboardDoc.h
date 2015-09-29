// DataToClipboardDoc.h : Schnittstelle der Klasse CDataToClipboardDoc
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATATOCLIPBOARDDOC_H__AFBB9174_A25B_43D4_83EB_3B8678D78114__INCLUDED_)
#define AFX_DATATOCLIPBOARDDOC_H__AFBB9174_A25B_43D4_83EB_3B8678D78114__INCLUDED_

#include "..\MUHCAMP\cdib.h"	// Hinzugefügt von ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CDataToClipboardDoc : public CDocument
{
protected: // Nur aus Serialisierung erzeugen
	CDataToClipboardDoc();
	DECLARE_DYNCREATE(CDataToClipboardDoc)

// Attribute
public:

// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CDataToClipboardDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementierung
public:
	bool m_bDragHere;
	CDib m_Dib;
	virtual ~CDataToClipboardDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CDataToClipboardDoc)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_DATATOCLIPBOARDDOC_H__AFBB9174_A25B_43D4_83EB_3B8678D78114__INCLUDED_)
