// dialog\DialogRecipeCellPCL.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EMIControl.h"
#include "dialog\DialogRecipeCellPCL.h"

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


// CDialogRecipeCellPCL 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogRecipeCellPCL, CDialogEx)

CDialogRecipeCellPCL::CDialogRecipeCellPCL(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogRecipeCellPCL::IDD, pParent)
	, m_edit_recipe_pcl_size_x0_0(0.0)
	, m_edit_recipe_pcl_size_y0_0(0.0)
	, m_edit_recipe_pcl_size_x1_0(0.0)
	, m_edit_recipe_pcl_size_y1_0(0.0)
	, m_edit_recipe_pcl_cnt_x_0(0.0)
	, m_edit_recipe_pcl_cnt_y_0(0.0)
	, m_edit_recipe_pcl_pitch_x_0(0.0)
	, m_edit_recipe_pcl_pitch_y_0(0.0)
	, m_edit_recipe_pcl_grpcnt_x_0(0.0)
	, m_edit_recipe_pcl_grpcnt_y_0(0.0)
	, m_edit_recipe_pcl_grppitch_x_0(0.0)
	, m_edit_recipe_pcl_grppitch_y_0(0.0)
	, m_edit_recipe_pcl_pos_x_0(0.0)
	, m_edit_recipe_pcl_pos_y_0(0.0)
	, m_edit_recipe_pcl_size_x0_1(0.0)
	, m_edit_recipe_pcl_size_y0_1(0.0)
	, m_edit_recipe_pcl_size_x1_1(0.0)
	, m_edit_recipe_pcl_size_y1_1(0.0)
	, m_edit_recipe_pcl_cnt_x_1(0.0)
	, m_edit_recipe_pcl_cnt_y_1(0.0)
	, m_edit_recipe_pcl_pitch_x_1(0.0)
	, m_edit_recipe_pcl_pitch_y_1(0.0)
	, m_edit_recipe_pcl_grpcnt_x_1(0.0)
	, m_edit_recipe_pcl_grpcnt_y_1(0.0)
	, m_edit_recipe_pcl_grppitch_x_1(0.0)
	, m_edit_recipe_pcl_grppitch_y_1(0.0)
	, m_edit_recipe_pcl_pos_x_1(0.0)
	, m_edit_recipe_pcl_pos_y_1(0.0)
	, m_edit_recipe_pcl_size_x0_2(0.0)
	, m_edit_recipe_pcl_size_y0_2(0.0)
	, m_edit_recipe_pcl_size_x1_2(0.0)
	, m_edit_recipe_pcl_size_y1_2(0.0)
	, m_edit_recipe_pcl_cnt_x_2(0.0)
	, m_edit_recipe_pcl_cnt_y_2(0.0)
	, m_edit_recipe_pcl_pitch_x_2(0.0)
	, m_edit_recipe_pcl_pitch_y_2(0.0)
	, m_edit_recipe_pcl_grpcnt_x_2(0.0)
	, m_edit_recipe_pcl_grpcnt_y_2(0.0)
	, m_edit_recipe_pcl_grppitch_x_2(0.0)
	, m_edit_recipe_pcl_grppitch_y_2(0.0)
	, m_edit_recipe_pcl_pos_x_2(0.0)
	, m_edit_recipe_pcl_pos_y_2(0.0)
	, m_edit_recipe_pcl_size_x0_3(0.0)
	, m_edit_recipe_pcl_size_y0_3(0.0)
	, m_edit_recipe_pcl_size_x1_3(0.0)
	, m_edit_recipe_pcl_size_y1_3(0.0)
	, m_edit_recipe_pcl_cnt_x_3(0.0)
	, m_edit_recipe_pcl_cnt_y_3(0.0)
	, m_edit_recipe_pcl_pitch_x_3(0.0)
	, m_edit_recipe_pcl_pitch_y_3(0.0)
	, m_edit_recipe_pcl_grpcnt_x_3(0.0)
	, m_edit_recipe_pcl_grpcnt_y_3(0.0)
	, m_edit_recipe_pcl_grppitch_x_3(0.0)
	, m_edit_recipe_pcl_grppitch_y_3(0.0)
	, m_edit_recipe_pcl_pos_x_3(0.0)
	, m_edit_recipe_pcl_pos_y_3(0.0)
{
	for(int i = 0; i < 100; i++)
	{
		m_dRecipeCellPCLPara[i] = 0;
	}
}

