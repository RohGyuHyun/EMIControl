// Dialog/DialogGlassManuCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EMIControl.h"
#include "Dialog/DialogGlassManuCtrl.h"
#include "afxdialogex.h"

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

// CDialogGlassManuCtrl 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogGlassManuCtrl, CDialogEx)

CDialogGlassManuCtrl::CDialogGlassManuCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogGlassManuCtrl::IDD, pParent)
{
	m_bLoaderUse = FALSE;
}

CDialogGlassManuCtrl::~CDialogGlassManuCtrl()
{
	SaveGlassManuPara(_T(GLASS_MANU_PATH));

	KillTimer(0);
}

void CDialogGlassManuCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTNENHCTRL_GL_JOG_SPD1, m_btn_gl_jog_spd1);
	DDX_Control(pDX, IDC_BTNENHCTRL_GL_JOG_SPD2, m_btn_gl_jog_spd2);
	DDX_Control(pDX, IDC_BTNENHCTRL_GL_JOG_SPD3, m_btn_gl_jog_spd3);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_AE4, m_btn_io_x051);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_AE9, m_btn_io_x050);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_EMO_LEFT, m_btn_io_emo_left);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_EMO_BACK, m_btn_io_emo_back);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_EMO_IN, m_btn_io_emo_in);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_EMO_OP, m_btn_io_emo_op);
	DDX_Control(pDX, IDC_BTNENHCTRL_DOOR_LOCK, m_btn_door_lock);
	DDX_Control(pDX, IDC_BTNENHCTRL_DOOR_UNLOCK, m_btn_door_unlock);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_DOOR1, m_btn_io_door1);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_DOOR2_LEFT, m_btn_io_door2_left);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_DOOR2_RIGHT, m_btn_io_door2_right);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_DOOR3_LEFT, m_btn_io_door3_left);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_DOOR3_RIGHT, m_btn_io_door3_right);
	DDX_Control(pDX, IDC_BTNENHCTRL_GLASS_MANU_VAC, m_btn_vac);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_ALIGN_FRONT1_FW, m_btn_io_front1_fw);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_ALIGN_FRONT1_BW, m_btn_io_front1_bw);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_ALIGN_FRONT2_FW, m_btn_io_front2_fw);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_ALIGN_FRONT2_BW, m_btn_io_front2_bw);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_ALIGN_REAR1_FW, m_btn_io_rear1_fw);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_ALIGN_REAR1_BW, m_btn_io_rear1_bw);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_ALIGN_REAR2_FW, m_btn_io_rear2_fw);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_ALIGN_REAR2_BW, m_btn_io_rear2_bw);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_SIDE_LEFT1_FW, m_btn_io_right1_fw);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_SIDE_LEFT1_BW, m_btn_io_right1_bw);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_SIDE_LEFT2_FW, m_btn_io_right2_fw);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_SIDE_LEFT2_BW, m_btn_io_right2_bw);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_SIDE_RIGHT1_FW, m_btn_io_left1_fw);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_SIDE_RIGHT1_BW, m_btn_io_left1_bw);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_SIDE_RIGHT2_FW, m_btn_io_left2_fw);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_SIDE_RIGHT2_BW, m_btn_io_left2_bw);
	DDX_Control(pDX, IDC_BTNENHCTR_GL_IO_FRONT_FW, m_btn_front_fw);
	DDX_Control(pDX, IDC_BTNENHCTR_GL_IO_FRONT_BW, m_btn_front_bw);
	DDX_Control(pDX, IDC_BTNENHCTR_GL_IO_LEFT_FW, m_btn_left_fw);
	DDX_Control(pDX, IDC_BTNENHCTR_GL_IO_LEFT_BW, m_btn_left_bw);
	DDX_Control(pDX, IDC_BTNENHCTR_GL_IO_RIGHT_FW, m_btn_right_fw);
	DDX_Control(pDX, IDC_BTNENHCTR_GL_IO_RIGHT_BW, m_btn_right_bw);
	DDX_Control(pDX, IDC_BTNENHCTR_GL_IO_REAR_FW, m_btn_rear_fw);
	DDX_Control(pDX, IDC_BTNENHCTR_GL_IO_REAR_BW, m_btn_rear_bw);
	DDX_Control(pDX, IDC_BTNENHCTRL_GLASS_MANU_PURGE, m_btn_purge);
	DDX_Control(pDX, IDC_BTNENHCTRL_GLASS_MANU_LOAD_ON_OFF, m_btn_loader_use);
	DDX_Control(pDX, IDC_BTNENHCTRL_GLASS_MANU_CYCLE_ON_OFF, m_btn_cycle_use);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_READ_VAC, m_btn_io_read_vac);
	DDX_Control(pDX, IDC_BTNENHCTRL_IO_READ_PURGE, m_btn_io_read_purge);
}


