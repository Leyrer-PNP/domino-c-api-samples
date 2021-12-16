//
//		nf_image.hpp
//
//	Definition of pattern generation classes:
//		CDrawEnvironment
//		CImagePattern
//

#ifndef _NF_IMAGE_HPP_
#define _NF_IMAGE_HPP_	1

class CImageStore;

class CDrawEnvironment
{
	private:
		HDC			m_hDc;			// Handle to device context
		RECT		m_ClientRect;	// Client area
		RECT		m_InvalidRect;	// Bounds of area to redraw
		BOOL		m_fIsMetafile;	// Are we drawing to a metafile context?
		HBRUSH		m_hImageBrush;	// Brush to draw with
		HPEN		m_hImagePen;	// Pen to draw with
		HBRUSH		m_hOldBrush;	// Original brush to restore
		HPEN		m_hOldPen;		// Original pen to restore

	public:
		CDrawEnvironment (HDC hDc, RECT FAR *lpClientRect,
			RECT FAR *lpInvalidRect, BOOL fIsMetafile);
		~CDrawEnvironment (void);

			// Get a pointer to the invalid rectangle
		RECT FAR *GetClientRectPtr (void) { return (&m_ClientRect); }
		RECT FAR *GetInvalidRectPtr (void) { return (&m_InvalidRect); }

			// Manage the painting color
			// (since Windows doesn't allow FillRect()
			// to be used when creating a metafile)
		void SetColors (COLORREF newBgColor, COLORREF newFgColor);
		void RestoreColors (void);

			// Fill in the specified rectangle
		void FillArea (RECT FAR *drawRect);

	private:
			// Prevent copying and assignment of this class
		CDrawEnvironment (const CDrawEnvironment &curEnv);
		CDrawEnvironment &operator= (const CDrawEnvironment &curEnv);
};

class CImagePattern
{
	private:
		COLORREF	m_ForegroundColor;
		COLORREF	m_BackgroundColor;

	public:
		CImagePattern (void);
		~CImagePattern (void);

		COLORREF GetForegroundColor (void) const { return (m_ForegroundColor); }
		COLORREF GetBackgroundColor (void) const { return (m_BackgroundColor); }

		void SetForegroundColor (COLORREF color) { m_ForegroundColor = color; }
		void SetBackgroundColor (COLORREF color) { m_BackgroundColor = color; }

		void NormalizeRectangle (RECT FAR *lpBounds) const;
		void DrawSierpinskiCarpet (CDrawEnvironment &drawEnv, RECT FAR *lpDrawRect);
};

#endif
