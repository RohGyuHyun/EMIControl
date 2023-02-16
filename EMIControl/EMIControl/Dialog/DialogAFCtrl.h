#pragma once
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "afxwin.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include <math.h>
#include "atf_def.h"
#include "atf_lib.h"
#include "common_AF.h"
#include "ui_cmd.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"

// invalid panel handle value
#define INVALID_PANEL_HANDLE -1

#define	OBJ_NUM_1	0
#define OBJ_NUM_2	1
#define OBJ_NUM_3	2

#define  PANEL_CVD                        1
#define  PANEL_CVD_RING_CVD_NUMSET        2       /* control type: ring, callback function: (none) */
#define  PANEL_CVD_CHECKBOX_CVD_ROLLING   3       /* control type: radioButton, callback function: (none) */
#define  PANEL_CVD_COMMANDBUTTON_CVD      4       /* control type: command, callback function: AutoFocus */

#define  PANEL_HM                         2
#define  PANEL_HM_TEXTMSG_1               2       /* control type: textMsg, callback function: (none) */
#define  PANEL_HM_NUMERIC_MV1             3       /* control type: numeric, callback function: (none) */
#define  PANEL_HM_NUMERIC_MV2             4       /* control type: numeric, callback function: (none) */
#define  PANEL_HM_NUMERIC_MV3             5       /* control type: numeric, callback function: (none) */
#define  PANEL_HM_NUMERIC_MV4             6       /* control type: numeric, callback function: (none) */
#define  PANEL_HM_TEXTMSG_2               7       /* control type: textMsg, callback function: (none) */
#define  PANEL_HM_NUMERIC_SPEED1          8       /* control type: numeric, callback function: (none) */
#define  PANEL_HM_NUMERIC_SPEED2          9       /* control type: numeric, callback function: (none) */
#define  PANEL_HM_NUMERIC_SPEED3          10      /* control type: numeric, callback function: (none) */
#define  PANEL_HM_NUMERIC_SPEED4          11      /* control type: numeric, callback function: (none) */
#define  PANEL_HM_COMMANDBUTTON_RUN       12      /* control type: command, callback function: HomingGo */
#define  PANEL_HM_COMMANDBUTTON_SET       13      /* control type: command, callback function: HomingSet */
#define  PANEL_HM_COMMANDBUTTON_GET       14      /* control type: command, callback function: HomingGet */
#define  PANEL_HM_COMMANDBUTTON_CLOSE     15      /* control type: command, callback function: HomingClose */

#define  PANEL_ML                         3
#define  PANEL_ML_NUMERIC_MINUSLIM        2       /* control type: numeric, callback function: (none) */
#define  PANEL_ML_NUMERIC_PLUSLIM         3       /* control type: numeric, callback function: (none) */
#define  PANEL_ML_COMMANDBUTTON_MLCLOSE   4       /* control type: command, callback function: MotionLimitClose */
#define  PANEL_ML_COMMANDBUTTON_MLUPDAT   5       /* control type: command, callback function: MotionLimitUpdate */
#define  PANEL_ML_CHECKBOX_ADAPTIVE       6       /* control type: radioButton, callback function: (none) */

