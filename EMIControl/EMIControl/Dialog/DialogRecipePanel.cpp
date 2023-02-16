// Dialog/DialogRecipePanel.cpp : 구현 파일입니다.
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

// CDialogRecipePanel 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogRecipePanel, CDialogEx)

CDialogRecipePanel::CDialogRecipePanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogRecipePanel::IDD, pParent)
	, m_edit_panel_name(_T(""))
	, m_edit_panel_size_x(0)
	, m_edit_panel_size_y(0)
	, m_edit_panel_load_y_pos(0)
	, m_edit_panel_load_t_pos(0)
	, m_edit_panel_load_y_pos_manu(0)
	, m_edit_panel_load_t_pos_manu(0)
	, m_edit_panel_start_x_pos(0)
	, m_edit_panel_start_y_pos(0)
	, m_edit_panel_film_start_x_pos(0)
	, m_edit_panel_film_start_y_pos(0)
	, m_edit_panel_cvd_start_x_pos(0)
	, m_edit_panel_cvd_start_y_pos(0)
	, m_edit_penel_cell_cnt_y(0)
	, m_edit_penel_cell_cnt_x(0)
	, m_edit_penel_cell_grpcnt_y(0)
	, m_edit_penel_cell_grpcnt_x(0)
	, m_edit_panel_mark_size0(0)
	, m_edit_panel_mark_size1(0)
	, m_edit_panel_mark_to_panel_pos_y0(0)
	, m_edit_panel_mark_to_panel_pos_x0(0)
	, m_edit_panel_mark_to_panel_pos_y1(0)
	, m_edit_panel_mark_to_panel_pos_x1(0)
	, m_edit_panel_cell_pitch_x(0)
	, m_edit_panel_cell_pitch_y(0)
	, m_edit_panel_cell_grppitch_x(0)
	, m_edit_panel_cell_grppitch_y(0)
	, m_edit_load_lift_up_pos(0)
	, m_edit_load_lift_dn_pos(0)
{

}

CDialogRecipePanel::~CDialogRecipePanel()
{
	SaveRecipePanelPara(_T(RECIPE_PANEL_PATH));
}

void CDialogRecipePanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PANEL_NAME, m_edit_panel_name);
	DDX_Text(pDX, IDC_EDIT_PANEL_SIZE_X, m_edit_panel_size_x);
	DDX_Text(pDX, IDC_EDIT_PANEL_SIZE_Y, m_edit_panel_size_y);
	DDX_Text(pDX, IDC_EDIT_PANEL_LOAD_POS_SCAN, m_edit_panel_load_y_pos);
	DDX_Text(pDX, IDC_EDIT_PANEL_LOAD_POS_THETA, m_edit_panel_load_t_pos);
	DDX_Text(pDX, IDC_EDIT_PANEL_LOAD_POS_SCAN_M, m_edit_panel_load_y_pos_manu);
	DDX_Text(pDX, IDC_EDIT_PANEL_LOAD_POS_THETA_M, m_edit_panel_load_t_pos_manu);
	DDX_Text(pDX, IDC_EDIT_PANEL_START_SCAN_POS, m_edit_panel_start_y_pos);
	DDX_Text(pDX, IDC_EDIT_PANEL_START_SHIFT_POS, m_edit_panel_start_x_pos);
	DDX_Text(pDX, IDC_EDIT_PANEL_FILM_START_SCAN_POS, m_edit_panel_film_start_y_pos);
	DDX_Text(pDX, IDC_EDIT_PANEL_FILM_START_SHIFT_POS, m_edit_panel_film_start_x_pos);
	DDX_Text(pDX, IDC_EDIT_PANEL_CVD_START_SCAN_POS, m_edit_panel_cvd_start_y_pos);
	DDX_Text(pDX, IDC_EDIT_PANEL_CVD_START_SHIFT_POS, m_edit_panel_cvd_start_x_pos);
	DDX_Text(pDX, IDC_EDIT_PANEL_CELL_CNT_Y, m_edit_penel_cell_cnt_y);
	DDX_Text(pDX, IDC_EDIT_PANEL_CELL_CNT_X, m_edit_penel_cell_cnt_x);
	DDX_Text(pDX, IDC_EDIT_PANEL_CELL_GRPCNT_Y, m_edit_penel_cell_grpcnt_y);
	DDX_Text(pDX, IDC_EDIT_PANEL_CELL_GRPCNT_X, m_edit_penel_cell_grpcnt_x);
	DDX_Text(pDX, IDC_EDIT_PANEL_MARK_SIZE0, m_edit_panel_mark_size0);
	DDX_Text(pDX, IDC_EDIT_PANEL_MARK_SIZE2, m_edit_panel_mark_size1);
	DDX_Text(pDX, IDC_EDIT_PANEL_MARK_PENEL_POS_Y0, m_edit_panel_mark_to_panel_pos_y0);
	DDX_Text(pDX, IDC_EDIT_PANEL_MARK_PENEL_POS_X0, m_edit_panel_mark_to_panel_pos_x0);
	DDX_Text(pDX, IDC_EDIT_PANEL_MARK_PENEL_POS_Y1, m_edit_panel_mark_to_panel_pos_y1);
	DDX_Text(pDX, IDC_EDIT_PANEL_MARK_PENEL_POS_X1, m_edit_panel_mark_to_panel_pos_x1);
	DDX_Text(pDX, IDC_EDIT_PANEL_CELL_PITCH_X, m_edit_panel_cell_pitch_x);
	DDX_Text(pDX, IDC_EDIT_PANEL_CELL_PITCH_Y, m_edit_panel_cell_pitch_y);
	DDX_Text(pDX, IDC_EDIT_PANEL_CELL_GRPPITCH_X, m_edit_panel_cell_grppitch_x);
	DDX_Text(pDX, IDC_EDIT_PANEL_CELL_GRPPITCH_Y, m_edit_panel_cell_grppitch_y);
	DDX_Text(pDX, IDC_EDIT_PANEL_LOAD_POS_LIFT_UP, m_edit_load_lift_up_pos);
	DDX_Text(pDX, IDC_EDIT_PANEL_LOAD_POS_LIFT_DN, m_edit_load_lift_dn_pos);
}


