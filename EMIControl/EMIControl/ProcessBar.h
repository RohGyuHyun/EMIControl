#pragma once
#include "afxcmn.h"

typedef struct ProcessBar
{
	int m_nProcessType;
	CString m_strProcess;
	int m_nProcessPer;
	CString m_strProcessTime;
}typeProcessBar;

// CProcessBar ��ȭ �����Դϴ�.
class CProcessBar : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessBar)

public:
	CProcessBar(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CProcessBar();

	void SetProcess(typeProcessBar process_bar);
	void SetProcessPer(int percent);
	BOOL m_bProcessComp;

protected:
	typeProcessBar m_ProcessBar;
	int m_iStep;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_PROCESS_BAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CProgressCtrl m_ctrlProcessBar;
	BOOL m_bProcessCancel;
	afx_msg void OnBnDropDownButtonCancel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonCancel();
};
