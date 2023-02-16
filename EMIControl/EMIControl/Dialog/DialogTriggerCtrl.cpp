// D:\working\emi\EMIControl_add_Trigger123\EMIControl_add_Trigger123\EMIControl_add_Trigger123\Dialog\DialogTriggerCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EMIControl.h"
#include "Trigger_recipe.h"
#include "Dialog/DialogTriggerCtrl.h"
#include "afxdialogex.h"

#include "EMIControlDlg.h"

// CDialogTriggerCtrl 대화 상자입니다.
CString sRecipeFileName;
using namespace RECIPE_NAMESPACE;
IMPLEMENT_DYNAMIC(CDialogTriggerCtrl, CDialogEx)

CDialogTriggerCtrl::CDialogTriggerCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogTriggerCtrl::IDD, pParent)
{
	m_bTriggerConnection = FALSE;

	m_edit_trigger_encoder_resolution = 0.;

	m_edit_trigger_start_pos0 = 0.;
	m_edit_trigger_end_pos0 = 0.;
	m_edit_trigger_pitch0 = 0.;
	m_edit_trigger_pulse_width0 = 0.;
	m_edit_trigger_delay0 = 0.;

	m_edit_trigger_start_pos1 = 0.;
	m_edit_trigger_end_pos1 = 0.;
	m_edit_trigger_pitch1 = 0.;
	m_edit_trigger_pulse_width1 = 0.;
	m_edit_trigger_delay1 = 0.;

	m_edit_trigger_start_pos2 = 0.;
	m_edit_trigger_end_pos2 = 0.;
	m_edit_trigger_pitch2 = 0.;
	m_edit_trigger_pulse_width2 = 0.;
	m_edit_trigger_delay2 = 0.;

	m_edit_trigger_start_pos3 = 0.;
	m_edit_trigger_end_pos3 = 0.;
	m_edit_trigger_pitch3 = 0.;
	m_edit_trigger_pulse_width3 = 0.;
	m_edit_trigger_delay3 = 0.;

	m_edit_trigger_mode0.Empty();
	m_edit_trigger_trigge_count0 = 0;
	m_edit_trigger_encoder_count0 = 0;
	m_edit_trigger_encoder_pos0 = 0;

	m_edit_trigger_mode1.Empty();
	m_edit_trigger_trigge_count1 = 0;
	m_edit_trigger_encoder_count1 = 0;
	m_edit_trigger_encoder_pos1 = 0;

	m_edit_trigger_mode2.Empty();
	m_edit_trigger_trigge_count2 = 0;
	m_edit_trigger_encoder_count2 = 0;
	m_edit_trigger_encoder_pos2 = 0;

	m_edit_trigger_mode3.Empty();
	m_edit_trigger_trigge_count3 = 0;
	m_edit_trigger_encoder_count3 = 0;
	m_edit_trigger_encoder_pos3 = 0;
	
	m_bTriggerThreadEnd = FALSE;
	m_bTriggerThreadStart = FALSE;
	
	memset(m_dTriggerPara, NULL,sizeof(double) * 2 * 25);
}

CDialogTriggerCtrl::~CDialogTriggerCtrl()
{
	m_bTriggerThreadStart = FALSE;
	m_bTriggerThreadEnd = TRUE;
#ifdef MACHINE		
	m_CommThread.ClosePort();
#endif
	ClickBtnenhctrlTriggerSave();
}

void CDialogTriggerCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNENHCTRL_TRIGGER_CONNECTION, m_btn_trigger_connection);
	DDX_Control(pDX, IDC_COMBO_TRIGGER_SELECT_CAM, m_combo_trigger_select_cam);
	DDX_Control(pDX, IDC_COMBO_TRIGGER_PORT, m_combo_trigger_port);
	DDX_Control(pDX, IDC_COMBO_TRIGGER_BAUD_RATE, m_combo_trigger_baud_rate);
	DDX_Control(pDX, IDC_COMBO_TRIGGER_DATA_BIT, m_combo_trigger_data_bit);
	DDX_Control(pDX, IDC_COMBO_TRIGGER_STOP_BIT, m_combo_trigger_stop_bit);
	DDX_Control(pDX, IDC_COMBO_TRIGGER_PARITY, m_combo_trigger_parity);
	DDX_Control(pDX, IDC_COMBO_TRIGGER_MODE_CH0, m_combo_trigger_mode_ch0);
	DDX_Control(pDX, IDC_COMBO_TRIGGER_MODE_CH1, m_combo_trigger_mode_ch1);
	DDX_Control(pDX, IDC_COMBO_TRIGGER_MODE_CH2, m_combo_trigger_mode_ch2);
	DDX_Control(pDX, IDC_COMBO_TRIGGER_MODE_CH3, m_combo_trigger_mode_ch3);

	DDX_Text(pDX, IDC_EDIT_TRIGGER_ENCODER_RESOLUTION, m_edit_trigger_encoder_resolution);

	DDX_Text(pDX, IDC_EDIT_TRIGGER_START_POS_0, m_edit_trigger_start_pos0);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_END_POS_0, m_edit_trigger_end_pos0);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_PITCH_0, m_edit_trigger_pitch0);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_PULSE_WIDTH_0, m_edit_trigger_pulse_width0);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_DELAY_0, m_edit_trigger_delay0);

	DDX_Text(pDX, IDC_EDIT_TRIGGER_START_POS_1, m_edit_trigger_start_pos1);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_END_POS_1, m_edit_trigger_end_pos1);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_PITCH_1, m_edit_trigger_pitch1);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_PULSE_WIDTH_1, m_edit_trigger_pulse_width1);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_DELAY_1, m_edit_trigger_delay1);

	DDX_Text(pDX, IDC_EDIT_TRIGGER_START_POS_2, m_edit_trigger_start_pos2);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_END_POS_2, m_edit_trigger_end_pos2);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_PITCH_2, m_edit_trigger_pitch2);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_PULSE_WIDTH_2, m_edit_trigger_pulse_width2);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_DELAY_2, m_edit_trigger_delay2);

	DDX_Text(pDX, IDC_EDIT_TRIGGER_START_POS_3, m_edit_trigger_start_pos3);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_END_POS_3, m_edit_trigger_end_pos3);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_PITCH_3, m_edit_trigger_pitch3);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_PULSE_WIDTH_3, m_edit_trigger_pulse_width3);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_DELAY_3, m_edit_trigger_delay3);

	DDX_Control(pDX, IDC_BTNENHCTRL_TRIGGER_STATUS_ENABLE0, m_btn_trigger_status_enable0);
	DDX_Control(pDX, IDC_BTNENHCTRL_TRIGGER_STATUS_ENABLE1, m_btn_trigger_status_enable1);
	DDX_Control(pDX, IDC_BTNENHCTRL_TRIGGER_STATUS_ENABLE2, m_btn_trigger_status_enable2);
	DDX_Control(pDX, IDC_BTNENHCTRL_TRIGGER_STATUS_ENABLE3, m_btn_trigger_status_enable3);

	DDX_Text(pDX, IDC_EDIT_TRIGGER_SELECTION_MODE0, m_edit_trigger_mode0);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_TRIGGER_COUNT0, m_edit_trigger_trigge_count0);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_ENCODER_COUNT0, m_edit_trigger_encoder_count0);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_ENCODR_POSITION0, m_edit_trigger_encoder_pos0);

	DDX_Text(pDX, IDC_EDIT_TRIGGER_SELECTION_MODE1, m_edit_trigger_mode1);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_TRIGGER_COUNT1, m_edit_trigger_trigge_count1);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_ENCODER_COUNT1, m_edit_trigger_encoder_count1);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_ENCODR_POSITION1, m_edit_trigger_encoder_pos1);

	DDX_Text(pDX, IDC_EDIT_TRIGGER_SELECTION_MODE2, m_edit_trigger_mode2);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_TRIGGER_COUNT2, m_edit_trigger_trigge_count2);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_ENCODER_COUNT2, m_edit_trigger_encoder_count2);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_ENCODR_POSITION2, m_edit_trigger_encoder_pos2);

	DDX_Text(pDX, IDC_EDIT_TRIGGER_SELECTION_MODE3, m_edit_trigger_mode3);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_TRIGGER_COUNT3, m_edit_trigger_trigge_count3);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_ENCODER_COUNT3, m_edit_trigger_encoder_count3);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_ENCODR_POSITION3, m_edit_trigger_encoder_pos3);
}