BEGIN_MESSAGE_MAP(CDialogRecipePanel, CDialogEx)
END_MESSAGE_MAP()


// CDialogRecipePanel 메시지 처리기입니다.

CFont font;
BOOL CDialogRecipePanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	LoadRecipePanelPara(_T(RECIPE_PANEL_PATH));


	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
BEGIN_EVENTSINK_MAP(CDialogRecipePanel, CDialogEx)
	ON_EVENT(CDialogRecipePanel, IDC_BTNENHCTRL_PANEL_SET_LOAD_POS, DISPID_CLICK, CDialogRecipePanel::ClickBtnenhctrlPanelSetLoadPos, VTS_NONE)
	ON_EVENT(CDialogRecipePanel, IDC_BTNENHCTRL_PANEL_SET_LOAD_MOVE, DISPID_CLICK, CDialogRecipePanel::ClickBtnenhctrlPanelSetLoadMove, VTS_NONE)
	ON_EVENT(CDialogRecipePanel, IDC_BTNENHCTRL_PANEL_SET_LOAD_POS_M, DISPID_CLICK, CDialogRecipePanel::ClickBtnenhctrlPanelSetLoadPosM, VTS_NONE)
	ON_EVENT(CDialogRecipePanel, IDC_BTNENHCTRL_PANEL_SET_LOAD_MOVE_M, DISPID_CLICK, CDialogRecipePanel::ClickBtnenhctrlPanelSetLoadMoveM, VTS_NONE)
