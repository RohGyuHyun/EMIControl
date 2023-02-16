
// EMIControlDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "EMIControl.h"

#include "Include/_CtrlImageRect.h"
#include "include/_FuncModuleHal.h"
#include "include/_FuncModuleBasic.h"

#include "include/Runtime.h"
#include "include/PMACComm.h"

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
#include "Dialog/DialogLogAlarm.h"
#include "DialogLogWindow.h"
#include "Dialog/DialogDFServer.h"
#include "Dialog/DefectList.h"

#include "EMIControlDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD _TimerCounter;

//Time
UINT _TimerID;
TIMECAPS _Caps;
BOOL _TimerPost;
void CALLBACK TimerReadFunc(UINT wID,UINT wUser,DWORD dwUser,DWORD dw1,DWORD dw2);

static void CALLBACK TimerReadFunc(UINT wID,UINT wUser,DWORD dwUser,DWORD dw1,DWORD dw2)
{
	if(_TimerPost==FALSE)
	{
		_TimerCounter++;
	}
	else
	{
		AfxMessageBox(_T("Error : high resolution timer event loss!"));
	}	
}


//////////////////////////////////////////////////////////////////////
//Motion Data
//////////////////////////////////////////////////////////////////////
CMotionData::CMotionData()
{
	memset(&m_ProSts, NULL, sizeof(typeMotionProcessSts) * MOT_STATUS_MAX_NUM * MOT_STATUS_SUB_MAX_NUM);
	memset(&m_Seq, NULL, sizeof(typeMotionSeq) * MOT_STATUS_MAX_NUM);
}

CMotionData::~CMotionData()
{

}

void CMotionData::ResetSeq()
{
	memset(&m_Seq, NULL, sizeof(typeMotionSeq) * MOT_STATUS_MAX_NUM);

}

// CEMIControlDlg 대화 상자
//Make directories
void _MakeDirectories(CString path)
{
	wchar_t tmp_path[MAX_PATH];
	wchar_t sub[MAX_PATH];
	long size;
	size = sizeof(path);

	long index = 0;

	wsprintf(tmp_path, _T("%s"), (LPCTSTR)path);
	memset(sub, NULL, sizeof(sub));

	while(1)
	{
		if(tmp_path[index] == '\\')
		{
			CreateDirectory(sub, NULL);
		}

		sub[index] = tmp_path[index];

		if(tmp_path[index] == '\0')
		{
			CreateDirectory(sub, NULL);
			break;
		}

		//base_index++;
		index++;

		if( (index > MAX_PATH) )
		{
			AfxMessageBox(_T("Error : index overflow(_MakeDirectories)!"));
			break;
		}

	}
}

CEMIControlDlg::CEMIControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEMIControlDlg::IDD, pParent)
	, m_check_use_move(FALSE)
	, m_check_defect_move(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pThread = NULL;
	m_pThreadECS = NULL;

	memset(&m_Flag, NULL, sizeof(typeMotionFlag));
	m_Flag.m_bProcessThread = TRUE;

	memset(&m_Rslt, NULL, sizeof(typeProcessResult));

	for(int i = 0; i < RECIPE_SCAN_TYPE_MAX_NUM; i++)
	{
		m_vision_Inspection_comp[i] = FALSE;
	}

	m_iScanTypeIdx = 0;

	m_pThreadDefectFile = NULL;
	m_bDefectFileLoadStart = FALSE;
	m_bDefectFileLoadEnd = FALSE;
	m_bDefectFileWriteComp[0] = FALSE;
	m_bDefectFileWriteComp[1] = FALSE;
	m_bDefectFileWriteComp[2] = FALSE;
	m_bDefectFileWriteComp[3] = FALSE;
	m_bDefectFileWriteComp[4] = FALSE;
	m_bDefectFileWriteBreak = FALSE;
	m_bDefectFileSave[0] = FALSE;
	m_bDefectFileSave[1] = FALSE;
	m_bDefectFileSave[2] = FALSE;
	m_bDefectFileSave[3] = FALSE;
	m_bDefectFileSave[4] = FALSE;

	m_pThreadDefectServer = NULL;
	m_bDefectServerStart= FALSE;
	m_bDefectServerEnd = FALSE;

	InitializeCriticalSection(&m_CSInline);

	m_bTestValue = FALSE;
	m_bTower = FALSE;
	m_bDefectWriteComp = FALSE;
	m_bDefectError = FALSE;
	m_bDefectServerFileMake = FALSE;
	m_bDefectServerFileReady = FALSE;

	bManualDefectServer = FALSE;
	m_bConnection = FALSE;
	m_bManualStart = FALSE;

	m_nAccessType = 0;
	m_bMachaAlignRepet = FALSE;
}

void CEMIControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_MAIN, m_btn_start);
	DDX_Check(pDX, IDC_CHECK_MAIN_USE_MOVE, m_check_use_move);
	DDX_Check(pDX, IDC_CHECK_MAIN_USE_DEFECT_MOVE, m_check_defect_move);
	DDX_Control(pDX, IDC_LIST_INS_RSLT, m_list_ins_rslt);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_STOP, m_btn_stop);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_RESET, m_btn_reset);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_CAMERA_SCAN_IMAGE, m_btn_check_ccd);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_LOGIN, m_btn_login);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_AUTORUN, m_btn_autorun);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_RECIPE, m_btn_recipe);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_SYSTEM, m_btn_system);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_ALIGN, m_btn_align);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_VISION, m_btn_vision);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_INLINE, m_btn_inline);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_LOG, m_btn_log);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_EXIT, m_btn_exit);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_8K, m_btn_scan_0);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_8K90, m_btn_scan_1);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_16K, m_btn_scan_2);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_16K2, m_btn_scan_3);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_16K3, m_btn_scan_4);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_STATUS, m_btn_main_sts);
	DDX_Control(pDX, IDC_BTNENHCTRL_MSG, m_btn_MainMsg);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_DFSERVER, m_btn_dfserver);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_DEFECT_LIST, m_btn_DefectList);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_SACN_WAIT, m_btn_ScanAfterWait);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_LOT_RESTART, m_btn_lot_restart);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_MECHA_ALIGN_REPET, m_btn_mecha_align_re);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_MECHA_ALIGN_REPET2, m_btn_mecha_align);
}

BEGIN_MESSAGE_MAP(CEMIControlDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_ONCONNECT,OnConnect)
	ON_MESSAGE(WM_CLOSE_SOCK, OnClose)
	ON_MESSAGE(WM_RECEIVE_DATA,OnReceive)
	ON_BN_CLICKED(IDC_CHECK_MAIN_USE_MOVE, &CEMIControlDlg::OnBnClickedCheckMainMove)
	ON_BN_CLICKED(IDC_CHECK_MAIN_USE_DEFECT_MOVE, &CEMIControlDlg::OnBnClickedCheckMainUseDefectMove)
	ON_MESSAGE(USER_MSG_MOT, OnMotMessage)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CEMIControlDlg 메시지 처리기

BOOL CEMIControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	this->SetWindowTextW(_T("EMIControl"));
#ifdef MACHINE
	if(IDCANCEL == AfxMessageBox(_T("Program Start?"), MB_OKCANCEL | MB_ICONQUESTION, 0))
	{
		exit(0);
	}
#endif

	m_ProcessBar = new CProcessBar;
	m_ProcessBar->Create(IDD_DIALOG_PROCESS_BAR, this);

	m_ProcessBar->SetWindowPos((&CWnd::wndTopMost), 50, 50, 100, 100, 
                                          SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW); 
	m_ProcessBar->ShowWindow(SW_HIDE);

	//20160530 ngh
	m_AccessRight = new CAccessRight;
	m_AccessRight->Create(IDD_DIALOG_ACCESS_RIGHT, this);

	m_dlgAlign = new CDialogAlign;
	m_dlgAlign->Create(IDD_DIALOG_ALIGN, this);
	m_Align = m_dlgAlign;

	
	m_dlgRecipe = new CDialogRecipe;
	m_dlgRecipe->Create(IDD_DIALOG_RECIPE, this);
	m_Recipe = m_dlgRecipe;

	m_dlgRecipePanel = new CDialogRecipePanel;
	m_dlgRecipePanel->Create(IDD_DIALOG_RECIPE_PANEL, this);
	m_RecipePanel = m_dlgRecipePanel;

	m_dlgRecipeCell = new CDialogRecipeCell;
	m_dlgRecipeCell->Create(IDD_DIALOG_RECIPE_CELL, this);
	m_RecipeCell = m_dlgRecipeCell;

	m_dlgRecipeCellPCL = new CDialogRecipeCellPCL;
	m_dlgRecipeCellPCL->Create(IDD_DIALOG_RECIPE_CELL_PCL, this);
	m_RecipeCellPCL = m_dlgRecipeCellPCL;

	m_dlgGlass = new CDialogGlass;
	m_dlgGlass->Create(IDD_DIALOG_GLASS, this);
	m_Glass = m_dlgGlass;

	m_dlgRecipeScan = new CDialogRecipeScan;
	m_dlgRecipeScan->Create(IDD_DIALOG_RECIPE_SCAN, this);
	m_RecipeScan = m_dlgRecipeScan;

	m_Glass->SetUpdate();
	m_RecipeScan->SetUpdate();

	m_dlgSystem = new CDialogSystem;
	m_dlgSystem->Create(IDD_DIALOG_MANUAL, this);

	m_dlgMotorCtrl = new CDialogMotorCtrl;
	m_dlgMotorCtrl->Create(IDD_DIALOG_MOTOR_CTRL, this);
	m_Robot = m_dlgMotorCtrl;

	m_dlgIOCtrl = new CDialogIOCtrl;
	m_dlgIOCtrl->Create(IDD_DIALOG_IO_CTRL, this);
	m_IO = m_dlgIOCtrl;

	m_dlgAFCtrl = new CDialogAFCtrl;
	m_dlgAFCtrl->Create(IDD_DIALOG_AF_CTRL, this);
	m_AFCtrl = m_dlgAFCtrl;

#ifdef MACHINE //20160720 ngh
	if(IDOK == AfxMessageBox(_T("EzMotor Origin Start"), MB_OKCANCEL | MB_ICONEXCLAMATION, 0))
	{
 		m_AFCtrl->SetEzHome();
	}

	if(IDOK == AfxMessageBox(_T("Moter Origin Start?"), MB_OKCANCEL | MB_ICONQUESTION, 0))
	{
		m_Robot->SetMoterAllOrigin();
	}
#endif

	m_dlgReview = new CDialogReview;
	m_dlgReview->Create(IDD_DIALOG_REVIEW, this);
	m_Review = m_dlgReview;

	m_dlgLogin = new CDialogLogin;
	m_dlgLogin->Create(IDD_DIALOG_LOGIN, this);

	m_dlgLightCtrl = new CDialogLightCtrl;
	m_dlgLightCtrl->Create(IDD_DIALOG_LIGHT_CTRL, this);
	m_Light = m_dlgLightCtrl;

	m_dlgIPSCtrl = new CDialogIPSCtrl;
	m_dlgIPSCtrl->Create(IDD_DIALOG_IPS_CTRL, this);
	m_IPSCtrl = m_dlgIPSCtrl;

	m_dlgCCD = new CDialogCCD;
	m_dlgCCD->Create(IDD_DIALOG_CCD, this);
	m_CCD = m_dlgCCD;

	m_dlgGlassManuCtrl = new CDialogGlassManuCtrl;
	m_dlgGlassManuCtrl->Create(IDD_DIALOG_GLASSMANUAL_CTRL, this);
	m_GlassManu = m_dlgGlassManuCtrl;

	m_dlgFilmAlign = new CDialogFilmAlign;
	m_dlgFilmAlign->Create(IDD_DIALOG_FILM_ALIGN_INSPECT, this);
	m_FilmAlign = m_dlgFilmAlign;

	m_dlgTriggerCtrl = new CDialogTriggerCtrl;
	m_dlgTriggerCtrl->Create(IDD_DIALOG_TRIGGER_CTRL, this);
	m_Trigger = m_dlgTriggerCtrl;

	m_dlgLogAlarm = new CDialogLogAlarm;
	m_dlgLogAlarm->Create(IDD_DIALOG_LOG_ALARM, this);
	m_LogAlarm = m_dlgLogAlarm;

	m_dlgInline = new CDialogInline;
	m_dlgInline->Create(IDD_DIALOG_INLINE_CTRL, this);
	m_Inline = m_dlgInline;

	m_dlgMainInfo = new CDialogMainInfo;
	m_dlgMainInfo->Create(IDD_DIALOG_MAIN_INFO, this);

	m_dlgLogwindow = new CDialogLogWindow;
	m_dlgLogwindow->Create(IDD_DIALOG_LOG_WINDOW,this);
	m_Logwindow = m_dlgLogwindow;
	
#ifdef DEFECT
	m_dlgDfserver = new CDialogDFServer;
	m_dlgDfserver->Create(IDD_DIALOG_DEFECT_SERVER,this);
	m_Dfserver = m_dlgDfserver;
#endif

	m_dlgDefectList = new CDefectList;
	m_dlgDefectList->Create(IDD_DIALOG_DEFECT_LIST,this);
	m_DefectList = m_dlgDefectList;



	CRect rect;
	
	m_dlgMainInfo->ShowWindow(SW_SHOW);
	m_dlgMainInfo->GetWindowRect(rect);
	m_dlgMainInfo->MoveWindow(925, 130, rect.Width(), rect.Height(), TRUE);

	m_dlgReview->GetWindowRect(rect);
	m_dlgReview->MoveWindow(925, 320, rect.Width(), rect.Height(), TRUE);
	m_dlgReview->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_STATIC_MAIN_RECT0)->GetClientRect(rect);
	GetDlgItem(IDC_STATIC_MAIN_RECT0)->ClientToScreen(rect);
	m_dlgGlass->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
	m_dlgGlass->ShowWindow(SW_SHOW);

	//Server AutoConnection Thread
	m_bConnection = FALSE;
	m_bConnectionEnd = FALSE;



	m_Client = new CClientSock;
	AfxSocketInit();
	delete m_Client;

	m_Client = new CClientSock;
	m_Client->Create();
	m_Client->SetWnd(this->m_hWnd);	
	
	ServerConnection(m_dlgIPSCtrl->GetIPSIP(), m_dlgIPSCtrl->GetIPSPort());

	m_pThreadServerConnection = AfxBeginThread(ServerConnectionThread, this, THREAD_PRIORITY_NORMAL);
	m_pThreadServerConnection->m_bAutoDelete = TRUE;
	
	//20160307 ngh
	m_pThreadDefectFile = AfxBeginThread(DefectFileThread, this, THREAD_PRIORITY_NORMAL);
	m_pThreadDefectFile->m_bAutoDelete = TRUE;

	//20160307 ngh
	m_pThreadDefectServer = AfxBeginThread(DefectServerThread, this, THREAD_PRIORITY_NORMAL);
	m_pThreadDefectServer->m_bAutoDelete = TRUE;
	m_bDefectServerStart = TRUE;

	//high resolution timer for IO reading & PLC init
	//(timer)
	_TimerPost=FALSE;
	timeGetDevCaps(&_Caps,sizeof(_Caps));
	_Caps.wPeriodMin = 1;
	timeBeginPeriod(_Caps.wPeriodMin);
	_TimerID = timeSetEvent(_Caps.wPeriodMin, _Caps.wPeriodMin, (LPTIMECALLBACK)&TimerReadFunc, 0, (UINT)TIME_PERIODIC);
	if(!_TimerID)
	{
		AfxMessageBox(_T("Error : high resolution timer set fail!"));
	}

	//Thread
	m_pThread = AfxBeginThread(MotionThread, this, THREAD_PRIORITY_NORMAL);
	if (m_pThread) m_pThread->m_bAutoDelete = TRUE;

	m_pThreadECS = AfxBeginThread(MotionThreadECS, this, THREAD_PRIORITY_NORMAL);
	if (m_pThreadECS) m_pThreadECS->m_bAutoDelete = TRUE;

	//List////////////////////////////////////////////////////////////////////
	LV_COLUMN m_lvColumn;
	CFont m_font;

	m_list_ins_rslt.SetExtendedStyle(LVS_EX_GRIDLINES);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=_T("  ");
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=130;
	m_list_ins_rslt.InsertColumn(0, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=_T("S(Yellow Fill)");
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=90;
	m_list_ins_rslt.InsertColumn(1, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=_T("M(Blue Fill)");
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=90;
	m_list_ins_rslt.InsertColumn(2, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=_T("L(Red Fill)");
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=90;
	m_list_ins_rslt.InsertColumn(3, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=_T("O(Green Fill)");
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=90;
	m_list_ins_rslt.InsertColumn(4, &m_lvColumn);

	/*m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=_T("주름(Pink Rect)");
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=100;
	m_list_ins_rslt.InsertColumn(5, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=_T("이물");
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=90;
	m_list_ins_rslt.InsertColumn(6, &m_lvColumn);*/

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=_T("SUM");
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=85;
	m_list_ins_rslt.InsertColumn(5, &m_lvColumn);

	//m_font.CreatePointFont(90, _T("돋음"));
	//m_list_ins_rslt.SetFont(&m_font, TRUE);

	m_list_ins_rslt.InsertItem(1, _T("불량(이물성)"));  
	m_list_ins_rslt.InsertItem(2, _T("불량(높이성)"));  
	/*
	m_list_ins_rslt.InsertItem(2, _T("이물"));  
	m_list_ins_rslt.InsertItem(3, _T("눌림"));   
	m_list_ins_rslt.InsertItem(4, _T("찍힘"));  
	*/
	m_list_ins_rslt.InsertItem(3, _T("TOTAL"));

	ResetRsltSizeCntView();

	//Button
	m_btn_start.SetEnabled(TRUE);
	m_btn_start.SetValue(TRUE);
	m_btn_stop.SetEnabled(FALSE);
	m_btn_stop.SetValue(FALSE);
	m_btn_reset.SetEnabled(FALSE);
	m_btn_reset.SetValue(FALSE);

	//Initialize dailog
	m_dlgMainInfo->SetUpdate();

	//Load recipe and ECS////////////////////////////////////////////////////////////////////////
	CDialogRecipeModel dlg;
	dlg.SetOpen();

	if(!dlg.GetOpenList(dlg.GetRecipeNo()))
	{
		m_Recipe->SetRecipeName(dlg.GetRecipeName());
		m_Recipe->SetRecipeNo(dlg.GetRecipeNo());

		//Load recipe
		SetLoadRecipeFile(dlg.GetRecipePath());
	}
	else
	{
		AfxMessageBox(_T("Error : Recipe Name이 존재 하지 않습니다. 생성된 Recipe가 아닙니다!"));

	}
	
	//ECS set recipe list/////////////////////////////
	int i;
	m_Inline->WriteLog(_T("SetRecipeNum List Write Start"));
	for(i = 0; i < RECIPE_MAX_NUM; i++)
	{
		//FALSE:있으면 false 없으면 true
		if(!dlg.GetOpenList(i))
		{
			m_Inline->SetRecipeNum(i + 1, FALSE);
		}
		else
		{
			m_Inline->SetRecipeNum(i + 1, TRUE);
		}
	}
	m_Inline->WriteLog(_T("SetRecipeNum List Write End"));
	m_Inline->SetRecipeListChange(TRUE);
	///////////////////////////////////////////////////
	
	//Load recipe data and send ECS////////////////////
	SetRecipeToECS();

	////////////////////////////////////////////////////

	//Glass exist check///////////////////////////////
	typeEQPStatusDataPara eq_sts;

	//Detect sensor power on
	m_IO->SetWriteDO(DO_GLASS_DETECT_SENSOR_RELAY_ON_OFF, TRUE);
	Sleep(500);

	if( ((m_IO->GetReadDI(DI_PIN_GLS_DETECT_FRONT))) || ((m_IO->GetReadDI(DI_PIN_GLS_DETECT_FRONT))) )
	{
		m_Flag.m_bGlassExist = TRUE;
	}
	else
	{
		m_Flag.m_bGlassExist = FALSE;
	}
	///////////////////////////////////////////////////

	//Init eqp status//////////////////////////////////
	eq_sts = m_Inline->GetStatusReport();
	eq_sts.bGlassExistInUnit = m_Flag.m_bGlassExist;					//Glass유무
	eq_sts.bGlassInProcess = FALSE;							//검사중인지
	eq_sts.bIonizer = TRUE;
	eq_sts.bUnitAutoMode = FALSE;							//자동반출 가능한지			
	eq_sts.nCurrenRecipeNum = m_Recipe->GetRecipeNo() + 1;
	if(m_Flag.m_bGlassExist)
	{
		eq_sts.nGlassCountInUnit = 1;
		eq_sts.nPutInfoPossibleCount = 0;
		//eq_sts.nGlassCountInUnit = 0;
		//eq_sts.nPutInfoPossibleCount = 1;
	}
	else
	{
		eq_sts.nGlassCountInUnit = 0;
		eq_sts.nPutInfoPossibleCount = 1;
		//eq_sts.nGlassCountInUnit = 1;
		//eq_sts.nPutInfoPossibleCount = 0;
	}

	eq_sts.nStatusRunIdleDown = !eq_sts.bGlassExistInUnit;	//0 : RUN~
	eq_sts.nUnitOpeationMode = 1;							//1 : Manual, 0 : auto

	m_Inline->SetStatus(eq_sts);

	//20160711 ngh
	//if(m_GlassManu->GetLoaderUse())
	{
		m_Inline->InitGlassPosition();
	}
	///////////////////////////////////////////////////////

	for(i = 0; i < RECIPE_SCAN_IMG_RECT_MAX_NUM; i++)
	{
		m_Glass->InitDefectData(i,TRUE);
		m_Glass->InitDefectData(i,FALSE);
	}

	for(int i = 0;i < 100;i++)
	{
		for(int j = 0;j < 6;j++)
		{
			m_film_image[i][j].Format(_T("%d_%d"),i,j);
		}
	}

	//20160526 ngh
	m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_RED, FALSE);
	m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_GRE, FALSE);
	m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_YEL, TRUE);

	m_bIPSAutoRunState = FALSE;

	//20160715 ngh
	ManuSelect(1);
	m_bScanAfterWait = FALSE;

	//20160722 ngh 임시
	SetAccess(3);

	m_ProcessBar->ShowWindow(SW_SHOW);
	m_ProcessBar->GetClientRect(&rect);
	m_ProcessBar->MoveWindow(1510, 50, rect.Width(), rect.Height());
	m_ProcessBar->ShowWindow(SW_HIDE);

	//20160510 sc
	//ClickBtnenhctrlMainLogin();


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CEMIControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CEMIControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(CEMIControlDlg, CDialogEx)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_EXIT, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainExit, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_AUTORUN, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainAutorun, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_RECIPE, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainRecipe, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_SYSTEM, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainSystem, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_LOGIN, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainLogin, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_ALIGN, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainAlign, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_MAIN, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainMain, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_STOP, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainStop, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_RESET, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainReset, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_VISION, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainVision, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_INLINE, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainInline, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_LOG, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainLog, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_LOAD_DEFECT_MAP, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainLoadDefectMap, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_CAMERA_SCAN_IMAGE, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainCameraScanImage, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_SAVE_FILMALIGN, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainSaveFilmalign, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_8K, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMain8k, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_8K90, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMain8k90, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_16K, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMain16k, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_16K2, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMain16k2, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_16K3, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMain16k3, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_MANUAL_START, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainManualStart, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_DFSERVER, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainDfserver, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_DEFECT_LIST, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainDefectList, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_SACN_WAIT, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainSacnWait, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_LOT_RESTART, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainLotRestart, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_MECHA_ALIGN_REPET, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainMechaAlignRepet, VTS_NONE)
	ON_EVENT(CEMIControlDlg, IDC_BTNENHCTRL_MAIN_MECHA_ALIGN_REPET2, DISPID_CLICK, CEMIControlDlg::ClickBtnenhctrlMainMechaAlignRepet2, VTS_NONE)
END_EVENTSINK_MAP()

LRESULT CEMIControlDlg::OnMotMessage(WPARAM para0, LPARAM para1)
{
	// TODO: Add your control notification handler code here
	int i, j, idx;
	CString temp;
	char message[500];

	
	switch(para0)
	{
	//Alarm
	case 0:
		for(i = 0; i < MOT_PROCESS_MAX_NUM; i++)
		{
			for(j = 0; j < MOT_STATUS_MAX_NUM; j++)
			{
				if(m_MDat[i].m_Seq[j].m_bAlarm)
				{
					m_dlgLogAlarm->SetMessage(m_MDat[i].m_ProSts[j][m_MDat[i].m_Seq[j].m_iSubSeqIdx].m_cNGStatus);
					m_dlgLogAlarm->ShowWindow(SW_SHOW);

					//Button
					m_btn_start.SetEnabled(FALSE);
					m_btn_start.SetValue(FALSE);
					m_btn_stop.SetEnabled(FALSE);
					m_btn_stop.SetValue(FALSE);
					m_btn_reset.SetEnabled(TRUE);
					m_btn_reset.SetValue(TRUE);
					
					break;
				}
			}
		}
		break;
	//DefectError
	case 1:
		m_dlgLogAlarm->SetMessage(m_cDefectError);
		m_dlgLogAlarm->ShowWindow(SW_SHOW);

		//Button
		m_btn_start.SetEnabled(FALSE);
		m_btn_start.SetValue(FALSE);
		m_btn_stop.SetEnabled(FALSE);
		m_btn_stop.SetValue(FALSE);
		m_btn_reset.SetEnabled(TRUE);
		m_btn_reset.SetValue(TRUE);
		break;
	default:
		AfxMessageBox(_T("Error : invalid index!(CEMIControlDlg::OnMotMessage)"));
		break;
	}

	return 0;
}

//20160531 ngh
void CEMIControlDlg::ProgramExit()
{
	ClickBtnenhctrlMainExit();
}

void CEMIControlDlg::ClickBtnenhctrlMainExit()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CDialogMSG dlg;
	dlg.SetCaption(_T("EMI 프로그램을 종료하시겠습니까?"));
	
	if(0 == dlg.DoModal())
	{
		return;
	}

	//light off
#ifdef MACHINE
	m_dlgLightCtrl->Set16KLight(0);
	m_dlgLightCtrl->Set8KLight(0);
	m_dlgLightCtrl->SetAlignLight(0, 0);
	m_dlgLightCtrl->SetAlignLight(1, 0);
	m_AFCtrl->SetReviewLed(0);
	KillTimer(100);
	m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_YEL, FALSE);
#endif

	//Save recipe///////////////////////////////////////////////////////////////
	CDialogRecipeModel mod_dlg;
	mod_dlg.SetOpen();
	SetSaveRecipeFile(mod_dlg.GetRecipePath());

	m_Flag.m_bProcessThread = FALSE;
	
	m_bDefectFileLoadStart = FALSE;
	
	m_bDefectFileWriteComp[0] = TRUE;
	m_bDefectFileWriteComp[1] = TRUE;
	m_bDefectFileWriteComp[2] = TRUE;
	m_bDefectFileWriteComp[3] = TRUE;
	m_bDefectFileWriteComp[4] = TRUE;

	m_bDefectFileLoadEnd = TRUE;

	m_bDefectServerEnd = TRUE;

	m_bConnectionEnd = TRUE;
	m_Client->Close();
	
	Sleep(500);

	delete m_dlgMainInfo;
	delete m_dlgSystem;
	delete m_dlgMotorCtrl;
	delete m_dlgIOCtrl;
	delete m_dlgReview;
	delete m_dlgLogin;
	delete m_dlgAFCtrl;
	delete m_dlgLightCtrl;
	delete m_dlgIPSCtrl;
	delete m_dlgAlign;
	delete m_dlgGlass;
	delete m_dlgRecipe;
	delete m_dlgRecipePanel;
	delete m_dlgRecipeCell;
	delete m_dlgRecipeScan; 
	delete m_dlgGlassManuCtrl;
	delete m_dlgFilmAlign;
	delete m_dlgTriggerCtrl;
	delete m_dlgLogAlarm;
	delete m_dlgCCD;
	delete m_dlgInline;
	delete m_ProcessBar;
	delete m_dlgLogwindow;
#ifdef DEFECT
	delete m_dlgDfserver;
#endif
	delete m_Client;
	delete m_AccessRight;
	delete m_dlgDefectList;

	//20160531
	//delete m_Loading;

	DeleteCriticalSection(&m_CSInline);

	EndDialog(0);
}

void CEMIControlDlg::WindowVisable_main()
{
	m_dlgMainInfo->ShowWindow(SW_SHOW);
	m_dlgReview->ShowWindow(SW_SHOW);
	m_dlgGlass->ShowWindow(SW_SHOW);
	m_dlgSystem->ShowWindow(SW_HIDE);
	m_dlgMotorCtrl->ShowWindow(SW_HIDE);
	m_dlgIOCtrl->ShowWindow(SW_HIDE);
	m_dlgLogin->ShowWindow(SW_HIDE);
	m_dlgAFCtrl->ShowWindow(SW_HIDE);
	m_dlgLightCtrl->ShowWindow(SW_HIDE);
	m_dlgIPSCtrl->ShowWindow(SW_HIDE);
	m_dlgAlign->ShowWindow(SW_HIDE);
	m_dlgRecipe->ShowWindow(SW_HIDE);
	m_dlgRecipePanel->ShowWindow(SW_HIDE);
	m_dlgRecipeCell->ShowWindow(SW_HIDE);
	m_dlgRecipeCellPCL->ShowWindow(SW_HIDE);
	m_dlgRecipeScan->ShowWindow(SW_HIDE);
	m_dlgInline->ShowWindow(SW_HIDE);
	m_dlgGlassManuCtrl->ShowWindow(SW_HIDE);
	m_dlgFilmAlign->ShowWindow(SW_HIDE);
	m_dlgTriggerCtrl->ShowWindow(SW_HIDE);
	m_dlgLogwindow->ShowWindow(SW_HIDE);
#ifdef DEFECT
	m_dlgDfserver->ShowWindow(SW_HIDE);
#endif

	m_btn_login.SetValue(FALSE);
	m_btn_autorun.SetValue(FALSE);
	m_btn_recipe.SetValue(FALSE);
	m_btn_system.SetValue(FALSE);
	m_btn_align.SetValue(FALSE);
	m_btn_vision.SetValue(FALSE);
	m_btn_inline.SetValue(FALSE);
	m_btn_log.SetValue(FALSE);
	m_btn_exit.SetValue(FALSE);
	m_btn_dfserver.SetValue(FALSE);
	m_btn_MainMsg.ShowWindow(SW_HIDE);

}

void CEMIControlDlg::WindowVisable_system()
{
	m_dlgMotorCtrl->ShowWindow(SW_HIDE);
	m_dlgIOCtrl->ShowWindow(SW_HIDE);
	m_dlgAFCtrl->ShowWindow(SW_HIDE);
	m_dlgLightCtrl->ShowWindow(SW_HIDE);
	m_dlgIPSCtrl->ShowWindow(SW_HIDE);
	m_dlgGlassManuCtrl->ShowWindow(SW_HIDE);
	m_dlgTriggerCtrl->ShowWindow(SW_HIDE);
	m_dlgLogwindow->ShowWindow(SW_HIDE);
}

void CEMIControlDlg::WindowVisable_recipe()
{
	m_dlgAlign->ShowWindow(SW_HIDE);
	m_dlgRecipePanel->ShowWindow(SW_HIDE);
	m_dlgRecipeCell->ShowWindow(SW_HIDE);
	m_dlgRecipeCellPCL->ShowWindow(SW_HIDE);
	m_dlgRecipeScan->ShowWindow(SW_HIDE);
	m_dlgFilmAlign->ShowWindow(SW_HIDE);
	m_btn_MainMsg.ShowWindow(SW_HIDE);
}

void CEMIControlDlg::ClickBtnenhctrlMainAutorun()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	WindowVisable_main();
	ManuSelect(1);
	m_btn_autorun.SetValue(TRUE);
	m_btn_MainMsg.ShowWindow(SW_SHOW);

	int i;
	for(i = 0; i < RECIPE_SCAN_TYPE_MAX_NUM; i++)
	{
		switch(i)
		{
		case 0:
			m_btn_scan_0.SetEnabled(m_RecipeScan->GetInspectUse(i));
			break;
		case 1:
			m_btn_scan_1.SetEnabled(m_RecipeScan->GetInspectUse(i));
			break;
		case 2:
			m_btn_scan_2.SetEnabled(m_RecipeScan->GetInspectUse(i));
			break;
		case 3:
			m_btn_scan_3.SetEnabled(m_RecipeScan->GetInspectUse(i));
			break;
		case 4:
			m_btn_scan_4.SetEnabled(m_RecipeScan->GetInspectUse(i));
			break;
		default:
			AfxMessageBox(_T("Error : invalid index(CEMIControlDlg::ClickBtnenhctrlMainAutorun)!"));
			break;
		}
	}
}

void CEMIControlDlg::ClickBtnenhctrlMainRecipe()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect rect;
	ManuSelect(2);
	WindowVisable_main();

	m_btn_MainMsg.ShowWindow(SW_SHOW);

	m_dlgRecipe->ShowWindow(SW_SHOW);
	m_btn_recipe.SetValue(TRUE);

	m_dlgRecipe->GetWindowRect(rect);
	m_dlgRecipe->MoveWindow(925, 130, rect.Width(), rect.Height(), TRUE);
}

void CEMIControlDlg::ClickBtnenhctrlMainSystem()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect rect;
	ManuSelect(3);
	WindowVisable_main();

	m_dlgSystem->ShowWindow(SW_SHOW);
	m_btn_system.SetValue(TRUE);

	m_dlgSystem->GetWindowRect(rect);
	m_dlgSystem->MoveWindow(5, 100, rect.Width(), rect.Height(), TRUE);

	m_dlgSystem->ManuSelect(0);
	SystemGlassManuCtrl();
}

void CEMIControlDlg::ClickBtnenhctrlMainLogin()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect rect;
	ManuSelect(0);
	WindowVisable_main();


	m_dlgLogin->ShowWindow(SW_SHOW);
	
	m_btn_login.SetValue(TRUE);

	m_dlgLogin->GetWindowRect(rect);
	//m_dlgLogin->MoveWindow(9, 20, rect.Width(), rect.Height(), TRUE);
}


void CEMIControlDlg::MainAlign(void)
{
	CRect rect;
	WindowVisable_main();

	m_dlgAlign->GetWindowRect(rect);
	m_dlgAlign->MoveWindow(5, 100, rect.Width(), rect.Height(), TRUE);

	m_dlgAlign->ShowWindow(SW_SHOW);
}

void CEMIControlDlg::SystemMotorCtrl(void)
{
	CRect rect;

	WindowVisable_system();

	m_dlgMotorCtrl->ShowWindow(SW_SHOW);
	m_dlgMotorCtrl->GetWindowRect(rect);
	m_dlgMotorCtrl->MoveWindow(5, 170, rect.Width(), rect.Height(), TRUE);
}

void CEMIControlDlg::SystemIOCtrl(void)
{
	CRect rect;

	WindowVisable_system();

	m_dlgIOCtrl->ShowWindow(SW_SHOW);
	m_dlgIOCtrl->GetWindowRect(rect);
	m_dlgIOCtrl->MoveWindow(5, 170, rect.Width(), rect.Height(), TRUE);
}

void CEMIControlDlg::SystemGlassManuCtrl(void)
{
	CRect rect;

	WindowVisable_system();

	m_dlgGlassManuCtrl->ShowWindow(SW_SHOW);
	m_dlgGlassManuCtrl->GetWindowRect(rect);
	m_dlgGlassManuCtrl->MoveWindow(5, 170, rect.Width(), rect.Height(), TRUE);
}

void CEMIControlDlg::SystemAFCtrl(void)
{
	CRect rect;

	WindowVisable_system();

	m_dlgAFCtrl->ShowWindow(SW_SHOW);
	m_dlgAFCtrl->GetWindowRect(rect);
	m_dlgAFCtrl->MoveWindow(5, 170, rect.Width(), rect.Height(), TRUE);
}

void CEMIControlDlg::SystemLightCtrl(void)
{
	CRect rect;

	WindowVisable_system();

	m_dlgLightCtrl->ShowWindow(SW_SHOW);
	m_dlgLightCtrl->GetWindowRect(rect);
	m_dlgLightCtrl->MoveWindow(5, 170, rect.Width(), rect.Height(), TRUE);
}

void CEMIControlDlg::SystemIPSCtrl(void)
{
	CRect rect;

	WindowVisable_system();

	m_dlgIPSCtrl->ShowWindow(SW_SHOW);
	m_dlgIPSCtrl->GetWindowRect(rect);
	m_dlgIPSCtrl->MoveWindow(5, 170, rect.Width(), rect.Height(), TRUE);
}

//20151204
void CEMIControlDlg::SystemTriggerCtrl(void)
{
	CRect rect;

	WindowVisable_system();

	m_dlgTriggerCtrl->ShowWindow(SW_SHOW);
	m_dlgTriggerCtrl->GetWindowRect(rect);
	m_dlgTriggerCtrl->MoveWindow(5, 170, rect.Width(), rect.Height(), TRUE);
}

void CEMIControlDlg::RecipePanel(void)
{
	CRect rect, dlg_rect;

	WindowVisable_recipe();
	
	m_dlgRecipePanel->GetWindowRect(dlg_rect);
	GetDlgItem(IDC_STATIC_MAIN_RECT0)->GetClientRect(rect);
	GetDlgItem(IDC_STATIC_MAIN_RECT0)->ClientToScreen(rect);
	m_dlgRecipePanel->MoveWindow(rect.left, rect.top, dlg_rect.Width(), dlg_rect.Height(), TRUE);
	m_dlgRecipePanel->ShowWindow(SW_SHOW);
}

void CEMIControlDlg::RecipeCell(void)
{
	CRect rect, dlg_rect;

	WindowVisable_recipe();
	
	m_dlgRecipeCell->GetWindowRect(dlg_rect);
	GetDlgItem(IDC_STATIC_MAIN_RECT0)->GetClientRect(rect);
	GetDlgItem(IDC_STATIC_MAIN_RECT0)->ClientToScreen(rect);
	m_dlgRecipeCell->MoveWindow(rect.left, rect.top, dlg_rect.Width(), dlg_rect.Height(), TRUE);
	m_dlgRecipeCell->ShowWindow(SW_SHOW);
}

void CEMIControlDlg::RecipeCellPCL(void)
{
	CRect rect, dlg_rect;

	WindowVisable_recipe();
	
	m_dlgRecipeCell->GetWindowRect(dlg_rect);
	GetDlgItem(IDC_STATIC_MAIN_RECT0)->GetClientRect(rect);
	GetDlgItem(IDC_STATIC_MAIN_RECT0)->ClientToScreen(rect);
	m_dlgRecipeCellPCL->MoveWindow(rect.left, rect.top, dlg_rect.Width(), dlg_rect.Height(), TRUE);
	m_dlgRecipeCellPCL->ShowWindow(SW_SHOW);
}

void CEMIControlDlg::RecipeScan(void)
{
	CRect rect, dlg_rect;

	WindowVisable_recipe();
	
	m_dlgRecipeScan->GetWindowRect(dlg_rect);
	GetDlgItem(IDC_STATIC_MAIN_RECT0)->GetClientRect(rect);
	GetDlgItem(IDC_STATIC_MAIN_RECT0)->ClientToScreen(rect);
	m_dlgRecipeScan->SetLoadRecipeScanPara(_T(RECIPE_SCAN_PATH));
	m_dlgRecipeScan->MoveWindow(rect.left, rect.top, dlg_rect.Width(), dlg_rect.Height(), TRUE);
	m_dlgRecipeScan->ShowWindow(SW_SHOW);
}


