#include <stdio.h>
#include <stddef.h>
#include <ASSERT.h>
#include "Interface.h"
#include "Spaceship.h"

CSpaceshipFactory g_factory;


BOOL CSpaceshipFactory::ExternalQueryInterface(int nIid, void** ppvObj)
{
	TRACE("Entering CSpaceshipFactory::ExternalQueryInterface - ID: %d\n", nIid);

	switch(nIid)
	{
		case IID_IUnknown:
		case IID_IClassFactory:
			*ppvObj = &m_xClassFactory;
			break;
		default:
			*ppvObj = NULL;
			return FALSE;
	}

	ExternalAddRef();
	return TRUE;
}

BOOL CSpaceshipFactory::XClassFactory::QueryInterface(int nIid, void** ppvObj)
{
	TRACE("Entering CSpaceshipFactory::XClassFactory::QueryInterface - ID: %d\n", nIid);
	METHOD_PROLOGUE(CSpaceshipFactory, ClassFactory);
	return pThis->ExternalQueryInterface(nIid, ppvObj);
}

BOOL CSpaceshipFactory::XClassFactory::CreateInstance(int nIid, void** ppvObj)
{
	TRACE("Entering CSpaceshipFactory::XClassFactory::CreateInstance - ID: %d\n", nIid);

	METHOD_PROLOGUE(CSpaceshipFactory, ClassFactory);
	CSpaceship* pObj = new CSpaceship();

	if(pObj->ExternalQueryInterface(nIid, ppvObj))
	{
		pObj->ExternalRelease();
		return TRUE;
	}
	return FALSE;
}

DWORD CSpaceshipFactory::XClassFactory::Release()
{
	TRACE("Entering CSpaceshipFactory::XClassFactory::Release\n");
	METHOD_PROLOGUE(CSpaceshipFactory, ClassFactory);
	return pThis->ExternalRelease();
}

DWORD CSpaceshipFactory::XClassFactory::AddRef()
{
	TRACE("Entering CSpaceshipFactory::XClassFactory::AddRef\n");
	METHOD_PROLOGUE(CSpaceshipFactory, ClassFactory)
	return pThis->ExternalAddRef();
}


BOOL CSpaceship::ExternalQueryInterface(int nIid, void** ppvObj)
{
	TRACE("Entering CSpaceship::ExternalQueryInterface - ID: %d\n", nIid);

	switch(nIid)
	{
		case IID_IUnknown:
		case IID_IMotion:
			*ppvObj = &m_xMotion;
			break;
		case IID_IVisual:
			*ppvObj = &m_xVisual;
			break;
		default:
			*ppvObj = NULL;
			return FALSE;
	}

	ExternalAddRef();
	return TRUE;
}

BOOL CSpaceship::XMotion::QueryInterface(int nIid, void** ppvObj)
{
	TRACE("Entering CSpaceship::XMotion::QueryInterface - ID: %d\n", nIid);

	METHOD_PROLOGUE(CSpaceship, Motion)
	return pThis->ExternalQueryInterface(nIid, ppvObj);
}

DWORD CSpaceship::XMotion::Release()
{
	TRACE("Entering CSpaceship::XMotion::Release\n");

	METHOD_PROLOGUE(CSpaceship, Motion)
	return pThis->ExternalRelease();
}

DWORD CSpaceship::XMotion::AddRef()
{
	TRACE("Entering CSpaceship::XMotion::AddRef\n");
	
	METHOD_PROLOGUE(CSpaceship, Motion)
	return pThis->ExternalAddRef();
}

void CSpaceship::XMotion::Fly()
{
	TRACE("Entering CSpaceship::XMotion::Fly\n");

	METHOD_PROLOGUE(CSpaceship, Motion)
	TRACE("this: %p, pThis: %p\n", this, pThis);
	TRACE("m_nPosition: %d\n", pThis->m_nPosition);
	TRACE("m_nAcceleration: %d\n", pThis->m_nPosition);
}

int& CSpaceship::XMotion::GetPosition()
{
	TRACE("Entering CSpaceship::XMotion::GetPosition\n");
	
	METHOD_PROLOGUE(CSpaceship, Motion)
	TRACE("this: %p, pThis: %p\n", this, pThis);
	TRACE("m_nPosition: %d\n", pThis->m_nPosition);
	TRACE("m_nAcceleration: %d\n", pThis->m_nPosition);
	return pThis->m_nPosition;
}

BOOL CSpaceship::XVisual::QueryInterface(int nIid, void** ppvObj)
{
	TRACE("Entering CSpaceship::XVisual::QueryInterface - ID: %d\n", nIid);

	METHOD_PROLOGUE(CSpaceship, Visual)
	return pThis->ExternalQueryInterface(nIid, ppvObj);
}

DWORD CSpaceship::XVisual::Release()
{
	TRACE("Entering CSpaceship::XVisual::Release\n");

	METHOD_PROLOGUE(CSpaceship, Visual)
	return pThis->ExternalRelease();
}

DWORD CSpaceship::XVisual::AddRef()
{
	TRACE("Entering CSpaceship::XVisual::AddRef\n");
	
	METHOD_PROLOGUE(CSpaceship, Visual)
	return pThis->ExternalAddRef();
}

void CSpaceship::XVisual::Display()
{
	TRACE("Entering CSpaceship::XVisual::Display\n");

	METHOD_PROLOGUE(CSpaceship, Visual);
	TRACE("this: %p, pThis: %p\n", this, pThis);
	TRACE("m_nPosition: %d\n", pThis->m_nPosition);
	TRACE("m_nColor: %d\n", pThis->m_nColor);
}

BOOL GetClassObject(int nClsid, int nIid, void** ppvObj)
{
	ASSERT(nClsid == CLSID_CSpaceship);
	ASSERT((nIid == IID_IUnknown) || (nIid == IID_IClassFactory));
	return g_factory.ExternalQueryInterface(nIid, ppvObj);
}