// Dialog/DialogAFCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EMIControl.h"
#include "common_AF.h"
//#include "llc2.h"
#include "ui_cmd.h"
#include "atf_def.h"
#include "atf_lib.h"

#include "Include/_CtrlImageRect.h"
#include "include/_FuncModuleHal.h"
#include "include/_FuncModuleBasic.h"

#include "include/Runtime.h"
#include "include/PMACComm.h"

#include "Dialog/DialogInline.h"
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
#include "Dialog/DialogLogAlarm.h"

#include "EMIControlDlg.h"

#include "afxdialogex.h"


// move LLC to predefined position - called after pressing one of "#1", "#2" ... "#8" buttons
int CDialogAFCtrl::LlcMoveToObjective(int control)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	bool bRetLoc = false;						// return code for local functions
	int iErrCode = 0;							// error code
	int i = 0;									// index
	int iObjectiveNum = atf_getObjectiveNum();	// number of available objectives

	// find pressed button constant name and move LLC to requested objective position
	for (i = 0; i < iObjectiveNum; i++)
	{
		if (control == m_iLlcObjCtrlNum[i])
		{
			bRetLoc = atf_moveLlcToObjective(i, false, &iErrCode);
			if(!bRetLoc)
			{
				m_strlog.Format(_T("atf_moveLlcToObjective(), ErrorCode : %d"),iErrCode);
				pdlg->WriteLog(REVIEW_LOG,m_strlog);
			}
			break;
		}
	}
	return 0;
}

// change objective lens selection
bool CDialogAFCtrl::ObjChangeLocal (int obj)
{	
	static int ls_curr_obj = -1;
	bool bForce = false;
	int iRet = 0;
	bool bRet = true;

	if (obj<0) { obj = m_measure.objective; bForce=true; }
	if (obj>8) obj = 8;
	
	if (ls_curr_obj!=obj
		|| m_measure.objective != obj
		 || bForce) 
	{
		m_measure.objective = obj;
		iRet = WriteObjNum (&m_measure);

		if (iRet != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to set current objective. [ErrorNum=%d]\n", iRet);
			LOGMSG(logMsg, CLogger::LogLow);
		}

		// read current magnification
		m_measure.mag = 0;
		iRet = atf_ReadMagnification(m_measure.objective, (short*)(&m_measure.mag));
		if (iRet != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to get current objective magnification. [ErrorNum=%d]\n", iRet);
			LOGMSG(logMsg, CLogger::LogLow);
		}
	}

	if (m_af_parameter != INVALID_PANEL_HANDLE) 
	{
		if (ls_curr_obj!=obj
			|| bForce) 
		{
			if (ls_curr_obj>=0) 
			{
				
			}
		}
	}

	ls_curr_obj = obj;

	bRet = false;
	return bRet;
}

// ---------- laser related functions
//
// switch laser power ON/OFF - called after pressing "Laser On/Off" button
int CDialogAFCtrl::LaserOnOff (int panel,  int button)
{
	int iRet = 0;

	if(button)
	{	//	Changed to on.
		iRet = atf_EnableLaser();
		if(iRet != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to turn laser on. [ErrorNum=%d]\n", iRet);
			LOGMSG(logMsg, CLogger::LogLow);
		}
	}
	else
	{	//	Changed to off.
		iRet = atf_DisableLaser();
		if(iRet != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to turn laser off. [ErrorNum=%d]\n", iRet);
			LOGMSG(logMsg, CLogger::LogLow);
		}
	}
	return 0;
}

// activate additional portion of the GUI based on extra rights granted by the parameter panel
void CDialogAFCtrl::MainMoreParams ()
{
	if (m_more_param == 0) return;

	if (gs_sensor_type >= ATF5)
	{
		unsigned short usTmp = 0;
		short sTmp = 0;
		
		// check ABC availability
		if (gs_ABC_avail == -1)
		{
			ReadABCZDelta(0, &usTmp);
		}
		if (gs_ABC_avail && m_flag_cm)
		{
		}
		// check TINY availability
		if (gs_tiny_avail == -1)
		{
			ReadTinyZDelta(0, &sTmp);
		}
		if (gs_tiny_avail)
		{
		}
	}

	//if (GetControlBoolValue(pEventsActions)==true) 
	//{
	//}

	/*if (m_flag_cm && GetControlBoolValue(m_pCm)) 
	{
	}
	if (m_IsVideoAvailable) 
	{
	}
	if ((gs_sensor_type >= ATF5 || gs_sensor_type <= ATF4) && GetControlBoolValue(m_pAoiTest))
	{
	}
	if (gs_sensor_type >= ATF5)
	{
	}*/
}

// change laser mode to auto/manual - called after pressing laser "Auto/Manual" button
int CDialogAFCtrl::LcChange (int panel, int control)
{

	int button = 1;
	u_short laser[15];
	int iRet = 0;

	if (button) 
	{	// auto laser control
		iRet = atf_LaserTrackOn();
		if (iRet != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to turn the laser track on. [ErrorNum=%d]\n", iRet);
			LOGMSG(logMsg, CLogger::LogLow);
		}
	}
	else
	{	// manual laser control
		iRet = atf_LaserTrackOff();
		if (iRet != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to turn the laser track off. [ErrorNum=%d]\n", iRet);
			LOGMSG(logMsg, CLogger::LogLow);
		}
		else
		{
			if (laser[0] == 0) 
			{
				// don't write 0, this disables laser. Often laser is so low that system is unable to report current value
				laser[0]=1;
			}
			
			iRet = atf_WriteLaserPower(laser[0], 3);
			if (iRet != ErrOK)
			{
				sprintf(logMsg, "Error: Failed to set laser power. [ErrorNum=%d]\n", iRet);
				LOGMSG(logMsg, CLogger::LogLow);
			}
		}
	}
	return 0;
}

// updates GUI when auto focus mode is disabled
void CDialogAFCtrl::GuiAfUpdate (bool bEnabled) 
{
	m_bAfEnable = bEnabled;
	m_ZTrackingCnt = 1;	// checked back in timer routine

}


bool CDialogAFCtrl::WriteMotionLimitsIfEnabled(int button_motionlimit)
{
	if (button_motionlimit)
	{
		int iRet = atf_WriteMotionLimits(m_upper_lim, m_lower_lim, m_adaptive_lim);

		if (iRet != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to set motion limits. [ErrorNum=%d]\n", iRet);
			LOGMSG(logMsg, CLogger::LogLow);
			return false;
		}
	}
	return true;
}

void CDialogAFCtrl::CheckMotionCapability()
{
	// is analog motion allowed ?
	m_analog_move_sensor = 0;
	if (m_analog_only_sensor) 
	{
		// see if anlog motion is enabled
		int buffer_int = 0;
		int err = ReadAnalogMove(&buffer_int);
		if (err != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to get Analog Motion status. [ErrorNum=%d]\n", err);
			LOGMSG(logMsg, CLogger::LogLow);
			strcat(logMsg, "There is a problem with the COM port - re-start application.");
			return;
		}
		m_analog_move_sensor = buffer_int ?  1 : 0;
	}

	// is motion allowed ?
	m_analog_or_ctl_move_sensor = 0;
	if ((m_mfc_conf&FpgaConfig_MtrCtl)
		|| m_analog_move_sensor)
	{
		m_analog_or_ctl_move_sensor = 1;
	}

	// adjust GUI to match 
}


// get and update motor parameters: SP, AC, DC and SF
void CDialogAFCtrl::getLlcParameters()
{
	bool bRetLoc = false;	// return code for local functions
	int iErrCode = 0;		// error code
	int iLlcParameter = 0;	// motor parameter value

	// display motor speed
	bRetLoc =  atf_getLlcSpeed(&iLlcParameter, &iErrCode);
	if (bRetLoc == true)
	{
		LOGMSG2(CLogger::LogLow, "%s%d%s", "Info: Motor speed = ", iLlcParameter, ".\n");
	}
	else
	{
		LOGMSG2(CLogger::LogLow, "%s%d%s", "Error: Failed to get motor speed. [ErrNum=", iErrCode, "]\n");
	}

	// display acceleration
	bRetLoc = atf_getLlcAcceleration(&iLlcParameter, &iErrCode);
	if (bRetLoc == true)
	{
		LOGMSG2(CLogger::LogLow, "%s%d%s", "Info: Motor acceleration = ", iLlcParameter, ".\n");
	}
	else
	{
		LOGMSG2(CLogger::LogLow, "%s%d%s", "Error: Failed to get motor acceleration. [ErrNum=", iErrCode, "]\n");
	}

	// display deceleration
	bRetLoc = atf_getLlcDeceleration(&iLlcParameter, &iErrCode);
	if (bRetLoc == true)
	{
		LOGMSG2(CLogger::LogLow, "%s%d%s", "Info: Motor deceleration = ", iLlcParameter, ".\n");
	}
	else
	{
		LOGMSG2(CLogger::LogLow, "%s%d%s", "Error: Failed to get motor deceleration. [ErrNum=", iErrCode, "]\n");
	}

	// display smooth factor
	bRetLoc = atf_getLlcSmoothFactor(&iLlcParameter, &iErrCode);
	if (bRetLoc == true)
	{
		LOGMSG2(CLogger::LogLow, "%s%d%s", "Info: smooth factor = ", iLlcParameter, ".\n");
	}
	else
	{
		LOGMSG2(CLogger::LogLow, "%s%d%s", "Error: Failed to get motor smooth factor. [ErrNum=", iErrCode, "]\n");
	}
}

