// Spaceship.cpp : Legt die Initialisierungsroutinen für die DLL fest.
//

#include "stdafx.h"
#include "Interface.h"
#include "Spaceship.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Beachten Sie!
//
//		Wird diese DLL dynamisch an die MFC-DLLs gebunden,
//		muss bei allen von dieser DLL exportierten Funktionen,
//		die MFC-Aufrufe durchführen, das Makro AFX_MANAGE_STATE
//		direkt am Beginn der Funktion eingefügt sein.
//
//		Beispiel:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//		// Hier normaler Funktionsrumpf
//		}
//
//		Es ist sehr wichtig, dass dieses Makro in jeder Funktion
//		vor allen MFC-Aufrufen erscheint. Dies bedeutet, dass es
//		als erste Anweisung innerhalb der Funktion ausgeführt werden
//		muss, sogar vor jeglichen Deklarationen von Objektvariablen,
//		da ihre Konstruktoren Aufrufe in die MFC-DLL generieren
//		könnten.
//
//		Siehe MFC Technical Notes 33 und 58 für weitere
//		Details.
//

/////////////////////////////////////////////////////////////////////////////
// CSpaceshipApp

BEGIN_MESSAGE_MAP(CSpaceshipApp, CWinApp)
	//{{AFX_MSG_MAP(CSpaceshipApp)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpaceshipApp Konstruktion

CSpaceshipApp::CSpaceshipApp()
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance platzieren
}

/////////////////////////////////////////////////////////////////////////////
// Das einzige CSpaceshipApp-Objekt

CSpaceshipApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSpaceshipApp Initialisierung

BOOL CSpaceshipApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Alle OLE-Server (-fabriken) als aktiv registrieren. Dies aktiviert die
	//  OLE-Bibliotheken, um Objekte von anderen Anwendungen zu erstellen.
	COleObjectFactory::RegisterAll();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Spezielle, für Inproc-Server benötigte Einsprungpunkte

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}

// Sie können regsvr.exe verwenden, indem Sie DllRegisterServer exportieren
STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	COleObjectFactory::UpdateRegistryAll();
	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
// CSpaceship

static const IID IID_IMotion = { 0x30af6bac, 0xbd29, 0x436e, { 0xaf, 0xfa, 0x95, 0xa, 0xf4, 0x83, 0x84, 0x3a } };

static const IID IID_IVisual = { 0x30af6bad, 0xbd29, 0x436e, { 0xaf, 0xfa, 0x95, 0xa, 0xf4, 0x83, 0x84, 0x3a } };

IMPLEMENT_DYNCREATE(CSpaceship, CCmdTarget)

CSpaceship::CSpaceship()
{
	TRACE("CSpaceship Constructor\n");
	m_nPosition = 100;
	m_nAcceleration = 101;
	m_nColor = 102;
	//EnableAutomation();
	
	// Um die Ausführung der Anwendung fortzusetzen, solange ein OLE-Automatisierungs-
	// objekt aktiv ist, ruft der Konstruktor AfxOleLockApp auf.
	
	AfxOleLockApp();
}

CSpaceship::~CSpaceship()
{
	TRACE("CSpaceship Destructor\n");
	// Um die Anwendung zu beenden, nachdem alle Objekte durch OLE-Automatisierung
	// erstellt wurden, ruft der Destruktor AfxOleUnlockApp auf.
	
	AfxOleUnlockApp();
}


void CSpaceship::OnFinalRelease()
{
	// Nachdem die letzte Referenz auf ein Automatisierungsobjekt freigegeben wurde,
	// wird OnFinalRelease aufgerufen. Die Basisklasse löscht das Objekt
	// automatisch. Fügen Sie zusätzlichen Bereinigungscode für Ihr Objekt
	// hinzu, bevor Sie die Basisklasse aufrufen.

	// CCmdTarget::OnFinalRelease();
	delete this;
}


BEGIN_MESSAGE_MAP(CSpaceship, CCmdTarget)
	//{{AFX_MSG_MAP(CSpaceship)
		// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//BEGIN_DISPATCH_MAP(CSpaceship, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CSpaceship)
		// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
	//}}AFX_DISPATCH_MAP
//END_DISPATCH_MAP()

