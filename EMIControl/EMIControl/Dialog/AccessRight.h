#pragma once
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"


#define MAX_ACCESS_RIGHT				4
#define ACCESS_PATH						_T("D:\\DATABASE\\SYSTEM")

typedef struct{
	BOOL bAutoRun;
	BOOL bRecipe;
	BOOL bSystem;
	BOOL bAlign;
	BOOL bInline;
	BOOL bLog;
	BOOL bDefectServer;
}typeAccessRight;
// AccessRight 대화 상자입니다.

class CAccessRight : public CDialogEx
{
	DECLARE_DYNAMIC(CAccessRight)

public:
	CAccessRight(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAccessRight();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ACCESS_RIGHT };

public:
	BOOL LoadAccessRightPara(CString path);
	BOOL SaveAccessRightPara(CString path);
	void Update(BOOL isUpdate);
	typeAccessRight GetAccessRight(int type);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	typeAccessRight m_AccessRight[MAX_ACCESS_RIGHT];

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlAccessSave();
	void ClickBtnenhctrlAccessApply();
	void ClickBtnenhctrlAccessCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CBtnenhctrl m_btn_auto_run0;
	CBtnenhctrl m_btn_recipe0;
	CBtnenhctrl m_btn_system0;
	CBtnenhctrl m_btn_align0;
	CBtnenhctrl m_btn_inline0;
	CBtnenhctrl m_btn_log0;
	CBtnenhctrl m_btn_defect_server0;
	CBtnenhctrl m_btn_auto_run1;
	CBtnenhctrl m_btn_recipe1;
	CBtnenhctrl m_btn_system1;
	CBtnenhctrl m_btn_align1;
	CBtnenhctrl m_btn_inline1;
	CBtnenhctrl m_btn_log1;
	CBtnenhctrl m_btn_defect_server1;
	CBtnenhctrl m_btn_auto_run2;
	CBtnenhctrl m_btn_recipe2;
	CBtnenhctrl m_btn_system2;
	CBtnenhctrl m_btn_align2;
	CBtnenhctrl m_btn_inline2;
	CBtnenhctrl m_btn_log2;
	CBtnenhctrl m_btn_defect_server2;
	CBtnenhctrl m_btn_auto_run3;
	CBtnenhctrl m_btn_recipe3;
	CBtnenhctrl m_btn_system3;
	CBtnenhctrl m_btn_align3;
	CBtnenhctrl m_btn_inline3;
	CBtnenhctrl m_btn_log3;
	CBtnenhctrl m_btn_defect_server3;
	virtual BOOL OnInitDialog();
};