//	ON_EVENT(CDialogRecipePanel, IDC_BTNENHCTRL_PANEL_SET_START_CELL_POS, DISPID_CLICK, CDialogRecipePanel::ClickBtnenhctrlPanelSetStartCellPos, VTS_NONE)
//	ON_EVENT(CDialogRecipePanel, IDC_BTNENHCTRL_PANEL_SET_START_CELL_MOVE, DISPID_CLICK, CDialogRecipePanel::ClickBtnenhctrlPanelSetStartCellMove, VTS_NONE)
	ON_EVENT(CDialogRecipePanel, IDC_BTNENHCTRL_RECIPE_PANEL_SAVE, DISPID_CLICK, CDialogRecipePanel::ClickBtnenhctrlRecipePanelSave, VTS_NONE)
	ON_EVENT(CDialogRecipePanel, IDC_BTNENHCTRL_RECIPE_PANEL_APPLY, DISPID_CLICK, CDialogRecipePanel::ClickBtnenhctrlRecipePanelApply, VTS_NONE)
	ON_EVENT(CDialogRecipePanel, IDC_BTNENHCTRL_RECIPE_PANEL_CANCEL, DISPID_CLICK, CDialogRecipePanel::ClickBtnenhctrlRecipePanelCancel, VTS_NONE)
	ON_EVENT(CDialogRecipePanel, IDC_BTNENHCTRL_PANEL_LOAD_SET_LIFT_UP_POS, DISPID_CLICK, CDialogRecipePanel::ClickBtnenhctrlPanelLoadSetLiftUpPos, VTS_NONE)
	ON_EVENT(CDialogRecipePanel, IDC_BTNENHCTRL_PANEL_LOAD_LIFT_UP_MOVE, DISPID_CLICK, CDialogRecipePanel::ClickBtnenhctrlPanelLoadLiftUpMove, VTS_NONE)
	ON_EVENT(CDialogRecipePanel, IDC_BTNENHCTRL_PANEL_LOAD_LIFT_DN_POS, DISPID_CLICK, CDialogRecipePanel::ClickBtnenhctrlPanelLoadLiftDnPos, VTS_NONE)
	ON_EVENT(CDialogRecipePanel, IDC_BTNENHCTRL_PANEL_LOAD_LIFT_DN_MOVE, DISPID_CLICK, CDialogRecipePanel::ClickBtnenhctrlPanelLoadLiftDnMove, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogRecipePanel::ClickBtnenhctrlPanelSetLoadPos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	m_edit_panel_load_y_pos = pdlg->m_Robot->GetRobotPos_mm(MOTOR_0);
	m_edit_panel_load_t_pos = pdlg->m_Robot->GetRobotPos_mm(MOTOR_2);

	UpdateData(FALSE);
}


void CDialogRecipePanel::ClickBtnenhctrlPanelSetLoadMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, m_edit_panel_load_y_pos);
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_2, m_edit_panel_load_t_pos);
}


void CDialogRecipePanel::ClickBtnenhctrlPanelSetLoadPosM()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	m_edit_panel_load_y_pos_manu = pdlg->m_Robot->GetRobotPos_mm(MOTOR_0);
	m_edit_panel_load_t_pos_manu = pdlg->m_Robot->GetRobotPos_mm(MOTOR_2);

	UpdateData(FALSE);
}


void CDialogRecipePanel::ClickBtnenhctrlPanelSetLoadMoveM()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, m_edit_panel_load_y_pos_manu);
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_2, m_edit_panel_load_t_pos_manu);
}


void CDialogRecipePanel::ClickBtnenhctrlRecipePanelSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDialogRecipePanel::ClickBtnenhctrlRecipePanelApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	ParameterCompare();
	Update(TRUE);
	pdlg->m_Glass->SetUpdate();
}


void CDialogRecipePanel::ClickBtnenhctrlRecipePanelCancel()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//2010412 ngh
void CDialogRecipePanel::SetUpdate(BOOL isUpdate)
{
	UpdateData(isUpdate);
}

