// Dialog/DialogLogin.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EMIControl.h"
#include "Dialog/DialogLogin.h"

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


// CDialogLogin ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogLogin, CDialogEx)

CDialogLogin::CDialogLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogLogin::IDD, pParent)
{
	m_iSelectType = 0;
	m_edit_password = _T("");
}

CDialogLogin::~CDialogLogin()
{
	SavePasswordPara(LOGIN_PATH);
}

void CDialogLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNENHCTRL_LOGIN_OPERATOR, m_btn_operator);
	DDX_Control(pDX, IDC_BTNENHCTRL_LOGIN_TECHCIAN, m_btn_technician);
	DDX_Control(pDX, IDC_BTNENHCTRL_LOGIN_ADMIN, m_btn_admin);
	//  DDX_Control(pDX, IDC_EDIT_LOGIN_PASSWORD, m_edit_password);
	DDX_Text(pDX, IDC_EDIT_LOGIN_PASSWORD, m_edit_password);
	DDX_Control(pDX, IDC_BTNENHCTRL_LOGIN_RESULT, m_btn_login_rslt);
}


BEGIN_MESSAGE_MAP(CDialogLogin, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogLogin �޽��� ó�����Դϴ�.


void CDialogLogin::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if(bShow)
	{
		GetDlgItem(IDC_EDIT_LOGIN_PASSWORD)->SetFocus();
	}
	else
	{
		pdlg->SetManuSelect(1);
	}
}

BEGIN_EVENTSINK_MAP(CDialogLogin, CDialogEx)
	ON_EVENT(CDialogLogin, IDC_BTNENHCTRL_LOGIN_MANAGEMENT, DISPID_CLICK, CDialogLogin::ClickBtnenhctrlLoginManagement, VTS_NONE)
	ON_EVENT(CDialogLogin, IDC_BTNENHCTRL_LOGIN_OPERATOR, DISPID_CLICK, CDialogLogin::ClickBtnenhctrlLoginOperator, VTS_NONE)
	ON_EVENT(CDialogLogin, IDC_BTNENHCTRL_LOGIN_TECHCIAN, DISPID_CLICK, CDialogLogin::ClickBtnenhctrlLoginTechcian, VTS_NONE)
	ON_EVENT(CDialogLogin, IDC_BTNENHCTRL_LOGIN_ADMIN, DISPID_CLICK, CDialogLogin::ClickBtnenhctrlLoginAdmin, VTS_NONE)
	ON_EVENT(CDialogLogin, IDC_BTNENHCTRL_LOGIN_ACCESS_RIGHT, DISPID_CLICK, CDialogLogin::ClickBtnenhctrlLoginAccessRight, VTS_NONE)
	ON_EVENT(CDialogLogin, IDC_BTNENHCTRL_EXIT, DISPID_CLICK, CDialogLogin::ClickBtnenhctrlExit, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogLogin::ClickBtnenhctrlLoginManagement()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	//Password��
	CString password;
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	//��ȣ �ҷ�����
	password.Format(_T("%s"),m_pwdata.manage);//�ӽ�
	//���߿��� ���Ͽ��� �ҷ��´�.
	//

	UpdateData(TRUE);
	if(0 == m_edit_password.Compare(password))
	{
		//OK
		pdlg->SetAccess(0);
		ShowWindow(SW_HIDE);
	}
	else
	{
		m_btn_login_rslt.SetCaption(_T("     �߸��� PASSWORD�Դϴ�"));
	}
}


void CDialogLogin::ClickBtnenhctrlLoginOperator()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	//Password��
	CString password;
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	//��ȣ �ҷ�����
	password.Format(_T("%s"),m_pwdata.oper);//�ӽ�
	//���߿��� ���Ͽ��� �ҷ��´�.
	//

	UpdateData(TRUE);
	if(0 == m_edit_password.Compare(password))
	{
		//OK
		pdlg->SetAccess(1);
		ShowWindow(SW_HIDE);

	}
	else
	{
		m_btn_login_rslt.SetCaption(_T("     �߸��� PASSWORD�Դϴ�"));
	}
}


