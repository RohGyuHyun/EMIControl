#pragma once
#include "afxcmn.h"
#include "btnenhctrl.h"


// CDialogLogWindow 대화 상자입니다.

class CDialogLogWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogLogWindow)

public:
	CDialogLogWindow(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogLogWindow();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LOG_WINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
