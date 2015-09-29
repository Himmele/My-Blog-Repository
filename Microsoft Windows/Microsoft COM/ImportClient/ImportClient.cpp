// ImportClient.cpp : Legt das Klassenverhalten für die Anwendung fest.
//

#include "stdafx.h"
#include "ImportClient.h"
#include "ImportClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportClientApp

BEGIN_MESSAGE_MAP(CImportClientApp, CWinApp)
	//{{AFX_MSG_MAP(CImportClientApp)
	ON_COMMAND(ID_EXCHANGEDATA_LOAD, OnExchangeDataLoad)
	ON_COMMAND(ID_EXCHANGEDATA_TEST, OnExchangeDataTest)
	ON_COMMAND(ID_EXCHANGEDATA_QUIT, OnExchangeDataQuit)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportClientApp Konstruktion

CImportClientApp::CImportClientApp()
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance platzieren
}

/////////////////////////////////////////////////////////////////////////////
// Das einzige CImportClientApp-Objekt

CImportClientApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CImportClientApp Initialisierung

BOOL CImportClientApp::InitInstance()
{
	AfxEnableControlContainer();
	AfxOleInit();

	// Standardinitialisierung
	// Wenn Sie diese Funktionen nicht nutzen und die Größe Ihrer fertigen 
	//  ausführbaren Datei reduzieren wollen, sollten Sie die nachfolgenden
	//  spezifischen Initialisierungsroutinen, die Sie nicht benötigen, entfernen.

#ifdef _AFXDLL
	Enable3dControls();			// Diese Funktion bei Verwendung von MFC in gemeinsam genutzten DLLs aufrufen
#else
	Enable3dControlsStatic();	// Diese Funktion bei statischen MFC-Anbindungen aufrufen
#endif

	CImportClientDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// ZU ERLEDIGEN: Fügen Sie hier Code ein, um ein Schließen des
		//  Dialogfelds über OK zu steuern
	}
	else if (nResponse == IDCANCEL)
	{
		// ZU ERLEDIGEN: Fügen Sie hier Code ein, um ein Schließen des
		//  Dialogfelds über "Abbrechen" zu steuern
	}

	// Da das Dialogfeld geschlossen wurde, FALSE zurückliefern, so dass wir die
	//  Anwendung verlassen, anstatt das Nachrichtensystem der Anwendung zu starten.
	return FALSE;
}

void CImportClientApp::OnExchangeDataLoad() 
{
	if(m_ExchangeData.CreateInstance(__uuidof(ExchangeData)) != S_OK)
	{
		AfxMessageBox("ExchangeData nicht gefunden!");
		return;
	}
}

void CImportClientApp::OnExchangeDataTest() 
{
	try
	{
		m_ExchangeData->PutTextData("Test");
		m_ExchangeData->PutLongData(7);
		m_ExchangeData->DisplayDialog();
		COleVariant vaData = m_ExchangeData->GetTextData();
		CString strTextData = vaData.bstrVal;
		AfxMessageBox(strTextData);
	}
	catch(_com_error& Error)
	{
		AfxMessageBox(Error.ErrorMessage());
	}
}

void CImportClientApp::OnExchangeDataQuit() 
{
	m_ExchangeData.Release();	
}