// Dialog/DialogFilmAlign.cpp : 구현 파일입니다.
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



// CDialogFilmAlign 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogFilmAlign, CDialogEx)

CDialogFilmAlign::CDialogFilmAlign(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogFilmAlign::IDD, pParent)
	, m_check_film_move(FALSE)
	, m_check_rgn_move(FALSE)
	, m_radio_scan_ud(0)
	, m_radio_scan_lr(0)
	, m_edit_thrsh_ud(0)
	, m_edit_thrsh_lr(0)
	, m_edit_film_num_col(1)
	, m_edit_film_num_row(1)
	, m_edit_light(0)
	, m_check_inspect_use(FALSE)
	, m_edit_dSpec_x(0)
	, m_edit_dSpec_y(0)
{
	memset(&m_FilmAlignData, NULL, sizeof(typeFilmAlignData));
	memset(&m_FilmAlignRslt, NULL, sizeof(typeFilmAlignResult));

	m_iSelectRgnNum = 0;
}

CDialogFilmAlign::~CDialogFilmAlign()
{
	SaveFilmAlignPara(_T(FILM_ALIGN_PATH));
}

void CDialogFilmAlign::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNENHCTRL_FILM_ALIGN_POS0, m_btn_rgn_pos0);
	DDX_Control(pDX, IDC_BTNENHCTRL_FILM_ALIGN_POS1, m_btn_rgn_pos1);
	DDX_Control(pDX, IDC_BTNENHCTRL_FILM_ALIGN_POS2, m_btn_rgn_pos2);
	DDX_Control(pDX, IDC_BTNENHCTRL_FILM_ALIGN_POS3, m_btn_rgn_pos3);
	DDX_Control(pDX, IDC_BTNENHCTRL_FILM_ALIGN_POS4, m_btn_rgn_pos4);
	DDX_Control(pDX, IDC_BTNENHCTRL_FILM_ALIGN_POS5, m_btn_rgn_pos5);
	DDX_Check(pDX, IDC_CHECK_FILM_ALIGN_FILM_MOVE, m_check_film_move);
	DDX_Check(pDX, IDC_CHECK_FILM_ALIGN_FILM_MOVE2, m_check_rgn_move);
	DDX_Radio(pDX, IDC_RADIO_FILM_ALIGN_UD0, m_radio_scan_ud);
	DDX_Radio(pDX, IDC_RADIO_FILM_ALIGN_LR0, m_radio_scan_lr);
	DDX_Text(pDX, IDC_EDIT_FILM_ALIGN_UD_THRSH, m_edit_thrsh_ud);
	DDX_Text(pDX, IDC_EDIT_FILM_ALIGN_LR_THRSH, m_edit_thrsh_lr);
	DDX_Text(pDX, IDC_EDIT_FILM_ALIGN_X, m_edit_film_num_col);
	DDX_Text(pDX, IDC_EDIT_FILM_ALIGN_Y, m_edit_film_num_row);
	DDX_Control(pDX, IDC_SLIDER_ALIGN_LIGHT0, m_slider_light);
	DDX_Text(pDX, IDC_EDIT_ALIGN_LIGHT0, m_edit_light);
	DDX_Check(pDX, IDC_CHECK_INSPECT_USE, m_check_inspect_use);
	DDX_Text(pDX, IDC_EDIT_FILM_ALIGN_SPEC_X, m_edit_dSpec_x);
	DDX_Text(pDX, IDC_EDIT_FILM_ALIGN_SPEC_Y, m_edit_dSpec_y);
}


BEGIN_MESSAGE_MAP(CDialogFilmAlign, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ALIGN_LIGHT0, &CDialogFilmAlign::OnNMCustomdrawSliderAlignLight0)
END_MESSAGE_MAP()


// CDialogFilmAlign 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDialogFilmAlign, CDialogEx)
//	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_POS0, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos0, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_POS1, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos1, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_POS2, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos2, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_POS5, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos5, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_POS4, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos4, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_POS3, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos3, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_MOVE, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignMove, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_LF, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignLf, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_RT, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignRt, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_UP, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignUp, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_DN, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignDn, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_MOVEPOS, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignMovepos, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_INSPECT, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignInspect, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_APPLY, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignApply, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_POS0, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos0, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_ALIGN_LIGHT_SET0, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlAlignLightSet0, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_CANCEL, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignCancel, VTS_NONE)
END_EVENTSINK_MAP()

BOOL CDialogFilmAlign::SetRegionMove(int row, int col, int rgn_idx, BOOL disp)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	double x, y;
	double sx, sy, ex, ey;
	int cam_idx = 0;

	//Cell limit조건
	if(1 > col)
	{
		col = 1;
	}
	if((pdlg->m_RecipePanel->GetCellGrpCntX_coordXY() + 1) < col)
	{
		col = pdlg->m_RecipePanel->GetCellGrpCntX_coordXY() + 1;
	}
	if(1 > row)
	{
		row = 1;
	}
	if((pdlg->m_RecipePanel->GetCellGrpCntY_coordXY() + 1) < row)
	{
		row = pdlg->m_RecipePanel->GetCellGrpCntY_coordXY() + 1;
	}
	if(GLASS_MAX_CELL_NUM <= (pdlg->m_RecipePanel->GetCellGrpCntX_coordXY() * pdlg->m_RecipePanel->GetCellGrpCntY_coordXY()) )
	{
		//Error log

		return FALSE;
	}

	//X(열), Y(행)
	x = 0.;
	y = 0.;
	pdlg->m_Glass->GetCellGrpRegionRobotPos_coordXY(row, col, &sx, &sy, &ex, &ey);
	switch(rgn_idx)
	{
	case 0:
		y = sx;
		x = sy;
		break;
	case 1:
		y = (sx + ex) * 0.5;
		x = sy;
		break;
	case 2:
		y = ex;
		x = sy;
		break;
	case 3:
		y = ex;
		x = ey;
		break;
	case 4:
		y = (sx + ex) * 0.5;
		x = ey;
		break;
	case 5:
		y = sx;
		x = ey;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(SetRegionMove(int rgn_idx))!"));
		break;
	}

	pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, x);
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, y);

	if(disp)
	{
		pdlg->m_Glass->SetDispGlassDat();
		pdlg->m_Glass->SetDrawCrossRobot(y, x);
	}

	m_dMotPos[MOTOR_1] = x;
	m_dMotPos[MOTOR_0] = y;



	return TRUE;
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	m_btn_rgn_pos0.SetValue(FALSE);
	m_btn_rgn_pos1.SetValue(FALSE);
	m_btn_rgn_pos2.SetValue(FALSE);
	m_btn_rgn_pos3.SetValue(FALSE);
	m_btn_rgn_pos4.SetValue(FALSE);
	m_btn_rgn_pos5.SetValue(FALSE);

	m_btn_rgn_pos0.SetValue(TRUE);
	m_iSelectRgnNum = 0;

	CButton *pWnd=(CButton *)GetDlgItem(IDC_CHECK_FILM_ALIGN_FILM_MOVE2);
	if(pWnd->GetCheck())
	{
		m_check_rgn_move = TRUE;
	}

	Update(FALSE);
	UpdateData(FALSE);

	SetRegionMove(m_edit_film_num_row, m_edit_film_num_col, m_iSelectRgnNum, TRUE);
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_btn_rgn_pos0.SetValue(FALSE);
	m_btn_rgn_pos1.SetValue(FALSE);
	m_btn_rgn_pos2.SetValue(FALSE);
	m_btn_rgn_pos3.SetValue(FALSE);
	m_btn_rgn_pos4.SetValue(FALSE);
	m_btn_rgn_pos5.SetValue(FALSE);

	m_btn_rgn_pos1.SetValue(TRUE);
	m_iSelectRgnNum = 1;

	CButton *pWnd=(CButton *)GetDlgItem(IDC_CHECK_FILM_ALIGN_FILM_MOVE2);
	if(pWnd->GetCheck())
	{
		m_check_rgn_move = TRUE;
	}

	Update(FALSE);
	UpdateData(FALSE);

	SetRegionMove(m_edit_film_num_row, m_edit_film_num_col, m_iSelectRgnNum, TRUE);
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_btn_rgn_pos0.SetValue(FALSE);
	m_btn_rgn_pos1.SetValue(FALSE);
	m_btn_rgn_pos2.SetValue(FALSE);
	m_btn_rgn_pos3.SetValue(FALSE);
	m_btn_rgn_pos4.SetValue(FALSE);
	m_btn_rgn_pos5.SetValue(FALSE);

	m_btn_rgn_pos2.SetValue(TRUE);
	m_iSelectRgnNum = 2;

	CButton *pWnd=(CButton *)GetDlgItem(IDC_CHECK_FILM_ALIGN_FILM_MOVE2);
	if(pWnd->GetCheck())
	{
		m_check_rgn_move = TRUE;
	}

	Update(FALSE);
	UpdateData(FALSE);

	SetRegionMove(m_edit_film_num_row, m_edit_film_num_col, m_iSelectRgnNum, TRUE);
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos5()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_btn_rgn_pos0.SetValue(FALSE);
	m_btn_rgn_pos1.SetValue(FALSE);
	m_btn_rgn_pos2.SetValue(FALSE);
	m_btn_rgn_pos3.SetValue(FALSE);
	m_btn_rgn_pos4.SetValue(FALSE);
	m_btn_rgn_pos5.SetValue(FALSE);

	m_btn_rgn_pos5.SetValue(TRUE);
	m_iSelectRgnNum = 5;

	CButton *pWnd=(CButton *)GetDlgItem(IDC_CHECK_FILM_ALIGN_FILM_MOVE2);
	if(pWnd->GetCheck())
	{
		m_check_rgn_move = TRUE;
	}

	Update(FALSE);
	UpdateData(FALSE);

	SetRegionMove(m_edit_film_num_row, m_edit_film_num_col, m_iSelectRgnNum, TRUE);
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos4()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_btn_rgn_pos0.SetValue(FALSE);
	m_btn_rgn_pos1.SetValue(FALSE);
	m_btn_rgn_pos2.SetValue(FALSE);
	m_btn_rgn_pos3.SetValue(FALSE);
	m_btn_rgn_pos4.SetValue(FALSE);
	m_btn_rgn_pos5.SetValue(FALSE);

	m_btn_rgn_pos4.SetValue(TRUE);
	m_iSelectRgnNum = 4;

	CButton *pWnd=(CButton *)GetDlgItem(IDC_CHECK_FILM_ALIGN_FILM_MOVE2);
	if(pWnd->GetCheck())
	{
		m_check_rgn_move = TRUE;
	}

	Update(FALSE);
	UpdateData(FALSE);

	SetRegionMove(m_edit_film_num_row, m_edit_film_num_col, m_iSelectRgnNum, TRUE);
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_btn_rgn_pos0.SetValue(FALSE);
	m_btn_rgn_pos1.SetValue(FALSE);
	m_btn_rgn_pos2.SetValue(FALSE);
	m_btn_rgn_pos3.SetValue(FALSE);
	m_btn_rgn_pos4.SetValue(FALSE);
	m_btn_rgn_pos5.SetValue(FALSE);

	m_btn_rgn_pos3.SetValue(TRUE);
	m_iSelectRgnNum = 3;

	CButton *pWnd=(CButton *)GetDlgItem(IDC_CHECK_FILM_ALIGN_FILM_MOVE2);
	if(pWnd->GetCheck())
	{
		m_check_rgn_move = TRUE;
	}

	Update(FALSE);
	UpdateData(FALSE);

	SetRegionMove(m_edit_film_num_row, m_edit_film_num_col, m_iSelectRgnNum, TRUE);
}



