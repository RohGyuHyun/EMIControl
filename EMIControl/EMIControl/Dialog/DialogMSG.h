#pragma once
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"


// CDialogMSG 대화 상자입니다.

class CDialogMSG : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogMSG)

public:
	CDialogMSG(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogMSG();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MSG };
public:
	void SetCaption(CString msg);
	void SetDefectAlarmCode(int alarm_code);

protected:
	CString m_sMsg;
	int m_nDefectAlarmCode;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
