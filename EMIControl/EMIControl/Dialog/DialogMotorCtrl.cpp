// Dialog/DialogMotorCtrl.cpp : 구현 파일입니다.
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
#include "Dialog/DialogGlassManuCtrl.h"
#include "Dialog/DialogFilmAlign.h"

#include "Dialog/DialogLightCtrl.h"
#include "afxdialogex.h"
#include "SerialPort.h"
#include "Dialog/DialogLogAlarm.h"

#include "EMIControlDlg.h"


// CDialogMotorCtrl 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogMotorCtrl, CDialogEx)

CDialogMotorCtrl::CDialogMotorCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogMotorCtrl::IDD, pParent)
	, m_edit_setspd1(0)
	, m_edit_setspd2(0)
	, m_edit_setspd3(0)
	, m_edit_s_pos0(0)
	, m_edit_setspd0(0)
	, m_edit_s_pos1(0)
	, m_edit_s_pos2(0)
	, m_edit_s_pos3(0)
	, m_edit_e_pos0(0)
	, m_edit_e_pos1(0)
	, m_edit_e_pos2(0)
	, m_edit_e_pos3(0)
	, m_edit_axi_name0(0)
	, m_edit_axi_name1(0)
	, m_edit_axi_name2(0)
	, m_edit_axi_name3(0)
	, m_edit_axi_id0(0)
	, m_edit_axi_id1(0)
	, m_edit_axi_id2(0)
	, m_edit_axi_id3(0)
	, m_edit_axi_p_lmt0(0)
	, m_edit_axi_p_lmt1(0)
	, m_edit_axi_p_lmt2(0)
	, m_edit_axi_p_lmt3(0)
	, m_edit_axi_m_lmt0(0)
	, m_edit_axi_m_lmt1(0)
	, m_edit_axi_m_lmt2(0)
	, m_edit_axi_m_lmt3(0)
	, m_edit_axi_spd_lmt0(0)
	, m_edit_axi_spd_lmt1(0)
	, m_edit_axi_spd_lmt2(0)
	, m_edit_axi_spd_lmt3(0)
	, m_edit_axi_res0(0)
	, m_edit_axi_res1(0)
	, m_edit_axi_res2(0)
	, m_edit_axi_res3(0)
	, m_edit_op_spd0(0)
	, m_edit_op_spd1(0)
	, m_edit_op_spd2(0)
	, m_edit_op_y_spd0(0)
	, m_edit_op_y_spd1(0)
	, m_edit_op_y_spd2(0)
	, m_edit_op_t_spd0(0)
	, m_edit_op_t_spd1(0)
	, m_edit_op_t_spd2(0)
	, m_edit_op_z_spd0(0)
	, m_edit_op_z_spd1(0)
	, m_edit_op_z_spd2(0)
	, m_edit_8k_scan_speed(0)
	, m_edit_16k_scan_speed(0)
{
	int i;

	InitializeCriticalSection(&m_csMachine);

	for(i=0; i<IO_MODULE_NUMBER; i++) {
		m_InputModule[i] = 0;
	}
	for(i=0; i<IO_MODULE_NUMBER; i++) {
		m_OutputModule[i] = 0;
	}

	m_iProcessMoveIdx = 0;

	m_bCommandLock = FALSE;

	for(i = 0; i < MAX_MOTOR; i++)
	{
		m_bOriginWait[i] = FALSE;
	}

	//20160721 ngh
	m_bMotorOriginStart = FALSE;
	m_bMotorOriginEnd = FALSE;

	//20160727 ngh
	m_bMotionJogDrawCrowsStart = FALSE;
	m_bMotionJogDrawCrowsEnd = FALSE;
}

CDialogMotorCtrl::~CDialogMotorCtrl()
{
	SaveMotorPara(_T(MOTOR_PATH));
	m_bMotionJogDrawCrowsEnd = TRUE;
	Sleep(100);
	ClickBtnenhctrlMotAllStop();
	KillTimer(0);
	KillTimer(10);
	KillTimer(11);
	KillTimer(12);
	KillTimer(13);
	KillTimer(20);
	KillTimer(21);
	KillTimer(22);
	KillTimer(23);
	DeleteCriticalSection(&m_csMachine);

}

void CDialogMotorCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_AE0, m_btn_ae0);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_POS0, m_btn_mot_pos0);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_SPD0, m_btn_mot_spd0);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_AF0, m_btn_af0);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_FE0, m_btn_fe0);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_I2T0, m_btn_i2t0);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_AE1, m_btn_ae1);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_AF1, m_btn_af1);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_FE1, m_btn_fe1);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_I2T1, m_btn_i2t1);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_AE2, m_btn_ae2);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_AF2, m_btn_af2);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_FE2, m_btn_fe2);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_I2T2, m_btn_i2t2);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_AE3, m_btn_ae3);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_AF3, m_btn_af3);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_FE3, m_btn_fe3);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_I2T3, m_btn_i2t3);
	DDX_Text(pDX, IDC_EDIT_MOT_S_POS0, m_edit_s_pos0);
	DDX_Text(pDX, IDC_EDIT_MOT_SETSPD0, m_edit_setspd0);
	DDX_Text(pDX, IDC_EDIT_MOT_SETSPD1, m_edit_setspd1);
	DDX_Text(pDX, IDC_EDIT_MOT_SETSPD2, m_edit_setspd2);
	DDX_Text(pDX, IDC_EDIT_MOT_SETSPD3, m_edit_setspd3);
	DDX_Text(pDX, IDC_EDIT_MOT_S_POS1, m_edit_s_pos1);
	DDX_Text(pDX, IDC_EDIT_MOT_S_POS2, m_edit_s_pos2);
	DDX_Text(pDX, IDC_EDIT_MOT_S_POS3, m_edit_s_pos3);
	DDX_Text(pDX, IDC_EDIT_MOT_E_POS0, m_edit_e_pos0);
	DDX_Text(pDX, IDC_EDIT_MOT_E_POS1, m_edit_e_pos1);
	DDX_Text(pDX, IDC_EDIT_MOT_E_POS2, m_edit_e_pos2);
	DDX_Text(pDX, IDC_EDIT_MOT_E_POS3, m_edit_e_pos3);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_NAME0, m_edit_axi_name0);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_NAME1, m_edit_axi_name1);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_NAME2, m_edit_axi_name2);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_NAME3, m_edit_axi_name3);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_ID0, m_edit_axi_id0);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_ID1, m_edit_axi_id1);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_ID2, m_edit_axi_id2);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_ID3, m_edit_axi_id3);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_P_LMT0, m_edit_axi_p_lmt0);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_P_LMT1, m_edit_axi_p_lmt1);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_P_LMT2, m_edit_axi_p_lmt2);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_P_LMT3, m_edit_axi_p_lmt3);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_M_LMT0, m_edit_axi_m_lmt0);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_M_LMT1, m_edit_axi_m_lmt1);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_M_LMT2, m_edit_axi_m_lmt2);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_M_LMT3, m_edit_axi_m_lmt3);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_SPD_LMT0, m_edit_axi_spd_lmt0);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_SPD_LMT1, m_edit_axi_spd_lmt1);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_SPD_LMT2, m_edit_axi_spd_lmt2);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_SPD_LMT3, m_edit_axi_spd_lmt3);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_RES0, m_edit_axi_res0);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_RES1, m_edit_axi_res1);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_RES2, m_edit_axi_res2);
	DDX_Text(pDX, IDC_EDIT_MOT_AXI_RES3, m_edit_axi_res3);
	DDX_Text(pDX, IDC_EDIT_MOT_OP_SPD0, m_edit_op_spd0);
	DDX_Text(pDX, IDC_EDIT_MOT_OP_SPD1, m_edit_op_spd1);
	DDX_Text(pDX, IDC_EDIT_MOT_OP_SPD2, m_edit_op_spd2);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_HOME0, m_btn_mot_home0);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_HOME1, m_btn_mot_home1);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_HOME2, m_btn_mot_home2);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_HOME3, m_btn_mot_home3);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_POS1, m_btn_mot_pos1);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_SPD1, m_btn_mot_spd1);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_POS2, m_btn_mot_pos2);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_SPD2, m_btn_mot_spd2);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_POS3, m_btn_mot_pos3);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_SPD3, m_btn_mot_spd3);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_PROGRESS_MOVE, m_btn_progress_move);
	DDX_Text(pDX, IDC_EDIT_MOT_OP_SPD3, m_edit_op_y_spd0);
	DDX_Text(pDX, IDC_EDIT_MOT_OP_SPD4, m_edit_op_y_spd1);
	DDX_Text(pDX, IDC_EDIT_MOT_OP_SPD5, m_edit_op_y_spd2);
	DDX_Text(pDX, IDC_EDIT_MOT_OP_SPD6, m_edit_op_t_spd0);
	DDX_Text(pDX, IDC_EDIT_MOT_OP_SPD7, m_edit_op_t_spd1);
	DDX_Text(pDX, IDC_EDIT_MOT_OP_SPD8, m_edit_op_t_spd2);
	DDX_Text(pDX, IDC_EDIT_MOT_OP_SPD9, m_edit_op_z_spd0);
	DDX_Text(pDX, IDC_EDIT_MOT_OP_SPD10, m_edit_op_z_spd1);
	DDX_Text(pDX, IDC_EDIT_MOT_OP_SPD11, m_edit_op_z_spd2);
	DDX_Text(pDX, IDC_EDIT_MOT_SCAN_SPD0, m_edit_8k_scan_speed);
	DDX_Text(pDX, IDC_EDIT_MOT_16K_SCAN_SPD0, m_edit_16k_scan_speed);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_JOG_P3, m_btnMot3_P3);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOT_JOG_M3, m_btnMot3_M3);
}


BEGIN_MESSAGE_MAP(CDialogMotorCtrl, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogMotorCtrl 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDialogMotorCtrl, CDialogEx)
	ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_HOME0, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotHome0, VTS_NONE)
