// Dialog/DialogIPSCtrl.cpp : ���� �����Դϴ�.
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


// CDialogIPSCtrl ��ȭ �����Դϴ�.

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


// CDialogIPSCtrl �޽��� ó�����Դϴ�.


BOOL CDialogIPSCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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

	//file ���� Ȯ�� 
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

	//file ���� Ȯ�� 
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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	LoadIPSPara(_T(IPS_PATH));
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