void CEMIControlDlg::FilmAlign(void)
{
	CRect rect, dlg_rect;

	WindowVisable_recipe();
	
	m_dlgFilmAlign->GetWindowRect(dlg_rect);
	GetDlgItem(IDC_STATIC_MAIN_RECT0)->GetClientRect(rect);
	GetDlgItem(IDC_STATIC_MAIN_RECT0)->ClientToScreen(rect);
	m_dlgFilmAlign->MoveWindow(rect.left, rect.bottom, dlg_rect.Width(), dlg_rect.Height(), TRUE);
	m_dlgFilmAlign->ShowWindow(SW_SHOW);
}

void CEMIControlDlg::ClickBtnenhctrlMainAlign()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ManuSelect(4);
	WindowVisable_recipe();
	MainAlign();
	m_btn_align.SetValue(TRUE);
}


void CEMIControlDlg::ClickBtnenhctrlMainMain()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//PacketSendIPS(GetGrabReadyPacket(1, m_RecipeScan->GetInspect90Degree(2), 2, 0, FALSE, 99000));
	CDialogMSG dlg;

	if(m_bScanAfterWait)
	{
		dlg.SetCaption(_T("스캔 후 대기 상태 입니다. 진행 하시겠습니까?"));
		if(0 == dlg.DoModal())
		{
			return;
		}
	}

	m_bDefectFileWriteBreak = FALSE;
	m_strMainMsg.Format(_T("Main Sequence Start"));
	m_btn_MainMsg.SetWindowTextW(m_strMainMsg);
	if(0 == AutoStart())
	{
		//OK

		//Button
		m_btn_start.SetEnabled(FALSE);
		m_btn_start.SetValue(FALSE);
		m_btn_stop.SetEnabled(TRUE);
		m_btn_stop.SetValue(TRUE);
		m_btn_reset.SetEnabled(FALSE);
		m_btn_reset.SetValue(FALSE);
	}
	else
	{
		//Button
		m_btn_start.SetEnabled(TRUE);
		m_btn_start.SetValue(TRUE);
		m_btn_stop.SetEnabled(FALSE);
		m_btn_stop.SetValue(FALSE);
		m_btn_reset.SetEnabled(FALSE);
		m_btn_reset.SetValue(FALSE);
	}
}


void CEMIControlDlg::ClickBtnenhctrlMainStop()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	AutoStop();
	m_bDefectFileWriteBreak = TRUE;

	//Button
	m_btn_start.SetEnabled(TRUE);
	m_btn_start.SetValue(TRUE);
	m_btn_stop.SetEnabled(FALSE);
	m_btn_stop.SetValue(FALSE);
	m_btn_reset.SetEnabled(FALSE);
	m_btn_reset.SetValue(FALSE);
}


void CEMIControlDlg::ClickBtnenhctrlMainReset()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	AutoReset();

	m_LogAlarm->ShowWindow(SW_HIDE);

	//Button
	m_btn_start.SetEnabled(TRUE);
	m_btn_start.SetValue(TRUE);
	m_btn_stop.SetEnabled(FALSE);
	m_btn_stop.SetValue(FALSE);
	m_btn_reset.SetEnabled(FALSE);
	m_btn_reset.SetValue(FALSE);
}

void CEMIControlDlg::ClickBtnenhctrlMainVision()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	WindowVisable_main();
	m_btn_vision.SetValue(TRUE);
}

void CEMIControlDlg::ClickBtnenhctrlMainInline()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect rect;
	ManuSelect(5);
	WindowVisable_main();

	m_dlgInline->ShowWindow(SW_SHOW);
	m_btn_inline.SetValue(TRUE);

	m_dlgInline->GetWindowRect(rect);
	m_dlgInline->MoveWindow(5, 100, rect.Width(), rect.Height(), TRUE);
}

void CEMIControlDlg::ClickBtnenhctrlMainLog()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect rect;
	ManuSelect(6);
	WindowVisable_main();

	m_dlgLogwindow->ShowWindow(SW_SHOW);
	m_btn_log.SetValue(TRUE);

	m_dlgLogwindow->GetWindowRect(rect);
	m_dlgLogwindow->MoveWindow(5, 100, rect.Width(), rect.Height(), TRUE);
}

//20160229 ngh
void CEMIControlDlg::SetServerConnect()
{
	m_Client->Close();
	delete m_Client;

	m_Client = new CClientSock;
	m_Client->Create();
	m_Client->SetWnd(this->m_hWnd);

	m_bConnection = FALSE;

	ServerConnection(m_dlgIPSCtrl->GetIPSIP(), m_dlgIPSCtrl->GetIPSPort());
}

void CEMIControlDlg::ServerConnection(CString strIP, int nPortNum)
{
	m_Client->Connect((LPCTSTR)strIP, nPortNum);
}

//서버 자동 접속 쓰레드
UINT CEMIControlDlg::ServerConnectionThread(LPVOID pParam)
{
	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	while(TRUE)
	{
		if(!pdlg->m_bConnection)
		{
			pdlg->ServerConnection(pdlg->m_dlgIPSCtrl->GetIPSIP(), pdlg->m_dlgIPSCtrl->GetIPSPort());
			Sleep(999);
		}
		else
		{
			Sleep(999);
		}

		if(pdlg->m_bConnectionEnd)
		{
			break;
		}
		
		Sleep(1);
	}
	
	return 0;
}

//LSS->GetGrabReadyPacket(int cam_idx, BOOL rotate, int scan_idx, BOOL derection, int grab_height, int rect_num, RECT *rect, int thresh)
//20160302 ngh
CString CEMIControlDlg::GetGrabReadyPacket(int cam_idx, BOOL rotate, int type_idx, int scan_idx, BOOL derection, int grab_height)
{
	CString senddata, temp1, temp2, temp;
	CString glass_code, lot_id;
	int i;
	RECT rect;

	temp.Format(_T(""));
	temp.Format(_T("%S"), m_Inline->GetUnloaderGlassData().strGlassID);
	AfxExtractSubString(glass_code, temp, 0, ' ');

	temp.Format(_T(""));
	temp.Format(_T("%S"), m_Inline->GetUnloaderGlassData().strLotID);
	AfxExtractSubString(lot_id, temp, 0, ' ');

	//Start packet
	//temp1.Format(_T("0001,%s,%d,%d,%d,%d,%d"), glass_code, cam_idx, rotate, scan_idx, derection, grab_height);
#ifdef CAMERA
	if(type_idx < 3)
	{
		temp1.Format(_T("0001,%s,%s,%d,0,%d,%d,%d,%d"), lot_id, glass_code, cam_idx, rotate, scan_idx, derection, grab_height);
	}
	else
	{
		temp1.Format(_T("0001,%s,%s,%d,1,%d,%d,%d,%d"), lot_id, glass_code, cam_idx, rotate, scan_idx, derection, grab_height);
	}
#else
	if(type_idx < 3)
	{
		temp1.Format(_T("0001,%s,%s,%d,0,%d,%d,%d,%d"), lot_id, glass_code, cam_idx, rotate, 0, derection, grab_height);
	}
	else
	{
		temp1.Format(_T("0001,%s,%s,%d,1,%d,%d,%d,%d"), lot_id, glass_code, cam_idx, rotate, 0, derection, grab_height);
	}
#endif

	if(cam_idx == 0)
	{
		//8K scan thresh 5개(가변) defect_size
		temp2.Format(_T("%s,5,%d,%d,%d,%d,%d,5,%f,%f,%f,%f,%f"), temp1, 
			m_RecipeScan->GetThresh(type_idx, 0), m_RecipeScan->GetThresh(type_idx, 1), m_RecipeScan->GetThresh(type_idx, 2), m_RecipeScan->GetThresh(type_idx, 3), m_RecipeScan->GetThresh(type_idx, 4),
			m_RecipeScan->GetDefectSize(type_idx, 0), m_RecipeScan->GetDefectSize(type_idx, 1), m_RecipeScan->GetDefectSize(type_idx, 2), m_RecipeScan->GetDefectSize(type_idx, 3), m_RecipeScan->GetDefectSize(type_idx, 4)
			);
		//noise size width, height, area
		temp1.Format(_T("%s,3,%d,%d,%d"), temp2, m_RecipeScan->GetNoiseSize(type_idx, 0), m_RecipeScan->GetNoiseSize(type_idx, 1), m_RecipeScan->GetNoiseSize(type_idx, 2));

		//marge distance
		temp2.Format(_T("%s,1,%f"),temp1, m_RecipeScan->GetMargeDistance(type_idx));
	}
	else
	{
		if(type_idx == 2)
		{
			if(rotate)
			{
				temp2.Format(_T("%s,5,%f,%d,%d,%d,%d,5,%d,%d,%d,%d,%d"), temp1, 
					m_RecipeScan->GetScanPixelY(type_idx), m_RecipeScan->GetThresh(type_idx, 0), m_RecipeScan->GetThresh(type_idx, 1), m_RecipeScan->GetThresh(type_idx, 2), m_RecipeScan->GetThresh(type_idx, 3),
					m_RecipeScan->GetDefectSize(type_idx, 0), m_RecipeScan->GetDefectSize(type_idx, 1), m_RecipeScan->GetDefectSize(type_idx, 2), m_RecipeScan->GetDefectSize(type_idx, 3), m_RecipeScan->GetDefectSize(type_idx, 4)
					);
			}
			else
			{
				temp2.Format(_T("%s,5,%f,%d,%d,%d,%d,5,%d,%d,%d,%d,%d"), temp1, 
					m_RecipeScan->GetScanPixelX(type_idx), m_RecipeScan->GetThresh(type_idx, 0), m_RecipeScan->GetThresh(type_idx, 1), m_RecipeScan->GetThresh(type_idx, 2), m_RecipeScan->GetThresh(type_idx, 3),
					m_RecipeScan->GetDefectSize(type_idx, 0), m_RecipeScan->GetDefectSize(type_idx, 1), m_RecipeScan->GetDefectSize(type_idx, 2), m_RecipeScan->GetDefectSize(type_idx, 3), m_RecipeScan->GetDefectSize(type_idx, 4)
					);
			}
		}
		else if(type_idx == 3)
		{
			temp2.Format(_T("%s,5,%f,%d,%d,%d,%d,5,%d,%d,%d,%d,%d"), temp1, 
				m_RecipeScan->GetScanPixelY(type_idx), m_RecipeScan->GetThresh(type_idx, 0), m_RecipeScan->GetThresh(type_idx, 1), m_RecipeScan->GetThresh(type_idx, 2), m_RecipeScan->GetThresh(type_idx, 3),
				m_RecipeScan->GetDefectSize(type_idx, 0), m_RecipeScan->GetDefectSize(type_idx, 1), m_RecipeScan->GetDefectSize(type_idx, 2), m_RecipeScan->GetDefectSize(type_idx, 3), m_RecipeScan->GetDefectSize(type_idx, 4)
				);
		}
		else
		{
			temp2.Format(_T("%s,5,%f,%d,%d,%d,%d,5,%d,%d,%d,%d,%d"), temp1, 
				m_RecipeScan->GetScanPixelX(type_idx), m_RecipeScan->GetThresh(type_idx, 0), m_RecipeScan->GetThresh(type_idx, 1), m_RecipeScan->GetThresh(type_idx, 2), m_RecipeScan->GetThresh(type_idx, 3),
				m_RecipeScan->GetDefectSize(type_idx, 0), m_RecipeScan->GetDefectSize(type_idx, 1), m_RecipeScan->GetDefectSize(type_idx, 2), m_RecipeScan->GetDefectSize(type_idx, 3), m_RecipeScan->GetDefectSize(type_idx, 4)
				);
		}
	}

	//검사 Rect 정보(포함된 rect만 보낸다. image좌표)
	temp1.Format(_T("%s,%d"), temp2, m_RecipeScan->GetActiveRectNum(type_idx, scan_idx));
	for(i = 0; i < m_RecipeScan->GetActiveRectNum(type_idx, scan_idx); i++)
	{
		rect = m_RecipeScan->GetActiveRect(type_idx, scan_idx, i);
		temp2.Format(_T("%s,%d,%d,%d,%d"), temp1, rect.left, rect.top, rect.right, rect.bottom);
		temp1 = temp2;
	}

	if(cam_idx == 1)
	{
		if(type_idx == 2)
		{
			temp2.Format(_T("%s,%d"), temp1, m_RecipeScan->GetCVDActiveRectNum(type_idx, scan_idx));
			for(i = 0; i < m_RecipeScan->GetCVDActiveRectNum(type_idx, scan_idx); i++)
			{
				rect = m_RecipeScan->GetCVDActiveRect(type_idx, scan_idx, i);
				temp1.Format(_T("%s,%d,%d,%d,%d"), temp2, rect.left, rect.top, rect.right, rect.bottom);
				temp2 = temp1;
			}

			temp1.Format(_T("%s,%d"), temp2, m_RecipeScan->GetCVDDNActiveRectNum(type_idx, scan_idx));
			for(i = 0; i < m_RecipeScan->GetCVDDNActiveRectNum(type_idx, scan_idx); i++)
			{
				rect = m_RecipeScan->GetCVDDNActiveRect(type_idx, scan_idx, i);
				temp2.Format(_T("%s,%d,%d,%d,%d"), temp1, rect.left, rect.top, rect.right, rect.bottom);
				temp1 = temp2;
			}
		}
		else if(type_idx >= 3)
		{
			temp2.Format(_T("%s,%d"), temp1, m_RecipeScan->GetPCLActiveRectNum(type_idx, scan_idx, 0));
			for(i = 0; i < m_RecipeScan->GetPCLActiveRectNum(type_idx, scan_idx, 0); i++)
			{
				rect = m_RecipeScan->GetPCLActiveRect(type_idx, scan_idx, i, 0);
				temp1.Format(_T("%s,%d,%d,%d,%d"), temp2, rect.left, rect.top, rect.right, rect.bottom);
				temp2 = temp1;
			}

			temp1.Format(_T("%s,%d"), temp2, m_RecipeScan->GetPCLActiveRectNum(type_idx, scan_idx, 1));
			for(i = 0; i < m_RecipeScan->GetPCLActiveRectNum(type_idx, scan_idx, 1); i++)
			{
				rect = m_RecipeScan->GetPCLActiveRect(type_idx, scan_idx, i, 1);
				temp2.Format(_T("%s,%d,%d,%d,%d"), temp1, rect.left, rect.top, rect.right, rect.bottom);
				temp1 = temp2;
			}
		}
		/*else if(type_idx == 4)
		{
			temp2.Format(_T("%s,%d"), temp1, m_RecipeScan->GetPCLActiveRectNum(type_idx, scan_idx, 0));
			for(i = 0; i < m_RecipeScan->GetPCLActiveRectNum(type_idx, scan_idx, 0); i++)
			{
				rect = m_RecipeScan->GetPCLActiveRect(type_idx, scan_idx, i, 0);
				temp1.Format(_T("%s,%d,%d,%d,%d"), temp2, rect.left, rect.top, rect.right, rect.bottom);
				temp2 = temp1;
			}

			temp1.Format(_T("%s,%d"), temp2, m_RecipeScan->GetPCLActiveRectNum(type_idx, scan_idx, 1));
			for(i = 0; i < m_RecipeScan->GetPCLActiveRectNum(type_idx, scan_idx, 1); i++)
			{
				rect = m_RecipeScan->GetPCLActiveRect(type_idx, scan_idx, i, 1);
				temp2.Format(_T("%s,%d,%d,%d,%d"), temp1, rect.left, rect.top, rect.right, rect.bottom);
				temp1 = temp2;
			}
		}*/
	}
	senddata = temp1;
	return senddata;
}

//20151130 ngh
BOOL CEMIControlDlg::PacketSendIPS(CString packet)
{
	if(!m_bConnection)
	{
		return FALSE;
	}
	char senddata[10240];

	sprintf_s(senddata, packet.GetLength() + 3, "%c%S%c", PACKET_CHAR_STX, packet, PACKET_CHAR_ETX);

	//20160601_2 ngh
	//senddata 값 확인
	//m_Client 생성 제대로 되었는지, ips 연결(ip, port num) 확인
	m_Client->Send(senddata, packet.GetLength() + 3);
	return TRUE;
}

LRESULT CEMIControlDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
	char temp[MAX_PACKET];
	CString rcv_dat, strCmd, strSubCmd, strTemp, strLotID, strCamIdx, strScanIdx;
	int nCamIdx, nRotateIdx, nScanIdx, nNGCnt;
	CString strLog;
	memset(temp, NULL, MAX_PACKET);

	m_Client->Receive(temp, MAX_PACKET);
	rcv_dat.Format(_T("%S"),temp);
	rcv_dat.Delete(0, 1);
	rcv_dat.Delete(rcv_dat.GetLength() - 1, 1);

	AfxExtractSubString(strCmd, rcv_dat, 0, ',');

	//20151130 ngh
	if(strCmd == "1001")//GrabReadyComp
	{
		AfxExtractSubString(strSubCmd, rcv_dat, 1, ',');
		AfxExtractSubString(strTemp, rcv_dat, 2, ',');//카메라 번호
		nCamIdx = _wtoi(strTemp);
		AfxExtractSubString(strTemp, rcv_dat, 3, ',');//회전 번호
		nRotateIdx = _wtoi(strTemp);
		AfxExtractSubString(strTemp, rcv_dat, 4, ',');//스캔 번호
		nScanIdx = _wtoi(strTemp);
		m_vision_Inspection_comp[m_Rslt.m_iScanTypeIdx] = FALSE;
		
		if(nCamIdx == 1)
		{
			strTemp.Format(_T("16K CVD Inspection"));
		}
		else if(nCamIdx == 2)
		{
			strTemp.Format(_T("16K PCL Inspection"));
			nCamIdx-=1;
		}
		else
		{
			strTemp.Format(_T("8K Inspection"));
		}

		if(strSubCmd == "OK")
		{
			strLog.Format(_T("%s %d Cam %d Degree %d Scan Grab Ready Comp OK"), strTemp, nCamIdx, nRotateIdx, nScanIdx);
			m_vision_grab_comp = FALSE;
			m_vision_grab_ready = TRUE;
		}
		else if(strSubCmd == "NG")
		{
			strLog.Format(_T("%s %d Cam %d Degree %d Scan Grab Ready Comp Fail"), strTemp, nCamIdx, nRotateIdx, nScanIdx);
			m_vision_grab_ready = FALSE;
		}
		
		WriteLog(MAIN_LOG, strLog);
	}
	else if(strCmd == "1002")//GrabComp
	{
		AfxExtractSubString(strSubCmd, rcv_dat, 1, ',');
		AfxExtractSubString(strTemp, rcv_dat, 2, ',');//카메라 번호
		nCamIdx = _wtoi(strTemp);
		AfxExtractSubString(strTemp, rcv_dat, 3, ',');//회전 번호
		nRotateIdx = _wtoi(strTemp);
		AfxExtractSubString(strTemp, rcv_dat, 4, ',');//스캔 번호
		nScanIdx = _wtoi(strTemp);

		if(nCamIdx == 1)
		{
			strTemp.Format(_T("16K CVD Inspection"));
		}
		else if(nCamIdx == 2)
		{
			strTemp.Format(_T("16K PCL Inspection"));
			nCamIdx-=1;
		}
		else
		{
			strTemp.Format(_T("8K Inspection"));
		}

		if(strSubCmd == "OK")
		{
			strLog.Format(_T("%s %d Cam %d Degree %d Scan Grab Comp OK"), strTemp, nCamIdx, nRotateIdx, nScanIdx);
			m_vision_grab_comp = TRUE;
			//m_bGrabComp[nCamIdx][nScanIdx] = TRUE;
		}
		else if(strSubCmd == "NG")
		{
			strLog.Format(_T("%s %d Cam %d Degree %d Scan Grab Comp Fail"), strTemp, nCamIdx, nRotateIdx, nScanIdx);
			m_vision_grab_comp = FALSE;
			//m_bGrabComp[nCamIdx][nScanIdx] = FALSE;
		}

		WriteLog(MAIN_LOG, strLog);
	}
	else if(strCmd == "1003")//Inspection Comp
	{
		AfxExtractSubString(strTemp, rcv_dat, 1, ',');//카메라 번호
		nCamIdx = _wtoi(strTemp);
		AfxExtractSubString(strTemp, rcv_dat, 2, ',');//회전 번호
		nRotateIdx = _wtoi(strTemp);
		AfxExtractSubString(strTemp, rcv_dat, 3, ',');//스캔 번호
		nScanIdx = _wtoi(strTemp);
		AfxExtractSubString(strTemp, rcv_dat, 4, ',');//불량 수
		nNGCnt = _wtoi(strTemp);
		AfxExtractSubString(strLotID, rcv_dat, 5, ',');//불량 수

		if(nCamIdx == 1)
		{
			strTemp.Format(_T("16K CVD Inspection"));
		}
		else if(nCamIdx == 2)
		{
			strTemp.Format(_T("16K PCL Inspection"));
			nCamIdx-=1;
		}
		else
		{
			strTemp.Format(_T("8K Inspection"));
		}

		if(nNGCnt == -1)
		{
			strLog.Format(_T("%s %d Cam %d Degree %d Scan Inspection Fail"), strTemp, nCamIdx, nRotateIdx, nScanIdx);
		}
		else
		{
			strLog.Format(_T("%s %d Cam %d Degree %d Scan Inspection OK"), strTemp, nCamIdx, nRotateIdx, nScanIdx);
		}

		WriteLog(MAIN_LOG, strLog);

		//m_vision_Inspection_comp = TRUE;
		if((nScanIdx + 1) == m_RecipeScan->GetScanCnt(m_Rslt.m_iScanTypeIdx))
		{
			m_vision_Inspection_comp[m_Rslt.m_iScanTypeIdx] = TRUE;
		}
	}
	else if(strCmd == "1004")//Grab Stop
	{
		m_Trigger->SetOPMode(FALSE);
	}

	else if(strCmd == "5000")//IPSAutoRunstate
	{
		m_bIPSAutoRunState = TRUE;
	}
	else if(strCmd == "5001")//IPSAutoRunstate
	{
		m_bIPSAutoRunState = FALSE;
	}
	return TRUE;
}

BOOL CEMIControlDlg::GetIPSAutoRunState()
{
	return m_bIPSAutoRunState;
}
//m_btn_ips_autorun_use
//IPS 서버 종료
LRESULT CEMIControlDlg::OnClose(WPARAM wParam, LPARAM lParam)
{
	m_Client->Close();
	delete m_Client;

	AfxSocketInit();
	m_Client = new CClientSock;
	m_Client->Create();
	m_Client->SetWnd(this->m_hWnd);

	m_bConnection = FALSE;

	return TRUE;
}

//IPS 서버 접속
LRESULT CEMIControlDlg::OnConnect(WPARAM wParam, LPARAM lParam)
{
	m_bConnection = TRUE;

	return TRUE;
}

UINT CEMIControlDlg::MotionThreadECS(LPVOID pParam)
{
	CEMIControlDlg *pdlg = (CEMIControlDlg *)pParam;

	pdlg->MotionThreadECS();

	return 0;
}

void CEMIControlDlg::MotionThreadECS()
{
	//Event bit만 체크함
	//준비가 안되었는데 ECS에서 요구하면 log 남길것.
	//시퀀스가 필요하면 motionprocess case 3 이용할것
	while(m_Flag.m_bProcessThread)
	{
		//ESC event check process(event bit reading)

		Sleep(10);
	}

	AfxEndThread(0);
}

UINT CEMIControlDlg::MotionThread(LPVOID pParam)
{
	CEMIControlDlg *pdlg = (CEMIControlDlg *)pParam;

	pdlg->MotionThread();

	return 0;
}

void CEMIControlDlg::MotionThread()
{
	int rslt[MOT_PROCESS_MAX_NUM][MOT_STATUS_MAX_NUM];
	int i, j, prc_idx, seq_idx, film_idx;
	CString log;
	BOOL chk = FALSE;
	film_idx = 0;
	while(m_Flag.m_bProcessThread)
	{ 
		//Read digital input
		//m_Motion.getDigitalInput(IO_IN_MAX_NUM, m_bIOIn);
		
		chk = TRUE;
		for(i = 0; i < MOT_PROCESS_MAX_NUM; i++)
		{
			for(j = 0; j < MOT_STATUS_MAX_NUM; j++)
			{
				if(!m_MDat[i].m_Seq[j].m_bAutoStart)
				{
					chk = FALSE;
				}
			}
		}
	
		if(chk)
		//Auto mode이면
		{
			//Proccessing
			for(i = 0; i < MOT_PROCESS_MAX_NUM; i++)
			{
				for(j = 0; j < MOT_STATUS_MAX_NUM; j++)
				{
					rslt[i][j] = MotionProcess(i, j);
					
					if(1 == rslt[i][j])
					{
						//Auto status
						
					}
					else if(0 == rslt[i][j])
					{
						
						//Error
						
						
						//정지
						AutoStop();

						//Alarm동작
						AlarmOperation(i, j, m_MDat[i].m_Seq[j].m_iSubSeqIdx);
						
						m_MDat[i].m_Seq[j].m_bAlarm = TRUE;

						//먼져 에러가 나는쪽 먼져 처리한다.
						::SendMessage(this->m_hWnd, USER_MSG_MOT, 0, 0);
						
												
						//리셋시 다음번 동작예약
						m_MDat[i].m_Seq[j].m_iSubSeqIdx = m_MDat[i].m_Seq[j].m_iNextSubSeqIdx;
						
						//Buzzer on
						m_IO->SetWriteDO(DO_BUZZER_MELODY1, TRUE);
						break;
					}
				}
			}

		}
		else
		{
			
			if( 1 == StopModeOperation(&prc_idx, &seq_idx) )
			{

			}
			else
			{
				//정지
				AutoStop();

				//Alarm동작
				AlarmOperation(prc_idx, seq_idx, m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx);

				//Error
				m_MDat[prc_idx].m_Seq[seq_idx].m_bAlarm = TRUE;

				//먼져 에러가 나는쪽 먼져 처리한다.
				::SendMessage(this->m_hWnd, USER_MSG_MOT, 0, 0);

				//Buzzer on
				m_IO->SetWriteDO(DO_BUZZER_MELODY1, TRUE);

			}

		
			Sleep(1);
		}

		Sleep(1);
	}

	AfxEndThread(0);
}

