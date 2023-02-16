#pragma once
//#include "xls\ExcelFormat.h"
//using namespace ExcelFormat;

// CDialogGlass 대화 상자입니다.
#define GLASS_IMG_RECT_MAX_NUM			1
#define	IMAGE_RECT_DIALOG				10000

//1pixel당 10um
#define GLASS_IMAGE_WIDTH				8000
#define GLASS_IMAGE_HEIGHT				5000
#define GLASS_START_OFFSET_X_MM			35
#define GLASS_START_OFFSET_Y_MM			20
#define GLASS_PIXEL_RATIO				0.01

#define GLASS_MAX_CELL_NUM				500		
#define GLASS_MAX_CELL_RV_SHADOW_NUM	100



#define DEFECT_DATA_MAX_NUM				30000
#define DEFECT_MAX_SCAN_NUM				10

#define MAX_INS_TYPE					2
#define MAX_RECT_CNT					4

#define CVD_CAD_CNT						44 * 700

typedef struct GlassData
{
	//Glass///////////////////////////////////////////////
	//Glass align mark
	double m_dGlassAlignMarkPoxX0;
	double m_dGlassAlignMarkPoxY0;
	double m_dGlassAlignMarkPoxX1;
	double m_dGlassAlignMarkPoxY1;

	//Cell////////////////////////////////////////////////
	double m_dCellNum;
	double m_dRVShadowNum;

	//Cell align mark
	double m_dCellAlignMarkPoxX[GLASS_MAX_CELL_NUM];
	double m_dCellAlignMarkPoxY[GLASS_MAX_CELL_NUM];

	//Cell active area
	double m_dCellActiveRectSX[GLASS_MAX_CELL_NUM];
	double m_dCellActiveRectSY[GLASS_MAX_CELL_NUM];
	double m_dCellActiveRectEX[GLASS_MAX_CELL_NUM];
	double m_dCellActiveRectEY[GLASS_MAX_CELL_NUM];
	double m_dCellActiveCirclCX[GLASS_MAX_CELL_NUM];
	double m_dCellActiveCirclCY[GLASS_MAX_CELL_NUM];
	double m_dCellActiveCirclR[GLASS_MAX_CELL_NUM];
	
	//Film align mark
	double m_dFilmAlignMarkPosX[GLASS_MAX_CELL_NUM];
	double m_dFilmAlignMarkPosY[GLASS_MAX_CELL_NUM];

	//Film active area
	double m_dFilmActiveRectSX[GLASS_MAX_CELL_NUM];
	double m_dFilmActiveRectSY[GLASS_MAX_CELL_NUM];
	double m_dFilmActiveRectEX[GLASS_MAX_CELL_NUM];
	double m_dFilmActiveRectEY[GLASS_MAX_CELL_NUM];
	double m_dFilmActiveCirclCX[GLASS_MAX_CELL_NUM];
	double m_dFilmActiveCirclCY[GLASS_MAX_CELL_NUM];
	double m_dFilmActiveCirclR[GLASS_MAX_CELL_NUM];

	//CVD align mark
	double m_dCVDAlignMarkPosX[GLASS_MAX_CELL_NUM];
	double m_dCVDAlignMarkPosY[GLASS_MAX_CELL_NUM];

	//CVD active area
	double m_dCVDActiveRectSX[GLASS_MAX_CELL_NUM];
	double m_dCVDActiveRectSY[GLASS_MAX_CELL_NUM];
	double m_dCVDActiveRectEX[GLASS_MAX_CELL_NUM];
	double m_dCVDActiveRectEY[GLASS_MAX_CELL_NUM];
	double m_dCVDActiveCirclCX[GLASS_MAX_CELL_NUM];
	double m_dCVDActiveCirclCY[GLASS_MAX_CELL_NUM];
	double m_dCVDActiveCirclR[GLASS_MAX_CELL_NUM];

	//CVDDN align mark
	double m_dCVDDNAlignMarkPosX[GLASS_MAX_CELL_NUM];
	double m_dCVDDNAlignMarkPosY[GLASS_MAX_CELL_NUM];

	//CVDDN active area
	double m_dCVDDNActiveRectSX[GLASS_MAX_CELL_NUM];
	double m_dCVDDNActiveRectSY[GLASS_MAX_CELL_NUM];
	double m_dCVDDNActiveRectEX[GLASS_MAX_CELL_NUM];
	double m_dCVDDNActiveRectEY[GLASS_MAX_CELL_NUM];
	double m_dCVDDNActiveCirclCX[GLASS_MAX_CELL_NUM];
	double m_dCVDDNActiveCirclCY[GLASS_MAX_CELL_NUM];
	double m_dCVDDNActiveCirclR[GLASS_MAX_CELL_NUM];

	//double m_dCVDCADResult_x;
	//double m_dCVDCADResult_y;


	double m_dPCLAlignMarkPosX[MAX_RECT_CNT][MAX_INS_TYPE][GLASS_MAX_CELL_NUM];
	double m_dPCLAlignMarkPosY[MAX_RECT_CNT][MAX_INS_TYPE][GLASS_MAX_CELL_NUM];
	double m_dPCLActiveRectSX[MAX_RECT_CNT][MAX_INS_TYPE][GLASS_MAX_CELL_NUM];
	double m_dPCLActiveRectSY[MAX_RECT_CNT][MAX_INS_TYPE][GLASS_MAX_CELL_NUM];
	double m_dPCLActiveRectEX[MAX_RECT_CNT][MAX_INS_TYPE][GLASS_MAX_CELL_NUM];
	double m_dPCLActiveRectEY[MAX_RECT_CNT][MAX_INS_TYPE][GLASS_MAX_CELL_NUM];
	

}typeGlassData;

