#pragma once
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
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"
#include "d:\working\emi\emicontrol\emicontrol\emicontrol\btnenhctrl.h"

// CDialogMotorCtrl 대화 상자입니다.
#define IO_MODULE_NUMBER				16		// 16 20150619 syz 

#define STATION_NUMBER					32		// CC Link Station Point 갯수
#define INPUT_MODULE_NUMBER				16
#define OUTPUT_MODULE_NUMBER			16

typedef struct {
	int nID;						// MOTOR_SCAN/...
	int nAxisNO;					// UMAC 상의 축 번호
	CString strName;
	double dbNegPostionLimit;
	double dbPosPostionLimit;
	double dbSpeedLimit;
	double dbResolution;
} SYS_MOTOR_STAGE;

#define MAX_MOTOR						4
#define MOTOR_0							0
#define MOTOR_1							1
#define MOTOR_2							2
#define MOTOR_3							3

#define DIR_NOMOVE						0
#define DIR_POSITIVE					1
#define DIR_NEGATIVE					2

#define	RES_MOTOR_1	0.15625f				//(um/count)
#define RES_MOTOR_2	0.15625f				//(um/count)
#define RES_MOTOR_3	0.15625f				//(um/count)
#define RES_MOTOR_4	0.15625f				//(um/count)

#define MANUAL_MOVE_SPEED				10.0 

#define MOTOR_0_ID				1
#define MOTOR_1_ID				2
#define MOTOR_2_ID				5
#define MOTOR_3_ID				6

#define MOTOR_0_CTS_PER_1MM				6400
#define MOTOR_1_CTS_PER_1MM				6400
#define MOTOR_2_CTS_PER_1DEG			15360
#define MOTOR_3_CTS_PER_1MM				200
//cts 
#define MOTOR_0_M_LMT_POS				((-32091) + MOTOR_0_CTS_PER_1MM)
#define MOTOR_0_P_LMT_POS				(5954767 - MOTOR_0_CTS_PER_1MM)

#define MOTOR_1_M_LMT_POS				((-32112) + MOTOR_1_CTS_PER_1MM)
#define MOTOR_1_P_LMT_POS				(4751583 - MOTOR_1_CTS_PER_1MM)

#define MOTOR_2_M_LMT_POS				((-45800) + MOTOR_2_CTS_PER_1DEG)
#define MOTOR_2_P_LMT_POS				(1408601 - MOTOR_2_CTS_PER_1DEG)

#define MOTOR_3_M_LMT_POS				((-2354) + MOTOR_3_CTS_PER_1MM)
#define MOTOR_3_P_LMT_POS				(10993 - MOTOR_3_CTS_PER_1MM)
//cts
#define MOTOR_0_LMT_SPD			2000
#define MOTOR_1_LMT_SPD			2000
#define MOTOR_2_LMT_SPD			500
#define MOTOR_3_LMT_SPD			200

#define MOTOR_INPOS_LMT			10
#define MOTOR_INPOS_MM			0.001

#define MOTOR_PATH						"D:\\DATABASE\\SYSTEM"

typedef std::map<CString, int> MOTOR_STATUS;
typedef std::map<UINT, float> LAST_POSITION;


class CDialogMotorCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogMotorCtrl)

public:
	CDialogMotorCtrl(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogMotorCtrl();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MOTOR_CTRL };

public:
	double GetRobotPos_mm(int axi);
	double GetRobotPos_cts(int axi);
	BOOL SetRobotMove_mm(int axi, double pos_mm);
	BOOL SetRobotMove_mm(int axi, double pos_mm, double spd);
	BOOL SetRobotMove_cts(int axi, double pos_cts);
	void SetRobotJogMove(int axi, int direct, int spd_select);
	void SetRobotJogStop(int axi);
	BOOL GetIsInposition(int axi);
	BOOL GetIsMoveComplete(int axi, double pos);
	double Get8KScanSpeed();
	double Get16KScanSpeed();
	void GetIsServoOriginComplete();
	BOOL GetServoOriginStatus(int axi);
	void SetMoterAllOrigin();

	//20160719 ngh
	double GetCtsToMM(int nAxis, double dCts);
	double GetMMToCts(int nAxis, double dMm);

	void SetMotorStop(int nAxis);

	//20160727 ngh
	BOOL m_bMotionJogDrawCrowsStart;
	BOOL m_bMotionJogDrawCrowsEnd;