// evaluate config file parameter against firmware version
bool CDialogAFCtrl::IsVersionOK(char *pVersion)
{


	char *pTmp = NULL;
	bool bRet = true;

	sprintf(logMsg, "Info: Version String = %s.\n", pVersion);
	LOGMSG(logMsg, CLogger::LogLow);

	// check for conditional character
	pTmp = pVersion + 3;

	// check if condition contains two characters
	if (*(pTmp + 1) != '=' && *(pTmp + 1) != '>' && *(pTmp + 1) != '<')
	{
		// one char condition 
		switch (*pTmp)
		{
			case '=':	// if (CurrentFirmwareVersion = SpecifiedFirmwareVersion)
				if (atoi(m_FirmwareVer) != atoi(pTmp + 1))
				{
					bRet = false;
				}
				break;

			case '>':	// if (CurrentFirmwareVersion > SpecifiedFirmwareVersion)
				if (atoi(m_FirmwareVer) <= atoi(pTmp + 1))
				{
					bRet = false;
				}
				break;
			case '<':	// if (CurrentFirmwareVersion < SpecifiedFirmwareVersion)
				if (atoi(m_FirmwareVer) >= atoi(pTmp + 1))
				{
					bRet = false;
				}
				break;
		}
	}
	else
	{
		// two char condition
		if ((_strnicmp(pTmp, "<=", 2) == 0 || _strnicmp(pTmp, "=<", 2) == 0) &&	// if (CurrentFirmwareVersion <= SpecifiedFirmwareVersion)
			(atoi(m_FirmwareVer) > atoi(pTmp + 2)))
		{
			bRet = false;
		}
		else if ((_strnicmp(pTmp, ">=", 2) == 0 || _strnicmp(pTmp, "=>", 2) == 0) &&	// if (CurrentFirmwareVersion >= SpecifiedFirmwareVersion)
				(atoi(m_FirmwareVer) < atoi(pTmp + 2)))
		{
			bRet = false;
		}
	}
	return bRet;
}

// get sensor information, like: sensor type, firmware version, serial number etc.
void CDialogAFCtrl::GetSensorInformation(void)
{


	int buffer_int = 0;
	int iMfcConfig = 0;
	unsigned int buffer_uint = 0;	
	char buffer_string[256];
	char *p_buffer_string = buffer_string;

	int left = 0;
	int width = 0;
	int iRet = 0;
	int iErrCode = 0;

	// clear the flags at the begining
	m_flag_split_cm = m_flag_cm = m_flag_ao = 0;
	

	//	Firmware version.
	iRet = atf_ReadFirmwareVer(&buffer_int);

	if (iRet != ErrOK)
	{
		sprintf(logMsg, "Error: Failed to get sensor firmware version. [ErrorNum=%d]\n", iRet);
		LOGMSG(logMsg, CLogger::LogLow);
	}
	else
	{
		sprintf(m_FirmwareBuffer, "%d.%02x.%02d.%02d", (buffer_int >> 24), ((buffer_int >> 16) & 0xff), ((buffer_int >> 8) & 0xff), (buffer_int & 0xff));
		sprintf(m_FirmwareVer,     "%d%02d%02d%02d",    (buffer_int >> 24), ((buffer_int >> 16) & 0xff), ((buffer_int >> 8) & 0xff), (buffer_int & 0xff));

		sprintf(logMsg, "Info: Sensor Firmware Version = %s\n", m_FirmwareBuffer);
		LOGMSG(logMsg, CLogger::LogLow);
	}
	// get sensor type (i.e. ATF5,6,7)
	gs_sensor_type = ReadSensorType();	// if not 5 or 6 than determine sensor type from firmware version

	if (gs_sensor_type < 1) 
	{
		// older firmware versions
		int ver = buffer_int >> 24;

		switch (ver)
		{
		case 5:
			if (((buffer_int >> 16)&0xff)>0x0c) gs_sensor_type = ATF5y;	// ATF5 old single window, same as merged
			else								gs_sensor_type = ATF5x;	// ATF5 old version
			break;
		case 6:
			gs_sensor_type = ATF6;	// ATF6 old version
			break;
		default:
			gs_sensor_type = ATFX;	// unknown sensor type
			sprintf(logMsg, "Error: Failed to detect sensor type.\n");
			LOGMSG(logMsg, CLogger::LogLow);
		}
	}

	m_pSensorName = gs_sensor_type==ATF2 ? m_pATF2 : 
				 (gs_sensor_type == ATF4) ? m_pATF4 : 
				  (gs_sensor_type >= ATF5x && gs_sensor_type < ATF6) ? m_pATF5 : 
				   (gs_sensor_type == ATF6) ? m_pATF6 : 
				    (gs_sensor_type >= ATF7) ? m_pATF7 : 
					 m_pATFX;
	sprintf(logMsg, "Info: Sensor Type = %s\n", m_pSensorName);

	LOGMSG(logMsg, CLogger::LogLow);

		//	Sensor serial number.
	iRet = atf_ReadSerialNumber(&buffer_uint);

	if (iRet != ErrOK)
	{
		sprintf(logMsg, "Error: Failed to get sensor serial number. [ErrorNum=%d]\n", iRet);
		LOGMSG(logMsg, CLogger::LogLow);
	}
	else
	{
		sprintf(p_buffer_string, "00%c %04d", 
			   (gs_sensor_type == ATF2 ? '2' :
			    (gs_sensor_type == ATF4 ? '4' :
			     (gs_sensor_type >= ATF5x && gs_sensor_type < ATF6 ? '5' : 
			      (gs_sensor_type == ATF6 ? '6' : 
			       (gs_sensor_type >= ATF7 ? '7' : ATFX))))), 
			   buffer_uint);

		sprintf(logMsg, "Info: Sensor Serial Number = %s\n", p_buffer_string);
		LOGMSG(logMsg, CLogger::LogLow);

		sprintf(p_buffer_string, "%s S/N:", m_pSensorName);
	}

	iRet = ReadModel(&buffer_int);

	if (iRet != ErrOK)
	{
		sprintf(logMsg, "Error: Failed to get sensor model info. [ErrorNum=%d]\n", iRet);
		LOGMSG(logMsg, CLogger::LogLow);
	}

	switch(buffer_int & 0xff)
	{
	case 0:
	case 2:
		iRet = ReadMfcConfig(&iMfcConfig);
		if (iRet != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to get MFC configuration to determine sensor model. [ErrorNum=%d]\n",iRet);
			LOGMSG(logMsg, CLogger::LogLow);
			sprintf(p_buffer_string, "Unknown");
		}
		else
		{
			if (iMfcConfig & FpgaConfig_Analog) 
			{
				sprintf(p_buffer_string, "Analog Output (SA)");
				m_flag_ao = 1;
			}
			else
			{
				sprintf(p_buffer_string, "SYS / AOI");
			}
		}

		m_flag_cm = 0;
		break;
	
	case 1:
		m_flag_split_cm = IsSplitOptics();
		sprintf(p_buffer_string, "Cell Module%s", m_flag_split_cm ? " - Split" : "");

		m_flag_cm = 1;		        
		break;
	}

	sprintf(logMsg, "Info: Sensor Model = %s\n", p_buffer_string);
	LOGMSG(logMsg, CLogger::LogLow);

	// read separation, distance between peaks
	//ReadSeparation (&m_separation, &m_separationOption);
}

// ---------- objective related functions
//
// change objective lens - called after pressing objective ring button
int CDialogAFCtrl::ObjChangeM (int control, int eventData1, int eventData2)
{

	int iLlcStat = -1;
	int  obj = control;

//	if (event != EVENT_COMMIT) return 0;

	// get obj #
	if (ObjChangeLocal (obj))
	{
		sprintf(logMsg, "Error: Failed to set current objective.");
		LOGMSG(logMsg, CLogger::LogLow);
	}

	int llc_num = control + 1;//

	// move LLC
	if (llc_num)
	{
		LlcMoveToObjective(m_iLlcObjCtrlNum[llc_num - 1]);
	}
	return 0;
}
// CDialogAFCtrl 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogAFCtrl, CDialogEx)

CDialogAFCtrl::CDialogAFCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogAFCtrl::IDD, pParent)
	, m_edit_af_microstep(0)
	, m_edit_af_fullstep(0)
	, m_edit_af_obj_num(0)
	, m_edit_af_infocusrange(0)
	, m_edit_af_max_acc(0)
	, m_edit_af_max_vel(0)
	, m_edit_ez_spd(60000)
	, m_edit_ez_move_pos(0)
	, m_radio_review(0)
	, m_edit_llc_offset0(0)
	, m_edit_llc_offset1(0)
	, m_edit_llc_offset2(0)
{
	m_iErrOk = 0;

	m_patf5 = "atf5";
	m_pATF4 = "ATF4";
	m_pATF5 = "ATF5";
	m_patf6 = "atf6";
	m_pATF6 = "ATF6";
	m_pATF7 = "ATF7";
	m_pATFX = "ATFX";
	m_pATF2 = "ATF2";
	m_pSensorName = 0;	
	m_FirmwareVer[0] = NULL;

	m_iLlcObjCtrlNum[0] = 1;
	m_iLlcObjCtrlNum[1] = 2;
	m_iLlcObjCtrlNum[2] = 3;
	m_GuiAutoOff = false;
	m_bAfEnable = false;
	m_IsVideoAvailable = false;
	m_IsRetarderPresent = false;
	m_ZTrackingCnt = 0;

	// panel handles
	m_cvdaf = INVALID_PANEL_HANDLE;			// cvd af panel handle

	// CVD AF
	m_cvd_numset = 2;
	m_cvd_rolling_not = 1;
	// motion limit
	m_upper_lim = 1000;
	m_lower_lim = -1000;
	m_adaptive_lim = 0;
	m_lEzdistance_cumulative = 0;
	m_bEzMoterOrigin = FALSE;
}

CDialogAFCtrl::~CDialogAFCtrl()
{
	SaveAFCtrl(AFCTRL_PATH);
	SaveAFLLCOffset(AFCTRL_PATH);
}

void CDialogAFCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_AF_MICRO_STEP, m_edit_af_microstep);
	DDX_Text(pDX, IDC_EDIT_AF_MAX_VEL2, m_edit_af_fullstep);
	DDX_Text(pDX, IDC_EDIT_AF_OBJ_NUM, m_edit_af_obj_num);
	DDX_Text(pDX, IDC_EDIT_AF_INFOCUS_RANGE, m_edit_af_infocusrange);
	DDX_Text(pDX, IDC_EDIT_AF_MAX_ACC, m_edit_af_max_acc);
	DDX_Text(pDX, IDC_EDIT_AF_MAX_VEL, m_edit_af_max_vel);
	DDX_Control(pDX, IDC_BTNENHCTRL_AF_INRANGE, m_btn_af_inrange);
	DDX_Control(pDX, IDC_BTNENHCTRL_AF_INFOCUS, m_btn_af_infocus);
	DDX_Control(pDX, IDC_BTNENHCTRL_AF_LASERON, m_btn_af_laseron);
	DDX_Control(pDX, IDC_BTNENHCTRL_AF_LASERONE, m_btn_af_laserone);
	DDX_Control(pDX, IDC_BTNENHCTRL_AF_TRACK_RUN, m_btn_af_track_run);
	DDX_Control(pDX, IDC_BTNENHCTRL_AF_MAKE0, m_btn_af_make0);
	DDX_Control(pDX, IDC_BTNENHCTRL_AF_SAVE, m_btn_af_save);
	DDX_Control(pDX, IDC_BTNENHCTRL_AF_POS1, m_btn_af_pos1);
	DDX_Text(pDX, IDC_EDIT_EZ_SPD, m_edit_ez_spd);
	DDX_Text(pDX, IDC_EDIT_EZ_MOVE_POS, m_edit_ez_move_pos);
	DDX_Control(pDX, IDC_BTNENHCTRL_EZ_POS0, m_btn_ez_pos);
	DDX_Control(pDX, IDC_BTNENHCTRL_AF_AOI_TRACK_RUN, m_btn_af_aoi_track_run);
	DDX_Control(pDX, IDC_BTNENHCTRL_AF_CONNECT1, m_btn_af_connect);
	DDX_Radio(pDX, IDC_RADIO_REVIEW1, m_radio_review);
	DDX_Control(pDX, IDC_BTNENHCTRL_EZ_CUMULATIVE_DISTANCE, m_btn_ez_cumulative_distance);
	DDX_Text(pDX, IDC_EDIT_LLCOFFSET0, m_edit_llc_offset0);
	DDX_Text(pDX, IDC_EDIT_LLCOFFSET1, m_edit_llc_offset1);
	DDX_Text(pDX, IDC_EDIT_LLCOFFSET2, m_edit_llc_offset2);
	DDX_Control(pDX, IDC_BTNENHCTRL_EZ_HOME0, m_btnEzOrigin);
	DDX_Control(pDX, IDC_BTNENHCTRL_EZ_SPD1, m_btn_ez_cycle_oiling);
}


BEGIN_MESSAGE_MAP(CDialogAFCtrl, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_AF_MICRO_STEP, &CDialogAFCtrl::OnEnChangeEditAfMicroStep)
	ON_EN_CHANGE(IDC_EDIT_AF_MAX_VEL2, &CDialogAFCtrl::OnEnChangeEditAfMaxVel2)

	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_EZ_SPD, &CDialogAFCtrl::OnEnChangeEditEzSpd)
	ON_BN_CLICKED(IDC_RADIO_REVIEW1, &CDialogAFCtrl::OnBnClickedRadioReview1)
	ON_BN_CLICKED(IDC_RADIO_REVIEW2, &CDialogAFCtrl::OnBnClickedRadioReview2)
	ON_BN_CLICKED(IDC_RADIO_REVIEW3, &CDialogAFCtrl::OnBnClickedRadioReview3)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogAFCtrl 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDialogAFCtrl, CDialogEx)
	
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_AF_CONNECT1, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlAfConnect1, VTS_NONE)	
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_AF_JOG_P1, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlAfJogP1, VTS_NONE)
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_MOT_JOG_M4, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlMotJogM4, VTS_NONE)	
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_AF_STOP1, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlAfStop1, VTS_NONE)	
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_AF_HOME1, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlAfHome1, VTS_NONE)

	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_EZ_HOME0, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlEzHome0, VTS_NONE)
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_EZ_JOG_P0, DISPID_MOUSEDOWN, CDialogAFCtrl::MouseDownBtnenhctrlEzJogP0, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_EZ_JOG_P0, DISPID_MOUSEUP, CDialogAFCtrl::MouseUpBtnenhctrlEzJogP0, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_EZ_JOG_M0, DISPID_MOUSEDOWN, CDialogAFCtrl::MouseDownBtnenhctrlEzJogM0, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_EZ_JOG_M0, DISPID_MOUSEUP, CDialogAFCtrl::MouseUpBtnenhctrlEzJogM0, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_EZ_MOVE0, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlEzMove0, VTS_NONE)
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_EZ_STOP0, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlEzStop0, VTS_NONE)
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_EZ_CONNECT0, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlEzConnect0, VTS_NONE)
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_AF_TRACK_RUN, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlAfTrackRun, VTS_NONE)
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_AF_LASERONE, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlAfLaserone, VTS_NONE)
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_AF_AOI_TRACK_RUN, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlAfAoiTrackRun, VTS_NONE)
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_AF_LASERON, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlAfLaseron, VTS_NONE)
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_AF_MAKE0, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlAfMake0, VTS_NONE)
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_EZ__DISCONNECT0, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlEz, VTS_NONE)
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_AF__DISCONNECT1, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlAf, VTS_NONE)
	
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_LLCAPPLY_, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlLlcapply, VTS_NONE)
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_EZ_SPD_SET0, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlEzSpdSet0, VTS_NONE)
	ON_EVENT(CDialogAFCtrl, IDC_BTNENHCTRL_EZ_OIL_COMP, DISPID_CLICK, CDialogAFCtrl::ClickBtnenhctrlEzOilComp, VTS_NONE)
END_EVENTSINK_MAP()

BOOL CDialogAFCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	UpdateData(TRUE);

	m_nPort = 9;  //Ezi
	m_lEzdistance_old = 0;
	m_lEzdistance_new = 0;

	m_infocus[0] = 3;
	m_acc[0] = 80;
	m_fullstep[0] = 400.0f;
	m_microstep[0] = 16;
	m_mag[0] = 2;
	m_speed[0] = 8.000000;
	m_slope[0] = 8.250000;
	m_p[0] = 0.799805;

	m_infocus[1] = 10;
	m_acc[1] = 80;
	m_fullstep[1] = 400.0f;
	m_microstep[1] = 16;
	m_mag[1] = 10;
	m_speed[1] = 8.000000;
	m_slope[1] = 0.369141;
	m_p[1] = 0.799805;

	m_infocus[2] = 15;
	m_acc[2] = 80;
	m_fullstep[2] = 400.0f;
	m_microstep[2] = 16;
	m_mag[2] = 20;
	m_speed[2] = 8.000000;
	m_slope[2] = 0.092773;
	m_p[2] = 0.799805;
		

	m_bConnected = FALSE;
	m_nPort = 9;
	m_iSlaveNo = 0;

	//m_nCmdPosition = 10000;
	m_lMoveVelocity = 10000;

	m_edit_af_infocusrange = m_infocus[0];
	m_edit_af_max_acc = m_acc[0];
	m_edit_af_max_vel = m_speed[0];
	m_edit_af_fullstep = m_fullstep[0];
	m_edit_af_microstep = m_microstep[0];

	m_lEzdistance_cumulative_mm = 0;
	for(int i = 0; i <= OBJ_NUM_3 ; i++)
	{
		m_illcoffset[i] = 0;
	}
	

	LoadAFCtrl(AFCTRL_PATH);
	LoadAFLLCOffset(AFCTRL_PATH);

	m_illcoffset[0] = m_edit_llc_offset0;
	m_illcoffset[1] = m_edit_llc_offset1;
	m_illcoffset[2] = m_edit_llc_offset2;

	m_lEzMaxdistance = 100000000000;
	m_Curoliling = 213;
	
	
	UpdateData(FALSE);
#ifdef MACHINE
	ClickBtnenhctrlAfConnect1();
	ClickBtnenhctrlEzConnect0();
	
#endif
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogAFCtrl::CurrentObjInfo()
{
	/*
	int iRet;

	if((iRet = atf_ReadObjNum(&m_cur_objective)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadObjNum"));
	}
	if((iRet = atf_ReadMagnification(m_cur_objective,&m_cur_mag)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadMagnification"));
	}
	if((iRet = atf_ReadInfocusRange(m_cur_objective,&m_cur_infocus)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadInfocusRange"));
	}
	if((iRet = atf_ReadP(m_cur_objective,&m_cur_readp)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadP"));
	}
	if((iRet = atf_ReadAccel(m_cur_objective,&m_cur_accel)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadAccel"));
	}
	if((iRet = atf_ReadSpeed(m_cur_objective,&m_cur_speed)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadSpeed"));
	}
	if((iRet = atf_ReadStepPerMmConversion(&m_cur_steppermm)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadStepPerMmConversion"));
	}
	if((iRet = atf_ReadMicrostep(&m_cur_microstep)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadMicrostep"));
	}
	if((iRet = atf_ReadDuvJump(m_cur_objective,&m_cur_duvjump)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadDuvJump"));
	}
	if((iRet = atf_ReadLastMoveZ(m_cur_readlastmovez)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadDuvJump"));
	}
	if((iRet = atf_ReadAbsZPos(&m_cur_abszpos)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadDuvJump"));
	}
	*/
	/*
	if((iRet = ATF_ReadObjNum(&m_cur_objective)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadObjNum"));
	}
	if((iRet = ATF_ReadMagnification(m_cur_objective,&m_cur_mag)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadMagnification"));
	}
	if((iRet = ATF_ReadInfocusRange(m_cur_objective,&m_cur_infocus)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadInfocusRange"));
	}
	if((iRet = ATF_ReadP(m_cur_objective,&m_cur_readp)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadP"));
	}
	if((iRet = ATF_ReadAccel(m_cur_objective,&m_cur_accel)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadAccel"));
	}
	if((iRet = ATF_ReadSpeed(m_cur_objective,&m_cur_speed)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadSpeed"));
	}
	if((iRet = ATF_ReadStepPerMmConversion(&m_cur_steppermm)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadStepPerMmConversion"));
	}
	if((iRet = ATF_ReadMicrostep(&m_cur_microstep)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadMicrostep"));
	}
	if((iRet = ATF_ReadDuvJump(m_cur_objective,&m_cur_duvjump)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadDuvJump"));
	}
	if((iRet = ATF_ReadLastMoveZ(&m_cur_readlastmovez)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadDuvJump"));
	}
	if((iRet = ATF_ReadAbsZPos(&m_cur_abszpos)) != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadDuvJump"));
	}*/

}