//메인 시퀀스 함수
int CEMIControlDlg::MotionProcess(int prc_idx, int seq_idx)
{
	CRect rect, dlg_rect;
	CString temp;
	CString strLog, strInsType;
	int rslt = 1;
	int i, j, idx, sub_seq_idx , film_idx;
	BOOL chk_timeout = TRUE;
	BOOL not_delay = TRUE;
	double spd = RECIPE_SCAN_SCANSPD;
	int scan_height = 0;
	int glass_rslt = 0;
	double p_time = 0;;

	char status_type[20];
	memset(status_type, NULL, sizeof(status_type));
	double x, y;

	double robot_x, robot_y;
	double pixel_size;
	double offset_x, offset_y;
	double glass_x, glass_y;
	double review_cent_x, review_cent_y;

	//20160811 ngh
	CString glass_id, lot_id;

	
	//ECS
	typeEQPStatusDataPara eq_sts;
	typeGlassDataPara glass_data;

	//20160711 ngh
	CTime cTime;
	cTime = CTime::GetCurrentTime();
	CString imagefilename, temp_image_path, image_path;


	/////////////////////////////////////////
	//
	//
	switch(seq_idx)
	{
		//Stop mode check
	case 0:
		chk_timeout = FALSE;

		break;

	case 1:
		/////////////////////////////////////
		//Safe and limit sensor(inter lock)
		//
		sub_seq_idx = m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx;
		switch(sub_seq_idx)
		{
		case 0:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "EMI Emergency On Error=X51B : EMO OP(EMO 누름 감지)!-000-");	

			if(!m_IO->GetReadDI(DI_EMI_OP))
			{
				SetSeqIdx(prc_idx, seq_idx, 1);
			}
			break;
		case 1:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "EMI Emergency On Error=X51C : EMO LEFT(EMO 누름 감지)!-001-");	

			if(!m_IO->GetReadDI(DI_EMI_LEFT))
			{
				SetSeqIdx(prc_idx, seq_idx, 2);
			}
			break;
		case 2:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "EMI Emergency On Error=X51D : EMO BACK(EMO 누름 감지)!-002-");	

			if(!m_IO->GetReadDI(DI_EMI_BACK))
			{
				SetSeqIdx(prc_idx, seq_idx, 3);
			}
			break;
		case 3:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "EMI Emergency On Error=X51E : EMO INSIDE(EMO 누름 감지)!-003-");	

			if(!m_IO->GetReadDI(DI_EMI_INSIDE))
			{
				SetSeqIdx(prc_idx, seq_idx, 4);
			}
			break;

		case 4:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Stage MC Off Alarm=Loader Emergency On Error=X500 : EMO LOADER(EMO 누름 감지)!-004-");	

			//if(!m_IO->GetReadDI(DI_LOADER_EMO_SIG))//사용안함
			if(1)
			{
				SetSeqIdx(prc_idx, seq_idx, 5);
			}
			break;

		case 5:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Stage MC Off Alarm=X501 : STAGE MC1 ALARM(센서 감지않됨)!-005-");	

			if(m_IO->GetReadDI(DI_STAGE_MC1_ON))
			{
				SetSeqIdx(prc_idx, seq_idx, 6);
			}
			break;

		case 6:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Stage MC Off Alarm=X502 : STAGE MC2 ALARM(센서 감지않됨)!-006-");	

			if(m_IO->GetReadDI(DI_STAGE_MC2_ON))
			{
				SetSeqIdx(prc_idx, seq_idx, 8);
			}
			break;

		case 8:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Safety MC Off Alarm=X510 : SAFETY UNIT MC 1, 2 감지않됨(센서 감지않됨)!-008-");	

			//if(m_IO->GetReadDI(DI_SAFETY_UNIT_MC_1_2_ON))
			if(1)//사용안함
			{
				SetSeqIdx(prc_idx, seq_idx, 9);
			}
			break;

		case 9:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Safety MC Off Alarm=X512 : SAFETY UNIT MC 3, 4 감지않됨(센서 감지않됨)!-009-");	

			//if(m_IO->GetReadDI(DI_SAFETY_UNIT_MC_3_4_ON))
			if(1)//사용안함
			{
				SetSeqIdx(prc_idx, seq_idx, 10);
			}
			break;

		case 10:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "FAN ALARM 1=X53E : OUTSIDE COOLING FAN ALARM(센서 감지않됨)!-010-");	

			if(m_IO->GetReadDI(DI_OP_COOLING_FAN_ALARM))
			{
				SetSeqIdx(prc_idx, seq_idx, 11);
			}
			break;

		case 11:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "FAN ALARM 2=X53F : INSIDE COOLING FAN ALARM(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_REAR_COOLING_FAN_ALARM))
			{
				SetSeqIdx(prc_idx, seq_idx, 12);
			}
			break;
		
		case 12:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Door Open Error=X514 : DOOR#1 LEFT(DOOR 열림 감지)!-012-");	
			if(m_Flag.m_bAutoStart)
			{
				if(m_IO->GetReadDI(DI_DO1_OPEN_LEFT))
				{
					SetSeqIdx(prc_idx, seq_idx, 13);
				}
			}
			break;
		//20160310 sh
		case 13:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ELC ALARM 2=X520 : ELCB 01 TRIP MAINT LAMP(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_ELCB_1_TRIP_MAINT_LAMP))
			{
				SetSeqIdx(prc_idx, seq_idx, 14);
			}
			break;
		case 14:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ELC ALARM 2=X526 : ELCB 02 TRIP UPS PL LAMP(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_ELCB_2_TRIP_UPS_PL_LAMP))
			{
				SetSeqIdx(prc_idx, seq_idx, 15);
			}
			break;
		case 15:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ELC ALARM 2=X521 : ELCB 03 TRIP EFU(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_ELCB_3_TRIP_EFU))
			{
				SetSeqIdx(prc_idx, seq_idx, 16);
			}
			break;
		case 16:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ELC ALARM 2=X522 : ELCB 04 TRIP METAL SOURCE(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_ELCB_4_TRIP_METAL_SRC))
			{
				SetSeqIdx(prc_idx, seq_idx, 17);
			}
			break;
		case 17:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ELC ALARM 2=X523 : ELCB 05 TRIP PC CONCENT(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_ELCB_5_TRIP_PC_CONCENT))
			{
				SetSeqIdx(prc_idx, seq_idx, 18);
			}
			break;
		case 18:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ELC ALARM 2=X527 :ELCB 06 TRIP MAIN PC, TRIGGER(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_ELCB_6_TRIP_MAIN_PC_TRIGGER))
			{
				SetSeqIdx(prc_idx, seq_idx, 19);
			}
			break;
		case 19:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ELC ALARM 2=X528 : ELCB 07 TRIP MODULE PC(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_ELCB_7_TRIP_MOD_PC))
			{
				SetSeqIdx(prc_idx, seq_idx, 20);
			}
			break;
		case 20:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ELC ALARM 2=X525 :ELCB 08 TRIP REVIEW, AF(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_ELCB_8_TRIP_REVIEW_AF))
			{
				SetSeqIdx(prc_idx, seq_idx, 21);
			}
			break;
		case 21:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "APC ALARM 2=X529 : ACP01 TRIP AVC METER(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_ACP1_TRIP_AVC_METER))
			{
				SetSeqIdx(prc_idx, seq_idx, 22);
			}
			break;
		case 22:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "APC ALARM 2=X52A : ACP02 TRIP AVC POWER LAMP(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_ACP2_TRIP_AVC_POW_LAMP))
			{
				SetSeqIdx(prc_idx, seq_idx, 23);
			}
			break;
		case 23:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "APC ALARM 2=X52B : ACP03, ACP04 TRIP MAINT LAMP(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_ACP3_4_TRIP_MAINT_LAMP))
			{
				SetSeqIdx(prc_idx, seq_idx, 24);
			}
			break;
		case 24:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "APC ALARM 2=X52C : ACP05 TRIP PC RACK FAN(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_ACP5_TRIP_PC_RACK_FAN))
			{
				SetSeqIdx(prc_idx, seq_idx, 25);
			}
			break;
		case 25:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "APC ALARM 2=X52D : ACP06, ACP17, ACP18 TRIP REVIEW, AF(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_ACP6_17_18_TRIP_REVIEW_AF))
			{
				SetSeqIdx(prc_idx, seq_idx, 26);
			}
			break;
		case 26:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "APC ALARM 2=X52E : ACP07, ACP08, ACP16 TRIP ALIGN CAMERA(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_ACP7_8_16_TRIP_ALIGN_CAM))
			{
				SetSeqIdx(prc_idx, seq_idx, 27);
			}
			break;
		case 27:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "APC ALARM 2=X52F : ACP09, ACP12, ACP13 TRIP OPTIC MODULE 1(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_ACP9_12_13_TRIP_OPTIC_MOD1))
			{
				SetSeqIdx(prc_idx, seq_idx, 28);
			}
			break;
		case 28:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "APC ALARM 2=X520 : ACP10, ACP14, ACP15 TRIP OPTIC MODULE 2(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_ACP10_14_15_TRIP_OPTIC_MOD2))
			{
				SetSeqIdx(prc_idx, seq_idx, 29);
			}
			break;
		case 29:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "UCP ALARM 2=X531 :UCP01 TRIP UPS METER(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_UCP1_TRIP_UPS_METER))
			{
				SetSeqIdx(prc_idx, seq_idx, 30);
			}
			break;
		case 30:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "UCP ALARM 2=X532 :UCP02 TRIP UPS POWER LAMP(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_UCP2_TRIP_UPS_POW_LAMP))
			{
				SetSeqIdx(prc_idx, seq_idx, 31);
			}
			break;
		case 31:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "UCP ALARM 2=X533 :UCP03 UCP04 TRIP SAFETY(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_UCP3_4_TRIP_SAFETY))
			{
				SetSeqIdx(prc_idx, seq_idx, 32);
			}
			break;
		case 32:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "UCP ALARM 2=X534 :UCP07, UCP08 TRIP MC COIL(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_UCP7_8_TRIP_MC_COIL))
			{
				SetSeqIdx(prc_idx, seq_idx, 33);
			}
			break;
		case 33:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "UCP ALARM 2=X535 :UCP12 TRIP IONIZER(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_UCP12_TRIP_IONIZER))
			{
				SetSeqIdx(prc_idx, seq_idx, 34);
			}
			break;
		case 34:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "UCP ALARM 2=X536 :UCP13 TRIP PANEL FAN(센서 감지않됨)!-011-");	

			if(m_IO->GetReadDI(DI_UCP13_TRIP_PANEL_PAN))
			{
				SetSeqIdx(prc_idx, seq_idx, 35);
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 35);
			}
			break;
		//20160310 sh
		//센서사용않함
		case 35:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Door Open Error=X515 : DOOR#1 RIGHT (DOOR 열림 감지)!-013-");	
			if(m_Flag.m_bAutoStart)
			{
				//if(m_IO->GetReadDI(DI_DO1_OPEN_RIGHT))
				if(1)
				{
					SetSeqIdx(prc_idx, seq_idx, 36);
				}
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 36);
			}
			break;
		case 36:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Door Open Error=X516 : DOOR#2 LEFT(DOOR 열림 감지)!-014-");	
			if(m_Flag.m_bAutoStart)
			{
				if(m_IO->GetReadDI(DI_DO2_OPEN_LEFT))
				{
					SetSeqIdx(prc_idx, seq_idx, 37);
				}
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 37);
			}
			break;
		case 37:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Door Open Error=X517 : DOOR#2 RIGHT(DOOR 열림 감지)!-015-");	
			if(m_Flag.m_bAutoStart)
			{
				if(m_IO->GetReadDI(DI_DO2_OPEN_RIGHT))
				{
					SetSeqIdx(prc_idx, seq_idx, 38);
				}
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 38);
			}
			break;
		case 38:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Door Open Error=X518 : DOOR#3 LEFT(DOOR 열림 감지)!-016-");	
			if(m_Flag.m_bAutoStart)
			{
				if(m_IO->GetReadDI(DI_DO3_OPEN_LEFT))
				{
					SetSeqIdx(prc_idx, seq_idx, 39);
				}
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 39);
			}
			break;
		case 39:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Door Open Error=X519 : DOOR#3 RIGHT(DOOR 열림 감지)!-017-");	
			if(m_Flag.m_bAutoStart)
			{
				if(m_IO->GetReadDI(DI_DO3_OPEN_RIGHT))
				{
					SetSeqIdx(prc_idx, seq_idx, 40);
				}
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 40);
			}
			break;
		case 40:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR + 500;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Main CDA Error=X50B : MAIN CDA PRESSURE(양압센서 감시않됨)!-018-");	
			if(m_Flag.m_bAutoStart)
			{
				if(1)//m_IO->GetReadDI(DI_CHUCK_AIR_BLOW_ON_DETECT))20160218
				{
					SetSeqIdx(prc_idx, seq_idx, 0);
				}
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 0);
			}
			break;
		}
		break;
	case 2:
		/////////////////////////////////////
		//Main sequence
		//
		sub_seq_idx = m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx;
		switch(sub_seq_idx)
		{
		case 100:
			m_Review->StopLive();
			//종료/////////////////////////////////////////////////////////////////////////////
			m_Light->Set8KLight(0);
			m_Light->Set16KLight(0);

			//memset(&m_Rslt, NULL, sizeof(typeProcessResult));

			m_Rslt.m_iScanTypeIdx = 0;
			m_bDefectWriteComp = FALSE;
			m_bDefectFileSave[0] = TRUE;
			m_bDefectFileSave[1] = TRUE;
			m_bDefectFileSave[2] = TRUE;
			m_bDefectFileSave[3] = TRUE;
			m_bDefectFileSave[4] = TRUE;

			for(int i = 0; i < RECIPE_SCAN_TYPE_MAX_NUM; i++)
			{
				m_vision_Inspection_comp[i] = FALSE;
			}

			m_strMainMsg.Format(_T("Main Sequence Start"));
			WriteLog(MAIN_LOG, m_strMainMsg);

			//Start
			SetSeqIdx(prc_idx, seq_idx, 110);
			//SetSeqIdx(prc_idx, seq_idx, 235);
			break;
		case 110:
			//초기화///////////////////////////////////////////////////////////////
			if(m_GlassManu->GetLoaderUse())
			{
				m_strMainMsg.Format(_T("Auto Inspection Start"));
			}
			else
			{
				ResetRsltSizeCntView();
				m_strMainMsg.Format(_T("Manual Inspection Start"));
			}
			WriteLog(MAIN_LOG, m_strMainMsg);

			SetSeqIdx(prc_idx, seq_idx, 120);

			break;
		case 120:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PIN GLS Front Detect Error=X550,51 : GLASS DETECT SENSOR TIMEOUT(GLASS 감지센서 이상, 모두 감지되거나 모두 감지 않되어야함, 현재 Loader 사용상태)!-120-");	

			//Detect sensor power on
			m_IO->SetWriteDO(DO_GLASS_DETECT_SENSOR_RELAY_ON_OFF, TRUE);

			if( (m_IO->GetReadDI(DI_PIN_GLS_DETECT_FRONT)) ||
				(m_IO->GetReadDI(DI_PIN_GLS_DETECT_REAR)) )
			{
				//Macha aling back
				//SetSeqIdx(prc_idx, seq_idx, 1200);
				SetSeqIdx(prc_idx, seq_idx, 150);
				m_strMainMsg.Format(_T("Glass In"));
				WriteLog(MAIN_LOG, m_strMainMsg);

				//m_Rslt.m_iMechAlignSeq = 130;
				//m_strMainMsg.Format(_T("Mech Align Back"));
				m_Flag.m_bGlassExist = TRUE;
				//WriteLog(MAIN_LOG, m_strMainMsg);
			}
			else
			{
				//Macha aling back
				//SetSeqIdx(prc_idx, seq_idx, 1200);
				SetSeqIdx(prc_idx, seq_idx, 150);
				m_strMainMsg.Format(_T("Glass Out"));
				WriteLog(MAIN_LOG, m_strMainMsg);
				//m_Rslt.m_iMechAlignSeq = 130;
				//m_strMainMsg.Format(_T("Mech Align Back"));
				m_Flag.m_bGlassExist = FALSE;
				//WriteLog(MAIN_LOG, m_strMainMsg);
			}
			break;
		//case 130:
		//	SetSeqIdx(prc_idx, seq_idx, 140);

		//	m_strMainMsg.Format(_T("Lift Pin Down Start"));
		//	WriteLog(MAIN_LOG, m_strMainMsg);

		//	//Lift pin down and mechnical align
		//	m_Rslt.m_dMotPos[MOTOR_3] =  m_RecipePanel->GetPanelLoadPosDN();
		//	m_Robot->SetRobotMove_mm(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]);
		//	break;
		//case 140:
		//	m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
		//	sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "LiftPinAxis Move Time Over Error=AX04 : UP/DN 축동작 타임아웃(축 동작 실패)!-140-");
		//	if(m_Robot->GetIsMoveComplete(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]))
		//	{
		//		m_strMainMsg.Format(_T("Lift Pin Down"));
		//		if(m_Robot->GetIsInposition(MOTOR_3))
		//		{
		//			m_strMainMsg.Format(_T("Mech Align Forwarding"));
		//			WriteLog(MAIN_LOG, m_strMainMsg);
		//			//Mech align forwarding
		//			SetSeqIdx(prc_idx, seq_idx, 1400);
		//			
		//			m_Rslt.m_iMechAlignSeq = 150;
		//		}
		//	}
		//	break;
		case 150:
			m_strMainMsg.Format(_T("Ez Move Start(Ez Motor Init)"));
			WriteLog(MAIN_LOG, m_strMainMsg);
			m_dlgAFCtrl->SetEzSpeed(60000);
			m_dlgAFCtrl->SetEziMove_mm(0);
			SetSeqIdx(prc_idx, seq_idx, 155);
			break;
		case 155:
			//Move end(ez)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Ez Move Time Over Error=AX05 : SCAN 축동작 타임아웃(축 동작 실패)!-155-");
			
			if(m_dlgAFCtrl->GetEzPosComplete(m_Rslt.m_iScanTypeIdx))
			{
				m_strMainMsg.Format(_T("Ez Move(Ez Motor Init)"));
				WriteLog(MAIN_LOG, m_strMainMsg);
				SetSeqIdx(prc_idx, seq_idx, 160);
			}
			break;
		case 160:
			if(m_GlassManu->GetLoaderUse())
			{
				m_strMainMsg.Format(_T("Loading Position Move Start"));
				WriteLog(MAIN_LOG, m_strMainMsg);
				//Move loading position
				m_Rslt.m_dMotPos[MOTOR_0] =  m_RecipePanel->GetPanelLoadPosY();
				m_Robot->SetRobotMove_mm(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]);
				m_Rslt.m_dMotPos[MOTOR_2] =  m_RecipePanel->GetPanelLoadPosT();
				m_Robot->SetRobotMove_mm(MOTOR_2, m_Rslt.m_dMotPos[MOTOR_2]);
			}
			else
			{
				m_strMainMsg.Format(_T("Manual Loading Position Move Start"));
				WriteLog(MAIN_LOG, m_strMainMsg);
				//Move manual loading position
				m_Rslt.m_dMotPos[MOTOR_0] = m_RecipePanel->GetPanelManuLoadPosY();
				m_Robot->SetRobotMove_mm(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]);
				m_Rslt.m_dMotPos[MOTOR_2] = m_RecipePanel->GetPanelManuLoadPosT();
				m_Robot->SetRobotMove_mm(MOTOR_2, m_Rslt.m_dMotPos[MOTOR_2]);
			}
			SetSeqIdx(prc_idx, seq_idx, 170);
			break;
		case 170:
			//Move end(scan)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ScanAxis Move Time Over Error=AX01 : SCAN 축동작 타임아웃(축 동작 실패)!-170-");

			if(m_Robot->GetIsMoveComplete(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]))
			{
				if(m_Robot->GetIsInposition(MOTOR_0))
				{
					m_strMainMsg.Format(_T("ScanAxis Move End"));
					WriteLog(MAIN_LOG, m_strMainMsg);
					SetSeqIdx(prc_idx, seq_idx, 180);
				}
			}
			break;
		case 180:
			//Move end(theta)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "RotatingAxis Move Time Over Error=AX03 : THETA 축동작 타임아웃(축 동작 실패)-180");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_2, m_Rslt.m_dMotPos[MOTOR_2]))
			{
				m_strMainMsg.Format(_T("RotatingAxis Move"));
				if(m_Robot->GetIsInposition(MOTOR_2))
				{
					if(m_GlassManu->GetLoaderUse())
					{
						//m_strMainMsg.Format(_T("Backword Mech Align"));
						//WriteLog(MAIN_LOG, m_strMainMsg);

						//Backword mech align
						//SetSeqIdx(prc_idx, seq_idx, 1200);
						SetSeqIdx(prc_idx, seq_idx, 190);
						
						//m_Rslt.m_iMechAlignSeq = 190;
					}
					else
					{
						if(m_Rslt.m_bStartManualInspect)
						{
							SetSeqIdx(prc_idx, seq_idx, 190);
						}
					}
				}
			}
			break;
		
		case 190:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 200);
			}	
			break;
		case 200:
			//Vac check 
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Chuck VAC Error=X50C : STAGE VACCUM PRESSURE OFF TIMEOUT(진공센서 감지됨)!-200-");	
			//if(!m_IO->GetReadDI(DI_CHUCK_VACUUM_ON_DETECT))
			//임시
			if(1)
			{
				SetSeqIdx(prc_idx, seq_idx, 205);

				//제품 유무체크도해야한다.

				m_strMainMsg.Format(_T("ShiftAxis Move Start"));
				WriteLog(MAIN_LOG, m_strMainMsg);
				//X축 안전위치 이동(위치 : 0)
				m_Rslt.m_dMotPos[MOTOR_1] =  0;
				m_Robot->SetRobotMove_mm(MOTOR_1, m_Rslt.m_dMotPos[MOTOR_1]);
			}
			break;
		case 205:
			//Move end(shift)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ShiftAxis Move Time Over Error=AX02 : SHIFT 축동작 타임아웃(축 동작 실패)!-205-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_1, m_Rslt.m_dMotPos[MOTOR_1]))
			{
				if(m_Robot->GetIsInposition(MOTOR_1))
				{
					m_strMainMsg.Format(_T("ShiftAxis Move"));
					WriteLog(MAIN_LOG, m_strMainMsg);
					SetSeqIdx(prc_idx, seq_idx, 206);
				}
			}
			break;
		case 206://20160726 ngh
			//Macha aling
			SetSeqIdx(prc_idx, seq_idx, 3000);
			m_strMainMsg.Format(_T("Unloading Wating Macha Align Start"));
			WriteLog(MAIN_LOG, m_strMainMsg);
			m_Rslt.m_iMechAlignSeq = 207;
			if(m_bMachaAlignRepet)
			{
				m_Rslt.m_iMechAlignSeq = 210;
				//SetSeqIdx(prc_idx, seq_idx, 210);
			}
			break;
		case 207:
			//20160711 ngh 스캔 후 대기
			if(m_bScanAfterWait && m_Flag.m_bGlassExist)
			{
				eq_sts = m_Inline->GetStatusReport();
				eq_sts.bGlassExistInUnit = m_Flag.m_bGlassExist;	    //Glass유무
				eq_sts.bGlassInProcess = FALSE;							//검사중인지
				eq_sts.bIonizer = TRUE;
				eq_sts.bUnitAutoMode = TRUE;							//자동반출 가능한지			
				eq_sts.nCurrenRecipeNum = m_Recipe->GetRecipeNo() + 1;
				if(m_Flag.m_bGlassExist)
				{
					eq_sts.nGlassCountInUnit = 1;
					eq_sts.nPutInfoPossibleCount = 0;
					eq_sts.nStatusRunIdleDown = 0;
				}
				else
				{
					eq_sts.nGlassCountInUnit = 0;
					eq_sts.nPutInfoPossibleCount = 1;
					eq_sts.nStatusRunIdleDown = 1;
				}

				eq_sts.nStatusRunIdleDown = !m_Flag.m_bGlassExist;		//0 : RUN~
				eq_sts.nUnitOpeationMode = 0;							//1 : Manual, 0 : auto
				m_Inline->SetStatus(eq_sts);

				m_Glass->SetLotInit();

				m_dStartSecond = clock();
				SetSeqIdx(prc_idx, seq_idx, 245);
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 210);
			}
			break;
		case 210:
			//Lift pin up and mechnical align
			m_strMainMsg.Format(_T("Lift Pin Up Start"));
			WriteLog(MAIN_LOG, m_strMainMsg);
			m_Rslt.m_dMotPos[MOTOR_3] =  m_RecipePanel->GetPanelLoadPosUP();
			m_Robot->SetRobotMove_mm(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]);
			SetSeqIdx(prc_idx, seq_idx, 220);
			break;
		case 220:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "LiftPinAxis Move Time Over Error=AX04 : UP/DN 축동작 타임아웃(축 동작 실패)!-220-");
			if(m_Robot->GetIsMoveComplete(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]))
			{
				if(m_Robot->GetIsInposition(MOTOR_3))
				{
					m_strMainMsg.Format(_T("Lift Pin Up"));
					WriteLog(MAIN_LOG, m_strMainMsg);
					//m_bDefectFileWriteBreak = TRUE;
					if(!m_bMachaAlignRepet)
					{
						SetSeqIdx(prc_idx, seq_idx, 225);
					}
					else
					{
						SetSeqIdx(prc_idx, seq_idx, 250);
					}
				}
			}
			break;
		case 225:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PIN GLS Front Detect Error=X550,51 : GLASS DETECT SENSOR TIMEOUT(GLASS 감지센서 이상, 모두 감지되거나 모두 감지 않되어야함, 현재 Loader 사용상태)!-225-");	
			BOOL bGlassSensor;
			//Detect sensor power on
			m_IO->SetWriteDO(DO_GLASS_DETECT_SENSOR_RELAY_ON_OFF, TRUE);

			if( (m_IO->GetReadDI(DI_PIN_GLS_DETECT_FRONT)) ||
				(m_IO->GetReadDI(DI_PIN_GLS_DETECT_REAR)) )
			{
				bGlassSensor = TRUE;
			}
			else
			{
				bGlassSensor = FALSE;
			}

			if(m_Flag.m_bGlassExist == bGlassSensor)
			{
				SetSeqIdx(prc_idx, seq_idx, 230);
			}
			break;
		//대기시작////////////////////////////////////////////////////////////////////////////////////////
		case 230:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 232);
			}
			break;
		case 232:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_DECISION;//ms
			glass_rslt = m_Inline->GlassDataComp(&m_Flag.m_bGlassExist);
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Glass Data 비교 실패=D0%d!-232-", glass_rslt - 2);

			if(m_GlassManu->GetLoaderUse())//Loader 사용유무
			{
				if(glass_rslt > 1)
				{
					m_strMainMsg.Format(_T("%S"), m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus);
					WriteLog(MAIN_LOG, m_strMainMsg);
				}

				//Glass Data 비교
				if(2 > glass_rslt)
				{
					SetSeqIdx(prc_idx, seq_idx, 235);
				}
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 235);
			}
			break;
		case 235:
			m_Rslt.m_bCompleteLoading = FALSE;
			
			eq_sts = m_Inline->GetStatusReport();
			if(m_GlassManu->GetLoaderUse())//Loader 사용유무
			{
				eq_sts.bGlassExistInUnit = m_Flag.m_bGlassExist;	    //Glass유무
				eq_sts.bGlassInProcess = FALSE;							//검사중인지
				eq_sts.bIonizer = TRUE;
				eq_sts.bUnitAutoMode = TRUE;							//자동반출 가능한지			
				eq_sts.nCurrenRecipeNum = m_Recipe->GetRecipeNo() + 1;
				if(m_Flag.m_bGlassExist)
				{
					eq_sts.nGlassCountInUnit = 1;
					eq_sts.nPutInfoPossibleCount = 0;
					eq_sts.nStatusRunIdleDown = 0;
				}
				else
				{
					eq_sts.nGlassCountInUnit = 0;
					eq_sts.nPutInfoPossibleCount = 1;
					eq_sts.nStatusRunIdleDown = 1;
				}

				//20160204 ngh
			
				eq_sts.nStatusRunIdleDown = !m_Flag.m_bGlassExist;		//0 : RUN~
				eq_sts.nUnitOpeationMode = 0;							//1 : Manual, 0 : auto
				m_Inline->SetStatus(eq_sts);

				SetSeqIdx(prc_idx, seq_idx, 240);

				m_Glass->SetLotInit();

				//20160711 ngh 스캔 후 대기
				if(m_bScanAfterWait && m_Flag.m_bGlassExist)
				{
					//m_strMainMsg.Format(_T(""));
					//WriteLog(MAIN_LOG, m_strMainMsg);
					m_Rslt.m_bWaitLoading = FALSE;
					m_Rslt.m_bCassetEmpty = FALSE;

					m_dStartSecond = clock();
					SetSeqIdx(prc_idx, seq_idx, 245);
				}
				else
				{
					//Loading 시작
					if(m_Flag.m_bGlassExist)
					{
#ifdef DEFECT
						m_Dfserver->DFServerClose();
#endif
						m_strMainMsg.Format(_T("Unloading or Loading 시작"));
						WriteLog(MAIN_LOG, m_strMainMsg);
						m_Inline->SetGlassLdUldEx(EXCHANGE);//
						m_Rslt.m_bWaitLoading = TRUE;
						m_Rslt.m_bCassetEmpty = FALSE;
					}
					else
					{
						m_strMainMsg.Format(_T("Loading 시작"));
						WriteLog(MAIN_LOG, m_strMainMsg);
						//Only loading
						m_Inline->SetGlassLdUldEx(LOAD_ONLY);
						m_Rslt.m_bWaitLoading = TRUE;
						m_Flag.m_bGlassExist = TRUE;
					}
				}
			}
			else
			{
				eq_sts.bUnitAutoMode = FALSE;							//자동반출 가능한지
				eq_sts.nCurrenRecipeNum = m_Recipe->GetRecipeNo() + 1;
				if(m_Flag.m_bGlassExist)
				{
					eq_sts.nGlassCountInUnit = 1;
					eq_sts.nPutInfoPossibleCount = 0;
					eq_sts.nStatusRunIdleDown = 0;
				}
				else
				{
					eq_sts.nGlassCountInUnit = 0;
					eq_sts.nPutInfoPossibleCount = 1;
					eq_sts.nStatusRunIdleDown = 1;
				}

				//20160204 ngh
			
				eq_sts.nStatusRunIdleDown = !m_Flag.m_bGlassExist;		//0 : RUN~
				eq_sts.nUnitOpeationMode = 1;							//1 : Manual, 0 : auto
				m_Inline->SetStatus(eq_sts);

				//count_seconds(&m_StartSecond);
				m_dStartSecond = clock();

				SetSeqIdx(prc_idx, seq_idx, 250);
			}
			break;
		case 240:
			//////////////////////////////////////////////////////////////////////////
			//Loading complete대기////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			//m_Rslt.m_bWaitLoading
			if(m_GlassManu->GetLoaderUse())//Loader 사용유무
			{
				if(m_Rslt.m_bWaitLoading)
				{
					m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = 0;
				}
				else
				{
					m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_ECS;//ms
					sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "%S", m_Inline->GetErrorString());
					//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ESC : LOADER ECS TIMEOUT(Loader 동작 이상 : %s)!-240-", m_Rslt.m_sLoaderUnloaderErrMsg);	
				}

				if(m_Rslt.m_bCompleteLoading)
				{
					m_dEndSecond = clock();
					//count_seconds(&m_EndSecond);

					if(m_Flag.m_bGlassExist)
					{
						m_dEndSecond = clock();
						p_time = (m_dEndSecond - m_dStartSecond) / 1000;
						/*m_EndSecond = m_EndSecond - m_StartSecond;
						m_lProcessTime = (long)(((double)m_EndSecond[0]) * 1000);*/
						strLog.Format(_T("Auto Process Time : %.3f sec"), p_time);
						WriteLog(MAIN_LOG, strLog);
					}

					//Unloading only
					if(m_Rslt.m_bCassetEmpty)
					{
						m_strMainMsg.Format(_T("UnLoading 완료"));
						WriteLog(MAIN_LOG, m_strMainMsg);
						ResetRsltSizeCntView();
						//다시 시작해서 대기
						SetSeqIdx(prc_idx, seq_idx, 242);
						m_Flag.m_bGlassExist = TRUE;
						//3) Glass없는것 확인해야한다.//////////////////////////////////////////////////
						//그리고 m_Flag.exsit를 Fasle한다
					}
					//Loading(loading only or exchange loading)
					else
					{
						m_strMainMsg.Format(_T("Loading 완료"));
						WriteLog(MAIN_LOG, m_strMainMsg);

						//검사시작
						SetSeqIdx(prc_idx, seq_idx, 250);

						//Glass data
						glass_data = m_Inline->GetUnloaderGlassData();//Unloader것을 사용해야한다.
					}
					m_Rslt.m_bWaitLoading = FALSE;
					m_Rslt.m_bCassetEmpty = FALSE;

					m_dStartSecond = clock();
					//count_seconds(&m_StartSecond);
				}
				//20160223 ngh 임시
				else if(!m_Rslt.m_bCompleteLoading && (m_Inline->GetInterLockError() >= 300))
				{
					m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_ECS;//ms
					sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "%S", m_Inline->GetErrorString());
				}
			}
			else
			{
				m_Rslt.m_bWaitLoading = FALSE;

				if(m_Rslt.m_bStartManualInspect)
				{
					SetSeqIdx(prc_idx, seq_idx, 245);
				}
			}
			break;
		//20160405 ngh
		//Unloading 시 Glass 없는 상태 확인
		case 242:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PIN GLS Front Detect Error=X550,51 : GLASS DETECT SENSOR TIMEOUT(GLASS 감지센서 이상, 모두 감지되거나 모두 감지 않되어야함, 현재 Loader 사용상태)!-242-");	
			//Detect sensor power on
			m_IO->SetWriteDO(DO_GLASS_DETECT_SENSOR_RELAY_ON_OFF, TRUE);

			if( (!m_IO->GetReadDI(DI_PIN_GLS_DETECT_FRONT)) &&
				(!m_IO->GetReadDI(DI_PIN_GLS_DETECT_REAR)) )
			{
				SetSeqIdx(prc_idx, seq_idx, 100);
			}
			break;
		//20160405 ngh
		//Exchange or Load Only 시 Glass 있는 상태 확인
		case 245:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PIN GLS Front Detect Error=X550,51 : GLASS DETECT SENSOR TIMEOUT(GLASS 감지센서 이상, 모두 감지되거나 모두 감지 않되어야함, 현재 Loader 사용상태)!-245-");	
			//Detect sensor power on
			m_IO->SetWriteDO(DO_GLASS_DETECT_SENSOR_RELAY_ON_OFF, TRUE);

			if( (m_IO->GetReadDI(DI_PIN_GLS_DETECT_FRONT)) ||
				(m_IO->GetReadDI(DI_PIN_GLS_DETECT_REAR)) )
			{
				SetSeqIdx(prc_idx, seq_idx, 250);
			}
			break;
		//1) Glass data 있는지 비교해야한다//////////////////////////////////////////////////////////////////////////////
		//2) Exist 확인해야한다//////////////////////////////////////////////////////////////////////////////////////////
		case 247:
#ifdef DEFECT
			m_Dfserver->InitDefectCnt();
			AfxExtractSubString(temp, m_Dfserver->GetDFFilePath(), 8, '\\');
			m_Dfserver->DFServerConnect(m_Dfserver->GetDFServerIP(), m_Dfserver->GetDFServerID(), 
				m_Dfserver->GetDFServerPW(), m_Dfserver->GetDFFilePath().Mid(0, m_Dfserver->GetDFFilePath().GetLength() - temp.GetLength() - 1),
				m_Dfserver->GetDFServerRemotePath(), 0);
			m_bDfIdxFile = FALSE;
			/*if(m_GlassManu->GetLoaderUse() && m_Dfserver->GetWriteOnCheck()) //m_Dfserver->GetWriteOnCheck())
			{
				m_bDfIdxFile = m_Dfserver->DefectFileUpdate();
			}*/
#endif
			break;
		case 250:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				eq_sts = m_Inline->GetStatusReport();
				eq_sts.bGlassInProcess = TRUE;	
				m_Inline->SetStatus(eq_sts);
				m_bDefectFileWriteComp[0] = FALSE;
				m_bDefectFileWriteComp[1] = FALSE;
				m_bDefectFileWriteComp[2] = FALSE;
				m_bDefectFileWriteComp[3] = FALSE;
				m_bDefectFileWriteComp[4] = FALSE;
				m_bDefectFileWriteBreak = FALSE;
				m_bDefectFileLoadStart = FALSE;


				//20160811 ngh
				//////////////////////////////////////////////////////////////////
				temp.Format(_T(""));
				temp.Format(_T("%S"), m_Inline->GetUnloaderGlassData().strGlassID);
				AfxExtractSubString(glass_id, temp, 0, ' ');

				temp.Format(_T(""));
				temp.Format(_T("%S"), m_Inline->GetUnloaderGlassData().strLotID);
				AfxExtractSubString(lot_id, temp, 0, ' ');

#ifdef MACHINE
				m_strResultFilePath.Format(_T("\\\\%s\\Result\\%04d%02d%02d\\%s\\%s\\"), m_IPSCtrl->GetIPSIP(), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id);
