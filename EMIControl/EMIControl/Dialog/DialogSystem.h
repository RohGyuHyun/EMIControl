#pragma once
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"


// CDialogSystem 대화 상자입니다.

class CDialogSystem : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSystem)

public:
	CDialogSystem(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogSystem();

	void ManuSelect(int nManuType);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SYSTEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlSystemMotorCtrl();
	void ClickBtnenhctrlSystemAfCtrl();
//	void AnimDLLDoneBtnenhctrlSystemLightCtrl(long iPicture, BOOL bResult);
	void ClickBtnenhctrlSystemLightCtrl();
	void ClickBtnenhctrlSystemIpsCtrl();
	virtual BOOL OnInitDialog();
//	void AnimDLLDoneBtnenhctrlSystemIoCtrl(long iPicture, BOOL bResult);
	void ClickBtnenhctrlSystemIoCtrl();
	void ClickBtnenhctrlSystemManualCtrl();
	void ClickBtnenhctrlSystemTriggerCtrl();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CBtnenhctrl m_btn_glass_ctrl;
	CBtnenhctrl m_btn_io_ctrl;
	CBtnenhctrl m_btn_motor_ctrl;
	CBtnenhctrl m_btn_light_ctrl;
	CBtnenhctrl m_btn_ips_ctrl;
	CBtnenhctrl m_btn_af_ctrl;
	CBtnenhctrl m_btn_trigger_ctrl;
};