void CDialogFilmAlign::ClickBtnenhctrlFilmAlignMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	double x, y;
	int cam_idx = 0;

	UpdateData(TRUE);

	//Cell limit조건
	if(1 > m_edit_film_num_col)
	{
		m_edit_film_num_col = 1;
	}
	if((pdlg->m_RecipePanel->GetCellGrpCntX_coordXY() + 1) < m_edit_film_num_col)
	{
		m_edit_film_num_col = pdlg->m_RecipePanel->GetCellGrpCntX_coordXY() + 1;
	}
	if(1 > m_edit_film_num_row)
	{
		m_edit_film_num_row = 1;
	}
	if((pdlg->m_RecipePanel->GetCellGrpCntY_coordXY() + 1) < m_edit_film_num_row)
	{
		m_edit_film_num_row = pdlg->m_RecipePanel->GetCellGrpCntY_coordXY() + 1;
	}
	if(GLASS_MAX_CELL_NUM <= (pdlg->m_RecipePanel->GetCellGrpCntX_coordXY() * pdlg->m_RecipePanel->GetCellGrpCntY_coordXY()) )
	{
		//Error log

		return;
	}

	//X(열), Y(행)
	y = pdlg->m_Glass->GetCellGrpRobotPosX_coordXY(m_edit_film_num_row, m_edit_film_num_col);
	x = pdlg->m_Glass->GetCellGrpRobotPosY_coordXY(m_edit_film_num_row, m_edit_film_num_col);

	pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, x);
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, y);

	pdlg->m_Glass->SetDispGlassDat();
	pdlg->m_Glass->SetDrawCrossRobot(y, x);
	

}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignLf()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	double x, y;
	int cam_idx = 0;


	UpdateData(TRUE);

	m_edit_film_num_col++;

	//Cell limit조건
	int num = 0;
	if(pdlg->m_RecipePanel->GetCellGrpCntX_coordXY())
	{
		num = pdlg->m_RecipePanel->GetCellCntX_coordXY() / pdlg->m_RecipePanel->GetCellGrpCntX_coordXY();
	}
	else
	{
		num = 0;
	}
	if(1 > m_edit_film_num_col)
	{
		m_edit_film_num_col = 1;
	}
	if(num < m_edit_film_num_col)
	{
		m_edit_film_num_col = num;
	}
	if(1 > m_edit_film_num_row)
	{
		m_edit_film_num_row = 1;
	}
	if((pdlg->m_RecipePanel->GetCellGrpCntY_coordXY() + 1) < m_edit_film_num_row)
	{
		m_edit_film_num_row = pdlg->m_RecipePanel->GetCellGrpCntY_coordXY() + 1;
	}
	if(GLASS_MAX_CELL_NUM <= (pdlg->m_RecipePanel->GetCellGrpCntX_coordXY() * pdlg->m_RecipePanel->GetCellGrpCntY_coordXY()) )
	{
		//Error log

		return;
	}

	//X(열), Y(행)
	y = pdlg->m_Glass->GetCellGrpRobotPosX_coordXY(m_edit_film_num_row, m_edit_film_num_col);
	x = pdlg->m_Glass->GetCellGrpRobotPosY_coordXY(m_edit_film_num_row, m_edit_film_num_col);

	pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, x);
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, y);

	pdlg->m_Glass->SetDispGlassDat();
	pdlg->m_Glass->SetDrawCrossRobot(y, x);

	UpdateData(FALSE);
	
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignRt()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	double x, y;
	int cam_idx = 0;

	UpdateData(TRUE);

	m_edit_film_num_col--;

	//Cell limit조건
	int num = 0;
	if(pdlg->m_RecipePanel->GetCellGrpCntX_coordXY())
	{
		num = pdlg->m_RecipePanel->GetCellCntX_coordXY() / pdlg->m_RecipePanel->GetCellGrpCntX_coordXY();
	}
	else
	{
		num = 0;
	}
	if(1 > m_edit_film_num_col)
	{
		m_edit_film_num_col = 1;
	}
	if(num < m_edit_film_num_col)
	{
		m_edit_film_num_col = num;
	}
	if(1 > m_edit_film_num_row)
	{
		m_edit_film_num_row = 1;
	}
	if((pdlg->m_RecipePanel->GetCellGrpCntY_coordXY() + 1) < m_edit_film_num_row)
	{
		m_edit_film_num_row = pdlg->m_RecipePanel->GetCellGrpCntY_coordXY() + 1;
	}
	if(GLASS_MAX_CELL_NUM <= (pdlg->m_RecipePanel->GetCellGrpCntX_coordXY() * pdlg->m_RecipePanel->GetCellGrpCntY_coordXY()) )
	{
		//Error log

		return;
	}

	//X(열), Y(행)
	y = pdlg->m_Glass->GetCellGrpRobotPosX_coordXY(m_edit_film_num_row, m_edit_film_num_col);
	x = pdlg->m_Glass->GetCellGrpRobotPosY_coordXY(m_edit_film_num_row, m_edit_film_num_col);

	pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, x);
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, y);

	pdlg->m_Glass->SetDispGlassDat();
	pdlg->m_Glass->SetDrawCrossRobot(y, x);

	UpdateData(FALSE);
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignUp()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	double x, y;
	int cam_idx = 0;

	UpdateData(TRUE);

	m_edit_film_num_row++;

	//Cell limit조건
	int num = 0;
	if(pdlg->m_RecipePanel->GetCellGrpCntX_coordXY())
	{
		num = pdlg->m_RecipePanel->GetCellCntX_coordXY() / pdlg->m_RecipePanel->GetCellGrpCntX_coordXY();
	}
	else
	{
		num = 0;
	}
	if(1 > m_edit_film_num_col)
	{
		m_edit_film_num_col = 1;
	}
	if((num + 1) < m_edit_film_num_col)
	{
		m_edit_film_num_col = pdlg->m_RecipePanel->GetCellGrpCntX_coordXY() + 1;
	}
	if(1 > m_edit_film_num_row)
	{
		m_edit_film_num_row = 1;
	}
	if(pdlg->m_RecipePanel->GetCellGrpCntY_coordXY() < m_edit_film_num_row)
	{
		m_edit_film_num_row = pdlg->m_RecipePanel->GetCellGrpCntY_coordXY();
	}
	if(GLASS_MAX_CELL_NUM <= (pdlg->m_RecipePanel->GetCellGrpCntX_coordXY() * pdlg->m_RecipePanel->GetCellGrpCntY_coordXY()) )
	{
		//Error log

		return;
	}

	//X(열), Y(행)
	y = pdlg->m_Glass->GetCellGrpRobotPosX_coordXY(m_edit_film_num_row, m_edit_film_num_col);
	x = pdlg->m_Glass->GetCellGrpRobotPosY_coordXY(m_edit_film_num_row, m_edit_film_num_col);

	pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, x);
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, y);

	pdlg->m_Glass->SetDispGlassDat();
	pdlg->m_Glass->SetDrawCrossRobot(y, x);

	UpdateData(FALSE);
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignDn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	double x, y;
	int cam_idx = 0;

	UpdateData(TRUE);

	m_edit_film_num_row--;

	//Cell limit조건
	int num = 0;
	if(pdlg->m_RecipePanel->GetCellGrpCntX_coordXY())
	{
		num = pdlg->m_RecipePanel->GetCellCntX_coordXY() / pdlg->m_RecipePanel->GetCellGrpCntX_coordXY();
	}
	else
	{
		num = 0;
	}
	if(1 > m_edit_film_num_col)
	{
		m_edit_film_num_col = 1;
	}
	if((num + 1) < m_edit_film_num_col)
	{
		m_edit_film_num_col = pdlg->m_RecipePanel->GetCellGrpCntX_coordXY() + 1;
	}
	if(1 > m_edit_film_num_row)
	{
		m_edit_film_num_row = 1;
	}
	if(pdlg->m_RecipePanel->GetCellGrpCntY_coordXY() < m_edit_film_num_row)
	{
		m_edit_film_num_row = pdlg->m_RecipePanel->GetCellGrpCntY_coordXY();
	}
	if(GLASS_MAX_CELL_NUM <= (pdlg->m_RecipePanel->GetCellGrpCntX_coordXY() * pdlg->m_RecipePanel->GetCellGrpCntY_coordXY()) )
	{
		//Error log

		return;
	}

	//X(열), Y(행)
	y = pdlg->m_Glass->GetCellGrpRobotPosX_coordXY(m_edit_film_num_row, m_edit_film_num_col);
	x = pdlg->m_Glass->GetCellGrpRobotPosY_coordXY(m_edit_film_num_row, m_edit_film_num_col);

	pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, x);
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, y);

	pdlg->m_Glass->SetDispGlassDat();
	pdlg->m_Glass->SetDrawCrossRobot(y, x);

	UpdateData(FALSE);
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignMovepos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CDialogFilmAlign::ClickBtnenhctrlFilmAlignInspect()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);
	double x, y;

	disp_obj(*pdlg->m_Review->GetImage(), pdlg->m_Review->GetView());

	if(0 == m_radio_scan_ud)
	{
		SetInspectFilmAlign(m_iSelectRgnNum, &x, &y,_T("0"));
	}
	else
	{
		//Shadow test용
		SetInspectFilmAlign2(m_iSelectRgnNum, &x, &y);
	}
}

