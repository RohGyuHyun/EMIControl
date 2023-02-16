// DialogLogWindow.cpp : 구현 파일입니다.
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





// CDialogLogWindow 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogLogWindow, CDialogEx)

CDialogLogWindow::CDialogLogWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogLogWindow::IDD, pParent)
{

}

CDialogLogWindow::~CDialogLogWindow()
{
}

void CDialogLogWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_list_log);
	DDX_Control(pDX, IDC_BTNENHCTRL_LOG_MAIN, m_btn_log_main);
	DDX_Control(pDX, IDC_BTNENHCTRL_LOG_ALIGN, m_btn_log_align);
	DDX_Control(pDX, IDC_BTNENHCTRL_LOG_ALARM, m_btn_log_alarm);
	DDX_Control(pDX, IDC_BTNENHCTRL_LOG_SYSTEM, m_btn_log_system);
	DDX_Control(pDX, IDC_BTNENHCTRL_LOG_MOTOR, m_btn_motor_log);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_LOG, m_btn_inline_log);
	DDX_Control(pDX, IDC_BTNENHCTRL_DEFECT_SERVER_LOG, m_btn_defect_log);
}


BEGIN_MESSAGE_MAP(CDialogLogWindow, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogLogWindow 메시지 처리기입니다.


BOOL CDialogLogWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_list_log.DeleteAllItems();

	// 리스트 스타일 설정
	m_list_log.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_list_log.InsertColumn(0, _T("Log"), LVCFMT_LEFT, 800, -1);
//	m_list_log.InsertColumn(1, _T("Before"), LVCFMT_CENTER, 109, -1);
//	m_list_log.InsertColumn(2, _T("After"), LVCFMT_CENTER, 109, -1);


	m_btn_log_main.SetValue(1);
	ClickBtnenhctrlLogMain();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_EVENTSINK_MAP(CDialogLogWindow, CDialogEx)
	ON_EVENT(CDialogLogWindow, IDC_BTNENHCTRL_LOG_MAIN, DISPID_CLICK, CDialogLogWindow::ClickBtnenhctrlLogMain, VTS_NONE)
	ON_EVENT(CDialogLogWindow, IDC_BTNENHCTRL_LOG_ALIGN, DISPID_CLICK, CDialogLogWindow::ClickBtnenhctrlLogAlign, VTS_NONE)
	ON_EVENT(CDialogLogWindow, IDC_BTNENHCTRL_LOG_ALARM, DISPID_CLICK, CDialogLogWindow::ClickBtnenhctrlLogAlarm, VTS_NONE)
	ON_EVENT(CDialogLogWindow, IDC_BTNENHCTRL_LOG_ALIGN_CANCLE, DISPID_CLICK, CDialogLogWindow::ClickBtnenhctrlLogAlignCancle, VTS_NONE)
	ON_EVENT(CDialogLogWindow, IDC_BTNENHCTRL_LOG_SYSTEM, DISPID_CLICK, CDialogLogWindow::ClickBtnenhctrlLogSystem, VTS_NONE)
	ON_EVENT(CDialogLogWindow, IDC_BTNENHCTRL_INLINE_LOG, DISPID_CLICK, CDialogLogWindow::ClickBtnenhctrlInlineLog, VTS_NONE)
	ON_EVENT(CDialogLogWindow, IDC_BTNENHCTRL_LOG_MOTOR, DISPID_CLICK, CDialogLogWindow::ClickBtnenhctrlLogMotor, VTS_NONE)
	ON_EVENT(CDialogLogWindow, IDC_BTNENHCTRL_DEFECT_SERVER_LOG, DISPID_CLICK, CDialogLogWindow::ClickBtnenhctrlDefectServerLog, VTS_NONE)
END_EVENTSINK_MAP()

BOOL CDialogLogWindow::ReadLog(int log_type)
{
	
	char *ps,*context;
	CString strName, strNum, temp;
//	CString readfilename;
	CString log,strLogType;
//	readfilename = filepath;
	char strFile[512];
	TCHAR* sData2 = new TCHAR[512];
	memset(&strFile, NULL, sizeof(char) * 512);
	int line_cnt = 0;

	CString strTime;

	char buffer[1024];//buffer[DAT_STR_LENG];

	switch(log_type)
	{
	case MAIN_LOG:
		strLogType = MAIN_LOG_FILE_NAME;
		break;
	case GLASS_LOG:
		strLogType = GLASS_LOG_FILE_NAME;
		break;
	case ALIGN_LOG:
		strLogType = ALIGN_LOG_FILE_NAME;
		break;
	case REVIEW_LOG:
		strLogType = REVIEW_LOG_FILE_NAME;
		break;
	case IO_LOG:
		strLogType = IO_LOG_FILE_NAME;
		break;
	case ALARM_LOG:
		strLogType = ALARM_LOG_FILE_NAME;
		break;
	case INLINE_LOG:
		strLogType = INLINE_LOG_FILE_NAME;
		break;
	case MOTOR_LOG:
		strLogType = MOTOR_LOG_FILE_NAME;
		break;
	case DEFECT_SERVER_LOG:
		strLogType = DEFECT_SERVER_LOG_FILE_NAME;
		break;
	}

	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);
	strTime.Format(_T("%04d%02d%02d"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
	
	temp.Format(_T("D:\\DATABASE\\LOG\\%s\\%s_%s"),strTime,strTime,strLogType);

//	sprintf_s(strFile, temp.GetLength() + strLogType.GetLength() + strTime.GetLength() + 2, "%S\\%S_%S", temp,strTime,strLogType);
	sprintf_s(strFile, temp.GetLength() + 1, "%S", temp);

	

	FILE *stream;
	
	if((stream = _fsopen( strFile, "rb" , _SH_DENYNO)) == NULL)
	{
		for(int i = 0; i < 3; i++)
		{
			stream = _fsopen( strFile, "rb" , _SH_DENYNO);
			if(stream != NULL)
			{
				break;
			}
			Sleep(100);
			if(i == 2)
			{
				return FALSE;
			}
		}
	}
	while (fgets(buffer, MAX_STRING_LINE, stream) != NULL)  // 텍스트파일을 한줄씩 읽습니다. 
	{
		ps = strchr(buffer, '\n');							// 제일 뒤쪽의 new line의 char을 찿아.
		if (ps != NULL) *ps = '\0';							// new line의 char을 null문자로 바꿉니다. 
	 
		if(1)//if(line_cnt != 0)
		{
			
			/*
			int cn = 0;

			ps = strtok_s(buffer, ",", &context);
			while (ps) {
				if (cn == 0) strName.Format(_T("%s"), ps);
				else strNum.Format(_T("%s"), ps);
				ps = strtok_s(NULL, ",", &context);
				cn++;
			}
			*/

			MultiByteToWideChar(CP_ACP, 0, (LPCSTR)buffer, -1, sData2, 512);
			log.Format(_T("%s"),sData2);
			m_list_log.InsertItem(line_cnt, log);

			TRACE("%s\r\n",buffer);
		}
		line_cnt++;
	}
	if(ps == NULL)
		delete ps;

	fclose(stream);
	 
	return TRUE;

}

void CDialogLogWindow::ClickBtnenhctrlLogMain()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_list_log.DeleteAllItems();
	m_btn_log_align.SetValue(0);
	m_btn_log_alarm.SetValue(0);
	m_btn_motor_log.SetValue(0);
	m_btn_inline_log.SetValue(0);
	m_btn_defect_log.SetValue(0);
	ReadLog(MAIN_LOG);
}
/*
CBtnenhctrl m_btn_motor_log;
	CBtnenhctrl m_btn_inline_log;
	CBtnenhctrl m_btn_defect_log;
	*/

void CDialogLogWindow::ClickBtnenhctrlLogAlign()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_list_log.DeleteAllItems();
	m_btn_log_main.SetValue(0);
	m_btn_log_alarm.SetValue(0);
	m_btn_motor_log.SetValue(0);
	m_btn_inline_log.SetValue(0);
	m_btn_defect_log.SetValue(0);
	ReadLog(ALIGN_LOG);
}


