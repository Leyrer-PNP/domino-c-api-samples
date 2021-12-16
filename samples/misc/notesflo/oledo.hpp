//
//		oledo.hpp
//
//	Declaration of:	CDataObject
//	Inherits from:	IDataObject
//

#ifndef _OLECF_HPP_
#define _OLECF_HPP_		1

class CImageObject;

class CDataObject : public IDataObject
{
	private:
		int			m_nRefCount;		// Reference count
		CImageObject FAR *	m_lpObj;	// Pointer to containing object

	public:
		CDataObject (void)
			: m_nRefCount (0), m_lpObj ((CImageObject FAR *) NULL)
			{ ; }
		~CDataObject (void) { ; }

			// IUnknown methods
		STDMETHODIMP QueryInterface (REFIID riid, LPVOID FAR *ppvObj);
		STDMETHODIMP_ (ULONG) AddRef (void);
		STDMETHODIMP_ (ULONG) Release (void);

			// IDataObject methods
		STDMETHODIMP GetData (LPFORMATETC pFormatEtcIn, LPSTGMEDIUM pStgMedium);
		STDMETHODIMP GetDataHere (LPFORMATETC pFormatEtc, LPSTGMEDIUM pStgMedium);
		STDMETHODIMP QueryGetData (LPFORMATETC pFormatEtc);
		STDMETHODIMP GetCanonicalFormatEtc (LPFORMATETC pFormatEtc,
			LPFORMATETC pFormatEtcOut);
		STDMETHODIMP SetData (LPFORMATETC pFormatEtc, STGMEDIUM FAR *pMedium,
			BOOL fRelease);
		STDMETHODIMP EnumFormatEtc (DWORD dwDirection,
			LPENUMFORMATETC FAR *ppEnumFormatEtc);
		STDMETHODIMP DAdvise (FORMATETC FAR *pFormatEtc, DWORD advf,
			LPADVISESINK pAdvSink, DWORD FAR *pdwConnection);
		STDMETHODIMP DUnadvise (DWORD dwConnection);
		STDMETHODIMP EnumDAdvise (LPENUMSTATDATA FAR *ppEnumAdvise);

			// Support functions
		void SetObjectPtr (CImageObject FAR *lpObj) { m_lpObj = lpObj; }
};

#endif
