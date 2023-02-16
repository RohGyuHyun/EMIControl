// Dialog/DialogLightCtrl.cpp : 구현 파일입니다.
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


// CDialogLightCtrl 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogLightCtrl, CDialogEx)

CDialogLightCtrl::CDialogLightCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogLightCtrl::IDD, pParent)
	, m_edit_light0(0)
	, m_edit_light1(0)
	, m_edit_light2(0)
	, m_edit_light3(0)
	, m_edit_light4(0)
{
	for(int i = 0; i < MAX_LIGHT; i++)
	{
		m_lLightTime[i] = 0;
		m_bLightTime[i] = FALSE;
	}
	m_bLightTimeSave = FALSE;
}

CDialogLightCtrl::~CDialogLightCtrl()
{
	SaveLightPara(_T(LIGHT_PATH));

	for(int i = 0; i < MAX_LIGHT; i++)
	{
		SetLightTime(i, FALSE);
	}
		
#ifdef MACHINE
	Light8kDisconnect();
	Light16kDisconnect();
#endif
}

void CDialogLightCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SLIDER_LIGHT0, m_slider_light0);
	DDX_Control(pDX, IDC_SLIDER_LIGHT1, m_slider_light1);
	DDX_Control(pDX, IDC_SLIDER_LIGHT2, m_slider_light2);
	DDX_Control(pDX, IDC_SLIDER_LIGHT3, m_slider_light3);
	DDX_Text(pDX, IDC_EDIT_LIGHT0, m_edit_light0);
	DDX_Text(pDX, IDC_EDIT_LIGHT1, m_edit_light1);
	DDX_Text(pDX, IDC_EDIT_LIGHT2, m_edit_light2);
	DDX_Text(pDX, IDC_EDIT_LIGHT3, m_edit_light3);
	DDX_Text(pDX, IDC_EDIT_LIGHT4, m_edit_light4);
	DDX_Control(pDX, IDC_SLIDER_LIGHT4, m_slider_light4);
	DDX_Control(pDX, IDC_BTNENHCTRL_TITLE_LIGHT8, m_btn_light_time_0);
	DDX_Control(pDX, IDC_BTNENHCTRL_TITLE_LIGHT7, m_btn_light_time_1);
	DDX_Control(pDX, IDC_BTNENHCTRL_TITLE_LIGHT5, m_btn_light_time_2);
	DDX_Control(pDX, IDC_BTNENHCTRL_TITLE_LIGHT6, m_btn_light_time_3);
	DDX_Control(pDX, IDC_BTNENHCTRL_TITLE_LIGHT9, m_btn_light_time_4);
	DDX_Control(pDX, IDC_BTNENHCTRL_LIGHT_SET0, m_btn_set_light_0);
	DDX_Control(pDX, IDC_BTNENHCTRL_LIGHT_SET1, m_btn_set_light_1);
	DDX_Control(pDX, IDC_BTNENHCTRL_LIGHT_SET2, m_btn_set_light_2);
	DDX_Control(pDX, IDC_BTNENHCTRL_LIGHT_SET3, m_btn_set_light_3);
	DDX_Control(pDX, IDC_BTNENHCTRL_LIGHT_SET4, m_btn_set_light_4);
}


