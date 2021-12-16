//
//		olecf.hpp
//
//	Declaration of:	CClassFactory
//	Inherits from:	IClassFactory
//

#ifndef _OLECF_HPP_
#define _OLECF_HPP_		1

class CImageObject;
class CImageApplication;

class CClassFactory : public IClassFactory
{
	private:
		int			m_nRefCount;		// Reference count
		CImageApplication FAR *	m_lpApp;

	public:
		CClassFactory (CImageApplication FAR *lpApp);
		~CClassFactory (void);

			// IUnknown methods
		STDMETHODIMP QueryInterface (REFIID riid, LPVOID FAR *ppvObj);
		STDMETHODIMP_ (ULONG) AddRef (void);
		STDMETHODIMP_ (ULONG) Release (void);

			// IClassFactory methods
		STDMETHODIMP CreateInstance (LPUNKNOWN pUnkOuter,
			REFIID riid, LPVOID FAR *ppvObject);
		STDMETHODIMP LockServer (BOOL fLock);
};

#endif
