#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include "interface.h"

int main()
{
	TRACE("Starting COM-Client-Application\n");

	IUnknown* pUnk;
	IMotion* pMot;
	IVisual* pVis;
	IClassFactory* pClf;

	GetClassObject(CLSID_CSpaceship, IID_IClassFactory, (void**) &pClf);

	pClf->CreateInstance(IID_IUnknown, (void**) &pUnk);
	pUnk->QueryInterface(IID_IMotion, (void**) &pMot);
	pMot->QueryInterface(IID_IVisual, (void**) &pVis);
	TRACE("pUnk: %p, pMot: %p, pVis: %p\n", pUnk, pMot, pVis);

	pMot->Fly();
	int nPos = pMot->GetPosition();
	TRACE("nPos: %d\n", nPos);
	pVis->Display();

	pClf->Release();
	pUnk->Release();
	pMot->Release();
	pVis->Release();

	return 0;
}