BEGIN_MESSAGE_MAP(CDialogLightCtrl, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LIGHT0, &CDialogLightCtrl::OnNMCustomdrawSliderLight0)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LIGHT1, &CDialogLightCtrl::OnNMCustomdrawSliderLight1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LIGHT2, &CDialogLightCtrl::OnNMCustomdrawSliderLight2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LIGHT3, &CDialogLightCtrl::OnNMCustomdrawSliderLight3)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LIGHT4, &CDialogLightCtrl::OnNMCustomdrawSliderLight4)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogLightCtrl 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDialogLightCtrl, CDialogEx)
	ON_EVENT(CDialogLightCtrl, IDC_BTNENHCTRL_LIGHT_SET0, DISPID_CLICK, CDialogLightCtrl::ClickBtnenhctrlLightSet0, VTS_NONE)
	ON_EVENT(CDialogLightCtrl, IDC_BTNENHCTRL_LIGHT_SET1, DISPID_CLICK, CDialogLightCtrl::ClickBtnenhctrlLightSet1, VTS_NONE)
	ON_EVENT(CDialogLightCtrl, IDC_BTNENHCTRL_LIGHT_SET2, DISPID_CLICK, CDialogLightCtrl::ClickBtnenhctrlLightSet2, VTS_NONE)
	ON_EVENT(CDialogLightCtrl, IDC_BTNENHCTRL_LIGHT_SET3, DISPID_CLICK, CDialogLightCtrl::ClickBtnenhctrlLightSet3, VTS_NONE)
	ON_EVENT(CDialogLightCtrl, IDC_BTNENHCTRL_LIGHT_SET4, DISPID_CLICK, CDialogLightCtrl::ClickBtnenhctrlLightSet4, VTS_NONE)
	ON_EVENT(CDialogLightCtrl, IDC_BTNENHCTRL_IPS_APPLY, DISPID_CLICK, CDialogLightCtrl::ClickBtnenhctrlIpsApply, VTS_NONE)
	ON_EVENT(CDialogLightCtrl, IDC_BTNENHCTRL_IPS_SAVE, DISPID_CLICK, CDialogLightCtrl::ClickBtnenhctrlIpsSave, VTS_NONE)
	ON_EVENT(CDialogLightCtrl, IDC_BTNENHCTRL_LIGHT_TIME_RESET_0, DISPID_CLICK, CDialogLightCtrl::ClickBtnenhctrlLightTimeReset0, VTS_NONE)
	ON_EVENT(CDialogLightCtrl, IDC_BTNENHCTRL_LIGHT_TIME_RESET_1, DISPID_CLICK, CDialogLightCtrl::ClickBtnenhctrlLightTimeReset1, VTS_NONE)
	ON_EVENT(CDialogLightCtrl, IDC_BTNENHCTRL_LIGHT_TIME_RESET_2, DISPID_CLICK, CDialogLightCtrl::ClickBtnenhctrlLightTimeReset2, VTS_NONE)
	ON_EVENT(CDialogLightCtrl, IDC_BTNENHCTRL_LIGHT_TIME_RESET_3, DISPID_CLICK, CDialogLightCtrl::ClickBtnenhctrlLightTimeReset3, VTS_NONE)
	ON_EVENT(CDialogLightCtrl, IDC_BTNENHCTRL_LIGHT_TIME_RESET_4, DISPID_CLICK, CDialogLightCtrl::ClickBtnenhctrlLightTimeReset4, VTS_NONE)
END_EVENTSINK_MAP()




BOOL CDialogLightCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	LoadLightPara(_T(LIGHT_PATH));

	//20160717 ngh
	LoadLightTime(_T(LIGHT_PATH));
	for(int i = 0; i < MAX_LIGHT; i++)
	{
		CString strValue;
		strValue.Format(_T("%d"), m_lLightTime[i]);
		switch(i)
		{
		case 0:
			m_btn_light_time_0.SetWindowTextW(_T("ALIGN LIGHT #1  ") + strValue + _T("  sec"));
			break;
		case 1:
			m_btn_light_time_1.SetWindowTextW(_T("ALIGN LIGHT #2  ") + strValue + _T("  sec"));
			break;
		case 2:
			m_btn_light_time_2.SetWindowTextW(_T("16K LIGHT  ") + strValue + _T("  sec"));
			break;
		case 3:
			m_btn_light_time_3.SetWindowTextW(_T("8K LIGHT  ") + strValue + _T("  sec"));
			break;
		case 4:
			m_btn_light_time_4.SetWindowTextW(_T("REVIEW LIGHT  ") + strValue + _T(" sec"));
			break;
		}
	}

	m_slider_light0.SetRange(0,255);  //align 0
	if(0 > m_edit_light0)
	{
		m_edit_light0 = 0;
	}
	if(255 < m_edit_light0)
	{
		m_edit_light0 = 255;
	}
	m_slider_light0.SetPos(m_edit_light0);

	m_slider_light1.SetRange(0,255);  //align 1
	if(0 > m_edit_light1)
	{
		m_edit_light1 = 0;
	}
	if(255 < m_edit_light1)
	{
		m_edit_light1 = 255;
	}
	m_slider_light1.SetPos(m_edit_light1);

	m_slider_light2.SetRange(0,255);  //16k
	if(0 > m_edit_light2)
	{
		m_edit_light2 = 0;
	}
	if(255 < m_edit_light2)
	{
		m_edit_light2 = 255;
	}
	m_slider_light2.SetPos(m_edit_light2);

	m_slider_light3.SetRange(0,255);  //8k
	if(0 > m_edit_light3)
	{
		m_edit_light3 = 0;
	}
	if(255 < m_edit_light3)
	{
		m_edit_light3 = 255;
	}
	m_slider_light3.SetPos(m_edit_light3);

	m_slider_light4.SetRange(0,100);  //AF Review Led
	if(0 > m_edit_light4)
	{
		m_edit_light4 = 0;
	}
	if(100 < m_edit_light4)
	{
		m_edit_light4 = 100;
	}
	m_slider_light4.SetPos(m_edit_light4);