#else
				m_strResultFilePath.Format(_T("D:\\Result\\%04d%02d%02d\\%s\\%s\\"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id);
#endif
				//////////////////////////////////////////////////////////////////
				SetSeqIdx(prc_idx, seq_idx, 260);
			}
			break;
		//Pin 다운 후 Mechnical align 시작////////////////////////////////////////////////////////////////////////////
		case 260:
			//Loader pork sensor도 체크
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ROBOT HAND Left Detect Error=X53C or X03D : ROBOT HAND DETECT TIMEOUT(로보트 핸드 센서 감지않됨-핸드가 있으면 감지 않된다-)!-260-");	
			if( (m_IO->GetReadDI(DI_ROBOT_HAND_DETECT_LEFT)) && (m_IO->GetReadDI(DI_ROBOT_HAND_DETECT_RIGHT)) )
			//if(1)//20160218
			{
				//m_strMainMsg.Format(_T("Lift Pin Down Start"));
				//WriteLog(MAIN_LOG, m_strMainMsg);
				//Lift pin down and mechnical align
				//m_Rslt.m_dMotPos[MOTOR_3] =  m_RecipePanel->GetPanelLoadPosDN();
				//m_Robot->SetRobotMove_mm(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]);
				SetSeqIdx(prc_idx, seq_idx, 265);
			}
			break;
		case 265:
			//Macha aling
			SetSeqIdx(prc_idx, seq_idx, 2000);
			m_strMainMsg.Format(_T("Loading Comp Macha Align Start"));
			WriteLog(MAIN_LOG, m_strMainMsg);
			m_Rslt.m_iMechAlignSeq = 300;
			break;
		//case 270:
		//	m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
		//	sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "LiftPinAxis Move Time Over Error=AX04 : UP/DN 축동작 타임아웃(축 동작 실패)!-270-");
		//	if(m_Robot->GetIsMoveComplete(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]))
		//	{
		//		if(m_Robot->GetIsInposition(MOTOR_3))
		//		{
		//			m_strMainMsg.Format(_T("Lift Pin Down"));
		//			WriteLog(MAIN_LOG, m_strMainMsg);
		//			SetSeqIdx(prc_idx, seq_idx, 280);
		//		}
		//	}
		//	break;
		//case 280:
		//	//Mech align forwarding
		//	SetSeqIdx(prc_idx, seq_idx, 1400);
		//	m_strMainMsg.Format(_T("Mech Align Forwarding"));
		//	WriteLog(MAIN_LOG, m_strMainMsg);
		//	m_Rslt.m_iMechAlignSeq = 300;
		//	break;
		case 300:
			//Vac check
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Chuck VAC Error=X50C : STAGE VACCUM PRESSURE TIMEOUT(진공센서 감지않됨)!-300-");	
			if(1)//m_IO->GetReadDI(DI_CHUCK_VACUUM_ON_DETECT)) 임시 20160729 sc
			{
				SetSeqIdx(prc_idx, seq_idx, 310);
			}
			break;
		//20160405 ngh
		//클램프 후 glass감지(edge sensor)
		//case 305:
		//	m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
		//	sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Chuck VAC Error=X50C : STAGE VACCUM PRESSURE TIMEOUT(진공센서 감지않됨)!-305-");	
		//	if(m_IO->GetReadDI(DI_CHUCK_VACUUM_ON_DETECT))
		//	{
		//		SetSeqIdx(prc_idx, seq_idx, 310);
		//	}
		//	break;
		case 310:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 340);
				//SetSeqIdx(prc_idx, seq_idx, 100);//임시 20160218
			}
			break;
		case 340:
			//Move align
			m_Rslt.m_dMotPos[MOTOR_0] =  m_Align->GetAlignReadyPosY();
			m_Robot->SetRobotMove_mm(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]);
			m_Rslt.m_dMotPos[MOTOR_2] =  m_Align->GetAlignReadyPosT();
			m_Robot->SetRobotMove_mm(MOTOR_2, m_Rslt.m_dMotPos[MOTOR_2]);

			m_strMainMsg.Format(_T("ScanAxis, RotatingAxis Move Start(Vision Align)"));
			WriteLog(MAIN_LOG, m_strMainMsg);

			SetSeqIdx(prc_idx, seq_idx, 350);
			break;
		case 350:
			//Move end(scan)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ScanAxis Move Time Over Error=AX01 : SCAN 축동작 타임아웃(축 동작 실패)!-350-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]))
			{
				if(m_Robot->GetIsInposition(MOTOR_0))
				{
					m_strMainMsg.Format(_T("ScanAxis Move(Vision Align)"));
					WriteLog(MAIN_LOG, m_strMainMsg);
					SetSeqIdx(prc_idx, seq_idx, 360);
				}
			}
			break;
		case 360:
			//Move end(theta)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "RotatingAxis Move Time Over Error=AX03 : THETA 축동작 타임아웃(축 동작 실패)!-360-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_2, m_Rslt.m_dMotPos[MOTOR_2]))
			{
				if(m_Robot->GetIsInposition(MOTOR_2))
				{
					m_strMainMsg.Format(_T("RotatingAxis Move(Vision Align)"));
					WriteLog(MAIN_LOG, m_strMainMsg);
					SetSeqIdx(prc_idx, seq_idx, 370);

					//Align light on
					m_Light->SetAlignLight(0, m_Align->GetLight(0));
					m_Light->SetAlignLight(1, m_Align->GetLight(1));
				}
			}
			break;
		case 370:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 380);
			}	
			break;
		case 380:
			//Align grab
			m_strMainMsg.Format(_T("Align Grab(Vision Align)"));
			WriteLog(MAIN_LOG, m_strMainMsg);
	 		m_Align->SetGrab();
			SetSeqIdx(prc_idx, seq_idx, 390);
			break;
		case 390:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 400);
			}	
			break;
		case 400:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_DECISION;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Align Camera Mark Search Fail=ALIGN : ALIGN 검사실패!-400-");
			//Align process
	 		if(0 == m_Align->SetFind())
			{
				m_strMainMsg.Format(_T("Align Mark Search(Vision Align)"));
				WriteLog(MAIN_LOG, m_strMainMsg);
				SetSeqIdx(prc_idx, seq_idx, 410);
			}
			break;
		case 410:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 2000);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 420);
			}	
			break;
		case 420:
			//Move after align
			m_Rslt.m_dMotPos[MOTOR_0] =  m_Align->GetAlignAfterPosY();
			m_Robot->SetRobotMove_mm(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]);
			m_Rslt.m_dMotPos[MOTOR_1] =  m_Align->GetAlignAfterPosX();
			m_Robot->SetRobotMove_mm(MOTOR_1, m_Rslt.m_dMotPos[MOTOR_1]);
			m_Rslt.m_dMotPos[MOTOR_2] =  m_Align->GetAlignAfterPosT();
			m_Robot->SetRobotMove_mm(MOTOR_2, m_Rslt.m_dMotPos[MOTOR_2]);

			m_strMainMsg.Format(_T("ScanAxis, ShiftAxis, RotatingAxis Move After Align Start(Vision Align)"));
			WriteLog(MAIN_LOG, m_strMainMsg);
			SetSeqIdx(prc_idx, seq_idx, 430);
			break;
		case 430:
			//Move end(scan)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ScanAxis Move Time Over Error=AX01 : SCAN 축동작 타임아웃(축 동작 실패)!-430-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]))
			{
				if(m_Robot->GetIsInposition(MOTOR_0))
				{
					m_strMainMsg.Format(_T("ScanAxis Move After Align(Vision Align)"));
					WriteLog(MAIN_LOG, m_strMainMsg);
					SetSeqIdx(prc_idx, seq_idx, 440);
				}
			}
			break;	

		case 440:
			//Move end(shift)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ShiftAxis Move Time Over Error=AX02 : SHIFT 축동작 타임아웃(축 동작 실패)!-440-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_1, m_Rslt.m_dMotPos[MOTOR_1]))
			{
				if(m_Robot->GetIsInposition(MOTOR_1))
				{
					m_strMainMsg.Format(_T("ShiftAxis Move After Align(Vision Align)"));
					WriteLog(MAIN_LOG, m_strMainMsg);
					SetSeqIdx(prc_idx, seq_idx, 450);
				}
			}
			break;	
		case 450:
			//Move end(theta)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "RotatingAxis Move Time Over Error=AX03 : THETA 축동작 타임아웃(축 동작 실패)!-450-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_2, m_Rslt.m_dMotPos[MOTOR_2]))
			{
				m_strMainMsg.Format(_T("RotatingAxis Move After Align(Vision Align)"));
				
				if(m_Robot->GetIsInposition(MOTOR_2))
				{

					SetSeqIdx(prc_idx, seq_idx, 451);
					//Scan
					//Move offset move
					m_Rslt.m_dMotPos[MOTOR_0] =  m_Align->GetAlignOffsetPosX_coordXY();
					m_Robot->SetRobotMove_mm(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]);
					m_Rslt.m_dMotPos[MOTOR_1] =  m_Align->GetAlignOffsetPosY_coordXY();
					m_Robot->SetRobotMove_mm(MOTOR_1, m_Rslt.m_dMotPos[MOTOR_1]);

					m_Rslt.m_dMotPos[MOTOR_2] =  m_Align->GetAlignOffsetPosT();
					m_Robot->SetRobotMove_mm(MOTOR_2, m_Rslt.m_dMotPos[MOTOR_2]);

					m_strMainMsg.Format(_T("ScanAxis, ShiftAxis, RotatingAxis Move Offset Move Start"));
					WriteLog(MAIN_LOG, m_strMainMsg);
				}
			}
			break;

		case 451:
			//Move end(scan)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ScanAxis Move Time Over Error=AX01 : SCAN 축동작 타임아웃(축 동작 실패)!-451-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]))
			{
				if(m_Robot->GetIsInposition(MOTOR_0))
				{
					m_strMainMsg.Format(_T("ScanAxis Move Offset Move"));
					WriteLog(MAIN_LOG, m_strMainMsg);
					SetSeqIdx(prc_idx, seq_idx, 452);
				}
			}
			break;	
		case 452:
			//Move end(shift)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ShiftAxis Move Time Over Error=AX02 : SHIFT 축동작 타임아웃(축 동작 실패)!-452-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_1, m_Rslt.m_dMotPos[MOTOR_1]))
			{
				if(m_Robot->GetIsInposition(MOTOR_1))
				{
					m_strMainMsg.Format(_T("ShiftAxis Move Offset Move"));
					WriteLog(MAIN_LOG, m_strMainMsg);
					SetSeqIdx(prc_idx, seq_idx, 453);
				}
			}
			break;	
		case 453:
			//Move end(theta)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "RotatingAxis Move Time Over Error=AX03 : THETA 축동작 타임아웃(축 동작 실패)!-453-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_2, m_Rslt.m_dMotPos[MOTOR_2]))
			{
				if(m_Robot->GetIsInposition(MOTOR_2))
				{
					m_strMainMsg.Format(_T("RotatingAxis Move Offset Move"));
					WriteLog(MAIN_LOG, m_strMainMsg);
					SetSeqIdx(prc_idx, seq_idx, 454);

					m_Review->SetGrab();
					m_Review->DrawCeterLine();
				}
			}
			break;
		case 454:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 2000);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 460);
			}	
			break;

		//Start//////////////////////////////////////////////////////////////////////////////////////////
		//
		//
		case 460:
			//Check start condition
			if(m_bMachaAlignRepet)
			{
				m_strMainMsg.Format(_T("Macha Align Repet"));
				WriteLog(MAIN_LOG, m_strMainMsg);
				//m_Rslt.m_iMechAlignSeq = 100;
				SetSeqIdx(prc_idx, seq_idx, 100);
				break;
			}
			
			if(m_Rslt.m_iScanTypeIdx < RECIPE_SCAN_TYPE_MAX_NUM)
			{
				if(m_RecipeScan->GetInspectUse(m_Rslt.m_iScanTypeIdx))
				{
					m_Rslt.m_bInspectionMove = TRUE;

					//Lignt On
					switch(m_Rslt.m_iScanTypeIdx)
					{
					//8K 0
					case 0:
						m_Light->Set8KLight(m_RecipeScan->GetLight(m_Rslt.m_iScanTypeIdx));
						m_Light->Set16KLight(0);

						m_Rslt.m_ScanCamIdx = 0;
						m_strMainMsg.Format(_T("8K Inspection Degree 0"));
						m_btn_scan_0.SetValue(TRUE);
						m_btn_scan_1.SetValue(FALSE);
						m_btn_scan_2.SetValue(FALSE);
						m_btn_scan_3.SetValue(FALSE);
						m_btn_scan_4.SetValue(FALSE);
						break;
					//8K 90
					case 1:
						m_Light->Set8KLight(m_RecipeScan->GetLight(m_Rslt.m_iScanTypeIdx));
						m_Light->Set16KLight(0);

						m_Rslt.m_ScanCamIdx = 0;
						m_strMainMsg.Format(_T("8K Inspection Degree 90"));
						m_btn_scan_0.SetValue(FALSE);
						m_btn_scan_1.SetValue(TRUE);
						m_btn_scan_2.SetValue(FALSE);
						m_btn_scan_3.SetValue(FALSE);
						m_btn_scan_4.SetValue(FALSE);
						break;
					//16K 0 or 90
					case 2:
						m_Light->Set8KLight(0);
						m_Light->Set16KLight(m_RecipeScan->GetLight(m_Rslt.m_iScanTypeIdx));

						m_Rslt.m_ScanCamIdx = 1;
						if(m_RecipeScan->GetInspect90Degree(m_Rslt.m_iScanTypeIdx))
						{
							m_strMainMsg.Format(_T("16K CVD Inspection Degree 90"));
						}
						else
						{
							m_strMainMsg.Format(_T("16K CVD Inspection Degree 0"));
						}
						m_btn_scan_0.SetValue(FALSE);
						m_btn_scan_1.SetValue(FALSE);
						m_btn_scan_2.SetValue(TRUE);
						m_btn_scan_3.SetValue(FALSE);
						m_btn_scan_4.SetValue(FALSE);
						break;
					case 3:
						m_Light->Set8KLight(0);
						m_Light->Set16KLight(m_RecipeScan->GetLight(m_Rslt.m_iScanTypeIdx));

						m_Rslt.m_ScanCamIdx = 1;
						m_strMainMsg.Format(_T("16K PCL Inspection Degree 90"));
						m_btn_scan_0.SetValue(FALSE);
						m_btn_scan_1.SetValue(FALSE);
						m_btn_scan_2.SetValue(FALSE);
						m_btn_scan_3.SetValue(TRUE);
						m_btn_scan_4.SetValue(FALSE);
						break;
					case 4:
						m_Light->Set8KLight(0);
						m_Light->Set16KLight(m_RecipeScan->GetLight(m_Rslt.m_iScanTypeIdx));

						m_Rslt.m_ScanCamIdx = 1;
						m_strMainMsg.Format(_T("16K PCL Inspection Degree 0"));
						m_btn_scan_0.SetValue(FALSE);
						m_btn_scan_1.SetValue(FALSE);
						m_btn_scan_2.SetValue(FALSE);
						m_btn_scan_3.SetValue(FALSE);
						m_btn_scan_4.SetValue(TRUE);
						break;
					default:
						AfxMessageBox(_T("Error : invalid index!(CEMIControlDlg::MotionProcess(case 460))"));
						break;
					}
					WriteLog(MAIN_LOG, m_strMainMsg);
					m_strInsType.Format(_T("%s"), m_strMainMsg);
					m_dInspectionStartSecond[m_Rslt.m_iScanTypeIdx] = clock();
					//count_seconds(&m_InspectionStartSecond[m_Rslt.m_iScanTypeIdx]);
					m_Trigger->SetCamTriggerRcipe(m_Rslt.m_ScanCamIdx);
					Sleep(2000);
					//m_Trigger->SetCamTriggerRcipe(m_Rslt.m_ScanCamIdx);  // pkh 20160805

					//Scan
					//Move offset move
					m_Rslt.m_dMotPos[MOTOR_0] =  m_Align->GetAlignOffsetPosX_coordXY();
					m_Robot->SetRobotMove_mm(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]);
					m_Rslt.m_dMotPos[MOTOR_1] =  m_Align->GetAlignOffsetPosY_coordXY();
					m_Robot->SetRobotMove_mm(MOTOR_1, m_Rslt.m_dMotPos[MOTOR_1]);

					m_Rslt.m_dMotPos[MOTOR_2] =  m_Align->GetAlignOffsetPosT() + m_RecipeScan->GetRoatateOffset(m_Rslt.m_iScanTypeIdx);
					m_Robot->SetRobotMove_mm(MOTOR_2, m_Rslt.m_dMotPos[MOTOR_2]);
					m_bDefectFileSave[m_Rslt.m_iScanTypeIdx] = FALSE;

					SetSeqIdx(prc_idx, seq_idx, 461);
				}
				else
				{
					m_bDefectFileSave[m_Rslt.m_iScanTypeIdx] = TRUE;
					m_Rslt.m_iScanTypeIdx++;
					SetSeqIdx(prc_idx, seq_idx, 460);
				}
			}
			else
			{
				//Scan종료////////////////////////////////////////////////////////////////////////////

				//Go to Film inspection///////////////////////////////////////////////////////////////
				//SetSeqIdx(prc_idx, seq_idx, 800);
				//ClickBtnenhctrlMainLoadDefectMap();

				m_Rslt.m_bInspectionMove = FALSE;
				SetSeqIdx(prc_idx, seq_idx, 750);
			}

			break;
		case 461:
			m_strMainMsg.Format(_T("Ez Move Start(%s)"), m_strInsType);
			WriteLog(MAIN_LOG, m_strMainMsg);
			m_dlgAFCtrl->SetEzSpeed(60000);
			switch(m_Rslt.m_iScanTypeIdx)
			{
			case 0:
			case 1:
				m_dlgAFCtrl->SetEziMove_mm(0);
				break;
			case 2:
			case 3:
			case 4:
				m_dlgAFCtrl->SetEziMove_mm(m_RecipeScan->GetEzPos(m_Rslt.m_iScanTypeIdx));
				break;
			}
			
			//m_dlgAFCtrl->SetEziMove_mm(m_RecipeScan->GetEzPos(m_Rslt.m_iScanTypeIdx));
			SetSeqIdx(prc_idx, seq_idx, 462);
			//m_dlgAFCtrl;
			break;
		case 462:
			//Move end(ez)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Ez Move Time Over Error=AX05 : SCAN 축동작 타임아웃(축 동작 실패)!-462-");
			
			if(m_dlgAFCtrl->GetEzPosComplete(m_Rslt.m_iScanTypeIdx))
			{
				m_strMainMsg.Format(_T("Ez Move(%s)"), m_strInsType);
				WriteLog(MAIN_LOG, m_strMainMsg);
				SetSeqIdx(prc_idx, seq_idx, 470);
			}
			break;
		case 470:
			//Move end(scan)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ScanAxis Move Time Over Error=AX01 : SCAN 축동작 타임아웃(축 동작 실패)!-470-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]))
			{
				if(m_Robot->GetIsInposition(MOTOR_0))
				{
					m_strMainMsg.Format(_T("ScanAxis Move(%s)"), m_strInsType);
					WriteLog(MAIN_LOG, m_strMainMsg);
					SetSeqIdx(prc_idx, seq_idx, 480);
				}
			}
			break;	
		case 480:
			//Move end(shift)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ShiftAxis Move Time Over Error=AX02 : SHIFT 축동작 타임아웃(축 동작 실패)!-480-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_1, m_Rslt.m_dMotPos[MOTOR_1]))
			{
				if(m_Robot->GetIsInposition(MOTOR_1))
				{
					m_strMainMsg.Format(_T("ShiftAxis Move(%s)"), m_strInsType);
					WriteLog(MAIN_LOG, m_strMainMsg);
					SetSeqIdx(prc_idx, seq_idx, 490);
				}
			}
			break;	
		case 490:
			//Move end(theta)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "RotatingAxis Move Time Over Error=AX03 : THETA 축동작 타임아웃(축 동작 실패)!-490-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_2, m_Rslt.m_dMotPos[MOTOR_2]))
			{
				if(m_Robot->GetIsInposition(MOTOR_2))
				{
					m_strMainMsg.Format(_T("RotatingAxis Move(%s)"), m_strInsType);
					WriteLog(MAIN_LOG, m_strMainMsg);
					SetSeqIdx(prc_idx, seq_idx, 500);
				}
			}
			break;
		case 500:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 510);
			}	
			break;
		case 510:
			m_strMainMsg.Format(_T("ScanAxis Move Start(%s)"), m_strInsType);
			WriteLog(MAIN_LOG, m_strMainMsg);
			m_Rslt.m_dMotPos[MOTOR_0] =  m_Align->GetAlignOffsetPosX_coordXY() - m_RecipeScan->GetScanStartX_coordXY(m_Rslt.m_iScanTypeIdx);
			m_Robot->SetRobotMove_mm(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]);
			SetSeqIdx(prc_idx, seq_idx, 520);
			break;
		case 520:
			//Move end(scan)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ScanAxis Move Time Over Error=AX01 : SCAN 축동작 타임아웃(축 동작 실패)!-520-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]))
			{
				if(m_Robot->GetIsInposition(MOTOR_0))
				{
					m_strMainMsg.Format(_T("ScanAxis Move(%s)"), m_strInsType);
					WriteLog(MAIN_LOG, m_strMainMsg);
					SetSeqIdx(prc_idx, seq_idx, 530);
				}
			}
			break;
		case 530:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				m_strMainMsg.Format(_T("ScanAxis Move End(%s)"), m_strInsType);
				WriteLog(MAIN_LOG, m_strMainMsg);
				SetSeqIdx(prc_idx, seq_idx, 600);
				//SetSeqIdx(prc_idx, seq_idx, 800);
			}	
			break;


		//8K scan동작 시작/////////////////////////////////////////////////////////////
		//Scan시작///////////////////////////////////
		case 600:
			//재초기화 필요
			m_Rslt.m_iScanIdxX = 0;
			m_Rslt.m_iScanIdxY = 0;
			m_Rslt.m_iScanIdx = 0;

			//Start packet send
			m_vision_grab_ready = FALSE;
			m_vision_grab_comp = FALSE;
			if(m_Rslt.m_ScanCamIdx == 1)
			{
				scan_height = (m_RecipeScan->GetScanLeng_coordXY(m_Rslt.m_iScanTypeIdx) * 1000) / 5;
			}
			else
			{
				scan_height = (m_RecipeScan->GetScanLeng_coordXY(m_Rslt.m_iScanTypeIdx) * 1000) / 12;
			}

			if(scan_height % 1000)
			{
				scan_height -= (scan_height % 1000);
			}
			m_Trigger->SetOPMode(TRUE);
			Sleep(1000);
			m_Trigger->SetResetEncoderCnt();
			Sleep(1000);
			m_Trigger->SetDirection(TRUE);
			Sleep(1000);
			PacketSendIPS(GetGrabReadyPacket(m_Rslt.m_ScanCamIdx, m_RecipeScan->GetInspect90Degree(m_Rslt.m_iScanTypeIdx), m_Rslt.m_iScanTypeIdx, m_Rslt.m_iScanIdx, FALSE, scan_height));
			m_strMainMsg.Format(_T("No.%d Scan Start(%s)"), m_Rslt.m_iScanIdx + 1, m_strInsType);
			WriteLog(MAIN_LOG, m_strMainMsg);
			SetSeqIdx(prc_idx, seq_idx, 610);
			break;
		case 610:
			//Ready receive
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_IPS_TCPIP_RCV;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "IPS Inspection End Time Over Error=IPS : SCAN READY 수신 타임아웃(IPS통신 수신실패)!-610-");

			if(m_vision_grab_ready)
			{
				Sleep(2000);
				//시작위치
				m_strMainMsg.Format(_T("No.%d Scan Shift Axis Move Start(%s)"), m_Rslt.m_iScanIdx + 1, m_strInsType);
				WriteLog(MAIN_LOG, m_strMainMsg);
				m_Rslt.m_dMotPos[MOTOR_1] = (m_Align->GetAlignOffsetPosY_coordXY() + m_RecipeScan->GetScanStartY_coordXY(m_Rslt.m_iScanTypeIdx)) + (m_RecipeScan->GetScanPitch_coordXY(m_Rslt.m_iScanTypeIdx) * m_Rslt.m_iScanIdxY);

				m_Robot->SetRobotMove_mm(MOTOR_1,m_Rslt.m_dMotPos[MOTOR_1],spd);

				SetSeqIdx(prc_idx, seq_idx, 620);
			}
			break;
		case 620:
			//Move(shift)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ShiftAxis Move Time Over Error=AX02 : SHIFT 축동작 타임아웃(축 동작 실패)!-620-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_1, m_Rslt.m_dMotPos[MOTOR_1]))
			{
				if(m_Robot->GetIsInposition(MOTOR_1))
				{
					m_strMainMsg.Format(_T("No.%d Scan Shift Axis Move(%s)"), m_Rslt.m_iScanIdx + 1, m_strInsType);
					WriteLog(MAIN_LOG, m_strMainMsg);
					SetSeqIdx(prc_idx, seq_idx, 625);
				}
			}
			break;	
		case 625:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				m_strMainMsg.Format(_T("No.%d Scan Shift Axis Move End(%s)"), m_Rslt.m_iScanIdx + 1, m_strInsType);
				WriteLog(MAIN_LOG, m_strMainMsg);
				SetSeqIdx(prc_idx, seq_idx, 630);
			}	
			break;
		case 630:
			/*
			if(0 == (((int)m_Rslt.m_iScanIdxX) % 2))
			{
				m_Rslt.m_dMotPos[MOTOR_0] =  m_Align->GetAlignOffsetPosX_coordXY() - m_RecipeScan->GetScanStartX_coordXY(m_Rslt.m_iScanTypeIdx);
			}
			else
			{
				m_Rslt.m_dMotPos[MOTOR_0] =  (m_Align->GetAlignOffsetPosX_coordXY() - m_RecipeScan->GetScanStartX_coordXY(m_Rslt.m_iScanTypeIdx)) - m_RecipeScan->GetScanLeng_coordXY(m_Rslt.m_iScanTypeIdx);
			}
			*/
			m_strMainMsg.Format(_T("No.%d Scan Grab Move Start(%s)"), m_Rslt.m_iScanIdx + 1, m_strInsType);
			WriteLog(MAIN_LOG, m_strMainMsg);

			if(0 == (int)m_Rslt.m_iScanIdxX)
			{
				m_Rslt.m_dMotPos[MOTOR_0] =  m_Align->GetAlignOffsetPosX_coordXY() - m_RecipeScan->GetScanStartX_coordXY(m_Rslt.m_iScanTypeIdx);
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 645);
			}

			//20160226 ngh
			if(GetRsltScanTypeIdx() >= 2)
			{
				spd = m_Robot->Get16KScanSpeed();
			}
			else
			{
				spd = m_Robot->Get8KScanSpeed();
			}

			m_Robot->SetRobotMove_mm(MOTOR_0,m_Rslt.m_dMotPos[MOTOR_0],spd);

			SetSeqIdx(prc_idx, seq_idx, 640);
			break;
		case 640:
			//Move(scan)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ScanAxis Move Time Over Error=AX01 : SCAN 축동작 타임아웃(축 동작 실패)!-640-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]))
			{
				if(m_Robot->GetIsInposition(MOTOR_0))
				{
					m_strMainMsg.Format(_T("No.%d Scan Grab Move(%s)"), m_Rslt.m_iScanIdx + 1, m_strInsType);
					WriteLog(MAIN_LOG, m_strMainMsg);
					SetSeqIdx(prc_idx, seq_idx, 645);
				}
			}
			break;
		case 645:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 650);
			}	
			break;

		//Scan종료///////////////////////////////////
		case 650:
			//Scan end
			if(0 == (((int)m_Rslt.m_iScanIdxX) % 2))
			{
				m_Rslt.m_dMotPos[MOTOR_0] = (m_Align->GetAlignOffsetPosX_coordXY() - m_RecipeScan->GetScanStartX_coordXY(m_Rslt.m_iScanTypeIdx)) - m_RecipeScan->GetScanLeng_coordXY(m_Rslt.m_iScanTypeIdx);
				
			}
			else
			{
				m_Rslt.m_dMotPos[MOTOR_0] =  m_Align->GetAlignOffsetPosX_coordXY() - m_RecipeScan->GetScanStartX_coordXY(m_Rslt.m_iScanTypeIdx);
			}

			//20160226 ngh
			if(GetRsltScanTypeIdx() >= 2)
			{
				spd = m_Robot->Get16KScanSpeed();
			}
			else
			{
				spd = m_Robot->Get8KScanSpeed();
			}

			m_Robot->SetRobotMove_mm(MOTOR_0,m_Rslt.m_dMotPos[MOTOR_0],spd);

			SetSeqIdx(prc_idx, seq_idx, 660);
			break;
		case 660:
			//Move end(scan)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ScanAxis Move Time Over Error=AX01 : SCAN 축동작 타임아웃(축 동작 실패)!-560-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]))
			{
				if(m_Robot->GetIsInposition(MOTOR_0))
				{
					SetSeqIdx(prc_idx, seq_idx, 665);
				}
			}
			break;	
		case 665:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 670);
			}	
			break;
		case 670:
			//Complete receive
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_IPS_TCPIP_RCV;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "IPS Inspection End Time Over Error=IPS : SCAN COMPLETE 수신 타임아웃(IPS통신 수신실패)!-670-");
			m_bGrabComp[m_Rslt.m_ScanCamIdx][m_Rslt.m_iScanIdx] = FALSE;
			if(m_vision_grab_comp)
			{
				m_bGrabComp[m_Rslt.m_ScanCamIdx][m_Rslt.m_iScanIdx] = TRUE;
				m_strMainMsg.Format(_T("No.%d Scan End(%s)"), m_Rslt.m_iScanIdx + 1, m_strInsType);
				WriteLog(MAIN_LOG, m_strMainMsg);
				m_vision_grab_comp = FALSE;
				m_Rslt.m_iScanIdxX++;
				m_Rslt.m_iScanIdxY++;

				m_Rslt.m_iScanIdx++;
				SetSeqIdx(prc_idx, seq_idx, 680);
			}
			break;
		case 680:
			//End 
			if(m_RecipeScan->GetScanCnt_coordXY(m_Rslt.m_iScanTypeIdx) > m_Rslt.m_iScanIdxY)
			{
				m_vision_grab_comp = FALSE;
				m_vision_grab_ready = FALSE;

				if(m_Rslt.m_ScanCamIdx == 1)
				{
					scan_height = (m_RecipeScan->GetScanLeng_coordXY(m_Rslt.m_iScanTypeIdx) * 1000) / 5;
				}
				else
				{
					scan_height = (m_RecipeScan->GetScanLeng_coordXY(m_Rslt.m_iScanTypeIdx) * 1000) / 12;
				}
			
				if(scan_height % 1000)
				{
					scan_height -= (scan_height % 1000);
				}
				if(0 == (((int)m_Rslt.m_iScanIdxX) % 2))
				{
					m_Trigger->SetDirection(TRUE);
					Sleep(1000);
					PacketSendIPS(GetGrabReadyPacket(m_Rslt.m_ScanCamIdx, m_RecipeScan->GetInspect90Degree(m_Rslt.m_iScanTypeIdx), m_Rslt.m_iScanTypeIdx, m_Rslt.m_iScanIdx, FALSE, scan_height));
					//PacketSendIPS(GetGrabReadyPacket(0, FALSE, m_Rslt.m_iScanTypeIdx, m_Rslt.m_iScanIdxX,FALSE,scan_height));
				}
				else
				{
					m_Trigger->SetDirection(FALSE);
					Sleep(1000);
					PacketSendIPS(GetGrabReadyPacket(m_Rslt.m_ScanCamIdx, m_RecipeScan->GetInspect90Degree(m_Rslt.m_iScanTypeIdx), m_Rslt.m_iScanTypeIdx, m_Rslt.m_iScanIdx, TRUE, scan_height));
					//PacketSendIPS(GetGrabReadyPacket(0, FALSE, m_Rslt.m_iScanTypeIdx, m_Rslt.m_iScanIdxX,TRUE,scan_height));
				}
				m_strMainMsg.Format(_T("No.%d Scan Start(%s)"), m_Rslt.m_iScanIdx + 1, m_strInsType);
				WriteLog(MAIN_LOG, m_strMainMsg);
				SetSeqIdx(prc_idx, seq_idx, 610);
			}
			else
			{
				m_Trigger->SetOPMode(FALSE);
				m_strMainMsg.Format(_T("Scan End(%s)"), m_strInsType);
				WriteLog(MAIN_LOG, m_strMainMsg);
				SetSeqIdx(prc_idx, seq_idx, 690);
			}
			break;
		case 690:
			m_Light->Set8KLight(0);
			m_Light->Set16KLight(0);
			SetSeqIdx(prc_idx, seq_idx, 700);
			break;
		case 700:
			//Scan complete
			m_Rslt.m_ScanComplete[m_Rslt.m_iScanTypeIdx] = TRUE;
			////20160307 ngh
			ResetRsltSizeCntView();
			m_strMainMsg.Format(_T("Inspection End Wait(%s)"), m_strInsType);
			WriteLog(MAIN_LOG, m_strMainMsg);

			SetSeqIdx(prc_idx, seq_idx, 710);			
			break;
		case 710:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = 60000;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "IPS Inspection End Time Over Error=IPS : 8K SCAN COMPLETE 수신 타임아웃(IPS통신 수신실패)!-670-");
			if(m_vision_Inspection_comp[m_Rslt.m_iScanTypeIdx])
			{
				m_dInspectionEndSecond[m_Rslt.m_iScanTypeIdx] = clock();
				p_time = (m_dInspectionEndSecond[m_Rslt.m_iScanTypeIdx] - m_dInspectionStartSecond[m_Rslt.m_iScanTypeIdx]) / 1000.0;
				strLog.Format(_T("%s Time : %.3f sec"), m_strInsType, p_time);
				WriteLog(MAIN_LOG, strLog);
				/*count_seconds(&m_InspectionEndSecond[m_Rslt.m_iScanTypeIdx]);
				m_InspectionEndSecond[m_Rslt.m_iScanTypeIdx] = m_InspectionEndSecond[m_Rslt.m_iScanTypeIdx] - m_InspectionStartSecond[m_Rslt.m_iScanTypeIdx];
				m_lInspectionProcessTime[m_Rslt.m_iScanTypeIdx] = (long)(((double)m_EndSecond[m_Rslt.m_iScanTypeIdx][0]) * 1000);
				strLog.Format(_T("%s Time : %d ms"), m_strInsType, m_lInspectionProcessTime[m_Rslt.m_iScanTypeIdx]);
				WriteLog(MAIN_LOG, strLog);*/

				m_bDefectFileWriteComp[m_Rslt.m_iScanTypeIdx] = FALSE;
				m_bDefectFileWriteBreak = FALSE;
				m_bDefectFileLoadStart = TRUE;
				strLog.Format(_T("%s End"), m_strInsType);
				WriteLog(MAIN_LOG, strLog);

				//Scan type 종료
				//m_Rslt.m_iScanTypeIdx++;
				SetSeqIdx(prc_idx, seq_idx, 720);
			}
			break;
		case 720://20160819 defect 파일 생성 완료까지 대기
			if(m_bDefectFileSave[m_Rslt.m_iScanTypeIdx] && !m_ProcessBar->m_bProcessCancel)
			{
				//Scan type 종료
				m_Rslt.m_iScanTypeIdx++;
				SetSeqIdx(prc_idx, seq_idx, 460);
			}

			//ProcessBar에서 Cancel 버튼 누를시 정지
			if(m_ProcessBar->m_bProcessCancel)
			{
				ClickBtnenhctrlMainStop();
			}
			break;
		//Review film align동작 준비////////////////////////////////
		case 750:
			if(m_FilmAlign->GetInspectUse())
			{
				m_bDefectWriteComp = FALSE;
				//Move theta
				m_Rslt.m_dMotPos[MOTOR_2] =  m_Align->GetAlignOffsetPosT();
				m_Robot->SetRobotMove_mm(MOTOR_2, m_Rslt.m_dMotPos[MOTOR_2]);

				m_strMainMsg.Format(_T("Film align Start"));
				WriteLog(MAIN_LOG, m_strMainMsg);

				SetSeqIdx(prc_idx, seq_idx, 760);
			}
			else
			{
				m_bDefectWriteComp = TRUE;
				SetSeqIdx(prc_idx, seq_idx, 890);
			}
			break;
		case 760:
			//Move end(theta)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "RotatingAxis Move Time Over Error=AX03 : THETA 축동작 타임아웃(축 동작 실패)!-760-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_2, m_Rslt.m_dMotPos[MOTOR_2]))
			{
				if(m_Robot->GetIsInposition(MOTOR_2))
				{
					SetSeqIdx(prc_idx, seq_idx, 800);
				}
			}
			break;
		//Review film align동작 시작///////////////////////////////////////////////////
		case 800:
			//Start
			if(m_FilmAlign->GetInspectUse())
			{
				m_dFilmAlignStartSecond = clock();
				m_bDefectWriteComp = FALSE;
				m_Rslt.m_iFilmAlignRowIdx = 0;
				m_Rslt.m_iFilmAlignColIdx = 0;
				m_Rslt.m_iFilmAlignRgnIdx = 0;
				//AF 조명-Film align-

				m_AFCtrl->SetReviewLed(m_FilmAlign->GetLight());
	
				m_dlgFilmAlign->GetWindowRect(dlg_rect);
				GetDlgItem(IDC_STATIC_MAIN_RECT0)->GetClientRect(rect);
				GetDlgItem(IDC_STATIC_MAIN_RECT0)->ClientToScreen(rect);
				m_dlgFilmAlign->MoveWindow(rect.left, rect.bottom, dlg_rect.Width(), dlg_rect.Height(), TRUE);
				m_FilmAlign->ShowWindow(SW_SHOW);

				//시간 대기
				not_delay = CheckDelay(prc_idx, seq_idx, 500);
				if(not_delay)
				{
					SetSeqIdx(prc_idx, seq_idx, 810);
				}
			}
			else
			{
				m_bDefectWriteComp = TRUE;
				SetSeqIdx(prc_idx, seq_idx, 890);
			}
			break;
		case 810:
			//Move film align inspect pos.
			m_FilmAlign->SetRegionMove(m_Rslt.m_iFilmAlignRowIdx + 1, m_Rslt.m_iFilmAlignColIdx + 1, m_Rslt.m_iFilmAlignRgnIdx, TRUE);
			m_Rslt.m_dMotPos[MOTOR_0] = m_FilmAlign->GetRobotPos(MOTOR_0);
			m_Rslt.m_dMotPos[MOTOR_1] = m_FilmAlign->GetRobotPos(MOTOR_1);
			SetSeqIdx(prc_idx, seq_idx, 820);
			break;
		case 820:
			//Move end(scan)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ScanAxis Move Time Over Error=AX01 : SCAN 축동작 타임아웃(축 동작 실패)!-820-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]))
			{
				if(m_Robot->GetIsInposition(MOTOR_0))
				{
					SetSeqIdx(prc_idx, seq_idx, 830);
				}
			}
			break;	
		case 830:
			//Move end(shift)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ShiftAxis Move Time Over Error=AX02 : SHIFT 축동작 타임아웃(축 동작 실패)!-830-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_1, m_Rslt.m_dMotPos[MOTOR_1]))
			{
				if(m_Robot->GetIsInposition(MOTOR_1))
				{
					SetSeqIdx(prc_idx, seq_idx, 840);
				}
			}
			break;
		case 840:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 200);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 850);
				//영상촬상
				m_Review->SetGrab();
			}	
			break;
		case 850:
			//검사
			m_FilmAlign->SetInspectFilmAlign(m_Rslt.m_iFilmAlignRgnIdx, &x, &y,m_film_image[m_Rslt.m_FilmAlignIdx][m_Rslt.m_iFilmAlignRgnIdx]);  //20160425 SC
			
			//Image좌표->robot좌표
			pixel_size = 0.001683168;
			robot_x =  m_Rslt.m_dMotPos[MOTOR_0];
			robot_y =  m_Rslt.m_dMotPos[MOTOR_1];
			//robot좌표를->glass좌표
			offset_x = m_Align->GetAlignOffsetPosX_coordXY() - m_RecipePanel->GetMarkPosX_coordXY(0);
			offset_y = m_Align->GetAlignOffsetPosY_coordXY() - m_RecipePanel->GetMarkPosY_coordXY(0);

			glass_x = robot_x - offset_x;
			glass_y = robot_y - offset_y;

			//Review카메라 중앙 기준으로 값을 수정(카메라 중심으로 티칭 하였다)
			review_cent_x = (REVIEW_IMAGE_WIDTH * 0.5 * pixel_size) - (x * pixel_size);
			review_cent_y = (REVIEW_IMAGE_HEIGHT * 0.5 * pixel_size) - (y * pixel_size);
			glass_x = glass_x - review_cent_x;
			glass_y = glass_y + review_cent_y;

			m_Rslt.m_FilmAlignX[m_Rslt.m_FilmAlignIdx][m_Rslt.m_iFilmAlignRgnIdx] = glass_x;
			m_Rslt.m_FilmAlignY[m_Rslt.m_FilmAlignIdx][m_Rslt.m_iFilmAlignRgnIdx] = glass_y;
			
			//20160711 ngh
			//image_path.Format(_T("\\\\%s\\result\\%04d%02d%02d\\%S\\%S\\Image\\FILM_ALIGN_IMAGE\\"), m_IPSCtrl->GetIPSIP(), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), glass_data.strLotID, glass_data.strGlassID);
			image_path.Format(_T("%sImage\\FILM_ALIGN_IMAGE\\"),GetResultFilePath());  //20160725 pkh
	
			_MakeDirectories(image_path);
			glass_data = m_Inline->GetLoaderGlassData();

			//index 20160712 ngh
			imagefilename.Format(_T("%s%S_%S_%d_%d_%s_%s_%.3f_%.3f.JPG"), image_path, glass_data.strLotID, glass_data.strGlassID, m_Rslt.m_iFilmAlignRowIdx, m_Rslt.m_iFilmAlignColIdx, _T("RRRRRRR"),
									_T("AAAAAAA"), m_Rslt.m_FilmAlignX[m_Rslt.m_FilmAlignIdx][m_Rslt.m_iFilmAlignRgnIdx], m_Rslt.m_FilmAlignY[m_Rslt.m_FilmAlignIdx][m_Rslt.m_iFilmAlignRgnIdx]);

			//20160822 ngh 이미지 저장
			m_FilmAlign->SaveFilmAlignImage(imagefilename);

			//temp_image_path.Format(_T("%S"), m_FilmAlign->m_FilmImage);
			//temp_image_path.Format(_T("%S"), image_path);  // 20160820
			//m_Dfserver->CopyFile_Df(image_path, imagefilename); // 20160820 pkh
			//m_Dfserver->CopyFile_Df(temp_image_path, imagefilename); // 20160820 pkh

			SetSeqIdx(prc_idx, seq_idx, 860);
			break;
		case 860:
			
			(m_Rslt.m_iFilmAlignRgnIdx)++;
			//Region index확인
			if(m_Rslt.m_iFilmAlignRgnIdx < FILM_ALIGN_RGN_POS_MAX_MUM)
			{
				
				SetSeqIdx(prc_idx, seq_idx, 810);
			}
			else
			{
				//한개 필름 끝남
				SetSeqIdx(prc_idx, seq_idx, 870);
				(m_Rslt.m_FilmAlignIdx)++;
			}
			break;
		case 870:
			(m_Rslt.m_iFilmAlignColIdx)++;
			//Film col index확인
			if(m_Rslt.m_iFilmAlignColIdx < m_RecipeCell->GetFilmCntX_coordXY())
			{
				m_Rslt.m_iFilmAlignRgnIdx = 0;
				SetSeqIdx(prc_idx, seq_idx, 810);
			}
			else
			{
				//한개의 col 끝남
				SetSeqIdx(prc_idx, seq_idx, 880);
			}
			break;
		case 880:
			//Film row index확인
			(m_Rslt.m_iFilmAlignRowIdx)++;
			if(m_Rslt.m_iFilmAlignRowIdx < m_RecipeCell->GetFilmCntY_coordXY())
			{
				m_Rslt.m_iFilmAlignColIdx = 0;
				m_Rslt.m_iFilmAlignRgnIdx = 0;
				SetSeqIdx(prc_idx, seq_idx, 810);
			}
			else
			{
				m_dFilmAlignEndSecond = clock();
				p_time = (m_dFilmAlignEndSecond - m_dFilmAlignStartSecond) / 1000;
				strLog.Format(_T("Film Align Inspection Time : %.3f sec"), p_time);
				WriteLog(MAIN_LOG, strLog);

				ClickBtnenhctrlMainSaveFilmalign();
				
				m_strMainMsg.Format(_T("Film align End"));
				m_FilmAlign->ShowWindow(SW_HIDE);
				m_Review->SetTimer(1,1000,NULL);
				SetSeqIdx(prc_idx, seq_idx, 890);
			}
			break;
		case 890:
			m_strMainMsg.Format(_T("Defect File Save Wait"));
			
			//20160720 ngh
			if(m_bDefectFileSave[0] && m_bDefectFileSave[1] && m_bDefectFileSave[2] && m_bDefectFileSave[3] && m_bDefectFileSave[4] &&m_bDefectWriteComp)
			{
				SetSeqIdx(prc_idx, seq_idx, 895);
			}
			break;
		case 895:
			if(m_GlassManu->GetLoaderUse())//Loader 사용유무
			{
				m_strMainMsg.Format(_T("Auto Inspection End"));
				WriteLog(MAIN_LOG, m_strMainMsg);
				
				//20160711 ngh 스캔 후 대기
				if(m_bScanAfterWait)
				{
					m_Light->Set8KLight(0);
					m_Light->Set16KLight(0);
					SetSeqIdx(prc_idx, seq_idx, 896);
				}
				else
				{
					//Exchange-unloading 대기
					SetSeqIdx(prc_idx, seq_idx, 100);
				}
			}
			else
			{
				m_dEndSecond = clock();
				p_time = (m_dEndSecond - m_dStartSecond) / 1000;
				strLog.Format(_T("Manual Process Time : %.3f sec"), p_time);
				WriteLog(MAIN_LOG, strLog);

				/*count_seconds(&m_EndSecond);

				m_EndSecond = m_EndSecond - m_StartSecond;
				m_lProcessTime = (long)(((double)m_EndSecond[0]) * 1000);
				strLog.Format(_T("Manual Process Time : %d ms"), m_lProcessTime);
				WriteLog(MAIN_LOG, strLog);*/

				m_strMainMsg.Format(_T("Manual Inspection End"));
				WriteLog(MAIN_LOG, m_strMainMsg);
				SetSeqIdx(prc_idx, seq_idx, 896);
			}			
			break;
			//20160714 ngh 90도 턴 검사 후 다시 원래 0도 위치로 이동
			//20160720 ngh Ez Motor 원점 위치로 이동
		case 896:
			m_Rslt.m_dMotPos[MOTOR_2] =  m_Align->GetAlignOffsetPosT() + m_RecipeScan->GetRoatateOffset(0);
			m_Robot->SetRobotMove_mm(MOTOR_2, m_Rslt.m_dMotPos[MOTOR_2]);
			m_strMainMsg.Format(_T("RotatingAxis Init Move Start"));
			WriteLog(MAIN_LOG, m_strMainMsg);
			SetSeqIdx(prc_idx, seq_idx, 897);
			break;
		case 897:
			//Move end(theta)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "RotatingAxis Move Time Over Error=AX03 : THETA 축동작 타임아웃(축 동작 실패)!-898-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_2, m_Rslt.m_dMotPos[MOTOR_2]))
			{
				if(m_Robot->GetIsInposition(MOTOR_2))
				{
					if(m_GlassManu->GetLoaderUse())//Loader 사용유무
					{
						//20160711 ngh 스캔 후 대기
						m_strMainMsg.Format(_T("RotatingAxis init Move End"));
						WriteLog(MAIN_LOG, m_strMainMsg);
						if(m_bScanAfterWait)
						{
							m_strMainMsg.Format(_T("Scan After Wait"));
							WriteLog(MAIN_LOG, m_strMainMsg);
							ClickBtnenhctrlMainStop();
							if(m_bDefectError)
							{
								//Button
								m_btn_start.SetEnabled(FALSE);
								m_btn_start.SetValue(FALSE);
								m_btn_stop.SetEnabled(FALSE);
								m_btn_stop.SetValue(FALSE);
								m_btn_reset.SetEnabled(TRUE);
								m_btn_reset.SetValue(TRUE);
							}
							else
							{
								//Button
								m_btn_start.SetEnabled(TRUE);
								m_btn_start.SetValue(TRUE);
								m_btn_stop.SetEnabled(FALSE);
								m_btn_stop.SetValue(FALSE);
								m_btn_reset.SetEnabled(FALSE);
								m_btn_reset.SetValue(FALSE);
							}
						}
					}
					else
					{
						m_strMainMsg.Format(_T("RotatingAxis init Move End"));
						WriteLog(MAIN_LOG, m_strMainMsg);
						SetSeqIdx(prc_idx, seq_idx, 899);
					}
				}
			}
			break;
		case 899:
			//수동검사 종료(정지하고 있으면된다)
			//종료/////////////////////////////////////////////////////////////////////////////
			m_Light->Set8KLight(0);
			m_Light->Set16KLight(0);
			ClickBtnenhctrlMainStop();
			if(m_bDefectError)
			{
				//Button
				m_btn_start.SetEnabled(FALSE);
				m_btn_start.SetValue(FALSE);
				m_btn_stop.SetEnabled(FALSE);
				m_btn_stop.SetValue(FALSE);
				m_btn_reset.SetEnabled(TRUE);
				m_btn_reset.SetValue(TRUE);
			}
			else
			{
				//Button
				m_btn_start.SetEnabled(TRUE);
				m_btn_start.SetValue(TRUE);
				m_btn_stop.SetEnabled(FALSE);
				m_btn_stop.SetValue(FALSE);
				m_btn_reset.SetEnabled(FALSE);
				m_btn_reset.SetValue(FALSE);
			}
			break;
		//사용안함///////////////////////////////////////////////////////////////////////////////////////////
		//Unloader///////////////////////////////////////////////////////////////////////////////////////////
		//case 900:
		//	//Loader pork sensor도 체크
		//	m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
		//	sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "X53C or X03D : ROBOT HAND DETECT TIMEOUT(로보트 핸드 센서 감지않됨-핸드가 있으면 감지 않된다-)!-260-");	
		//	if( (m_IO->GetReadDI(DI_ROBOT_HAND_DETECT_LEFT)) && (m_IO->GetReadDI(DI_ROBOT_HAND_DETECT_RIGHT)) )
		//	{
		//		SetSeqIdx(prc_idx, seq_idx, 910);

		//		//Move loading position
		//		m_Rslt.m_dMotPos[MOTOR_0] =  m_RecipePanel->GetPanelLoadPosY();
		//		m_Robot->SetRobotMove_mm(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]);
		//		m_Rslt.m_dMotPos[MOTOR_2] =  m_RecipePanel->GetPanelLoadPosT();
		//		m_Robot->SetRobotMove_mm(MOTOR_2, m_Rslt.m_dMotPos[MOTOR_2]);

		//	}
		//	break;
		//case 910:
		//	//Move end(scan)
		//	m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
		//	sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ScanAxis Move Time Over Error=AX01 : SCAN 축동작 타임아웃(축 동작 실패)!-910-");

		//	if(m_Robot->GetIsMoveComplete(MOTOR_0, m_Rslt.m_dMotPos[MOTOR_0]))
		//	{
		//		if(m_Robot->GetIsInposition(MOTOR_0))
		//		{
		//			SetSeqIdx(prc_idx, seq_idx, 915);
		//		}
		//	}
		//	break;
		//case 915:
		//	//Move end(theta)
		//	m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
		//	sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "RotatingAxis Move Time Over Error=AX03 : THETA 축동작 타임아웃(축 동작 실패)-915-");
		//	
		//	if(m_Robot->GetIsMoveComplete(MOTOR_2, m_Rslt.m_dMotPos[MOTOR_2]))
		//	{
		//		if(m_Robot->GetIsInposition(MOTOR_2))
		//		{
		//			SetSeqIdx(prc_idx, seq_idx, 920);
		//		}
		//	}
		//	break;
		case 920:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 930);

				//Sensor back
				m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_FW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_BW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_FW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_BW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_FW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_BW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_FW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_BW_SOL, TRUE);
			}
			break;
		case 930:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 1 Forward Error=X540 : STAGE ALIGN FRONT BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)-930-!");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT1_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 940);
			}
			break;
		case 940:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 2 Forward Error=X542 : STAGE ALIGN FRONT BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)-940-!");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT2_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 950);
			}
			break;
		case 950:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 1  Forward Error=X544 : STAGE ALIGN REAR BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)-950-!");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR1_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 960);
			}
			break;
		case 960:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 2 Forward Error=X546 : STAGE ALIGN REAR BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)-960-!");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR2_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 970);
			}
			break;
		case 970:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Left 1 Forward Error=X548 : STAGE SIDE LEFT BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)-970-!");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT1_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 980);
			}
			break;
		case 980:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Left 2 Forward Error=X54A : STAGE SIDE LEFT BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)-980-!");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT2_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 990);
			}
			break;
		case 990:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 1 Forward Error=X54C : STAGE SIDE RIGHT BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)-990-!");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT1_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 1000);
			}
			break;
		case 1000:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 2 Forward Error=X54E : STAGE SIDE RIGHT BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)-1000-!");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT2_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 1010);

				//Vaccum off
				m_IO->SetWriteDO(DO_STAGE_VACUUM_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_AIR_BLOW_SOL, FALSE);//20160218
			}
			break;
		case 1010:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 1015);
				m_IO->SetWriteDO(DO_STAGE_AIR_BLOW_SOL, FALSE);

				//X축 안전위치 이동(위치 : 0)
				m_Rslt.m_dMotPos[MOTOR_1] =  0;
				m_Robot->SetRobotMove_mm(MOTOR_1, m_Rslt.m_dMotPos[MOTOR_1]);
			}
			break;
		case 1015:
			//Move end(shift)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ShiftAxis Move Time Over Error=AX02 : SHIFT 축동작 타임아웃(축 동작 실패)!-1015-");
			
			if(m_Robot->GetIsMoveComplete(MOTOR_1, m_Rslt.m_dMotPos[MOTOR_1]))
			{
				if(m_Robot->GetIsInposition(MOTOR_1))
				{
					SetSeqIdx(prc_idx, seq_idx, 1020);
				}
			}
			break;
		case 1020:
			//Vac check
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Chuck VAC Error=X50C : STAGE VACCUM PRESSURE OFF TIMEOUT(진공센서 감지됨)!-1020-");	
			//if(!m_IO->GetReadDI(DI_CHUCK_VACUUM_ON_DETECT))
			//임시
			if(1)
			{
				SetSeqIdx(prc_idx, seq_idx, 1030);

				//Lift pin up and mechnical align
				m_Rslt.m_dMotPos[MOTOR_3] =  m_RecipePanel->GetPanelLoadPosUP();
				m_Robot->SetRobotMove_mm(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]);
			}
			break;	
		case 1030:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "LiftPinAxis Move Time Over Error=AX04 : UP/DN 축동작 타임아웃(축 동작 실패)!-1030-");
			if(m_Robot->GetIsMoveComplete(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]))
			{
				if(m_Robot->GetIsInposition(MOTOR_3))
				{
					SetSeqIdx(prc_idx, seq_idx, 1040);
				}
			}
			break;
		case 1040:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 1050);
			}
			break;
		case 1050:
			//Unloading complete대기////////////////////////////////////////////////////
			if(m_GlassManu->GetLoaderUse())//Unloader 사용유무
			{
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_ECS;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "ESC : UNLOADER ECS TIMEOUT(Unloader 동작 이상 : %s)!-1050-", m_Rslt.m_sLoaderUnloaderErrMsg);	

				//Unloading 시작
	
				if(m_Rslt.m_bCompleteLoading)
				{
					SetSeqIdx(prc_idx, seq_idx, 1060);
				}
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 1060);
			}
			break;
		case 1060:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 1070);
				//Lift pin dn and mechnical align
				m_Rslt.m_dMotPos[MOTOR_3] =  m_RecipePanel->GetPanelLoadPosDN();
				m_Robot->SetRobotMove_mm(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]);
			}
			break;
		case 1070:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "LiftPinAxis Move Time Over Error=AX04 : UP/DN 축동작 타임아웃(축 동작 실패)!-1070-");
			if(m_Robot->GetIsMoveComplete(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]))
			{
				if(m_Robot->GetIsInposition(MOTOR_3))
				{
					SetSeqIdx(prc_idx, seq_idx, 1080);
				}
			}
			break;
		case 1080:
			//종료/////////////////////////////////////////////////////////////////////////////
			m_Light->Set8KLight(0);
			m_Light->Set16KLight(0);

			break;
		//Mechanical aling backword////////////////////////////////////////////////////////////
		case 1200:
			//Vaccum
			m_IO->SetWriteDO(DO_STAGE_VACUUM_SOL, TRUE);
			m_IO->SetWriteDO(DO_STAGE_AIR_BLOW_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 1210);
			break;
		case 1210:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 1220);

				//Sensor backword
				m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_FW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_BW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_FW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_BW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_FW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_BW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_FW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_BW_SOL, TRUE);
			}
			break;
		case 1220:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 1 Backward Error=X541 : STAGE ALIGN FRONT BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-1220-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT1_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 1230);
			}
			break;
		case 1230:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 2 Backward Error=X543 : STAGE ALIGN FRONT BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-1230-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT2_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 1240);
			}
			break;
		case 1240:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 1 Backward Error=X545 : STAGE ALIGN REAR BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-1240-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR1_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 1250);
			}
			break;
		case 1250:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 2 Backward Error=X547 : STAGE ALIGN REAR BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-1250-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR2_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 1260);
			}
			break;
		case 1260:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Left 2 Forward Error=X54A : STAGE SIDE LEFT BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-1260-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT1_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 1270);
			}
			break;
		case 1270:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 1 Forward Error=X54C : STAGE SIDE LEFT BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-1270-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT2_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 1280);
			}
			break;
		case 1280:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 2 Forward Error=X54E : STAGE SIDE RIGHT BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-1280-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT1_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 1290);
			}
			break;
		case 1290:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 2 Backward Error=X54F : STAGE SIDE RIGHT BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-1290-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT2_BW))
			{
				//Vaccum
				m_IO->SetWriteDO(DO_STAGE_VACUUM_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_AIR_BLOW_SOL, FALSE);//20160218
				SetSeqIdx(prc_idx, seq_idx, 1300);
			}
			break;
		case 1300:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, m_Rslt.m_iMechAlignSeq);
			}
			break;
		//Macha align forward////////////////////////////////////////////////////////////////////////////////////
		case 1400:
			//Vaccum
			m_IO->SetWriteDO(DO_STAGE_VACUUM_SOL, FALSE);
			m_IO->SetWriteDO(DO_STAGE_AIR_BLOW_SOL, FALSE);//20160218

			SetSeqIdx(prc_idx, seq_idx, 1410);
			break;
		case 1410:
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 1420);

				//Sensor forward
				m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_FW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_BW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_FW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_BW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_FW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_BW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_FW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_BW_SOL, FALSE);
			}
			break;
		case 1420:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 1 Forward Error=X540 : STAGE ALIGN FRONT FW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-290-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT1_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 1430);
			}
			break;
		case 1430:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 2 Forward Error=X542 : STAGE ALIGN FRONT FW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-291-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT2_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 1440);
			}
			break;
		case 1440:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 1  Forward Error=X544 : STAGE ALIGN REAR FW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-292-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR1_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 1450);
			}
			break;
		case 1450:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 2 Forward Error=X546 : STAGE ALIGN REAR FW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-293-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR2_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 1460);
			}
			break;
		case 1460:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Left 1 Forward Error=X548 : STAGE SIDE LEFT FW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-294-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT1_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 1470);
			}
			break;
		case 1470:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Left 2 Forward Error=X54A : STAGE SIDE LEFT FW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-295-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT2_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 1480);
			}
			break;
		case 1480:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 1 Forward Error=X54C : STAGE SIDE RIGHT FW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-296-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT1_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 1490);
			}
			break;
		case 1490:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 2 Forward Error=X54E : STAGE SIDE RIGHT FW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-297-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT2_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 1500);
			}
			break;
		case 1500:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 1510);
				
				//Vaccum ON
				m_IO->SetWriteDO(DO_STAGE_VACUUM_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_AIR_BLOW_SOL, FALSE);
			}
			break;
		case 1510:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, m_Rslt.m_iMechAlignSeq);
			}
			break;


		//20160726 ngh MechAlignSeq 수정
		//Glass Unloading
		case 2000:
			//Lift pin dn and mechnical align
			m_Rslt.m_dMotPos[MOTOR_3] = -4.5;//임시 위치
			m_Robot->SetRobotMove_mm(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]);
			m_strMainMsg.Format(_T("Lift Pin Mid Down Move Start"));
			WriteLog(MAIN_LOG, m_strMainMsg);
			SetSeqIdx(prc_idx, seq_idx, 2010);
			break;
		case 2010:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "LiftPinAxis Move Time Over Error=AX04 : UP/DN 축동작 타임아웃(축 동작 실패)!-2010-");
			if(m_Robot->GetIsMoveComplete(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]))
			{
				if(m_Robot->GetIsInposition(MOTOR_3))
				{
					m_strMainMsg.Format(_T("Lift Pin Mid Down Move End"));
					WriteLog(MAIN_LOG, m_strMainMsg);
					SetSeqIdx(prc_idx, seq_idx, 2020);
				}
			}
			break;
		case 2020:
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				m_IO->SetWriteDO(DO_STAGE_AIR_BLOW_SOL, TRUE);
				SetSeqIdx(prc_idx, seq_idx, 2030);
			}
			break;
		case 2030:
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 2040);

				//Sensor forward
				m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_FW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_BW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_FW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_BW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_FW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_BW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_FW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_BW_SOL, FALSE);
			}
			break;
		case 2040:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 1 Forward Error=X540 : STAGE ALIGN FRONT FW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2040-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT1_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2050);
			}
			break;
		case 2050:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 2 Forward Error=X542 : STAGE ALIGN FRONT FW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2050-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT2_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2060);
			}
			break;
		case 2060:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 1  Forward Error=X544 : STAGE ALIGN REAR FW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2060-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR1_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2070);
			}
			break;
		case 2070:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 2 Forward Error=X546 : STAGE ALIGN REAR FW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2070-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR2_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2080);
			}
			break;
		case 2080:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Left 1 Forward Error=X548 : STAGE SIDE LEFT FW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2080-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT1_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2090);
			}
			break;
		case 2090:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Left 2 Forward Error=X54A : STAGE SIDE LEFT FW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2090-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT2_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2100);
			}
			break;
		case 2100:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 1 Forward Error=X54C : STAGE SIDE RIGHT FW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2100-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT1_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2110);
			}
			break;
		case 2110:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 2 Forward Error=X54E : STAGE SIDE RIGHT FW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2110-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT2_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2120);
			}
			break;
		case 2120:
			//Lift pin full down
			m_Rslt.m_dMotPos[MOTOR_3] =  m_RecipePanel->GetPanelLoadPosDN();
			m_Robot->SetRobotMove_mm(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]);
			m_strMainMsg.Format(_T("Lift Pin Down Move Start"));
			WriteLog(MAIN_LOG, m_strMainMsg);
			SetSeqIdx(prc_idx, seq_idx, 2130);
			break;
		case 2130:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "LiftPinAxis Move Time Over Error=AX04 : UP/DN 축동작 타임아웃(축 동작 실패)!-2130-");
			if(m_Robot->GetIsMoveComplete(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]))
			{	
				if(m_Robot->GetIsInposition(MOTOR_3))
				{
					m_strMainMsg.Format(_T("Lift Pin Down Move End"));
					WriteLog(MAIN_LOG, m_strMainMsg);
					//Mech align forwarding
					SetSeqIdx(prc_idx, seq_idx, 2140);
				}
			}
			break;
		case 2140:
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				m_IO->SetWriteDO(DO_STAGE_AIR_BLOW_SOL, FALSE);
				SetSeqIdx(prc_idx, seq_idx, 2150);
			}
			break;
		case 2150:
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				m_IO->SetWriteDO(DO_STAGE_VACUUM_SOL, TRUE);
				SetSeqIdx(prc_idx, seq_idx, 2160);
			}
			break;
		case 2160:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 2170);

				//Sensor backword
				m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_FW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_BW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_FW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_BW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_FW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_BW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_FW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_BW_SOL, TRUE);
			}
			break;
		case 2170:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 1 Backward Error=X541 : STAGE ALIGN FRONT BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2170-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT1_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2180);
			}
			break;
		case 2180:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 2 Backward Error=X543 : STAGE ALIGN FRONT BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2180-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT2_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2190);
			}
			break;
		case 2190:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 1 Backward Error=X545 : STAGE ALIGN REAR BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2190-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR1_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2200);
			}
			break;
		case 2200:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 2 Backward Error=X547 : STAGE ALIGN REAR BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2200-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR2_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2210);
			}
			break;
		case 2210:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Left 2 Forward Error=X54A : STAGE SIDE LEFT BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2210-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT1_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2220);
			}
			break;
		case 2220:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 1 Forward Error=X54C : STAGE SIDE LEFT BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2220-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT2_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2230);
			}
			break;
		case 2230:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 2 Forward Error=X54E : STAGE SIDE RIGHT BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2230-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT1_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2240);
			}
			break;
		case 2240:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 2 Backward Error=X54F : STAGE SIDE RIGHT BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2240-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT2_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2250);
			}
			break;
		case 2250:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, m_Rslt.m_iMechAlignSeq);
			}
			break;
		//Glass Loading
		case 3000:
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				m_IO->SetWriteDO(DO_STAGE_VACUUM_SOL, FALSE);
				SetSeqIdx(prc_idx, seq_idx, 3010);
			}
			break;
		case 3010:
			//Lift pin dn and mechnical align
			m_Rslt.m_dMotPos[MOTOR_3] =  -4.5;//임시 위치
			m_Robot->SetRobotMove_mm(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]);
			m_strMainMsg.Format(_T("Lift Pin Mid Up Move Start"));
			WriteLog(MAIN_LOG, m_strMainMsg);
			SetSeqIdx(prc_idx, seq_idx, 3020);
			break;
		case 3020:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "LiftPinAxis Move Time Over Error=AX04 : UP/DN 축동작 타임아웃(축 동작 실패)!-3020-");
			if(m_Robot->GetIsMoveComplete(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]))
			{
				if(m_Robot->GetIsInposition(MOTOR_3))
				{
					m_strMainMsg.Format(_T("Lift Pin Mid Up Move End"));
					WriteLog(MAIN_LOG, m_strMainMsg);
					SetSeqIdx(prc_idx, seq_idx, 3030);
				}
			}
			break;
		case 3030:
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				m_IO->SetWriteDO(DO_STAGE_AIR_BLOW_SOL, TRUE);
				SetSeqIdx(prc_idx, seq_idx, 3040);
			}
			break;
		case 3040:
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 3050);

				//Sensor forward
				m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_FW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_BW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_FW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_BW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_FW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_BW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_FW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_BW_SOL, FALSE);
			}
			break;
		case 3050:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 1 Forward Error=X540 : STAGE ALIGN FRONT FW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2040-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT1_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 3060);
			}
			break;
		case 3060:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 2 Forward Error=X542 : STAGE ALIGN FRONT FW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2050-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT2_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 3070);
			}
			break;
		case 3070:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 1  Forward Error=X544 : STAGE ALIGN REAR FW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2060-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR1_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 3080);
			}
			break;
		case 3080:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 2 Forward Error=X546 : STAGE ALIGN REAR FW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2070-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR2_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 3090);
			}
			break;
		case 3090:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Left 1 Forward Error=X548 : STAGE SIDE LEFT FW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2080-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT1_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 3100);
			}
			break;
		case 3100:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Left 2 Forward Error=X54A : STAGE SIDE LEFT FW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2090-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT2_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 3110);
			}
			break;
		case 3110:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 1 Forward Error=X54C : STAGE SIDE RIGHT FW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2100-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT1_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 3120);
			}
			break;
		case 3120:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 2 Forward Error=X54E : STAGE SIDE RIGHT FW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2110-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT2_FW))
			{
				SetSeqIdx(prc_idx, seq_idx, 3130);
			}
			break;
		case 3130:
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				m_IO->SetWriteDO(DO_STAGE_AIR_BLOW_SOL, FALSE);
				SetSeqIdx(prc_idx, seq_idx, 3140);
			}
			break;
		case 3140:
			//시간 대기
			not_delay = CheckDelay(prc_idx, seq_idx, 500);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 2170);

				//Sensor backword
				m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_FW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_BW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_FW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_BW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_FW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_BW_SOL, TRUE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_FW_SOL, FALSE);
				m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_BW_SOL, TRUE);
			}
			break;
		case 3150:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 1 Backward Error=X541 : STAGE ALIGN FRONT BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2170-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT1_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2180);
			}
			break;
		case 3160:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 2 Backward Error=X543 : STAGE ALIGN FRONT BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2180-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT2_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2190);
			}
			break;
		case 3170:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 1 Backward Error=X545 : STAGE ALIGN REAR BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2190-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR1_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2200);
			}
			break;
		case 3180:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 2 Backward Error=X547 : STAGE ALIGN REAR BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2200-");	
			if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR2_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2210);
			}
			break;
		case 3190:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Left 2 Forward Error=X54A : STAGE SIDE LEFT BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2210-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT1_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2220);
			}
			break;
		case 3200:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 1 Forward Error=X54C : STAGE SIDE LEFT BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2220-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT2_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2230);
			}
			break;
		case 3210:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 2 Forward Error=X54E : STAGE SIDE RIGHT BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2230-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT1_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 2240);
			}
			break;
		case 3220:
			//Sensor(1~4)
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 2 Backward Error=X54F : STAGE SIDE RIGHT BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2240-");	
			if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT2_BW))
			{
				SetSeqIdx(prc_idx, seq_idx, 3230);
			}
			break;
		case 3230:
			//Lift pin up and mechnical align
			m_strMainMsg.Format(_T("Lift Pin Up Move Start"));
			WriteLog(MAIN_LOG, m_strMainMsg);
			m_Rslt.m_dMotPos[MOTOR_3] =  m_RecipePanel->GetPanelLoadPosUP();
			m_Robot->SetRobotMove_mm(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]);
			SetSeqIdx(prc_idx, seq_idx, 3240);
			break;
		case 3240:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "LiftPinAxis Move Time Over Error=AX04 : UP/DN 축동작 타임아웃(축 동작 실패)!-3240-");
			if(m_Robot->GetIsMoveComplete(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]))
			{
				if(m_Robot->GetIsInposition(MOTOR_3))
				{
					m_strMainMsg.Format(_T("Lift Pin Up Move End"));
					WriteLog(MAIN_LOG, m_strMainMsg);
					//m_bDefectFileWriteBreak = TRUE;
					SetSeqIdx(prc_idx, seq_idx, m_Rslt.m_iMechAlignSeq);
				}
			}
			break;
		}



		//
		//
		/////////////////////////////////////
		break;
	case 3:
		/////////////////////////////////////
		//ECS sequence
		//

		sub_seq_idx = m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx;
		switch(sub_seq_idx)
		{
		case 100:
			
			//ECS -> Recipe 변경요구////////////////////////////////////////////
			if(0)
			{
				SetSeqIdx(prc_idx, seq_idx, 120);
			}
			//ECS -> 판넬 로딩 요청/////////////////////////////////////////////
			else if(0)
			{
				SetSeqIdx(prc_idx, seq_idx, 130);
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 110);
			}
			break;
		case 110:
			SetSeqIdx(prc_idx, seq_idx, 100);
			break;
		}

		break;
	}

	if(chk_timeout)
	{
		rslt = CheckTimeout(prc_idx, seq_idx, m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx, status_type);
	}

	m_btn_MainMsg.SetWindowTextW(m_strMainMsg);

	return rslt;
}