BOOL CDialogRecipePanel::SetLoadRecipePanelPara(CString path)
{
	return LoadRecipePanelPara(path);
}
BOOL CDialogRecipePanel::LoadRecipePanelPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\RecipePanel.par");

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
		
		key.Empty();
		key.Format(_T("m_edit_panel_name"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		m_edit_panel_name = temp;
		
		key.Empty();
		key.Format(_T("m_edit_panel_size_x"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_size_x = atof(ctemp);
		
		key.Empty();
		key.Format(_T("m_edit_panel_size_y"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_size_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_load_y_pos"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_load_y_pos = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_load_t_pos"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_load_t_pos = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_load_y_pos_manu"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_load_y_pos_manu = atof(ctemp);
		
		key.Empty();
		key.Format(_T("m_edit_panel_load_t_pos_manu"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_load_t_pos_manu = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_start_x_pos"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_start_x_pos = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_start_y_pos"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_start_y_pos = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_film_start_x_pos"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_film_start_x_pos = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_film_start_y_pos"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_film_start_y_pos = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_cvd_start_x_pos"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_cvd_start_x_pos = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_cvd_start_y_pos"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_cvd_start_y_pos = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_penel_cell_cnt_y"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_penel_cell_cnt_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_penel_cell_cnt_x"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_penel_cell_cnt_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_penel_cell_grpcnt_y"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_penel_cell_grpcnt_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_penel_cell_grpcnt_x"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_penel_cell_grpcnt_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_mark_size0"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_mark_size0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_mark_size1"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_mark_size1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_mark_to_panel_pos_y0"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_mark_to_panel_pos_y0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_mark_to_panel_pos_x0"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_mark_to_panel_pos_x0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_mark_to_panel_pos_y1"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_mark_to_panel_pos_y1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_mark_to_panel_pos_x1"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_mark_to_panel_pos_x1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_cell_pitch_x"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_cell_pitch_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_cell_pitch_y"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_cell_pitch_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_cell_grppitch_x"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_cell_grppitch_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_panel_cell_grppitch_y"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_panel_cell_grppitch_y = atof(ctemp);


		key.Empty();
		key.Format(_T("m_edit_load_lift_up_pos"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_load_lift_up_pos = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_load_lift_dn_pos"));
		GetPrivateProfileString(_T("RECIPE PANEL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_load_lift_dn_pos = atof(ctemp);


	}
	fd=0;
	
	return rslt;
}

BOOL CDialogRecipePanel::SetSaveRecipePanelPara(CString path)
{
	return SaveRecipePanelPara(path);
}

BOOL CDialogRecipePanel::SaveRecipePanelPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\RecipePanel.par");

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

		key.Empty();
		key.Format(_T("m_edit_panel_name"));
		dat.Empty();
		dat.Format(_T("%s"), m_edit_panel_name);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);		

		key.Empty();
		key.Format(_T("m_edit_panel_size_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_size_x);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);
		
		key.Empty();
		key.Format(_T("m_edit_panel_size_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_size_y);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_load_y_pos"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_load_y_pos);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_load_t_pos"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_load_t_pos);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_load_y_pos_manu"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_load_y_pos_manu);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_load_t_pos_manu"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_load_t_pos_manu);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_start_x_pos"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_start_x_pos);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_start_y_pos"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_start_y_pos);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_film_start_x_pos"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_film_start_x_pos);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_film_start_y_pos"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_film_start_y_pos);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_cvd_start_x_pos"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_cvd_start_x_pos);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_cvd_start_y_pos"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_cvd_start_y_pos);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_penel_cell_cnt_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_penel_cell_cnt_y);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_penel_cell_cnt_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_penel_cell_cnt_x);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_penel_cell_grpcnt_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_penel_cell_grpcnt_y);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_penel_cell_grpcnt_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_penel_cell_grpcnt_x);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_mark_size0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_mark_size0);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_mark_size1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_mark_size1);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_mark_to_panel_pos_y0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_mark_to_panel_pos_y0);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_mark_to_panel_pos_x0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_mark_to_panel_pos_x0);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_mark_to_panel_pos_y1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_mark_to_panel_pos_y1);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_mark_to_panel_pos_x1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_mark_to_panel_pos_x1);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_cell_pitch_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_cell_pitch_x);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_cell_pitch_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_cell_pitch_y);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_cell_grppitch_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_cell_grppitch_x);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_panel_cell_grppitch_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_panel_cell_grppitch_y);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_load_lift_up_pos"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_load_lift_up_pos);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_load_lift_dn_pos"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_load_lift_dn_pos);
		WritePrivateProfileString(_T("RECIPE PANEL"), key, dat, file_path);

	}
	fd=0;
	
	return rslt;
}

double CDialogRecipePanel::GetMarkSize(int cam_idx)
{	
	switch(cam_idx)
	{
	case 0:
		return m_edit_panel_mark_size0;
		break;
	case 1:
		return m_edit_panel_mark_size1;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogRecipePanel::GetMarkSize)!"));
		break;
	}
	return 0.;
}

double CDialogRecipePanel::GetMarkPosX(int cam_idx)
{	
	
	switch(cam_idx)
	{
	case 0:
		return m_edit_panel_mark_to_panel_pos_x0;
		break;
	case 1:
		return m_edit_panel_mark_to_panel_pos_x1;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogRecipePanel::GetPanelSizeX)!"));
		break;
	}

	return 0.;
}

double CDialogRecipePanel::GetMarkPosY(int cam_idx)
{	
	
	switch(cam_idx)
	{
	case 0:
		return m_edit_panel_mark_to_panel_pos_y0;
		break;
	case 1:
		return m_edit_panel_mark_to_panel_pos_y1;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogRecipePanel::GetPanelSizeX)!"));
		break;
	}

	return 0.;
}

double CDialogRecipePanel::GetPanelSizeX()
{	
	
	return m_edit_panel_size_x;
}

double CDialogRecipePanel::GetPanelSizeY()
{	
	
	return m_edit_panel_size_y;
}

double CDialogRecipePanel::GetCellCntX()
{	
	
	return m_edit_penel_cell_cnt_x;
}

double CDialogRecipePanel::GetCellCntY()
{	
	
	return m_edit_penel_cell_cnt_y;
}

double CDialogRecipePanel::GetCellPitchX()
{	
	
	return m_edit_panel_cell_pitch_x;
}

double CDialogRecipePanel::GetCellPitchY()
{	
	
	return m_edit_panel_cell_pitch_y;
}

double CDialogRecipePanel::GetCellGrpCntX()
{	
	
	return m_edit_penel_cell_grpcnt_x;
}

double CDialogRecipePanel::GetCellGrpCntY()
{	
	
	return m_edit_penel_cell_grpcnt_y;
}

double CDialogRecipePanel::GetCellGrpPitchX()
{	
	
	return m_edit_panel_cell_grppitch_x;
}

double CDialogRecipePanel::GetCellGrpPitchY()
{	
	
	return m_edit_panel_cell_grppitch_y;
}

double CDialogRecipePanel::GetCellFirstPosX()
{	
	
	return m_edit_panel_start_x_pos;
}

double CDialogRecipePanel::GetCellFirstPosY()
{	
	
	return m_edit_panel_start_y_pos;
}

double CDialogRecipePanel::GetFilmFirstPosX()
{	
	
	return m_edit_panel_film_start_x_pos;
}

double CDialogRecipePanel::GetFilmFirstPosY()
{	
	
	return m_edit_panel_film_start_y_pos;
}

double CDialogRecipePanel::GetCVDFirstPosX()
{	
	
	return m_edit_panel_cvd_start_x_pos;
}

double CDialogRecipePanel::GetCVDFirstPosY()
{	
	
	return m_edit_panel_cvd_start_y_pos;
}


double CDialogRecipePanel::GetPanelLoadPosY()
{	
	
	return m_edit_panel_load_y_pos;
}

double CDialogRecipePanel::GetPanelLoadPosT()
{	
	
	return m_edit_panel_load_t_pos;
}

double CDialogRecipePanel::GetPanelManuLoadPosY()
{	
	
	return m_edit_panel_load_y_pos_manu;
}

double CDialogRecipePanel::GetPanelManuLoadPosT()
{	
	
	return m_edit_panel_load_t_pos_manu;
}

double CDialogRecipePanel::GetPanelLoadPosUP()
{	
	
	return m_edit_load_lift_up_pos;
}

double CDialogRecipePanel::GetPanelLoadPosDN()
{	
	
	return m_edit_load_lift_dn_pos;
}

//Glass//////////////////////////////////////////////////////////////////////////////////////////////////
double CDialogRecipePanel::GetMarkSize_coordXY(int cam_idx)
{	
	switch(cam_idx)
	{
	case 0:
		return m_edit_panel_mark_size0;
		break;
	case 1:
		return m_edit_panel_mark_size1;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogRecipePanel::GetMarkSize)!"));
		break;
	}
	return 0.;
}

double CDialogRecipePanel::GetMarkPosX_coordXY(int cam_idx)
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	switch(cam_idx)
	{
	case 0:
		return -m_edit_panel_mark_to_panel_pos_y0;
		break;
	case 1:
		return -m_edit_panel_mark_to_panel_pos_y1;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index_coordXY(CDialogRecipePanel::GetPanelSizeX)!"));
		break;
	}

	return 0.;
}

