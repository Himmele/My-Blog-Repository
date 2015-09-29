// stdafx.h : Include-Datei für Standard-System-Include-Dateien,
//  oder projektspezifische Include-Dateien, die häufig benutzt, aber
//      in unregelmäßigen Abständen geändert werden.
//

#if !defined(AFX_STDAFX_H__8E3C28FE_C10F_4053_8CD9_DB0F82F68B73__INCLUDED_)
#define AFX_STDAFX_H__8E3C28FE_C10F_4053_8CD9_DB0F82F68B73__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header nicht einbinden

#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen
#include <afxdisp.h>        // MFC OLE-Automatisierungsklassen
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC-Unterstützung für gängige Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxdisp.h>
#import "..\ExchangeData\Release\ExchangeData.tlb" rename_namespace("ExchangeDataComponent")
using namespace ExchangeDataComponent;


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_STDAFX_H__8E3C28FE_C10F_4053_8CD9_DB0F82F68B73__INCLUDED_)