void CDialogAFCtrl::ClickBtnenhctrlAfConnect1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iPortConnect;
	int iPortSpeed = 9600;
	char *cPort = {"COM8"};
	int iRet = 0, iMfcConfig;
	MeasureDataType m_measure;
	int err = 0;
	int buffer_int = 0;
	int iErrCode = 0;
	short buffer_short = 0;
	unsigned short buffer_ushort = 0;
	double buffer_double = 0.0f;
	double temp3 = 0.;
	BOOL bRet = FALSE;

	
	
	//if(m_btn_af_connect.GetValue())
	//{
		iRet = atf_isSerialConnection();
		atf_CloseConnection();
		iPortConnect = atf_OpenConnection(cPort, iPortSpeed);
		iRet = ::ReadPosition(&m_measure);

		iRet = RecoverSensor();
		if(0 == iRet)
		{ 
			sprintf(logMsg, 
					"Error: Failed to connect to the sensor. [ComPortName=%s][BaudRate=%d][ErrorNum=%d]\n", 
					cPort, iPortSpeed, iRet);
			LOGMSG(logMsg, CLogger::LogLow);
			atf_CloseConnection();
		}
		else
		{
			sprintf(logMsg, "Warning: Sensor connection has been restored.\n");
			LOGMSG(logMsg, CLogger::LogMid);
		}

		GetSensorInformation();

		if (m_IsRetarderPresent = (ReadCmRetarderStatus () != CmRetarderStatusUnavailable)) 
		{
			/*/ in this case button is taken over by polarization control
			// AL: - don't use "AF Auto Polar" for now. we need to test this feature more to be sure it works
			SetCtrlAttribute(m_main, PANELM_TOGGLEBUTTON_AFVIDEO, ATTR_VISIBLE, CTRL_VISIBLE);
			SetCtrlAttribute(m_main, PANELM_TOGGLEBUTTON_AFVIDEO, ATTR_LABEL_TEXT, "AF Auto Polar.");
			//*/
		}

		if (( gs_sensor_type == ATF6 || gs_sensor_type == ATF5))
		{
			m_IsVideoAvailable = true;
		}
		err = atf_AfStop();
		if (err != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to turn autofocus off. [ErrorNum=%d]\n", err);
			LOGMSG(logMsg, CLogger::LogLow);
			if(err != ErrOperFailed)
			{
				strcat(logMsg, "There is a problem with the COM port - re-start application.");
			}
		}

		//	Read step resolution.
		err = atf_ReadStepPerMmConversion(&buffer_ushort);
		if (err != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to get motor steps per mm value. [ErrorNum=%d]\n", err);
			LOGMSG(logMsg, CLogger::LogLow);
			strcat(logMsg, "There is a problem with the COM port - re-start application.");
		}

		m_step_per_mm = (float) buffer_ushort;

		err = atf_ReadMicrostep(&buffer_ushort);
		if (err != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to get motor micro-stepping value. [ErrorNum=%d]\n", err);
			LOGMSG(logMsg, CLogger::LogLow);
			strcat(logMsg, "There is a problem with the COM port - re-start application.");
		}

		m_step_per_mm *= (float)buffer_ushort;

		err = ReadP(&temp3, m_measure.objective);
		if (err != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to get value of gain P. [ErrorNum=%d]\n", err);
			LOGMSG(logMsg, CLogger::LogLow);
			strcat(logMsg, "There is a problem with the COM port - re-start application.");
		}

		if (temp3 < 0)
		{
			m_step_per_mm = -m_step_per_mm;
		}

		err = ReadLLCComCorrOption(&buffer_int);
		if (err != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to get Whistle configuration. [ErrorNum=%d]\n", err);
			LOGMSG(logMsg, CLogger::LogLow);
		}
		else
		{
			sprintf(logMsg, "Info: Whistle Configuration = 0x%X.\n", buffer_int);
			LOGMSG(logMsg, CLogger::LogLow);
		}

		CRect rect;
	

		err = ReadMfcConfig (&m_mfc_conf);
		if (err != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to get MFC configuration. [ErrorNum=%d]\n", err);
			LOGMSG(logMsg, CLogger::LogLow);
			strcat(logMsg, "There is a problem with the COM port - re-start application.");
		}
		else
		{
			sprintf(logMsg, "Info: FPGA Configuration = 0x%X.\n", m_mfc_conf);
			LOGMSG(logMsg, CLogger::LogLow);
		}

		if ((m_mfc_conf & FpgaConfig_Mfc_Mcm) && 
			(m_mfc_conf & FpgaConfigMfc_Whistle))
		{
			// mfc w/ LLC
			u_short sErrorSettings[10];

			char cOffsetFileDir[MAX_PATH] = {'\0'};			// objective position offset file directory path
			FILE *pFileTmp = NULL;							// temp file handle
			bool bRetLoc = false;							// return code for local functions
			bool bIsVisible = true;							// is panel visible flag
			bool *pCallbackData = false;	// data passed to function - true = make LLC panel visible, false = have LLC panel hidden
			char *pTmp = NULL;								// temp pointer
			int iErrCode = 0;								// error code
			int iHdwrObjNum = 0;							// number of objectives saved in LLC
			int iCnfgObjNum = 0;							// number of objectives taken from configuration file
			int iCurrObjNum = 0;							// final number of objectives
			int i = 0;										// index
			int iObjPosOffset = 0;
			//OpenLlc(m_mb, MB_MB_LLC, (void *)&bIsVisible, 1);
			bRetLoc = atf_initializeLlc(pTmp, &iErrCode);
			// test for errors
			if (bRetLoc == false)
			{
				// LLC initialization failed
				sprintf(logMsg, "Error: Failed to initialize LLC. [ErrorNum=%d]\n", iErrCode);
				LOGMSG(logMsg, CLogger::LogLow);
			}
			// initialization succeded
			m_bIsLlcInit = true;
			UpdateData(TRUE);
			// get and set number of available objectives
			iHdwrObjNum = atf_getObjectiveNum();
			iCnfgObjNum = m_edit_af_obj_num;//GetControlIntValue(m_pNumberOfLenses, 8);
			if (iCnfgObjNum < 0) iCnfgObjNum = 0;
			if (iCnfgObjNum > 8) iCnfgObjNum = 8;
		
			if (iHdwrObjNum > 0 && iCnfgObjNum > 0)
			{
				iCurrObjNum = min(iHdwrObjNum, iCnfgObjNum);
				atf_setObjectiveNum(iCurrObjNum);
			}
			else if (iHdwrObjNum == 0 && iCnfgObjNum > 0)
			{
				iCurrObjNum = iCnfgObjNum;
				atf_setObjectiveNum(iCurrObjNum);
			}
			else
			{
				iCurrObjNum = iHdwrObjNum;
			}

			LOGMSG2(CLogger::LogLow, "%s%d%s", "Info: Number of available objectives = ", iCurrObjNum, ".\n");

			// display objective position offsets
			for (i = 0; i < iCurrObjNum; i++)
			{
				bRetLoc = atf_getLlcObjectivePositionOffset(i, &m_illcoffset[i]);
				if (bRetLoc = true)
				{
					LOGMSG2(CLogger::LogLow, "%s%d%s%d%s", "Info: Objective ", i, " position offset = ", iObjPosOffset, ".\n");
				}
				else
				{
					LOGMSG2(CLogger::LogLow, "%s%d%s%d%s", "Error: Failed to get objective ", i, " position offset. [ErrNum=", iErrCode, "]\n");
				}
			}

			// get LLC command delay constant
			GetLLCCharDelay();
			// get and display on the GUI panel LLC parameters
			getLlcParameters();

			err = g_epp.ReadArray ((void*)&sErrorSettings, 't', 173, DtWordEnm, 9);
			if (err != ErrOK)
			{
				if (err == ErrOperFailed)
				{
					sprintf(logMsg, "Info: LLC statistics not supported.\n");
					LOGMSG(logMsg, CLogger::LogLow);
				}
				else
				{
					sprintf(logMsg, "Error: Failed to set LLC statistics. [ErrorNum=%d]\n", err);
					LOGMSG(logMsg, CLogger::LogLow);
				}
			}
			else
			{
				sprintf(logMsg,
						"Info: LLC statistics (old): "
						"[Config = %d; State = %d; "
						"Wr Del = %d; Rd Del = %d;] "
						"[Char counts: lost(%d), rpt(%d), ok(%d), extra(%d), dropped(%d)]\n",
						sErrorSettings[0],sErrorSettings[1],sErrorSettings[2],sErrorSettings[3],sErrorSettings[4],
						sErrorSettings[5],sErrorSettings[6],sErrorSettings[7],sErrorSettings[8]);
				LOGMSG(logMsg, CLogger::LogLow);
			}
		}
		///////////comp

		// see if sensor is anlog
		m_analog_only_sensor = 0;
		if (m_mfc_conf&FpgaConfig_Analog)			// analog sensor
		{
			m_analog_only_sensor  = 1;
		}

		CheckMotionCapability();


		// clear button is not a good idea for analog motion since it is ignored by fw
		if (m_analog_move_sensor)
		{
		}

		// read autotransit mode
		err = ReadWindowCurrMode(&m_win_mode);
		if (err != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to get window current mode. [ErrorNum=%d]\n", err);
			LOGMSG(logMsg, CLogger::LogLow);
			strcat(logMsg, "There is a problem with the COM port - re-start application.");
		}
		if (m_win_mode==eWM_Error) 
		{
			m_win_mode = eWM_Auto;
			err = WriteWindowCurrMode (eWM_Auto); 
			if (err != ErrOK)
			{
				sprintf(logMsg, "Error: Failed to set window current mode. [ErrorNum=%d]\n", err);
				LOGMSG(logMsg, CLogger::LogLow);
				strcat(logMsg, "There is a problem with the COM port - re-start application.");
			}
		}

		// adopt GUI to auto off
		err = IsAutoOffEnabled(&m_GuiAutoOff);
		if (err != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to get auto off feature state. [ErrorNum=%d]\n", err);
			LOGMSG(logMsg, CLogger::LogLow);
			strcat(logMsg, "There is a problem with the COM port - re-start application.");
		}


		// see if AF is on, and if so set a button
		int flags;
		err = ReadExecFlag (&flags);
		if (err != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to get auto focus current state. [ErrorNum=%d]\n", err);
			LOGMSG(logMsg, CLogger::LogLow);
			strcat(logMsg, "There is a problem with the COM port - re-start application.");
		}

		// check laser auto/manual mode
		if ((flags & (MsLaserTracking|MsSoftLaserTracking)) == 0)
		{
			// laser is in manual mode - update controls accordingly
			LcChange(m_main, PANELM_TOGGLEBUTTON_LC);
		}

		// check laser enabled/disabled mode
		// laser is disabled - update controls accordingly

		// enable homing if defined

		// enable jogging if defined


		MainMoreParams ();

		/*
		if(iPortConnect = ATF_OpenConnection(cPort,iPortSpeed) != m_iErrOk)
		{
			::AfxMessageBox(_T("Error : not Open Connect"));
		}
		iRet = ATF_ReadMfcConfig(&iMfcConfig);
		*/
		//ATF_DisableLaser();

		//iRet = ::atf_EnableLaser();
		BOOL bWaitForEnd = FALSE;
		BOOL bRetLlcHome = FALSE;
		/*
		bRetLlcHome = atf_homeLlc(bWaitForEnd,&iErrCode);
		if(!bRetLlcHome)
		{
			AfxMessageBox(_T("LLC dose not home"));
		}
		if(iErrCode != m_iErrOk)
		{
			AfxMessageBox(_T("Error : %d LLC home fail"),iErrCode);
		}
		Sleep(2000);
		bRetLlcHome = atf_isLlcInHomePos(&iRet);
		if(!bRetLlcHome)
		{
			AfxMessageBox(_T("LLC home position is not reached"));
		}
		*/
		if(m_btn_af_connect.GetValue())
		{
			ObjChangeM(OBJ_NUM_1, 0, 0);
			AFinfocusrangestate();
		}

		
		int iLaserRet = m_btn_af_laseron.GetValue();
		iRet = LaserOnOff(m_main, TRUE);

		iRet = ReadObjNum (&m_measure);

		MeasureDataType m_measure2;
		//short status;
		//iRet = ::atf_ReadStatus((short*)&m_measure.status);
		//m_measure.infocus = 0;
		//iRet = WriteInfoc(&m_measure);
		//m_measure2.objective = m_measure.objective;
		//iRet = ReadInfoc(&m_measure);
		

		if(bRet = AFlaseronoffstate())
			m_btn_af_laseron.SetValue(TRUE);
		else
			m_btn_af_laseron.SetValue(FALSE);

	/*	
	}
	else
	{
		iRet = LaserOnOff(m_main,FALSE);
		atf_CloseConnection();
	}*/
	UpdateData(FALSE);
}

