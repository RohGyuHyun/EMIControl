#pragma once
#include "afxcmn.h"

#define IPS_PATH						"D:\\DATABASE\\SYSTEM"

// CDialogIPSCtrl ��ȭ �����Դϴ�.

class CDialogIPSCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogIPSCtrl)

public:
	CDialogIPSCtrl(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogIPSCtrl();

	int GetIPSPort();
	CString GetIPSIP();

protected:
	BOOL SaveIPSPara(CString path);
	BOOL LoadIPSPara(CString path);

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_IPS_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CIPAddressCtrl m_IPS_Address;
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlIpsApply();
	virtual BOOL OnInitDialog();
	int m_edit_nips_port;
};