typedef struct CadResult
{
	double m_dCVDCADResult_x;
	double m_dCVDCADResult_y;
}typeCadData;

#define typeRobotData			typeGlassData	

typedef struct DefectData
{
	//Index//////////////////////////////////////////////////////////
	int m_iDefectIdx;
	int m_iDefectIdxPerScan;

	int m_iScanIdx;
	int m_iCellIdx;

	//Data///////////////////////////////////////////////////////////
	double m_dCntX;
	double m_dCntY;
	double m_dWidth;
	double m_dHeight;
	double m_dArea;
	double m_dgraymin; //sh
	double m_dgraymax; //sh
	double m_dgraymean; //sh gray 평균값
	double m_dgraydeviation; //sh gray 편차값
	char m_sImgPath[MAX_PATH];
	BOOL m_bBlackWhite;

	//20160405 ngh
	BOOL m_bBeforeAfterCompare;

	//20160406 ngh
	int m_bOFDefect;

	//20160408 ngh
	char m_cCellID[5];

	//20160421 ngh
	int m_iSize;

	//20160523 ngh
	BOOL m_CVDUpDw;//0 : dw, 1 : up
	BOOL m_CVDRslt;
	BOOL m_CVDUpRsltX;
	BOOL m_CVDUpRsltY;
	BOOL m_CVDDwRsltX;
	BOOL m_CVDDwRsltY;
}typeDefectData;

typedef struct ScanMange
{
	int m_iLastScanNum;
	int m_iLastDefectIdxPerScan[DEFECT_MAX_SCAN_NUM];
	int m_iLastDefectIdx;
	int m_iCVDNgCnt;
}typeScanManage;

typedef struct DfCount
{
	int idx;
	int count;
}typeDfCount;


class CDialogGlass : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogGlass)

public:
	CDialogGlass(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogGlass();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_GLASS };
	//sh Flim_id
	int m_iFilm_id_a;
	int m_iFilm_id_b;
	int m_defect_size_rslt[DEFECT_DATA_MAX_NUM];

	CString m_strCell_table[30][30];
//	int m_iCell_df_idx_table[30][30];
//	int m_idf_count[30][30];
	typeDfCount m_DfCount[30][30];
