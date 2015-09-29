// SpaceshipCOMClientDlg.h : Header-Datei
//

#if !defined(AFX_SPACESHIPCOMCLIENTDLG_H__553E8415_638C_41FE_B682_36416623AAE6__INCLUDED_)
#define AFX_SPACESHIPCOMCLIENTDLG_H__553E8415_638C_41FE_B682_36416623AAE6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CSpaceshipCOMClientDlg Dialogfeld

class CSpaceshipCOMClientDlg : public CDialog
{
// Konstruktion
public:
	CSpaceshipCOMClientDlg(CWnd* pParent = NULL);	// Standard-Konstruktor

// Dialogfelddaten
	//{{AFX_DATA(CSpaceshipCOMClientDlg)
	enum { IDD = IDD_SPACESHIPCOMCLIENT_DIALOG };
		// HINWEIS: der Klassenassistent fügt an dieser Stelle Datenelemente (Members) ein
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CSpaceshipCOMClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Message-Map-Funktionen
	//{{AFX_MSG(CSpaceshipCOMClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCOMClientButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_SPACESHIPCOMCLIENTDLG_H__553E8415_638C_41FE_B682_36416623AAE6__INCLUDED_)