//시퀀스 인덱스 넣기(리셋후 다음동작 않함)
void CEMIControlDlg::SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx)
{
	m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx = 0;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDispSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iNextSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_bOpSts = 0;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDeviceIdx = 0;
}

//시퀀스 인덱스 넣기(리셋하며 다음동작으로 넘어감)
void CEMIControlDlg::SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx, int next_seq_sub_idx, int type)
{
	m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx = 0;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDispSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iNextSubSeqIdx = next_seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_bOpSts = type;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDeviceIdx = 0;
}

void CEMIControlDlg::SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx, int next_seq_sub_idx, int type, int device_idx)
{
	m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx = 0;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDispSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iNextSubSeqIdx = next_seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_bOpSts = type;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDeviceIdx = device_idx;
}

//Check timeout
int CEMIControlDlg::CheckTimeout(int prc_idx, int seq_idx, int seq_sub_idx, char* status_type)
{

	if(0 == m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx)
	{
		m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx++;
		m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeoutS = _TimerCounter;
	}
	else
	{
		//Compare timeout
		if(0 <m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeout)
		{
			m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeoutE = _TimerCounter;
			if( m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeout < (m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeoutE - m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeoutS) )
			{
				return 0;
			}
		}
	}

	sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iTypeStatus, "%s", status_type);//축번호

	return 1;
}

//Check timeout
int CEMIControlDlg::CheckDelay(int prc_idx, int seq_idx, int delay)
{
	if(0 == m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_iNGSeqIdx)
	{
		m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_iNGSeqIdx++;
		m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_dTimeoutS = _TimerCounter;
	}
	else
	{
		//Compare timeout
		if(0 < delay)
		{
			m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_dTimeoutE = _TimerCounter;
			if( delay < (m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_dTimeoutE - m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_dTimeoutS) )
			{
				return 1;
			}
		}
		else
		{
			return 1;
		}
	}

	return 0;
}

int CEMIControlDlg::AutoStart()
{
	int i, j;

#ifdef MACHINE
	//20160310 ngh
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CDialogMSG dlg;
	CString str;
	
	//Home check
	if(m_Robot->GetServoOriginStatus(MOTOR_0) != 1)
	{
		str.Format(_T("Scan Axi Origin Error"));
		dlg.SetCaption(str);
		dlg.DoModal();
		return 1;
	}

	if(m_Robot->GetServoOriginStatus(MOTOR_1) != 1)
	{
		str.Format(_T("Shift Axi Origin Error"));
		dlg.SetCaption(str);
		dlg.DoModal();
		return 1;
	}

	if(m_Robot->GetServoOriginStatus(MOTOR_2) != 1)
	{
		str.Format(_T("Theta Origin Error"));
		dlg.SetCaption(str);
		dlg.DoModal();
		return 1;
	}

	if(m_Robot->GetServoOriginStatus(MOTOR_3) != 1)
	{
		str.Format(_T("PinUp/Down Origin Error"));
		dlg.SetCaption(str);
		dlg.DoModal();
		return 1;
	}

	if(!m_AFCtrl->GerEzMoterOrigin())
	{
		str.Format(_T("EzMoter Origin Error"));
		dlg.SetCaption(str);
		dlg.DoModal();
		return 1;
	}
#endif
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//Apply
	m_Glass->SetUpdate();
	m_RecipeScan->SetUpdate();

	//First start
	if(!m_Flag.m_bFirstStart)
	{
		m_Flag.m_bFirstStart = TRUE;

		//검사시작
		if(!m_bManualStart)
		{
			SetLotInit();
		}
	}

	for(i = 0; i < MOT_PROCESS_MAX_NUM; i++)
	{
		for(j = 0; j < MOT_STATUS_MAX_NUM; j++)
		{


			m_Flag.m_bAutoStart = TRUE;
			m_MDat[i].m_Seq[j].m_bAutoStart = m_Flag.m_bAutoStart;
		}
	}

	//ECS/////////////////////////////
	//Opeation mode
	m_Inline->SetBitOModeUnitAuto(TRUE);
	m_Inline->SetBitOModeOperation_AutoOrManual(FALSE);

	//Equipment status
	typeEQPStatusDataPara eq_sts;
	typeGlassDataPara glass_data;
	glass_data = m_Inline->GetLoaderGlassData();
	CString glass_id;
	glass_id.Format(_T("%S"), glass_data.strGlassID);

	eq_sts = m_Inline->GetStatusReport();
	eq_sts.bGlassExistInUnit = m_Flag.m_bGlassExist;		//Glass유무
	if(m_Flag.m_bGlassExist)
	{
		m_Inline->SetEStatus(0);
		//if(!glass_id.IsEmpty())
		{
			//if(m_GlassManu->GetLoaderUse())
			{
				m_Inline->InitGlassPosition();
			}
		}
	}
	else
	{
		m_Inline->SetEStatus(1);
	}

	if(m_Rslt.m_bStartManualInspect)
	{
		m_btn_main_sts.SetCaption(_T("수동검사상태"));
		m_btn_main_sts.SetBackColor(RGB(255, 255, 0));
	}
	else
	{
		m_btn_main_sts.SetCaption(_T("자동검사상태"));
		m_btn_main_sts.SetBackColor(RGB(0, 255, 0));
	}

	m_bAutoSQ = TRUE;

	//20160525 ngh
	m_btn_login.SetEnabled(FALSE);
	m_btn_exit.SetEnabled(FALSE);
	m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_RED, FALSE);
	m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_GRE, TRUE);
	m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_YEL, FALSE);

	//20160805 ngh
	m_dlgGlass->CvdCadResult();
	m_dlgGlass->DamCadResult();
	m_dlgGlass->MakeCellID();

	return 0;
}

void CEMIControlDlg::SetLotInit()
{
	m_MDat[0].m_Seq[2].m_iSubSeqIdx = 100;
	m_MDat[0].m_Seq[3].m_iSubSeqIdx = 100;

	//Data 초기화
	memset(&m_Rslt, NULL, sizeof(typeProcessResult));

	for(int i = 0; i < RECIPE_SCAN_TYPE_MAX_NUM; i++)
	{
		for(int j = 0; j < IPS_MAX_GRAB_CNT; j++)
		{
			m_bGrabComp[i][j] = FALSE;
		}
	}
	
	m_Glass->SetLotInit();
}

int CEMIControlDlg::AutoStop()
{
	int i, j;	
	for(i = 0; i < MOT_PROCESS_MAX_NUM; i++)
	{
		for(j = 0; j < MOT_STATUS_MAX_NUM; j++)
		{
			m_Flag.m_bAutoStart = FALSE;
			m_MDat[i].m_Seq[j].m_bAutoStart = m_Flag.m_bAutoStart;
		}
	}

	//ECS/////////////////////////////
	//Opeation mode
	m_Inline->SetBitOModeUnitAuto(FALSE);
	m_Inline->SetBitOModeOperation_AutoOrManual(TRUE);

	typeEQPStatusDataPara eq_sts;

	eq_sts = m_Inline->GetStatusReport();

	//eq_sts.bGlassExistInUnit = m_Flag.m_bGlassExist;		//Glass유무
	//20160225 ngh
	if(m_Inline->GetInterLockError())
	{
		m_Inline->SetEStatus(2);
	}
	else
	{
		if(eq_sts.bGlassExistInUnit)
		{
			m_Inline->SetEStatus(0);
		}
		else
		{
			m_Inline->SetEStatus(1);
		}
	}
	//m_Inline->SetBitEStatusGlassExist(eq_sts.bGlassExistInUnit);

	if(m_Rslt.m_bStartManualInspect)
	{
		m_btn_main_sts.SetCaption(_T("수동검사정지상태"));
		m_btn_main_sts.SetBackColor(RGB(1, 1, 1));
	}
	else
	{
		m_btn_main_sts.SetCaption(_T("자동검사정지상태"));
		m_btn_main_sts.SetBackColor(RGB(1, 1, 1));
		m_Inline->SetGlassLdUldEx(3);
	}

	//20160525 ngh
	m_btn_login.SetEnabled(TRUE);
	m_btn_exit.SetEnabled(TRUE);

	m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_RED, FALSE);
	m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_GRE, FALSE);
	m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_YEL, TRUE);

	m_bDefectFileWriteBreak = TRUE;
	m_bDefectWriteComp = TRUE;
	m_ProcessBar->m_bProcessComp = TRUE;
	m_strMainMsg.Format(_T("Main Sequence Stop"));
	m_btn_MainMsg.SetWindowTextW(m_strMainMsg);

	return 0;
}

int CEMIControlDlg::AutoReset()
{
	int i, j;	
	int prc_idx, seq_idx, sub_seq_idx;
	prc_idx = 0;
	seq_idx = 0;
	sub_seq_idx = 0;

	for(i = 0; i < MOT_PROCESS_MAX_NUM; i++)
	{
		for(j = 0; j < MOT_STATUS_MAX_NUM; j++)
		{
			if(m_MDat[i].m_Seq[j].m_bAlarm)
			{
				prc_idx = i;
				seq_idx = j;
				sub_seq_idx =  m_MDat[i].m_Seq[j].m_iSubSeqIdx;
			}
			m_MDat[i].m_Seq[j].m_bAlarm = FALSE;
		}
	}

	int category, code, level;

	MakeAlarmCode(prc_idx, seq_idx, sub_seq_idx, &category, &code, &level);

	//ECS/////////////////////////////
	//Alarm

	if(0 == level)
	{
		m_Inline->SetLightAlarmReport(FALSE, code);
	}
	else
	{
		m_Inline->SetDownAlarmCode(code);
		m_Inline->SetHeavyAlarmReport(FALSE, code);
	}

	m_bDefectError = FALSE;
	m_bTower = FALSE;

	//Buzzer off
	m_IO->SetWriteDO(DO_BUZZER_MELODY1, FALSE);
	
	//ECS///////////////////////////////////////////////////////
	typeEQPStatusDataPara eq_sts;

	eq_sts = m_Inline->GetStatusReport();
	//eq_sts.bGlassExistInUnit = m_Flag.m_bGlassExist;		//Glass유무
	//20160225 ngh
	if(m_Inline->GetInterLockError())
	{
		m_Inline->SetEStatus(2);
	}
	else
	{
		if(eq_sts.bGlassExistInUnit)
		{
			m_Inline->SetEStatus(0);
		}
		else
		{
			m_Inline->SetEStatus(1);
		}
	}

	//20160525
	m_Trigger->SetOPMode(FALSE);

	//20160526 ngh
	KillTimer(100);
	m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_YEL, FALSE);
	m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_RED, FALSE);
	m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_YEL, TRUE);

	//m_Inline->SetBitEStatusGlassExist(eq_sts.bGlassExistInUnit);

	return 0;
}

CString CEMIControlDlg::GetResultFilePath(){return m_strResultFilePath;}



