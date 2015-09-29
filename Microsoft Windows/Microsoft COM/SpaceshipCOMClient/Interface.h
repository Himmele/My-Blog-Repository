struct IMotion : public IUnknown
{
	STDMETHOD_(void, Fly) () = 0;
	STDMETHOD_(int&, GetPosition) () = 0;
};

struct IVisual : public IUnknown
{
	STDMETHOD_(void, Display) () = 0;
};