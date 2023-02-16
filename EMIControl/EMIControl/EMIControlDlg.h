
// EMIControlDlg.h : 헤더 파일
//

#pragma once
#include "btnenhctrl.h"
#include "Include/_CtrlImageRect.h"
#include "include/_FuncModuleHal.h"
#include "include/_FuncModuleBasic.h"

#include "include/Runtime.h"
#include "include/PMACComm.h"

#include "Clientsok.H"

#include "Dialog/AccessRight.h"
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
#include "Dialog/DialogRecipeCellPCL.h"
#include "Dialog/DialogRecipeScan.h"
#include "Dialog/DialogGlassManuCtrl.h"
#include "Dialog/DialogFilmAlign.h"
#include "Dialog/DialogTriggerCtrl.h"
#include "ProcessBar.h"


#include "Dialog\DialogLogAlarm.h"
#include "Dialog/Dialogmsg.h"
#include "DialogLogWindow.h"
#include "Dialog/DialogDFServer.h"
#include "Dialog/DefectList.h"

#define _TEST_VALUE

#define IPS_MAX_GRAB_CNT					20

#define IPS_IP_ADDRESS						_T("127.0.0.1")//_T("192.168.5.1")
#define FILM_RESULT_FILEPATH				_T("D:\\Database\\FILM_ALIGN\\")
#define IPS_PORT_NUM						3011

#define MOT_PROCESS_MAX_NUM					1
#define MOT_SEQ_MAX_NUM						4
#define MOT_STATUS_MAX_NUM					MOT_SEQ_MAX_NUM
#define MOT_STATUS_SUB_MAX_NUM				2000

#define TIMEOUT_DECISION					10
#define TIMEOUT_SAFE_SENSOR					10
#define TIMEOUT_CYL_SENSOR					5000
#define TIMEOUT_MOTOR_MOVE					50000//ms
#define TIMEOUT_IPS_TCPIP_RCV				30000
#define TIMEOUT_ECS							10000

#define MAX_STRING_LINE						30000

#define USER_MSG_MOT						WM_USER+21

#define DEFECT_SIZE_SMALL					0
#define DEFECT_SIZE_MEDIUM					1
#define DEFECT_SIZE_LARGE					2
#define DEFECT_SIZE_OVER_FLOW				3
#define DEFECT_SIZE_BLACK_WHITE				4

#define BLACK_DEFECT				0
#define BLACK_WHITE_DEFECT			1

#define FILM_ALIGN_RSLT_MAX_NUM				1000

#define FILM_POS_LT							0
#define FILM_POS_MT							1
#define FILM_POS_RT							2
#define FILM_POS_RB							3
#define FILM_POS_MB							4
#define FILM_POS_LB							5


#define MAIN_LOG							0
#define GLASS_LOG							1
#define ALIGN_LOG							2
#define REVIEW_LOG							3
#define MOTOR_LOG							4
#define LIGHT_LOG							5
#define IO_LOG								6
#define EZ_LOG								7
#define ALARM_LOG							8
#define RECIPE_LOG							9
#define FILM_ALIGN_LOG						10
#define DEFECT_SERVER_LOG					11
#define INLINE_LOG							12

#define MAIN_LOG_FILE_NAME					_T("Main_Log.log")
#define GLASS_LOG_FILE_NAME					_T("Glass_Log.log")
#define ALIGN_LOG_FILE_NAME					_T("Align_Log.log")
#define REVIEW_LOG_FILE_NAME				_T("Review_Log.log")
#define LIGHT_LOG_FILE_NAME					_T("Light_Log.log")
#define IO_LOG_FILE_NAME					_T("Io_Log.log")
#define MOTOR_LOG_FILE_NAME					_T("Motor_Log.log")
#define EZ_LOG_FILE_NAME					_T("Ez_Log.log")
#define ALARM_LOG_FILE_NAME					_T("Alarm_Log.log")
#define RECIPE_LOG_FILE_NAME				_T("Recipe_Log.log")
#define FILM_ALIGN_LOG_FILE_NAME			_T("Film_Align_Log.log")
#define DEFECT_SERVER_LOG_FILE_NAME			_T("Defect_Server_Log.log")
#define INLINE_LOG_FILE_NAME				_T("Inline_Log.log")

