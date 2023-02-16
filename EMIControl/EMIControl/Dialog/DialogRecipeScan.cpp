// Dialog/DialogRecipeScan.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "EMIControl.h"


#include "Include/_CtrlImageRect.h"
#include "include/_FuncModuleHal.h"
#include "include/_FuncModuleBasic.h"

#include "include/Runtime.h"
#include "include/PMACComm.h"

#include "Dialog/DialogInline.h"
#include "Dialog/DialogCCD.h"
#include "Dialog/DialogRecipePanel.h"
#include "Dialog/DialogRecipeModel.h"
#include "Dialog/DialogRecipe.h"
#include "Dialog/DialogMainInfo.h"
#include "Dialog/DialogSystem.h"
#include "Dialog/DialogMotorCtrl.h"
#include "Dialog/DialogIOCtrl.h"
#include "Dialog/DialogReview.h"
#include "Dialog/DialogLogin.h"
#include "Dialog/DialogAFCtrl.h"
#include "Dialog/DialogLightCtrl.h"
#include "Dialog/DialogIPSCtrl.h"
#include "Dialog/DialogAlign.h"
#include "Dialog/DialogGlass.h"
#include "Dialog/DialogRecipeCell.h"
#include "Dialog/DialogRecipeScan.h"
#include "Dialog/DialogGlassManuCtrl.h"
#include "Dialog/DialogFilmAlign.h"
#include "Dialog/DialogLogAlarm.h"
#include "DialogLogWindow.h"

#include "EMIControlDlg.h"

#include "afxdialogex.h"


// CDialogRecipeScan 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogRecipeScan, CDialogEx)

CDialogRecipeScan::CDialogRecipeScan(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogRecipeScan::IDD, pParent)
	, m_edit_recipe_scan_start_x(0)
	, m_edit_recipe_scan_start_y(0)
	, m_edit_recipe_scan_leng(0)
	, m_edit_recipe_scan_pitch(0)
	, m_edit_recipe_scan_pixel_x(0)
	, m_edit_recipe_scan_pixel_y(0)
	, m_edit_recipe_scan_cnt(0)
	, m_edit_recipe_scan_coord_x(0)
	, m_edit_recipe_scan_coord_y(0)
	, m_edit_recipe_glass_x(0)
	, m_edit_recipe_glass_y(0)
	, m_edit_scan_idx(1)
	, m_edit_thrsh0(0)
	, m_edit_thrsh1(0)
	, m_edit_thrsh2(0)
	, m_edit_thrsh3(0)
	, m_edit_thrsh4(0)
	, m_edit_defect_size0(0)
	, m_edit_defect_size1(0)
	, m_edit_defect_size2(0)
	, m_edit_defect_size3(0)
	, m_edit_defect_size4(0)
	, m_edit_recipe_defect_size_small(0)
	, m_edit_recipe_defect_size_medium(0)
	, m_edit_recipe_defect_size_large(0)
	, m_edit_light0(0)
	, m_check_inspect_use(FALSE)
	, m_check_inspect_90degree(FALSE)
	, m_edit_rotate_theta(0)
	, m_edit_recipe_scan_overlay(0)
	, m_edit_recipe_scan_overlay2(0)
	, m_edit_recipe_scan_ezi_pos(0)
	, m_edit_nSelectDefectErrorCnt0(0)
	, m_check_bSmallSize(FALSE)
	, m_check_bMediumSize(FALSE)
	, m_check_bLargeSize(FALSE)
	, m_check_bOverflowSize(FALSE)
	, m_edit_nSelectDefectErrorCnt1(0)
	, m_edit_nSelectDefectErrorCnt2(0)
	, m_edit_nSelectDefectErrorCnt3(0)
	, m_edit_nNoise0(0)
	, m_edit_nNoise1(0)
	, m_edit_nNoise2(0)
	, m_edit_dMargeDistance(0)
	, m_edit_dTest0(0)
	, m_edit_dTest1(0)
	, m_edit_dTest2(0)
	, m_edit_dTest3(0)
	, m_edit_dTest4(0)
	, m_edit_dTest5(0)
	, m_edit_reduce_size(0)
	, m_edit_nSelectDefectErrorCnt4(0)
	, m_check_bBlackWhite(FALSE)
	, m_edit_nSelectDefectErrorCnt5(0)
	, m_edit_dMaskDefectDistance(0)
	, m_check_bMaskDefect(FALSE)
	, m_edit_OFWidth(0)
	, m_edit_OFHeight(0)
	, m_edit_dDefectCompare_Distance(0)
{
	m_iScanSeq = 0;
	m_tab_type_idx = 0;

	for(int i = 0; i < RECIPE_SCAN_TYPE_MAX_NUM; i++)
	{
		memset(&m_ScanDat[i], NULL, sizeof(typeScanData));
	}
}

CDialogRecipeScan::~CDialogRecipeScan()
{
	KillTimer(0);

	SaveRecipeScanPara(_T(RECIPE_SCAN_PATH));
}

void CDialogRecipeScan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_START_X, m_edit_recipe_scan_start_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_START_Y, m_edit_recipe_scan_start_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_SCAN_LENG, m_edit_recipe_scan_leng);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_SCAN_PITCH, m_edit_recipe_scan_pitch);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_SCAN_RESOL_X, m_edit_recipe_scan_pixel_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_SCAN_RESOL_Y, m_edit_recipe_scan_pixel_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_CNT_X, m_edit_recipe_scan_cnt);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_COORD_X, m_edit_recipe_scan_coord_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_COORD_Y, m_edit_recipe_scan_coord_y);
	DDX_Control(pDX, IDC_BTNENHCTRL_PANEL_SCAN_SET_REVIEW_POS, m_btn_panel_scan_set_review_pos);
	DDX_Control(pDX, IDC_BTNENHCTRL_PANEL_SCAN_SET_REVIEW_MOVE, m_btn_panel_scan_set_review_move);
	DDX_Control(pDX, IDC_BTNENHCTRL_PANEL_SCAN_SCANSTART, m_btn_panel_scan_scanstart);
	DDX_Control(pDX, IDC_BTNENHCTRL_PANEL_SCAN_LOAD_SCANIMAGE, m_btn_panel_scan_load_scanimage);
	DDX_Control(pDX, IDC_BTNENHCTRL_PANEL_SCAN_DRAW_COORD, m_btn_panel_scan_draw_coord);
	DDX_Text(pDX, IDC_EDIT_RECIPE_GLASS_COORD_X, m_edit_recipe_glass_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_GLASS_Y, m_edit_recipe_glass_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_IDX, m_edit_scan_idx);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_THRSH0, m_edit_thrsh0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_THRSH1, m_edit_thrsh1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_THRSH2, m_edit_thrsh2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_THRSH3, m_edit_thrsh3);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_THRSH4, m_edit_thrsh4);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_SIZE0, m_edit_defect_size0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_SIZE1, m_edit_defect_size1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_SIZE2, m_edit_defect_size2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_SIZE3, m_edit_defect_size3);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_SIZE4, m_edit_defect_size4);
	DDX_Text(pDX, IDC_EDIT_RECIPE_DEFECT_SIZE_SMALL, m_edit_recipe_defect_size_small);
	DDX_Text(pDX, IDC_EDIT_RECIPE_DEFECT_SIZE_MEDIUM, m_edit_recipe_defect_size_medium);
	DDX_Text(pDX, IDC_EDIT_RECIPE_DEFECT_SIZE_LARGE, m_edit_recipe_defect_size_large);
	DDX_Control(pDX, IDC_BTNENHCTRL_RECIPE_SCAN_TAB0, m_tab_0);
	DDX_Control(pDX, IDC_BTNENHCTRL_RECIPE_SCAN_TAB1, m_tab_1);
	DDX_Control(pDX, IDC_BTNENHCTRL_RECIPE_SCAN_TAB2, m_tab_2);
	DDX_Control(pDX, IDC_BTNENHCTRL_RECIPE_SCAN_TAB3, m_tab_3);
	DDX_Control(pDX, IDC_BTNENHCTRL_RECIPE_SCAN_TAB4, m_tab_4);
	DDX_Text(pDX, IDC_EDIT_RECIPE_DEFECT_LIGHT0, m_edit_light0);
	DDX_Check(pDX, IDC_CHECK_INSPECT_USE, m_check_inspect_use);
	DDX_Check(pDX, IDC_CHECK_INSPECT_DIRECT, m_check_inspect_90degree);
	DDX_Text(pDX, IDC_EDIT_RECIPE_ROTATE_THETA, m_edit_rotate_theta);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_SCAN_PITCH2, m_edit_recipe_scan_overlay);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_SCAN_PITCH3, m_edit_recipe_scan_overlay2);
	DDX_Text(pDX, IDC_EDIT_EZI_POS, m_edit_recipe_scan_ezi_pos);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_comboErrorSizeSelect);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_CNT0, m_edit_nSelectDefectErrorCnt0);
	DDX_Check(pDX, IDC_CHECK_DEFECT_SIZE0, m_check_bSmallSize);
	DDX_Check(pDX, IDC_CHECK_DEFECT_SIZE1, m_check_bMediumSize);
	DDX_Check(pDX, IDC_CHECK_DEFECT_SIZE2, m_check_bLargeSize);
	DDX_Check(pDX, IDC_CHECK_DEFECT_SIZE3, m_check_bOverflowSize);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_CNT1, m_edit_nSelectDefectErrorCnt1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_CNT2, m_edit_nSelectDefectErrorCnt2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_CNT3, m_edit_nSelectDefectErrorCnt3);
	DDX_Text(pDX, IDC_EDIT_RECIPE_NOISE_0, m_edit_nNoise0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_NOISE_1, m_edit_nNoise1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_NOISE_2, m_edit_nNoise2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_DISTANCE, m_edit_dMargeDistance);
	DDX_Text(pDX, IDC_EDIT_TEST_0, m_edit_dTest0);
	DDX_Text(pDX, IDC_EDIT_TEST_1, m_edit_dTest1);
	DDX_Text(pDX, IDC_EDIT_TEST_2, m_edit_dTest2);
	DDX_Text(pDX, IDC_EDIT_TEST_3, m_edit_dTest3);
	DDX_Text(pDX, IDC_EDIT_TEST_4, m_edit_dTest4);
	DDX_Text(pDX, IDC_EDIT_TEST_5, m_edit_dTest5);
	DDX_Text(pDX, IDC_EDIT_RECIPE_REDUCE, m_edit_reduce_size);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_CNT4, m_edit_nSelectDefectErrorCnt4);
	DDX_Check(pDX, IDC_CHECK_DEFECT_SIZE4, m_check_bBlackWhite);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_CNT5, m_edit_nSelectDefectErrorCnt5);
	DDX_Text(pDX, IDC_EDIT_RECIPE_DEFECT_DISTANCE, m_edit_dMaskDefectDistance);
	DDX_Check(pDX, IDC_CHECK_DEFECT_SIZE5, m_check_bMaskDefect);
	DDX_Text(pDX, IDC_EDIT_RECIPE_OF_WIDTH, m_edit_OFWidth);
	DDX_Text(pDX, IDC_EDIT_RECIPE_OF_HEIGHT, m_edit_OFHeight);
	DDX_Control(pDX, IDC_COMBO_REDUCE_SELECT, m_combo_ReduceSelect);
	DDX_Text(pDX, IDC_EDIT_RECIPE_SCAN_DEFECT_COMPAR_DISTANCE, m_edit_dDefectCompare_Distance);
}


BEGIN_MESSAGE_MAP(CDialogRecipeScan, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_MESSAGE(USER_MSG_IMG_RECT, OnImageRectMessage)
	ON_EN_CHANGE(IDC_EDIT_RECIPE_SCAN_IDX, &CDialogRecipeScan::OnChangeEditRecipeScanIdx)
	ON_BN_CLICKED(IDC_RADIO_MAIN, &CDialogRecipeScan::OnBnClickedRadioMain)
	ON_BN_CLICKED(IDC_RADIO_SUB, &CDialogRecipeScan::OnBnClickedRadioSub)
END_MESSAGE_MAP()


// CDialogRecipeScan 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDialogRecipeScan, CDialogEx)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_RECIPE_SCAN_APPLY, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlRecipeScanApply, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_RECIPE_SCAN_SAVE, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlRecipeScanSave, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_RECIPE_SCAN_CANCEL, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlRecipeScanCancel, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_PANEL_SCAN_SET_REVIEW_POS, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlPanelScanSetReviewPos, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_PANEL_SCAN_SET_REVIEW_MOVE, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlPanelScanSetReviewMove, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_PANEL_SCAN_SCANSTART, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlPanelScanScanstart, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_PANEL_SCAN_LOAD_SCANIMAGE, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlPanelScanLoadScanimage, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_PANEL_SCAN_DRAW_COORD, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlPanelScanDrawCoord, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_PANEL_SCAN_SET_LOAD_MOVE, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlPanelScanSetLoadMove, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_PANEL_SCAN_THRESH_TEST, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlPanelScanThreshTest, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_PANEL_SCAN_APPLY_TO_GLASS, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlPanelScanApplyToGlass, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_RECIPE_SCAN_TAB0, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlRecipeScanTab0, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_RECIPE_SCAN_TAB1, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlRecipeScanTab1, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_RECIPE_SCAN_TAB2, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlRecipeScanTab2, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_RECIPE_SCAN_TAB3, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlRecipeScanTab3, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_RECIPE_SCAN_TAB4, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlRecipeScanTab4, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_PANEL_SCAN_EZI_SET_POS, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlPanelScanEziSetPos, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_PANEL_SCAN_MOVE_POS, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlPanelScanMovePos, VTS_NONE)
	ON_EVENT(CDialogRecipeScan, IDC_BTNENHCTRL_WINDOW_RESIZE, DISPID_CLICK, CDialogRecipeScan::ClickBtnenhctrlWindowResize, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogRecipeScan::ClickBtnenhctrlRecipeScanApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ParameterCompare(m_tab_type_idx);
	Update(TRUE);
}


void CDialogRecipeScan::ClickBtnenhctrlRecipeScanSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CDialogRecipeModel mod_dlg;
	mod_dlg.SetOpen();
	UpdateData(TRUE);
	ParameterCompare(m_tab_type_idx);
	Update(TRUE);
	SaveRecipeScanPara(_T(RECIPE_SCAN_PATH));
	SaveRecipeScanPara(mod_dlg.GetRecipePath());
}


void CDialogRecipeScan::ClickBtnenhctrlRecipeScanCancel()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ShowWindow(SW_HIDE);
}

BOOL CDialogRecipeScan::SetLoadRecipeScanPara(CString path)
{
	return LoadRecipeScanPara(path);
}

BOOL CDialogRecipeScan::LoadRecipeScanPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	char path_temp[MAX_PATH];
	size_t getval;
	getval = 0;
	int i, idx;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;


	file_path = path + _T("\\RecipeScan.par");

	//file 존재 확인 
	HANDLE fd = CreateFile( file_path,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);
	if(((LONG)fd)<0)
	{
		rslt = FALSE;
	}
	else
	{
		CloseHandle(fd);

		for(i = 0; i < RECIPE_SCAN_TYPE_MAX_NUM; i++)
		{
			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_start_x_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dScan_start_x = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_start_y_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dScan_start_y = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_leng_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dScan_leng = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_pitch_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dScan_pitch = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_overlay_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dScan_overlay = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_overlay2_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dScan_overlay2 = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_pixel_x_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dScan_pixel_x = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_pixel_y_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dScan_pixel_y = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_cnt_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dScan_cnt = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_coord_x_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dScan_coord_x = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_coord_y_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dScan_coord_y = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_glass_x_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dScan_glass_x = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_glass_y_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dScan_glass_y = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dThrsh0_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dThrsh0 = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dThrsh1_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dThrsh1 = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dThrsh2_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dThrsh2 = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dThrsh3_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dThrsh3 = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dThrsh4_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dThrsh4 = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dDefect_size0_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dDefect_size0 = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dDefect_size1_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dDefect_size1 = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dDefect_size2_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dDefect_size2 = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dDefect_size3_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dDefect_size3 = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dDefect_size4_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dDefect_size4 = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dDefect_size_small_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dDefect_size_small = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dDefect_size_medium_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dDefect_size_medium = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dDefect_size_large_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dDefect_size_large = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dlight0_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dLight0 = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_bInspectUse_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_bInspectUse = (BOOL)(atof(ctemp));

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_bInspect90Degree_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_bInspect90Degree = (BOOL)(atof(ctemp));

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScanRotateOffset_%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dScanRotateOffset = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScanEziPos%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_dScanEziPos = atof(ctemp);
			
			for(int j = 0; j < MAX_DEFECT_SIZE_IDX; j++)
			{
				key.Empty();
				key.Format(_T("m_ScanDat[i].m_ErrorSizeSelect%d_%d"), i, j);
				GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
				dat = temp;
				wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
				m_ScanDat[i].m_ErrorSizeSelect[j] = atoi(ctemp);

				key.Empty();
				key.Format(_T("m_ScanDat[i].m_SelectDefectErrorCnt%d_%d"), i, j);
				GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
				dat = temp;
				wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
				m_ScanDat[i].m_SelectDefectErrorCnt[j] = atoi(ctemp);
			}

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_BlackWhiteErrorSelect%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_bErrorBlackWhite = atoi(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_SelectBlackWhiteDefectErrorCnt%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_BlackWhiteCnt = atoi(ctemp);

			for(int j = 0; j < MAX_NOISE_SIZE_INDEX; j++)
			{
				key.Empty();
				key.Format(_T("m_ScanDat[i].m_NoiseSize%d_%d"), i, j);
				GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
				dat = temp;
				wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
				m_ScanDat[i].m_NoiseSize[j] = atoi(ctemp);
			}

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_MargeDistance%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_MargeDistance = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_ReduceSelect%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_ReduceSelect = atoi(ctemp);
			m_combo_ReduceSelect.SetCurSel(m_ScanDat[i].m_ReduceSelect);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_ReduceSize%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_ReduceSize = atof(ctemp);


			key.Empty();
			key.Format(_T("m_ScanDat[i].m_bMaskDefect%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_bMaskDefect = atoi(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_MaskDefectDistance%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_MaskDefectDistance = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_MaskDefectCnt%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_MaskDefectCnt = atoi(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_OFDefectSizeWidth%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_OFDefectSizeWidth = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_OFDefectSizeHeight%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_OFDefectSizeHeight = atof(ctemp);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_edit_dDefectCompare_Distance%d"), i);
			GetPrivateProfileString(_T("RECIPE SCAN"), key, _T("0.0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_ScanDat[i].m_edit_dDefectCompare_Distance = atof(ctemp);

			//;
		}

	}
	fd=0;

	return rslt;
}

BOOL CDialogRecipeScan::SetSaveRecipeScanPara(CString path)
{
	return SaveRecipeScanPara(path);
}

BOOL CDialogRecipeScan::SaveRecipeScanPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	char path_temp[MAX_PATH];
	size_t getval;
	getval = 0;
	int i, idx;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	
	file_path = path + _T("\\RecipeScan.par");

	//file 존재 확인 
	HANDLE fd=CreateFile(   file_path,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);
	if(((LONG)fd)<0)
	{
		rslt = FALSE;
	}
	else
	{
		CloseHandle(fd);

		for(i = 0; i < RECIPE_SCAN_TYPE_MAX_NUM; i++)
		{
			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_start_x_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dScan_start_x);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_start_y_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dScan_start_y);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_leng_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dScan_leng);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_pitch_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dScan_pitch);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_overlay_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dScan_overlay);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_overlay2_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dScan_overlay2);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_pixel_x_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dScan_pixel_x);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_pixel_y_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dScan_pixel_y);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_cnt_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dScan_cnt);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_coord_x_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dScan_coord_x);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_coord_y_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dScan_coord_y);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_glass_x_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dScan_glass_x);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScan_glass_y_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dScan_glass_y);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dThrsh0_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dThrsh0);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dThrsh1_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dThrsh1);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dThrsh2_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dThrsh2);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dThrsh3_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dThrsh3);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dThrsh4_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dThrsh4);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dDefect_size0_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dDefect_size0);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dDefect_size1_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dDefect_size1);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dDefect_size2_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dDefect_size2);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dDefect_size3_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dDefect_size3);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dDefect_size4_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dDefect_size4);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dDefect_size_small_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dDefect_size_small);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dDefect_size_medium_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dDefect_size_medium);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dDefect_size_large_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dDefect_size_large);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);
	
			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dlight0_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dLight0);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);
	
			key.Empty();
			key.Format(_T("m_ScanDat[i].m_bInspectUse_%d"), i);
			dat.Empty();
			dat.Format(_T("%d"), m_ScanDat[i].m_bInspectUse);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_bInspect90Degree_%d"), i);
			dat.Empty();
			dat.Format(_T("%d"), m_ScanDat[i].m_bInspect90Degree);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScanRotateOffset_%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dScanRotateOffset);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_dScanEziPos%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_dScanEziPos);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			for(int j = 0; j < MAX_DEFECT_SIZE_IDX; j++)
			{
				key.Empty();
				key.Format(_T("m_ScanDat[i].m_ErrorSizeSelect%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_ScanDat[i].m_ErrorSizeSelect[j]);
				WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

				key.Empty();
				key.Format(_T("m_ScanDat[i].m_SelectDefectErrorCnt%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_ScanDat[i].m_SelectDefectErrorCnt[j]);
				WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);
			}

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_BlackWhiteErrorSelect%d"), i);
			dat.Empty();
			dat.Format(_T("%d"), m_ScanDat[i].m_bErrorBlackWhite);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_SelectBlackWhiteDefectErrorCnt%d"), i);
			dat.Empty();
			dat.Format(_T("%d"), m_ScanDat[i].m_BlackWhiteCnt);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			for(int j = 0; j < MAX_NOISE_SIZE_INDEX; j++)
			{
				key.Empty();
				key.Format(_T("m_ScanDat[i].m_NoiseSize%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_ScanDat[i].m_NoiseSize[j]);
				WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);
			}

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_MargeDistance%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_MargeDistance);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_ReduceSelect%d"), i);
			dat.Empty();
			dat.Format(_T("%d"), m_ScanDat[i].m_ReduceSelect);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_ReduceSize%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_ReduceSize);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_bMaskDefect%d"), i);
			dat.Empty();
			dat.Format(_T("%d"), m_ScanDat[i].m_bMaskDefect);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_MaskDefectDistance%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_MaskDefectDistance);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_MaskDefectCnt%d"), i);
			dat.Empty();
			dat.Format(_T("%d"), m_ScanDat[i].m_MaskDefectCnt);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_OFDefectSizeWidth%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_OFDefectSizeWidth);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_OFDefectSizeHeight%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_OFDefectSizeHeight);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_ScanDat[i].m_edit_dDefectCompare_Distance%d"), i);
			dat.Empty();
			dat.Format(_T("%f"), m_ScanDat[i].m_edit_dDefectCompare_Distance);
			WritePrivateProfileString(_T("RECIPE SCAN"), key, dat, file_path);
		}
	
	}
	fd=0;
	
	return rslt;
}