BEGIN_MESSAGE_MAP(CDialogTriggerCtrl, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_COMM_READ , OnCommunication) //추가
	ON_CBN_CLOSEUP(IDC_COMBO_TRIGGER_SELECT_CAM, &CDialogTriggerCtrl::OnCbnCloseupComboTriggerSelectCam)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_SELECT_CAM, &CDialogTriggerCtrl::OnCbnSelchangeComboTriggerSelectCam)
	ON_MESSAGE(USER_MSG_TRIGGER, OnTriggerMessage)
END_MESSAGE_MAP()


// CDialogTriggerCtrl 메시지 처리기입니다.
HWND hCommWnd;

BOOL CDialogTriggerCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	hCommWnd = m_hWnd;

	CString recipe = get_recipe(0);
	sRecipeFileName = recipe;
	RecipeFileToList(recipe);
	RecipeFileToSystemDialog(0);
	
#ifdef MACHINE
	TriggerOpen();
#endif	
	SetCamTriggerRcipe(1);
	SetCamTriggerRcipe(0);

	Update(FALSE, 0);
	UpdateData(FALSE);
	return TRUE;
}

void CDialogTriggerCtrl::TriggerInfo()
{
	SendTriggerModeInfo();
	SendTriggerCountInfo();
	SendEncoderCountInfo();
}

//서버 자동 접속 쓰레드
UINT CDialogTriggerCtrl::TiggerInfoThread(LPVOID pParam)
{
	CDialogTriggerCtrl *pdlg = (CDialogTriggerCtrl *)pParam;

	while(TRUE)
	{
		if(pdlg->m_bTriggerThreadStart)
		{
			::SendMessageA(pdlg->m_hWnd, USER_MSG_TRIGGER, TRIGGER_INFO_VIEW, 0);
			::SendMessageA(pdlg->m_hWnd, USER_MSG_TRIGGER, TRIGGER_INFO_VIEW, 1);
			::SendMessageA(pdlg->m_hWnd, USER_MSG_TRIGGER, TRIGGER_INFO_VIEW, 2);
			::SendMessageA(pdlg->m_hWnd, USER_MSG_TRIGGER, TRIGGER_INFO_VIEW, 3);
			Sleep(10);
		}

		if(pdlg->m_bTriggerThreadEnd)
		{
			break;
		}
		Sleep(1);
	}
	return 0;
}

void CDialogTriggerCtrl::ControlInit()
{
	m_combo_trigger_select_cam.SetCurSel(0);
	m_combo_trigger_port.SetCurSel(0);;
	m_combo_trigger_baud_rate.SetCurSel(2);
	m_combo_trigger_data_bit.SetCurSel(3);
	m_combo_trigger_stop_bit.SetCurSel(0);
	m_combo_trigger_parity.SetCurSel(0);
	
	m_combo_trigger_mode_ch0.SetCurSel(1);
	m_combo_trigger_mode_ch1.SetCurSel(1);
	m_combo_trigger_mode_ch2.SetCurSel(1);
	m_combo_trigger_mode_ch3.SetCurSel(1);

	m_edit_trigger_start_pos0 = 1;
	m_edit_trigger_end_pos0 = 10000;
	m_edit_trigger_pitch0 = 10;
	m_edit_trigger_pulse_width0 = 12;
	m_edit_trigger_delay0 = 12;

	m_edit_trigger_start_pos1 = 1;
	m_edit_trigger_end_pos1 = 10000;
	m_edit_trigger_pitch1 = 10;
	m_edit_trigger_pulse_width1 = 12;
	m_edit_trigger_delay1 = 12;

	m_edit_trigger_start_pos2 = 1;
	m_edit_trigger_end_pos2 = 10000;
	m_edit_trigger_pitch2 = 10;
	m_edit_trigger_pulse_width2 = 12;
	m_edit_trigger_delay2 = 12;

	m_edit_trigger_start_pos3 = 1;
	m_edit_trigger_end_pos3 = 10000;
	m_edit_trigger_pitch3 = 10;
	m_edit_trigger_pulse_width3 = 12;
	m_edit_trigger_delay3 = 12;

	m_btn_trigger_status_enable0.SetValue(FALSE);
	m_btn_trigger_status_enable1.SetValue(FALSE);
	m_btn_trigger_status_enable2.SetValue(FALSE);
	m_btn_trigger_status_enable3.SetValue(FALSE);
}

void CDialogTriggerCtrl::ControlEnable(BOOL bEnable)
{
	m_combo_trigger_port.EnableWindow(!bEnable);
	m_combo_trigger_baud_rate.EnableWindow(!bEnable);
	m_combo_trigger_data_bit.EnableWindow(!bEnable);
	m_combo_trigger_stop_bit.EnableWindow(!bEnable);
	m_combo_trigger_parity.EnableWindow(!bEnable);

	m_combo_trigger_select_cam.EnableWindow(bEnable);
	m_combo_trigger_mode_ch0.EnableWindow(bEnable);
	m_combo_trigger_mode_ch1.EnableWindow(bEnable);
	m_combo_trigger_mode_ch2.EnableWindow(bEnable);
	m_combo_trigger_mode_ch3.EnableWindow(bEnable);

	GetDlgItem(IDC_EDIT_TRIGGER_ENCODER_RESOLUTION)->EnableWindow(bEnable);

	GetDlgItem(IDC_EDIT_TRIGGER_START_POS_0)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_TRIGGER_END_POS_0)->EnableWindow(bEnable); 
	GetDlgItem(IDC_EDIT_TRIGGER_PITCH_0)->EnableWindow(bEnable); 
	GetDlgItem(IDC_EDIT_TRIGGER_PULSE_WIDTH_0)->EnableWindow(bEnable); 
	GetDlgItem(IDC_EDIT_TRIGGER_DELAY_0)->EnableWindow(bEnable); 

	GetDlgItem(IDC_EDIT_TRIGGER_START_POS_1)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_TRIGGER_END_POS_1)->EnableWindow(bEnable); 
	GetDlgItem(IDC_EDIT_TRIGGER_PITCH_1)->EnableWindow(bEnable); 
	GetDlgItem(IDC_EDIT_TRIGGER_PULSE_WIDTH_1)->EnableWindow(bEnable); 
	GetDlgItem(IDC_EDIT_TRIGGER_DELAY_1)->EnableWindow(bEnable); 

	GetDlgItem(IDC_EDIT_TRIGGER_START_POS_2)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_TRIGGER_END_POS_2)->EnableWindow(bEnable); 
	GetDlgItem(IDC_EDIT_TRIGGER_PITCH_2)->EnableWindow(bEnable); 
	GetDlgItem(IDC_EDIT_TRIGGER_PULSE_WIDTH_2)->EnableWindow(bEnable); 
	GetDlgItem(IDC_EDIT_TRIGGER_DELAY_2)->EnableWindow(bEnable); 

	GetDlgItem(IDC_EDIT_TRIGGER_START_POS_3)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_TRIGGER_END_POS_3)->EnableWindow(bEnable); 
	GetDlgItem(IDC_EDIT_TRIGGER_PITCH_3)->EnableWindow(bEnable); 
	GetDlgItem(IDC_EDIT_TRIGGER_PULSE_WIDTH_3)->EnableWindow(bEnable); 
	GetDlgItem(IDC_EDIT_TRIGGER_DELAY_3)->EnableWindow(bEnable); 

	m_btn_trigger_status_enable0.EnableWindow(bEnable);
	StatusEnable(0, m_btn_trigger_status_enable0.GetValue());

	m_btn_trigger_status_enable1.EnableWindow(bEnable);
	StatusEnable(1, m_btn_trigger_status_enable1.GetValue());

	m_btn_trigger_status_enable2.EnableWindow(bEnable);
	StatusEnable(2, m_btn_trigger_status_enable2.GetValue());

	m_btn_trigger_status_enable3.EnableWindow(bEnable);
	StatusEnable(3, m_btn_trigger_status_enable3.GetValue());
}