//Motion process status parameter
typedef struct MotionProcessStatus
{
	//I/O status
	int m_bIOIn;

	//Timeout
	double m_dTimeoutS;//시작 시간
	double m_dTimeoutE;//종료 시간(현재)
	double m_dTimeout;//지정된 시간
	
	//Status문자(에러 또는 정상종료시 출력할 문자열)
	char m_cNGStatus[DAT_STR_LENG];
	int m_iNGCode;
	int m_iNGCategory;
	int m_iNGLevel;

	char m_cGoodStatus[DAT_STR_LENG];
	char m_iTypeStatus[DAT_STR_LENG];
	
	int m_iNGSeqIdx;

}typeMotionProcessSts;

typedef struct MotionSeq
{
	//Sequence
	int m_iSubSeqIdx;
	int m_iDispSubSeqIdx;
	int m_iNextSubSeqIdx;

	int m_iSubRoutineIdx;

	//Flag
	BOOL m_bAutoStart;
	BOOL m_bHomeOK;
	BOOL m_bHomeStart;
	BOOL m_bEmerAlarm;
	BOOL m_bLotStart;//0 : 정상 상태, 1 : 남은상태(정상상태가 아니면 남은상태에 따라서 비젼 검사를 하지않는다)
	BOOL m_bLotEnd;
	BOOL m_bAlarm;


	//버튼중심의 동작상태
	BOOL m_bOpSts;

	//정보
	int m_iDeviceIdx;


}typeMotionSeq;

//Motion flag
typedef struct MotionFlag
{
	BOOL m_bProcessThread;
	
	BOOL m_bAutoStart;
	BOOL m_bMainHomeStart;

	BOOL m_bFirstStart;

	BOOL m_bGlassExist;
}typeMotionFlag;

typedef struct ProcessResult
{
	//Motion
	double m_dMotPos[MAX_MOTOR];

	//Scan
	int m_iScanTypeIdx;

	int m_iScanIdxX;
	int m_iScanIdxY;

	int m_ScanCamIdx;
	int m_ScanComplete[RECIPE_SCAN_TYPE_MAX_NUM];

	//Scan rslt read
	int m_iScanIdx;

	//Film align
	int m_iFilmAlignRgnIdx;
	int m_iFilmAlignIdx;
	int m_iFilmAlignColIdx;
	int m_iFilmAlignRowIdx;

	//pat
	long m_iSmallCnt;
	long m_iMediumCnt;
	long m_iLargeCnt;
	long m_iOverflowCnt;
	long m_iPatCnt;

	//white
	long m_iWhiteSmallCnt;
	long m_iWhiteMediumCnt;
	long m_iWhiteLargeCnt;
	long m_iWhiteOverflowCnt;
	long m_iWhiteCnt;

	//sum cnt;
	long m_iSumSmallCnt;
	long m_iSumMediumCnt;
	long m_iSumLargeCnt;
	long m_iSumOverflowCnt;
	long m_iSumCnt;

	//Film aling result
	int m_FilmAlignIdx;
	double m_FilmAlignX[FILM_ALIGN_RSLT_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];
	double m_FilmAlignY[FILM_ALIGN_RSLT_MAX_NUM][FILM_ALIGN_RGN_POS_MAX_MUM];

	

	//ECS///////////////////////////////////////////////////////////////////
	
	//Loader/unloader
	BOOL m_bWaitLoading;
	BOOL m_bCompleteLoading;
	BOOL m_bCassetEmpty;

	char m_sLoaderUnloaderErrMsg[DAT_STR_LENG];
	int m_iLoaderUnloaderErrCode;

	//Mech align////////////////////////////////////////////////////////
	int m_iMechAlignSeq;

	//Inspect Sequence
	BOOL m_bStartManualInspect;
	BOOL m_bInspectionMove;

	//20160328 ngh
	//long m_iBlackWhiteCnt;
}typeProcessResult;

class CMotionData  
{
public:

	CMotionData();
	virtual ~CMotionData();

	//Motion process status
	typeMotionProcessSts m_ProSts[MOT_STATUS_MAX_NUM][MOT_STATUS_SUB_MAX_NUM];

	//Motion sequence
	typeMotionSeq m_Seq[MOT_STATUS_MAX_NUM];

	void ResetSeq();
};

void _MakeDirectories(CString path);

// CEMIControlDlg 대화 상자
class CEMIControlDlg : public CDialogEx
{
// 생성입니다.
public:
	CEMIControlDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EMICONTROL_DIALOG };

public:
#ifdef _TEST_VALUE
	BOOL m_bTestValue;
