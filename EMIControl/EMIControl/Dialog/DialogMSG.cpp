// Dialog/DialogMSG.cpp : 구현 파일입니다.
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
#include "SerialPort.h"
#include "afxdialogex.h"


// CDialogMSG 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogMSG, CDialogEx)

CDialogMSG::CDialogMSG(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogMSG::IDD, pParent)
{
	m_nDefectAlarmCode = 0;
}

CDialogMSG::~CDialogMSG()
{
}

void CDialogMSG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNENHCTRL_MSG, m_btn_msg);
}


BEGIN_MESSAGE_MAP(CDialogMSG, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogMSG 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDialogMSG, CDialogEx)
	ON_EVENT(CDialogMSG, IDC_BTNENHCTRL_MSG_OK, DISPID_CLICK, CDialogMSG::ClickBtnenhctrlMsgOk, VTS_NONE)
	ON_EVENT(CDialogMSG, IDC_BTNENHCTRL_MSG_CANCEL, DISPID_CLICK, CDialogMSG::ClickBtnenhctrlMsgCancel, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogMSG::ClickBtnenhctrlMsgOk()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
#ifdef MACHINE
	pdlg->m_Inline->SetDownAlarmCode(m_nDefectAlarmCode);
	pdlg->m_Inline->SetHeavyAlarmReport(FALSE, m_nDefectAlarmCode);
#endif
	EndDialog(1);
}


void CDialogMSG::ClickBtnenhctrlMsgCancel()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	EndDialog(0);
}

void CDialogMSG::SetCaption(CString msg)
{
	m_sMsg = msg;
}

void CDialogMSG::SetDefectAlarmCode(int alarm_code)
{
	m_nDefectAlarmCode = alarm_code;
}
void CDialogMSG::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow)
	{
		m_btn_msg.SetCaption(m_sMsg);
	}
	else
	{

	}
}

BOOL CDialogMSG::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
