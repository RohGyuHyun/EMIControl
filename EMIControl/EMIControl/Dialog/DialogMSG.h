#pragma once
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"


// CDialogMSG ��ȭ �����Դϴ�.

class CDialogMSG : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogMSG)

public:
	CDialogMSG(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogMSG();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MSG };
public:
	void SetCaption(CString msg);
	void SetDefectAlarmCode(int alarm_code);

protected:
	CString m_sMsg;
	int m_nDefectAlarmCode;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
protected:
	CBtnenhctrl m_btn_msg;
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlMsgOk();
	void ClickBtnenhctrlMsgCancel();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
