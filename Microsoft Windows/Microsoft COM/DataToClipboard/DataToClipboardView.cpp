// DataToClipboardView.cpp : Implementierung der Klasse CDataToClipboardView
//

#include "stdafx.h"
#include "DataToClipboard.h"
#include "DataToClipboardDoc.h"
#include "DataToClipboardView.h"
#include "CDib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataToClipboardView

IMPLEMENT_DYNCREATE(CDataToClipboardView, CView)

BEGIN_MESSAGE_MAP(CDataToClipboardView, CView)
	//{{AFX_MSG_MAP(CDataToClipboardView)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	// Standard-Druckbefehle
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataToClipboardView Konstruktion/Destruktion

CDataToClipboardView::CDataToClipboardView() : m_sizeTotal(800, 1050), m_rectTracker(50, 50, 250, 250), m_DragOffset(0, 0), m_rectTrackerEnter(50, 50, 250, 250)
{
}

CDataToClipboardView::~CDataToClipboardView()
{
}

BOOL CDataToClipboardView::PreCreateWindow(CREATESTRUCT& cs)
{
	// ZU ERLEDIGEN: Ändern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDataToClipboardView Zeichnen

void CDataToClipboardView::OnDraw(CDC* pDC)
{
	CDib& Dib = GetDocument()->m_Dib;
	m_Tracker.m_rect = m_rectTracker;
	pDC->LPtoDP(m_Tracker.m_rect);
	m_Tracker.Draw(pDC);
	Dib.Draw(pDC, m_rectTracker.TopLeft(), m_rectTracker.Size());
}

/////////////////////////////////////////////////////////////////////////////
// CDataToClipboardView Drucken

BOOL CDataToClipboardView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->SetMaxPage(1);
	// Standardvorbereitung
	return DoPreparePrinting(pInfo);
}

void CDataToClipboardView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// ZU ERLEDIGEN: Zusätzliche Initialisierung vor dem Drucken hier einfügen
}

void CDataToClipboardView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// ZU ERLEDIGEN: Hier Bereinigungsarbeiten nach dem Drucken einfügen
}

/////////////////////////////////////////////////////////////////////////////
// CDataToClipboardView Diagnose

#ifdef _DEBUG
void CDataToClipboardView::AssertValid() const
{
	CView::AssertValid();
}

void CDataToClipboardView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDataToClipboardDoc* CDataToClipboardView::GetDocument() // Die endgültige (nicht zur Fehlersuche kompilierte) Version ist Inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDataToClipboardDoc)));
	return (CDataToClipboardDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDataToClipboardView Hilfsfunktionen

BOOL CDataToClipboardView::DoPasteDib(COleDataObject* pDataObject)
{
	if(!pDataObject->IsDataAvailable(CF_DIB))
	{
		TRACE("CF_DIB-Format nicht verfügbar!");
		return FALSE;
	}

	CDataToClipboardDoc* pDoc = GetDocument();
	HGLOBAL hDib = pDataObject->GetGlobalData(CF_DIB);
	LPVOID lpDib = ::GlobalLock(hDib);
	pDoc->m_Dib.AttachMemory(lpDib, TRUE, hDib);
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL);
	return TRUE;
}

COleDataSource* CDataToClipboardView::SaveDib()
{
	CDib& Dib = GetDocument()->m_Dib;
	if(Dib.GetSizeImage() > 0)
	{
		COleDataSource* pSource = new COleDataSource();
		int nHeaderSize = Dib.GetSizeHeader();
		int nImageSize = Dib.GetSizeImage();
		HGLOBAL hHeader = ::GlobalAlloc(GMEM_SHARE, nHeaderSize + nImageSize);
		LPVOID pHeader = ::GlobalLock(hHeader);
		LPVOID pImage = (LPBYTE) pHeader + nHeaderSize;
		memcpy(pHeader, Dib.m_lpBMIH, nHeaderSize);
		memcpy(pImage, Dib.m_lpImage, nImageSize);
		::GlobalUnlock(hHeader);
		pSource->CacheGlobalData(CF_DIB, hHeader);
		return pSource;
	}

	return 0;
}



void CDataToClipboardView::OnEditCopy() 
{
	COleDataSource* pSource = SaveDib();
	if(pSource)
		pSource->SetClipboard();
}

void CDataToClipboardView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	CDib& Dib = GetDocument()->m_Dib;
	pCmdUI->Enable(Dib.GetSizeImage() > 0L);
}

void CDataToClipboardView::OnFileSaveAs() 
{
	CDib& Dib = GetDocument()->m_Dib;
	CFileDialog dlg(FALSE, "bmp", "*.bmp");
	if(dlg.DoModal() != IDOK)
		return;

	BeginWaitCursor();
	Dib.CopyToMapFile(dlg.GetPathName());
	EndWaitCursor();
}

void CDataToClipboardView::OnEditCut() 
{
}