BOOL CDialogFilmAlign::SaveFilmAlignImage(CString file_path)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	Herror herr;
	char temp[512];
	CString tmpPath_Name;
	sprintf_s(temp, file_path.GetLength() + 1, "%S", file_path);
	set_check("~give_error");
	herr = write_image(*pdlg->m_Review->GetImage(), HTuple("bmp"), NULL, temp);
	set_check("give_error");
	if(H_MSG_TRUE == herr)
	{
	}
	else
	{
		AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1!"));
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDialogFilmAlign::SetInspectFilmAlign(int rgn_idx, double *inter_x, double *inter_y,CString idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	BOOL rslt = TRUE;
	CString strLog;
	int cam_idx;
	HTuple AmplitudeSecond, MeasureHandle, IntraDistance, InterDistance, AmplitudeFirst;
	HTuple RowEdgeFirstLR, ColumnEdgeFirstLR, RowEdgeSecondLR, ColumnEdgeSecondLR, RowEdgeFirstUD, ColumnEdgeFirstUD, RowEdgeSecondUD, ColumnEdgeSecondUD; 
	HTuple InterCol, InterRow, IsOver;
	Hlong edgeCnt;
	HTuple wd, ht, row, col, leng;
	Herror herr;
	Hobject lr_image, ud_image, r_image, g_image, b_image;
	double start_x_lr, start_y_lr, end_x_lr, end_y_lr, start_x_ud, start_y_ud, end_x_ud, end_y_ud;
	*inter_x = 0.;
	*inter_y = 0.;

	cam_idx = 0;
	
	Update(TRUE);

	int film_idx = 0;
	int rslt_num = 0;

	strLog.Format(_T("SetInspectFilmAlign() Index = %d Start"), rgn_idx);
	pdlg->WriteLog(FILM_ALIGN_LOG, strLog);
	
	/*
	m_FilmAlignData.m_dThrshLR[film_idx][rgn_idx] = m_edit_thrsh_lr;
	m_FilmAlignData.m_dThrshUD[film_idx][rgn_idx] = m_edit_thrsh_ud;

	m_FilmAlignData.m_iScanUD[film_idx][rgn_idx] = m_radio_scan_ud;
	m_FilmAlignData.m_iScanRD[film_idx][rgn_idx] = m_radio_scan_lr;

	pdlg->m_Review->GetFilmAlignRect(film_idx, rgn_idx, 0, &m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx]);
	pdlg->m_Review->GetFilmAlignRect(film_idx, rgn_idx, 1, &m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx]);
	*/

	
	decompose3(*pdlg->m_Review->GetImage(), &r_image, &g_image, &b_image);
	mean_image(r_image, &lr_image, 1, 300);
	emphasize(lr_image, &lr_image, 25, 25, 1);
	mean_image(r_image, &ud_image, 300, 1);
	emphasize(ud_image, &ud_image, 25, 25, 1);

	double mark_width, mark_height;
	mark_width = 0.;
	mark_height = 0.;


	set_check("~give_error");
	herr = get_image_pointer1(*pdlg->m_Review->GetImage(), NULL, NULL, &wd, &ht);
	set_check("give_error");
	if(H_MSG_TRUE == herr)
	{
	}
	else
	{
		AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1!"));
		return rslt;
	}

	switch(rgn_idx)
	{
	case 0:
	case 2:
	case 3:
	case 5:
		//Left right
		row = (Hlong)((m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx].bottom + m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx].top) * 0.5);
		col = (Hlong)((m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx].right + m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx].left) * 0.5);
		leng = (m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx].right - m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx].left) * 0.5;
		gen_measure_rectangle2(row,col,HTuple(0),leng,ALIGN_MEASURE_LENG,wd,ht,HTuple("nearest_neighbor"),&MeasureHandle);

		measure_pairs(lr_image, MeasureHandle, HTuple(FILM_ALIGN_EDGE_SIGMA), HTuple(m_edit_thrsh_lr), HTuple("negative_strongest"), HTuple("all"),
							&RowEdgeFirstLR,&ColumnEdgeFirstLR,&AmplitudeFirst,  
							&RowEdgeSecondLR,&ColumnEdgeSecondLR,&AmplitudeSecond,
							&IntraDistance, &InterDistance);

		close_measure(MeasureHandle);
	
		edgeCnt = RowEdgeFirstLR.Num();
		if(0 < edgeCnt)
		{
			set_color(pdlg->m_Review->GetView(), HTuple("yellow"));
			set_draw(pdlg->m_Review->GetView(), HTuple("margin"));
			disp_line(pdlg->m_Review->GetView(), 0, ColumnEdgeFirstLR[0], ht, ColumnEdgeFirstLR[0]);
			disp_line(pdlg->m_Review->GetView(), 0,ColumnEdgeSecondLR[0], ht, ColumnEdgeSecondLR[0]);

			rslt_num++;
		}
		else
		{
			set_color(pdlg->m_Review->GetView(), HTuple("red"));
			set_draw(pdlg->m_Review->GetView(), HTuple("margin"));
			set_tposition(pdlg->m_Review->GetView(), 10, 10);
			write_string(pdlg->m_Review->GetView(), HTuple("Can't find!"));

			rslt = FALSE;
		}

		strLog.Format(_T("row=%f,col=%f,leng=%f,edgeCnt=%d,ColumnEdgeFirstLR[0]=%f,ColumnEdgeFirstLR[0]=%f,ColumnEdgeSecondLR[0]=%f,ColumnEdgeSecondLR[0]=%f"),
			row,col,leng,ColumnEdgeFirstLR[0],ColumnEdgeFirstLR[0],ColumnEdgeSecondLR[0],ColumnEdgeSecondLR[0]);
		pdlg->WriteLog(FILM_ALIGN_LOG, strLog);
		break;
	case 1:
	case 4:

		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogFilmAlign::SetInspectFilmAlign-0)!"));
		break;
	}

	switch(rgn_idx)
	{
	//All
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		//Up dn
		row = (Hlong)((m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].bottom + m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].top) * 0.5);
		col = (Hlong)((m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].right + m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].left) * 0.5);
	
		leng = (m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].bottom - m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].top) * 0.5;
		gen_measure_rectangle2(row,col,HTuple(-1.57079632679),leng,ALIGN_MEASURE_LENG,wd,ht,HTuple("nearest_neighbor"),&MeasureHandle);

		measure_pairs(ud_image, MeasureHandle, HTuple(FILM_ALIGN_EDGE_SIGMA), HTuple(m_edit_thrsh_ud), HTuple("negative_strongest"), HTuple("all"),
							&RowEdgeFirstUD,&ColumnEdgeFirstUD,&AmplitudeFirst,  
							&RowEdgeSecondUD,&ColumnEdgeSecondUD,&AmplitudeSecond,
							&IntraDistance, &InterDistance);

		close_measure(MeasureHandle);
	
		edgeCnt = RowEdgeFirstUD.Num();
		if(0 < edgeCnt)
		{
			set_color(pdlg->m_Review->GetView(), HTuple("yellow"));
			set_draw(pdlg->m_Review->GetView(), HTuple("margin"));
			
			InterCol = (double)
			disp_line(pdlg->m_Review->GetView(), RowEdgeFirstUD[0], 0, RowEdgeFirstUD[0], wd);
			disp_line(pdlg->m_Review->GetView(), RowEdgeSecondUD[0], 0, RowEdgeSecondUD[0], wd);

			rslt_num++;
		}
		else
		{
			set_color(pdlg->m_Review->GetView(), HTuple("red"));
			set_draw(pdlg->m_Review->GetView(), HTuple("margin"));
			set_tposition(pdlg->m_Review->GetView(), 10, 10);
			write_string(pdlg->m_Review->GetView(), HTuple("Can't find!"));

			rslt = FALSE;
		}
		strLog.Format(_T("row=%f,col=%f,leng=%f,edgeCnt=%d,ColumnEdgeFirstLR[0]=%f,ColumnEdgeFirstLR[0]=%f,ColumnEdgeSecondLR[0]=%f,RowEdgeSecondLR[0]=%f,RowEdgeFirstLR[0]=%f,RowEdgeSecondLR[0]=%f,RowEdgeSecondLR[0]=%f"),
			row,col,leng,ColumnEdgeFirstLR[0],ColumnEdgeFirstLR[0],ColumnEdgeSecondLR[0],ColumnEdgeSecondLR[0],RowEdgeSecondLR[0],RowEdgeFirstLR[0],RowEdgeSecondLR[0],RowEdgeSecondLR[0]);
		pdlg->WriteLog(FILM_ALIGN_LOG, strLog);
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogFilmAlign::SetInspectFilmAlign-1)!"));
		break;
	}

	if(rslt)
	{
		if(1 < rslt_num)
		{
			//교차점 구하기
			switch(rgn_idx)
			{
			case 0://좌상단
				//Left->right
				start_x_lr = ColumnEdgeFirstLR[0];
				start_y_lr = 0.;
				end_x_lr = ColumnEdgeFirstLR[0];
				end_y_lr = (double)ht[0];

				//Down->Up
				start_x_ud = 0.;
				start_y_ud = RowEdgeFirstUD[0];
				end_x_ud = (double)wd[0];
				end_y_ud = RowEdgeFirstUD[0];
				break;
			case 2://우상단
				//Left->right
				start_x_lr = ColumnEdgeSecondLR[0];
				start_y_lr = 0.;
				end_x_lr = ColumnEdgeSecondLR[0];
				end_y_lr = (double)ht[0];

				//Down->Up
				start_x_ud = 0.;
				start_y_ud = RowEdgeFirstUD[0];
				end_x_ud = (double)wd[0];
				end_y_ud = RowEdgeFirstUD[0];
				break;
			case 3://우하단
				//Left->right
				start_x_lr = ColumnEdgeSecondLR[0];
				start_y_lr = 0.;
				end_x_lr = ColumnEdgeSecondLR[0];
				end_y_lr = (double)ht[0];

				//Down->Up
				start_x_ud = 0.;
				start_y_ud = RowEdgeSecondUD[0];
				end_x_ud = (double)wd[0];
				end_y_ud = RowEdgeSecondUD[0];
				break;
			case 5://좌하단
				//Left->right
				start_x_lr = ColumnEdgeFirstLR[0];
				start_y_lr = 0.;
				end_x_lr = ColumnEdgeFirstLR[0];
				end_y_lr = (double)ht[0];

				//Down->Up
				start_x_ud = 0.;
				start_y_ud = RowEdgeSecondUD[0];
				end_x_ud = (double)wd[0];
				end_y_ud = RowEdgeSecondUD[0];
				break;
			default:
				AfxMessageBox(_T("Error : invalid index(CDialogFilmAlign::SetInspectFilmAlign-2)!"));
				break;
			}

		}
		else
		{
			//높이 구하기
			switch(rgn_idx)
			{
			case 1://중상단
				//Left->right
				start_x_lr = ((double)wd[0]) * 0.5;
				start_y_lr = 0.;
				end_x_lr = ((double)wd[0]) * 0.5;
				end_y_lr = (double)ht[0];

				//Down->Up
				start_x_ud = 0.;
				start_y_ud = RowEdgeFirstUD[0];
				end_x_ud = (double)wd[0];
				end_y_ud = RowEdgeFirstUD[0];
				break;
			case 4://중하단
				//Left->right
				start_x_lr = ((double)wd[0]) * 0.5;
				start_y_lr = 0.;
				end_x_lr = ((double)wd[0]) * 0.5;
				end_y_lr = (double)ht[0];

				//Down->Up
				start_x_ud = 0.;
				start_y_ud = RowEdgeSecondUD[0];
				end_x_ud = (double)wd[0];
				end_y_ud = RowEdgeSecondUD[0];
				break;
			default:
				AfxMessageBox(_T("Error : invalid index(CDialogFilmAlign::SetInspectFilmAlign-3)!"));
				break;
			}
		}

		//교차점
		intersection_lines(start_y_lr, start_x_lr, end_y_lr, end_x_lr, start_y_ud, start_x_ud, end_y_ud, end_x_ud, &InterRow, &InterCol, &IsOver);
		*inter_x = (double)InterCol[0];
		*inter_y = (double)InterRow[0];

		edgeCnt = InterCol.Num();
		if(0 < edgeCnt)
		{
			set_color(pdlg->m_Review->GetView(), HTuple("blue"));
			set_draw(pdlg->m_Review->GetView(), HTuple("margin"));
			disp_line(pdlg->m_Review->GetView(), *inter_y, *inter_x - 50, *inter_y, *inter_x + 50);
			disp_line(pdlg->m_Review->GetView(), *inter_y - 50, *inter_x, *inter_y + 50, *inter_x);
			set_color(pdlg->m_Review->GetView(), HTuple("green"));
			set_draw(pdlg->m_Review->GetView(), HTuple("margin"));
			disp_line(pdlg->m_Review->GetView(), *inter_y, *inter_x - 20, *inter_y, *inter_x + 20);
			disp_line(pdlg->m_Review->GetView(), *inter_y - 20, *inter_x, *inter_y + 20, *inter_x);
		}
		else
		{
			set_color(pdlg->m_Review->GetView(), HTuple("red"));
			set_draw(pdlg->m_Review->GetView(), HTuple("margin"));
			set_tposition(pdlg->m_Review->GetView(), 10, 10);
			write_string(pdlg->m_Review->GetView(), HTuple("Can't find intersection!"));

			rslt = FALSE;
		}

		strLog.Format(_T("edgeCnt=%d,start_y_lr=%f,start_x_lr=%f,end_y_lr=%f,end_x_lr=%f,start_y_ud=%f,start_x_ud=%f,end_y_ud=%f,end_x_ud=%f,InterRow[0]=%f, InterCol[0]=%f, IsOver[0]=%f"),
			edgeCnt,start_y_lr, start_x_lr, end_y_lr, end_x_lr, start_y_ud, start_x_ud, end_y_ud, end_x_ud, InterRow[0], InterCol[0], IsOver[0]);
		pdlg->WriteLog(FILM_ALIGN_LOG, strLog);
	}
	char temp[512];
	CString tmpPath_Name;
	
	set_color(pdlg->m_Review->GetView(), "blue");
	set_draw(pdlg->m_Review->GetView(), HTuple("margin"));
	set_tposition(pdlg->m_Review->GetView(), 10, 10);
	write_string(pdlg->m_Review->GetView(), HTuple("(X, Y) position : (") + HTuple(*inter_x) + HTuple(",") + HTuple(*inter_y) + HTuple(")"));
	
	//20160811 ngh
	//////////////////////////////////////////////////////////////////
	//tmpPath_Name.Format(_T("D:\\DATABASE\\FILM_ALIGN\\film_%s"),idx);  //20160425 SC
	//sprintf_s(temp, tmpPath_Name.GetLength() + 1, "%S", tmpPath_Name);	//20160425 SC
	//write_image(*pdlg->m_Review->GetImage(), HTuple("bmp"),NULL,temp);	//20160425 SC
	//
	//sprintf(m_FilmImage, "%s.bmp", temp);
	//////////////////////////////////////////////////////////////////
	strLog.Format(_T("SetInspectFilmAlign() End"));
	pdlg->WriteLog(FILM_ALIGN_LOG, strLog);
	
	return rslt;
	//False return :edge 못찾을때->측정불량으로 표기
}