void CDialogTriggerCtrl::StatusEnable(int ch, BOOL bEnable)
{
	switch(ch)
	{
	case 0:
		GetDlgItem(IDC_EDIT_TRIGGER_SELECTION_MODE0)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_TRIGGER_TRIGGER_COUNT0)->EnableWindow(bEnable); 
		GetDlgItem(IDC_EDIT_TRIGGER_ENCODER_COUNT0)->EnableWindow(bEnable); 
		GetDlgItem(IDC_EDIT_TRIGGER_ENCODR_POSITION0)->EnableWindow(bEnable);
		break;
	case 1:
		GetDlgItem(IDC_EDIT_TRIGGER_SELECTION_MODE1)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_TRIGGER_TRIGGER_COUNT1)->EnableWindow(bEnable); 
		GetDlgItem(IDC_EDIT_TRIGGER_ENCODER_COUNT1)->EnableWindow(bEnable); 
		GetDlgItem(IDC_EDIT_TRIGGER_ENCODR_POSITION1)->EnableWindow(bEnable);
		break;
	case 2:
		GetDlgItem(IDC_EDIT_TRIGGER_SELECTION_MODE2)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_TRIGGER_TRIGGER_COUNT2)->EnableWindow(bEnable); 
		GetDlgItem(IDC_EDIT_TRIGGER_ENCODER_COUNT2)->EnableWindow(bEnable); 
		GetDlgItem(IDC_EDIT_TRIGGER_ENCODR_POSITION2)->EnableWindow(bEnable);
		break;
	case 3:
		GetDlgItem(IDC_EDIT_TRIGGER_SELECTION_MODE3)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_TRIGGER_TRIGGER_COUNT3)->EnableWindow(bEnable); 
		GetDlgItem(IDC_EDIT_TRIGGER_ENCODER_COUNT3)->EnableWindow(bEnable); 
		GetDlgItem(IDC_EDIT_TRIGGER_ENCODR_POSITION3)->EnableWindow(bEnable);
		break;
	}
}

CString CDialogTriggerCtrl::byIndexComPort(int xPort)
{
	CString PortName;
	switch(xPort)
	{
		case 0:		PortName = "COM1"; 			break;
		case 1:		PortName = "COM2";			break;
		case 2:		PortName = "COM3"; 			break;
		case 3:		PortName = "COM4";			break;
		case 4:		PortName = "\\\\.\\COM5"; 	break;
		case 5:		PortName = "\\\\.\\COM6";	break;
		case 6:		PortName = "\\\\.\\COM7"; 	break;
		case 7:		PortName = "\\\\.\\COM8";	break;
		case 8:		PortName = "\\\\.\\COM9"; 	break;
		case 9:		PortName = "\\\\.\\COM10";	break;
		case 10:	PortName = "\\\\.\\COM11";	break;
		case 11:	PortName = "\\\\.\\COM12";	break;
		case 12:	PortName = "\\\\.\\COM13";	break;
		case 13:	PortName = "\\\\.\\COM14";	break;
		case 14:	PortName = "\\\\.\\COM15";	break;
		case 15:	PortName = "\\\\.\\COM16";	break;
		case 16:	PortName = "\\\\.\\COM17";	break;
		case 17:	PortName = "\\\\.\\COM18";	break;
		case 18:	PortName = "\\\\.\\COM19";	break;
		case 19:	PortName = "\\\\.\\COM20";	break;
		case 20:	PortName = "\\\\.\\COM21";	break;
		case 21:	PortName = "\\\\.\\COM22";	break;
		case 22:	PortName = "\\\\.\\COM23";	break;
		case 23:	PortName = "\\\\.\\COM24";	break;
		case 24:	PortName = "\\\\.\\COM25";	break;
		case 25:	PortName = "\\\\.\\COM26";	break;
		case 26:	PortName = "\\\\.\\COM27";	break;
		case 27:	PortName = "\\\\.\\COM28";	break;
		case 28:	PortName = "\\\\.\\COM29";	break;
		case 29:	PortName = "\\\\.\\COM30";	break;
		case 30:	PortName = "\\\\.\\COM31";	break;
		case 31:	PortName = "\\\\.\\COM32";	break;
	}

	return PortName;
}

DWORD CDialogTriggerCtrl::byIndexBaud(int xBaud)
{
	DWORD dwBaud;
	switch(xBaud)
	{
		case 0:		dwBaud = CBR_2400;		break;
		case 1:		dwBaud = CBR_4800;		break;
		case 2:		dwBaud = CBR_9600;		break;
		case 3:		dwBaud = CBR_14400;		break;
		case 4:		dwBaud = CBR_19200;		break;
		case 5:		dwBaud = CBR_38400;		break;
		case 6:		dwBaud = CBR_57600;		break;
		case 7:		dwBaud = CBR_115200;	break;
		case 8:		dwBaud = 230400;		break;
		case 9:		dwBaud = 460800;		break;
		case 10:	dwBaud = 921600;		break;
		case 11:	dwBaud = 1024000;		break;
		case 12:	dwBaud = 2048000;		break;
		case 13:	dwBaud = 3072000;		break;
		//case 14:	dwBaud = 5120000;		break;
	}

	return dwBaud;
}

BYTE CDialogTriggerCtrl::byIndexData(int xData)
{
	BYTE byData;
	switch(xData)
	{
		case 0 :	byData = 5;			break;
		case 1 :	byData = 6;			break;
		case 2 :	byData = 7;			break;
		case 3 :	byData = 8;			break;
	}
	return byData;
}

BYTE CDialogTriggerCtrl::byIndexStop(int xStop)
{
	BYTE byStop;
	if(xStop == 0)
	{
		byStop = ONESTOPBIT;
	}
	else
	{
		byStop = TWOSTOPBITS;
	}
	return byStop;
}

BYTE CDialogTriggerCtrl::byIndexParity(int xParity)
{
	BYTE byParity;
	switch(xParity)
	{
	case 0 :	byParity = NOPARITY;	break;
	case 1 :	byParity = ODDPARITY;	break;
	case 2 :	byParity = EVENPARITY;	break;
	}

	return byParity;
}

void CDialogTriggerCtrl::TriggerOpen()
{
	CString str;
	if(IsWindowVisible())
	{
		UpdateData(TRUE);
	}

	ControlInit();
	
	if(m_CommThread.m_bConnected == FALSE)//포트가 닫혀 있을 경우에만 포트를 열기 위해
	{
		if(m_CommThread.OpenPort(byIndexComPort(m_combo_trigger_port.GetCurSel()), byIndexBaud(m_combo_trigger_baud_rate.GetCurSel()), byIndexData(m_combo_trigger_data_bit.GetCurSel()), byIndexStop(m_combo_trigger_stop_bit.GetCurSel()), byIndexParity(m_combo_trigger_parity.GetCurSel())) == TRUE)
		{
			TriggerSetting(0);
			m_bTriggerConnection = TRUE;
			m_btn_trigger_connection.SetValue(m_bTriggerConnection);
			m_btn_trigger_connection.SetWindowTextW(_T("Connection"));
		}
		else
		{
			if(m_CommThread.m_bConnected == TRUE)
			{	
				m_CommThread.ClosePort();
				str.Format(_T("%s Init Fail"), TRIGGER_COM_PORT);
				AfxMessageBox(str);
			}
			m_bTriggerConnection = FALSE;
			m_btn_trigger_connection.SetValue(m_bTriggerConnection);
			m_btn_trigger_connection.SetWindowTextW(_T("DeConnection"));
		}
	}
	else
	{
		str.Format(_T("%s이 이미 사용중인지 확인하세요"), TRIGGER_COM_PORT);
		AfxMessageBox(str);
	}
	ControlEnable(m_bTriggerConnection);
	if(IsWindowVisible())
	{
		UpdateData(FALSE);
	}
}

