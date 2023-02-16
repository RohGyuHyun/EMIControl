#pragma once
#include "include/CommThread.h"	// Added by ClassView
#include "Trigger_recipe.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"

#pragma once
#define USER_MSG_TRIGGER			WM_USER+10 

#define TRIGGER_INFO_VIEW		0

#define MAX_CHANAL				4
////////////////////////////////////////////////////////
//Trigger Ch 4개중 2개만 사용
#define TRIGGER_8K				0
#define TRIGGER_16K				1
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//com open define
#define TRIGGER_COM_PORT		_T("COM1")
#define TRIGGER_BAUDRATE		9600
#define TRIGGER_BAUDRATE_IDX	2
#define TRIGGER_START_BIT		8
#define TRIGGER_STOP_BIT		0
#define TRIGGER_PARITY			NOPARITY
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//Trigger init define
#define	TRIGGER_SETTING_TIME_BASE			0
#define TRIGGER_SETTING_PITCH_BASE			1
#define TRIGGER_SETTING_ENCODER_RESOLUTION	1

#define TRIGGER_SETTING_START_POS_8K		1//mm
#define TRIGGER_SETTING_END_POS_8K			10000//mm
#define TRIGGER_SETTING_PITCH_8K			5//um
#define TRIGGER_SETTING_PULSE_WIDTH_8K		12//us
#define TRIGGER_SETTING_DELAY_8K			12//us

#define TRIGGER_SETTING_START_POS_16K		1//mm
#define TRIGGER_SETTING_END_POS_16K			10000//mm
#define TRIGGER_SETTING_PITCH_16K			5//um
#define TRIGGER_SETTING_PULSE_WIDTH_16K		12//us
#define TRIGGER_SETTING_DELAY_16K			12//us
////////////////////////////////////////////////////////

#define COMPFREQ			0.0000001	//0.0005
#define COMPPERIOD			0.005
#define COMPPULSEWIDTH		0.005

#define COMMMODE_ON			1
#define COMMMODE_OFF		0
#define COMMMODE_FAIL		2

#define BASECLK				10		//ns
//#define BASECLK			5		//ns

#define ENC_BASEPOS			2147483648
//#define ENCCOMP_MAXADDR		511
#define ENCCOMP_MAXADDR		4095
#define ENCCOMP_BASEADDR	2048
#define MODE5_MAXADDR		4095
#define MODE3_MAXADDR		255
#define MODE4_MAXPADDR		255
#define MODE4_MAXAADDR		14

#define HCMD_STOP				_T("of")
#define HCMD_OPON				_T("on")
#define HCMD_ENCCNTCLR			_T("re")
#define HCMD_CLRTRGCNT			_T("rt")
#define HCMD_COMPMODEON			_T("co")
#define HCMD_COMPMODEOFF		_T("cf")
#define HCMD_COMPBOUND			_T("cb")
#define HCMD_COMPTARGET			_T("cv")
#define HCMD_SETTRGMODE			_T("tm")
#define HCMD_LCDPAGE			_T("ld")
#define HCMD_ENCSIMON			_T("vn")
#define HCMD_ENCSIMOFF			_T("vf")
#define HCMD_ENCSIMPRD			_T("vd")
#define HCMD_ENCSIMMODE			_T("vm")
#define HCMD_MODE0PERIOD		_T("0d")
#define HCMD_MODE0PULSEWIDTH	_T("0w")
#define HCMD_MODE1DIR			_T("1i")
#define HCMD_MODE1START			_T("1b")
#define HCMD_MODE1END			_T("1e")
#define HCMD_MODE1PITCH			_T("1p")
#define HCMD_MODE1PULSEWIDTH	_T("1w")
#define HCMD_MODE1DELAY			_T("1l")
#define HCMD_MODE2PITCH			_T("2p")
#define HCMD_MODE2OFFSET		_T("2f")
#define HCMD_MODE2PULSEWIDTH	_T("2w")
#define HCMD_MODE2PULSEWIDTH0	_T("20")
#define HCMD_MODE2PULSEWIDTH1	_T("21")
#define HCMD_MODE2PULSEWIDTH2	_T("22")
#define HCMD_MODE2PULSEWIDTH3	_T("23")
#define HCMD_MODE2PULSEDELAY	_T("2l")
#define HCMD_MODE3VELOCITY		_T("3v")
#define HCMD_MODE3PERIOD		_T("3d")
#define HCMD_MODE3PULSEWIDTH	_T("3w")
#define HCMD_MODE3VECTORPITCH	_T("3p")
#define HCMD_MODE4PERIOD		_T("4d")
#define HCMD_MODE4PULSEWIDTH	_T("4w")
#define HCMD_MODE4AREA0			_T("4x")
#define HCMD_MODE4AREA1			_T("4y")
#define HCMD_MODE5LINEON		_T("5n")
#define HCMD_MODE5POS			_T("5s")
#define HCMD_MODE5PULSEWIDTH	_T("5w")
#define HCMD_CODMODE			_T("dm")
#define HCMD_CODCHECK			_T("dk")

