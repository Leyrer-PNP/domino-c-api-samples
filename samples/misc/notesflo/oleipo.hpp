//
//		oleipo.hpp
//
//	Declaration of:	COleInPlaceObject
//	Inherits from:	IOleInPlaceObject
//

#ifndef _OLEIPO_HPP_
#define _OLEIPO_HPP_		1

class CImageObject;

class COleInPlaceObject : public IOleInPlaceObject
{
	private:
		CImageObject FAR *m_lpObj;		// Containing object
		int			m_nRefCount;		// Reference count

	public:
		COleInPlaceObject (void)
			: m_lpObj ((CImageObject FAR *) NULL), m_nRefCount (0)
		{
			;
		}

		~COleInPlaceObject (void) { ; }

			// IUnknown methods
		STDMETHODIMP QueryInterface (REFIID riid, LPVOID FAR *ppvObj);
		STDMETHODIMP_(ULONG) AddRef (void);
		STDMETHODIMP_(ULONG) Release (void);

			// IOleWindow methods
		STDMETHODIMP GetWindow (HWND FAR *lphWnd);
		STDMETHODIMP ContextSensitiveHelp (BOOL fEnterMode);

			// IOleInPlaceObject methods
		STDMETHODIMP InPlaceDeactivate (void);
		STDMETHODIMP UIDeactivate (void);
		STDMETHODIMP SetObjectRects (LPCRECT lpPosRect, LPCRECT lpClipRect);
		STDMETHODIMP ReactivateAndUndo (void);

			// Support functions
		void SetObjectPtr (CImageObject FAR *lpObj) { m_lpObj = lpObj; }
};

#endif