void CDialogAFCtrl::AFinfocusrangestate()
{
	int iRet = 0;
	short status[1] = {0};
	short rslt[16] = {0,};
	Sleep(500);
	iRet = ::atf_ReadStatus(status);
	
	for(int i = 0; i < 16; i++)
	{
		rslt[i] = status[0] % 2;
		status[0] = status[0] / 2;
	}

	rslt[12];//InRange
	rslt[13];//InFocus

	if(rslt[12])
	{
		m_btn_af_inrange.SetBackColor(RGB(0,255,0));
		if(rslt[13])
		{
			m_btn_af_infocus.SetBackColor(RGB(0,255,0));
			KillTimer(1);
		}
		else
		{
			m_btn_af_infocus.SetBackColor(RGB(0,0,0));
		}
	}
	else
	{
		m_btn_af_inrange.SetBackColor(RGB(0,255,0));
	}
}

BOOL CDialogAFCtrl::AFlaseronoffstate()
{
	int iRet = 0;
	short status[1] = {0};
	short rslt[16] = {0,};
	BOOL bRet = FALSE;

	iRet = ::atf_ReadStatus(status);
	
	for(int i = 0; i < 16; i++)
	{
		rslt[i] = status[0] % 2;
		status[0] = status[0] / 2;
	}

	rslt[4];//Laser Power on off state
	if(rslt[4])
	{		
		m_btn_af_laserone.EnableWindow(TRUE);
		m_btn_af_track_run.EnableWindow(TRUE);
		m_btn_af_aoi_track_run.EnableWindow(TRUE);
		bRet = TRUE;

	}
	else
	{		
		m_btn_af_laserone.EnableWindow(FALSE);
		m_btn_af_track_run.EnableWindow(FALSE);
		m_btn_af_aoi_track_run.EnableWindow(FALSE);
		bRet = FALSE;
	}
	return bRet;
}


void CDialogAFCtrl::ClickBtnenhctrlAfJogP1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	atf_MoveZ(m_edit_af_microstep * m_edit_af_fullstep / 1000.0f);	//DistanceToMove[um] * MicroStepping * FullSteps/mm) / 1000
	AFinfocusrangestate();
}


void CDialogAFCtrl::ClickBtnenhctrlMotJogM4()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	atf_MoveZ(-m_edit_af_microstep * m_edit_af_fullstep / 1000.0f);	//DistanceToMove[um] * MicroStepping * FullSteps/mm) / 1000
	AFinfocusrangestate();
}

void CDialogAFCtrl::ClickBtnenhctrlAfStop1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	atf_StopZMotor();
}

void CDialogAFCtrl::SetEzHome()
{
	ClickBtnenhctrlEzHome0();
}

void CDialogAFCtrl::ClickBtnenhctrlAfHome1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int rslt = 0;
	int iHomingArray[8];

		
	iHomingArray[0] = 20000;	//move1				메뉴얼에 나와 있는 homein parameter 값 적용
	iHomingArray[1] = 400;		//move2	
	iHomingArray[2] = -500;		//move3
	iHomingArray[3] = 2880;		//move4
	iHomingArray[4] = 6000;		//move1 속도
	iHomingArray[5] = 6000;		//move2	속도
	iHomingArray[6] = 200;		//move3	속도
	iHomingArray[7] = 6000;		//move4 속도

//	bool bInHome;

	for(int i=0 ; i<4 ; i++)
	{

		atf_WriteHomingZ_Parameters(&iHomingArray[i]);
		atf_WriteHomingZ_Parameters(&iHomingArray[i+4]);
//	Sleep(10);

		atf_RunHomingZ(&iHomingArray[i]);
		atf_RunHomingZ(&iHomingArray[i+4]);
		//Sleep(5);
	}

}





void CDialogAFCtrl::OnEnChangeEditAfMicroStep()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	UpdateData(FALSE);
}




void CDialogAFCtrl::OnEnChangeEditAfMaxVel2()   // FullStep Editbox
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	UpdateData(FALSE);
}



void CDialogAFCtrl::ClickBtnenhctrlAfTrackRun()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int iRetafonoff = 0;
	int iRet;
	iRetafonoff = m_btn_af_track_run.GetValue();
	bool bRunningAF = false;
	bool bStopped = false;
	int button = 0;
	/*
	//if (event!=EVENT_COMMIT) return 0;
	ENTERFUN(pFunName);

	if (panel==m_cvdaf) {
		// OK on panel cvd was pressed. assume regular tracking on main panel
		control = PANELM_TOGGLEBUTTON_AF;
		panel = m_main;
	}
	*/					// on/off regular AF
	SetTimer(1,200,NULL);
	if (iRetafonoff) 
	{
		if (gs_sensor_type==ATF5x)
		{
			int button_cvd = 0;

			if (button_cvd) 
			{
					//DiscardPanel (m_cvdaf);
					// write cvd related ...
				m_cvd_rolling_not = !m_cvd_rolling_not;

				if (m_cvd_numset < 4)
				{  // 2 or 3
					iRet = WriteCvdAfParm (m_cvd_numset, m_cvd_rolling_not);
				}
				else
				{	// 4 or 6 and use half data 
					iRet = WriteCvdAfParm (m_cvd_numset, m_cvd_rolling_not | 2);
				}
			}
			else
			{
					// not CVD AF, clear parm
				iRet = WriteCvdAfParm (0, 0);
			}
			if (iRet != ErrOK)
			{
				AfxMessageBox(_T("Error: Failed to set CVD atofocus parameters. [ErrorNum=%d]\n"), iRet);
				sprintf(logMsg, "Error: Failed to set CVD atofocus parameters. [ErrorNum=%d]\n", iRet);
				LOGMSG(logMsg, CLogger::LogLow);
			}
		}
		if (!WriteMotionLimitsIfEnabled(button)) return ;
		iRet = atf_AFTrack();
		bRunningAF = true;
	}
	else {		
		iRet = atf_AfStop();
		bStopped = true;
	}


}