BOOL CDialogFilmAlign::SetInspectFilmAlign2(int rgn_idx, double *inter_x, double *inter_y)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	BOOL rslt = TRUE;

	int cam_idx;
	HTuple AmplitudeSecond, MeasureHandle, IntraDistance, InterDistance, AmplitudeFirst;
	HTuple RowEdgeFirstLR, ColumnEdgeFirstLR, RowEdgeSecondLR, ColumnEdgeSecondLR, RowEdgeFirstUD, ColumnEdgeFirstUD, RowEdgeSecondUD, ColumnEdgeSecondUD; 
	HTuple InterCol, InterRow, IsOver;
	Hlong edgeCnt;
	HTuple wd, ht, row, col, leng;
	Herror herr;
	Hobject lr_image, ud_image, r_image, g_image, b_image;
	double start_x_lr, start_y_lr, end_x_lr, end_y_lr, start_x_ud, start_y_ud, end_x_ud, end_y_ud;
	*inter_x = 0.;
	*inter_y = 0.;

	cam_idx = 0;
	
	Update(TRUE);

	int film_idx = 0;
	int rslt_num = 0;
	
	disp_obj(*pdlg->m_Review->GetImage(), pdlg->m_Review->GetView());

	decompose3(*pdlg->m_Review->GetImage(), &r_image, &g_image, &b_image);
	emphasize(b_image, &lr_image, 25, 25, 1);
	emphasize(g_image, &ud_image, 25, 25, 1);

	double mark_width, mark_height;
	mark_width = 0.;
	mark_height = 0.;


	set_check("~give_error");
	herr = get_image_pointer1(*pdlg->m_Review->GetImage(), NULL, NULL, &wd, &ht);
	set_check("give_error");
	if(H_MSG_TRUE == herr)
	{
	}
	else
	{
		AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1!"));
		return rslt;
	}

	switch(rgn_idx)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		//Left right
		row = (Hlong)((m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx].bottom + m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx].top) * 0.5);
		col = (Hlong)((m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx].right + m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx].left) * 0.5);
		leng = (m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].right - m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].left) * 0.5;
		gen_measure_rectangle2(row,col,HTuple(0),leng,ALIGN_MEASURE_LENG,wd,ht,HTuple("nearest_neighbor"),&MeasureHandle);

		measure_pairs(lr_image, MeasureHandle, HTuple(2), HTuple(m_edit_thrsh_lr), HTuple("positive_strongest"), HTuple("all"),
							&RowEdgeFirstLR,&ColumnEdgeFirstLR,&AmplitudeFirst,  
							&RowEdgeSecondLR,&ColumnEdgeSecondLR,&AmplitudeSecond,
							&IntraDistance, &InterDistance);
	
		close_measure(MeasureHandle);
	
		edgeCnt = RowEdgeFirstLR.Num();
		if(0 < edgeCnt)
		{
			set_color(pdlg->m_Review->GetView(), HTuple("yellow"));
			set_draw(pdlg->m_Review->GetView(), HTuple("margin"));
			disp_line(pdlg->m_Review->GetView(), 0, ColumnEdgeFirstLR[0], ht, ColumnEdgeFirstLR[0]);
			disp_line(pdlg->m_Review->GetView(), 0,ColumnEdgeSecondLR[0], ht, ColumnEdgeSecondLR[0]);

			rslt_num++;
		}
		else
		{
			set_color(pdlg->m_Review->GetView(), HTuple("red"));
			set_draw(pdlg->m_Review->GetView(), HTuple("margin"));
			set_tposition(pdlg->m_Review->GetView(), 10, 10);
			write_string(pdlg->m_Review->GetView(), HTuple("Can't find!"));

			rslt = FALSE;
		}
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogFilmAlign::SetInspectFilmAlign-0)!"));
		break;
	}

	switch(rgn_idx)
	{
	//All
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		//Up dn
		row = (Hlong)((m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].bottom + m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].top) * 0.5);
		col = (Hlong)((m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].right + m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].left) * 0.5);
	
		leng = (m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].bottom - m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].top) * 0.5;
		gen_measure_rectangle2(row,col,HTuple(-1.57079632679),leng,ALIGN_MEASURE_LENG,wd,ht,HTuple("nearest_neighbor"),&MeasureHandle);

		measure_pos(ud_image, MeasureHandle, HTuple(2), HTuple(m_edit_thrsh_ud), HTuple("positive"), HTuple("first"),
							&RowEdgeFirstUD,&ColumnEdgeFirstUD,&AmplitudeFirst,  
							&IntraDistance);

		close_measure(MeasureHandle);
	
		edgeCnt = RowEdgeFirstUD.Num();
		if(0 < edgeCnt)
		{
			set_color(pdlg->m_Review->GetView(), HTuple("yellow"));
			set_draw(pdlg->m_Review->GetView(), HTuple("margin"));
			
			InterCol = (double)
			disp_line(pdlg->m_Review->GetView(), RowEdgeFirstUD[0], 0, RowEdgeFirstUD[0], wd);


			rslt_num++;
		}
		else
		{
			set_color(pdlg->m_Review->GetView(), HTuple("red"));
			set_draw(pdlg->m_Review->GetView(), HTuple("margin"));
			set_tposition(pdlg->m_Review->GetView(), 10, 10);
			write_string(pdlg->m_Review->GetView(), HTuple("Can't find!"));

			rslt = FALSE;
		}
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogFilmAlign::SetInspectFilmAlign-1)!"));
		break;
	}

	/*
	if(rslt)
	{
		if(1 < rslt_num)
		{
			//교차점 구하기
			switch(rgn_idx)
			{
			case 0://좌상단
				//Left->right
				start_x_lr = ColumnEdgeFirstLR[0];
				start_y_lr = 0.;
				end_x_lr = ColumnEdgeFirstLR[0];
				end_y_lr = (double)ht[0];

				//Down->Up
				start_x_ud = 0.;
				start_y_ud = RowEdgeFirstUD[0];
				end_x_ud = (double)wd[0];
				end_y_ud = RowEdgeFirstUD[0];
				break;
			case 2://우상단
				//Left->right
				start_x_lr = ColumnEdgeSecondLR[0];
				start_y_lr = 0.;
				end_x_lr = ColumnEdgeSecondLR[0];
				end_y_lr = (double)ht[0];

				//Down->Up
				start_x_ud = 0.;
				start_y_ud = RowEdgeFirstUD[0];
				end_x_ud = (double)wd[0];
				end_y_ud = RowEdgeFirstUD[0];
				break;
			case 3://우하단
				//Left->right
				start_x_lr = ColumnEdgeSecondLR[0];
				start_y_lr = 0.;
				end_x_lr = ColumnEdgeSecondLR[0];
				end_y_lr = (double)ht[0];

				//Down->Up
				start_x_ud = 0.;
				start_y_ud = RowEdgeSecondUD[0];
				end_x_ud = (double)wd[0];
				end_y_ud = RowEdgeSecondUD[0];
				break;
			case 5://좌하단
				//Left->right
				start_x_lr = ColumnEdgeFirstLR[0];
				start_y_lr = 0.;
				end_x_lr = ColumnEdgeFirstLR[0];
				end_y_lr = (double)ht[0];

				//Down->Up
				start_x_ud = 0.;
				start_y_ud = RowEdgeSecondUD[0];
				end_x_ud = (double)wd[0];
				end_y_ud = RowEdgeSecondUD[0];
				break;
			default:
				AfxMessageBox(_T("Error : invalid index(CDialogFilmAlign::SetInspectFilmAlign-2)!"));
				break;
			}

		}
		else
		{
			//높이 구하기
			switch(rgn_idx)
			{
			case 1://중상단
				//Left->right
				start_x_lr = ((double)wd[0]) * 0.5;
				start_y_lr = 0.;
				end_x_lr = ((double)wd[0]) * 0.5;
				end_y_lr = (double)ht[0];

				//Down->Up
				start_x_ud = 0.;
				start_y_ud = RowEdgeFirstUD[0];
				end_x_ud = (double)wd[0];
				end_y_ud = RowEdgeFirstUD[0];
				break;
			case 4://중하단
				//Left->right
				start_x_lr = ((double)wd[0]) * 0.5;
				start_y_lr = 0.;
				end_x_lr = ((double)wd[0]) * 0.5;
				end_y_lr = (double)ht[0];

				//Down->Up
				start_x_ud = 0.;
				start_y_ud = RowEdgeSecondUD[0];
				end_x_ud = (double)wd[0];
				end_y_ud = RowEdgeSecondUD[0];
				break;
			default:
				AfxMessageBox(_T("Error : invalid index(CDialogFilmAlign::SetInspectFilmAlign-3)!"));
				break;
			}
		}


		//교차점
		intersection_lines(start_y_lr, start_x_lr, end_y_lr, end_x_lr, start_y_ud, start_x_ud, end_y_ud, end_x_ud, &InterRow, &InterCol, &IsOver);
		*inter_x = (double)InterCol[0];
		*inter_y = (double)InterRow[0];

		edgeCnt = InterCol.Num();
		if(0 < edgeCnt)
		{
			set_color(pdlg->m_Review->GetView(), HTuple("blue"));
			set_draw(pdlg->m_Review->GetView(), HTuple("margin"));
			disp_line(pdlg->m_Review->GetView(), *inter_y, *inter_x - 50, *inter_y, *inter_x + 50);
			disp_line(pdlg->m_Review->GetView(), *inter_y - 50, *inter_x, *inter_y + 50, *inter_x);
			set_color(pdlg->m_Review->GetView(), HTuple("green"));
			set_draw(pdlg->m_Review->GetView(), HTuple("margin"));
			disp_line(pdlg->m_Review->GetView(), *inter_y, *inter_x - 20, *inter_y, *inter_x + 20);
			disp_line(pdlg->m_Review->GetView(), *inter_y - 20, *inter_x, *inter_y + 20, *inter_x);
		}
		else
		{
			set_color(pdlg->m_Review->GetView(), HTuple("red"));
			set_draw(pdlg->m_Review->GetView(), HTuple("margin"));
			set_tposition(pdlg->m_Review->GetView(), 10, 10);
			write_string(pdlg->m_Review->GetView(), HTuple("Can't find intersection!"));

			rslt = FALSE;
		}
	}

	set_color(pdlg->m_Review->GetView(), "blue");
	set_draw(pdlg->m_Review->GetView(), HTuple("margin"));
	set_tposition(pdlg->m_Review->GetView(), ALIGN_IMAGE_HEIGHT - 100, 10);
	write_string(pdlg->m_Review->GetView(), HTuple("(X, Y) position : (") + HTuple(*inter_x) + HTuple(",") + HTuple(*inter_y) + HTuple(")"));
	*/

	set_color(pdlg->m_Review->GetView(), "blue");
	set_draw(pdlg->m_Review->GetView(), HTuple("margin"));
	set_tposition(pdlg->m_Review->GetView(), 10, 10);
	write_string(pdlg->m_Review->GetView(), HTuple("(X, Y) position : (") + HTuple(ColumnEdgeFirstLR[0]) + HTuple(",") + HTuple(RowEdgeFirstUD[0]) + HTuple(")"));
	
	return rslt;
	//False return :edge 못찾을때->측정불량으로 표기
}

