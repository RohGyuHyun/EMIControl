// Dialog/DialogIPSCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EMIControl.h"
#include "Dialog/DialogIPSCtrl.h"

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
#include "DialogLogWindow.h"

#include "EMIControlDlg.h"
#include "afxdialogex.h"


// CDialogIPSCtrl 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogIPSCtrl, CDialogEx)

CDialogIPSCtrl::CDialogIPSCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogIPSCtrl::IDD, pParent)
	, m_edit_nips_port(0)
{
}

CDialogIPSCtrl::~CDialogIPSCtrl()
{
	SaveIPSPara(_T(IPS_PATH));
}

void CDialogIPSCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_IPS_IP, m_IPS_Address);
	DDX_Text(pDX, IDC_EDIT_IPS_PORT, m_edit_nips_port);
}


BEGIN_MESSAGE_MAP(CDialogIPSCtrl, CDialogEx)
END_MESSAGE_MAP()


// CDialogIPSCtrl 메시지 처리기입니다.


BOOL CDialogIPSCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
BEGIN_EVENTSINK_MAP(CDialogIPSCtrl, CDialogEx)
	ON_EVENT(CDialogIPSCtrl, IDC_BTNENHCTRL_IPS_APPLY, DISPID_CLICK, CDialogIPSCtrl::ClickBtnenhctrlIpsApply, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogIPSCtrl::ClickBtnenhctrlIpsApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	SaveIPSPara(_T(IPS_PATH));
	pdlg->SetServerConnect();
}

int CDialogIPSCtrl::GetIPSPort(){ return m_edit_nips_port; }

CString CDialogIPSCtrl::GetIPSIP()
{
	char temp[300];
	CString rslt;
	m_IPS_Address.GetWindowTextW((LPTSTR)temp, 300);
	rslt.Format(_T("%s"), temp);
	return rslt;
}


BOOL CDialogIPSCtrl::LoadIPSPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\IPSCtrl.par");

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
		key.Format(_T("m_IPS_Address"));
		GetPrivateProfileString(_T("IPS CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		m_IPS_Address.SetWindowTextW(dat);

		key.Empty();
		key.Format(_T("m_edit_nips_port"));
		GetPrivateProfileString(_T("IPS CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_nips_port = atoi(ctemp);
	}
	fd=0;
	
	return rslt;
}

BOOL CDialogIPSCtrl::SaveIPSPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\IPSCtrl.par");

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
		key.Format(_T("m_IPS_Address"));
		dat.Empty();
		
		CString rslt;
		m_IPS_Address.GetWindowTextW((LPTSTR)ctemp, 300);
		dat.Format(_T("%s"), ctemp);
		WritePrivateProfileString(_T("IPS CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_nips_port"));
		dat.Empty();
		dat.Format(_T("%d"), m_edit_nips_port);
		WritePrivateProfileString(_T("IPS CTRL"), key, dat, file_path);
	}
	fd = 0;

	return rslt;
}



BOOL CDialogIPSCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	LoadIPSPara(_T(IPS_PATH));
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
