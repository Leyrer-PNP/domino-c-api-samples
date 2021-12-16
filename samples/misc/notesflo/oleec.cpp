//
//		oleec.cpp
//
//	Implementation of CExternalConnection
//

	// Windows header files
#include <windows.h>
#include <ole2.h>
#include <ole2ver.h>

	// Domino and Notes header files
#include <global.h>
#include <olenotes.h>

	// Application header files
#include "notesflo.h"
#include "oleobj.hpp"
#include "oleps.hpp"
#include "oledo.hpp"
#include "oleipo.hpp"
#include "oleipao.hpp"
#include "oleec.hpp"
#include "nf_obj.hpp"

//
//		IUnknown methods
//

	// QueryInterface
STDMETHODIMP CExternalConnection::QueryInterface (
	REFIID			riid,
	LPVOID FAR *	ppvObj
) {
		// Let the containing CImageObject handle this
	return (m_lpObj->QueryInterface (riid, ppvObj));
}

	// AddRef
STDMETHODIMP_ (ULONG) CExternalConnection::AddRef (
	void
) {
		// Increment the local counter
	++m_nRefCount;

		// A reference to *this is a reference to the containing CImageObject!
	return (m_lpObj->AddRef ());
}

	// Release
STDMETHODIMP_ (ULONG) CExternalConnection::Release (
	void
) {
	--m_nRefCount;

		// A reference to *this is a reference to the containing CImageObject!
	return (m_lpObj->Release ());
}

//
//		IExternalConnection methods
//

	// Add an external connection
STDMETHODIMP_(DWORD) CExternalConnection::AddConnection (
	DWORD	ExtConn,
	DWORD	Reserved
) {
	if (EXTCONN_STRONG & ExtConn)
		return (++m_dwStongCount);
	else
		return (0);
}

	// Release an external connection
STDMETHODIMP_(DWORD) CExternalConnection::ReleaseConnection (
	DWORD	ExtConn,
	DWORD	Reserved,
	BOOL	fLastReleaseCloses
) {
	if (EXTCONN_STRONG & ExtConn)
	{
		DWORD	dwCurCount = --m_dwStongCount;

			// On the last release, close the COleObject instance
		if ((0 == m_dwStongCount) && fLastReleaseCloses)
			(m_lpObj->GetOleObjectPtr ())->Close (OLECLOSE_SAVEIFDIRTY);

		return (dwCurCount);
	}
	else
		return (0);
}