void CDialogFilmAlign::ClickBtnenhctrlFilmAlignApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ParameterCompare();
	Update(TRUE);
}

BOOL CDialogFilmAlign::SetLoadFilmAlignPara(CString path)
{
	return LoadFilmAlignPara(path);
}

BOOL CDialogFilmAlign::LoadFilmAlignPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	char path_temp[MAX_PATH];
	size_t getval;
	getval = 0;
	int i, j, idx;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	
	file_path = path + _T("\\FilmAlign.par");

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

		for(i = 0; i < FILM_ALIGN_MAX_NUM; i++)
		{
			for(j = 0; j < FILM_ALIGN_RGN_POS_MAX_MUM; j++)
			{
				//ins rect0//////////////////////////////////////////////////////////////
				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectLR_left%d_%d"), i, j);
				m_FilmAlignData.m_InsRectLR[i][j].left = GetPrivateProfileInt(_T("FILM ALIGN"), key, 0, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectLR_top%d_%d"), i, j);
				m_FilmAlignData.m_InsRectLR[i][j].top = GetPrivateProfileInt(_T("FILM ALIGN"), key, 0, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectLR_right%d_%d"), i, j);
				m_FilmAlignData.m_InsRectLR[i][j].right = GetPrivateProfileInt(_T("FILM ALIGN"), key, 0, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectLR_bottom%d_%d"), i, j);
				m_FilmAlignData.m_InsRectLR[i][j].bottom = GetPrivateProfileInt(_T("FILM ALIGN"), key, 0, file_path);

				if( (0 >= m_FilmAlignData.m_InsRectLR[i][j].left) ||
					(REVIEW_IMAGE_WIDTH < m_FilmAlignData.m_InsRectLR[i][j].left) ||
					(0 >= m_FilmAlignData.m_InsRectLR[i][j].top) ||
					(REVIEW_IMAGE_HEIGHT <m_FilmAlignData.m_InsRectLR[i][j].top) ||
					(0 >= m_FilmAlignData.m_InsRectLR[i][j].right) ||
					(REVIEW_IMAGE_WIDTH < m_FilmAlignData.m_InsRectLR[i][j].right) ||
					(0 >= m_FilmAlignData.m_InsRectLR[i][j].bottom) ||
					(REVIEW_IMAGE_HEIGHT < m_FilmAlignData.m_InsRectLR[i][j].bottom)
				  )
				{
					m_FilmAlignData.m_InsRectLR[i][j] = CRect(100, 100, 500, 500);
				}

				//ins rect1//////////////////////////////////////////////////////////////
				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectUD_left%d_%d"), i, j);
				m_FilmAlignData.m_InsRectUD[i][j].left = GetPrivateProfileInt(_T("FILM ALIGN"), key, 0, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectUD_top%d_%d"), i, j);
				m_FilmAlignData.m_InsRectUD[i][j].top = GetPrivateProfileInt(_T("FILM ALIGN"), key, 0, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectUD_right%d_%d"), i, j);
				m_FilmAlignData.m_InsRectUD[i][j].right = GetPrivateProfileInt(_T("FILM ALIGN"), key, 0, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectUD_bottom%d_%d"), i, j);
				m_FilmAlignData.m_InsRectUD[i][j].bottom = GetPrivateProfileInt(_T("FILM ALIGN"), key, 0, file_path);

				if( (0 >= m_FilmAlignData.m_InsRectUD[i][j].left) ||
					(REVIEW_IMAGE_WIDTH < m_FilmAlignData.m_InsRectUD[i][j].left) ||
					(0 >= m_FilmAlignData.m_InsRectUD[i][j].top) ||
					(REVIEW_IMAGE_HEIGHT <m_FilmAlignData.m_InsRectUD[i][j].top) ||
					(0 >= m_FilmAlignData.m_InsRectUD[i][j].right) ||
					(REVIEW_IMAGE_WIDTH < m_FilmAlignData.m_InsRectUD[i][j].right) ||
					(0 >= m_FilmAlignData.m_InsRectUD[i][j].bottom) ||
					(REVIEW_IMAGE_HEIGHT < m_FilmAlignData.m_InsRectUD[i][j].bottom)
				  )
				{
					m_FilmAlignData.m_InsRectUD[i][j] = CRect(700, 100, 1100, 500);
				}

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_dThrshLR%d_%d"), i, j);
				GetPrivateProfileString(_T("FILM ALIGN"), key, _T("0.0"), temp, sizeof(temp), file_path);
				dat = temp;
				wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
				m_FilmAlignData.m_dThrshLR[i][j] = atof(ctemp);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_dThrshUD%d_%d"), i, j);
				GetPrivateProfileString(_T("FILM ALIGN"), key, _T("0.0"), temp, sizeof(temp), file_path);
				dat = temp;
				wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
				m_FilmAlignData.m_dThrshUD[i][j] = atof(ctemp);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_iScanUD%d_%d"), i, j);
				GetPrivateProfileString(_T("FILM ALIGN"), key, _T("0.0"), temp, sizeof(temp), file_path);
				dat = temp;
				wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
				m_FilmAlignData.m_iScanUD[i][j] = atof(ctemp);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_iScanRD%d_%d"), i, j);
				GetPrivateProfileString(_T("FILM ALIGN"), key, _T("0.0"), temp, sizeof(temp), file_path);
				dat = temp;
				wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
				m_FilmAlignData.m_iScanRD[i][j] = atof(ctemp);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_dSpec_x%d_%d"), i, j);
				GetPrivateProfileString(_T("FILM ALIGN"), key, _T("0.0"), temp, sizeof(temp), file_path);
				dat = temp;
				wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
				m_FilmAlignData.m_dSpec_x[i][j] = atof(ctemp);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_dSpec_y%d_%d"), i, j);
				GetPrivateProfileString(_T("FILM ALIGN"), key, _T("0.0"), temp, sizeof(temp), file_path);
				dat = temp;
				wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
				m_FilmAlignData.m_dSpec_y[i][j] = atof(ctemp);
			}

		}

		key.Empty();
		key.Format(_T("m_FilmAlignData.m_dLight"));
		GetPrivateProfileString(_T("FILM ALIGN"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_FilmAlignData.m_dLight = atof(ctemp);

		key.Empty();
		key.Format(_T("m_FilmAlignData.m_bInspectUse"));
		GetPrivateProfileString(_T("FILM ALIGN"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_FilmAlignData.m_bInspectUse = (BOOL)(atof(ctemp));

	}
	fd=0;

	return rslt;
}

BOOL CDialogFilmAlign::SetSaveFilmAlignPara(CString path)
{
	return SaveFilmAlignPara(path);
}

BOOL CDialogFilmAlign::SaveFilmAlignPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	char path_temp[MAX_PATH];
	size_t getval;
	getval = 0;
	int i, j, idx;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	
	file_path = path + _T("\\FilmAlign.par");

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

		for(i = 0; i < FILM_ALIGN_MAX_NUM; i++)
		{
			for(j = 0; j < FILM_ALIGN_RGN_POS_MAX_MUM; j++)
			{
				//ins rect0//////////////////////////////////////////////////////////////
				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectLR_left%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_FilmAlignData.m_InsRectLR[i][j].left);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectLR_top%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_FilmAlignData.m_InsRectLR[i][j].top);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectLR_right%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_FilmAlignData.m_InsRectLR[i][j].right);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectLR_bottom%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_FilmAlignData.m_InsRectLR[i][j].bottom);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);


				//ins rect1//////////////////////////////////////////////////////////////
				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectUD_left%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_FilmAlignData.m_InsRectUD[i][j].left);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectUD_top%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_FilmAlignData.m_InsRectUD[i][j].top);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectUD_right%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_FilmAlignData.m_InsRectUD[i][j].right);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectUD_bottom%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_FilmAlignData.m_InsRectUD[i][j].bottom);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_dThrshLR%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%f"), m_FilmAlignData.m_dThrshLR[i][j]);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_dThrshUD%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%f"), m_FilmAlignData.m_dThrshUD[i][j]);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_iScanUD%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%f"), m_FilmAlignData.m_iScanUD[i][j]);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);


				key.Empty();
				key.Format(_T("m_FilmAlignData.m_iScanRD%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%f"), m_FilmAlignData.m_iScanRD[i][j]);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_dSpec_x%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%f"), m_FilmAlignData.m_dSpec_x[i][j]);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);


				key.Empty();
				key.Format(_T("m_FilmAlignData.m_dSpec_y%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%f"), m_FilmAlignData.m_dSpec_y[i][j]);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);
			}

		}

		key.Empty();
		key.Format(_T("m_FilmAlignData.m_dLight"));
		dat.Empty();
		dat.Format(_T("%f"), m_FilmAlignData.m_dLight);
		WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_FilmAlignData.m_bInspectUse"));
		dat.Empty();
		dat.Format(_T("%d"), m_FilmAlignData.m_bInspectUse);
		WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);
	}
	fd=0;
	
	return rslt;
}