BEGIN_MESSAGE_MAP(CDialogGlassManuCtrl, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogGlassManuCtrl 메시지 처리기입니다.
BOOL CDialogGlassManuCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	LoadGlassManuPara(_T(GLASS_MANU_PATH));
	
	return TRUE;
}


BEGIN_EVENTSINK_MAP(CDialogGlassManuCtrl, CDialogEx)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_GL_JOG_L, DISPID_MOUSEDOWN, CDialogGlassManuCtrl::MouseDownBtnenhctrlGlJogL, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_GL_JOG_L, DISPID_MOUSEUP, CDialogGlassManuCtrl::MouseUpBtnenhctrlGlJogL, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_GL_JOG_U, DISPID_MOUSEDOWN, CDialogGlassManuCtrl::MouseDownBtnenhctrlGlJogU, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_GL_JOG_U, DISPID_MOUSEUP, CDialogGlassManuCtrl::MouseUpBtnenhctrlGlJogU, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_GL_JOG_R, DISPID_MOUSEDOWN, CDialogGlassManuCtrl::MouseDownBtnenhctrlGlJogR, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_GL_JOG_R, DISPID_MOUSEUP, CDialogGlassManuCtrl::MouseUpBtnenhctrlGlJogR, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_GL_JOG_D, DISPID_MOUSEDOWN, CDialogGlassManuCtrl::MouseDownBtnenhctrlGlJogD, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_GL_JOG_D, DISPID_MOUSEUP, CDialogGlassManuCtrl::MouseUpBtnenhctrlGlJogD, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_MORTOR_STOP, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrlMortorStop, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_GL_JOG_SPD1, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrlGlJogSpd1, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_GL_JOG_SPD2, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrlGlJogSpd2, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_GL_JOG_SPD3, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrlGlJogSpd3, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTR_GL_IO_FRONT_BW, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrGlIoFrontBw, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTR_GL_IO_FRONT_FW, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrGlIoFrontFw, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTR_GL_IO_REAR_FW, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrGlIoRearFw, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTR_GL_IO_REAR_BW, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrGlIoRearBw, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTR_GL_IO_LEFT_FW, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrGlIoLeftFw, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTR_GL_IO_LEFT_BW, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrGlIoLeftBw, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTR_GL_IO_RIGHT_FW, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrGlIoRightFw, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTR_GL_IO_RIGHT_BW, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrGlIoRightBw, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_ALIGN_CY_FW, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrlAlignCyFw, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_ALIGN_CY_BW, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrlAlignCyBw, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_DOOR_LOCK, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrlDoorLock, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_DOOR_UNLOCK, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrlDoorUnlock, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_GLASS_MANU_VAC, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrlGlassManuVac, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_GLASS_MANU_PURGE, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrlGlassManuPurge, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_START_POS, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrlStartPos, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_GLASS_MANU_LOAD_ON_OFF, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrlGlassManuLoadOnOff, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_GLASSMANU_APPLY, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrlGlassmanuApply, VTS_NONE)
	ON_EVENT(CDialogGlassManuCtrl, IDC_BTNENHCTRL_GLASS_MANU_CYCLE_ON_OFF, DISPID_CLICK, CDialogGlassManuCtrl::ClickBtnenhctrlGlassManuCycleOnOff, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogGlassManuCtrl::MouseDownBtnenhctrlGlJogL(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_0, DIR_POSITIVE, m_iJogSpdSelect);
}


