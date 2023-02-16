#pragma once
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"


// CDialogFilmAlign 대화 상자입니다.
#define FILM_ALIGN_MAX_NUM							
#define FILM_ALIGN_RGN_POS_MAX_MUM					6
#define FILM_ALIGN_MAX_NUM							1	

#define FILM_ALIGN_PATH								"D:\\DATABASE\\SYSTEM"

typedef struct FilmAlignData
{
	RECT m_InsRectLR[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];
	RECT m_InsRectUD[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];

	double m_dThrshLR[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];
	double m_dThrshUD[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];

	int m_iScanUD[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];
	int m_iScanRD[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];
}typeFilmAlignData;

typedef struct FilmAlignResult
{
	double m_dFindPosX_LR[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];
	double m_dFindPosY_LR[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];
	double m_dFindPosX_UD[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];
	double m_dFindPosY_UD[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];

	double m_dFindPosX[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];
	double m_dFindPosY[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];
	double m_dFindPosX_mm[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];
	double m_dFindPosY_mm[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];
}typeFilmAlignResult;

class CDialogFilmAlign : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogFilmAlign)

public:
	CDialogFilmAlign(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogFilmAlign();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_FILM_ALIGN_INSPECT };
protected:
	typeFilmAlignData m_FilmAlignData;
	typeFilmAlignResult m_FilmAlignRslt;

	int m_iSelectRgnNum;

	void Update(BOOL type);
	BOOL LoadFilmAlignPara(CString path);
	BOOL SaveFilmAlignPara(CString path);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
protected:
	DECLARE_EVENTSINK_MAP()
//	void ClickBtnenhctrlFilmAlignPos0();
	void ClickBtnenhctrlFilmAlignPos1();
	void ClickBtnenhctrlFilmAlignPos2();
	void ClickBtnenhctrlFilmAlignPos5();
	void ClickBtnenhctrlFilmAlignPos4();
	void ClickBtnenhctrlFilmAlignPos3();
	CBtnenhctrl m_btn_rgn_pos0;
	CBtnenhctrl m_btn_rgn_pos1;
	CBtnenhctrl m_btn_rgn_pos2;
	CBtnenhctrl m_btn_rgn_pos3;
	CBtnenhctrl m_btn_rgn_pos4;
	CBtnenhctrl m_btn_rgn_pos5;
	void ClickBtnenhctrlFilmAlignMove();
	void ClickBtnenhctrlFilmAlignLf();
	void ClickBtnenhctrlFilmAlignRt();
	void ClickBtnenhctrlFilmAlignUp();
	void ClickBtnenhctrlFilmAlignDn();
	BOOL m_check_film_move;
	BOOL m_check_rgn_move;
	void ClickBtnenhctrlFilmAlignMovepos();
	void ClickBtnenhctrlFilmAlignInspect();
	int m_radio_scan_ud;
	int m_radio_scan_lr;
	void ClickBtnenhctrlFilmAlignApply();
	double m_edit_thrsh_ud;
	double m_edit_thrsh_lr;
	int m_edit_film_num_x;
	double m_edit_film_num_y;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void ClickBtnenhctrlFilmAlignPos0();
};