void CDialogTriggerCtrl::Update(BOOL isUpdate, int idx)
{
	if(isUpdate)
	{
		UpdateData(TRUE);
		m_dTriggerPara[idx][0] = 1;//m_combo_trigger_mode_ch0.GetCurSel();
		m_dTriggerPara[idx][1] = 1;//m_combo_trigger_mode_ch1.GetCurSel();
		m_dTriggerPara[idx][2] = 1;//m_combo_trigger_mode_ch2.GetCurSel();
		m_dTriggerPara[idx][3] = 1;//m_combo_trigger_mode_ch3.GetCurSel();

		m_dTriggerPara[idx][4] = m_edit_trigger_encoder_resolution;

		m_dTriggerPara[idx][5] = m_edit_trigger_start_pos0;
		m_dTriggerPara[idx][6] = m_edit_trigger_end_pos0;
		m_dTriggerPara[idx][7] = m_edit_trigger_pitch0;
		m_dTriggerPara[idx][8] = m_edit_trigger_pulse_width0;
		m_dTriggerPara[idx][9] = m_edit_trigger_delay0;

		m_dTriggerPara[idx][10] = m_edit_trigger_start_pos1;
		m_dTriggerPara[idx][11] = m_edit_trigger_end_pos1;
		m_dTriggerPara[idx][12] = m_edit_trigger_pitch1;
		m_dTriggerPara[idx][13] = m_edit_trigger_pulse_width1;
		m_dTriggerPara[idx][14] = m_edit_trigger_delay1;

		m_dTriggerPara[idx][15] = m_edit_trigger_start_pos2;
		m_dTriggerPara[idx][16] = m_edit_trigger_end_pos2;
		m_dTriggerPara[idx][17] = m_edit_trigger_pitch2;
		m_dTriggerPara[idx][18] = m_edit_trigger_pulse_width2;
		m_dTriggerPara[idx][19] = m_edit_trigger_delay2;

		m_dTriggerPara[idx][20] = m_edit_trigger_start_pos3;
		m_dTriggerPara[idx][21] = m_edit_trigger_end_pos3;
		m_dTriggerPara[idx][22] = m_edit_trigger_pitch3;
		m_dTriggerPara[idx][23] = m_edit_trigger_pulse_width3;
		m_dTriggerPara[idx][24] = m_edit_trigger_delay3;
	}
	else
	{
		m_combo_trigger_mode_ch0.SetCurSel((int)m_dTriggerPara[idx][0]);
		m_combo_trigger_mode_ch1.SetCurSel((int)m_dTriggerPara[idx][1]);
		m_combo_trigger_mode_ch2.SetCurSel((int)m_dTriggerPara[idx][2]);
		m_combo_trigger_mode_ch3.SetCurSel((int)m_dTriggerPara[idx][3]);

		m_edit_trigger_encoder_resolution = m_dTriggerPara[idx][4];

		m_edit_trigger_start_pos0 = m_dTriggerPara[idx][5];
		m_edit_trigger_end_pos0 = m_dTriggerPara[idx][6];
		m_edit_trigger_pitch0 = m_dTriggerPara[idx][7];
		m_edit_trigger_pulse_width0 = m_dTriggerPara[idx][8];
		m_edit_trigger_delay0 = m_dTriggerPara[idx][9];

		m_edit_trigger_start_pos1 = m_dTriggerPara[idx][10];
		m_edit_trigger_end_pos1 = m_dTriggerPara[idx][11];
		m_edit_trigger_pitch1 = m_dTriggerPara[idx][12];
		m_edit_trigger_pulse_width1 = m_dTriggerPara[idx][13];
		m_edit_trigger_delay1 = m_dTriggerPara[idx][14];
		
		m_edit_trigger_start_pos2 = m_dTriggerPara[idx][15];
		m_edit_trigger_end_pos2 = m_dTriggerPara[idx][16];
		m_edit_trigger_pitch2 = m_dTriggerPara[idx][17];
		m_edit_trigger_pulse_width2 = m_dTriggerPara[idx][18];
		m_edit_trigger_delay2 = m_dTriggerPara[idx][19];

		m_edit_trigger_start_pos3 = m_dTriggerPara[idx][20];
		m_edit_trigger_end_pos3 = m_dTriggerPara[idx][21];
		m_edit_trigger_pitch3 = m_dTriggerPara[idx][22];
		m_edit_trigger_pulse_width3 = m_dTriggerPara[idx][23];
		m_edit_trigger_delay3 = m_dTriggerPara[idx][24];
		UpdateData(FALSE);
	}
}

void CDialogTriggerCtrl::TriggerSetting(int idx)
{
	CString SendCmdData;

	SendCmdData.Format(_T("/%s%d%x/"),HCMD_SETTRGMODE, 0, (int)m_dTriggerPara[idx][0]);
	SendCommand(SendCmdData);
	SendWait();

	SendCmdData.Format(_T("/%s%d%x/"),HCMD_SETTRGMODE, 1, (int)m_dTriggerPara[idx][1]);
	SendCommand(SendCmdData);
	SendWait();

	SendCmdData.Format(_T("/%s%d%x/"),HCMD_SETTRGMODE, 2, (int)m_dTriggerPara[idx][2]);
	SendCommand(SendCmdData);
	SendWait();

	SendCmdData.Format(_T("/%s%d%x/"),HCMD_SETTRGMODE, 3, (int)m_dTriggerPara[idx][3]);
	SendCommand(SendCmdData);
	SendWait();

	SendMode1StartEndPosition(0, m_dTriggerPara[idx][5], m_dTriggerPara[idx][6], m_dTriggerPara[idx][4]);
	SendMode1StartEndPosition(1, m_dTriggerPara[idx][10], m_dTriggerPara[idx][11], m_dTriggerPara[idx][4]);
	SendMode1StartEndPosition(2, m_dTriggerPara[idx][15], m_dTriggerPara[idx][16], m_dTriggerPara[idx][4]);
	SendMode1StartEndPosition(3, m_dTriggerPara[idx][20], m_dTriggerPara[idx][21], m_dTriggerPara[idx][4]);
	SendMode1Pitch(0, m_dTriggerPara[idx][7], m_dTriggerPara[idx][4]);
	SendMode1Pitch(1, m_dTriggerPara[idx][12], m_dTriggerPara[idx][4]);
	SendMode1Pitch(2, m_dTriggerPara[idx][17], m_dTriggerPara[idx][4]);
	SendMode1Pitch(3, m_dTriggerPara[idx][22], m_dTriggerPara[idx][4]);
	SendMode1PulseWidth(0, m_dTriggerPara[idx][8]);
	SendMode1PulseWidth(1, m_dTriggerPara[idx][13]);
	SendMode1PulseWidth(2, m_dTriggerPara[idx][18]);
	SendMode1PulseWidth(3, m_dTriggerPara[idx][23]);
	SendMode1Delay(0, m_dTriggerPara[idx][9]);
	SendMode1Delay(1, m_dTriggerPara[idx][14]);
	SendMode1Delay(2, m_dTriggerPara[idx][19]);
	SendMode1Delay(3, m_dTriggerPara[idx][24]);

	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;    //20160803 pkh

	CString str0;

	//str0.Format(_T("%d_%d_%d_%d_Test_pkh"),m_dTriggerPara[idx][0], m_dTriggerPara[idx][1], m_dTriggerPara[idx][2],m_dTriggerPara[idx][3]);
	//str0.Format(_T("%x_%x_%x_%x_Test_pkh"),&m_dTriggerPara[idx][0], &m_dTriggerPara[idx][1], &m_dTriggerPara[idx][2],&m_dTriggerPara[idx][3]);
	str0.Format(_T("%f_%f_%f_%f_Test_pkh"),m_dTriggerPara[idx][0], m_dTriggerPara[idx][1], m_dTriggerPara[idx][2],m_dTriggerPara[idx][3]);
	pdlg->WriteLog(REVIEW_LOG,str0);

	CString str1;
	
	str1.Format(_T("%f_%f_%f_Test1"),m_dTriggerPara[idx][5], m_dTriggerPara[idx][6], m_dTriggerPara[idx][4]);
	pdlg->WriteLog(REVIEW_LOG,str1);

	CString str2;
//	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;    //20160803 pkh
	str2.Format(_T("%f_%f_%f_Test2"),m_dTriggerPara[idx][10], m_dTriggerPara[idx][11], m_dTriggerPara[idx][4]);
	pdlg->WriteLog(REVIEW_LOG,str2);

	CString str3;
//	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;    //20160803 pkh
	str3.Format(_T("%f_%f_%f_Test3"),m_dTriggerPara[idx][15], m_dTriggerPara[idx][16], m_dTriggerPara[idx][4]);
	pdlg->WriteLog(REVIEW_LOG,str3);

	CString str4;
//	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;    //20160803 pkh
	str4.Format(_T("%f_%f_%f_Test4"),m_dTriggerPara[idx][20], m_dTriggerPara[idx][21], m_dTriggerPara[idx][4]);
	pdlg->WriteLog(REVIEW_LOG,str4);


	CString str5;
//	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;    //20160803 pkh
	str5.Format(_T("%f_%f_Test5"),m_dTriggerPara[idx][7], m_dTriggerPara[idx][4]);
	pdlg->WriteLog(REVIEW_LOG,str5);

	CString str6;
//	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;    //20160803 pkh
	str6.Format(_T("%f_%f_Test6"),m_dTriggerPara[idx][12], m_dTriggerPara[idx][4]);
	pdlg->WriteLog(REVIEW_LOG,str6);

	CString str7;
//	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;    //20160803 pkh
	str7.Format(_T("%f_%f_Test7"),m_dTriggerPara[idx][17], m_dTriggerPara[idx][4]);
	pdlg->WriteLog(REVIEW_LOG,str7);

	CString str8;
	//CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;    //20160803 pkh
	str8.Format(_T("%f_%f_Test8"),m_dTriggerPara[idx][22], m_dTriggerPara[idx][4]);
	pdlg->WriteLog(REVIEW_LOG,str8);


	CString str9;
	//CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;    //20160803 pkh
	str9.Format(_T("%f_%f,%f,%f,Test9"),m_dTriggerPara[idx][8],m_dTriggerPara[idx][13],m_dTriggerPara[idx][18],m_dTriggerPara[idx][23]);
	pdlg->WriteLog(REVIEW_LOG,str9);

	CString str10;
	//CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;    //20160803 pkh
	str10.Format(_T("%f_%f,%f,%f,Test10"),m_dTriggerPara[idx][9],m_dTriggerPara[idx][14],m_dTriggerPara[idx][19],m_dTriggerPara[idx][24]);
	pdlg->WriteLog(REVIEW_LOG,str10);
}

