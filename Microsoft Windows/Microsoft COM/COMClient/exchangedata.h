// Von Klassen-Assistent automatisch erstellte IDispatch-Kapselungsklasse(n). 
/////////////////////////////////////////////////////////////////////////////
// Wrapper-Klasse IExchangeDataAuto 

class IExchangeDataAuto : public COleDispatchDriver
{
public:
	IExchangeDataAuto() {}		// Ruft den Standardkonstruktor für COleDispatchDriver auf
	IExchangeDataAuto(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IExchangeDataAuto(const IExchangeDataAuto& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attribute
public:
	long GetLongData();
	void SetLongData(long);
	CString GetTextData();
	void SetTextData(LPCTSTR);

// Operationen
public:
	BOOL DisplayDialog();
};