//	ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_HOME1, 10, CDialogMotorCtrl::CellClickBtnenhctrlMotHome1, VTS_I2)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_HOME1, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotHome1, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_JOG_P0, DISPID_MOUSEDOWN, CDialogMotorCtrl::MouseDownBtnenhctrlMotJogP0, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_JOG_P0, DISPID_MOUSEUP, CDialogMotorCtrl::MouseUpBtnenhctrlMotJogP0, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_JOG_M0, DISPID_MOUSEDOWN, CDialogMotorCtrl::MouseDownBtnenhctrlMotJogM0, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_JOG_M0, DISPID_MOUSEUP, CDialogMotorCtrl::MouseUpBtnenhctrlMotJogM0, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_JOG_P1, DISPID_MOUSEDOWN, CDialogMotorCtrl::MouseDownBtnenhctrlMotJogP1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_JOG_P1, DISPID_MOUSEUP, CDialogMotorCtrl::MouseUpBtnenhctrlMotJogP1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_JOG_M1, DISPID_MOUSEDOWN, CDialogMotorCtrl::MouseDownBtnenhctrlMotJogM1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_JOG_M1, DISPID_MOUSEUP, CDialogMotorCtrl::MouseUpBtnenhctrlMotJogM1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_JOG_P2, DISPID_MOUSEDOWN, CDialogMotorCtrl::MouseDownBtnenhctrlMotJogP2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_JOG_P2, DISPID_MOUSEUP, CDialogMotorCtrl::MouseUpBtnenhctrlMotJogP2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_JOG_M2, DISPID_MOUSEDOWN, CDialogMotorCtrl::MouseDownBtnenhctrlMotJogM2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_JOG_M2, DISPID_MOUSEUP, CDialogMotorCtrl::MouseUpBtnenhctrlMotJogM2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_JOG_P3, DISPID_MOUSEDOWN, CDialogMotorCtrl::MouseDownBtnenhctrlMotJogP3, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_JOG_P3, DISPID_MOUSEUP, CDialogMotorCtrl::MouseUpBtnenhctrlMotJogP3, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_JOG_M3, DISPID_MOUSEDOWN, CDialogMotorCtrl::MouseDownBtnenhctrlMotJogM3, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_JOG_M3, DISPID_MOUSEUP, CDialogMotorCtrl::MouseUpBtnenhctrlMotJogM3, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_HOME2, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotHome2, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_STOP0, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotStop0, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_SAVE, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotSave, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_APPLY, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotApply, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_CANCEL, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotCancel, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_HOME3, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotHome3, VTS_NONE)
//ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_STOP1, 6, CDialogMotorCtrl::AnimDLLDoneBtnenhctrlMotStop1, VTS_I4 VTS_BOOL)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_STOP2, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotStop2, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_STOP1, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotStop1, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_STOP3, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotStop3, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_ALL_STOP, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotAllStop, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_REPEAT0, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotRepeat0, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_REPEAT1, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotRepeat1, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_REPEAT2, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotRepeat2, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_REPEAT3, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotRepeat3, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_MOVE0, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotMove0, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_MOVE1, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotMove1, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_MOVE2, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotMove2, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_MOVE3, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotMove3, VTS_NONE)
ON_EVENT(CDialogMotorCtrl, IDC_BTNENHCTRL_MOT_ALL_HOME, DISPID_CLICK, CDialogMotorCtrl::ClickBtnenhctrlMotAllHome, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogMotorCtrl::ClickBtnenhctrlMotHome0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(m_bOriginWait[MOTOR_0])
	{
		CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
		CString msg;
		msg.Format(_T("%d Axis Origin, No Move Control"), MOTOR_0);
		pdlg->WriteLog(MOTOR_LOG, msg);
		AfxMessageBox(msg);
		return;
	}

	m_bOriginWait[MOTOR_0] = TRUE;
	ServoOrigin(MOTOR_0);
	m_iProcessMoveIdx = 0;
	if(m_bOriginWait[MOTOR_0])
	{
		SetTimer(20, 1000, NULL);
	}
}

void CDialogMotorCtrl::ClickBtnenhctrlMotHome1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(m_bOriginWait[MOTOR_1])
	{
		CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
		CString msg;
		msg.Format(_T("%d Axis Origin, No Move Control"), MOTOR_0);
		pdlg->WriteLog(MOTOR_LOG, msg);
		AfxMessageBox(msg);
		return;
	}
	m_bOriginWait[MOTOR_1] = TRUE;
	ServoOrigin(MOTOR_1);
	m_iProcessMoveIdx = 0;
	if(m_bOriginWait[MOTOR_1])
	{
		SetTimer(21, 500, NULL);
	}
}

void CDialogMotorCtrl::ClickBtnenhctrlMotHome2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(m_bOriginWait[MOTOR_2])
	{
		CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
		CString msg;
		msg.Format(_T("%d Axis Origin, No Move Control"), MOTOR_0);
		pdlg->WriteLog(MOTOR_LOG, msg);
		AfxMessageBox(msg);
		return;
	}
	m_bOriginWait[MOTOR_2] = TRUE;
	ServoOrigin(MOTOR_2);	
	m_iProcessMoveIdx = 0;
	if(m_bOriginWait[MOTOR_2])
	{
		SetTimer(22, 500, NULL);
	}
}



void CDialogMotorCtrl::ClickBtnenhctrlMotHome3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	if(m_bOriginWait[MOTOR_3])
	{
		CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
		CString msg;
		msg.Format(_T("%d Axis Origin, No Move Control"), MOTOR_0);
		pdlg->WriteLog(MOTOR_LOG, msg);
		AfxMessageBox(msg);
		return;
	}
	
	BOOL bState = FALSE;
	
	IoStateCheck();
	
	m_bOriginWait[MOTOR_3] = TRUE;
	ServoOrigin(MOTOR_3);
	m_iProcessMoveIdx = 0;
	if(m_bOriginWait[MOTOR_3])
	{
		SetTimer(23, 500, NULL);
	}
	
}

BOOL CDialogMotorCtrl::IoStateCheck()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	BOOL bState = FALSE;
	CDialogMSG dlg;
	CString str;
	bState = pdlg->m_IO->GetMotorCtrlReadDI(DI_STAGE_ALIGN_FRONT1_BW);
	if(bState)
	{
	}
	else
	{
		dlg.SetCaption(_T("STAGE_ALIGN_FRONT가 Forward 상태입니다."));
		dlg.DoModal();		
		return FALSE;
	}
	bState = pdlg->m_IO->GetMotorCtrlReadDI(DI_STAGE_ALIGN_REAR1_BW);
	if(bState)
	{
	}
	else
	{
		dlg.SetCaption(_T("STAGE_ALIGN_REAR가 Forward 상태입니다."));
		dlg.DoModal();
		return FALSE;
	}
	bState = pdlg->m_IO->GetMotorCtrlReadDI(DI_STAGE_SIDE_LEFT1_BW);
	if(bState)
	{
	}
	else
	{
		dlg.SetCaption(_T("STAGE_SIDE_LEFT이 Forward 상태입니다."));
		dlg.DoModal();
		return FALSE;
		
	}
	bState = pdlg->m_IO->GetMotorCtrlReadDI(DI_STAGE_SIDE_RIGHT1_BW);
	if(bState)
	{
	}
	else
	{
		dlg.SetCaption(_T("STAGE_SIDE_RIGHT가 Forward 상태입니다."));
		dlg.DoModal();
		return FALSE;
		
	}
	bState = pdlg->m_IO->GetMotorCtrlReadDO(DO_STAGE_VACUUM_SOL);
	if(!bState)
	{
	}
	else
	{
		dlg.SetCaption(_T("VACUUM_SOL이 On 상태입니다."));
		dlg.DoModal();
		return FALSE;
		
	}
	bState = pdlg->m_IO->GetMotorCtrlReadDO(DO_STAGE_AIR_BLOW_SOL);
	if(!bState)
	{	
	}
	else
	{
		dlg.SetCaption(_T("AIR_BLOW_SOL이 On 상태입니다."));
		dlg.DoModal();
		return FALSE;
		
	}
	return TRUE;
}

/*
DI_STAGE_ALIGN_FRONT1_FW,				// DI_X540		
	DI_STAGE_ALIGN_FRONT1_BW,				// DI_X541		
	DI_STAGE_ALIGN_FRONT2_FW,				// DI_X542		
	DI_STAGE_ALIGN_FRONT2_BW,				// DI_X543		
	DI_STAGE_ALIGN_REAR1_FW,				// DI_X544		
	DI_STAGE_ALIGN_REAR1_BW,				// DI_X545		
	DI_STAGE_ALIGN_REAR2_FW,				// DI_X546		
	DI_STAGE_ALIGN_REAR2_BW,				// DI_X547		 
	DI_STAGE_SIDE_LEFT1_FW,					// DI_X548		
	DI_STAGE_SIDE_LEFT1_BW,					// DI_X549		   
	DI_STAGE_SIDE_LEFT2_FW,					// DI_X54A		
	DI_STAGE_SIDE_LEFT2_BW,					// DI_X54B			
	DI_STAGE_SIDE_RIGHT1_FW,				// DI_X54C		
	DI_STAGE_SIDE_RIGHT1_BW,				// DI_X54D		
	DI_STAGE_SIDE_RIGHT2_FW,				// DI_X54E		
	DI_STAGE_SIDE_RIGHT2_BW,				// DI_X54F

	*/
BOOL CDialogMotorCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
#ifdef MACHINE
	if(!m_PMAC.Init())
	{
		AfxMessageBox(_T("Error : UMAC initialize fail!"));
		return FALSE;
	}

	ReadServoState();

	m_dmLastPostion[MOTOR_0]		= GetCurPos(MOTOR_0);
	m_dmLastPostion[MOTOR_1]		= GetCurPos(MOTOR_1);
	m_dmLastPostion[MOTOR_2]		= GetCurPos(MOTOR_2);
	m_dmLastPostion[MOTOR_3]		= GetCurPos(MOTOR_3);
#endif

	LoadMotorPara(_T(MOTOR_PATH));

	UpdateData(FALSE);

	m_btn_progress_move.SetProgressBarMin(0);
	m_btn_progress_move.SetProgressBarMax(100);
	m_btn_progress_move.SetProgressBarValue(m_iProcessMoveIdx);
	m_btn_progress_move.SetCaption(_T(" "));

	SetTimer(0, 200, NULL);

	m_pThreadMotionJogDrawCrows = AfxBeginThread(MotionJogDrawCrowsThread, this, THREAD_PRIORITY_NORMAL);
	if (m_pThreadMotionJogDrawCrows) m_pThreadMotionJogDrawCrows->m_bAutoDelete = TRUE;

	m_bMotorOriginStart = TRUE;
	m_bMotorOriginEnd = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

UINT CDialogMotorCtrl::MotionOriginThread(LPVOID pParam)
{
	CDialogMotorCtrl *pdlg = (CDialogMotorCtrl *)pParam;

	pdlg->MotionOriginThread();

	return 0;
}

void CDialogMotorCtrl::MotionOriginThread()
{
	CString msg;
	while(TRUE)
	{
		if(m_bMotorOriginStart)
		{
			m_bMotorOriginStart = FALSE;
			
			for(int axi = 0; axi < 4; axi++)
			{
				m_bOriginWait[axi] = TRUE;
				ServoOrigin(axi);
			}

			for(int axi = 0; axi < 4; axi++)
			{
				while(m_bOriginWait[axi])
				{
					if(IsServoOriginComplete(axi))
					{
						msg.Format(_T("%d Axis Origin End"), axi);
						WriteLog(msg);
						m_bOriginWait[axi] = FALSE;
						break;
					}
				}
			}

			m_bMotorOriginEnd = TRUE;
		}

		if(m_bMotorOriginEnd)
		{
			break;
		}
		Sleep(1);
	}
}


UINT CDialogMotorCtrl::MotionJogDrawCrowsThread(LPVOID pParam)
{
	CDialogMotorCtrl *pdlg = (CDialogMotorCtrl *)pParam;

	pdlg->MotionJogDrawCrowsThread();

	return 0;
}