#endif

	void SetLotInit();
	int GetScanTypeIdx();
	BOOL GetIPSConnect();
	void SetLoadingComplete(BOOL bit);
	void SetCassetEmpty(BOOL bit);
	int GetRsltScanTypeIdx();

	void SystemMotorCtrl(void);
	void SystemIOCtrl(void);
	void SystemAFCtrl(void);
	void SystemLightCtrl(void);
	void SystemIPSCtrl(void);
	void MainAlign(void);
	void RecipePanel(void);
	void RecipeCell(void);
	void RecipeCellPCL(void);
	void RecipeScan(void);
	void SystemGlassManuCtrl(void);
	void SystemTriggerCtrl(void);
	void FilmAlign(void);
	void ReviewCCD();

	CDialogMotorCtrl *m_Robot;
	CDialogLightCtrl *m_Light;
	CDialogRecipePanel *m_RecipePanel;
	CDialogRecipeCell *m_RecipeCell;
	CDialogRecipeCellPCL *m_RecipeCellPCL;
	CDialogRecipeScan *m_RecipeScan;
	CDialogAlign *m_Align;
	CDialogGlass *m_Glass;
	CDialogIOCtrl *m_IO;
	CDialogIPSCtrl *m_IPSCtrl;
	CDialogGlassManuCtrl *m_GlassManu;
	CDialogReview *m_Review;
	CDialogTriggerCtrl *m_Trigger;
	CDialogAFCtrl *m_AFCtrl;
	CDialogLogAlarm *m_LogAlarm;
	CDialogFilmAlign *m_FilmAlign;
	CDialogCCD *m_CCD;
	CDialogInline *m_Inline;
	CDialogRecipe *m_Recipe;
	CProcessBar *m_ProcessBar;
	CDialogLogWindow *m_Logwindow;
#ifdef DEFECT
	CDialogDFServer *m_Dfserver;
#endif
	CDefectList *m_DefectList;
	//20160530 ngh
	CAccessRight *m_AccessRight;
	void SetAccess(int accessType);
	void SetAccessType(int accessType);

	BOOL GetEnableMove();
	BOOL GetEnableDefectMove();

	//20151130 ngh
	CString GetGrabReadyPacket(int cam_idx, BOOL rotat, int type_idx, int scan_idx, BOOL derection, int grab_height);
	BOOL PacketSendIPS(CString packet);

	//20151208 ngh
	BOOL ReadDefectFile(int cam_idx, BOOL rotat, int scan_type_idx, int scan_idx, CString lot_id, int defectCnt);

	//20151210 ngh
	void SetRsltSizeCntView(int defect_size, BOOL bBlackWhite = FALSE);
	void ResetRsltSizeCntView();

	BOOL WriteInsResultFile(FILE *fp, CString file_pat, CString result_data);
	CString GetResultFilePath();
	

	BOOL SetLoadRecipeFile(CString path);
	BOOL SetSaveRecipeFile(CString path);

	void SetManualInspect(BOOL var);
	void SetWaitLoading(BOOL var);
	void SetRecipeToECS();
	//20160229 ngh
	void SetServerConnect();
	//20160307 ngh
	BOOL WriteLog(int log_type, CString text);
	BOOL GetDefectFileWriteBreak();

	void SetDefectFileWriteBreak(BOOL isOn);

	//20160315 ngh
	BOOL DefectErrorCnt(int scan_type_idx, int defect_idx);
	//int GetRsltDefectCnt(int defect_idx, BOOL bBlackWhite = FALSE);
	CString m_strMainMsg;

	//20160406 ngh
	BOOL OFDefectError(int scan_type_idx);
	BOOL GetDefectError();

	//20160418 SC
	int GetRsltDefectSumCnt(int defect_idx);
	int GetRsltDefectCnt(int defect_idx);
	int GetRsltDefectWhiteCnt(int defect_idx);

	CString m_film_image[100][6]; //20160425 sc

	//20160422 ngh
	void SetDefectServerFileReady(BOOL isOn);
	BOOL bManualDefectServer;
	void ParaCompareLogWrite(int para1, int para2, CString strName);
	void ParaCompareLogWrite(double para1, double para2, CString strName);
	void ParaCompareLogWrite(CString para1, CString para2, CString strName);

	//20160531 ngh
	void ProgramExit();

	BOOL m_bDfIdxFile;  // 1 : IDX 파일 찾음  0 : IDX 파일 못찾음

	BOOL GetIPSAutoRunState();

	//20160721 ngh
	int GetAccessType();
	void SetOperater(int access_type);
	CString GetOperater();

	//20160726 ngh
	BOOL m_bMachaAlignRepet;
	void SetManuSelect(int nManuType);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.