BOOL CDialogFilmAlign::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	LoadFilmAlignPara(_T(FILM_ALIGN_PATH));

	m_slider_light.SetRange(0, 100);
	m_slider_light.SetPos(m_edit_light);

	Update(FALSE);

	UpdateData(FALSE);

	pdlg->m_Review->SetFilmAlignRect(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogFilmAlign::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	if(bShow)
	{
		m_btn_rgn_pos0.SetValue(FALSE);
		m_btn_rgn_pos1.SetValue(FALSE);
		m_btn_rgn_pos2.SetValue(FALSE);
		m_btn_rgn_pos3.SetValue(FALSE);
		m_btn_rgn_pos4.SetValue(FALSE);
		m_btn_rgn_pos5.SetValue(FALSE);
		switch(m_iSelectRgnNum)
		{
		case 0:
			m_btn_rgn_pos0.SetValue(TRUE);
			break;
		case 1:
			m_btn_rgn_pos1.SetValue(TRUE);
			break;
		case 2:
			m_btn_rgn_pos2.SetValue(TRUE);
			break;
		case 3:
			m_btn_rgn_pos3.SetValue(TRUE);
			break;
		case 4:
			m_btn_rgn_pos4.SetValue(TRUE);
			break;
		case 5:
			m_btn_rgn_pos5.SetValue(TRUE);
			break;
		default:
			AfxMessageBox(_T("Error : invalid index(CDialogFilmAlign::OnShowWindow)!"));
			break;
		}

		//20160717 ngh
		ClickBtnenhctrlAlignLightSet0();

		Update(FALSE);

		UpdateData(FALSE);
	}
	else
	{
		//20160717 ngh
		pdlg->m_Review->SetReviewLight();
		pdlg->m_Review->SetFilmAlignRect(FALSE);
	}
}