#ifdef MACHINE
	OpenLight();

	SetTimer(0, 500, NULL);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogLightCtrl::OpenLight()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	m_cLight_Align.Open(2,9600,m_cLight_Align.NoParity,8,m_cLight_Align.OneStopBit,m_cLight_Align.NoFlowControl,0);  //align 0 , 1 open
	if(!m_cLight_Align.IsOpen())
	{
		AfxMessageBox(_T("Error : open serial align light!"));
		m_strlog.Format(_T("Error : open serial align light!"));
		pdlg->WriteLog(LIGHT_LOG,m_strlog);
	}
	m_cLight_16k.Open(7,38400, m_cLight_16k.NoParity,8, m_cLight_16k.TwoStopBits, m_cLight_16k.NoFlowControl,0);  //16k open
	if(!m_cLight_16k.IsOpen())
	{
		AfxMessageBox(_T("error open serial 16k light!"));
		m_strlog.Format(_T("error open serial 16k light!"));
		pdlg->WriteLog(LIGHT_LOG,m_strlog);
	}	
	m_cLight_8k.Open(6,19200,m_cLight_8k.NoParity,8,m_cLight_8k.OneStopBit,m_cLight_8k.NoFlowControl,0);  //8k open

	if(!m_cLight_8k.IsOpen())
	{
		AfxMessageBox(_T("Error : open serial 8k light!"));
		m_strlog.Format(_T("Error : open serial 8k light!"));
		pdlg->WriteLog(LIGHT_LOG,m_strlog);
	}
}

BOOL CDialogLightCtrl::SaveLightPara(CString path)								
{
	
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\LightCtrl.par");

	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	m_strlog.Format(_T("SaveLightPara() Start"));
	pdlg->WriteLog(LIGHT_LOG,m_strlog);
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
		key.Format(_T("m_edit_light0"));
		dat.Empty();
		dat.Format(_T("%d"), m_edit_light0);
		WritePrivateProfileString(_T("Light CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_light1"));
		dat.Empty();
		dat.Format(_T("%d"), m_edit_light1);
		WritePrivateProfileString(_T("Light CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_light2"));
		dat.Empty();
		dat.Format(_T("%d"), m_edit_light2);
		WritePrivateProfileString(_T("Light CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_light3"));
		dat.Empty();
		dat.Format(_T("%d"), m_edit_light3);
		WritePrivateProfileString(_T("Light CTRL"), key, dat, file_path);
	}
	fd=0;
	
	
	return rslt;
}

