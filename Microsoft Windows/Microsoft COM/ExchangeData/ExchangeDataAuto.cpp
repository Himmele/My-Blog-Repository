// ExchangeDataAuto.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "ExchangeData.h"
#include "ExchangeDataAuto.h"
#include "PromptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExchangeDataAuto

IMPLEMENT_DYNCREATE(CExchangeDataAuto, CCmdTarget)

CExchangeDataAuto::CExchangeDataAuto()
{
	EnableAutomation();
	
	// Um die Ausführung der Anwendung fortzusetzen, solange ein OLE-Automatisierungs
	//	objekt aktiv ist, ruft der Konstruktor AfxOleLockApp auf.
	
	::VariantInit(&m_TextData);
	m_LongData = 0;
	AfxOleLockApp();
}

CExchangeDataAuto::~CExchangeDataAuto()
{
	// Um die Anwendung zu beenden, nachdem alle Objekte durch OLE-Automatisierung
	// 	erstellt wurden, ruft der Destruktor AfxOleUnlockApp auf.
	
	AfxOleUnlockApp();
}


void CExchangeDataAuto::OnFinalRelease()
{
	// Nachdem die letzte Referenz auf ein Automatisierungsobjekt freigegeben wurde,
	// wird OnFinalRelease aufgerufen. Die Basisklasse löscht das Objekt
	// automatisch. Fügen Sie zusätzlichen Bereinigungscode für Ihr Objekt
	// hinzu, bevor Sie die Basisklasse aufrufen.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CExchangeDataAuto, CCmdTarget)
	//{{AFX_MSG_MAP(CExchangeDataAuto)
		// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CExchangeDataAuto, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CExchangeDataAuto)
	DISP_PROPERTY_NOTIFY(CExchangeDataAuto, "LongData", m_LongData, OnLongDataChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CExchangeDataAuto, "TextData", m_TextData, OnTextDataChanged, VT_VARIANT)
	DISP_FUNCTION(CExchangeDataAuto, "DisplayDialog", DisplayDialog, VT_BOOL, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Hinweis: Wir stellen Unterstützung für IID_IExchangeDataAuto zur Verfügung, um typsicheres Binden
//  von VBA zu ermöglichen. Diese IID muss mit der GUID übereinstimmen, die in der
//  Disp-Schnittstelle in der .ODL-Datei angegeben ist.

// {96BD13D6-DC81-49DC-AADD-D290DDC4FC42}
static const IID IID_IExchangeDataAuto =
{ 0x96bd13d6, 0xdc81, 0x49dc, { 0xaa, 0xdd, 0xd2, 0x90, 0xdd, 0xc4, 0xfc, 0x42 } };

BEGIN_INTERFACE_MAP(CExchangeDataAuto, CCmdTarget)
	INTERFACE_PART(CExchangeDataAuto, IID_IExchangeDataAuto, Dispatch)
END_INTERFACE_MAP()

// {2BBC3374-3403-4195-8929-3B66C0B58280}
IMPLEMENT_OLECREATE(CExchangeDataAuto, "ExchangeData", 0x2bbc3374, 0x3403, 0x4195, 0x89, 0x29, 0x3b, 0x66, 0xc0, 0xb5, 0x82, 0x80)

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CExchangeDataAuto 

void CExchangeDataAuto::OnLongDataChanged() 
{
	// ZU ERLEDIGEN: Code für Behandlungsroutine der Benachrichtigung hinzufügen

}

void CExchangeDataAuto::OnTextDataChanged() 
{
	// ZU ERLEDIGEN: Code für Behandlungsroutine der Benachrichtigung hinzufügen

}

BOOL CExchangeDataAuto::DisplayDialog() 
{
	BOOL ReturnValue = TRUE;
	AfxLockTempMaps();			// MFC Note #3
	CWnd* pTopWnd = CWnd::FromHandle(::GetTopWindow(NULL));
	
	try
	{
		CPromptDlg dlg;
		if(m_TextData.vt == VT_BSTR)
			dlg.m_String = m_TextData.bstrVal;
		
		dlg.m_Figure = m_LongData;

		if(dlg.DoModal() == IDOK)
		{
			m_TextData = COleVariant(dlg.m_String).Detach();
			m_LongData = dlg.m_Figure;
			ReturnValue = TRUE;
		}
		else
		{
			ReturnValue = FALSE;
		}
	}

	catch(CException* Exception)
	{
		ReturnValue = FALSE;
		Exception->Delete();
	}

	AfxUnlockTempMaps();
	return ReturnValue;
}