BOOL CDialogRecipeScan::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Herror herr;	
	CRect rect;
	int i;
	HTuple wd, ht;
	long width, height;
	
	for(i = 0; i < RECIPE_SCAN_IMG_RECT_MAX_NUM; i++)
	{
		m_dlgImgRect[i] = new C_CtrlImageRect;
		m_dlgImgRect[i]->Create(IMAGE_RECT_DIALOG, this);

		switch(i)
		{
		case 0:
		case 1:
			GetDlgItem(IDC_STATIC_SCAN)->GetClientRect(rect);
			GetDlgItem(IDC_STATIC_SCAN)->ClientToScreen(rect);

			m_dlgImgRect[i]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);
			m_dlgImgRect[i]->SetCamIndex(i, _T("                                                            8K SCAN WINDOW"), this->m_hWnd);
			break;
		case 2:
#ifdef PCL_TEST
		case 3:
		case 4:
#endif
			GetDlgItem(IDC_STATIC_SCAN)->GetClientRect(rect);
			GetDlgItem(IDC_STATIC_SCAN)->ClientToScreen(rect);

			m_dlgImgRect[i]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);
			m_dlgImgRect[i]->SetCamIndex(i, _T("                                                            16K SCAN WINDOW"), this->m_hWnd);
			break;
		default:
			AfxMessageBox(_T("Error : invalid index(CDialogRecipeScan::OnInitDialog())!"));
			break;
		}

		m_dlgImgRect[i]->ShowWindow(SW_SHOW);

	}

	//Halcon image initialize
	for(i = 0; i < RECIPE_SCAN_IMG_RECT_MAX_NUM; i++)
	{
		switch(i)
		{
		case 0:
		case 1:
			gen_image_const(&m_HalImage[i], HTuple("byte"), RECIPE_SCAN_SCAN_WD, RECIPE_SCAN_SCAN_HT);
			break;
		case 2:
#ifdef PCL_TEST
		case 3:
		case 4:
#endif
			gen_image_const(&m_HalImage[i], HTuple("byte"), RECIPE_SCAN_SCAN_16K_WD, RECIPE_SCAN_SCAN_16K_HT);
			break;
		default:
			AfxMessageBox(_T("Error : invalid index(CDialogRecipeScan::OnInitDialog())!"));
			break;
		}


		set_check("~give_error");
			herr = get_image_pointer1(m_HalImage[i], NULL, NULL, &wd, &ht);
		set_check("give_error");

		if(H_MSG_TRUE == herr)
		{
		}
		else
		{
			AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1_00!"));
			return FALSE;
		}			

		width = (Hlong)wd[0];
		height = (Hlong)ht[0];
	
		switch(i)
		{
		case 0:
		case 1:
			m_dlgImgRect[i]->SetZoomRatio(2.);
			m_dlgImgRect[i]->SetZoom(0, (long)width, (long)(height * RECIPE_SCAN_ZOOM_HEIGHT_MULT));
			break;
		case 2:
#ifdef PCL_TEST
		case 3:
		case 4:
#endif
			m_dlgImgRect[i]->SetZoomRatio(20.);
			m_dlgImgRect[i]->SetZoom(0, (long)width, (long)(height * RECIPE_SCAN_ZOOM_HEIGHT_MULT));
			break;
		default:
			AfxMessageBox(_T("Error : invalid index(!"));
		}



		set_font(m_dlgImgRect[i]->GetView(), "-Arial-12-*-*-*-*-*-1-");
		set_draw(m_dlgImgRect[i]->GetView(), HTuple("margin"));

		disp_obj(m_HalImage[i], m_dlgImgRect[i]->GetView());	
		m_combo_ReduceSelect.SetCurSel(0);

		m_dlgImgRect[i]->ShowWindow(SW_HIDE);
	}
	
	LoadRecipeScanPara(_T(RECIPE_SCAN_PATH));

	Update(FALSE);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogRecipeScan::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int i;
	CRect rect;

	if(bShow)
	{
		GetDlgItem(IDC_STATIC_SCAN)->GetClientRect(rect);
		GetDlgItem(IDC_STATIC_SCAN)->ClientToScreen(rect);

		m_dlgImgRect[m_tab_type_idx]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);

		m_dlgImgRect[m_tab_type_idx]->ShowWindow(SW_SHOW);

		Update(FALSE);
		UpdateData(FALSE);
	}
	else
	{
		for(int i = 0; i < RECIPE_SCAN_IMG_RECT_MAX_NUM; i++)
		{
			m_dlgImgRect[i]->ShowWindow(SW_HIDE);
		}

		KillTimer(0);
	}
}








double CDialogRecipeScan::GetScanStartX(int type_idx)
{
	return  m_ScanDat[type_idx].m_dScan_start_x;
}

double CDialogRecipeScan::GetScanStartY(int type_idx)
{
	return  m_ScanDat[type_idx].m_dScan_start_y;
}

double CDialogRecipeScan::GetScanLeng(int type_idx)
{
	return  m_ScanDat[type_idx].m_dScan_leng;
}

double CDialogRecipeScan::GetScanPitch(int type_idx)
{
	return  m_ScanDat[type_idx].m_dScan_pitch;
}

double CDialogRecipeScan::GetScanPixelX(int type_idx)
{
	return  m_ScanDat[type_idx].m_dScan_pixel_x;
}

double CDialogRecipeScan::GetScanPixelY(int type_idx)
{
	return  m_ScanDat[type_idx].m_dScan_pixel_y;
}

double CDialogRecipeScan::GetScanCnt(int type_idx)
{
	return  m_ScanDat[type_idx].m_dScan_cnt;
}

double CDialogRecipeScan::GetScanImageX(int type_idx)
{
	return  m_ScanDat[type_idx].m_dScan_coord_x;
}

double CDialogRecipeScan::GetScanImageY(int type_idx)
{
	return  m_ScanDat[type_idx].m_dScan_coord_y;
}

double CDialogRecipeScan::GetScanGlassX(int type_idx)
{
	return  m_ScanDat[type_idx].m_dScan_glass_x;
}

double CDialogRecipeScan::GetScanGlassY(int type_idx)
{
	return  m_ScanDat[type_idx].m_dScan_glass_y;
}

double CDialogRecipeScan::GetScanStartX_coordXY(int type_idx)
{
	//return m_edit_recipe_scan_start_y;
	return  m_ScanDat[type_idx].m_dScan_start_y;
}

double CDialogRecipeScan::GetScanStartY_coordXY(int type_idx)
{
	//return m_edit_recipe_scan_start_x;
	return  m_ScanDat[type_idx].m_dScan_start_x;
}

double CDialogRecipeScan::GetScanLeng_coordXY(int type_idx)
{
	//return m_edit_recipe_scan_leng;
	return  m_ScanDat[type_idx].m_dScan_leng;
}

double CDialogRecipeScan::GetScanPitch_coordXY(int type_idx)
{
	//return m_edit_recipe_scan_pitch;
	return  m_ScanDat[type_idx].m_dScan_pitch;
}

double CDialogRecipeScan::GetScanOverlay(int type_idx, int type)
{
	//return m_edit_recipe_scan_pitch;
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_ScanDat[type_idx].m_dScan_overlay;
		break;
	case 1:
	case 2:
		rslt = m_ScanDat[type_idx].m_dScan_overlay2;
		break;
	}
	return rslt;
	/*if(type == 0)
	{
		return  m_ScanDat[type_idx].m_dScan_overlay;
	}
	else if(type == 1)
	{
		return m_ScanDat[type_idx].m_dScan_overlay2;
	}*/
}

double CDialogRecipeScan::GetScanPixelX_coordXY(int type_idx)
{
	//return m_edit_recipe_scan_pixel_y;
	return  m_ScanDat[type_idx].m_dScan_pixel_y;
}

double CDialogRecipeScan::GetScanPixelY_coordXY(int type_idx)
{
	//return m_edit_recipe_scan_pixel_x;
	return  m_ScanDat[type_idx].m_dScan_pixel_x;
}

double CDialogRecipeScan::GetScanCnt_coordXY(int type_idx)
{
	//return m_edit_recipe_scan_cnt;
	return  m_ScanDat[type_idx].m_dScan_cnt;
}

double CDialogRecipeScan::GetScanImageX_coordXY(int type_idx)
{
	//return m_edit_recipe_scan_coord_x;
	return  m_ScanDat[type_idx].m_dScan_coord_x;
}

double CDialogRecipeScan::GetScanImageY_coordXY(int type_idx)
{
	//return m_edit_recipe_scan_coord_y;
	return  m_ScanDat[type_idx].m_dScan_coord_y;
}

double CDialogRecipeScan::GetScanGlassX_coordXY(int type_idx)
{
	//return m_edit_recipe_scan_glass_y;
	return  m_ScanDat[type_idx].m_dScan_glass_y;
}

double CDialogRecipeScan::GetScanGlassY_coordXY(int type_idx)
{
	//return m_edit_recipe_scan_glass_x;
	return  m_ScanDat[type_idx].m_dScan_glass_x;
}

double CDialogRecipeScan::GetEzPos(int type_idx)
{
	return m_ScanDat[type_idx].m_dScanEziPos;
}

void CDialogRecipeScan::ClickBtnenhctrlPanelScanSetLoadMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, pdlg->m_Align->GetAlignOffsetPosX_coordXY() - pdlg->m_RecipeScan->GetScanStartX_coordXY(m_tab_type_idx));
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, pdlg->m_Align->GetAlignOffsetPosY_coordXY() + pdlg->m_RecipeScan->GetScanStartY_coordXY(m_tab_type_idx));
}

void CDialogRecipeScan::ClickBtnenhctrlPanelScanSetReviewPos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}


void CDialogRecipeScan::ClickBtnenhctrlPanelScanSetReviewMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}


void CDialogRecipeScan::ClickBtnenhctrlPanelScanScanstart()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strLog;
	int nCamIdx, nRotateIdx, nScanIdx;
	if(0 == m_iScanSeq)
	{
		UpdateData(TRUE);
		switch(m_tab_type_idx)
		{
		case 0:
			nCamIdx = 0;
			nRotateIdx = 0;
			break;
		case 1:
			nCamIdx = 0;
			nRotateIdx = 1;
			break;
		case 2:
			nCamIdx = 1;
			nRotateIdx = m_check_inspect_90degree;
			break;
		case 3:
			nCamIdx = 1;
			nRotateIdx = 1;
			break;
		case 4:
			nCamIdx = 1;
			nRotateIdx = 0;
			break;
		}
		nScanIdx = m_edit_scan_idx - 1;
		strLog.Format(_T("%d Cam %d Degree %d Scan Test Grab Start"), nCamIdx, nRotateIdx, nScanIdx);
		pdlg->WriteLog(MAIN_LOG, strLog);
		SetTimer(0, 500, NULL);
	}
	else
	{
		KillTimer(0);
		m_iScanSeq = 0;
		AfxMessageBox(_T("Scan중입니다. scan을 중지하겠습니다!"));
	}
}


void CDialogRecipeScan::ClickBtnenhctrlPanelScanLoadScanimage()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	HTuple wd, ht;
	Herror herr;
	CString status;
	int cam_idx = 0;

	Hal_OpenHalImage(&m_HalImage[cam_idx], CString(""), FALSE);

	set_check("~give_error");
	herr = get_image_pointer1(m_HalImage[cam_idx], NULL, NULL, &wd, &ht);
	set_check("give_error");
	if(H_MSG_TRUE == herr)
	{
	}
	else
	{
		AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1!"));
		return;
	}

	disp_obj(m_HalImage[m_tab_type_idx], m_dlgImgRect[m_tab_type_idx]->GetView());
	m_dlgImgRect[m_tab_type_idx]->Display();
}

void CDialogRecipeScan::DispGlassDat()
{
	MakeGlassDat(m_tab_type_idx, m_edit_scan_idx - 1, TRUE);
}

