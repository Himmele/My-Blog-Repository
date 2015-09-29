#ifndef AFX_PROMPTDLG_H__C5DFCD08_89DB_41F3_A8B1_DDAE06288FE7__INCLUDED_
#define AFX_PROMPTDLG_H__C5DFCD08_89DB_41F3_A8B1_DDAE06288FE7__INCLUDED_

// PromptDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CPromptDlg 

class CPromptDlg : public CDialog
{
// Konstruktion
public:
	CPromptDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CPromptDlg)
	enum { IDD = IDD_PROMPT_DIALOG };
	CString	m_String;
	long	m_Figure;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CPromptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CPromptDlg)
		// HINWEIS: Der Klassen-Assistent fügt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // AFX_PROMPTDLG_H__C5DFCD08_89DB_41F3_A8B1_DDAE06288FE7__INCLUDED_
