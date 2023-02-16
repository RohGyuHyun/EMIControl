// Dialog/DialogLogAlarm.cpp : 구현 파일입니다.
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


// CDialogLogAlarm 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogLogAlarm, CDialogEx)

CDialogLogAlarm::CDialogLogAlarm(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogLogAlarm::IDD, pParent)
{
	m_sMessage.Empty();
}

CDialogLogAlarm::~CDialogLogAlarm()
{
}

void CDialogLogAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNENHCTRL_LOG_ALARM_MSG, m_btn_msg);
}


BEGIN_MESSAGE_MAP(CDialogLogAlarm, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogLogAlarm 메시지 처리기입니다.
void CDialogLogAlarm::SetMessage(char *msg)
{
	TCHAR* sData = new TCHAR[512];
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)msg, -1, sData, 512);
	m_sMessage.Format(_T("%s"), sData);
}

void CDialogLogAlarm::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow)
	{
		m_btn_msg.SetCaption(m_sMessage);
	}
	else
	{

	}
}
BEGIN_EVENTSINK_MAP(CDialogLogAlarm, CDialogEx)
	ON_EVENT(CDialogLogAlarm, IDC_BTNENHCTRL_LOG_BUZ_OFF, DISPID_CLICK, CDialogLogAlarm::ClickBtnenhctrlLogBuzOff, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogLogAlarm::ClickBtnenhctrlLogBuzOff()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	//Buzzer off
	pdlg->m_IO->SetWriteDO(DO_BUZZER_MELODY1, FALSE);
}


BOOL CDialogLogAlarm::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