BOOL CDialogLightCtrl::LoadLightPara(CString path)
{
	
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\LightCtrl.par");
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	m_strlog.Format(_T("LoadLightPara() Start"));
	pdlg->WriteLog(LIGHT_LOG,m_strlog);
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
		key.Format(_T("m_edit_light0"));
		GetPrivateProfileString(_T("Light CTRL"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_light0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_light1"));
		GetPrivateProfileString(_T("Light CTRL"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_light1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_light2"));
		GetPrivateProfileString(_T("Light CTRL"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_light2 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_light3"));
		GetPrivateProfileString(_T("Light CTRL"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_light3 = atof(ctemp);
	}

	fd=0;
	
	return rslt;
}


void CDialogLightCtrl::OnNMCustomdrawSliderLight0(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	UpdateData(TRUE);
	m_edit_light0 = m_slider_light0.GetPos();
	BYTE *packet = new BYTE [512];

	sprintf((char *)packet, "L1%03d%c%c",m_edit_light0, 13, 10);
#ifdef MACHINE
	m_cLight_Align.Write(packet, sizeof(packet));
#endif

	//20160717 ngh
	if(m_edit_light0 > 0)
	{
		SetLightTime(0, TRUE);
	}
	else
	{
		SetLightTime(0, FALSE);
	}

	m_strlog.Format(_T("OnNMCustomdrawSliderLight0() : %d "),m_edit_light0);
	pdlg->WriteLog(LIGHT_LOG,m_strlog);
	UpdateData(FALSE);
	*pResult = 0;
}


void CDialogLightCtrl::OnNMCustomdrawSliderLight1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	m_edit_light1 = m_slider_light1.GetPos();
	BYTE *packet = new BYTE [512];

	sprintf((char *)packet, "L2%03d%c%c",m_edit_light1, 13, 10);
#ifdef MACHINE
	m_cLight_Align.Write(packet, sizeof(packet));
#endif

	//20160717 ngh
	if(m_edit_light1 > 0)
	{
		SetLightTime(1, TRUE);
	}
	else
	{
		SetLightTime(1, FALSE);
	}

	m_strlog.Format(_T("OnNMCustomdrawSliderLight1() : %d "),m_edit_light1);
	pdlg->WriteLog(LIGHT_LOG,m_strlog);
	

	UpdateData(FALSE);
	*pResult = 0;
}


void CDialogLightCtrl::OnNMCustomdrawSliderLight2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	m_edit_light2 = m_slider_light2.GetPos();
	BYTE *packet = new BYTE [512];

	sprintf((char *)packet, "%cWD8%03d%c",2,m_edit_light2,3);
#ifdef MACHINE
	m_cLight_16k.Write(packet, sizeof(packet));
#endif

	//20160717 ngh
	if(m_edit_light2 > 0)
	{
		SetLightTime(2, TRUE);
	}
	else
	{
		SetLightTime(2, FALSE);
	}

	m_strlog.Format(_T("OnNMCustomdrawSliderLight2() : %d "),m_edit_light2);
	pdlg->WriteLog(LIGHT_LOG,m_strlog);
	UpdateData(FALSE);
	
	*pResult = 0;
}


void CDialogLightCtrl::OnNMCustomdrawSliderLight3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	m_edit_light3 = m_slider_light3.GetPos();
	BYTE *packet = new BYTE [512];

	sprintf((char *)packet, "#A1%03d&",m_edit_light3);	
#ifdef MACHINE
	m_cLight_8k.Write(packet, sizeof(packet));
#endif

	//20160717 ngh
	if(m_edit_light3 > 0)
	{
		SetLightTime(3, TRUE);
	}
	else
	{
		SetLightTime(3, FALSE);
	}

	m_strlog.Format(_T("OnNMCustomdrawSliderLight3() : %d "),m_edit_light3);
	pdlg->WriteLog(LIGHT_LOG,m_strlog);
	UpdateData(FALSE);
	*pResult = 0;
}

//20160717 ngh
void CDialogLightCtrl::SetLightTime(int light_idx, BOOL start_light_time)
{
	if(start_light_time)
	{
		if(!m_bLightTime[light_idx])
		{
			m_bLightTime[light_idx] = TRUE;
			SetTimer(light_idx + 1, 1000, NULL);
		}
	}
	else
	{
		m_bLightTime[light_idx] = FALSE;
		KillTimer(light_idx + 1);
		SaveLightTime(_T(LIGHT_PATH));
	}
}

void CDialogLightCtrl::ClickBtnenhctrlLightSet0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	if((m_edit_light0 < 0) || (m_edit_light0 > 255))
	{
		::AfxMessageBox(_T("Light value 0~255"));
		m_edit_light0 = 0;
	}
	BYTE *packet = new BYTE [512];

	sprintf((char *)packet, "L1%03d%c%c",m_edit_light0, 13, 10);
#ifdef MACHINE
	m_cLight_Align.Write(packet, sizeof(packet));
#endif

	//20160717 ngh
	if(m_edit_light0 > 0)
	{
		SetLightTime(0, TRUE);
	}
	else
	{
		SetLightTime(0, FALSE);
	}
	
	m_strlog.Format(_T("ClickBtnenhctrlLightSet0() : %d "),m_edit_light0);
	pdlg->WriteLog(LIGHT_LOG,m_strlog);
	m_slider_light0.SetPos(m_edit_light0);
}

void CDialogLightCtrl::ClickBtnenhctrlLightSet1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	if((m_edit_light0 < 0) || (m_edit_light0 > 255))
	{
		::AfxMessageBox(_T("Light value 0~255"));
		m_edit_light0 = 0;
	}
	BYTE *packet = new BYTE [512];

	sprintf((char *)packet, "L2%03d%c%c",m_edit_light1, 13, 10);
#ifdef MACHINE
	m_cLight_Align.Write(packet, sizeof(packet));
#endif
	//20160717 ngh
	if(m_edit_light1 > 0)
	{
		SetLightTime(1, TRUE);
	}
	else
	{
		SetLightTime(1, FALSE);
	}
	m_strlog.Format(_T("ClickBtnenhctrlLightSet1() : %d "),m_edit_light1);
	pdlg->WriteLog(LIGHT_LOG,m_strlog);
	m_slider_light1.SetPos(m_edit_light1);
}


void CDialogLightCtrl::ClickBtnenhctrlLightSet2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	UpdateData(TRUE);
	if((m_edit_light0 < 0) || (m_edit_light0 > 255))
	{
		::AfxMessageBox(_T("Light value 0~255"));
		m_edit_light0 = 0;
	}
	BYTE *packet = new BYTE [512];
	
	sprintf((char *)packet, "%cWD8%03d%c",2,m_edit_light2,3);
#ifdef MACHINE
	m_cLight_16k.Write(packet, sizeof(packet));
#endif
	//20160717 ngh
	if(m_edit_light2 > 0)
	{
		SetLightTime(2, TRUE);
	}
	else
	{
		SetLightTime(2, FALSE);
	}
	m_strlog.Format(_T("ClickBtnenhctrlLightSet2() : %d "),m_edit_light2);
	pdlg->WriteLog(LIGHT_LOG,m_strlog);
	m_slider_light2.SetPos(m_edit_light2);
}


void CDialogLightCtrl::ClickBtnenhctrlLightSet3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	if((m_edit_light0 < 0) || (m_edit_light0 > 255))
	{ 
		::AfxMessageBox(_T("Light value 0~255"));
		m_edit_light0 = 0;
	}

	BYTE *packet = new BYTE [512];

	sprintf((char *)packet, "#A1%03d&",m_edit_light3);		
#ifdef MACHINE
	m_cLight_8k.Write(packet, sizeof(packet));
#endif
	//20160717 ngh
	if(m_edit_light3 > 0)
	{
		SetLightTime(3, TRUE);
	}
	else
	{
		SetLightTime(3, FALSE);
	}
	m_strlog.Format(_T("ClickBtnenhctrlLightSet3() : %d "),m_edit_light3);
	pdlg->WriteLog(LIGHT_LOG,m_strlog);
	m_slider_light3.SetPos(m_edit_light3);
}