void CDialogMotorCtrl::MotionJogDrawCrowsThread()
{
	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString msg;
	double robot_y, robot_x;
	double temp_x, temp_y;
	while(TRUE)
	{
		if(m_bMotionJogDrawCrowsStart)
		{
			pdlg->m_Glass->SetDrawCrossReviewX(pdlg->m_Glass->m_dDestRobot_y, pdlg->m_Glass->m_dDestRobot_x);
			pdlg->m_Glass->SetDrawCrossReviewY(pdlg->m_Glass->m_dDestRobot_y, pdlg->m_Glass->m_dDestRobot_x);

			robot_y = GetRobotPos_mm(MOTOR_0);
			robot_x = GetRobotPos_mm(MOTOR_1);
			
			pdlg->m_Glass->SetDrawCrossReviewX(robot_y, robot_x);
			pdlg->m_Glass->SetDrawCrossReviewY(robot_y, robot_x);

			Sleep(50);
		}

		if(m_bMotionJogDrawCrowsEnd)
		{
			break;
		}
		Sleep(1);
	}
}

BOOL CDialogMotorCtrl::WriteLog(CString strLog)
{
	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	return pdlg->WriteLog(MOTOR_LOG, strLog);
}

BOOL CDialogMotorCtrl::ReadServoState() // 1 2 3 5 6
{
	if(m_bCommandLock) return TRUE;
	
	CMD_SET_LIST lstCMDSet;
	lstCMDSet.clear();

	int nIndex = 0;
	lstCMDSet[nIndex++].strAddress = "#1P";		// Current Position
	lstCMDSet[nIndex++].strAddress = "#2P";
	lstCMDSet[nIndex++].strAddress = "#5P";
	lstCMDSet[nIndex++].strAddress = "#6P";
	
 	lstCMDSet[nIndex++].strAddress = "M145";	// Origin Complete
 	lstCMDSet[nIndex++].strAddress = "M245";
	lstCMDSet[nIndex++].strAddress = "M545";     
	lstCMDSet[nIndex++].strAddress = "M645";     
	

	lstCMDSet[nIndex++].strAddress = "I169";	// motor 부하 limit
	lstCMDSet[nIndex++].strAddress = "I269";
	lstCMDSet[nIndex++].strAddress = "I569";
	lstCMDSet[nIndex++].strAddress = "I669";

 	lstCMDSet[nIndex++].strAddress = "M140";	// In Position
	lstCMDSet[nIndex++].strAddress = "M240";
 	lstCMDSet[nIndex++].strAddress = "M540";
 	lstCMDSet[nIndex++].strAddress = "M640";

	/*
	lstCMDSet[nIndex++].strAddress = "M133";	// Velocity
	lstCMDSet[nIndex++].strAddress = "M233";
	lstCMDSet[nIndex++].strAddress = "M533";
 	lstCMDSet[nIndex++].strAddress = "M633";
	*/

	lstCMDSet[nIndex++].strAddress = "M121";	// Positive Limit
	lstCMDSet[nIndex++].strAddress = "M221";
	lstCMDSet[nIndex++].strAddress = "M521";
	lstCMDSet[nIndex++].strAddress = "M621";

	lstCMDSet[nIndex++].strAddress = "M122";	// Negative Limit
	lstCMDSet[nIndex++].strAddress = "M222";
	lstCMDSet[nIndex++].strAddress = "M522";
	lstCMDSet[nIndex++].strAddress = "M622";

	lstCMDSet[nIndex++].strAddress = "M168";	// motor 부하 
	lstCMDSet[nIndex++].strAddress = "M268";
	lstCMDSet[nIndex++].strAddress = "M568";
	lstCMDSet[nIndex++].strAddress = "M668";

	lstCMDSet[nIndex++].strAddress = "I122";	// 현재설정된 속도 읽어오기
	lstCMDSet[nIndex++].strAddress = "I222";
	lstCMDSet[nIndex++].strAddress = "I522";
	lstCMDSet[nIndex++].strAddress = "I622";

	
	lstCMDSet[nIndex++].strAddress = "M139";	//AE
	lstCMDSet[nIndex++].strAddress = "M239";
	lstCMDSet[nIndex++].strAddress = "M539";
	lstCMDSet[nIndex++].strAddress = "M639";
	
	lstCMDSet[nIndex++].strAddress = "M143";	//AF
	lstCMDSet[nIndex++].strAddress = "M243";
	lstCMDSet[nIndex++].strAddress = "M543";
	lstCMDSet[nIndex++].strAddress = "M643";
	
	lstCMDSet[nIndex++].strAddress = "M142";	//FE
	lstCMDSet[nIndex++].strAddress = "M242";
	lstCMDSet[nIndex++].strAddress = "M542";
	lstCMDSet[nIndex++].strAddress = "M642";
	
	lstCMDSet[nIndex++].strAddress = "M147";	//I2T
	lstCMDSet[nIndex++].strAddress = "M247";
	lstCMDSet[nIndex++].strAddress = "M547";
	lstCMDSet[nIndex++].strAddress = "M647";
	


 	m_bCommandLock = TRUE;
	m_PMAC.GetPmacDataInt(&lstCMDSet);
	m_bCommandLock = FALSE;

	nIndex = 0;
	m_dmMotorStatus[_T("#1P")]		= lstCMDSet[nIndex++].nValue;	// Current Position
	m_dmMotorStatus[_T("#2P")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("#5P")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("#6P")]		= lstCMDSet[nIndex++].nValue;
	
	m_dmMotorStatus[_T("M145")]		= lstCMDSet[nIndex++].nValue;	// Origin Complete(home을 찾게 되면 값이 1)
	m_dmMotorStatus[_T("M245")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M545")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M645")]		= lstCMDSet[nIndex++].nValue;
	

	m_dmMotorStatus[_T("I169")]		= lstCMDSet[nIndex++].nValue;	// motor 부하 limit
	m_dmMotorStatus[_T("I269")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("I569")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("I669")]		= lstCMDSet[nIndex++].nValue;

	m_dmMotorStatus[_T("M140")]		= lstCMDSet[nIndex++].nValue;	// In Position
	m_dmMotorStatus[_T("M240")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M540")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M640")]		= lstCMDSet[nIndex++].nValue;

	/*
	m_dmMotorStatus[_T("M133")]		= lstCMDSet[nIndex++].nValue;	// Velocity
	m_dmMotorStatus[_T("M233")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M533")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M633")]		= lstCMDSet[nIndex++].nValue;
	*/

	m_dmMotorStatus[_T("M121")]		= lstCMDSet[nIndex++].nValue;	// Positive Limit
	m_dmMotorStatus[_T("M221")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M521")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M621")]		= lstCMDSet[nIndex++].nValue;

	m_dmMotorStatus[_T("M122")]		= lstCMDSet[nIndex++].nValue;	// Negative Limit
	m_dmMotorStatus[_T("M222")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M522")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M622")]		= lstCMDSet[nIndex++].nValue;

	m_dmMotorStatus[_T("M168")]		= lstCMDSet[nIndex++].nValue;	// motor 부하 
	m_dmMotorStatus[_T("M268")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M568")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M668")]		= lstCMDSet[nIndex++].nValue;

	m_dmMotorStatus[_T("I122")]		= lstCMDSet[nIndex++].nValue;	// 현재 설정된 속도 읽어오기
	m_dmMotorStatus[_T("I222")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("I522")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("I622")]		= lstCMDSet[nIndex++].nValue;
	
	
	m_dmMotorStatus[_T("M139")]		= lstCMDSet[nIndex++].nValue;	
	m_dmMotorStatus[_T("M239")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M539")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M639")]		= lstCMDSet[nIndex++].nValue;
	
	m_dmMotorStatus[_T("M143")]		= lstCMDSet[nIndex++].nValue;	 
	m_dmMotorStatus[_T("M243")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M543")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M643")]		= lstCMDSet[nIndex++].nValue;
	
	m_dmMotorStatus[_T("M142")]		= lstCMDSet[nIndex++].nValue;	 
	m_dmMotorStatus[_T("M242")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M542")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M642")]		= lstCMDSet[nIndex++].nValue;
	
	m_dmMotorStatus[_T("M147")]		= lstCMDSet[nIndex++].nValue;	 
	m_dmMotorStatus[_T("M247")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M547")]		= lstCMDSet[nIndex++].nValue;
	m_dmMotorStatus[_T("M647")]		= lstCMDSet[nIndex++].nValue;
	
	return TRUE;
}

double CDialogMotorCtrl::GetCurPos(int nAxis)
{
	double position;
	CString strCommand, strReponse;
	int axi_idx = 0;

	switch(nAxis)
	{
	case 0:
		axi_idx = m_edit_axi_id0;
		break;
	case 1:
		axi_idx = m_edit_axi_id1;
		break;
	case 2:
		axi_idx = m_edit_axi_id2;
		break;
	case 3:
		axi_idx = m_edit_axi_id3;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::GetCurPos)!"));
		break;
	}

	strCommand.Format(_T("#%dP"), axi_idx);
#ifdef MACHINE
	position = (double)m_dmMotorStatus[strCommand];
#else
	position = 0.;
#endif
	//position = ConvertPosition(nAxis,position,FALSE);임시
	return position;
}

double CDialogMotorCtrl::GetCurSpeed(int nAxis)
{
	CString strAddress;
	int axi_idx = 0;

	switch(nAxis)
	{
	case 0:
		axi_idx = m_edit_axi_id0;
		break;
	case 1:
		axi_idx = m_edit_axi_id1;
		break;
	case 2:
		axi_idx = m_edit_axi_id2;
		break;
	case 3:
		axi_idx = m_edit_axi_id3;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::GetCurSpeed)!"));
		break;
	}

	strAddress.Format(_T("I%d22"), axi_idx);
#ifdef MACHINE
	double dbSpeed = (double)m_dmMotorStatus[strAddress];
#else
	double dbSpeed = 600.;
#endif
	//dbSpeed = ConvertSpeed(nAxis,dbSpeed,FALSE);임시
	return dbSpeed;
}


double CDialogMotorCtrl::ConvertPosition(int nAxis, double dbPosition, BOOL bCount)
{
	//임시
	/*
	double dbRes = 0.f;
	if (bCount) { // mm -> Count
		switch (nAxis) {
		case MOTOR_0				:	if (RES_MOTOR_1!=0.f) 		dbPosition *= (1.f/RES_MOTOR_1)* 1000.0;	break;			
 		case MOTOR_1				:	if (RES_MOTOR_2!=0.f) 		dbPosition *= (1.f/RES_MOTOR_2)* 1000.0;	break;
		case MOTOR_2				:	if (RES_MOTOR_3!=0.f)  		dbPosition *= (1.f/RES_MOTOR_3)* 1000.0;	break;
		case MOTOR_3				:	if (RES_MOTOR_4!=0.f)  		dbPosition /= RES_MOTOR_4;					break;
		}
	}
	else { // Count -> mm
		switch (nAxis) {
		case MOTOR_0				:   dbPosition *= RES_MOTOR_1/1000.0;	break;         
 		case MOTOR_1	        	:	dbPosition *= RES_MOTOR_2/1000.0;	break;
		case MOTOR_2	        	:	dbPosition *= RES_MOTOR_3/1000.0;	break;
		case MOTOR_3	        	:	dbPosition *= RES_MOTOR_4;			break;
		}
	}
	*/
	
	return dbPosition;
}

