#pragma once
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"


// CDialogMainInfo 대화 상자입니다.

class CDialogMainInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogMainInfo)

public:
	CDialogMainInfo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogMainInfo();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MAIN_INFO };
public:
	void SetUpdate();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
protected:
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlInfoPrcsOrigin();
	CBtnenhctrl m_main_info_origin;
	CBtnenhctrl m_main_info_load;
	CBtnenhctrl m_main_info_align;
	CBtnenhctrl m_btn_main_info_scan8k;
	CBtnenhctrl m_btn_main_info_scan16k;
	CBtnenhctrl m_btn_main_info_film;
	CBtnenhctrl m_btn_main_info_shadow;
	CBtnenhctrl m_btn_main_info_judge;
	CBtnenhctrl m_btn_main_info_review;
	CBtnenhctrl m_btn_main_info_unload;
	CBtnenhctrl m_btn_main_info_lot;
	CBtnenhctrl m_btn_main_info_glass;
	CBtnenhctrl m_btn_main_info_recipe;
	CBtnenhctrl m_btn_main_info_operator;
	CBtnenhctrl m_btn_main_info_time;
	CBtnenhctrl m_btn_main_info_cassete;
	CBtnenhctrl m_btn_main_info_slot;
	CBtnenhctrl m_btn_main_info_recipe_no;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CBtnenhctrl m_btn_loader_use;
public:
	CBtnenhctrl b_btn_ips_online;
	CBtnenhctrl m_btn_8k_use;
	CBtnenhctrl m_btn_8k90_use;
	CBtnenhctrl m_btn_16k_use;
	CBtnenhctrl m_btn_16k_pcl_s_use;
	CBtnenhctrl m_btn_16k_pcl_l_use;
	CBtnenhctrl m_btn_filmalign_use;
	CBtnenhctrl m_btn_shadow_use;
	CBtnenhctrl m_btn_cycle_use;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CBtnenhctrl m_btn_DefServer;
	CBtnenhctrl m_btn_ips_autorun_use;
};