double CDialogRecipePanel::GetMarkPosY_coordXY(int cam_idx)
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	switch(cam_idx)
	{
	case 0:
		return m_edit_panel_mark_to_panel_pos_x0;
		break;
	case 1:
		return m_edit_panel_mark_to_panel_pos_x1;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index_coordXY(CDialogRecipePanel::GetPanelSizeX)!"));
		break;
	}

	return 0.;
}

double CDialogRecipePanel::GetPanelSizeX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_panel_size_y;
}

double CDialogRecipePanel::GetPanelSizeY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_panel_size_x;
}

double CDialogRecipePanel::GetCellCntX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_penel_cell_cnt_y;
}

double CDialogRecipePanel::GetCellCntY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_penel_cell_cnt_x;
}

double CDialogRecipePanel::GetCellPitchX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return -m_edit_panel_cell_pitch_y;
}

double CDialogRecipePanel::GetCellPitchY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_panel_cell_pitch_x;
}

double CDialogRecipePanel::GetCellGrpCntX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_penel_cell_grpcnt_y;
}

double CDialogRecipePanel::GetCellGrpCntY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_penel_cell_grpcnt_x;
}

double CDialogRecipePanel::GetCellGrpPitchX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return -m_edit_panel_cell_grppitch_y;
}

double CDialogRecipePanel::GetCellGrpPitchY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_panel_cell_grppitch_x;
}

