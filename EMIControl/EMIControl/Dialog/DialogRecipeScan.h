#pragma once
#include "afxcmn.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "afxwin.h"


// CDialogRecipeScan 대화 상자입니다.

#define RECIPE_SCAN_PATH						"D:\\DATABASE\\SYSTEM"			
#define RECIPE_SCAN_SCANSPD						1500.

#ifdef CAMERA
#define RECIPE_SCAN_SCAN_WD						63000
#define RECIPE_SCAN_SCAN_16K_WD					190000
#else
#define RECIPE_SCAN_SCAN_WD						10
#define RECIPE_SCAN_SCAN_16K_WD					10
#endif
#define RECIPE_SCAN_SCAN_HT						8208
#define RECIPE_SCAN_SCAN_16K_HT					16400


#ifdef PCL_TEST
#define RECIPE_SCAN_IMG_RECT_MAX_NUM			5
#else
#define RECIPE_SCAN_IMG_RECT_MAX_NUM			3
#endif
#define RECIPE_SCAN_ZOOM_HEIGHT_MULT			3.

#define MAX_DEFECT_SIZE_IDX						4
#define MAX_NOISE_SIZE_INDEX					3

#define MAX_HALCON_IMAGE_WIDTH_8K				RECIPE_SCAN_SCAN_HT + 16
#define MAX_HALCON_IMAGE_HEIGHT_8K				RECIPE_SCAN_SCAN_16K_HT + 0
#define NG_IMAGE_HEIGHT							20
#define NG_IMAGE_WIDTH							20