CDialogRecipeCellPCL::~CDialogRecipeCellPCL()
{
	SaveRecipeCellPCLPara(_T(RECIPE_CELL_PATH));
}

void CDialogRecipeCellPCL::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_POS_X_0, m_edit_recipe_pcl_pos_x_0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_POS_Y_0, m_edit_recipe_pcl_pos_y_0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_SIZE_X0_0, m_edit_recipe_pcl_size_x0_0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_SIZE_Y0_0, m_edit_recipe_pcl_size_y0_0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_SIZE_X1_0, m_edit_recipe_pcl_size_x1_0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_SIZE_Y1_0, m_edit_recipe_pcl_size_y1_0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_CNT_X_0, m_edit_recipe_pcl_cnt_x_0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_CNT_Y_0, m_edit_recipe_pcl_cnt_y_0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_PITCH_X_0, m_edit_recipe_pcl_pitch_x_0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_PITCH_Y_0, m_edit_recipe_pcl_pitch_y_0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_GRPCNT_X_0, m_edit_recipe_pcl_grpcnt_x_0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_GRPCNT_Y_0, m_edit_recipe_pcl_grpcnt_y_0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_GRPPITCH_X_0, m_edit_recipe_pcl_grppitch_x_0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_GRPPITCH_Y_0, m_edit_recipe_pcl_grppitch_y_0);
	
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_POS_X_1, m_edit_recipe_pcl_pos_x_1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_POS_Y_1, m_edit_recipe_pcl_pos_y_1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_SIZE_X0_1, m_edit_recipe_pcl_size_x0_1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_SIZE_Y0_1, m_edit_recipe_pcl_size_y0_1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_SIZE_X1_1, m_edit_recipe_pcl_size_x1_1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_SIZE_Y1_1, m_edit_recipe_pcl_size_y1_1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_CNT_X_1, m_edit_recipe_pcl_cnt_x_1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_CNT_Y_1, m_edit_recipe_pcl_cnt_y_1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_PITCH_X_1, m_edit_recipe_pcl_pitch_x_1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_PITCH_Y_1, m_edit_recipe_pcl_pitch_y_1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_GRPCNT_X_1, m_edit_recipe_pcl_grpcnt_x_1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_GRPCNT_Y_1, m_edit_recipe_pcl_grpcnt_y_1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_GRPPITCH_X_1, m_edit_recipe_pcl_grppitch_x_1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_GRPPITCH_Y_1, m_edit_recipe_pcl_grppitch_y_1);

	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_POS_X_2, m_edit_recipe_pcl_pos_x_2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_POS_Y_2, m_edit_recipe_pcl_pos_y_2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_SIZE_X0_2, m_edit_recipe_pcl_size_x0_2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_SIZE_Y0_2, m_edit_recipe_pcl_size_y0_2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_SIZE_X1_2, m_edit_recipe_pcl_size_x1_2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_SIZE_Y1_2, m_edit_recipe_pcl_size_y1_2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_CNT_X_2, m_edit_recipe_pcl_cnt_x_2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_CNT_Y_2, m_edit_recipe_pcl_cnt_y_2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_PITCH_X_2, m_edit_recipe_pcl_pitch_x_2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_PITCH_Y_2, m_edit_recipe_pcl_pitch_y_2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_GRPCNT_X_2, m_edit_recipe_pcl_grpcnt_x_2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_GRPCNT_Y_2, m_edit_recipe_pcl_grpcnt_y_2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_GRPPITCH_X_2, m_edit_recipe_pcl_grppitch_x_2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_GRPPITCH_Y_2, m_edit_recipe_pcl_grppitch_y_2);

	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_POS_X_3, m_edit_recipe_pcl_pos_x_3);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_POS_Y_3, m_edit_recipe_pcl_pos_y_3);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_SIZE_X0_3, m_edit_recipe_pcl_size_x0_3);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_SIZE_Y0_3, m_edit_recipe_pcl_size_y0_3);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_SIZE_X1_3, m_edit_recipe_pcl_size_x1_3);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_SIZE_Y1_3, m_edit_recipe_pcl_size_y1_3);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_CNT_X_3, m_edit_recipe_pcl_cnt_x_3);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_CNT_Y_3, m_edit_recipe_pcl_cnt_y_3);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_PITCH_X_3, m_edit_recipe_pcl_pitch_x_3);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_PITCH_Y_3, m_edit_recipe_pcl_pitch_y_3);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_GRPCNT_X_3, m_edit_recipe_pcl_grpcnt_x_3);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_GRPCNT_Y_3, m_edit_recipe_pcl_grpcnt_y_3);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_GRPPITCH_X_3, m_edit_recipe_pcl_grppitch_x_3);
	DDX_Text(pDX, IDC_EDIT_RECIPE_PCL_GRPPITCH_Y_3, m_edit_recipe_pcl_grppitch_y_3);
}