void CDialogTriggerCtrl::SendOPMode(BOOL isOPMode)
{
	CString SendCmdData;

	if(isOPMode)
	{
		SendCmdData.Format(_T("/%s/"),HCMD_OPON);
		SendCommand(SendCmdData);
	} 
	else{
		SendCmdData.Format(_T("/%s/"),HCMD_STOP);
		SendCommand(SendCmdData);
	}
}

void CDialogTriggerCtrl::SendMode1Direction(BOOL isDirection)
{
	CString SendCmdData;

	if(isDirection)
	{
		SendCmdData.Format(_T("/%s%x/"),HCMD_MODE1DIR,SET_MODE1DIRPOS);
		SendCommand(SendCmdData);
	}
	else
	{
		SendCmdData.Format(_T("/%s%x/"),HCMD_MODE1DIR,SET_MODE1DIRNEG);
		SendCommand(SendCmdData);
	}
}

void CDialogTriggerCtrl::SendCommand(CString senddata)
{
	if(senddata.Mid(1,2)=="gi")
	{
		b_GetInformation=TRUE;
	}
	else
	{
		b_GetInformation=FALSE;
	}

	int nSize = senddata.GetLength();

	char start[256];
	sprintf_s(start, nSize + 1, "%S", senddata);

	if(m_bTriggerConnection)
	{
		m_CommThread.WriteCommBlock(start,nSize);
	}
}

void CDialogTriggerCtrl::Wait(DWORD dwMillisecond)
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

void CDialogTriggerCtrl::SendWait()
{
	if(IsWindowVisible())
	{
		UpdateData(TRUE);
	}

	MSG msg;
	DWORD dwStart, dwMillisecond;
	dwStart = GetTickCount();

	switch(m_combo_trigger_baud_rate.GetCurSel())
	{
		case 0:		dwMillisecond = 150;	break;
		case 1:		dwMillisecond = 75; 	break;
		case 2:		dwMillisecond = 38; 	break;
		case 3:		dwMillisecond = 25; 	break;
		case 4:		dwMillisecond = 19; 	break;
		case 5:		dwMillisecond = 10; 	break;
		case 6:		dwMillisecond = 7;		break;
		case 7:		dwMillisecond = 7; 		break;
		case 8:		dwMillisecond = 4; 		break;
		case 9:		dwMillisecond = 2; 		break;
		case 10:	dwMillisecond = 1; 		break;
		case 11:	dwMillisecond = 1; 		break;
		case 12:	dwMillisecond = 1; 		break;
		case 13:	dwMillisecond = 1; 		break;
		case 14:	dwMillisecond = 1; 		break;
	}

	while(GetTickCount() - dwStart < dwMillisecond)
	{
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	if(IsWindowVisible())
	{
		UpdateData(FALSE);
	}
} 

void CDialogTriggerCtrl::SendMode1StartEndPosition(UINT n_ch, double d_Start, double d_End, double d_EncRes)
{
	CString SendCmdData;
	UINT n_TrgStart, nTrgEnd;

	if(d_Start<0) n_TrgStart=0;
	else n_TrgStart = (UINT)(d_Start*1000.0/d_EncRes+0.5);

	SendCmdData.Format(_T("/%s%x%08x/"),HCMD_MODE1START,n_ch,n_TrgStart+ENC_BASEPOS);
	SendCommand(SendCmdData);

	SendWait();

	if(d_End<0) nTrgEnd=0;
	else nTrgEnd = (UINT)(d_End*1000.0/d_EncRes+0.5);

	SendCmdData.Format(_T("/%s%x%08x/"),HCMD_MODE1END,n_ch,nTrgEnd+ENC_BASEPOS);
	SendCommand(SendCmdData);

	SendWait();
}

void CDialogTriggerCtrl::SendMode1Pitch(UINT n_ch, double d_pitch, double d_EncRes)
{
	CString SendCmdData;
	UINT n_TrgPitch;

	if(d_pitch<0) n_TrgPitch=0;
	else n_TrgPitch = (UINT)(d_pitch/d_EncRes+0.5);

	SendCmdData.Format(_T("/%s%x%08x/"),HCMD_MODE1PITCH,n_ch,n_TrgPitch);
	SendCommand(SendCmdData);

	SendWait();
}

void CDialogTriggerCtrl::SendMode1PulseWidth(UINT n_ch, double d_pulsewidth)
{
	CString SendCmdData;
	UINT n_Mode1PW;

	if(d_pulsewidth<0) n_Mode1PW=0;
	else n_Mode1PW = (UINT)(d_pulsewidth*100.0+COMPPULSEWIDTH);

	SendCmdData.Format(_T("/%s%x%08x/"),HCMD_MODE1PULSEWIDTH,n_ch,n_Mode1PW);
	SendCommand(SendCmdData);

	SendWait();
}

void CDialogTriggerCtrl::SendMode1Delay(UINT n_ch, double d_delay)
{
	CString SendCmdData;
	UINT n_Mode1Delay;

	if(d_delay<0) n_Mode1Delay=0;
	else n_Mode1Delay = (UINT)(d_delay*100.0+COMPPULSEWIDTH);

	SendCmdData.Format(_T("/%s%x%08x/"),HCMD_MODE1DELAY,n_ch,n_Mode1Delay);
	SendCommand(SendCmdData);

	SendWait();
}

void CDialogTriggerCtrl::SendResetEncoderCnt()
{
	CString SendCmdData;

	SendCmdData.Format(_T("/%s/"),HCMD_ENCCNTCLR);
	SendCommand(SendCmdData);
}	

void CDialogTriggerCtrl::SendResetTriggerCnt()
{
	CString SendCmdData;

	SendCmdData.Format(_T("/%s/"),HCMD_CLRTRGCNT);
	SendCommand(SendCmdData);
}

void CDialogTriggerCtrl::SetResetEncoderCnt()
{
	SendResetEncoderCnt();
	Sleep(100);
}

void CDialogTriggerCtrl::SetResetTriggerCnt()
{
	SendResetTriggerCnt();
	Sleep(100);
}

void CDialogTriggerCtrl::SetOPMode(BOOL isOPMode)
{
	SendOPMode(isOPMode);
	Sleep(100);
}

void CDialogTriggerCtrl::SetDirection(BOOL isDirection)
{
	if (isDirection)
	{
		SendMode1Direction(TRUE);
		Sleep(30);
		SendMode1Direction(FALSE);
		Sleep(30);
		SendMode1Direction(TRUE);
		Sleep(30);
		SendMode1Direction(FALSE);
		Sleep(30);
		SendMode1Direction(TRUE);
		Sleep(30);
	}
	else
	{		
		SendMode1Direction(FALSE);
		Sleep(30);
		SendMode1Direction(TRUE);
		Sleep(30);
		SendMode1Direction(FALSE);
		Sleep(30);
		SendMode1Direction(TRUE);
		Sleep(30);
		SendMode1Direction(FALSE);
		Sleep(30);
	}
	
	SendResetTriggerCnt();	
}
BEGIN_EVENTSINK_MAP(CDialogTriggerCtrl, CDialogEx)
	ON_EVENT(CDialogTriggerCtrl, IDC_BTNENHCTRL_TRIGGER_CONNECTION, DISPID_CLICK, CDialogTriggerCtrl::ClickBtnenhctrlTriggerConnection, VTS_NONE)
	ON_EVENT(CDialogTriggerCtrl, IDC_BTNENHCTRL_TRIGGER_APPLY, DISPID_CLICK, CDialogTriggerCtrl::ClickBtnenhctrlTriggerApply, VTS_NONE)
	ON_EVENT(CDialogTriggerCtrl, IDC_BTNENHCTRL_TRIGGER_SAVE, DISPID_CLICK, CDialogTriggerCtrl::ClickBtnenhctrlTriggerSave, VTS_NONE)
	ON_EVENT(CDialogTriggerCtrl, IDC_BTNENHCTRL_TRIGGER_CANCEL, DISPID_CLICK, CDialogTriggerCtrl::ClickBtnenhctrlTriggerCancel, VTS_NONE)
	ON_EVENT(CDialogTriggerCtrl, IDC_BTNENHCTRL_TRIGGER_STATUS_ENABLE0, DISPID_CLICK, CDialogTriggerCtrl::ClickBtnenhctrlTriggerStatusEnable0, VTS_NONE)
	ON_EVENT(CDialogTriggerCtrl, IDC_BTNENHCTRL_TRIGGER_STATUS_ENABLE1, DISPID_CLICK, CDialogTriggerCtrl::ClickBtnenhctrlTriggerStatusEnable1, VTS_NONE)
	ON_EVENT(CDialogTriggerCtrl, IDC_BTNENHCTRL_TRIGGER_STATUS_ENABLE2, DISPID_CLICK, CDialogTriggerCtrl::ClickBtnenhctrlTriggerStatusEnable2, VTS_NONE)
	ON_EVENT(CDialogTriggerCtrl, IDC_BTNENHCTRL_TRIGGER_STATUS_ENABLE3, DISPID_CLICK, CDialogTriggerCtrl::ClickBtnenhctrlTriggerStatusEnable3, VTS_NONE)
	END_EVENTSINK_MAP()


	
void CDialogTriggerCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow)
	{
		m_combo_trigger_select_cam.SetCurSel(0);
		m_bTriggerThreadStart = TRUE;
		m_bTriggerThreadEnd = FALSE;
		m_pThreadTriggerInfo = AfxBeginThread(TiggerInfoThread, this, THREAD_PRIORITY_NORMAL);
		m_pThreadTriggerInfo->m_bAutoDelete = TRUE;
		Update(FALSE, m_combo_trigger_select_cam.GetCurSel());
	}
	else
	{
		m_bTriggerThreadStart = FALSE;
		m_bTriggerThreadEnd = TRUE;
	}
}

