// Dialog/DialogSystem.cpp : 구현 파일입니다.
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


// CDialogSystem 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogSystem, CDialogEx)

CDialogSystem::CDialogSystem(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogSystem::IDD, pParent)
{

}

CDialogSystem::~CDialogSystem()
{
}

void CDialogSystem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNENHCTRL_SYSTEM_MANUAL_CTRL, m_btn_glass_ctrl);
	DDX_Control(pDX, IDC_BTNENHCTRL_SYSTEM_IO_CTRL, m_btn_io_ctrl);
	DDX_Control(pDX, IDC_BTNENHCTRL_SYSTEM_MOTOR_CTRL, m_btn_motor_ctrl);
	DDX_Control(pDX, IDC_BTNENHCTRL_SYSTEM_LIGHT_CTRL, m_btn_light_ctrl);
	DDX_Control(pDX, IDC_BTNENHCTRL_SYSTEM_IPS_CTRL, m_btn_ips_ctrl);
	DDX_Control(pDX, IDC_BTNENHCTRL_SYSTEM_AF_CTRL, m_btn_af_ctrl);
	DDX_Control(pDX, IDC_BTNENHCTRL_SYSTEM_TRIGGER_CTRL, m_btn_trigger_ctrl);
}


BEGIN_MESSAGE_MAP(CDialogSystem, CDialogEx)
END_MESSAGE_MAP()


// CDialogSystem 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDialogSystem, CDialogEx)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_MOTOR_CTRL, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemMotorCtrl, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_AF_CTRL, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemAfCtrl, VTS_NONE)
//	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_LIGHT_CTRL, 6, CDialogSystem::AnimDLLDoneBtnenhctrlSystemLightCtrl, VTS_I4 VTS_BOOL)
ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_LIGHT_CTRL, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemLightCtrl, VTS_NONE)
ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_IPS_CTRL, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemIpsCtrl, VTS_NONE)
//ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_IO_CTRL, 6, CDialogSystem::AnimDLLDoneBtnenhctrlSystemIoCtrl, VTS_I4 VTS_BOOL)
ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_IO_CTRL, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemIoCtrl, VTS_NONE)
ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_MANUAL_CTRL, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemManualCtrl, VTS_NONE)
ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_TRIGGER_CTRL, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemTriggerCtrl, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogSystem::ClickBtnenhctrlSystemMotorCtrl()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	ManuSelect(2);
	pdlg->SystemMotorCtrl();
}

void CDialogSystem::ClickBtnenhctrlSystemAfCtrl()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	ManuSelect(5);
	pdlg->SystemAFCtrl();
}


void CDialogSystem::ClickBtnenhctrlSystemLightCtrl()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	ManuSelect(3);
	pdlg->SystemLightCtrl();
}


void CDialogSystem::ClickBtnenhctrlSystemIpsCtrl()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	ManuSelect(4);
	pdlg->SystemIPSCtrl();
}


BOOL CDialogSystem::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.



	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogSystem::ClickBtnenhctrlSystemIoCtrl()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	ManuSelect(1);
	pdlg->SystemIOCtrl();
}


void CDialogSystem::ClickBtnenhctrlSystemManualCtrl()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	ManuSelect(0);
	pdlg->SystemGlassManuCtrl();
}

//20151204 ngh 트리거 다이얼로그 표시
void CDialogSystem::ClickBtnenhctrlSystemTriggerCtrl()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	ManuSelect(6);
	pdlg->SystemTriggerCtrl();
}


BOOL CDialogSystem::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDialogSystem::ManuSelect(int nManuType)
{
	switch(nManuType)
	{
	case 0:
		m_btn_glass_ctrl.SetValue(TRUE);
		m_btn_io_ctrl.SetValue(FALSE);
		m_btn_motor_ctrl.SetValue(FALSE);
		m_btn_light_ctrl.SetValue(FALSE);
		m_btn_ips_ctrl.SetValue(FALSE);
		m_btn_af_ctrl.SetValue(FALSE);
		m_btn_trigger_ctrl.SetValue(FALSE);
		break;
	case 1:
		m_btn_glass_ctrl.SetValue(FALSE);
		m_btn_io_ctrl.SetValue(TRUE);
		m_btn_motor_ctrl.SetValue(FALSE);
		m_btn_light_ctrl.SetValue(FALSE);
		m_btn_ips_ctrl.SetValue(FALSE);
		m_btn_af_ctrl.SetValue(FALSE);
		m_btn_trigger_ctrl.SetValue(FALSE);
		break;
	case 2:
		m_btn_glass_ctrl.SetValue(FALSE);
		m_btn_io_ctrl.SetValue(FALSE);
		m_btn_motor_ctrl.SetValue(TRUE);
		m_btn_light_ctrl.SetValue(FALSE);
		m_btn_ips_ctrl.SetValue(FALSE);
		m_btn_af_ctrl.SetValue(FALSE);
		m_btn_trigger_ctrl.SetValue(FALSE);
		break;
	case 3:
		m_btn_glass_ctrl.SetValue(FALSE);
		m_btn_io_ctrl.SetValue(FALSE);
		m_btn_motor_ctrl.SetValue(FALSE);
		m_btn_light_ctrl.SetValue(TRUE);
		m_btn_ips_ctrl.SetValue(FALSE);
		m_btn_af_ctrl.SetValue(FALSE);
		m_btn_trigger_ctrl.SetValue(FALSE);
		break;
	case 4:
		m_btn_glass_ctrl.SetValue(FALSE);
		m_btn_io_ctrl.SetValue(FALSE);
		m_btn_motor_ctrl.SetValue(FALSE);
		m_btn_light_ctrl.SetValue(FALSE);
		m_btn_ips_ctrl.SetValue(TRUE);
		m_btn_af_ctrl.SetValue(FALSE);
		m_btn_trigger_ctrl.SetValue(FALSE);
		break;
	case 5:
		m_btn_glass_ctrl.SetValue(FALSE);
		m_btn_io_ctrl.SetValue(FALSE);
		m_btn_motor_ctrl.SetValue(FALSE);
		m_btn_light_ctrl.SetValue(FALSE);
		m_btn_ips_ctrl.SetValue(FALSE);
		m_btn_af_ctrl.SetValue(TRUE);
		m_btn_trigger_ctrl.SetValue(FALSE);
		break;
	case 6:
		m_btn_glass_ctrl.SetValue(FALSE);
		m_btn_io_ctrl.SetValue(FALSE);
		m_btn_motor_ctrl.SetValue(FALSE);
		m_btn_light_ctrl.SetValue(FALSE);
		m_btn_ips_ctrl.SetValue(FALSE);
		m_btn_af_ctrl.SetValue(FALSE);
		m_btn_trigger_ctrl.SetValue(TRUE);
		break;
	}
}