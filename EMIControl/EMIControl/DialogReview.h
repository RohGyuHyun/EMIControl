#pragma once
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"

// CDialogReview 대화 상자입니다.

#define REVIEW_IMG_RECT_MAX_NUM				1
#define	IMAGE_RECT_DIALOG					10000

#define REVIEW_IMAGE_WIDTH					2448
#define REVIEW_IMAGE_HEIGHT					2050

#define	REVIEW_ARECAM_WB_COEFFICIENTS_NUM	3

class CDialogReview : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogReview)

public:
	CDialogReview(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogReview();



// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_REVIEW };
protected:
	C_CtrlImageRect *m_dlgImgRect[REVIEW_IMG_RECT_MAX_NUM];

	//Halcon image
	Hobject m_HalImage[REVIEW_IMG_RECT_MAX_NUM];
	Hobject m_HalImageR[REVIEW_IMG_RECT_MAX_NUM];
	Hobject m_HalImageG[REVIEW_IMG_RECT_MAX_NUM];
	Hobject m_HalImageB[REVIEW_IMG_RECT_MAX_NUM];

	MIL_ID MilApplication;
	MIL_ID MilSystem[REVIEW_IMG_RECT_MAX_NUM];
	MIL_ID MilDigitizer[REVIEW_IMG_RECT_MAX_NUM];

	MIL_ID MilGrabBuf[REVIEW_IMG_RECT_MAX_NUM];
	MIL_ID MilBayerBuf[REVIEW_IMG_RECT_MAX_NUM];
	MIL_ID MilBayerBuf_R[REVIEW_IMG_RECT_MAX_NUM];
	MIL_ID MilBayerBuf_G[REVIEW_IMG_RECT_MAX_NUM];
	MIL_ID MilBayerBuf_B[REVIEW_IMG_RECT_MAX_NUM];

	MIL_ID MilWBCoefficients[REVIEW_IMG_RECT_MAX_NUM];
	float WBCoefficients[REVIEW_IMG_RECT_MAX_NUM][REVIEW_ARECAM_WB_COEFFICIENTS_NUM];

	int m_iJogSpdSelect;

	void Grab(int cam_idx, BOOL disp);
	void Live(int cam_idx, BOOL disp);
	void DrawCross();
	
	//Image display
	LRESULT OnImageRectMessage(WPARAM para0, LPARAM para1);
public:
	void SetFilmAlignRect(BOOL type);
	void SetFilmAlignRect(int cell_x_idx, int cell_y_idx, int film_idx, int rgn_idx, int sub_rgn_idx, RECT rect);
	void GetFilmAlignRect(int film_idx, int rgn_idx, int sub_rgn_idx, RECT *rect);
	void SetDrawCross(int x, int y);
	Hlong GetView();
	Hobject *GetImage();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlRvZoom1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ClickBtnenhctrlRvJogSpd1();
	void MouseDownBtnenhctrlRvJogL(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlRvJogL(short Button, short Shift, long x, long y);
	void ClickBtnenhctrlRvJogSpd2();
	void ClickBtnenhctrlRvJogSpd3();
	void MouseDownBtnenhctrlRvJogR(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlRvJogR(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlRvJogU(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlRvJogU(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlRvJogD(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlRvJogD(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlRvJogCcw(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlRvJogCcw(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlRvJogCw(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlRvJogCw(short Button, short Shift, long x, long y);
	CBtnenhctrl m_btn_jog_spd0;
	CBtnenhctrl m_btn_jog_spd1;
	CBtnenhctrl m_btn_jog_spd2;
	double m_edit_rv_cell_row;
	double m_edit_rv_cell_col;
	void ClickBtnenhctrlRvCellMove();
	void ClickBtnenhctrlRvCellLf();
	void ClickBtnenhctrlRvCellRt();
	void ClickBtnenhctrlRvCellU();
	void ClickBtnenhctrlRvCellDn();
	BOOL m_check_rv_cell_nevi_move;
public:
	

	void ClickBtnenhctrlRvZoomMag2();
	void ClickBtnenhctrlRvZoomMag3();
	CBtnenhctrl m_btn_rv_zoom_1;
	CBtnenhctrl m_btn_rv_zoom_2;
	CBtnenhctrl m_btn_rv_zoom_3;
	void MouseDownBtnenhctrlRvAfUp(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlRvAfDn(short Button, short Shift, long x, long y);
};