double CDialogMotorCtrl::ConvertSpeed(int nAxis, double dbSpeed, BOOL bCount)
{
	//임시
	/*
	double dbResolution = 1;

	if(pMotorStage!=NULL) 
	{
		dbResolution = pMotorStage->dbResolution;
	}
	*/

	/*
	if (bCount) 
	{ // mm -> Count
		if (dbResolution!=0.f)
		{
			dbSpeed*= (1.f/dbResolution);
		}
	} 
	else 
	{ // Count -> mm
		dbSpeed *= dbResolution;
	}
	*/

	return dbSpeed;	
}

void CDialogMotorCtrl::SetMotorStop(int nAxis)
{
	ServoStop(nAxis);
}

void CDialogMotorCtrl::ServoStop(int nAxis)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strCommand;
	int axi_idx = 0;

	if(m_bOriginWait[nAxis])
	{
		
		m_bOriginWait[nAxis] = FALSE;
		KillTimer(20 + nAxis);
		CString msg;
		msg.Format(_T("%d Axis Origin Stop"), nAxis);
		pdlg->WriteLog(MOTOR_LOG, msg);
	}

	switch(nAxis)
	{
	case 0:
		axi_idx = m_edit_axi_id0;
		break;
	case 1:
		axi_idx = m_edit_axi_id1;
		break;
	case 2:
		axi_idx = m_edit_axi_id2;
		break;
	case 3:
		axi_idx = m_edit_axi_id3;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::ServoStop)!"));
		break;
	}

	strCommand.Format(_T("#%dJ/"), axi_idx);

	for (UINT nElaspedTime=0; nElaspedTime<1000; nElaspedTime+=10) {
		if(!m_bCommandLock) break;
		::Sleep(10);
	}

	m_bCommandLock = TRUE;
	m_PMAC.SetPmac(strCommand);
	m_bCommandLock = FALSE;

	m_bMotionJogDrawCrowsStart = FALSE;
}


void CDialogMotorCtrl::ServoOrigin(int nAxis)
{
	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	int axi_idx = 0;
	CString msg;
	msg.Format(_T("%d Axis Origin Start"), nAxis);
	pdlg->WriteLog(MOTOR_LOG, msg);

	switch(nAxis)
	{
	case 0:
		axi_idx = m_edit_axi_id0;
		break;
	case 1:
		axi_idx = m_edit_axi_id1;
		break;
	case 2:
		axi_idx = m_edit_axi_id2;
		break;
	case 3:
		axi_idx = m_edit_axi_id3;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::ServoOrigin)!"));
		break;
	}

#ifdef MACHINE
	CString strCommand;
	UINT nElaspedTime;
	for (nElaspedTime=0; nElaspedTime<1000; nElaspedTime+=10) {
		if(!m_bCommandLock) break;
		::Sleep(10);
	}
	strCommand.Format(_T("DIS PLC 1%d"),axi_idx);
	m_PMAC.SetPmac(strCommand);
	Sleep(100);

	for (nElaspedTime=0; nElaspedTime<1000; nElaspedTime+=10) {
		if(!m_bCommandLock) break;
		::Sleep(10);
	}
	strCommand.Format(_T("ENA PLC 1%d"),axi_idx);
	m_PMAC.SetPmac(strCommand);
	strCommand.Format(_T("P100%d=1"),axi_idx);
	m_PMAC.SetPmac(strCommand);
#else

#endif
}


void CDialogMotorCtrl::ServoJogMove(int nAxis, int nDirection, double dbSpeed)
{
	double pposition = 0.;
	double mposition = 0.;
	switch(nAxis)
	{
	case 0:
		pposition = GetCtsToMM(nAxis, MOTOR_0_P_LMT_POS);
		mposition = GetCtsToMM(nAxis, MOTOR_0_M_LMT_POS);
		break;
	case 1:
		pposition = GetCtsToMM(nAxis, MOTOR_1_P_LMT_POS);
		mposition = GetCtsToMM(nAxis, MOTOR_1_M_LMT_POS);
		break;
	case 2:
		pposition = GetCtsToMM(nAxis, MOTOR_2_P_LMT_POS);
		mposition = GetCtsToMM(nAxis, MOTOR_2_M_LMT_POS);
		break;
	case 3:
		pposition = GetCtsToMM(nAxis, MOTOR_3_P_LMT_POS);
		mposition = GetCtsToMM(nAxis, MOTOR_3_M_LMT_POS);
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::ServoMove)!"));
		break;
	}

	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	pdlg->m_Glass->SetDrawCrossRobot(50, -50);
	//pdlg->m_Glass->SetDrawCrossRobot(0,0);

	if(nDirection==DIR_POSITIVE)
	{
		ServoMove(nAxis, pposition, dbSpeed);
	}
	else if(nDirection==DIR_NEGATIVE)
	{
		ServoMove(nAxis, mposition, dbSpeed);
	}
	else
		ServoStop(nAxis);
}

int CDialogMotorCtrl::GetAllOriginFalseIdx()
{
	int rslt = -1;
	if(m_bOriginWait[MOTOR_0])
	{
		rslt = MOTOR_0;
	}
	if(m_bOriginWait[MOTOR_1])
	{
		rslt = MOTOR_1;
	}
	if(m_bOriginWait[MOTOR_2])
	{
		rslt = MOTOR_2;
	}
	if(m_bOriginWait[MOTOR_3])
	{
		rslt = MOTOR_3;
	}
	return rslt;
}

BOOL CDialogMotorCtrl::ServoMove(int nAxis, double dbPosition_mm, double dbSpeed_mm)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	CString strlog;
	double dbPosition;
	double dbSpeed;
	int axi_idx = 0;

	dbPosition = GetMMToCts(nAxis, dbPosition_mm);
	dbSpeed = GetMMToCts(nAxis, dbSpeed_mm);


	if(GetAllOriginFalseIdx() != -1)
	{
		CString msg;
		msg.Format(_T("%d Axis Origin, No Move Control"), GetAllOriginFalseIdx());
		pdlg->WriteLog(MOTOR_LOG, msg);
		AfxMessageBox(msg);
		return TRUE;
	}

	//20160717 ngh
	if(nAxis == 0 || nAxis == 1 || nAxis == 2)
	{
		if(GetRobotPos_mm(3) > 1)
		{
			return FALSE;
		}
	}

	////20160727 ngh
	//if(nAxis == 3)
	//{
	//	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	//	if(!pdlg->m_IO->GetGlassEdgeStatus())
	//	{
	//		return FALSE;
	//	}
	//}

	switch(nAxis)
	{
	case 0:
		axi_idx = m_edit_axi_id0;
		break;
	case 1:
		axi_idx = m_edit_axi_id1;
		break;
	case 2:
		axi_idx = m_edit_axi_id2;
		break;
	case 3:
		axi_idx = m_edit_axi_id3;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::ServoMove)!"));
		break;
	}
	
	//Position limit
	switch(nAxis)
	{
	case 0:
		if(MOTOR_0_P_LMT_POS < dbPosition)
		{
			dbPosition = MOTOR_0_P_LMT_POS;
			//LOG
			dbPosition_mm = dbPosition * MOTOR_0_CTS_PER_1MM;
			strlog.Format(_T("ServoMove(Axis : %d,Position (mm) : %.0f ,Position (cts) : %.0f , Speed : %.0f)"),nAxis,dbPosition_mm,dbPosition,dbSpeed);
			pdlg->WriteLog(MOTOR_LOG,strlog);
		}
		if(MOTOR_0_M_LMT_POS > dbPosition)
		{
			dbPosition = MOTOR_0_M_LMT_POS;
			//LOG
			dbPosition_mm = dbPosition * MOTOR_0_CTS_PER_1MM;
			strlog.Format(_T("ServoMove(Axis : %d,Position (mm) : %.0f ,Position (cts) : %.0f , Speed : %.0f)"),nAxis,dbPosition_mm,dbPosition,dbSpeed);
			pdlg->WriteLog(MOTOR_LOG,strlog);
		}
		break;
	case 1:
		if(MOTOR_1_P_LMT_POS < dbPosition)
		{
			dbPosition = MOTOR_1_P_LMT_POS;
			//LOG
			dbPosition_mm = dbPosition * MOTOR_1_CTS_PER_1MM;
			strlog.Format(_T("ServoMove(Axis : %d,Position (mm) : %.0f ,Position (cts) : %.0f , Speed : %.0f)"),nAxis,dbPosition_mm,dbPosition,dbSpeed);
			pdlg->WriteLog(MOTOR_LOG,strlog);
		}
		if(MOTOR_1_M_LMT_POS > dbPosition)
		{
			dbPosition = MOTOR_1_M_LMT_POS;
			//LOG
			dbPosition_mm = dbPosition * MOTOR_1_CTS_PER_1MM;
			strlog.Format(_T("ServoMove(Axis : %d,Position (mm) : %.0f ,Position (cts) : %.0f , Speed : %.0f)"),nAxis,dbPosition_mm,dbPosition,dbSpeed);
			pdlg->WriteLog(MOTOR_LOG,strlog);

		}
		break;
	case 2:
		if(MOTOR_2_P_LMT_POS < dbPosition)
		{
			dbPosition = MOTOR_2_P_LMT_POS;
			//LOG
			dbPosition_mm = dbPosition * MOTOR_2_CTS_PER_1DEG;
			strlog.Format(_T("ServoMove(Axis : %d,Position (deg) : %.0f ,Position (cts) : %.0f , Speed : %.0f)"),nAxis,dbPosition_mm,dbPosition,dbSpeed);
			pdlg->WriteLog(MOTOR_LOG,strlog);
		}
		if(MOTOR_2_M_LMT_POS > dbPosition)
		{
			dbPosition = MOTOR_2_M_LMT_POS;
			//LOG
			dbPosition_mm = dbPosition * MOTOR_2_CTS_PER_1DEG;
			strlog.Format(_T("ServoMove(Axis : %d,Position (deg) : %.0f ,Position (cts) : %.0f , Speed : %.0f)"),nAxis,dbPosition_mm,dbPosition,dbSpeed);
			pdlg->WriteLog(MOTOR_LOG,strlog);
		}
		break;
	case 3:
		if(MOTOR_3_P_LMT_POS < dbPosition)
		{
			dbPosition = MOTOR_3_M_LMT_POS;
			//LOG
			dbPosition_mm = dbPosition * MOTOR_3_CTS_PER_1MM;
			strlog.Format(_T("ServoMove(Axis : %d,Position (mm) : %.0f ,Position (cts) : %.0f , Speed : %.0f)"),nAxis,dbPosition_mm,dbPosition,dbSpeed);
			pdlg->WriteLog(MOTOR_LOG,strlog);
		}
		if(MOTOR_3_M_LMT_POS > dbPosition)
		{
			dbPosition = MOTOR_3_M_LMT_POS;
			//LOG
			dbPosition_mm = dbPosition * MOTOR_3_CTS_PER_1MM;
			strlog.Format(_T("ServoMove(Axis : %d,Position (mm) : %.0f ,Position (cts) : %.0f , Speed : %.0f)"),nAxis,dbPosition_mm,dbPosition,dbSpeed);
			pdlg->WriteLog(MOTOR_LOG,strlog);

		}
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::ServoMove)!"));
		break;
	}

	//Speed limit
	switch(nAxis)
	{
	case 0:
		if(MOTOR_0_LMT_SPD < dbSpeed)
		{
			dbSpeed = MOTOR_0_LMT_SPD;
			//LOG

		}
		break;
	case 1:
		if(MOTOR_1_LMT_SPD < dbSpeed)
		{
			dbSpeed = MOTOR_1_LMT_SPD;
			//LOG

		}
		break;
	case 2:
		if(MOTOR_2_LMT_SPD < dbSpeed)
		{
			dbSpeed = MOTOR_2_LMT_SPD;
			//LOG

		}
		break;
	case 3:
		if(MOTOR_3_LMT_SPD < dbSpeed)
		{
			dbSpeed = MOTOR_3_LMT_SPD;
			//LOG

		}
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::ServoMove)!"));
		break;
	}

	/*

#define MOTOR_0_M_LMT_POS				((-32091) + MOTOR_0_CTS_PER_1MM)
#define MOTOR_0_P_LMT_POS				(5954767 - MOTOR_0_CTS_PER_1MM)

#define MOTOR_1_M_LMT_POS				((-32112) + MOTOR_1_CTS_PER_1MM)
#define MOTOR_1_P_LMT_POS				(4426600 - MOTOR_1_CTS_PER_1MM)

#define MOTOR_2_M_LMT_POS				((-45800) + MOTOR_2_CTS_PER_1DEG)
#define MOTOR_2_P_LMT_POS				(1408601 - MOTOR_2_CTS_PER_1DEG)

#define MOTOR_3_M_LMT_POS				((-2354) + MOTOR_3_CTS_PER_1MM)
#define MOTOR_3_P_LMT_POS				(10993 - MOTOR_3_CTS_PER_1MM)
	*/
	/*임시
	if(IsServoAmpFault(nAxis)) 
	{
		logmsg(LT_ERROR, "%d Motor Amp Fault Occured!!! [ServoMove]", nAxis);
	}
	
	if(IsServoFollowingError(nAxis)) 
	{
		logmsg(LT_ERROR, "%d Motor Following Error Occured!!! [ServoMove]", nAxis);
	}
	
	if(IsServoI2TFault(nAxis)) 
	{
		logmsg(LT_ERROR, "%d Motor I2T Fault Occured!!! [ServoMove]", nAxis);
	}
	*/

	/*
	SYS_MOTOR_STAGE *pMotorStage = _PARAMSYS->GetMotorStage(nAxis);
	if(pMotorStage == NULL) return FALSE;

	dbPosition	= max(dbPosition, pMotorStage->dbNegPostionLimit);
	dbPosition	= min(dbPosition, pMotorStage->dbPosPostionLimit);
	*/

	double dbCurPos = GetCurPos(nAxis);
	double dbSubMeter = fabs(dbCurPos-m_dmLastPostion[nAxis]);

	/*
	CString strText;
	strText.Format("MOVEMETER %02d", nAxis);
	LONG nMoveMetor = AfxGetApp()->GetProfileInt(strText, "MOVEMETER",	0);
	nMoveMetor += dbSubMeter;
	AfxGetApp()->WriteProfileInt(strText, "MOVEMETER",	nMoveMetor);
	m_dmLastPostion[nAxis] = dbCurPos;
	*/

	/*
 	dbSpeed		= max(dbSpeed, 0);
	dbSpeed		= min(dbSpeed, pMotorStage->dbSpeedLimit);

	dbPosition	= ConvertPosition(nAxis,dbPosition);
	dbSpeed		= ConvertSpeed(nAxis,dbSpeed);
	*/

	CString strCommand;

	strCommand.Format(_T("I%d20=%d I%d21=%d"),	axi_idx, 100, axi_idx, 100);

	m_bCommandLock = TRUE;
	m_PMAC.SetPmac(strCommand);
	m_bCommandLock = FALSE;


	strCommand.Format(_T("I%d22=%d #%dJ=%.2f"), axi_idx, (int)dbSpeed,
											axi_idx, dbPosition);

	for (UINT nElaspedTime=0; nElaspedTime<1000; nElaspedTime+=10) {
		if(!m_bCommandLock) break;
		::Sleep(10);
	}

	m_bCommandLock = TRUE;
	m_PMAC.SetPmac(strCommand);
	m_bCommandLock = FALSE;
	strlog.Format(_T("ServoMove(Axis : %d,Position (mm) : %.0f ,Position (cts) : %.0f , Speed : %.0f)"),nAxis, dbPosition_mm,dbPosition,dbSpeed);
	pdlg->WriteLog(MOTOR_LOG,strlog);
	return TRUE;
}