void CDialogRecipeScan::MakeGlassDat(int type_idx, int scan_idx, BOOL disp)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	double rot_x, rot_y;
	double sx, sy, ex, ey;
	double size;
	double dist_inter_mark_x, dist_inter_mark_y;
	long l_sx, l_sy, l_ex, l_ey;
	int cam_idx, mark_idx, i, j;
	double x, y;
	int idx, grp_idx_x, grp_idx_y;

	double up_limit_ht, dn_limit_ht;
	CRect rect, rslt_rect, tot_rect;

	int rect_num = 0;
	int ins_type = 0;

	cam_idx = 0;

	if(disp)
	{
		clear_window(m_dlgImgRect[type_idx]->GetView());
		disp_obj(m_HalImage[type_idx], m_dlgImgRect[type_idx]->GetView());
	}

	//Draw cell Align mark///////////////////////////////////////////////////////////////////////////////////////
	cam_idx = 0;
	mark_idx = 0;
	size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

	idx = 0;
	if(pdlg->m_RecipePanel->GetCellCntX_coordXY() > pdlg->m_RecipePanel->GetCellGrpCntX_coordXY())
	{
		grp_idx_x = (int)pdlg->m_RecipePanel->GetCellGrpCntX_coordXY();
	}
	else
	{
		grp_idx_x = 0;
	}
	if(pdlg->m_RecipePanel->GetCellCntY_coordXY() > pdlg->m_RecipePanel->GetCellGrpCntY_coordXY())
	{
		grp_idx_y = (int)pdlg->m_RecipePanel->GetCellGrpCntY_coordXY();
	}
	else
	{
		grp_idx_y = 0;
	}

	x = 0;
	y = 0;

	for(i = 0; i < pdlg->m_RecipePanel->GetCellCntY_coordXY(); i++)
	{
		if(0 == i)
		{
			y = pdlg->m_RecipePanel->GetMarkPosY_coordXY(mark_idx) + pdlg->m_RecipePanel->GetCellFirstPosY_coordXY();
		}
		else if(i == grp_idx_y)
		{
			y =  y + pdlg->m_RecipePanel->GetCellGrpPitchY_coordXY();
		}
		else
		{
			y =  y + pdlg->m_RecipePanel->GetCellPitchY_coordXY();
		}

		for(j = 0; j < pdlg->m_RecipePanel->GetCellCntX_coordXY();j++)
		{
			if(GLASS_MAX_CELL_NUM <= idx)
			{
				break;
			}

			if(0 == j)
			{
				x = pdlg->m_RecipePanel->GetMarkPosX_coordXY(mark_idx) + pdlg->m_RecipePanel->GetCellFirstPosX_coordXY();
			}
			else if(j == grp_idx_x)
			{
				x =  x + pdlg->m_RecipePanel->GetCellGrpPitchX_coordXY();
			}
			else
			{
				x =  x + pdlg->m_RecipePanel->GetCellPitchX_coordXY();
			}

			if(0 > scan_idx)
			{
				scan_idx = 0;
			}

			//Horz
			if(disp)
			{
				//Rotate 90
				if(m_ScanDat[type_idx].m_bInspect90Degree)
				{
					RotatedPt(x, y,	0., 0.,	90., &rot_x, &rot_y);
					rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
				}
				else
				{
					rot_x = x;
					rot_y = y;
				}

				GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x - size, rot_y, &sx, &sy);
				GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x + size, rot_y, &ex, &ey);

				l_sx = (sx + 0.5);// + GetReduceSize(type_idx);
				l_sy = (sy + 0.5);// - GetReduceSize(type_idx);
				l_ex = (ex + 0.5);// + GetReduceSize(type_idx);
				l_ey = (ey + 0.5);// - GetReduceSize(type_idx);
				set_color(m_dlgImgRect[type_idx]->GetView(), "blue");
				set_draw(m_dlgImgRect[type_idx]->GetView(), HTuple("margin"));
				disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);
			}
			//Vert
			if(disp)
			{
				//Rotate 90
				if(m_ScanDat[type_idx].m_bInspect90Degree)
				{
					RotatedPt(x, y,	0., 0.,	90., &rot_x, &rot_y);
					rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
				}
				else
				{
					rot_x = x;
					rot_y = y;
				}

				GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y - size, &sx, &sy);
				GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y + size, &ex, &ey);

				l_sx = (sx + 0.5);// + GetReduceSize(type_idx);
				l_sy = (sy + 0.5);//- GetReduceSize(type_idx);
				l_ex = (ex + 0.5);// + GetReduceSize(type_idx);
				l_ey = (ey + 0.5);// - GetReduceSize(type_idx);
				set_color(m_dlgImgRect[type_idx]->GetView(), "blue");
				set_draw(m_dlgImgRect[type_idx]->GetView(), HTuple("margin"));
				disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);
			}
			idx++;

		}
	}

	//Draw cell active area////////////////////////////////////////////////////////////////////////////
	cam_idx = 0;
	mark_idx = 0;
	size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

	double l = 0.,t = 0.,r = 0.,b = 0.;
	switch(GetReduceSelect(type_idx))
	{
	case 0://L
		l = GetReduceSize(type_idx) / GetScanPixelX(type_idx);
		break;
	case 1://T
		t = GetReduceSize(type_idx) / GetScanPixelY(type_idx);
		break;
	case 2://R
		r = GetReduceSize(type_idx) / GetScanPixelX(type_idx);
		break;
	case 3://B
		b = GetReduceSize(type_idx) / GetScanPixelY(type_idx);
		break;
	case 4://LR
		l = GetReduceSize(type_idx) / GetScanPixelX(type_idx);
		r = GetReduceSize(type_idx) / GetScanPixelX(type_idx);
		break;
	case 5://TB
		t = GetReduceSize(type_idx) / GetScanPixelY(type_idx);
		b = GetReduceSize(type_idx) / GetScanPixelY(type_idx);
		break;
	case 6://LT
		l = GetReduceSize(type_idx) / GetScanPixelX(type_idx);
		t = GetReduceSize(type_idx) / GetScanPixelY(type_idx);
		break;
	case 7://RB
		r = GetReduceSize(type_idx) / GetScanPixelX(type_idx);
		b = GetReduceSize(type_idx) / GetScanPixelY(type_idx);
		break;
	case 8://LB
		l = GetReduceSize(type_idx) / GetScanPixelX(type_idx);
		b = GetReduceSize(type_idx) / GetScanPixelY(type_idx);
		break;
	case 9://RT
		r = GetReduceSize(type_idx) / GetScanPixelX(type_idx);
		t = GetReduceSize(type_idx) / GetScanPixelY(type_idx);
		break;
	case 10://LTR
		l = GetReduceSize(type_idx) / GetScanPixelX(type_idx);
		t = GetReduceSize(type_idx) / GetScanPixelY(type_idx);
		r = GetReduceSize(type_idx) / GetScanPixelX(type_idx);
		break;
	case 11://LBR
		l = GetReduceSize(type_idx) / GetScanPixelX(type_idx);
		b = GetReduceSize(type_idx) / GetScanPixelY(type_idx);
		r = GetReduceSize(type_idx) / GetScanPixelX(type_idx);
		break;
	case 12://TLB
		t = GetReduceSize(type_idx) / GetScanPixelY(type_idx);
		l = GetReduceSize(type_idx) / GetScanPixelX(type_idx);
		b = GetReduceSize(type_idx) / GetScanPixelY(type_idx);
		break;
	case 13://TRB
		t = GetReduceSize(type_idx) / GetScanPixelY(type_idx);
		r = GetReduceSize(type_idx) / GetScanPixelX(type_idx);
		b = GetReduceSize(type_idx) / GetScanPixelY(type_idx);
		break;
	case 14://LTRB
		l = GetReduceSize(type_idx) / GetScanPixelX(type_idx);
		t = GetReduceSize(type_idx) / GetScanPixelY(type_idx);
		r = GetReduceSize(type_idx) / GetScanPixelX(type_idx);
		b = GetReduceSize(type_idx) / GetScanPixelY(type_idx);
		break;
	}

	idx = 0;
	rect_num = 0;
	for(i = 0; i < pdlg->m_RecipePanel->GetCellCntY_coordXY(); i++)
	{
		for(j = 0; j < pdlg->m_RecipePanel->GetCellCntX_coordXY();j++)
		{
			if(GLASS_MAX_CELL_NUM <= idx)
			{
				break;
			}

			//Rotate 90
			if(m_ScanDat[type_idx].m_bInspect90Degree)
			{
				RotatedPt(pdlg->m_Glass->GetCellActiveRectSX_coordXY(idx), pdlg->m_Glass->GetCellActiveRectSY_coordXY(idx),	0., 0.,	90., &rot_x, &rot_y);
				rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
			}
			else
			{
				rot_x = pdlg->m_Glass->GetCellActiveRectSX_coordXY(idx);
				rot_y = pdlg->m_Glass->GetCellActiveRectSY_coordXY(idx);
			}
			GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y, &sx, &sy);

			//Rotate 90
			if(m_ScanDat[type_idx].m_bInspect90Degree)
			{
				RotatedPt(pdlg->m_Glass->GetCellActiveRectEX_coordXY(idx), pdlg->m_Glass->GetCellActiveRectEY_coordXY(idx),	0., 0.,	90., &rot_x, &rot_y);
				rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
			}
			else
			{
				rot_x = pdlg->m_Glass->GetCellActiveRectEX_coordXY(idx);
				rot_y = pdlg->m_Glass->GetCellActiveRectEY_coordXY(idx);
			}
			GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y, &ex, &ey);

			//Image 안에 있는지 확인, 위아래는 %씩 뺀다(overlay)
			switch(type_idx)
			{
			case 0:
			case 1:
				up_limit_ht = RECIPE_SCAN_SCAN_HT * (0.01 * GetScanOverlay(type_idx, 0));
				dn_limit_ht = RECIPE_SCAN_SCAN_HT - (RECIPE_SCAN_SCAN_HT * (0.01 * GetScanOverlay(type_idx, 1)));

				tot_rect.left = 0;
				tot_rect.top = up_limit_ht;
				tot_rect.right = RECIPE_SCAN_SCAN_WD;
				tot_rect.bottom = dn_limit_ht;
				break;
			case 2:
			case 3:
			case 4:
				up_limit_ht = RECIPE_SCAN_SCAN_16K_HT * (0.01 * GetScanOverlay(type_idx, 0));
				dn_limit_ht = RECIPE_SCAN_SCAN_16K_HT - up_limit_ht;

				tot_rect.left = 0;
				tot_rect.top = up_limit_ht;
				tot_rect.right = RECIPE_SCAN_SCAN_16K_WD;
				tot_rect.bottom = dn_limit_ht;
				break;
			default:
				AfxMessageBox(_T("Error : invalid index(CDialogRecipeScan::MakeGlassDat)!"));
				break;
			}

			rect.left = sx;
			rect.top = sy;
			rect.right = ex;
			rect.bottom = ey;
			rect.NormalizeRect();

			if(rslt_rect.IntersectRect(rect, tot_rect))
			{
				//20160323 ngh GetReduceSize() 추가
				m_ScanDat[type_idx].m_dCellActiveRectSX[scan_idx][rect_num] = rslt_rect.left + l + 0.5;
				m_ScanDat[type_idx].m_dCellActiveRectSY[scan_idx][rect_num] = rslt_rect.top + t + 0.5;
				m_ScanDat[type_idx].m_dCellActiveRectEX[scan_idx][rect_num] = rslt_rect.right - r - 0.5;
				m_ScanDat[type_idx].m_dCellActiveRectEY[scan_idx][rect_num] = rslt_rect.bottom - b - 0.5;

				rect_num++;
				m_ScanDat[type_idx].m_iScanRectNum[scan_idx] = rect_num;

				l_sx = rslt_rect.left + l + 0.5;
				l_sy = rslt_rect.top + t + 0.5;
				l_ex = rslt_rect.right - r - 0.5;
				l_ey = rslt_rect.bottom - b - 0.5;
				if(disp)
				{
					set_color(m_dlgImgRect[type_idx]->GetView(), "blue");
					set_draw(m_dlgImgRect[type_idx]->GetView(), HTuple("margin"));
					disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
					disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
					disp_line(m_dlgImgRect[type_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
					disp_line(m_dlgImgRect[type_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);
				}
			}

			idx++;
		}
	}

	if(2 == type_idx)
	{
		//Draw CVD align mark////////////////////////////////////////////////////////////////////////////////////
		cam_idx = 0;
		mark_idx = 0;
		rect_num = 0;
		size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

		idx = 0;
		if(pdlg->m_RecipeCell->GetCVDCntX_coordXY() > pdlg->m_RecipeCell->GetCVDGrpCntX_coordXY())
		{
			grp_idx_x = (int)pdlg->m_RecipeCell->GetCVDGrpCntX_coordXY();
		}
		else
		{
			grp_idx_x = 0;
		}
		if(pdlg->m_RecipeCell->GetCVDCntY_coordXY() > pdlg->m_RecipeCell->GetCVDGrpCntY_coordXY())
		{
			grp_idx_y = (int)pdlg->m_RecipeCell->GetCVDGrpCntY_coordXY();
		}
		else
		{
			grp_idx_y = 0;
		}

		x = 0;
		y = 0;
		for(i = 0; i < pdlg->m_RecipeCell->GetCVDCntY_coordXY(); i++)
		{
			if(0 == i)
			{
				y = pdlg->m_RecipePanel->GetMarkPosY_coordXY(mark_idx) + pdlg->m_RecipePanel->GetCVDFirstPosY_coordXY();
			}
			else if(i == grp_idx_y)
			{
				y =  y + pdlg->m_RecipeCell->GetCVDGrpPitchY_coordXY();
			}
			else
			{
				y =  y + pdlg->m_RecipeCell->GetCVDPitchY_coordXY();
			}

			for(j = 0; j < pdlg->m_RecipeCell->GetCVDCntX_coordXY();j++)
			{
				if(GLASS_MAX_CELL_NUM <= idx)
				{
					break;
				}

				if(0 == j)
				{
					x = pdlg->m_RecipePanel->GetMarkPosX_coordXY(mark_idx) + pdlg->m_RecipePanel->GetCVDFirstPosX_coordXY();
				}
				else if(j == grp_idx_y)
				{
					x =  x + pdlg->m_RecipeCell->GetCVDGrpPitchX_coordXY();
				}
				else
				{
					x =  x + pdlg->m_RecipeCell->GetCVDPitchX_coordXY();
				}


				if(0 > scan_idx)
				{
					scan_idx = 0;
				}


				//Horz
				if(disp)
				{
					//Rotate 90
					if(m_ScanDat[type_idx].m_bInspect90Degree)
					{
						RotatedPt(x, y,	0., 0.,	90., &rot_x, &rot_y);
						rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
					}
					else
					{
						rot_x = x;
						rot_y = y;
					}

					GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x - size, rot_y, &sx, &sy);
					GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x + size, rot_y, &ex, &ey);

					l_sx = sx + 0.5;
					l_sy = sy + 0.5;
					l_ex = ex + 0.5;
					l_ey = ey + 0.5;
					set_color(m_dlgImgRect[type_idx]->GetView(), "yellow");
					set_draw(m_dlgImgRect[type_idx]->GetView(), HTuple("margin"));
					disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);
				}
				//Vert
				if(disp)
				{
					//Rotate 90
					if(m_ScanDat[type_idx].m_bInspect90Degree)
					{
						RotatedPt(x, y,	0., 0.,	90., &rot_x, &rot_y);
						rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
					}
					else
					{
						rot_x = x;
						rot_y = y;
					}

					GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y - size, &sx, &sy);
					GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y + size, &ex, &ey);

					l_sx = sx + 0.5;
					l_sy = sy + 0.5;
					l_ex = ex + 0.5;
					l_ey = ey + 0.5;
					set_color(m_dlgImgRect[type_idx]->GetView(), "yellow");
					set_draw(m_dlgImgRect[type_idx]->GetView(), HTuple("margin"));
					disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);
				}
				idx++;
			}
		}

		//Draw cvd active area////////////////////////////////////////////////////////////////////////////
		cam_idx = 0;
		mark_idx = 0;
		rect_num = 0;
		size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

		idx = 0;
		for(i = 0; i < pdlg->m_RecipeCell->GetCVDCntY_coordXY(); i++)
		{
			for(j = 0; j < pdlg->m_RecipeCell->GetCVDCntX_coordXY();j++)
			{
				if(GLASS_MAX_CELL_NUM <= idx)
				{
					break;
				}

				//Rotate 90
				if(m_ScanDat[type_idx].m_bInspect90Degree)
				{
					RotatedPt(pdlg->m_Glass->GetCVDActiveRectSX_coordXY(idx), pdlg->m_Glass->GetCVDActiveRectSY_coordXY(idx),	0., 0.,	90., &rot_x, &rot_y);
					rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
				}
				else
				{
					rot_x = pdlg->m_Glass->GetCVDActiveRectSX_coordXY(idx);
					rot_y = pdlg->m_Glass->GetCVDActiveRectSY_coordXY(idx);
				}
				GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y, &sx, &sy);

				//Rotate 90
				if(m_ScanDat[type_idx].m_bInspect90Degree)
				{
					RotatedPt(pdlg->m_Glass->GetCVDActiveRectEX_coordXY(idx), pdlg->m_Glass->GetCVDActiveRectEY_coordXY(idx),	0., 0.,	90., &rot_x, &rot_y);
					rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
				}
				else
				{
					rot_x = pdlg->m_Glass->GetCVDActiveRectEX_coordXY(idx);
					rot_y = pdlg->m_Glass->GetCVDActiveRectEY_coordXY(idx);
				}
				GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y, &ex, &ey);

				//Image 안에 있는지 확인, 위아래는 %씩 뺀다(overlay)
				switch(type_idx)
				{
				case 0:
				case 1:
					up_limit_ht = RECIPE_SCAN_SCAN_HT * (0.01 * GetScanOverlay(type_idx, 0));
					dn_limit_ht = RECIPE_SCAN_SCAN_HT - up_limit_ht;

					tot_rect.left = 0;
					tot_rect.top = up_limit_ht;
					tot_rect.right = RECIPE_SCAN_SCAN_WD;
					tot_rect.bottom = dn_limit_ht;
					break;
				case 2:
				case 3:
				case 4:
					up_limit_ht = RECIPE_SCAN_SCAN_16K_HT * (0.01 * GetScanOverlay(type_idx, 0));
					dn_limit_ht = RECIPE_SCAN_SCAN_16K_HT - up_limit_ht;

					tot_rect.left = 0;
					tot_rect.top = up_limit_ht;
					tot_rect.right = RECIPE_SCAN_SCAN_16K_WD;
					tot_rect.bottom = dn_limit_ht;
					break;
				default:
					AfxMessageBox(_T("Error : invalid index(CDialogRecipeScan::MakeGlassDat)!"));
					break;
				}

				rect.left = sx;
				rect.top = sy;
				rect.right = ex;
				rect.bottom = ey;
				rect.NormalizeRect();

				if(rslt_rect.IntersectRect(rect, tot_rect))
				{
					m_ScanDat[type_idx].m_dCVDActiveRectSX[scan_idx][rect_num] = rslt_rect.left;
					m_ScanDat[type_idx].m_dCVDActiveRectSY[scan_idx][rect_num] = rslt_rect.top;
					m_ScanDat[type_idx].m_dCVDActiveRectEX[scan_idx][rect_num] = rslt_rect.right;
					m_ScanDat[type_idx].m_dCVDActiveRectEY[scan_idx][rect_num] = rslt_rect.bottom;

					rect_num++;
					m_ScanDat[type_idx].m_iCVDScanRectNum[scan_idx] = rect_num;

					l_sx = rslt_rect.left;
					l_sy = rslt_rect.top;
					l_ex = rslt_rect.right;
					l_ey = rslt_rect.bottom;
					if(disp)
					{
						set_color(m_dlgImgRect[type_idx]->GetView(), "yellow");
						set_draw(m_dlgImgRect[type_idx]->GetView(), HTuple("margin"));
						disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
						disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
						disp_line(m_dlgImgRect[type_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
						disp_line(m_dlgImgRect[type_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);
					}
				}


				idx++;
			}
		}

		//Draw CVDDN active area////////////////////////////////////////////////////////////////////////////
		cam_idx = 0;
		mark_idx = 0;
		rect_num = 0;
		size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

		idx = 0;
		for(i = 0; i < pdlg->m_RecipeCell->GetCVDDNCntY_coordXY(); i++)
		{
			for(j = 0; j < pdlg->m_RecipeCell->GetCVDDNCntX_coordXY();j++)
			{
				if(GLASS_MAX_CELL_NUM <= idx)
				{
					break;
				}

				//Rotate 90
				if(m_ScanDat[type_idx].m_bInspect90Degree)
				{
					RotatedPt(pdlg->m_Glass->GetCVDDNActiveRectSX_coordXY(idx), pdlg->m_Glass->GetCVDDNActiveRectSY_coordXY(idx),	0., 0.,	90., &rot_x, &rot_y);
					rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
				}
				else
				{
					rot_x = pdlg->m_Glass->GetCVDDNActiveRectSX_coordXY(idx);
					rot_y = pdlg->m_Glass->GetCVDDNActiveRectSY_coordXY(idx);
				}
				GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y, &sx, &sy);

				//Rotate 90
				if(m_ScanDat[type_idx].m_bInspect90Degree)
				{
					RotatedPt(pdlg->m_Glass->GetCVDDNActiveRectEX_coordXY(idx), pdlg->m_Glass->GetCVDDNActiveRectEY_coordXY(idx),	0., 0.,	90., &rot_x, &rot_y);
					rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
				}
				else
				{
					rot_x = pdlg->m_Glass->GetCVDDNActiveRectEX_coordXY(idx);
					rot_y = pdlg->m_Glass->GetCVDDNActiveRectEY_coordXY(idx);
				}
				GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y, &ex, &ey);

				//Image 안에 있는지 확인, 위아래는 %씩 뺀다(overlay)
				switch(type_idx)
				{
				case 0:
				case 1:
					up_limit_ht = RECIPE_SCAN_SCAN_HT * (0.01 * GetScanOverlay(type_idx, 0));
					dn_limit_ht = RECIPE_SCAN_SCAN_HT - up_limit_ht;

					tot_rect.left = 0;
					tot_rect.top = up_limit_ht;
					tot_rect.right = RECIPE_SCAN_SCAN_WD;
					tot_rect.bottom = dn_limit_ht;
					break;
				case 2:
				case 3:
				case 4:
					up_limit_ht = RECIPE_SCAN_SCAN_16K_HT * (0.01 * GetScanOverlay(type_idx, 0));
					dn_limit_ht = RECIPE_SCAN_SCAN_16K_HT - up_limit_ht;

					tot_rect.left = 0;
					tot_rect.top = up_limit_ht;
					tot_rect.right = RECIPE_SCAN_SCAN_16K_WD;
					tot_rect.bottom = dn_limit_ht;
					break;
				default:
					AfxMessageBox(_T("Error : invalid index(CDialogRecipeScan::MakeGlassDat)!"));
					break;
				}

				rect.left = sx;
				rect.top = sy;
				rect.right = ex;
				rect.bottom = ey;
				rect.NormalizeRect();

				if(rslt_rect.IntersectRect(rect, tot_rect))
				{
					m_ScanDat[type_idx].m_dCVDDNActiveRectSX[scan_idx][rect_num] = rslt_rect.left;
					m_ScanDat[type_idx].m_dCVDDNActiveRectSY[scan_idx][rect_num] = rslt_rect.top;
					m_ScanDat[type_idx].m_dCVDDNActiveRectEX[scan_idx][rect_num] = rslt_rect.right;
					m_ScanDat[type_idx].m_dCVDDNActiveRectEY[scan_idx][rect_num] = rslt_rect.bottom;

					rect_num++;
					m_ScanDat[type_idx].m_iCVDDNScanRectNum[scan_idx] = rect_num;

					l_sx = rslt_rect.left;
					l_sy = rslt_rect.top;
					l_ex = rslt_rect.right;
					l_ey = rslt_rect.bottom;
					if(disp)
					{
						set_color(m_dlgImgRect[type_idx]->GetView(), "yellow");
						set_draw(m_dlgImgRect[type_idx]->GetView(), HTuple("margin"));
						disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
						disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
						disp_line(m_dlgImgRect[type_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
						disp_line(m_dlgImgRect[type_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);
					}
				}


				idx++;
			}
		}
	}

	ins_type = 0;
	if(3 == type_idx)
	{
		for(int type = 0; type < 2; type++)
		{
			//Draw PCL align mark////////////////////////////////////////////////////////////////////////////////////
			cam_idx = 0;
			mark_idx = 0;
			rect_num = 0;
			size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

			idx = 0;
			if(pdlg->m_RecipeCellPCL->GetPCLCntX_coordXY(type) > pdlg->m_RecipeCellPCL->GetPCLGrpCntX_coordXY(type))
			{
				grp_idx_x = (int)pdlg->m_RecipeCellPCL->GetPCLGrpCntX_coordXY(type);
			}
			else
			{
				grp_idx_x = 0;
			}
			if(pdlg->m_RecipeCellPCL->GetPCLCntY_coordXY(type) > pdlg->m_RecipeCellPCL->GetPCLGrpCntY_coordXY(type))
			{
				grp_idx_y = (int)pdlg->m_RecipeCellPCL->GetPCLGrpCntY_coordXY(type);
			}
			else
			{
				grp_idx_y = 0;
			}

			x = 0;
			y = 0;
			for(i = 0; i < pdlg->m_RecipeCellPCL->GetPCLCntY_coordXY(type); i++)
			{
				if(0 == i)
				{
					y = pdlg->m_RecipePanel->GetMarkPosY_coordXY(mark_idx) + pdlg->m_RecipePanel->GetCVDFirstPosY_coordXY();
				}
				else if(i == grp_idx_y)
				{
					y =  y + pdlg->m_RecipeCellPCL->GetPCLGrpPitchY_coordXY(type);
				}
				else
				{
					y =  y + pdlg->m_RecipeCellPCL->GetPCLPitchY_coordXY(type);
				}

				for(j = 0; j < pdlg->m_RecipeCellPCL->GetPCLCntX_coordXY(type);j++)
				{
					if(GLASS_MAX_CELL_NUM <= idx)
					{
						break;
					}

					if(0 == j)
					{
						x = pdlg->m_RecipePanel->GetMarkPosX_coordXY(mark_idx) + pdlg->m_RecipePanel->GetCVDFirstPosX_coordXY();
					}
					else if(j == grp_idx_y)
					{
						x =  x + pdlg->m_RecipeCellPCL->GetPCLGrpPitchX_coordXY(type);
					}
					else
					{
						x =  x + pdlg->m_RecipeCellPCL->GetPCLPitchX_coordXY(type);
					}


					if(0 > scan_idx)
					{
						scan_idx = 0;
					}


					//Horz
					if(disp)
					{
						//Rotate 90
						if(m_ScanDat[type_idx].m_bInspect90Degree)
						{
							RotatedPt(x, y,	0., 0.,	90., &rot_x, &rot_y);
							rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
						}
						else
						{
							rot_x = x;
							rot_y = y;
						}

						GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x - size, rot_y, &sx, &sy);
						GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x + size, rot_y, &ex, &ey);

						l_sx = sx + 0.5;
						l_sy = sy + 0.5;
						l_ex = ex + 0.5;
						l_ey = ey + 0.5;
						set_color(m_dlgImgRect[type_idx]->GetView(), "yellow");
						set_draw(m_dlgImgRect[type_idx]->GetView(), HTuple("margin"));
						disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);
					}
					//Vert
					if(disp)
					{
						//Rotate 90
						if(m_ScanDat[type_idx].m_bInspect90Degree)
						{
							RotatedPt(x, y,	0., 0.,	90., &rot_x, &rot_y);
							rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
						}
						else
						{
							rot_x = x;
							rot_y = y;
						}

						GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y - size, &sx, &sy);
						GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y + size, &ex, &ey);

						l_sx = sx + 0.5;
						l_sy = sy + 0.5;
						l_ex = ex + 0.5;
						l_ey = ey + 0.5;
						set_color(m_dlgImgRect[type_idx]->GetView(), "yellow");
						set_draw(m_dlgImgRect[type_idx]->GetView(), HTuple("margin"));
						disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);
					}
					idx++;
				}
			}

			//Draw PCL active area////////////////////////////////////////////////////////////////////////////
			cam_idx = 0;
			mark_idx = 0;
			rect_num = 0;
			size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

			idx = 0;
			for(i = 0; i < pdlg->m_RecipeCellPCL->GetPCLCntY_coordXY(type); i++)
			{
				for(j = 0; j < pdlg->m_RecipeCellPCL->GetPCLCntX_coordXY(type);j++)
				{
					if(GLASS_MAX_CELL_NUM <= idx)
					{
						break;
					}

					//Rotate 90
					if(m_ScanDat[type_idx].m_bInspect90Degree)
					{
						RotatedPt(pdlg->m_Glass->GetPCLActiveRectSX_coordXY(type, ins_type, idx), pdlg->m_Glass->GetPCLActiveRectSY_coordXY(type, ins_type, idx),	0., 0.,	90., &rot_x, &rot_y);
						rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
					}
					else
					{
						rot_x = pdlg->m_Glass->GetPCLActiveRectSX_coordXY(type, ins_type, idx);
						rot_y = pdlg->m_Glass->GetPCLActiveRectSY_coordXY(type, ins_type, idx);
					}
					GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y, &sx, &sy);

					//Rotate 90
					if(m_ScanDat[type_idx].m_bInspect90Degree)
					{
						RotatedPt(pdlg->m_Glass->GetPCLActiveRectEX_coordXY(type, ins_type, idx), pdlg->m_Glass->GetPCLActiveRectEY_coordXY(type, ins_type, idx),	0., 0.,	90., &rot_x, &rot_y);
						rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
					}
					else
					{
						rot_x = pdlg->m_Glass->GetPCLActiveRectEX_coordXY(type, ins_type, idx);
						rot_y = pdlg->m_Glass->GetPCLActiveRectEY_coordXY(type, ins_type, idx);
					}
					GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y, &ex, &ey);

					//Image 안에 있는지 확인, 위아래는 %씩 뺀다(overlay)
					switch(type_idx)
					{
					case 0:
					case 1:
						up_limit_ht = RECIPE_SCAN_SCAN_HT * (0.01 * GetScanOverlay(type_idx, 0));
						dn_limit_ht = RECIPE_SCAN_SCAN_HT - up_limit_ht;

						tot_rect.left = 0;
						tot_rect.top = up_limit_ht;
						tot_rect.right = RECIPE_SCAN_SCAN_WD;
						tot_rect.bottom = dn_limit_ht;
						break;
					case 2:
					case 3:
					case 4:
						up_limit_ht = RECIPE_SCAN_SCAN_16K_HT * (0.01 * GetScanOverlay(type_idx, 0));
						dn_limit_ht = RECIPE_SCAN_SCAN_16K_HT - up_limit_ht;

						tot_rect.left = 0;
						tot_rect.top = up_limit_ht;
						tot_rect.right = RECIPE_SCAN_SCAN_16K_WD;
						tot_rect.bottom = dn_limit_ht;
						break;
					default:
						AfxMessageBox(_T("Error : invalid index(CDialogRecipeScan::MakeGlassDat)!"));
						break;
					}

					rect.left = sx;
					rect.top = sy;
					rect.right = ex;
					rect.bottom = ey;
					rect.NormalizeRect();

					if(rslt_rect.IntersectRect(rect, tot_rect))
					{
						m_ScanDat[type_idx].m_dPCLActiveRectSX[ins_type][scan_idx][rect_num] = rslt_rect.left;
						m_ScanDat[type_idx].m_dPCLActiveRectSY[ins_type][scan_idx][rect_num] = rslt_rect.top;
						m_ScanDat[type_idx].m_dPCLActiveRectEX[ins_type][scan_idx][rect_num] = rslt_rect.right;
						m_ScanDat[type_idx].m_dPCLActiveRectEY[ins_type][scan_idx][rect_num] = rslt_rect.bottom;

						rect_num++;
						m_ScanDat[type_idx].m_iPCLScanRectNum[ins_type][scan_idx] = rect_num;

						l_sx = rslt_rect.left;
						l_sy = rslt_rect.top;
						l_ex = rslt_rect.right;
						l_ey = rslt_rect.bottom;
						if(disp)
						{
							set_color(m_dlgImgRect[type_idx]->GetView(), "yellow");
							set_draw(m_dlgImgRect[type_idx]->GetView(), HTuple("margin"));
							disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
							disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
							disp_line(m_dlgImgRect[type_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
							disp_line(m_dlgImgRect[type_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);
						}
					}


					idx++;
				}
			}
			ins_type++;
		}
	}

	//20160613 ngh test
	ins_type = 0;
	if(4 == type_idx)
	{
		for(int type = 2; type < 4; type++)
		{
			//Draw PCL align mark////////////////////////////////////////////////////////////////////////////////////
			cam_idx = 0;
			mark_idx = 0;
			rect_num = 0;
			size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

			idx = 0;
			if(pdlg->m_RecipeCellPCL->GetPCLCntX_coordXY(type) > pdlg->m_RecipeCellPCL->GetPCLGrpCntX_coordXY(type))
			{
				grp_idx_x = (int)pdlg->m_RecipeCellPCL->GetPCLGrpCntX_coordXY(type);
			}
			else
			{
				grp_idx_x = 0;
			}
			if(pdlg->m_RecipeCellPCL->GetPCLCntY_coordXY(type) > pdlg->m_RecipeCellPCL->GetPCLGrpCntY_coordXY(type))
			{
				grp_idx_y = (int)pdlg->m_RecipeCellPCL->GetPCLGrpCntY_coordXY(type);
			}
			else
			{
				grp_idx_y = 0;
			}

			x = 0;
			y = 0;
			for(i = 0; i < pdlg->m_RecipeCellPCL->GetPCLCntY_coordXY(type); i++)
			{
				if(0 == i)
				{
					y = pdlg->m_RecipePanel->GetMarkPosY_coordXY(mark_idx) + pdlg->m_RecipePanel->GetCVDFirstPosY_coordXY();
				}
				else if(i == grp_idx_y)
				{
					y =  y + pdlg->m_RecipeCellPCL->GetPCLGrpPitchY_coordXY(type);
				}
				else
				{
					y =  y + pdlg->m_RecipeCellPCL->GetPCLPitchY_coordXY(type);
				}

				for(j = 0; j < pdlg->m_RecipeCellPCL->GetPCLCntX_coordXY(type);j++)
				{
					if(GLASS_MAX_CELL_NUM <= idx)
					{
						break;
					}

					if(0 == j)
					{
						x = pdlg->m_RecipePanel->GetMarkPosX_coordXY(mark_idx) + pdlg->m_RecipePanel->GetCVDFirstPosX_coordXY();
					}
					else if(j == grp_idx_y)
					{
						x =  x + pdlg->m_RecipeCellPCL->GetPCLGrpPitchX_coordXY(type);
					}
					else
					{
						x =  x + pdlg->m_RecipeCellPCL->GetPCLPitchX_coordXY(type);
					}


					if(0 > scan_idx)
					{
						scan_idx = 0;
					}


					//Horz
					if(disp)
					{
						//Rotate 90
						if(m_ScanDat[type_idx].m_bInspect90Degree)
						{
							RotatedPt(x, y,	0., 0.,	90., &rot_x, &rot_y);
							rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
						}
						else
						{
							rot_x = x;
							rot_y = y;
						}

						GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x - size, rot_y, &sx, &sy);
						GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x + size, rot_y, &ex, &ey);

						l_sx = sx + 0.5;
						l_sy = sy + 0.5;
						l_ex = ex + 0.5;
						l_ey = ey + 0.5;
						set_color(m_dlgImgRect[type_idx]->GetView(), "yellow");
						set_draw(m_dlgImgRect[type_idx]->GetView(), HTuple("margin"));
						disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);
					}
					//Vert
					if(disp)
					{
						//Rotate 90
						if(m_ScanDat[type_idx].m_bInspect90Degree)
						{
							RotatedPt(x, y,	0., 0.,	90., &rot_x, &rot_y);
							rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
						}
						else
						{
							rot_x = x;
							rot_y = y;
						}

						GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y - size, &sx, &sy);
						GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y + size, &ex, &ey);

						l_sx = sx + 0.5;
						l_sy = sy + 0.5;
						l_ex = ex + 0.5;
						l_ey = ey + 0.5;
						set_color(m_dlgImgRect[type_idx]->GetView(), "yellow");
						set_draw(m_dlgImgRect[type_idx]->GetView(), HTuple("margin"));
						disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);
					}
					idx++;
				}
			}

			//Draw PCL active area////////////////////////////////////////////////////////////////////////////
			cam_idx = 0;
			mark_idx = 0;
			rect_num = 0;
			size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

			idx = 0;
			for(i = 0; i < pdlg->m_RecipeCellPCL->GetPCLCntY_coordXY(type); i++)
			{
				for(j = 0; j < pdlg->m_RecipeCellPCL->GetPCLCntX_coordXY(type);j++)
				{
					if(GLASS_MAX_CELL_NUM <= idx)
					{
						break;
					}

					//Rotate 90
					if(m_ScanDat[type_idx].m_bInspect90Degree)
					{
						RotatedPt(pdlg->m_Glass->GetPCLActiveRectSX_coordXY(type, ins_type, idx), pdlg->m_Glass->GetPCLActiveRectSY_coordXY(type, ins_type, idx),	0., 0.,	90., &rot_x, &rot_y);
						rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
					}
					else
					{
						rot_x = pdlg->m_Glass->GetPCLActiveRectSX_coordXY(type, ins_type, idx);
						rot_y = pdlg->m_Glass->GetPCLActiveRectSY_coordXY(type, ins_type, idx);
					}
					GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y, &sx, &sy);

					//Rotate 90
					if(m_ScanDat[type_idx].m_bInspect90Degree)
					{
						RotatedPt(pdlg->m_Glass->GetPCLActiveRectEX_coordXY(type, ins_type, idx), pdlg->m_Glass->GetPCLActiveRectEY_coordXY(type, ins_type, idx),	0., 0.,	90., &rot_x, &rot_y);
						rot_y = rot_y + pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
					}
					else
					{
						rot_x = pdlg->m_Glass->GetPCLActiveRectEX_coordXY(type, ins_type, idx);
						rot_y = pdlg->m_Glass->GetPCLActiveRectEY_coordXY(type, ins_type, idx);
					}
					GetCoordScanXYfromGlass_coordXY(type_idx, scan_idx, GetScanPitch_coordXY(type_idx), rot_x, rot_y, &ex, &ey);

					//Image 안에 있는지 확인, 위아래는 %씩 뺀다(overlay)
					switch(type_idx)
					{
					case 0:
					case 1:
						up_limit_ht = RECIPE_SCAN_SCAN_HT * (0.01 * GetScanOverlay(type_idx, 0));
						dn_limit_ht = RECIPE_SCAN_SCAN_HT - up_limit_ht;

						tot_rect.left = 0;
						tot_rect.top = up_limit_ht;
						tot_rect.right = RECIPE_SCAN_SCAN_WD;
						tot_rect.bottom = dn_limit_ht;
						break;
					case 2:
					case 3:
					case 4:
						up_limit_ht = RECIPE_SCAN_SCAN_16K_HT * (0.01 * GetScanOverlay(type_idx, 0));
						dn_limit_ht = RECIPE_SCAN_SCAN_16K_HT - up_limit_ht;

						tot_rect.left = 0;
						tot_rect.top = up_limit_ht;
						tot_rect.right = RECIPE_SCAN_SCAN_16K_WD;
						tot_rect.bottom = dn_limit_ht;
						break;
					default:
						AfxMessageBox(_T("Error : invalid index(CDialogRecipeScan::MakeGlassDat)!"));
						break;
					}

					rect.left = sx;
					rect.top = sy;
					rect.right = ex;
					rect.bottom = ey;
					rect.NormalizeRect();

					if(rslt_rect.IntersectRect(rect, tot_rect))
					{
						m_ScanDat[type_idx].m_dPCLActiveRectSX[ins_type][scan_idx][rect_num] = rslt_rect.left;
						m_ScanDat[type_idx].m_dPCLActiveRectSY[ins_type][scan_idx][rect_num] = rslt_rect.top;
						m_ScanDat[type_idx].m_dPCLActiveRectEX[ins_type][scan_idx][rect_num] = rslt_rect.right;
						m_ScanDat[type_idx].m_dPCLActiveRectEY[ins_type][scan_idx][rect_num] = rslt_rect.bottom;

						rect_num++;
						m_ScanDat[type_idx].m_iPCLScanRectNum[ins_type][scan_idx] = rect_num;

						l_sx = rslt_rect.left;
						l_sy = rslt_rect.top;
						l_ex = rslt_rect.right;
						l_ey = rslt_rect.bottom;
						if(disp)
						{
							set_color(m_dlgImgRect[type_idx]->GetView(), "yellow");
							set_draw(m_dlgImgRect[type_idx]->GetView(), HTuple("margin"));
							disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
							disp_line(m_dlgImgRect[type_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
							disp_line(m_dlgImgRect[type_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
							disp_line(m_dlgImgRect[type_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);
						}
					}


					idx++;
				}
			}
			ins_type++;
		}
	}
}

void CDialogRecipeScan::ClickBtnenhctrlPanelScanDrawCoord()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DispGlassDat();
}

