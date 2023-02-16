#pragma once
#include "afxcmn.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"


// CDialogAlign 대화 상자입니다.

#define ALIGN_IMG_RECT_MAX_NUM	2
#define	IMAGE_RECT_DIALOG		10000

#define ALIGN_IMAGE_WIDTH		1280
#define ALIGN_IMAGE_HEIGHT		960

#define ALIGN_CAM_MAX_NUM	    2
#define ALIGN_BUF_MAX_NUM		1

#define ALIGN_THRESH			30
#define ALIGN_PAT_SCORE			0.5

#define ALIGN_MEASURE_LENG		3

#define ALIGN_PATH				"D:\\DATABASE\\SYSTEM"

typedef struct AlignData
{
	//패턴영역
	RECT m_InsRect[ALIGN_CAM_MAX_NUM];
	RECT m_PatRect[ALIGN_CAM_MAX_NUM];

	Hlong m_PatIDAlign[ALIGN_CAM_MAX_NUM];
	BOOL m_bCreatPatIDAlign[ALIGN_CAM_MAX_NUM];
}typeAlignData;

typedef struct AlignResult
{
	double m_dFindPosX_pixel[ALIGN_CAM_MAX_NUM];					//찾은 mark중심
	double m_dFindPosY_pixel[ALIGN_CAM_MAX_NUM];
	double m_dFindPosA_pixel[ALIGN_CAM_MAX_NUM];					//찾은 mark각도(사용않함)
	double m_dFindPosX_mm[ALIGN_CAM_MAX_NUM];						//찾은 mark중심(mm)
	double m_dFindPosY_mm[ALIGN_CAM_MAX_NUM];
	double m_dTeachPosX_mm[ALIGN_CAM_MAX_NUM];						//티칭 mark중심(mm)
	double m_dTeachPosY_mm[ALIGN_CAM_MAX_NUM];

	double m_dFindScore[ALIGN_CAM_MAX_NUM];							//찾은 score
	double m_dFindNum[ALIGN_CAM_MAX_NUM];							//찾은 개수(1이여야 찾은것임)

	double m_dFindDiffX_pixel[ALIGN_CAM_MAX_NUM];					//찾은 mark - 티칭(pixel)		
	double m_dFindDiffY_pixel[ALIGN_CAM_MAX_NUM];
	double m_dFindDiffCentX_pixel[ALIGN_CAM_MAX_NUM];				//찾은 mark - 카메라중심(pixel)
	double m_dFindDiffCentY_pixel[ALIGN_CAM_MAX_NUM];
	double m_dFindDiffX_mm[ALIGN_CAM_MAX_NUM];						//찾은 mark - 티칭(mm)
	double m_dFindDiffY_mm[ALIGN_CAM_MAX_NUM];
	double m_dFindDiffCentX_mm[ALIGN_CAM_MAX_NUM];					//찾은 mark - 카메라중심(mm)
	double m_dFindDiffCentY_mm[ALIGN_CAM_MAX_NUM];

	double m_dRobotPosX_afteralign;									//Align후 위치 X
	double m_dRobotPosY_afteralign;									//Align후 위치 Y
	double m_dRobotPosT_afteralign;									//Align후 위치 T

	double m_dRobotPosX_afteroffset;								//Align후 offset적용 위치 X
	double m_dRobotPosY_afteroffset;								//Align후 offset적용 위치 Y
	double m_dRobotPosT_afteroffset;								//Align후 offset적용 위치 T

	//좌표계 형성///////////////////////////////////////////////////////////////////////////////////////////
	//Camera 좌표계 중심 위치
	double m_dCoordCntX;
	double m_dCoordCntY;

	//mark0에서부터 Mark1까지 X, Y거리
	double m_dDistInterMarkX;
	double m_dDistInterMarkY;

	//Aling0번 카메라의 중심이 좌표계 중심임.
	double m_dCoordAng_teach;										//티칭당시 mark0, mark1각도
	double m_dCoordAng_rslt;										//결과 mark0, mark1각도
	double m_dCoordX_rslt;
	double m_dCoordY_rslt;
	double m_dCoordAng_diff;										//결과 - 티칭
	double m_dCoordX_diff;
	double m_dCoordY_diff;
	
	double m_dCoordMarkCntX_teach[ALIGN_CAM_MAX_NUM];				//티칭당시 mark0, mark1 중심		
	double m_dCoordMarkCntY_teach[ALIGN_CAM_MAX_NUM];	
	double m_dCoordMarkCntX_rslt[ALIGN_CAM_MAX_NUM];				//결과 mark0, mark1 중심		
	double m_dCoordMarkCntY_rslt[ALIGN_CAM_MAX_NUM];

	double m_dCoordMarkCntX_rslt_afterRot[ALIGN_CAM_MAX_NUM];		//결과 mark0, mark1 중심을 회전한 결과
	double m_dCoordMarkCntY_rslt_afterRot[ALIGN_CAM_MAX_NUM];

	double m_dCoordShiftX;
	double m_dCoordShiftY;

	
	
}typeAlignResult;

