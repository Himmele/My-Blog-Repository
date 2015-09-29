// Alarm.cpp : implementation file
//

#include "stdafx.h"
#include "ex24c.h"
#include "Alarm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlarm

IMPLEMENT_DYNAMIC(CAlarm, CCmdTarget)

// no default ctor
CAlarm::CAlarm(DATE time) 
{
	TRACE("Entering CAlarm ctor -- vaTime = %f\n", (double) time);
	m_time = time;
	EnableAutomation();
}

CAlarm::~CAlarm()
{
}


void CAlarm::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CAlarm, CCmdTarget)
	//{{AFX_MSG_MAP(CAlarm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAlarm, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CAlarm)
	DISP_PROPERTY(CAlarm, "Time", m_time, VT_DATE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IAlarm to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {A9515B16-5B85-11D0-848F-00400526305B}
static const IID IID_IAlarm =
{ 0xa9515b16, 0x5b85, 0x11d0, { 0x84, 0x8f, 0x0, 0x40, 0x5, 0x26, 0x30, 0x5b } };

BEGIN_INTERFACE_MAP(CAlarm, CCmdTarget)
	INTERFACE_PART(CAlarm, IID_IAlarm, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarm message handlers
