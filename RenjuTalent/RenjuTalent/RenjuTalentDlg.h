
// RenjuTalentDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CRenjuTalentDlg dialog
class CRenjuTalentDlg : public CDialogEx
{
// Construction
public:
	CRenjuTalentDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RENJUTALENT_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	CEdit m_DebugText;
	void outputDebugText(const char * text);
	afx_msg void OnEnChangeEdit1();
};