double CDialogRecipePanel::GetCellFirstPosX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return -m_edit_panel_start_y_pos;
}

double CDialogRecipePanel::GetCellFirstPosY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_panel_start_x_pos;
}

double CDialogRecipePanel::GetFilmFirstPosX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return -m_edit_panel_film_start_y_pos;
}

double CDialogRecipePanel::GetFilmFirstPosY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_panel_film_start_x_pos;
}

double CDialogRecipePanel::GetCVDFirstPosX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return -m_edit_panel_cvd_start_y_pos;
}

double CDialogRecipePanel::GetCVDFirstPosY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_panel_cvd_start_x_pos;
}


void CDialogRecipePanel::ClickBtnenhctrlPanelLoadSetLiftUpPos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	m_edit_load_lift_up_pos = pdlg->m_Robot->GetRobotPos_mm(MOTOR_3);

	UpdateData(FALSE);
}


void CDialogRecipePanel::ClickBtnenhctrlPanelLoadLiftUpMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotMove_mm(MOTOR_3, m_edit_load_lift_up_pos);
}


void CDialogRecipePanel::ClickBtnenhctrlPanelLoadLiftDnPos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	m_edit_load_lift_dn_pos = pdlg->m_Robot->GetRobotPos_mm(MOTOR_3);

	UpdateData(FALSE);
}


void CDialogRecipePanel::ClickBtnenhctrlPanelLoadLiftDnMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotMove_mm(MOTOR_3, m_edit_load_lift_dn_pos);
}

typeRecipePanel CDialogRecipePanel::GetRecipe()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	typeRecipePanel dat;

	sprintf(dat.cPanel_name, "%S", m_edit_panel_name);
	dat.dPanel_size_x = GetPanelSizeX();
	dat.dPanel_size_y = GetPanelSizeY();
	dat.dPanel_load_y_pos = GetPanelLoadPosY();
	dat.dPanel_load_t_pos = GetPanelLoadPosT();
	dat.dPanel_load_y_pos_manu = GetPanelManuLoadPosY();
	dat.dPanel_load_t_pos_manu = GetPanelManuLoadPosT();
	dat.dPanel_start_x_pos = GetCellFirstPosX();
	dat.dPanel_start_y_pos = GetCellFirstPosY();
	dat.dPanel_film_start_x_pos = GetFilmFirstPosX();
	dat.dPanel_film_start_y_pos = GetFilmFirstPosY();
	dat.dPenel_cell_cnt_y = GetCellCntX();
	dat.dPenel_cell_cnt_x = GetCellCntY();
	dat.dPenel_cell_grpcnt_y = GetCellGrpCntY();
	dat.dPenel_cell_grpcnt_x = GetCellGrpCntX();
	dat.dPanel_mark_size0 = GetMarkSize(0);
	dat.dPanel_mark_size1 = GetMarkSize(1);
	dat.dPanel_mark_to_panel_pos_y0 = GetMarkPosX(0);
	dat.dPanel_mark_to_panel_pos_x0 = GetMarkPosY(0);
	dat.dPanel_mark_to_panel_pos_y1 = GetMarkPosX(1);
	dat.dPanel_mark_to_panel_pos_x1 = GetMarkPosY(1);
	dat.dPanel_cell_pitch_x = GetCellPitchX();
	dat.dPanel_cell_pitch_y = GetCellPitchY();
	dat.dPanel_cell_grppitch_x = GetCellGrpPitchX();
	dat.dPanel_cell_grppitch_y = GetCellGrpPitchY();
	dat.dLoad_lift_up_pos = GetPanelLoadPosUP();
	dat.dLoad_lift_dn_pos = GetPanelLoadPosDN();

	return dat;
}


