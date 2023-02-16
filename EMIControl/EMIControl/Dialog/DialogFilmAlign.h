#pragma once
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "afxcmn.h"


// CDialogFilmAlign 대화 상자입니다.
#define FILM_ALIGN_MAX_NUM							
#define FILM_ALIGN_RGN_POS_MAX_MUM					6
#define FILM_ALIGN_MAX_NUM							1	

#define FILM_ALIGN_EDGE_SIGMA						10

#define FILM_ALIGN_PATH								"D:\\DATABASE\\SYSTEM"

typedef struct FilmAlignData
{
	RECT m_InsRectLR[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];
	RECT m_InsRectUD[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];

	double m_dThrshLR[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];
	double m_dThrshUD[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];

	int m_iScanUD[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];
	int m_iScanRD[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];

	double m_dLight;
	BOOL m_bInspectUse;
	double m_dSpec_x[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];
	double m_dSpec_y[FILM_ALIGN_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];
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
public:
	BOOL SetRegionMove(int row, int col, int rgn_idx, BOOL disp);
	double GetRobotPos(int robo_idx);
	BOOL SetInspectFilmAlign(int rgn_idx, double *inter_x, double *inter_y,CString idx);  //20160425 SC
	BOOL SetInspectFilmAlign2(int rgn_idx, double *inter_x, double *inter_y);//Shadow test용
	short GetLight();
	BOOL GetInspectUse();

	BOOL SetLoadFilmAlignPara(CString path);
	BOOL SetSaveFilmAlignPara(CString path);

	//2010412 ngh
	void SetUpdate(BOOL isUpdate);
	char m_FilmImage[512];

	//20160801 ngh
	double GetFilmAlignSpecX(int rgn_idx);
	double GetFilmAlignSpecY(int rgn_idx);

	BOOL SaveFilmAlignImage(CString file_path);
protected:
	typeFilmAlignData m_FilmAlignData;
	typeFilmAlignResult m_FilmAlignRslt;

	int m_iSelectRgnNum;

	void Update(BOOL type);
	BOOL LoadFilmAlignPara(CString path);
	BOOL SaveFilmAlignPara(CString path);


	//Motion
	double m_dMotPos[MAX_MOTOR];

	void ParameterCompare();
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
	int m_edit_film_num_col;
	int m_edit_film_num_row;
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void ClickBtnenhctrlFilmAlignPos0();
	CSliderCtrl m_slider_light;
	double m_edit_light;
	afx_msg void OnNMCustomdrawSliderAlignLight0(NMHDR *pNMHDR, LRESULT *pResult);
	void ClickBtnenhctrlAlignLightSet0();
public:
	BOOL m_check_inspect_use;
	void ClickBtnenhctrlFilmAlignCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	double m_edit_dSpec_x;
	double m_edit_dSpec_y;
};