void CDialogRecipeScan::GetCoordScanXYfromGlass_coordXY(int type_idx, int pitch_idx, double scan_wd, double glass_x, double glass_y, double *scan_x, double *scan_y)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	double wd;
	//*scan_x = ((glass_x + m_edit_recipe_glass_y) / Get8KScanPixelX_coordXY()) + m_edit_recipe_scan_coord_x;
	//Glass좌표를 Scan 좌표계로 변환///////////////////////////////////////////////////////////////
	if(0 < GetScanPixelX_coordXY(type_idx))
	{
		*scan_x = ( (glass_x + GetScanGlassX_coordXY(type_idx)) / GetScanPixelX_coordXY(type_idx) ) + GetScanImageX_coordXY(type_idx);
	}
	else
	{
		*scan_x = 0.;
	}

	if(0 < GetScanPixelY_coordXY(type_idx))
	{
		*scan_y = -( (glass_y - GetScanGlassY_coordXY(type_idx)) / GetScanPixelY_coordXY(type_idx) ) + GetScanImageY_coordXY(type_idx);
	}
	else
	{
		*scan_y = 0.;
	}
	
	//Shift scan pitch
	if(0 < GetScanPixelY_coordXY(type_idx))
	{
		wd = scan_wd / GetScanPixelY_coordXY(type_idx);
	}
	else
	{
		wd = 0.;
	}
	*scan_y = *scan_y + (pitch_idx * wd);
}

void CDialogRecipeScan::GetCoordGlassXYfromScan_coordXY(int type_idx, int pitch_idx, double scan_wd, double scan_x, double scan_y, double *glass_x, double *glass_y)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	double wd;
	double rot_x, rot_y;

	//Glass좌표를 Scan 좌표계로 변환///////////////////////////////////////////////////////////////
	*glass_x = ((scan_x -  GetScanImageX_coordXY(type_idx)) * GetScanPixelX_coordXY(type_idx)) - GetScanGlassX_coordXY(type_idx);

	*glass_y = ((-scan_y + GetScanImageY_coordXY(type_idx)) * GetScanPixelY_coordXY(type_idx)) + GetScanGlassY_coordXY(type_idx);


	//Shift scan pitch
	if(0 < GetScanPixelY_coordXY(type_idx))
	{
		wd = scan_wd;// / GetScanPixelY_coordXY();
	}
	else
	{
		wd = 0.;
	}
	*glass_y = *glass_y + (pitch_idx * wd);

	//Rotate 90
	if(m_ScanDat[type_idx].m_bInspect90Degree)
	{
		RotatedPt(*glass_x, *glass_y,	0., 0.,	-90., &rot_x, &rot_y);
		rot_x = rot_x - pdlg->m_RecipePanel->GetPanelSizeX_coordXY();
		*glass_x = rot_x;
		*glass_y = rot_y;
	}
}

void CDialogRecipeScan::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	double spd = RECIPE_SCAN_SCANSPD;
	double pos;
	int scan_height = 0;
	BOOL derection;

	switch(nIDEvent)
	{
	case 0:
#ifndef MACHINE
		m_iScanSeq = 4;
#endif
		switch(m_iScanSeq)
		{
		case 0:
			//Start
			m_iScanSeq++;
			break;
		case 1:
			//Move start position
			pdlg->m_Robot->SetRobotMove_mm(MOTOR_0,GetScanStartX_coordXY(m_tab_type_idx), spd);
			pdlg->m_Robot->SetRobotMove_mm(MOTOR_1,GetScanStartY_coordXY(m_tab_type_idx), spd);

			m_iScanSeq++;
			break;
		case 2:
			if(pdlg->m_Robot->GetIsMoveComplete(MOTOR_0, GetScanStartX_coordXY(m_tab_type_idx)))
			{
				if(pdlg->m_Robot->GetIsInposition(MOTOR_0))
				{
					m_iScanSeq++;
				}
			}
			break;
		case 3:
			if(pdlg->m_Robot->GetIsMoveComplete(MOTOR_1, GetScanStartY_coordXY(m_tab_type_idx)))
			{
				if(pdlg->m_Robot->GetIsInposition(MOTOR_1))
				{
					m_iScanSeq++;
				}
			}
			break;
		case 4:
			//Send start packet
			if(m_tab_type_idx < 2)
			{
				scan_height = (GetScanLeng_coordXY(m_tab_type_idx) * 1000) / 12;
			}
			else
			{
				scan_height = (GetScanLeng_coordXY(m_tab_type_idx) * 1000) / 5;
			}
			if(scan_height % 1000)
			{
				scan_height -= (scan_height % 1000);
			}
			pdlg->m_Trigger->SetOPMode(TRUE);
			pdlg->m_Trigger->SetResetEncoderCnt();
			int cam_idx;
			switch(m_tab_type_idx)
			{
			case 0:
			case 1:
				cam_idx = 0;
				break;
			case 2:
			case 3:
			case 4:
				cam_idx = 1;
				break;
			}

			if(0 == ((m_edit_scan_idx - 1) % 2))
			{
				derection = FALSE;
			}
			else
			{
				derection = TRUE;
			}

			pdlg->PacketSendIPS(pdlg->GetGrabReadyPacket(cam_idx, m_check_inspect_90degree, m_tab_type_idx, m_edit_scan_idx - 1, derection, scan_height));
			pdlg->m_Trigger->SetDirection(TRUE);
#ifdef MACHINE
			m_iScanSeq++;
#else
			m_iScanSeq = 0;
			KillTimer(0);
#endif
			break;
		case 5:
			m_iScanSeq++;
			break;
		case 6:
			pos = GetScanStartX_coordXY(m_tab_type_idx) - GetScanLeng_coordXY(m_tab_type_idx);
			pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, pos, spd);

			m_iScanSeq++;
			break;
		case 7:
			pos = GetScanStartX_coordXY(m_tab_type_idx) - GetScanLeng_coordXY(m_tab_type_idx);
			if(pdlg->m_Robot->GetIsMoveComplete(MOTOR_0, pos))
			{
				if(pdlg->m_Robot->GetIsInposition(MOTOR_0))
				{
					m_iScanSeq++;
				}
			}
			break;
		case 8:
			//Complete
			pdlg->m_Trigger->SetOPMode(FALSE);
			KillTimer(nIDEvent);
			m_iScanSeq = 0;
			AfxMessageBox(_T("Scan comlete!"));
			
			break;
		}

		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogRecipeScan::OnTimer)!"));
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CDialogRecipeScan::OnImageRectMessage(WPARAM para0, LPARAM para1)
{
	
	// TODO: Add your control notification handler code here
	HTuple wd, ht;
	Herror herr;
	CString status;
	int cam_idx;
	cam_idx = para0;

	switch((int)para1)
	{
	//Display
	case 0:
		disp_obj(m_HalImage[m_tab_type_idx], m_dlgImgRect[m_tab_type_idx]->GetView());
		m_dlgImgRect[m_tab_type_idx]->Display();
		DispGlassDat();
		break;
	//Live on
	case 1:

		break;
	//Live off
	case 2:

		break;
	//Image load
	case 3:
		Hal_OpenHalImage(&m_HalImage[m_tab_type_idx], CString(""), FALSE);

		set_check("~give_error");
		herr = get_image_pointer1(m_HalImage[m_tab_type_idx], NULL, NULL, &wd, &ht);
		set_check("give_error");
		if(H_MSG_TRUE == herr)
		{
		}
		else
		{
			AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1!"));
			return 0;
		}

		disp_obj(m_HalImage[m_tab_type_idx], m_dlgImgRect[m_tab_type_idx]->GetView());
		m_dlgImgRect[m_tab_type_idx]->Display();

		break;
		//Image save
	case 4:
		Hal_SaveHalImage(&m_HalImage[m_tab_type_idx], CString(""), CString("bmp"));
		break;
	//Gray value
	case 5:
	
		break;
		//Region confirm
		case 6:
			
			break;
		//Test
		case 7:
	
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogRecipeScan::OnImageRectMessage)!"));
		break;
	}

	return 0;
}