void CDialogMotorCtrl::MouseDownBtnenhctrlMotJogP0(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	ServoJogMove(MOTOR_0, DIR_POSITIVE, m_edit_setspd0);
}


void CDialogMotorCtrl::MouseUpBtnenhctrlMotJogP0(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ServoStop(MOTOR_0);
}


void CDialogMotorCtrl::MouseDownBtnenhctrlMotJogM0(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	ServoJogMove(MOTOR_0, DIR_NEGATIVE, m_edit_setspd0);
}


void CDialogMotorCtrl::MouseUpBtnenhctrlMotJogM0(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ServoStop(MOTOR_0);
}


void CDialogMotorCtrl::MouseDownBtnenhctrlMotJogP1(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ServoJogMove(MOTOR_1, DIR_POSITIVE, m_edit_setspd1);
}


void CDialogMotorCtrl::MouseUpBtnenhctrlMotJogP1(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ServoStop(MOTOR_1);
}


void CDialogMotorCtrl::MouseDownBtnenhctrlMotJogM1(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ServoJogMove(MOTOR_1, DIR_NEGATIVE, m_edit_setspd1);
}


void CDialogMotorCtrl::MouseUpBtnenhctrlMotJogM1(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ServoStop(MOTOR_1);
}


void CDialogMotorCtrl::MouseDownBtnenhctrlMotJogP2(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ServoJogMove(MOTOR_2,DIR_POSITIVE,m_edit_setspd2);
}


void CDialogMotorCtrl::MouseUpBtnenhctrlMotJogP2(short Button, short Shift, long x, long y)
{
	ServoStop(MOTOR_2);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDialogMotorCtrl::MouseDownBtnenhctrlMotJogM2(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ServoJogMove(MOTOR_2,DIR_NEGATIVE,m_edit_setspd2);
}


void CDialogMotorCtrl::MouseUpBtnenhctrlMotJogM2(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ServoStop(MOTOR_2);
}


void CDialogMotorCtrl::MouseDownBtnenhctrlMotJogP3(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CDialogMSG dlg;
	CString str;
	CRect rect;
	GetDlgItem(IDC_BTNENHCTRL_MOT_JOG_P3)->GetClientRect(&rect);
	if((GetRobotPos_mm(MOTOR_1) == 0) && ((0 < x) && (x < rect.right)) && ((0 < y) && (y < rect.bottom)))
	{
		ServoJogMove(MOTOR_3,DIR_POSITIVE,m_edit_setspd3);
	}
	else if((GetRobotPos_mm(MOTOR_1) != 0) && ((0 < x) && (x < rect.right)) && ((0 < y) && (y < rect.bottom)))
	{
		str.Format(_T("Shift Axie Waite Error\n대기위치(홈위치)로 이동 바랍니다."));
		dlg.SetCaption(str);
		dlg.DoModal();
	}
}


void CDialogMotorCtrl::MouseUpBtnenhctrlMotJogP3(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ServoStop(MOTOR_3);
}


void CDialogMotorCtrl::MouseDownBtnenhctrlMotJogM3(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CDialogMSG dlg;
	CString str;
	CRect rect;
	short a;
	GetDlgItem(IDC_BTNENHCTRL_MOT_JOG_M3)->GetClientRect(&rect);
	if((GetRobotPos_mm(MOTOR_1) == 0) && ((0 < x) && (x < rect.right)) && ((0 < y) && (y < rect.bottom)))
	{
		ServoJogMove(MOTOR_3,DIR_NEGATIVE,m_edit_setspd3);
	}
	else if((GetRobotPos_mm(MOTOR_1) != 0) && ((0 < x) && (x < rect.right)) && ((0 < y) && (y < rect.bottom)))
	{
		str.Format(_T("Shift Axie Waite Error\n대기위치(홈위치)로 이동 바랍니다."));
		dlg.SetCaption(str);
		dlg.DoModal();
	}
}


void CDialogMotorCtrl::MouseUpBtnenhctrlMotJogM3(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다
	ServoStop(MOTOR_3);
	m_iProcessMoveIdx = 0;
}

void CDialogMotorCtrl::ClickBtnenhctrlMotStop0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//20151130ngh trigger 추가
	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	ServoStop(MOTOR_0);
	m_iProcessMoveIdx = 0;
	KillTimer(10);

	pdlg->m_Trigger->SetOPMode(FALSE);
}

void CDialogMotorCtrl::ClickBtnenhctrlMotStop1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ServoStop(MOTOR_1);
	m_iProcessMoveIdx = 0;
	KillTimer(11);
}

void CDialogMotorCtrl::ClickBtnenhctrlMotStop2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ServoStop(MOTOR_2);
	m_iProcessMoveIdx = 0;
	KillTimer(12);
}

void CDialogMotorCtrl::ClickBtnenhctrlMotStop3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ServoStop(MOTOR_3);
	m_iProcessMoveIdx = 0;
	KillTimer(13);
}

void CDialogMotorCtrl::ClickBtnenhctrlMotAllStop()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	for(int i = 0; i < MAX_MOTOR; i++)
	{
		ServoStop(i);
	}
}

void CDialogMotorCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strValue, msg;
	double pos;

	//20151130ngh trigger 추가

	switch(nIDEvent)
	{
	case 0:
		ReadServoState();

		if(IsWindowVisible())
		{
			strValue.Format(_T("%4.06f"), GetRobotPos_mm(MOTOR_0));
			m_btn_mot_pos0.SetCaption(strValue);
			strValue.Format(_T("%4.06f"), GetCtsToMM(MOTOR_0, GetCurSpeed(MOTOR_0)));
			m_btn_mot_spd0.SetCaption(strValue);

			if(IsServoAmpEnable(MOTOR_0))
			{
				if(!m_btn_ae0.GetValue())
				{
					m_btn_ae0.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_ae0.GetValue())
				{
					m_btn_ae0.SetValue(FALSE);
				}
			}
			if(IsServoAmpFault(MOTOR_0))
			{
				if(!m_btn_af0.GetValue())
				{
					m_btn_af0.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_af0.GetValue())
				{
					m_btn_af0.SetValue(FALSE);
				}
			}		
			if(IsServoFollowingError(MOTOR_0))
			{
				if(!m_btn_fe0.GetValue())
				{
					m_btn_fe0.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_fe0.GetValue())
				{
					m_btn_fe0.SetValue(FALSE);
				}
			}
			if(IsServoI2TFault(MOTOR_0))
			{
				if(!m_btn_i2t0.GetValue())
				{
					m_btn_i2t0.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_i2t0.GetValue())
				{
					m_btn_i2t0.SetValue(FALSE);
				}
			}

			strValue.Format(_T("%4.06f"), GetRobotPos_mm(MOTOR_1));
			m_btn_mot_pos1.SetCaption(strValue);
			strValue.Format(_T("%4.06f"), GetCtsToMM(MOTOR_1, GetCurSpeed(MOTOR_1)));
			m_btn_mot_spd1.SetCaption(strValue);
			if(IsServoAmpEnable(MOTOR_1))
			{
				if(!m_btn_ae1.GetValue())
				{
					m_btn_ae1.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_ae1.GetValue())
				{
					m_btn_ae1.SetValue(FALSE);
				}
			}
			if(IsServoAmpFault(MOTOR_1))
			{
				if(!m_btn_af1.GetValue())
				{
					m_btn_af1.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_af1.GetValue())
				{
					m_btn_af1.SetValue(FALSE);
				}
			}		
			if(IsServoFollowingError(MOTOR_1))
			{
				if(!m_btn_fe1.GetValue())
				{
					m_btn_fe1.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_fe1.GetValue())
				{
					m_btn_fe1.SetValue(FALSE);
				}
			}
			if(IsServoI2TFault(MOTOR_1))
			{
				if(!m_btn_i2t1.GetValue())
				{
					m_btn_i2t1.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_i2t1.GetValue())
				{
					m_btn_i2t1.SetValue(FALSE);
				}
			}

			strValue.Format(_T("%4.06f"), GetRobotPos_mm(MOTOR_2));
			m_btn_mot_pos2.SetCaption(strValue);
			strValue.Format(_T("%4.06f"), GetCtsToMM(MOTOR_2, GetCurSpeed(MOTOR_2)));
			m_btn_mot_spd2.SetCaption(strValue);
			if(IsServoAmpEnable(MOTOR_2))
			{
				if(!m_btn_ae2.GetValue())
				{
					m_btn_ae2.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_ae2.GetValue())
				{
					m_btn_ae2.SetValue(FALSE);
				}
			}
			if(IsServoAmpFault(MOTOR_2))
			{
				if(!m_btn_af2.GetValue())
				{
					m_btn_af2.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_af2.GetValue())
				{
					m_btn_af2.SetValue(FALSE);
				}
			}		
			if(IsServoFollowingError(MOTOR_2))
			{
				if(!m_btn_fe2.GetValue())
				{
					m_btn_fe2.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_fe2.GetValue())
				{
					m_btn_fe2.SetValue(FALSE);
				}
			}
			if(IsServoI2TFault(MOTOR_2))
			{
				if(!m_btn_i2t2.GetValue())
				{
					m_btn_i2t2.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_i2t2.GetValue())
				{
					m_btn_i2t2.SetValue(FALSE);
				}
			}

			strValue.Format(_T("%4.06f"), GetRobotPos_mm(MOTOR_3));
			m_btn_mot_pos3.SetCaption(strValue);
			strValue.Format(_T("%4.06f"), GetCtsToMM(MOTOR_3, GetCurSpeed(MOTOR_3)));
			m_btn_mot_spd3.SetCaption(strValue);
			if(IsServoAmpEnable(MOTOR_3))
			{
				if(!m_btn_ae3.GetValue())
				{
					m_btn_ae3.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_ae3.GetValue())
				{
					m_btn_ae3.SetValue(FALSE);
				}
			}
			if(IsServoAmpFault(MOTOR_3))
			{
				if(!m_btn_af3.GetValue())
				{
					m_btn_af3.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_af3.GetValue())
				{
					m_btn_af3.SetValue(FALSE);
				}
			}		
			if(IsServoFollowingError(MOTOR_3))
			{
				if(!m_btn_fe3.GetValue())
				{
					m_btn_fe3.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_fe3.GetValue())
				{
					m_btn_fe3.SetValue(FALSE);
				}
			}
			if(IsServoI2TFault(MOTOR_3))
			{
				if(!m_btn_i2t3.GetValue())
				{
					m_btn_i2t3.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_i2t3.GetValue())
				{
					m_btn_i2t3.SetValue(FALSE);
				}
			}

			if(IsServoOriginComplete(MOTOR_0))
			{
				if(!m_btn_mot_home0.GetValue())
				{
					m_btn_mot_home0.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_mot_home0.GetValue())
				{
					m_btn_mot_home0.SetValue(FALSE);
				}
			}

			if(IsServoOriginComplete(MOTOR_1))
			{
				if(!m_btn_mot_home1.GetValue())
				{
					m_btn_mot_home1.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_mot_home1.GetValue())
				{
					m_btn_mot_home1.SetValue(FALSE);
				}
			}

			if(IsServoOriginComplete(MOTOR_2))
			{
				if(!m_btn_mot_home2.GetValue())
				{
					m_btn_mot_home2.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_mot_home2.GetValue())
				{
					m_btn_mot_home2.SetValue(FALSE);
				}
			}

			if(IsServoOriginComplete(MOTOR_3))
			{
				if(!m_btn_mot_home3.GetValue())
				{
					m_btn_mot_home3.SetValue(TRUE);
				}
			}
			else
			{
				if(m_btn_mot_home3.GetValue())
				{
					m_btn_mot_home3.SetValue(FALSE);
				}
			}
		}
		break;
	case 10:
		m_iProcessMoveIdx += 5;
		if(100 < m_iProcessMoveIdx)m_iProcessMoveIdx = 0;
		m_btn_progress_move.SetProgressBarValue(m_iProcessMoveIdx);

		//Move
		ReadServoState();
		if(IsServoInPosition(MOTOR_0))
		{
			pos = GetCurPos(MOTOR_0);

			//20160727 ngh
			//20151130ngh trigger 추가
			if(((m_edit_e_pos0 * 6400) - MOTOR_INPOS_LMT) <= pos && pos <= ((m_edit_e_pos0 * 6400) + MOTOR_INPOS_LMT))
			{
				pdlg->m_Trigger->SetResetEncoderCnt();
			}

			if(pos == (m_edit_e_pos0 * 6400))
			{
				pdlg->m_Trigger->SetDirection(FALSE);
				//ServoMove(MOTOR_0, m_edit_e_pos0, m_edit_setspd0);
				SetRobotMove_mm(MOTOR_0, m_edit_e_pos0, m_edit_setspd0);
			}
			else
			{
				pdlg->m_Trigger->SetDirection(TRUE);
				//ServoMove(MOTOR_0, m_edit_s_pos0, m_edit_setspd0);
				SetRobotMove_mm(MOTOR_0, m_edit_e_pos0, m_edit_setspd0);
			}
		}
		break;
	case 11:
		m_iProcessMoveIdx += 5;
		if(100 < m_iProcessMoveIdx)m_iProcessMoveIdx = 0;
		m_btn_progress_move.SetProgressBarValue(m_iProcessMoveIdx);
		break;
	case 12:
		m_iProcessMoveIdx += 5;
		if(100 < m_iProcessMoveIdx)m_iProcessMoveIdx = 0;
		m_btn_progress_move.SetProgressBarValue(m_iProcessMoveIdx);
		break;
	case 13:
		m_iProcessMoveIdx += 5;
		if(100 < m_iProcessMoveIdx)m_iProcessMoveIdx = 0;
		m_btn_progress_move.SetProgressBarValue(m_iProcessMoveIdx);
		break;
	case 20:
		m_iProcessMoveIdx += 5;
		if(100 < m_iProcessMoveIdx)m_iProcessMoveIdx = 0;
		if(IsWindowVisible())
		{
			m_btn_progress_move.SetProgressBarValue(m_iProcessMoveIdx);
		}
#ifdef MACHINE
		if(IsServoOriginComplete(MOTOR_0))
		{
			msg.Format(_T("0 Axis Origin End"));
			pdlg->WriteLog(MOTOR_LOG, msg);
			m_bOriginWait[MOTOR_0] = FALSE;
			KillTimer(20);
		}
#else
		msg.Format(_T("0 Axis Origin End"));
		pdlg->WriteLog(MOTOR_LOG, msg);
		m_bOriginWait[MOTOR_0] = FALSE;
		KillTimer(20);
#endif
		break;
	case 21:
		m_iProcessMoveIdx += 5;
		if(100 < m_iProcessMoveIdx)m_iProcessMoveIdx = 0;
		if(IsWindowVisible())
		{
			m_btn_progress_move.SetProgressBarValue(m_iProcessMoveIdx);
		}
#ifdef MACHINE
		if(IsServoOriginComplete(MOTOR_1))
		{
			msg.Format(_T("1 Axis Origin End"));
			pdlg->WriteLog(MOTOR_LOG, msg);
			m_bOriginWait[MOTOR_1] = FALSE;
			KillTimer(21);
		}
#else
		msg.Format(_T("1 Axis Origin End"));
		pdlg->WriteLog(MOTOR_LOG, msg);
		m_bOriginWait[MOTOR_1] = FALSE;
		KillTimer(21);
#endif
		break;
	case 22:
		m_iProcessMoveIdx += 5;
		if(100 < m_iProcessMoveIdx)m_iProcessMoveIdx = 0;
		if(IsWindowVisible())
		{
			m_btn_progress_move.SetProgressBarValue(m_iProcessMoveIdx);
		}
#ifdef MACHINE
		if(IsServoOriginComplete(MOTOR_2))
		{
			msg.Format(_T("2 Axis Origin End"));
			pdlg->WriteLog(MOTOR_LOG, msg);
			m_bOriginWait[MOTOR_2] = FALSE;
			KillTimer(22);
		}
#else
		msg.Format(_T("2 Axis Origin End"));
		pdlg->WriteLog(MOTOR_LOG, msg);
		m_bOriginWait[MOTOR_2] = FALSE;
		KillTimer(22);
#endif
		break;
	case 23:
		m_iProcessMoveIdx += 5;
		if(100 < m_iProcessMoveIdx)m_iProcessMoveIdx = 0;
		m_btn_progress_move.SetProgressBarValue(m_iProcessMoveIdx);
#ifdef MACHINE
		if(IsServoOriginComplete(MOTOR_3))
		{
			msg.Format(_T("3 Axis Origin End"));
			pdlg->WriteLog(MOTOR_LOG, msg);
			m_bOriginWait[MOTOR_3] = FALSE;
			KillTimer(23);
		}
#else
		msg.Format(_T("3 Axis Origin End"));
		pdlg->WriteLog(MOTOR_LOG, msg);
		m_bOriginWait[MOTOR_3] = FALSE;
		KillTimer(23);
#endif
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::OnTimer)!"));
		break;
	}
}


void CDialogMotorCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow)
	{

	}
	else
	{
		ClickBtnenhctrlMotAllStop();
		//KillTimer(0);(죽이면않됨)
		//20151204 ngh 임시로 막음
		//KillTimer(10);
		KillTimer(11);
		KillTimer(12);
		KillTimer(13);

		KillTimer(20);
		KillTimer(21);
		KillTimer(22);
		KillTimer(23);
	}
}



void CDialogMotorCtrl::ClickBtnenhctrlMotSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}


void CDialogMotorCtrl::ClickBtnenhctrlMotApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CDialogMotorCtrl::ClickBtnenhctrlMotCancel()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ShowWindow(SW_HIDE);
}