void CDialogLightCtrl::Set8KLight(int val)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	BYTE *packet = new BYTE [512];

	sprintf((char *)packet, "#A1%03d&",val);
#ifdef MACHINE
	m_cLight_8k.Write(packet, sizeof(packet));
#endif

	//20160717 ngh
	if(val > 0)
	{
		SetLightTime(3, TRUE);
	}
	else
	{
		SetLightTime(3, FALSE);
	}

	m_strlog.Format(_T("Set8KLight() : %d "),val);
	pdlg->WriteLog(LIGHT_LOG,m_strlog);

	delete packet;
}

void CDialogLightCtrl::Set16KLight(int val)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	BYTE *packet = new BYTE [512];

	sprintf((char *)packet, "%cWD8%03d%c",2,val,3);
#ifdef MACHINE
	m_cLight_16k.Write(packet, sizeof(packet));
#endif

	//20160717 ngh
	if(val > 0)
	{
		SetLightTime(2, TRUE);
	}
	else
	{
		SetLightTime(2, FALSE);
	}

	m_strlog.Format(_T("Set16KLight() : %d "),val);
	pdlg->WriteLog(LIGHT_LOG,m_strlog);

	delete packet;
}

void CDialogLightCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	BYTE *packet = new BYTE [512];
	CString strValue;
	int light_idx = 0;
	int nHour = 0, nMinute = 0, nSecound = 0;
	switch(nIDEvent)
	{
	case 0:
		sprintf((char *)packet, "%cWDD1%c",2,3);
		m_cLight_16k.Write(packet, sizeof(packet));
		
		KillTimer(nIDEvent);
		break;
	//20160717 ngh
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		light_idx = nIDEvent - 1;
		m_lLightTime[light_idx]++;
		m_bLightTime[light_idx] = TRUE;

		SaveLightTime(_T(LIGHT_PATH));

		nSecound = (m_lLightTime[light_idx] % 60);;
		nMinute = ((m_lLightTime[light_idx] / 60) % 60);
		nHour = ((m_lLightTime[light_idx] / 60) / 60);

		strValue.Format(_T("%d : %02d : %02d"), nHour, nMinute, nSecound);
		switch(light_idx)
		{
		case 0:
			m_btn_light_time_0.SetWindowTextW(_T("ALIGN LIGHT #1  ") + strValue);// + _T("  sec"));
			break;
		case 1:
			m_btn_light_time_1.SetWindowTextW(_T("ALIGN LIGHT #2  ") + strValue);// + _T("  sec"));
			break;
		case 2:
			m_btn_light_time_2.SetWindowTextW(_T("16K LIGHT  ") + strValue);// + _T("  sec"));
			break;
		case 3:
			m_btn_light_time_3.SetWindowTextW(_T("8K LIGHT  ") + strValue);// + _T("  sec"));
			break;
		case 4:
			m_btn_light_time_4.SetWindowTextW(_T("REVIEW LIGHT  ") + strValue);// + _T(" sec"));
			break;
		}
		break;
	default:
		m_strlog.Format(_T("Error : invalid index(CDialogLightCtrl::OnTimer)!"));
		pdlg->WriteLog(LIGHT_LOG,m_strlog);
		AfxMessageBox(m_strlog);
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CDialogLightCtrl::LoadLightTime(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\") + LIGHT_TIME_FILE;
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	m_strlog.Format(_T("LoadLightTiem() Start"));
	pdlg->WriteLog(LIGHT_LOG,m_strlog);
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
		
		for(int i = 0; i < MAX_LIGHT; i++)
		{
			key.Empty();
			key.Format(_T("m_lLightTime_%d"), i);
			GetPrivateProfileString(_T("Light Time"), key, _T("0"), temp, sizeof(temp), file_path);
			dat = temp;
			wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
			m_lLightTime[i] = atoi(ctemp);
		}
	}

	fd=0;
	
	m_strlog.Format(_T("LoadLightTiem() End"));
	pdlg->WriteLog(LIGHT_LOG,m_strlog);

	return rslt;
}