void CDialogRecipeScan::OnChangeEditRecipeScanIdx()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

int CDialogRecipeScan::GetNoiseSize(int type_idx, int idx)
{
	return m_ScanDat[type_idx].m_NoiseSize[idx];
}

double CDialogRecipeScan::GetMargeDistance(int type_idx)
{
	return m_ScanDat[type_idx].m_MargeDistance;
}

int CDialogRecipeScan::GetThresh(int type_idx, int idx)
{
	int rslt = 0.;

	switch(idx)
	{
	case 0:
		rslt = (int)(m_ScanDat[type_idx].m_dThrsh0);
		break;
	case 1:
		rslt = (int)(m_ScanDat[type_idx].m_dThrsh1);
		break;
	case 2:
		rslt = (int)(m_ScanDat[type_idx].m_dThrsh2);
		break;
	case 3:
		rslt = (int)(m_ScanDat[type_idx].m_dThrsh3);
		break;
	case 4:
		rslt = (int)(m_ScanDat[type_idx].m_dThrsh4);
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogRecipeScan::GetThresh)!"));
		break;
	}

	return rslt;
}

int CDialogRecipeScan::GetLight(int type_idx)
{
	int rslt = 0.;

	rslt = (int)(m_ScanDat[type_idx].m_dLight0);

	return rslt;
}

double CDialogRecipeScan::GetDefectSize(int type_idx, int idx)
{
	double rslt = 0.;

	switch(idx)
	{
	case 0:
		rslt = (m_ScanDat[type_idx].m_dDefect_size0);
		break;
	case 1:
		rslt = (m_ScanDat[type_idx].m_dDefect_size1);
		break;
	case 2:
		rslt = (m_ScanDat[type_idx].m_dDefect_size2);
		break;
	case 3:
		rslt = (m_ScanDat[type_idx].m_dDefect_size3);
		break;
	case 4:
		rslt = (m_ScanDat[type_idx].m_dDefect_size4);
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogRecipeScan::GetDefectSize)!"));
		break;
	}

	return rslt;
}

int CDialogRecipeScan::GetActiveRectNum(int type_idx, int scan_idx)
{
	return m_ScanDat[type_idx].m_iScanRectNum[scan_idx];
}

int CDialogRecipeScan::GetCVDActiveRectNum(int type_idx, int scan_idx)
{
	return m_ScanDat[type_idx].m_iCVDScanRectNum[scan_idx];
}

int CDialogRecipeScan::GetCVDDNActiveRectNum(int type_idx, int scan_idx)
{
	return m_ScanDat[type_idx].m_iCVDDNScanRectNum[scan_idx];
}


RECT CDialogRecipeScan::GetActiveRect(int type_idx, int scan_idx, int rect_idx)
{
	RECT rect;

	rect.left = m_ScanDat[type_idx].m_dCellActiveRectSX[scan_idx][rect_idx];
	rect.top = m_ScanDat[type_idx].m_dCellActiveRectSY[scan_idx][rect_idx];
	rect.right = m_ScanDat[type_idx].m_dCellActiveRectEX[scan_idx][rect_idx];
	rect.bottom = m_ScanDat[type_idx].m_dCellActiveRectEY[scan_idx][rect_idx];

	return rect;
}

RECT CDialogRecipeScan::GetCVDActiveRect(int type_idx, int scan_idx, int rect_idx)
{
	RECT rect;

	rect.left = m_ScanDat[type_idx].m_dCVDActiveRectSX[scan_idx][rect_idx];
	rect.top = m_ScanDat[type_idx].m_dCVDActiveRectSY[scan_idx][rect_idx];
	rect.right = m_ScanDat[type_idx].m_dCVDActiveRectEX[scan_idx][rect_idx];
	rect.bottom = m_ScanDat[type_idx].m_dCVDActiveRectEY[scan_idx][rect_idx];

	return rect;
}

RECT CDialogRecipeScan::GetCVDDNActiveRect(int type_idx, int scan_idx, int rect_idx)
{
	RECT rect;

	rect.left = m_ScanDat[type_idx].m_dCVDDNActiveRectSX[scan_idx][rect_idx];
	rect.top = m_ScanDat[type_idx].m_dCVDDNActiveRectSY[scan_idx][rect_idx];
	rect.right = m_ScanDat[type_idx].m_dCVDDNActiveRectEX[scan_idx][rect_idx];
	rect.bottom = m_ScanDat[type_idx].m_dCVDDNActiveRectEY[scan_idx][rect_idx];

	return rect;
}

int CDialogRecipeScan::GetPCLActiveRectNum(int type_idx, int scan_idx, int type)
{
	return m_ScanDat[type_idx].m_iPCLScanRectNum[type][scan_idx];
}


RECT CDialogRecipeScan::GetPCLActiveRect(int type_idx, int scan_idx, int rect_idx, int type)
{
	RECT rect;

	rect.left = m_ScanDat[type_idx].m_dPCLActiveRectSX[type][scan_idx][rect_idx];
	rect.top = m_ScanDat[type_idx].m_dPCLActiveRectSY[type][scan_idx][rect_idx];
	rect.right = m_ScanDat[type_idx].m_dPCLActiveRectEX[type][scan_idx][rect_idx];
	rect.bottom = m_ScanDat[type_idx].m_dPCLActiveRectEY[type][scan_idx][rect_idx];

	return rect;
}

void CDialogRecipeScan::ClickBtnenhctrlPanelScanThreshTest()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Hobject slct_rgn, pat_rslt_rgn;
	long rslt_num, num, i;
	long pat_rslt_num;
	HTuple Num, area, row0, col0, row1, col1, ccol, crow;
	CRect rslt_rect;
	int cam_idx;
	cam_idx = 0;

	double rslt_x[DEFECT_DATA_MAX_NUM], rslt_y[DEFECT_DATA_MAX_NUM];

	switch(m_tab_type_idx)
	{
	case 0:
	case 1:
		MakeGlassDat(m_tab_type_idx, m_edit_scan_idx - 1, TRUE);
		//SetInspectScan( m_dlgImgRect[m_tab_type_idx]->GetView(), &m_HalImage[m_tab_type_idx], m_tab_type_idx, GetThresh(m_tab_type_idx, 0), &m_InspectRegion, &rslt_num, &m_DefectRegion);
		SetInspectScan( m_dlgImgRect[m_tab_type_idx]->GetView(), &m_HalImage[m_tab_type_idx], m_tab_type_idx, &rslt_num, &m_DefectRegion, &pat_rslt_num, &pat_rslt_rgn);

		if(m_dlgImgRect[m_tab_type_idx]->GetView())
		{
			set_color(m_dlgImgRect[m_tab_type_idx]->GetView(), HTuple("red"));
			set_draw(m_dlgImgRect[m_tab_type_idx]->GetView(), HTuple("margin"));
			disp_obj(m_DefectRegion, m_dlgImgRect[m_tab_type_idx]->GetView());

			set_color(m_dlgImgRect[m_tab_type_idx]->GetView(), HTuple("yellow"));
			set_draw(m_dlgImgRect[m_tab_type_idx]->GetView(), HTuple("margin"));
			disp_obj(pat_rslt_rgn, m_dlgImgRect[m_tab_type_idx]->GetView());
		}

		num = 0;
		if(rslt_num)
		{
			//불량의 width, height얻기
			connection(m_DefectRegion, &m_DefectRegion);
			select_shape(m_DefectRegion, &m_DefectRegion, HTuple("area"), HTuple("and"), HTuple(2),  HTuple("max"));
			count_obj(m_DefectRegion, &Num);
		
			num = (Hlong)Num[0];
			char file_name[512];
			Hobject rslt_image;
			CRect rNGrect;
			for(i = 0; i < num; i++)
			{
				select_obj(m_DefectRegion, &slct_rgn, i + 1);

				area_center(slct_rgn, &area, &crow, &ccol);
				smallest_rectangle1(slct_rgn, &row0, &col0, &row1, &col1);
				rslt_rect = CRect((Hlong)col0[0], (Hlong)row0[0], (Hlong)col1[0], (Hlong)row1[0]);

				rNGrect.left = (rslt_rect.left - NG_IMAGE_HEIGHT < 0) ? 0 : rslt_rect.left - NG_IMAGE_HEIGHT;
				rNGrect.top = (rslt_rect.top - NG_IMAGE_WIDTH < 0) ? 0 : rslt_rect.top - NG_IMAGE_WIDTH;
				rNGrect.right = (rslt_rect.right + NG_IMAGE_HEIGHT > MAX_HALCON_IMAGE_HEIGHT_8K) ?  MAX_HALCON_IMAGE_HEIGHT_8K : rslt_rect.right + NG_IMAGE_HEIGHT;
				rNGrect.bottom = (rslt_rect.bottom + NG_IMAGE_WIDTH > MAX_HALCON_IMAGE_WIDTH_8K) ? MAX_HALCON_IMAGE_WIDTH_8K : rslt_rect.bottom + NG_IMAGE_WIDTH;

				//Width
				rslt_rect.Width();
				//Height
				rslt_rect.Height();
				//Center
				rslt_rect.CenterPoint();
			}
		}
		if(pat_rslt_num)
		{
			//불량의 width, height얻기
			connection(pat_rslt_rgn, &pat_rslt_rgn);
			select_shape(pat_rslt_rgn, &pat_rslt_rgn, HTuple("area"), HTuple("and"), HTuple(2),  HTuple("max"));
			count_obj(pat_rslt_rgn, &Num);
		
			num = (Hlong)Num[0];
			char file_name[512];
			Hobject rslt_image;
			CRect rNGrect;
			for(i = 0; i < num; i++)
			{
				select_obj(pat_rslt_rgn, &slct_rgn, i + 1);

				area_center(slct_rgn, &area, &crow, &ccol);
				smallest_rectangle1(slct_rgn, &row0, &col0, &row1, &col1);
				rslt_rect = CRect((Hlong)col0[0], (Hlong)row0[0], (Hlong)col1[0], (Hlong)row1[0]);

				rNGrect.left = (rslt_rect.left - NG_IMAGE_HEIGHT < 0) ? 0 : rslt_rect.left - NG_IMAGE_HEIGHT;
				rNGrect.top = (rslt_rect.top - NG_IMAGE_WIDTH < 0) ? 0 : rslt_rect.top - NG_IMAGE_WIDTH;
				rNGrect.right = (rslt_rect.right + NG_IMAGE_HEIGHT > MAX_HALCON_IMAGE_HEIGHT_8K) ?  MAX_HALCON_IMAGE_HEIGHT_8K : rslt_rect.right + NG_IMAGE_HEIGHT;
				rNGrect.bottom = (rslt_rect.bottom + NG_IMAGE_WIDTH > MAX_HALCON_IMAGE_WIDTH_8K) ? MAX_HALCON_IMAGE_WIDTH_8K : rslt_rect.bottom + NG_IMAGE_WIDTH;

				//Width
				rslt_rect.Width();
				//Height
				rslt_rect.Height();
				//Center
				rslt_rect.CenterPoint();
			}
		}
		break;
	case 2:
		MakeGlassDat(m_tab_type_idx, m_edit_scan_idx - 1, TRUE);
		SetInspectScanCVD(m_dlgImgRect[m_tab_type_idx]->GetView(), &m_HalImage[m_tab_type_idx], TRUE, m_tab_type_idx, GetScanPixelY_coordXY(m_tab_type_idx), GetThresh(m_tab_type_idx, 0), &rslt_num, rslt_x, rslt_y);
		SetInspectScanCVD(m_dlgImgRect[m_tab_type_idx]->GetView(), &m_HalImage[m_tab_type_idx], FALSE, m_tab_type_idx, GetScanPixelY_coordXY(m_tab_type_idx), GetThresh(m_tab_type_idx, 0), &rslt_num, rslt_x, rslt_y);
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogRecipeScan::ClickBtnenhctrlPanelScanThreshTest)!"));
		break;
	}
}

BOOL CDialogRecipeScan::SetInspectScan(long disp, Hobject *image, int type_idx, int thresh_, Hobject *ins_rgn, long *rslt_num, Hobject *rslt_rgn)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	BOOL rslt = TRUE;
	int cam_idx = 0;
	int i;
	long num;
	int noise[3];
	long black_rslt_num, white_rslt_num;
	Hobject black_rslt_rgn, white_rslt_rgn;
	int thresh[5];
	char file_name[512];

	for(int i = 0; i < 5; i++)
	{
		thresh[i] = GetThresh(type_idx, i);
	}


	noise[0] = GetNoiseSize(type_idx, 0);
	noise[1] = GetNoiseSize(type_idx, 1);
	noise[2] = GetNoiseSize(type_idx, 2);


	RECT rect;
	Hobject sub_ins_rgn, Region;
	Hobject ImageReduced, EmphaImage, MeanImage;
	Herror herr;
	HTuple Num;
	Hobject slct_rgn, slct_rgn2, sum_rgn, temp_rgn, temp_rgn2;
	HTuple area, row0, col0, row1, col1, ccol, crow;
	CRect rslt_rect, rslt_rect2;

	int scan_idx = m_edit_scan_idx - 1;

	if(disp)
	{
		//disp_obj(*image, disp);
	}

	gen_empty_obj(ins_rgn);
	gen_empty_obj(rslt_rgn);
	*rslt_num = 0;
	black_rslt_num = 0;
	white_rslt_num = 0;

	//black
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for(i = 0; i < m_ScanDat[type_idx].m_iScanRectNum[scan_idx]; i++)
	{
		rect.left = m_ScanDat[type_idx].m_dCellActiveRectSX[scan_idx][i];
		rect.top = m_ScanDat[type_idx].m_dCellActiveRectSY[scan_idx][i];
		rect.right = m_ScanDat[type_idx].m_dCellActiveRectEX[scan_idx][i];
		rect.bottom = m_ScanDat[type_idx].m_dCellActiveRectEY[scan_idx][i];
		gen_rectangle1(&sub_ins_rgn, rect.top, rect.left, rect.bottom, rect.right);

		union2(sub_ins_rgn, *ins_rgn, ins_rgn);
	}
	reduce_domain(m_HalImage[type_idx], *ins_rgn, &ImageReduced);

	mean_image(ImageReduced, &MeanImage, 2, 2);
	emphasize(MeanImage, &EmphaImage, 3, 3, 1);
	//mean_image(ImageReduced, &MeanImage, 2, 2);
	//emphasize(MeanImage, &EmphaImage, 15, 15, 2);

	set_check("~give_error");
	herr = threshold(EmphaImage, &Region, 0, thresh[0]);
	set_check("give_error");
	if(H_MSG_TRUE == herr)
	{
	}
	else
	{
		//Error log

		rslt = FALSE;
		return rslt;
	}	

	connection(Region, &black_rslt_rgn);
	//작은 노이즈 없에기
	select_shape(black_rslt_rgn, &black_rslt_rgn, HTuple("area"), HTuple("and"), HTuple(noise[2]),  HTuple("max"));
	select_shape(black_rslt_rgn, &black_rslt_rgn, HTuple("width"), HTuple("and"), HTuple(noise[0]),  HTuple("max"));
	select_shape(black_rslt_rgn, &black_rslt_rgn, HTuple("height"), HTuple("and"), HTuple(noise[1]),  HTuple("max"));

	count_obj(Region, &Num);
	/*num = 0;
	num = (Hlong)Num[0];
	if(0 < num)
	{
		if(disp)
		{
			set_color(disp, HTuple("red"));
			set_draw(disp, HTuple("margin"));
			disp_obj(black_rslt_rgn, disp);
		}

		black_rslt_num = num;
		union1(Region, &Region);
		copy_obj(Region, &black_rslt_rgn, 1, -1);
	}
	else
	{

	}*/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////


	//white
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	gen_empty_obj(ins_rgn);

	for(i = 0; i < m_ScanDat[type_idx].m_iScanRectNum[scan_idx]; i++)
	{
		rect.left = m_ScanDat[type_idx].m_dCellActiveRectSX[scan_idx][i];
		rect.top = m_ScanDat[type_idx].m_dCellActiveRectSY[scan_idx][i];
		rect.right = m_ScanDat[type_idx].m_dCellActiveRectEX[scan_idx][i];
		rect.bottom = m_ScanDat[type_idx].m_dCellActiveRectEY[scan_idx][i];
		gen_rectangle1(&sub_ins_rgn, rect.top, rect.left, rect.bottom, rect.right);

		union2(sub_ins_rgn, *ins_rgn, ins_rgn);
	}
	reduce_domain(m_HalImage[type_idx], *ins_rgn, &ImageReduced);
	
	//mean_image(ImageReduced, &MeanImage, m_edit_dTest0, m_edit_dTest1);
	//emphasize(MeanImage, &EmphaImage, m_edit_dTest2, m_edit_dTest3, m_edit_dTest4);
	mean_image(ImageReduced, &MeanImage, 2, 2);
	emphasize(MeanImage, &EmphaImage, 3, 3, 1);
	//mean_image(ImageReduced, &MeanImage, 2, 2);
	//emphasize(MeanImage, &EmphaImage, 15, 15, 2);
	
	set_check("~give_error");
	herr = threshold(EmphaImage, &Region, thresh[1], thresh[2]);
	set_check("give_error");
	if(H_MSG_TRUE == herr)
	{
	}
	else
	{
		//Error log

		rslt = FALSE;
		return rslt;
	}	

	connection(Region, &white_rslt_rgn);

	//작은 노이즈 없에기
	select_shape(white_rslt_rgn, &white_rslt_rgn, HTuple("area"), HTuple("and"), HTuple(noise[2]),  HTuple("max"));
	select_shape(white_rslt_rgn, &white_rslt_rgn, HTuple("width"), HTuple("and"), HTuple(noise[0]),  HTuple("max"));
	select_shape(white_rslt_rgn, &white_rslt_rgn, HTuple("height"), HTuple("and"), HTuple(noise[1]),  HTuple("max"));



	/*count_obj(Region, &Num);
	num = 0;
	num = (Hlong)Num[0];
	if(0 < num)
	{
		if(disp)
		{
			set_color(disp, HTuple("green"));
			set_draw(disp, HTuple("margin"));
			disp_obj(white_rslt_rgn, disp);
		}

		white_rslt_num = num;
		union1(Region, &Region);
		copy_obj(Region, &white_rslt_rgn, 1, -1);
	}
	else
	{

	}*/
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//union2(black_rslt_rgn, white_rslt_rgn, &temp_rgn);
	double distance = m_ScanDat[type_idx].m_MargeDistance / 0.012;
	////dilation_circle(temp_rgn, &temp_rgn2, HTuple(distance));

	////erosion_circle(temp_rgn2, &sum_rgn, HTuple(distance));

	HTuple bNum, wNum, Area, row, col;
	long lbNum, lwNum;
	Hobject sel_black_rgn, sel_white_rgn, dil_black, inter_rgn, sub_tot_rgn;
	count_obj(black_rslt_rgn, &bNum);
	count_obj(white_rslt_rgn, &wNum);

	lbNum = (Hlong)bNum[0];
	lwNum = (Hlong)wNum[0];

	for(i = 0; i < lbNum; i++)
	{
		select_obj(black_rslt_rgn, &sel_black_rgn, i + 1);
		dilation_circle(sel_black_rgn, &dil_black, distance);
		for(int j = 0; j < lwNum; j++)
		{
			select_obj(white_rslt_rgn, &sel_white_rgn, j + 1);
			intersection(dil_black, sel_white_rgn, &inter_rgn);
			area_center(inter_rgn, &Area, &row, &col);

			if(1 < (Hlong)Area[0])
			{
				union2(sel_black_rgn, sel_white_rgn, &sub_tot_rgn);
				union2(temp_rgn, sub_tot_rgn, &temp_rgn);
			}
		}
		closing_circle(temp_rgn, &temp_rgn, HTuple(distance));
	}

	closing_circle(temp_rgn, &sum_rgn, HTuple(distance));

	connection(sum_rgn, &sum_rgn);

	//작은 노이즈 없에기
	select_shape(sum_rgn, &sum_rgn, HTuple("area"), HTuple("and"), HTuple(noise[2]),  HTuple("max"));
	select_shape(sum_rgn, &sum_rgn, HTuple("width"), HTuple("and"), HTuple(noise[0]),  HTuple("max"));
	select_shape(sum_rgn, &sum_rgn, HTuple("height"), HTuple("and"), HTuple(noise[1]),  HTuple("max"));

	count_obj(sum_rgn, &Num);
	num = 0;
	num = (Hlong)Num[0];
	
	if(0 < num)
	{
		if(disp)
		{
			set_color(disp, HTuple("red"));
			set_draw(disp, HTuple("margin"));
			disp_obj(sum_rgn, disp);
		}

		*rslt_num = num;
		copy_obj(sum_rgn, rslt_rgn, 1, -1);
		connection(*rslt_rgn, rslt_rgn);
	}
	else
	{

	}

	if(disp)
	{
		set_color(disp, HTuple("blue"));
		set_draw(disp, HTuple("margin"));
		disp_obj(*ins_rgn, disp);
	}

	return rslt;
}