class CDialogAlign : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogAlign)

public:
	CDialogAlign(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogAlign();

	
	

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ALIGN };
public:
	double GetAlignPosX();
	double GetAlignPosY();
	double GetAlignPosT();
	double GetAlignOffsetPosX();
	double GetAlignOffsetPosY();
	double GetAlignOffsetPosT();

	double GetAlignReadyPosY();
	double GetAlignReadyPosT();

	int SetFind();
	void SetGrab();
	double GetAlignAfterPosX();
	double GetAlignAfterPosY();
	double GetAlignAfterPosT();
	
	//Glass
	double GetAlignOffsetPosX_coordXY();
	double GetAlignOffsetPosY_coordXY();

	//Light
	int GetLight(int idx);

	BOOL SetLoadAlignPara(CString path);
	BOOL SetSaveAlignPara(CString path);

	//2010412 ngh
	void SetUpdate(BOOL isUpdate);

	//20160720 ngh
	BOOL CamReconnect();
protected:
	C_CtrlImageRect *m_dlgImgRect[ALIGN_IMG_RECT_MAX_NUM];

	//Halcon image
	Hobject m_HalImage[ALIGN_IMG_RECT_MAX_NUM];

	//Teli
	int m_iCamNum;
    CAM_HANDLE				m_hCam[ALIGN_CAM_MAX_NUM];				//Handle of camera assigned by TeliCAMApi on opening camera.
    CAM_STRM_HANDLE			m_hStrm[ALIGN_CAM_MAX_NUM];				//Handle of stream channel assigned by TeliCAMApi on opening stream channel.
    void					*m_pvImgBGRA[ALIGN_CAM_MAX_NUM];				// Current BGRA image converted from m_pucLatestImg. 
	BOOL Open();													//Teli open
	BOOL Initialize();												//Initialize variables.
	void CloseAllCameras();											//Close All cameras.
	BOOL OpenStream();
	BOOL CloseStream();
	void DrawCross(int cam_idx);
	BOOL MakeRsltCoord(double tx0, double ty0, double tx1, double ty1, 
		               double rx0, double ry0, double rx1, double ry1,
					   double shift_x, double shift_y, 
					   double camera_cnt_x, double camera_cnt_y,
					   double dist_inter_mark_x, double dist_inter_mark_y);

	static void CALLBACK CallBackImageAcquired(CAM_HANDLE hCam,
									CAM_STRM_HANDLE hStrm,
									CAM_IMAGE_INFO *psImageInfo,
									uint32_t uiBufferIndex,
									void *pvContext);				// Callback for handling received image.

	//Image display
	LRESULT OnImageRectMessage(WPARAM para0, LPARAM para1);
	
	void Grab(int cam_idx, BOOL disp);
	void Live(int cam_idx, BOOL disp);
	void Stop(int cam_idx);
	void Update(BOOL flag);
	BOOL LoadAlignPara(CString path);
	BOOL SaveAlignPara(CString path);

	int AlignInspectFindPos();

	BOOL m_bIsDisp[ALIGN_CAM_MAX_NUM];
	BOOL m_bIsSnap[ALIGN_CAM_MAX_NUM];
	int m_iJogSpdSelect;

	//Align data
	typeAlignData m_AlignData;
	typeAlignResult m_AlignRslt;
	typePatPara m_AlignPara[ALIGN_CAM_MAX_NUM];

	//20160422 ngh
	double m_dRecipeAlignPara[100];
	void ParameterCompare();


	//20160708 ngh
	int m_nFindWhileCnt;
	int m_nFindAlignCnt;
	BOOL m_bFindAlign;

	//20160725 ngh
	BOOL m_bAlignInspection;
	BOOL m_bAlignTestInspection1;
	BOOL m_bAlignTestInspection2;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlAlignSnap();
	void ClickBtnenhctrlAlignLive();
	void ClickBtnenhctrlAlignStop();
	void ClickBtnenhctrlAlignFindpos();
	void ClickBtnenhctrlAlignMovepos();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ClickBtnenhctrlAlignMarkTeach0();
	void ClickBtnenhctrlAlignMarkTeach1();
	double m_edit_align_y_axi_pos;
	double m_edit_align_t_axi_pos;
	double m_edit_align_offset_x;
	double m_edit_align_offset_y;
	double m_edit_align_mark_size0;
	double m_edit_align_mark_size1;
	double m_edit_align_mark_res0;
	double m_edit_align_mark_res1;
	double m_edit_align_posx0;
	double m_edit_align_posy0;
	double m_edit_align_posx1;
	double m_edit_align_posy1;
	double m_edit_align_shaftx;
	double m_edit_align_shafty;
	double m_edit_align_light0;
	double m_edit_align_light1;
	CSliderCtrl m_slider_align_light0;
	CSliderCtrl m_slider_align_light1;
	afx_msg void OnNMCustomdrawSliderAlignLight0(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderAlignLight1(NMHDR *pNMHDR, LRESULT *pResult);
	void MouseDownBtnenhctrlAlignScanJogM(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlAlignScanJogM(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlAlignScanJogP(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlAlignScanJogP(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlAlignJogCcw(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlAlignJogCw(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlAlignJogCw(short Button, short Shift, long x, long y);
	void ClickBtnenhctrlAlignJogSpd1();
	void ClickBtnenhctrlAlignJogSpd2();
	void ClickBtnenhctrlAlignJogSpd3();
	void ClickBtnenhctrlAlignApply();

	void ClickBtnenhctrlAlignMarkCopy0To1();
	double m_edit_offset_x_rat;
	double m_edit_offset_y_rat;
	double m_edit_align_pixel_size;
	double m_edit_align_offset_t;
	void MouseDownBtnenhctrlAlignJogCcw(short Button, short Shift, long x, long y);
	CBtnenhctrl m_btn_jog_spd0;
	CBtnenhctrl m_btn_jog_spd1;
	CBtnenhctrl m_btn_jog_spd2;
	void ClickBtnenhctrlAlignMarkFind0();
	void ClickBtnenhctrlAlignMarkPosset0();
	void ClickBtnenhctrlAlignMarkPosset1();
	CBtnenhctrl m_btn_align_light_set0;
	CBtnenhctrl m_btn_align_light_set1;
	void ClickBtnenhctrlAlignLightSet0();
	void ClickBtnenhctrlAlignLightSet1();
	void ClickBtnenhctrlAlignMarkFind1();
	double m_edit_align_jog_pitch;
	double m_edit_align_shaft_offset_x;
	double m_edit_align_shaft_offset_y;
	void ClickBtnenhctrlAlignSave();
	void ClickBtnenhctrlAlignSetAlignPos();
	void ClickBtnenhctrlAlignAlignMove();
	void ClickBtnenhctrlAlignReviewMovePos0();
	void ClickBtnenhctrlAlignReviewMovePos1();
	double m_edit_align_x_axi_pos;
	void ClickBtnenhctrlAlignSetAlignPosShift();
	void ClickBtnenhctrlAlignAlignMoveShift();
protected:
	void MouseDownBtnenhctrlAlignJogL(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlAlignJogL(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlAlignJogR(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlAlignJogR(short Button, short Shift, long x, long y);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
