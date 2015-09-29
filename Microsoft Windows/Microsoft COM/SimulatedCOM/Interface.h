#define BOOL int
#define DWORD unsigned int
#define TRUE 1
#define FALSE 0
#define TRACE printf
#define ASSERT assert

#define CLSID_CSpaceship 10
#define IID_IUnknown 0
#define IID_IClassFactory 1
#define IID_IMotion 2
#define IID_IVisual 3

#define METHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = ((theClass*)((char*)(this) - \
	offsetof(theClass, m_x##localClass))); \


BOOL GetClassObject(int nClsid, int nIid, void** ppvObj);


// Interfaces

struct IUnknown
{
	IUnknown() { TRACE("Entering IUnknown - this %p\n", this); }
	virtual BOOL QueryInterface(int nIid, void** ppvObj) = 0;
	virtual DWORD Release() = 0;
	virtual DWORD AddRef() = 0;
};

struct IClassFactory : public IUnknown
{
	IClassFactory() { TRACE("Entering IClassFactory - this: %p\n", this); }
	virtual BOOL CreateInstance(int nIid, void** ppvObj) = 0;
};

struct IMotion : public IUnknown
{
	IMotion() { TRACE("Entering IMotion - this: %p\n", this); }
	virtual void Fly() = 0;
	virtual int& GetPosition() = 0;
};

struct IVisual : public IUnknown
{
	IVisual() { TRACE("Entering IVisual - this: %p\n", this); }
	virtual void Display() = 0;
};


class CSimulatedCmdTarget
{
	public:
		DWORD m_dwRef;

	protected:
		CSimulatedCmdTarget()
		{ 
			TRACE("Constructing Object of Type CSimulatedCmdTarget - this: %p\n", this);
			m_dwRef = 1;
		}

		virtual ~CSimulatedCmdTarget()
		{
			TRACE("Destructing Object of Type CSimulatedCmdTarget - this: %p\n", this);
		}

	DWORD ExternalRelease()
	{
		TRACE("Entering CSimulatedCmdTarget::ExternalRelease - RefCount: %ld\n", m_dwRef);

		m_dwRef--;
		
		if(m_dwRef == 0L)
		{
			TRACE("Deleting CSimulatedCmdTarget - this: %p\n", this);
			delete this;
			return 0;
		}

		return m_dwRef;
	}

	DWORD ExternalAddRef()
	{
		return ++m_dwRef;
	}
};