BEGIN_MESSAGE_MAP(CDialogRecipeCellPCL, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogRecipeCellPCL 메시지 처리기입니다.


BOOL CDialogRecipeCellPCL::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDialogRecipeCellPCL::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	LoadRecipeCellPCLPara(_T(RECIPE_CELL_PATH));
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogRecipeCellPCL::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		UpdateData(FALSE);
	}
	else
	{

	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
BEGIN_EVENTSINK_MAP(CDialogRecipeCellPCL, CDialogEx)
	ON_EVENT(CDialogRecipeCellPCL, IDC_BTNENHCTRL_RECIPE_CELL_SAVE, DISPID_CLICK, CDialogRecipeCellPCL::ClickBtnenhctrlRecipeCellSave, VTS_NONE)
	ON_EVENT(CDialogRecipeCellPCL, IDC_BTNENHCTRL_RECIPE_CELL_APPLY, DISPID_CLICK, CDialogRecipeCellPCL::ClickBtnenhctrlRecipeCellApply, VTS_NONE)
	ON_EVENT(CDialogRecipeCellPCL, IDC_BTNENHCTRL_CELL_PANEL_CANCEL, DISPID_CLICK, CDialogRecipeCellPCL::ClickBtnenhctrlCellPanelCancel, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogRecipeCellPCL::ClickBtnenhctrlRecipeCellSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CDialogRecipeModel mod_dlg;
	mod_dlg.SetOpen();
	UpdateData(TRUE);
	ParameterCompare();
	Update(TRUE);
	SetSaveRecipeCellPCLPara(RECIPE_CELL_PCL_PATH);
	SetSaveRecipeCellPCLPara(mod_dlg.GetRecipePath());
}


void CDialogRecipeCellPCL::ClickBtnenhctrlRecipeCellApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	ParameterCompare();
	Update(TRUE);
	pdlg->m_Glass->SetUpdate();
}


void CDialogRecipeCellPCL::ClickBtnenhctrlCellPanelCancel()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ShowWindow(SW_HIDE);
}