protected:
	CPMACComm m_PMAC;

	CRITICAL_SECTION m_csMachine;
	BOOL m_bCommandLock;

	LAST_POSITION m_dmLastPostion;
	MOTOR_STATUS m_dmMotorStatus;

	BYTE m_InputModule[INPUT_MODULE_NUMBER];
    BYTE m_OutputModule[OUTPUT_MODULE_NUMBER];

	int m_iProcessMoveIdx;

	double GetCurPos(int nAxis);
	double GetCurSpeed(int nAxis);
	BOOL ReadServoState();
	double ConvertPosition(int nAxis, double dbPosition, BOOL bCount);
	double ConvertSpeed(int nAxis, double dbSpeed, BOOL bCount);
	void ServoStop(int nAxis);
	void ServoOrigin(int nAxis);
	void ServoJogMove(int nAxis, int nDirection, double dbSpeed);
	BOOL ServoMove(int nAxis, double dbPosition, double dbSpeed);
	BOOL IsServoAmpEnable(int nAxis);
	BOOL IsServoAmpFault(int nAxis);
	BOOL IsServoFollowingError(int nAxis);
	BOOL IsServoI2TFault(int nAxis);
	BOOL IsServoOriginComplete(int nAxis);
	BOOL IsServoInPosition(int nAxis);
	void Update(BOOL flag);
	BOOL LoadMotorPara(CString path);
	BOOL SaveMotorPara(CString path);
	BOOL IoStateCheck();
	void Wait(DWORD dwMillisecond);
	

	//20160406 ngh
	BOOL m_bOriginWait[MAX_MOTOR];

	//20160530 ngh
	void MotorAllOrigin();

	//20160720 ngh
	int GetAllOriginFalseIdx();

	//20160721 ngh
	CWinThread   *m_pThreadMotionOrigin;
    UINT static   MotionOriginThread(LPVOID pParam);
	void MotionOriginThread();
	BOOL m_bMotorOriginStart;
	BOOL m_bMotorOriginEnd;
	BOOL WriteLog(CString strLog);

	//20160721 ngh
	CWinThread   *m_pThreadMotionJogDrawCrows;
    UINT static   MotionJogDrawCrowsThread(LPVOID pParam);
	void MotionJogDrawCrowsThread();
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
protected:
	CBtnenhctrl m_btn_ae0;
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlMotHome0();
	virtual BOOL OnInitDialog();
	void ClickBtnenhctrlMotHome1();
	void MouseDownBtnenhctrlMotJogP0(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlMotJogP0(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlMotJogM0(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlMotJogM0(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlMotJogP1(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlMotJogP1(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlMotJogM1(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlMotJogM1(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlMotJogP2(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlMotJogP2(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlMotJogM2(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlMotJogM2(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlMotJogP3(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlMotJogP3(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlMotJogM3(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlMotJogM3(short Button, short Shift, long x, long y);
	void ClickBtnenhctrlMotHome2();
	void ClickBtnenhctrlMotStop0();
	CBtnenhctrl m_btn_mot_pos0;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CBtnenhctrl m_btn_mot_spd0;
	CBtnenhctrl m_btn_af0;
	CBtnenhctrl m_btn_fe0;
	CBtnenhctrl m_btn_i2t0;
	CBtnenhctrl m_btn_ae1;
	CBtnenhctrl m_btn_af1;
	CBtnenhctrl m_btn_fe1;
	CBtnenhctrl m_btn_i2t1;
	CBtnenhctrl m_btn_ae2;
	CBtnenhctrl m_btn_af2;
	CBtnenhctrl m_btn_fe2;
	CBtnenhctrl m_btn_i2t2;
	CBtnenhctrl m_btn_ae3;
	CBtnenhctrl m_btn_af3;
	CBtnenhctrl m_btn_fe3;
	CBtnenhctrl m_btn_i2t3;
	void ClickBtnenhctrlMotSave();
	void ClickBtnenhctrlMotApply();
	void ClickBtnenhctrlMotCancel();
	double m_edit_setspd0;
	double m_edit_setspd1;
	double m_edit_setspd2;
	double m_edit_setspd3;
	double m_edit_s_pos0;
	double m_edit_s_pos1;
	double m_edit_s_pos2;
	double m_edit_s_pos3;
	double m_edit_e_pos0;
	double m_edit_e_pos1;
	double m_edit_e_pos2;
	double m_edit_e_pos3;
	double m_edit_axi_name0;
	double m_edit_axi_name1;
	double m_edit_axi_name2;
	double m_edit_axi_name3;
	double m_edit_axi_id0;
	double m_edit_axi_id1;
	double m_edit_axi_id2;
	double m_edit_axi_id3;
	double m_edit_axi_p_lmt0;
	double m_edit_axi_p_lmt1;
	double m_edit_axi_p_lmt2;
	double m_edit_axi_p_lmt3;
	double m_edit_axi_m_lmt0;
	double m_edit_axi_m_lmt1;
	double m_edit_axi_m_lmt2;
	double m_edit_axi_m_lmt3;
	double m_edit_axi_spd_lmt0;
	double m_edit_axi_spd_lmt1;
	double m_edit_axi_spd_lmt2;
	double m_edit_axi_spd_lmt3;
	double m_edit_axi_res0;
	double m_edit_axi_res1;
	double m_edit_axi_res2;
	double m_edit_axi_res3;
	double m_edit_op_spd0;
	double m_edit_op_spd1;
	double m_edit_op_spd2;
	CBtnenhctrl m_btn_mot_home0;
	CBtnenhctrl m_btn_mot_home1;
	CBtnenhctrl m_btn_mot_home2;
	CBtnenhctrl m_btn_mot_home3;
	void ClickBtnenhctrlMotHome3();
//	void AnimDLLDoneBtnenhctrlMotStop1(long iPicture, BOOL bResult);
	void ClickBtnenhctrlMotStop2();
	void ClickBtnenhctrlMotStop1();
	void ClickBtnenhctrlMotStop3();
	void ClickBtnenhctrlMotAllStop();
	CBtnenhctrl m_btn_mot_pos1;
	CBtnenhctrl m_btn_mot_spd1;
	CBtnenhctrl m_btn_mot_pos2;
	CBtnenhctrl m_btn_mot_spd2;
	CBtnenhctrl m_btn_mot_pos3;
	CBtnenhctrl m_btn_mot_spd3;
	void ClickBtnenhctrlMotRepeat0();
	void ClickBtnenhctrlMotRepeat1();
	void ClickBtnenhctrlMotRepeat2();
	void ClickBtnenhctrlMotRepeat3();
	CBtnenhctrl m_btn_progress_move;
public:
	double m_edit_op_y_spd0;
	double m_edit_op_y_spd1;
	double m_edit_op_y_spd2;
	double m_edit_op_t_spd0;
	double m_edit_op_t_spd1;
	double m_edit_op_t_spd2;
	double m_edit_op_z_spd0;
	double m_edit_op_z_spd1;
	double m_edit_op_z_spd2;
	void ClickBtnenhctrlMotMove0();
	void ClickBtnenhctrlMotMove1();
	void ClickBtnenhctrlMotMove2();
	void ClickBtnenhctrlMotMove3();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	double m_edit_8k_scan_speed;
	double m_edit_16k_scan_speed;
	CBtnenhctrl m_btnMot3_P3;
	CBtnenhctrl m_btnMot3_M3;
	void ClickBtnenhctrlMotAllHome();
};
