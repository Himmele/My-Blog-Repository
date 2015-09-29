// ImportClientDlg.h : Header-Datei
//

#if !defined(AFX_IMPORTCLIENTDLG_H__2978E805_3307_4E31_A63D_1101D1D665CE__INCLUDED_)
#define AFX_IMPORTCLIENTDLG_H__2978E805_3307_4E31_A63D_1101D1D665CE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CImportClientDlg Dialogfeld

class CImportClientDlg : public CDialog
{
// Konstruktion
public:
	CImportClientDlg(CWnd* pParent = NULL);	// Standard-Konstruktor

// Dialogfelddaten
	//{{AFX_DATA(CImportClientDlg)
	enum { IDD = IDD_IMPORTCLIENT_DIALOG };
		// HINWEIS: der Klassenassistent fügt an dieser Stelle Datenelemente (Members) ein
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CImportClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Message-Map-Funktionen
	//{{AFX_MSG(CImportClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_IMPORTCLIENTDLG_H__2978E805_3307_4E31_A63D_1101D1D665CE__INCLUDED_)