double CDialogRecipeCellPCL::GetPCLSizeX0(int type)
{
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_size_x0_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_size_x0_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_size_x0_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_size_x0_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLSizeY0(int type)
{
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_size_y0_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_size_y0_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_size_y0_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_size_y0_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLSizeX1(int type)
{
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_size_x1_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_size_x1_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_size_x1_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_size_x1_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLSizeY1(int type)
{
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_size_y1_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_size_y1_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_size_y1_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_size_y1_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLCntX(int type)
{
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_cnt_x_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_cnt_x_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_cnt_x_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_cnt_x_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLCntY(int type)
{
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_cnt_y_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_cnt_y_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_cnt_y_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_cnt_y_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLPitchX(int type)
{
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_pitch_x_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_pitch_x_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_pitch_x_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_pitch_x_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLPitchY(int type)
{
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_pitch_y_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_pitch_y_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_pitch_y_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_pitch_y_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLGrpCntX(int type)
{
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_grpcnt_x_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_grpcnt_x_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_grpcnt_x_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_grpcnt_x_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLGrpCntY(int type)
{
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_grpcnt_y_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_grpcnt_y_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_grpcnt_y_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_grpcnt_y_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLGrpPitchX(int type)
{
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_grppitch_x_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_grppitch_x_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_grppitch_x_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_grppitch_x_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLGrpPitchY(int type)
{
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_grppitch_y_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_grppitch_y_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_grppitch_y_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_grppitch_y_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLPosFromPCLAlignX(int type)
{
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_pos_x_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_pos_x_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_pos_x_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_pos_x_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLPosFromPCLAlignY(int type)
{
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_pos_y_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_pos_y_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_pos_y_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_pos_y_3;
		break;
	}
	return rslt;
}


double CDialogRecipeCellPCL::GetPCLSizeX0_coordXY(int type)
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_size_y0_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_size_y0_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_size_y0_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_size_y0_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLSizeY0_coordXY(int type)
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_size_x0_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_size_x0_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_size_x0_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_size_x0_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLSizeX1_coordXY(int type)
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_size_y1_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_size_y1_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_size_y1_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_size_y1_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLSizeY1_coordXY(int type)
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_size_x1_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_size_x1_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_size_x1_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_size_x1_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLCntX_coordXY(int type)
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_cnt_y_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_cnt_y_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_cnt_y_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_cnt_y_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLCntY_coordXY(int type)
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_cnt_x_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_cnt_x_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_cnt_x_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_cnt_x_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLPitchX_coordXY(int type)
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = -m_edit_recipe_pcl_pitch_y_0;
		break;
	case 1:
		rslt = -m_edit_recipe_pcl_pitch_y_1;
		break;
	case 2:
		rslt = -m_edit_recipe_pcl_pitch_y_2;
		break;
	case 3:
		rslt = -m_edit_recipe_pcl_pitch_y_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLPitchY_coordXY(int type)
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_pitch_x_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_pitch_x_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_pitch_x_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_pitch_x_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLGrpCntX_coordXY(int type)
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_grpcnt_y_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_grpcnt_y_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_grpcnt_y_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_grpcnt_y_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLGrpCntY_coordXY(int type)
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_grpcnt_x_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_grpcnt_x_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_grpcnt_x_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_grpcnt_x_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLGrpPitchX_coordXY(int type)
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = -m_edit_recipe_pcl_grppitch_y_0;
		break;
	case 1:
		rslt = -m_edit_recipe_pcl_grppitch_y_1;
		break;
	case 2:
		rslt = -m_edit_recipe_pcl_grppitch_y_2;
		break;
	case 3:
		rslt = -m_edit_recipe_pcl_grppitch_y_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLGrpPitchY_coordXY(int type)
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_grppitch_x_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_grppitch_x_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_grppitch_x_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_grppitch_x_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLPosFromPCLAlignX_coordXY(int type)
{
	//축이 x, y 바뀌어 있다. 반대로 리턴
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = -m_edit_recipe_pcl_pos_y_0;
		break;
	case 1:
		rslt = -m_edit_recipe_pcl_pos_y_1;
		break;
	case 2:
		rslt = -m_edit_recipe_pcl_pos_y_2;
		break;
	case 3:
		rslt = -m_edit_recipe_pcl_pos_y_3;
		break;
	}
	return rslt;
}

double CDialogRecipeCellPCL::GetPCLPosFromPCLAlignY_coordXY(int type)
{
	//축이 x, y 바뀌어 있다. 반대로 리턴
	double rslt = 0.;
	switch(type)
	{
	case 0:
		rslt = m_edit_recipe_pcl_pos_x_0;
		break;
	case 1:
		rslt = m_edit_recipe_pcl_pos_x_1;
		break;
	case 2:
		rslt = m_edit_recipe_pcl_pos_x_2;
		break;
	case 3:
		rslt = m_edit_recipe_pcl_pos_x_3;
		break;
	}
	return rslt;
}

BOOL CDialogRecipeCellPCL::SetLoadRecipeCellPCLPara(CString path)
{
	return LoadRecipeCellPCLPara(path);
}

BOOL CDialogRecipeCellPCL::LoadRecipeCellPCLPara(CString path)
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


	file_path = path + _T("\\RecipeCellPCL.par");

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
		

		//up
		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_x0_0"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_size_x0_0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_y0_0"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_size_y0_0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_x1_0"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_size_x1_0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_y1_0"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_size_y1_0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_cnt_x_0"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_cnt_x_0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_cnt_y_0"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_cnt_y_0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pitch_x_0"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_pitch_x_0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pitch_y_0"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_pitch_y_0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grpcnt_x_0"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_grpcnt_x_0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grpcnt_y_0"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_grpcnt_y_0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grppitch_x_0"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_grppitch_x_0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grppitch_y_0"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_grppitch_y_0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pos_x_0"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_pos_x_0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pos_y_0"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_pos_y_0 = atof(ctemp);

		//down
		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_x0_1"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_size_x0_1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_y0_1"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_size_y0_1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_x1_1"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_size_x1_1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_y1_1"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_size_y1_1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_cnt_x_1"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_cnt_x_1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_cnt_y_1"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_cnt_y_1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pitch_x_1"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_pitch_x_1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pitch_y_1"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_pitch_y_1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grpcnt_x_1"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_grpcnt_x_1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grpcnt_y_1"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_grpcnt_y_1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grppitch_x_1"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_grppitch_x_1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grppitch_y_1"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_grppitch_y_1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pos_x_1"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_pos_x_1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pos_y_1"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_pos_y_1 = atof(ctemp);

		//left
		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_x0_2"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_size_x0_2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_y0_2"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_size_y0_2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_x1_2"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_size_x1_2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_y1_2"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_size_y1_2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_cnt_x_2"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_cnt_x_2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_cnt_y_2"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_cnt_y_2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pitch_x_2"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_pitch_x_2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pitch_y_2"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_pitch_y_2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grpcnt_x_2"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_grpcnt_x_2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grpcnt_y_2"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_grpcnt_y_2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grppitch_x_2"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_grppitch_x_2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grppitch_y_2"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_grppitch_y_2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pos_x_2"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_pos_x_2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pos_y_2"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_pos_y_2 = atof(ctemp);

		//right
		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_x0_3"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_size_x0_3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_y0_3"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_size_y0_3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_x1_3"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_size_x1_3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_y1_3"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_size_y1_3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_cnt_x_3"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_cnt_x_3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_cnt_y_3"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_cnt_y_3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pitch_x_3"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_pitch_x_3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pitch_y_3"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_pitch_y_3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grpcnt_x_3"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_grpcnt_x_3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grpcnt_y_3"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_grpcnt_y_3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grppitch_x_3"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_grppitch_x_3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grppitch_y_3"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_grppitch_y_3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pos_x_3"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_pos_x_3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pos_y_3"));
		GetPrivateProfileString(_T("RECIPE CELL PCL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_pcl_pos_y_3 = atof(ctemp);
	}
	fd=0;

	return rslt;
}

BOOL CDialogRecipeCellPCL::SetSaveRecipeCellPCLPara(CString path)
{
	return SaveRecipeCellPCLPara(path);
}

BOOL CDialogRecipeCellPCL::SaveRecipeCellPCLPara(CString path)
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
	
	file_path = path + _T("\\RecipeCellPCL.par");

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

		//up
		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_x0_0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_size_x0_0);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_y0_0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_size_y0_0);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_x1_0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_size_x1_0);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_y1_0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_size_y1_0);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_cnt_x_0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_cnt_x_0);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_cnt_y_0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_cnt_y_0);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pitch_x_0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_pitch_x_0);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pitch_y_0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_pitch_y_0);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grpcnt_x_0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_grpcnt_x_0);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grpcnt_y_0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_grpcnt_y_0);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grppitch_x_0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_grppitch_x_0);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grppitch_y_0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_grppitch_y_0);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pos_x_0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_pos_x_0);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pos_y_0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_pos_y_0);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		//down
		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_x0_1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_size_x0_1);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_y0_1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_size_y0_1);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_x1_1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_size_x1_1);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_y1_1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_size_y1_1);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_cnt_x_1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_cnt_x_1);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_cnt_y_1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_cnt_y_1);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pitch_x_1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_pitch_x_1);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pitch_y_1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_pitch_y_1);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grpcnt_x_1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_grpcnt_x_1);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grpcnt_y_1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_grpcnt_y_1);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grppitch_x_1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_grppitch_x_1);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grppitch_y_1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_grppitch_y_1);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pos_x_1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_pos_x_1);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pos_y_1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_pos_y_1);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		//left
		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_x0_2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_size_x0_2);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_y0_2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_size_y0_2);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_x1_2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_size_x1_2);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_y1_2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_size_y1_2);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_cnt_x_2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_cnt_x_2);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_cnt_y_2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_cnt_y_2);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pitch_x_2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_pitch_x_2);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pitch_y_2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_pitch_y_2);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grpcnt_x_2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_grpcnt_x_2);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grpcnt_y_2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_grpcnt_y_2);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grppitch_x_2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_grppitch_x_2);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grppitch_y_2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_grppitch_y_2);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pos_x_2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_pos_x_2);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pos_y_2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_pos_y_2);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		//right
		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_x0_3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_size_x0_3);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_y0_3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_size_y0_3);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_x1_3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_size_x1_3);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_size_y1_3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_size_y1_3);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_cnt_x_3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_cnt_x_3);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_cnt_y_3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_cnt_y_3);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pitch_x_3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_pitch_x_3);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pitch_y_3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_pitch_y_3);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grpcnt_x_3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_grpcnt_x_3);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grpcnt_y_3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_grpcnt_y_3);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grppitch_x_3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_grppitch_x_3);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_grppitch_y_3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_grppitch_y_3);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pos_x_3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_pos_x_3);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_pcl_pos_y_3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_pcl_pos_y_3);
		WritePrivateProfileString(_T("RECIPE CELL PCL"), key, dat, file_path);
	}
	fd=0;
	
	return rslt;
}


