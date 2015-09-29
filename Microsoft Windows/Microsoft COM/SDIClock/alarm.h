#ifndef __ALARM_H__
#define __ALARM_H__

// Alarm.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CAlarm command target

class CAlarm : public CCmdTarget
{
	DECLARE_DYNAMIC(CAlarm)
public:
	CAlarm(DATE time);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlarm)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAlarm();

	// Generated message map functions
	//{{AFX_MSG(CAlarm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
public:
	//{{AFX_DISPATCH(CAlarm)
	DATE m_time;
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __ALARM_H__