#define  PANELM                           4       /* callback function: OtherSubsystemChanges */
#define  PANELM_TEXTMSG_2                 2       /* control type: textMsg, callback function: (none) */
#define  PANELM_LED_IN_FOCUS              3       /* control type: LED, callback function: (none) */
#define  PANELM_LED_IN_RANGE              4       /* control type: LED, callback function: (none) */
#define  PANELM_LED_REG_15                5       /* control type: LED, callback function: (none) */
#define  PANELM_LED_REG_7                 6       /* control type: LED, callback function: (none) */
#define  PANELM_LED_REG_0                 7       /* control type: LED, callback function: (none) */
#define  PANELM_LED_REG_9                 8       /* control type: LED, callback function: (none) */
#define  PANELM_LED_REG_1                 9       /* control type: LED, callback function: (none) */
#define  PANELM_LED_REG_8                 10      /* control type: LED, callback function: (none) */
#define  PANELM_LED_LLC                   11      /* control type: LED, callback function: (none) */
#define  PANELM_LED_REG_10                12      /* control type: LED, callback function: (none) */
#define  PANELM_TEXTMSG_LP                13      /* control type: textMsg, callback function: (none) */
#define  PANELM_NUMERICSLIDE_LP_7         14      /* control type: scale, callback function: (none) */
#define  PANELM_NUMERICSLIDE_LP_6         15      /* control type: scale, callback function: (none) */
#define  PANELM_NUMERICSLIDE_LP_5         16      /* control type: scale, callback function: (none) */
#define  PANELM_TIMER_MAIN                17      /* control type: timer, callback function: MainTimerTick */
#define  PANELM_NUMERICSLIDE_LP_3         18      /* control type: scale, callback function: (none) */
#define  PANELM_NUMERICSLIDE_LP_2         19      /* control type: scale, callback function: (none) */
#define  PANELM_NUMERICSLIDE_LP_4         20      /* control type: scale, callback function: LpChange */
#define  PANELM_STRING_SENSORSN           21      /* control type: string, callback function: (none) */
#define  PANELM_RING_OBJ                  22      /* control type: ring, callback function: ObjChangeM */
#define  PANELM_RING_BAUD                 23      /* control type: ring, callback function: ChangeBaud */
#define  PANELM_RING_COM                  24      /* control type: ring, callback function: SelectCom */
#define  PANELM_TOGGLEBUTTON_LASER        25      /* control type: textButton, callback function: LaserOnOff */
#define  PANELM_TOGGLEBUTTON_AF           26      /* control type: textButton, callback function: AutoFocus */
#define  PANELM_TOGGLEBUTTON_AF_OFF       27      /* control type: textButton, callback function: AutoFocus */
#define  PANELM_TOGGLEBUTTON_AFVIDEO      28      /* control type: textButton, callback function: AutoFocus */
#define  PANELM_TOGGLEBUTTON_AF_TRK       29      /* control type: textButton, callback function: AutoFocus */
#define  PANELM_TOGGLEBUTTON_M_LIMIT      30      /* control type: textButton, callback function: MotionLimit */
#define  PANELM_TOGGLE_CVD                31      /* control type: textButton, callback function: CvdAf */
#define  PANELM_TOGGLEBUTTON_ABSPOS       32      /* control type: textButton, callback function: AbsPosToggle */
#define  PANELM_NUMERIC_ABS_ZPOS          33      /* control type: numeric, callback function: (none) */
#define  PANELM_COMMANDBUTTON_RABSPOS     34      /* control type: command, callback function: ResetAbsPos */
#define  PANELM_NUMERIC_MOVE_ZPOS         35      /* control type: numeric, callback function: (none) */
#define  PANELM_NUMERIC_JOGSPEED          36      /* control type: numeric, callback function: (none) */
#define  PANELM_PICTUREBUTTON_SUP         37      /* control type: pictButton, callback function: StepUpDown */
#define  PANELM_COMMANDBUTTON_STOP        38      /* control type: command, callback function: StopZ */
#define  PANELM_PICTUREBUTTON_SDN         39      /* control type: pictButton, callback function: StepUpDown */
#define  PANELM_RINGSLIDE_ZSIZE           40      /* control type: slide, callback function: (none) */
#define  PANELM_COMMANDBUTTON_HOME        41      /* control type: command, callback function: Home */
#define  PANELM_COMMANDBUTTON_MAKE0       42      /* control type: command, callback function: Make0 */
#define  PANELM_COMMANDBUTTON_EXIT        43      /* control type: command, callback function: ExitAll */
#define  PANELM_TOGGLEBUTTON_LC           44      /* control type: textButton, callback function: LcChange */
#define  PANELM_TEXTMSG_SNRMODE           45      /* control type: textMsg, callback function: (none) */
#define  PANELM_NUMERICSLIDE_LP_1         46      /* control type: scale, callback function: (none) */
#define  PANELM_NUMERICSLIDE_SENSOR       47      /* control type: scale, callback function: (none) */
#define  PANELM_NUMERIC_SENSOR            48      /* control type: numeric, callback function: (none) */
#define  PANELM_TEXTMSG_MODE              49      /* control type: textMsg, callback function: (none) */
#define  PANELM_LED_COMM_OK               50      /* control type: LED, callback function: (none) */
#define  PANELM_DECORATION_6              51      /* control type: deco, callback function: (none) */
#define  PANELM_DECORATION_14             52      /* control type: deco, callback function: (none) */
#define  PANELM_DECORATION_16             53      /* control type: deco, callback function: (none) */
#define  PANELM_DECORATION_12             54      /* control type: deco, callback function: (none) */
#define  PANELM_DECORATION_2              55      /* control type: deco, callback function: (none) */
#define  PANELM_DECORATION                56      /* control type: deco, callback function: (none) */
#define  PANELM_DECORATION_3              57      /* control type: deco, callback function: (none) */
#define  PANELM_DECORATION_4              58      /* control type: deco, callback function: (none) */
#define  PANELM_DECORATION_5              59      /* control type: deco, callback function: (none) */
#define  PANELM_DECORATION_7              60      /* control type: deco, callback function: (none) */
#define  PANELM_DECORATION_8              61      /* control type: deco, callback function: (none) */
#define  PANELM_SPLITTER                  62      /* control type: splitter, callback function: (none) */
#define  PANELM_TEXTMSG_INFO              63      /* control type: textMsg, callback function: (none) */