#define HCMD_FRAMRD				_T("fr")
#define HCMD_FRAMWR				_T("fw")
#define HCMD_INITIALIZE			_T("zz")
#define HCMD_GETINFO			_T("gi")

#define SET_INIT_M0PRD_0		3
#define SET_INIT_M0PRD_1		4
#define SET_INIT_M0PRD_2		5
#define SET_INIT_M0PRD_3		6
#define SET_INIT_M0PW_0			7
#define SET_INIT_M0PW_1			8
#define SET_INIT_M0PW_2			9
#define SET_INIT_M0PW_3			10
#define SET_INIT_M1PITCH_0		16
#define SET_INIT_M1PITCH_1		17
#define SET_INIT_M1PITCH_2		18
#define SET_INIT_M1PITCH_3		19
#define SET_INIT_M1PW_0			20
#define SET_INIT_M1PW_1			21
#define SET_INIT_M1PW_2			22
#define SET_INIT_M1PW_3			23

// #define HCMD_STOP				"op0"
// #define HCMD_OPON				"op1"
// #define HCMD_ENCCNTCLR			"ecl"
// #define HCMD_CLRTRGCNT			"tcl"
// #define HCMD_COMPMODEON			"cp1"
// #define HCMD_COMPMODEOFF		"cp0"
// #define HCMD_COMPBOUND			"cpb"
// #define HCMD_COMPTARGET			"cpt"
// #define HCMD_SETTRGMODE			"tmd"
// #define HCMD_SELTRGOUT0			"ch0"
// #define HCMD_SELTRGOUT1			"ch1"
// #define HCMD_SELTRGOUT2			"ch2"
// #define HCMD_SELTRGOUT3			"ch3"
// #define HCMD_LCDPAGE			"lcd"
// #define HCMD_ENCSIMON			"ve1"
// #define HCMD_ENCSIMOFF			"ve0"
// #define HCMD_ENCSIMPRD			"ved"
// #define HCMD_ENCSIMMODE			"vem"
// #define HCMD_MODE0PERIOD		"m0d"
// #define HCMD_MODE0PULSEWIDTH	"m0w"
// #define HCMD_MODE1DIR			"m1i"
// #define HCMD_MODE1START			"m1b"
// #define HCMD_MODE1END			"m1e"
// #define HCMD_MODE1PITCH			"m1p"
// #define HCMD_MODE1PULSEWIDTH	"m1w"
// #define HCMD_MODE1DELAY			"m1l"
// #define HCMD_MODE2PITCH			"m2p"
// #define HCMD_MODE2OFFSET		"m2f"
// //#define HCMD_MODE2PULSEWIDTH	"m2w"
// #define HCMD_MODE2PULSEWIDTH0	"m20"
// #define HCMD_MODE2PULSEWIDTH1	"m21"
// #define HCMD_MODE2PULSEWIDTH2	"m22"
// #define HCMD_MODE2PULSEWIDTH3	"m23"
// #define HCMD_MODE2PULSEDELAY	"m2l"
// #define HCMD_MODE3VELOCITY		"m3v"
// #define HCMD_MODE3PERIOD		"m3d"
// #define HCMD_MODE3PULSEWIDTH	"m3w"
// #define HCMD_MODE3VECTORPITCH	"m3p"
// #define HCMD_MODE4PERIOD		"m4d"
// #define HCMD_MODE4PULSEWIDTH	"m4w"
// #define HCMD_MODE4AREA0			"m4x"
// #define HCMD_MODE4AREA1			"m4y"
// #define HCMD_MODE5LINEON		"m5n"
// #define HCMD_MODE5POS			"m5s"
// #define HCMD_MODE5PULSEWIDTH	"m5w"
// #define HCMD_CODMODE			"cdm"
// #define HCMD_CODCHECK			"cdk"
// 
// #define HCMD_FRAMRD				"frd"
// #define HCMD_FRAMWR				"fwr"
// #define HCMD_INITIALIZE			"zzz"
// #define HCMD_GETINFO			"get"