void CDialogLogWindow::ClickBtnenhctrlLogAlarm()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_list_log.DeleteAllItems();
	m_btn_log_align.SetValue(0);
	m_btn_log_main.SetValue(0);
	m_btn_motor_log.SetValue(0);
	m_btn_inline_log.SetValue(0);
	m_btn_defect_log.SetValue(0);
	ReadLog(ALARM_LOG);
}


void CDialogLogWindow::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow)
	{
		m_btn_log_main.SetValue(1);
		ClickBtnenhctrlLogMain();
	}
}


void CDialogLogWindow::ClickBtnenhctrlLogAlignCancle()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	this->ShowWindow(SW_HIDE);
}


void CDialogLogWindow::ClickBtnenhctrlLogSystem()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_list_log.DeleteAllItems();
	m_btn_log_align.SetValue(0);
	m_btn_log_main.SetValue(0);
	m_btn_log_alarm.SetValue(0);
	ReadLog(MAIN_LOG);
}


void CDialogLogWindow::ClickBtnenhctrlInlineLog()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_list_log.DeleteAllItems();
	m_btn_log_align.SetValue(0);
	m_btn_log_main.SetValue(0);
	m_btn_log_alarm.SetValue(0);
	m_btn_motor_log.SetValue(0);
	m_btn_defect_log.SetValue(0);
	ReadLog(INLINE_LOG);
}


void CDialogLogWindow::ClickBtnenhctrlLogMotor()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_list_log.DeleteAllItems();
	m_btn_log_align.SetValue(0);
	m_btn_log_main.SetValue(0);
	m_btn_log_alarm.SetValue(0);
	m_btn_inline_log.SetValue(0);
	m_btn_defect_log.SetValue(0);
	ReadLog(MOTOR_LOG);
}


void CDialogLogWindow::ClickBtnenhctrlDefectServerLog()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_list_log.DeleteAllItems();
	m_btn_log_align.SetValue(0);
	m_btn_log_main.SetValue(0);
	m_btn_log_alarm.SetValue(0);
	m_btn_motor_log.SetValue(0);
	m_btn_inline_log.SetValue(0);
	ReadLog(DEFECT_SERVER_LOG);
}
