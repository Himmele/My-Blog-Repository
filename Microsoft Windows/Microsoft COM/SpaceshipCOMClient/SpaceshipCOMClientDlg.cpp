// SpaceshipCOMClientDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "SpaceshipCOMClient.h"
#include "SpaceshipCOMClientDlg.h"
#include "Interface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// Keine Nachrichten-Handler
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpaceshipCOMClientDlg Dialogfeld

CSpaceshipCOMClientDlg::CSpaceshipCOMClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpaceshipCOMClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpaceshipCOMClientDlg)
		// HINWEIS: Der Klassenassistent fügt hier Member-Initialisierung ein
	//}}AFX_DATA_INIT
	// Beachten Sie, dass LoadIcon unter Win32 keinen nachfolgenden DestroyIcon-Aufruf benötigt
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSpaceshipCOMClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpaceshipCOMClientDlg)
		// HINWEIS: Der Klassenassistent fügt an dieser Stelle DDX- und DDV-Aufrufe ein
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSpaceshipCOMClientDlg, CDialog)
	//{{AFX_MSG_MAP(CSpaceshipCOMClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_COMCLIENT_BUTTON, OnCOMClientButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpaceshipCOMClientDlg Nachrichten-Handler

BOOL CSpaceshipCOMClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden
	
	// ZU ERLEDIGEN: Hier zusätzliche Initialisierung einfügen
	
	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

void CSpaceshipCOMClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// Wollen Sie Ihrem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch für Sie erledigt.

void CSpaceshipCOMClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext für Zeichnen

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Symbol in Client-Rechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Die Systemaufrufe fragen den Cursorform ab, die angezeigt werden soll, während der Benutzer
//  das zum Symbol verkleinerte Fenster mit der Maus zieht.
HCURSOR CSpaceshipCOMClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


static const IID IID_IMotion = { 0x30af6bac, 0xbd29, 0x436e, { 0xaf, 0xfa, 0x95, 0xa, 0xf4, 0x83, 0x84, 0x3a } };
static const IID IID_IVisual = { 0x30af6bad, 0xbd29, 0x436e, { 0xaf, 0xfa, 0x95, 0xa, 0xf4, 0x83, 0x84, 0x3a } };


void CSpaceshipCOMClientDlg::OnCOMClientButton() 
{
	AfxOleInit();

	CLSID clsid;
	LPCLASSFACTORY pClf;
	LPUNKNOWN pUnk;
	IMotion* pMot;
	IVisual* pVis;
	HRESULT hr;

	if((hr = ::CLSIDFromProgID(L"Spaceship.Spaceship", &clsid)) != NOERROR)
	{
		TRACE("Unable to find Program ID - Error: %x\n", hr);
		return;
	}

	if((hr = ::CoGetClassObject(clsid, 
								CLSCTX_INPROC_SERVER, 
								NULL,
								IID_IUnknown, //IID_IClassFactory,
								(void**) &pClf)) != NOERROR)
	{
		TRACE("Unable to find CLSID - Error: %x\n", hr);
		return;
	}

	pClf->CreateInstance(NULL, IID_IUnknown, (void**) &pUnk);
	pUnk->QueryInterface(IID_IMotion, (void**) &pMot);
	pMot->QueryInterface(IID_IVisual, (void**) &pVis);

	TRACE("pUnk: %p, pMot: %p, pVis: %p\n", pUnk, pMot, pVis);

	pMot->Fly();
	int nPos = pMot->GetPosition();
	TRACE("nPos: %d\n", nPos);
	pVis->Display();

	pClf->Release();
	pUnk->Release();
	pMot->Release();
	pVis->Release();

	AfxMessageBox("Test succeeded. See debug window for output.\n");
}