void CDialogGlassManuCtrl::MouseUpBtnenhctrlGlJogL(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_0);
}


void CDialogGlassManuCtrl::MouseDownBtnenhctrlGlJogU(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_1, DIR_POSITIVE, m_iJogSpdSelect);
}


void CDialogGlassManuCtrl::MouseUpBtnenhctrlGlJogU(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_1);
}


void CDialogGlassManuCtrl::MouseDownBtnenhctrlGlJogR(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_0, DIR_NEGATIVE, m_iJogSpdSelect);
}


void CDialogGlassManuCtrl::MouseUpBtnenhctrlGlJogR(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_0);
}


void CDialogGlassManuCtrl::MouseDownBtnenhctrlGlJogD(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_1, DIR_NEGATIVE, m_iJogSpdSelect);
}


void CDialogGlassManuCtrl::MouseUpBtnenhctrlGlJogD(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_1);
}


void CDialogGlassManuCtrl::ClickBtnenhctrlMortorStop()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_0);

	pdlg->m_Robot->SetRobotJogStop(MOTOR_1);


}


void CDialogGlassManuCtrl::ClickBtnenhctrlGlJogSpd1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_iJogSpdSelect = 0;
	m_btn_gl_jog_spd1.SetValue(TRUE);
	m_btn_gl_jog_spd2.SetValue(FALSE);
	m_btn_gl_jog_spd3.SetValue(FALSE);
}


void CDialogGlassManuCtrl::ClickBtnenhctrlGlJogSpd2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_iJogSpdSelect = 1;
	m_btn_gl_jog_spd2.SetValue(TRUE);
	m_btn_gl_jog_spd1.SetValue(FALSE);
	m_btn_gl_jog_spd3.SetValue(FALSE);
}


void CDialogGlassManuCtrl::ClickBtnenhctrlGlJogSpd3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_iJogSpdSelect = 2;
	m_btn_gl_jog_spd3.SetValue(TRUE);
	m_btn_gl_jog_spd1.SetValue(FALSE);
	m_btn_gl_jog_spd2.SetValue(FALSE);
}

/*
CBtnenhctrl m_btn_front_fw;
	CBtnenhctrl m_btn_front_bw;
	CBtnenhctrl m_btn_left_fw;
	CBtnenhctrl m_btn_left_bw;
	CBtnenhctrl m_btn_right_fw;
	CBtnenhctrl m_btn_right_bw;
	CBtnenhctrl m_btn_rear_fw;
	CBtnenhctrl m_btn_rear_bw;
	*/

void CDialogGlassManuCtrl::ClickBtnenhctrGlIoFrontBw()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	BOOL bRet = FALSE;
	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_ALIGN_FRONT1_FW_SOL, FALSE);
	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_ALIGN_FRONT1_BW_SOL, TRUE);
	
}

void CDialogGlassManuCtrl::ClickBtnenhctrGlIoFrontFw()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	BOOL bRet = FALSE;

	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_ALIGN_FRONT1_FW_SOL, TRUE);
	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_ALIGN_FRONT1_BW_SOL, FALSE);

}



void CDialogGlassManuCtrl::ClickBtnenhctrGlIoRearFw()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	BOOL bRet = FALSE;
	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_ALIGN_REAR1_FW_SOL,TRUE);
	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_ALIGN_REAR1_BW_SOL,FALSE);

	
}


void CDialogGlassManuCtrl::ClickBtnenhctrGlIoRearBw()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	BOOL bRet = FALSE;
	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_ALIGN_REAR1_FW_SOL,FALSE);
	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_ALIGN_REAR1_BW_SOL,TRUE);

	
}