public:
	void SetUpdate();
	double GetCellRobotPosX_coordXY(int cell_idx_x, int cell_idx_y);
	double GetCellRobotPosY_coordXY(int cell_idx_x, int cell_idx_y);
	double GetCellGrpRobotPosX_coordXY(int cell_idx_x, int cell_idx_y);
	double GetCellGrpRobotPosY_coordXY(int cell_idx_x, int cell_idx_y);
	void GetCellGrpRegionRobotPos_coordXY(int cell_col_idx, int cell_row_idx, double *sx, double *sy, double *ex, double *ey);
	void SetDrawCross(int x, int y);
	void SetLotInit();
	void SetDrawCrossRobot(int x, int y);
	double GetCellActiveRectSX_coordXY(int idx);
	double GetCellActiveRectSY_coordXY(int idx);
	double GetCellActiveRectEX_coordXY(int idx);
	double GetCellActiveRectEY_coordXY(int idx);

	double GetCVDActiveRectSX_coordXY(int idx);
	double GetCVDActiveRectSY_coordXY(int idx);
	double GetCVDActiveRectEX_coordXY(int idx);
	double GetCVDActiveRectEY_coordXY(int idx);

	double GetCVDDNActiveRectSX_coordXY(int idx);
	double GetCVDDNActiveRectSY_coordXY(int idx);
	double GetCVDDNActiveRectEX_coordXY(int idx);
	double GetCVDDNActiveRectEY_coordXY(int idx);

	//20160613 ngh
	double GetPCLActiveRectSX_coordXY(int rect_idx, int type, int idx);
	double GetPCLActiveRectSY_coordXY(int rect_idx, int type, int idx);
	double GetPCLActiveRectEX_coordXY(int rect_idx, int type, int idx);
	double GetPCLActiveRectEY_coordXY(int rect_idx, int type, int idx);

	CString GetCVDIndex(int scan_type_idx, int defect_idx);
	void GetCVDCadValue(int scan_type_idx, int defect_idx, double *x, double *y, BOOL XorY);

	void SetDefectData_scan(int scan_type_idx, typeDefectData dat);
	typeDefectData GetDefectData_scan(int scan_type_idx, int idx);
	typeDefectData GetDefectData_coordXY(int scan_type_idx, int idx);
	void GetCoordGlassToDisp(double gx, double gy, double *dx, double *dy);
	void GetCoordDispToGlass(double dx, double dy, double *gx, double *gy);
	void SetDispRegion();
	Hobject *GetDefectRgn();
	Hobject *GetInspectRgn();
	void SetDefectRgn(Hobject *region);
	void SetDispGlassDat();
	void SetMakeDefectGlassData(int scan_type_idx);
	void SetDispDefectGlassData(int scan_type_idx);
	void GetCoordGlassToRobot(double gx, double gy, double *rx, double *ry);
	int GetInRectInGlass(int scan_type_idx, double x, double y, double *cx, double *cy);
	int GetRsltSizeCntType(int type_idx, double width, double height);
	double GetRsltSizeCntTypeArea(int type_idx, double Area );
	CString GetInsActive_id(double x, double y, int scan_id_idx, int film_idx_x, int film_idx_y);  
	double GetFilmActiveRectSX(int idx);
	double GetFilmActiveRectSY(int idx);
	
	//20160405 ngh
	BOOL DefectCompare(int scan_type);
	int GetDefectCompareCnt(int scan_type);
	void SetBeforeDefect(int scan_type);
	void SetDispCompareDefectGlassData(int scan_type);
	void InitDefectData(int scan_type, BOOL befor_after);

	//2010406 ngh
	BOOL OFDefectError(int scan_type, double width, double height);
	int GetOFDefectCnt(int scantype);

	//20160407 ngh
	BOOL GlassWindowMapSave(int scan_type, CString path);

	//20160420 ngh
	void MakeCellID();

	//20160421 ngh
	void GetDefectGlassXY(int scan_type, int idx, double *cent_x, double *cent_y, int offset = 2.);
	void DefectMove(int idx);

	//20160422
	int GetDfPanelidx(CString cellid,int type);

	//20160524 ngh
	BOOL GetCVDRslt();
	int GetCVDDefectCnt();

	//20160727 ngh
	C_CtrlImageRect* GetImageRect();

	//20160728 ngh
	double m_dDestRobot_y;
	double m_dDestRobot_x;
	void SetDrawCrossReviewX(int x, int y);
	void SetDrawCrossReviewY(int x, int y);

	void CvdCadResult();
	void DamCadResult();
	long m_lCVDUpPointCnt;
	long m_lCVDDwPointCnt;
protected:
	C_CtrlImageRect *m_dlgImgRect[GLASS_IMG_RECT_MAX_NUM];
	//Halcon image
	Hobject m_HalImage[GLASS_IMG_RECT_MAX_NUM];
	Hobject m_DefectRegion;
	Hobject m_InspectRegion;

	void DispGlassDat();
	void MakeGlassDat(BOOL disp);
	void CoordGlassToRobotAll();
	void CoordRobotToDisp(double robo_x, double robo_y, double *rx, double *ry);
	LRESULT OnImageRectMessage(WPARAM para0, LPARAM para1);
	

	typeGlassData m_GlassDat;
	
	typeRobotData m_RobotDat;
	
	typeDefectData *m_DftGlassDat_coordXY[RECIPE_SCAN_TYPE_MAX_NUM];
	typeDefectData *m_DftScanDat_scan[RECIPE_SCAN_TYPE_MAX_NUM];
	typeScanManage m_ScanManage[RECIPE_SCAN_TYPE_MAX_NUM];

	//20160405 ngh
	typeDefectData *m_DftScanData_before[RECIPE_SCAN_TYPE_MAX_NUM];
	int DefectBeforeAfterCompare(int scan_type, int beforeCnt, int afterCnt, double distance);
	int BeforeDefectCnt[RECIPE_SCAN_TYPE_MAX_NUM];
	int *DefectCompareIdx;
	int m_nCompareDefectCnt[RECIPE_SCAN_TYPE_MAX_NUM];

	//20160406 ngh
	int m_OFDefectCnt[RECIPE_SCAN_TYPE_MAX_NUM];

	typeCadData m_CvdCad[CVD_CAD_CNT];
	typeCadData m_DamCad[CVD_CAD_CNT];

	//CVD
	//20160524 ngh
	int m_nCvdDefectCnt;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	DECLARE_MESSAGE_MAP();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