BOOL CDialogRecipePanel::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

double CDialogRecipePanel::GetCellTotalCnt()
{
	return m_edit_penel_cell_cnt_x * m_edit_penel_cell_cnt_y;
}


void CDialogRecipePanel::Update(BOOL isUpdate)
{
	int idx = 0;
	if(isUpdate)
	{
		m_strPanelName = m_edit_panel_name;
		m_dRecipePanelPara[idx++] = m_edit_panel_size_x;
		m_dRecipePanelPara[idx++] = m_edit_panel_size_y;
		m_dRecipePanelPara[idx++] = m_edit_panel_load_y_pos;
		m_dRecipePanelPara[idx++] = m_edit_panel_load_t_pos;
		m_dRecipePanelPara[idx++] = m_edit_panel_load_y_pos_manu;
		m_dRecipePanelPara[idx++] = m_edit_panel_load_t_pos_manu;
		m_dRecipePanelPara[idx++] = m_edit_panel_start_x_pos;
		m_dRecipePanelPara[idx++] = m_edit_panel_start_y_pos;
		m_dRecipePanelPara[idx++] = m_edit_panel_film_start_x_pos;
		m_dRecipePanelPara[idx++] = m_edit_panel_film_start_y_pos;
		m_dRecipePanelPara[idx++] = m_edit_panel_cvd_start_x_pos;
		m_dRecipePanelPara[idx++] = m_edit_panel_cvd_start_y_pos;
		m_dRecipePanelPara[idx++] = m_edit_penel_cell_cnt_y;
		m_dRecipePanelPara[idx++] = m_edit_penel_cell_cnt_x;
		m_dRecipePanelPara[idx++] = m_edit_penel_cell_grpcnt_y;
		m_dRecipePanelPara[idx++] = m_edit_penel_cell_grpcnt_x;
		m_dRecipePanelPara[idx++] = m_edit_panel_mark_size0;
		m_dRecipePanelPara[idx++] = m_edit_panel_mark_size1;
		m_dRecipePanelPara[idx++] = m_edit_panel_mark_to_panel_pos_y0;
		m_dRecipePanelPara[idx++] = m_edit_panel_mark_to_panel_pos_x0;
		m_dRecipePanelPara[idx++] = m_edit_panel_mark_to_panel_pos_y1;
		m_dRecipePanelPara[idx++] = m_edit_panel_mark_to_panel_pos_x1;
		m_dRecipePanelPara[idx++] = m_edit_panel_cell_pitch_x;
		m_dRecipePanelPara[idx++] = m_edit_panel_cell_pitch_y;
		m_dRecipePanelPara[idx++] = m_edit_panel_cell_grppitch_x;
		m_dRecipePanelPara[idx++] = m_edit_panel_cell_grppitch_y;
		m_dRecipePanelPara[idx++] = m_edit_load_lift_up_pos;
		m_dRecipePanelPara[idx++] = m_edit_load_lift_dn_pos;
	}
	else
	{
		m_edit_panel_name = m_strPanelName;
		m_edit_panel_size_x = m_dRecipePanelPara[idx++];
		m_edit_panel_size_y = m_dRecipePanelPara[idx++];
		m_edit_panel_load_y_pos = m_dRecipePanelPara[idx++];
		m_edit_panel_load_t_pos = m_dRecipePanelPara[idx++];
		m_edit_panel_load_y_pos_manu = m_dRecipePanelPara[idx++];
		m_edit_panel_load_t_pos_manu = m_dRecipePanelPara[idx++];
		m_edit_panel_start_x_pos = m_dRecipePanelPara[idx++];
		m_edit_panel_start_y_pos = m_dRecipePanelPara[idx++];
		m_edit_panel_film_start_x_pos = m_dRecipePanelPara[idx++];
		m_edit_panel_film_start_y_pos = m_dRecipePanelPara[idx++];
		m_edit_panel_cvd_start_x_pos = m_dRecipePanelPara[idx++];
		m_edit_panel_cvd_start_y_pos = m_dRecipePanelPara[idx++];
		m_edit_penel_cell_cnt_y = m_dRecipePanelPara[idx++];
		m_edit_penel_cell_cnt_x = m_dRecipePanelPara[idx++];
		m_edit_penel_cell_grpcnt_y = m_dRecipePanelPara[idx++];
		m_edit_penel_cell_grpcnt_x = m_dRecipePanelPara[idx++];
		m_edit_panel_mark_size0 = m_dRecipePanelPara[idx++];
		m_edit_panel_mark_size1 = m_dRecipePanelPara[idx++];
		m_edit_panel_mark_to_panel_pos_y0 = m_dRecipePanelPara[idx++];
		m_edit_panel_mark_to_panel_pos_x0 = m_dRecipePanelPara[idx++];
		m_edit_panel_mark_to_panel_pos_y1 = m_dRecipePanelPara[idx++];
		m_edit_panel_mark_to_panel_pos_x1 = m_dRecipePanelPara[idx++];
		m_edit_panel_cell_pitch_x = m_dRecipePanelPara[idx++];
		m_edit_panel_cell_pitch_y = m_dRecipePanelPara[idx++];
		m_edit_panel_cell_grppitch_x = m_dRecipePanelPara[idx++];
		m_edit_panel_cell_grppitch_y = m_dRecipePanelPara[idx++];
		m_edit_load_lift_up_pos = m_dRecipePanelPara[idx++];
		m_edit_load_lift_dn_pos = m_dRecipePanelPara[idx++];
	}
}

