// ElectronicCash.cpp : Legt das Klassenverhalten für die Anwendung fest.
//

#include "stdafx.h"
#include "ElectronicCash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElectronicCashApp

BEGIN_MESSAGE_MAP(CElectronicCashApp, CWinApp)
	//{{AFX_MSG_MAP(CElectronicCashApp)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElectronicCashApp Konstruktion

CElectronicCashApp::CElectronicCashApp()
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance platzieren
}

/////////////////////////////////////////////////////////////////////////////
// Das einzige CElectronicCashApp-Objekt

CElectronicCashApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CElectronicCashApp Initialisierung

BOOL CElectronicCashApp::InitInstance()
{
	AfxOleInit();
	if(RunEmbedded() || RunAutomated())
	{
		COleTemplateServer::RegisterAll();
		return TRUE;
	}

	COleObjectFactory::UpdateRegistryAll();
	AfxMessageBox("ElectronicCash successfully registred!");


	// Standardinitialisierung
	// Wenn Sie diese Funktionen nicht nutzen und die Größe Ihrer fertigen 
	//  ausführbaren Datei reduzieren wollen, sollten Sie die nachfolgenden
	//  spezifischen Initialisierungsroutinen, die Sie nicht benötigen, entfernen.

	// Da das Dialogfeld geschlossen wurde, FALSE zurückliefern, so dass wir die
	//  Anwendung verlassen, anstatt das Nachrichtensystem der Anwendung zu starten.
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
// CElectronicCash

IMPLEMENT_DYNCREATE(CElectronicCash, CCmdTarget)

CElectronicCash::CElectronicCash()
{
	EnableAutomation();
	
	// Um die Ausführung der Anwendung fortzusetzen, solange ein OLE-Automatisierungs
	//	objekt aktiv ist, ruft der Konstruktor AfxOleLockApp auf.
	
	m_dBalance = 0.0;

	AfxOleLockApp();
	
	TRACE("Constructing Object of Type ElectronicCash\n");
}

CElectronicCash::~CElectronicCash()
{
	// Um die Anwendung zu beenden, nachdem alle Objekte durch OLE-Automatisierung
	// 	erstellt wurden, ruft der Destruktor AfxOleUnlockApp auf.
	
	AfxOleUnlockApp();
}


void CElectronicCash::OnFinalRelease()
{
	// Nachdem die letzte Referenz auf ein Automatisierungsobjekt freigegeben wurde,
	// wird OnFinalRelease aufgerufen. Die Basisklasse löscht das Objekt
	// automatisch. Fügen Sie zusätzlichen Bereinigungscode für Ihr Objekt
	// hinzu, bevor Sie die Basisklasse aufrufen.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CElectronicCash, CCmdTarget)
	//{{AFX_MSG_MAP(CElectronicCash)
		// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CElectronicCash, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CElectronicCash)
	DISP_PROPERTY_EX(CElectronicCash, "Balance", GetBalance, SetBalance, VT_R8)
	DISP_FUNCTION(CElectronicCash, "Withdrawal", Withdrawal, VT_R8, VTS_R8)
	DISP_FUNCTION(CElectronicCash, "Deposit", Deposit, VT_EMPTY, VTS_R8)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Hinweis: Wir stellen Unterstützung für IID_IElectronicCash zur Verfügung, um typsicheres Binden
//  von VBA zu ermöglichen. Diese IID muss mit der GUID übereinstimmen, die in der
//  Disp-Schnittstelle in der .ODL-Datei angegeben ist.

// {18775CC8-DE9F-4130-A7BE-F5ABCF2DFAF2}
static const IID IID_IElectronicCash =
{ 0x18775cc8, 0xde9f, 0x4130, { 0xa7, 0xbe, 0xf5, 0xab, 0xcf, 0x2d, 0xfa, 0xf2 } };

BEGIN_INTERFACE_MAP(CElectronicCash, CCmdTarget)
	INTERFACE_PART(CElectronicCash, IID_IElectronicCash, Dispatch)
END_INTERFACE_MAP()

// {B8F56246-BBCD-40BF-8DCB-84E60A031F4F}
IMPLEMENT_OLECREATE2(CElectronicCash, "ElectronicCash", 0xb8f56246, 0xbbcd, 0x40bf, 0x8d, 0xcb, 0x84, 0xe6, 0xa, 0x3, 0x1f, 0x4f)

// different instances or multiple handles?

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CElectronicCash 

double CElectronicCash::Withdrawal(double dAmount) 
{
	if(dAmount < 0.0)
		return 0.0;

	if(dAmount <= m_dBalance)
	{
		m_dBalance -= dAmount;
		return dAmount;
	}

	double dTemp = m_dBalance;
	m_dBalance = 0.0;
	return dTemp;
}

void CElectronicCash::Deposit(double dAmount) 
{
	if(dAmount < 0.0)
		return;

	m_dBalance += dAmount;
}

double CElectronicCash::GetBalance() 
{
	return m_dBalance;
}

void CElectronicCash::SetBalance(double newValue) 
{
	TRACE("Sorry, I can't do this!\n");
}
