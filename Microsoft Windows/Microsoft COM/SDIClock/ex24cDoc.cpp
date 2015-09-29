// ex24cDoc.cpp : implementation of the CEx24cDoc class
//

#include "stdafx.h"
#include "ex24c.h"

#include "Alarm.h"
#include "ex24cDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEx24cDoc

IMPLEMENT_DYNCREATE(CEx24cDoc, CDocument)

BEGIN_MESSAGE_MAP(CEx24cDoc, CDocument)
	//{{AFX_MSG_MAP(CEx24cDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CEx24cDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CEx24cDoc)
	DISP_PROPERTY_NOTIFY(CEx24cDoc, "Time", m_time, OnTimeChanged, VT_DATE)
	DISP_FUNCTION(CEx24cDoc, "ShowWin", ShowWin, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CEx24cDoc, "CreateAlarm", CreateAlarm, VT_DISPATCH, VTS_DATE)
	DISP_FUNCTION(CEx24cDoc, "RefreshWin", Refresh, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_PARAM(CEx24cDoc, "Figure", GetFigure, SetFigure, VT_VARIANT, VTS_I2)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IEx24c to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {A9515B02-5B85-11D0-848F-00400526305B}
static const IID IID_IEx24c =
{ 0xa9515b02, 0x5b85, 0x11d0, { 0x84, 0x8f, 0x0, 0x40, 0x5, 0x26, 0x30, 0x5b } };

BEGIN_INTERFACE_MAP(CEx24cDoc, CDocument)
	INTERFACE_PART(CEx24cDoc, IID_IEx24c, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEx24cDoc construction/destruction

CEx24cDoc::CEx24cDoc()
{
	// TODO: add one-time construction code here

	TRACE("Entering CEx24cDoc ctor\n");
	m_time = COleDateTime(0, 0, 0, 5, 10, 15);
	// bogus initial values
	m_strFigure[0] = "XII"; m_strFigure[1] = "III";
	m_strFigure[2] = "VI"; m_strFigure[3] = "IX";
	m_pAlarm = NULL;

	EnableAutomation();

	AfxOleLockApp();
}

CEx24cDoc::~CEx24cDoc()
{
	AfxOleUnlockApp();
}

BOOL CEx24cDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CEx24cDoc serialization

void CEx24cDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEx24cDoc diagnostics

#ifdef _DEBUG
void CEx24cDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEx24cDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEx24cDoc commands

void CEx24cDoc::OnTimeChanged() 
{
	// TODO: Add notification handler code

}

VARIANT CEx24cDoc::GetFigure(short n) 
{
	TRACE("Entering CEx24cDoc::GetFigure -- n = %d m_strFigure[n] = %s\n",
           n, m_strFigure[n]);
	return COleVariant(m_strFigure[n]).Detach();
}

void CEx24cDoc::SetFigure(short n, const VARIANT FAR& newValue) 
{
	TRACE("Entering CEx24cDoc::SetFigure -- n = %d, vt = %d\n", n, 
		   newValue.vt);
	COleVariant vaTemp;
	vaTemp.ChangeType(VT_BSTR, (COleVariant*) &newValue);
	m_strFigure[n] = vaTemp.bstrVal; // converts double-to-single
}

void CEx24cDoc::ShowWin() 
{
	TRACE("Entering CEx24cDoc::ShowWin\n");
	CRect rectWindow;
	CWnd* pFrm = AfxGetApp()->m_pMainWnd;
	pFrm->GetWindowRect(rectWindow);
	WINDOWPLACEMENT wndpl;
	wndpl.length = sizeof(WINDOWPLACEMENT);
	wndpl.showCmd = SW_SHOWNORMAL;
	wndpl.rcNormalPosition.left = rectWindow.left;
	wndpl.rcNormalPosition.top = rectWindow.top;
	wndpl.rcNormalPosition.right = rectWindow.left + 150;
	wndpl.rcNormalPosition.bottom = rectWindow.top + 150;
	pFrm->SetWindowPlacement(&wndpl);
	pFrm->ShowWindow(AfxGetApp()->m_nCmdShow);
	pFrm->UpdateWindow();
	pFrm->BringWindowToTop();
}

LPDISPATCH CEx24cDoc::CreateAlarm(DATE time) 
{
	TRACE("Entering CEx24cDoc::CreateAlarm, time = %f\n", time);
	// OLE deletes any prior CAlarm object
	m_pAlarm = new CAlarm(time);
	return m_pAlarm->GetIDispatch(FALSE);   // no AddRef here
}

void CEx24cDoc::Refresh() 
{
	TRACE("Entering CEx24cDoc::Refresh\n");
	UpdateAllViews(NULL);  // invalidates the one-and-only view
//	AfxGetApp()->m_pMainWnd->BringWindowToTop();
}

BOOL CEx24cDoc::SaveModified() 
{
	return TRUE;
}