BOOL CDialogLightCtrl::SaveLightTime(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\") + LIGHT_TIME_FILE;

	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	//m_strlog.Format(_T("SaveLightTime() Start"));
	//pdlg->WriteLog(LIGHT_LOG,m_strlog);
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

		for(int i = 0; i < MAX_LIGHT; i++)
		{
			key.Empty();
			key.Format(_T("m_lLightTime_%d"), i);
			dat.Empty();
			dat.Format(_T("%d"), m_lLightTime[i]);
			WritePrivateProfileString(_T("Light Time"), key, dat, file_path);
		}
	}
	fd=0;

	//m_strlog.Format(_T("SaveLightTime() End"));
	//pdlg->WriteLog(LIGHT_LOG,m_strlog);
	
	return rslt;
}

void CDialogLightCtrl::SetAlignLight(int ch, int bright)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
#ifdef MACHINE
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	BYTE *packet = new BYTE [512];

	switch(ch)
	{
	case 0:
		sprintf((char *)packet, "L1%03d%c%c", bright, 13, 10);
		break;
	case 1:
		sprintf((char *)packet, "L2%03d%c%c", bright, 13, 10);
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogLightCtrl::SetAlignLight)!"));
		m_strlog.Format(_T("Error : invalid index(CDialogLightCtrl::SetAlignLight)!"));
		pdlg->WriteLog(LIGHT_LOG,m_strlog);
		break;
	}

	m_cLight_Align.Write(packet, sizeof(packet));
#endif
}


void CDialogLightCtrl::OnNMCustomdrawSliderLight4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	unsigned short LedPwm = 0;

	m_edit_light4 = m_slider_light4.GetPos();
	
	LedPwm = m_edit_light4;
#ifdef MACHINE
	pdlg->m_AFCtrl->SetReviewLed(LedPwm);