void CDialogRecipePanel::ParameterCompare()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strType;
	strType.Format(_T("RecipePanel_Para_"));
	int idx = 0;
	
	pdlg->ParaCompareLogWrite(m_strPanelName, m_edit_panel_name, strType + _T("m_edit_panel_name"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_size_x, strType + _T("m_edit_panel_size_x"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_size_y, strType + _T("m_edit_panel_size_y"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_load_y_pos, strType + _T("m_edit_panel_load_y_pos"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_load_t_pos, strType + _T("m_edit_panel_load_t_pos"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_load_y_pos_manu, strType + _T("m_edit_panel_load_y_pos_manu"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_load_t_pos_manu, strType + _T("m_edit_panel_load_t_pos_manu"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_start_x_pos, strType + _T("m_edit_panel_start_x_pos"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_start_y_pos, strType + _T("m_edit_panel_start_y_pos"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_film_start_x_pos, strType + _T("m_edit_panel_film_start_x_pos"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_film_start_y_pos, strType + _T("m_edit_panel_film_start_y_pos"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_cvd_start_x_pos, strType + _T("m_edit_panel_cvd_start_x_pos"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_cvd_start_y_pos, strType + _T("m_edit_panel_cvd_start_y_pos"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_penel_cell_cnt_y, strType + _T("m_edit_penel_cell_cnt_y"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_penel_cell_cnt_x, strType + _T("m_edit_penel_cell_cnt_x"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_penel_cell_grpcnt_y, strType + _T("m_edit_penel_cell_grpcnt_y"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_penel_cell_grpcnt_x, strType + _T("m_edit_penel_cell_grpcnt_x"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_mark_size0, strType + _T("m_edit_panel_mark_size0"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_mark_size1, strType + _T("m_edit_panel_mark_size1"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_mark_to_panel_pos_y0, strType + _T("m_edit_panel_mark_to_panel_pos_y0"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_mark_to_panel_pos_x0, strType + _T("m_edit_panel_mark_to_panel_pos_x0"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_mark_to_panel_pos_y1, strType + _T("m_edit_panel_mark_to_panel_pos_y1"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_mark_to_panel_pos_x1, strType + _T("m_edit_panel_mark_to_panel_pos_x1"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_cell_pitch_x, strType + _T("m_edit_panel_cell_pitch_x"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_cell_pitch_y, strType + _T("m_edit_panel_cell_pitch_y"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_cell_grppitch_x, strType + _T("m_edit_panel_cell_grppitch_x"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_panel_cell_grppitch_y, strType + _T("m_edit_panel_cell_grppitch_y"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_load_lift_up_pos, strType + _T("m_edit_load_lift_up_pos"));
	pdlg->ParaCompareLogWrite(m_dRecipePanelPara[idx++], m_edit_load_lift_dn_pos, strType + _T("m_edit_load_lift_dn_pos"));
}