//2160401 ngh
BOOL CDialogRecipeScan::SetInspectScan(long disp, Hobject *image, int type_idx, long *rslt_num, Hobject *rslt_rgn, long *pat_rslt_num, Hobject *pat_rslt_rgn)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	BOOL rslt = TRUE;
	int cam_idx = 0;
	long num;
	int noise[3];
	long black_rslt_num, white_rslt_num;
	Hobject black_rslt_rgn, white_rslt_rgn;
	Hobject pat_black_rslt_rgn, pat_white_rslt_rgn, pat_rslt_rgn1;
	int thresh[5];
	double distance =  m_ScanDat[m_tab_type_idx].m_MargeDistance / 0.012;
	HTuple bwNum;
	HTuple bNum, wNum, pNum, Area, row, col;
	long lwNum, lpNum;
	Hobject sel_black_rgn, sel_white_rgn, dil_black, inter_rgn, sub_tot_rgn, sel_pat_rgn, dil_pat, sel_black_white_rgn, dil_white;
	Hobject dil_rgn, dil_rgn2, sel_white_rgn2, sum_white_rslt_rgn, sum_pat_rslt_rgn, sel_pat_rgn2;
	BOOL bSum;
	int scan_idx = 0;

	scan_idx = m_edit_scan_idx;

	for(int i = 0; i < 5; i++)
	{
		thresh[i] = GetThresh(type_idx, i);
	}

	noise[0] = GetNoiseSize(type_idx, 0);
	noise[1] = GetNoiseSize(type_idx, 1);
	noise[2] = GetNoiseSize(type_idx, 2);


	RECT rect;
	CRect ins_rect[512];
	Hobject sub_ins_rgn, Region, ins_rgn;
	Hobject ImageReduced, EmphaImage, MeanImage;
	Herror herr;
	HTuple Num;
	Hobject slct_rgn, slct_rgn2, sum_rgn, temp_rgn, temp_rgn2, black_white_rgn;
	HTuple area, row0, col0, row1, col1, ccol, crow;
	CRect rslt_rect, rslt_rect2;

	if(disp)
	{
		//disp_obj(*image, disp);
	}

	gen_empty_obj(&ins_rgn);
	gen_empty_obj(rslt_rgn);
	gen_empty_obj(&Region);
	*rslt_num = 0;
	black_rslt_num = 0;
	white_rslt_num = 0;
	//이물
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for(int i = 0; i < m_ScanDat[type_idx].m_iScanRectNum[scan_idx]; i++)
	{
		rect.left = m_ScanDat[type_idx].m_dCellActiveRectSX[scan_idx][i];
		rect.top = m_ScanDat[type_idx].m_dCellActiveRectSY[scan_idx][i];
		rect.right = m_ScanDat[type_idx].m_dCellActiveRectEX[scan_idx][i];
		rect.bottom = m_ScanDat[type_idx].m_dCellActiveRectEY[scan_idx][i];
		gen_rectangle1(&sub_ins_rgn, rect.top, rect.left, rect.bottom, rect.right);

		union2(sub_ins_rgn, ins_rgn, &ins_rgn);
	}

	mean_image(*image, &MeanImage, 2, 2);
	emphasize(MeanImage, &EmphaImage, 15, 15,  m_ScanDat[m_tab_type_idx].m_dDefect_size1);

	reduce_domain(EmphaImage, ins_rgn, &ImageReduced);

	set_check("~give_error");
	herr = threshold(ImageReduced, &Region, 0, thresh[2]);
	set_check("give_error");
	if(H_MSG_TRUE == herr)
	{
	}
	else
	{
		//Error log

		rslt = FALSE;
		return rslt;
	}	

	connection(Region, &pat_black_rslt_rgn);

	//작은 노이즈 없에기
	select_shape(pat_black_rslt_rgn, &pat_black_rslt_rgn, HTuple("area"), HTuple("and"), HTuple(noise[2]),  HTuple("max"));
	select_shape(pat_black_rslt_rgn, &pat_black_rslt_rgn, HTuple("width"), HTuple("and"), HTuple(noise[0]),  HTuple("max"));
	select_shape(pat_black_rslt_rgn, &pat_black_rslt_rgn, HTuple("height"), HTuple("and"), HTuple(noise[1]),  HTuple("max"));

	dilation_circle(pat_black_rslt_rgn, &pat_black_rslt_rgn, HTuple(distance));
	closing_circle(pat_black_rslt_rgn, &pat_black_rslt_rgn, HTuple(distance));

	gen_empty_obj(&sum_rgn);

	connection(pat_black_rslt_rgn, &sum_rgn);

	count_obj(sum_rgn, &Num);

	num = 0;
	num = (Hlong)Num[0];

	for(int i = 0; i < num - 1; i++)
	{
		gen_empty_obj(&temp_rgn);
		gen_empty_obj(&sub_tot_rgn);
		gen_empty_obj(&inter_rgn);

		bSum = FALSE;
		select_obj(pat_black_rslt_rgn, &sel_pat_rgn, i + 1);
		dilation_circle(sel_pat_rgn, &dil_rgn, distance);
		
		for(int j = 1; j < num; j++)
		{
			select_obj(pat_black_rslt_rgn, &sel_pat_rgn2, j + 1);
			intersection(dil_rgn, sel_pat_rgn2, &inter_rgn);
			area_center(inter_rgn, &Area, &row, &col);

			if(1 < (Hlong)Area[0])
			{
				bSum = TRUE;
				union2(sel_pat_rgn, sel_pat_rgn2, &sub_tot_rgn);
				union2(temp_rgn, sub_tot_rgn, &temp_rgn);
			}
		}
		
		if(bSum)
		{
			closing_circle(temp_rgn, &temp_rgn, HTuple(distance));
			union2(temp_rgn, sum_pat_rslt_rgn, &sum_pat_rslt_rgn);
		}
		else
		{
			copy_obj(sel_pat_rgn, &temp_rgn, 1, -1);
			union2(temp_rgn, sum_pat_rslt_rgn, &sum_pat_rslt_rgn);
		}
	}

	erosion_circle(sum_pat_rslt_rgn, &sum_pat_rslt_rgn, HTuple(distance));
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//높이(주름)
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	gen_empty_obj(&ins_rgn);
	gen_empty_obj(rslt_rgn);
	gen_empty_obj(&Region);
	*rslt_num = 0;
	black_rslt_num = 0;
	white_rslt_num = 0;

	for(int i = 0; i < m_ScanDat[type_idx].m_iScanRectNum[scan_idx]; i++)
	{
		rect.left = m_ScanDat[type_idx].m_dCellActiveRectSX[scan_idx][i];
		rect.top = m_ScanDat[type_idx].m_dCellActiveRectSY[scan_idx][i];
		rect.right = m_ScanDat[type_idx].m_dCellActiveRectEX[scan_idx][i];
		rect.bottom = m_ScanDat[type_idx].m_dCellActiveRectEY[scan_idx][i];
		gen_rectangle1(&sub_ins_rgn, rect.top, rect.left, rect.bottom, rect.right);

		union2(sub_ins_rgn, ins_rgn, &ins_rgn);
	}

	mean_image(*image, &MeanImage, 20, 20);
	emphasize(MeanImage, &EmphaImage, 100, 100, m_ScanDat[m_tab_type_idx].m_dDefect_size0);

	reduce_domain(EmphaImage, ins_rgn, &ImageReduced);

	set_check("~give_error");
	herr = threshold(ImageReduced, &Region, thresh[0], thresh[1]);
	set_check("give_error");
	if(H_MSG_TRUE == herr)
	{
	}
	else
	{
		//Error log

		rslt = FALSE;
		return rslt;
	}	

	connection(Region, &white_rslt_rgn);

	//작은 노이즈 없에기
	select_shape(white_rslt_rgn, &white_rslt_rgn, HTuple("area"), HTuple("and"), HTuple(noise[2]),  HTuple("max"));
	select_shape(white_rslt_rgn, &white_rslt_rgn, HTuple("width"), HTuple("and"), HTuple(noise[0]),  HTuple("max"));
	select_shape(white_rslt_rgn, &white_rslt_rgn, HTuple("height"), HTuple("and"), HTuple(noise[1]),  HTuple("max"));

	dilation_circle(white_rslt_rgn, &white_rslt_rgn, HTuple(distance));
	closing_circle(white_rslt_rgn, &white_rslt_rgn, HTuple(distance));

	gen_empty_obj(&sum_rgn);
	connection(white_rslt_rgn, &sum_rgn);

	count_obj(sum_rgn, &Num);

	num = 0;
	num = (Hlong)Num[0];

	for(int i = 0; i < num - 1; i++)
	{
		gen_empty_obj(&temp_rgn);
		gen_empty_obj(&sub_tot_rgn);
		gen_empty_obj(&inter_rgn);

		bSum = FALSE;
		select_obj(white_rslt_rgn, &sel_white_rgn, i + 1);
		dilation_circle(sel_white_rgn, &dil_rgn, distance);
		
		for(int j = 1; j < num; j++)
		{
			select_obj(white_rslt_rgn, &sel_white_rgn2, j + 1);
			intersection(dil_rgn, sel_white_rgn2, &inter_rgn);
			area_center(inter_rgn, &Area, &row, &col);

			if(1 < (Hlong)Area[0])
			{
				bSum = TRUE;
				union2(sel_white_rgn, sel_white_rgn2, &sub_tot_rgn);
				union2(temp_rgn, sub_tot_rgn, &temp_rgn);
			}
		}
		
		if(bSum)
		{
			closing_circle(temp_rgn, &temp_rgn, HTuple(distance));
			union2(temp_rgn, sum_white_rslt_rgn, &sum_white_rslt_rgn);
		}
		else
		{
			copy_obj(sel_white_rgn, &temp_rgn, 1, -1);
			union2(temp_rgn, sum_white_rslt_rgn, &sum_white_rslt_rgn);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//이물 - 높이
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//작은 노이즈 없에기
	select_shape(sum_pat_rslt_rgn, &sum_pat_rslt_rgn, HTuple("area"), HTuple("and"), HTuple(noise[2]),  HTuple("max"));
	select_shape(sum_pat_rslt_rgn, &sum_pat_rslt_rgn, HTuple("width"), HTuple("and"), HTuple(noise[0]),  HTuple("max"));
	select_shape(sum_pat_rslt_rgn, &sum_pat_rslt_rgn, HTuple("height"), HTuple("and"), HTuple(noise[1]),  HTuple("max"));

	connection(sum_pat_rslt_rgn, &sum_pat_rslt_rgn);

	//작은 노이즈 없에기
	select_shape(sum_white_rslt_rgn, &sum_white_rslt_rgn, HTuple("area"), HTuple("and"), HTuple(noise[2]),  HTuple("max"));
	select_shape(sum_white_rslt_rgn, &sum_white_rslt_rgn, HTuple("width"), HTuple("and"), HTuple(noise[0]),  HTuple("max"));
	select_shape(sum_white_rslt_rgn, &sum_white_rslt_rgn, HTuple("height"), HTuple("and"), HTuple(noise[1]),  HTuple("max"));

	connection(sum_white_rslt_rgn, &sum_white_rslt_rgn);

	Hobject sum_rslt_rgn;
	lpNum = 0;
	lwNum = 0;
	count_obj(sum_pat_rslt_rgn, &Num);
	lpNum = (Hlong)Num[0];
	count_obj(sum_white_rslt_rgn, &Num);
	lwNum = (Hlong)Num[0];

	Hobject sel_sum_rgn, sel_sum_rgn2, tot_sum_rslt_rgn;

	for(int i = 0; i < lpNum; i++)
	{
		gen_empty_obj(&temp_rgn);
		gen_empty_obj(&sub_tot_rgn);
		gen_empty_obj(&inter_rgn);

		bSum = FALSE;
		select_obj(sum_pat_rslt_rgn, &sel_pat_rgn, i + 1);
		dilation_circle(sel_pat_rgn, &dil_rgn, distance);
		
		for(int j = 0; j < lwNum; j++)
		{
			select_obj(sum_white_rslt_rgn, &sel_sum_rgn, j + 1);
			//dilation_circle(sel_sum_rgn, &dil_rgn2, distance);
			intersection(dil_rgn, sel_sum_rgn, &inter_rgn);
			area_center(inter_rgn, &Area, &row, &col);

			if(1 < (Hlong)Area[0])
			{
				bSum = TRUE;
			}
		}

		if(!bSum)
		{
			copy_obj(sel_pat_rgn, &temp_rgn, 1, -1);
			union2(temp_rgn, tot_sum_rslt_rgn, &tot_sum_rslt_rgn);
		}
	}
	gen_empty_obj(&sum_rgn);
	connection(tot_sum_rslt_rgn, &sum_rgn);

	//이물 Display
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//작은 노이즈 없에기
	select_shape(sum_rgn, &sum_rgn, HTuple("area"), HTuple("and"), HTuple(noise[2]),  HTuple("max"));
	select_shape(sum_rgn, &sum_rgn, HTuple("width"), HTuple("and"), HTuple(noise[0]),  HTuple("max"));
	select_shape(sum_rgn, &sum_rgn, HTuple("height"), HTuple("and"), HTuple(noise[1]),  HTuple("max"));

	connection(sum_rgn, &sum_rgn);

	count_obj(sum_rgn, &Num);

	num = 0;
	num = (Hlong)Num[0];
	
	if(0 < num)
	{
		if(disp)
		{
			set_color(disp, HTuple("yellow"));
			set_draw(disp, HTuple("margin"));
			disp_obj(sum_rgn, disp);
		}

		*pat_rslt_num = num;
		copy_obj(sum_rgn, pat_rslt_rgn, 1, -1);
		connection(*pat_rslt_rgn, pat_rslt_rgn);
	}
	else
	{

	}


	//주름 Display
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	gen_empty_obj(&sum_rgn);
	erosion_circle(sum_white_rslt_rgn, &sum_rgn, HTuple(distance));

	connection(sum_rgn, &sum_rgn);

	//작은 노이즈 없에기
	select_shape(sum_rgn, &sum_rgn, HTuple("area"), HTuple("and"), HTuple(noise[2]),  HTuple("max"));
	select_shape(sum_rgn, &sum_rgn, HTuple("width"), HTuple("and"), HTuple(noise[0]),  HTuple("max"));
	select_shape(sum_rgn, &sum_rgn, HTuple("height"), HTuple("and"), HTuple(noise[1]),  HTuple("max"));

	connection(sum_rgn, &sum_rgn);

	count_obj(sum_rgn, &Num);

	num = 0;
	num = (Hlong)Num[0];
	
	if(0 < num)
	{
		if(disp)
		{
			set_color(disp, HTuple("red"));
			set_draw(disp, HTuple("margin"));
			disp_obj(sum_rgn, disp);
		}

		*rslt_num = num;
		copy_obj(sum_rgn, rslt_rgn, 1, -1);
		connection(*rslt_rgn, rslt_rgn);
	}
	else
	{

	}

	if(disp)
	{
		set_color(disp, HTuple("blue"));
		set_draw(disp, HTuple("margin"));
		disp_obj(ins_rgn, disp);
	}

	return rslt;
}


BOOL CDialogRecipeScan::SetInspectScanCVD(long disp, Hobject *image, BOOL up_direct, int type_idx, double pixel_size, int thresh, long *rslt_num, double *rslt_x, double *rslt_y)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	BOOL rslt = TRUE;
	int cam_idx = 0;
	int i, j;
	long num;

	int pitch;

	HTuple RowEdgeFirstLR, ColumnEdgeFirstLR, RowEdgeSecondLR, ColumnEdgeSecondLR, RowEdgeFirstUD, ColumnEdgeFirstUD, RowEdgeSecondUD, ColumnEdgeSecondUD; 
	HTuple AmplitudeSecond, MeasureHandle, IntraDistance, InterDistance, AmplitudeFirst;
	HTuple wd, ht, row, col, leng;
	Hlong edgeCnt;

	RECT rect;
	Hobject sub_ins_rgn, Region;
	Hobject ImageReduced, EmphaImage, MeanImage;
	Herror herr;
	HTuple Num;

	int scan_idx = m_edit_scan_idx - 1;

	if(disp)
	{
		//disp_obj(*image, disp);
	}

	*rslt_num = 0;
	
	if(0 >= m_ScanDat[type_idx].m_iCVDScanRectNum[scan_idx])
	{
		return FALSE;
	}

	set_check("~give_error");
	herr = get_image_pointer1(*image, NULL, NULL, &wd, &ht);
	set_check("give_error");
	if(H_MSG_TRUE == herr)
	{
	}
	else
	{
		AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1!"));
		return FALSE;
	}



	/*
	CString file_path, data;
	file_path.Format(_T("D:\\16K_CVD_%d.csv"), scan_idx);
	data.Format(_T("index,Up&Down,result_x(pixel),result_y(pixel)"));
	pdlg->WriteInsResultFile(file_path, data);
	*/


	for(i = 0; i < m_ScanDat[type_idx].m_iCVDScanRectNum[scan_idx]; i++)
	{
		if(up_direct)
		{
			rect.left = m_ScanDat[type_idx].m_dCVDActiveRectSX[scan_idx][i];
			rect.top = m_ScanDat[type_idx].m_dCVDActiveRectSY[scan_idx][i];
			rect.right = m_ScanDat[type_idx].m_dCVDActiveRectEX[scan_idx][i];
			rect.bottom = m_ScanDat[type_idx].m_dCVDActiveRectEY[scan_idx][i];
		}
		else
		{
			rect.left = m_ScanDat[type_idx].m_dCVDDNActiveRectSX[scan_idx][i];
			rect.top = m_ScanDat[type_idx].m_dCVDDNActiveRectSY[scan_idx][i];
			rect.right = m_ScanDat[type_idx].m_dCVDDNActiveRectEX[scan_idx][i];
			rect.bottom = m_ScanDat[type_idx].m_dCVDDNActiveRectEY[scan_idx][i];
		}

		//LR
		row = (Hlong)(rect.top);
		col = (Hlong)((rect.right + rect.left) * 0.5);

		leng = (rect.right - rect.left) * 0.5;
		gen_measure_rectangle2(row,col,HTuple(0),leng,50,wd,ht,HTuple("nearest_neighbor"),&MeasureHandle);


		if(0 < pixel_size)
		{
			pitch = 1. / pixel_size;
		}
		else
		{
			pitch = 1;
		}

		for(j = 0; (j < rect.bottom) && ((row + pitch) < rect.bottom); j++)
		{

			if(0 < pixel_size)
			{
				pitch = (1. * j) / pixel_size;
			}
			else
			{
				pitch = j;
			}
		
			translate_measure(MeasureHandle, row + pitch, col);

			measure_pairs(*image, MeasureHandle, HTuple(3), HTuple(thresh), HTuple("negative_strongest"), HTuple("all"),
				&RowEdgeFirstLR,&ColumnEdgeFirstLR,&AmplitudeFirst,  
				&RowEdgeSecondLR,&ColumnEdgeSecondLR,&AmplitudeSecond,
				&IntraDistance, &InterDistance);

			edgeCnt = RowEdgeFirstLR.Num();
			if(0 < edgeCnt)
			{
				set_color(disp, HTuple("green"));
				set_draw(disp, HTuple("margin"));

				disp_line(disp, RowEdgeFirstLR[0], ColumnEdgeFirstLR[0], RowEdgeSecondLR[0], ColumnEdgeSecondLR[0]);

				set_color(disp, HTuple("red"));
				set_draw(disp, HTuple("margin"));

				if(up_direct)
				{
					//위에서서 아래(up) 이미지 상에서는 왼쪽에서 오른쪽
					//RowEdgeFirstLR[0], ColumnEdgeFirstLR[0]

					rslt_x[*rslt_num] = ColumnEdgeFirstLR[0];
					rslt_y[*rslt_num] = RowEdgeFirstLR[0];

					//data.Format(_T("%d,UP,%.3f,%.3f"), i + 1, rslt_x[*rslt_num], rslt_y[*rslt_num]);
					//pdlg->WriteInsResultFile(file_path, data);


					disp_line(disp, rslt_y[*rslt_num] - 5, rslt_x[*rslt_num], rslt_y[*rslt_num] + 5, rslt_x[*rslt_num]);
					disp_line(disp, rslt_y[*rslt_num], rslt_x[*rslt_num] - 5, rslt_y[*rslt_num], rslt_x[*rslt_num] + 5);
				}
				else
				{
					//아래에서 위(down) 이미지 상에서는 오른쪽에서 왼쪽
					//RowEdgeSecondLR[0], ColumnEdgeSecondLR[0]

					rslt_x[*rslt_num] = ColumnEdgeSecondLR[0];
					rslt_y[*rslt_num] = RowEdgeSecondLR[0];

					//data.Format(_T("%d,DW,%.3f,%.3f"), i + 1, rslt_x[*rslt_num], rslt_y[*rslt_num]);
					//pdlg->WriteInsResultFile(file_path, data);

					disp_line(disp, rslt_y[*rslt_num] - 5, rslt_x[*rslt_num], rslt_y[*rslt_num] + 5, rslt_x[*rslt_num]);
					disp_line(disp, rslt_y[*rslt_num], rslt_x[*rslt_num] - 5, rslt_y[*rslt_num], rslt_x[*rslt_num] + 5);
				}

				if(DEFECT_DATA_MAX_NUM < *rslt_num)
				{
					break;
				}

				

			
			}
			else
			{
				rslt_x[*rslt_num] = 0.;
				rslt_y[*rslt_num] = 0.;

				/*
				if(up_direct)
				{
					data.Format(_T("%d,UP,%.3f,%.3f"), i + 1, rslt_x[*rslt_num], rslt_y[*rslt_num]);
					pdlg->WriteInsResultFile(file_path, data);
				}
				else
				{
					data.Format(_T("%d,DW,%.3f,%.3f"), i + 1, rslt_x[*rslt_num], rslt_y[*rslt_num]);
					pdlg->WriteInsResultFile(file_path, data);
				}
				*/

				rslt = FALSE;
			}

			(*rslt_num)++;
		}
	}
	close_measure(MeasureHandle);

	

	return rslt;
}