typedef struct ScanData
{
	//Cell align mark
	double m_dCellAlignMarkPoxX[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCellAlignMarkPoxY[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];

	//Cell active area
	double m_dCellActiveRectSX[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCellActiveRectSY[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCellActiveRectEX[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCellActiveRectEY[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCellActiveCirclCX[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCellActiveCirclCY[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCellActiveCirclR[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	
	int m_iScanRectNum[DEFECT_MAX_SCAN_NUM];

	//Cell active area
	double m_dCVDActiveRectSX[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCVDActiveRectSY[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCVDActiveRectEX[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCVDActiveRectEY[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCVDActiveCirclCX[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCVDActiveCirclCY[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCVDActiveCirclR[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	
	int m_iCVDScanRectNum[DEFECT_MAX_SCAN_NUM];

	//Cell active area
	double m_dCVDDNActiveRectSX[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCVDDNActiveRectSY[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCVDDNActiveRectEX[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCVDDNActiveRectEY[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCVDDNActiveCirclCX[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCVDDNActiveCirclCY[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dCVDDNActiveCirclR[DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	
	int m_iCVDDNScanRectNum[DEFECT_MAX_SCAN_NUM];

	double m_dPCLActiveRectSX[MAX_INS_TYPE][DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dPCLActiveRectSY[MAX_INS_TYPE][DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dPCLActiveRectEX[MAX_INS_TYPE][DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];
	double m_dPCLActiveRectEY[MAX_INS_TYPE][DEFECT_MAX_SCAN_NUM][GLASS_MAX_CELL_NUM];

	int m_iPCLScanRectNum[MAX_INS_TYPE][DEFECT_MAX_SCAN_NUM];

	//저장데이타
	////////////////////////////////////////////////////////////////
	double m_dScan_start_x;
	double m_dScan_start_y;
	double m_dScan_leng;
	double m_dScan_pitch;
	double m_dScan_pixel_x;
	double m_dScan_pixel_y;
	double m_dScan_overlay;
	double m_dScan_overlay2;

	double m_dScan_cnt;

	double m_dScan_coord_x;
	double m_dScan_coord_y;

	double m_dScan_glass_x;
	double m_dScan_glass_y;

	double m_dThrsh0;
	double m_dThrsh1;
	double m_dThrsh2;
	double m_dThrsh3;
	double m_dThrsh4;

	double m_dDefect_size0;
	double m_dDefect_size1;
	double m_dDefect_size2;
	double m_dDefect_size3;
	double m_dDefect_size4;

	double m_dDefect_size_small;
	double m_dDefect_size_medium;
	double m_dDefect_size_large;

	double m_dLight0;

	BOOL m_bInspectUse;
	BOOL m_bInspect90Degree;

	double m_dScanRotateOffset;

	double m_dScanEziPos;
	////////////////////////////////////////////////////////////////

	//20160313 ngh
	BOOL m_ErrorSizeSelect[MAX_DEFECT_SIZE_IDX];
	int m_SelectDefectErrorCnt[MAX_DEFECT_SIZE_IDX];

	//20160317 ngh
	int m_NoiseSize[MAX_NOISE_SIZE_INDEX];
	double m_MargeDistance;

	//20160323 ngh
	double m_ReduceSize;

	//20160328 ngh
	BOOL m_bErrorBlackWhite;
	int m_BlackWhiteCnt;

	//20160406
	int m_MaskDefectCnt;
	BOOL m_bMaskDefect;
	double m_MaskDefectDistance;

	double m_OFDefectSizeWidth;
	double m_OFDefectSizeHeight;

	int m_ReduceSelect;
	
	//20160719 ngh
	double m_edit_dDefectCompare_Distance;
}typeScanData;


class CDialogRecipeScan : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogRecipeScan)

public:
	CDialogRecipeScan(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogRecipeScan();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_RECIPE_SCAN };
public:
	///////////////////////////////////////////////////////

	double GetScanStartX(int type_idx);
	double GetScanStartY(int type_idx);
	double GetScanLeng(int type_idx);
	double GetScanPitch(int type_idx);
	double GetScanPixelX(int type_idx);
	double GetScanPixelY(int type_idx);
	double GetScanCnt(int type_idx);
	double GetScanImageX(int type_idx);
	double GetScanImageY(int type_idx);
	double GetScanGlassX(int type_idx);
	double GetScanGlassY(int type_idx);

	double GetScanStartX_coordXY(int type_idx);
	double GetScanStartY_coordXY(int type_idx);
	double GetScanLeng_coordXY(int type_idx);
	double GetScanPitch_coordXY(int type_idx);
	double GetScanPixelX_coordXY(int type_idx);
	double GetScanPixelY_coordXY(int type_idx);
	double GetScanOverlay(int type_idx, int type);


	double GetScanCnt_coordXY(int type_idx);
	double GetScanImageX_coordXY(int type_idx);
	double GetScanImageY_coordXY(int type_idx);
	double GetScanGlassX_coordXY(int type_idx);
	double GetScanGlassY_coordXY(int type_idx);

	double GetEzPos(int type_idx);

	void GetCoordScanXYfromGlass_coordXY(int type_idx, int pitch_idx, double scan_wd, double glass_x, double galss_y, double *scan_x, double *scan_y);
	void GetCoordGlassXYfromScan_coordXY(int type_idx, int pitch_idx, double scan_wd, double scan_x, double scan_y, double *glass_x, double *glass_y);
	int GetThresh(int type_idx, int idx);
	double GetDefectSize(int type_idx, int idx);
	int GetLight(int type_idx);
	int GetActiveRectNum(int type_idx, int scan_idx);
	int GetCVDActiveRectNum(int type_idx, int scan_idx);
	int GetCVDDNActiveRectNum(int type_idx, int scan_idx);
	RECT GetActiveRect(int type_idx, int scan_idx, int rect_idx);
	RECT GetCVDActiveRect(int type_idx, int scan_idx, int rect_idx);
	RECT GetCVDDNActiveRect(int type_idx, int scan_idx, int rect_idx);
	BOOL SetInspectScan(long disp, Hobject *image, int type_idx, int thresh, Hobject *ins_rgn, long *rslt_num, Hobject *rslt_rgn);
	BOOL SetInspectScan(long disp, Hobject *image, int scan_idx, long *rslt_num, Hobject *rslt_rgn, long *pat_rslt_num, Hobject *pat_rslt_rgn);
	BOOL SetInspectScanCVD(long disp, Hobject *image, BOOL up_direct, int type_idx, double pixel_size, int thresh, long *rslt_num, double *rslt_x, double *rslt_y);
	void GetRegionGlassfromScan_coordXY(int type_idx, int pitch_idx, double scan_wd, Hobject scan_rgn, Hobject *glass_rgn);

	double GetDefectSmall(int type_idx);
	double GetDefectMedium(int type_idx);
	double GetDefectLarge(int type_idx);

	void SetUpdate();

	int GetInspectUse(int type_idx);
	int GetInspect90Degree(int type_idx);
	double GetRoatateOffset(int type_idx);
	CString GetGlassMainsub();

	BOOL SetLoadRecipeScanPara(CString path);
	BOOL SetSaveRecipeScanPara(CString path);

	typeRecipeScan GetRecipe(int type_idx);
	///////////////////////////////////////////////////////

	//20160313 ngh
	BOOL GetDefectCntError(int type_idx, int nDefectIndex, int nDefectCnt);
	int GetSelectDefectErrorSize(int type_idx, int defect_idx);
	int GetSelectDefectErrorCnt(int type_idx, int defect_idx);

	//20160317 ngh
	int GetNoiseSize(int type_idx, int idx);
	double GetMargeDistance(int type_idx);

	//20160323 ngh
	double GetReduceSize(int type_idx);

	//20160328
	int GetBlackWhiteDefectCnt(int type_idx);

	//20160406 ngh
	double GetMaskDefectDistance(int type_idx);
	int GetMaskDefectCnt(int type_idx);
	void GetOFDefectSize(int type_idx, double *width, double *height);
	int GetReduceSelect(int type_idx);

	//2010412 ngh
	void SetUpdate(BOOL isUpdate);

	//20160422
	int GetScanBright();
	void GetThresh(int type_idx, int *rslt);
	void GetFactor(int type_idx, int *rslt);
	void GetFactor(int type_idx, double *rslt);

	void ParameterCompare(int type_idx);

	//201600613 ngh
	int GetPCLActiveRectNum(int type_idx, int scan_idx, int type);
	RECT GetPCLActiveRect(int type_idx, int scan_idx, int rect_idx, int type);

	//20160615 sc
	BOOL GetScanDatState(int type);

	//20160719 ngh
	double GetDefectCompareDistance(int type_idx);
protected:
	C_CtrlImageRect *m_dlgImgRect[RECIPE_SCAN_IMG_RECT_MAX_NUM];

	//Halcon image
	Hobject m_HalImage[RECIPE_SCAN_IMG_RECT_MAX_NUM];
	Hobject m_DefectRegion;
	Hobject m_InspectRegion;

	BOOL LoadRecipeScanPara(CString path);
	BOOL SaveRecipeScanPara(CString path);

	int m_iScanSeq;

	typeScanData m_ScanDat[RECIPE_SCAN_TYPE_MAX_NUM];

	//Image display
	LRESULT OnImageRectMessage(WPARAM para0, LPARAM para1);
	
	void DispGlassDat();
	void MakeGlassDat(int type_idx, int scan_idx, BOOL disp);

	void Update(BOOL falg);

	//20160322 ngh
	void ParameterCaptionChange(int type_idx);

	//20160509 sc
	CString m_strGlassMainSub;

	//20160722 ngh
	void ParameterVisible(int type_idx);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
protected:

	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlRecipeScanApply();
	void ClickBtnenhctrlRecipeScanSave();
	void ClickBtnenhctrlRecipeScanCancel();

	void ClickBtnenhctrlPanelScanSetReviewPos();
	void ClickBtnenhctrlPanelScanSetReviewMove();
	void ClickBtnenhctrlPanelScanScanstart();
	void ClickBtnenhctrlPanelScanLoadScanimage();
	void ClickBtnenhctrlPanelScanDrawCoord();
	void ClickBtnenhctrlPanelScanSetLoadMove();
	void ClickBtnenhctrlPanelScanThreshTest();
	void ClickBtnenhctrlPanelScanApplyToGlass();

	void ClickBtnenhctrlRecipeScanTab0();
	void ClickBtnenhctrlRecipeScanTab1();

	void ClickBtnenhctrlRecipeScanTab2();
	void ClickBtnenhctrlRecipeScanTab3();
	void ClickBtnenhctrlRecipeScanTab4();

	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnChangeEditRecipeScanIdx();

	////////////////////////////////////////////////////////////////
	double m_edit_recipe_scan_start_x;
	double m_edit_recipe_scan_start_y;
	double m_edit_recipe_scan_wd;
	double m_edit_recipe_scan_leng;
	double m_edit_recipe_scan_pitch;
	double m_edit_recipe_scan_pixel_x;
	double m_edit_recipe_scan_pixel_y;

	double m_edit_recipe_scan_cnt;

	double m_edit_recipe_scan_coord_x;
	double m_edit_recipe_scan_coord_y;

	double m_edit_recipe_glass_x;
	double m_edit_recipe_glass_y;

	double m_edit_thrsh0;
	double m_edit_thrsh1;
	double m_edit_thrsh2;
	double m_edit_thrsh3;
	double m_edit_thrsh4;

	double m_edit_defect_size0;
	double m_edit_defect_size1;
	double m_edit_defect_size2;
	double m_edit_defect_size3;
	double m_edit_defect_size4;
	double m_edit_recipe_defect_size_small;
	double m_edit_recipe_defect_size_medium;
	double m_edit_recipe_defect_size_large;
	////////////////////////////////////////////////////////////////

	int m_tab_type_idx;
	int m_edit_scan_idx;

	CBtnenhctrl m_btn_panel_scan_set_review_pos;
	CBtnenhctrl m_btn_panel_scan_set_review_move;
	CBtnenhctrl m_btn_panel_scan_scanstart;
	CBtnenhctrl m_btn_panel_scan_load_scanimage;
	CBtnenhctrl m_btn_panel_scan_draw_coord;

	CBtnenhctrl m_tab_0;
	CBtnenhctrl m_tab_1;
	CBtnenhctrl m_tab_2;
	CBtnenhctrl m_tab_3;
	CBtnenhctrl m_tab_4;
public:
	double m_edit_light0;
	BOOL m_check_inspect_use;
	BOOL m_check_inspect_90degree;
	double m_edit_rotate_theta;
	double m_edit_recipe_scan_overlay;
	double m_edit_recipe_scan_overlay2;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	double m_edit_recipe_scan_ezi_pos;
	void ClickBtnenhctrlPanelScanEziSetPos();
	void ClickBtnenhctrlPanelScanMovePos();
	void ClickBtnenhctrlWindowResize();
	CComboBox m_comboErrorSizeSelect;
	int m_edit_nSelectDefectErrorCnt0;
	BOOL m_check_bSmallSize;
	BOOL m_check_bMediumSize;
	BOOL m_check_bLargeSize;
	BOOL m_check_bOverflowSize;
	int m_edit_nSelectDefectErrorCnt1;
	int m_edit_nSelectDefectErrorCnt2;
	int m_edit_nSelectDefectErrorCnt3;
	int m_edit_nNoise0;
	int m_edit_nNoise1;
	int m_edit_nNoise2;
	double m_edit_dMargeDistance;
	double m_edit_dTest0;
	double m_edit_dTest1;
	double m_edit_dTest2;
	double m_edit_dTest3;
	double m_edit_dTest4;
	double m_edit_dTest5;
	double m_edit_reduce_size;
	int m_edit_nSelectDefectErrorCnt4;
	BOOL m_check_bBlackWhite;
	int m_edit_nSelectDefectErrorCnt5;
	double m_edit_dMaskDefectDistance;
	BOOL m_check_bMaskDefect;
	double m_edit_OFWidth;
	double m_edit_OFHeight;
	CComboBox m_combo_ReduceSelect;
	afx_msg void OnBnClickedRadioMain();
	afx_msg void OnBnClickedRadioSub();
	double m_edit_dDefectCompare_Distance;
};


