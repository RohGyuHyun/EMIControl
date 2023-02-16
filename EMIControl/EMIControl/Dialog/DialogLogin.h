#pragma once
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "afxwin.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"

#define LOGIN_PATH						_T("D:\\DATABASE\\SYSTEM")


// CDialogLogin ��ȭ �����Դϴ�.
typedef struct pw
{
	CString oper;
	CString tech;
	CString admin;
	CString manage;
}typepw;

class CDialogLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogLogin)

public:
	CDialogLogin(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogLogin();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	int m_iSelectType;

	CBtnenhctrl m_btn_operator;
	CBtnenhctrl m_btn_technician;
	CBtnenhctrl m_btn_admin;
//	CEdit m_edit_password;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlLoginManagement();
	void ClickBtnenhctrlLoginOperator();
	void ClickBtnenhctrlLoginTechcian();
	void ClickBtnenhctrlLoginAdmin();
	CString m_edit_password;
	CBtnenhctrl m_btn_login_rslt;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL LoadPasswordPara(CString path);
	BOOL SavePasswordPara(CString path);
	typepw m_pwdata;
	BOOL PassWord();
	virtual BOOL OnInitDialog();
	void ClickBtnenhctrlLoginAccessRight();
	void ClickBtnenhctrlExit();
};