void CDialogTriggerCtrl::ClickBtnenhctrlTriggerConnection()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(m_btn_trigger_connection.GetValue() && !m_bTriggerConnection)
	{
		TriggerOpen();
	}
	else if(!m_btn_trigger_connection.GetValue() && m_bTriggerConnection)
	{
		m_CommThread.ClosePort();
		m_bTriggerConnection = FALSE;
		m_btn_trigger_connection.SetValue(m_bTriggerConnection);
		m_btn_trigger_connection.SetWindowTextW(_T("DeConnection"));
	}
	ControlEnable(TRUE);
	UpdateData(FALSE);
}


void CDialogTriggerCtrl::OnCbnCloseupComboTriggerSelectCam()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int cam_idx = m_combo_trigger_select_cam.GetCurSel();
	
}


void CDialogTriggerCtrl::ClickBtnenhctrlTriggerApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString filename;
	//m_combo_trigger_select_cam.GetWindowTextW(filename);
	filename.Format(_T("Cam0_8K"));
	RecipeListToFile(filename, 0);
	filename.Format(_T("Cam1_16K"));
	RecipeListToFile(filename, 1);
#ifdef MACHINE
	TriggerSetting(m_combo_trigger_select_cam.GetCurSel());
	Sleep(10);
	TriggerSetting(m_combo_trigger_select_cam.GetCurSel());
#endif
	UpdateData(TRUE);
}


void CDialogTriggerCtrl::ClickBtnenhctrlTriggerSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString filename;
	filename.Format(_T("Cam0_8K"));
	RecipeListToFile(filename, 0);
	filename.Format(_T("Cam1_16K"));
	RecipeListToFile(filename, 1);
#ifdef MACHINE
	TriggerSetting(m_combo_trigger_select_cam.GetCurSel());
#endif
	UpdateData(TRUE);
}


void CDialogTriggerCtrl::ClickBtnenhctrlTriggerCancel()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ShowWindow(SW_HIDE);
}

void CDialogTriggerCtrl::RecipeListToFile(CString filename, int cam_idx)
{
	UpdateData(TRUE);

	g_recipe.listTempGenRecipes.clear();
	g_recipe.listTempCmpMode0Recipes.clear();
	g_recipe.listTempCmpMode1Recipes.clear();

	GENRECIPE gentemp;
	gentemp.d_EncoderResolution = m_dTriggerPara[cam_idx][4];
	g_recipe.listTempGenRecipes.push_back(gentemp);

	CMPMODE1RECIPES::const_iterator K;
	for(K=g_recipe.listCmpMode1Recipes.begin();K!=g_recipe.listCmpMode1Recipes.end();K++)
	{
		CMPMODE1RECIPE cmpmode1temp;
		switch(K->n_ID){
			case 0:
				cmpmode1temp.n_ID = 0;
				cmpmode1temp.d_StartPos = m_dTriggerPara[cam_idx][5];
				cmpmode1temp.d_EndPos = m_dTriggerPara[cam_idx][6];
				cmpmode1temp.d_Pitch = m_dTriggerPara[cam_idx][7];
				cmpmode1temp.d_PulseWidth = m_dTriggerPara[cam_idx][8];
				cmpmode1temp.d_Delay = m_dTriggerPara[cam_idx][9];
				g_recipe.listTempCmpMode1Recipes.push_back(cmpmode1temp);
				break;
			case 1:
				cmpmode1temp.n_ID = 1;
				cmpmode1temp.d_StartPos = m_dTriggerPara[cam_idx][10];
				cmpmode1temp.d_EndPos = m_dTriggerPara[cam_idx][11];
				cmpmode1temp.d_Pitch = m_dTriggerPara[cam_idx][12];
				cmpmode1temp.d_PulseWidth = m_dTriggerPara[cam_idx][13];
				cmpmode1temp.d_Delay = m_dTriggerPara[cam_idx][14];
				g_recipe.listTempCmpMode1Recipes.push_back(cmpmode1temp);
				break;
			case 2:
				cmpmode1temp.n_ID = 2;
				cmpmode1temp.d_StartPos = m_dTriggerPara[cam_idx][15];
				cmpmode1temp.d_EndPos = m_dTriggerPara[cam_idx][16];
				cmpmode1temp.d_Pitch = m_dTriggerPara[cam_idx][17];
				cmpmode1temp.d_PulseWidth = m_dTriggerPara[cam_idx][18];
				cmpmode1temp.d_Delay = m_dTriggerPara[cam_idx][19];
				g_recipe.listTempCmpMode1Recipes.push_back(cmpmode1temp);
				break;
			case 3:
				cmpmode1temp.n_ID = 3;
				cmpmode1temp.d_StartPos = m_dTriggerPara[cam_idx][20];
				cmpmode1temp.d_EndPos = m_dTriggerPara[cam_idx][21];
				cmpmode1temp.d_Pitch = m_dTriggerPara[cam_idx][22];
				cmpmode1temp.d_PulseWidth = m_dTriggerPara[cam_idx][23];
				cmpmode1temp.d_Delay = m_dTriggerPara[cam_idx][24];
				g_recipe.listTempCmpMode1Recipes.push_back(cmpmode1temp);
				break;
		}
	}

	g_recipe.CopyTempToList();
	char fileName[DAT_STR_LENG];
	CString recipe_path = _T("D:\\DATABASE\\SYSTEM\\");
	sprintf_s(fileName, recipe_path.GetLength() + filename.GetLength() + 13, "%STrigger_%S.rcp", recipe_path, filename);
	//sprintf_s(fileName, filename.GetLength() + 1, "%S", filename);
	g_recipe.WriteRecipe(fileName);

	UpdateData(FALSE);
}

int CDialogTriggerCtrl::set_recipe(const char *recipe_name)
{
	int sel;
	RCPFILE_LIST::const_iterator F;
	for ( sel=0, F=g_recipe.m_recipefiles.begin(); F!=g_recipe.m_recipefiles.end(); F++, sel++ ) {

		if ( strstr( F->data(), recipe_name ) != NULL ) {
			m_combo_trigger_select_cam.SetCurSel(sel);
			return sel;
		}
	}	
	return 0;
}

CString CDialogTriggerCtrl::get_recipe(int cam_idx)
{
	CString rp;

	if(cam_idx == 0)
	{
		rp.Format(_T("Cam0_8K"));
	}
	else if(cam_idx == 1)
	{
		rp.Format(_T("Cam1_16K"));
	}

	return rp;
	/*
	int sel = m_combo_trigger_select_cam.GetCurSel();
	if ( sel<0 ) return rp;

	CString sel_str;
	m_combo_trigger_select_cam.GetLBText( sel, sel_str );

	if ( sel_str.GetLength() <= 0 ) return rp;
	else return sel_str;*/
}

