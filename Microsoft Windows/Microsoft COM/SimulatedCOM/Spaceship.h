class CSpaceship;

class CSpaceshipFactory : public CSimulatedCmdTarget
{
	public:
		CSpaceshipFactory()
		{
			TRACE("Constructing CSpaceshipFactory - this: %p\n", this);
		}

		~CSpaceshipFactory()
		{
			TRACE("Destructing CSpaceshipFactory - this: %p\n", this);
		}

		BOOL ExternalQueryInterface(int lRid, void** ppvObj);

		class XClassFactory : public IClassFactory
		{
			public:
				XClassFactory() { TRACE("Constructing XClassFactory - this: %p\n", this); }
				virtual BOOL QueryInterface(int lRid, void** ppvObj);
				virtual DWORD Release();
				virtual DWORD AddRef();
				virtual BOOL CreateInstance(int lRid, void** ppvObj);
		} m_xClassFactory;

		friend class XClassFactory;
};

class CSpaceship : public CSimulatedCmdTarget
{
	private:
		int m_nPosition;
		int m_nAcceleration;
		int m_nColor;

	public:
		CSpaceship()
		{
			TRACE("Constructing CSpaceship - this: %p\n", this);
			m_nPosition = 100;
			m_nAcceleration = 101;
			m_nColor = 102;
		}

		~CSpaceship()
		{
			TRACE("Destructing CSpaceship - this: %p\n", this);
		}

		BOOL ExternalQueryInterface(int lRid, void** ppvObj);

		class XMotion : public IMotion
		{
			public:
				XMotion()
				{
					TRACE("Constructing XMotion - this: %p\n", this);
				}

				virtual BOOL QueryInterface(int lRid, void** ppvObj);
				virtual DWORD Release();
				virtual DWORD AddRef();
				virtual void Fly();
				virtual int& GetPosition();
		} m_xMotion;

		class XVisual : public IVisual
		{
			public:
				XVisual()
				{
					TRACE("Constructing XVisual - this: %p\n", this);
				}

				virtual BOOL QueryInterface(int lRid, void** ppvObj);
				virtual DWORD Release();
				virtual DWORD AddRef();
				virtual void Display();
		} m_xVisual;

		friend class XVisual;
		friend class XMotion;
		friend class CSpaceshipFactory::XClassFactory;
};