#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define DEFECT_SORT
#define DEFECT_MOVE

struct SORTPARAM
{
	int iSotColumm;
	BOOL bSortDirect;
	CListCtrl *pList;
};

// CDefectList 대화 상자입니다.

class CDefectList : public CDialogEx
{
	DECLARE_DYNAMIC(CDefectList)

public:
	CDefectList(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDefectList();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DEFECT_LIST };

public:
	CListCtrl *GetList();
	void SetDefectListInfo(int idx, CString strData);
	void InitList(int iSotColumm, BOOL bSortDirect);
	void ChangeListCol(int iSotColumm, BOOL bSortDirect);
protected:
	void WriteList();
	void ListUpdate();

	vector<CString> m_strDefectListInfo;

	int m_iListIdx;
	BOOL m_bSortAscending;
	int m_nSortColumn;
	CFont m_font;
private:
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_defect;
	BOOL m_chk_Pat;
	BOOL m_chk_White;
	BOOL m_chk_Small;
	BOOL m_chk_Medium;
	BOOL m_chk_Large;
	BOOL m_chk_OverLarge;
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedCheckDefectSelect0();
	afx_msg void OnBnClickedCheckDefectSelect1();
	afx_msg void OnBnClickedCheckDefectSelect2();
	afx_msg void OnBnClickedCheckDefectSelect3();
	afx_msg void OnBnClickedCheckDefectSelect4();
	afx_msg void OnBnClickedCheckDefectSelect5();
	afx_msg void OnNMDblclkListDefect(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnHdnItemclickListDefect(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_chk_Defect0;
	CButton m_chk_Defect1;
	CButton m_chk_Defect2;
	CButton m_chk_Defect3;
	CButton m_chk_Defect4;
	CButton m_chk_Defect5;
};