void CDialogGlassManuCtrl::ClickBtnenhctrGlIoLeftFw()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_SIDE_LIFT1_FW_SOL,TRUE);
	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_SIDE_LIFT1_BW_SOL,FALSE);

	
}


void CDialogGlassManuCtrl::ClickBtnenhctrGlIoLeftBw()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_SIDE_LIFT1_FW_SOL,FALSE);
	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_SIDE_LIFT1_BW_SOL,TRUE);

	
}


void CDialogGlassManuCtrl::ClickBtnenhctrGlIoRightFw()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_SIDE_RIGHT1_FW_SOL,TRUE);
	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_SIDE_RIGHT1_BW_SOL,FALSE);

	
}


void CDialogGlassManuCtrl::ClickBtnenhctrGlIoRightBw()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_SIDE_RIGHT1_FW_SOL,FALSE);
	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_SIDE_RIGHT1_BW_SOL,TRUE);

}


void CDialogGlassManuCtrl::ClickBtnenhctrlAlignCyFw()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ClickBtnenhctrGlIoFrontFw();
	ClickBtnenhctrGlIoRearFw();
	ClickBtnenhctrGlIoLeftFw();
	ClickBtnenhctrGlIoRightFw();
}


void CDialogGlassManuCtrl::ClickBtnenhctrlAlignCyBw()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ClickBtnenhctrGlIoFrontBw();
	ClickBtnenhctrGlIoRearBw();
	ClickBtnenhctrGlIoLeftBw();
	ClickBtnenhctrGlIoRightBw();
}



void CDialogGlassManuCtrl::InputState()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	if(IsWindowVisible())
	{
		m_btn_io_door1.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_DO1_OPEN_LEFT));
		m_btn_io_door2_left.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_DO2_OPEN_LEFT));
		m_btn_io_door2_right.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_DO2_OPEN_RIGHT));
		m_btn_io_door3_left.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_DO3_OPEN_LEFT));
		m_btn_io_door3_right.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_DO3_OPEN_RIGHT));

		m_btn_io_emo_left.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_EMI_LEFT));
		m_btn_io_emo_back.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_EMI_BACK));
		m_btn_io_emo_in.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_EMI_INSIDE));
		m_btn_io_emo_op.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_EMI_OP));

		m_btn_io_front1_fw.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_STAGE_ALIGN_FRONT1_FW));
		m_btn_io_front1_bw.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_STAGE_ALIGN_FRONT1_BW));
		m_btn_io_front2_fw.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_STAGE_ALIGN_FRONT2_FW));
		m_btn_io_front2_bw.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_STAGE_ALIGN_FRONT2_BW));
		m_btn_io_rear1_fw.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_STAGE_ALIGN_REAR1_FW));
		m_btn_io_rear1_bw.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_STAGE_ALIGN_REAR1_BW));
		m_btn_io_rear2_fw.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_STAGE_ALIGN_REAR2_FW));
		m_btn_io_rear2_bw.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_STAGE_ALIGN_REAR2_BW));
		m_btn_io_right1_fw.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_STAGE_SIDE_LEFT1_FW));
		m_btn_io_right1_bw.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_STAGE_SIDE_LEFT1_BW));
		m_btn_io_right2_fw.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_STAGE_SIDE_LEFT2_FW));
		m_btn_io_right2_bw.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_STAGE_SIDE_LEFT2_BW));
		m_btn_io_left1_fw.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_STAGE_SIDE_RIGHT1_FW));
		m_btn_io_left1_bw.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_STAGE_SIDE_RIGHT1_BW));
		m_btn_io_left2_fw.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_STAGE_SIDE_RIGHT2_FW));
		m_btn_io_left2_bw.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_STAGE_SIDE_RIGHT2_BW));