void CDialogLogin::ClickBtnenhctrlLoginTechcian()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	//Password��
	CString password;
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	//��ȣ �ҷ�����
	password.Format(_T("%s"),m_pwdata.tech);//�ӽ�
	//���߿��� ���Ͽ��� �ҷ��´�.
	//

	UpdateData(TRUE);
	if(0 == m_edit_password.Compare(password))
	{
		//OK
		pdlg->SetAccess(2);
		ShowWindow(SW_HIDE);
	}
	else
	{
		m_btn_login_rslt.SetCaption(_T("     �߸��� PASSWORD�Դϴ�"));
	}
}


void CDialogLogin::ClickBtnenhctrlLoginAdmin()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	//Password��
	CString password;
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	//��ȣ �ҷ�����
	password.Format(_T("%s"),m_pwdata.admin);//�ӽ�
	//���߿��� ���Ͽ��� �ҷ��´�.
	//

	UpdateData(TRUE);
	if(0 == m_edit_password.Compare(password))
	{
		//OK
		pdlg->SetAccess(3);
		ShowWindow(SW_HIDE);
	}
	else
	{
		m_btn_login_rslt.SetCaption(_T("     �߸��� PASSWORD�Դϴ�"));
	}
}


BOOL CDialogLogin::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDialogLogin::LoadPasswordPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	
	size_t getval = 0;
	
	file_path = path + _T("\\Password.par");

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
		key.Format(_T("m_pwdata.admin(pulse)"));
		GetPrivateProfileString(_T("LOGIN"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_pwdata.admin.Format(_T("%S"),ctemp);
	
		key.Empty();
		key.Format(_T("m_pwdata.manage(pulse)"));
		GetPrivateProfileString(_T("LOGIN"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_pwdata.manage.Format(_T("%S"),ctemp);

		key.Empty();
		key.Format(_T("m_pwdata.oper(pulse)"));
		GetPrivateProfileString(_T("LOGIN"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_pwdata.oper.Format(_T("%S"),ctemp);

		key.Empty();
		key.Format(_T("m_pwdata.tech(pulse)"));
		GetPrivateProfileString(_T("LOGIN"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_pwdata.tech.Format(_T("%S"),ctemp);
		
	}
	fd=0;
	
	return rslt;
}

BOOL CDialogLogin::SavePasswordPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\Password.par");

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
		key.Format(_T("m_pwdata.admin"));
		dat.Empty();
		dat.Format(_T("%s"), m_pwdata.admin);
		WritePrivateProfileString(_T("LOGIN"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_pwdata.manage"));
		dat.Empty();
		dat.Format(_T("%s"), m_pwdata.manage);
		WritePrivateProfileString(_T("LOGIN"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_pwdata.oper"));
		dat.Empty();
		dat.Format(_T("%s"), m_pwdata.oper);
		WritePrivateProfileString(_T("LOGIN"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_pwdata.tech"));
		dat.Empty();
		dat.Format(_T("%s"), m_pwdata.tech);
		WritePrivateProfileString(_T("LOGIN"), key, dat, file_path);

	}
	fd=0;
	
	return rslt;
}

BOOL CDialogLogin::PassWord()
{
	m_pwdata.admin.Format(_T("0000"));
	m_pwdata.manage.Format(_T("0000"));
	m_pwdata.oper.Format(_T("0000"));
	m_pwdata.tech.Format(_T("0000"));
	return TRUE;
}

BOOL CDialogLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	PassWord();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDialogLogin::ClickBtnenhctrlLoginAccessRight()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	//Password��
	CString password;
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	//��ȣ �ҷ�����
	password.Format(_T("%s"),m_pwdata.admin);//�ӽ�
	//���߿��� ���Ͽ��� �ҷ��´�.
	//

	UpdateData(TRUE);
	if(0 == m_edit_password.Compare(password))
	{
		//OK
		//ShowWindow(SW_HIDE);
		pdlg->m_AccessRight->ShowWindow(SW_SHOW);
	}
	else
	{
		m_btn_login_rslt.SetCaption(_T("     �߸��� PASSWORD�Դϴ�"));
	}
}


void CDialogLogin::ClickBtnenhctrlExit()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	pdlg->ProgramExit();
}