//2010412 ngh
void CDialogFilmAlign::SetUpdate(BOOL isUpdate)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	Update(isUpdate);
	UpdateData(isUpdate);
	if(IsWindowVisible())
	{
		pdlg->m_Review->SetFilmAlignRect(TRUE);
	}
	else
	{
		pdlg->m_Review->SetFilmAlignRect(FALSE);
	}
}

//X, Y 좌표계(수학)
void CDialogFilmAlign::Update(BOOL type)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	int i;
	int img_idx;
	int film_idx;
	img_idx = 0;
	film_idx = 0;

	int cell_x_idx, cell_y_idx, rgn_idx;
	cell_x_idx = 0;
	cell_y_idx = 0;

	rgn_idx = m_iSelectRgnNum;

	/*
	cell_x_idx--;
	if(0 > cell_x_idx)
	{
		cell_x_idx = 0;
		//Error log

	}
	cell_y_idx--;
	if(0 > cell_y_idx)
	{
		cell_y_idx = 0;
		//Error log

	}
	
	film_idx = (cell_y_idx * pdlg->m_RecipeCell->GetFilmCntX_coordXY()) + cell_x_idx;
	*/

	if(type)
	{
		m_FilmAlignData.m_dThrshLR[film_idx][rgn_idx] = m_edit_thrsh_lr;
		m_FilmAlignData.m_dThrshUD[film_idx][rgn_idx] = m_edit_thrsh_ud;

		m_FilmAlignData.m_iScanUD[film_idx][rgn_idx] = m_radio_scan_ud;
		m_FilmAlignData.m_iScanRD[film_idx][rgn_idx] = m_radio_scan_lr;

		switch(rgn_idx)
		{
		case 0:
		case 2:
		case 3:
		case 5:
			//LR
			pdlg->m_Review->GetFilmAlignRect(film_idx, rgn_idx, 0, &m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx]);
			//UD
			pdlg->m_Review->GetFilmAlignRect(film_idx, rgn_idx, 1, &m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx]);
		break;
		case 1:
		case 4:
			//UD
			pdlg->m_Review->GetFilmAlignRect(film_idx, rgn_idx, 0, &m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx]);
			break;
		default:
			AfxMessageBox(_T("Error : invalid index(CDialogFilmAlign::Update)!"));
			break;
		}

		m_FilmAlignData.m_dLight = m_edit_light;
		m_FilmAlignData.m_bInspectUse = m_check_inspect_use;

		//20160801 ngh
		m_FilmAlignData.m_dSpec_x[film_idx][rgn_idx] = m_edit_dSpec_x;
		m_FilmAlignData.m_dSpec_y[film_idx][rgn_idx] = m_edit_dSpec_y;

	}
	else
	{
		m_edit_thrsh_lr = m_FilmAlignData.m_dThrshLR[film_idx][rgn_idx];
		m_edit_thrsh_ud = m_FilmAlignData.m_dThrshUD[film_idx][rgn_idx];

		m_radio_scan_ud = m_FilmAlignData.m_iScanUD[film_idx][rgn_idx];
		m_radio_scan_lr = m_FilmAlignData.m_iScanRD[film_idx][rgn_idx];

		

		pdlg->m_Review->SetFilmAlignRect(TRUE);

		switch(rgn_idx)
		{
		case 0:
		case 2:
		case 3:
		case 5:
			//Region LR
			pdlg->m_Review->SetFilmAlignRect(cell_x_idx, cell_y_idx, film_idx, rgn_idx, 0, m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx]);
			//Region UD
			pdlg->m_Review->SetFilmAlignRect(cell_x_idx, cell_y_idx, film_idx, rgn_idx, 1, m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx]);
		break;
		case 1:
		case 4:
			//Region UD
			pdlg->m_Review->SetFilmAlignRect(cell_x_idx, cell_y_idx, film_idx, rgn_idx, 0, m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx]);
			break;
		default:
			AfxMessageBox(_T("Error : invalid index(CDialogFilmAlign::Update)!"));
			break;
		}
		
		m_edit_light = m_FilmAlignData.m_dLight;
		m_slider_light.SetPos(m_edit_light);
		m_check_inspect_use = m_FilmAlignData.m_bInspectUse;

		//20160801 ngh
		m_edit_dSpec_x = m_FilmAlignData.m_dSpec_x[film_idx][rgn_idx];
		m_edit_dSpec_y = m_FilmAlignData.m_dSpec_y[film_idx][rgn_idx];
	}
}