void CDialogAFCtrl::ClickBtnenhctrlAfLaserone()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int iRetafonoff = 0;
	int iRet;
	iRetafonoff = m_btn_af_laserone.GetValue();
	bool bRunningAF = false;
	bool bStopped = false;
	int button = 0;
	SetTimer(1,200,NULL);
	if (iRetafonoff) 
	{
		if (!WriteMotionLimitsIfEnabled(button)) return ;
		iRet = atf_AfDuvJump();
		bRunningAF = true;	
	}
	else 
	{		
		iRet = atf_AfStop();
		bStopped = true;
	}
	m_btn_af_laserone.SetValue(1);
}


void CDialogAFCtrl::ClickBtnenhctrlAfAoiTrackRun()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int iRetafonoff = 0;
	int iRet;
	iRetafonoff = m_btn_af_aoi_track_run.GetValue();
	bool bRunningAF = false;
	bool bStopped = false;
	int button = 0;
	SetTimer(1,200,NULL);
	if (iRetafonoff) 
	{
		if (!WriteMotionLimitsIfEnabled(button)) return;
		iRet = atf_AFAoiTrack();
		bRunningAF = true;
	}
	else {		
		iRet = atf_AfStop();
		bStopped = true;
	}
}


void CDialogAFCtrl::ClickBtnenhctrlAfLaseron()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int iRet;


	if(m_btn_af_laseron.GetValue())
	{	//	Changed to on.
		iRet = atf_EnableLaser();
		AFlaseronoffstate();
		if(iRet != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to turn laser on. [ErrorNum=%d]\n", iRet);
			LOGMSG(logMsg, CLogger::LogLow);
		}
	}
	else
	{	//	Changed to off.
		iRet = atf_DisableLaser();
		AFlaseronoffstate();
		if(iRet != ErrOK)
		{
			sprintf(logMsg, "Error: Failed to turn laser off. [ErrorNum=%d]\n", iRet);
			LOGMSG(logMsg, CLogger::LogLow);
		}
	}
	
}


void CDialogAFCtrl::ClickBtnenhctrlAfMake0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int iRet = 0;

	if(iRet = m_btn_af_make0.GetValue())
	{
		iRet = atf_Make0() ;
		if (iRet != ErrOK)
			AfxMessageBox(_T("Error: Failed to make curent position zero. [ErrorNum=%d]\n", iRet));			
	}
	else
		return ;
}


	


void CDialogAFCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString strValue,str;
	CDialogMSG dlg;

	switch (nIDEvent) 
	{ 
	case 1:
		AFinfocusrangestate();
		break;
	case 2:		
		strValue.Format(_T("%d"),AFGetCurPos());					//AF
		m_btn_af_pos1.SetCaption(strValue);
		break;
	case 3:
		strValue.Format(_T("%d"),m_lEzdistance_new = EZGetCurPos());					//EZ
		m_btn_ez_pos.SetCaption(strValue);
		if(((m_lEzdistance_old >= 0) && (m_lEzdistance_new < 0)) || ((m_lEzdistance_old < 0) && (m_lEzdistance_new >= 0)))
		{
			m_lEzdistance_difference = abs(m_lEzdistance_old) + abs(m_lEzdistance_new);		 //거리 차이값
		}
		else
		{
			m_lEzdistance_difference = abs(m_lEzdistance_old - m_lEzdistance_new);			 //거리 차이값
		}
		m_lEzdistance_cumulative += m_lEzdistance_difference;								//거리 누적값
		m_lEzdistance_cumulative_mm = m_lEzdistance_cumulative * 0.0001;				//Ezi 10000per 당 1mm
		strValue.Format(_T("%0.4f mm/%d mm"),m_lEzdistance_cumulative_mm,m_lEzMaxdistance);
		m_btn_ez_cumulative_distance.SetCaption(strValue);
		m_lEzdistance_old = m_lEzdistance_new;											//EZ
		if(m_lEzdistance_cumulative_mm > m_lEzMaxdistance * 0.9)
		{
			str.Format(_T("경고 : Ezi 모터 주행 가능거리 확인 요망 ( %0.4f / %d )"),m_lEzdistance_cumulative_mm,m_lEzMaxdistance);
			dlg.SetCaption(str);
			dlg.DoModal();
		}
		break;
	case 4:
		m_lEzdistance_new = EZGetCurPos();											//EZ
		if(((m_lEzdistance_old >= 0) && (m_lEzdistance_new < 0)) || ((m_lEzdistance_old < 0) && (m_lEzdistance_new >= 0)))
		{
			m_lEzdistance_difference = abs(m_lEzdistance_old) + abs(m_lEzdistance_new);		 //거리 차이값
		}
		else
		{
			m_lEzdistance_difference = abs(m_lEzdistance_old - m_lEzdistance_new);			 //거리 차이값
		}
		m_lEzdistance_cumulative += m_lEzdistance_difference;							 //거리 누적값
		if(m_lEzdistance_old == m_lEzdistance_new)
		{
			m_lEzdistance_old = m_lEzdistance_new;
			KillTimer(4);
		}
		else
		{
			m_lEzdistance_old = m_lEzdistance_new;
		}
		break;																	//EZ
	case 100:
		if(GetEziPos_mm() == 0.0)
		{
			KillTimer(100);
			m_bEzMoterOrigin = TRUE;
			m_btnEzOrigin.SetValue(m_bEzMoterOrigin);
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

int CDialogAFCtrl::AFGetCurPos()
{
	int iRet;
	//CString strCommand;
	iRet = atf_ReadAbsZPos(&m_cur_abszpos);
	if(iRet != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadAbsZPos ((CDialogAFCtrl::GetCurPos)!"));
	}
	return m_cur_abszpos;
}




void CDialogAFCtrl::ClickBtnenhctrlEzHome0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int iRet;
	m_bEzMoterOrigin = FALSE;
	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CDialogMSG dlg;
	CString str;

	if(pdlg->m_Robot->GetRobotPos_mm(MOTOR_3) < 5)
	{
		ClickBtnenhctrlEzSpdSet0();
		iRet = FAS_AllMoveOriginSingleAxis(m_nPort);
	}
	else
	{
		str.Format(_T("LiftPin Axie Waite Pos Error\nLiftPin 위치가 높습니다."));
		pdlg->WriteLog(EZ_LOG,str);
		dlg.SetCaption(str);
		dlg.DoModal();
		m_btnEzOrigin.SetValue(FALSE);
		return;
	}
	
	if(iRet != m_iErrOk)
	{
		AfxMessageBox(_T("error : origin fail!"));
		str.Format(_T("error : origin fail!"));
		pdlg->WriteLog(EZ_LOG,str);
		return;
	}
	SetTimer(100, 1000, NULL);
}


void CDialogAFCtrl::MouseDownBtnenhctrlEzJogP0(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CDialogMSG dlg;
	CString str;
	CRect rect;

	if(!m_bEzMoterOrigin)
	{
		return;
	}

	GetDlgItem(IDC_BTNENHCTRL_EZ_JOG_P0)->GetClientRect(&rect);
	if((pdlg->m_Robot->GetRobotPos_mm(MOTOR_3) < 2) && ((0 < x) && (x < rect.right)) && ((0 < y) && (y < rect.bottom)))
	{
		FAS_MoveVelocity(m_nPort, m_iSlaveNo, m_lMoveVelocity, DIR_DEC);
	}
	else if((pdlg->m_Robot->GetRobotPos_mm(MOTOR_3) > 2) && ((0 < x) && (x < rect.right)) && ((0 < y) && (y < rect.bottom)))
	{
		str.Format(_T("LiftPin Axie Waite Error\nLiftPin 위치가 높습니다."));
		pdlg->WriteLog(EZ_LOG,str);
		dlg.SetCaption(str);
		dlg.DoModal();
	}
}


void CDialogAFCtrl::MouseUpBtnenhctrlEzJogP0(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	FAS_MoveStop(m_nPort, m_iSlaveNo);
}


void CDialogAFCtrl::MouseDownBtnenhctrlEzJogM0(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CDialogMSG dlg;
	CString str;
	CRect rect;

	if(!m_bEzMoterOrigin)
	{
		return;
	}

	GetDlgItem(IDC_BTNENHCTRL_EZ_JOG_M0)->GetClientRect(&rect);
	if((pdlg->m_Robot->GetRobotPos_mm(MOTOR_3) < 2) && ((0 < x) && (x < rect.right)) && ((0 < y) && (y < rect.bottom)))
	{
		FAS_MoveVelocity(m_nPort, m_iSlaveNo, m_lMoveVelocity, DIR_INC);
	}
	else if((pdlg->m_Robot->GetRobotPos_mm(MOTOR_3) > 2) && ((0 < x) && (x < rect.right)) && ((0 < y) && (y < rect.bottom)))
	{
		str.Format(_T("LiftPin Axie Waite Error\nLiftPin 위치가 높습니다."));
		pdlg->WriteLog(EZ_LOG,str);
		dlg.SetCaption(str);
		dlg.DoModal();
	}
}


void CDialogAFCtrl::MouseUpBtnenhctrlEzJogM0(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	FAS_MoveStop(m_nPort, m_iSlaveNo);
}


void CDialogAFCtrl::ClickBtnenhctrlEzMove0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iRet;

	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CDialogMSG dlg;
	CString str;

	if(!m_bEzMoterOrigin)
	{
		return;
	}

	if(pdlg->m_Robot->GetRobotPos_mm(MOTOR_3) < 2)
	{
		iRet = FAS_MoveSingleAxisAbsPos(m_nPort, m_iSlaveNo, m_edit_ez_move_pos, m_lMoveVelocity);
	}
	else
	{
		str.Format(_T("LiftPin Axie Waite Error\nLiftPin 위치가 높습니다."));
		pdlg->WriteLog(EZ_LOG,str);
		dlg.SetCaption(str);
		dlg.DoModal();
		return;
	}

	if (iRet != FMM_OK)
	{
		AfxMessageBox(_T("Error happended : %d",iRet));
		str.Format(_T("Error happended : %d",iRet));
		pdlg->WriteLog(EZ_LOG,str);
	}
}


void CDialogAFCtrl::ClickBtnenhctrlEzStop0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	FAS_MoveStop(m_nPort, m_iSlaveNo);
}


