#pragma once
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"


// CDialogRecipe ��ȭ �����Դϴ�.

class CDialogRecipe : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogRecipe)

public:
	CDialogRecipe(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogRecipe();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_RECIPE };
public:
	CString GetRecipeName();
	int GetRecipeNo();
	void SetRecipeName(CString name);
	void SetRecipeNo(int no);

	void SetLotRestart();

protected:
	CString m_sRecipeName;
	int m_iRecipeNo;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
protected:
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlRecipePanel();
	void ClickBtnenhctrlRecipeAlign();
	void ClickBtnenhctrlRecipeCell();
	void ClickBtnenhctrlRecipeFilmAlign();
	void ClickBtnenhctrlRecipeScan();
	void ClickBtnenhctrlRecipeLotRestart();
	CBtnenhctrl m_btn_recipe_name;
	CBtnenhctrl m_btn_recipe_no;
	void ClickBtnenhctrlRecipeNew();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CBtnenhctrl m_btn_recipe_new;
	CBtnenhctrl m_btn_recipe_select;
	CBtnenhctrl m_btn_recipe_delete;
	CBtnenhctrl m_btn_recipe_copy;

	void ClickBtnenhctrlRecipeSave();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ClickBtnenhctrlRecipeCellPcl();
};