void CDialogTriggerCtrl::RecipeFileToList(CString filename)
{
	CString recipe_path = _T("D:\\DATABASE\\SYSTEM\\");
	char readfilename[DAT_STR_LENG];
	sprintf_s(readfilename, recipe_path.GetLength() + filename.GetLength() + 13, "%STrigger_%S.rcp", recipe_path, filename);
	g_recipe.ReadRecipe(readfilename);
	g_recipe.CopyListToTemp();
}

void CDialogTriggerCtrl::RecipeFileToSystemDialog(int cam_idx)
{
	m_dTriggerPara[cam_idx][0] = 1;
	m_dTriggerPara[cam_idx][1] = 1;
	m_dTriggerPara[cam_idx][2] = 1;
	m_dTriggerPara[cam_idx][3] = 1;

	RECIPE_NAMESPACE::TEMPGENRECIPES::const_iterator G;
	for ( G=g_recipe.listTempGenRecipes.begin() ; G!=g_recipe.listTempGenRecipes.end() ; G++ )
	{	
		m_dTriggerPara[cam_idx][4] = G->d_EncoderResolution;
	}

	RECIPE_NAMESPACE::TEMPCMPMODE1RECIPES::const_iterator K;
	for ( K=g_recipe.listTempCmpMode1Recipes.begin() ; K!=g_recipe.listTempCmpMode1Recipes.end() ; K++ )
	{	
		if(K->n_ID==0){
			m_dTriggerPara[cam_idx][5] = K->d_StartPos;
			m_dTriggerPara[cam_idx][6] = K->d_EndPos;
			m_dTriggerPara[cam_idx][7] = K->d_Pitch;
			m_dTriggerPara[cam_idx][8] = K->d_PulseWidth;
			m_dTriggerPara[cam_idx][9] = K->d_Delay;
		}
		else if(K->n_ID==1){
			m_dTriggerPara[cam_idx][10] = K->d_StartPos;
			m_dTriggerPara[cam_idx][11] = K->d_EndPos;
			m_dTriggerPara[cam_idx][12] = K->d_Pitch;
			m_dTriggerPara[cam_idx][13] = K->d_PulseWidth;
			m_dTriggerPara[cam_idx][14] = K->d_Delay;
		}
		else if(K->n_ID==2){
			m_dTriggerPara[cam_idx][15] = K->d_StartPos;
			m_dTriggerPara[cam_idx][16] = K->d_EndPos;
			m_dTriggerPara[cam_idx][17] = K->d_Pitch;
			m_dTriggerPara[cam_idx][18] = K->d_PulseWidth;
			m_dTriggerPara[cam_idx][19] = K->d_Delay;
		}
		else if(K->n_ID==3){
			m_dTriggerPara[cam_idx][20] = K->d_StartPos;
			m_dTriggerPara[cam_idx][21] = K->d_EndPos;
			m_dTriggerPara[cam_idx][22] = K->d_Pitch;
			m_dTriggerPara[cam_idx][23] = K->d_PulseWidth;
			m_dTriggerPara[cam_idx][24] = K->d_Delay;
		}
	}

	if(IsWindowVisible())
	{
		UpdateData(FALSE);
	}
}

void CDialogTriggerCtrl::OnCbnSelchangeComboTriggerSelectCam()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString recipe = get_recipe(m_combo_trigger_select_cam.GetCurSel());
	sRecipeFileName = recipe;
	RecipeFileToList(recipe);
	RecipeFileToSystemDialog(m_combo_trigger_select_cam.GetCurSel());
	Update(FALSE, m_combo_trigger_select_cam.GetCurSel());
	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;    //20160804 pkh
	CString stra0;
	stra0.Format(_T("%s_get_recipe1"),get_recipe(m_combo_trigger_select_cam.GetCurSel()));
	pdlg->WriteLog(REVIEW_LOG,stra0);
}

void CDialogTriggerCtrl::SetCamTriggerRcipe(int cam_idx)
{
	//m_combo_trigger_select_cam.SetCurSel(cam_idx);
	CString recipe = get_recipe(cam_idx);
	sRecipeFileName = recipe;
	RecipeFileToList(recipe);
	RecipeFileToSystemDialog(cam_idx);
	if(IsWindowVisible())
	{
		Update(FALSE, cam_idx);
	}

	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;    //20160804 pkh
	CString stra1;
	stra1.Format(_T("%s_get_recipe2"),get_recipe(cam_idx));
	pdlg->WriteLog(REVIEW_LOG,stra1);

#ifdef MACHINE
	TriggerSetting(cam_idx);
	Sleep(10);
	TriggerSetting(cam_idx);  //pkh 20160805
#endif
}

void CDialogTriggerCtrl::ClickBtnenhctrlTriggerStatusEnable0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(m_btn_trigger_status_enable0.GetValue())
	{
		m_btn_trigger_status_enable0.SetWindowTextW(_T("Enable"));
		m_bTriggerThreadStart = TRUE;
	}
	else
	{
		m_btn_trigger_status_enable0.SetWindowTextW(_T("Disable"));
		m_bTriggerThreadStart = FALSE;
	}
	StatusEnable(0, m_btn_trigger_status_enable0.GetValue());
	//TriggerInfo();
}


void CDialogTriggerCtrl::ClickBtnenhctrlTriggerStatusEnable1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(m_btn_trigger_status_enable1.GetValue())
	{
		m_btn_trigger_status_enable1.SetWindowTextW(_T("Enable"));
		m_bTriggerThreadStart = TRUE;
	}
	else
	{
		m_btn_trigger_status_enable1.SetWindowTextW(_T("Disable"));
		m_bTriggerThreadStart = FALSE;
	}
	StatusEnable(1, m_btn_trigger_status_enable1.GetValue());
	//TriggerInfo();
}


void CDialogTriggerCtrl::ClickBtnenhctrlTriggerStatusEnable2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(m_btn_trigger_status_enable2.GetValue())
	{
		m_btn_trigger_status_enable2.SetWindowTextW(_T("Enable"));
		m_bTriggerThreadStart = TRUE;
	}
	else
	{
		m_btn_trigger_status_enable2.SetWindowTextW(_T("Disable"));
		m_bTriggerThreadStart = FALSE;
	}
	StatusEnable(2, m_btn_trigger_status_enable2.GetValue());
	//TriggerInfo();
}


void CDialogTriggerCtrl::ClickBtnenhctrlTriggerStatusEnable3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(m_btn_trigger_status_enable3.GetValue())
	{
		m_btn_trigger_status_enable3.SetWindowTextW(_T("Enable"));
		m_bTriggerThreadStart = TRUE;
	}
	else
	{
		m_btn_trigger_status_enable3.SetWindowTextW(_T("Disable"));
		m_bTriggerThreadStart = FALSE;
	}
	StatusEnable(3, m_btn_trigger_status_enable3.GetValue());
	//TriggerInfo();
}


LRESULT CDialogTriggerCtrl::OnCommunication(WPARAM wParam, LPARAM lParam)
{
	if(IsWindowVisible())
	{
		UpdateData(TRUE);//받는 데이터 타입을 알기 위해
	}
	CString str = _T("");
	CString result, resultDisp;
	BYTE aByte; //데이터를 저장할 변수 
	int iSize =  (m_CommThread.m_QueueRead).GetSize(); //포트로 들어온 데이터 갯수
													//SetCommState에 설정된 내용 때문에 거의 8개씩 들어옴
//	m_iSize += iSize;	//받은 데이터의 총 갯수를 설정창에 보여주기 위해 
//	wsprintf(size,"Receive %d characters\r\n",m_iSize);
// 	if(iSize != 0)//받은 갯수가 0이 아니면 화면에 들어온 갯수를 설정창에  출력
// 	{
// 		//m_EditControl.SetSel(-1,0);
// 		//m_EditControl.ReplaceSel(size);
// 	}

	for(int i  = 0 ; i < iSize; i++) //들어온 갯수 만큼 데이터를 읽어 화면에 보여줌
	{
		(m_CommThread.m_QueueRead).GetByte(&aByte);//큐에서 데이터 한개를 읽어옴

		////////////////// End값 처리(2013.6.13) for LGMTB JHCHA //////////////////
		///////////////// 중간에 끊기는 데이터 처리 /////////////////////////
		if(aByte=='\r'){
		//if(aByte=='\/'){
			//resultDisp.Format("%s\r\n",m_ResultMessage);						//result값을 member 변수로 사용
			resultDisp.Format(_T("%s"),m_ResultMessage);							//result값을 member 변수로 사용
			if(b_GetInformation==TRUE)	ReviewReceivedData(resultDisp);			//result 결과 처리
			//ReviewReceivedData(resultDisp);			//result 결과 처리
			m_ResultMessage=_T("\0");												//result값 초기화
		}
		else{
			str.Format(_T("%c"), aByte);
			m_ResultMessage+=str;												//result값 누적
		}
	}	
	if(m_ResultMessage.GetLength()>255) m_ResultMessage=_T("\0");

	return 1;
}