void CDialogRecipeScan::ClickBtnenhctrlPanelScanApplyToGlass()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	Hobject scan_rgn;

	if(0 > (m_edit_scan_idx - 1))
	{
		m_edit_scan_idx = 1;
	}
	GetRegionGlassfromScan_coordXY(m_tab_type_idx, m_edit_scan_idx - 1, GetScanPitch_coordXY(m_tab_type_idx), m_DefectRegion, pdlg->m_Glass->GetDefectRgn());
	GetRegionGlassfromScan_coordXY(m_tab_type_idx, m_edit_scan_idx - 1, GetScanPitch_coordXY(m_tab_type_idx), m_InspectRegion, pdlg->m_Glass->GetInspectRgn());

}

void CDialogRecipeScan::GetRegionGlassfromScan_coordXY(int type_idx, int pitch_idx, double scan_wd, Hobject scan_rgn, Hobject *glass_rgn)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	HTuple HomMat2DIdentity, HomMat2DTrans, HomMat2DScale;
	double wd;
	double x, y, scan_x, scan_y, glass_x, glass_y;
	double offset_x, offset_y;
	double ratio_x, ratio_y;

	ratio_x = GetScanPixelX_coordXY(type_idx) / 0.1;
	ratio_y = GetScanPixelY_coordXY(type_idx) / 0.1;

	//Scan영상 좌상점을 glass어디 위치로 보내는 문제이다.
	glass_x = -GetScanGlassX_coordXY(type_idx) - ( GetScanImageX_coordXY(type_idx) * GetScanPixelX_coordXY(type_idx));
	glass_y = GetScanGlassY_coordXY(type_idx) + (GetScanImageY_coordXY(type_idx) * GetScanPixelY_coordXY(type_idx));
	
	//Glass to glass image
	pdlg->m_Glass->GetCoordGlassToDisp(glass_x, glass_y, &x, &y);

	//Region 이동///////////////////////////////////////////////////////////////
	hom_mat2d_identity(&HomMat2DIdentity);
	hom_mat2d_scale(HomMat2DIdentity, ratio_y, ratio_x, 0, 0, &HomMat2DScale);
	hom_mat2d_translate(HomMat2DScale, y, x, &HomMat2DTrans);
	affine_trans_region(scan_rgn, glass_rgn, HomMat2DTrans, HTuple("false"));
}

void CDialogRecipeScan::SetUpdate()
{
	int i, j;

	for(i = 0; i < RECIPE_SCAN_TYPE_MAX_NUM; i++)
	{
		for(j = 0; j < DEFECT_MAX_SCAN_NUM; j++)
		{
			MakeGlassDat(i, j, FALSE);
		}
	}
}

int CDialogRecipeScan::GetBlackWhiteDefectCnt(int type_idx)
{
	return m_ScanDat[type_idx].m_BlackWhiteCnt;
}

double CDialogRecipeScan::GetDefectSmall(int type_idx)
{
	//return (double)m_edit_recipe_defect_size_small;
	return m_ScanDat[type_idx].m_dDefect_size_small;
}

double CDialogRecipeScan::GetDefectMedium(int type_idx)
{
	//return (double)m_edit_recipe_defect_size_medium;
	return m_ScanDat[type_idx].m_dDefect_size_medium;
}

double CDialogRecipeScan::GetDefectLarge(int type_idx)
{
	//return (double)m_edit_recipe_defect_size_large;
	return m_ScanDat[type_idx].m_dDefect_size_large;
}

int CDialogRecipeScan::GetInspectUse(int type_idx)
{
	return (int)m_ScanDat[type_idx].m_bInspectUse;
}

int CDialogRecipeScan::GetInspect90Degree(int type_idx)
{
	return (int)m_ScanDat[type_idx].m_bInspect90Degree;
}

double CDialogRecipeScan::GetRoatateOffset(int type_idx)
{
	return m_ScanDat[type_idx].m_dScanRotateOffset;
}

void CDialogRecipeScan::ParameterCaptionChange(int type_idx)
{
	switch(type_idx)
	{
	case 0:
	case 1:
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN12)->SetWindowTextW(_T("불량(주름, 표면결함) 감도(FACTOR)"));
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN13)->SetWindowTextW(_T("불량 SIZE(SMALL, MEDIUM, LARGE)(Area)(mm^2)"));
		break;
	case 2:
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN12)->SetWindowTextW(_T("2차 Dam -> CVD Line 거리(mm) (up, down)"));
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN13)->SetWindowTextW(_T("Spec (mm) (x, y)"));
		break;
	case 3:
	case 4:

		break;
	default:
		AfxMessageBox(_T("ParameterCaptionChange Index Error"));
		break;
	}
	ParameterVisible(type_idx);
}

void CDialogRecipeScan::ParameterVisible(int type_idx)
{
	switch(type_idx)
	{
	case 0:
	case 1:
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_THRSH0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_THRSH1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_THRSH2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_THRSH3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_THRSH4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_DEFECT_SIZE_SMALL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_DEFECT_SIZE_MEDIUM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_DEFECT_SIZE_LARGE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN16)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN17)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN23)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN24)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN25)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN26)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN13)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN19)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_DEFECT_DISTANCE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_DEFECT_COMPAR_DISTANCE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_OF_WIDTH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_OF_HEIGHT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN12)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_SIZE0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_SIZE1)->ShowWindow(SW_SHOW);
		break;
	case 2:
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_THRSH0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_THRSH1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_THRSH2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_THRSH3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_THRSH4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_DEFECT_SIZE_SMALL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_DEFECT_SIZE_MEDIUM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_DEFECT_SIZE_LARGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN16)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN17)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN23)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN24)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN25)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN26)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN13)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN19)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_DEFECT_DISTANCE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_DEFECT_COMPAR_DISTANCE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_OF_WIDTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_OF_HEIGHT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN12)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_SIZE0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_SIZE1)->ShowWindow(SW_SHOW);
		break;
	case 3:
	case 4:
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_THRSH0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_THRSH1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_THRSH2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_THRSH3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_THRSH4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_DEFECT_SIZE_SMALL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_DEFECT_SIZE_MEDIUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_DEFECT_SIZE_LARGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN16)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN17)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN23)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN24)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN25)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN26)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN13)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN19)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_DEFECT_SIZE5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_CNT5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_DEFECT_DISTANCE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_DEFECT_COMPAR_DISTANCE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_OF_WIDTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_OF_HEIGHT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_TITLE_RECIPE_SCAN12)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_SIZE0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECIPE_SCAN_SIZE1)->ShowWindow(SW_HIDE);
		break;
	default:
		AfxMessageBox(_T(" ParameterVisible Index Error"));
		break;
	}

	
}

void CDialogRecipeScan::ClickBtnenhctrlRecipeScanTab0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_tab_0.SetValue(TRUE);
	m_tab_1.SetValue(FALSE);
	m_tab_2.SetValue(FALSE);
	m_tab_3.SetValue(FALSE);
	m_tab_4.SetValue(FALSE);

	m_tab_type_idx = 0;

	Update(FALSE);
	UpdateData(FALSE);

	for(int i = 0; i < RECIPE_SCAN_IMG_RECT_MAX_NUM; i++)
	{
		m_dlgImgRect[i]->ShowWindow(SW_HIDE);
	}

	CRect rect;

	GetDlgItem(IDC_STATIC_SCAN)->GetClientRect(rect);
	GetDlgItem(IDC_STATIC_SCAN)->ClientToScreen(rect);

	m_dlgImgRect[m_tab_type_idx]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);

	m_dlgImgRect[m_tab_type_idx]->ShowWindow(SW_SHOW);
	ParameterCaptionChange(0);
	DispGlassDat();

}


void CDialogRecipeScan::ClickBtnenhctrlRecipeScanTab1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_tab_0.SetValue(FALSE);
	m_tab_1.SetValue(TRUE);
	m_tab_2.SetValue(FALSE);
	m_tab_3.SetValue(FALSE);
	m_tab_4.SetValue(FALSE);

	m_tab_type_idx = 1;

	Update(FALSE);
	UpdateData(FALSE);

	for(int i = 0; i < RECIPE_SCAN_IMG_RECT_MAX_NUM; i++)
	{
		m_dlgImgRect[i]->ShowWindow(SW_HIDE);
	}

	CRect rect;

	GetDlgItem(IDC_STATIC_SCAN)->GetClientRect(rect);
	GetDlgItem(IDC_STATIC_SCAN)->ClientToScreen(rect);

	m_dlgImgRect[m_tab_type_idx]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);

	m_dlgImgRect[m_tab_type_idx]->ShowWindow(SW_SHOW);
	ParameterCaptionChange(1);
	DispGlassDat();
}


void CDialogRecipeScan::ClickBtnenhctrlRecipeScanTab2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_tab_0.SetValue(FALSE);
	m_tab_1.SetValue(FALSE);
	m_tab_2.SetValue(TRUE);
	m_tab_3.SetValue(FALSE);
	m_tab_4.SetValue(FALSE);

	m_tab_type_idx = 2;

	Update(FALSE);
	UpdateData(FALSE);

	for(int i = 0; i < RECIPE_SCAN_IMG_RECT_MAX_NUM; i++)
	{
		m_dlgImgRect[i]->ShowWindow(SW_HIDE);
	}

	CRect rect;

	GetDlgItem(IDC_STATIC_SCAN)->GetClientRect(rect);
	GetDlgItem(IDC_STATIC_SCAN)->ClientToScreen(rect);

	m_dlgImgRect[m_tab_type_idx]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);

	m_dlgImgRect[m_tab_type_idx]->ShowWindow(SW_SHOW);
	ParameterCaptionChange(2);
	DispGlassDat();
}


void CDialogRecipeScan::ClickBtnenhctrlRecipeScanTab3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_tab_0.SetValue(FALSE);
	m_tab_1.SetValue(FALSE);
	m_tab_2.SetValue(FALSE);
	m_tab_3.SetValue(TRUE);
	m_tab_4.SetValue(FALSE);

	m_tab_type_idx = 3;

	Update(FALSE);
	UpdateData(FALSE);

	for(int i = 0; i < RECIPE_SCAN_IMG_RECT_MAX_NUM; i++)
	{
		m_dlgImgRect[i]->ShowWindow(SW_HIDE);
	}

	CRect rect;

	GetDlgItem(IDC_STATIC_SCAN)->GetClientRect(rect);
	GetDlgItem(IDC_STATIC_SCAN)->ClientToScreen(rect);

	m_dlgImgRect[m_tab_type_idx]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);

	m_dlgImgRect[m_tab_type_idx]->ShowWindow(SW_SHOW);
	ParameterCaptionChange(3);
}

void CDialogRecipeScan::ClickBtnenhctrlRecipeScanTab4()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_tab_0.SetValue(FALSE);
	m_tab_1.SetValue(FALSE);
	m_tab_2.SetValue(FALSE);
	m_tab_3.SetValue(FALSE);
	m_tab_4.SetValue(TRUE);

	m_tab_type_idx = 4;

	Update(FALSE);
	UpdateData(FALSE);

	for(int i = 0; i < RECIPE_SCAN_IMG_RECT_MAX_NUM; i++)
	{
		m_dlgImgRect[i]->ShowWindow(SW_HIDE);
	}

	CRect rect;

	GetDlgItem(IDC_STATIC_SCAN)->GetClientRect(rect);
	GetDlgItem(IDC_STATIC_SCAN)->ClientToScreen(rect);

	m_dlgImgRect[m_tab_type_idx]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);

	m_dlgImgRect[m_tab_type_idx]->ShowWindow(SW_SHOW);
	ParameterCaptionChange(4);
}

//2010412 ngh
void CDialogRecipeScan::SetUpdate(BOOL isUpdate)
{
	Update(isUpdate);
}