void CDialogRecipeCellPCL::Update(BOOL isUpdate)
{
	int idx = 0;
	if(isUpdate)
	{
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_size_x0_0;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_size_y0_0;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_size_x1_0;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_size_y1_0;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_cnt_x_0;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_cnt_y_0;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_pitch_x_0;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_pitch_y_0;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_grpcnt_x_0;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_grpcnt_y_0;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_grppitch_x_0;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_grppitch_y_0;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_pos_x_0;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_pos_y_0;

		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_size_x0_1;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_size_y0_1;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_size_x1_1;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_size_y1_1;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_cnt_x_1;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_cnt_y_1;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_pitch_x_1;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_pitch_y_1;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_grpcnt_x_1;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_grpcnt_y_1;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_grppitch_x_1;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_grppitch_y_1;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_pos_x_1;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_pos_y_1;

		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_size_x0_2;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_size_y0_2;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_size_x1_2;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_size_y1_2;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_cnt_x_2;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_cnt_y_2;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_pitch_x_2;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_pitch_y_2;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_grpcnt_x_2;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_grpcnt_y_2;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_grppitch_x_2;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_grppitch_y_2;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_pos_x_2;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_pos_y_2;

		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_size_x0_3;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_size_y0_3;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_size_x1_3;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_size_y1_3;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_cnt_x_3;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_cnt_y_3;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_pitch_x_3;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_pitch_y_3;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_grpcnt_x_3;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_grpcnt_y_3;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_grppitch_x_3;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_grppitch_y_3;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_pos_x_3;
		m_dRecipeCellPCLPara[idx++] = m_edit_recipe_pcl_pos_y_3;
		UpdateData(FALSE);
	}
	else
	{
		m_edit_recipe_pcl_size_x0_0 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_size_y0_0 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_size_x1_0 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_size_y1_0 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_cnt_x_0 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_cnt_y_0 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_pitch_x_0 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_pitch_y_0 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_grpcnt_x_0 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_grpcnt_y_0 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_grppitch_x_0 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_grppitch_y_0 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_pos_x_0 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_pos_y_0 = m_dRecipeCellPCLPara[idx++];

		m_edit_recipe_pcl_size_x0_1 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_size_y0_1 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_size_x1_1 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_size_y1_1 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_cnt_x_1 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_cnt_y_1 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_pitch_x_1 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_pitch_y_1 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_grpcnt_x_1 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_grpcnt_y_1 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_grppitch_x_1 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_grppitch_y_1 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_pos_x_1 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_pos_y_1 = m_dRecipeCellPCLPara[idx++];

		m_edit_recipe_pcl_size_x0_2 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_size_y0_2 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_size_x1_2 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_size_y1_2 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_cnt_x_2 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_cnt_y_2 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_pitch_x_2 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_pitch_y_2 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_grpcnt_x_2 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_grpcnt_y_2 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_grppitch_x_2 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_grppitch_y_2 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_pos_x_2 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_pos_y_2 = m_dRecipeCellPCLPara[idx++];

		m_edit_recipe_pcl_size_x0_3 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_size_y0_3 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_size_x1_3 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_size_y1_3 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_cnt_x_3 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_cnt_y_3 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_pitch_x_3 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_pitch_y_3 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_grpcnt_x_3 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_grpcnt_y_3 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_grppitch_x_3 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_grppitch_y_3 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_pos_x_3 = m_dRecipeCellPCLPara[idx++];
		m_edit_recipe_pcl_pos_y_3 = m_dRecipeCellPCLPara[idx++];
	}
}