void CDialogAFCtrl::ClickBtnenhctrlEzConnect0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	DWORD dwBaudrate = 115200;
	
	int iRet;
	m_nPort = 9;
	iRet = FAS_Connect(9,115200);
	if(iRet != 1)
	{
		AfxMessageBox(_T("error : connect fail!"));
		m_strlog.Format(_T("error : connect fail!"));
		pdlg->WriteLog(EZ_LOG,m_strlog);
	}	
	iRet = FAS_ServoEnable(m_nPort, m_iSlaveNo, TRUE);
	m_lEzdistance_old = EZGetCurPos();
	
	
}


void CDialogAFCtrl::OnEnChangeEditEzSpd()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	UpdateData(FALSE);
}

long CDialogAFCtrl::EZGetCurPos()
{
	long lActPos;
	FAS_GetActualPos(m_nPort,m_iSlaveNo,&lActPos);
	return lActPos;
}

BOOL CDialogAFCtrl::GetEzPosComplete(int scan_type_idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	long pos;
	BOOL rslt = FALSE;
	pos = EZGetCurPos();
	if(scan_type_idx >= 2)
	{
		if(pos == (long)(pdlg->m_RecipeScan->GetEzPos(scan_type_idx) * 10000.0))
		{
			rslt = TRUE;
			m_strlog.Format(_T("GetEzPosComplete(scan_type_idx : %d ,pos : %.3f)"),scan_type_idx,pos);
			pdlg->WriteLog(EZ_LOG,m_strlog);
		}
	}
	else
	{
		if(pos == 0)
		{
			rslt = TRUE;
			m_strlog.Format(_T("GetEzPosComplete(scan_type_idx : %d ,pos : %.3f)"),scan_type_idx,pos);
			pdlg->WriteLog(EZ_LOG,m_strlog);
		}
	}
	return rslt;
}

void CDialogAFCtrl::SetEzSpeed(int speed)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	m_lMoveVelocity = speed;
	m_edit_ez_spd = speed;
	m_strlog.Format(_T("SetEzSpeed(speed : %d)"),speed);
	pdlg->WriteLog(EZ_LOG,m_strlog);
}


double CDialogAFCtrl::GetEziPos_mm()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	double pos_cts = 0;
	double pos_mm = 0.;

	pos_cts = EZGetCurPos();

	pos_mm = pos_cts / EZ_CTS_MM;

	m_strlog.Format(_T("GetEziPos_mm(pos_cts : %.3f,pos_mm : %.3f)"),pos_cts,pos_mm);
	pdlg->WriteLog(EZ_LOG,m_strlog);

	return pos_mm;
}

void CDialogAFCtrl::AFLLCOffsetUpdate()
{
	UpdateData(TRUE);
	BOOL bRetLoc = FALSE;

	m_illcoffset[0] = m_edit_llc_offset0;
	m_illcoffset[1] = m_edit_llc_offset1;
	m_illcoffset[2] = m_edit_llc_offset2;

	for (int i = 0; i < OBJ_NUM_3; i++)
	{
		bRetLoc = atf_getLlcObjectivePositionOffset(i, &m_illcoffset[i]);
		if (bRetLoc)
		{
			//LOGMSG2(CLogger::LogLow, "%s%d%s%d%s", "Info: Objective ", i, " position offset = ", iObjPosOffset, ".\n");
		}
		else
		{
			AfxMessageBox(_T("Error: Failed to get objective LLC Offset "));
		}
	}
}

void CDialogAFCtrl::OnBnClickedRadioReview1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_edit_af_obj_num = OBJ_NUM_1;
	m_edit_af_infocusrange = m_infocus[0];
	m_edit_af_max_acc = m_acc[0];
	m_edit_af_max_vel = m_speed[0];
	m_edit_af_fullstep = m_fullstep[0];
	m_edit_af_microstep = m_microstep[0];



	if(m_btn_af_connect.GetValue())
	{
		ObjChangeM(m_edit_af_obj_num, 0, 0);
		AFinfocusrangestate();
	}

	UpdateData(FALSE);
}


void CDialogAFCtrl::OnBnClickedRadioReview2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_edit_af_obj_num = OBJ_NUM_2;
	m_edit_af_infocusrange = m_infocus[1];
	m_edit_af_max_acc = m_acc[1];
	m_edit_af_max_vel = m_speed[1];
	m_edit_af_fullstep = m_fullstep[1];
	m_edit_af_microstep = m_microstep[1];


	if(m_btn_af_connect.GetValue())
	{
		ObjChangeM(m_edit_af_obj_num, 0, 0);
		AFinfocusrangestate();
	}

	UpdateData(FALSE);
	
}


void CDialogAFCtrl::OnBnClickedRadioReview3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_edit_af_obj_num = OBJ_NUM_3;
	m_edit_af_infocusrange = m_infocus[2];
	m_edit_af_max_acc = m_acc[2];
	m_edit_af_max_vel = m_speed[2];
	m_edit_af_fullstep = m_fullstep[2];
	m_edit_af_microstep = m_microstep[2];


	if(m_btn_af_connect.GetValue())
	{
		ObjChangeM(m_edit_af_obj_num, 0, 0);
		AFinfocusrangestate();
	}

	UpdateData(FALSE);
}

void CDialogAFCtrl::SetAfMove(BOOL bRet,int nstep)
{
	int iRet = 0;
	switch (bRet)
	{
	case 0 :
		{
			iRet = atf_MoveZ(-nstep * m_edit_af_fullstep / 1000.0f);	//DistanceToMove[um] * MicroStepping * FullSteps/mm) / 1000
//			AFinfocusrangestate();
			break ;
		}
	case 1 :
		{
			iRet = atf_MoveZ(nstep * m_edit_af_fullstep / 1000.0f);	//DistanceToMove[um] * MicroStepping * FullSteps/mm) / 1000
//			AFinfocusrangestate();
			break ;
		}
	}
}

int CDialogAFCtrl::SetAfLlc(int iRet)
{
	UpdateData(TRUE);
	int Ret = 0;
	switch (iRet)
	{
	case 0 :
		{
			m_edit_af_obj_num = OBJ_NUM_1;
			m_edit_af_infocusrange = m_infocus[0];
			m_edit_af_max_acc = m_acc[0];
			m_edit_af_max_vel = m_speed[0];
			m_edit_af_fullstep = m_fullstep[0];
			m_edit_af_microstep = m_microstep[0];

#ifdef MACHINE
			ObjChangeM(m_edit_af_obj_num, 0, 0);
			AFinfocusrangestate();
#endif			
			Ret = 0;
			break ;
		}
	case 1 :
		{
			m_edit_af_obj_num = OBJ_NUM_2;
			m_edit_af_infocusrange = m_infocus[1];
			m_edit_af_max_acc = m_acc[1];
			m_edit_af_max_vel = m_speed[1];
			m_edit_af_fullstep = m_fullstep[1];
			m_edit_af_microstep = m_microstep[1];

#ifdef MACHINE
			ObjChangeM(m_edit_af_obj_num, 0, 0);
			AFinfocusrangestate();
#endif
			Ret = 1;
			break ; 
		}
	case 2 :
		{
			m_edit_af_obj_num = OBJ_NUM_3;
			m_edit_af_infocusrange = m_infocus[2];
			m_edit_af_max_acc = m_acc[2];
			m_edit_af_max_vel = m_speed[2];
			m_edit_af_fullstep = m_fullstep[2];
			m_edit_af_microstep = m_microstep[2];
#ifdef MACHINE
			ObjChangeM(m_edit_af_obj_num, 0, 0);
			AFinfocusrangestate();
#endif
			Ret = 2;
			break ;
		}
	}

	UpdateData(TRUE);
	return Ret;
}

void CDialogAFCtrl::AFWriteLedCtrl(unsigned short LedPwm)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	int iRet = 0;
	unsigned short LedPWM = 10;
	unsigned short LedCurrent = 0;


	iRet = atf_ReadLedCurrent(AF_LED_CHANNEL,&LedCurrent);
	if(iRet != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadLedCurrent Error code %d"),iRet);

	}
	if(LedCurrent != LedPWM)
	{
		if(!(iRet = atf_WriteLedCurrent(AF_LED_CHANNEL,LedPWM)))
		{
//			AfxMessageBox(_T("Error : WriteLedCurrent Error code %d"),iRet);
		}
	}
	iRet = atf_WriteLedPwm(AF_LED_CHANNEL,LedPwm);
	if(iRet != m_iErrOk)
	{
//		AfxMessageBox(_T("Error : WriteLedPwm Error code %d"),iRet);
	}

	//20160721 ngh
	if(LedPwm > 0)
	{
		pdlg->m_Light->SetLightTime(4, TRUE);
	}
	else
	{
		pdlg->m_Light->SetLightTime(4, FALSE);
	}
}

unsigned short CDialogAFCtrl::AFGetLedCtrl()
{
	int iRet = 0;
	unsigned short LedCurPWM = 10;
	unsigned short LedCurrent = 0;
	unsigned short LedPWM = 0;


	iRet = atf_ReadLedCurrent(AF_LED_CHANNEL,&LedCurrent);
	if(iRet != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadLedCurrent Error code %d"),iRet);
	}
	if(LedCurrent != LedCurPWM)
	{
		if(!(iRet = atf_WriteLedCurrent(AF_LED_CHANNEL,LedPWM)))
		{
	//		AfxMessageBox(_T("Error : WriteLedCurrent Error code %d"),iRet);
		}
	}
	iRet = atf_ReadLedPwm(AF_LED_CHANNEL,&LedPWM);
	if(iRet != m_iErrOk)
	{
		AfxMessageBox(_T("Error : ReadLedPwm Error code %d"),iRet);
	}
	return LedPWM;
}

