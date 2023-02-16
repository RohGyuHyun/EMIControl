#pragma once
#include "afxcmn.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"


// CDialogRecipeModel 대화 상자입니다.

#define MAX_MOD_NAME					100
#define MAX_MOD_DESC					100

#define RECIPE_MAX_NUM					99

#define MOD_MODEL_PATH					"D:\\DATABASE\\MODEL"
#define MOD_MODEL_REGI   				"SOFTWARE\\EMI\\MODEL"

#define RECIPE_PATH						"D:\\DATABASE\\MODEL"


class CDialogRecipeModel : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogRecipeModel)

public:
	CDialogRecipeModel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogRecipeModel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_RECIPE_MODEL };

public:
	int GetRecipeNo();
	CString GetRecipeName();
	CString GetRecipePath();
	void SetOpen();
	BOOL GetOpenList(int idx);

protected:
	wchar_t m_sRecipeName[RECIPE_MAX_NUM][MAX_MOD_NAME];
	wchar_t m_sRecipePath[RECIPE_MAX_NUM][MAX_PATH];

	int m_iRecipeNo;
	int m_iSelectNo;

	BOOL LoadRecipeLabelPara(CString path);
	BOOL SaveRecipeLabelPara(CString path);
	BOOL LoadRecipeNoPara(CString path);
	BOOL SaveRecipeNoPara(CString path);

	//20160721 ngh
	BOOL m_bRecipeCopy;
	int m_nCopyRecipeNo;
	void RecipeFileCopy();
	CString m_strCopyDesFilePath;
	CString m_strCopySrcFilePath;
	BOOL m_bDefaultRecipeCopy;
	int m_nDefaultRecipeNo;
	BOOL m_bSelectRecipeCopy;
	int m_nSelectRecipeNo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlRecipeModelOk();
	void ClickBtnenhctrlRecipeModelCancel();
	CListCtrl m_list_model;
	CString m_edit_name;
	CString m_edit_desc;
	virtual BOOL OnInitDialog();
	CBtnenhctrl m_btn_new;
	CBtnenhctrl m_btn_select;
	CBtnenhctrl m_btn_delete;
	void ClickBtnenhctrlRecipeModelNew();
	void ClickBtnenhctrlRecipeModelSelect();
	void ClickBtnenhctrlRecipeModelDelete();
	int m_edit_recipe_model_recipe_no;
	afx_msg void OnDblclkListModel(NMHDR *pNMHDR, LRESULT *pResult);
	int m_edit_recipe_no_new;
	CString m_edit_recipe_name_new;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//20160721 ngh
	int m_edit_recipe_no_default;
	CString m_edit_recipe_name_default;
	void ClickBtnenhctrlRecipeModelDefault();
	void ClickBtnenhctrlRecipeModelNew2();
	int m_edit_recipe_no_new2;
	CString m_edit_recipe_name_new2;
	void ClickBtnenhctrlRecipeModelCopy();
	int m_edit_recipe_no_copy;
};
