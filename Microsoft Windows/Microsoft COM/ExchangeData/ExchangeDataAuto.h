#ifndef AFX_EXCHANGEDATAAUTO_H__F9FA6754_6841_4CBA_B472_D8C7A2DB8440__INCLUDED_
#define AFX_EXCHANGEDATAAUTO_H__F9FA6754_6841_4CBA_B472_D8C7A2DB8440__INCLUDED_

// ExchangeDataAuto.h : Header-Datei
//



/////////////////////////////////////////////////////////////////////////////
// Befehlsziel CExchangeDataAuto 

class CExchangeDataAuto : public CCmdTarget
{
	DECLARE_DYNCREATE(CExchangeDataAuto)

	CExchangeDataAuto();           // Dynamische Erstellung verwendet geschützten Konstruktor

// Attribute
public:

// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CExchangeDataAuto)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual ~CExchangeDataAuto();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CExchangeDataAuto)
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CExchangeDataAuto)

	// Generierte OLE-Dispatch-Zuordnungsfunktionen
	//{{AFX_DISPATCH(CExchangeDataAuto)
	long m_LongData;
	afx_msg void OnLongDataChanged();
	VARIANT m_TextData;
	afx_msg void OnTextDataChanged();
	afx_msg BOOL DisplayDialog();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // AFX_EXCHANGEDATAAUTO_H__F9FA6754_6841_4CBA_B472_D8C7A2DB8440__INCLUDED_
