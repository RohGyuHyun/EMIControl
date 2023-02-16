#pragma once
#include "afxcmn.h"

#define IPS_PATH						"D:\\DATABASE\\SYSTEM"

// CDialogIPSCtrl 대화 상자입니다.

class CDialogIPSCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogIPSCtrl)

public:
	CDialogIPSCtrl(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogIPSCtrl();

	int GetIPSPort();
	CString GetIPSIP();

protected:
	BOOL SaveIPSPara(CString path);
	BOOL LoadIPSPara(CString path);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_IPS_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CIPAddressCtrl m_IPS_Address;
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlIpsApply();
	virtual BOOL OnInitDialog();
	int m_edit_nips_port;
};
