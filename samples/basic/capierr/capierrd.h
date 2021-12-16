// CapiErrD.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCapiErrD dialog

#define LINEOTEXT   256

class CCapiErrD : public CDialog
{
// Construction
public:
	CCapiErrD(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCapiErrD)
	enum { IDD = IDD_CapiErr_DIALOG };
	CString	m_errCode;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCapiErrD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCapiErrD)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnGetErrorClick();
	afx_msg void OnDecClick();
	afx_msg void OnHexClick();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
