// dialog\AccessRight.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EMIControl.h"
#include "dialog\AccessRight.h"
#include "afxdialogex.h"


// AccessRight 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAccessRight, CDialogEx)

CAccessRight::CAccessRight(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAccessRight::IDD, pParent)
{
	memset(&m_AccessRight, NULL, sizeof(typeAccessRight) * MAX_ACCESS_RIGHT);
}

CAccessRight::~CAccessRight()
{

}

void CAccessRight::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_AUTORUN_0, m_btn_auto_run0);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_RECIPE_0, m_btn_recipe0);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_SYSTEM_0, m_btn_system0);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_ALIGN_0, m_btn_align0);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_INLINE_0, m_btn_inline0);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_LOG_0, m_btn_log0);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_DFSERVER_0, m_btn_defect_server0);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_AUTORUN_1, m_btn_auto_run1);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_RECIPE_1, m_btn_recipe1);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_SYSTEM_1, m_btn_system1);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_ALIGN_1, m_btn_align1);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_INLINE_1, m_btn_inline1);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_LOG_1, m_btn_log1);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_DFSERVER_1, m_btn_defect_server1);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_AUTORUN_2, m_btn_auto_run2);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_RECIPE_2, m_btn_recipe2);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_SYSTEM_2, m_btn_system2);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_ALIGN_2, m_btn_align2);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_INLINE_2, m_btn_inline2);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_LOG_2, m_btn_log2);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_DFSERVER_2, m_btn_defect_server2);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_AUTORUN_3, m_btn_auto_run3);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_RECIPE_3, m_btn_recipe3);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_SYSTEM_3, m_btn_system3);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_ALIGN_3, m_btn_align3);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_INLINE_3, m_btn_inline3);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_LOG_3, m_btn_log3);
	DDX_Control(pDX, IDC_BTNENHCTRL_ACCESS_DFSERVER_3, m_btn_defect_server3);
}

BEGIN_MESSAGE_MAP(CAccessRight, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// AccessRight 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CAccessRight, CDialogEx)
	ON_EVENT(CAccessRight, IDC_BTNENHCTRL_ACCESS_SAVE, DISPID_CLICK, CAccessRight::ClickBtnenhctrlAccessSave, VTS_NONE)
	ON_EVENT(CAccessRight, IDC_BTNENHCTRL_ACCESS_APPLY, DISPID_CLICK, CAccessRight::ClickBtnenhctrlAccessApply, VTS_NONE)
	ON_EVENT(CAccessRight, IDC_BTNENHCTRL_ACCESS_CANCEL, DISPID_CLICK, CAccessRight::ClickBtnenhctrlAccessCancel, VTS_NONE)
END_EVENTSINK_MAP()

void CAccessRight::ClickBtnenhctrlAccessSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SaveAccessRightPara(ACCESS_PATH);
}

void CAccessRight::ClickBtnenhctrlAccessApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Update(FALSE);
}

void CAccessRight::ClickBtnenhctrlAccessCancel()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ShowWindow(SW_HIDE);
}