//		m_btn_io_x050.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_PIN_GLS_DETECT_FRONT));
//		m_btn_io_x051.SetValue(pdlg->m_IO->GetGlassCtrlReadDI(DI_PIN_GLS_DETECT_REAR));

		//20160721 ngh


	//	DO_STAGE_VACUUM_SOL,					// DO_Y61E // ROBOT_ENABLE
	//	DO_STAGE_AIR_BLOW_SOL,					// DO_Y61F *DO_STAGE_PURGE_SOL
	}
}

void CDialogGlassManuCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent) 
	{ 
	case 0:
		InputState();
		break;

	}

	CDialogEx::OnTimer(nIDEvent);
}


void CDialogGlassManuCtrl::ClickBtnenhctrlDoorLock()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_IO->SetGlassCtrlDO(DO_DOOR1_UNLOCK_LEFT, FALSE);
	pdlg->m_IO->SetGlassCtrlDO(DO_DOOR1_UNLOCK_RIGHT, FALSE);
	pdlg->m_IO->SetGlassCtrlDO(DO_DOOR2_UNLOCK_LEFT, FALSE);
	pdlg->m_IO->SetGlassCtrlDO(DO_DOOR2_UNLOCK_RIGHT, FALSE);
	pdlg->m_IO->SetGlassCtrlDO(DO_DOOR3_UNLOCK_LEFT, FALSE);
	pdlg->m_IO->SetGlassCtrlDO(DO_DOOR3_UNLOCK_RIGHT, FALSE);

	m_btn_door_lock.SetValue(1);
	m_btn_door_unlock.SetValue(0);

}


void CDialogGlassManuCtrl::ClickBtnenhctrlDoorUnlock()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_IO->SetGlassCtrlDO(DO_DOOR1_UNLOCK_LEFT, TRUE);
	pdlg->m_IO->SetGlassCtrlDO(DO_DOOR1_UNLOCK_RIGHT, TRUE);
	pdlg->m_IO->SetGlassCtrlDO(DO_DOOR2_UNLOCK_LEFT, TRUE);
	pdlg->m_IO->SetGlassCtrlDO(DO_DOOR2_UNLOCK_RIGHT, TRUE);
	pdlg->m_IO->SetGlassCtrlDO(DO_DOOR3_UNLOCK_LEFT, TRUE);
	pdlg->m_IO->SetGlassCtrlDO(DO_DOOR3_UNLOCK_RIGHT, TRUE);

	m_btn_door_lock.SetValue(0);
	m_btn_door_unlock.SetValue(1);
}

void CDialogGlassManuCtrl::ClickBtnenhctrlGlassManuVac()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_VACUUM_SOL, m_btn_vac.GetValue());
	m_btn_io_read_vac.SetValue(m_btn_vac.GetValue());
/*
	BOOL rslt = pdlg->m_IO->GetGlassCtrlReadDI(DI_CHUCK_VACUUM_ON_DETECT);
	m_btn_io_read_vac.SetValue(!rslt);
	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_VACUUM_SOL, !rslt);*/
	//if(m_btn_io_read_vac.GetValue())
	//{
	//	//Vac on
	//	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_VACUUM_SOL, FALSE);
	//}
	//else
	//{
	//	//Vac off
	//	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_VACUUM_SOL,TRUE);
	//	//Purge
	//	//pdlg->m_IO->SetWriteDO(DO_STAGE_AIR_BLOW_SOL, TRUE);
	//	//Sleep(500);
	//	//pdlg->m_IO->SetWriteDO(DO_STAGE_AIR_BLOW_SOL, FALSE);
	//}
}

void CDialogGlassManuCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow)
	{
		SetTimer(0,1000,NULL);
		VacPurgestate();

		m_btn_loader_use.SetValue(m_bLoaderUse);
	}
	else
	{
		KillTimer(0);
	}
}