// 구현입니다.
protected:
	BOOL m_bTower;
	BOOL m_bAutoSQ;
	BOOL m_bDefectWriteComp;
	

	CRITICAL_SECTION	m_CSInline;

	int m_iScanTypeIdx;

	//Motion data class
	CMotionData m_MDat[MOT_PROCESS_MAX_NUM];

	//Flag
	typeMotionFlag m_Flag;

	//Process result
	typeProcessResult m_Rslt;

	//ECS
	CWinThread   *m_pThreadECS;
    UINT static   MotionThreadECS(LPVOID pParam);
	void MotionThreadECS();

	//Process
	CWinThread   *m_pThread;
    UINT static   MotionThread(LPVOID pParam);
	void MotionThread();
	int MotionProcess(int prc_idx, int seq_idx);

	//DefectFile
	CWinThread   *m_pThreadDefectFile;
    UINT static   DefectFileThread(LPVOID pParam);
	BOOL m_bDefectFileLoadStart;
	BOOL m_bDefectFileLoadEnd;
	BOOL m_bDefectFileWriteBreak;
	BOOL m_bDefectFileWriteComp[RECIPE_SCAN_TYPE_MAX_NUM];
	BOOL m_bDefectFileSave[RECIPE_SCAN_TYPE_MAX_NUM];

	//DefectServerFile
	CWinThread   *m_pThreadDefectServer;
    UINT static   DefectServerThread(LPVOID pParam);
	BOOL m_bDefectServerStart;
	BOOL m_bDefectServerEnd;
	BOOL m_bDefectServerFileMake;
	BOOL m_bDefectServerFileReady;
	BOOL m_bDefectServerImageMake;

	void SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx);
	void SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx, int next_seq_sub_idx, int type);
	void SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx, int next_seq_sub_idx, int type, int device_idx);
	int CheckTimeout(int prc_idx, int seq_idx, int seq_sub_idx, char* status_type);
	int CheckDelay(int prc_idx, int seq_idx, int delay);
	int AutoStart();
	int AutoStop();
	int AutoReset();
	void AlarmOperation(int pro_idx, int sts_idx, int sub_seq_idx);

	void WindowVisable_main();
	void WindowVisable_system();
	void WindowVisable_recipe();

	void MakeAlarmCode(int pro_idx, int sts_idx, int sub_seq_idx, int *category, int *code, int *level);

	HICON m_hIcon;

	CClientSock *m_Client;

	CDialogMainInfo *m_dlgMainInfo;
	CDialogSystem *m_dlgSystem;
	CDialogMotorCtrl *m_dlgMotorCtrl;
	CDialogIOCtrl *m_dlgIOCtrl;
	CDialogReview *m_dlgReview;
	CDialogLogin *m_dlgLogin;
	CDialogAFCtrl *m_dlgAFCtrl;
	CDialogLightCtrl *m_dlgLightCtrl;
	CDialogIPSCtrl *m_dlgIPSCtrl;
	CDialogAlign *m_dlgAlign;
	CDialogGlass *m_dlgGlass;
	CDialogRecipe * m_dlgRecipe;
	CDialogRecipePanel *m_dlgRecipePanel;
	CDialogRecipeCell *m_dlgRecipeCell;
	CDialogRecipeCellPCL *m_dlgRecipeCellPCL;
	CDialogRecipeScan *m_dlgRecipeScan;
	CDialogGlassManuCtrl *m_dlgGlassManuCtrl;
	CDialogFilmAlign *m_dlgFilmAlign;
	CDialogTriggerCtrl *m_dlgTriggerCtrl;
	CDialogLogAlarm *m_dlgLogAlarm;
	CDialogCCD *m_dlgCCD;
	CDialogInline *m_dlgInline;
	CDialogLogWindow *m_dlgLogwindow;
#ifdef DEFECT
	CDialogDFServer *m_dlgDfserver;
