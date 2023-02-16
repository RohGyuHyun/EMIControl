#pragma once
#include "afxcmn.h"

typedef struct ProcessBar
{
	int m_nProcessType;
	CString m_strProcess;
	int m_nProcessPer;
	CString m_strProcessTime;
}typeProcessBar;

// CProcessBar 대화 상자입니다.
class CProcessBar : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessBar)

public:
	CProcessBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CProcessBar();

	void SetProcess(typeProcessBar process_bar);
	void SetProcessPer(int percent);
	BOOL m_bProcessComp;

protected:
	typeProcessBar m_ProcessBar;
	int m_iStep;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PROCESS_BAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
