#pragma once
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"


// CDialogLogAlarm ��ȭ �����Դϴ�.

class CDialogLogAlarm : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogLogAlarm)

public:
	CDialogLogAlarm(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogLogAlarm();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_LOG_ALARM };
public:
	void SetMessage(char *msg);
protected:
	CString m_sMessage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CBtnenhctrl m_btn_msg;
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlLogBuzOff();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