BOOL CEMIControlDlg::ReadDefectFile(int cam_idx, BOOL rotat, int scan_type_idx, int scan_idx, CString glass_id, int defectCnt)
{
	char buffer[DAT_STR_LENG], *ps, *context;
	CString strName, strNum, temp, strImagePath, strImageFilePath;
	CString strNgInfo, strLog, lot_id;
	CString strDefectIdx, strDefectWidth, strDefectHeight, strDefectCentPoX, strDefectCentPoY, strDefectArea, strGrayMin, strGrayMax, strGrayMean, strGrayDeviation, strUpDw, strBlackWhite;
	int nDefectIdx = 0;//, nDefectWidth, nDefectHeight, nDefectCentPoX, nDefectCentPoY;
    int line_cnt = 0;
	int ng_cnt = 0;
	char strFile[260];
	CTime cTime;
	typeDefectData defect_dat;
	cTime = CTime::GetCurrentTime();
	int rect_idx = 0;

	ps = NULL;

	temp.Format(_T(""));
	temp.Format(_T("%S"), m_Inline->GetUnloaderGlassData().strGlassID);
	AfxExtractSubString(glass_id, temp, 0, ' ');

	temp.Format(_T(""));
	temp.Format(_T("%S"), m_Inline->GetUnloaderGlassData().strLotID);
	AfxExtractSubString(lot_id, temp, 0, ' ');

	//strLog.Format(_T("ReadDefectFile Start(cam:%d,rotat:%d,scan_type_idx:%d,scan_idx:%d,glass_id:%s)"), cam_idx, rotat, scan_type_idx, scan_idx, glass_id);
	strLog.Format(_T("ReadDefectFile Start(lot_id:%s,cam:%d,rotat:%d,scan_type_idx:%d,scan_idx:%d,glass_id:%s)"), lot_id, cam_idx, rotat, scan_type_idx, scan_idx, glass_id);
	WriteLog(MAIN_LOG, strLog);



#ifdef MACHINE
	m_strResultFilePath.Format(_T("\\\\%s\\Result\\%04d%02d%02d\\%s\\%s\\"), m_IPSCtrl->GetIPSIP(), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id);
#else
	m_strResultFilePath.Format(_T("D:\\Result\\%04d%02d%02d\\%s\\%s\\"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id);
#endif

	if(scan_type_idx < 2)
	{
#ifdef MACHINE
	strImagePath.Format(_T("\\\\%s\\Result\\%04d%02d%02d\\%s\\%s\\Image\\%d_%d_%d\\"), m_IPSCtrl->GetIPSIP(), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx);
	temp.Format(_T("\\\\%s\\Result\\%04d%02d%02d\\%s\\%s\\Data\\%d_%d_%d\\%d_%d_%d_%s.csv"), m_IPSCtrl->GetIPSIP(), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx, cam_idx, rotat, scan_idx, glass_id);
#else
	strImagePath.Format(_T("D:\\Result\\%04d%02d%02d\\%s\\%s\\Image\\%d_%d_%d\\"),cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx);
	temp.Format(_T("D:\\Result\\%04d%02d%02d\\%s\\%s\\Data\\%d_%d_%d\\%d_%d_%d_%s.csv"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx, cam_idx, rotat, scan_idx, glass_id);
#endif
	}
	else
	{
		if(scan_type_idx == 2)
		{
#ifdef MACHINE
#ifdef PCL
			strImagePath.Format(_T("\\\\%s\\Result\\%04d%02d%02d\\%s\\%s\\Image\\CVD\\%d_%d_%d\\"), m_IPSCtrl->GetIPSIP(), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx);
			temp.Format(_T("\\\\%s\\Result\\%04d%02d%02d\\%s\\%s\\Data\\CVD\\%d_%d_%d\\%d_%d_%d_%s.csv"), m_IPSCtrl->GetIPSIP(), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx, cam_idx, rotat, scan_idx, glass_id);
#else
			strImagePath.Format(_T("\\\\%s\\Result\\%04d%02d%02d\\%s\\%s\\Image\\%d_%d_%d\\"), m_IPSCtrl->GetIPSIP(), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx);
			temp.Format(_T("\\\\%s\\Result\\%04d%02d%02d\\%s\\%s\\Data\\%d_%d_%d\\%d_%d_%d_%s.csv"), m_IPSCtrl->GetIPSIP(), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx, cam_idx, rotat, scan_idx, glass_id);
#endif
#else
#ifdef PCL
			strImagePath.Format(_T("D:\\Result\\%04d%02d%02d\\%s\\%s\\Image\\CVD\\%d_%d_%d\\"),cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx);
			temp.Format(_T("D:\\Result\\%04d%02d%02d\\%s\\%s\\Data\\CVD\\%d_%d_%d\\%d_%d_%d_%s.csv"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx, cam_idx, rotat, scan_idx, glass_id);
#else
			strImagePath.Format(_T("D:\\Result\\%04d%02d%02d\\%s\\%s\\Image\\%d_%d_%d\\"),cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx);
			temp.Format(_T("D:\\Result\\%04d%02d%02d\\%s\\%s\\Data\\%d_%d_%d\\%d_%d_%d_%s.csv"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx, cam_idx, rotat, scan_idx, glass_id);
#endif
#endif
		}
		else if(scan_type_idx == 3)
		{
			cam_idx+=1;
#ifdef MACHINE
#ifdef PCL
			strImagePath.Format(_T("\\\\%s\\Result\\%04d%02d%02d\\%s\\%s\\Image\\PCL_S\\%d_%d_%d\\"), m_IPSCtrl->GetIPSIP(), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx);
			temp.Format(_T("\\\\%s\\Result\\%04d%02d%02d\\%s\\%s\\Data\\PCL_S\\%d_%d_%d\\%d_%d_%d_%s.csv"), m_IPSCtrl->GetIPSIP(), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx, cam_idx, rotat, scan_idx, glass_id);
#else
			strImagePath.Format(_T("\\\\%s\\Result\\%04d%02d%02d\\%s\\%s\\Image\\%d_%d_%d\\"), m_IPSCtrl->GetIPSIP(),cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx);
			temp.Format(_T("\\\\%s\\Result\\%04d%02d%02d\\%s\\%s\\Data\\%d_%d_%d\\%d_%d_%d_%s.csv"), m_IPSCtrl->GetIPSIP(), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx, cam_idx, rotat, scan_idx, glass_id);
#endif
#else
#ifdef PCL
			strImagePath.Format(_T("D:\\Result\\%04d%02d%02d\\%s\\%s\\Image\\PCL_S\\%d_%d_%d\\"),cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx);
			temp.Format(_T("D:\\Result\\%04d%02d%02d\\%s\\%s\\Data\\PCL_S\\%d_%d_%d\\%d_%d_%d_%s.csv"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx, cam_idx, rotat, scan_idx, glass_id);
#else
			strImagePath.Format(_T("D:\\Result\\%04d%02d%02d\\%s\\%s\\Image\\%d_%d_%d\\"),cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx);
			temp.Format(_T("D:\\Result\\%04d%02d%02d\\%s\\%s\\Data\\%d_%d_%d\\%d_%d_%d_%s.csv"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx, cam_idx, rotat, scan_idx, glass_id);
#endif
#endif
			cam_idx-=1;
		}
		else
		{
			cam_idx+=1;
#ifdef MACHINE
			strImagePath.Format(_T("\\\\%s\\Result\\%04d%02d%02d\\%s\\%s\\Image\\PCL_L\\%d_%d_%d\\"), m_IPSCtrl->GetIPSIP(), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx);
			temp.Format(_T("\\\\%s\\Result\\%04d%02d%02d\\%s\\%s\\Data\\PCL_L\\%d_%d_%d\\%d_%d_%d_%s.csv"), m_IPSCtrl->GetIPSIP(), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx, cam_idx, rotat, scan_idx, glass_id);
#else
#ifdef PCL
			strImagePath.Format(_T("D:\\Result\\%04d%02d%02d\\%s\\%s\\Image\\PCL_L\\%d_%d_%d\\"),cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx);
			temp.Format(_T("D:\\Result\\%04d%02d%02d\\%s\\%s\\Data\\PCL_L\\%d_%d_%d\\%d_%d_%d_%s.csv"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx, cam_idx, rotat, scan_idx, glass_id);
#else
			strImagePath.Format(_T("D:\\Result\\%04d%02d%02d\\%s\\%s\\Image\\%d_%d_%d\\"),cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx);
			temp.Format(_T("D:\\Result\\%04d%02d%02d\\%s\\%s\\Data\\%d_%d_%d\\%d_%d_%d_%s.csv"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), lot_id, glass_id, cam_idx, rotat, scan_idx, cam_idx, rotat, scan_idx, glass_id);
#endif
#endif
			cam_idx-=1;
		}
	}
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
				strLog.Format(_T("ReadDefectFile Error(lot_id:%s,cam:%d,rotat:%d,scan_type_idx:%d,scan_idx:%d,glass_id:%s)"), lot_id, cam_idx, rotat, scan_type_idx, scan_idx, glass_id);
				//strLog.Format(_T("ReadDefectFile Error(lot_id:%s,cam:%d,rotat:%d,scan_type_idx:%d,scan_idx:%d,glass_id%s)"), lot_id, cam_idx, rotat, scan_type_idx, scan_idx, glass_id);
				WriteLog(MAIN_LOG, strLog);
				return FALSE;
			}
		}
	}
	 
	while (fgets(buffer, MAX_STRING_LINE, stream) != NULL)  // 텍스트파일을 한줄씩 읽습니다. 
	{
		ps = strchr(buffer, '\n');							// 제일 뒤쪽의 new line의 char을 찿아.
		if (ps != NULL) *ps = '\0';							// new line의 char을 null문자로 바꿉니다. 
	 
		if(line_cnt != 0)
		{
			if(m_bDefectFileWriteBreak)
			{
				m_bDefectFileWriteComp[0] = FALSE;
				m_bDefectFileWriteComp[1] = FALSE;
				m_bDefectFileWriteComp[2] = FALSE;
				m_bDefectFileWriteComp[3] = FALSE;
				m_bDefectFileWriteComp[4] = FALSE;
				m_bDefectFileLoadStart = FALSE;
				//m_bDefectFileWriteBreak = FALSE;
				m_strMainMsg.Format(_T("DefectFile Load & Write Break"));
				m_btn_MainMsg.SetWindowTextW(m_strMainMsg);
				break;
			}

			if(cam_idx == 0)
			{
				strNgInfo.Format(_T("%S"), buffer);

				AfxExtractSubString(strDefectIdx, strNgInfo, 0, ',');
				nDefectIdx = _wtoi(strDefectIdx) - 1;
				AfxExtractSubString(strDefectWidth, strNgInfo, 1, ',');
				AfxExtractSubString(strDefectHeight, strNgInfo, 2, ',');
				if(rotat)
				{
					defect_dat.m_dWidth =_wtof(strDefectHeight);
					defect_dat.m_dHeight =_wtof(strDefectWidth);
				}
				else
				{
					defect_dat.m_dWidth =_wtof(strDefectWidth);
					defect_dat.m_dHeight =_wtof(strDefectHeight);
				}
				
				AfxExtractSubString(strDefectCentPoX, strNgInfo, 3, ',');
				defect_dat.m_dCntX =_wtof(strDefectCentPoX);
				AfxExtractSubString(strDefectCentPoY, strNgInfo, 4, ',');
				defect_dat.m_dCntY =_wtof(strDefectCentPoY);
				AfxExtractSubString(strDefectArea, strNgInfo, 5, ',');   //sh
				defect_dat.m_dArea =_wtof(strDefectArea);
				AfxExtractSubString(strGrayMin, strNgInfo, 6, ',');
				defect_dat.m_dgraymin =_wtof(strGrayMin);
				AfxExtractSubString(strGrayMax, strNgInfo, 7, ',');  //sh
				defect_dat.m_dgraymax =_wtof(strGrayMax);
				AfxExtractSubString(strGrayMean, strNgInfo, 8, ',');  //sh
				defect_dat.m_dgraymean =_wtof(strGrayMean);
				AfxExtractSubString(strGrayDeviation, strNgInfo, 9, ',');  //sh
				defect_dat.m_dgraydeviation =_wtof(strGrayDeviation);
				memset(defect_dat.m_sImgPath, NULL, 260);
				strImageFilePath.Format(_T("%s%d_%d_%d_%04d.jpg"), strImagePath, cam_idx, rotat, scan_idx, nDefectIdx + 1);
				sprintf(defect_dat.m_sImgPath, "%S", strImageFilePath);

				AfxExtractSubString(strBlackWhite, strNgInfo, 10, ',');  //sh
				defect_dat.m_bBlackWhite =_wtoi(strBlackWhite);
				//defect_dat.m_dArea(영역추가예정)
			
				defect_dat.m_iScanIdx = scan_idx;
				defect_dat.m_iDefectIdx = nDefectIdx;
				defect_dat.m_iDefectIdxPerScan = nDefectIdx + 1;
	
				m_Glass->SetDefectData_scan(scan_type_idx, defect_dat);
			}
			else if(cam_idx >= 1)
			{
				strNgInfo.Format(_T("%S"), buffer);
				memset(&defect_dat, NULL, sizeof(typeDefectData));

				AfxExtractSubString(strDefectIdx, strNgInfo, 0, ',');
				AfxExtractSubString(strUpDw, strNgInfo, 1, ',');
				
				if(strUpDw == _T("Up"))
				{
					defect_dat.m_dWidth = 0.5;
					defect_dat.m_dHeight = 0.5;
					strImageFilePath.Format(_T("%s%d_%d_%d_%06d_T.jpg"), strImagePath, cam_idx, rotat, scan_idx, nDefectIdx);
				}
				else if(strUpDw == _T("Down"))
				{
					defect_dat.m_dWidth = 0.5;
					defect_dat.m_dHeight = 0.5;
					strImageFilePath.Format(_T("%s%d_%d_%d_%06d_B.jpg"), strImagePath,cam_idx, rotat, scan_idx, nDefectIdx);
				}				
				
				sprintf(defect_dat.m_sImgPath, "%S", strImageFilePath);

				AfxExtractSubString(strDefectCentPoX, strNgInfo, 2, ',');
				defect_dat.m_dCntX =_wtof(strDefectCentPoX);
				AfxExtractSubString(strDefectCentPoY, strNgInfo, 3, ',');
				defect_dat.m_dCntY =_wtof(strDefectCentPoY);

				defect_dat.m_iScanIdx = scan_idx;
				defect_dat.m_iDefectIdx = nDefectIdx;
				defect_dat.m_iDefectIdxPerScan = nDefectIdx + 1;
				
				m_Glass->SetDefectData_scan(scan_type_idx, defect_dat);
				nDefectIdx++;
			}

			int cn = 0;

			ps = strtok_s(buffer, ",", &context);
			while (ps) {
				if (cn == 0) strName.Format(_T("%s"), ps);
				else strNum.Format(_T("%s"), ps);
				ps = strtok_s(NULL, ",", &context);
				cn++;
			}
			TRACE("%s\r\n",buffer);
		}
		line_cnt++;
	}
	if(ps == NULL)
		delete ps;

	fclose(stream);

	strLog.Format(_T("ReadDefectFile end(lot_id:%s,cam:%d,rotat:%d,scan_type_idx:%d,scan_idx:%d,glass_id:%s)"), lot_id, cam_idx, rotat, scan_type_idx, scan_idx, glass_id);
	WriteLog(MAIN_LOG, strLog);

	return TRUE;
}

void CEMIControlDlg::OnBnClickedCheckMainMove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

BOOL CEMIControlDlg::GetEnableMove()
{
	return m_check_use_move;
}

void CEMIControlDlg::OnBnClickedCheckMainUseDefectMove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

BOOL CEMIControlDlg::GetEnableDefectMove()
{
	return m_check_defect_move;
}

BOOL CEMIControlDlg::GetDefectFileWriteBreak()
{
	return m_bDefectFileWriteBreak;
}

void CEMIControlDlg::SetDefectFileWriteBreak(BOOL isOn)
{
	m_bDefectFileWriteBreak = isOn;
}

void CEMIControlDlg::SetDefectServerFileReady(BOOL isOn)
{

}

UINT CEMIControlDlg::DefectServerThread(LPVOID pParam)
{
	CEMIControlDlg *pdlg = (CEMIControlDlg *)pParam;
	int scan_type_idx = 0;
	typeProcessBar process_bar;
	int percent;
	CString strDefectFilePath, strXmlFIleName;
	BOOL bDefectFileSave = FALSE;
	while(TRUE)
	{
		if(pdlg->m_bDefectServerStart)
		{
			if(pdlg->m_bDefectServerFileMake && pdlg->m_bDefectServerImageMake)
			{
#ifdef DEFECT
				bDefectFileSave = FALSE;
				pdlg->m_bDefectServerFileMake = FALSE;
				pdlg->m_bDefectServerFileReady = FALSE;
				pdlg->m_strMainMsg.Format(_T("DefectServer File Write Start"));
				pdlg->WriteLog(MAIN_LOG, pdlg->m_strMainMsg);
				pdlg->m_btn_MainMsg.SetWindowTextW(pdlg->m_strMainMsg);

				process_bar.m_strProcess.Format(_T("DefectServer File Write"));
				process_bar.m_nProcessType = 1;
				process_bar.m_nProcessPer = 0;
				process_bar.m_strProcessTime.Format(_T("0 sec"));
				pdlg->m_ProcessBar->m_bProcessComp = FALSE;
				pdlg->m_ProcessBar->SetProcess(process_bar);
				pdlg->m_ProcessBar->ShowWindow(SW_SHOW);

				pdlg->m_Dfserver->m_bWriteDefectFile = TRUE;
				
				//if(!pdlg->m_bDfIdxFile)
				::SendMessageA(pdlg->m_Dfserver->m_hWnd, USER_MSG_DEFECT_SERVER, CREATE_DEFECT_XML_FILE, 0);
				
				if(pdlg->m_bDfIdxFile)//else
				{
					::SendMessageA(pdlg->m_Dfserver->m_hWnd, USER_MSG_DEFECT_SERVER, XML_FILE_UPDATE, 0);  //20160615 sc 수정중
				}

				if(pdlg->m_Dfserver->GetDefectCnt() > 0)
				{
					bDefectFileSave = TRUE;
				}
				for(int i = 0; i < pdlg->m_Dfserver->GetDefectCnt(); i++)
				{
					pdlg->m_Dfserver->m_bWriteDefectFile = FALSE;
					pdlg->m_Dfserver->WriteDefectInfo(i);
	
					percent = (int)((double)((double)i / (double)pdlg->m_Dfserver->GetDefectCnt()) * 100.0);

					pdlg->m_ProcessBar->SetProcessPer(percent);
					if(pdlg->m_bDefectFileWriteBreak)
					{
						break;
					}
				}
				//if(!pdlg->m_bDfIdxFile)
				{
					::SendMessageA(pdlg->m_Dfserver->m_hWnd, USER_MSG_DEFECT_SERVER, XML_FILE_TRANSFORM, 0);
				}

				pdlg->m_strMainMsg.Format(_T("DefectServer File Write End"));
				pdlg->WriteLog(MAIN_LOG, pdlg->m_strMainMsg);

				if(pdlg->m_bDefectFileWriteBreak)
				{
					pdlg->m_ProcessBar->m_bProcessComp = TRUE;
					continue;
				}

				AfxExtractSubString(strXmlFIleName, pdlg->m_Dfserver->GetDFFilePath(), 8, '\\');
				if(!pdlg->m_Dfserver->DFServerConnect(pdlg->m_Dfserver->GetDFServerIP(), pdlg->m_Dfserver->GetDFServerID(), pdlg->m_Dfserver->GetDFServerPW(), 
						pdlg->m_Dfserver->GetDFFilePath().Mid(0, pdlg->m_Dfserver->GetDFFilePath().GetLength() - strXmlFIleName.GetLength() - 1),
						pdlg->m_Dfserver->GetDFServerRemotePath(), DF_SERVER_CONNECT_UP_LOAD))
				{
					pdlg->m_ProcessBar->m_bProcessComp = TRUE;
					if(pdlg->bManualDefectServer)
					{
						pdlg->bManualDefectServer = FALSE;
						pdlg->m_Dfserver->DFServerClose();
					}
					continue;
				}
				Sleep(100);

				pdlg->m_ProcessBar->m_bProcessComp = TRUE;

				if(pdlg->bManualDefectServer)
				{
					pdlg->bManualDefectServer = FALSE;
					pdlg->m_Dfserver->DFServerClose();
				}
#endif
			}
			else if(!pdlg->m_bDefectServerFileMake && pdlg->m_bDefectServerImageMake)
			{
				//20160707

			}
		}

		if(pdlg->m_bDefectServerEnd)
		{
			break;
		}

		Sleep(1);
	}

	return 0;
}


UINT CEMIControlDlg::DefectFileThread(LPVOID pParam)
{
	CEMIControlDlg *pdlg = (CEMIControlDlg *)pParam;
	typeProcessBar process_bar;
	double end_time, start_time, p_time;
	CRect rect;
	
	while(TRUE)
	{
		if(pdlg->m_bDefectFileLoadStart && !pdlg->m_bDefectFileWriteBreak)
		{
			pdlg->m_bDefectFileWriteBreak = FALSE;
			pdlg->m_bDefectFileLoadStart = FALSE;
			pdlg->m_strMainMsg.Format(_T("DefectFile Load and Write Start"));
			pdlg->WriteLog(MAIN_LOG, pdlg->m_strMainMsg);
			pdlg->m_btn_MainMsg.SetWindowTextW(pdlg->m_strMainMsg);
			CString glass_id, temp;
			int cam_idx,rotat;
			temp.Format(_T("%S"), pdlg->m_Inline->GetUnloaderGlassData().strGlassID);
			AfxExtractSubString(glass_id, temp, 0, ' ');

			for(int scan_type_idx = 0; scan_type_idx < RECIPE_SCAN_TYPE_MAX_NUM; scan_type_idx++)
			{
				if(pdlg->m_RecipeScan->GetInspectUse(scan_type_idx) && !pdlg->m_bDefectFileWriteBreak)
				{
					if(!pdlg->m_bDefectFileWriteComp[scan_type_idx] && !pdlg->m_bDefectFileWriteBreak)
					{
						pdlg->m_bDefectFileSave[scan_type_idx] = FALSE;
						pdlg->m_bDefectFileWriteComp[scan_type_idx] = TRUE;

						switch(scan_type_idx)
						{
						case 0:
							cam_idx = 0;
							rotat = 0;
							process_bar.m_strProcess.Format(_T("8K Degree 0, Defect File Load and Write"));
							pdlg->m_btn_scan_0.SetValue(TRUE);
							pdlg->m_btn_scan_1.SetValue(FALSE);
							pdlg->m_btn_scan_2.SetValue(FALSE);
							pdlg->m_btn_scan_3.SetValue(FALSE);
							pdlg->m_btn_scan_4.SetValue(FALSE);
							pdlg->m_iScanTypeIdx = 0;
							break;
						case 1:
							cam_idx = 0;
							rotat = 1;
							process_bar.m_strProcess.Format(_T("8K Degree 1, Defect File Load and Write"));
							pdlg->m_btn_scan_0.SetValue(FALSE);
							pdlg->m_btn_scan_1.SetValue(TRUE);
							pdlg->m_btn_scan_2.SetValue(FALSE);
							pdlg->m_btn_scan_3.SetValue(FALSE);
							pdlg->m_btn_scan_4.SetValue(FALSE);
							pdlg->m_iScanTypeIdx = 1;
							break;
						case 2:
							cam_idx = 1;
							rotat = pdlg->m_RecipeScan->GetInspect90Degree(scan_type_idx);
							process_bar.m_strProcess.Format(_T("16K Degree %d, CVD Defect File Load and Write"), rotat);
							pdlg->m_btn_scan_0.SetValue(FALSE);
							pdlg->m_btn_scan_1.SetValue(FALSE);
							pdlg->m_btn_scan_2.SetValue(TRUE);
							pdlg->m_btn_scan_3.SetValue(FALSE);
							pdlg->m_btn_scan_4.SetValue(FALSE);
							pdlg->m_iScanTypeIdx = 2;
							break;
						case 3:
							cam_idx = 1;
							rotat = 1;
							process_bar.m_strProcess.Format(_T("16K Degree %d, PCL Short Defect File Load and Write"), rotat);
							pdlg->m_btn_scan_0.SetValue(FALSE);
							pdlg->m_btn_scan_1.SetValue(FALSE);
							pdlg->m_btn_scan_2.SetValue(FALSE);
							pdlg->m_btn_scan_3.SetValue(TRUE);
							pdlg->m_btn_scan_4.SetValue(FALSE);
							pdlg->m_iScanTypeIdx = 3;
							break;
						case 4:
							cam_idx = 1;
							rotat = 0;
							process_bar.m_strProcess.Format(_T("16K Degree %d, PCL Long Defect File Load and Write"), rotat);
							pdlg->m_btn_scan_0.SetValue(FALSE);
							pdlg->m_btn_scan_1.SetValue(FALSE);
							pdlg->m_btn_scan_2.SetValue(FALSE);
							pdlg->m_btn_scan_3.SetValue(FALSE);
							pdlg->m_btn_scan_4.SetValue(TRUE);
							pdlg->m_iScanTypeIdx = 4;
							break;
						}
						::SendMessageA(pdlg->m_Glass->m_hWnd, USER_MSG_IMG_RECT, 0, 0);

						pdlg->WriteLog(MAIN_LOG, process_bar.m_strProcess);

						if(pdlg->m_Rslt.m_ScanComplete[scan_type_idx])
						//if(scan_type_idx == 0)
						{
							pdlg->m_Glass->InitDefectData(scan_type_idx, TRUE);
							//20160308 ngh
							process_bar.m_nProcessType = 1;
							process_bar.m_nProcessPer = 0;
							process_bar.m_strProcessTime.Format(_T("0 sec"));
							pdlg->m_ProcessBar->SetProcess(process_bar);
							pdlg->m_ProcessBar->ShowWindow(SW_SHOW);
							//pdlg->m_ProcessBar->GetClientRect(&rect);
							//pdlg->m_ProcessBar->MoveWindow(1510, 50, rect.Width(), rect.Height());
							//pdlg->m_ProcessBar->SetWindowPos(&CWnd::wndTopMost, 0, 850, 50, 230, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
							for(int i = 0; (i < pdlg->m_RecipeScan->GetScanCnt_coordXY(scan_type_idx)) && (i < DEFECT_MAX_SCAN_NUM) ; i++)
							{
								if(!pdlg->ReadDefectFile(cam_idx, rotat, scan_type_idx, i, glass_id, 0) && pdlg->m_bDefectFileWriteBreak)
								{
#ifdef DEFECT
									pdlg->m_Dfserver->m_bWriteDefectFile = TRUE;
#endif
									break;
								}
							}

							if(pdlg->m_bDefectFileWriteBreak)
							{
								pdlg->m_bDefectWriteComp = TRUE;
								pdlg->m_bDefectFileWriteComp[0] = FALSE;
								pdlg->m_bDefectFileWriteComp[1] = FALSE;
								pdlg->m_bDefectFileWriteComp[2] = FALSE;
								pdlg->m_bDefectFileWriteComp[3] = FALSE;
								pdlg->m_bDefectFileWriteComp[4] = FALSE;
								pdlg->m_bDefectFileLoadStart = FALSE;
								scan_type_idx = RECIPE_SCAN_TYPE_MAX_NUM;
								pdlg->m_strMainMsg.Format(_T("DefectFile Load & Write Break"));
								pdlg->WriteLog(MAIN_LOG, pdlg->m_strMainMsg);
								pdlg->m_btn_MainMsg.SetWindowTextW(pdlg->m_strMainMsg);
								break;
							}

							//Glass data변환
							pdlg->m_Glass->SetMakeDefectGlassData(scan_type_idx);

							//Glass defect display
							pdlg->m_Glass->SetDispDefectGlassData(scan_type_idx);
							Sleep(100);
							
							pdlg->m_bDefectFileSave[scan_type_idx] = TRUE;

							Sleep(100);
							if(scan_type_idx == 0)
							{
#ifdef DEFECT
								if(pdlg->m_Dfserver->GetDFWriteFile())
								{
									//디펙 서버 카운트 초기화
									//20160720 ngh
									//bManualDefectServer = TRUE;
									//m_Dfserver->InitDefectCnt();
									pdlg->m_bDefectServerFileMake = TRUE;
								}
#endif
							}

							if(scan_type_idx < 2)
							{
								pdlg->m_bDefectServerImageMake = TRUE;
							}

							pdlg->m_Glass->GlassWindowMapSave(scan_type_idx, pdlg->GetResultFilePath());

							if(!pdlg->m_Dfserver->GetWriteOnCheck())
							{
								pdlg->m_ProcessBar->m_bProcessComp = TRUE;
							}

							if(scan_type_idx < 2)
							{
#ifdef DEFECT
								pdlg->m_Dfserver->m_bWriteComp = FALSE;
								start_time = clock();
								if(pdlg->m_Dfserver->GetWriteOnCheck())
								{
									while(TRUE)
									{
										if(pdlg->m_Dfserver->GetWriteComp())
										{
											break;
										}

										if(pdlg->m_bDefectFileWriteBreak)
										{
											break;
										}

										end_time = clock();
										p_time = (end_time - start_time) / 1000;
										if(p_time >= pdlg->m_Dfserver->GetWriteTimeOut())
										{
											pdlg->m_bDefectFileWriteBreak = TRUE;
											break;
										}
									
										Sleep(1);
									}
								}

								if(pdlg->m_bDefectFileWriteBreak)
								{
									pdlg->m_bDefectWriteComp = TRUE;
									pdlg->m_bDefectFileWriteComp[0] = FALSE;
									pdlg->m_bDefectFileWriteComp[1] = FALSE;
									pdlg->m_bDefectFileWriteComp[2] = FALSE;
									pdlg->m_bDefectFileWriteComp[3] = FALSE;
									pdlg->m_bDefectFileWriteComp[4] = FALSE;
									pdlg->m_bDefectFileLoadStart = FALSE;
									//pdlg->m_bDefectFileWriteBreak = FALSE;
									scan_type_idx = RECIPE_SCAN_TYPE_MAX_NUM;
									pdlg->m_strMainMsg.Format(_T("DefectServerFile Write Time Out Error"));
									pdlg->WriteLog(MAIN_LOG, pdlg->m_strMainMsg);
									pdlg->m_btn_MainMsg.SetWindowTextW(pdlg->m_strMainMsg);
									break;
								}
#endif
								//20160313 ngh
								for(int j = 6; j >= 0; j--)
								{
									if(!pdlg->DefectErrorCnt(scan_type_idx, j))
									{
										break;
									}
								}
							}
							else if(scan_type_idx == 2)
							{
								if(!pdlg->DefectErrorCnt(scan_type_idx, 0))
								{
									break;
								}
							}

							pdlg->m_ProcessBar->m_bProcessComp = TRUE;
						}
					}
				}

				if(pdlg->m_bDefectFileWriteBreak)
				{
					pdlg->m_bDefectWriteComp = TRUE;
					pdlg->m_bDefectFileWriteComp[0] = FALSE;
					pdlg->m_bDefectFileWriteComp[1] = FALSE;
					pdlg->m_bDefectFileWriteComp[2] = FALSE;
					pdlg->m_bDefectFileWriteComp[3] = FALSE;
					pdlg->m_bDefectFileWriteComp[4] = FALSE;
					pdlg->m_bDefectFileLoadStart = FALSE;
					//pdlg->m_bDefectFileWriteBreak = FALSE;
					scan_type_idx = RECIPE_SCAN_TYPE_MAX_NUM;
					pdlg->m_strMainMsg.Format(_T("DefectFile Load & Write Break"));
					pdlg->WriteLog(MAIN_LOG, pdlg->m_strMainMsg);
					pdlg->m_btn_MainMsg.SetWindowTextW(pdlg->m_strMainMsg);
					break;
				}
				Sleep(1000);
			}

			pdlg->m_strMainMsg.Format(_T("DefectFile Load and Write End"));
			pdlg->WriteLog(MAIN_LOG, pdlg->m_strMainMsg);
			pdlg->m_btn_MainMsg.SetWindowTextW(pdlg->m_strMainMsg);
			pdlg->m_bDefectWriteComp = TRUE;
		}
		
		if(pdlg->m_bDefectFileLoadEnd)
		{
			break;
		}
		Sleep(1);
	}

	return 0;
}

BOOL CEMIControlDlg::DefectErrorCnt(int scan_type_idx, int defect_idx)
{
	BOOL rslt = TRUE;
	CString strScanType, strDefectType, msg, strLog;
	msg.Format(_T(""));
	strScanType.Format(_T(""));
	strDefectType.Format(_T(""));

	strLog.Format(_T("DefectErrorCnt() Start, Type : %d, Defect_Type : %d"), scan_type_idx, defect_idx);
	WriteLog(MAIN_LOG, strLog);

	switch(scan_type_idx)
	{
	case 0:
		strDefectType.Format(_T("8K Degre 0"));
		break;
	case 1:
		strDefectType.Format(_T("8K Degre 90"));
		break;
	case 2:
		if(m_RecipeScan->GetInspect90Degree(scan_type_idx))
		{
			strDefectType.Format(_T("16K Degre 90 CVD"));
		}
		else
		{
			strDefectType.Format(_T("16K Degre 0 CVD"));
		}
		break;
	case 3:
		strDefectType.Format(_T("16K Degre 90 PCL"));
		break;
	case 4:
		strDefectType.Format(_T("16K Degre 0 PCL"));
		break;
	}

	//if(defect_compare)
	//{
	//	//20160405 ngh
	//	if(m_Glass->GetDefectCompareCnt(scan_type_idx))
	//	{
	//		//Glass Compare defect display
	//		m_Glass->SetDispCompareDefectGlassData(scan_type_idx);
	//		msg.Format(_T("Scan Type %s, Defect Compare, Find Defect Compare Cnt %d, Compare Recipe Cnt %d"), strDefectType, 
	//			m_Glass->GetDefectCompareCnt(scan_type_idx), m_RecipeScan->GetSelectDefectErrorCnt(scan_type_idx, defect_idx));
	//		code = 305;
	//		rslt = FALSE;
	//	}
	//}
	//else
	//{	

	//20160527 ngh
	if(scan_type_idx < 2)
	{
		if(m_RecipeScan->GetSelectDefectErrorSize(scan_type_idx, defect_idx))
		{
			switch(defect_idx)
			{
			case 0:
				if(m_RecipeScan->GetDefectCntError(scan_type_idx, defect_idx, m_Rslt.m_iSumSmallCnt))
				{
					msg.Format(_T("Defect Error=D10,Scan Type %s, Defect Size Small, Find Defect Cnt %d, Compare Recipe Cnt %d"), strDefectType, 
						m_Rslt.m_iSmallCnt, m_RecipeScan->GetSelectDefectErrorCnt(scan_type_idx, defect_idx));
					rslt = FALSE;
				}
				break;
			case 1:
				if(m_RecipeScan->GetDefectCntError(scan_type_idx, defect_idx, m_Rslt.m_iSumMediumCnt))
				{
					msg.Format(_T("Defect Error=D11,Scan Type %s, Defect Size Medium, Find Defect Cnt %d, Compare Recipe Cnt %d"), strDefectType,
						m_Rslt.m_iMediumCnt, m_RecipeScan->GetSelectDefectErrorCnt(scan_type_idx, defect_idx));
					rslt = FALSE;
				}
				break;
			case 2:
				if(m_RecipeScan->GetDefectCntError(scan_type_idx, defect_idx, m_Rslt.m_iSumLargeCnt))
				{
					msg.Format(_T("Defect Error=D12,Scan Type %s, Defect Size Large, Find Defect Cnt %d, Compare Recipe Cnt %d"), strDefectType,
						m_Rslt.m_iLargeCnt, m_RecipeScan->GetSelectDefectErrorCnt(scan_type_idx, defect_idx));
					rslt = FALSE;
				}
				break;
			case 3:
				if(m_RecipeScan->GetDefectCntError(scan_type_idx, defect_idx, m_Rslt.m_iSumOverflowCnt))
				{
					msg.Format(_T("Defect Error=D13,Scan Type %s, Defect Size Overflow, Find Defect Cnt %d, Compare Recipe Cnt %d"), strDefectType,
						m_Rslt.m_iOverflowCnt, m_RecipeScan->GetSelectDefectErrorCnt(scan_type_idx, defect_idx));
					rslt = FALSE;
				}
				break;
			case 4:
				if(m_RecipeScan->GetDefectCntError(scan_type_idx, defect_idx, m_Rslt.m_iWhiteCnt))
				{
					msg.Format(_T("Defect Error=D14,Scan Type %s, BlackWhite Defect, Find Defect Cnt %d, Compare Recipe Cnt %d"), strDefectType,
						m_Rslt.m_iWhiteCnt, m_RecipeScan->GetSelectDefectErrorCnt(scan_type_idx, defect_idx));
					rslt = FALSE;
				}
				break;
			case 5:
				if(m_Glass->GetDefectCompareCnt(scan_type_idx) >= m_RecipeScan->GetMaskDefectCnt(scan_type_idx))
				{
					//Glass Compare defect display
					//m_Glass->SetDispCompareDefectGlassData(scan_type_idx);
					msg.Format(_T("Defect Error=D15,Scan Type %s, Defect Compare, Find Defect Compare Cnt %d, Compare Recipe Cnt %d"), strDefectType, 
						m_Glass->GetDefectCompareCnt(scan_type_idx), m_RecipeScan->GetSelectDefectErrorCnt(scan_type_idx, defect_idx));
					rslt = FALSE;
				}
				break;
			case 6:
				if(!OFDefectError(scan_type_idx))
				{
					msg.Format(_T("Defect Error=D16,Scan Type %s, OF Defect, Find OF Defect Cnt %d"), strDefectType, 
						m_Glass->GetOFDefectCnt(scan_type_idx));
					rslt = FALSE;
				}
				break;
			}
		}
	}
	else
	{
		switch(defect_idx)
		{
		case 0:
			if(!m_Glass->GetCVDRslt())
			{
				msg.Format(_T("Defect Error=D17,Scan Type %s, Find Defect Cnt %d"), strDefectType, 
						m_Glass->GetCVDDefectCnt());
				rslt = FALSE;
			}
			break;
		}
	}

	if(!rslt)
	{
		WriteLog(MAIN_LOG, msg);
#ifdef MACHINE
		AutoStop();

		m_bTower = TRUE;
		m_IO->SetWriteDO(DO_BUZZER_MELODY1, TRUE);
		//20160526 ngh
		m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_RED, FALSE);
		m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_GRE, FALSE);
		m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_YEL, FALSE);
		SetTimer(100, 500, NULL);
#endif
		m_bDefectError = TRUE;
		sprintf(m_cDefectError, "%S", msg);
		SendMessage(USER_MSG_MOT, 1, 0);
	}
	strLog.Format(_T("DefectErrorCnt() End, Type : %d, Defect_Type : %d"), scan_type_idx, defect_idx);
	WriteLog(MAIN_LOG, strLog);

	return rslt;
}

int CEMIControlDlg::GetRsltDefectSumCnt(int defect_idx)  //20160418 sc
{
	int rslt = 0;
	switch(defect_idx)
	{
	case 0:
		rslt = m_Rslt.m_iSumSmallCnt;
		break;
	case 1:
		rslt = m_Rslt.m_iSumMediumCnt;
		break;
	case 2:
		rslt = m_Rslt.m_iSumLargeCnt;
		break;
	case 3:
		rslt = m_Rslt.m_iSumOverflowCnt;
		break;
	case 4:
		rslt = m_Rslt.m_iSumCnt;
		break;
	}
	return rslt;
}

int CEMIControlDlg::GetRsltDefectCnt(int defect_idx)  //20160418 sc
{
	int rslt = 0;
	switch(defect_idx)
	{
	case 0:
		rslt = m_Rslt.m_iSmallCnt;
		break;
	case 1:
		rslt = m_Rslt.m_iMediumCnt;
		break;
	case 2:
		rslt = m_Rslt.m_iLargeCnt;
		break;
	case 3:
		rslt = m_Rslt.m_iOverflowCnt;
		break;
	case 4:
		rslt = m_Rslt.m_iPatCnt;
		break;
	}
	return rslt;
}

int CEMIControlDlg::GetRsltDefectWhiteCnt(int defect_idx)  //20160418 sc
{
	int rslt = 0;
	switch(defect_idx)
	{
	case 0:
		rslt = m_Rslt.m_iWhiteSmallCnt;
		break;
	case 1:
		rslt = m_Rslt.m_iWhiteMediumCnt;
		break;
	case 2:
		rslt = m_Rslt.m_iWhiteLargeCnt;
		break;
	case 3:
		rslt = m_Rslt.m_iWhiteOverflowCnt;
		break;
	case 4:
		rslt = m_Rslt.m_iWhiteCnt;
		break;
	}
	return rslt;
}
void CEMIControlDlg::ClickBtnenhctrlMainLoadDefectMap()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	/*m_bDefectFileWriteBreak = FALSE;
	m_Glass->SetMakeDefectGlassData(2);*/

	/*CString strLog;
	strLog.Format(_T("SetMakeDefectGlassData Start)"));
	WriteLog(GLASS_LOG, strLog);

	int a = 0;
	for(int i = 0; i < 13000; i++)
	{
		for(int j = 0; j < 600; j++)
		{
			a+=1;
		}
	}

	strLog.Format(_T("SetMakeDefectGlassData End)"));
	WriteLog(GLASS_LOG, strLog);*/
	BOOL bRet = FALSE;
	

	//20160307 ngh
	m_Glass->SetLotInit();

	bManualDefectServer = TRUE;
#ifdef DEFECT
	m_Dfserver->InitDefectCnt();
	CString temp;
	AfxExtractSubString(temp, m_Dfserver->GetDFFilePath(), 8, '\\');
	if(bRet = m_Dfserver->GetWriteOnCheck())//if(m_Dfserver->GetDefectFileUpload())
	{
		AfxExtractSubString(temp, m_Dfserver->GetDFFilePath(), 8, '\\');
		if(!m_Dfserver->DFServerConnect(m_Dfserver->GetDFServerIP(), m_Dfserver->GetDFServerID(), 
			m_Dfserver->GetDFServerPW(), m_Dfserver->GetDFFilePath().Mid(0, m_Dfserver->GetDFFilePath().GetLength() - temp.GetLength() - 1),
			m_Dfserver->GetDFServerRemotePath(), 0))
		{
			bManualDefectServer = FALSE;
			AfxMessageBox(_T("Server Connect Error"));
			return;
		}
	}
//	m_bDfIdxFile = m_Dfserver->DefectFileUpdate();

//	if(m_GlassManu->GetLoaderUse() && m_Dfserver->GetWriteOnCheck())
//	{
//		m_bDfIdxFile = m_Dfserver->DefectFileUpdate();
//	}
#endif
	for(int i = 0; i < RECIPE_SCAN_TYPE_MAX_NUM; i++)
	{
		m_Rslt.m_ScanComplete[i] = FALSE;
		if(m_RecipeScan->GetInspectUse(i))
		{
			m_Rslt.m_ScanComplete[i] = TRUE;
		}
	}

	
//	m_bDfIdxFile = FALSE;

	/*if(m_RecipeScan->GetInspectUse(0))
	{
		m_Rslt.m_ScanComplete[0] = TRUE;
	}
	if(m_RecipeScan->GetInspectUse(1))
	{
		m_Rslt.m_ScanComplete[1] = TRUE;
	}
	if(m_RecipeScan->GetInspectUse(2))
	{
		m_Rslt.m_ScanComplete[2] = TRUE;
	}*/
	//m_bDefectServerFileMake = TRUE;

	ResetRsltSizeCntView();
	m_DefectList->GetList()->DeleteAllItems();
	m_bAutoSQ = FALSE;
	m_bDefectFileWriteComp[0] = FALSE;
	m_bDefectFileWriteComp[1] = FALSE;
	m_bDefectFileWriteComp[2] = FALSE;
	m_bDefectFileWriteComp[3] = FALSE;
	m_bDefectFileWriteComp[4] = FALSE;
	m_bDefectFileWriteBreak = FALSE;
	m_bDefectFileLoadStart = TRUE;

	//Read data
	//int i;
	//int cam_idx = 0;
	//int scan_type_idx;
	//CString glass_code;
	//glass_code.Format(_T("%d"), m_Inline->GetLoaderGlassData().nGlassCode);

	//ResetRsltSizeCntView();

	//8K////////////////////////////////////////////////////////////////////////////////////////
	//scan_type_idx = 0;
	//if(m_Rslt.m_ScanComplete[scan_type_idx])
	//{
	//	for(i = 0; (i < m_RecipeScan->GetScanCnt_coordXY(scan_type_idx)) && (i < DEFECT_MAX_SCAN_NUM) ; i++)
	//	{
	//		ReadDefectFile(cam_idx, 0, scan_type_idx, i, glass_code, 0);
	//	}

	//	Glass data변환
	//	m_Glass->SetMakeDefectGlassData(scan_type_idx);
	//}

	//8K 90/////////////////////////////////////////////////////////////////////////////////////
	//scan_type_idx = 1;
	//if(m_Rslt.m_ScanComplete[scan_type_idx])
	//{
	//	for(i = 0; (i < m_RecipeScan->GetScanCnt_coordXY(scan_type_idx)) && (i < DEFECT_MAX_SCAN_NUM) ; i++)
	//	{
	//		ReadDefectFile(cam_idx, 1, scan_type_idx, i, glass_code, 0);
	//	}

	//	Glass data변환
	//	m_Glass->SetMakeDefectGlassData(scan_type_idx);
	//}

	//16K///////////////////////////////////////////////////////////////////////////////////////
	//scan_type_idx = 2;
	//cam_idx = 1;
	//if(m_Rslt.m_ScanComplete[scan_type_idx])
	//{
	//	for(i = 0; (i < m_RecipeScan->GetScanCnt_coordXY(scan_type_idx)) && (i < DEFECT_MAX_SCAN_NUM) ; i++)
	//	{
	//		ReadDefectFile(cam_idx, m_RecipeScan->GetInspect90Degree(2), scan_type_idx, i, glass_code, 0);
	//	}

	//	Glass data변환
	//	m_Glass->SetMakeDefectGlassData(scan_type_idx);
	//}
	//
	//Glass defect display
	//m_Glass->SetDispDefectGlassData(m_iScanTypeIdx);
}


void CEMIControlDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int i;
	if(bShow)
	{
		for(i = 0; i < RECIPE_SCAN_TYPE_MAX_NUM; i++)
		{
			switch(i)
			{
			case 0:
				m_btn_scan_0.SetEnabled(m_RecipeScan->GetInspectUse(i));
				break;
			case 1:
				m_btn_scan_1.SetEnabled(m_RecipeScan->GetInspectUse(i));
				break;
			case 2:
				m_btn_scan_2.SetEnabled(m_RecipeScan->GetInspectUse(i));
				break;
			case 3:
				m_btn_scan_3.SetEnabled(m_RecipeScan->GetInspectUse(i));
				break;
			case 4:
				m_btn_scan_4.SetEnabled(m_RecipeScan->GetInspectUse(i));
				break;
			default:
				AfxMessageBox(_T("Error : invalid index(CEMIControlDlg::OnShowWindow)!"));
				break;
			}
		}
	}
	else
	{

	}
}

void CEMIControlDlg::ReviewCCD()
{
	CRect rect;

	m_dlgCCD->GetWindowRect(rect);
	m_dlgCCD->MoveWindow(925, 320, rect.Width(), rect.Height(), TRUE);
	m_dlgCCD->ShowWindow(SW_SHOW);
}