void CDataToClipboardView::OnEditPaste() 
{
	CDataToClipboardDoc* pDoc = GetDocument();
	COleDataObject DataObject;
	DataObject.AttachClipboard();
	DoPasteDib(&DataObject);
	CClientDC dc(this);
	pDoc->m_Dib.UsePalette(&dc);
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL);
}


void CDataToClipboardView::OnFileOpen() 
{
	CDataToClipboardDoc* pDoc = GetDocument();
	CFileDialog dlg(TRUE, "bmp", "*.bmp");
	if(dlg.DoModal() != IDOK)
		return;
	if(pDoc->m_Dib.AttachMapFile(dlg.GetPathName(), TRUE))
	{
		CClientDC dc(this);
		pDoc->m_Dib.SetSystemPalette(&dc);
		pDoc->m_Dib.UsePalette(&dc);
		pDoc->SetModifiedFlag();
		pDoc->UpdateAllViews(NULL);
	}
}

void CDataToClipboardView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	if(pDC->IsPrinting())
	{
		int nHSize = pDC->GetDeviceCaps(HORZSIZE) * 1000 / 254;
		int nVSize = pDC->GetDeviceCaps(VERTSIZE) * 1000 / 254;
		pDC->SetMapMode(MM_ANISOTROPIC);
		pDC->SetWindowExt(nHSize, nVSize);
		pDC->SetViewportExt(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
	}
	else
		CView::OnPrepareDC(pDC, pInfo);
}

void CDataToClipboardView::OnInitialUpdate() 
{
	m_DropTarget.Register(this);

	CView::OnInitialUpdate();	
}

void CDataToClipboardView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDataToClipboardDoc* pDoc = GetDocument();
	if(m_Tracker.HitTest(point) == CRectTracker::hitMiddle)
	{
		COleDataSource* pSource = SaveDib();
		if(pSource)
		{
			CClientDC dc(this);
			OnPrepareDC(&dc);
			CPoint TopLeft = m_rectTracker.TopLeft();
			dc.LPtoDP(&TopLeft);
			m_DragOffset = point - TopLeft;
			pDoc->m_bDragHere = TRUE;
			DROPEFFECT DropEffect = pSource->DoDragDrop(DROPEFFECT_MOVE | DROPEFFECT_COPY, CRect(0, 0, 0, 0));
		}
		pDoc->m_bDragHere = FALSE;
		delete pSource;
	}
	else
	{
		if(m_Tracker.Track(this, point, FALSE, NULL))
		{
			CClientDC dc(this);
			OnPrepareDC(&dc);
			m_rectTracker = m_Tracker.m_rect;
			dc.LPtoDP(m_rectTracker);
		}
	}

	Invalidate();
}

BOOL CDataToClipboardView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(m_Tracker.SetCursor(pWnd, nHitTest))
	{
		return TRUE;
	}
	else
	{
		return CView::OnSetCursor(pWnd, nHitTest, message);
	}
}

void CDataToClipboardView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	
	AfxGetApp()->m_pMainWnd->SendMessage(WM_PALETTECHANGED, (UINT)GetSafeHwnd());	
}

DROPEFFECT CDataToClipboardView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	m_rectTrackerEnter = m_rectTracker;
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DrawFocusRect(m_rectTracker);
	
	return CView::OnDragEnter(pDataObject, dwKeyState, point);
}

DROPEFFECT CDataToClipboardView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	if(!pDataObject->IsDataAvailable(CF_DIB))
		return DROPEFFECT_NONE;
	
	MoveTrackRect(point);
	
	if((dwKeyState & MK_CONTROL) == MK_CONTROL)
		return DROPEFFECT_COPY;
	
	if((dwKeyState & MK_ALT) == MK_ALT)
		return DROPEFFECT_MOVE;

	return DROPEFFECT_MOVE;
}

void CDataToClipboardView::OnDragLeave() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DrawFocusRect(m_rectTracker);
	m_rectTracker = m_rectTrackerEnter;
}

BOOL CDataToClipboardView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	BOOL bReturn;
	CDataToClipboardDoc* pDoc = GetDocument();
	MoveTrackRect(point);
	if(pDoc->m_bDragHere)
	{
		pDoc->m_bDragHere = FALSE;
		bReturn = TRUE;
	}
	else
	{
		bReturn = DoPasteDib(pDataObject);
	}

	return bReturn;
}

void CDataToClipboardView::MoveTrackRect(CPoint point)
{
	CClientDC dc(this);
	OnPrepareDC (&dc);
	dc.DrawFocusRect(m_rectTracker);
	dc.LPtoDP(m_rectTracker);
	CSize SizeTrack = m_rectTracker.Size();
	CPoint newTopLeft = point - m_DragOffset;
	m_rectTracker = CRect(newTopLeft, SizeTrack);
	m_Tracker.m_rect = m_rectTracker;
	dc.DPtoLP(m_rectTracker);
	dc.DrawFocusRect(m_rectTracker);
}