#endif
	CDefectList *m_dlgDefectList;
	

	void ServerConnection(CString strIP, int nPortNum);
	UINT static ServerConnectionThread(LPVOID pParam);// 서버 자동 접속 쓰레드
	CWinThread *m_pThreadServerConnection;//서버 자동 접속 쓰레드 포인터
	BOOL m_bConnectionEnd;//서버 자동 접속 쓰레드 종료
	BOOL m_bConnection;//서버 접속 상태

	BOOL m_bGrabComp[RECIPE_SCAN_TYPE_MAX_NUM][IPS_MAX_GRAB_CNT];
	BOOL m_vision_grab_ready;
	BOOL m_vision_grab_comp;
	BOOL m_vision_Inspection_comp[RECIPE_SCAN_TYPE_MAX_NUM];

	LRESULT OnMotMessage(WPARAM para0, LPARAM para1);

	int StopModeOperation(int *prc_idx, int *seq_idx);

	CString m_strResultFilePath;
	CString m_strFlimResultFilePath;

	//20160406 ngh
	char m_cDefectError[DAT_STR_LENG];
	BOOL m_bDefectError;

	//20160408 ngh
	double m_dStartSecond;
	double m_dEndSecond;
	double m_dInspectionStartSecond[RECIPE_SCAN_TYPE_MAX_NUM];
	double m_dInspectionEndSecond[RECIPE_SCAN_TYPE_MAX_NUM];

	//20160711 ngh
	double m_dFilmAlignStartSecond;
	double m_dFilmAlignEndSecond;

	CString m_strInsType;

	//20160530 ngh
	void SetAccessButton(int accessType);
	int m_nAccessType;

	//20160711 ngh
	BOOL m_bScanAfterWait;

	//20160715 ngh
	void ManuSelect(int nManuType);
	

	//20160721 ngh
	BOOL m_bManualStart;
	CString m_strOperater;
protected:
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceive(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnConnect(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
protected:
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlMainExit();
	void ClickBtnenhctrlMainAutorun();
	void ClickBtnenhctrlMainRecipe();
	void ClickBtnenhctrlMainSystem();
	void ClickBtnenhctrlMainLogin();

	CBtnenhctrl m_btn_start;
	void ClickBtnenhctrlMainAlign();
	void ClickBtnenhctrlMainMain();
	void ClickBtnenhctrlMainStop();
	void ClickBtnenhctrlMainReset();
	void ClickBtnenhctrlMainVision();
	void ClickBtnenhctrlMainInline();
	void ClickBtnenhctrlMainLog();
protected:
	BOOL m_check_use_move;
	afx_msg void OnBnClickedCheckMainMove();
	BOOL m_check_defect_move;
	afx_msg void OnBnClickedCheckMainUseDefectMove();
	CListCtrl m_list_ins_rslt;
	void ClickBtnenhctrlMainLoadDefectMap();
	CBtnenhctrl m_btn_stop;
	CBtnenhctrl m_btn_reset;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void ClickBtnenhctrlMainCameraScanImage();
	CBtnenhctrl m_btn_check_ccd;
	void ClickBtnenhctrlMainSaveFilmalign();
	CBtnenhctrl m_btn_login;
	CBtnenhctrl m_btn_autorun;
	CBtnenhctrl m_btn_recipe;
	CBtnenhctrl m_btn_system;
	CBtnenhctrl m_btn_align;
	CBtnenhctrl m_btn_vision;
	CBtnenhctrl m_btn_inline;
	CBtnenhctrl m_btn_log;
	CBtnenhctrl m_btn_exit;
	CBtnenhctrl m_btn_scan_0;
	CBtnenhctrl m_btn_scan_1;
	CBtnenhctrl m_btn_scan_2;
	CBtnenhctrl m_btn_scan_3;
	CBtnenhctrl m_btn_scan_4;
	void ClickBtnenhctrlMain8k();
	void ClickBtnenhctrlMain8k90();
	void ClickBtnenhctrlMain16k();
	void ClickBtnenhctrlMain16k2();
	void ClickBtnenhctrlMain16k3();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ClickBtnenhctrlMainManualStart();
	CBtnenhctrl m_btn_main_sts;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CBtnenhctrl m_btn_MainMsg;
	void ClickBtnenhctrlMainDfserver();
	CBtnenhctrl m_btn_dfserver;
	void ClickBtnenhctrlMainDefectList();
	CBtnenhctrl m_btn_DefectList;

	BOOL m_bIPSAutoRunState;
	CBtnenhctrl m_btn_ScanAfterWait;
	void ClickBtnenhctrlMainSacnWait();
	void ClickBtnenhctrlMainLotRestart();
	CBtnenhctrl m_btn_lot_restart;
	void ClickBtnenhctrlMainMechaAlignRepet();
	CBtnenhctrl m_btn_mecha_align_re;
	CBtnenhctrl m_btn_mecha_align;
	void ClickBtnenhctrlMainMechaAlignRepet2();
};