#define SET_TRGMODE0		0
#define SET_TRGMODE1		1
#define SET_TRGMODE2		2
#define SET_TRGMODE3		3
#define SET_TRGMODE4		4
#define SET_TRGMODE5		5
#define SET_TRGMODEALL		15

#define SET_LCDPAGE0		0
#define SET_LCDPAGE1		1
#define SET_LCDPAGE3		3

#define SET_CODMODEON		1
#define SET_CODMODEOFF		0
#define SET_COMPMODEON		1
#define SET_COMPMODEOFF		0
#define SET_MODE1DIRPOS		1
#define SET_MODE1DIRNEG		0
#define SET_MODE3PITCHUPPER	1
#define SET_MODE3PITCHLOWER	0
#define SET_MODE5LINEON		1
#define SET_MODE5LINEOFF	0
#define SET_ENCSIMENON		3
#define SET_ENCSIMENOFF		0
#define	SET_ENCSIMPOS		2
#define	SET_ENCSIMNEG		1
#define	SET_ENCSIMSTOP		0
#define	SET_ENCSIMMOVETO	8
#define	SET_ENCSIMMOVEAS	4

#define GET_RTN_FWVERSION	_T("cm")
#define GET_RTN_HWSTATES	_T("st")
#define GET_RTN_TRGMODE		_T("tm")
#define GET_RTN_ENCCNT		_T("ec")
#define GET_RTN_TRGCNT		_T("tc")
#define GET_RTN_M0_PRD		_T("0d")
#define GET_RTN_M0_PW		_T("0w")
#define GET_RTN_M1_START	_T("1b")
#define GET_RTN_M1_END		_T("1e")
#define GET_RTN_M1_PITCH	_T("1p")
#define GET_RTN_M1_PW		_T("1w")
#define GET_RTN_M1_DELAY	_T("1l")

