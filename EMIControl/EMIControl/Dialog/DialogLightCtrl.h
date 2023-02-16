#pragma once
#include "afxcmn.h"
#include "SerialPort.h"
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

#define LIGHT_PATH						"D:\\DATABASE\\SYSTEM"
//20160717 ngh
#define MAX_LIGHT						5
#define LIGHT_TIME_FILE					_T("Light_Time.ini")

// CDialogLightCtrl 대화 상자입니다.

class CDialogLightCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogLightCtrl)

public:
	CDialogLightCtrl(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogLightCtrl();
public:
	void SetAlignLight(int ch, int bright);
	void Set8KLight(int val);
	void Set16KLight(int val);
	void SetLightTime(int light_idx, BOOL start_light_time);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LIGHT_CTRL };
protected:
	CSerialPort m_cLight_Align;
	CSerialPort m_cLight_16k;
	CSerialPort m_cLight_8k;

	//20160717 ngh
	long m_lLightTime[MAX_LIGHT + 1];

	BOOL m_bLightTimeSave;
	BOOL LoadLightTime(CString path);
	BOOL SaveLightTime(CString path);

public:
	BOOL m_bLightTime[MAX_LIGHT];
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
protected:
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlLightSet0();
	virtual BOOL OnInitDialog();
	BOOL LoadLightPara(CString path); 
	BOOL SaveLightPara(CString path);
	void OpenLight();
	
	CSliderCtrl m_slider_light0;
	CSliderCtrl m_slider_light1;
	CSliderCtrl m_slider_light2;
	CSliderCtrl m_slider_light3;
	int m_edit_light0;
	int m_edit_light1;
	int m_edit_light2;
	int m_edit_light3;
	afx_msg void OnNMCustomdrawSliderLight0(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderLight1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderLight2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderLight3(NMHDR *pNMHDR, LRESULT *pResult);
	void ClickBtnenhctrlLightSet1();
	void ClickBtnenhctrlLightSet2();
	void ClickBtnenhctrlLightSet3();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_edit_light4;
	CSliderCtrl m_slider_light4;
	afx_msg void OnNMCustomdrawSliderLight4(NMHDR *pNMHDR, LRESULT *pResult);
	void ClickBtnenhctrlLightSet4();
	void Light8kDisconnect();
	void Light16kDisconnect();
	CString m_strlog;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void ClickBtnenhctrlIpsApply();
	void ClickBtnenhctrlIpsSave();
	CBtnenhctrl m_btn_light_time_0;
	CBtnenhctrl m_btn_light_time_1;
	CBtnenhctrl m_btn_light_time_2;
	CBtnenhctrl m_btn_light_time_3;
	CBtnenhctrl m_btn_light_time_4;
	CBtnenhctrl m_btn_set_light_0;
	CBtnenhctrl m_btn_set_light_1;
	CBtnenhctrl m_btn_set_light_2;
	CBtnenhctrl m_btn_set_light_3;
	CBtnenhctrl m_btn_set_light_4;
	void ClickBtnenhctrlLightTimeReset0();
	void ClickBtnenhctrlLightTimeReset1();
	void ClickBtnenhctrlLightTimeReset2();
	void ClickBtnenhctrlLightTimeReset3();
	void ClickBtnenhctrlLightTimeReset4();
};
