#include <iostream.h>
#include <stddef.h>


// Makros und konstante Variablen

#define METHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = ((theClass*)((char*)(this) - \
	offsetof(theClass, m_x##localClass)));

const int CLSID_CSpaceship = 1;
const int IID_IUnknown = 2;
const int IID_IMotion = 3;
const int IID_IVisual = 4;


// Schnittstellen - Interfaces

struct IUnknown
{
	virtual bool QueryInterface(int nIid, void** ppvObj) = 0;
};

struct IMotion : public IUnknown
{
	virtual void Fly() = 0;
	virtual int& GetPosition() = 0;
};

struct IVisual : public IUnknown
{
	virtual void Display() = 0;
};


// Verschachtelte Klassendefinition

class CSpaceship
{
	protected:
		int m_nPosition;
		int m_nAcceleration;
		int m_nColor;
	public:
		CSpaceship()
		{
			m_nPosition = m_nAcceleration = m_nColor = 0;
		}

	class XMotion : public IMotion
	{
		public:
			XMotion() {}
			virtual void Fly();
			virtual int& GetPosition();
			virtual bool QueryInterface(int nIid, void** ppvObj);
	} m_xMotion;

	class XVisual : public IVisual
	{
		public:
			XVisual() {}
			virtual void Display();
			virtual bool QueryInterface(int nIid, void** ppvObj)
			{ return true; }
	} m_xVisual;

	friend class XVisual;	// notwendig für Zugriff auf protected-Datenelemente
	friend class XMotion;
};


// Definitionen der virtuellen Elementfunktionen

bool CSpaceship::XMotion::QueryInterface(int nIid, void** ppvObj)
{
	METHOD_PROLOGUE(CSpaceship, Motion)

	switch(nIid)
	{
		case IID_IUnknown:
		case IID_IMotion:
			*ppvObj = &pThis->m_xMotion;
			break;
		case IID_IVisual:
			*ppvObj = &pThis->m_xVisual;
			break;
		default:
			*ppvObj = NULL;
			return false;
	}

	return true;
}

int& CSpaceship::XMotion::GetPosition()
{
	METHOD_PROLOGUE(CSpaceship, Motion)
	return pThis->m_nPosition;
}

void CSpaceship::XMotion::Fly()
{
	cout << "Fly\n";
}

void CSpaceship::XVisual::Display()
{
	cout << "Display\n";
}


// Globaler "Konstruktor"

bool GetClassObject(int nClsid, int nIid, void** ppvObj)
{
	//ASSERT(nClsid == CLSID_CSpaceship);
	CSpaceship* pObj = new CSpaceship();
	IUnknown* pUnk = &pObj->m_xMotion;
	return pUnk->QueryInterface(nIid, ppvObj);
}


void main()
{
	IMotion* pMot;
	IVisual* pVis;

	GetClassObject(CLSID_CSpaceship, IID_IMotion, (void**) &pMot);

	pMot->Fly();
	pMot->QueryInterface(IID_IVisual, (void**) &pVis);
	pVis->Display();

	// Da pMot ein Zeiger ist, führt eine Konvertierung in (void*) zu
	// einem "pass-by value".
	// Bildet man jedoch einen Zeiger auf pMot führt dies zu einem
	// "pass-by-reference".
}