///////////////////// Return Control ///////////////////////////////////////////////
BOOL CDialogTriggerCtrl::ReviewReceivedData(CString sRcvData)
{
	CString s_ReturnHead;
	CString s_ReturnCh;
	CString s_4bitdata;
	CString s_32bitData;
	CString s_32bitHigh,s_32bitLow;
	CString s_SetData;
	int n_getvalue;
	double d_getvalue;

	s_ReturnHead = sRcvData.Mid(0,2);

	if(s_ReturnHead==GET_RTN_TRGMODE)
	{
		s_ReturnCh=sRcvData.Mid(2,1);
		s_4bitdata=sRcvData.Mid(3,1);
		n_getvalue=(int)wcstol(s_4bitdata,NULL,16);
		//s_SetData.Format("Mode %d",n_getvalue);
		s_SetData.Format(_T("Mode %d"),(int)wcstol(s_4bitdata,NULL,16));
		if(s_ReturnCh=="0" && m_btn_trigger_status_enable0.GetValue()){		m_edit_trigger_mode0.Format(_T("%s"),s_SetData);}
		else if(s_ReturnCh=="1" && m_btn_trigger_status_enable1.GetValue()){	m_edit_trigger_mode1.Format(_T("%s"),s_SetData);}
		else if(s_ReturnCh=="2" && m_btn_trigger_status_enable2.GetValue()){	m_edit_trigger_mode2.Format(_T("%s"),s_SetData);}
		else if(s_ReturnCh=="3" && m_btn_trigger_status_enable3.GetValue()){	m_edit_trigger_mode3.Format(_T("%s"),s_SetData);}
	}
	else if(s_ReturnHead==GET_RTN_ENCCNT)
	{
		if(IsWindowVisible())
		{
			UpdateData(TRUE);
		}
		s_ReturnCh=sRcvData.Mid(2,1);
		s_32bitHigh=sRcvData.Mid(3,4);
		s_32bitLow=sRcvData.Mid(7,4);
		n_getvalue = (int)(((int)wcstol(s_32bitHigh,NULL,16)-32768)*65536+(int)wcstol(s_32bitLow,NULL,16));
		s_SetData.Format(_T("%d"),n_getvalue);
 		if(s_ReturnCh=="0" && m_btn_trigger_status_enable0.GetValue())			m_edit_trigger_encoder_count0 = n_getvalue;
 		else if(s_ReturnCh=="1" && m_btn_trigger_status_enable1.GetValue())	m_edit_trigger_encoder_count1 = n_getvalue;
 		else if(s_ReturnCh=="2" && m_btn_trigger_status_enable2.GetValue())	m_edit_trigger_encoder_count2 = n_getvalue;
 		else if(s_ReturnCh=="3" && m_btn_trigger_status_enable3.GetValue())	m_edit_trigger_encoder_count3 = n_getvalue;
		d_getvalue = (double)(((int)wcstol(s_32bitHigh,NULL,16)-32768)*65536+(int)wcstol(s_32bitLow,NULL,16))*(double)m_edit_trigger_encoder_resolution/1000.0;
		s_SetData.Format(_T("%10.4f"),d_getvalue);
		if(s_ReturnCh=="0")			m_edit_trigger_encoder_pos0 = d_getvalue;
		else if(s_ReturnCh=="1" && m_btn_trigger_status_enable1.GetValue())	m_edit_trigger_encoder_pos1 = d_getvalue;
		else if(s_ReturnCh=="2" && m_btn_trigger_status_enable2.GetValue())	m_edit_trigger_encoder_pos2 = d_getvalue;
		else if(s_ReturnCh=="3" && m_btn_trigger_status_enable3.GetValue())	m_edit_trigger_encoder_pos3 = d_getvalue;
	}
	else if(s_ReturnHead==GET_RTN_TRGCNT)
	{
		s_ReturnCh=sRcvData.Mid(2,1);
		s_32bitData=sRcvData.Mid(3,8);
		n_getvalue = (int)((UINT)wcstol(s_32bitData,NULL,16));
		s_SetData.Format(_T("%d"),n_getvalue);
		if(s_ReturnCh=="0" && m_btn_trigger_status_enable0.GetValue())			m_edit_trigger_trigge_count0 = n_getvalue;
		else if(s_ReturnCh=="1" && m_btn_trigger_status_enable1.GetValue())	m_edit_trigger_trigge_count1 = n_getvalue;
		else if(s_ReturnCh=="2" && m_btn_trigger_status_enable2.GetValue())	m_edit_trigger_trigge_count2 = n_getvalue;
		else if(s_ReturnCh=="3" && m_btn_trigger_status_enable3.GetValue())	m_edit_trigger_trigge_count3 = n_getvalue;
	}
	if(IsWindowVisible())
	{
		UpdateData(FALSE);
	}

	return TRUE;
}

void CDialogTriggerCtrl::GetInformation(UINT n_ch, UINT n_getid)
{
	CString SendCmdData;

//	b_GetInformation=TRUE;

	SendCmdData.Format(_T("/%s%x%x/"),HCMD_GETINFO,n_ch,n_getid);
	if(m_bTriggerConnection)
	{
		SendCommand(SendCmdData);
	}

	SendWait();
}

void CDialogTriggerCtrl::SendTriggerModeInfo()
{
	GetInformation(GET_MODE_CH0, GET_MODE_CHTRGSEL);
	GetInformation(GET_MODE_CH1, GET_MODE_CHTRGSEL);
	GetInformation(GET_MODE_CH2, GET_MODE_CHTRGSEL);
	GetInformation(GET_MODE_CH3, GET_MODE_CHTRGSEL);
}

void CDialogTriggerCtrl::SendTriggerCountInfo()
{
	GetInformation(GET_MODE_CH0, GET_MODE_TRGCNT);
	GetInformation(GET_MODE_CH1, GET_MODE_TRGCNT);
	GetInformation(GET_MODE_CH2, GET_MODE_TRGCNT);
	GetInformation(GET_MODE_CH3, GET_MODE_TRGCNT);
}

void CDialogTriggerCtrl::SendEncoderCountInfo()
{
	GetInformation(GET_MODE_CH0, GET_MODE_ENCCNT);
	GetInformation(GET_MODE_CH1, GET_MODE_ENCCNT);
	GetInformation(GET_MODE_CH2, GET_MODE_ENCCNT);
	GetInformation(GET_MODE_CH3, GET_MODE_ENCCNT);
}

void CDialogTriggerCtrl::SendTriggerModeInfo(int ch)
{
	GetInformation(ch + 10, GET_MODE_CHTRGSEL);
}

void CDialogTriggerCtrl::SendTriggerCountInfo(int ch)
{
	GetInformation(ch + 10, GET_MODE_TRGCNT);
}

void CDialogTriggerCtrl::SendEncoderCountInfo(int ch)
{
	GetInformation(ch + 10, GET_MODE_ENCCNT);
}

void CDialogTriggerCtrl::TriggerInfo(int ch)
{
	SendTriggerModeInfo(ch);
	SendTriggerCountInfo(ch);
	SendEncoderCountInfo(ch);
}


LRESULT CDialogTriggerCtrl::OnTriggerMessage(WPARAM para0, LPARAM para1)
{
	// TODO: Add your control notification handler code here
	int nEvent = (int)para0;

	switch(nEvent)
	{
	case TRIGGER_INFO_VIEW:
		int ch = (int)para1;
		if(m_btn_trigger_status_enable0.GetValue())
		{
			TriggerInfo(ch);
		}
		if(m_btn_trigger_status_enable1.GetValue())
		{
			TriggerInfo(ch);
		}
		if(m_btn_trigger_status_enable2.GetValue())
		{
			TriggerInfo(ch);
		}
		if(m_btn_trigger_status_enable3.GetValue())
		{
			TriggerInfo(ch);
		}
		break;
	}

	return 0;
}

BOOL CDialogTriggerCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
