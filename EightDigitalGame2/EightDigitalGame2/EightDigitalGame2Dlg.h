
// EightDigitalGame2Dlg.h : header file
//

#pragma once
#include "afxwin.h"


// CEightDigitalGame2Dlg dialog
class CEightDigitalGame2Dlg : public CDialogEx
{
// Construction
public:
	CEightDigitalGame2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EIGHTDIGITALGAME2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Var1;
	afx_msg void OnBnClickedMyTest();
	CEdit m_var2;
};
