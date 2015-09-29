// Von Klassen-Assistent automatisch erstellte IDispatch-Kapselungsklasse(n). 
/////////////////////////////////////////////////////////////////////////////
// Wrapper-Klasse IEx24c 

class IEx24c : public COleDispatchDriver
{
public:
	IEx24c() {}		// Ruft den Standardkonstruktor für COleDispatchDriver auf
	IEx24c(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IEx24c(const IEx24c& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attribute
public:
	DATE GetTime();
	void SetTime(DATE);

// Operationen
public:
	VARIANT GetFigure(short n);
	void SetFigure(short n, const VARIANT& newValue);
	void ShowWin();
	LPDISPATCH CreateAlarm(DATE Time);
	void RefreshWin();
};
/////////////////////////////////////////////////////////////////////////////
// Wrapper-Klasse IAlarm 

class IAlarm : public COleDispatchDriver
{
public:
	IAlarm() {}		// Ruft den Standardkonstruktor für COleDispatchDriver auf
	IAlarm(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAlarm(const IAlarm& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attribute
public:
	DATE GetTime();
	void SetTime(DATE);

// Operationen
public:
};
