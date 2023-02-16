#pragma once
#include "afxcmn.h"
#include "btnenhctrl.h"


// CDialogLogWindow ��ȭ �����Դϴ�.

class CDialogLogWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogLogWindow)

public:
	CDialogLogWindow(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogLogWindow();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_LOG_WINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL ReadLog(int log_type);
	
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlLogMain();
	CListCtrl m_list_log;
	void ClickBtnenhctrlLogAlign();
	CBtnenhctrl m_btn_log_main;
	CBtnenhctrl m_btn_log_align;
	void ClickBtnenhctrlLogAlarm();
	CBtnenhctrl m_btn_log_alarm;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void ClickBtnenhctrlLogAlignCancle();
	void ClickBtnenhctrlLogSystem();
	CBtnenhctrl m_btn_log_system;
	void ClickBtnenhctrlInlineLog();
	void ClickBtnenhctrlLogMotor();
	void ClickBtnenhctrlDefectServerLog();
	CBtnenhctrl m_btn_motor_log;
	CBtnenhctrl m_btn_inline_log;
	CBtnenhctrl m_btn_defect_log;
};
