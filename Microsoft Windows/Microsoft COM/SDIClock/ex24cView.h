// ex24cView.h : interface of the CEx24cView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EX24CVIEW_H__A9515B0E_5B85_11D0_848F_00400526305B__INCLUDED_)
#define AFX_EX24CVIEW_H__A9515B0E_5B85_11D0_848F_00400526305B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEx24cView : public CView
{
private:
   static const int nX[60];
   static const int nY[60];
   CFont m_font;
protected: // create from serialization only
   CEx24cView();
   DECLARE_DYNCREATE(CEx24cView)

// Attributes
public:
   CEx24cDoc* GetDocument();

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CEx24cView)
   public:
   virtual void OnDraw(CDC* pDC);  // overridden to draw this view
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CEx24cView();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
   //{{AFX_MSG(CEx24cView)
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ex24cView.cpp
inline CEx24cDoc* CEx24cView::GetDocument()
   { return (CEx24cDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EX24CVIEW_H__A9515B0E_5B85_11D0_848F_00400526305B__INCLUDED_)