void CDialogRecipeCellPCL::ParameterCompare()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strType;
	strType.Format(_T("RecipeCellPCL_Para_"));
	int idx = 0;

	//up
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_size_x0_0, strType + _T("m_edit_recipe_pcl_size_x0_0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_size_y0_0, strType + _T("m_edit_recipe_pcl_size_y0_0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_size_x1_0, strType + _T("m_edit_recipe_pcl_size_x1_0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_size_y1_0, strType + _T("m_edit_recipe_pcl_size_y1_0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_cnt_x_0, strType + _T("m_edit_recipe_pcl_cnt_x_0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_cnt_y_0, strType + _T("m_edit_recipe_pcl_cnt_y_0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_pitch_x_0, strType + _T("m_edit_recipe_pcl_pitch_x_0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_pitch_y_0, strType + _T("m_edit_recipe_pcl_pitch_y_0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_grpcnt_x_0, strType + _T("m_edit_recipe_pcl_grpcnt_x_0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_grpcnt_y_0, strType + _T("m_edit_recipe_pcl_grpcnt_y_0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_grppitch_x_0, strType + _T("m_edit_recipe_pcl_grppitch_x_0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_grppitch_y_0, strType + _T("m_edit_recipe_pcl_grppitch_y_0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_pos_x_0, strType + _T("m_edit_recipe_pcl_pos_x_0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_pos_y_0, strType + _T("m_edit_recipe_pcl_pos_y_0"));

	//down
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_size_x0_1, strType + _T("m_edit_recipe_pcl_size_x0_1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_size_y0_1, strType + _T("m_edit_recipe_pcl_size_y0_1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_size_x1_1, strType + _T("m_edit_recipe_pcl_size_x1_1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_size_y1_1, strType + _T("m_edit_recipe_pcl_size_y1_1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_cnt_x_1, strType + _T("m_edit_recipe_pcl_cnt_x_1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_cnt_y_1, strType + _T("m_edit_recipe_pcl_cnt_y_1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_pitch_x_1, strType + _T("m_edit_recipe_pcl_pitch_x_1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_pitch_y_1, strType + _T("m_edit_recipe_pcl_pitch_y_1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_grpcnt_x_1, strType + _T("m_edit_recipe_pcl_grpcnt_x_1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_grpcnt_y_1, strType + _T("m_edit_recipe_pcl_grpcnt_y_1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_grppitch_x_1, strType + _T("m_edit_recipe_pcl_grppitch_x_1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_grppitch_y_1, strType + _T("m_edit_recipe_pcl_grppitch_y_1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_pos_x_1, strType + _T("m_edit_recipe_pcl_pos_x_1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_pos_y_1, strType + _T("m_edit_recipe_pcl_pos_y_1"));

	//left
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_size_x0_2, strType + _T("m_edit_recipe_pcl_size_x0_2"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_size_y0_2, strType + _T("m_edit_recipe_pcl_size_y0_2"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_size_x1_2, strType + _T("m_edit_recipe_pcl_size_x1_2"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_size_y1_2, strType + _T("m_edit_recipe_pcl_size_y1_2"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_cnt_x_2, strType + _T("m_edit_recipe_pcl_cnt_x_2"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_cnt_y_2, strType + _T("m_edit_recipe_pcl_cnt_y_2"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_pitch_x_2, strType + _T("m_edit_recipe_pcl_pitch_x_2"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_pitch_y_2, strType + _T("m_edit_recipe_pcl_pitch_y_2"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_grpcnt_x_2, strType + _T("m_edit_recipe_pcl_grpcnt_x_2"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_grpcnt_y_2, strType + _T("m_edit_recipe_pcl_grpcnt_y_2"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_grppitch_x_2, strType + _T("m_edit_recipe_pcl_grppitch_x_2"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_grppitch_y_2, strType + _T("m_edit_recipe_pcl_grppitch_y_2"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_pos_x_2, strType + _T("m_edit_recipe_pcl_pos_x_2"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_pos_y_2, strType + _T("m_edit_recipe_pcl_pos_y_2"));

	//right
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_size_x0_3, strType + _T("m_edit_recipe_pcl_size_x0_3"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_size_y0_3, strType + _T("m_edit_recipe_pcl_size_y0_3"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_size_x1_3, strType + _T("m_edit_recipe_pcl_size_x1_3"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_size_y1_3, strType + _T("m_edit_recipe_pcl_size_y1_3"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_cnt_x_3, strType + _T("m_edit_recipe_pcl_cnt_x_3"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_cnt_y_3, strType + _T("m_edit_recipe_pcl_cnt_y_3"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_pitch_x_3, strType + _T("m_edit_recipe_pcl_pitch_x_3"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_pitch_y_3, strType + _T("m_edit_recipe_pcl_pitch_y_3"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_grpcnt_x_3, strType + _T("m_edit_recipe_pcl_grpcnt_x_3"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_grpcnt_y_3, strType + _T("m_edit_recipe_pcl_grpcnt_y_3"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_grppitch_x_3, strType + _T("m_edit_recipe_pcl_grppitch_x_3"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_grppitch_y_3, strType + _T("m_edit_recipe_pcl_grppitch_y_3"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_pos_x_3, strType + _T("m_edit_recipe_pcl_pos_x_3"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPCLPara[idx++], m_edit_recipe_pcl_pos_y_3, strType + _T("m_edit_recipe_pcl_pos_y_3"));
}