// Hinweis: Wir stellen Unterstützung für IID_ISpaceship zur Verfügung, um typsicheres Binden
//  von VBA zu ermöglichen. Diese IID muss mit der GUID übereinstimmen, die in der
//  Disp-Schnittstelle in der .ODL-Datei angegeben ist.

// {30AF6BAC-BD29-436E-AFFA-950AF483843A}
static const IID IID_ISpaceship =
{ 0x30af6bac, 0xbd29, 0x436e, { 0xaf, 0xfa, 0x95, 0xa, 0xf4, 0x83, 0x84, 0x3a } };

BEGIN_INTERFACE_MAP(CSpaceship, CCmdTarget)
	//INTERFACE_PART(CSpaceship, IID_ISpaceship, Dispatch)
	INTERFACE_PART(CSpaceship, IID_IMotion, Motion)
	INTERFACE_PART(CSpaceship, IID_IVisual, Visual)
END_INTERFACE_MAP()

// {2CE112F6-7F0A-4A34-BB8D-F32AA4A7D3BA}
IMPLEMENT_OLECREATE(CSpaceship, "Spaceship.Spaceship", 0x2ce112f6, 0x7f0a, 0x4a34, 0xbb, 0x8d, 0xf3, 0x2a, 0xa4, 0xa7, 0xd3, 0xba)

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSpaceship 

STDMETHODIMP_(ULONG) CSpaceship::XMotion::AddRef()
{
	TRACE("CSpaceship::XMotion::AddRef\n");
	METHOD_PROLOGUE(CSpaceship, Motion)
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CSpaceship::XMotion::Release()
{
	TRACE("CSpaceship::XMotion::Release\n");
	METHOD_PROLOGUE(CSpaceship, Motion)
	return pThis->ExternalRelease();
}

STDMETHODIMP CSpaceship::XMotion::QueryInterface(REFIID iid, LPVOID* ppvObj)
{
	ITrace(iid, "CSpaceship::XMotion::QueryInterface");
	METHOD_PROLOGUE(CSpaceship, Motion)
	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP_(void) CSpaceship::XMotion::Fly()
{
	TRACE("CSpaceship::XMotion::Fly\n");
	METHOD_PROLOGUE(CSpaceship, Motion)
	TRACE("m_nPosition: %d\n", pThis->m_nPosition);
	TRACE("m_nAcceleration: %d\n", pThis->m_nAcceleration);
	return;
}

STDMETHODIMP_(int&) CSpaceship::XMotion::GetPosition()
{
	TRACE("CSpaceship::XMotion::GetPosition\n");
	METHOD_PROLOGUE(CSpaceship, Motion)
	TRACE("m_nPosition: %d\n", pThis->m_nPosition);
	TRACE("m_nAcceleration: %d\n", pThis->m_nAcceleration);
	return pThis->m_nPosition;
}

STDMETHODIMP_(ULONG) CSpaceship::XVisual::AddRef()
{
	TRACE("CSpaceship::XVisual::AddRef\n");
	METHOD_PROLOGUE(CSpaceship, Visual);
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CSpaceship::XVisual::Release()
{
	TRACE("CSpaceship::XVisual::Release\n");
	METHOD_PROLOGUE(CSpaceship, Visual);
	return pThis->ExternalRelease();
}

STDMETHODIMP CSpaceship::XVisual::QueryInterface(REFIID iid, LPVOID* ppvObj)
{
	ITrace(iid, "CSpaceship::XVisual::QueryInterface");
	METHOD_PROLOGUE(CSpaceship, Visual)
	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP_(void) CSpaceship::XVisual::Display()
{
	TRACE("CSpaceship::XVisual::Display\n");
	METHOD_PROLOGUE(CSpaceship, Visual)
	TRACE("m_nPosition: %d\n", pThis->m_nPosition);
	TRACE("m_nColor: %d\n", pThis->m_nColor);
}

void ITrace(REFIID iid, const char* str)
{
	OLECHAR* lpszIID;
	::StringFromIID(iid, &lpszIID);
	CString strTemp = (LPCWSTR) lpszIID;
	TRACE("%s - %s\n", (const char*) strTemp, (const char*) str);
	AfxFreeTaskMem(lpszIID);
}