#define MAX_LLC_OBJ_NUM		8	// maximum number of possible LLC objectives
#define MAX_LLC_FAILURES	5	// maximum connection attemts to LLC

#define AF_LED_CHANNEL 0

#define EZ_CTS_MM				10000

#define AFCTRL_PATH							_T("D:\\DATABASE\\SYSTEM")

// CDialogAFCtrl 대화 상자입니다.

class CDialogAFCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogAFCtrl)

public:
	CDialogAFCtrl(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogAFCtrl();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_AF_CTRL };

	HICON m_hIcon;							//Ezi
	BYTE	m_nPort;
	BYTE	m_iSlaveNo;
	BOOL	m_bConnected;
	static const int	DIR_INC = 1;
	static const int	DIR_DEC = 0;
	long	m_nCmdPosition;
	long	m_lMoveVelocity;
	long	m_lJogVelocity;	
	long	m_lEzdistance_old;
	long	m_lEzdistance_new;			
	long	m_lEzdistance_difference;				//old ,new 차이값
	long	m_lEzdistance_cumulative;				//누적 거리값
	double	m_lEzdistance_cumulative_mm;
	long	m_lEzMaxdistance;

	
											//Ezi


	int m_iErrOk;	
	short m_mag[3];				
	int m_infocus[3];
	float m_acc[3];
	unsigned short m_fullstep[3];
	unsigned short m_microstep[3];
	float m_speed[3];
	float m_slope[3];
	float m_p[3];

	/*
[0]

Infocus 3
Slope 8.250000
P 0.799805
Accel 80
Speed 8.000000
RowDelay 0 0 0
IntegrationTime 23 8 12
BeheadingThr 0.16 0.08 0.08
LineWidth5x 64 8 16
ZDelta 0
ABC_Delta 0
ABC_LaserPower 0
*/

	int m_cur_objective;		
	short m_cur_mag;									//배율 값
	int m_cur_infocus;
	float m_cur_readp;
	float m_cur_accel;
	unsigned short m_cur_steppermm;
	unsigned short m_cur_microstep;
	short m_cur_duvjump;
	float m_cur_slope;							
	float m_cur_speed;
	int m_cur_readlastmovez;
	int m_cur_abszpos;
	int SetAfLlc(int iRet);
	void SetAfMove(BOOL bRet,int nstep);
	void SetReviewLed(unsigned short LedPwm);
	unsigned short GetReviewLed();
	BOOL SetAFTrackRun(BOOL bOn);
	BOOL GetAFInfocus();
	BOOL LoadAFCtrl(CString path);
	BOOL SaveAFCtrl(CString path);	
	void SetEziMove_mm(double pos);
	double GetEziPos_mm();
	BOOL GetEzPosComplete(int scan_type_idx);
	void SetEzSpeed(int speed);
	BOOL GerEzMoterOrigin();
	
	void SetEzHome();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	BOOL m_bEzMoterOrigin;
	int m_af_parameter;
	int m_mfc_parameter;
	int m_analog_only_sensor;
	int m_analog_move_sensor;
	int m_analog_or_ctl_move_sensor;
	int m_flag_cm;
	int m_more_param;
	int m_mfc_conf;

	const char *m_pAoiTest;
	const char *m_pCm;
	const char *m_pNumberOfLenses;
	const char *m_patf5;
	const char *m_pATF4;
	const char *m_pATF5;
	const char *m_patf6;
	const char *m_pATF6;
	const char *m_pATF7;
	const char *m_pATFX;
	const char *m_pATF2;
	const char *m_pSensorName;

	const char *m_pHwCog;

	int m_main;
	int m_mb;
	int m_scanline;
	char m_FirmwareBuffer[64];
	char m_FirmwareVer[64];

	MeasureDataType m_measure;
	float m_step_per_mm;
	EnumWindowMode m_win_mode;

	int m_flag_split_cm;
	int m_flag_ao;
	int m_separation;
	int m_separationOption;

	bool m_bIsLlcInit;

	bool m_GuiAutoOff;		// true if laser is off till AF is started
	bool m_bAfEnable;		// true if AF is started

	bool m_IsVideoAvailable;		// enable video AF if true
	bool m_IsRetarderPresent;		// true if retarder control is present
	// update text info with some additinal information
	int m_ZTrackingCnt;		// count timer calls while tracking focus
	int m_iLlcObjCtrlNum[3];

	// panel handles
	int m_cvdaf;			// cvd af panel handle

	// CVD AF
	int m_cvd_numset;
	int m_cvd_rolling_not;
	// motion limit
	int m_upper_lim;
	int m_lower_lim;
	int m_adaptive_lim;
	int m_illcoffset[3];
	CString m_strlog;
	
	// move LLC to predefined position - called after pressing one of "#1", "#2" ... "#8" buttons
	int LlcMoveToObjective(int control);
	// change objective lens selection
	bool ObjChangeLocal (int obj);
	// switch laser power ON/OFF - called after pressing "Laser On/Off" button
	int LaserOnOff (int panel,  int button);
	// activate additional portion of the GUI based on extra rights granted by the parameter panel
	void MainMoreParams ();
	// change laser mode to auto/manual - called after pressing laser "Auto/Manual" button
	int LcChange (int panel, int control);
	// updates GUI when auto focus mode is disabled
	void GuiAfUpdate (bool bEnabled) ;
	bool WriteMotionLimitsIfEnabled(int button_motionlimit);
	void CheckMotionCapability();
	// get and update motor parameters: SP, AC, DC and SF
	void getLlcParameters();
	// evaluate config file parameter against firmware version
	bool IsVersionOK(char *pVersion);
	// get sensor information, like: sensor type, firmware version, serial number etc.
	void GetSensorInformation(void);
	// change objective lens - called after pressing objective ring button
	int ObjChangeM (int control, int eventData1, int eventData2);

	BOOL SaveAFLLCOffset(CString path);

	BOOL LoadAFLLCOffset(CString path);

	void AFLLCOffsetUpdate();

	

	

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	virtual BOOL OnInitDialog();

	void ClickBtnenhctrlAfConnect1();
	void ClickBtnenhctrlAfJogP1();
	void ClickBtnenhctrlMotJogM4();
	void ClickBtnenhctrlAfStop1();
	void ClickBtnenhctrlAfHome1();
	void ClickBtnenhctrlAfTrackRun();
	void ClickBtnenhctrlAfLaserone();
	void ClickBtnenhctrlAfAoiTrackRun();
	void ClickBtnenhctrlAfLaseron();
	void ClickBtnenhctrlAfMake0();
	void AFinfocusrangestate();
	BOOL AFlaseronoffstate();

	afx_msg void OnEnChangeEditAfMicroStep();
	afx_msg void OnEnChangeEditAfMaxVel2();


	int AFGetCurPos();

	
	int m_edit_af_microstep;
	int m_edit_af_obj_num;
	int m_edit_af_infocusrange;
	float m_edit_af_max_acc;
	float m_edit_af_max_vel;
	float m_edit_af_fullstep;
	
	CBtnenhctrl m_btn_af_inrange;
	CBtnenhctrl m_btn_af_infocus;
	CBtnenhctrl m_btn_af_laseron;
	CBtnenhctrl m_btn_af_laserone;
	CBtnenhctrl m_btn_af_track_run;
	CBtnenhctrl m_btn_af_make0;
	CBtnenhctrl m_btn_af_save;
	CBtnenhctrl m_btn_af_pos1;
	CBtnenhctrl m_btn_af_aoi_track_run;
	CBtnenhctrl m_btn_af_connect;


	void CurrentObjInfo();
	
	double m_edit_ez_spd;							//Ezi
	double m_edit_ez_move_pos;
	void MouseDownBtnenhctrlEzJogP0(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlEzJogP0(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlEzJogM0(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlEzJogM0(short Button, short Shift, long x, long y);
	void ClickBtnenhctrlEzHome0();
	void ClickBtnenhctrlEzMove0();
	void ClickBtnenhctrlEzStop0();
	void ClickBtnenhctrlEzConnect0();
	long EZGetCurPos();
	afx_msg void OnEnChangeEditEzSpd();
	void EziCycleOiling();
	short m_Curoliling;
	short m_Cycleoliling;
	
	CBtnenhctrl m_btn_ez_pos;						//Ezi
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	int m_radio_review;
	afx_msg void OnBnClickedRadioReview1();
	afx_msg void OnBnClickedRadioReview2();
	afx_msg void OnBnClickedRadioReview3();

	void AFWriteLedCtrl(unsigned short LedPwm);
	unsigned short AFGetLedCtrl();
	
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void ClickBtnenhctrlEz();
	void ClickBtnenhctrlAf();
	CBtnenhctrl m_btn_ez_cumulative_distance;
	int m_edit_llc_offset0;
	int m_edit_llc_offset1;
	int m_edit_llc_offset2;

	void ClickBtnenhctrlLlcapply();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ClickBtnenhctrlEzSpdSet0();
	CBtnenhctrl m_btnEzOrigin;
	void ClickBtnenhctrlEzOilComp();
	CBtnenhctrl m_btn_ez_cycle_oiling;
};