BOOL CDialogMotorCtrl::IsServoAmpEnable(int nAxis)
{
	BOOL state = FALSE;
	CString strAddress;

	int axi_idx = 0;

	switch(nAxis)
	{
	case 0:
		axi_idx = m_edit_axi_id0;
		break;
	case 1:
		axi_idx = m_edit_axi_id1;
		break;
	case 2:
		axi_idx = m_edit_axi_id2;
		break;
	case 3:
		axi_idx = m_edit_axi_id3;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::IsServoAmpEnable)!"));
		break;
	}
	
	strAddress.Format(_T("M%d39"), axi_idx);
	state = m_dmMotorStatus[strAddress];
	
	return state;
}

BOOL CDialogMotorCtrl::IsServoAmpFault(int nAxis)
{
	BOOL state = FALSE;
	CString strAddress;

	int axi_idx = 0;

	switch(nAxis)
	{
	case 0:
		axi_idx = m_edit_axi_id0;
		break;
	case 1:
		axi_idx = m_edit_axi_id1;
		break;
	case 2:
		axi_idx = m_edit_axi_id2;
		break;
	case 3:
		axi_idx = m_edit_axi_id3;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::IsServoAmpEnable)!"));
		break;
	}
	
	strAddress.Format(_T("M%d43"), axi_idx);
	state = m_dmMotorStatus[strAddress];
	
	return state;
}

BOOL CDialogMotorCtrl::IsServoFollowingError(int nAxis)
{
	BOOL state = FALSE;
	CString strAddress;

	int axi_idx = 0;

	switch(nAxis)
	{
	case 0:
		axi_idx = m_edit_axi_id0;
		break;
	case 1:
		axi_idx = m_edit_axi_id1;
		break;
	case 2:
		axi_idx = m_edit_axi_id2;
		break;
	case 3:
		axi_idx = m_edit_axi_id3;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::IsServoFollowingError)!"));
		break;
	}
	
	strAddress.Format(_T("M%d42"), axi_idx);
	state = m_dmMotorStatus[strAddress];
	
	return state;
}

BOOL CDialogMotorCtrl::IsServoI2TFault(int nAxis)
{
	BOOL state = FALSE;
	CString strAddress;

	int axi_idx = 0;

	switch(nAxis)
	{
	case 0:
		axi_idx = m_edit_axi_id0;
		break;
	case 1:
		axi_idx = m_edit_axi_id1;
		break;
	case 2:
		axi_idx = m_edit_axi_id2;
		break;
	case 3:
		axi_idx = m_edit_axi_id3;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::IsServoI2TFault)!"));
		break;
	}
	
	strAddress.Format(_T("M%d47"), axi_idx);
	state = m_dmMotorStatus[strAddress];
	
	return state;
}

BOOL CDialogMotorCtrl::IsServoOriginComplete(int nAxis)  // P변수 : USER 정의 함수 
{
	BOOL state = FALSE;
	CString strAddress, strRes;

	int axi_idx = 0;

	switch(nAxis)
	{
	case 0:
		axi_idx = m_edit_axi_id0;
		break;
	case 1:
		axi_idx = m_edit_axi_id1;
		break;
	case 2:
		axi_idx = m_edit_axi_id2;
		break;
	case 3:
		axi_idx = m_edit_axi_id3;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::IsServoOriginComplete)!"));
		break;
	}

#ifdef MACHINE
	strAddress.Format(_T("M%d45"), axi_idx);
	state = m_dmMotorStatus[strAddress];
#else
	state = TRUE;
#endif
	return state;
}

BOOL CDialogMotorCtrl::IsServoInPosition(int nAxis)
{
	BOOL state = FALSE;
	CString strAddress, strRes;

	int axi_idx = 0;

	switch(nAxis)
	{
	case 0:
		axi_idx = m_edit_axi_id0;
		break;
	case 1:
		axi_idx = m_edit_axi_id1;
		break;
	case 2:
		axi_idx = m_edit_axi_id2;
		break;
	case 3:
		axi_idx = m_edit_axi_id3;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::IsServoInPosition)!"));
		break;
	}
	
	strAddress.Format(_T("M%d40"), axi_idx);
	state = m_dmMotorStatus[strAddress];

	return state;
}

void CDialogMotorCtrl::Update(BOOL flag)
{
	CRect rect;
	int i, idx;

	if(flag)
	{
		//리소스 변수로 가져오기

	}
	else
	{
		//리소스 변수로 주기

	}
}




BOOL CDialogMotorCtrl::LoadMotorPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\MotorCtrl.par");

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
		key.Format(_T("m_edit_setspd0"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_setspd0 = atof(ctemp);
	
		
		key.Empty();
		key.Format(_T("m_edit_setspd1"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_setspd1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_setspd2"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_setspd2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_setspd3"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_setspd3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_s_pos0"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_s_pos0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_s_pos1"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_s_pos1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_s_pos2"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_s_pos2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_s_pos3"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_s_pos3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_e_pos0"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_e_pos0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_e_pos1"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_e_pos1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_e_pos2"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_e_pos2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_e_pos3"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_e_pos3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_name0"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_name0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_name1"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_name1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_name2"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_name2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_name3"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_name3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_id0"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_id0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_id1"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_id1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_id2"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_id2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_id3"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_id3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_p_lmt0"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_p_lmt0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_p_lmt1"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_p_lmt1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_p_lmt2"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_p_lmt2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_p_lmt3"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_p_lmt3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_m_lmt0"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_m_lmt0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_m_lmt1"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_m_lmt1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_m_lmt2"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_m_lmt2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_m_lmt3"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_m_lmt3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_spd_lmt0"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_spd_lmt0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_spd_lmt1"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_spd_lmt1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_spd_lmt2"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_spd_lmt2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_spd_lmt3"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_spd_lmt3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_res0"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_res0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_res1"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_res1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_res2"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_res2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_axi_res3"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_axi_res3 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_op_spd0"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_op_spd0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_op_spd1"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_op_spd1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_op_spd2"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_op_spd2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_op_y_spd0"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_op_y_spd0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_op_y_spd1"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_op_y_spd1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_op_y_spd2"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_op_y_spd2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_op_t_spd0"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_op_t_spd0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_op_t_spd1"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_op_t_spd1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_op_t_spd2"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_op_t_spd2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_op_z_spd0"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_op_z_spd0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_op_z_spd1"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_op_z_spd1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_op_z_spd2"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_op_z_spd2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_8k_scan_speed"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_8k_scan_speed = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_16k_scan_speed"));
		GetPrivateProfileString(_T("MOTOR CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_16k_scan_speed = atof(ctemp);

		;
	}
	

	//Fixed
	m_edit_axi_id0 = MOTOR_0_ID;
	m_edit_axi_id1 = MOTOR_1_ID;
	m_edit_axi_id2 = MOTOR_2_ID;
	m_edit_axi_id3 = MOTOR_3_ID;

	m_edit_axi_res0 = MOTOR_0_CTS_PER_1MM;		//cts/mm
	m_edit_axi_res1 = MOTOR_1_CTS_PER_1MM;		//cts/mm
	m_edit_axi_res2 = MOTOR_2_CTS_PER_1DEG;		//cts/1도
	m_edit_axi_res3 = MOTOR_3_CTS_PER_1MM;		//cts/mm(1회전 5mm이동, 1회전당 1,000cts)

	m_edit_axi_p_lmt0 = GetCtsToMM(MOTOR_0, MOTOR_0_P_LMT_POS);
	m_edit_axi_p_lmt1 = GetCtsToMM(MOTOR_1, MOTOR_1_P_LMT_POS);
	m_edit_axi_p_lmt2 = GetCtsToMM(MOTOR_2, MOTOR_2_P_LMT_POS);
	m_edit_axi_p_lmt3 = GetCtsToMM(MOTOR_3, MOTOR_3_P_LMT_POS);

	m_edit_axi_m_lmt0 = GetCtsToMM(MOTOR_0, MOTOR_0_M_LMT_POS);
	m_edit_axi_m_lmt1 = GetCtsToMM(MOTOR_1, MOTOR_1_M_LMT_POS);
	m_edit_axi_m_lmt2 = GetCtsToMM(MOTOR_2, MOTOR_2_M_LMT_POS);
	m_edit_axi_m_lmt3 = GetCtsToMM(MOTOR_3, MOTOR_3_M_LMT_POS);

	m_edit_axi_spd_lmt0 = GetCtsToMM(MOTOR_0, MOTOR_0_LMT_SPD);
	m_edit_axi_spd_lmt1 = GetCtsToMM(MOTOR_1, MOTOR_1_LMT_SPD);
	m_edit_axi_spd_lmt2 = GetCtsToMM(MOTOR_2, MOTOR_2_LMT_SPD);
	m_edit_axi_spd_lmt3 = GetCtsToMM(MOTOR_3, MOTOR_3_LMT_SPD);


	fd=0;
	
	return rslt;
}

BOOL CDialogMotorCtrl::SaveMotorPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\MotorCtrl.par");

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
		key.Format(_T("m_edit_setspd0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_setspd0);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_setspd1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_setspd1);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_setspd2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_setspd2);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_setspd3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_setspd3);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_s_pos0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_s_pos0);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_s_pos1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_s_pos1);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_s_pos2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_s_pos2);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_s_pos3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_s_pos3);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_e_pos0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_e_pos0);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_e_pos1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_e_pos1);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_e_pos2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_e_pos2);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_e_pos3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_e_pos3);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_name0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_name0);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_name1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_name1);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_name2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_name2);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_name3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_name3);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_id0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_id0);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_id1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_id1);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_id2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_id2);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_id3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_id3);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_p_lmt0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_p_lmt0);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_p_lmt1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_p_lmt1);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_p_lmt2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_p_lmt2);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_p_lmt3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_p_lmt3);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_m_lmt0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_m_lmt0);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_m_lmt1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_m_lmt1);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_m_lmt2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_m_lmt2);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_m_lmt3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_m_lmt3);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_spd_lmt0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_spd_lmt0);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_spd_lmt1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_spd_lmt1);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_spd_lmt2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_spd_lmt2);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_spd_lmt3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_spd_lmt3);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_res0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_res0);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_res1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_res1);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_res2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_res2);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_axi_res3"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_axi_res3);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_op_spd0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_op_spd0);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_op_spd1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_op_spd1);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_op_spd2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_op_spd2);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_op_y_spd0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_op_y_spd0);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_op_y_spd1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_op_y_spd1);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_op_y_spd2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_op_y_spd2);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_op_t_spd0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_op_t_spd0);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_op_t_spd1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_op_t_spd1);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_op_t_spd2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_op_t_spd2);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_op_z_spd0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_op_z_spd0);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_op_z_spd1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_op_z_spd1);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_op_z_spd2"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_op_z_spd2);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_8k_scan_speed"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_8k_scan_speed);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_16k_scan_speed"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_16k_scan_speed);
		WritePrivateProfileString(_T("MOTOR CTRL"), key, dat, file_path);

		
	}
	fd=0;
	
	return rslt;
}


void CDialogMotorCtrl::ClickBtnenhctrlMotRepeat0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//20151130ngh trigger 추가
	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	pdlg->m_Trigger->SetOPMode(TRUE);

	SetTimer(10, 200, NULL);
	m_iProcessMoveIdx = 0;
}


void CDialogMotorCtrl::ClickBtnenhctrlMotRepeat1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SetTimer(11, 200, NULL);
	m_iProcessMoveIdx = 0;
}


