// Von Klassen-Assistent automatisch erstellte IDispatch-Kapselungsklasse(n).

#include "stdafx.h"
#include "exchangedata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// Eigenschaften IExchangeDataAuto 

long IExchangeDataAuto::GetLongData()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void IExchangeDataAuto::SetLongData(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

CString IExchangeDataAuto::GetTextData()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void IExchangeDataAuto::SetTextData(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Operationen IExchangeDataAuto 

BOOL IExchangeDataAuto::DisplayDialog()
{
	BOOL result;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}