int CEMIControlDlg::StopModeOperation(int *prc_idx, int *seq_idx)
{
	*prc_idx = 0;
	*seq_idx = 0;

	if(m_MDat[*prc_idx].m_Seq[*seq_idx].m_bAlarm)
	{
		return 1;
	}

#ifdef MACHINE
	//Safe and limit sensor check
	if(m_IO->GetReadDI(DI_EMI_OP))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_EMI_OP;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "EMI Emergency On Error=X51B : EMO OP(EMO 누름 감지)!");

		return 0;
	}

	if(m_IO->GetReadDI(DI_EMI_LEFT))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_EMI_LEFT;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "EMI Emergency On Error=X51C : EMO LEFT(EMO 누름 감지)!");

		return 0;
	}

	if(m_IO->GetReadDI(DI_EMI_BACK))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_EMI_BACK;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "EMI Emergency On Error=X51D : EMO BACK(EMO 누름 감지)!");

		return 0;
	}

	if(m_IO->GetReadDI(DI_EMI_INSIDE))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_EMI_INSIDE;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "EMI Emergency On Error=X51E : EMO INSIDE(EMO 누름 감지)!");

		return 0;
	}

	/*사용안함
	if(m_IO->GetReadDI(DI_LOADER_EMO_SIG))//사용안함
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_LOADER_EMO_SIG;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "EMI Emergency On Error=X500 : EMO INSIDE(EMO 누름 감지)!");

		return 0;
	}
	*/

	if(!m_IO->GetReadDI(DI_STAGE_MC1_ON))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_STAGE_MC1_ON;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "Stage MC Off Alarm=X501 : STAGE MC1 ALARM(센서 감지않됨)!");

		return 0;
	}

	if(!m_IO->GetReadDI(DI_STAGE_MC2_ON))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_STAGE_MC2_ON;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "Stage MC Off Alarm=X502 : STAGE MC2 ALARM(센서 감지않됨)!");

		return 0;
	}

	//if(!m_IO->GetReadDI(DI_SAFETY_UNIT_MC_1_2_ON))
	if(0)//사용안함
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_SAFETY_UNIT_MC_1_2_ON;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "Safety MC Off Alarm=X510 : SAFETY UNIT MC 1, 2 감지않됨(센서 감지않됨)!");

		return 0;
	}

	//if(!m_IO->GetReadDI(DI_SAFETY_UNIT_MC_3_4_ON))
	if(0)//사용안함
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_SAFETY_UNIT_MC_3_4_ON;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "Safety MC Off Alarm=X512 : SAFETY UNIT MC 3, 4 감지않됨(센서 감지않됨)!");

		return 0;
	}

	if(!m_IO->GetReadDI(DI_OP_COOLING_FAN_ALARM))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_OP_COOLING_FAN_ALARM;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "FAN ALARM 1=X53E : OUTSIDE COOLING FAN ALARM(센서 감지않됨)!");

		return 0;
	}

	if(!m_IO->GetReadDI(DI_REAR_COOLING_FAN_ALARM))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_REAR_COOLING_FAN_ALARM;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "FAN ALARM 2=X53F : INSIDE COOLING FAN ALARM(센서 감지않됨)!");

		return 0;
	}

	//20160310 sh
	if(!m_IO->GetReadDI(DI_ELCB_1_TRIP_MAINT_LAMP))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_ELCB_1_TRIP_MAINT_LAMP;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "ELC ALARM 2=X520 : ELCB 01 TRIP MAINT LAMP(센서 감지않됨)!-11-");	

		return 0;
	}
			
	if(!m_IO->GetReadDI(DI_ELCB_2_TRIP_UPS_PL_LAMP))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_ELCB_2_TRIP_UPS_PL_LAMP;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "ELC ALARM 2=X526 : ELCB 02 TRIP UPS PL LAMP(센서 감지않됨)!-11-");	
		return 0;
	}
			
	if(!m_IO->GetReadDI(DI_ELCB_3_TRIP_EFU))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_ELCB_3_TRIP_EFU;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "ELC ALARM 2=X521 : ELCB 03 TRIP EFU(센서 감지않됨)!-11-");	

		return 0;
	}


	if(!m_IO->GetReadDI(DI_ELCB_4_TRIP_METAL_SRC))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_ELCB_4_TRIP_METAL_SRC;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "ELC ALARM 2=X522 : ELCB 04 TRIP METAL SOURCE(센서 감지않됨)!-11-");	

		return 0;
	}
	if(!m_IO->GetReadDI(DI_ELCB_5_TRIP_PC_CONCENT))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_ELCB_5_TRIP_PC_CONCENT;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "ELC ALARM 2=X523 : ELCB 05 TRIP PC CONCENT(센서 감지않됨)!-11-");	

		return 0;
	}
	if(!m_IO->GetReadDI(DI_ELCB_6_TRIP_MAIN_PC_TRIGGER))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_ELCB_6_TRIP_MAIN_PC_TRIGGER;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "ELC ALARM 2=X527 :ELCB 06 TRIP MAIN PC, TRIGGER(센서 감지않됨)!-11-");	


		return 0;
	}
	if(!m_IO->GetReadDI(DI_ELCB_7_TRIP_MOD_PC))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_ELCB_7_TRIP_MOD_PC;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "ELC ALARM 2=X528 : ELCB 07 TRIP MODULE PC(센서 감지않됨)!-11-");	


		return 0;
	}

	if(!m_IO->GetReadDI(DI_ELCB_8_TRIP_REVIEW_AF))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_ELCB_8_TRIP_REVIEW_AF;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "ELC ALARM 2=X525 :ELCB 08 TRIP REVIEW, AF(센서 감지않됨)!-11-");	

		return 0;
	}
	if(!m_IO->GetReadDI(DI_ACP1_TRIP_AVC_METER))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_ACP1_TRIP_AVC_METER;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "APC ALARM 2=X529 : ACP01 TRIP AVC METER(센서 감지않됨)!-11-");	

		return 0;
	}
	if(!m_IO->GetReadDI(DI_ACP2_TRIP_AVC_POW_LAMP))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_ACP2_TRIP_AVC_POW_LAMP;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "APC ALARM 2=X52A : ACP02 TRIP AVC POWER LAMP(센서 감지않됨)!-11-");	

		return 0;
	}
	if(!m_IO->GetReadDI(DI_ACP3_4_TRIP_MAINT_LAMP))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_ACP3_4_TRIP_MAINT_LAMP;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "APC ALARM 2=X52B : ACP03, ACP04 TRIP MAINT LAMP(센서 감지않됨)!-11-");	

		return 0;
	}
	if(!m_IO->GetReadDI(DI_ACP5_TRIP_PC_RACK_FAN))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_ACP5_TRIP_PC_RACK_FAN;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "APC ALARM 2=X52C : ACP05 TRIP PC RACK FAN(센서 감지않됨)!-11-");	

		return 0;
	}
	if(!m_IO->GetReadDI(DI_ACP6_17_18_TRIP_REVIEW_AF))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_ACP6_17_18_TRIP_REVIEW_AF;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "APC ALARM 2=X52D : ACP06, ACP17, ACP18 TRIP REVIEW, AF(센서 감지않됨)!-11-");	

		return 0;
	}
	if(!m_IO->GetReadDI(DI_ACP7_8_16_TRIP_ALIGN_CAM))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_ACP7_8_16_TRIP_ALIGN_CAM;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "APC ALARM 2=X52E : ACP07, ACP08, ACP16 TRIP ALIGN CAMERA(센서 감지않됨)!-11-");	

		return 0;
	}
	if(!m_IO->GetReadDI(DI_ACP9_12_13_TRIP_OPTIC_MOD1))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_ACP9_12_13_TRIP_OPTIC_MOD1;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "APC ALARM 2=X52F : ACP09, ACP12, ACP13 TRIP OPTIC MODULE 1(센서 감지않됨)!-11-");	

		return 0;
	}
	if(!m_IO->GetReadDI(DI_ACP10_14_15_TRIP_OPTIC_MOD2))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_ACP10_14_15_TRIP_OPTIC_MOD2;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "APC ALARM 2=X520 : ACP10, ACP14, ACP15 TRIP OPTIC MODULE 2(센서 감지않됨)!-11-");	

		return 0;
	}
	if(!m_IO->GetReadDI(DI_UCP1_TRIP_UPS_METER))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_UCP1_TRIP_UPS_METER;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "UCP ALARM 2=X531 :UCP01 TRIP UPS METER(센서 감지않됨)!-11-");	

		return 0;
	}
	if(!m_IO->GetReadDI(DI_UCP2_TRIP_UPS_POW_LAMP))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_UCP2_TRIP_UPS_POW_LAMP;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "UCP ALARM 2=X532 :UCP02 TRIP UPS POWER LAMP(센서 감지않됨)!-11-");	

		return 0;
	}
	if(!m_IO->GetReadDI(DI_UCP3_4_TRIP_SAFETY))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_UCP3_4_TRIP_SAFETY;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "UCP ALARM 2=X533 :UCP03 UCP04 TRIP SAFETY(센서 감지않됨)!-11-");	

		return 0;
	}

	if(!m_IO->GetReadDI(DI_UCP7_8_TRIP_MC_COIL))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_UCP7_8_TRIP_MC_COIL;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "UCP ALARM 2=X534 :UCP07, UCP08 TRIP MC COIL(센서 감지않됨)!-11-");	

		return 0;
	}
	if(!m_IO->GetReadDI(DI_UCP12_TRIP_IONIZER))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_UCP12_TRIP_IONIZER;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "UCP ALARM 2=X535 :UCP12 TRIP IONIZER(센서 감지않됨)!-11-");	

		return 0;
	}
	if(!m_IO->GetReadDI(DI_UCP13_TRIP_PANEL_PAN))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_UCP13_TRIP_PANEL_PAN;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "UCP ALARM 2=X536 :UCP13 TRIP PANEL FAN(센서 감지않됨)!-11-");	

		return 0;
	}
	//20160310 sh
	/*사용안함
	if(!m_IO->GetReadDI(DI_DO1_OPEN_LEFT))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_DO1_OPEN_LEFT;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "X514 : DOOR#1 LEFT(DOOR 열림 감지)!");

		return 0;
	}


	if(!m_IO->GetReadDI(DI_DO1_OPEN_RIGHT))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_DO1_OPEN_RIGHT;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "X515 : DOOR#1 RIGHT(DOOR 열림 감지)!");

		return 0;
	}
	

	if(!m_IO->GetReadDI(DI_DO2_OPEN_LEFT))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_DO2_OPEN_LEFT;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "X516 : DOOR#2 LEFT(DOOR 열림 감지)!");

		return 0;
	}

		if(!m_IO->GetReadDI(DI_DO2_OPEN_RIGHT))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_DO2_OPEN_RIGHT;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "X517 : DOOR#2 RIGHT(DOOR 열림 감지)!");

		return 0;
	}

	if(!m_IO->GetReadDI(DI_DO3_OPEN_LEFT))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_DO3_OPEN_LEFT;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "X518 : DOOR#3 LEFT(DOOR 열림 감지)!");

		return 0;
	}

	if(!m_IO->GetReadDI(DI_DO3_OPEN_RIGHT))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = DI_DO3_OPEN_RIGHT;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "X519 : DOOR#3 RIGHT(DOOR 열림 감지)!");

		return 0;
	}
	*/
#endif

	return 1;
}

void CEMIControlDlg::ClickBtnenhctrlMainCameraScanImage()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//int a = 810;
	//m_Rslt.m_iFilmAlignColIdx = 0;
	//m_Rslt.m_iFilmAlignRgnIdx = 0;
	//m_Rslt.m_iFilmAlignRowIdx = 0;
	//while(a < 890)
	//{
	//	switch(a)
	//	{
	//	//Review film align동작 시작///////////////////////////////////////////////////
	//	case 810:
	//		//Move film align inspect pos.
	//		m_FilmAlign->SetRegionMove(m_Rslt.m_iFilmAlignRowIdx + 1, m_Rslt.m_iFilmAlignColIdx + 1, m_Rslt.m_iFilmAlignRgnIdx, TRUE);
	//		a=820;
	//		break;
	//	case 820:
	//		//Move end(scan)
	//		
	//				a=830;
	//		break;	
	//	case 830:
	//		//Move end(shift)
	//		
	//				a=840;
	//		break;
	//	case 840:
	//		//시간 대기
	//			a=850;
	//			//영상촬상
	//		break;
	//	case 850:
	//		//검사

	//		a=860;
	//		break;
	//	case 860:
	//		
	//		(m_Rslt.m_iFilmAlignRgnIdx)++;
	//		//Region index확인
	//		if(m_Rslt.m_iFilmAlignRgnIdx < FILM_ALIGN_RGN_POS_MAX_MUM)
	//		{
	//			
	//			a=810;
	//		}
	//		else
	//		{
	//			//한개 필름 끝남
	//			a=870;
	//			(m_Rslt.m_FilmAlignIdx)++;
	//		}
	//		break;
	//	case 870:
	//		(m_Rslt.m_iFilmAlignColIdx)++;
	//		//Film col index확인
	//		if(m_Rslt.m_iFilmAlignColIdx < m_RecipeCell->GetFilmCntX_coordXY())
	//		{
	//			m_Rslt.m_iFilmAlignRgnIdx = 0;
	//			a=810;
	//		}
	//		else
	//		{
	//			//한개의 col 끝남
	//			a=880;
	//		}
	//		break;
	//	case 880:
	//		//Film row index확인
	//		(m_Rslt.m_iFilmAlignRowIdx)++;
	//		if(m_Rslt.m_iFilmAlignRowIdx < m_RecipeCell->GetFilmCntY_coordXY())
	//		{
	//			m_Rslt.m_iFilmAlignColIdx = 0;
	//			m_Rslt.m_iFilmAlignRgnIdx = 0;
	//			a=810;
	//		}
	//		else
	//		{
	//			//ClickBtnenhctrlMainSaveFilmalign();
	//			
	//			m_strMainMsg.Format(_T("Film align End"));
	//			a=890;
	//		}
	//		break;
	//	}
	//}


	m_bDefectFileWriteBreak = TRUE;

	if(m_btn_check_ccd.GetValue())
	{
		ReviewCCD();
	}
	else
	{
		m_CCD->ShowWindow(SW_HIDE);
	}
}

//20151210 ngh
void CEMIControlDlg::SetRsltSizeCntView(int defect_size, BOOL bBlackWhite)
{
	CString data;
	long num = 0;
	int idx = 0;
	if(bBlackWhite)
	{
		idx = 1;
	}
	switch(defect_size)
	{
	case DEFECT_SIZE_SMALL:
		if(bBlackWhite)
		{
			num = ++m_Rslt.m_iWhiteSmallCnt;
		}
		else
		{
			num = ++m_Rslt.m_iSmallCnt;
		}
		data.Format(_T("%d"), num);
		m_list_ins_rslt.SetItemText(idx, 1, data);
		break;
	case DEFECT_SIZE_MEDIUM:
		if(bBlackWhite)
		{
			num = ++m_Rslt.m_iWhiteMediumCnt;
		}
		else
		{
			num = ++m_Rslt.m_iMediumCnt;
		}
		data.Format(_T("%d"), num);
		m_list_ins_rslt.SetItemText(idx, 2, data);
		break;
	case DEFECT_SIZE_LARGE:
		if(bBlackWhite)
		{
			num = ++m_Rslt.m_iWhiteLargeCnt;
		}
		else
		{
			num = ++m_Rslt.m_iLargeCnt;
		}
		data.Format(_T("%d"), num);
		m_list_ins_rslt.SetItemText(idx, 3, data);
		break;
	case DEFECT_SIZE_OVER_FLOW:
		if(bBlackWhite)
		{
			num = ++m_Rslt.m_iWhiteOverflowCnt;
		}
		else
		{
			num = ++m_Rslt.m_iOverflowCnt;
		}
		data.Format(_T("%d"), num);
		m_list_ins_rslt.SetItemText(idx, 4, data);
		break;
	default:

		break;
	}

	m_Rslt.m_iPatCnt = m_Rslt.m_iSmallCnt + m_Rslt.m_iMediumCnt + m_Rslt.m_iLargeCnt + m_Rslt.m_iOverflowCnt;
	data.Format(_T("%d"), m_Rslt.m_iPatCnt);
	m_list_ins_rslt.SetItemText(0, 5, data);

	m_Rslt.m_iWhiteCnt = m_Rslt.m_iWhiteSmallCnt + m_Rslt.m_iWhiteMediumCnt + m_Rslt.m_iWhiteLargeCnt + m_Rslt.m_iWhiteOverflowCnt;
	data.Format(_T("%d"), m_Rslt.m_iWhiteCnt);
	m_list_ins_rslt.SetItemText(1, 5, data);

	m_Rslt.m_iSumSmallCnt = m_Rslt.m_iSmallCnt + m_Rslt.m_iWhiteSmallCnt;
	data.Format(_T("%d"), m_Rslt.m_iSumSmallCnt);
	m_list_ins_rslt.SetItemText(2, 1, data);

	m_Rslt.m_iSumMediumCnt = m_Rslt.m_iMediumCnt + m_Rslt.m_iWhiteMediumCnt;
	data.Format(_T("%d"), m_Rslt.m_iSumMediumCnt);
	m_list_ins_rslt.SetItemText(2, 2, data);

	m_Rslt.m_iSumLargeCnt = m_Rslt.m_iLargeCnt + m_Rslt.m_iWhiteLargeCnt;
	data.Format(_T("%d"), m_Rslt.m_iSumLargeCnt);
	m_list_ins_rslt.SetItemText(2, 3, data);

	m_Rslt.m_iSumOverflowCnt = m_Rslt.m_iOverflowCnt + m_Rslt.m_iWhiteOverflowCnt;
	data.Format(_T("%d"), m_Rslt.m_iSumOverflowCnt);
	m_list_ins_rslt.SetItemText(2, 4, data);

	m_Rslt.m_iSumCnt = m_Rslt.m_iPatCnt + m_Rslt.m_iWhiteCnt;
	data.Format(_T("%d"), m_Rslt.m_iSumCnt);
	m_list_ins_rslt.SetItemText(2, 5, data);

	//data.Format(_T("%d"), m_Rslt.m_iPatCnt);
	//m_list_ins_rslt.SetItemText(0, 7, data);
}

void CEMIControlDlg::ResetRsltSizeCntView()
{
	CString data;
	m_Rslt.m_iSmallCnt = 0;
	m_Rslt.m_iMediumCnt = 0;
	m_Rslt.m_iLargeCnt = 0;
	m_Rslt.m_iOverflowCnt = 0;
	m_Rslt.m_iPatCnt = 0;

	m_Rslt.m_iWhiteSmallCnt = 0;
	m_Rslt.m_iWhiteMediumCnt = 0;
	m_Rslt.m_iWhiteLargeCnt = 0;
	m_Rslt.m_iWhiteOverflowCnt = 0;
	m_Rslt.m_iWhiteCnt = 0;
	

	for(int i = 0; i < 3; i++)
	{
		for(int j = 1; j < 6; j++)
		{
			data.Format(_T("0"));
			m_list_ins_rslt.SetItemText(i, j, data);
		}
	}
}

BOOL CEMIControlDlg::WriteInsResultFile(FILE *fp, CString file_path, CString result_data)
{
	BOOL result = FALSE;

	result = fseek(fp, 0L, SEEK_END);
    if(result)
	{
		return FALSE;
	}

	fwrite(result_data, result_data.GetLength() + 1, 2, fp);

	if(m_bDefectFileLoadEnd)
	{
		return FALSE;
	}

	return TRUE;
}


void CEMIControlDlg::ClickBtnenhctrlMainSaveFilmalign()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_strMainMsg.Format(_T("Save Film Align Start"));
	m_btn_MainMsg.SetWindowTextW(m_strMainMsg);
	CString file_path, data, temp, file_name, strGlassID, strLot_id, path;
	CString strGlass_id, strTime, strFilm_id, strPosition, strResult_x, strResult_y;
	double cad_x = 0, cad_y = 0, measure_x = 0, measure_y = 0;
	double  delta_x = 0, delta_y = 0;
	int ispec_y = 0, ispec_x = 0, idx = 0, sub_idx = 0;
	int i,j,film_grab_point;
	m_dlgGlass->m_iFilm_id_a = 1;
	m_dlgGlass->m_iFilm_id_b = 1;
	film_grab_point = 0;
	FILE *fp;
	char strFile[512];
	int j_count = 0;
	
				
	strGlass_id.Empty();
	strTime.Empty();
	strFilm_id.Empty();
	strPosition.Empty();
	strResult_x.Empty();
	strResult_y.Empty();

	SYSTEMTIME time;
	GetLocalTime(&time);

	//strGlassID.Format(_T("%S"), m_Inline->GetLoaderGlassData().strGlassID);
	temp.Format(_T("%S"), m_Inline->GetUnloaderGlassData().strGlassID);
	AfxExtractSubString(strGlassID, temp, 0, ' ');
	temp.Format(_T("%S"), m_Inline->GetUnloaderGlassData().strLotID);
	AfxExtractSubString(strLot_id, temp, 0, ' ');

	strTime.Format(_T("%04d-%02d-%02d-%02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	file_name.Format(_T("FilmAlign_%04d%02d%02d_%02d%02d%02d.csv"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	//file_path.Format(_T("%sFilmAlign_%s.csv"), FILM_RESULT_FILEPATH, strGlassID);
	path.Format(_T("\\\\%s\\RESULT\\%04d%02d%02d\\%s\\%s\\FilmAlign"), m_dlgIPSCtrl->GetIPSIP(), time.wYear, time.wMonth, time.wDay, 
		strLot_id, strGlassID);
	file_path.Format(_T("%s\\%s"), path, file_name);

	_MakeDirectories(path);

	//20160126 ngh
	if(m_Inline->FileSearch(path, file_name))
	{
		m_Inline->FileDelete(path, file_name);
	}

	//_MakeDirectories(_T("D:\\DATABASE\\FILM_ALIGN"));
	//sh data.Format(_T("Index,left_top_x(mm),left_top_y(mm),mid_top_x(mm),mid_top_y(mm),right_top_x(mm),right_top_y(mm),right_bottom_x(mm),right_bottom_y(mm),mid_bottom_x(mm),mid_bottom_y(mm),left_bottom_x(mm),left_bottom_y(mm)"));
	data.Format(_T("Glass_ID, Time, INDEX, FILM_ID, POSITION, CAD_X(mm), CAD_Y(mm), MEASURE_X(mm), MEASURE_Y(mm), SPEC_X(um), SPEC_Y(um), DELTA_X(um), DELTA_Y(um), RESULT_X, RESULT_Y\n"));
	sprintf(strFile, "%S", file_path);
//	fp = fopen( strFile, "a+" ); 
	if((fp = _fsopen(strFile, "a+", _SH_DENYNO)) == NULL)
	{
		HANDLE fd = CreateFile( (LPCWSTR)(LPCSTR)strFile,
							GENERIC_READ|GENERIC_WRITE,
							//GENERIC_WRITE,
							FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);

		fp = _fsopen(strFile, "a+", _SH_DENYNO);
		if(fp == NULL)
		{
			CloseHandle(fd);
	//		return FALSE;
		}

		CloseHandle(fd);
	}
	if(fp == NULL)
	{
		m_strMainMsg.Format(_T("Save Film Align End"));
		m_btn_MainMsg.SetWindowTextW(m_strMainMsg);
		return;
	}

	if(!WriteInsResultFile(fp, file_path, data))
	{
		if(fp != NULL)
		{
			m_strMainMsg.Format(_T("Save Film Align End"));
			m_btn_MainMsg.SetWindowTextW(m_strMainMsg);
			fclose(fp);
			return;
		}
	}

	for(i = 0 ; (i < m_dlgRecipeCell->GetFilmCntY_coordXY()) && (1000 > i); i++)//m_Rslt.m_FilmAlignIdx) && (1000 > i); i++)
	{
		temp.Format(_T("%d"),i);
		
		for(j = 0; j < m_dlgRecipeCell->GetFilmCntX_coordXY(); j++)
		{
			for(film_grab_point = 0; film_grab_point < 6; film_grab_point++)
			{
				switch (film_grab_point)
				{
				case FILM_POS_LT:
					cad_x = abs(m_dlgGlass->GetFilmActiveRectSX(idx));
					cad_y = abs(m_dlgGlass->GetFilmActiveRectSY(idx));
					measure_x = abs(m_Rslt.m_FilmAlignX[(i * 1) + j_count][film_grab_point]);
					measure_y = abs(m_Rslt.m_FilmAlignY[(i * 1) + j_count][film_grab_point]);
					delta_x = (cad_x - measure_x) * 1000;
					delta_y = (cad_y - measure_y) * 1000;
					//20160801 ngh
					ispec_x = m_dlgFilmAlign->GetFilmAlignSpecX(film_grab_point);
					ispec_y = m_dlgFilmAlign->GetFilmAlignSpecY(film_grab_point);

					if(ispec_x >= abs(delta_x)) 
						strResult_x.Format(_T("OK"));
					else
						strResult_x.Format(_T("NG"));
					if(ispec_y >= abs(delta_y)) 
						strResult_y.Format(_T("OK"));
					else
						strResult_y.Format(_T("NG"));
					strPosition.Format(_T("LT"));
					break;
				case FILM_POS_MT:
					cad_x = abs(m_dlgGlass->GetFilmActiveRectSX(idx) + (m_dlgRecipeCell->GetFilmSizeX0_coordXY() * 0.5));
					cad_y = abs(m_dlgGlass->GetFilmActiveRectSY(idx));
					measure_x = abs(m_Rslt.m_FilmAlignX[(i * 1) + j_count][film_grab_point]);
					measure_y = abs(m_Rslt.m_FilmAlignY[(i * 1) + j_count][film_grab_point]);
					delta_x = (cad_x - measure_x) * 1000;
					delta_y = (cad_y - measure_y) * 1000;
					//20160801 ngh
					ispec_x = m_dlgFilmAlign->GetFilmAlignSpecX(film_grab_point);
					ispec_y = m_dlgFilmAlign->GetFilmAlignSpecY(film_grab_point);
				
					strResult_x.Format(_T(""));
				
					if(ispec_y >= abs(delta_y)) 
						strResult_y.Format(_T("OK"));
					else
						strResult_y.Format(_T("NG"));
					strPosition.Format(_T("MT"));
					break;
				case FILM_POS_RT:
					cad_x = abs(m_dlgGlass->GetFilmActiveRectSX(idx) + m_dlgRecipeCell->GetFilmSizeX0_coordXY());
					cad_y = abs(m_dlgGlass->GetFilmActiveRectSY(idx));
					measure_x = abs(m_Rslt.m_FilmAlignX[(i * 1) + j_count][film_grab_point]);
					measure_y = abs(m_Rslt.m_FilmAlignY[(i * 1) + j_count][film_grab_point]);
					delta_x = (cad_x - measure_x) * 1000;
					delta_y = (cad_y - measure_y) * 1000;
					//20160801 ngh
					ispec_x = m_dlgFilmAlign->GetFilmAlignSpecX(film_grab_point);
					ispec_y = m_dlgFilmAlign->GetFilmAlignSpecY(film_grab_point);

					if(ispec_x >= abs(delta_x)) 
						strResult_x.Format(_T("OK"));
					else
						strResult_x.Format(_T("NG"));
					if(ispec_y >= abs(delta_y)) 
						strResult_y.Format(_T("OK"));
					else
						strResult_y.Format(_T("NG"));
					strPosition.Format(_T("RT"));
					break;
				case FILM_POS_RB:
					cad_x = abs(m_dlgGlass->GetFilmActiveRectSX(idx) + m_dlgRecipeCell->GetFilmSizeX0_coordXY());
					cad_y = abs(m_dlgGlass->GetFilmActiveRectSY(idx) - m_dlgRecipeCell->GetFilmSizeY0_coordXY());
					measure_x = abs(m_Rslt.m_FilmAlignX[(i * 1) + j_count][film_grab_point]);
					measure_y = abs(m_Rslt.m_FilmAlignY[(i * 1) + j_count][film_grab_point]);
					delta_x = (cad_x - measure_x) * 1000;
					delta_y = (cad_y - measure_y) * 1000;
					//20160801 ngh
					ispec_x = m_dlgFilmAlign->GetFilmAlignSpecX(film_grab_point);
					ispec_y = m_dlgFilmAlign->GetFilmAlignSpecY(film_grab_point);

					if(ispec_x >= abs(delta_x)) 
						strResult_x.Format(_T("OK"));
					else
						strResult_x.Format(_T("NG"));
					if(ispec_y >= abs(delta_y)) 
						strResult_y.Format(_T("OK"));
					else
						strResult_y.Format(_T("NG"));
					strPosition.Format(_T("RB"));
					break;
				case FILM_POS_MB:
					cad_x = abs(m_dlgGlass->GetFilmActiveRectSX(idx) + (m_dlgRecipeCell->GetFilmSizeX0_coordXY() * 0.5));
					cad_y = abs(m_dlgGlass->GetFilmActiveRectSY(idx) - m_dlgRecipeCell->GetFilmSizeY0_coordXY());
					measure_x = abs(m_Rslt.m_FilmAlignX[(i * 1) + j_count][film_grab_point]);
					measure_y = abs(m_Rslt.m_FilmAlignY[(i * 1) + j_count][film_grab_point]);
					delta_x = (cad_x - measure_x) * 1000;
					delta_y = (cad_y - measure_y) * 1000;
					//20160801 ngh
					ispec_x = m_dlgFilmAlign->GetFilmAlignSpecX(film_grab_point);
					ispec_y = m_dlgFilmAlign->GetFilmAlignSpecY(film_grab_point);
				
					strResult_x.Format(_T(""));

					if(ispec_y >= abs(delta_y)) 
						strResult_y.Format(_T("OK"));
					else
						strResult_y.Format(_T("NG"));
					strPosition.Format(_T("MB"));
					break;
				case FILM_POS_LB:
					cad_x = abs(m_dlgGlass->GetFilmActiveRectSX(idx));//  - m_dlgRecipeCell->GetFilmSizeX0_coordXY());
					cad_y = abs(m_dlgGlass->GetFilmActiveRectSY(idx) - m_dlgRecipeCell->GetFilmSizeY0_coordXY());
					measure_x = abs(m_Rslt.m_FilmAlignX[(i * 1) + j_count][film_grab_point]);
					measure_y = abs(m_Rslt.m_FilmAlignY[(i * 1) + j_count][film_grab_point]);
					delta_x = (cad_x - measure_x) * 1000;
					delta_y = (cad_y - measure_y) * 1000;
					//20160801 ngh
					ispec_x = m_dlgFilmAlign->GetFilmAlignSpecX(film_grab_point);
					ispec_y = m_dlgFilmAlign->GetFilmAlignSpecY(film_grab_point);

					if(ispec_x >= abs(delta_x)) 
						strResult_x.Format(_T("OK"));
					else
						strResult_x.Format(_T("NG"));
					if(ispec_y >= abs(delta_y)) 
						strResult_y.Format(_T("OK"));
					else
						strResult_y.Format(_T("NG"));
					strPosition.Format(_T("LB"));
					break;
				}
				strFilm_id = m_dlgGlass->GetInsActive_id(0,0,FILM_ID, i, j);
				data.Format(_T("%s,%s,%d,%s,%s,%.3f,%.3f,%.3f,%.3f,%d,%d,%.3f,%.3f,%s,%s\n"), strGlass_id, strTime, idx + 1, strFilm_id,strPosition, cad_x,cad_y,measure_x,measure_y,ispec_x,ispec_y,delta_x,delta_y,strResult_x,strResult_y);
				//WriteInsResultFile(file_path, data);
				//	m_Rslt.m_FilmAlignX[i][j], m_Rslt.m_FilmAlignY[i][j]);
				if(!WriteInsResultFile(fp, file_path, data))
				{
					if(fp != NULL)
					{
						m_strMainMsg.Format(_T("Save Film Align End"));
						m_btn_MainMsg.SetWindowTextW(m_strMainMsg);
						fclose(fp);
						return;
					}
				}
			}
			idx++;
			temp = data;
			film_grab_point = 0;
			j_count++;
		}
		j_count = j_count - 1;
	}

	m_strMainMsg.Format(_T("Save Film Align End"));
	m_btn_MainMsg.SetWindowTextW(m_strMainMsg);

	m_bDefectWriteComp = TRUE;

	if(fp != NULL)
	{
		fclose(fp);
		return;
	}
	
}

void CEMIControlDlg::AlarmOperation(int pro_idx, int sts_idx, int sub_seq_idx)
{
	int category, code, level;
	CString log, strLevel;

	MakeAlarmCode(pro_idx, sts_idx, sub_seq_idx, &category, &code, &level);

	//ECS/////////////////////////////
	//Alarm

	if(0 == level)
	{
		if(m_dlgGlassManuCtrl->GetLoaderUse())
		{
			m_Inline->SetLightAlarmReport(TRUE, code);
		}
		strLevel.Format(_T("LightAlarm"));
	}
	else
	{
		if(m_dlgGlassManuCtrl->GetLoaderUse())
		{
			m_Inline->SetDownAlarmCode(code);
			m_Inline->SetHeavyAlarmReport(TRUE, code);
		}
		strLevel.Format(_T("HeavyAlarm"));
	}

	//log
	log.Format(_T("Code:%d, Level:%s, %S"), code, strLevel, m_MDat[pro_idx].m_ProSts[sts_idx][sub_seq_idx].m_cNGStatus);
	WriteLog(ALARM_LOG, log);
	
	//Opeation mode
	m_Inline->SetBitOModeUnitAuto(FALSE);
	m_Inline->SetBitOModeOperation_AutoOrManual(FALSE);

	//Equipment status->Alarm일때만 ECS내부에서 처리

	if(!m_bGrabComp[m_Rslt.m_ScanCamIdx][m_Rslt.m_iScanIdx])
	{
		//20160608 ngh
		/*CString packet;
		packet.Format(_T("0002,%d"), m_Rslt.m_ScanCamIdx);
		PacketSendIPS(packet);*/
	}
}

void CEMIControlDlg::MakeAlarmCode(int pro_idx, int sts_idx, int sub_seq_idx, int *category, int *code, int *level)
{
	char str[DAT_STR_LENG];
	char part_str[DAT_STR_LENG];

	memset(str, NULL, sizeof(char) * DAT_STR_LENG);
	memset(part_str, NULL, sizeof(char) * DAT_STR_LENG);

	if(GetDefectError())
	{
		sprintf(str, "%s", m_cDefectError);
	}
	else
	{
		sprintf(str, "%s", m_MDat[pro_idx].m_ProSts[sts_idx][sub_seq_idx].m_cNGStatus);
	}


	CString Temp;
	CString SearchStr;
	BOOL bRet = FALSE;
	Temp.Empty();
	SearchStr.Empty();
	Temp.Format(_T("%S"), str);
	bRet = AfxExtractSubString(SearchStr,Temp,1,'=');
	if(bRet)
	{
		SearchStr = SearchStr.Mid(0,4);
		sprintf(part_str, "%S", SearchStr);
	}
	else
	{
		//AfxMessageBox(_T("Error : invalid error code!"));

		*category = 1;
		*code = 99;
		*level = 0;
	}

	//20160223 ngh
	if(Temp.GetLength() > 15 && (!Temp.Mid(0, 15).CompareNoCase(_T("TIMER_INTERLOCK")) || !Temp.Mid(0, 7).CompareNoCase(_T("Error :"))))
	{
		*code = m_Inline->GetInterLockError();
		*category = 2;
		*level = m_Inline->GetInterLockErrorLevel();
		return;
	}

	//Data
	if('D' == part_str[0]) 
	{
		//Glass Data
		if('0' == part_str[2])
		{
			if('0' == part_str[2])
			{
				*category = 8;
				*code = 270;
				*level = 1;
			}
			else if('1' == part_str[2])
			{
				*category = 8;
				*code = 271;
				*level = 1;
			}
			else if('2' == part_str[2])
			{
				*category = 8;
				*code = 272;
				*level = 1;
			}
			else if('3' == part_str[2])
			{
				*category = 8;
				*code = 273;
				*level = 1;
			}
			else if('4' == part_str[2])
			{
				*category = 8;
				*code = 274;
				*level = 1;
			}
			else if('5' == part_str[2])
			{
				*category = 8;
				*code = 275;
				*level = 1;
			}
		}
		//defect cnt
		else if('1' == part_str[2])
		{
			if('0' == part_str[2])
			{
				*category = 8;
				*code = 300;
				*level = 1;
			}
			else if('1' == part_str[2])
			{
				*category = 8;
				*code = 301;
				*level = 1;
			}
			else if('2' == part_str[2])
			{
				*category = 8;
				*code = 302;
				*level = 1;
			}
			else if('3' == part_str[2])
			{
				*category = 8;
				*code = 303;
				*level = 1;
			}
			else if('4' == part_str[2])
			{
				*category = 8;
				*code = 304;
				*level = 1;
			}
			else if('5' == part_str[2])
			{
				*category = 8;
				*code = 305;
				*level = 1;
			}
			else if('6' == part_str[2])
			{
				*category = 8;
				*code = 306;
				*level = 1;
			}
		}
	}
	//Sensor/////////////////////////////////////////////////////////////////////////////////////////
	else if('X' == part_str[0])
	{
		if('0' == part_str[2])
		{
			if('0' == part_str[3])
			{
				*category = 1;
				*code = 2;
				*level = 1;
			}
			else if('1' == part_str[3])
			{
				*category = 2;
				*code = 128;
				*level = 1;
			}
			else if('2' == part_str[3])
			{
				*category = 1;
				*code = 128;
				*level = 1;
			}
			/*
			else if('3' == part_str[3])
			{
				
				*category = ;
				*code = ;
				*level = ;
				
			}
			*/
			/*
			else if('4' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			*/
			/*
			else if('5' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			*/
			/*
			else if('6' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
		    */
			/*
			else if('7' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			*/
			/*
			else if('8' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			*/
			/*
			else if('9' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			*/
			/*
			else if('A' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			*/
			else if('B' == part_str[3])
			{
				*category = 2;
				*code = 5;
				*level = 1;
			}
			else if('C' == part_str[3])
			{
				*category = 1;
				*code = 4;
				*level = 1;
			}
			/*
			else if('D' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			*/
			/*
			else if('E' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			*/
			/*
			else if('F' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			*/
			else
			{
				*category = 1;
				*code = 99;
				*level = 0;
			}
		}
		else if('1' == part_str[2])
		{
			/*
			if('0' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			else if('1' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			else if('2' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			else if('3' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			*/
			if('4' == part_str[3])
			{
				*category = 1;
				*code = 3;
				*level = 1;
			}
			else if('5' == part_str[3])
			{
				*category = 1;
				*code = 3;
				*level = 1;
			}
			else if('6' == part_str[3])
			{
				*category = 1;
				*code = 3;
				*level = 1;
			}
			else if('7' == part_str[3])
			{
				*category = 1;
				*code = 3;
				*level = 1;
			}
			else if('8' == part_str[3])
			{
				*category = 1;
				*code = 3;
				*level = 1;
			}
			else if('9' == part_str[3])
			{
				*category = 1;
				*code = 3;
				*level = 1;
			}
			/*
			else if('A' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			*/
			else if('B' == part_str[3])
			{
				*category = 1;
				*code = 1;
				*level = 1;
			}
			else if('C' == part_str[3])
			{
				*category = 1;
				*code = 1;
				*level = 1;
			}
			else if('D' == part_str[3])
			{
				*category = 1;
				*code = 1;
				*level = 1;
			}
			else if('E' == part_str[3])
			{
				*category = 1;
				*code = 1;
				*level = 1;
			}
			/*
			else if('F' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			*/
			else
			{
				*category = 1;
				*code = 99;
				*level = 0;
			}
		}
		else if('2' == part_str[2])
		{
			if('0' == part_str[3])
			{
				*category = 2;
				*code = 133;
				*level = 1;
			}
			else if('1' == part_str[3])
			{
				*category = 2;
				*code = 135;
				*level = 1;
			}
			else if('2' == part_str[3])
			{
				*category = 2;
				*code = 136;
				*level = 1;
			}
			else if('3' == part_str[3])
			{
				*category = 2;
				*code = 137;
				*level = 1;
			}
			else if('4' == part_str[3])
			{
				*category = 2;
				*code = 141;
				*level = 1;
			}
			else if('5' == part_str[3])
			{
				*category = 2;
				*code = 140;
				*level = 1;
			}
			else if('6' == part_str[3])
			{
				*category = 2;
				*code = 134;
				*level = 1;
			}
			else if('7' == part_str[3])
			{
				*category = 2;
				*code = 138;
				*level = 1;
			}
			else if('8' == part_str[3])
			{
				*category = 2;
				*code = 139;
				*level = 1;
			}
			else if('9' == part_str[3])
			{
				*category = 2;
				*code = 145;
				*level = 1;
			}
			else if('A' == part_str[3])
			{
				*category = 2;
				*code = 146;
				*level = 1;
			}
			else if('B' == part_str[3])
			{
				*category = 2;
				*code = 147;
				*level = 1;
			}
			else if('C' == part_str[3])
			{
				*category = 2;
				*code = 148;
				*level = 1;
			}
			else if('D' == part_str[3])
			{
				*category = 2;
				*code = 149;
				*level = 1;
			}
			else if('E' == part_str[3])
			{
				*category = 2;
				*code = 150;
				*level = 1;
			}
			else if('F' == part_str[3])
			{
				*category = 2;
				*code = 151;
				*level = 1;
			}
			else
			{
				*category = 1;
				*code = 99;
				*level = 0;
			}
		}

		else if('3' == part_str[2])
		{
			if('0' == part_str[3])
			{
				*category = 2;
				*code = 152;
				*level = 1;
			}
			else if('1' == part_str[3])
			{
				*category = 2;
				*code = 161;
				*level = 1;
			}
			else if('2' == part_str[3])
			{
				*category = 2;
				*code = 162;
				*level = 1;
			}
			else if('3' == part_str[3])
			{
				*category = 2;
				*code = 163;
				*level = 1;
			}
			else if('4' == part_str[3])
			{
				*category = 2;
				*code = 164;
				*level = 1;
			}
			else if('5' == part_str[3])
			{
				*category = 2;
				*code = 165;
				*level = 1;
			}
			else if('6' == part_str[3])
			{
				*category = 2;
				*code = 166;
				*level = 1;
			}
			/*
			else if('7' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			*/
			else if('8' == part_str[3])
			{
				*category = 2;
				*code = 224;
				*level = 1;
			}
			else if('9' == part_str[3])
			{
				*category = 2;
				*code = 225;
				*level = 1;
			}
			/*
			else if('A' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			else if('B' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			*/
			else if('C' == part_str[3])
			{
				*category = 2;
				*code = 207;
				*level = 1;
			}
			else if('D' == part_str[3])
			{
				*category = 2;
				*code = 208;
				*level = 1;
			}
			else if('E' == part_str[3])
			{
				*category = 2;
				*code = 156;
				*level = 1;
			}
			else if('F' == part_str[3])
			{
				*category = 2;
				*code = 157;
				*level = 1;
			}
			else
			{
				*category = 1;
				*code = 99;
				*level = 0;
			}
		}

		else if('4' == part_str[2])
		{
			if('0' == part_str[3])
			{
				*category = 2;
				*code = 170;
				*level = 1;
			}
			else if('1' == part_str[3])
			{
				*category = 2;
				*code = 171;
				*level = 1;
			}
			else if('2' == part_str[3])
			{
				*category = 2;
				*code = 172;
				*level = 1;
			}
			else if('3' == part_str[3])
			{
				*category = 2;
				*code = 173;
				*level = 1;
			}
			else if('4' == part_str[3])
			{
				*category = 2;
				*code = 174;
				*level = 1;
			}
			else if('5' == part_str[3])
			{
				*category = 2;
				*code = 175;
				*level = 1;
			}
			else if('6' == part_str[3])
			{
				*category = 2;
				*code = 176;
				*level = 1;
			}
			else if('7' == part_str[3])
			{
				*category = 2;
				*code = 177;
				*level = 1;
			}
			else if('8' == part_str[3])
			{
				*category = 2;
				*code = 178;
				*level = 1;
			}
			else if('9' == part_str[3])
			{
				*category = 2;
				*code = 179;
				*level = 1;
			}
			else if('A' == part_str[3])
			{
				*category = 2;
				*code = 180;
				*level = 1;
			}
			else if('B' == part_str[3])
			{
				*category = 2;
				*code = 181;
				*level = 1;
			}
			else if('C' == part_str[3])
			{
				*category = 2;
				*code = 182;
				*level = 1;
			}
			else if('D' == part_str[3])
			{
				*category = 2;
				*code = 183;
				*level = 1;
			}
			else if('E' == part_str[3])
			{
				*category = 2;
				*code = 184;
				*level = 1;
			}
			else if('F' == part_str[3])
			{
				*category = 2;
				*code = 185;
				*level = 1;
			}
			else
			{
				*category = 1;
				*code = 99;
				*level = 0;
			}
		}

		else if('5' == part_str[2])
		{
			if('0' == part_str[3])
			{
				*category = 2;
				*code = 212;
				*level = 1;
			}
			else if('1' == part_str[3])
			{
				*category = 2;
				*code = 208;
				*level = 1;
			}
			else if('2' == part_str[3])
			{
				*category = 2;
				*code = 189;
				*level = 1;
			}
			else if('3' == part_str[3])
			{
				*category = 2;
				*code = 191;
				*level = 1;
			}
			else if('4' == part_str[3])
			{
				*category = 2;
				*code = 190;
				*level =1 ;
			}
			else if('5' == part_str[3])
			{
				*category = 2;
				*code = 192;
				*level = 1;
			}
			/*
			else if('6' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			else if('7' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			else if('8' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			else if('9' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			else if('A' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			else if('B' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			else if('C' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			else if('D' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			else if('E' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			else if('F' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			*/
			else
			{
				*category = 1;
				*code = 99;
				*level = 0;
			}
		}
	}
	else if('E' == part_str[0])
	{
		if('0' == part_str[2])
		{
			if('0' == part_str[3])
			{
				*category = 2;
				*code = 18;
				*level = 1;
			}
			else if('1' == part_str[3])
			{
				*category = 2;
				*code = 19;
				*level = 1;
			}
			else if('2' == part_str[3])
			{
				*category = 2;
				*code = 20;
				*level = 1;
			}
			else if('3' == part_str[3])
			{
				*category = 2;
				*code = 21;
				*level = 1;
			}
			else if('4' == part_str[3])
			{
				*category = 2;
				*code = 240;
				*level = 0;
			}
			else if('5' == part_str[3])
			{
				*category = 2;
				*code = 241;
				*level = 0;
			}
			else if('6' == part_str[3])
			{
				*category = 2;
				*code = 242;
				*level = 0;
			}
			else if('7' == part_str[3])
			{
				*category = 2;
				*code = 243;
				*level = 0;
			}
			else if('8' == part_str[3])
			{
				*category = 2;
				*code = 244;
				*level = 0;
			}
			else if('9' == part_str[3])
			{
				*category = 2;
				*code = 245;
				*level = 0;
			}
			else if('A' == part_str[3])
			{
				*category = 2;
				*code = 246;
				*level = 0;
			}
			else if('B' == part_str[3])
			{
				*category = 2;
				*code = 247;
				*level = 0;
			}
			else if('C' == part_str[3])
			{
				*category = 2;
				*code = 248;
				*level = 0;
			}
			else if('D' == part_str[3])
			{
				*category = 2;
				*code = 249;
				*level = 0;
			}
			else if('E' == part_str[3])
			{
				*category = 2;
				*code = 250;
				*level = 0;
			}
			else if('F' == part_str[3])
			{
				*category = 2;
				*code = 251;
				*level = 0;
			}
			else
			{
				*category = 1;
				*code = 99;
				*level = 0;
			}
		}
		if('1' == part_str[2])
		{
			if('0' == part_str[3])
			{
				*category = 1;
				*code = 252;
				*level = 0;
			}
			else if('1' == part_str[3])
			{
				*category = 1;
				*code = 253;
				*level = 0;
			}
			else if('2' == part_str[3])
			{
				*category = 1;
				*code = 254;
				*level = 0;
			}
			else if('3' == part_str[3])
			{
				*category = 1;
				*code = 255;
				*level = 0;
			}
			else if('4' == part_str[3])
			{
				*category = 1;
				*code = 256;
				*level = 0;
			}
			else if('5' == part_str[3])
			{
				*category = 1;
				*code = 257;
				*level = 0;
			}
			else if('6' == part_str[3])
			{
				*category = 1;
				*code = 258;
				*level = 0;
			}
			else if('7' == part_str[3])
			{
				*category = 1;
				*code = 259;
				*level = 0;
			}
			else if('8' == part_str[3])
			{
				*category = 1;
				*code = 260;
				*level = 0;
			}
			else if('9' == part_str[3])
			{
				*category = 1;
				*code = 261;
				*level = 0;
			}
			else if('A' == part_str[3])
			{
				*category = 1;
				*code = 262;
				*level = 0;
			}
			else if('B' == part_str[3])
			{
				*category = 1;
				*code = 263;
				*level = 0;
			}
			else if('C' == part_str[3])
			{
				*category = 1;
				*code = 264;
				*level = 0;
			}
			else if('D' == part_str[3])
			{
				*category = 1;
				*code = 265;
				*level = 0;
			}
			else if('E' == part_str[3])
			{
				*category = 1;
				*code = 266;
				*level = 0;
			}
			/*
			else if('F' == part_str[3])
			{
				*category = ;
				*code = ;
				*level = ;
			}
			*/
			else
			{
				*category = 1;
				*code = 99;
				*level = 0;
			}
		}
	}
	else
	{
		*category = 1;
		*code = 99;
		*level = 0;
	}
	/////////////////////////////////////////////////////////////////////////////////////
}

void CEMIControlDlg::ClickBtnenhctrlMain8k()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_btn_scan_0.SetValue(1);
	m_btn_scan_1.SetValue(0);
	m_btn_scan_2.SetValue(0);

	m_iScanTypeIdx = 0;

	//Glass defect display
	m_Glass->SetDispDefectGlassData(m_iScanTypeIdx);
	double robot_y = m_Robot->GetRobotPos_mm(MOTOR_0);
	double robot_x = m_Robot->GetRobotPos_mm(MOTOR_1);

	m_Glass->SetDrawCrossRobot(robot_y, robot_x);
}