void CDialogRecipeScan::ParameterCompare(int type_idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strType;
	if(type_idx < 2)
	{
		strType.Format(_T("RecipeScan_Para_8K_Type_%d_"), type_idx);
	}
	else if(type_idx == 2)
	{
		strType.Format(_T("RecipeScan_Para_16K_CVD_"));
	}
	else if(type_idx > 2)
	{
		strType.Format(_T("RectipScan_Para_16K_PCL_Type_%d_"), type_idx - 3);
	}
	
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dScan_start_x, m_edit_recipe_scan_start_x, strType + _T("m_edit_recipe_scan_start_x"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dScan_start_y, m_edit_recipe_scan_start_y, strType + _T("m_edit_recipe_scan_start_y"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dScan_leng, m_edit_recipe_scan_leng, strType + _T("m_edit_recipe_scan_leng"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dScan_pitch, m_edit_recipe_scan_pitch, strType + _T("m_edit_recipe_scan_pitch"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dScan_overlay, m_edit_recipe_scan_overlay, strType + _T("m_edit_recipe_scan_overlay"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dScan_overlay2, m_edit_recipe_scan_overlay2, strType + _T("m_edit_recipe_scan_overlay2"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dScan_pixel_x, m_edit_recipe_scan_pixel_x, strType + _T("m_edit_recipe_scan_pixel_x"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dScan_pixel_y, m_edit_recipe_scan_pixel_y, strType + _T("m_edit_recipe_scan_pixel_y"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dScan_cnt, m_edit_recipe_scan_cnt, strType + _T("m_edit_recipe_scan_cnt"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dScan_coord_x, m_edit_recipe_scan_coord_x, strType + _T("m_edit_recipe_scan_coord_x"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dScan_coord_y, m_edit_recipe_scan_coord_y, strType + _T("m_edit_recipe_scan_coord_y"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dScan_glass_x, m_edit_recipe_glass_x, strType + _T("m_edit_recipe_glass_x"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dScan_glass_y, m_edit_recipe_glass_y, strType + _T("m_edit_recipe_glass_y"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dThrsh0, m_edit_thrsh0, strType + _T("m_edit_thrsh0"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dThrsh1, m_edit_thrsh1, strType + _T("m_edit_thrsh1"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dThrsh2, m_edit_thrsh2, strType + _T("m_edit_thrsh2"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dThrsh3, m_edit_thrsh3, strType + _T("m_edit_thrsh3"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dThrsh4, m_edit_thrsh4, strType + _T("m_edit_thrsh4"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dDefect_size0, m_edit_defect_size0, strType + _T("m_edit_defect_size0"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dDefect_size1, m_edit_defect_size1, strType + _T("m_edit_defect_size1"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dDefect_size2, m_edit_defect_size2, strType + _T("m_edit_defect_size2"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dDefect_size3, m_edit_defect_size3, strType + _T("m_edit_defect_size3"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dDefect_size4, m_edit_defect_size4, strType + _T("m_edit_defect_size4"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dDefect_size_small, m_edit_recipe_defect_size_small, strType + _T("m_edit_recipe_defect_size_small"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dDefect_size_medium, m_edit_recipe_defect_size_medium, strType + _T("m_edit_recipe_defect_size_medium"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dDefect_size_large, m_edit_recipe_defect_size_large, strType + _T("m_edit_recipe_defect_size_large"));

	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dLight0, m_edit_light0, strType + _T("m_edit_light0"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_bInspectUse, m_check_inspect_use, strType + _T("m_check_inspect_use"));	
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dScanRotateOffset, m_edit_rotate_theta, strType + _T("m_edit_rotate_theta"));

	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_dScanEziPos,  m_edit_recipe_scan_ezi_pos, strType + _T("m_edit_recipe_scan_ezi_pos"));

	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_ErrorSizeSelect[0], m_check_bSmallSize, strType + _T("m_check_bSmallSize"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_ErrorSizeSelect[1], m_check_bMediumSize, strType + _T("m_check_bMediumSize"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_ErrorSizeSelect[2], m_check_bLargeSize, strType + _T("m_check_bLargeSize"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_ErrorSizeSelect[3], m_check_bOverflowSize, strType + _T("m_check_bOverflowSize"));

	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_BlackWhiteCnt, m_edit_nSelectDefectErrorCnt4, strType + _T("m_edit_nSelectDefectErrorCnt4"));
		
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_SelectDefectErrorCnt[0], m_edit_nSelectDefectErrorCnt0, strType + _T("m_edit_nSelectDefectErrorCnt0"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_SelectDefectErrorCnt[1], m_edit_nSelectDefectErrorCnt1, strType + _T("m_edit_nSelectDefectErrorCnt1"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_SelectDefectErrorCnt[2], m_edit_nSelectDefectErrorCnt2, strType + _T("m_edit_nSelectDefectErrorCnt2"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_SelectDefectErrorCnt[3], m_edit_nSelectDefectErrorCnt3, strType + _T("m_edit_nSelectDefectErrorCnt3"));

	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_bErrorBlackWhite, m_check_bBlackWhite, strType + _T("m_check_bBlackWhite"));

	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_NoiseSize[0], m_edit_nNoise0, strType + _T("m_edit_nNoise0"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_NoiseSize[1], m_edit_nNoise1, strType + _T("m_edit_nNoise1"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_NoiseSize[2], m_edit_nNoise2, strType + _T("m_edit_nNoise2"));

	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_MargeDistance, m_edit_dMargeDistance, strType + _T("m_edit_dMargeDistance"));

	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_ReduceSelect, m_combo_ReduceSelect.GetCurSel(), strType + _T("m_combo_ReduceSelect"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_ReduceSize, m_edit_reduce_size, strType + _T("m_edit_reduce_size"));

	//20160406 ngh
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_MaskDefectDistance, m_edit_dMaskDefectDistance, strType + _T("m_edit_dMaskDefectDistance"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_MaskDefectCnt, m_edit_nSelectDefectErrorCnt5, strType + _T("m_edit_nSelectDefectErrorCnt5"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_bMaskDefect, m_check_bMaskDefect, strType + _T("m_check_bMaskDefect"));

	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_OFDefectSizeWidth, m_edit_OFWidth, strType + _T("m_edit_OFWidth"));
	pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_OFDefectSizeHeight, m_edit_OFHeight, strType + _T("m_edit_OFHeight"));


	switch(type_idx)
	{
	case 0:
		m_check_inspect_90degree, FALSE;
		pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_bInspect90Degree, m_check_inspect_90degree, strType + _T("m_check_inspect_90degree"));
		break;
	case 1:
		m_check_inspect_90degree, TRUE;
		pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_bInspect90Degree, m_check_inspect_90degree, strType + _T("m_check_inspect_90degree"));
		break;
	case 2:
		pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_bInspect90Degree, m_check_inspect_90degree, strType + _T("m_check_inspect_90degree"));
		break;
	case 3:
		m_check_inspect_90degree, TRUE;
		pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_bInspect90Degree, m_check_inspect_90degree, strType + _T("m_check_inspect_90degree"));
		break;
	case 4:
		m_check_inspect_90degree, FALSE;
		pdlg->ParaCompareLogWrite(m_ScanDat[type_idx].m_bInspect90Degree, m_check_inspect_90degree, strType + _T("m_check_inspect_90degree"));
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogRecipeScan::ParameterCompare)!"));
		break;
	}
}

void CDialogRecipeScan::Update(BOOL flag)
{
	if(flag)
	{
		 m_ScanDat[m_tab_type_idx].m_dScan_start_x = m_edit_recipe_scan_start_x;
		 m_ScanDat[m_tab_type_idx].m_dScan_start_y = m_edit_recipe_scan_start_y;
		 m_ScanDat[m_tab_type_idx].m_dScan_leng = m_edit_recipe_scan_leng;
		 m_ScanDat[m_tab_type_idx].m_dScan_pitch = m_edit_recipe_scan_pitch;
		 m_ScanDat[m_tab_type_idx].m_dScan_overlay = m_edit_recipe_scan_overlay;
		 m_ScanDat[m_tab_type_idx].m_dScan_overlay2 = m_edit_recipe_scan_overlay2;
		 m_ScanDat[m_tab_type_idx].m_dScan_pixel_x = m_edit_recipe_scan_pixel_x;
		 m_ScanDat[m_tab_type_idx].m_dScan_pixel_y = m_edit_recipe_scan_pixel_y;
		 m_ScanDat[m_tab_type_idx].m_dScan_cnt = m_edit_recipe_scan_cnt;
		 m_ScanDat[m_tab_type_idx].m_dScan_coord_x = m_edit_recipe_scan_coord_x;
		 m_ScanDat[m_tab_type_idx].m_dScan_coord_y = m_edit_recipe_scan_coord_y;
		 m_ScanDat[m_tab_type_idx].m_dScan_glass_x = m_edit_recipe_glass_x;
		 m_ScanDat[m_tab_type_idx].m_dScan_glass_y = m_edit_recipe_glass_y;
		 m_ScanDat[m_tab_type_idx].m_dThrsh0 = m_edit_thrsh0;
		 m_ScanDat[m_tab_type_idx].m_dThrsh1 = m_edit_thrsh1;
		 m_ScanDat[m_tab_type_idx].m_dThrsh2 = m_edit_thrsh2;
		 m_ScanDat[m_tab_type_idx].m_dThrsh3 = m_edit_thrsh3;
		 m_ScanDat[m_tab_type_idx].m_dThrsh4 = m_edit_thrsh4;
		 m_ScanDat[m_tab_type_idx].m_dDefect_size0 = m_edit_defect_size0;
		 m_ScanDat[m_tab_type_idx].m_dDefect_size1 = m_edit_defect_size1;
		 m_ScanDat[m_tab_type_idx].m_dDefect_size2 = m_edit_defect_size2;
		 m_ScanDat[m_tab_type_idx].m_dDefect_size3 = m_edit_defect_size3;
		 m_ScanDat[m_tab_type_idx].m_dDefect_size4 = m_edit_defect_size4;
		 m_ScanDat[m_tab_type_idx].m_dDefect_size_small = m_edit_recipe_defect_size_small;
		 m_ScanDat[m_tab_type_idx].m_dDefect_size_medium = m_edit_recipe_defect_size_medium;
		 m_ScanDat[m_tab_type_idx].m_dDefect_size_large = m_edit_recipe_defect_size_large;

		 m_ScanDat[m_tab_type_idx].m_dLight0 = m_edit_light0;
		 m_ScanDat[m_tab_type_idx].m_bInspectUse = m_check_inspect_use;	
		 m_ScanDat[m_tab_type_idx].m_dScanRotateOffset = m_edit_rotate_theta;

		 m_ScanDat[m_tab_type_idx].m_dScanEziPos =  m_edit_recipe_scan_ezi_pos;

		 m_ScanDat[m_tab_type_idx].m_ErrorSizeSelect[0] = m_check_bSmallSize;
		 m_ScanDat[m_tab_type_idx].m_ErrorSizeSelect[1] = m_check_bMediumSize;
		 m_ScanDat[m_tab_type_idx].m_ErrorSizeSelect[2] = m_check_bLargeSize;
		 m_ScanDat[m_tab_type_idx].m_ErrorSizeSelect[3] = m_check_bOverflowSize;

		 m_ScanDat[m_tab_type_idx].m_BlackWhiteCnt = m_edit_nSelectDefectErrorCnt4;
		
		 m_ScanDat[m_tab_type_idx].m_SelectDefectErrorCnt[0] = m_edit_nSelectDefectErrorCnt0;
		 m_ScanDat[m_tab_type_idx].m_SelectDefectErrorCnt[1] = m_edit_nSelectDefectErrorCnt1;
		 m_ScanDat[m_tab_type_idx].m_SelectDefectErrorCnt[2] = m_edit_nSelectDefectErrorCnt2;
		 m_ScanDat[m_tab_type_idx].m_SelectDefectErrorCnt[3] = m_edit_nSelectDefectErrorCnt3;

		 m_ScanDat[m_tab_type_idx].m_bErrorBlackWhite = m_check_bBlackWhite;

		 m_ScanDat[m_tab_type_idx].m_NoiseSize[0] = m_edit_nNoise0;
		 m_ScanDat[m_tab_type_idx].m_NoiseSize[1] = m_edit_nNoise1;
		 m_ScanDat[m_tab_type_idx].m_NoiseSize[2] = m_edit_nNoise2;

		 m_ScanDat[m_tab_type_idx].m_MargeDistance = m_edit_dMargeDistance;

		 m_ScanDat[m_tab_type_idx].m_ReduceSelect = m_combo_ReduceSelect.GetCurSel();
		 m_ScanDat[m_tab_type_idx].m_ReduceSize = m_edit_reduce_size;

		 //20160406 ngh
		 m_ScanDat[m_tab_type_idx].m_MaskDefectDistance = m_edit_dMaskDefectDistance;
		 m_ScanDat[m_tab_type_idx].m_MaskDefectCnt = m_edit_nSelectDefectErrorCnt5;
		 m_ScanDat[m_tab_type_idx].m_bMaskDefect = m_check_bMaskDefect;

		 m_ScanDat[m_tab_type_idx].m_OFDefectSizeWidth = m_edit_OFWidth;
		 m_ScanDat[m_tab_type_idx].m_OFDefectSizeHeight = m_edit_OFHeight;

		 //20160719 ngh
		 m_ScanDat[m_tab_type_idx].m_edit_dDefectCompare_Distance = m_edit_dDefectCompare_Distance;


		 switch(m_tab_type_idx)
		 {
		 case 0:
			 m_check_inspect_90degree = FALSE;	
			 m_ScanDat[m_tab_type_idx].m_bInspect90Degree = m_check_inspect_90degree;
			 break;
		 case 1:
			 m_check_inspect_90degree = TRUE;
			 m_ScanDat[m_tab_type_idx].m_bInspect90Degree = m_check_inspect_90degree;
			 break;
		 case 2:
			 m_ScanDat[m_tab_type_idx].m_bInspect90Degree = m_check_inspect_90degree;
			 break;
		 case 3:
			 m_check_inspect_90degree = TRUE;	
			 m_ScanDat[m_tab_type_idx].m_bInspect90Degree = m_check_inspect_90degree;
			 break;
		 case 4:
			 m_check_inspect_90degree = FALSE;
			 m_ScanDat[m_tab_type_idx].m_bInspect90Degree = m_check_inspect_90degree;
			 break;
		 default:
			 AfxMessageBox(_T("Error : invalid index(CDialogRecipeScan::Update)!"));
			 break;
		 }
	}
	else
	{
		 m_edit_recipe_scan_start_x = m_ScanDat[m_tab_type_idx].m_dScan_start_x;
		 m_edit_recipe_scan_start_y = m_ScanDat[m_tab_type_idx].m_dScan_start_y;
		 m_edit_recipe_scan_leng = m_ScanDat[m_tab_type_idx].m_dScan_leng;   
		 m_edit_recipe_scan_pitch = m_ScanDat[m_tab_type_idx].m_dScan_pitch;  
		 m_edit_recipe_scan_overlay = m_ScanDat[m_tab_type_idx].m_dScan_overlay;
		 m_edit_recipe_scan_overlay2 = m_ScanDat[m_tab_type_idx].m_dScan_overlay2;
		 m_edit_recipe_scan_pixel_x = m_ScanDat[m_tab_type_idx].m_dScan_pixel_x;
		 m_edit_recipe_scan_pixel_y = m_ScanDat[m_tab_type_idx].m_dScan_pixel_y;
		 m_edit_recipe_scan_cnt = m_ScanDat[m_tab_type_idx].m_dScan_cnt;    
		 m_edit_recipe_scan_coord_x = m_ScanDat[m_tab_type_idx].m_dScan_coord_x;
		 m_edit_recipe_scan_coord_y = m_ScanDat[m_tab_type_idx].m_dScan_coord_y;
		 m_edit_recipe_glass_x = m_ScanDat[m_tab_type_idx].m_dScan_glass_x;
		 m_edit_recipe_glass_y = m_ScanDat[m_tab_type_idx].m_dScan_glass_y;
		 m_edit_thrsh0 = m_ScanDat[m_tab_type_idx].m_dThrsh0;      
		 m_edit_thrsh1 = m_ScanDat[m_tab_type_idx].m_dThrsh1;      
		 m_edit_thrsh2 = m_ScanDat[m_tab_type_idx].m_dThrsh2;      
		 m_edit_thrsh3 = m_ScanDat[m_tab_type_idx].m_dThrsh3;      
		 m_edit_thrsh4 = m_ScanDat[m_tab_type_idx].m_dThrsh4;      
		 m_edit_defect_size0 = m_ScanDat[m_tab_type_idx].m_dDefect_size0;
		 m_edit_defect_size1 = m_ScanDat[m_tab_type_idx].m_dDefect_size1;
		 m_edit_defect_size2 = m_ScanDat[m_tab_type_idx].m_dDefect_size2;
		 m_edit_defect_size3 = m_ScanDat[m_tab_type_idx].m_dDefect_size3;
		 m_edit_defect_size4 = m_ScanDat[m_tab_type_idx].m_dDefect_size4;
		 m_edit_recipe_defect_size_small = m_ScanDat[m_tab_type_idx].m_dDefect_size_small;
		 m_edit_recipe_defect_size_medium = m_ScanDat[m_tab_type_idx].m_dDefect_size_medium;
		 m_edit_recipe_defect_size_large = m_ScanDat[m_tab_type_idx].m_dDefect_size_large;

		 m_edit_light0 = m_ScanDat[m_tab_type_idx].m_dLight0;
		 m_check_inspect_use = m_ScanDat[m_tab_type_idx].m_bInspectUse;	
		 m_edit_rotate_theta = m_ScanDat[m_tab_type_idx].m_dScanRotateOffset;

		 m_edit_recipe_scan_ezi_pos = m_ScanDat[m_tab_type_idx].m_dScanEziPos;

		 m_check_bSmallSize = m_ScanDat[m_tab_type_idx].m_ErrorSizeSelect[0];
		 m_check_bMediumSize = m_ScanDat[m_tab_type_idx].m_ErrorSizeSelect[1];
		 m_check_bLargeSize = m_ScanDat[m_tab_type_idx].m_ErrorSizeSelect[2];
		 m_check_bOverflowSize = m_ScanDat[m_tab_type_idx].m_ErrorSizeSelect[3];

		 m_edit_nSelectDefectErrorCnt4 = m_ScanDat[m_tab_type_idx].m_BlackWhiteCnt;
		
		 m_edit_nSelectDefectErrorCnt0 = m_ScanDat[m_tab_type_idx].m_SelectDefectErrorCnt[0];
		 m_edit_nSelectDefectErrorCnt1 = m_ScanDat[m_tab_type_idx].m_SelectDefectErrorCnt[1];
		 m_edit_nSelectDefectErrorCnt2 = m_ScanDat[m_tab_type_idx].m_SelectDefectErrorCnt[2];
		 m_edit_nSelectDefectErrorCnt3 = m_ScanDat[m_tab_type_idx].m_SelectDefectErrorCnt[3];

		 m_check_bBlackWhite = m_ScanDat[m_tab_type_idx].m_bErrorBlackWhite;

		 m_edit_nNoise0 = m_ScanDat[m_tab_type_idx].m_NoiseSize[0];
		 m_edit_nNoise1 = m_ScanDat[m_tab_type_idx].m_NoiseSize[1];
		 m_edit_nNoise2 = m_ScanDat[m_tab_type_idx].m_NoiseSize[2];

		 m_edit_dMargeDistance = m_ScanDat[m_tab_type_idx].m_MargeDistance;


		 m_combo_ReduceSelect.SetCurSel(m_ScanDat[m_tab_type_idx].m_ReduceSelect);
		 m_edit_reduce_size = m_ScanDat[m_tab_type_idx].m_ReduceSize;


		  //20160406 ngh
		 m_edit_dMaskDefectDistance = m_ScanDat[m_tab_type_idx].m_MaskDefectDistance;
		 m_edit_nSelectDefectErrorCnt5 = m_ScanDat[m_tab_type_idx].m_MaskDefectCnt;
		 m_check_bMaskDefect = m_ScanDat[m_tab_type_idx].m_bMaskDefect;

		 m_edit_OFWidth = m_ScanDat[m_tab_type_idx].m_OFDefectSizeWidth;
		 m_edit_OFHeight = m_ScanDat[m_tab_type_idx].m_OFDefectSizeHeight;

		 //20160719 ngh
		 m_edit_dDefectCompare_Distance = m_ScanDat[m_tab_type_idx].m_edit_dDefectCompare_Distance;

		 switch(m_tab_type_idx)
		 {
		 case 0:
			 m_ScanDat[m_tab_type_idx].m_bInspect90Degree = FALSE;
			 m_check_inspect_90degree = m_ScanDat[m_tab_type_idx].m_bInspect90Degree;	
			 break;
		 case 1:
			 m_ScanDat[m_tab_type_idx].m_bInspect90Degree = TRUE;
			 m_check_inspect_90degree = m_ScanDat[m_tab_type_idx].m_bInspect90Degree;	
			 break;
		 case 2:
			 m_check_inspect_90degree = m_ScanDat[m_tab_type_idx].m_bInspect90Degree;	
			 break;
		 case 3:
			 m_ScanDat[m_tab_type_idx].m_bInspect90Degree = TRUE;
			 m_check_inspect_90degree = m_ScanDat[m_tab_type_idx].m_bInspect90Degree;	
			 break;
		 case 4:
			 m_ScanDat[m_tab_type_idx].m_bInspect90Degree = FALSE;
			 m_check_inspect_90degree = m_ScanDat[m_tab_type_idx].m_bInspect90Degree;	
			 break;
		 default:
			 AfxMessageBox(_T("Error : invalid index(CDialogRecipeScan::Update)!"));
			 break;
		 }
	}
}

typeRecipeScan CDialogRecipeScan::GetRecipe(int type_idx)
{
	typeRecipeScan dat;

	dat.dScan_start_x = GetScanStartX(type_idx);
	dat.dScan_start_y = GetScanStartY(type_idx);
	dat.dScan_leng = GetScanLeng(type_idx);;
	dat.dScan_pitch = GetScanPitch(type_idx);
	dat.dScan_pixel_x = GetScanPixelX(type_idx);
	dat.dScan_pixel_y = GetScanPixelY(type_idx);
	dat.dScan_cnt = GetScanCnt(type_idx);
	dat.dScan_coord_x = GetScanImageX(type_idx);
	dat.dScan_coord_y = GetScanImageY(type_idx);
	dat.dScan_glass_x = GetScanGlassX(type_idx);
	dat.dScan_glass_y = GetScanGlassY(type_idx);
	dat.dThrsh0 = m_ScanDat[type_idx].m_dThrsh0;
	dat.dThrsh1 = m_ScanDat[type_idx].m_dThrsh1;
	dat.dThrsh2 = m_ScanDat[type_idx].m_dThrsh2;
	dat.dThrsh3 = m_ScanDat[type_idx].m_dThrsh3;
	dat.dThrsh4 = m_ScanDat[type_idx].m_dThrsh4;
	dat.dDefect_size0 = m_ScanDat[type_idx].m_dDefect_size0;
	dat.dDefect_size1 = m_ScanDat[type_idx].m_dDefect_size1;
	dat.dDefect_size2 = m_ScanDat[type_idx].m_dDefect_size2;
	dat.dDefect_size3 = m_ScanDat[type_idx].m_dDefect_size3;
	dat.dDefect_size4 = m_ScanDat[type_idx].m_dDefect_size4;
	dat.dDefect_size_small = m_ScanDat[type_idx].m_dDefect_size_small;
	dat.dDefect_size_medium = m_ScanDat[type_idx].m_dDefect_size_medium;
	dat.dDefect_size_large = m_ScanDat[type_idx].m_dDefect_size_large;

	return dat;
}



BOOL CDialogRecipeScan::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogRecipeScan::ClickBtnenhctrlPanelScanEziSetPos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	m_edit_recipe_scan_ezi_pos = pdlg->m_AFCtrl->GetEziPos_mm();

	UpdateData(FALSE);
	Update(FALSE);
}


void CDialogRecipeScan::ClickBtnenhctrlPanelScanMovePos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	pdlg->m_AFCtrl->SetEziMove_mm(m_edit_recipe_scan_ezi_pos);
	UpdateData(FALSE);
}

void CDialogRecipeScan::ClickBtnenhctrlWindowResize()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}

//20160313 ngh
BOOL CDialogRecipeScan::GetDefectCntError(int type_idx, int nDefectIndex, int nDefectCnt)
{
	BOOL rslt = FALSE;

	if(nDefectIndex == 4)
	{
		if(nDefectCnt >= m_ScanDat[type_idx].m_BlackWhiteCnt)
		{
			rslt = TRUE;
		}
	}
	else
	{
		if(nDefectCnt >= m_ScanDat[type_idx].m_SelectDefectErrorCnt[nDefectIndex])
		{
			rslt = TRUE;
		}
	}

	return rslt;
}

BOOL CDialogRecipeScan::GetSelectDefectErrorSize(int type_idx, int defect_idx)
{
	if(defect_idx == 4)
	{
		return m_ScanDat[m_tab_type_idx].m_bErrorBlackWhite;
	}
	else if(defect_idx == 5)
	{
		return m_ScanDat[m_tab_type_idx].m_bMaskDefect;
	}
	else if(defect_idx == 6)
	{
		return TRUE;
	}

	return m_ScanDat[type_idx].m_ErrorSizeSelect[defect_idx];
}

int CDialogRecipeScan::GetSelectDefectErrorCnt(int type_idx, int defect_idx)
{
	if(defect_idx == 4)
	{
		return m_ScanDat[type_idx].m_BlackWhiteCnt;
	}
	return m_ScanDat[type_idx].m_SelectDefectErrorCnt[defect_idx];
}

double CDialogRecipeScan::GetReduceSize(int type_idx)
{
	return m_ScanDat[type_idx].m_ReduceSize;
}

double CDialogRecipeScan::GetMaskDefectDistance(int type_idx)
{
	return m_ScanDat[type_idx].m_MaskDefectDistance;
}

int CDialogRecipeScan::GetMaskDefectCnt(int type_idx)
{
	return m_ScanDat[type_idx].m_MaskDefectCnt;
}

void CDialogRecipeScan::GetOFDefectSize(int type_idx, double *width, double *height)
{
	*width = m_ScanDat[type_idx].m_OFDefectSizeWidth / GetScanPixelX(type_idx);
	*height = m_ScanDat[type_idx].m_OFDefectSizeHeight / GetScanPixelY(type_idx);
}

int CDialogRecipeScan::GetReduceSelect(int type_idx)
{
	return m_ScanDat[type_idx].m_ReduceSelect;
}

int CDialogRecipeScan::GetScanBright()
{
	return (int)m_edit_light0;
}

void CDialogRecipeScan::OnBnClickedRadioMain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_strGlassMainSub.Format(_T("M"));
}


void CDialogRecipeScan::OnBnClickedRadioSub()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_strGlassMainSub.Format(_T("S"));
}

CString CDialogRecipeScan::GetGlassMainsub()
{
	return m_strGlassMainSub;
}


void CDialogRecipeScan::GetThresh(int type_idx, int *rslt)
{
	double thresh[5];
	thresh[0] = m_ScanDat[type_idx].m_dThrsh0;
	thresh[1] = m_ScanDat[type_idx].m_dThrsh1;
	thresh[2] = m_ScanDat[type_idx].m_dThrsh2;
	thresh[3] = m_ScanDat[type_idx].m_dThrsh3;
	thresh[4] = m_ScanDat[type_idx].m_dThrsh4;

	rslt = (int*)thresh;
}

void CDialogRecipeScan::GetFactor(int type_idx, int *rslt)
{
	double factor[2];
	factor[0] = m_ScanDat[type_idx].m_dDefect_size0;
	factor[1] = m_ScanDat[type_idx].m_dDefect_size1;
	
	rslt = (int*)factor;
}

void CDialogRecipeScan::GetFactor(int type_idx, double *rslt)
{
	double factor[2];
	rslt[0] = m_ScanDat[type_idx].m_dDefect_size0;
	rslt[1] = m_ScanDat[type_idx].m_dDefect_size1;
	
	//rslt = factor;
}

BOOL CDialogRecipeScan::GetScanDatState(int type)
{
	return m_ScanDat[type].m_bInspectUse;
}

double CDialogRecipeScan::GetDefectCompareDistance(int type_idx)
{
	return m_ScanDat[type_idx].m_edit_dDefectCompare_Distance;
}