double CDialogFilmAlign::GetRobotPos(int robo_idx)
{
	return m_dMotPos[robo_idx];
}


void CDialogFilmAlign::OnNMCustomdrawSliderAlignLight0(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	unsigned short LedPwm = 0;

	m_edit_light = m_slider_light.GetPos();
	
	LedPwm = m_edit_light;

	pdlg->m_AFCtrl->SetReviewLed(LedPwm);
	UpdateData(FALSE);

	*pResult = 0;
}


void CDialogFilmAlign::ClickBtnenhctrlAlignLightSet0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	unsigned short LedPwm = 0;

	LedPwm = m_edit_light;
	m_slider_light.SetPos(LedPwm);

	pdlg->m_AFCtrl->SetReviewLed(LedPwm);

}

short CDialogFilmAlign::GetLight()
{
	return (unsigned short)m_FilmAlignData.m_dLight;
}

BOOL CDialogFilmAlign::GetInspectUse()
{
	return m_FilmAlignData.m_bInspectUse;
}

void CDialogFilmAlign::ClickBtnenhctrlFilmAlignCancel()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ShowWindow(SW_HIDE);
}


BOOL CDialogFilmAlign::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDialogFilmAlign::ParameterCompare()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strType;
	strType.Format(_T("RecipeFilmAlign_Para_"));
	int idx = 0;
	int img_idx = 0;
	int film_idx = 0;
	int rgn_idx = m_iSelectRgnNum;

	pdlg->ParaCompareLogWrite(m_FilmAlignData.m_dThrshUD[film_idx][rgn_idx], m_edit_thrsh_ud, strType + _T("m_radio_scan_ud"));
	pdlg->ParaCompareLogWrite(m_FilmAlignData.m_dThrshLR[film_idx][rgn_idx], m_edit_thrsh_lr, strType + _T("m_edit_thrsh_lr"));
	pdlg->ParaCompareLogWrite((int)m_FilmAlignData.m_iScanUD[film_idx][rgn_idx], m_radio_scan_ud, strType + _T("m_radio_scan_ud"));
	pdlg->ParaCompareLogWrite((int)m_FilmAlignData.m_iScanRD[film_idx][rgn_idx], m_radio_scan_lr, strType + _T("m_radio_scan_ud"));
	pdlg->ParaCompareLogWrite(m_FilmAlignData.m_dLight, m_edit_light, strType + _T("m_edit_light"));
	pdlg->ParaCompareLogWrite((int)m_FilmAlignData.m_bInspectUse, m_check_inspect_use, strType + _T("m_check_inspect_use"));
}

//20160801 ngh
double CDialogFilmAlign::GetFilmAlignSpecX(int rgn_idx)
{
	int film_idx = 0;
	return m_FilmAlignData.m_dSpec_x[film_idx][rgn_idx];
}

double CDialogFilmAlign::GetFilmAlignSpecY(int rgn_idx)
{
	int film_idx = 0;
	return m_FilmAlignData.m_dSpec_y[film_idx][rgn_idx];
}