#endif

	//20160717 ngh
	if(m_edit_light4 > 0)
	{
		SetLightTime(4, TRUE);
	}
	else
	{
		SetLightTime(4, FALSE);
	}

	UpdateData(FALSE);

	*pResult = 0;
}


void CDialogLightCtrl::ClickBtnenhctrlLightSet4()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);


	if((m_edit_light4 < 0) || (m_edit_light4 > 100))
	{ 
		::AfxMessageBox(_T("Light value 0~100"));
		m_edit_light4 = 0;
	}

	//20160717 ngh
	if(m_edit_light4 > 0)
	{
		SetLightTime(4, TRUE);
	}
	else
	{
		SetLightTime(4, FALSE);
	}

	pdlg->m_AFCtrl->SetReviewLed((unsigned short) m_edit_light4);

	m_slider_light4.SetPos(m_edit_light4);
}

void CDialogLightCtrl::Light8kDisconnect()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	BYTE *packet = new BYTE [512];
	

	sprintf((char *)packet, "#A1000&");
	m_cLight_8k.Write(packet, sizeof(packet));
	m_strlog.Format(_T("Light8kDisconnect()"));
	pdlg->WriteLog(LIGHT_LOG,m_strlog);

}
void CDialogLightCtrl::Light16kDisconnect()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	BYTE *packet = new BYTE [512];
	

	sprintf((char *)packet, "%cWDD0%c",2,3);    // STXWDD0ETX -> WDD0 off - WDD1 On - WD8255 조명값255
	m_cLight_16k.Write(packet, sizeof(packet));
	m_strlog.Format(_T("Light16kDisconnect()"));
	pdlg->WriteLog(LIGHT_LOG,m_strlog);
}

BOOL CDialogLightCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogLightCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow)
	{
		LoadLightPara(_T(LIGHT_PATH));
		LoadLightTime(_T(LIGHT_PATH));

		m_slider_light0.SetRange(0,255);  //align 0
		if(0 > m_edit_light0)
		{
			m_edit_light0 = 0;
		}
		if(255 < m_edit_light0)
		{
			m_edit_light0 = 255;
		}
		m_slider_light0.SetPos(m_edit_light0);

		m_slider_light1.SetRange(0,255);  //align 1
		if(0 > m_edit_light1)
		{
			m_edit_light1 = 0;
		}
		if(255 < m_edit_light1)
		{
			m_edit_light1 = 255;
		}
		m_slider_light1.SetPos(m_edit_light1);

		m_slider_light2.SetRange(0,255);  //16k
		if(0 > m_edit_light2)
		{
			m_edit_light2 = 0;
		}
		if(255 < m_edit_light2)
		{
			m_edit_light2 = 255;
		}
		m_slider_light2.SetPos(m_edit_light2);

		m_slider_light3.SetRange(0,255);  //8k
		if(0 > m_edit_light3)
		{
			m_edit_light3 = 0;
		}
		if(255 < m_edit_light3)
		{
			m_edit_light3 = 255;
		}
		m_slider_light3.SetPos(m_edit_light3);

		m_slider_light4.SetRange(0,100);  //AF Review Led
		if(0 > m_edit_light4)
		{
			m_edit_light4 = 0;
		}
		if(100 < m_edit_light4)
		{
			m_edit_light4 = 100;
		}
		m_slider_light4.SetPos(m_edit_light4);

		for(int i = 0; i < MAX_LIGHT; i++)
		{
			SetLightTime(i, TRUE);
		}

	}
	else
	{
		for(int i = 0; i < MAX_LIGHT; i++)
		{
			SetLightTime(i, FALSE);
		}
	}
}


void CDialogLightCtrl::ClickBtnenhctrlIpsApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
}


void CDialogLightCtrl::ClickBtnenhctrlIpsSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	SaveLightPara(_T(LIGHT_PATH));
}


void CDialogLightCtrl::ClickBtnenhctrlLightTimeReset0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_lLightTime[0] = 0;
}


void CDialogLightCtrl::ClickBtnenhctrlLightTimeReset1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_lLightTime[1] = 0;
}


void CDialogLightCtrl::ClickBtnenhctrlLightTimeReset2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_lLightTime[2] = 0;
}


void CDialogLightCtrl::ClickBtnenhctrlLightTimeReset3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_lLightTime[3] = 0;
}


void CDialogLightCtrl::ClickBtnenhctrlLightTimeReset4()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_lLightTime[4] = 0;
}
