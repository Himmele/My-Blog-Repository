// DataToClipboardView.h : Schnittstelle der Klasse CDataToClipboardView
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATATOCLIPBOARDVIEW_H__2C026A9D_D4F1_4DB6_BEE7_FBA8B40B3E0E__INCLUDED_)
#define AFX_DATATOCLIPBOARDVIEW_H__2C026A9D_D4F1_4DB6_BEE7_FBA8B40B3E0E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDataToClipboardView : public CView
{
	CRectTracker m_Tracker;
	CRect m_rectTracker;
	CSize m_sizeTotal;
protected: // Nur aus Serialisierung erzeugen
	CDataToClipboardView();
	DECLARE_DYNCREATE(CDataToClipboardView)

// Attribute
public:
	CDataToClipboardDoc* GetDocument();

// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CDataToClipboardView)
	public:
	virtual void OnDraw(CDC* pDC);  // überladen zum Zeichnen dieser Ansicht
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnInitialUpdate();
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementierung
public:
	void MoveTrackRect(CPoint point);
	CSize m_DragOffset;
	COleDropTarget m_DropTarget;
	CRect m_rectTrackerEnter;
	virtual ~CDataToClipboardView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CDataToClipboardView)
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAs();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnFileOpen();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL DoPasteDib(COleDataObject* pDataObject);
	COleDataSource* SaveDib();
};

#ifndef _DEBUG  // Testversion in DataToClipboardView.cpp
inline CDataToClipboardDoc* CDataToClipboardView::GetDocument()
   { return (CDataToClipboardDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_DATATOCLIPBOARDVIEW_H__2C026A9D_D4F1_4DB6_BEE7_FBA8B40B3E0E__INCLUDED_)