#define GET_FPGAVERSION		_T("ff")
#define GET_BOARDSTATUS		_T("00")
#define GET_LCDPAGE			_T("05")
#define GET_ENCCNTHEX0		_T("06")
#define GET_ENCCNTHEX1		_T("07")
#define GET_SIMENABLE		_T("0a")
#define GET_SIMMODE			_T("0b")
#define GET_SIMPRD0			_T("0c")
#define GET_SIMPRD1			_T("0d")
#define GET_ENCCNTBCD0		_T("0e")
#define GET_ENCCNTBCD1		_T("0f")
#define GET_COMPBOUND		_T("1b")
#define GET_COMPTARGET		_T("1c")
#define GET_MODE_CH0		10
#define GET_MODE_CH1		11
#define GET_MODE_CH2		12
#define GET_MODE_CH3		13
#define GET_MODE_CHTRGSEL	0
#define GET_MODE0_PRD		1
#define GET_MODE0_PW		2
#define GET_MODE1_START		10
#define GET_MODE1_END		11
#define GET_MODE1_PITCH		12
#define GET_MODE1_PW		13
#define GET_MODE1_DELAY		14
#define GET_MODE_ENCCNT		4
#define GET_MODE_TRGCNT		5
//#define GET_MODE0_PRD		"51"
//#define GET_MODE0_PW		"52"
#define GET_MODE1_DIR		_T("5a")
//#define GET_MODE1_START		"5b"
//#define GET_MODE1_END		"5c"
//#define GET_MODE1_PITCH		"5d"
//#define GET_MODE1_PW		"5e"
//#define GET_MODE1_DELAY		"5f"
#define GET_MODE2_PITCH		_T("61")
#define GET_MODE2_DELAY		_T("64")
#define GET_MODE2_PW		_T("65")
#define GET_MODE3_VELOCITY	_T("70")
#define GET_MODE3_PRD		_T("71")
#define GET_MODE3_PW		_T("72")
#define GET_MODE3_PITCH		_T("73")
#define GET_MODE4_PRD		_T("b0")
#define GET_MODE4_PW		_T("b1")
#define GET_MODE4_AREA0		_T("b2")
#define GET_MODE4_AREA1		_T("b3")
#define GET_MODE5_SETANDPOS	_T("c0")
#define GET_MODE5_PW		_T("c1")
#define GET_TRGCNTHEX0		_T("d5")
#define GET_TRGCNTHEX1		_T("d6")
#define GET_TRGCNTHEX2		_T("d7")
#define GET_TRGCNTHEX3		_T("d8")
#define GET_TRGCNTBCD0		_T("da")
#define GET_TRGCNTBCD1		_T("db")
#define GET_TRGCNTBCD2		_T("dc")
#define GET_TRGCNTBCD3		_T("dd")
#define GET_CODMODEON		_T("ea")
#define GET_CODERROR		_T("ec")
#define GET_CODSYNCCNTHEX	_T("ed")
#define GET_CODSYNCCNTBCD	_T("ee")

// CDialogTriggerCtrl 대화 상자입니다.

class CDialogTriggerCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogTriggerCtrl)

public:
	CDialogTriggerCtrl(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogTriggerCtrl();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TRIGGER_CTRL };

