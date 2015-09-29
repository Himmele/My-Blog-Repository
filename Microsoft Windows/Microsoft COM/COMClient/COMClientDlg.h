// COMClientDlg.h : Header-Datei
//

#if !defined(AFX_COMCLIENTDLG_H__210B9EB8_953B_4A4B_9B61_CA905EFBF00B__INCLUDED_)
#define AFX_COMCLIENTDLG_H__210B9EB8_953B_4A4B_9B61_CA905EFBF00B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CCOMClientDlg Dialogfeld

class CCOMClientDlg : public CDialog
{
// Konstruktion
public:
	CCOMClientDlg(CWnd* pParent = NULL);	// Standard-Konstruktor

// Dialogfelddaten
	//{{AFX_DATA(CCOMClientDlg)
	enum { IDD = IDD_COMCLIENT_DIALOG };
		// HINWEIS: der Klassenassistent fügt an dieser Stelle Datenelemente (Members) ein
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CCOMClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Message-Map-Funktionen
	//{{AFX_MSG(CCOMClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_COMCLIENTDLG_H__210B9EB8_953B_4A4B_9B61_CA905EFBF00B__INCLUDED_)
