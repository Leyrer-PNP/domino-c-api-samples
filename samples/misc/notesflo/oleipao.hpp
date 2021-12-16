//
//		oleipao.hpp
//
//	Declaration of:	COleInPlaceActiveObject
//	Inherits from:	IOleInPlaceActiveObject
//

#ifndef _OLEIPAO_HPP_
#define _OLEIPAO_HPP_		1

class CImageObject;

class COleInPlaceActiveObject : public IOleInPlaceActiveObject
{
	private:
		CImageObject FAR *m_lpObj;		// Containing object
		int			m_nRefCount;		// Reference count

	public:
		COleInPlaceActiveObject (void)
			: m_lpObj ((CImageObject FAR *) NULL), m_nRefCount (0)
		{
			;
		}

		~COleInPlaceActiveObject (void) { ; }

			// IUnknown methods
		STDMETHODIMP QueryInterface (REFIID riid, LPVOID FAR *ppvObj);
		STDMETHODIMP_(ULONG) AddRef (void);
		STDMETHODIMP_(ULONG) Release (void);

			// IOleWindow methods
		STDMETHODIMP GetWindow (HWND FAR *lphWnd);
		STDMETHODIMP ContextSensitiveHelp (BOOL fEnterMode);

			// IOleInPlaceActiveObject methods
		STDMETHODIMP TranslateAccelerator (LPMSG lpMsg);
		STDMETHODIMP OnFrameWindowActivate (BOOL fActivate);
		STDMETHODIMP OnDocWindowActivate (BOOL fActivate);
		STDMETHODIMP ResizeBorder (LPCRECT prcBorder,
			LPOLEINPLACEUIWINDOW pUIWindow, BOOL fFrameWindow);
		STDMETHODIMP EnableModeless (BOOL fEnable);

			// Support functions
		void SetObjectPtr (CImageObject FAR *lpObj) { m_lpObj = lpObj; }
};

#endif