void CEMIControlDlg::ClickBtnenhctrlMain8k90()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_btn_scan_0.SetValue(0);
	m_btn_scan_1.SetValue(1);
	m_btn_scan_2.SetValue(0);
	m_btn_scan_3.SetValue(0);
	m_btn_scan_4.SetValue(0);

	m_iScanTypeIdx = 1;

	//Glass defect display
	m_Glass->SetDispDefectGlassData(m_iScanTypeIdx);
	double robot_y = m_Robot->GetRobotPos_mm(MOTOR_0);
	double robot_x = m_Robot->GetRobotPos_mm(MOTOR_1);

	m_Glass->SetDrawCrossRobot(robot_y, robot_x);
}

void CEMIControlDlg::ClickBtnenhctrlMain16k()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_btn_scan_0.SetValue(0);
	m_btn_scan_1.SetValue(0);
	m_btn_scan_2.SetValue(1);
	m_btn_scan_3.SetValue(0);
	m_btn_scan_4.SetValue(0);

	m_iScanTypeIdx = 2;

	//Glass defect display
	m_Glass->SetDispDefectGlassData(m_iScanTypeIdx);
	double robot_y = m_Robot->GetRobotPos_mm(MOTOR_0);
	double robot_x = m_Robot->GetRobotPos_mm(MOTOR_1);

	m_Glass->SetDrawCrossRobot(robot_y, robot_x);
}

void CEMIControlDlg::ClickBtnenhctrlMain16k2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_btn_scan_0.SetValue(0);
	m_btn_scan_1.SetValue(0);
	m_btn_scan_2.SetValue(0);
	m_btn_scan_3.SetValue(1);
	m_btn_scan_4.SetValue(0);

	m_iScanTypeIdx = 3;

	//Glass defect display
	m_Glass->SetDispDefectGlassData(m_iScanTypeIdx);
	double robot_y = m_Robot->GetRobotPos_mm(MOTOR_0);
	double robot_x = m_Robot->GetRobotPos_mm(MOTOR_1);

	m_Glass->SetDrawCrossRobot(robot_y, robot_x);
}

void CEMIControlDlg::ClickBtnenhctrlMain16k3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_btn_scan_0.SetValue(0);
	m_btn_scan_1.SetValue(0);
	m_btn_scan_2.SetValue(0);
	m_btn_scan_3.SetValue(0);
	m_btn_scan_4.SetValue(1);

	m_iScanTypeIdx = 4;

	//Glass defect display
	m_Glass->SetDispDefectGlassData(m_iScanTypeIdx);
	double robot_y = m_Robot->GetRobotPos_mm(MOTOR_0);
	double robot_x = m_Robot->GetRobotPos_mm(MOTOR_1);

	m_Glass->SetDrawCrossRobot(robot_y, robot_x);
}

int CEMIControlDlg::GetScanTypeIdx()
{
	return m_iScanTypeIdx;
}

BOOL CEMIControlDlg::GetIPSConnect()
{
	return m_bConnection;
}

void CEMIControlDlg::SetLoadingComplete(BOOL bit)
{
	m_Rslt.m_bCompleteLoading = bit;
}

void CEMIControlDlg::SetCassetEmpty(BOOL bit)
{
	m_Rslt.m_bCassetEmpty = bit;
}

int CEMIControlDlg::GetRsltScanTypeIdx()
{
	return m_Rslt.m_iScanTypeIdx;
}

void CEMIControlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch(nIDEvent)
	{
	case 100://주황색 타워 점멸
		if(m_bTower)
		{
			m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_YEL, FALSE);
		}
		else
		{
			m_IO->SetWriteDO(DO_SIG_TOWER_FRONT_YEL, TRUE);
		}
		m_bTower = !m_bTower;
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CEMIControlDlg::SetLoadRecipeFile(CString path)
{
	//20160714 PCL 데이터 로드 안되는 현상 수정
	m_Align->SetLoadAlignPara(path);
	m_RecipeCellPCL->SetLoadRecipeCellPCLPara(path);
	m_RecipeCell->SetLoadRecipeCellPara(path);
	m_RecipePanel->SetLoadRecipePanelPara(path);
	m_FilmAlign->SetLoadFilmAlignPara(path);
	m_RecipeScan->SetLoadRecipeScanPara(path);

	//20160412 ngh
	m_Align->SetUpdate(FALSE);
	m_RecipeCellPCL->Update(TRUE);
	m_RecipeCell->SetUpdate(FALSE);
	m_RecipePanel->SetUpdate(FALSE);
	m_FilmAlign->SetUpdate(FALSE);
	m_RecipeScan->SetUpdate(FALSE);

	return TRUE;
}

BOOL CEMIControlDlg::SetSaveRecipeFile(CString path)
{
	_MakeDirectories(path);

	m_Align->SetSaveAlignPara(path);
	m_RecipeCell->SetSaveRecipeCellPara(path);
	m_RecipePanel->SetSaveRecipePanelPara(path);
	m_FilmAlign->SetSaveFilmAlignPara(path);
	m_RecipeScan->SetSaveRecipeScanPara(path);

	return TRUE;
}

void CEMIControlDlg::SetManualInspect(BOOL var)
{
	m_Rslt.m_bStartManualInspect = var;
}


void CEMIControlDlg::ClickBtnenhctrlMainManualStart()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CDialogMSG dlg, dlg2;

	dlg.SetCaption(_T("수동검사를 진행 하시겠습니까(수동검사 후 자동모드로 전환 하려면 RECIPE에서 재시작 버튼을 클릭해야합니다)?"));
	if(0 == dlg.DoModal())
	{
		m_bManualStart = FALSE;
		m_btn_main_sts.SetWindowTextW(_T("자동검사 정지 상태"));
		return;
	}
	m_btn_main_sts.SetWindowTextW(_T("수동검사 정지 상태"));

	if(m_GlassManu->GetLoaderUse())
	{
		dlg2.SetCaption(_T("LOADER 사용 상태에서는 수동검사를 할 수 없습니다. LOADER 미사용 상태로 전환하시겠습니까?"));
		if(0 == dlg2.DoModal())
		{
			m_bManualStart = FALSE;
			m_btn_main_sts.SetWindowTextW(_T("자동검사 정지 상태"));
			return;
		}
	}
	m_btn_main_sts.SetWindowTextW(_T("수동검사 정지 상태"));
	m_bManualStart = TRUE;
	m_GlassManu->SetLoaderUse(FALSE);

	SetLotInit();

	m_Rslt.m_bStartManualInspect = m_bManualStart;

	SetManualInspect(m_bManualStart);
}

void CEMIControlDlg::SetWaitLoading(BOOL var)
{
	m_Rslt.m_bCassetEmpty = var;
}

void CEMIControlDlg::SetRecipeToECS()
{
	typeRecipePara recipe_para;
	CString strLog;
	CDialogRecipeModel dlg;
	dlg.SetOpen();

	recipe_para.nRecipeNumber = dlg.GetRecipeNo() + 1;
	sprintf(recipe_para.cRecipeName, "%S", dlg.GetRecipeName());

	recipe_para.RecipeReport.nRecipeNumber = dlg.GetRecipeNo() + 1;
	recipe_para.RecipeReport.nRecipeLevel = 2;
	recipe_para.RecipeReport.cRecipeType = 'M';
	recipe_para.RecipeReport.wRecipeVersion[3] = 1;
	recipe_para.RecipeReport.nRecipeStatus = 1;
	recipe_para.RecipeReport.nUnitNumber = 1;

	recipe_para.RecipeCell = m_RecipeCell->GetRecipe();
	recipe_para.RecipePanel = m_RecipePanel->GetRecipe();
	for(int i = 0; i < RECIPE_SCAN_TYPE_MAX_NUM; i++)
	{
		recipe_para.RecipeScan[i] = m_RecipeScan->GetRecipe(i);
	}

	strLog.Format(_T("SetRecipeToECS() : Recipe Name : %S, Recipe Num : %d"), recipe_para.cRecipeName, recipe_para.nRecipeNumber);
	m_Inline->WriteLog(strLog);

	m_Inline->SetRecipe(recipe_para);
	m_Inline->SetCurrentEquipmentRecipe(recipe_para.RecipeReport.nRecipeNumber);
}



BOOL CEMIControlDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CEMIControlDlg::WriteLog(int log_type, CString text)
{
	EnterCriticalSection(&m_CSInline);

	FILE*	stream;
	char	strFile[MAX_PATH], strToFile[MAX_PATH];
    long	result, line;
	char	szTime[DAT_STR_LENG*3]; 
	CString bak_file;
	int limit_line = 500000;
	CString path;
	CString strTime;
	CString log,strLogType;
	char buffer[DAT_STR_LENG];

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
	case MOTOR_LOG:
		strLogType = MOTOR_LOG_FILE_NAME;
		break;
	case LIGHT_LOG:
		strLogType = LIGHT_LOG_FILE_NAME;
		break;
	case IO_LOG:
		strLogType = IO_LOG_FILE_NAME;
		break;
	case EZ_LOG:
		strLogType = EZ_LOG_FILE_NAME;
		break;
	case ALARM_LOG:
		strLogType = ALARM_LOG_FILE_NAME;
		break;
	case RECIPE_LOG:
		strLogType = RECIPE_LOG_FILE_NAME;
		break;
	case FILM_ALIGN_LOG:
		strLogType = FILM_ALIGN_LOG_FILE_NAME;
		break;
	}

	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);
	strTime.Format(_T("%04d%02d%02d"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
	log.Format(_T("%04d-%02d-%02d-%02d:%02d:%02d:%03d->%s"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay, lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond, lpSystemTime.wMilliseconds, text);

	sprintf(strFile, "D:\\DATABASE\\LOG\\%S\\%S_%S", strTime, strTime, strLogType);

	_MakeDirectories(_T("D:\\DATABASE\\LOG\\") + strTime);

	if((stream = _fsopen(strFile, "a+", _SH_DENYNO)) == NULL)
	{
		HANDLE fd = CreateFile( (LPCWSTR)(LPCSTR)strFile,
							GENERIC_READ|GENERIC_WRITE,
							//GENERIC_WRITE,
							FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);

		stream = _fsopen(strFile, "a+", _SH_DENYNO);
		if(stream == NULL)
		{
			CloseHandle(fd);
			return FALSE;
		}

		CloseHandle(fd);
	}

	//Check first time
    result = fseek(stream, 0L, SEEK_SET);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}
	line = 0;
	while(fgets(buffer, DAT_STR_LENG, stream)) 
	{
		line++;
		if(1 < line)
		{
			break;
		}
	}

    result = fseek(stream, 0L, SEEK_END);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}

	memset(szTime, NULL, sizeof(char) * 512 * 3);
	sprintf(szTime, "%S", log);
    fputs(szTime, stream);
	fputs("\n", stream);

    result = fseek(stream, 0L, SEEK_SET);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}

	//Check limit line
	line = 0;
	if(limit_line)
	{
		while(fgets(buffer, DAT_STR_LENG, stream)) line++;
		if(line > limit_line)
		{
			sprintf_s(strToFile, path.GetLength() + bak_file.GetLength() + 2, "%S\\%S", path, bak_file); 
			CopyFile((LPCWSTR)(LPCSTR)strFile, (LPCWSTR)(LPCSTR)strToFile, FALSE);
			fclose(stream);
			DeleteFile((LPCWSTR)(LPCSTR)strFile);
		}
		else
		{
			fclose(stream);
		}
	}
	else
	{
		fclose(stream);
	}

	LeaveCriticalSection(&m_CSInline);

	return TRUE;
}

BOOL CEMIControlDlg::OFDefectError(int scan_type_idx)
{
	BOOL rslt = TRUE;

	double width, height;

	m_RecipeScan->GetOFDefectSize(scan_type_idx, &width, &height);

	rslt = m_Glass->OFDefectError(scan_type_idx, width, height);

	return rslt;
}

BOOL CEMIControlDlg::GetDefectError()
{
	return m_bDefectError;
}

void CEMIControlDlg::ClickBtnenhctrlMainDfserver()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect rect;
	ManuSelect(7);
	WindowVisable_main();
#ifdef DEFECT
	m_dlgDfserver->ShowWindow(SW_SHOW);
	m_btn_dfserver.SetValue(TRUE);

	m_dlgDfserver->GetWindowRect(rect);
	m_dlgDfserver->MoveWindow(5, 100, rect.Width(), rect.Height(), TRUE);
#endif
}


void CEMIControlDlg::ClickBtnenhctrlMainDefectList()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_btn_DefectList.GetValue())
	{
		m_dlgDefectList->ShowWindow(SW_SHOW);
	}
	else
	{
		m_dlgDefectList->ShowWindow(SW_HIDE);
	}
}

//20160721 ngh
int CEMIControlDlg::GetAccessType()
{
	return m_nAccessType;
}

//20160530 ngh
void CEMIControlDlg::SetAccess(int accessType)
{
	SetAccessType(accessType);
	SetAccessButton(accessType);
}

void CEMIControlDlg::SetAccessButton(int accessType)
{
	typeAccessRight access;
	access = m_AccessRight->GetAccessRight(accessType);
	
	m_btn_autorun.SetEnabled(access.bAutoRun);
	m_btn_recipe.SetEnabled(access.bRecipe);
	m_btn_system.SetEnabled(access.bSystem);
	m_btn_align.SetEnabled(access.bAlign);
	m_btn_inline.SetEnabled(access.bInline);
	m_btn_log.SetEnabled(access.bLog);
	m_btn_dfserver.SetEnabled(access.bDefectServer);

}

void CEMIControlDlg::SetAccessType(int accessType)
{
	m_nAccessType = accessType;

	SetOperater(m_nAccessType);
}

void CEMIControlDlg::SetOperater(int accessType)
{
	switch(accessType)
	{
	case 0:
		m_strOperater.Format(_T("MANAGEMENT"));
		break;
	case 1:
		m_strOperater.Format(_T("OPERATOR"));
		break;
	case 2:
		m_strOperater.Format(_T("TECHNICIAN"));
		break;
	case 3:
		m_strOperater.Format(_T("ADMINISTRATOR"));
		break;
	}
}

CString CEMIControlDlg::GetOperater()
{
	return m_strOperater;
}

void CEMIControlDlg::ParaCompareLogWrite(int para1, int para2, CString strName)
{
	CString strLog;
	if(para1 == para2)
	{
		return;
	}
	else
	{
		strLog.Format(_T("%s Parameter Change %d -> %d"), strName, para1, para2);
		WriteLog(RECIPE_LOG, strLog);
	}
}

void CEMIControlDlg::ParaCompareLogWrite(double para1, double para2, CString strName)
{
	CString strLog;
	if(para1 == para2)
	{
		return;
	}
	else
	{
		strLog.Format(_T("%s Parameter Change %f -> %f"), strName, para1, para2);
		WriteLog(RECIPE_LOG, strLog);
	}
}

void CEMIControlDlg::ParaCompareLogWrite(CString para1, CString para2, CString strName)
{
	CString strLog;
	if(!para1.Compare(para2))
	{
		return;
	}
	else
	{
		strLog.Format(_T("%s Parameter Change %s -> %s"), strName, para1, para2);
		WriteLog(RECIPE_LOG, strLog);
	}
}

void CEMIControlDlg::ClickBtnenhctrlMainSacnWait()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_bScanAfterWait = m_btn_ScanAfterWait.GetValue();
	//20160711 ngh 스캔 후 대기
	//typeEQPStatusDataPara eq_sts;
	//m_Flag.m_bGlassExist = TRUE;
	//if(m_bScanAfterWait && m_Flag.m_bGlassExist)
	//{
	//	eq_sts = m_Inline->GetStatusReport();
	//	eq_sts.bGlassExistInUnit = m_Flag.m_bGlassExist;	    //Glass유무
	//	eq_sts.bGlassInProcess = FALSE;							//검사중인지
	//	eq_sts.bIonizer = TRUE;
	//	eq_sts.bUnitAutoMode = TRUE;							//자동반출 가능한지			
	//	eq_sts.nCurrenRecipeNum = m_Recipe->GetRecipeNo() + 1;
	//	if(m_Flag.m_bGlassExist)
	//	{
	//		eq_sts.nGlassCountInUnit = 1;
	//		eq_sts.nPutInfoPossibleCount = 0;
	//		eq_sts.nStatusRunIdleDown = 0;
	//	}
	//	else
	//	{
	//		eq_sts.nGlassCountInUnit = 0;
	//		eq_sts.nPutInfoPossibleCount = 1;
	//		eq_sts.nStatusRunIdleDown = 1;
	//	}

	//	eq_sts.nStatusRunIdleDown = !m_Flag.m_bGlassExist;		//0 : RUN~
	//	eq_sts.nUnitOpeationMode = 0;							//1 : Manual, 0 : auto
	//	m_Inline->SetStatus(eq_sts);

	//	m_Glass->SetLotInit();

	//	m_dStartSecond = clock();
	//	//SetSeqIdx(prc_idx, seq_idx, 245);
	//}
	//else
	//{
	//	//SetSeqIdx(prc_idx, seq_idx, 210);
	//}

	//double robot_y, robot_x;
	//robot_y = m_Robot->GetRobotPos_mm(MOTOR_0);
	//robot_x = m_Robot->GetRobotPos_mm(MOTOR_1);

	//m_Glass->SetDispGlassDat();
	//m_Glass->SetDispDefectGlassData(GetScanTypeIdx());
	//m_Glass->SetDrawCrossRobot(-robot_y, robot_x);
}


void CEMIControlDlg::ClickBtnenhctrlMainLotRestart()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_Recipe->SetLotRestart();
}

void CEMIControlDlg::SetManuSelect(int nManuType)
{
	ManuSelect(nManuType);
}

void CEMIControlDlg::ManuSelect(int nManuType)
{
	switch(nManuType)
	{
	case 0:
		m_btn_login.SetValue(TRUE);
		m_btn_autorun.SetValue(FALSE);
		m_btn_recipe.SetValue(FALSE);
		m_btn_system.SetValue(FALSE);
		m_btn_align.SetValue(FALSE);
		m_btn_inline.SetValue(FALSE);
		m_btn_log.SetValue(FALSE);
		m_btn_dfserver.SetValue(FALSE);
		m_btn_lot_restart.ShowWindow(SW_HIDE);
		m_btn_ScanAfterWait.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_MAIN_USE_MOVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_MAIN_USE_DEFECT_MOVE)->ShowWindow(SW_HIDE);
		//m_btn_mecha_align_re.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_btn_login.SetValue(FALSE);
		m_btn_autorun.SetValue(TRUE);
		m_btn_recipe.SetValue(FALSE);
		m_btn_system.SetValue(FALSE);
		m_btn_align.SetValue(FALSE);
		m_btn_inline.SetValue(FALSE);
		m_btn_log.SetValue(FALSE);
		m_btn_dfserver.SetValue(FALSE);
		m_btn_lot_restart.ShowWindow(SW_SHOW);
		m_btn_ScanAfterWait.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_MAIN_USE_MOVE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_MAIN_USE_DEFECT_MOVE)->ShowWindow(SW_SHOW);
		//m_btn_mecha_align_re.ShowWindow(SW_SHOW);
		break;
	case 2:
		m_btn_login.SetValue(FALSE);
		m_btn_autorun.SetValue(FALSE);
		m_btn_recipe.SetValue(TRUE);
		m_btn_system.SetValue(FALSE);
		m_btn_align.SetValue(FALSE);
		m_btn_inline.SetValue(FALSE);
		m_btn_log.SetValue(FALSE);
		m_btn_dfserver.SetValue(FALSE);
		m_btn_lot_restart.ShowWindow(SW_HIDE);
		m_btn_ScanAfterWait.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_MAIN_USE_MOVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_MAIN_USE_DEFECT_MOVE)->ShowWindow(SW_HIDE);
		//m_btn_mecha_align_re.ShowWindow(SW_HIDE);
		break;
	case 3:
		m_btn_login.SetValue(FALSE);
		m_btn_autorun.SetValue(FALSE);
		m_btn_recipe.SetValue(FALSE);
		m_btn_system.SetValue(TRUE);
		m_btn_align.SetValue(FALSE);
		m_btn_inline.SetValue(FALSE);
		m_btn_log.SetValue(FALSE);
		m_btn_dfserver.SetValue(FALSE);
		m_btn_lot_restart.ShowWindow(SW_HIDE);
		m_btn_ScanAfterWait.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_MAIN_USE_MOVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_MAIN_USE_DEFECT_MOVE)->ShowWindow(SW_HIDE);
		//m_btn_mecha_align_re.ShowWindow(SW_HIDE);
		break;
	case 4:
		m_btn_login.SetValue(FALSE);
		m_btn_autorun.SetValue(FALSE);
		m_btn_recipe.SetValue(FALSE);
		m_btn_system.SetValue(FALSE);
		m_btn_align.SetValue(TRUE);
		m_btn_inline.SetValue(FALSE);
		m_btn_log.SetValue(FALSE);
		m_btn_dfserver.SetValue(FALSE);
		m_btn_lot_restart.ShowWindow(SW_HIDE);
		m_btn_ScanAfterWait.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_MAIN_USE_MOVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_MAIN_USE_DEFECT_MOVE)->ShowWindow(SW_HIDE);
		//m_btn_mecha_align_re.ShowWindow(SW_HIDE);
		break;
	case 5:
		m_btn_login.SetValue(FALSE);
		m_btn_autorun.SetValue(FALSE);
		m_btn_recipe.SetValue(FALSE);
		m_btn_system.SetValue(FALSE);
		m_btn_align.SetValue(FALSE);
		m_btn_inline.SetValue(TRUE);
		m_btn_log.SetValue(FALSE);
		m_btn_dfserver.SetValue(FALSE);
		m_btn_lot_restart.ShowWindow(SW_HIDE);
		m_btn_ScanAfterWait.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_MAIN_USE_MOVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_MAIN_USE_DEFECT_MOVE)->ShowWindow(SW_HIDE);
		//m_btn_mecha_align_re.ShowWindow(SW_HIDE);
		break;
	case 6:
		m_btn_login.SetValue(FALSE);
		m_btn_autorun.SetValue(FALSE);
		m_btn_recipe.SetValue(FALSE);
		m_btn_system.SetValue(FALSE);
		m_btn_align.SetValue(FALSE);
		m_btn_inline.SetValue(FALSE);
		m_btn_log.SetValue(TRUE);
		m_btn_dfserver.SetValue(FALSE);
		m_btn_lot_restart.ShowWindow(SW_HIDE);
		m_btn_ScanAfterWait.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_MAIN_USE_MOVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_MAIN_USE_DEFECT_MOVE)->ShowWindow(SW_HIDE);
		//m_btn_mecha_align_re.ShowWindow(SW_HIDE);
		break;
	case 7:
		m_btn_login.SetValue(FALSE);
		m_btn_autorun.SetValue(FALSE);
		m_btn_recipe.SetValue(FALSE);
		m_btn_system.SetValue(FALSE);
		m_btn_align.SetValue(FALSE);
		m_btn_inline.SetValue(FALSE);
		m_btn_log.SetValue(FALSE);
		m_btn_dfserver.SetValue(TRUE);
		m_btn_lot_restart.ShowWindow(SW_HIDE);
		m_btn_ScanAfterWait.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_MAIN_USE_MOVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_MAIN_USE_DEFECT_MOVE)->ShowWindow(SW_HIDE);
		//m_btn_mecha_align_re.ShowWindow(SW_HIDE);
		break;
	}
}

void CEMIControlDlg::ClickBtnenhctrlMainMechaAlignRepet()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_bMachaAlignRepet = m_btn_mecha_align_re.GetValue();
}


void CEMIControlDlg::ClickBtnenhctrlMainMechaAlignRepet2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int idx = 0;
	BOOL not_delay = TRUE;
	if(m_btn_mecha_align.GetValue())
	{
		m_btn_mecha_align.SetCaption(_T("Loading Mecha Align On"));
		idx = 2000;
		while(idx > 0)
		{
			switch(idx)
			{
			//Glass Unloading
			case 2000:
				//Lift pin dn and mechnical align
				m_Rslt.m_dMotPos[MOTOR_3] =  0;//임시 위치
				m_Robot->SetRobotMove_mm(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]);
				m_strMainMsg.Format(_T("Lift Pin Mid Down Move Start"));
				WriteLog(MAIN_LOG, m_strMainMsg);
				//SetSeqIdx(prc_idx, seq_idx, 2010);
				idx = 2010;
				break;
			case 2010:
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "LiftPinAxis Move Time Over Error=AX04 : UP/DN 축동작 타임아웃(축 동작 실패)!-2010-");
				if(m_Robot->GetIsMoveComplete(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]))
				{
					if(m_Robot->GetIsInposition(MOTOR_3))
					{
						m_strMainMsg.Format(_T("Lift Pin Mid Down Move End"));
						WriteLog(MAIN_LOG, m_strMainMsg);
						//SetSeqIdx(prc_idx, seq_idx, 2020);
						idx = 2020;
					}
				}
				break;
			case 2020:
				//not_delay = CheckDelay(prc_idx, seq_idx, 500);
				if(not_delay)
				{
					m_IO->SetWriteDO(DO_STAGE_AIR_BLOW_SOL, TRUE);
					idx = 2030;
					//SetSeqIdx(prc_idx, seq_idx, 2030);
				}
				break;
			case 2030:
				//not_delay = CheckDelay(prc_idx, seq_idx, 500);
				if(not_delay)
				{
					//SetSeqIdx(prc_idx, seq_idx, 2040);
					idx = 2040;

					//Sensor forward
					m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_FW_SOL, TRUE);
					m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_BW_SOL, FALSE);
					m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_FW_SOL, TRUE);
					m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_BW_SOL, FALSE);
					m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_FW_SOL, TRUE);
					m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_BW_SOL, FALSE);
					m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_FW_SOL, TRUE);
					m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_BW_SOL, FALSE);
				}
				break;
			case 2040:
				//Sensor(1~4)
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 1 Forward Error=X540 : STAGE ALIGN FRONT FW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2040-");	
				if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT1_FW))
				{
					//SetSeqIdx(prc_idx, seq_idx, 2050);
					idx = 2050;
				}
				break;
			case 2050:
				//Sensor(1~4)
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 2 Forward Error=X542 : STAGE ALIGN FRONT FW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2050-");	
				if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT2_FW))
				{
					//SetSeqIdx(prc_idx, seq_idx, 2060);
					idx = 2060;
				}
				break;
			case 2060:
				//Sensor(1~4)
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 1  Forward Error=X544 : STAGE ALIGN REAR FW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2060-");	
				if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR1_FW))
				{
					//SetSeqIdx(prc_idx, seq_idx, 2070);
					idx = 2070;
				}
				break;
			case 2070:
				//Sensor(1~4)
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 2 Forward Error=X546 : STAGE ALIGN REAR FW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2070-");	
				if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR2_FW))
				{
					//SetSeqIdx(prc_idx, seq_idx, 2080);
					idx = 2080;
				}
				break;
			case 2080:
				//Sensor(1~4)
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Left 1 Forward Error=X548 : STAGE SIDE LEFT FW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2080-");	
				if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT1_FW))
				{
					//SetSeqIdx(prc_idx, seq_idx, 2090);
					idx = 2090;
				}
				break;
			case 2090:
				//Sensor(1~4)
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Left 2 Forward Error=X54A : STAGE SIDE LEFT FW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2090-");	
				if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT2_FW))
				{
					//SetSeqIdx(prc_idx, seq_idx, 2100);
					idx = 2100;
				}
				break;
			case 2100:
				//Sensor(1~4)
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 1 Forward Error=X54C : STAGE SIDE RIGHT FW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2100-");	
				if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT1_FW))
				{
					//SetSeqIdx(prc_idx, seq_idx, 2110);
					idx = 2110;
				}
				break;
			case 2110:
				//Sensor(1~4)
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 2 Forward Error=X54E : STAGE SIDE RIGHT FW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2110-");	
				if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT2_FW))
				{
					//SetSeqIdx(prc_idx, seq_idx, 2120);
					idx = 2120;
				}
				break;
			case 2120:
				//Lift pin full down
				m_Rslt.m_dMotPos[MOTOR_3] =  m_RecipePanel->GetPanelLoadPosDN();
				m_Robot->SetRobotMove_mm(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]);
				m_strMainMsg.Format(_T("Lift Pin Down Move Start"));
				WriteLog(MAIN_LOG, m_strMainMsg);
				//SetSeqIdx(prc_idx, seq_idx, 2130);
				idx = 2130;
				break;
			case 2130:
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "LiftPinAxis Move Time Over Error=AX04 : UP/DN 축동작 타임아웃(축 동작 실패)!-2130-");
				if(m_Robot->GetIsMoveComplete(MOTOR_3, m_Rslt.m_dMotPos[MOTOR_3]))
				{	
					if(m_Robot->GetIsInposition(MOTOR_3))
					{
						m_strMainMsg.Format(_T("Lift Pin Down Move End"));
						WriteLog(MAIN_LOG, m_strMainMsg);
						//Mech align forwarding
						//SetSeqIdx(prc_idx, seq_idx, 2140);
						idx = 2140;
					}
				}
				break;
			case 2140:
				//not_delay = CheckDelay(prc_idx, seq_idx, 500);
				if(not_delay)
				{
					m_IO->SetWriteDO(DO_STAGE_AIR_BLOW_SOL, FALSE);
					idx = 2150;
					//SetSeqIdx(prc_idx, seq_idx, 2150);
				}
				break;
			case 2150:
				//not_delay = CheckDelay(prc_idx, seq_idx, 500);
				if(not_delay)
				{
					m_IO->SetWriteDO(DO_STAGE_VACUUM_SOL, TRUE);
					idx = 2160;
					//SetSeqIdx(prc_idx, seq_idx, 2160);
				}
				break;
			case 2160:
				//시간 대기
				//not_delay = CheckDelay(prc_idx, seq_idx, 500);
				if(not_delay)
				{
					//SetSeqIdx(prc_idx, seq_idx, 2170);
					idx = 2170;

					//Sensor backword
					m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_FW_SOL, FALSE);
					m_IO->SetWriteDO(DO_STAGE_ALIGN_FRONT1_BW_SOL, TRUE);
					m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_FW_SOL, FALSE);
					m_IO->SetWriteDO(DO_STAGE_SIDE_RIGHT1_BW_SOL, TRUE);
					m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_FW_SOL, FALSE);
					m_IO->SetWriteDO(DO_STAGE_SIDE_LIFT1_BW_SOL, TRUE);
					m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_FW_SOL, FALSE);
					m_IO->SetWriteDO(DO_STAGE_ALIGN_REAR1_BW_SOL, TRUE);
				}
				break;
			case 2170:
				//Sensor(1~4)
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 1 Backward Error=X541 : STAGE ALIGN FRONT BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2170-");	
				if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT1_BW))
				{
					//SetSeqIdx(prc_idx, seq_idx, 2180);
					idx = 2180;
				}
				break;
			case 2180:
				//Sensor(1~4)
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Front 2 Backward Error=X543 : STAGE ALIGN FRONT BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2180-");	
				if(m_IO->GetReadDI(DI_STAGE_ALIGN_FRONT2_BW))
				{
					//SetSeqIdx(prc_idx, seq_idx, 2190);
					idx = 2190;
				}
				break;
			case 2190:
				//Sensor(1~4)
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 1 Backward Error=X545 : STAGE ALIGN REAR BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2190-");	
				if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR1_BW))
				{
					//SetSeqIdx(prc_idx, seq_idx, 2200);
					idx = 2200;
				}
				break;
			case 2200:
				//Sensor(1~4)
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Rear 2 Backward Error=X547 : STAGE ALIGN REAR BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2200-");	
				if(m_IO->GetReadDI(DI_STAGE_ALIGN_REAR2_BW))
				{
					//SetSeqIdx(prc_idx, seq_idx, 2210);
					idx = 2210;
				}
				break;
			case 2210:
				//Sensor(1~4)
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Left 2 Forward Error=X54A : STAGE SIDE LEFT BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2210-");	
				if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT1_BW))
				{
					//SetSeqIdx(prc_idx, seq_idx, 2220);
					idx = 2220;
				}
				break;
			case 2220:
				//Sensor(1~4)
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 1 Forward Error=X54C : STAGE SIDE LEFT BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2220-");	
				if(m_IO->GetReadDI(DI_STAGE_SIDE_LEFT2_BW))
				{
					//SetSeqIdx(prc_idx, seq_idx, 2230);
					idx = 2230;
				}
				break;
			case 2230:
				//Sensor(1~4)
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 2 Forward Error=X54E : STAGE SIDE RIGHT BW #1 SENSOR TIMEOUT(실린더센서 감지않됨)!-2230-");	
				if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT1_BW))
				{
					//SetSeqIdx(prc_idx, seq_idx, 2240);
					idx = 2240;
				}
				break;
			case 2240:
				//Sensor(1~4)
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "STAGE Align Cylinder Right 2 Backward Error=X54F : STAGE SIDE RIGHT BW #2 SENSOR TIMEOUT(실린더센서 감지않됨)!-2240-");	
				if(m_IO->GetReadDI(DI_STAGE_SIDE_RIGHT2_BW))
				{
					//SetSeqIdx(prc_idx, seq_idx, 2250);
					idx = 2250;
				}
				break;
			case 2250:
				//시간 대기
				//not_delay = CheckDelay(prc_idx, seq_idx, 500);
				if(not_delay)
				{
					//SetSeqIdx(prc_idx, seq_idx, m_Rslt.m_iMechAlignSeq);
					idx = 0;
				}
				break;
			}
		}
	}
	else
	{
		m_btn_mecha_align.SetCaption(_T("Unloading Mecha Align On"));

	}
}