public:
	virtual BOOL OnInitDialog();
	
	void SetResetEncoderCnt();
	void SetResetTriggerCnt();
	//isOPMode = TRUE -> 트리거 발생, FALSE -> 트리거 미발생
	void SetOPMode(BOOL isOPMode);	
	//isDirection = TRUE -> 정방향, FALSE -> 역방향
	void SetDirection(BOOL isDirection);	
	void SetCamTriggerRcipe(int cam_idx);

	LRESULT OnTriggerMessage(WPARAM para0, LPARAM para1);

	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
	
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	recipe m_trigger_rcipe;

	double m_dTriggerPara[2][25];

	CBtnenhctrl m_btn_trigger_connection;

	CComboBox m_combo_trigger_select_cam;

	CComboBox m_combo_trigger_port;
	CComboBox m_combo_trigger_baud_rate;
	CComboBox m_combo_trigger_data_bit;
	CComboBox m_combo_trigger_stop_bit;
	CComboBox m_combo_trigger_parity;

	CComboBox m_combo_trigger_mode_ch0;
	CComboBox m_combo_trigger_mode_ch1;
	CComboBox m_combo_trigger_mode_ch2;
	CComboBox m_combo_trigger_mode_ch3;

	CBtnenhctrl m_btn_trigger_status_enable0;
	CBtnenhctrl m_btn_trigger_status_enable1;
	CBtnenhctrl m_btn_trigger_status_enable2;
	CBtnenhctrl m_btn_trigger_status_enable3;

	double m_edit_trigger_encoder_resolution;

	double m_edit_trigger_start_pos0;
	double m_edit_trigger_end_pos0;
	double m_edit_trigger_pitch0;
	double m_edit_trigger_pulse_width0;
	double m_edit_trigger_delay0;

	double m_edit_trigger_start_pos1;
	double m_edit_trigger_end_pos1;
	double m_edit_trigger_pitch1;
	double m_edit_trigger_pulse_width1;
	double m_edit_trigger_delay1;

	double m_edit_trigger_start_pos2;
	double m_edit_trigger_end_pos2;
	double m_edit_trigger_pitch2;
	double m_edit_trigger_pulse_width2;
	double m_edit_trigger_delay2;

	double m_edit_trigger_start_pos3;
	double m_edit_trigger_end_pos3;
	double m_edit_trigger_pitch3;
	double m_edit_trigger_pulse_width3;
	double m_edit_trigger_delay3;

	CString m_edit_trigger_mode0;
	int m_edit_trigger_trigge_count0;
	int m_edit_trigger_encoder_count0;
	double m_edit_trigger_encoder_pos0;

	CString m_edit_trigger_mode1;
	int m_edit_trigger_trigge_count1;
	int m_edit_trigger_encoder_count1;
	double m_edit_trigger_encoder_pos1;

	CString m_edit_trigger_mode2;
	int m_edit_trigger_trigge_count2;
	int m_edit_trigger_encoder_count2;
	double m_edit_trigger_encoder_pos2;

	CString m_edit_trigger_mode3;
	int m_edit_trigger_trigge_count3;
	int m_edit_trigger_encoder_count3;
	double m_edit_trigger_encoder_pos3;

	void TriggerInfo();
	void TriggerInfo(int ch);
	UINT static TiggerInfoThread(LPVOID pParam);
	CWinThread *m_pThreadTriggerInfo;
	BOOL m_bTriggerThreadEnd;
	BOOL m_bTriggerThreadStart;
	

	void Wait(DWORD dwMillisecond);
	void SendWait();
	void SendCommand(CString senddata);
	BOOL b_GetInformation;

	CCommThread m_CommThread;

	BOOL m_bTriggerConnection;

	void Update(BOOL isUpdate, int idx);
	void ControlInit();
	void ControlEnable(BOOL bEnable);
	void TriggerOpen();
	void TriggerSetting(int idx);
	void StatusEnable(int ch, BOOL bEnable);
	void GetInformation(UINT n_ch, UINT n_getid);
	void SendOPMode(BOOL isOPMode);
	void SendMode1Direction(BOOL isDirection);
	void SendMode1StartEndPosition(UINT n_ch, double d_Start, double d_End, double d_EncRes);
	void SendMode1Pitch(UINT n_ch, double d_pitch, double d_EncRes);
	void SendMode1PulseWidth(UINT n_ch, double d_pulsewidth);
	void SendMode1Delay(UINT n_ch, double d_delay);
	void SendResetEncoderCnt();
	void SendResetTriggerCnt();
	void SendTriggerModeInfo();
	void SendTriggerCountInfo();
	void SendEncoderCountInfo();
	void SendTriggerModeInfo(int ch);
	void SendTriggerCountInfo(int ch);
	void SendEncoderCountInfo(int ch);
	CString byIndexComPort(int xPort);	// 포트 이름 받기 
	DWORD byIndexBaud(int xBaud);		// 전송률 받기
	BYTE byIndexData(int xData);		// 데이터 비트 받기
	BYTE byIndexStop(int xStop);		// 스톱 비트 받기 
	BYTE byIndexParity(int xParity);	// 패리티 받기
	LRESULT OnCommunication(WPARAM wParam, LPARAM lParam);
	CString m_ResultMessage;
	BOOL ReviewReceivedData(CString sRcvData);

	int	set_recipe( const char * );
	CString get_recipe(int cam_idx);
	void RecipeListToFile(CString filename, int camidx);
	void RecipeFileToList(CString filename);
	void RecipeFileToSystemDialog(int cam_idx);
public:
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlTriggerConnection();
	afx_msg void OnCbnCloseupComboTriggerSelectCam();
	void ClickBtnenhctrlTriggerApply();
	void ClickBtnenhctrlTriggerSave();
	void ClickBtnenhctrlTriggerCancel();
	afx_msg void OnCbnSelchangeComboTriggerSelectCam();
	void ClickBtnenhctrlTriggerStatusEnable0();
	void ClickBtnenhctrlTriggerStatusEnable1();
	void ClickBtnenhctrlTriggerStatusEnable2();
	void ClickBtnenhctrlTriggerStatusEnable3();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