void CDialogMotorCtrl::ClickBtnenhctrlMotRepeat2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SetTimer(12, 200, NULL);
	m_iProcessMoveIdx = 0;
}


void CDialogMotorCtrl::ClickBtnenhctrlMotRepeat3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SetTimer(13, 200, NULL);
	m_iProcessMoveIdx = 0;
}


double CDialogMotorCtrl::GetRobotPos_mm(int axi)
{
	double pos_cts = 0;
	double pos_mm = 0.;

	//Get robot cts
	pos_cts = GetCurPos(axi);

	//Convert unit
	switch(axi)
	{
	//Convert cts->mm
	case 0:
		if(0 < m_edit_axi_res0)
		{
			pos_mm = pos_cts / m_edit_axi_res0;
		}
		else
		{

		}
		break;
	case 1:
		if(0 < m_edit_axi_res1)
		{
			pos_mm = pos_cts / m_edit_axi_res1;
		}
		else
		{

		}
		break;
	case 3:
		if(0 < m_edit_axi_res3)
		{
			pos_mm = pos_cts / m_edit_axi_res3;
		}
		else
		{

		}
		break;
	//Convert cts->1'
	case 2:
		if(0 < m_edit_axi_res2)
		{
			pos_mm = pos_cts / m_edit_axi_res2;
		}
		else
		{

		}
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::GetRobotPos_mm)!"));
		break;
	}

	return pos_mm;
}

double CDialogMotorCtrl::GetRobotPos_cts(int axi)
{
	double pos_cts = 0;

	//Get robot cts
	pos_cts = GetCurPos(axi);

	return pos_cts;
}

double CDialogMotorCtrl::Get8KScanSpeed(){ return m_edit_8k_scan_speed;}
double CDialogMotorCtrl::Get16KScanSpeed(){ return m_edit_16k_scan_speed;}

BOOL CDialogMotorCtrl::SetRobotMove_mm(int axi, double pos_mm)
{
	double rslt = FALSE;
	double pos_cts = 0.;
	double speed = 0.;
	double lift_pos = 0.;

	//20160717 ngh
	if(axi == 0 || axi == 1 || axi == 2)
	{
		if(GetRobotPos_mm(3) > 1)
		{
			return FALSE;
		}
	}

	////20160727 ngh
	//if(axi == 3)
	//{
	//	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	//	if(!pdlg->m_IO->GetGlassEdgeStatus())
	//	{
	//		return FALSE;
	//	}
	//}

	//Convert
	switch(axi)
	{
	//Convert mm -> cts
	case 0:
		/*if(0 < m_edit_axi_res0)
		{
			pos_cts = pos_mm * m_edit_axi_res0;
		}
		else
		{

		}*/
		
		rslt= ServoMove(axi, pos_mm, m_edit_setspd0);
		break;
	case 1:
		/*if(0 < m_edit_axi_res1)
		{
			pos_cts = pos_mm * m_edit_axi_res1;
		}
		else
		{

		}*/
		rslt= ServoMove(axi, pos_mm, m_edit_setspd1);
		break;
	case 3:
		/*if(0 < m_edit_axi_res3)
		{
			pos_cts = pos_mm * m_edit_axi_res3;
		}
		else
		{

		}*/
		rslt= ServoMove(axi, pos_mm, m_edit_setspd3);
		break;
	//Convert cts->1'
	case 2:
		/*if(0 < m_edit_axi_res2)
		{
			pos_cts = pos_mm * m_edit_axi_res2;
		}
		else
		{

		}*/
		rslt= ServoMove(axi, pos_mm, m_edit_setspd2);
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::SetRobotMove_mm)!"));
		break;
	}

	return rslt;
}


BOOL CDialogMotorCtrl::SetRobotMove_mm(int axi, double pos_mm, double spd)
{
	double rslt = FALSE;
	double pos_cts = 0.;

	//Convert
	switch(axi)
	{
	//Convert mm -> cts
	case 0:
		/*if(0 < m_edit_axi_res0)
		{
			pos_cts = pos_mm * m_edit_axi_res0;
		}
		else
		{

		}*/
		rslt= ServoMove(axi, pos_mm, spd);
		break;
	case 1:
		/*if(0 < m_edit_axi_res1)
		{
			pos_cts = pos_mm * m_edit_axi_res1;
		}
		else
		{

		}*/
		rslt= ServoMove(axi, pos_mm, spd);
		break;
	case 3:
		/*if(0 < m_edit_axi_res3)
		{
			pos_cts = pos_mm * m_edit_axi_res3;
		}
		else
		{

		}*/
		rslt= ServoMove(axi, pos_mm, spd);
		break;
	//Convert cts->1'
	case 2:
		/*if(0 < m_edit_axi_res2)
		{
			pos_cts = pos_mm * m_edit_axi_res2;
		}
		else
		{

		}*/
		rslt= ServoMove(axi, pos_mm, spd);
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::SetRobotMove_mm)!"));
		break;
	}

	return rslt;
}



BOOL CDialogMotorCtrl::SetRobotMove_cts(int axi, double pos_cts)
{
	double rslt = FALSE;

	rslt= ServoMove(axi, pos_cts, m_edit_setspd0);

	return rslt;
}

void CDialogMotorCtrl::SetRobotJogMove(int axi, int direct, int spd_select)
{
	double spd_x = 0;
	double spd_y = 0;
	double spd_t = 0;
	double spd_z = 0;

	switch(spd_select)
	{
	case 0:
		spd_x = m_edit_op_spd0;
		spd_y = m_edit_op_y_spd0;
		spd_t = m_edit_op_t_spd0;
		spd_z = m_edit_op_z_spd0;
		break;
	case 1:
		spd_x = m_edit_op_spd1;
		spd_y = m_edit_op_y_spd1;
		spd_t = m_edit_op_t_spd1;
		spd_z = m_edit_op_z_spd1;
		break;
	case 2:
		spd_x = m_edit_op_spd2;
		spd_y = m_edit_op_y_spd2;
		spd_t = m_edit_op_t_spd2;
		spd_z = m_edit_op_z_spd2;
		break;
	default:
		spd_x = m_edit_op_spd0;
		spd_y = m_edit_op_y_spd0;
		spd_t = m_edit_op_t_spd0;
		spd_z = m_edit_op_z_spd0;
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::SetRobotJogMove)!"));
		break;
	}

	m_bMotionJogDrawCrowsStart = TRUE;

	switch(axi)
	{
	case 0:
		ServoJogMove(axi, direct, spd_y);
		break;
	case 1:
		ServoJogMove(axi, direct, spd_x);
		break;
	case 2:
		ServoJogMove(axi, direct, spd_t);
		break;
	case 3:
		ServoJogMove(axi, direct, spd_z);
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::SetRobotJogMove)!"));
		break;
	}	
}

void CDialogMotorCtrl::SetRobotJogStop(int axi)
{
	ServoStop(axi);
	m_bMotionJogDrawCrowsStart = FALSE;
}


BOOL CDialogMotorCtrl::GetIsInposition(int axi)
{
	return IsServoInPosition(axi);
}

BOOL CDialogMotorCtrl::GetIsMoveComplete(int axi, double pos)
{
	double cur_pos;
	cur_pos = GetRobotPos_mm(axi);

	if((pos - MOTOR_INPOS_MM) <= cur_pos && cur_pos <= (pos + MOTOR_INPOS_MM))
	{
		return TRUE;
	}

	return FALSE;
}



void CDialogMotorCtrl::ClickBtnenhctrlMotMove0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ServoMove(MOTOR_0, m_edit_s_pos0, m_edit_setspd0);
}


void CDialogMotorCtrl::ClickBtnenhctrlMotMove1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ServoMove(MOTOR_1, m_edit_s_pos1, m_edit_setspd1);
}


void CDialogMotorCtrl::ClickBtnenhctrlMotMove2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ServoMove(MOTOR_2, m_edit_s_pos2, m_edit_setspd2);
}


void CDialogMotorCtrl::ClickBtnenhctrlMotMove3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CDialogMSG dlg;
	CString str;
	if(GetRobotPos_mm(MOTOR_1) == 0)
	{
		ServoMove(MOTOR_3, m_edit_s_pos3, m_edit_setspd3);
	}
	else
	{
		str.Format(_T("Shift Axie Waite Error\n대기위치(홈위치)로 이동 바랍니다."));
		dlg.SetCaption(str);
		dlg.DoModal();
	}
}


BOOL CDialogMotorCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDialogMotorCtrl::GetIsServoOriginComplete()
{
	if(!IsServoOriginComplete(MOTOR_0))
	{

	}
}

BOOL CDialogMotorCtrl::GetServoOriginStatus(int axi)
{
	BOOL rslt = FALSE;

#ifdef MACHINE
	ReadServoState();
#endif

	switch(axi)
	{
	case 0:
		rslt = m_dmMotorStatus[_T("M145")];
		break;
	case 1:
		rslt = m_dmMotorStatus[_T("M245")];
		break;
	case 2:
		rslt = m_dmMotorStatus[_T("M545")];
		break;
	case 3:
		rslt = m_dmMotorStatus[_T("M645")];
		break;
	}

	return rslt;
}

void CDialogMotorCtrl::ClickBtnenhctrlMotAllHome()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);

#ifdef MACHINE
	if(IoStateCheck())
#else
	if(1)
#endif
	{
		MotorAllOrigin();
	}
	else
		return;
}

void CDialogMotorCtrl::Wait(DWORD dwMillisecond)
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();

	while(GetTickCount() - dwStart < dwMillisecond)
	{
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
} 

//20160530 ngh
void CDialogMotorCtrl::SetMoterAllOrigin()
{
	MotorAllOrigin();
}
void CDialogMotorCtrl::MotorAllOrigin()
{
	//20160721 ngh
	m_bMotorOriginStart = TRUE;
	m_bMotorOriginEnd = FALSE;

	m_pThreadMotionOrigin = AfxBeginThread(MotionOriginThread, this, THREAD_PRIORITY_NORMAL);
	if (m_pThreadMotionOrigin) m_pThreadMotionOrigin->m_bAutoDelete = TRUE;
}

double CDialogMotorCtrl::GetMMToCts(int nAxis, double dMm)
{
	double dVal = 0.;

	switch(nAxis)
	{
	case 0:
		dVal = dMm * m_edit_axi_res0;
		break;
	case 1:
		dVal = dMm * m_edit_axi_res1;
		break;
	case 2:
		dVal = dMm * m_edit_axi_res2;
		break;
	case 3:
		dVal = dMm * m_edit_axi_res3;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::GetMMToCts)!"));
		break;
	}

	return dVal;
}

double CDialogMotorCtrl::GetCtsToMM(int nAxis, double dCts)
{
	double dVal = 0.;

	switch(nAxis)
	{
	case 0:
		dVal = dCts / m_edit_axi_res0;
		break;
	case 1:
		dVal = dCts / m_edit_axi_res1;
		break;
	case 2:
		dVal = dCts / m_edit_axi_res2;
		break;
	case 3:
		dVal = dCts / m_edit_axi_res3;
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogMotorCtrl::GetCtsToMM)!"));
		break;
	}

	return dVal;
}