void CDialogAFCtrl::SetReviewLed(unsigned short LedPwm)
{
	AFWriteLedCtrl(LedPwm);
}
unsigned short CDialogAFCtrl::GetReviewLed()
{
	unsigned short LedPwmRet = 0;
	LedPwmRet = AFGetLedCtrl();
	return LedPwmRet;
}

BOOL CDialogAFCtrl::SetAFTrackRun(BOOL bOn)
{
//	int iRetafonoff = 0;
	int iRet;
//	iRetafonoff = m_btn_af_track_run.GetValue();
//	bool bRunningAF = false;
//	bool bStopped = false;
	int button = 0;
	BOOL bRet = FALSE;
	/*
	//if (event!=EVENT_COMMIT) return 0;
	ENTERFUN(pFunName);

	if (panel==m_cvdaf) {
		// OK on panel cvd was pressed. assume regular tracking on main panel
		control = PANELM_TOGGLEBUTTON_AF;
		panel = m_main;
	}
	*/					// on/off regular AF
	//SetTimer(1,200,NULL);
	if (bOn) 
	{
		if (gs_sensor_type==ATF5x)
		{
			int button_cvd = 0;

			if (button_cvd) 
			{
					//DiscardPanel (m_cvdaf);
					// write cvd related ...
				m_cvd_rolling_not = !m_cvd_rolling_not;

				if (m_cvd_numset < 4)
				{  // 2 or 3
					iRet = WriteCvdAfParm (m_cvd_numset, m_cvd_rolling_not);
				}
				else
				{	// 4 or 6 and use half data 
					iRet = WriteCvdAfParm (m_cvd_numset, m_cvd_rolling_not | 2);
				}
			}
			else
			{
					// not CVD AF, clear parm
				iRet = WriteCvdAfParm (0, 0);
			}
			if (iRet != ErrOK)
			{
				AfxMessageBox(_T("Error: Failed to set CVD atofocus parameters. [ErrorNum=%d]\n", iRet));
				sprintf(logMsg, "Error: Failed to set CVD atofocus parameters. [ErrorNum=%d]\n", iRet);
				LOGMSG(logMsg, CLogger::LogLow);
			}
		}
		if (!WriteMotionLimitsIfEnabled(button)) ;
		iRet = atf_AFTrack();
//		bRunningAF = true;
		bRet = TRUE;
	}
	else {		
		iRet = atf_AfStop();
//		bStopped = true;
		bRet = FALSE;
	}
	return bRet;
}

void CDialogAFCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow)
	{
#ifdef MACHINE
		SetTimer(2,200,NULL);
		SetTimer(3,200,NULL);
		KillTimer(4);
#endif
	}
	else
	{
#ifdef MACHINE
		KillTimer(1);
		KillTimer(2);
		KillTimer(3);
		SetTimer(4,200,NULL);
#endif
	}
}

BOOL CDialogAFCtrl::GetAFInfocus()
{
	int iRet = 0;
	short status[1] = {0};
	short rslt[16] = {0,};
	Sleep(500);
	iRet = atf_ReadStatus(status);
	
	for(int i = 0; i < 16; i++)
	{
		rslt[i] = status[0] % 2;
		status[0] = status[0] / 2;
	}
	rslt[13];//InFocus
	if(rslt[13])
	{
		return TRUE;	
	}
	return FALSE;
}

void CDialogAFCtrl::ClickBtnenhctrlEz()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	FAS_Close(m_nPort);
	m_bEzMoterOrigin = FALSE;
	m_btnEzOrigin.SetValue(m_bEzMoterOrigin);
}


void CDialogAFCtrl::ClickBtnenhctrlAf()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KillTimer(1);
	KillTimer(2);
	
	atf_CloseConnection();
}

BOOL CDialogAFCtrl::LoadAFCtrl(CString path)
{

	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	char path_temp[MAX_PATH];
	size_t getval;
	getval = 0;
	int i, idx;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;


	file_path = path + _T("\\AFCtrl.par");

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
		key.Format(_T("m_lEzdistance_cumulative(pulse)"));
		GetPrivateProfileString(_T("AFCTRL"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_lEzdistance_cumulative = atol(ctemp);

		key.Empty();
		key.Format(_T("m_lEzdistance_cumulative(mm)"));
		GetPrivateProfileString(_T("AFCTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_lEzdistance_cumulative_mm = atof(ctemp);

		key.Empty();
		key.Format(_T("m_Cycleoliling(day)"));
		GetPrivateProfileString(_T("AFCTRL"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_Cycleoliling = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_ez_spd"));
		GetPrivateProfileString(_T("AFCTRL"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_ez_spd = atof(ctemp);

		;

		////////////////////////////////////////////////////////////////////////////
	}
	fd=0;

	return rslt;
}


BOOL CDialogAFCtrl::SaveAFCtrl(CString path)								
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\AFCtrl.par");

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
		key.Format(_T("m_lEzdistance_cumulative(pulse)"));
		dat.Empty();
		dat.Format(_T("%d"), m_lEzdistance_cumulative);
		WritePrivateProfileString(_T("AFCTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_lEzdistance_cumulative(mm)"));
		dat.Empty();
		dat.Format(_T("%0.4f"), m_lEzdistance_cumulative_mm);
		WritePrivateProfileString(_T("AFCTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_Cycleoliling(day)"));
		dat.Empty();
		dat.Format(_T("%d"), m_Cycleoliling);
		WritePrivateProfileString(_T("AFCTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_ez_spd"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_ez_spd);
		WritePrivateProfileString(_T("AFCTRL"), key, dat, file_path);
		
	}
	fd=0;
	
	return rslt;
}

BOOL CDialogAFCtrl::SaveAFLLCOffset(CString path)								
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\AFLLCOffset.par");

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
		key.Format(_T("m_edit_llc_offset0"));
		dat.Empty();
		dat.Format(_T("%d"), m_edit_llc_offset0);
		WritePrivateProfileString(_T("AFCTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_llc_offset1"));
		dat.Empty();
		dat.Format(_T("%d"), m_edit_llc_offset1);
		WritePrivateProfileString(_T("AFCTRL"), key, dat, file_path);key.Empty();

		key.Format(_T("m_edit_llc_offset2"));
		dat.Empty();
		dat.Format(_T("%d"), m_edit_llc_offset2);
		WritePrivateProfileString(_T("AFCTRL"), key, dat, file_path);


	}
	fd=0;
	
	return rslt;
}

BOOL CDialogAFCtrl::LoadAFLLCOffset(CString path)
{

	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	char path_temp[MAX_PATH];
	size_t getval;
	getval = 0;
	int i, idx;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;


	file_path = path + _T("\\AFLLCOffset.par");

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
		key.Format(_T("m_edit_llc_offset0"));
		GetPrivateProfileString(_T("AFCTRL"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_llc_offset0 = atol(ctemp);

		key.Empty();
		key.Format(_T("m_edit_llc_offset1"));
		GetPrivateProfileString(_T("AFCTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_llc_offset1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_llc_offset2"));
		GetPrivateProfileString(_T("AFCTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_llc_offset2 = atof(ctemp);
		////////////////////////////////////////////////////////////////////////////
	}
	fd=0;

	return rslt;
}


void CDialogAFCtrl::ClickBtnenhctrlLlcapply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	SaveAFLLCOffset(AFCTRL_PATH);
	AFLLCOffsetUpdate();

}


BOOL CDialogAFCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDialogAFCtrl::SetEziMove_mm(double pos)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	int iRet;
	double pos_cts;

	pos_cts = pos * EZ_CTS_MM;

	if(m_lMoveVelocity == 0)
	{
		m_lMoveVelocity = 10000;
		m_strlog.Format(_T("EZ 기본 speed 10000으로 설정"));
		pdlg->WriteLog(EZ_LOG,m_strlog);
	}


	iRet = FAS_MoveSingleAxisAbsPos(m_nPort, m_iSlaveNo, (long)pos_cts, m_lMoveVelocity);
	m_strlog.Format(_T("SetEziMove_mm(port : %d, slaveNo : %d, pos_cts : %d, speed : %d)"),m_nPort, m_iSlaveNo, (long)pos_cts, m_lMoveVelocity);
	pdlg->WriteLog(EZ_LOG,m_strlog);
	if (iRet != FMM_OK)
	{
		AfxMessageBox(_T("Error happended : %d",iRet));
		m_strlog.Format(_T("Error happended : %d,SetEziMove_mm()"),iRet);
		pdlg->WriteLog(EZ_LOG,m_strlog);
	}
}

void CDialogAFCtrl::ClickBtnenhctrlEzSpdSet0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	m_lMoveVelocity = m_edit_ez_spd;
	m_strlog.Format(_T("ClickBtnenhctrlEzSpdSet0() : %d"),m_lMoveVelocity);
	pdlg->WriteLog(EZ_LOG,m_strlog);
}


BOOL CDialogAFCtrl::GerEzMoterOrigin()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	m_strlog.Format(_T("GerEzMoterOrigin() : %d"),m_bEzMoterOrigin);
	pdlg->WriteLog(EZ_LOG,m_strlog);
	return m_bEzMoterOrigin;
}

void CDialogAFCtrl::EziCycleOiling()
{
	CDialogMSG dlg;
	CString str;
	CTime time;
	short newday,hour,tempday;
	hour = time.GetHour();
	newday = time.GetDay();
//	tempday = time.GetDay() - 1;
//	if(tempday + 1 == newday
	if(newday != m_Cycleoliling)
	{
		m_Curoliling++;
	}
	str.Format(_T("%d / 730 (day)"),m_Curoliling);
	m_btn_ez_cycle_oiling.SetCaption(str);
	if(m_Curoliling < 720)
	{
		str.Format(_T("경고 : Ezi 모터 주유 주기 확인"));
		dlg.SetCaption(str);
		dlg.DoModal();
	}
}

void CDialogAFCtrl::ClickBtnenhctrlEzOilComp()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_Curoliling = 0;
}