void CAccessRight::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		LoadAccessRightPara(ACCESS_PATH);
		Update(TRUE);
	}
	else
	{

	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CAccessRight::LoadAccessRightPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	CString file_path, pat_path, extention, master_path;
	size_t getval = 0;
	
	file_path = path + _T("\\AccessRight.par");

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
		
		for(int i = 0; i < MAX_ACCESS_RIGHT; i++)
		{
			key.Empty();
			key.Format(_T("m_AccessRight[%d].bAutoRun"), i);
			GetPrivateProfileString(_T("ACCESS_RIGHT"), key, _T("0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_AccessRight[i].bAutoRun = atoi(ctemp);

			key.Empty();
			key.Format(_T("m_AccessRight[%d].bRecipe"), i);
			GetPrivateProfileString(_T("ACCESS_RIGHT"), key, _T("0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_AccessRight[i].bRecipe = atoi(ctemp);

			key.Empty();
			key.Format(_T("m_AccessRight[%d].bSystem"), i);
			GetPrivateProfileString(_T("ACCESS_RIGHT"), key, _T("0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_AccessRight[i].bSystem = atoi(ctemp);

			key.Empty();
			key.Format(_T("m_AccessRight[%d].bAlign"), i);
			GetPrivateProfileString(_T("ACCESS_RIGHT"), key, _T("0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_AccessRight[i].bAlign = atoi(ctemp);

			key.Empty();
			key.Format(_T("m_AccessRight[%d].bInline"), i);
			GetPrivateProfileString(_T("ACCESS_RIGHT"), key, _T("0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_AccessRight[i].bInline = atoi(ctemp);

			key.Empty();
			key.Format(_T("m_AccessRight[%d].bLog"), i);
			GetPrivateProfileString(_T("ACCESS_RIGHT"), key, _T("0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_AccessRight[i].bLog = atoi(ctemp);

			key.Empty();
			key.Format(_T("m_AccessRight[%d].bDefectServer"), i);
			GetPrivateProfileString(_T("ACCESS_RIGHT"), key, _T("0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_AccessRight[i].bDefectServer = atoi(ctemp);
		}
	}
	fd=0;
	
	return rslt;
}

BOOL CAccessRight::SaveAccessRightPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	CString file_path, pat_path, extention, master_path;
	size_t getval = 0;
	
	file_path = path + _T("\\AccessRight.par");

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

		for(int i = 0; i < MAX_ACCESS_RIGHT; i++)
		{
			key.Empty();
			key.Format(_T("m_AccessRight[%d].bAutoRun"), i);
			dat.Empty();
			dat.Format(_T("%d"), m_AccessRight[i].bAutoRun);
			WritePrivateProfileString(_T("ACCESS_RIGHT"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_AccessRight[%d].bRecipe"), i);
			dat.Empty();
			dat.Format(_T("%d"), m_AccessRight[i].bRecipe);
			WritePrivateProfileString(_T("ACCESS_RIGHT"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_AccessRight[%d].bSystem"), i);
			dat.Empty();
			dat.Format(_T("%d"), m_AccessRight[i].bSystem);
			WritePrivateProfileString(_T("ACCESS_RIGHT"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_AccessRight[%d].bAlign"), i);
			dat.Empty();
			dat.Format(_T("%d"), m_AccessRight[i].bAlign);
			WritePrivateProfileString(_T("ACCESS_RIGHT"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_AccessRight[%d].bInline"), i);
			dat.Empty();
			dat.Format(_T("%d"), m_AccessRight[i].bInline);
			WritePrivateProfileString(_T("ACCESS_RIGHT"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_AccessRight[%d].bLog"), i);
			dat.Empty();
			dat.Format(_T("%d"), m_AccessRight[i].bLog);
			WritePrivateProfileString(_T("ACCESS_RIGHT"), key, dat, file_path);

			key.Empty();
			key.Format(_T("m_AccessRight[%d].bDefectServer"), i);
			dat.Empty();
			dat.Format(_T("%d"), m_AccessRight[i].bDefectServer);
			WritePrivateProfileString(_T("ACCESS_RIGHT"), key, dat, file_path);
		}
	}
	fd = 0;

	return rslt;
}

void CAccessRight::Update(BOOL isUpdate)
{
	if(isUpdate)
	{
		m_btn_auto_run0.SetValue(m_AccessRight[0].bAutoRun);
		m_btn_recipe0.SetValue(m_AccessRight[0].bRecipe);
		m_btn_system0.SetValue(m_AccessRight[0].bSystem);
		m_btn_align0.SetValue(m_AccessRight[0].bAlign);
		m_btn_inline0.SetValue(m_AccessRight[0].bInline);
		m_btn_log0.SetValue(m_AccessRight[0].bLog);
		m_btn_defect_server0.SetValue(m_AccessRight[0].bDefectServer);

		m_btn_auto_run1.SetValue(m_AccessRight[1].bAutoRun);
		m_btn_recipe1.SetValue(m_AccessRight[1].bRecipe);
		m_btn_system1.SetValue(m_AccessRight[1].bSystem);
		m_btn_align1.SetValue(m_AccessRight[1].bAlign);
		m_btn_inline1.SetValue(m_AccessRight[1].bInline);
		m_btn_log1.SetValue(m_AccessRight[1].bLog);
		m_btn_defect_server1.SetValue(m_AccessRight[1].bDefectServer);

		m_btn_auto_run2.SetValue(m_AccessRight[2].bAutoRun);
		m_btn_recipe2.SetValue(m_AccessRight[2].bRecipe);
		m_btn_system2.SetValue(m_AccessRight[2].bSystem);
		m_btn_align2.SetValue(m_AccessRight[2].bAlign);
		m_btn_inline2.SetValue(m_AccessRight[2].bInline);
		m_btn_log2.SetValue(m_AccessRight[2].bLog);
		m_btn_defect_server2.SetValue(m_AccessRight[2].bDefectServer);

		m_btn_auto_run3.SetValue(m_AccessRight[3].bAutoRun);
		m_btn_recipe3.SetValue(m_AccessRight[3].bRecipe);
		m_btn_system3.SetValue(m_AccessRight[3].bSystem);
		m_btn_align3.SetValue(m_AccessRight[3].bAlign);
		m_btn_inline3.SetValue(m_AccessRight[3].bInline);
		m_btn_log3.SetValue(m_AccessRight[3].bLog);
		m_btn_defect_server3.SetValue(m_AccessRight[3].bDefectServer);
		UpdateData(TRUE);
	}
	else
	{
		UpdateData(FALSE);
		m_AccessRight[0].bAutoRun = m_btn_auto_run0.GetValue();
		m_AccessRight[0].bRecipe = m_btn_recipe0.GetValue();
		m_AccessRight[0].bSystem = m_btn_system0.GetValue();
		m_AccessRight[0].bAlign = m_btn_align0.GetValue();
		m_AccessRight[0].bInline = m_btn_inline0.GetValue();
		m_AccessRight[0].bLog = m_btn_log0.GetValue();
		m_AccessRight[0].bDefectServer = m_btn_defect_server0.GetValue();

		m_AccessRight[1].bAutoRun = m_btn_auto_run1.GetValue();
		m_AccessRight[1].bRecipe = m_btn_recipe1.GetValue();
		m_AccessRight[1].bSystem = m_btn_system1.GetValue();
		m_AccessRight[1].bAlign = m_btn_align1.GetValue();
		m_AccessRight[1].bInline = m_btn_inline1.GetValue();
		m_AccessRight[1].bLog = m_btn_log1.GetValue();
		m_AccessRight[1].bDefectServer = m_btn_defect_server1.GetValue();

		m_AccessRight[2].bAutoRun = m_btn_auto_run2.GetValue();
		m_AccessRight[2].bRecipe = m_btn_recipe2.GetValue();
		m_AccessRight[2].bSystem = m_btn_system2.GetValue();
		m_AccessRight[2].bAlign = m_btn_align2.GetValue();
		m_AccessRight[2].bInline = m_btn_inline2.GetValue();
		m_AccessRight[2].bLog = m_btn_log2.GetValue();
		m_AccessRight[2].bDefectServer = m_btn_defect_server2.GetValue();

		m_AccessRight[3].bAutoRun = m_btn_auto_run3.GetValue();
		m_AccessRight[3].bRecipe = m_btn_recipe3.GetValue();
		m_AccessRight[3].bSystem = m_btn_system3.GetValue();
		m_AccessRight[3].bAlign = m_btn_align3.GetValue();
		m_AccessRight[3].bInline = m_btn_inline3.GetValue();
		m_AccessRight[3].bLog = m_btn_log3.GetValue();
		m_AccessRight[3].bDefectServer = m_btn_defect_server3.GetValue();
	}
}

BOOL CAccessRight::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	LoadAccessRightPara(ACCESS_PATH);
	Update(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

typeAccessRight CAccessRight::GetAccessRight(int type)
{
	return m_AccessRight[type];
}