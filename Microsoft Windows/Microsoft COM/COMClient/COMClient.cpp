// COMClient.cpp : Legt das Klassenverhalten für die Anwendung fest.
//

#include "stdafx.h"
#include "COMClient.h"
#include "COMClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCOMClientApp

BEGIN_MESSAGE_MAP(CCOMClientApp, CWinApp)
	//{{AFX_MSG_MAP(CCOMClientApp)
	ON_COMMAND(ID_EXCHANGEDATA_LOAD, OnExchangeDataLoad)
	ON_COMMAND(ID_EXCHANGEDATA_TEST, OnExchangeDataTest)
	ON_COMMAND(ID_EXCHANGEDATA_QUIT, OnExchangeDataQuit)
	ON_COMMAND(ID_CLOCK_ALARM, OnClockAlarm)
	ON_COMMAND(ID_CLOCK_LOAD, OnClockLoad)
	ON_COMMAND(ID_CLOCK_TIME, OnClockTime)
	ON_COMMAND(ID_CLOCK_QUIT, OnClockQuit)
	ON_COMMAND(ID_EXCEL_LOAD, OnExcelLoad)
	ON_COMMAND(ID_EXCEL_RUN, OnExcelRun)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCOMClientApp Konstruktion

CCOMClientApp::CCOMClientApp()
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance platzieren
}

/////////////////////////////////////////////////////////////////////////////
// Das einzige CCOMClientApp-Objekt

CCOMClientApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCOMClientApp Initialisierung

BOOL CCOMClientApp::InitInstance()
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

	CCOMClientDlg dlg;
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

void CCOMClientApp::OnExchangeDataLoad() 
{
	if(!m_ExchangeData.CreateDispatch("ExchangeData"))
	{
		AfxMessageBox("ExchangeData Komponente nicht gefunden!");
		return;
	}

	m_ExchangeData.SetTextData("Test");
	m_ExchangeData.SetLongData(7);
}

void CCOMClientApp::OnExchangeDataTest() 
{
	m_ExchangeData.DisplayDialog();
	COleVariant vaData = m_ExchangeData.GetTextData();
	CString strTextData = vaData.bstrVal;
	AfxMessageBox(strTextData);
}

void CCOMClientApp::OnExchangeDataQuit() 
{
	m_ExchangeData.ReleaseDispatch();	
}

void CCOMClientApp::OnClockAlarm() 
{
	COleDateTime Date(2001, 7, 15, 14, 30, 59);
	LPDISPATCH pAlarm = m_Ex24c.CreateAlarm(Date);
	m_Alarm.AttachDispatch(pAlarm);
	m_Ex24c.RefreshWin();
}

void CCOMClientApp::OnClockLoad() 
{
	if(!m_Ex24c.CreateDispatch("Ex24c.Document"))
	{
		AfxMessageBox("Ex24c.Document nicht gefunden!");
		return;
	}

	m_Ex24c.SetFigure(0, COleVariant("XII"));
	m_Ex24c.SetFigure(1, COleVariant("III"));
	m_Ex24c.SetFigure(2, COleVariant("VI"));
	m_Ex24c.SetFigure(3, COleVariant("IX"));
	OnClockTime();
	m_Ex24c.ShowWin();
}

void CCOMClientApp::OnClockTime() 
{
	COleDateTime Now = COleDateTime::GetCurrentTime();
	m_Ex24c.SetTime(Now);
	m_Ex24c.RefreshWin();
}

void CCOMClientApp::OnClockQuit() 
{
	m_Ex24c.ReleaseDispatch();	
}

void CCOMClientApp::OnExcelLoad() 
{
	LPDISPATCH pDisp;
	LPUNKNOWN pUnk;
	CLSID clsid;

	BeginWaitCursor();
	::CLSIDFromProgID(L"Excel.Application", &clsid);
	
	if(::GetActiveObject(clsid, NULL, &pUnk) == S_OK)
	{
		pUnk->QueryInterface(IID_IDispatch, (void**)&pDisp);
		m_Excel.AttachDispatch(pDisp);
		pUnk->Release();
	}
	else
	{
		if(!m_Excel.CreateDispatch("Excel.Application"))
			AfxMessageBox("Microsoft Excel nicht gefunden!");
	}

	EndWaitCursor();
}

void CCOMClientApp::OnExcelRun() 
{
	LPDISPATCH pRange, pWorkbooks;
	
	CWnd* pWnd = CWnd::FindWindow("XLMAIN", NULL);
	if(pWnd != NULL)
	{
		pWnd->ShowWindow(SW_SHOWNORMAL);
		pWnd->UpdateWindow();
		pWnd->BringWindowToTop();
	}

	m_Excel.SetSheetsInNewWorkbook(1);
	pWorkbooks = m_Excel.GetWorkbooks();
	m_Workbooks.AttachDispatch(pWorkbooks);

	LPDISPATCH pWorkbook = NULL;
	if(m_Workbooks.GetCount() == 0)
	{
		pWorkbook = m_Workbooks.Add(COleVariant((short) 1));
	}

	LPDISPATCH pWorksheets = m_Excel.GetWorksheets();
	m_Worksheets.AttachDispatch(pWorksheets);
	LPDISPATCH pWorksheet = m_Worksheets.GetItem(COleVariant((short) 1));
	m_Worksheet.AttachDispatch(pWorksheet);
	m_Worksheet.Select(COleVariant((short) 1));

	pRange = m_Worksheet.GetRange(COleVariant("A1"));
	m_Range[0].AttachDispatch(pRange);

	pRange = m_Worksheet.GetRange(COleVariant("A3"));
	m_Range[1].AttachDispatch(pRange);

	pRange = m_Worksheet.GetRange(COleVariant("A4"));
	m_Range[2].AttachDispatch(pRange);

	pRange = m_Worksheet.GetRange(COleVariant("A4"), COleVariant("C6"));
	m_Range[3].AttachDispatch(pRange);

	pRange = m_Worksheet.GetRange(COleVariant("A8"));
	m_Range[4].AttachDispatch(pRange);

	m_Range[4].SetValue(COleVariant("Excel"));
	m_Range[0].SetValue(COleVariant("Test"));
	m_Range[1].SetValue(COleVariant(3.14159));
	m_Range[2].SetFormula(COleVariant("=$A3*2.0"));
	m_Range[3].FillRight();
	m_Range[3].FillDown();

	if(pWorkbook != NULL)
		pWorkbook->Release();
}