void CDialogGlassManuCtrl::ClickBtnenhctrlGlassManuPurge()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//Purge
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_AIR_BLOW_SOL, m_btn_purge.GetValue());
	m_btn_io_read_purge.SetValue(m_btn_purge.GetValue());

	/*BOOL rslt = pdlg->m_IO->GetGlassCtrlReadDI(DI_CHUCK_AIR_BLOW_ON_DETECT);
	m_btn_purge.SetValue(!rslt);
	pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_AIR_BLOW_SOL, !rslt);*/

	//pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_AIR_BLOW_SOL, !m_btn_purge.GetValue());
	/*if(m_btn_purge.GetValue())
	{
		pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_AIR_BLOW_SOL, FALSE);
	}
	else
	{
		pdlg->m_IO->SetGlassCtrlDO(DO_STAGE_AIR_BLOW_SOL, TRUE);
	}*/
}


void CDialogGlassManuCtrl::ClickBtnenhctrlStartPos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CDialogGlassManuCtrl::VacPurgestate()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	BOOL rslt;
	
	rslt = pdlg->m_IO->GetGlassCtrlReadDO(DO_STAGE_VACUUM_SOL);
	m_btn_vac.SetValue(rslt);
	m_btn_io_read_vac.SetValue(rslt);

	rslt = pdlg->m_IO->GetGlassCtrlReadDO(DO_STAGE_AIR_BLOW_SOL);
	m_btn_purge.SetValue(rslt);
	m_btn_io_read_purge.SetValue(rslt);
	/*BOOL bRet = FALSE;
	bRet = pdlg->m_IO->GetGlassCtrlReadDI(DI_CHUCK_VACUUM_ON_DETECT);
	if(bRet)
	{
		m_btn_vac.SetValue(TRUE);
	}
	else
	{
		m_btn_vac.SetValue(FALSE);
	}*/
}

void CDialogGlassManuCtrl::ClickBtnenhctrlGlassManuLoadOnOff()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}

BOOL CDialogGlassManuCtrl::GetLoaderUse()
{
	return m_bLoaderUse;
}

void CDialogGlassManuCtrl::SetLoaderUse(BOOL var)
{
	m_bLoaderUse = var;

	if(IsWindowVisible())
	{
		UpdateData(FALSE);
	}
}

BOOL CDialogGlassManuCtrl::GetCycleUse()
{
	return m_bCycleUse;
}

void CDialogGlassManuCtrl::ClickBtnenhctrlGlassmanuApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_btn_loader_use.GetValue())
	{
		m_bLoaderUse = TRUE;
	}
	else
	{
		m_bLoaderUse = FALSE;
	}

	if(m_btn_cycle_use.GetValue())
	{
		m_bCycleUse = TRUE;
		m_btn_cycle_use.SetCaption(_T("CYCLE 진행"));
	}
	else
	{
		m_bCycleUse = FALSE;
		m_btn_cycle_use.SetCaption(_T("CYCLE 정지"));
	}


	
}

BOOL CDialogGlassManuCtrl::LoadGlassManuPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\GlassManuCtrl.par");

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
		key.Format(_T("m_bLoaderUse"));
		GetPrivateProfileString(_T("GLASS MANU CTRL"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_bLoaderUse = (BOOL)(atof(ctemp));

		key.Empty();
		key.Format(_T("m_bCycleUse"));
		GetPrivateProfileString(_T("GLASS MANU CTRL"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_bCycleUse = (BOOL)(atof(ctemp));
	
	}

	fd=0;
	
	return rslt;
}

BOOL CDialogGlassManuCtrl::SaveGlassManuPara(CString path)								
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\GlassManuCtrl.par");

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
		key.Format(_T("m_bLoaderUse"));
		dat.Empty();
		dat.Format(_T("%d"), m_bLoaderUse);
		WritePrivateProfileString(_T("GLASS MANU CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_bCycleUse"));
		dat.Empty();
		dat.Format(_T("%d"), m_bCycleUse);
		WritePrivateProfileString(_T("GLASS MANU CTRL"), key, dat, file_path);
	
	}
	fd=0;
	
	return rslt;
}

void CDialogGlassManuCtrl::ClickBtnenhctrlGlassManuCycleOnOff()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CDialogGlassManuCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
