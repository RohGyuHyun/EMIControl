// Dialog/DialogInline.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EMIControl.h"
#include "Dialog/DialogInline.h"


#include "Include/_CtrlImageRect.h"
#include "include/_FuncModuleHal.h"
#include "include/_FuncModuleBasic.h"

#include "include/Runtime.h"
#include "include/PMACComm.h"

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
#include "Dialog/DialogInline.h"
#include "Dialog/DialogRecipeCell.h"
#include "Dialog/DialogRecipeScan.h"
#include "Dialog/DialogGlassManuCtrl.h"
#include "Dialog/DialogFilmAlign.h"
#include "Dialog/DialogLogAlarm.h"
#include "DialogLogWindow.h"
#include "EMIControlDlg.h"
#include "afxdialogex.h"

// CDialogInline 대화 상자입니다.
IMPLEMENT_DYNAMIC(CDialogInline, CDialogEx)

CDialogInline::CDialogInline(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogInline::IDD, pParent)
	, m_chk_e_sts_run(FALSE)
	, m_chk_e_sts_idle(FALSE)
	, m_chk_e_sts_down(FALSE)
	, m_chk_glass_in_process(FALSE)
	, m_chk_glass_exist(FALSE)
	, m_chk_o_mode_online(FALSE)
	, m_chk_o_mode_auto(FALSE)
	, m_chk_o_mode_operation(FALSE)
	, m_chk_e_status_ion_status(FALSE)
	, m_edit_e_status_recipe_no(0)
	, m_edit_e_status_glass_cnt(0)
	, m_edit_e_status_glass_posi_cnt(0)
	, m_edit_strLotID(_T(""))
	, m_edit_strProcessingCode(_T(""))
	, m_edit_strLotSpecificData1(_T(""))
	, m_edit_strLotSpecificData2(_T(""))
	, m_edit_strLotSpecificData3(_T(""))
	, m_edit_strLotSpecificData4(_T(""))
	, m_edit_nHostRecipeNumber(0)
	, m_edit_nGlassType(0)
	, m_edit_nGlassCodeLot(0)  
	, m_edit_nGlassCodeSLot(0)
	, m_edit_strGlassID(_T(""))
	, m_edit_nGlassSpecificData1(0)
	, m_edit_nGlassSpecificData2(0)
	, m_edit_nGlassSpecificData3(0)
	, m_edit_nGlassSpecificData4(0)
	, m_edit_nGlassAddData1(0)
	, m_edit_nGlassAddData2(0)
	, m_edit_nGlassAddData3(0)
	, m_edit_nGlassAddData4(0)
	, m_edit_nGlassAddData5(0)
	, m_edit_nGlassAddData6(0)
	, m_edit_nPreviousEqpProcessingData1(0)
	, m_edit_nPreviousEqpProcessingData2(0)
	, m_edit_nPreviousEqpProcessingData3(0)
	, m_edit_nPreviousEqpProcessingData4(0)
	, m_edit_nGlassProcessingData1(0)
	, m_edit_nGlassProcessingData2(0)
	, m_edit_nGlassProcessingData3(0)
	, m_edit_nGlassRoutingData1(0)
	, m_edit_nGlassRoutingData2(0)
	, m_edit_nGlassRoutingData3(0)
	, m_edit_strGlassJudge(_T(""))
	, m_edit_nRecipeChangeNum(0)
	, m_edit_strEcs_Time(_T(""))
	, m_edit_nTimeout_0(0)
	, m_edit_nTimeout_1(0)
	, m_edit_nTimeout_2(0)
	, m_edit_nTimeout_3(0)
	, m_edit_nTimeout_4(0)
	, m_edit_nTimeout_5(0)
	, m_edit_nTimeout_6(0)
	, m_edit_nTrigger_0(0)
	, m_edit_nTrigger_1(0)
	, m_edit_nTrigger_2(0)
	, m_edit_nTrigger_3(0)
	, m_edit_nTrigger_4(0)
	, m_edit_nTrigger_5(0)
	, m_edit_nTrigger_6(0)
	, m_edit_nTrigger_7(0)
	, m_radio_lost_glass_data_option(FALSE)
	, m_edit_nGlassCode(0)  //pkh
	, m_edit_nReipeReportCnt(0)
	, m_check_bUnload_Exchang(FALSE)
{
	m_PLCComm = new CPLCComm();
	m_MelsecListPara = new typeMelsecListPara[MAX_ITEM_CNT];
	m_MelsecItem = new PLC_MEMORY_ITEM[MAX_ITEM_CNT];
	m_MelsecAddr = new PLC_MEMORY_MAP[MAX_MAP_CNT];
	m_RecipeData = new typeRecipeDataPara[MAX_RECIPE_CNT];
	m_RecipeReport = new typeRecipeReportDataPara[PLC_DEVW_RP_RECIPE_PARAMETER_CNT];
	memset(m_MelsecListPara, NULL, sizeof(typeMelsecListPara) * MAX_ITEM_CNT);
	memset(m_MelsecItem, NULL, sizeof(PLC_MEMORY_ITEM) * MAX_ITEM_CNT);
	memset(m_MelsecAddr, NULL, sizeof(PLC_MEMORY_MAP) * MAX_MAP_CNT);
	memset(m_RecipeData, NULL, sizeof(typeRecipeDataPara) * MAX_RECIPE_CNT);

	m_SelectRecipeData.RecipeBody = new WORD[PLC_DEVW_RBD_RECIPE_BODY_CNT];
	memset(m_SelectRecipeData.RecipeBody, NULL, sizeof(WORD) * PLC_DEVW_RBD_RECIPE_BODY_CNT);
	memset(m_RecipeReport, NULL, sizeof(typeRecipeReportDataPara) * PLC_DEVW_RP_RECIPE_PARAMETER_CNT);

	memset(&m_EQPStatus, NULL, sizeof(typeEQPStatusDataPara));
	memset(&m_EMILoaderStatus, NULL, sizeof(typeEMILoaderStatusDataPara));
	memset(&m_EMIUnloaderStatus, NULL, sizeof(typeEMIUnloaderStatusDataPara));
	memset(&m_RobotLoaderStatus, NULL, sizeof(typeRobotLoaderStatusDataPara));
	memset(&m_RobotUnloaderStatus, NULL, sizeof(typeRobotUnloaderStatusDataPara));
	
	memset(&m_Recipe, NULL, sizeof(typeRecipePara));
	memset(&m_APDData, NULL, sizeof(typeAPDDataPara));
	memset(&m_TPDData, NULL, sizeof(typeTPDDataPara));

	m_wAPDData = new WORD[PLC_DEVW_APD_DATA_CNT];
	m_wPositionData = new WORD[MAX_POSITION_CNT];
	memset(m_wAPDData, NULL, sizeof(WORD) * PLC_DEVW_APD_DATA_CNT);
	memset(m_wPositionData, NULL, sizeof(WORD) * MAX_POSITION_CNT);
	memset(&m_TPDData, NULL, sizeof(typeTPDDataPara));

	memset(m_nTimeout, NULL, sizeof(int) * MAX_TIMEOUT_CNT);
	memset(m_nTrigger, NULL, sizeof(int) * MAX_TRIGGER_CNT);

	memset(&m_LoaderGlassData, NULL, sizeof(typeGlassDataPara));
	memset(&m_UnloaderGlassData, NULL, sizeof(typeGlassDataPara));
	memset(&m_UpdateGlassData, NULL, sizeof(typeGlassDataPara));

	for(int i = 0; i < MAX_RECIPE_CNT; i++)
	{
		m_RecipeData[i].nRecipeNum = i + 1;
	}

	for(int i = 0; i < MAX_TIMEOUT_CNT; i++)
	{
		m_nTimeout[i] = 10;
		m_nTrigger[i] = 1;
	}

	m_bMelsecConnect = FALSE;
	m_bAllValueRead = FALSE;
	m_bWord1ValueRead = FALSE;
	m_bWord2ValueRead = FALSE;
	m_bBitValueRead = FALSE;

	m_iSeqIdx= 0;
	m_iTimeout100mm = 0;
	m_nTPDNum = 0;

	m_nReportStart_RecipeListCnt = 0;
	m_nReportEnd_RecipeListCnt = 0;
	m_nOrderCEID = 0;

	m_nInterLockError = 0;
	m_nSeqDelay100mm = 0;

	m_nTest = 0;
	m_bTest = TRUE;

	InitializeCriticalSection(&m_CSInline);
}

CDialogInline::~CDialogInline()
{
	SaveTemp();
	KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
	KillTimer(TIMER_INTERLOCK_UNLOAD_ONLY);
	KillTimer(TIMER_INTERLOCK_EXCHANGE);
	KillTimer(TIMER_ECS_START_STATUE_CHECK);
	KillTimer(TIMER_ECS_TIME_DATA_GET);
	KillTimer(TIMER_RECIPE_BODY_DATA_REQUEST);
	
	InitSeqBit(INIT_SEQ_BIT);

	//KillTimer(100);
	//KillTimer(110);
	//KillTimer(120);
	//KillTimer(130);
	SetBitOModeOnline(FALSE);

	if(m_bAllValueRead)
		KillTimer(MELSEC_ALL_ADDRESS_READ);

	if(m_bWord1ValueRead)
		KillTimer(MELSEC_WORD_ADDRESS1_READ);

	if(m_bWord2ValueRead)
		KillTimer(MELSEC_WORD_ADDRESS2_READ);

	if(m_bBitValueRead)
		KillTimer(MELSEC_BIT_ADDRESS_READ);

	if (m_bOpened) 
	{
		int ret = mdClose(MELSEC_CHANNEL_NO);		//	close channel	
		if(ret) {
			mdClose(MELSEC_CHANNEL_NO);			//	close channel	
		}
	}

	m_bOpened = FALSE;
	DeleteCriticalSection(&m_CSInline);
	//delete m_pThreadMelsecRead;
	delete m_MelsecListPara;
	delete m_MelsecItem;
	delete m_MelsecAddr;
	delete m_PLCComm;
}

void CDialogInline::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_E_STATUS_RUN, m_chk_e_sts_run);
	DDX_Check(pDX, IDC_CHECK_E_STATUS_IDLE, m_chk_e_sts_idle);
	DDX_Check(pDX, IDC_CHECK_E_STATUS_DOWN, m_chk_e_sts_down);
	DDX_Check(pDX, IDC_CHECK_E_STATUS_GLASS_INPROCESS, m_chk_glass_in_process);
	DDX_Check(pDX, IDC_CHECK_E_STATUS_GLASS_EXIST, m_chk_glass_exist);
	DDX_Check(pDX, IDC_CHECK_O_MODE_ONLINE, m_chk_o_mode_online);
	DDX_Check(pDX, IDC_CHECK_O_MODE_AUTO, m_chk_o_mode_auto);
	DDX_Check(pDX, IDC_CHECK_O_MODE_OPERATE, m_chk_o_mode_operation);
	DDX_Check(pDX, IDC_CHECK_E_STATUS_ION_STATUS, m_chk_e_status_ion_status);
	DDX_Text(pDX, IDC_EDIT_E_STATUS_RECIPE_NO, m_edit_e_status_recipe_no);
	DDX_Text(pDX, IDC_EDIT_E_STATUS_GLASS_CNT, m_edit_e_status_glass_cnt);
	DDX_Text(pDX, IDC_EDIT_E_STATUS_GLASS_POSI_CNT, m_edit_e_status_glass_posi_cnt);
	DDX_Text(pDX, IDC_EDIT_GLASS_INFO_LOTID, m_edit_strLotID);
	DDX_Text(pDX, IDC_EDIT_GLASS_INFO_PROCESS_CODE, m_edit_strProcessingCode);
	DDX_Text(pDX, IDC_EDIT_GLASS_INFO_LOTSPEC1, m_edit_strLotSpecificData1);
	DDX_Text(pDX, IDC_EDIT_GLASS_INFO_LOTSPEC2, m_edit_strLotSpecificData2);
	DDX_Text(pDX, IDC_EDIT_GLASS_INFO_LOTSPEC3, m_edit_strLotSpecificData3);
	DDX_Text(pDX, IDC_EDIT_GLASS_INFO_LOTSPEC4, m_edit_strLotSpecificData4);
	DDX_Text(pDX, IDC_EDIT_HOST_RECIPE_NUMBER, m_edit_nHostRecipeNumber);
	DDX_Text(pDX, IDC_EDIT_GLASS_TYPE, m_edit_nGlassType);
	DDX_Text(pDX, IDC_EDIT_GLASS_CODE_LOT, m_edit_nGlassCodeLot);
	DDX_Text(pDX, IDC_EDIT_GLASS_CODE_SLOT, m_edit_nGlassCodeSLot);
	DDX_Text(pDX, IDC_EDIT_GLASS_ID, m_edit_strGlassID);
	DDX_Text(pDX, IDC_EDIT_GLASS_SPECIFIC_DATA_1, m_edit_nGlassSpecificData1);
	DDX_Text(pDX, IDC_EDIT_GLASS_SPECIFIC_DATA_2, m_edit_nGlassSpecificData2);
	DDX_Text(pDX, IDC_EDIT_GLASS_SPECIFIC_DATA_3, m_edit_nGlassSpecificData3);
	DDX_Text(pDX, IDC_EDIT_GLASS_SPECIFIC_DATA_4, m_edit_nGlassSpecificData4);
	DDX_Text(pDX, IDC_EDIT_GLASS_ADD_DATA_1, m_edit_nGlassAddData1);
	DDX_Text(pDX, IDC_EDIT_GLASS_ADD_DATA_2, m_edit_nGlassAddData2);
	DDX_Text(pDX, IDC_EDIT_GLASS_ADD_DATA_3, m_edit_nGlassAddData3);
	DDX_Text(pDX, IDC_EDIT_GLASS_ADD_DATA_4, m_edit_nGlassAddData4);
	DDX_Text(pDX, IDC_EDIT_GLASS_ADD_DATA_5, m_edit_nGlassAddData5);
	DDX_Text(pDX, IDC_EDIT_GLASS_ADD_DATA_6, m_edit_nGlassAddData6);
	DDX_Text(pDX, IDC_EDIT_PREVIOUS_EQP_PROCESSING_DATA_1, m_edit_nPreviousEqpProcessingData1);
	DDX_Text(pDX, IDC_EDIT_PREVIOUS_EQP_PROCESSING_DATA_2, m_edit_nPreviousEqpProcessingData2);
	DDX_Text(pDX, IDC_EDIT_PREVIOUS_EQP_PROCESSING_DATA_3, m_edit_nPreviousEqpProcessingData3);
	DDX_Text(pDX, IDC_EDIT_PREVIOUS_EQP_PROCESSING_DATA_4, m_edit_nPreviousEqpProcessingData4);
	DDX_Text(pDX, IDC_EDIT_GLASS_PROCESSING_DATA_1, m_edit_nGlassProcessingData1);
	DDX_Text(pDX, IDC_EDIT_GLASS_PROCESSING_DATA_2, m_edit_nGlassProcessingData2);
	DDX_Text(pDX, IDC_EDIT_GLASS_PROCESSING_DATA_3, m_edit_nGlassProcessingData3);
	DDX_Text(pDX, IDC_EDIT_GLASS_ROUTING_DATA_1, m_edit_nGlassRoutingData1);
	DDX_Text(pDX, IDC_EDIT_GLASS_ROUTING_DATA_2, m_edit_nGlassRoutingData2);
	DDX_Text(pDX, IDC_EDIT_GLASS_ROUTING_DATA_3, m_edit_nGlassRoutingData3);
	DDX_Text(pDX, IDC_EDIT_GLASS_JUDGE, m_edit_strGlassJudge);
	DDX_Text(pDX, IDC_EDIT_RECIPE_NUM, m_edit_nRecipeChangeNum);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_EQP_LOADER_NORMAL_STATUS, m_label_eqp_loader_normal_status);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_EQP_UNLOADER_NORMAL_STATUS, m_label_eqp_unloader_normal_status);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_ROBOT_LOADER_NORMAL_STATUS, m_label_robot_loader_normal_status);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_ROBOT_UNLOADER_NORMAL_STATUS, m_label_robot_unloader_normal_status);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_EQP_LOADER_ENABLE, m_label_eqp_loader_enable);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_EQP_UNLOADER_ENABLE, m_label_eqp_unloader_enable);
	DDX_Text(pDX, IDC_EDIT_ECS_TIME, m_edit_strEcs_Time);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_0, m_edit_nTimeout_0);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_1, m_edit_nTimeout_1);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_2, m_edit_nTimeout_2);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_3, m_edit_nTimeout_3);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_4, m_edit_nTimeout_4);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_5, m_edit_nTimeout_5);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_6, m_edit_nTimeout_6);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_7, m_edit_nTimeout_7);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_0, m_edit_nTrigger_0);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_1, m_edit_nTrigger_1);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_2, m_edit_nTrigger_2);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_3, m_edit_nTrigger_3);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_4, m_edit_nTrigger_4);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_5, m_edit_nTrigger_5);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_6, m_edit_nTrigger_6);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_7, m_edit_nTrigger_7);
	DDX_Radio(pDX, IDC_RADIO_LOST_GLASS_DATA_OPTION_1, m_radio_lost_glass_data_option);
	DDX_Text(pDX, IDC_EDIT_GLASS_CODE, m_edit_nGlassCode);
	DDX_Text(pDX, IDC_EDIT_RECIPE_REPORT_CNT, m_edit_nReipeReportCnt);
	DDX_Check(pDX, IDC_CHECK_UNLOADER_EXCHANGE, m_check_bUnload_Exchang);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_LOADER_0, m_btn_LoaderEnable);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_LOADER_1, m_btn_LoaderGlassDataRequest);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_LOADER_2, m_btn_LoaderNormalStatus);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_LOADER_3, m_btn_LoaderLoadRequest);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_LOADER_4, m_btn_LoaderLoadCompleteConfirm);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_LOADER_5, m_btn_LoaderLoadWait);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_UNLOADER_0, m_btn_UnLoaderEnable);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_UNLOADER_1, m_btn_UnLoaderGlassDataReport);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_UNLOADER_2, m_btn_UnLoaderNormalStatus);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_UNLOADER_3, m_btn_UnLoaderUnLoadRequest);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_UNLOADER_4, m_btn_UnLoaderUnloadCompleteConfirm);
	DDX_Control(pDX, IDC_BTNENHCTRL_INLINE_UNLOADER_5, m_btn_UnLoaderUnLoadWait);
}


BEGIN_MESSAGE_MAP(CDialogInline, CDialogEx)
//	ON_BN_CLICKED(IDC_CHECK_E_STATUS_GLS_INPROCESS2, &CDialogInline::OnBnClickedCheckEStatusGlsInprocess2)
	ON_BN_CLICKED(IDC_BUTTON_O_MODE_UNIT_OPERATION_SEND, &CDialogInline::OnBnClickedButtonOModeUnitOperationSend)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_E_STATUS_RECIPE_NO_SEND, &CDialogInline::OnBnClickedButtonEStatusRecipeNoSend)
	ON_BN_CLICKED(IDC_BUTTON_E_STATUS_GLASS_CNT_SEND, &CDialogInline::OnBnClickedButtonEStatusGlassCntSend)
	ON_BN_CLICKED(IDC_BUTTON_E_STATUS_GLASS_POSI_CNT_SEND, &CDialogInline::OnBnClickedButtonEStatusGlassPosiCntSend)
	ON_BN_CLICKED(IDC_BUTTON_E_STATUS_ION_STATUS_SEND, &CDialogInline::OnBnClickedButtonEStatusIonStatusSend)
	ON_WM_TIMER()
	ON_MESSAGE(USE_MSG_INLINE_CTRL, OnInLineCtrlMessage)
	ON_BN_CLICKED(IDC_BUTTON_EXCHANGE, &CDialogInline::OnBnClickedButtonExchange)
	ON_BN_CLICKED(IDC_BUTTON_TIME_DATA, &CDialogInline::OnBnClickedButtonTimeData)
	ON_BN_CLICKED(IDC_BUTTON_LIGHT_ALARM, &CDialogInline::OnBnClickedButtonLightAlarm)
	ON_BN_CLICKED(IDC_BUTTON_HEVY_ALARM, &CDialogInline::OnBnClickedButtonHevyAlarm)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CDialogInline::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_IDLE, &CDialogInline::OnBnClickedButtonIdle)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CDialogInline::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_CHECK_E_STATUS_GLASS_INPROCESS, &CDialogInline::OnBnClickedCheckEStatusGlassInprocess)
	ON_BN_CLICKED(IDC_CHECK_E_STATUS_GLASS_EXIST, &CDialogInline::OnBnClickedCheckEStatusGlassExist)
	ON_BN_CLICKED(IDC_BUTTON_LOADER_GLASS_DATA_READ, &CDialogInline::OnBnClickedButtonLoaderGlassDataRead)
	ON_BN_CLICKED(IDC_BUTTON_UNLOADER_GLASS_DATA_WRITE, &CDialogInline::OnBnClickedButtonUnloaderGlassDataWrite)
	ON_BN_CLICKED(IDC_BUTTON_EQUPMENT_RECIPE_CHANGE, &CDialogInline::OnBnClickedButtonEqupmentRecipeChange)
	ON_BN_CLICKED(IDC_BUTTON_RECIPE_VARIOUS_REQUEST, &CDialogInline::OnBnClickedButtonRecipeVariousRequest)
	ON_BN_CLICKED(IDC_BUTTON_RECIPE_VARIOUS_REPORT, &CDialogInline::OnBnClickedButtonRecipeVariousReport)
	ON_BN_CLICKED(IDC_BUTTON_SCRAP_GLASS_DATA, &CDialogInline::OnBnClickedButtonScrapGlassData)
	ON_BN_CLICKED(IDC_BUTTON_LOST_GLASS_DATA, &CDialogInline::OnBnClickedButtonLostGlassData)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_GLASS_DATA, &CDialogInline::OnBnClickedButtonChangeGlassData)
	ON_BN_CLICKED(IDC_BUTTON_GLASS_POSITION, &CDialogInline::OnBnClickedButtonGlassPosition)
	ON_BN_CLICKED(IDC_BUTTON_GLASS_APD_DATA, &CDialogInline::OnBnClickedButtonGlassApdData)
	ON_BN_CLICKED(IDC_BUTTON_GLASS_TPD_DATA, &CDialogInline::OnBnClickedButtonGlassTpdData)
	ON_BN_CLICKED(IDC_BUTTON_LIGHT_ALARM2, &CDialogInline::OnBnClickedButtonLightAlarm2)
	ON_BN_CLICKED(IDC_BUTTON_HEVY_ALARM2, &CDialogInline::OnBnClickedButtonHevyAlarm2)
	ON_BN_CLICKED(IDC_BUTTON_RECIPE_BODY, &CDialogInline::OnBnClickedButtonRecipeBody)
	ON_BN_CLICKED(IDC_BUTTON_RECIPE_PARAMETER, &CDialogInline::OnBnClickedButtonRecipeParameter)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialogInline::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_GLASS_DATA, &CDialogInline::OnBnClickedButtonDeleteGlassData)
END_MESSAGE_MAP()


BOOL CDialogInline::FileDelete(CString FilePath, CString FileName)
{
	CFileFind ff;
	CString strTarget = FilePath;
	CStringArray strFileArray;
	BOOL rslt = FALSE;

	if(strTarget.Right(1) != '\\')
	{
		strTarget += '\\';
	}
	
	strTarget += "*.*";
	if(ff.FindFile(strTarget)==TRUE)
	{
		CString Filename;
		BOOL bFlag = true;
		while(bFlag == TRUE)
		{
			bFlag = ff.FindNextFile();
			Filename = ff.GetFilePath();
			if(ff.IsDots())
			{
				continue;
			}
			if(ff.IsDirectory())
			{
				continue;
			}
			if(ff.GetFileName() == FileName)
			{
				DeleteFile(Filename);
				break;
			}
		}
	}
	ff.Close();
	
	return rslt;
}


BOOL CDialogInline::FileSearch(CString FilePath, CString FileName)
{
	CFileFind ff;
	CString strTarget = FilePath;
	CStringArray strFileArray;
	BOOL rslt = FALSE;

	if(strTarget.Right(1) != '\\')
	{
		strTarget += '\\';
	}
	
	strTarget += "*.*";
	if(ff.FindFile(strTarget)==TRUE)
	{
		CString Filename;
		BOOL bFlag = true;
		while(bFlag == TRUE)
		{
			bFlag = ff.FindNextFile();
			Filename = ff.GetFilePath();
			if(ff.IsDots())
			{
				continue;
			}
			if(ff.IsDirectory())
			{
				continue;
			}
			if(ff.GetFileName() == FileName)
			{
				rslt = TRUE;
				break;
			}
		}
	}
	ff.Close();
	
	return rslt;
}

// CDialogInline 메시지 처리기입니다.
BOOL CDialogInline::SetBitOModeOnline(BOOL bit)
{
	WriteLog(_T("SetBitOModeOnline Start!"));
	m_chk_o_mode_online = bit;
	m_EQPStatus.bOnline= bit;

	m_MelsecListPara[DEVB_ONLINE].bValue = bit;
	
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_ONLINE);
	WriteLog(_T("SetBitOModeOnline End!"));

	return TRUE;
}

BOOL CDialogInline::GetBitOModeOnline()
{
	WriteLog(_T("GetBitOModeOnline Start!"));
	SendMessage(USE_MSG_INLINE_CTRL, MSG_READ_ADDRS, DEVB_ONLINE);
	m_chk_o_mode_online = m_MelsecListPara[DEVB_ONLINE].bValue;
	WriteLog(_T("GetBitOModeOnline End!"));
	return m_chk_o_mode_online;
}

BOOL CDialogInline::SetBitOModeUnitAuto(BOOL bit)
{
	WriteLog(_T("SetBitOModeUnitAuto Start!"));
	m_chk_o_mode_auto = bit;

	m_MelsecListPara[DEVB_AUTO_MODE].bValue = bit;	

	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_AUTO_MODE);
	WriteLog(_T("SetBitOModeUnitAuto End!"));
	return TRUE;
}

BOOL CDialogInline::GetBitOModeAuto()
{
	WriteLog(_T("GetBitOModeAuto Start!"));
	SendMessage(USE_MSG_INLINE_CTRL, MSG_READ_ADDRS, DEVB_AUTO_MODE);
	m_chk_o_mode_auto = m_MelsecListPara[DEVB_AUTO_MODE].bValue;
	WriteLog(_T("GetBitOModeAuto End!"));
	return m_chk_o_mode_auto;
}

BOOL CDialogInline::SetBitOModeOperation_AutoOrManual(BOOL bit)
{
	WriteLog(_T("SetBitOModeOperation_AutoOrManual Start!"));
	m_chk_o_mode_operation = bit;

	m_MelsecListPara[DEVB_MODE_AUTO].bValue = bit;
	m_MelsecListPara[DEVB_MODE_MANUAL].bValue = !bit;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_MODE_AUTO);
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_MODE_MANUAL);
	if(bit)
	{
		InitSeqBit(3);
	}
	WriteLog(_T("SetBitOModeOperation_AutoOrManual End!"));
	return TRUE;
}

BOOL CDialogInline::GetBitOModeOperation()
{
	WriteLog(_T("GetBitOModeOperation Start!"));
	SendMessage(USE_MSG_INLINE_CTRL, MSG_READ_ADDRS, DEVB_MODE_AUTO);
	m_chk_e_sts_run = m_MelsecListPara[DEVB_MODE_AUTO].bValue;
	WriteLog(_T("GetBitOModeOperation End!"));
	return m_chk_o_mode_operation;
}

BOOL CDialogInline::SetBitEStatusRun(BOOL bit)
{
	WriteLog(_T("SetBitEStatusRun Start!"));
	m_chk_e_sts_run = bit;

	if(bit)
	{
		m_MelsecListPara[DEVB_RUN].bValue = TRUE;
	}
	else
	{
		m_MelsecListPara[DEVB_RUN].bValue = FALSE;
	}
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_RUN);
	WriteLog(_T("SetBitEStatusRun End!"));
	return TRUE;
}

BOOL CDialogInline::GetBitEStatusRun()
{
	WriteLog(_T("GetBitEStatusRun Start!"));
	SendMessage(USE_MSG_INLINE_CTRL, MSG_READ_ADDRS, DEVB_RUN);
	m_chk_e_sts_run = m_MelsecListPara[DEVB_RUN].bValue;
	WriteLog(_T("GetBitEStatusRun End!"));
	return m_chk_e_sts_run;
}

BOOL CDialogInline::SetBitEStatusIdle(BOOL bit)
{
	WriteLog(_T("SetBitEStatusIdle Start!"));
	m_chk_e_sts_idle = bit;

	if(bit)
	{
		m_MelsecListPara[DEVB_IDLE].bValue = TRUE;
	}
	else
	{
		m_MelsecListPara[DEVB_IDLE].bValue = FALSE;
	}
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_IDLE);
	WriteLog(_T("SetBitEStatusIdle End!"));
	return TRUE;
}

BOOL CDialogInline::GetBitEStatusIdle()
{
	WriteLog(_T("GetBitEStatusIdle Start!"));
	SendMessage(USE_MSG_INLINE_CTRL, MSG_READ_ADDRS, DEVB_IDLE);
	m_chk_e_sts_idle = m_MelsecListPara[DEVB_IDLE].bValue;
	WriteLog(_T("GetBitEStatusIdle End!"));
	return m_chk_e_sts_idle;
}

BOOL CDialogInline::SetBitEStatusDown(BOOL bit)
{
	WriteLog(_T("SetBitEStatusDown Start!"));
	m_chk_e_sts_down = bit;

	if(bit)
	{
		m_MelsecListPara[DEVW_USD_UNIT_DOWN_ALARM_CODE].wValue = m_nDownAlarmCode;
		SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_USD_UNIT_DOWN_ALARM_CODE);
		m_MelsecListPara[DEVB_DOWN].bValue = TRUE;
	}
	else
	{
		m_MelsecListPara[DEVB_DOWN].bValue = FALSE;
	}
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_DOWN);
	WriteLog(_T("SetBitEStatusDown End!"));
	return TRUE;
}

BOOL CDialogInline::GetBitEStatusDown()
{
	WriteLog(_T("GetBitEStatusDown Start!"));
	SendMessage(USE_MSG_INLINE_CTRL, MSG_READ_ADDRS, DEVB_DOWN);
	m_chk_e_sts_down = m_MelsecListPara[DEVB_DOWN].bValue;
	WriteLog(_T("GetBitEStatusDown End!"));
	return m_chk_e_sts_down;
}

BOOL CDialogInline::SetBitEStatusGlassInPrcs(BOOL bit)
{
	WriteLog(_T("SetBitEStatusGlassInPrcs Start!"));
	m_chk_glass_in_process = bit;

	m_MelsecListPara[DEVB_GLASS_IN_PROCESSING].bValue = bit;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_GLASS_IN_PROCESSING);
	WriteLog(_T("SetBitEStatusGlassInPrcs End!"));
	return TRUE;
}

BOOL CDialogInline::GetBitEStatusGlassInPrcs()
{
	WriteLog(_T("GetBitEStatusGlassInPrcs Start!"));
	SendMessage(USE_MSG_INLINE_CTRL, MSG_READ_ADDRS, DEVB_GLASS_IN_PROCESSING);
	m_chk_glass_in_process = m_MelsecListPara[DEVB_GLASS_IN_PROCESSING].bValue;
	WriteLog(_T("GetBitEStatusGlassInPrcs End!"));
	return m_chk_glass_in_process;
}

BOOL CDialogInline::SetBitEStatusGlassExist(BOOL bit)
{
	WriteLog(_T("SetBitEStatusGlassExist Start!"));
	m_chk_glass_exist = bit;
	m_EQPStatus.bGlassExistInUnit = bit;
	if(bit)
	{
		m_MelsecListPara[DEVW_POG_POSITION_1_GLASS_CODE].wValue = m_LoaderGlassData.nGlassCode;
	}
	else
	{
		m_MelsecListPara[DEVW_POG_POSITION_1_GLASS_CODE].wValue = 1;
	}
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_POG_POSITION_1_GLASS_CODE);

	Sleep(100);
	m_MelsecListPara[DEVB_GLASS_EXIST_IN_UNIT].bValue = bit;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_GLASS_EXIST_IN_UNIT);
	
	SetBitEStatusGlassPosition(bit);

	if(bit)
	{
		SetEStatus(STATUS_RUN);
	}
	else
	{
		SetEStatus(STATUS_IDLE);
	}

	WriteLog(_T("SetBitEStatusGlassExist End!"));
	return TRUE;
}

BOOL CDialogInline::GetBitEStatusGlassExist()
{
	WriteLog(_T("GetBitEStatusGlassExist Start!"));
	SendMessage(USE_MSG_INLINE_CTRL, MSG_READ_ADDRS, DEVB_GLASS_EXIST_IN_UNIT);
	m_chk_glass_exist = m_MelsecListPara[DEVB_GLASS_EXIST_IN_UNIT].bValue;
	WriteLog(_T("GetBitEStatusGlassExist End!"));
	return m_chk_glass_exist;
}

BOOL CDialogInline::GetBitEStatusGlassPosition()
{
	WriteLog(_T("GetBitEStatusGlassPosition Start!"));
	SendMessage(USE_MSG_INLINE_CTRL, MSG_READ_ADDRS, DEVB_POSITION1_GLASS_EXIST);
	SendMessage(USE_MSG_INLINE_CTRL, MSG_READ_ADDRS, DEVB_POSITION2_GLASS_EXIST);
	m_EQPStatus.bGlassPosition = !m_MelsecListPara[DEVB_POSITION1_GLASS_EXIST].bValue;
	WriteLog(_T("GetBitEStatusGlassPosition End!"));
	return m_chk_glass_in_process;
}

BOOL CDialogInline::SetBitEStatusGlassPosition(BOOL bit)
{
	WriteLog(_T("SetBitEStatusGlassPosition Start!"));
	//m_chk_glass_exist = bit;

	m_MelsecListPara[DEVB_POSITION1_GLASS_EXIST].bValue = bit;
	m_MelsecListPara[DEVB_POSITION2_GLASS_EXIST].bValue = 0;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_POSITION1_GLASS_EXIST);
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_POSITION2_GLASS_EXIST);
	WriteLog(_T("SetBitEStatusGlassPosition End!"));
	return TRUE;
}

BOOL CDialogInline::SetBitEStatusIonStatus(BOOL bit)
{
	WriteLog(_T("SetBitEStatusIonStatus Start!"));
	m_chk_e_status_ion_status = bit;

	if(bit)
	{
		m_MelsecListPara[DEVW_USD_IONIZER_STATUS_0_15].wValue = 0x02;
		m_MelsecListPara[DEVW_USD_IONIZER_STATUS_16_31].wValue = 0x00;
	}
	else
	{
		m_MelsecListPara[DEVW_USD_IONIZER_STATUS_0_15].wValue = 0x00;
		m_MelsecListPara[DEVW_USD_IONIZER_STATUS_16_31].wValue = 0x00;
	}
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_USD_IONIZER_STATUS_0_15);
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_USD_IONIZER_STATUS_16_31);
	WriteLog(_T("SetBitEStatusIonStatus End!"));
	return TRUE;
}

BOOL CDialogInline::GetBitEStatusIonStatus()
{
	WriteLog(_T("GetBitEStatusIonStatus Start!"));
	SendMessage(USE_MSG_INLINE_CTRL, MSG_READ_ADDRS, DEVW_USD_IONIZER_STATUS_0_15);
	SendMessage(USE_MSG_INLINE_CTRL, MSG_READ_ADDRS, DEVW_USD_IONIZER_STATUS_16_31);
	
	m_chk_e_status_ion_status =	m_MelsecListPara[DEVW_USD_IONIZER_STATUS_0_15].wValue;
	WriteLog(_T("GetBitEStatusIonStatus End!"));
	return m_chk_e_status_ion_status;
}

BOOL CDialogInline::SetBitEStatusRecipeNo(int dat)
{
	WriteLog(_T("SetBitEStatusRecipeNo Start!"));
	m_edit_e_status_recipe_no = dat;

	m_MelsecListPara[DEVW_USD_CURRENT_EQUIPMENT_RECIPE_NUMBER].wValue = dat;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_USD_CURRENT_EQUIPMENT_RECIPE_NUMBER);
	WriteLog(_T("SetBitEStatusRecipeNo End!"));
	return TRUE;
}

int CDialogInline::GetBitEStatusRecipeNo()
{
	WriteLog(_T("GetBitEStatusRecipeNo Start!"));
	SendMessage(USE_MSG_INLINE_CTRL, MSG_READ_ADDRS, DEVW_USD_CURRENT_EQUIPMENT_RECIPE_NUMBER);
	m_edit_e_status_recipe_no = m_MelsecListPara[DEVW_USD_CURRENT_EQUIPMENT_RECIPE_NUMBER].wValue;
	WriteLog(_T("GetBitEStatusRecipeNo End!"));
	return m_edit_e_status_recipe_no;
}

BOOL CDialogInline::SetBitEStatusGlassCnt(int dat)
{
	WriteLog(_T("SetBitEStatusGlassCnt Start!"));
	m_edit_e_status_glass_cnt = dat;

	m_MelsecListPara[DEVW_USD_GLASS_COUNT_IN_UNIT].wValue = dat;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_USD_GLASS_COUNT_IN_UNIT);
	WriteLog(_T("SetBitEStatusGlassCnt End!"));
	return TRUE;
}

int CDialogInline::GetBitEStatusGlassCnt()
{
	WriteLog(_T("GetBitEStatusGlassCnt Start!"));
	SendMessage(USE_MSG_INLINE_CTRL, MSG_READ_ADDRS, DEVW_USD_GLASS_COUNT_IN_UNIT);
	m_edit_e_status_glass_cnt = m_MelsecListPara[DEVW_USD_GLASS_COUNT_IN_UNIT].wValue;
	WriteLog(_T("GetBitEStatusGlassCnt End!"));
	return m_edit_e_status_glass_cnt;
}

BOOL CDialogInline::SetBitEStatusGlassPosiCnt(int dat)
{
	WriteLog(_T("SetBitEStatusGlassPosiCnt Start!"));
	m_edit_e_status_glass_posi_cnt = dat;

	m_MelsecListPara[DEVW_USD_PUT_INTO_POSSIBLE_COUNT].wValue = dat;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_USD_PUT_INTO_POSSIBLE_COUNT);
	WriteLog(_T("SetBitEStatusGlassPosiCnt End!"));
	return TRUE;
}

int CDialogInline::GetBitEStatusGlassPosiCnt()
{
	WriteLog(_T("GetBitEStatusGlassPosiCnt Start!"));
	SendMessage(USE_MSG_INLINE_CTRL, MSG_READ_ADDRS, DEVW_USD_PUT_INTO_POSSIBLE_COUNT);
	m_edit_e_status_glass_posi_cnt = m_MelsecListPara[DEVW_USD_PUT_INTO_POSSIBLE_COUNT].wValue;
	WriteLog(_T("GetBitEStatusGlassPosiCnt End!"));
	return m_edit_e_status_glass_posi_cnt;
}

int CDialogInline::GetHostRecipeNumber()
{
	int value = 0;
	WriteLog(_T("GetBitEStatusGlassPosiCnt Start!"));
	SendMessage(USE_MSG_INLINE_CTRL, MSG_READ_ADDRS, ECS_W_ADDRESS_EQUIPMENT_RECIPE_NUMBER);
	value = m_MelsecListPara[ECS_W_ADDRESS_EQUIPMENT_RECIPE_NUMBER].wValue;
	WriteLog(_T("GetBitEStatusGlassPosiCnt End!"));
	return value;
}

void CDialogInline::SetLightAlarmCode(int dat, BOOL on_off)
{
	WriteLog(_T("SetLightAlarmCode Start!"));
	BOOL bit[16];
	WORD word;

	WordToBit(dat, bit);
	bit[15] = on_off;
	BitToWord(bit, &word);
	m_MelsecListPara[DEVW_USD_LIGHT_ALARM_CODE].wValue = word;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_USD_LIGHT_ALARM_CODE);
	WriteLog(_T("SetLightAlarmCode End!"));
}

void CDialogInline::SetHeavyAlarmCode(int dat, BOOL on_off)
{
	WriteLog(_T("SetHeavyAlarmCode Start!"));
	BOOL bit[16];
	WORD word;

	WordToBit(dat, bit);
	bit[15] = on_off;
	BitToWord(bit, &word);
	m_MelsecListPara[DEVW_USD_HEAVY_ALARM_CODE].wValue = word;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_USD_HEAVY_ALARM_CODE);
	WriteLog(_T("SetHeavyAlarmCode End!"));
}

BOOL CDialogInline::SetLightAlarmReport(BOOL bit, int dat)
{
	WriteLog(_T("SetLightAlarmReport Start!"));
	m_nLightAlarmCode = dat;
	m_MelsecListPara[DEVB_LIGHT_ALARM_REPORT].bValue = TRUE;
	SetLightAlarmCode(dat, bit);
	m_EQPStatus.bLightAlarm = bit;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_LIGHT_ALARM_REPORT);
	WriteLog(_T("SetLightAlarmReport End!"));
	return TRUE;
}

BOOL CDialogInline::SetHeavyAlarmReport(BOOL bit, int dat)
{
	WriteLog(_T("SetHeavyAlarmReport Start!"));
	m_nHeavyAlarmCode = dat;
	m_MelsecListPara[DEVB_HEAVY_ALARM_REPORT].bValue = TRUE;
	SetHeavyAlarmCode(dat, bit);
	m_EQPStatus.bHeavyAlarm = bit;
	m_EQPStatus.bDownAlarm = bit;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_HEAVY_ALARM_REPORT);
	if(bit)
	{
		SetEStatus(STATUS_DOWN);
	}
	else
	{
		SetEStatus(STATUS_IDLE);
	}

	InitSeqBit(DOWN_ALARM);

	WriteLog(_T("SetHeavyAlarmReport End!"));
	return TRUE;
}

void CDialogInline::SetRecipeBodyRequest(BOOL on_off)
{
	WriteLog(_T("SetRecipeBodyRequest Start!"));
	WORD wWord = 0;
	
	if(on_off)
	{
		m_PLCComm->Read(DevW, ECS_W_ADDRESS_RBD_REQUEST_RECIPE_NUMBER, ECS_W_ADDRESS_RBD_REQUEST_RECIPE_NUMBER, &wWord, 1);
		m_nRequestRecipeNum = wWord;
		if(SetRecipeBodyDataWrite(m_RecipeData[m_nRequestRecipeNum].bRecipeBody))
		{
			WriteRecipe(m_RecipeBodyData.nDataCEID, 0);
		}
	}

	m_MelsecListPara[DEVB_RECIPE_BODY_DATA_REPORT].bValue = on_off;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_RECIPE_BODY_DATA_REPORT);

	if(on_off)
	{
		//Sleep(200);
		SetTimer(TIMER_RECIPE_BODY_DATA_REPORT, 100, NULL);
	}
	WriteLog(_T("SetRecipeBodyRequest End!"));
}

BOOL CDialogInline::SetRecipeBodyDataWrite(BOOL isEmpty)
{
	WriteLog(_T("SetRecipeBodyDataWrite Start!"));
	CString strTemp;
	if((m_nRequestRecipeNum == GetBitEStatusRecipeNo()) && isEmpty)
	{
		m_RecipeBodyData.nDataCEID = 3;
		m_RecipeBodyData.nRecipeNumber = GetBitEStatusRecipeNo();
		m_RecipeBodyData.nConfirmAck = 0;
		strTemp.Format(_T("RecipeConfirm OK = RequestRecipeNum = %d, EQPRecipeNum = %d"), m_nRequestRecipeNum, GetBitEStatusRecipeNo());
		WriteLog(strTemp);
	}
	else if((m_nRequestRecipeNum != GetBitEStatusRecipeNo()) && isEmpty)
	{
		m_RecipeBodyData.nDataCEID = 3;
		m_RecipeBodyData.nRecipeNumber = m_nRequestRecipeNum;
		m_RecipeBodyData.nConfirmAck = 2;
		strTemp.Format(_T("Recipe Confirm Not Accept = RequestRecipeNum = %d, EQPRecipeNum = %d"), m_nRequestRecipeNum, GetBitEStatusRecipeNo());
		WriteLog(strTemp);
	}
	else if(!isEmpty)
	{
		m_RecipeBodyData.nDataCEID = 3;
		m_RecipeBodyData.nRecipeNumber = m_nRequestRecipeNum;
		m_RecipeBodyData.nConfirmAck = 1;
		strTemp.Format(_T("Invalid Recipe Number = RequestRecipeNum = %d, EQPRecipeNum = %d"), m_nRequestRecipeNum, GetBitEStatusRecipeNo());
		WriteLog(strTemp);
	}
	
	m_RecipeBodyData.cRecipeType = 'M';
	m_PLCComm->SetWORD(PLC_DEVW_RBR_RECIPE_REPORT_CEID, m_RecipeBodyData.nDataCEID);
	m_PLCComm->SetWORD(PLC_DEVW_RBR_RECIPE_NUMBER, m_RecipeBodyData.nRecipeNumber);
	m_PLCComm->SetWORD(PLC_DEVW_RBR_RECIPE_CONFIRM_ACK, m_RecipeBodyData.nConfirmAck);
	if(!isEmpty)
	{
		WriteLog(_T("SetRecipeBodyDataWrite Fail!"));
		return FALSE;
	}
	m_PLCComm->SetWORD(PLC_DEVW_RBR_RECIPE_LEVEL, 2);
	m_PLCComm->SetWORD(PLC_DEVW_RBR_RECIPE_TYPE, m_RecipeBodyData.cRecipeType);
	m_PLCComm->SetWORD(PLC_DEVW_RBR_RECIPE_VERSION, m_RecipeBodyData.wRecipeVersion[0]);
	m_PLCComm->SetWORD(PLC_DEVW_RBR_RECIPE_VERSION + 1, m_RecipeBodyData.wRecipeVersion[1]);
	m_PLCComm->SetWORD(PLC_DEVW_RBR_RECIPE_VERSION + 1, m_RecipeBodyData.wRecipeVersion[2]);

	WriteLog(_T("SetRecipeBodyDataWrite End!"));
	return TRUE;
}

BOOL CDialogInline::SetDownAlarmCode(int dat)
{
	WriteLog(_T("SetDownAlarmCode Start!"));
	m_nDownAlarmCode = dat;
	m_MelsecListPara[DEVW_USD_UNIT_DOWN_ALARM_CODE].wValue = dat;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_USD_UNIT_DOWN_ALARM_CODE);
	WriteLog(_T("SetDownAlarmCode End!"));
	return TRUE;
}

BEGIN_EVENTSINK_MAP(CDialogInline, CDialogEx)
	ON_EVENT(CDialogInline, IDC_BTNENHCTRL_INLINE_APPLY, DISPID_CLICK, CDialogInline::ClickBtnenhctrlInlineApply, VTS_NONE)
	ON_EVENT(CDialogInline, IDC_BTNENHCTRL_INLINE_CANCEL, DISPID_CLICK, CDialogInline::ClickBtnenhctrlInlineCancel, VTS_NONE)
	ON_EVENT(CDialogInline, IDC_BTNENHCTRL_INLINE_LOADER_GLASS_READ, DISPID_CLICK, CDialogInline::ClickBtnenhctrlInlineLoaderGlassRead, VTS_NONE)
	ON_EVENT(CDialogInline, IDC_BTNENHCTRL_INLINE_LOADER_GLASS_READ2, DISPID_CLICK, CDialogInline::ClickBtnenhctrlInlineLoaderGlassRead2, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogInline::ClickBtnenhctrlInlineApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);
	GlassDataUpdate(TRUE);
	UpdateTimeoutTrigger(TRUE);
	SaveTimeoutTrigger();
	SaveTemp();

	m_LoaderGlassData = m_UpdateGlassData;

	if(!pdlg->m_GlassManu->GetLoaderUse())
	{
		m_UnloaderGlassData = m_UpdateGlassData;
	}
	
	//SetUnloaderGlassData(m_UpdateGlassData);
}


BOOL CDialogInline::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	WriteLog(_T("InlineDlg Oninit Start!"));
	if(MelsecPLCInit())
	{
		m_bMelsecConnect = TRUE;
	}
	else
	{
		m_bMelsecConnect = FALSE;
	}

	//UpdateMelsecList();

	//ECS////////////////////////////////////
	//On-line
	LoadTimeoutTrigger();
	UpdateTimeoutTrigger(FALSE);
	m_EQPStatus.bOnline= TRUE;
	//SetBitOModeOnline(TRUE);

	InitSeqBit(INIT_SEQ_BIT);

	//SetTimer(TIMER_ECS_START_STATUE_CHECK, 100, NULL);//ecs start status
	SetTimer(TIMER_ECS_TIME_DATA_GET, 100, NULL);//ecs time sync
	SetTimer(TIMER_RECIPE_BODY_DATA_REQUEST, 100, NULL);// eqp recipe request
	SetTimer(TIMER_RECIPE_VARIOUS_DATA_REQUEST, 100, NULL);// eqp various request

	WriteLog(_T("InlineDlg Oninit End!"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogInline::InitGlassPosition()
{
	WriteLog(_T("InitGlassPosition Start!"));
	typeLostGlassDataPara lostGlass;
	memset(&lostGlass, NULL, sizeof(typeLostGlassDataPara));

	//if(m_EQPStatus.bGlassExistInUnit)
	{
		LoadTemp();
		m_UpdateGlassData = m_LoaderGlassData;
		lostGlass.nGlassOption = LOST_GLASS_DATA_OPTION_CODE;
		lostGlass.nGlassCode = m_LoaderGlassData.nGlassCode;

		SetLostGlassDataReport(lostGlass);
		WriteLostGlassData(lostGlass.nGlassOption);
		SetLostGlassDataReport(TRUE);
	}

	WriteLog(_T("InitGlassPosition End!"));
}

void CDialogInline::WordToBit(WORD word, BOOL *bit)
{
	for(int i = 15; i > -1; i--)
	{
		bit[i] = (word>>i) & 0x00000001;
	}
}

void CDialogInline::BitToWord(BOOL *bit, WORD *word)
{
	WORD data = 0;

	for(int i = 15; i > -1; i--)
	{
		data = data|(bit[i]<<i);
	}

	*word = data;
}

void CDialogInline::WordToString(WORD word, char *str)
{
	str[1] = (word>>8) & 0x00FF;
	str[0] = word & 0x00FF;
}

void CDialogInline::StringToWord(char str[2], WORD *word)
{
	WORD data = 0;

	data = data|(str[1]<<8);
	data = data|str[0];

	*word = data;
}

void CDialogInline::ListInit()
{
	LV_COLUMN m_lvColumn;
	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_LEFT;
	m_lvColumn.pszText=_T("Address");
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=100;
	//m_list_melsec_addr1_word.InsertColumn(0, &m_lvColumn);
	m_lvColumn.pszText=_T("Name");
	m_lvColumn.iSubItem=1;
	m_lvColumn.cx=300;
	//m_list_melsec_addr1_word.InsertColumn(1, &m_lvColumn);
	m_lvColumn.pszText=_T("Value");
	m_lvColumn.iSubItem=2;
	m_lvColumn.cx=50;
	//m_list_melsec_addr1_word.InsertColumn(2, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_LEFT;
	m_lvColumn.pszText=_T("Address");
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=100;
	//m_list_melsec_addr2_word.InsertColumn(0, &m_lvColumn);
	m_lvColumn.pszText=_T("Name");
	m_lvColumn.iSubItem=1;
	m_lvColumn.cx=300;
	//m_list_melsec_addr2_word.InsertColumn(1, &m_lvColumn);
	m_lvColumn.pszText=_T("Value");
	m_lvColumn.iSubItem=2;
	m_lvColumn.cx=50;
	//m_list_melsec_addr2_word.InsertColumn(2, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_LEFT;
	m_lvColumn.pszText=_T("Address");
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=100;
	//m_list_melsec_addr1_bit.InsertColumn(0, &m_lvColumn);
	m_lvColumn.pszText=_T("Name");
	m_lvColumn.iSubItem=1;
	m_lvColumn.cx=300;
	//m_list_melsec_addr1_bit.InsertColumn(1, &m_lvColumn);
	m_lvColumn.pszText=_T("Value");
	m_lvColumn.iSubItem=2;
	m_lvColumn.cx=50;
	//m_list_melsec_addr1_bit.InsertColumn(2, &m_lvColumn);
}

BOOL CDialogInline::MelsecListParaLoad()
{
	WriteLog(_T("MelsecListParaLoad Start!"));
	char buffer[1000], *ps;
	CString path, strName, strAddr, strSize, temp, temp2, temp5;
	int line_cnt = 0;
	int cnt = 0;
	int all_cnt = 0;
	int type = 0;
	int v = 1;
	int temp3;
	char strFile[260];
	char string[256];
	char *stop;
	char temp4[256];
	CString strNum;
	path.Format(_T("%S\\MelsecList.par"), DATABASE_PATH);
	sprintf_s(strFile, path.GetLength() + 1, "%S", path);
	 
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
			Sleep(500);
			if(i == 2)
			{
				return FALSE;
			}
		}
	}
	 
	while (fgets(buffer, MAX_STRING_LINE, stream) != NULL) // 텍스트파일을 한줄씩 읽습니다. 
	{
		ps = strchr(buffer, '\n'); // 제일 뒤쪽의 new line의 char을 찿아.
		if (ps != NULL) *ps = '\0';// new line의 char을 null문자로 바꿉니다. 
	 
		temp.Format(_T("%S"), buffer);
		AfxExtractSubString(strName, temp, 0, '=');
		AfxExtractSubString(strAddr, temp, 1, '=');
		if(!AfxExtractSubString(strSize, temp, 2, '='))
		{
			strSize.Format(_T("1"));
		}


		sprintf_s(temp4, "%S", temp);

		if(temp4[12] == 'W')
		{
			if(temp4[29] == '1')
			{
				type = ADDRESS_TYPE_WORD_AREA1;
				cnt = 0;
				continue;
			}
			else if(temp4[29] == '2')
			{
				type = ADDRESS_TYPE_WORD_AREA2;
				cnt = 0;
				continue;
			}
		}
		if(temp4[12] == 'B')
		{
			if(temp4[28] == '1')
			{
				type = ADDRESS_TYPE_BIT_AREA1;
				cnt = 0;
				continue;
			}
		}

		sprintf_s(temp4, "%S", strName.Mid(0, strName.GetLength()-2));
		temp5 = strAddr;
		if(!strcmp(temp4, "PLC_DEVW_TPD_DATA_") && strName.GetLength() > 18)
		{
			for(int i = 0; i < 255; i++)
			{
				strName.Format(_T("PLC_DEVW_TPD_DATA_V%03d"), i + 1);
				sprintf_s(string, temp5.GetLength() + 1, "%S", temp5);
				temp3 = strtol(string,&stop,16) + i;
				strAddr.Format(_T("0x%04X"), temp3);
				//MelsecListView(type, cnt, strAddr, strName);
				cnt++;
				m_MelsecListPara[all_cnt].nAddrType = type;
				m_MelsecListPara[all_cnt].nSize = _wtoi(strSize);
				sprintf_s(m_MelsecListPara[all_cnt].strAddr, strAddr.GetLength() + 1, "%S", strAddr);
				sprintf_s(m_MelsecListPara[all_cnt].strName, strName.GetLength() + 1, "%S", strName);
				m_MelsecListPara[all_cnt].nAddr = HexToDec(strAddr, 4);
				all_cnt++;
			}		
			continue;
		}
		else if(!strcmp(temp4, "PLC_DEVW_APD_DATA_") && strName.GetLength() > 18)
		{
			for(int i = 0; i < PLC_DEVW_APD_DATA_CNT; i++)
			{
				strName.Format(_T("PLC_DEVW_APD_DATA_V%04d"), i + 1);
				sprintf_s(string, temp5.GetLength() + 1, "%S", temp5);
				temp3 = strtol(string,&stop,16) + i;
				strAddr.Format(_T("0x%04X"), temp3);
				//MelsecListView(type, cnt, strAddr, strName);
				cnt++;
				m_MelsecListPara[all_cnt].nAddrType = type;
				m_MelsecListPara[all_cnt].nSize = _wtoi(strSize);
				sprintf_s(m_MelsecListPara[all_cnt].strAddr, strAddr.GetLength() + 1, "%S", strAddr);
				sprintf_s(m_MelsecListPara[all_cnt].strName, strName.GetLength() + 1, "%S", strName);
				m_MelsecListPara[all_cnt].nAddr = HexToDec(strAddr, 4);
				all_cnt++;
			}		
			continue;
		}
		else if(!strcmp(temp4, "PLC_DEVW_ESP_EQUIPMENT_SPECIFIC_DATA_") && strName.GetLength() > 37)
		{
			for(int i = 0; i < PLC_DEVW_ESP_EQUIPMENT_SPECIFIC_DATA_CNT; i++)
			{
				strName.Format(_T("PLC_DEVW_ESP_EQUIPMENT_SPECIFIC_DATA_V%02d"), i + 1);
				sprintf_s(string, temp5.GetLength() + 1, "%S", temp5);
				temp3 = strtol(string,&stop,16) + i;
				strAddr.Format(_T("0x%05X"), temp3);
				//MelsecListView(type, cnt, strAddr, strName);
				cnt++;
				m_MelsecListPara[all_cnt].nAddrType = type;
				m_MelsecListPara[all_cnt].nSize = _wtoi(strSize);
				sprintf_s(m_MelsecListPara[all_cnt].strAddr, strAddr.GetLength() + 1, "%S", strAddr);
				sprintf_s(m_MelsecListPara[all_cnt].strName, strName.GetLength() + 1, "%S", strName);
				m_MelsecListPara[all_cnt].nAddr = HexToDec(strAddr, 5);
				all_cnt++;
			}		
			continue;
		}
		else if(!strcmp(temp4, "PLC_DEVW_RBD_RECIPE_BODY_") && strName.GetLength() > 25)
		{
			for(int i = 0; i < PLC_DEVW_RBD_RECIPE_BODY_CNT; i++)
			{
				strName.Format(_T("PLC_DEVW_RBD_RECIPE_BODY_V%03d"), i + 1);
				sprintf_s(string, temp5.GetLength() + 1, "%S", temp5);
				temp3 = strtol(string,&stop,16) + i;
				strAddr.Format(_T("0x%05X"), temp3);
				//MelsecListView(type, cnt, strAddr, strName);
				cnt++;
				m_MelsecListPara[all_cnt].nAddrType = type;
				m_MelsecListPara[all_cnt].nSize = _wtoi(strSize);
				sprintf_s(m_MelsecListPara[all_cnt].strAddr, strAddr.GetLength() + 1, "%S", strAddr);
				sprintf_s(m_MelsecListPara[all_cnt].strName, strName.GetLength() + 1, "%S", strName);
				m_MelsecListPara[all_cnt].nAddr = HexToDec(strAddr, 5);
				all_cnt++;
			}		
			continue;
		}
		else if(!strcmp(temp4, "PLC_DEVW_ESD_EQUIPMENT_SPECIFIC_DATA_") && strName.GetLength() > 37)
		{
			for(int i = 0; i < PLC_DEVW_ESD_EQUIPMENT_SPECIFIC_DATA_CNT; i++)
			{
				strName.Format(_T("PLC_DEVW_ESD_EQUIPMENT_SPECIFIC_DATA_V%02d"), i + 1);
				sprintf_s(string, temp5.GetLength() + 1, "%S", temp5);
				temp3 = strtol(string,&stop,16) + i;
				strAddr.Format(_T("0x%05X"), temp3);
				//MelsecListView(type, cnt, strAddr, strName);
				cnt++;
				m_MelsecListPara[all_cnt].nAddrType = type;
				m_MelsecListPara[all_cnt].nSize = _wtoi(strSize);
				sprintf_s(m_MelsecListPara[all_cnt].strAddr, strAddr.GetLength() + 1, "%S", strAddr);
				sprintf_s(m_MelsecListPara[all_cnt].strName, strName.GetLength() + 1, "%S", strName);
				m_MelsecListPara[all_cnt].nAddr = HexToDec(strAddr, 5);
				all_cnt++;
			}		
			continue;
		}
		else if(!strcmp(temp4, "PLC_DEVW_RP_RECIPE_PARAMETER_") && strName.GetLength() > 30)
		{
			for(int i = 0; i < PLC_DEVW_RP_RECIPE_PARAMETER_CNT; i++)
			{
				strName.Format(_T("PLC_DEVW_RP_RECIPE_PARAMETER_V%03d"), i + 1);
				sprintf_s(string, temp5.GetLength() + 1, "%S", temp5);
				temp3 = strtol(string,&stop,16) + i;
				strAddr.Format(_T("0x%05X"), temp3);
				//MelsecListView(type, cnt, strAddr, strName);
				cnt++;
				m_MelsecListPara[all_cnt].nAddrType = type;
				m_MelsecListPara[all_cnt].nSize = _wtoi(strSize);
				sprintf_s(m_MelsecListPara[all_cnt].strAddr, strAddr.GetLength() + 1, "%S", strAddr);
				sprintf_s(m_MelsecListPara[all_cnt].strName, strName.GetLength() + 1, "%S", strName);
				m_MelsecListPara[all_cnt].nAddr = HexToDec(strAddr, 5);
				all_cnt++;
			}		
			continue;
		}

		m_MelsecListPara[all_cnt].nAddrType = type;
		m_MelsecListPara[all_cnt].nSize = _wtoi(strSize);
		sprintf_s(m_MelsecListPara[all_cnt].strAddr, strAddr.GetLength() + 1, "%S", strAddr);
		sprintf_s(m_MelsecListPara[all_cnt].strName, strName.GetLength() + 1, "%S", strName);
		//MelsecListView(type, cnt, strAddr, strName);	

		if(type == ADDRESS_TYPE_WORD_AREA1)
		{
			m_MelsecListPara[all_cnt].nAddr = HexToDec(strAddr, 4);
		}
		else if(type == ADDRESS_TYPE_WORD_AREA2)
		{
			m_MelsecListPara[all_cnt].nAddr = HexToDec(strAddr, 5);
		}
		else if(type == ADDRESS_TYPE_BIT_AREA1)
		{
			m_MelsecListPara[all_cnt].nAddr = HexToDec(strAddr, 4);
		}

		cnt++;
		all_cnt++;
	}
	delete ps;
	fclose(stream);
	m_nAllMelsecListCnt = all_cnt - 1;
	WriteLog(_T("MelsecListParaLoad End!"));
	return TRUE;
}

int CDialogInline::HexToDec(CString strHex, int size)
{
	int rslt = 0;
	int temp[6];
	char cHex[6];

	sprintf_s(cHex, size + 1, "%S", strHex.Mid(2, size));

	for(int i = 0; i < size; i++)
	{
		if(cHex[i] == 'A')
		{
			temp[i] = 10;
		}
		else if(cHex[i] == 'B')
		{
			temp[i] = 11;
		}
		else if(cHex[i] == 'C')
		{
			temp[i] = 12;
		}
		else if(cHex[i] == 'D')
		{
			temp[i] = 13;
		}
		else if(cHex[i] == 'E')
		{
			temp[i] = 14;
		}
		else if(cHex[i] == 'F')
		{
			temp[i] = 15;
		}
		else if(_wtoi(strHex.Mid(i + 2, 1)) < 10)
		{
			temp[i] = _wtoi(strHex.Mid(i + 2, 1));
		}
	}
	if(size == 4)
	{
		rslt = (temp[0] * (16 * 16 * 16)) + (temp[1] * (16 * 16)) + (temp[2] * 16) + (temp[3]);
	}
	else if(size == 5)
	{
		rslt = (temp[0] * (16 * 16 * 16 * 16)) + (temp[1] * (16 * 16 * 16)) + (temp[2] * (16 * 16)) + (temp[3] * 16) + (temp[4]);
	}
	return rslt;
}

void CDialogInline::MelsecListView(int type, int line_cnt, CString strAddr, CString strName)
{
	CString temp, temp2;

	if(type == ADDRESS_TYPE_WORD_AREA1)
	{
		//m_list_melsec_addr1_word.InsertItem(line_cnt, (LPCTSTR)strAddr);
		//m_list_melsec_addr1_word.SetItemText(line_cnt, 1, (LPCTSTR)strName);
	}
	else if(type == ADDRESS_TYPE_WORD_AREA2)
	{
		//m_list_melsec_addr2_word.InsertItem(line_cnt, (LPCTSTR)strAddr);
		//m_list_melsec_addr2_word.SetItemText(line_cnt, 1, (LPCTSTR)strName);
	}
	else if(type == ADDRESS_TYPE_BIT_AREA1)
	{
		//m_list_melsec_addr1_bit.InsertItem(line_cnt, (LPCTSTR)strAddr);
		//m_list_melsec_addr1_bit.SetItemText(line_cnt, 1, (LPCTSTR)strName);
	}
}

void CDialogInline::InitMelsecItem()
{
	WriteLog(_T("InitMelsecItem Start!"));
	int idx = 0;
	CString temp;
	//Memory Map
	m_MelsecAddr[idx].nType = DevW;
	m_MelsecAddr[idx].nStartAddress = 0x0000;
	m_MelsecAddr[idx++].nEndAddress = 0x27FF;

	m_MelsecAddr[idx].nType = DevW;
	m_MelsecAddr[idx].nStartAddress = 0x00010000;
	m_MelsecAddr[idx++].nEndAddress = 0x000147FF;

	m_MelsecAddr[idx].nType = DevB;
	m_MelsecAddr[idx].nStartAddress = 0x0000;
	m_MelsecAddr[idx++].nEndAddress = PLC_DEVB_END_ADDRESS;

	//m_MelsecAddr[idx].nType = DevW;
	//m_MelsecAddr[idx].nStartAddress = 0x0000;
	//m_MelsecAddr[idx++].nEndAddress = 0x147FF;

	///*m_MelsecAddr[idx].nType = DevW;
	//m_MelsecAddr[idx].nStartAddress = 0x10000;
	//m_MelsecAddr[idx++].nEndAddress = 0x14FFFF;*/

	//m_MelsecAddr[idx].nType = DevB;
	//m_MelsecAddr[idx].nStartAddress = 0x0000;
	//m_MelsecAddr[idx++].nEndAddress = 0x13FF;

	//Memory Item
	int k = 0;
	for(int i = 0; i < 3; i++)
	{
		k = 0;
		for(idx = 0; idx < MAX_ITEM_CNT; idx++)
		{
			if(m_MelsecListPara[idx].nAddrType == 0)
			{
				SetMemoryItem(idx, DevW, m_MelsecListPara[idx].nAddr, m_MelsecListPara[idx].nSize);//atoi(m_MelsecListPara[idx].strAddr), m_MelsecListPara[idx].nSize);
			}
			else if(m_MelsecListPara[idx].nAddrType == 1)
			{
				SetMemoryItem(idx, DevW, m_MelsecListPara[idx].nAddr, m_MelsecListPara[idx].nSize);//atoi(m_MelsecListPara[idx].strAddr), m_MelsecListPara[idx].nSize);
			}
			else if(m_MelsecListPara[idx].nAddrType == 2)
			{
				SetMemoryItem(idx, DevB, m_MelsecListPara[idx].nAddr, m_MelsecListPara[idx].nSize);//atoi(m_MelsecListPara[idx].strAddr), m_MelsecListPara[idx].nSize);
			}
		}
	}
	WriteLog(_T("InitMelsecItem End!"));
}

void CDialogInline::SetMemoryItem(int idx, int nType, int nAddr, int nSize)
{
	m_MelsecItem[idx].nType = nType;
	m_MelsecItem[idx].nAddress = nAddr;
	m_MelsecItem[idx].nSize = nSize;
}

BOOL CDialogInline::MelsecPLCInit()
{
	WriteLog(_T("MelsecPLCInit Start!"));
	BOOL rslt = TRUE;
	m_bOpened = TRUE;
	if(!MelsecListParaLoad())
	{
		m_strLogString.Format(_T("Melsec List Load Error"));
		WriteLog(m_strLogString);
	}

	if(!m_PLCComm->Init(MELSEC_CHANNEL_NO, MELSEC_STATION_NO, MELSEC_PLC_STATION))
	{
		m_strLogString.Format(_T("Melsec Init Error"));	
		WriteLog(m_strLogString);
		rslt = FALSE;
		m_bOpened = FALSE;

	}
	else
	{
		
	}

	InitMelsecItem();
	//ComboInit();

	if(rslt)
	{
		for( int nMap=0; ; nMap++)
		{
			if(m_MelsecAddr[nMap].nType==0) break;
			m_PLCComm->AddAddress(m_MelsecAddr[nMap].nType, m_MelsecAddr[nMap].nStartAddress, m_MelsecAddr[nMap].nEndAddress);
		}
	
		for(int nItem=0; ; nItem++) 
		{
			if(m_MelsecItem[nItem].nType==0) break;
			m_PLCComm->AddAddressSize(m_MelsecItem[nItem].nType, m_MelsecItem[nItem].nAddress, m_MelsecItem[nItem].nSize);
		}
	}
	WriteLog(_T("MelsecPLCInit End!"));
	return rslt;
}

CString CDialogInline::GetMCSCTime(int nAddress)
{
	WriteLog(_T("GetMCSCTime Start!"));
	CString strTime;
	strTime.Empty();
	if(m_bMelsecConnect)
	{
		WORD wYear = m_PLCComm->GetWORD(nAddress);
		WORD wMonth = m_PLCComm->GetWORD(nAddress+1);
		WORD wDay = m_PLCComm->GetWORD(nAddress+2);
		WORD wHour = m_PLCComm->GetWORD(nAddress+3);
		WORD wMinute = m_PLCComm->GetWORD(nAddress+4);
		WORD wSecond = m_PLCComm->GetWORD(nAddress+5);
		//WORD wMilliseconds = m_PLCComm->GetWORD(nAddress+6);
	
		//strTime.Format(_T("%04d%02d%02d%02d%02d%02d%03d"), wYear, wMonth, wDay, wHour, wMinute, wSecond, wMilliseconds)
		strTime.Format(_T("%04d%02d%02d%02d%02d%02d"), wYear, wMonth, wDay, wHour, wMinute, wSecond);
		SYSTEMTIME lptime;
		GetLocalTime(&lptime);
		lptime.wYear = wYear;
		lptime.wMonth = wMonth;
		lptime.wDay = wDay;
		lptime.wHour = wHour;
		lptime.wMinute = wMinute;
		lptime.wSecond = wSecond;
		SetLocalTime(&lptime);
		//::set
	}
	WriteLog(_T("GetMCSCTime End!"));
	return strTime;
}

int CDialogInline::GetMelsecErrValue()
{
	int nErr = 0;
	if(m_bMelsecConnect)
	{
		nErr = m_PLCComm->GetMelsecErr();
	}
	else
	{
		nErr = MELSEC_STATUS_DISCONNECT;
	}

	return nErr;
}

CString CDialogInline::GetMelsecErrorMsg()  // melsec error 
{
	CString strErr;
	
	//char strErr[512] = {0,};

	int nErrorCode = 0;
	if(m_bMelsecConnect)
	{
		nErrorCode = m_PLCComm->GetMelsecErr();

		switch(nErrorCode) 
		{
		case 0:
			strErr.Format(_T("정상"));
			return strErr;

		case 100:
			strErr.Format(_T("Melsec Board H/W resource busy : pc 재부팅 필요함."));
			return strErr;

		case 102:
			strErr.Format(_T("Melsec Board Driver I/F error : pc 재부팅 필요함."));
			return strErr;

		case 103:
			strErr.Format(_T("Melsec Board Driver I/F error : pc 재부팅 필요함."));
			return strErr;

		case 133:
			strErr.Format(_T("Melsec Parameter error : pc 재부팅 필요함."));	
			return strErr;

		case -32:
			strErr.Format(_T("Melsec Resource time-out error : pc 재부팅 필요함."));
			return strErr;

		case -37:
			strErr.Format(_T("Melsec Communication initialization setting error : pc 재부팅 필요함."));
			return strErr;

		case -42:
			strErr.Format(_T("Melsec Close error : pc 재부팅 필요함."));
			return strErr;

		case -65:
			strErr.Format(_T("Melsec Event wait time-out error : pc 재부팅 필요함."));
			return strErr;

		case -66:
			strErr.Format(_T("Melsec Event initialization error : pc 재부팅 필요함."));
			return strErr;

		case -26334:
			strErr.Format(_T("Melsec Reset error : pc 재부팅 필요함."));
			return strErr;

		case -28158:
			strErr.Format(_T("Melsec Driver WDT error : pc 재부팅 필요함."));
			return strErr;

		case -28622:
			strErr.Format(_T("Melsec Channel busy (dedicated instruction) error : pc 재부팅 필요함."));
			return strErr;
	
		case -28634:
			strErr.Format(_T("Melsec Hardware self-diagnosis error : pc 재부팅 필요함."));
			return strErr;

		case -28636:
			strErr.Format(_T("Melsec Hardware self-diagnosis error : pc 재부팅 필요함."));
			return strErr;
		
		default: 
			strErr.Format(_T("Cannot Find Error Information : Melsec manual 참고"));
			return strErr;
		}
	}
	else
	{
		strErr.Format(_T("Melsec DisConnect : 연결 상태 확인."));
		return strErr;
	}
	WriteLog(strErr);
	return strErr;
}

void CDialogInline::TimerReadMemory(int read_type, BOOL isRead, int millisec)
{
	if(isRead)
	{
		SetTimer(read_type, millisec, NULL);
	}
	else
	{
		switch(read_type)
		{
		case MELSEC_ALL_ADDRESS_READ :
			if(m_bAllValueRead)
				KillTimer(MELSEC_ALL_ADDRESS_READ);
			break;
		case MELSEC_WORD_ADDRESS1_READ:
			if(m_bWord1ValueRead)
				KillTimer(MELSEC_WORD_ADDRESS1_READ);
			break;
		case MELSEC_WORD_ADDRESS2_READ:
			if(m_bWord2ValueRead)
				KillTimer(MELSEC_WORD_ADDRESS2_READ);
			break;
		case MELSEC_BIT_ADDRESS_READ:
			if(m_bBitValueRead)
				KillTimer(MELSEC_BIT_ADDRESS_READ);
			break;
		}
	}
}

void CDialogInline::ReadMemory(int type)
{
	if(m_bMelsecConnect)
	{
		switch(type)
		{
		case MELSEC_ALL_ADDRESS_READ:
			m_PLCComm->Read(DevW, PLC_DEVW_BASE_ADDRESS1, PLC_DEVW_END_ADDRESS1);
			m_PLCComm->Read(DevW, PLC_DEVW_BASE_ADDRESS2, PLC_DEVW_END_ADDRESS2);
			m_PLCComm->Read(DevB, PLC_DEVB_BASE_ADDRESS, PLC_DEVB_END_ADDRESS);
			break;
		case MELSEC_WORD_ADDRESS1_READ:
			m_PLCComm->Read(DevW, PLC_DEVW_BASE_ADDRESS1, PLC_DEVW_END_ADDRESS1);
			break;
		case MELSEC_WORD_ADDRESS2_READ:
			m_PLCComm->Read(DevW, PLC_DEVW_BASE_ADDRESS2, PLC_DEVW_END_ADDRESS2);
			break;
		case MELSEC_BIT_ADDRESS_READ:
			m_PLCComm->Read(DevB, PLC_DEVB_BASE_ADDRESS, PLC_DEVB_END_ADDRESS);
			break;
		}
	}
	UpdateMelsecList();
}

void CDialogInline::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow)
	{
		GlassDataUpdate(FALSE);
		TimeOutParameterEnable(pdlg->GetAccessType());
		UpdateData(FALSE);
		if(pdlg->GetAccessType() == 3)
		{
			GetDlgItem(IDC_BUTTON_DELETE_GLASS_DATA)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_DELETE_GLASS_DATA)->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		GlassDataUpdate(TRUE);
		KillTimer(10);
	}
}


void CDialogInline::UpdateMelsecList()
{
	UpdateValue();
	UpdateListValue();
	Update();
}

void CDialogInline::UpdateValue()
{
	if(m_bMelsecConnect)
	{
		for(int i = 0; i < MAX_ITEM_CNT; i++)
		{
			if(m_MelsecListPara[i].nAddrType == ADDRESS_TYPE_WORD_AREA1)
			{
				m_MelsecListPara[i].wValue = m_PLCComm->GetWORD(m_MelsecItem[i].nAddress, FALSE);
			}
			else if(m_MelsecListPara[i].nAddrType == ADDRESS_TYPE_WORD_AREA2)
			{
				m_MelsecListPara[i].wValue = m_PLCComm->GetWORD(m_MelsecItem[i].nAddress, FALSE);
			}
			else if(m_MelsecListPara[i].nAddrType == ADDRESS_TYPE_BIT_AREA1)
			{
				m_MelsecListPara[i].bValue = m_PLCComm->GetBit(m_MelsecItem[i].nAddress, FALSE);
			}
		}
	}
}

void CDialogInline::Update()
{
	SetBitEStatusRun(m_MelsecListPara[DEVB_RUN].bValue);
	SetBitEStatusIdle(m_MelsecListPara[DEVB_IDLE].bValue);
	SetBitEStatusDown(m_MelsecListPara[DEVB_DOWN].bValue);
	SetBitEStatusGlassInPrcs(m_MelsecListPara[DEVB_GLASS_IN_PROCESSING].bValue);
	SetBitEStatusGlassExist(m_MelsecListPara[DEVB_GLASS_EXIST_IN_UNIT].bValue);
	SetBitOModeOnline(m_MelsecListPara[DEVB_ONLINE].bValue);
	SetBitOModeUnitAuto(m_MelsecListPara[DEVB_AUTO_MODE].bValue);
	SetBitOModeOperation_AutoOrManual(m_MelsecListPara[DEVB_ONLINE].bValue);
	SetBitEStatusIonStatus(m_MelsecListPara[DEVB_ONLINE].wValue);
	SetBitEStatusRecipeNo(m_MelsecListPara[DEVB_ONLINE].wValue);
	SetBitEStatusGlassCnt(m_MelsecListPara[DEVB_ONLINE].wValue);
	SetBitEStatusGlassPosiCnt(m_MelsecListPara[DEVB_ONLINE].wValue);
	UpdateData(FALSE);
}

void CDialogInline::UpdateListValue()
{
	CString temp, temp1;
	char strWord[2];
	BOOL bit[16];
	strWord[0] = 0;
	strWord[1] = 0;
	int w1_idx = 0, w2_idx = 0, b_idx = 0;
	for(int i = 0; i < MAX_ITEM_CNT; i++)
	{
		if(m_MelsecListPara[i].nAddrType == ADDRESS_TYPE_WORD_AREA1)
		{
			temp.Format(_T("%S"), m_MelsecListPara[i].strValue);
			//m_list_melsec_addr1_word.SetItemText(w1_idx++, 2, temp);
		}
		else if(m_MelsecListPara[i].nAddrType == ADDRESS_TYPE_WORD_AREA2)
		{
			temp.Format(_T("%S"), m_MelsecListPara[i].strValue);
			//m_list_melsec_addr2_word.SetItemText(w2_idx++, 2, temp);
		}
		else if(m_MelsecListPara[i].nAddrType == ADDRESS_TYPE_BIT_AREA1)
		{
			temp.Format(_T("%S"), m_MelsecListPara[i].strValue);
			//m_list_melsec_addr1_bit.SetItemText(b_idx++, 2, temp);
		}
	}
}

void CDialogInline::SetValue(int idx, WORD wValue)
{
	m_MelsecListPara[idx].wValue = wValue;
	m_strLogString.Format(_T("SetValue WORD -> ITEM_NAME : %S, Value : %X"), m_MelsecListPara[idx].strName, m_MelsecListPara[idx].wValue);
	WriteLog(m_strLogString);
	if(m_bMelsecConnect)
		m_PLCComm->SetWORD(m_MelsecItem[idx].nAddress, m_MelsecListPara[idx].wValue);
}

void CDialogInline::SetValue(int idx, BOOL bValue)
{
	m_MelsecListPara[idx].bValue = bValue;
	m_strLogString.Format(_T("SetValue Bit -> ITEM_NAME : %S, Value : %d"), m_MelsecListPara[idx].strName, m_MelsecListPara[idx].bValue);
	WriteLog(m_strLogString);
	if(m_bMelsecConnect)
		m_PLCComm->SetBit(m_MelsecItem[idx].nAddress, m_MelsecListPara[idx].bValue);
}

WORD CDialogInline::GetWordValue(int idx)
{
	if(m_bMelsecConnect)
		m_MelsecListPara[idx].wValue = m_PLCComm->GetWORD(m_MelsecItem[idx].nAddress);
	m_strLogString.Format(_T("GetValue WORD -> ITEM_NAME : %S, Value : %X"), m_MelsecListPara[idx].strName, m_MelsecListPara[idx].wValue);
	WriteLog(m_strLogString);
	return m_MelsecListPara[idx].wValue;
}

BYTE CDialogInline::GetBitValue(int idx)
{
	if(m_bMelsecConnect)
		m_MelsecListPara[idx].bValue = m_PLCComm->GetBit(m_MelsecItem[idx].nAddress);
	m_strLogString.Format(_T("GetValue Bit -> ITEM_NAME : %S, Value : %d"), m_MelsecListPara[idx].strName, m_MelsecListPara[idx].bValue);
	WriteLog(m_strLogString);
	return m_MelsecListPara[idx].bValue;
	//return m_PLCComm->GetBit(m_MelsecItem[idx].nAddress);
}

void CDialogInline::ClickBtnenhctrlInlineCancel()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ShowWindow(SW_HIDE);
}

void CDialogInline::OnBnClickedButtonOModeUnitOperationSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(GetBitOModeOperation())
	{
		m_MelsecListPara[DEVB_MODE_AUTO].bValue = TRUE;
		m_MelsecListPara[DEVB_MODE_MANUAL].bValue = FALSE;
	}
	else
	{
		m_MelsecListPara[DEVB_MODE_AUTO].bValue = FALSE;
		m_MelsecListPara[DEVB_MODE_MANUAL].bValue = TRUE;
	}
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_MODE_AUTO);
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_MODE_MANUAL);
}

void CDialogInline::OnBnClickedButtonEStatusRecipeNoSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_MelsecListPara[DEVW_RGD_HOST_RECIPE_NUMBER].wValue = GetBitEStatusRecipeNo();
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_RGD_HOST_RECIPE_NUMBER);
}

void CDialogInline::OnBnClickedButtonEStatusGlassCntSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_MelsecListPara[DEVW_USD_GLASS_COUNT_IN_UNIT].wValue = GetBitEStatusGlassCnt();
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_USD_GLASS_COUNT_IN_UNIT);
}

void CDialogInline::OnBnClickedButtonEStatusGlassPosiCntSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_MelsecListPara[DEVB_MODE_AUTO].wValue = GetBitEStatusGlassPosiCnt();
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_UIGD_HOST_RECIPE_NUMBER);
}

void CDialogInline::OnBnClickedButtonEStatusIonStatusSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_chk_e_status_ion_status)
	{
		m_MelsecListPara[DEVW_USD_IONIZER_STATUS_0_15].wValue = 0x02;
		m_MelsecListPara[DEVW_USD_IONIZER_STATUS_16_31].wValue = 0x00;
	}
	else
	{
		m_MelsecListPara[DEVW_USD_IONIZER_STATUS_0_15].wValue = 0x00;
		m_MelsecListPara[DEVW_USD_IONIZER_STATUS_16_31].wValue = 0x00;
	}
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_USD_IONIZER_STATUS_0_15);
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_USD_IONIZER_STATUS_16_31);
}

void CDialogInline::InitInterLockStatus()
{
	//un status
	//normal, enable, glass data report, unload request, complete confirm
	m_MelsecListPara[DEVB_UNLOAD_ENABLE].bValue = FALSE;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_UNLOAD_ENABLE);
	m_MelsecListPara[DEVB_UNLOAD_GLASS_DATA_REQUEST].bValue = FALSE;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_UNLOAD_GLASS_DATA_REQUEST);
	m_MelsecListPara[DEVB_UNLOAD_NORMAL_STATUS].bValue = FALSE;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_UNLOAD_NORMAL_STATUS);
	m_MelsecListPara[DEVB_UNLOAD_REQUEST].bValue = FALSE;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_UNLOAD_REQUEST);
	m_MelsecListPara[DEVB_UNLOAD_COMPLETE_CONFIRM].bValue = FALSE;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_UNLOAD_COMPLETE_CONFIRM);

	//ld status
	//normal, enable, glass data request, load request, complete confirm
	m_MelsecListPara[DEVB_LOAD_ENABLE].bValue = FALSE;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_LOAD_ENABLE);
	m_MelsecListPara[DEVB_LOAD_GLASS_DATA_REQUEST].bValue = FALSE;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_LOAD_GLASS_DATA_REQUEST);
	m_MelsecListPara[DEVB_LOAD_NORMAL_STATUS].bValue = FALSE;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_LOAD_NORMAL_STATUS);
	m_MelsecListPara[DEVB_LOAD_REQUEST].bValue = FALSE;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_LOAD_REQUEST);
	m_MelsecListPara[DEVB_LOAD_COMPLETE_CONFIRM].bValue = FALSE;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_LOAD_COMPLETE_CONFIRM);
}

void CDialogInline::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strLog;
	m_bLogWrite = FALSE;
	CString temp;
	SYSTEMTIME time;
	if(nIDEvent > MELSEC_BIT_ADDRESS_READ)
	{
	//	KillTimer(nIDEvent);
	}

	switch(nIDEvent)
	{
	case MELSEC_ALL_ADDRESS_READ:
		if(m_bWord1ValueRead)
			KillTimer(MELSEC_WORD_ADDRESS1_READ);

		if(m_bWord2ValueRead)
			KillTimer(MELSEC_WORD_ADDRESS2_READ);

		if(m_bBitValueRead)
			KillTimer(MELSEC_BIT_ADDRESS_READ);

		m_bAllValueRead = TRUE;
		m_bWord1ValueRead = FALSE;
		m_bWord2ValueRead = FALSE;
		m_bBitValueRead = FALSE;
		break;
	case MELSEC_WORD_ADDRESS1_READ:
		if(m_bAllValueRead)
			KillTimer(MELSEC_ALL_ADDRESS_READ);

		if(m_bWord2ValueRead)
			KillTimer(MELSEC_WORD_ADDRESS2_READ);

		if(m_bBitValueRead)
			KillTimer(MELSEC_BIT_ADDRESS_READ);

		m_bAllValueRead = FALSE;
		m_bWord1ValueRead = TRUE;
		m_bWord2ValueRead = FALSE;
		m_bBitValueRead = FALSE;
		break;
	case MELSEC_WORD_ADDRESS2_READ:
		if(m_bAllValueRead)
			KillTimer(MELSEC_ALL_ADDRESS_READ);

		if(m_bWord1ValueRead)
			KillTimer(MELSEC_WORD_ADDRESS1_READ);

		if(m_bBitValueRead)
			KillTimer(MELSEC_BIT_ADDRESS_READ);

		m_bAllValueRead = FALSE;
		m_bWord1ValueRead = FALSE;
		m_bWord2ValueRead = TRUE;
		m_bBitValueRead = FALSE;
		break;
	case MELSEC_BIT_ADDRESS_READ:
		if(m_bAllValueRead)
			KillTimer(MELSEC_ALL_ADDRESS_READ);

		if(m_bWord1ValueRead)
			KillTimer(MELSEC_WORD_ADDRESS1_READ);

		if(m_bWord2ValueRead)
			KillTimer(MELSEC_WORD_ADDRESS2_READ);

		m_bAllValueRead = FALSE;
		m_bWord1ValueRead = FALSE;
		m_bWord2ValueRead = FALSE;
		m_bBitValueRead = TRUE;
		break;
	case TIMER_INTERLOCK_LOAD_ONLY://Load only
		switch(m_iSeqIdx)
		{
		case 10:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//Normal status ON                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
				m_PLCComm->SetBit(PLC_DEVB_LOAD_NORMAL_STATUS, TRUE);
				SetLoaderBtn(PLC_DEVB_LOAD_NORMAL_STATUS, TRUE);
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_NORMAL_STATUS, TRUE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_NORMAL_STATUS, TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_LOAD_ONLY_010, PLC_DEVB_LOAD_NORMAL_STATUS & PLC_DEVB_UNLOAD_NORMAL_STATUS TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 20;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 20:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//glass data request ON
				m_PLCComm->SetBit(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, TRUE);
				SetLoaderBtn(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_LOAD_ONLY_020, PLC_DEVB_LOAD_GLASS_DATA_REQUEST TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 30;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 30:
			//glass data report ON
			m_nSeqDelay100mm++;
			if(m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_GLASS_DATA_REPORT))//Read)
			{
				//glass data read
				ReadLoaderGlassData();
				GlassDataUpdate(FALSE);
				m_EQPStatus.nHostRecipeNum = m_LoaderGlassData.nHostRecipeNumber;
				m_strStatusString.Format(_T("TIMER_INTERLOCK_LOAD_ONLY_030, Loader Glass data Load!"));
				WriteLog(m_strStatusString);
				m_nSeqDelay100mm = 0;

				//Glass Data Check
				if(m_LoaderGlassData.strLotID == NULL)
				{
					m_iSeqIdx = 0;
					m_iTimeout100mm = 0;
					KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
					InitSeqBit(LOAD_ONLY);
					//m_PLCComm->SetBit(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
					m_strErrorString.Format(_T("Error : lo : Loader Lot ID NULL fail!"));
					WriteLog(m_strErrorString);
					m_nInterLockError = INTERLOCK_ERROR_LOT_ID_NULL;//INTERLOCK_ERROR_RECIPE_CHECK;
					m_nInterLockErrorLevel = INTERLOCK_EROR_LIGHT_ALARM;
				}

				if(m_LoaderGlassData.strGlassID == NULL)
				{
					m_iSeqIdx = 0;
					m_iTimeout100mm = 0;
					KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
					InitSeqBit(LOAD_ONLY);
					//m_PLCComm->SetBit(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
					m_strErrorString.Format(_T("Error : lo : Loader Glass ID NULL fail!"));
					WriteLog(m_strErrorString);
					m_nInterLockError = INTERLOCK_ERROR_GLASS_ID_NULL;//INTERLOCK_ERROR_RECIPE_CHECK;
					m_nInterLockErrorLevel = INTERLOCK_EROR_LIGHT_ALARM;
				}

				if(RecipeNumCheck(m_LoaderGlassData.nHostRecipeNumber))
				{
					m_iSeqIdx = 40;
					m_iTimeout100mm = 0;
					SetBitEStatusRecipeNo(m_EQPStatus.nCurrenRecipeNum);
					SetHostRecipeRequest(m_EQPStatus.nHostRecipeNum, TRUE);
					m_strStatusString.Format(_T("TIMER_INTERLOCK_LOAD_ONLY_030, Loader Glass data Load Comp!"));
					WriteLog(m_strStatusString);
				}
				else
				{
					m_iSeqIdx = 0;
					m_iTimeout100mm = 0;
					KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
					InitSeqBit(LOAD_ONLY);
					//m_PLCComm->SetBit(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
					m_strErrorString.Format(_T("Error : lo : ecs host recipe number and eqp host recipe number fail!"));
					WriteLog(m_strErrorString);
					m_nInterLockError = INTERLOCK_ERROR_RECIPE_NUM_MISMATCH;//INTERLOCK_ERROR_RECIPE_CHECK;
					m_nInterLockErrorLevel = INTERLOCK_EROR_LIGHT_ALARM;
					//Down Alarm
					
					//Opeation mode
					//SetBitOModeUnitAuto(FALSE);
					//SetBitOModeOperation_AutoOrManual(FALSE);
				}				
			}
			break;
		case 40:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//load request ON
				m_PLCComm->SetBit(PLC_DEVB_LOAD_REQUEST, TRUE);
				SetLoaderBtn(PLC_DEVB_LOAD_REQUEST, TRUE);
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_REQUEST, FALSE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_REQUEST, FALSE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_LOAD_ONLY_040, PLC_DEVB_LOAD_REQUEST TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 50;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 50:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//load enable ON
				m_PLCComm->SetBit(PLC_DEVB_LOAD_ENABLE, TRUE);
				SetLoaderBtn(PLC_DEVB_LOAD_ENABLE, TRUE);
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_ENABLE, FALSE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_ENABLE, FALSE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_LOAD_ONLY_050, PLC_DEVB_LOAD_ENABLE TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 60;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 60:
			//load noninterference Off check
			m_nSeqDelay100mm++;
			if(!m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_NONINTERFERENCE))//Read)
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_LOAD_ONLY_060, ROBOT_B_ADDRESS_LOADER_NONINTERFERENCE off check!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 70;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
				break;
			}

			m_iTimeout100mm++;
			if(m_nTrigger[TRIGGER_SEQUENCE_DELAY] < m_iTimeout100mm)
			{
				//Error
				KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
				InitSeqBit(LOAD_ONLY);

				m_strErrorString.Format(_T("Error : lo : load noninterference Off check timeout!(case 60)"));	
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_LOADER_NONINTERFERENCET_OFF_TIMEOUT;//INTERLOCK_ERROR_LOADER_NONINTERFERENCET_OFF_TIMEOUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Down Alarm
				
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			break;
		case 70:
			//robot busy On check
			m_nSeqDelay100mm++;
			if(m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_ROBOT_BUSY))//Read)
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_LOAD_ONLY_070, ROBOT_B_ADDRESS_LOADER_ROBOT_BUSY on check!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 80;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_ROBOTBUSY] < m_iTimeout100mm)
			{
				////Error
				KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
				InitSeqBit(LOAD_ONLY);
				m_strErrorString.Format(_T("Error : lo : robot busy On check timeout!(case 70)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_LOADER_ROBOT_BUSY_ON_TIMEOUT;//INTERLOCK_ERROR_LOADER_ROBOT_BUSY_ON_TIME_OVEROUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			//SetTimer(TIMER_INTERLOCK_LOAD_ONLY, 100,NULL);
			break;
		case 80:
			//load complete ON check
			m_nSeqDelay100mm++;
			if(m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_COMPLETE))
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_LOAD_ONLY_080, ROBOT_B_ADDRESS_LOADER_COMPLETE on check!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 90;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;

				GetLocalTime(&time);
				m_APDData.GlassStartTime.wYear = time.wYear;
				m_APDData.GlassStartTime.wMonth = time.wMonth;
				m_APDData.GlassStartTime.wDay = time.wDay;
				m_APDData.GlassStartTime.wHour = time.wHour;
				m_APDData.GlassStartTime.wMinute = time.wMinute;
				m_APDData.GlassStartTime.wSecond = time.wSecond;

				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_LOAD_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
				InitSeqBit(LOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : lo : EQPLoaderNormalStatus off!(case 80)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_LOADER_NORMAL_STATUS_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
				InitSeqBit(LOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : lo : RobotLoaderNormalStatus off!(case 80)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_ROBOT_LOADER_NORMAL_STATUS_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_LOAD_ENABLE))
			{
				KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
				InitSeqBit(LOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : lo : EQPLoaderEnable off!(case 80)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_LOADER_ENABLE_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] < m_iTimeout100mm)
			{
				//Error
				KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
				InitSeqBit(LOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				m_strErrorString.Format(_T("Error : lo : load complete ON check timeout!(case 80)"));
				WriteLog(m_strErrorString);	
				m_nInterLockError = INTERLOCK_ERROR_LOADER_ROBOT_COMPLETE_ON_TIMEOUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			//SetTimer(TIMER_INTERLOCK_LOAD_ONLY, 100,NULL);
			break;
		case 90:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//load complete confirm ON
				m_PLCComm->SetBit(PLC_DEVB_LOAD_COMPLETE_CONFIRM, TRUE);
				SetLoaderBtn(PLC_DEVB_LOAD_COMPLETE_CONFIRM, TRUE);
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_COMPLETE_CONFIRM, FALSE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_COMPLETE_CONFIRM, FALSE);
				//SetBitEStatusGlassExist(TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_LOAD_ONLY_090, PLC_DEVB_LOAD_COMPLETE_CONFIRM TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 100;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 100:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//load request Off, 
				m_PLCComm->SetBit(PLC_DEVB_LOAD_REQUEST, FALSE);
				SetLoaderBtn(PLC_DEVB_LOAD_REQUEST, FALSE);
				//glass data request off
				m_PLCComm->SetBit(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
				SetLoaderBtn(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_LOAD_ONLY_100, PLC_DEVB_LOAD_REQUEST TRUE Set & PLC_DEVB_LOAD_GLASS_DATA_REQUEST FALSE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 110;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 110:
			//load complete Off chek
			m_nSeqDelay100mm++;
			if(!m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_COMPLETE))//Read)
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_LOAD_ONLY_110, ROBOT_B_ADDRESS_LOADER_COMPLETE off check!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 120;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_LOAD_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
				InitSeqBit(LOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : lo : EQPLoaderNormalStatus off!(case 110)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_LOADER_NORMAL_STATUS_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
				InitSeqBit(LOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : lo : RobotLoaderNormalStatus off!(case 110)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_ROBOT_LOADER_NORMAL_STATUS_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_LOAD_ENABLE))
			{
				KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
				InitSeqBit(LOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : lo : EQPLoaderEnable off!(case 110)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_LOADER_ENABLE_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] < m_iTimeout100mm)
			{
				//Error
				KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
				InitSeqBit(LOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				m_strErrorString.Format(_T("Error : lo : load complete Off chek timeout!(case 110)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_LOADER_ROBOT_COMPLETE_OFF_TIMEOUT;//INTERLOCK_ERROR_TIMEOUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			break;
		case 120:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//load complete confirm Off
				m_PLCComm->SetBit(PLC_DEVB_LOAD_COMPLETE_CONFIRM, FALSE);
				SetLoaderBtn(PLC_DEVB_LOAD_COMPLETE_CONFIRM, FALSE);
				SetBitEStatusGlassExist(TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_LOAD_ONLY_120, PLC_DEVB_LOAD_COMPLETE_CONFIRM TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 130;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 130:
			//robot busy Off check //load noninterference On
			m_nSeqDelay100mm++;
			if(!m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_ROBOT_BUSY) && m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_NONINTERFERENCE))//Read)
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_LOAD_ONLY_130, ROBOT_B_ADDRESS_LOADER_ROBOT_BUSY off, ROBOT_B_ADDRESS_LOADER_NONINTERFERENCE on check!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 140;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_LOAD_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
				InitSeqBit(LOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : lo : EQPLoaderNormalStatus off!(case 130)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_LOADER_NORMAL_STATUS_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
				InitSeqBit(LOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : lo : RobotLoaderNormalStatus off!(case 130)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_ROBOT_LOADER_NORMAL_STATUS_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_LOAD_ENABLE))
			{
				KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
				InitSeqBit(LOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : lo : EQPLoaderEnable off!(case 130)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_LOADER_ENABLE_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_ROBOTBUSY] < m_iTimeout100mm)
			{
				//Error
				KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
				InitSeqBit(LOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				m_strErrorString.Format(_T("Error : lo: robot busy Off check & load noninterference On timeout!(case 130)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_LOADER_NONINTERFERENCET_ON_TIMEOUT;//INTERLOCK_ERROR_LOADER_NONINTERFERENCET_ON_TIMEOUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			break;
		case 140:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//load enable Off
				m_PLCComm->SetBit(PLC_DEVB_LOAD_ENABLE, FALSE);
				SetLoaderBtn(PLC_DEVB_LOAD_ENABLE, FALSE);
				// Glass In Processing & GlassExist In Unit On
				m_PLCComm->SetBit(PLC_DEVB_GLASS_IN_PROCESSING, TRUE);
				m_PLCComm->SetBit(PLC_DEVB_GLASS_EXIST_IN_UNIT, TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_LOAD_ONLY_140, PLC_DEVB_UNLOAD_ENABLE FALSE Set & PLC_DEVB_GLASS_IN_PROCESSING TRUE Set & PLC_DEVB_GLASS_EXIST_IN_UNIT TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 150;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 150:
			KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
			InitSeqBit(LOAD_ONLY);
			pdlg->SetLoadingComplete(TRUE);
			m_UnloaderGlassData = m_LoaderGlassData;
			SYSTEMTIME	SystemTime;  //20160725 sc
			GetLocalTime(&SystemTime); //20160725 sc
			m_LotInTime.strInTime.Format(_T("%04d%02d%02d%02d%02d%02d"), SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);//20160725 sc
			m_strStatusString.Format(_T("TIMER_INTERLOCK_LOAD_ONLY_150, Load complete ok"));
			WriteLog(m_strStatusString);
			break;
		}
		break;
	case TIMER_INTERLOCK_UNLOAD_ONLY://Unload only
		switch(m_iSeqIdx)
		{
		case 10:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//Glass Data write                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
				m_PLCComm->SetBit(PLC_DEVB_LOAD_NORMAL_STATUS, FALSE);
				SetLoaderBtn(PLC_DEVB_LOAD_NORMAL_STATUS, FALSE);
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_NORMAL_STATUS, TRUE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_NORMAL_STATUS, TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_UNLOAD_ONLY_010, PLC_DEVB_LOAD_NORMAL_STATUS TRUE Set & PLC_DEVB_UNLOAD_NORMAL_STATUS TRUE Set!"));
				WriteLog(m_strStatusString);
				WriteUnLoaderGlassData();
				m_bAPDDataReport = TRUE;
				m_iSeqIdx = 20;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 20:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//glass data report ON
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, TRUE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_UNLOAD_ONLY_020, PLC_DEVB_UNLOAD_GLASS_DATA_REPORT TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 30;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 30:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//unload request ON
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_NORMAL_STATUS, TRUE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_NORMAL_STATUS, TRUE);
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, TRUE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, TRUE);
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_REQUEST, TRUE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_REQUEST, TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_UNLOAD_ONLY_030, PLC_DEVB_UNLOAD_REQUEST TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 40;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 40:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//glass data request ON
				m_PLCComm->SetBit(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
				SetLoaderBtn(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_UNLOAD_ONLY_040, PLC_DEVB_LOAD_GLASS_DATA_REQUEST FALSE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 50;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 50:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//unload enable ON
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_ENABLE, TRUE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_ENABLE, TRUE);
				m_PLCComm->SetBit(PLC_DEVB_LOAD_ENABLE, FALSE);
				SetLoaderBtn(PLC_DEVB_LOAD_ENABLE, FALSE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_UNLOAD_ONLY_050, PLC_DEVB_UNLOAD_ENABLE TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 60;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 60:
			//unload noninterference off check
			m_nSeqDelay100mm++;
			if(!m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_NONINTERFERENCE))//Read)
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_UNLOAD_ONLY_060, ROBOT_B_ADDRESS_UNLOADER_NONINTERFERENCE off check!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 70;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_SEQUENCE] < m_iTimeout100mm)
			{
				//Error
				KillTimer(TIMER_INTERLOCK_UNLOAD_ONLY);
				InitSeqBit(UNLOAD_ONLY);
				m_strErrorString.Format(_T("Error : un : unload noninterference off check timeout!(case 60)"));
				WriteLog(m_strErrorString);		
				m_nInterLockError = INTERLOCK_ERROR_UNLOADER_NONINTERFERENCET_OFF_TIMEOUT;//INTERLOCK_ERROR_UNLOADER_NONINTERFERENCET_OFF_TIMEOUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			break;
		case 70:
			//robot busy check ON
			m_nSeqDelay100mm++;
			if(m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_ROBOT_BUSY))//Read)
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_UNLOAD_ONLY_070, ROBOT_B_ADDRESS_UNLOADER_ROBOT_BUSY on check!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 80;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_ROBOTBUSY] < m_iTimeout100mm)
			{
				KillTimer(TIMER_INTERLOCK_UNLOAD_ONLY);
				InitSeqBit(UNLOAD_ONLY);
				//Error
				m_strErrorString.Format(_T("Error : un : robot busy check ON timeout!(case 70)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_UNLOADER_ROBOT_BUSY_ON_TIMEOUT;//INTERLOCK_ERROR_UNLOADER_ROBOT_BUSY_ON_TIMEOUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			break;
		case 80:
			//unload complete check
			m_nSeqDelay100mm++;
			if(m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_COMPLETE))// && m_bEQPUnloaderNormalStatus && m_bRobotUnloaderNormalStatus && m_bEQPUnloaderEnable)//Read)
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_UNLOAD_ONLY_080, ROBOT_B_ADDRESS_UNLOADER_COMPLETE on check!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 90;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
				
				GetLocalTime(&time);
				m_APDData.GlassEndTime.wYear = time.wYear;
				m_APDData.GlassEndTime.wMonth = time.wMonth;
				m_APDData.GlassEndTime.wDay = time.wDay;
				m_APDData.GlassEndTime.wHour = time.wHour;
				m_APDData.GlassEndTime.wMinute = time.wMinute;
				m_APDData.GlassEndTime.wSecond = time.wSecond;
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_UNLOAD_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_UNLOAD_ONLY);
				InitSeqBit(UNLOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : un : EQPUnloaderNormalStatus off!(case 80)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_UNLOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_EQP_UNLOADER_NORMAL_STATUS_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_UNLOAD_ONLY);
				InitSeqBit(UNLOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : un : RobotUnloaderNormalStatus off!(case 80)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_ROBOT_UNLOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_ROBOT_UNLOADERNORMAL_STATUS_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_UNLOAD_ENABLE))
			{
				KillTimer(TIMER_INTERLOCK_UNLOAD_ONLY);
				InitSeqBit(UNLOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : un : EQPUnloaderEnable off!(case 80)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_UNLOADER_ENABLE_OFF;//INTERLOCK_ERROR_EQP_UNLOADER_ENABLE_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] < m_iTimeout100mm)
			{
				KillTimer(TIMER_INTERLOCK_UNLOAD_ONLY);
				InitSeqBit(UNLOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : un : unload complete check timeout!(case 80)"));
				WriteLog(m_strErrorString);		
				m_nInterLockError = INTERLOCK_ERROR_UNLOADER_ROBOT_COMPLETE_ON_TIMEOUT;//INTERLOCK_ERROR_EQP_UNLOADER_COMPLETE_TIMEOUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			break;
		case 90:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;

			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//unload complete confirm on
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_COMPLETE_CONFIRM, TRUE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_COMPLETE_CONFIRM, TRUE);
				SetTimer(TIMER_APD_DATA_REPORT, 100, NULL);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_UNLOAD_ONLY_090, PLC_DEVB_UNLOAD_COMPLETE_CONFIRM TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 100;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 100:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//unload request off
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_REQUEST, FALSE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_REQUEST, FALSE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_UNLOAD_ONLY_100, PLC_DEVB_UNLOAD_REQUEST FALSE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 110;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			};
			break;
		case 110:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//glass data report Off
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, FALSE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, FALSE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_UNLOAD_ONLY_110, PLC_DEVB_UNLOAD_GLASS_DATA_REPORT FALSE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 120;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 120:
			//unload complete Off check 
			m_nSeqDelay100mm++;
			if(!m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_COMPLETE))//Read)
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_UNLOAD_ONLY_120, ROBOT_B_ADDRESS_UNLOADER_COMPLETE off check!"));
				WriteLog(m_strStatusString);
				SetBitEStatusGlassExist(FALSE);
				m_iSeqIdx = 130;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_UNLOAD_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_UNLOAD_ONLY);
				InitSeqBit(UNLOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : un : EQPUnloaderNormalStatus off!(case 120)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_UNLOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_EQP_UNLOADER_NORMAL_STATUS_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_UNLOAD_ONLY);
				InitSeqBit(UNLOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : un : RobotUnloaderNormalStatus off!(case 120)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_ROBOT_UNLOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_ROBOT_UNLOADERNORMAL_STATUS_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_UNLOAD_ENABLE))
			{
				KillTimer(TIMER_INTERLOCK_UNLOAD_ONLY);
				InitSeqBit(UNLOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : un : EQPUnloaderEnable off!(case 120)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_UNLOADER_ENABLE_OFF;//INTERLOCK_ERROR_EQP_UNLOADER_ENABLE_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] < m_iTimeout100mm)
			{
				KillTimer(TIMER_INTERLOCK_UNLOAD_ONLY);
				InitSeqBit(UNLOAD_ONLY);
				//Error
				m_strErrorString.Format(_T("Error : un : unload complete Off check timeout!(case 120)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_UNLOADER_ROBOT_COMPLETE_OFF_TIMEOUT;//INTERLOCK_ERROR_EQP_UNLOADER_COMPLETE_TIMEOUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			break;
		case 130:
			//unload complete confirm Off
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//load enable Off
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_COMPLETE_CONFIRM, FALSE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_COMPLETE_CONFIRM, FALSE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_UNLOAD_ONLY_130, PLC_DEVB_UNLOAD_COMPLETE_CONFIRM FALSE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 140;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 140:
			//robot busy Off check, unload noninterference check 
			m_nSeqDelay100mm++;
			if(!m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_ROBOT_BUSY) && m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_NONINTERFERENCE))//Read)
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_UNLOAD_ONLY_140, ROBOT_B_ADDRESS_UNLOADER_ROBOT_BUSY off, ROBOT_B_ADDRESS_UNLOADER_NONINTERFERENCE on check!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 150;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_UNLOAD_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_UNLOAD_ONLY);
				InitSeqBit(UNLOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : un : EQPUnloaderNormalStatus off!(case 140)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_UNLOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_EQP_UNLOADER_NORMAL_STATUS_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_UNLOAD_ONLY);
				InitSeqBit(UNLOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : un : RobotUnloaderNormalStatus off!(case 140)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_ROBOT_UNLOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_ROBOT_UNLOADERNORMAL_STATUS_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_UNLOAD_ENABLE))
			{
				KillTimer(TIMER_INTERLOCK_UNLOAD_ONLY);
				InitSeqBit(UNLOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : un : EQPUnloaderEnable off!(case 140)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_UNLOADER_ENABLE_OFF;//INTERLOCK_ERROR_EQP_UNLOADER_ENABLE_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_ROBOTBUSY] < m_iTimeout100mm)
			{
				//Error
				KillTimer(TIMER_INTERLOCK_UNLOAD_ONLY);
				InitSeqBit(UNLOAD_ONLY);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				m_strErrorString.Format(_T("Error : un : robot busy Off check, unload noninterference check timeout!(case 140)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_UNLOADER_NONINTERFERENCET_ON_TIMEOUT;//INTERLOCK_ERROR_TIMEOUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			break;
		case 150:
			//unload enable Off
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//load enable Off
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_ENABLE, FALSE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_ENABLE, FALSE);
				// Glass In Processing & GlassExist In Unit On
				memset(&m_LoaderGlassData, NULL, sizeof(typeGlassDataPara));
				memset(&m_UnloaderGlassData, NULL, sizeof(typeGlassDataPara));
				memset(&m_UpdateGlassData, NULL, sizeof(typeGlassDataPara));
				//SetUpdateGlassData(m_LoaderGlassData);
				GlassDataUpdate(FALSE);
				SaveTemp();
				SetBitEStatusGlassInPrcs(FALSE);
				SetBitEStatusGlassExist(FALSE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_UNLOAD_ONLY_150, PLC_DEVB_UNLOAD_ENABLE FALSE Set & PLC_DEVB_GLASS_IN_PROCESSING FALSE Set & PLC_DEVB_GLASS_EXIST_IN_UNIT FALSE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 160;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 160:
			KillTimer(TIMER_INTERLOCK_UNLOAD_ONLY);
			InitSeqBit(UNLOAD_ONLY);
			m_bLoadingComplete = TRUE;
			m_strStatusString.Format(_T("TIMER_INTERLOCK_UNLOAD_ONLY_160, UnLoad complete ok"));
			pdlg->SetCassetEmpty(TRUE);
			pdlg->SetLoadingComplete(TRUE);
			WriteLog(m_strStatusString);
			break;
		}
		break;
	case TIMER_INTERLOCK_EXCHANGE://Exchange
		switch(m_iSeqIdx)
		{
		case 10: //Loder, Unload Normal Status On
			if(0 == m_iTimeout100mm)
			{
				//Normal status ON                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
				m_PLCComm->SetBit(PLC_DEVB_LOAD_NORMAL_STATUS, TRUE);
				SetLoaderBtn(PLC_DEVB_LOAD_NORMAL_STATUS, TRUE);
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_NORMAL_STATUS, TRUE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_NORMAL_STATUS, TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_010, PLC_DEVB_LOAD_NORMAL_STATUS TRUE Set & PLC_DEVB_UNLOAD_NORMAL_STATUS TRUE Set!"));
				WriteLog(m_strStatusString);
				m_bLoadingComplete = FALSE;
				m_iSeqIdx = 20;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 20://Unload Glass Data Write
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//unload Glass Data write 
				WriteUnLoaderGlassData();
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_020, WriteUnLoaderGlassData Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 30;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 30://Unload Glass Data Report on
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//glass data report ON
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, TRUE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_030, PLC_DEVB_UNLOAD_GLASS_DATA_REPORT TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 40;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 40://Unloader request on
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//unload request ON
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_REQUEST, TRUE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_REQUEST, TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_040, PLC_DEVB_UNLOAD_REQUEST TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 50;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 50://Glass Data Request On
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//glass data request ON
				m_PLCComm->SetBit(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, TRUE);
				SetLoaderBtn(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_050, PLC_DEVB_LOAD_GLASS_DATA_REQUEST TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 60;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 60://Loader Glass Data Report On Check
			m_nSeqDelay100mm++;
			if(m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_GLASS_DATA_REPORT))//Read)
			{
				//glass data read
				ReadLoaderGlassData();
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_060, Loader Glass data Load!"));
				WriteLog(m_strStatusString);
				m_nSeqDelay100mm = 0;

				//Glass Data Check
				if(m_LoaderGlassData.strLotID == NULL)
				{
					m_iSeqIdx = 0;
					m_iTimeout100mm = 0;
					KillTimer(TIMER_INTERLOCK_EXCHANGE);
					InitSeqBit(EXCHANGE);
					//m_PLCComm->SetBit(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
					m_strErrorString.Format(_T("Error : ex : Loader Lot ID NULL fail!"));
					WriteLog(m_strErrorString);
					m_nInterLockError = INTERLOCK_ERROR_LOT_ID_NULL;//INTERLOCK_ERROR_RECIPE_CHECK;
					m_nInterLockErrorLevel = INTERLOCK_EROR_LIGHT_ALARM;
				}

				if(m_LoaderGlassData.strGlassID == NULL)
				{
					m_iSeqIdx = 0;
					m_iTimeout100mm = 0;
					KillTimer(TIMER_INTERLOCK_EXCHANGE);
					InitSeqBit(EXCHANGE);
					//m_PLCComm->SetBit(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
					m_strErrorString.Format(_T("Error : ex : Loader Glass ID NULL fail!"));
					WriteLog(m_strErrorString);
					m_nInterLockError = INTERLOCK_ERROR_GLASS_ID_NULL;//INTERLOCK_ERROR_RECIPE_CHECK;
					m_nInterLockErrorLevel = INTERLOCK_EROR_LIGHT_ALARM;
				}

				if(RecipeNumCheck(m_LoaderGlassData.nHostRecipeNumber))
				{
					m_iSeqIdx = 70;
					m_iTimeout100mm = 0;
					SetBitEStatusRecipeNo(m_EQPStatus.nCurrenRecipeNum);
					m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_060, Loader Glass data Load Comp!"));
					WriteLog(m_strStatusString);
					break;
				}
				else
				{
					m_iSeqIdx = 0;
					m_iTimeout100mm = 0;
					KillTimer(TIMER_INTERLOCK_EXCHANGE);
					InitSeqBit(EXCHANGE);
					m_strErrorString.Format(_T("Error : ex : ecs host recipe number and eqp current recipe number compare fail!(case 60)"));
					WriteLog(m_strErrorString);
					m_nInterLockError = INTERLOCK_ERROR_RECIPE_NUM_MISMATCH;
					m_nInterLockErrorLevel = INTERLOCK_EROR_LIGHT_ALARM;
					
					//Opeation mode
					//SetBitOModeUnitAuto(FALSE);
					//SetBitOModeOperation_AutoOrManual(FALSE);
				}		
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_GLASS_DATA_REQUEST] < m_iTimeout100mm && !m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_GLASS_DATA_REPORT))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);
				m_PLCComm->SetBit(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
				SetLoaderBtn(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_NORMAL_STATUS, TRUE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_NORMAL_STATUS, TRUE);
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, TRUE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, TRUE);
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_REQUEST, TRUE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_REQUEST, TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_060, ROBOT_B_ADDRESS_LOADER_GLASS_DATA_REPORT off (Timeout %dms) check!"), m_nTimeout[TIMEOUT_GLASS_DATA_REQUEST]);
				WriteLog(m_strStatusString);
				m_iSeqIdx = 30;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_060 -> TIMER_INTERLOCK_UNLOADER_030 InterLock Type Change!"));
				WriteLog(m_strStatusString);
				SetTimer(TIMER_INTERLOCK_UNLOAD_ONLY, 100, NULL);
			}
			break;
		case 70:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//load request on
				m_PLCComm->SetBit(PLC_DEVB_LOAD_REQUEST, TRUE);
				SetLoaderBtn(PLC_DEVB_LOAD_REQUEST, TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_070, PLC_DEVB_LOAD_REQUEST TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 80;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 80:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//unload enable on
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_ENABLE, TRUE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_ENABLE, TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_080, PLC_DEVB_UNLOAD_ENABLE TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 90;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 90:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//load enable on,
				m_PLCComm->SetBit(PLC_DEVB_LOAD_ENABLE, TRUE);
				SetLoaderBtn(PLC_DEVB_LOAD_ENABLE, TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_090, PLC_DEVB_LOAD_ENABLE TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 100;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 100:
			m_nSeqDelay100mm++;
			//unload robot busy on check & unload noninterference off
			if(m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_ROBOT_BUSY) && !m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_NONINTERFERENCE))//Read)
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_100, ROBOT_B_ADDRESS_UNLOADER_ROBOT_BUSY on & ROBOT_B_ADDRESS_UNLOADER_NONINTERFERENCE off check!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 110;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_ROBOTBUSY] < m_iTimeout100mm)
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				////Error
				InitSeqBit(EXCHANGE);
				m_strErrorString.Format(_T("Error : ex : unload noninterference off check timeout!(case 100)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_UNLOADER_NONINTERFERENCET_OFF_TIMEOUT;//INTERLOCK_ERROR_TIMEOUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			break;
		case 110:
			//load robot busy on check & load noninterference off
			m_nSeqDelay100mm++;
			if(m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_ROBOT_BUSY) &&!m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_NONINTERFERENCE))//Read)
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_110, ROBOT_B_ADDRESS_LOADER_ROBOT_BUSY on & ROBOT_B_ADDRESS_LOADER_NONINTERFERENCE off check!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 120;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_ROBOTBUSY] < m_iTimeout100mm)
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);
				//Error
				m_strErrorString.Format(_T("Error : ex : load noninterference off check timeout!(case 110)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_LOADER_NONINTERFERENCET_OFF_TIMEOUT;//INTERLOCK_ERROR_TIMEOUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			break;
		case 120:
			//unload complete on check
			m_nSeqDelay100mm++;
			if(m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_COMPLETE))
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_120, ROBOT_B_ADDRESS_UNLOADER_COMPLETE on check!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 130;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
				
				GetLocalTime(&time);
				m_APDData.GlassEndTime.wYear = time.wYear;
				m_APDData.GlassEndTime.wMonth = time.wMonth;
				m_APDData.GlassEndTime.wDay = time.wDay;
				m_APDData.GlassEndTime.wHour = time.wHour;
				m_APDData.GlassEndTime.wMinute = time.wMinute;
				m_APDData.GlassEndTime.wSecond = time.wSecond;
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_UNLOAD_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : ex : EQPUnloaderNormalStatus off!(case 120)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_UNLOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_EQP_UNLOADE_NORMAL_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;

				SetStatus(eqp_stat);
				//Error
				m_strErrorString.Format(_T("Error : ex : RobotUnloaderNormalStatus off!(case 120)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_ROBOT_UNLOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_ROBOT_UNLOADE_NORMAL_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] < m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_UNLOAD_ENABLE))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : ex : EQPUnloaderEnableStatus off!(case 120)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_UNLOADER_ENABLE_OFF;//INTERLOCK_ERROR_EQP_UNLOADE_ENABLE_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] < m_iTimeout100mm)
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;

				SetStatus(eqp_stat);
				//Error
				m_strErrorString.Format(_T("Error : ex : unload complete on check timeout!(case 120)"));
				WriteLog(m_strErrorString);	
				m_nInterLockError = INTERLOCK_ERROR_UNLOADER_ROBOT_COMPLETE_ON_TIMEOUT;//INTERLOCK_ERROR_TIMEOUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			break;
		case 130:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//unload complete confirm on
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_COMPLETE_CONFIRM, TRUE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_COMPLETE_CONFIRM, TRUE);
				SetTimer(TIMER_APD_DATA_REPORT, 100, NULL);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_130, PLC_DEVB_UNLOAD_COMPLETE_CONFIRM TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 140;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 140:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//unload data report off
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, FALSE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, FALSE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_140, PLC_DEVB_UNLOAD_GLASS_DATA_REPORT FALSE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 150;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 150:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//unload rquest off
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_REQUEST, FALSE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_REQUEST, FALSE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_150, PLC_DEVB_UNLOAD_REQUEST FALSE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 160;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 160:
			//unload complete off check
			m_nSeqDelay100mm++;
			if(!m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_COMPLETE))//Read)
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_160, ROBOT_B_ADDRESS_UNLOADER_COMPLETE off check!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 170;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_UNLOAD_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : ex : EQPUnloaderNormalStatus off!(case 160)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_UNLOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_EQP_UNLOADE_NORMAL_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;

				SetStatus(eqp_stat);
				//Error
				m_strErrorString.Format(_T("Error : ex : RobotUnloaderNormalStatus off!(case 160)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_ROBOT_UNLOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_ROBOT_UNLOADE_NORMAL_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] < m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_UNLOAD_ENABLE))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : ex : EQPUnloaderEnableStatus off!(case 160)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_UNLOADER_ENABLE_OFF;//INTERLOCK_ERROR_EQP_UNLOADE_ENABLE_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] < m_iTimeout100mm)
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : ex : unload complete off check timeout!(case 160)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_UNLOADER_ROBOT_COMPLETE_OFF_TIMEOUT;//INTERLOCK_ERROR_TIMEOUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			break;
		case 170:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//unload complete confirm off
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_COMPLETE_CONFIRM, FALSE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_COMPLETE_CONFIRM, FALSE);
				SetBitEStatusGlassExist(FALSE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_170, PLC_DEVB_UNLOAD_COMPLETE_CONFIRM FALSE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 180;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 180:
			//load complete on check
			m_nSeqDelay100mm++;
			if(m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_COMPLETE))// && m_bEQPLoaderNormalStatus && m_bRobotLoaderNormalStatus)//Read)
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_180, ROBOT_B_ADDRESS_LOADER_COMPLETE on check!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 190;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;

				GetLocalTime(&time);
				m_APDData.GlassStartTime.wYear = time.wYear;
				m_APDData.GlassStartTime.wMonth = time.wMonth;
				m_APDData.GlassStartTime.wDay = time.wDay;
				m_APDData.GlassStartTime.wHour = time.wHour;
				m_APDData.GlassStartTime.wMinute = time.wMinute;
				m_APDData.GlassStartTime.wSecond = time.wSecond;
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_LOAD_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : ex : EQPLoaderNormalStatus timeout!(case 180)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_LOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_EQP_LOADE_NORMAL_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : ex : RobotLoaderNormalStatus timeout!(case 180)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_ROBOT_LOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_ROBOT_LOADE_NORMAL_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_LOAD_ENABLE))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : ex : RobotLoaderEnableStatus timeout!(case 180)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_LOADER_ENABLE_OFF;//INTERLOCK_ERROR_EQP_LOADE_ENABLE_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] < m_iTimeout100mm)
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;

				SetStatus(eqp_stat);
				//Error
				m_strErrorString.Format(_T("Error : ex : load complete on check timeout!(case 180)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_LOADER_ROBOT_COMPLETE_ON_TIMEOUT;//INTERLOCK_ERROR_TIMEOUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			break;
		case 190:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//load complete confirm on
				m_PLCComm->SetBit(PLC_DEVB_LOAD_COMPLETE_CONFIRM, TRUE);
				SetLoaderBtn(PLC_DEVB_LOAD_COMPLETE_CONFIRM, TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_190, PLC_DEVB_LOAD_COMPLETE_CONFIRM TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 200;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 200:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//load glass data request off
				m_PLCComm->SetBit(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
				SetLoaderBtn(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_200, PLC_DEVB_LOAD_GLASS_DATA_REQUEST FALSE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 210;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 210:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//load rquest off
				m_PLCComm->SetBit(PLC_DEVB_LOAD_REQUEST, FALSE);
				SetLoaderBtn(PLC_DEVB_LOAD_REQUEST, FALSE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_210, PLC_DEVB_LOAD_REQUEST FALSE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 220;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 220:
			//load complete off check
			m_nSeqDelay100mm++;
			if(!m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_COMPLETE))//Read)
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_220, ROBOT_B_ADDRESS_LOADER_COMPLETE off check!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 230;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_LOAD_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : ex : EQPLoaderNormalStatus timeout!(case 220)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_LOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_EQP_LOADE_NORMAL_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : ex : RobotLoaderNormalStatus timeout!(case 220)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_ROBOT_LOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_ROBOT_LOADE_NORMAL_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_LOAD_ENABLE))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : ex : RobotLoaderEnableStatus timeout!(case 220)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_LOADER_ENABLE_OFF;//INTERLOCK_ERROR_EQP_LOADE_ENABLE_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_SEQUENCE] < m_iTimeout100mm)
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : ex : load complete off check timeout!(case 220)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_LOADER_ROBOT_COMPLETE_OFF_TIMEOUT;//INTERLOCK_ERROR_TIMEOUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			break;
		case 230:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//load complete confirm off
				m_PLCComm->SetBit(PLC_DEVB_LOAD_COMPLETE_CONFIRM, FALSE);
				SetLoaderBtn(PLC_DEVB_LOAD_COMPLETE_CONFIRM, FALSE);
				SetBitEStatusGlassExist(TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_230, PLC_DEVB_LOAD_COMPLETE_CONFIRM FALSE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 240;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 240:
			//robot busy Off check, unload noninterference on check 
			m_nSeqDelay100mm++;
			if(!m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_ROBOT_BUSY) && m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_NONINTERFERENCE))//Read)
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_240, ROBOT_B_ADDRESS_UNLOADER_ROBOT_BUSY off, ROBOT_B_ADDRESS_UNLOADER_NONINTERFERENCE on check!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 250;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_UNLOAD_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : ex : EQPUnloaderNormalStatus off!(case 240)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_UNLOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_EQP_UNLOADE_NORMAL_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(ROBOT_B_ADDRESS_UNLOADER_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;

				SetStatus(eqp_stat);
				//Error
				m_strErrorString.Format(_T("Error : ex : RobotUnloaderNormalStatus off!(case 240)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_ROBOT_UNLOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_ROBOT_UNLOADE_NORMAL_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] < m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_UNLOAD_ENABLE))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = FALSE;
				eqp_stat.bGlassPosition = FALSE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : ex : EQPUnloaderEnableStatus off!(case 240)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_UNLOADER_ENABLE_OFF;//INTERLOCK_ERROR_EQP_UNLOADE_ENABLE_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_ROBOTBUSY] < m_iTimeout100mm)
			{
				//Error
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				m_strErrorString.Format(_T("Error : ex : robot busy Off check, unload noninterference check timeout!(case 240)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_UNLOADER_NONINTERFERENCET_ON_TIMEOUT;//INTERLOCK_ERROR_TIMEOUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			break;
		case 250://unload enable Off
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//unload enable Off
				m_PLCComm->SetBit(PLC_DEVB_UNLOAD_ENABLE, FALSE);
				SetUnLoaderBtn(PLC_DEVB_UNLOAD_ENABLE, FALSE);
				// Glass In Processing & GlassExist In Unit On
				m_PLCComm->SetBit(PLC_DEVB_GLASS_IN_PROCESSING, FALSE);
				m_PLCComm->SetBit(PLC_DEVB_GLASS_EXIST_IN_UNIT, FALSE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_250, PLC_DEVB_UNLOAD_ENABLE FALSE Set & PLC_DEVB_GLASS_IN_PROCESSING FALSE Set & PLC_DEVB_GLASS_EXIST_IN_UNIT FALSE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 260;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 260:
			//robot busy Off check //load noninterference On
			m_nSeqDelay100mm++;
			if(!m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_ROBOT_BUSY) && m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_NONINTERFERENCE))//Read)
			{
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_260, ROBOT_B_ADDRESS_LOADER_ROBOT_BUSY off, ROBOT_B_ADDRESS_LOADER_NONINTERFERENCE on check!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 270;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_LOAD_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : ex : EQPLoaderNormalStatus timeout!(case 260)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_LOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_EQP_LOADE_NORMAL_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(ROBOT_B_ADDRESS_LOADER_NORMAL_STATUS))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : ex : RobotLoaderNormalStatus timeout!(case 260)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_ROBOT_LOADER_NORMAL_STATUS_OFF;//INTERLOCK_ERROR_ROBOT_LOADE_NORMAL_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			if(m_nTimeout[TIMEOUT_SEQUENCE] > m_iTimeout100mm && !m_PLCComm->GetBit(PLC_DEVB_LOAD_ENABLE))
			{
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				//Error
				m_strErrorString.Format(_T("Error : ex : RobotLoaderEnableStatus timeout!(case 260)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_EQP_LOADER_ENABLE_OFF;//INTERLOCK_ERROR_EQP_LOADE_ENABLE_OFF;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}

			m_iTimeout100mm++;
			if(m_nTimeout[TIMEOUT_ROBOTBUSY] < m_iTimeout100mm)
			{
				//Error
				KillTimer(TIMER_INTERLOCK_EXCHANGE);
				InitSeqBit(EXCHANGE);

				typeEQPStatusDataPara eqp_stat;
				eqp_stat = GetStatusReport();
				eqp_stat.bGlassInProcess = FALSE;
				eqp_stat.bGlassExistInUnit = TRUE;
				eqp_stat.bGlassPosition = TRUE;
				SetStatus(eqp_stat);

				m_strErrorString.Format(_T("Error : ex: robot busy Off check & load noninterference On timeout!(case 260)"));
				WriteLog(m_strErrorString);
				m_nInterLockError = INTERLOCK_ERROR_LOADER_NONINTERFERENCET_ON_TIMEOUT;//INTERLOCK_ERROR_TIMEOUT;
				m_nInterLockErrorLevel = INTERLOCK_EROR_HEAVY_ALARM;
				//Opeation mode
				//SetBitOModeUnitAuto(FALSE);
				//SetBitOModeOperation_AutoOrManual(FALSE);
				break;
			}
			break;
		case 270:
			//if(0 == m_iTimeout100mm)
			m_nSeqDelay100mm++;
			if(0 == m_iTimeout100mm && m_nSeqDelay100mm == m_nTrigger[TRIGGER_SEQUENCE_DELAY])
			{
				//load enable Off
				m_PLCComm->SetBit(PLC_DEVB_LOAD_ENABLE, FALSE);
				SetLoaderBtn(PLC_DEVB_LOAD_ENABLE, FALSE);
				// Glass In Processing & GlassExist In Unit On
				m_PLCComm->SetBit(PLC_DEVB_GLASS_IN_PROCESSING, TRUE);
				m_PLCComm->SetBit(PLC_DEVB_GLASS_EXIST_IN_UNIT, TRUE);
				m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_270, PLC_DEVB_UNLOAD_ENABLE FALSE Set & PLC_DEVB_GLASS_IN_PROCESSING TRUE Set & PLC_DEVB_GLASS_EXIST_IN_UNIT TRUE Set!"));
				WriteLog(m_strStatusString);
				m_iSeqIdx = 280;
				m_iTimeout100mm = 0;
				m_nSeqDelay100mm = 0;
			}
			break;
		case 280:
			KillTimer(TIMER_INTERLOCK_EXCHANGE);
			InitSeqBit(EXCHANGE);
			m_bLoadingComplete = TRUE;
			m_UnloaderGlassData = m_LoaderGlassData;
			m_strStatusString.Format(_T("TIMER_INTERLOCK_EXCHANGE_280, Exchange complete ok"));
			pdlg->SetLoadingComplete(TRUE);
			WriteLog(m_strStatusString);
			break;
		}
		break;
	case 160://ecs start status
		if(!m_bECSStart)//m_PLCComm->GetBit(ECS_B_ADDRESS_ECS_START) && !m_bECSStart)
		{
			//KillTimer(160);
			m_bECSStart = TRUE;
			//SetStatusReport(m_bECSStart);
			SetTimer(TIMER_TPD_DATA_REPORT, 100, NULL);
		}
		else if(m_bECSStart)
		{
			m_bECSStart = FALSE;
			m_EQPStatus.nStatusRunIdleDown = STATUS_IDLE;
			//SetStatus(m_EQPStatus);
			//SetTimer(TIMER_ECS_START_STATUE_CHECK, 100, NULL);
		}
		break;
	case 170:
		
		break;
	case 300://recipe body data request
		//recipe body data request check
		if(m_PLCComm->GetBit(ECS_B_ADDRESS_RECIPE_BODY_DATA_REQUEST))//Read)
		{
			KillTimer(300);
			m_strStatusString.Format(_T("Recipe body data request on"));
			WriteLog(m_strStatusString);
			//body data write 해야함
			//main recip body get
			//SetRecipe(getRecipe);
			m_nTime = 0;
			SetRecipeBodyRequest(TRUE);
		}
		break;
	case 310://recipe body data report
		if(!m_PLCComm->GetBit(ECS_B_ADDRESS_RECIPE_BODY_DATA_REQUEST))//Read)
		{
			KillTimer(310);
			m_strStatusString.Format(_T("Recipe body data request off"));
			WriteLog(m_strStatusString);
			SetRecipeBodyRequest(FALSE);
			SetTimer(300, 100, NULL);
		}

		m_nTimeCheck[TIMEOUT_RECIPE_BODY_DATA_REQUEST]++;
		if(m_nTimeCheck[TIMEOUT_RECIPE_BODY_DATA_REQUEST] > m_nTimeout[TIMEOUT_RECIPE_BODY_DATA_REQUEST])
		{
			//Error
			KillTimer(310);
			m_strErrorString.Format(_T("Error : ECS_B_ADDRESS_RECIPE_BODY_DATA_REQUEST check timeout!(case 310)"));
			WriteLog(m_strErrorString);
			m_nInterLockError = INTERLOCK_ERROR_RECIPE_BODY_DATA_REQUEST_OFF_TIMEOUT;
			m_nInterLockErrorLevel = INTERLOCK_EROR_LIGHT_ALARM;
			m_nTimeCheck[TIMEOUT_RECIPE_BODY_DATA_REQUEST] = 0;
			//light alarm 발생
			SetTimer(300, 100, NULL);
		}
		break;
	case 320://ecs time data get
		if(m_PLCComm->GetBit(ECS_B_ADDRESS_TIME_DATA_SEND))
		{
			KillTimer(320);
			m_strECSTime = GetMCSCTime(ECS_W_ADDRESS_TIME_DATA);
			m_edit_strEcs_Time = m_strECSTime;
			UpdateData(FALSE);
			SetTimer(320, 100, NULL);
		}
		break;
	case 330://recipe various data request
		if(m_PLCComm->GetBit(ECS_B_ADDRESS_RECIPE_VARIOUS_DATA_REQUEST))
		{
			KillTimer(330);
			strLog.Format(_T("ECS_B_ADDRESS_RECIPE_VARIOUS_DATA_REQUEST ON"));
			WriteLog(strLog);
			ReadRecipeVariousData();
			if(m_RequestRecipeVariousData.nRecipeCEID == 4)
			{ 
				if(m_RequestRecipeVariousData.nRecipeCommand == 1)//recipe built
				{
					SetRecipeNum(m_RequestRecipeData.RecipeReport.nRecipeNumber, FALSE);
					strLog.Format(_T("Recipe Built, Recipe Number = %d, Recipe Level = %d, Recipe Type = %c, Recipe Version = %d%d%d, Eqp Specific Data V1 = %d, Recipe Para V1 = %d"),
						m_RequestRecipeData.RecipeReport.nRecipeNumber, m_RequestRecipeData.RecipeReport.nRecipeLevel, m_RequestRecipeData.RecipeReport.cRecipeType, 
						m_RequestRecipeData.RecipeReport.wRecipeVersion[0], m_RequestRecipeData.RecipeReport.wRecipeVersion[1], m_RequestRecipeData.RecipeReport.wRecipeVersion[2],
						m_wRequestSpecificData[0], m_RequestRecipeData.RecipeCell.dCell_pos_x);
					WriteLog(strLog);
				}
				else if(m_RequestRecipeVariousData.nRecipeCommand == 2)//recipe change
				{
					strLog.Format(_T("Recipe Change, Recipe Number = %d, Recipe Level = %d, Recipe Type = %c, Recipe Version = %d%d%d, Eqp Specific Data V1 = %d, Recipe Para V1 = %d"),
						m_RequestRecipeData.RecipeReport.nRecipeNumber, m_RequestRecipeData.RecipeReport.nRecipeLevel, m_RequestRecipeData.RecipeReport.cRecipeType, 
						m_RequestRecipeData.RecipeReport.wRecipeVersion[0], m_RequestRecipeData.RecipeReport.wRecipeVersion[1], m_RequestRecipeData.RecipeReport.wRecipeVersion[2],
						m_wRequestSpecificData[0], m_RequestRecipeData.RecipeCell.dCell_pos_x);
					WriteLog(strLog);
				}
				else if(m_RequestRecipeVariousData.nRecipeCommand == 3)//recipe delete
				{
					SetRecipeNum(m_RequestRecipeData.RecipeReport.nRecipeNumber, TRUE);
					strLog.Format(_T("Recipe Delete, Recipe Number = %d, Recipe Level = %d, Recipe Type = %c, Recipe Version = %d%d%d"),
						m_RequestRecipeData.RecipeReport.nRecipeNumber, m_RequestRecipeData.RecipeReport.nRecipeLevel, m_RequestRecipeData.RecipeReport.cRecipeType, 
						m_RequestRecipeData.RecipeReport.wRecipeVersion[0], m_RequestRecipeData.RecipeReport.wRecipeVersion[1], m_RequestRecipeData.RecipeReport.wRecipeVersion[2]);
					WriteLog(strLog);
				}
				else if(m_RequestRecipeVariousData.nRecipeCommand == 4)//recipe rename
				{
					strLog.Format(_T("Recipe Rename, dest Recipe Number = %d, rename Recipe Number = %d, Recipe Level = %d, Recipe Type = %c, Recipe Version = %d%d%d"),
						m_RequestRecipeData.RecipeReport.nRecipeNumber, m_RequestRecipeData.RecipeCell.dCell_pos_x, m_RequestRecipeData.RecipeReport.nRecipeLevel, m_RequestRecipeData.RecipeReport.cRecipeType, 
						m_RequestRecipeData.RecipeReport.wRecipeVersion[0], m_RequestRecipeData.RecipeReport.wRecipeVersion[1], m_RequestRecipeData.RecipeReport.wRecipeVersion[2]);
					WriteLog(strLog);
				}
			}
			WriteRecipeVariousAckData();
			//SetTimer(TIMER_RECIPE_VARIOUS_DATA_REQUEST, 100, NULL);// eqp various request
		}
		break;
	case 340://scrap glass data report
		//glass apd report falus check, glass data change report falus check
		if(!m_PLCComm->GetBit(PLC_DEVB_GLASS_APD_DATA_REPORT) && !m_PLCComm->GetBit(PLC_DEVB_GLASS_DATA_CHANGE_REPORT))
		{
			KillTimer(340);
			SetScrapGlassData(GetUpdateGlassData());
			WriteReportGlassData(0);
			WriteScrapGlassData();
			SetScrapGlass(TRUE);
		}
		break;
	case 350://lost glass data report
		//glass apd report falus check, glass data change report falus check
#ifdef MACHINE
		if(m_PLCComm->GetBit(ECS_B_ADDRESS_GLASS_DATA_REPORT))
		{
			if(m_PLCComm->GetBit(ECS_B_ADDRESS_GLASS_DATA_REQUEST_ACK_OK))
			{
				KillTimer(350);
				SetLostGlassDataReport(FALSE);
				m_bLostGlassDataRequest = TRUE;
				ReadLostGlassData();
				SetUpdateGlassData(m_LostGlassData);
				GlassDataUpdate(FALSE);
				m_LoaderGlassData = GetUpdateGlassData();
				m_UnloaderGlassData = GetUpdateGlassData();
				//if(m_EQPStatus.bGlassExistInUnit)
				SetBitEStatusGlassExist(m_EQPStatus.bGlassExistInUnit);
			}
			else if(m_PLCComm->GetBit(ECS_B_ADDRESS_GLASS_DATA_REQUEST_ACK_NG))
			{
				KillTimer(350);
				SetLostGlassDataReport(FALSE);
				m_bLostGlassDataRequest = FALSE;
				
				if(m_nLoastGlassDataOption == 0)
				{
					AfxMessageBox(_T("해당 Glass ID에 해당 되는 Glass Data가 없습니다."));
				}
				else if(m_nLoastGlassDataOption == 1)
				{
					AfxMessageBox(_T("해당 Glass Code에 해당 되는 Glass Data가 없습니다."));
				}
			}
		}

		m_nTimeCheck[TIMEOUT_LOST_GLASS_DATA_REQUEST]++;
		if(m_nTimeCheck[TIMEOUT_LOST_GLASS_DATA_REQUEST] > m_nTimeout[TIMEOUT_LOST_GLASS_DATA_REQUEST])
		{
			//Error
			m_nTimeCheck[TIMEOUT_LOST_GLASS_DATA_REQUEST] = 0;
			KillTimer(350);
			SetLostGlassDataReport(FALSE);
			m_strErrorString.Format(_T("Error : TIMEOUT_LOST_GLASS_DATA_REQUEST check timeout!(case 350)"));
			WriteLog(m_strErrorString);
			m_nInterLockError = INTERLOCK_ERROR_LAST_GLASS_DATA_REQUEST_ON_TIMEOUT;
			m_nInterLockErrorLevel = INTERLOCK_EROR_LIGHT_ALARM;
			m_strErrorString.Format(_T("Error : INTERLOCK_ERROR_LAST_GLASS_DATA_REQUEST_ON_TIMEOUT"));
			WriteLog(m_strErrorString);
			//light alarm 발생
		}
#else
		m_UnloaderGlassData = m_LoaderGlassData;
		KillTimer(350);
#endif
		break;
	case 360://change glass data report
		//glass apd report falus check, glass data change report falus check
		if(!m_PLCComm->GetBit(PLC_DEVB_GLASS_APD_DATA_REPORT) && !m_PLCComm->GetBit(PLC_DEVB_SCRAP_GLASS_DATA_REPORT))
		{
			KillTimer(360);
			SetChangeGlassData(GetLoaderGlassData());
			WriteChangeGlassData();
			WriteReportGlassData(7);
			SetChangeGlass(TRUE);
		}
		break;
	case 370://glass apd data write
		if(!m_PLCComm->GetBit(PLC_DEVB_GLASS_DATA_CHANGE_REPORT) && !m_PLCComm->GetBit(PLC_DEVB_SCRAP_GLASS_DATA_REPORT))
		{
			KillTimer(370);
			//SetChangeGlassData(GetUnloaderGlassData());
			//WriteChangeGlassData();
			//WriteReportGlassData(1);
			//SetAPDData();
			typeAPDDataPara apdData;
			apdData = GetAPD();
			sprintf(apdData.cGlassID, "%s", m_UnloaderGlassData.strGlassID);
			apdData.cAlignSuccess = 'T';
			apdData.nAlignMarkX1 = 1;
			apdData.nAlignMarkX2 = 2;
			apdData.nAlignMarkY1 = 3;
			apdData.nAlignMarkY2 = 4;
			apdData.nInspMode = 5;
			SetAPD(apdData);
			SetAPDData();
			m_MelsecListPara[DEVB_GLASS_APD_DATA_REPORT].bValue = TRUE;
			SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_GLASS_APD_DATA_REPORT);
			//SetAPDData();
		}
		break;
	case 380://tpd data write
		m_nTPDWriteDelay++;
		if(m_nTPDWriteDelay == m_nTrigger[TRIGGER_TPD_DATA_DELAY])
		{
			//main get tpd data
#ifdef _ECS_TEST
			m_TPDData.nChuckVac += 1;
			if(m_TPDData.nChuckVac == 65550)
				m_TPDData.nChuckVac = 65535;

			m_TPDData.nLIFT_AXIS_TQ += 1;
			if(m_TPDData.nLIFT_AXIS_TQ == 65550)
				m_TPDData.nLIFT_AXIS_TQ = 65535;

			m_TPDData.nRV_Y_Axis_TQ += 1;
			if(m_TPDData.nRV_Y_Axis_TQ == 65550)
				m_TPDData.nRV_Y_Axis_TQ = 65535;

			m_TPDData.nScan_X_Axis_TQ += 1;
			if(m_TPDData.nScan_X_Axis_TQ == 65550)
				m_TPDData.nScan_X_Axis_TQ = 65535;

			m_TPDData.nSolBlw += 1;
			if(m_TPDData.nSolBlw == 65550)
				m_TPDData.nSolBlw = 65535;
#endif
			//SetTPD(getTPD);
			m_nTPDWriteDelay = 0;
			SetTPDData();
		}
		break;
	case 390://glass position data

		break;
	//Bit off(밑으로는 모두 bit off이다)///////////////////////////////////////////////////////////////////////////
	case 500 + (DEVB_LIGHT_ALARM_REPORT - AREA_BIT_1_START_ITEM_NUM):
		//SetLightAlarmReport(FALSE, 99);
		KillTimer(500 + (DEVB_LIGHT_ALARM_REPORT - AREA_BIT_1_START_ITEM_NUM));
		strLog.Format(_T("DEVB_LIGHT_ALARM_REPORT OFF"));
			WriteLog(strLog);
		m_MelsecListPara[DEVB_LIGHT_ALARM_REPORT].bValue = FALSE;
		SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_LIGHT_ALARM_REPORT);
		break;
	case 500 + (DEVB_HEAVY_ALARM_REPORT - AREA_BIT_1_START_ITEM_NUM):
		KillTimer(500 + (DEVB_HEAVY_ALARM_REPORT - AREA_BIT_1_START_ITEM_NUM));
		strLog.Format(_T("DEVB_HEAVY_ALARM_REPORT OFF"));
			WriteLog(strLog);
		m_MelsecListPara[DEVB_HEAVY_ALARM_REPORT].bValue = FALSE;
		SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_HEAVY_ALARM_REPORT);
		break;
	case 500 + (DEVB_EQP_RECIPE_LIST_CHANGE - AREA_BIT_1_START_ITEM_NUM):
		KillTimer(500 + (DEVB_EQP_RECIPE_LIST_CHANGE - AREA_BIT_1_START_ITEM_NUM));
		strLog.Format(_T("DEVB_EQP_RECIPE_LIST_CHANGE OFF"));
			WriteLog(strLog);
		SetRecipeListChange(FALSE);
		break;
	case 500 + (DEVB_EQIPMENT_RECIPE_REQUEST - AREA_BIT_1_START_ITEM_NUM):
		if(m_PLCComm->GetBit(ECS_B_ADDRESS_EQUIPMENT_RECIPE_REPORT))
		{
			KillTimer(500 + (DEVB_EQIPMENT_RECIPE_REQUEST - AREA_BIT_1_START_ITEM_NUM));
			strLog.Format(_T("DEVB_EQIPMENT_RECIPE_REQUEST OFF"));
			WriteLog(strLog);
			SetHostRecipeRequest(m_SelectRecipeData.RecipeData.nRecipeNum, FALSE);
		}
		
		m_nTimeCheck[TIMEOUT_EQP_RECIPE_REQUEST_CONFIRM]++;
		if(m_nTimeCheck[TIMEOUT_EQP_RECIPE_REQUEST_CONFIRM] > m_nTimeout[TIMEOUT_EQP_RECIPE_REQUEST_CONFIRM])
		{
			//Error
			KillTimer(500 + (DEVB_EQIPMENT_RECIPE_REQUEST - AREA_BIT_1_START_ITEM_NUM));
			m_MelsecListPara[DEVB_EQIPMENT_RECIPE_REQUEST].bValue = FALSE;
			SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_EQIPMENT_RECIPE_REQUEST);
			m_strErrorString.Format(_T("Error : DEVB_EQIPMENT_RECIPE_REQUEST check timeout!(case 500 + (DEVB_EQIPMENT_RECIPE_REQUEST - AREA_BIT_1_START_ITEM_NUM))"));
			WriteLog(m_strErrorString);
			m_nInterLockError = INTERLOCK_ERROR_EQP_RECIPE_REQUEST_CONFIRM_ON_TIMEOUT;
			m_nInterLockErrorLevel = INTERLOCK_EROR_LIGHT_ALARM;
			m_strErrorString.Format(_T("Error : INTERLOCK_ERROR_EQP_RECIPE_REQUEST_CONFIRM_ON_TIMEOUT"));
			WriteLog(m_strErrorString);
			//light alarm 발생
		}
		break;
	case 500 + (DEVB_RECIPE_VARIOUS_REQUEST_CONFIRM - AREA_BIT_1_START_ITEM_NUM):
		if(!m_PLCComm->GetBit(ECS_B_ADDRESS_RECIPE_VARIOUS_DATA_REQUEST))
		{
			KillTimer(500 + (DEVB_RECIPE_VARIOUS_REQUEST_CONFIRM - AREA_BIT_1_START_ITEM_NUM));
			strLog.Format(_T("DEVB_RECIPE_VARIOUS_REQUEST_CONFIRM OFF"));
			WriteLog(strLog);
			m_MelsecListPara[DEVB_RECIPE_VARIOUS_REQUEST_CONFIRM].bValue = FALSE;
			SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_RECIPE_VARIOUS_REQUEST_CONFIRM);

			if(m_RequestRecipeVariousData.nRecipeCEID == 1)//recipe list
			{
				int recipe_total_cnt = 0;
				typeRecipeReportDataPara *recipeReportData;
				recipeReportData = new typeRecipeReportDataPara[992];

				memset(recipeReportData, NULL, sizeof(typeRecipeReportDataPara) * 992);
	
				//recipe list load;
				//m_RecipeReport 구조체 배열에 담기
				int recipeCnt = 0;
				for(int i = 0; i < 999; i++)
				{
					if(m_RecipeData[i].bRecipeNum)
					{
						recipe_total_cnt++;
						recipeReportData[recipeCnt].nRecipeNumber = m_RecipeData[i].nRecipeNum;
						recipeReportData[recipeCnt].nRecipeLevel = 2;
						recipeReportData[recipeCnt].cRecipeType = 'M';
						recipeReportData[recipeCnt].wRecipeVersion[0] = m_RecipeData[i].nVersion[0];
						recipeReportData[recipeCnt].wRecipeVersion[1] = m_RecipeData[i].nVersion[1];
						recipeReportData[recipeCnt].wRecipeVersion[2] = m_RecipeData[i].nVersion[2];
						if(m_RecipeData[i].nRecipeNum == m_Recipe.nRecipeNumber)
						{
							recipeReportData[recipeCnt].nRecipeStatus = 1;
						}
						else
						{
							recipeReportData[recipeCnt].nRecipeStatus = 3;
						}
						recipeReportData[recipeCnt++].nUnitNumber = 1;
					}
				}
				SetRecipeList(recipeReportData);
				m_RecipeVariousData.nRecipeTotalCount = recipe_total_cnt;
				m_RecipeVariousData.nRecipeReportCount = m_nReport_RecipeCnt;
				
				if(m_RecipeVariousData.nRecipeTotalCount < m_RecipeVariousData.nRecipeReportCount)
				{
					m_nReportStart_RecipeListCnt = 0;
					m_nReportEnd_RecipeListCnt = m_RecipeVariousData.nRecipeTotalCount;
					m_nOrderCEID = 2;
				}
				else
				{
					if(m_nReportStart_RecipeListCnt == 0 && ((double)((double)m_RecipeVariousData.nRecipeTotalCount / (double)m_RecipeVariousData.nRecipeReportCount) > 1.0))
					{
						m_nReportStart_RecipeListCnt = 0;
						m_nReportEnd_RecipeListCnt = m_RecipeVariousData.nRecipeReportCount;
						m_nOrderCEID = 0;
					}
					else if(m_nReportStart_RecipeListCnt == 0 && ((double)((double)m_RecipeVariousData.nRecipeTotalCount / (double)m_RecipeVariousData.nRecipeReportCount) <= 1.0))
					{
						m_nReportStart_RecipeListCnt = 0;
						m_nReportEnd_RecipeListCnt = m_RecipeVariousData.nRecipeTotalCount;
						m_nOrderCEID = 2;
					}
					else if(m_nReportStart_RecipeListCnt > 0 && ((double)((double)m_RecipeVariousData.nRecipeTotalCount / (double)(m_RecipeVariousData.nRecipeReportCount + (double)m_nReportStart_RecipeListCnt)) > 1.0))
					{
						m_nReportStart_RecipeListCnt = m_nReportEnd_RecipeListCnt;//m_RecipeVariousData.nRecipeReportCount;
						m_nReportEnd_RecipeListCnt = m_nReportStart_RecipeListCnt + m_RecipeVariousData.nRecipeReportCount;
						m_nOrderCEID = 1;
					}
					else if(m_nReportStart_RecipeListCnt > 0 && ((double)((double)m_RecipeVariousData.nRecipeTotalCount / (double)(m_RecipeVariousData.nRecipeReportCount + (double)m_nReportStart_RecipeListCnt)) <= 1.0))
					{
						m_nReportStart_RecipeListCnt = m_nReportEnd_RecipeListCnt;
						m_nReportEnd_RecipeListCnt = m_RecipeVariousData.nRecipeTotalCount;
						m_nOrderCEID = 2;
					}
				}

				WriteRecipeList(m_nOrderCEID, m_nReportStart_RecipeListCnt, m_nReportEnd_RecipeListCnt);

#ifndef _ECS_TEST
				m_MelsecListPara[DEVB_RECIPE_VARIOUS_DATA_REPORT].bValue = TRUE;
				SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_RECIPE_VARIOUS_DATA_REPORT);	
#endif

				if(m_nOrderCEID == 2)
				{
					m_nReportStart_RecipeListCnt = 0;
				}
			}
			else if(m_RequestRecipeVariousData.nRecipeCEID == 3)//recipe parameter
			{
				//m_RequestRecipeVariousData.nRecipeNumber
				//해당 recipe 파라메터 read
				//해당 recipe 파라메터 write
				WriteRecipeVariousData(m_RequestRecipeVariousData.nRecipeCEID);
			}
			else if(m_RequestRecipeVariousData.nRecipeCEID == 6)
			{
				WriteRecipeVariousData(m_RequestRecipeVariousData.nRecipeCEID);
			}

			//WriteRecipeVariousData(m_RequestRecipeVariousData.nRecipeCEID);

			//SetTimer(TIMER_RECIPE_VARIOUS_DATA_REQUEST, 100, NULL);// eqp various request
			//SetTimer(390, 100, NULL);
		}
		
		m_nTimeCheck[TIMEOUT_RECIPE_VARIOUS_DATA_REQUEST]++;
		if(m_nTimeCheck[TIMEOUT_RECIPE_VARIOUS_DATA_REQUEST] > m_nTimeout[TIMEOUT_RECIPE_VARIOUS_DATA_REQUEST])
		{
			//Error
			KillTimer(500 + (DEVB_RECIPE_VARIOUS_REQUEST_CONFIRM - AREA_BIT_1_START_ITEM_NUM));
			m_MelsecListPara[DEVB_RECIPE_VARIOUS_REQUEST_CONFIRM].bValue = FALSE;
			SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_RECIPE_VARIOUS_REQUEST_CONFIRM);
			m_strErrorString.Format(_T("Error : TIMEOUT_RECIPE_VARIOUS_DATA_REQUEST check timeout!(case 500 + (DEVB_RECIPE_VARIOUS_REQUEST_CONFIRM - AREA_BIT_1_START_ITEM_NUM))"));
			WriteLog(m_strErrorString);
			if(m_RequestRecipeVariousData.nRecipeCEID == 1)//recipe list
			{
				m_nInterLockError = INTERLOCK_ERROR_RECIPE_LIST_REQUEST_OFF_TIMEOUT;
				m_strErrorString.Format(_T("Error : INTERLOCK_ERROR_RECIPE_LIST_REQUEST_OFF_TIMEOUT"));
			}
			else if(m_RequestRecipeVariousData.nRecipeCEID == 3)//recipe parameter
			{
				m_nInterLockError = INTERLOCK_ERROR_RECIPE_PARAMETER_REQUEST_OFF_TIMEOUT;
				m_strErrorString.Format(_T("Error : INTERLOCK_ERROR_RECIPE_PARAMETER_REQUEST_OFF_TIMEOUT"));
			}
			else if(m_RequestRecipeVariousData.nRecipeCEID == 4)//command
			{
				if(m_RequestRecipeVariousData.nRecipeCommand == 1)//built
				{
					//recipe 생성
					m_nInterLockError = INTERLOCK_ERROR_RECIPE_COMMAND_BUILT_REQUEST_OFF_TIMEOUT;
					m_strErrorString.Format(_T("Error : INTERLOCK_ERROR_RECIPE_COMMAND_BUILT_REQUEST_OFF_TIMEOUT"));
				}
				else if(m_RequestRecipeVariousData.nRecipeCommand == 2)//change
				{
					//recipe 변경
					m_nInterLockError = INTERLOCK_ERROR_RECIPE_COMMAND_CHANGE_REQUEST_OFF_TIMEOUT;
					m_strErrorString.Format(_T("Error : INTERLOCK_ERROR_RECIPE_COMMAND_CHANGE_REQUEST_OFF_TIMEOUT"));
				}
				else if(m_RequestRecipeVariousData.nRecipeCommand == 3)//delete
				{
					//recipe 삭제
					m_nInterLockError = INTERLOCK_ERROR_RECIPE_COMMAND_DELETE_REQUEST_OFF_TIMEOUT;
					m_strErrorString.Format(_T("Error : INTERLOCK_ERROR_RECIPE_COMMAND_DELETE_REQUEST_OFF_TIMEOUT"));
				}
				else if(m_RequestRecipeVariousData.nRecipeCommand == 4)//rename
				{
					//recipe 이름 변경
					m_nInterLockError = INTERLOCK_ERROR_RECIPE_COMMAND_RENAME_REQUEST_OFF_TIMEOUT;
					m_strErrorString.Format(_T("Error : INTERLOCK_ERROR_RECIPE_COMMAND_RENAME_REQUEST_OFF_TIMEOUT"));
				}
			}
			else if(m_RequestRecipeVariousData.nRecipeCEID == 5)//recipe change
			{
				m_nInterLockError = INTERLOCK_ERROR_RECIPE_CHANGE_REPORT_CONFIRM_ON_TIMEOUT;
				m_strErrorString.Format(_T("Error : INTERLOCK_ERROR_RECIPE_CHANGE_REPORT_CONFIRM_ON_TIMEOUT"));
			}
			else if(m_RequestRecipeVariousData.nRecipeCEID == 6 || m_RequestRecipeVariousData.nRecipeCEID == 7)//recipe constant para
			{
				m_nInterLockError = INTERLOCK_ERROR_EQP_CONSTANT_RECIPE_PARAMETER_REQUEST_OFF_TIMEOUT;
				m_strErrorString.Format(_T("Error : INTERLOCK_ERROR_EQP_CONSTANT_RECIPE_PARAMETER_REQUEST_OFF_TIMEOUT"));
			}
			WriteLog(m_strErrorString);

			m_nInterLockErrorLevel = INTERLOCK_EROR_LIGHT_ALARM;
			//light alarm 발생
		}
		break;
	case 500 + (DEVB_RECIPE_VARIOUS_DATA_REPORT - AREA_BIT_1_START_ITEM_NUM):
		if(!m_PLCComm->GetBit(ECS_B_ADDRESS_RECIPE_VARIOUS_DATA_CONFIRM))
		{
			KillTimer(500 + (DEVB_RECIPE_VARIOUS_DATA_REPORT - AREA_BIT_1_START_ITEM_NUM));
			strLog.Format(_T("DEVB_RECIPE_VARIOUS_DATA_REPORT OFF"));
			WriteLog(strLog);
			m_MelsecListPara[DEVB_RECIPE_VARIOUS_DATA_REPORT].bValue = FALSE;
			SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_RECIPE_VARIOUS_DATA_REPORT);
			//SetEquipmentRecipeRequest(m_SelectRecipeData.RecipeData.nRecipeNum, FALSE);
		}
		
		m_nTimeCheck[TIMEOUT_RECIPE_VARIOUS_DATA_REPORT]++;
		if(m_nTimeCheck[TIMEOUT_RECIPE_VARIOUS_DATA_REPORT] > m_nTimeout[TIMEOUT_RECIPE_VARIOUS_DATA_REPORT])
		{
			//Error
			KillTimer(500 + (DEVB_RECIPE_VARIOUS_DATA_REPORT - AREA_BIT_1_START_ITEM_NUM));
			m_MelsecListPara[DEVB_RECIPE_VARIOUS_DATA_REPORT].bValue = FALSE;
			SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_RECIPE_VARIOUS_DATA_REPORT);
			m_strErrorString.Format(_T("Error : TIMEOUT_RECIPE_VARIOUS_DATA_REPORT check timeout!(case 500 + (DEVB_RECIPE_VARIOUS_DATA_REPORT - AREA_BIT_1_START_ITEM_NUM))"));
			WriteLog(m_strErrorString);

			if(m_RequestRecipeVariousData.nRecipeCEID == 1)//recipe list
			{
				m_nInterLockError = INTERLOCK_ERROR_RECIPE_LIST_REQUEST_CONFIRM_ON_TIMEOUT;
				m_strErrorString.Format(_T("Error : INTERLOCK_ERROR_RECIPE_LIST_REQUEST_CONFIRM_ON_TIMEOUT"));
			}
			else if(m_RequestRecipeVariousData.nRecipeCEID == 3)//recipe parameter
			{
				m_nInterLockError = INTERLOCK_ERROR_RECIPE_PARAMETER_REQUEST_CONFIRM_ON_TIMEOUT;
				m_strErrorString.Format(_T("Error : INTERLOCK_ERROR_RECIPE_PARAMETER_REQUEST_CONFIRM_ON_TIMEOUT"));
			}
			else if(m_RequestRecipeVariousData.nRecipeCEID == 6 || m_RequestRecipeVariousData.nRecipeCEID == 7)//recipe constant para
			{
				m_nInterLockError = INTERLOCK_ERROR_EQP_CONSTANT_RECIPE_PARAMETER_CONFIRM_ON_TIMEOUT;
				m_strErrorString.Format(_T("Error : INTERLOCK_ERROR_EQP_CONSTANT_RECIPE_PARAMETER_CONFIRM_ON_TIMEOUT"));
			}
			WriteLog(m_strErrorString);

			m_nInterLockError = INTERLOCK_EROR_LIGHT_ALARM;
			//light alarm 발생
		}
		break;
	case 500 + (DEVB_SCRAP_GLASS_DATA_REPORT - AREA_BIT_1_START_ITEM_NUM):
		SetScrapGlass(FALSE);
		KillTimer(500 + (DEVB_SCRAP_GLASS_DATA_REPORT - AREA_BIT_1_START_ITEM_NUM));
		break;
	case 500 + (DEVB_GLASS_DATA_CHANGE_REPORT - AREA_BIT_1_START_ITEM_NUM):
		SetChangeGlass(FALSE);
		KillTimer(500 + (DEVB_GLASS_DATA_CHANGE_REPORT - AREA_BIT_1_START_ITEM_NUM));
		break;
	case 500 +(DEVB_GLASS_APD_DATA_REPORT - AREA_BIT_1_START_ITEM_NUM):
		m_MelsecListPara[DEVB_GLASS_APD_DATA_REPORT].bValue = FALSE;
		SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_GLASS_APD_DATA_REPORT);
		KillTimer(500 + (DEVB_GLASS_APD_DATA_REPORT - AREA_BIT_1_START_ITEM_NUM));
		break;
	}

	
	//if(m_bLogWrite)
	//{
	//	WriteLog(m_strErrorString);
	//}

	if(m_nInterLockError == INTERLOCK_EROR_HEAVY_ALARM)
	{
		pdlg->SetLoadingComplete(FALSE);
	}

	if(nIDEvent < 4)
	{
		ReadMemory((int)nIDEvent);
	}

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CDialogInline::SetRecipeNum(int recipeNum, BOOL bDelete)
{
	//WriteLog(_T("SetRecipeNum Start"));
	CString strLog;
	BOOL bit1[16], bit2[16], bit3[16];
	WORD word1, word2;
	word1 = 0;
	word2 = 0;
	BOOL bRecipeChange = FALSE;
	m_RecipeData[recipeNum].nRecipeNum = recipeNum;
	m_RecipeData[recipeNum].bRecipeNum = !bDelete;
	m_RecipeData[recipeNum].bRecipeBody = !bDelete;
	int i = 0;

	if(recipeNum > 15)
	{
		i = recipeNum / 16;
	}

	if((recipeNum == 1) || (recipeNum%16 == 0))
	{
		m_MelsecListPara[DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_1_15 + i].wValue = 0;
	}

	if(1 <= recipeNum && recipeNum < 16)
	{
		i = 0;	
		if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i)))
		{
			word1 = 2;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 1)))
		{
			word1 = 4;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 2)))
		{
			word1 = 8;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 3)))
		{
			word1 = 16;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 4)))
		{
			word1 = 32;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 5)))
		{
			word1 = 64;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 6)))
		{
			word1 = 128;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 7)))
		{
			word1 = 256;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 8)))
		{
			word1 = 512;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 9)))
		{
			word1 = 1024;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 10)))
		{
			word1 = 2048;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 11)))
		{
			word1= 4096;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 12)))
		{
			word1 = 8192;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 13)))
		{
			word1 = 16384;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 14)))
		{
			word1 = 32768;
		}

		WordToBit(word1, bit1);
		WordToBit(0, bit2);

		if(bDelete)
		{
			for(int i = 15; i > -1; i--)
			{
				bit3[i] = bit2[i] & bit1[i]; 
			}
		}
		else
		{
			for(int i = 15; i > -1; i--)
			{
				bit3[i] = bit2[i] | bit1[i]; 
			}
		}
		BitToWord(bit3, &word2);
		m_MelsecListPara[DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_1_15].wValue += word2;
		bRecipeChange = TRUE;
		SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_1_15);
	}
	else if((1 + (RECIPE_SEARCH_CNT * i) + (i - 1)) <= recipeNum && recipeNum <= (16 + (RECIPE_SEARCH_CNT * i) + (i - 1)))
	{
		if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i - 1)))
		{
			word1 = 1;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i)))
		{
			word1 = 2;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 1)))
		{
			word1 = 4;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 2)))
		{
			word1 = 8;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 3)))
		{
			word1 = 16;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 4)))
		{
			word1 = 32;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 5)))
		{
			word1 = 64;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 6)))
		{
			word1 = 128;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 7)))
		{
			word1 = 256;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 8)))
		{
			word1 = 512;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 9)))
		{
			word1 = 1024;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 10)))
		{
			word1 = 2048;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 11)))
		{
			word1= 4096;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 12)))
		{
			word1 = 8192;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 13)))
		{
			word1 = 16384;
		}
		else if(recipeNum == (1 + (RECIPE_SEARCH_CNT * i) + (i + 14)))
		{
			word1 = 32768;
		}
			
			
		if(i == 3)
		{
			word1;
		}

		WordToBit(word1, bit1);
		WordToBit(0, bit2);

		if(bDelete)
		{
			for(int j = 15; j > -1; j--)
			{
				bit3[j] = bit2[j] & bit1[j]; 
			}
		}
		else
		{
			for(int k = 15; k > -1; k--)
			{
				bit3[k] = bit2[k] | bit1[k]; 
			}
		}

		BitToWord(bit3, &word2);
		m_MelsecListPara[DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_1_15 + i].wValue += word2;
		bRecipeChange = TRUE;
		SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_1_15 + i);
	}

	if(bDelete)
	{
		strLog.Format(_T("Num %d Recipe List OFF"), recipeNum);
	}
	else
	{
		strLog.Format(_T("Num %d Recipe List On"), recipeNum);
	}
	WriteLog(strLog);

	if(recipeNum == 15)
	{
		strLog.Format(_T("%S, Word Value : %d"), m_MelsecListPara[DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_1_15 + i].strName, m_MelsecListPara[DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_1_15 + i].wValue);
		WriteLog(strLog);
	}

	if((i > 0) && ((recipeNum + 1) == (16 * (i + 1))))
	{
		strLog.Format(_T("%S, Word Value : %d"), m_MelsecListPara[DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_1_15 + i].strName, m_MelsecListPara[DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_1_15 + i].wValue);
		WriteLog(strLog);
	}
	

	//WriteLog(_T("SetRecipeNum End"));
	return TRUE;
}

BOOL CDialogInline::SetRecipeListChange(BOOL bit)
{
	WriteLog(_T("SetRecipeListChange Start"));
	m_MelsecListPara[DEVB_EQP_RECIPE_LIST_CHANGE].bValue = bit;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_EQP_RECIPE_LIST_CHANGE);
	WriteLog(_T("SetRecipeListChange End"));
	return TRUE;
}

void CDialogInline::Write(CString data, int nAreaType, int AddrItemIdx)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	WriteLog(_T("Write Start"));
	UpdateData(TRUE);
	WORD *wWord;
	BOOL bBit;
	char temp[10];
	int cnt = 0;

	wWord = new WORD;

	if(nAreaType == 0 || nAreaType == 1)
	{
		cnt = m_MelsecListPara[AREA_WORD_1_START_ITEM_NUM + AddrItemIdx].nSize;
		for(int i = 0; i < cnt; i++)
		{
			if(data.Mid(i * 2, 2).GetLength() == 0)
			{
				break;
			}
			sprintf(temp, "%S", data.Mid(i * 2, 2));
			StringToWord(temp, wWord);
			if(nAreaType == 0)
			{
				m_MelsecListPara[AREA_WORD_1_START_ITEM_NUM + AddrItemIdx].wValue = *wWord;
			}
			else if(nAreaType == 1)
			{
				m_MelsecListPara[AREA_WORD_2_START_ITEM_NUM + AddrItemIdx].wValue = *wWord;
			}
		
			m_PLCComm->Write(DevW, m_MelsecItem[AddrItemIdx].nAddress + i, m_MelsecItem[AddrItemIdx].nAddress + m_MelsecItem[AddrItemIdx].nSize - 1, wWord, sizeof(WORD));
		}
	}
	else if(nAreaType == 2)
	{
		bBit = _wtoi(data);
		WriteData(AddrItemIdx, bBit, nAreaType);
	}
	
	ReadData(AddrItemIdx, nAreaType);
	UpdateListValue();
	//ValueTypeItemCommboUpdate(AddrItemIdx, nAreaType);
	delete wWord;
	WriteLog(_T("Write End"));
	//UpdateData(FALSE);
}

void CDialogInline::ReadData(int idx, int type)
{
	WriteLog(_T("ReadData Start"));
	char temp[2];
	CString temp1, temp2;
	temp1.Empty();
	temp2.Empty();
	int cnt = 0;
	WORD wWord = 0;
	BOOL bit[16];
	BOOL bBit = FALSE;

	memset(temp, NULL, 2);
	memset(bit, NULL, 16);
	if(type == 0)
	{
		if(m_MelsecListPara[idx + AREA_WORD_1_START_ITEM_NUM].bWORD_BIT)
		{
			wWord = m_PLCComm->GetWORD(m_MelsecItem[idx +AREA_WORD_1_START_ITEM_NUM].nAddress);
			WordToBit(wWord, bit);
			for(int i = 15; i > -1; i--)
			{
				temp1.Format(_T("%s%c"), temp[i], temp1);
				temp2.Format(_T("%s"), temp1);
			}
			//m_edit_strReadValue.Format(_T("%s"), temp2);
			sprintf(m_MelsecListPara[idx + AREA_WORD_1_START_ITEM_NUM].strValue, "%S",  temp2);
		}
		else
		{
			cnt = m_MelsecListPara[idx + AREA_WORD_1_START_ITEM_NUM].nSize;
			for(int i = 0; i < cnt; i++)
			{
				wWord = m_PLCComm->GetWORD(m_MelsecItem[idx +AREA_WORD_1_START_ITEM_NUM].nAddress + i);
				if(wWord == 0)
				{
					break;
				}
				WordToString(wWord, temp);
				temp1.Format(_T("%s%c%c"), temp2, temp[0], temp[1]);
				temp2.Format(_T("%s"), temp1);
			}
			//m_edit_strReadValue.Format(_T("%s"), temp2);
			sprintf(m_MelsecListPara[idx + AREA_WORD_1_START_ITEM_NUM].strValue, "%S",  temp2);
		}
	}
	else if(type == 1)
	{
		if(m_MelsecListPara[idx + AREA_WORD_2_START_ITEM_NUM].bWORD_BIT)
		{
			wWord = m_PLCComm->GetWORD(m_MelsecItem[idx +AREA_WORD_2_START_ITEM_NUM].nAddress);
			WordToBit(wWord, bit);
			for(int i = 15; i > -1; i--)
			{
				temp1.Format(_T("%s%c"), temp[i], temp1);
				temp2.Format(_T("%s"), temp1);
			}
			//m_edit_strReadValue.Format(_T("%s"), temp2);
			sprintf(m_MelsecListPara[idx + AREA_WORD_2_START_ITEM_NUM].strValue, "%S",  temp2);
		}
		else
		{
			cnt = m_MelsecListPara[idx + AREA_WORD_2_START_ITEM_NUM].nSize;
			for(int i = 0; i < cnt; i++)
			{
				wWord = m_PLCComm->GetWORD(m_MelsecItem[idx +AREA_WORD_2_START_ITEM_NUM].nAddress + i);
				if(wWord == 0)
				{
					break;
				}
				WordToString(wWord, temp);
				temp1.Format(_T("%s%c%c"), temp2, temp[0], temp[1]);
				temp2.Format(_T("%s"), temp1);
			}
			//m_edit_strReadValue.Format(_T("%s"), temp2);
			sprintf(m_MelsecListPara[idx + AREA_WORD_2_START_ITEM_NUM].strValue, "%S",  temp2);
		}
	}
	else if(type == 2)
	{
		bBit = m_PLCComm->GetBit(m_MelsecItem[idx +AREA_BIT_1_START_ITEM_NUM].nAddress);
		//m_edit_strReadValue.Format(_T("%d"), GetBitValue(idx + AREA_BIT_1_START_ITEM_NUM));
		sprintf(m_MelsecListPara[idx + AREA_WORD_2_START_ITEM_NUM].strValue, "%d", bBit);
	}
	WriteLog(_T("ReadData End"));
	//UpdateData(FALSE);
}

void CDialogInline::WriteData(int idx, WORD wWord, int type)
{
	WriteLog(_T("WriteData(WORD) Start"));
	if(type == 0)
	{ 
		SetValue(idx + AREA_WORD_1_START_ITEM_NUM, wWord);
	}
	else if(type == 1)
	{
		SetValue(idx + AREA_WORD_2_START_ITEM_NUM, wWord);
	}
	WriteLog(_T("WriteData End"));
}

void CDialogInline::WriteData(int idx, BOOL nBit, int type)
{
	WriteLog(_T("WriteData(BIT) Start"));
	if(type == 2)
	{
		SetValue(idx + AREA_BIT_1_START_ITEM_NUM, nBit);
	}
	WriteLog(_T("WriteData(BIT) End"));
}

LRESULT CDialogInline::OnInLineCtrlMessage(WPARAM para0, LPARAM para1)
{
	int nEvent = (int)para0;
	int idx = (int)para1;
	CString temp1, temp2;
	//if(m_bOpened)
	{
		switch(nEvent)
		{
		case MSG_READ_ADDRS:
			if(m_MelsecListPara[idx].nAddrType == ADDRESS_TYPE_WORD_AREA1 || m_MelsecListPara[idx].nAddrType == ADDRESS_TYPE_WORD_AREA2)
			{
				temp1.Format(_T("MSG_READ_WORD->Item = %S, Value = "), m_MelsecListPara[idx].strName);
				//Word
				for(int i = 0; i < m_MelsecListPara[idx].nSize; i++)
				{
					m_MelsecListPara[idx + i].wValue = m_PLCComm->GetWORD(m_MelsecListPara[idx + i].nAddr);
					temp2.Format(_T("%s, %08d"), temp1, m_MelsecListPara[idx].wValue);
					temp1 = temp2;
				}
				if((idx < DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_1_15) || (DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_992_999 < idx))
				{
					m_strLogString.Format(_T("%s"), temp1);
					WriteLog(m_strLogString);
				}
			}
			else if(m_MelsecListPara[idx].nAddrType == ADDRESS_TYPE_BIT_AREA1)
			{
				//Bit
				m_MelsecListPara[idx].bValue = m_PLCComm->GetBit(m_MelsecListPara[idx].nAddr);
				m_strLogString.Format(_T("MSG_READ->Item = %S, Value = %d"), m_MelsecListPara[idx].strName, m_MelsecListPara[idx].bValue);
				WriteLog(m_strLogString);
			}
			
			break;
		case MSG_WRITE_ADDRS:
			if(m_MelsecListPara[idx].nAddrType == ADDRESS_TYPE_WORD_AREA1 || m_MelsecListPara[idx].nAddrType == ADDRESS_TYPE_WORD_AREA2)
			{
				temp1.Format(_T("MSG_WRITE_WORD->Item = %S, Value = "), m_MelsecListPara[idx].strName);
				//Word
				for(int i = 0; i < m_MelsecListPara[idx].nSize; i++)
				{
					m_PLCComm->SetWORD(m_MelsecListPara[idx + i].nAddr, m_MelsecListPara[idx + i].wValue);
					temp2.Format(_T("%s, %08d"), temp1, m_MelsecListPara[idx].wValue);
					temp1 = temp2;
				}
				if((idx < DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_1_15) || (DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_992_999 < idx))
				{
					m_strLogString.Format(_T("%s"), temp1);
					WriteLog(m_strLogString);
				}
			}
			else if(m_MelsecListPara[idx].nAddrType == ADDRESS_TYPE_BIT_AREA1)
			{
				//Bit
				m_PLCComm->SetBit(m_MelsecListPara[idx].nAddr, m_MelsecListPara[idx].bValue);

				//Off 동작
				if(m_MelsecListPara[idx].bValue)
				{
					m_nTime = 0;
					switch(idx)
					{
					case DEVB_LIGHT_ALARM_REPORT://DEVB_LIGHT_ALARM_REPORT off
						SetTimer(500 + (idx - AREA_BIT_1_START_ITEM_NUM), m_nTrigger[TRIGGER_LIGHT_ALARM], NULL);
						break;
					case DEVB_HEAVY_ALARM_REPORT://DEVB_HEAVY_ALARM_REPORT off
						SetTimer(500 + (idx - AREA_BIT_1_START_ITEM_NUM), m_nTrigger[TRIGGER_HEAVY_ALARM], NULL);
						break;
					case DEVB_EQP_RECIPE_LIST_CHANGE://DEVB_EQP_RECIPE_LIST_CHANGE off
						SetTimer(500 + (idx - AREA_BIT_1_START_ITEM_NUM), m_nTrigger[TRIGGER_RECIPE_NUMBER_LIST_CHANGE], NULL);
						break;
					case DEVB_EQIPMENT_RECIPE_REQUEST://DEVB_EQIPMENT_RECIPE_REQUEST off
						SetTimer(500 + (idx - AREA_BIT_1_START_ITEM_NUM), 100, NULL);
						break;
					case DEVB_RECIPE_VARIOUS_REQUEST_CONFIRM://DEVB_RECIPE_VARIOUS_REQUEST_CONFIRM off
						SetTimer(500 + (idx - AREA_BIT_1_START_ITEM_NUM), 100, NULL);
						break;
					case DEVB_RECIPE_VARIOUS_DATA_REPORT://DEVB_RECIPE_VARIOUS_DATA_REPORT off
						SetTimer(500 + (idx - AREA_BIT_1_START_ITEM_NUM), 100, NULL);
						break;
					case DEVB_SCRAP_GLASS_DATA_REPORT://DEVB_SCRAP_GLASS_DATA_REPORT off
						SetTimer(500 + (idx - AREA_BIT_1_START_ITEM_NUM), m_nTrigger[TRIGGER_SCRAP_GLASS_DATA_REPORT], NULL);
						break;
					case DEVB_GLASS_DATA_CHANGE_REPORT://DEVB_GLASS_DATA_CHANGE_REPORT off
						SetTimer(500 + (idx - AREA_BIT_1_START_ITEM_NUM), m_nTrigger[TRIGGER_GLASS_DATA_CHANGE_REPORT], NULL);
						break;
					case DEVB_GLASS_APD_DATA_REPORT://DEVB_GLASS_APD_DATA_REPORT off
						SetTimer(500 + (idx - AREA_BIT_1_START_ITEM_NUM), m_nTrigger[TRIGGER_GLASS_APD_DATA_REPORT], NULL);
						break;
					}
				}
				m_strLogString.Format(_T("MSG_WRITE_BIT->Item = %S, Value = %d"), m_MelsecListPara[idx].strName, m_MelsecListPara[idx].bValue);
				WriteLog(m_strLogString);
			}
			
			break;
		}
	}
	
	
	return 0;
}

void CDialogInline::SetGlassLdUldEx(int type)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	WriteLog(_T("SetGlassLdUldEx Start"));
	m_iSeqIdx= 10;
	m_iTimeout100mm = 0;
	m_nSeqDelay100mm = 0;

	CDialogMSG dlg;
	CString str;
	if(pdlg->m_Robot->GetRobotPos_mm(MOTOR_1) != 0 && type != 3)
	{
		str.Format(_T("Shift Axie Waite Error\n대기위치(홈위치)로 이동 바랍니다."));
		dlg.SetCaption(str);
		dlg.DoModal();
		return;
	}

	if(type == 0)
	{
		InitSeqBit(LOAD_ONLY);
		SetTimer(TIMER_INTERLOCK_LOAD_ONLY, 100, NULL);
	}
	else if(type == 1)
	{
		InitSeqBit(UNLOAD_ONLY);
		SetTimer(TIMER_INTERLOCK_UNLOAD_ONLY, 100, NULL);
	}
	else if(type == 2)
	{
		InitSeqBit(EXCHANGE);
		SetTimer(TIMER_INTERLOCK_EXCHANGE, 100, NULL);
	}
	else if(type == 3)
	{
		InitSeqBit(DOWN_ALARM);
		KillTimer(TIMER_INTERLOCK_LOAD_ONLY);
		KillTimer(TIMER_INTERLOCK_UNLOAD_ONLY);
		KillTimer(TIMER_INTERLOCK_EXCHANGE);
		WriteLog(_T("InterLock Seq Stop"));
	}
	WriteLog(_T("SetGlassLdUldEx End"));
}

void CDialogInline::ClickBtnenhctrlInlineLoaderGlassRead()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SetGlassLdUldEx(0);
}

void CDialogInline::ClickBtnenhctrlInlineLoaderGlassRead2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	typeEQPStatusDataPara eq_sts;
	eq_sts.bGlassExistInUnit = 1;		//Glass유무
	eq_sts.bGlassInProcess = FALSE;							//검사중인지
	eq_sts.bIonizer = TRUE;
	eq_sts.bUnitAutoMode = TRUE;							//자동반출 가능한지			
	eq_sts.nCurrenRecipeNum = 1;
	if(eq_sts.bGlassExistInUnit)
	{
		eq_sts.nGlassCountInUnit = 0;
		eq_sts.nPutInfoPossibleCount = 1;
	}
	else
	{
		eq_sts.nGlassCountInUnit = 1;
		eq_sts.nPutInfoPossibleCount = 0;
	}

	//20160204 ngh
	eq_sts.nStatusRunIdleDown = !eq_sts.bGlassExistInUnit;							//0 : RUN~
	eq_sts.nUnitOpeationMode = 1;							//0 : Manual, 1 : auto
	SetStatus(eq_sts);

	SetGlassLdUldEx(1);
}

CString CDialogInline::GetTime()
{
	return m_strECSTime;
}

void CDialogInline::ReadLostGlassData()
{
	WriteLog(_T("ReadLostGlassData Start"));
	WORD *word;
	char strTemp[3];
	CString temp1, temp2;
	int idx;
	temp1.Empty();
	temp2.Empty();
	word = new WORD[48];
	memset(word, NULL, sizeof(WORD) * 48);
	
	if(m_bOpened)
		m_PLCComm->Read(DevW, ECS_W_ADDRESS_GLASS_DATA, ECS_W_ADDRESS_GLASS_DATA + 47, word, GLASS_DATA_SIZE);
	
	for(int i = 0; i < 48; i++)
	{
		if(m_bOpened)
			word[i] = m_PLCComm->GetWORD(ECS_W_ADDRESS_GLASS_DATA + i);

		m_wUnloaderGlassData[i] = word[i];//m_wUnloaderGlassData[i];//word[i];
		//word[i] = m_wLoaderGlassData[i];
	}


	idx = 0;
	memset(strTemp, NULL, sizeof(char) * 3);
	temp2.Format(_T(""));
	for(int i = idx; i < 8; i++)
	{
		if(word[i] == 0)
		{
			break;
		}
		WordToString(word[i], strTemp);
		if(strTemp[0] == 0x20)
		{
			sprintf(m_LostGlassData.strLotID, "%S%c", temp2, strTemp[1]);
			temp2.Format(_T("%S"), m_LostGlassData.strLotID);
		}
		else if(strTemp[1] == 0x20)
		{
			sprintf(m_LostGlassData.strLotID, "%S%c", temp2, strTemp[0]);
			temp2.Format(_T("%S"), m_LostGlassData.strLotID);
		}
		else if(strTemp[0] == 0x20 && strTemp[1] == 0x20)
		{

		}
		else
		{
			sprintf(m_LostGlassData.strLotID, "%S%s", temp2, strTemp);
			temp2.Format(_T("%S"), m_LostGlassData.strLotID);
		}

		/*else
		{
			temp1.Format(_T("%s%c%c"), temp2, strTemp[0], strTemp[1]);
		}
		temp2.Format(_T("%s"), temp1);	*/
	}
	//sprintf(m_LostGlassData.strLotID, "%S", temp2);

	idx = 8;
	memset(strTemp, NULL, sizeof(char) * 3);
	temp2.Format(_T(""));
	for(int i = idx; i < idx + 4; i++)
	{
		if(word[i] == 0)
		{
			break;
		}
		WordToString(word[i], strTemp);
		if(strTemp[0] == 0x20)
		{
			sprintf(m_LostGlassData.strProcessingCode, "%S%c", temp2, strTemp[1]);
			temp2.Format(_T("%S"), m_LostGlassData.strProcessingCode);
		}
		else if(strTemp[1] == 0x20)
		{
			sprintf(m_LostGlassData.strProcessingCode, "%S%c", temp2, strTemp[0]);
			temp2.Format(_T("%S"), m_LostGlassData.strProcessingCode);
		}
		else if(strTemp[0] == 0x20 && strTemp[1] == 0x20)
		{

		}
		else
		{
			sprintf(m_LostGlassData.strProcessingCode, "%S%s", temp2, strTemp);
			temp2.Format(_T("%S"), m_LostGlassData.strProcessingCode);
		}
	}

	idx = 12;
	memset(strTemp, NULL, sizeof(char) * 3);
	temp2.Format(_T(""));
	for(int i = idx; i < idx + 4; i++)
	{
		if(word[i] == 0)
		{
			sprintf(m_LostGlassData.strLotSpecificData, "%c", word[i]);
			break;
		}
		WordToString(word[i], strTemp);
		if(strTemp[0] == 0x20)
		{
			sprintf(m_LostGlassData.strLotSpecificData, "%S%c", temp2, strTemp[1]);
			temp2.Format(_T("%S"), m_LostGlassData.strLotSpecificData);
		}
		else if(strTemp[1] == 0x20)
		{
			sprintf(m_LostGlassData.strLotSpecificData, "%S%c", temp2, strTemp[0]);
			temp2.Format(_T("%S"), m_LostGlassData.strLotSpecificData);
		}
		else if(strTemp[0] == 0x20 && strTemp[1] == 0x20)
		{

		}
		else
		{
			sprintf(m_LostGlassData.strLotSpecificData, "%S%s", temp2, strTemp);
			temp2.Format(_T("%S"), m_LostGlassData.strLotSpecificData);
		}
	}

	idx = 16;
	m_LostGlassData.nHostRecipeNumber = word[idx];

	idx = 17;
	m_LostGlassData.nGlassType = word[idx];

	idx = 18;
	m_LostGlassData.nGlassCode = word[idx];

	idx = 19;
	memset(strTemp, NULL, sizeof(char) * 3);
	temp2.Format(_T(""));
	for(int i = idx; i < idx + 8; i++)
	{
		WordToString(word[i], strTemp);
		if(strTemp[0] == 0x20)
		{
			sprintf(m_LostGlassData.strGlassID, "%S%c", temp2, strTemp[1]);
			temp2.Format(_T("%S"), m_LostGlassData.strGlassID);
		}
		else if(strTemp[1] == 0x20)
		{
			sprintf(m_LostGlassData.strGlassID, "%S%c", temp2, strTemp[0]);
			temp2.Format(_T("%S"), m_LostGlassData.strGlassID);
		}
		else if(strTemp[0] == 0x20 && strTemp[1] == 0x20)
		{

		}
		else
		{
			sprintf(m_LostGlassData.strGlassID, "%S%s", temp2, strTemp);
			temp2.Format(_T("%S"), m_LostGlassData.strGlassID);
		}
	}

	idx = 27;
	memset(strTemp, NULL, sizeof(char) * 3);
	WordToString(word[idx], strTemp);
	temp2.Format(_T(""));
	if(strTemp[0] == 0x20)
	{
		sprintf(m_LostGlassData.strGlassJudge, "%S%c", temp2, strTemp[1]);
		temp2.Format(_T("%S"), m_LostGlassData.strGlassJudge);
	}
	else if(strTemp[1] == 0x20)
	{
		sprintf(m_LostGlassData.strGlassJudge, "%S%c", temp2, strTemp[0]);
		temp2.Format(_T("%S"), m_LostGlassData.strGlassJudge);
	}
	else if(strTemp[0] == 0x20 && strTemp[1] == 0x20)
	{

	}
	else
	{
		sprintf(m_LostGlassData.strGlassJudge, "%S%s", temp2, strTemp);
		temp2.Format(_T("%S"), m_LostGlassData.strGlassJudge);
	}

	idx = 28;
	for(int i = idx; i < idx + 4; i++)
	{
		m_LostGlassData.nGlassSpecificData[i - idx] = word[i];	
	}

	idx = 32;
	for(int i = idx; i < idx + 6; i++)
	{
		m_LostGlassData.nGlassAddData[i - idx] = word[i];	
	}

	idx = 38;
	for(int i = idx; i < idx + 4; i++)
	{
		m_LostGlassData.nPreviousUnitProcessingData[i - idx] = word[i];	
	}

	idx = 42;
	for(int i = idx; i < idx + 3; i++)
	{
		m_LostGlassData.nGlassProcessingStatus[i - idx] = word[i];	
	}

	idx = 45;
	for(int i = idx; i < idx + 3; i++)
	{
		m_LostGlassData.nGlassRoutingData[i - idx] = word[i];	
	}
	temp1.Format(_T("Read Glass Data -> Glass ID : %S, Glass Code : %d"), m_LostGlassData.strGlassID, m_LostGlassData.nGlassCode);
	WriteLog(temp1);
	WriteLog(_T("ReadLostGlassData End"));
}

void CDialogInline::ReadLoaderGlassData()
{
	WriteLog(_T("ReadLoaderGlassData Start"));
	WORD *word;
	char strTemp[3];
	CString temp1, temp2;
	int idx;
	temp1.Empty();
	temp2.Empty();
	word = new WORD[48];
	memset(word, NULL, sizeof(WORD) * 48);
	
	if(m_bOpened)
		m_PLCComm->Read(DevW, ROBOT_W_ADDRESS_LOADER_GLASS_DATA, ROBOT_W_ADDRESS_LOADER_GLASS_DATA + 47, word, GLASS_DATA_SIZE);
	
	for(int i = 0; i < 48; i++)
	{
		if(m_bOpened)
			word[i] = m_PLCComm->GetWORD(ROBOT_W_ADDRESS_LOADER_GLASS_DATA + i);
		//
		m_wLoaderGlassData[i] = word[i];//m_wUnloaderGlassData[i];//word[i];
		//word[i] = m_wLoaderGlassData[i];
	}

	idx = 0;
	memset(strTemp, NULL, sizeof(char) * 3);
	temp2.Format(_T(""));
	for(int i = idx; i < 8; i++)
	{
		if(word[i] == 0)
		{
			break;
		}
		WordToString(word[i], strTemp);
		if(strTemp[0] == 0x20)
		{
			sprintf(m_LoaderGlassData.strLotID, "%S%c", temp2, strTemp[1]);
			temp2.Format(_T("%S"), m_LoaderGlassData.strLotID);
		}
		else if(strTemp[1] == 0x20)
		{
			sprintf(m_LoaderGlassData.strLotID, "%S%c", temp2, strTemp[0]);
			temp2.Format(_T("%S"), m_LoaderGlassData.strLotID);
		}
		else if(strTemp[0] == 0x20 && strTemp[1] == 0x20)
		{

		}
		else if(strTemp[0] == 0x00 && strTemp[1] == 0x00)
		{
			break;
		}
		else
		{
			sprintf(m_LoaderGlassData.strLotID, "%S%s", temp2, strTemp);
			temp2.Format(_T("%S"), m_LoaderGlassData.strLotID);
		}

		/*else
		{
			temp1.Format(_T("%s%c%c"), temp2, strTemp[0], strTemp[1]);
		}
		temp2.Format(_T("%s"), temp1);	*/
	}
	//sprintf(m_LoaderGlassData.strLotID, "%S", temp2);

	idx = 8;
	memset(strTemp, NULL, sizeof(char) * 3);
	temp2.Format(_T(""));
	for(int i = idx; i < idx + 4; i++)
	{
		if(word[i] == 0)
		{
			break;
		}
		WordToString(word[i], strTemp);
		if(strTemp[0] == 0x20)
		{
			sprintf(m_LoaderGlassData.strProcessingCode, "%S%c", temp2, strTemp[1]);
			temp2.Format(_T("%S"), m_LoaderGlassData.strProcessingCode);
		}
		else if(strTemp[1] == 0x20)
		{
			sprintf(m_LoaderGlassData.strProcessingCode, "%S%c", temp2, strTemp[0]);
			temp2.Format(_T("%S"), m_LoaderGlassData.strProcessingCode);
		}
		else if(strTemp[0] == 0x20 && strTemp[1] == 0x20)
		{

		}
		else if(strTemp[0] == 0x00 && strTemp[1] == 0x00)
		{
			break;
		}
		else
		{
			sprintf(m_LoaderGlassData.strProcessingCode, "%S%s", temp2, strTemp);
			temp2.Format(_T("%S"), m_LoaderGlassData.strProcessingCode);
		}
	}

	idx = 12;
	memset(strTemp, NULL, sizeof(char) * 3);
	temp2.Format(_T(""));
	for(int i = idx; i < idx + 4; i++)
	{
		if(word[i] == 0)
		{
			sprintf(m_LoaderGlassData.strLotSpecificData, "%c", word[i]);
			break;
		}
		WordToString(word[i], strTemp);
		if(strTemp[0] == 0x20)
		{
			sprintf(m_LoaderGlassData.strLotSpecificData, "%S%c", temp2, strTemp[1]);
			temp2.Format(_T("%S"), m_LoaderGlassData.strLotSpecificData);
		}
		else if(strTemp[1] == 0x20)
		{
			sprintf(m_LoaderGlassData.strLotSpecificData, "%S%c", temp2, strTemp[0]);
			temp2.Format(_T("%S"), m_LoaderGlassData.strLotSpecificData);
		}
		else if(strTemp[0] == 0x20 && strTemp[1] == 0x20)
		{

		}
		else
		{
			sprintf(m_LoaderGlassData.strLotSpecificData, "%S%s", temp2, strTemp);
			temp2.Format(_T("%S"), m_LoaderGlassData.strLotSpecificData);
		}
	}

	idx = 16;
	m_LoaderGlassData.nHostRecipeNumber = word[idx];

	idx = 17;
	m_LoaderGlassData.nGlassType = word[idx];

	idx = 18;
	m_LoaderGlassData.nGlassCode = word[idx];

	idx = 19;
	memset(strTemp, NULL, sizeof(char) * 3);
	temp2.Format(_T(""));
	for(int i = idx; i < idx + 8; i++)
	{
		WordToString(word[i], strTemp);
		if(strTemp[0] == 0x20)
		{
			sprintf(m_LoaderGlassData.strGlassID, "%S%c", temp2, strTemp[1]);
			temp2.Format(_T("%S"), m_LoaderGlassData.strGlassID);
		}
		else if(strTemp[1] == 0x20)
		{
			sprintf(m_LoaderGlassData.strGlassID, "%S%c", temp2, strTemp[0]);
			temp2.Format(_T("%S"), m_LoaderGlassData.strGlassID);
		}
		else if(strTemp[0] == 0x20 && strTemp[1] == 0x20)
		{

		}
		else if(strTemp[0] == 0x00 && strTemp[1] == 0x00)
		{
			break;
		}
		else
		{
			sprintf(m_LoaderGlassData.strGlassID, "%S%s", temp2, strTemp);
			temp2.Format(_T("%S"), m_LoaderGlassData.strGlassID);
		}
	}

	idx = 27;
	memset(strTemp, NULL, sizeof(char) * 3);
	WordToString(word[idx], strTemp);
	temp2.Format(_T(""));
	if(strTemp[0] == 0x20)
	{
		sprintf(m_LoaderGlassData.strGlassJudge, "%S%c", temp2, strTemp[1]);
		temp2.Format(_T("%S"), m_LoaderGlassData.strGlassJudge);
	}
	else if(strTemp[1] == 0x20)
	{
		sprintf(m_LoaderGlassData.strGlassJudge, "%S%c", temp2, strTemp[0]);
		temp2.Format(_T("%S"), m_LoaderGlassData.strGlassJudge);
	}
	else if(strTemp[0] == 0x20 && strTemp[1] == 0x20)
	{

	}
	else
	{
		sprintf(m_LoaderGlassData.strGlassJudge, "%S%s", temp2, strTemp);
		temp2.Format(_T("%S"), m_LoaderGlassData.strGlassJudge);
	}

	idx = 28;
	for(int i = idx; i < idx + 4; i++)
	{
		m_LoaderGlassData.nGlassSpecificData[i - idx] = word[i];	
	}

	idx = 32;
	for(int i = idx; i < idx + 6; i++)
	{
		m_LoaderGlassData.nGlassAddData[i - idx] = word[i];	
	}

	idx = 38;
	for(int i = idx; i < idx + 4; i++)
	{
		m_LoaderGlassData.nPreviousUnitProcessingData[i - idx] = word[i];	
	}

	idx = 42;
	for(int i = idx; i < idx + 3; i++)
	{
		m_LoaderGlassData.nGlassProcessingStatus[i - idx] = word[i];	
	}

	idx = 45;
	for(int i = idx; i < idx + 3; i++)
	{
		m_LoaderGlassData.nGlassRoutingData[i - idx] = word[i];	
	}
	//m_UnloaderGlassData = m_LoaderGlassData;
	m_UpdateGlassData = m_LoaderGlassData;
	SaveTemp();
	temp1.Format(_T("Read Glass Data -> Glass ID : %S, Glass Code : %d, HostRecipe Num : %d"), m_LoaderGlassData.strGlassID, m_LoaderGlassData.nGlassCode, m_LoaderGlassData.nHostRecipeNumber);
	WriteLog(temp1);
	WriteLog(_T("ReadLoaderGlassData End"));
}

void CDialogInline::ReadLoaderGlassData(typeGlassDataPara *glassData)
{
	WriteLog(_T("ReadLoaderGlassData Start"));
	WORD *word;
	char strTemp[3];
	CString temp1, temp2;
	int idx;
	temp1.Empty();
	temp2.Empty();
	word = new WORD[48];
	memset(word, NULL, sizeof(WORD) * 48);
	
	if(m_bOpened)
		m_PLCComm->Read(DevW, ROBOT_W_ADDRESS_LOADER_GLASS_DATA, ROBOT_W_ADDRESS_LOADER_GLASS_DATA + 47, word, GLASS_DATA_SIZE);
	
	for(int i = 0; i < 48; i++)
	{
		if(m_bOpened)
			word[i] = m_PLCComm->GetWORD(ROBOT_W_ADDRESS_LOADER_GLASS_DATA + i);
	}

	idx = 0;
	memset(strTemp, NULL, sizeof(char) * 3);
	temp2.Format(_T(""));
	for(int i = idx; i < 8; i++)
	{
		if(word[i] == 0)
		{
			break;
		}
		WordToString(word[i], strTemp);
		if(strTemp[0] == 0x20)
		{
			sprintf(glassData->strLotID, "%S%c", temp2, strTemp[1]);
			temp2.Format(_T("%S"), glassData->strLotID);
		}
		else if(strTemp[1] == 0x20)
		{
			sprintf(glassData->strLotID, "%S%c", temp2, strTemp[0]);
			temp2.Format(_T("%S"), glassData->strLotID);
		}
		else if(strTemp[0] == 0x20 && strTemp[1] == 0x20)
		{

		}
		else
		{
			sprintf(glassData->strLotID, "%S%s", temp2, strTemp);
			temp2.Format(_T("%S"), glassData->strLotID);
		}

		/*else
		{
			temp1.Format(_T("%s%c%c"), temp2, strTemp[0], strTemp[1]);
		}
		temp2.Format(_T("%s"), temp1);	*/
	}
	//sprintf(glassData->strLotID, "%S", temp2);

	idx = 8;
	memset(strTemp, NULL, sizeof(char) * 3);
	temp2.Format(_T(""));
	for(int i = idx; i < idx + 4; i++)
	{
		if(word[i] == 0)
		{
			break;
		}
		WordToString(word[i], strTemp);
		if(strTemp[0] == 0x20)
		{
			sprintf(glassData->strProcessingCode, "%S%c", temp2, strTemp[1]);
			temp2.Format(_T("%S"), glassData->strProcessingCode);
		}
		else if(strTemp[1] == 0x20)
		{
			sprintf(glassData->strProcessingCode, "%S%c", temp2, strTemp[0]);
			temp2.Format(_T("%S"), glassData->strProcessingCode);
		}
		else if(strTemp[0] == 0x20 && strTemp[1] == 0x20)
		{

		}
		else
		{
			sprintf(glassData->strProcessingCode, "%S%s", temp2, strTemp);
			temp2.Format(_T("%S"), glassData->strProcessingCode);
		}
	}

	idx = 12;
	memset(strTemp, NULL, sizeof(char) * 3);
	temp2.Format(_T(""));
	for(int i = idx; i < idx + 4; i++)
	{
		if(word[i] == 0)
		{
			sprintf(glassData->strLotSpecificData, "%c", word[i]);
			break;
		}
		WordToString(word[i], strTemp);
		if(strTemp[0] == 0x20)
		{
			sprintf(glassData->strLotSpecificData, "%S%c", temp2, strTemp[1]);
			temp2.Format(_T("%S"), glassData->strLotSpecificData);
		}
		else if(strTemp[1] == 0x20)
		{
			sprintf(glassData->strLotSpecificData, "%S%c", temp2, strTemp[0]);
			temp2.Format(_T("%S"), glassData->strLotSpecificData);
		}
		else if(strTemp[0] == 0x20 && strTemp[1] == 0x20)
		{

		}
		else
		{
			sprintf(glassData->strLotSpecificData, "%S%s", temp2, strTemp);
			temp2.Format(_T("%S"), glassData->strLotSpecificData);
		}
	}

	idx = 16;
	glassData->nHostRecipeNumber = word[idx];

	idx = 17;
	glassData->nGlassType = word[idx];

	idx = 18;
	glassData->nGlassCode = word[idx];

	idx = 19;
	memset(strTemp, NULL, sizeof(char) * 3);
	temp2.Format(_T(""));
	for(int i = idx; i < idx + 8; i++)
	{
		WordToString(word[i], strTemp);
		if(strTemp[0] == 0x20)
		{
			sprintf(glassData->strGlassID, "%S%c", temp2, strTemp[1]);
			temp2.Format(_T("%S"), glassData->strGlassID);
		}
		else if(strTemp[1] == 0x20)
		{
			sprintf(glassData->strGlassID, "%S%c", temp2, strTemp[0]);
			temp2.Format(_T("%S"), glassData->strGlassID);
		}
		else if(strTemp[0] == 0x20 && strTemp[1] == 0x20)
		{

		}
		else
		{
			sprintf(glassData->strGlassID, "%S%s", temp2, strTemp);
			temp2.Format(_T("%S"), glassData->strGlassID);
		}
	}

	idx = 27;
	memset(strTemp, NULL, sizeof(char) * 3);
	WordToString(word[idx], strTemp);
	temp2.Format(_T(""));
	if(strTemp[0] == 0x20)
	{
		sprintf(glassData->strGlassJudge, "%S%c", temp2, strTemp[1]);
		temp2.Format(_T("%S"), glassData->strGlassJudge);
	}
	else if(strTemp[1] == 0x20)
	{
		sprintf(glassData->strGlassJudge, "%S%c", temp2, strTemp[0]);
		temp2.Format(_T("%S"), glassData->strGlassJudge);
	}
	else if(strTemp[0] == 0x20 && strTemp[1] == 0x20)
	{

	}
	else
	{
		sprintf(glassData->strGlassJudge, "%S%s", temp2, strTemp);
		temp2.Format(_T("%S"), glassData->strGlassJudge);
	}

	idx = 28;
	for(int i = idx; i < idx + 4; i++)
	{
		glassData->nGlassSpecificData[i - idx] = word[i];	
	}

	idx = 32;
	for(int i = idx; i < idx + 6; i++)
	{
		glassData->nGlassAddData[i - idx] = word[i];	
	}

	idx = 38;
	for(int i = idx; i < idx + 4; i++)
	{
		glassData->nPreviousUnitProcessingData[i - idx] = word[i];	
	}

	idx = 42;
	for(int i = idx; i < idx + 3; i++)
	{
		glassData->nGlassProcessingStatus[i - idx] = word[i];	
	}

	idx = 45;
	for(int i = idx; i < idx + 3; i++)
	{
		glassData->nGlassRoutingData[i - idx] = word[i];	
	}

	//SaveTemp();
	temp1.Format(_T("Read Glass Data -> Glass ID : %S, Glass Code : %d"), m_LoaderGlassData.strGlassID, m_LoaderGlassData.nGlassCode);
	WriteLog(temp1);
	WriteLog(_T("ReadLoaderGlassData End"));
}

void CDialogInline::WriteReportGlassData(int type)
{
	WriteLog(_T("WriteReportGlassData Start"));
	WORD *word;
	int idx = 0;
	word = new WORD[16];
	memset(word, NULL, sizeof(WORD) * 16);

	if(type == 0)
	{
		word[idx++] = 2;
		word[idx++] = 9100;
		word[idx++] = 9200;
	}
	else if(type == 1)
	{
		word[idx++] = 1;
	}
	else if(type == 7)
	{
		word[idx++] = 7;
	}

	if(m_bOpened)
		m_PLCComm->Write(DevW, PLC_DEVW_RP_REPORT_PARAMETER_CEID, PLC_DEVW_RP_REPORT_PARAMETER_CEID + 16, word, 16);

	WriteLog(_T("WriteReportGlassData End"));
}

void CDialogInline::WriteChangeGlassData()
{
	WriteLog(_T("WriteChangeGlassData Start"));
	//2110 ~ 213f
	WORD *word;
	WORD tempWord;
	BOOL bit[16];
	char strTemp[3];
	char temp3[256];
	CString temp1, temp2;
	int idx = 0;
	temp1.Empty();
	temp2.Empty();
	word = new WORD[48];
	memset(word, NULL, sizeof(WORD) * 48);
	memset(bit, NULL, sizeof(BOOL) * 16);

	memset(strTemp, NULL, sizeof(char) * 3);
	memset(temp3, NULL, sizeof(char) * 256);

	for(int i = 0; i < 16; i++)
	{
		temp1.Format(_T("%s%c"), temp2, m_ChangeGlassData.strLotID[i]);

		if(i % 2 != 0)
		{
			sprintf(strTemp, "%S", temp1);
			StringToWord(strTemp, &tempWord);
			word[idx++] = tempWord;
			temp2.Format(_T("%S"), strTemp);
			temp1.Empty();
		}
	}

	idx = 8;
	memset(strTemp, NULL, sizeof(char) * 3);
	for(int i = 0; i < 8; i++)
	{
		temp1.Format(_T("%s%c"), temp1, m_ChangeGlassData.strProcessingCode[i]);

		if(i % 2 != 0)
		{
			sprintf(strTemp, "%S", temp1);
			StringToWord(strTemp, &tempWord);
			word[idx++] = tempWord;
			temp1.Empty();
		}
	}

	idx = 12;
	memset(strTemp, NULL, sizeof(char) * 3);
	for(int i = 0; i < 8; i++)
	{
		temp1.Format(_T("%s%c"), temp1, m_ChangeGlassData.strLotSpecificData[i]);

		if(i % 2 != 0)
		{
			sprintf(strTemp, "%S", temp1);
			StringToWord(strTemp, &tempWord);
			word[idx++] = tempWord;
			temp1.Empty();
		}
	}
	idx = 16;
	word[idx++] = m_ChangeGlassData.nHostRecipeNumber;

	WordToBit(m_ChangeGlassData.nGlassType, bit);
	bit[15] = TRUE;
	WORD wTemp = 0;
	BitToWord(bit, &wTemp);
	word[idx++] = wTemp;

	word[idx++] = m_ChangeGlassData.nGlassCode;

	idx = 19;
	memset(strTemp, NULL, sizeof(char) * 3);
	for(int i = 0; i < 16; i++)
	{
		temp1.Format(_T("%s%c"), temp1, m_ChangeGlassData.strGlassID[i]);

		if(i % 2 != 0)
		{
			sprintf(strTemp, "%S", temp1);
			StringToWord(strTemp, &tempWord);
			word[idx++] = tempWord;
			temp1.Empty();
		}
	}

	idx = 27;
	memset(strTemp, NULL, sizeof(char) * 3);

	for(int i = 0; i < 2; i++)
	{
		temp1.Format(_T("%s%c"), temp1, m_ChangeGlassData.strGlassJudge[i]);

		if(i % 2 != 0)
		{
			sprintf(strTemp, "%S", temp1);
			StringToWord(strTemp, &tempWord);
			word[idx++] = tempWord;
			temp1.Empty();
		}
	}

	idx = 29;
	for(int i = 0; i < 4; i++)
	{
		word[i + idx++] = m_ChangeGlassData.nGlassSpecificData[i];	
	}

	for(int i = 0; i < 6; i++)
	{
		word[i + idx++] = m_ChangeGlassData.nGlassAddData[i];	
	}

	for(int i = 0; i < 4; i++)
	{
		word[i + idx++] = m_ChangeGlassData.nPreviousUnitProcessingData[i];	
	}

	for(int i = 0; i < 3; i++)
	{
		word[i + idx++] = m_ChangeGlassData.nGlassProcessingStatus[i];	
	}

	for(int i = 0; i < 3; i++)
	{
		word[i + idx++] = m_ChangeGlassData.nGlassRoutingData[i];	
	}

	for(int i = 0; i < 48; i++)
	{
		m_wScrapGlassData[i] = word[i];
	}

	if(m_bOpened)
		m_PLCComm->Write(DevW, PLC_DEVW_RGD_LOT_ID, PLC_DEVW_RGD_LOT_ID + 47, word, GLASS_DATA_SIZE);

	WriteLog(_T("WriteChangeGlassData End"));
}

void CDialogInline::SetLostGlassDataReport(typeLostGlassDataPara lostGlass)
{
	m_ReportLostGlassData = lostGlass;
}

void CDialogInline::WriteLostGlassData(int option)
{
	WriteLog(_T("WriteLostGlassData Start"));
	//2070 ~ 2079
	WORD *word;
	WORD tempWord;
	char *strTemp;
	//char *temp3;
	strTemp = new char[3];
	//temp3 = new char[256];
	CString temp1, temp2;
	word = new WORD[10];
	memset(word, NULL, sizeof(WORD) * 10);
	memset(strTemp, NULL, sizeof(char) * 3);
	//memset(temp3, NULL, sizeof(char) * 256);
	int idx = 0;
	temp1.Empty();
	temp2.Empty();
	for(int i = 0; i < 16; i++)
	{
		temp1.Format(_T("%s%c"), temp2, m_ReportLostGlassData.strGlassID[i]);
		temp2 = temp1;
		if(i % 2 != 0)
		{
			sprintf(strTemp, "%S", temp2.Mid(1,2));
			StringToWord(strTemp, &tempWord);
			word[idx++] = tempWord;
			temp2.Empty();
		}
	}
	word[idx++] = m_ReportLostGlassData.nGlassCode;
	word[idx] = option;

	for(int i = 0; i < 10; i++)
	{
		m_wReportLostGlassData[i] = word[i];
	}

	if(m_bOpened)
		m_PLCComm->Write(DevW, PLC_DEVW_GSD_GLASS_DATA_REQUEST_GLASS_ID, PLC_DEVW_GSD_GLASS_DATA_REQUEST_GLASS_ID + 9, word, 10);

	delete strTemp;
	//delete temp3;
	WriteLog(_T("WriteLostGlassData End"));
}

void CDialogInline::WriteScrapGlassData()
{
	WriteLog(_T("WriteScrapGlassData Start"));
	//2110 ~ 213f
	WORD *word;
	WORD tempWord;
	char strTemp[3];
	char temp3[256];
	CString temp1, temp2;
	int idx = 0;
	temp1.Empty();
	temp2.Empty();
	word = new WORD[48];
	memset(word, NULL, sizeof(WORD) * 48);

	memset(strTemp, NULL, sizeof(char) * 3);
	memset(temp3, NULL, sizeof(char) * 256);

	for(int i = 0; i < 16; i++)
	{
		temp1.Format(_T("%s%c"), temp2, m_ScrapGlassData.strLotID[i]);
		temp2 = temp1;
		if(i % 2 != 0)
		{
			sprintf(strTemp, "%S", temp2);
			StringToWord(strTemp, &tempWord);
			word[idx++] = tempWord;
			temp2.Empty();
		}
	}

	idx = 8;
	memset(strTemp, NULL, sizeof(char) * 3);
	for(int i = 0; i < 8; i++)
	{
		temp1.Format(_T("%s%c"), temp2, m_ScrapGlassData.strProcessingCode[i]);
		temp2 = temp1;
		if(i % 2 != 0)
		{
			sprintf(strTemp, "%S", temp2);
			StringToWord(strTemp, &tempWord);
			word[idx++] = tempWord;
			temp2.Empty();
		}
	}

	idx = 12;
	memset(strTemp, NULL, sizeof(char) * 3);
	for(int i = 0; i < 8; i++)
	{
		temp1.Format(_T("%s%c"), temp2, m_ScrapGlassData.strLotSpecificData[i]);
		temp2 = temp1;
		if(i % 2 != 0)
		{
			sprintf(strTemp, "%S", temp2);
			StringToWord(strTemp, &tempWord);
			word[idx++] = tempWord;
			temp2.Empty();
		}
	}
	idx = 16;
	word[idx++] = m_ScrapGlassData.nHostRecipeNumber;

	word[idx++] = m_ScrapGlassData.nGlassType;

	word[idx++] = m_ScrapGlassData.nGlassCode;

	idx = 19;
	memset(strTemp, NULL, sizeof(char) * 3);
	for(int i = 0; i < 16; i++)
	{
		temp1.Format(_T("%s%c"), temp2, m_ScrapGlassData.strGlassID[i]);
		temp2 = temp1;
		if(i % 2 != 0)
		{
			sprintf(strTemp, "%S", temp1);
			StringToWord(strTemp, &tempWord);
			word[idx++] = tempWord;
			temp1.Empty();
		}
	}

	idx = 27;
	memset(strTemp, NULL, sizeof(char) * 3);
	for(int i = 0; i < 2; i++)
	{
		temp1.Format(_T("%s%c"), temp2, m_ScrapGlassData.strGlassJudge[i]);
		temp2 = temp1;
		if(i % 2 != 0)
		{
			sprintf(strTemp, "%S", temp2);
			StringToWord(strTemp, &tempWord);
			word[idx++] = tempWord;
			temp2.Empty();
		}
	}

	idx = 29;
	for(int i = 0; i < 4; i++)
	{
		word[i + idx++] = m_ScrapGlassData.nGlassSpecificData[i];	
	}

	for(int i = 0; i < 6; i++)
	{
		word[i + idx++] = m_ScrapGlassData.nGlassAddData[i];	
	}

	for(int i = 0; i < 4; i++)
	{
		word[i + idx++] = m_ScrapGlassData.nPreviousUnitProcessingData[i];	
	}

	for(int i = 0; i < 3; i++)
	{
		word[i + idx++] = m_ScrapGlassData.nGlassProcessingStatus[i];	
	}

	for(int i = 0; i < 3; i++)
	{
		word[i + idx++] = m_ScrapGlassData.nGlassRoutingData[i];	
	}

	for(int i = 0; i < 48; i++)
	{
		m_wScrapGlassData[i] = word[i];
	}

	if(m_bOpened)
		m_PLCComm->Write(DevW, PLC_DEVW_RGD_LOT_ID, PLC_DEVW_RGD_LOT_ID + 47, word, GLASS_DATA_SIZE);

	WriteLog(_T("WriteScrapGlassData End"));
}

void CDialogInline::WriteUnLoaderGlassData()
{
	WriteLog(_T("WriteUnLoaderGlassData Start"));
	//2110 ~ 213f
	WORD *word;
	WORD tempWord;
	char *strTemp;
	char *temp3;
	strTemp = new char[3];
	temp3 = new char[256];
	CString temp1, temp2;
	int idx = 0;
	temp1.Empty();
	temp2.Empty();
	word = new WORD[48];
	memset(word, NULL, sizeof(WORD) * 48);

	memset(strTemp, NULL, sizeof(char) * 3);
	memset(temp3, NULL, sizeof(char) * 256);

	for(int i = 0; i < 16; i++)
	{
		temp1.Format(_T("%s%c"), temp2, m_UnloaderGlassData.strLotID[i]);
		temp2 = temp1;
		if(i % 2 != 0)
		{
			if(temp2 == "0")
			{
				break;
			}
			sprintf(strTemp, "%S", temp2.Mid(1,2));
			StringToWord(strTemp, &tempWord);
			word[idx++] = tempWord;
			temp2.Empty();
		}
	}

	idx = 8;
	memset(strTemp, NULL, sizeof(char) * 3);
	for(int i = 0; i < 8; i++)
	{
		temp1.Format(_T("%s%c"), temp2, m_UnloaderGlassData.strProcessingCode[i]);
		temp2 = temp1;
		if(i % 2 != 0)
		{
			if(temp2 == "0")
			{
				break;
			}
			sprintf(strTemp, "%S", temp2.Mid(1,2));
			StringToWord(strTemp, &tempWord);
			word[idx++] = tempWord;
			temp2.Empty();
		}
	}

	idx = 12;
	memset(strTemp, NULL, sizeof(char) * 3);
	for(int i = 0; i < 8; i++)
	{
		temp1.Format(_T("%s%c"), temp2, m_UnloaderGlassData.strLotSpecificData[i]);
		temp2 = temp1;
		if(i % 2 != 0)
		{
			if(temp2 == "0")
			{
				break;
			}
			sprintf(strTemp, "%S", temp2.Mid(1,2));
			StringToWord(strTemp, &tempWord);
			word[idx++] = tempWord;
			temp2.Empty();
		}
	}
	idx = 16;
	word[idx++] = m_UnloaderGlassData.nHostRecipeNumber;

	word[idx++] = m_UnloaderGlassData.nGlassType;

	word[idx++] = m_UnloaderGlassData.nGlassCode;

	idx = 19;
	memset(strTemp, NULL, sizeof(char) * 3);
	for(int i = 0; i < 16; i++)
	{
		temp1.Format(_T("%s%c"), temp2, m_UnloaderGlassData.strGlassID[i]);
		temp2 = temp1;
		if(i % 2 != 0)
		{
			if(temp2 == "0")
			{
				break;
			}
			sprintf(strTemp, "%S", temp2.Mid(1,2));
			StringToWord(strTemp, &tempWord);
			word[idx++] = tempWord;
			temp2.Empty();
		}
	}

	idx = 27;
	memset(strTemp, NULL, sizeof(char) * 3);
	for(int i = 0; i < 2; i++)
	{

		temp1.Format(_T("%s%c"), temp2, m_UnloaderGlassData.strGlassJudge[i]);
		temp2 = temp1;
		if(i % 2 != 0)
		{
			if(temp2 == "0")
			{
				break;
			}
			sprintf(strTemp, "%S", temp2.Mid(1,2));
			StringToWord(strTemp, &tempWord);
			word[idx++] = tempWord;
			temp2.Empty();
		}
	}

	idx = 29;
	for(int i = 0; i < 4; i++)
	{
		word[i + idx++] = m_UnloaderGlassData.nGlassSpecificData[i];	
	}

	for(int i = 0; i < 6; i++)
	{
		word[i + idx++] = m_UnloaderGlassData.nGlassAddData[i];	
	}

	for(int i = 0; i < 4; i++)
	{
		word[i + idx++] = m_UnloaderGlassData.nPreviousUnitProcessingData[i];	
	}

	for(int i = 0; i < 3; i++)
	{
		word[i + idx++] = m_UnloaderGlassData.nGlassProcessingStatus[i];	
	}

	for(int i = 0; i < 3; i++)
	{
		word[i + idx++] = m_UnloaderGlassData.nGlassRoutingData[i];	
	}

	for(int i = 0; i < 48; i++)
	{
		m_wUnloaderGlassData[i] = word[i];
	}

	if(m_bOpened)
		m_PLCComm->Write(DevW, PLC_DEVW_UIGD_LOT_ID, PLC_DEVW_UIGD_LOT_ID + 47, word, GLASS_DATA_SIZE);

	//delete strTemp;
	//delete temp3;
	temp1.Format(_T("Write Glass Data -> Glass ID : %S, Glass Code : %d"), m_UnloaderGlassData.strGlassID, m_UnloaderGlassData.nGlassCode);
	WriteLog(temp1);
	WriteLog(_T("WriteUnLoaderGlassData End"));
}

void CDialogInline::GlassDataUpdate(BOOL isUpdate)
{
	if(isUpdate)
	{
		UpdateData(TRUE);
		sprintf(m_UpdateGlassData.strLotID, "%S", m_edit_strLotID);
		sprintf(m_UpdateGlassData.strProcessingCode, "%S", m_edit_strProcessingCode);
		sprintf(m_UpdateGlassData.strLotSpecificData, "%S%S%S%S", m_edit_strLotSpecificData1, m_edit_strLotSpecificData2, m_edit_strLotSpecificData3, m_edit_strLotSpecificData4);
		sprintf(m_UpdateGlassData.strGlassJudge, "%S", m_edit_strGlassJudge);
		sprintf(m_UpdateGlassData.strGlassID, "%S", m_edit_strGlassID);

		m_UpdateGlassData.nHostRecipeNumber = m_edit_nHostRecipeNumber;
		m_UpdateGlassData.nGlassType = m_edit_nGlassType;
		m_UpdateGlassData.GlassCode.nLotNumber = m_edit_nGlassCodeLot; 
		m_UpdateGlassData.nGlassCode = m_edit_nGlassCode;  //pkh
		m_UpdateGlassData.GlassCode.nSlotNumber = m_edit_nGlassCodeSLot;
		
		m_UpdateGlassData.nGlassSpecificData[0] = m_edit_nGlassSpecificData1;
		m_UpdateGlassData.nGlassSpecificData[1] = m_edit_nGlassSpecificData2;
		m_UpdateGlassData.nGlassSpecificData[2] = m_edit_nGlassSpecificData3;
		m_UpdateGlassData.nGlassSpecificData[3] = m_edit_nGlassSpecificData4;
		m_UpdateGlassData.nGlassAddData[0] = m_edit_nGlassAddData1;
		m_UpdateGlassData.nGlassAddData[1] = m_edit_nGlassAddData2;
		m_UpdateGlassData.nGlassAddData[2] = m_edit_nGlassAddData3;
		m_UpdateGlassData.nGlassAddData[3] = m_edit_nGlassAddData4;
		m_UpdateGlassData.nGlassAddData[4] = m_edit_nGlassAddData5;
		m_UpdateGlassData.nGlassAddData[5] = m_edit_nGlassAddData6;
		m_UpdateGlassData.nPreviousUnitProcessingData[0] = m_edit_nPreviousEqpProcessingData1;
		m_UpdateGlassData.nPreviousUnitProcessingData[1] = m_edit_nPreviousEqpProcessingData2;
		m_UpdateGlassData.nPreviousUnitProcessingData[2] = m_edit_nPreviousEqpProcessingData3;
		m_UpdateGlassData.nPreviousUnitProcessingData[3] = m_edit_nPreviousEqpProcessingData4;
		m_UpdateGlassData.nGlassProcessingStatus[0] = m_edit_nGlassProcessingData1;
		m_UpdateGlassData.nGlassProcessingStatus[1] = m_edit_nGlassProcessingData2;
		m_UpdateGlassData.nGlassProcessingStatus[2] = m_edit_nGlassProcessingData3;
		m_UpdateGlassData.nGlassRoutingData[0] = m_edit_nGlassRoutingData1;
		m_UpdateGlassData.nGlassRoutingData[1] = m_edit_nGlassRoutingData2;
		m_UpdateGlassData.nGlassRoutingData[2] = m_edit_nGlassRoutingData3;
	}
	else
	{
		m_edit_strLotID.Format(_T("%S"), m_UpdateGlassData.strLotID);
		m_edit_strProcessingCode.Format(_T("%S"), m_UpdateGlassData.strProcessingCode);
		m_edit_strLotSpecificData1.Format(_T("%c%c"), m_UpdateGlassData.strLotSpecificData[0], m_UpdateGlassData.strLotSpecificData[1]);
		m_edit_strLotSpecificData2.Format(_T("%c%c"), m_UpdateGlassData.strLotSpecificData[2], m_UpdateGlassData.strLotSpecificData[3]);
		m_edit_strLotSpecificData3.Format(_T("%c%c"), m_UpdateGlassData.strLotSpecificData[4], m_UpdateGlassData.strLotSpecificData[5]);
		m_edit_strLotSpecificData4.Format(_T("%c%c"), m_UpdateGlassData.strLotSpecificData[6], m_UpdateGlassData.strLotSpecificData[7]);
		m_edit_strGlassJudge.Format(_T("%S"), m_UpdateGlassData.strGlassJudge);
		m_edit_strGlassID.Format(_T("%S"), m_UpdateGlassData.strGlassID);

		m_edit_nHostRecipeNumber = m_UpdateGlassData.nHostRecipeNumber;
		m_edit_nGlassType = m_UpdateGlassData.nGlassType;
		m_edit_nGlassCodeLot = m_UpdateGlassData.GlassCode.nLotNumber;
		m_edit_nGlassCodeSLot = m_UpdateGlassData.GlassCode.nSlotNumber;
		m_edit_nGlassCode = m_UpdateGlassData.nGlassCode; //pkh
		m_edit_nGlassSpecificData1 = m_UpdateGlassData.nGlassSpecificData[0];
		m_edit_nGlassSpecificData2 = m_UpdateGlassData.nGlassSpecificData[1];
		m_edit_nGlassSpecificData3 = m_UpdateGlassData.nGlassSpecificData[2];
		m_edit_nGlassSpecificData4 = m_UpdateGlassData.nGlassSpecificData[3];
		m_edit_nGlassAddData1 = m_UpdateGlassData.nGlassAddData[0];
		m_edit_nGlassAddData2 = m_UpdateGlassData.nGlassAddData[1];
		m_edit_nGlassAddData3 = m_UpdateGlassData.nGlassAddData[2];
		m_edit_nGlassAddData4 = m_UpdateGlassData.nGlassAddData[3];
		m_edit_nGlassAddData5 = m_UpdateGlassData.nGlassAddData[4];
		m_edit_nGlassAddData6 = m_UpdateGlassData.nGlassAddData[5];
		m_edit_nPreviousEqpProcessingData1 = m_UpdateGlassData.nPreviousUnitProcessingData[0];
		m_edit_nPreviousEqpProcessingData2 = m_UpdateGlassData.nPreviousUnitProcessingData[1];
		m_edit_nPreviousEqpProcessingData3 = m_UpdateGlassData.nPreviousUnitProcessingData[2];
		m_edit_nPreviousEqpProcessingData4 = m_UpdateGlassData.nPreviousUnitProcessingData[3];
		m_edit_nGlassProcessingData1 = m_UpdateGlassData.nGlassProcessingStatus[0];
		m_edit_nGlassProcessingData2 = m_UpdateGlassData.nGlassProcessingStatus[1];
		m_edit_nGlassProcessingData3 = m_UpdateGlassData.nGlassProcessingStatus[1];
		m_edit_nGlassRoutingData1 = m_UpdateGlassData.nGlassRoutingData[0];
		m_edit_nGlassRoutingData2 = m_UpdateGlassData.nGlassRoutingData[1];
		m_edit_nGlassRoutingData3 = m_UpdateGlassData.nGlassRoutingData[2];
		
		UpdateData(FALSE);
	}
}

void CDialogInline::SetUnloaderGlassData(typeGlassDataPara glassData)
{
	m_UnloaderGlassData = glassData;
}

void CDialogInline::SetScrapGlassData(typeGlassDataPara glassData)
{
	m_ScrapGlassData = glassData;
}

void CDialogInline::SetChangeGlassData(typeGlassDataPara glassData)
{
	m_ChangeGlassData = glassData;
}

void CDialogInline::SetUpdateGlassData(typeGlassDataPara glassData)
{
	m_UpdateGlassData = glassData;
}

typeGlassDataPara CDialogInline::GetLoaderGlassData()
{
	return m_LoaderGlassData;
}

typeGlassDataPara CDialogInline::GetUpdateGlassData()
{
	return m_UpdateGlassData;
}

typeGlassDataPara CDialogInline::GetUnloaderGlassData()
{
	return m_UnloaderGlassData;
}

void CDialogInline::SetChangeGlass(BOOL on_off)
{
	WriteLog(_T("SetChangeGlass Start"));
	m_MelsecListPara[DEVB_GLASS_DATA_CHANGE_REPORT].bValue = on_off;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_GLASS_DATA_CHANGE_REPORT);
	WriteLog(_T("SetChangeGlass End"));
}

void CDialogInline::SetScrapGlass(BOOL on_off)
{
	WriteLog(_T("SetScrapGlass Start"));
	m_MelsecListPara[DEVB_SCRAP_GLASS_DATA_REPORT].bValue = on_off;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_SCRAP_GLASS_DATA_REPORT);
	WriteLog(_T("SetScrapGlass End"));
}

void CDialogInline::SetGlassInProcessing(BOOL isOn)
{
	WriteLog(_T("SetGlassInProcessing Start"));
	m_MelsecListPara[DEVB_GLASS_IN_PROCESSING].bValue = isOn;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_GLASS_IN_PROCESSING);
	WriteLog(_T("SetGlassInProcessing End"));
}

void CDialogInline::SetGlassExistInUnit(BOOL isOn)
{
	WriteLog(_T("SetGlassExistInUnit Start"));
	m_MelsecListPara[DEVB_GLASS_EXIST_IN_UNIT].bValue = isOn;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_GLASS_EXIST_IN_UNIT);
	WriteLog(_T("SetGlassExistInUnit nd"));
}

void CDialogInline::SetCurrentEquipmentRecipe(int nRecipeNum)
{
	WriteLog(_T("SetCurrentEquipmentRecipe Start"));
	m_MelsecListPara[DEVW_USD_CURRENT_EQUIPMENT_RECIPE_NUMBER].wValue = nRecipeNum;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_USD_CURRENT_EQUIPMENT_RECIPE_NUMBER);
	WriteLog(_T("SetCurrentEquipmentRecipe End"));
}

void CDialogInline::SetNormalStatus(int nLoader_UnLoader, BOOL isOn)
{
	WriteLog(_T("SetNormalStatus Start"));
	if(nLoader_UnLoader == 0)
	{
		m_MelsecListPara[DEVB_LOAD_NORMAL_STATUS].bValue = isOn;
		SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_LOAD_NORMAL_STATUS);
	}
	else if(nLoader_UnLoader == 1)
	{
		m_MelsecListPara[DEVB_UNLOAD_NORMAL_STATUS].bValue = isOn;
		SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_UNLOAD_NORMAL_STATUS);
	}
	WriteLog(_T("SetNormalStatus End"));
}

void CDialogInline::SetHostRecipeRequest(WORD wRecipeNum, BOOL on_off)
{
	WriteLog(_T("SetHostRecipeRequest Start"));
	m_nTime = 0;
	m_MelsecListPara[DEVW_USD_REQUEST_HOST_RECIPE_NUMBER].wValue = wRecipeNum;
	SetHostRecipeWrite(on_off);
	m_MelsecListPara[DEVB_EQIPMENT_RECIPE_REQUEST].bValue = on_off;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_EQIPMENT_RECIPE_REQUEST);
	if(!on_off)
	{
		KillTimer(500 + (DEVB_EQIPMENT_RECIPE_REQUEST - AREA_BIT_1_START_ITEM_NUM));
	}
	WriteLog(_T("SetHostRecipeRequest End"));
}

void CDialogInline::SetHostRecipeWrite(BOOL isNULL)
{
	WriteLog(_T("SetEquipmentRecipeWrite Start"));
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_USD_REQUEST_HOST_RECIPE_NUMBER);
	WriteLog(_T("SetEquipmentRecipeWrite End"));
}

void CDialogInline::OnBnClickedButtonExchange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//SetGlassLdUldEx(2);
	//SetRecipeBodyRequest(TRUE);
	GetSlotNumber();
}

void CDialogInline::GetEcsTime()
{
	WriteLog(_T("GetEcsTime Start"));
	SetTimer(TIMER_ECS_TIME_DATA_GET, 100, NULL);
	WriteLog(_T("GetEcsTime End"));
}

void CDialogInline::OnBnClickedButtonTimeData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//GetEcsTime();
	m_strECSTime = GetMCSCTime(ECS_W_ADDRESS_TIME_DATA);
	m_edit_strEcs_Time = m_strECSTime;
	UpdateData(FALSE);
}


void CDialogInline::OnBnClickedButtonLightAlarm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetLightAlarmReport(TRUE, 100);
}


void CDialogInline::OnBnClickedButtonHevyAlarm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetHeavyAlarmReport(TRUE, 1);
}

void CDialogInline::SetEStatus(int type)
{
	WriteLog(_T("SetEStatus Start"));
	m_EQPStatus.nStatusRunIdleDown = type;
	if(type == STATUS_RUN)
	{
		SetBitEStatusIdle(FALSE);
		SetBitEStatusDown(FALSE);
		SetBitEStatusRun(TRUE);
	}
	else if(type == STATUS_IDLE)
	{
		SetBitEStatusRun(FALSE);
		SetBitEStatusDown(FALSE);
		SetBitEStatusIdle(TRUE);
	}
	else if(type == STATUS_DOWN)
	{
		SetBitEStatusRun(FALSE);
		SetBitEStatusIdle(FALSE);
		SetBitEStatusDown(TRUE);
	}
	WriteLog(_T("SetEStatus End"));
}

void CDialogInline::OnBnClickedButtonRun()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetEStatus(STATUS_RUN);
}


void CDialogInline::OnBnClickedButtonIdle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetEStatus(STATUS_IDLE);
}


void CDialogInline::OnBnClickedButtonDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetEStatus(STATUS_DOWN);
}

void CDialogInline::OnBnClickedCheckEStatusGlassInprocess()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_chk_glass_in_process)
	{
		m_PLCComm->SetBit(PLC_DEVB_GLASS_IN_PROCESSING, TRUE);
	}
	else
	{
		m_PLCComm->SetBit(PLC_DEVB_GLASS_IN_PROCESSING, FALSE);
	}
}


void CDialogInline::OnBnClickedCheckEStatusGlassExist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_chk_glass_exist)
	{
		m_PLCComm->SetBit(PLC_DEVB_GLASS_EXIST_IN_UNIT, TRUE);
	}
	else
	{
		m_PLCComm->SetBit(PLC_DEVB_GLASS_EXIST_IN_UNIT, FALSE);
	}
}

BOOL CDialogInline::GetGlassExist()
{
	return m_chk_glass_exist;
}

void CDialogInline::OnBnClickedButtonLoaderGlassDataRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ReadLoaderGlassData();
}


void CDialogInline::OnBnClickedButtonUnloaderGlassDataWrite()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	WriteUnLoaderGlassData();
}

BOOL CDialogInline::EquipmentRecipeChangeCheck(typeRecipeDataPara recipeData)
{
	WriteLog(_T("EquipmentRecipeChangeCheck Start"));
	for(int i = 0; i < 999; i++)
	{
		if(m_RecipeData[i].nRecipeNum == recipeData.nRecipeNum)
		{
			if(m_RecipeData[i].bRecipeBody && m_RecipeData[i].bRecipeNum)
			{
				return TRUE;
			}
		}
	}
	WriteLog(_T("EquipmentRecipeChangeCheck End"));
	return FALSE;
}

//recipe 추가 및 삭제 시 진행
void CDialogInline::SetEquipmentRecipeChange(typeRecipeDataPara recipeData)
{
	WriteLog(_T("SetEquipmentRecipeChange Start"));
	BOOL bRecipeChange = FALSE;

	for(int i = 0; i < 999; i++)
	{
		if(i == recipeData.nRecipeNum)
		{
			m_RecipeData[i] = recipeData;
			SetRecipeNum(recipeData.nRecipeNum, recipeData.bRecipeNum);
			break;
		}
	}
	//if(bRecipeChange)
	SetRecipeListChange(TRUE);
	WriteLog(_T("SetEquipmentRecipeChange End"));
}

void CDialogInline::SetEquipmentRecipeChange(BOOL on_off)
{
	WriteLog(_T("SetEquipmentRecipeChange Start"));
	m_MelsecListPara[DEVB_EQP_RECIPE_LIST_CHANGE].bValue = on_off;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_EQP_RECIPE_LIST_CHANGE);
	WriteLog(_T("SetEquipmentRecipeChange End"));
}

void CDialogInline::OnBnClickedButtonEqupmentRecipeChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	typeRecipeDataPara recipeData;
	recipeData.nRecipeNum = m_edit_nRecipeChangeNum;
	recipeData.bRecipeNum = TRUE;
	recipeData.bRecipeBody = TRUE;
	SetEquipmentRecipeChange(recipeData);
}

void CDialogInline::DWordToDec(WORD *word, int idx, double *dat)
{
	int temp;
	temp = word[idx + 1] << 16 | word[idx];
	*dat = (double)temp / 10000.0;
}
void CDialogInline::SetRequestRecipePara(typeRecipePara recipePara)
{
	m_RequestRecipeData = recipePara;
}

typeRecipePara CDialogInline::GetRequestRecipePara()
{
	return m_RequestRecipeData;
}

void CDialogInline::ReadRecipeVariousData()
{
	WriteLog(_T("ReadRecipeVariousData Start"));
	int idx = 0;
	typeRecipePara recipePara;
	CString temp1, temp2;
	char *cTemp;
	cTemp = new char[3];
	temp1.Empty();
	temp2.Empty();

	m_PLCComm->Read(DevW, ECS_W_ADDRESS_RVD_REQUEST_CEID, ECS_W_ADDRESS_RVD_VARIOUS_DATA_END, m_wRecipeVariousData, ECS_W_ADDRESS_RVD_VARIOUS_DATA_END - ECS_W_ADDRESS_RVD_REQUEST_CEID);
	m_RequestRecipeVariousData.nRecipeCEID = m_wRecipeVariousData[0];
	m_RequestRecipeVariousData.nRecipeCommand = m_wRecipeVariousData[1];
	m_RequestRecipeVariousData.nRecipeNumber = m_wRecipeVariousData[2];
	m_RequestRecipeVariousData.nRecipeLevel = m_wRecipeVariousData[3];
	m_RequestRecipeVariousData.cRecipeType = m_wRecipeVariousData[4];
	m_RequestRecipeVariousData.nRecipeVersion[0] = m_wRecipeVariousData[5];
	m_RequestRecipeVariousData.nRecipeVersion[1] = m_wRecipeVariousData[6];
	m_RequestRecipeVariousData.nRecipeVersion[2] = m_wRecipeVariousData[7];
	m_RequestRecipeVariousData.nUnitNumber = m_wRecipeVariousData[8];

	recipePara.RecipeReport.nRecipeNumber = m_RequestRecipeVariousData.nRecipeNumber;
	recipePara.RecipeReport.nRecipeLevel = m_RequestRecipeVariousData.nRecipeLevel;
	recipePara.RecipeReport.cRecipeType = m_RequestRecipeVariousData.cRecipeType;
	recipePara.RecipeReport.wRecipeVersion[0] = m_RequestRecipeVariousData.nRecipeVersion[0];
	recipePara.RecipeReport.wRecipeVersion[1] = m_RequestRecipeVariousData.nRecipeVersion[1];
	recipePara.RecipeReport.wRecipeVersion[2] = m_RequestRecipeVariousData.nRecipeVersion[2];
	recipePara.RecipeReport.nUnitNumber = m_RequestRecipeVariousData.nUnitNumber;
	
	for(int i = 0; i < 16; i++)
	{
		m_wRequestSpecificData[i] = m_wRecipeVariousData[9 + i];
	}
	
	for(int i = 0; i < 992; i++)
	{
		m_wRecipeParameter[i] = m_wRecipeVariousData[16 + 9 + i];
	}
	
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dCell_pos_x);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dCell_pos_y);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dCell_activesize_x);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dCell_activesize_y);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dRecipe_film_size_x0);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dRecipe_film_size_y0);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dRecipe_film_size_x1);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dRecipe_film_size_y1);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dRecipe_film_cnt_x);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dRecipe_film_cnt_y);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dRecipe_film_pitch_x);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dRecipe_film_pitch_y);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dRecipe_film_grpcnt_x);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dRecipe_film_grpcnt_y);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dRecipe_film_grppitch_x);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dRecipe_film_grppitch_y);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dRecipe_film_pos_x);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeCell.dRecipe_film_pos_y);idx+=2;
	
	//recipe panel
	for(int i = 0; i < 10; i++)
	{
		WordToString(m_wRecipeParameter[idx++], cTemp);
		temp1.Format(_T("%S"), cTemp, temp2);
		temp2 = temp1;
	}
	sprintf(recipePara.RecipePanel.cPanel_name, "%S", temp2);

	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_size_x);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_size_y);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_load_y_pos);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_load_t_pos);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_load_y_pos_manu);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_load_t_pos_manu);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_start_x_pos);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_start_y_pos);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_film_start_x_pos);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_film_start_y_pos);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPenel_cell_cnt_y);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPenel_cell_cnt_x);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPenel_cell_grpcnt_y);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPenel_cell_grpcnt_x);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_mark_size0);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_mark_size1);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_mark_to_panel_pos_y0);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_mark_to_panel_pos_x0);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_mark_to_panel_pos_y1);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_mark_to_panel_pos_x1);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_cell_pitch_x);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_cell_pitch_y);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_cell_grppitch_x);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dPanel_cell_grppitch_y);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dLoad_lift_up_pos);idx+=2;
	DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipePanel.dLoad_lift_dn_pos);idx+=2;

	//recipe scan
	for(int i = 0; i < 3; i++)
	{
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dScan_start_x);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dScan_start_y);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dScan_leng);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dScan_pitch);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dScan_pixel_x);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dScan_pixel_y);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dScan_cnt);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dScan_coord_x);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dScan_coord_y);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dScan_glass_x);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dScan_glass_y);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dThrsh0);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dThrsh1);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dThrsh2);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dThrsh3);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dThrsh4);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dDefect_size0);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dDefect_size1);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dDefect_size2);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dDefect_size3);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dDefect_size4);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dDefect_size_small);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dDefect_size_medium);idx+=2;
		DWordToDec(m_wRecipeParameter, idx, &recipePara.RecipeScan[i].dDefect_size_large);idx+=2;
	}
	SetRequestRecipePara(recipePara);
	
	WriteLog(_T("ReadRecipeVariousData End"));
}

BOOL CDialogInline::RecipeNumCheck(int nHostRecipeNum)
{
	CString strLog;
	strLog.Format(_T("RecipeNumCheck(), m_EQPStatus.nCurrenRecipeNum : %04d, m_Recipe.nRecipeNumber : %04d, Request HostRecipeNum : %04d"), m_EQPStatus.nCurrenRecipeNum, m_Recipe.nRecipeNumber, nHostRecipeNum);
	WriteLog(strLog);
	if(m_EQPStatus.nCurrenRecipeNum/*m_Recipe.nRecipeNumber*/ == nHostRecipeNum)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CDialogInline::RecipeChange()
{
	//recipe commend fun
	int idx = 0;
	m_wRecipeVariousData[idx++] = m_RecipeVariousData.nRecipeCEID = 5;
	m_wRecipeVariousData[idx++] = m_RecipeVariousData.nRecipeCommand = 2;
	m_wRecipeVariousData[idx++] = m_RecipeVariousData.nRecipeNumber;
	m_wRecipeVariousData[idx++] = m_RecipeVariousData.nRecipeLevel;
	m_wRecipeVariousData[idx++] = m_RecipeVariousData.cRecipeType;
	m_wRecipeVariousData[idx++] = m_RecipeVariousData.nRecipeVersion[0];
	m_wRecipeVariousData[idx++] = m_RecipeVariousData.nRecipeVersion[1];
	m_wRecipeVariousData[idx++] = m_RecipeVariousData.nRecipeVersion[2];
	
	idx = 0;
	for(int i = 0; i < 16; i++)
	{
		m_wRecipeVariousData[idx++] = m_wRequestSpecificData[i];
	}
	m_PLCComm->SetWORDS(PLC_DEVW_ESD_EQUIPMENT_SPECIFIC_DATA_V1, idx - 1, m_wRecipeVariousData);

	WriteRecipe(m_RecipeVariousData.nRecipeCEID, 0);

	m_MelsecListPara[DEVB_RECIPE_VARIOUS_DATA_REPORT].bValue = TRUE;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_RECIPE_VARIOUS_DATA_REPORT);		
}

void CDialogInline::WriteRecipeVariousAckData()
{
	WriteLog(_T("WriteRecipeVariousAckData Start"));
	m_MelsecListPara[DEVB_RECIPE_VARIOUS_REQUEST_CONFIRM].bValue = TRUE;
	//
	if(m_RequestRecipeVariousData.nRecipeCEID == 1)//recipe list
	{
		m_RecipeVariousAck.nRequestConfirmCEID = 1;
		//recipe list check;
		m_RecipeVariousAck.RequestConfirmACK = 0;
	}
	else if(m_RequestRecipeVariousData.nRecipeCEID == 3)//recipe parameter
	{
		m_RecipeVariousAck.nRequestConfirmCEID = 3;
		m_RecipeVariousAck.RequestConfirmACK = 0;
	}
	else if(m_RequestRecipeVariousData.nRecipeCEID == 4)//recipe commend
	{
		//recipe commend fun
		m_RecipeVariousAck.nRequestConfirmCEID = 4;
		m_RecipeVariousAck.RequestConfirmACK = 0;
	}
	else if(m_RequestRecipeVariousData.nRecipeCEID == 5)//recipe Change report
	{
		//recipe commend fun
		m_RecipeVariousAck.nRequestConfirmCEID = 5;
		m_RecipeVariousAck.RequestConfirmACK = 0;
	}
	else if(m_RequestRecipeVariousData.nRecipeCEID == 6)//EQP Constant Recipe Parameter(online)
	{
		m_RecipeVariousAck.nRequestConfirmCEID = 6;
		m_RecipeVariousAck.RequestConfirmACK = 0;
	}
	else if(m_RequestRecipeVariousData.nRecipeCEID == 7)//EQP Constant Recipe Parameter(request)
	{
		m_RecipeVariousAck.nRequestConfirmCEID = 7;
		m_RecipeVariousAck.RequestConfirmACK = 0;
	}

	//WriteRecipeVariousAckData
	m_PLCComm->SetWORD(0x14400, (WORD)m_RecipeVariousAck.nRequestConfirmCEID);
	m_PLCComm->SetWORD(0x14401, (WORD)m_RecipeVariousAck.RequestConfirmACK);
	//SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_RCR_REQUEST_CONFIRM_CEID);
	//SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_RCR_REQUEST_CONFIRM_ACK);
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_RECIPE_VARIOUS_REQUEST_CONFIRM);

	

	WriteLog(_T("WriteRecipeVariousAckData End"));
}

void CDialogInline::SetReipeVariousData(typeRecipeVariousDataPara variousData)
{
	WriteLog(_T("SetReipeVariousData Start"));
	m_RecipeVariousData = variousData;
	WriteRecipeVariousData(m_RequestRecipeVariousData.nRecipeCEID);
	WriteLog(_T("SetReipeVariousData End"));
}

void CDialogInline::WriteRecipeVariousData(int ceid)
{
	WriteLog(_T("WriteRecipeVariousData Start"));
	CString strLog;
	m_MelsecListPara[DEVB_RECIPE_VARIOUS_DATA_REPORT].bValue = TRUE;
	//WriteRecipeVariousData
	memset(m_wRecipeVariousData, NULL, sizeof(WORD) * 1024);
	int idx = 0;
	//m_PLCComm->SetWORD(0x14000, 999);
	if(m_RequestRecipeVariousData.nRecipeCEID == 3)//recipe parameter
	{
		m_wRecipeVariousData[idx] = 3;//ceid
		//m_PLCComm->SetWORD(0x14000, 999);
		//m_PLCComm->SetWORD(PLC_DEVW_RCR_RECIPE_REPORT_CEID, m_wRecipeVariousData[idx++], FALSE);
		idx+=5;
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].nRecipeNumber;//recipe number
		//m_PLCComm->SetWORD(PLC_DEVW_RCR_RECIPE_REPORT_CEID + idx, m_wRecipeVariousData[idx++]);
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].nRecipeLevel;//recipe level
		//m_PLCComm->SetWORD(PLC_DEVW_RCR_RECIPE_REPORT_CEID + idx, m_wRecipeVariousData[idx++]);
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].cRecipeType;//recipe type
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].wRecipeVersion[0];//recipe version0
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].wRecipeVersion[1];//recipe version1
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].wRecipeVersion[2];//recipe version2
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].nUnitNumber;
		m_PLCComm->SetWORDS(PLC_DEVW_RCR_RECIPE_REPORT_CEID, idx - 1, m_wRecipeVariousData);

		idx = 0;
		for(int i = 0; i < 16; i++)
		{
			m_wRecipeVariousData[idx++] = m_wRequestSpecificData[i];
		}
		m_PLCComm->SetWORDS(PLC_DEVW_ESD_EQUIPMENT_SPECIFIC_DATA_V1, idx - 1, m_wRecipeVariousData);

		WriteRecipe(m_RequestRecipeVariousData.nRecipeCEID, 1);
#ifndef _ECS_TEST
		m_MelsecListPara[DEVB_RECIPE_VARIOUS_DATA_REPORT].bValue = TRUE;
		SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_RECIPE_VARIOUS_DATA_REPORT);	
#endif

	}
	else if(m_RequestRecipeVariousData.nRecipeCEID == 4)//recipe commend
	{
		m_PLCComm->Read(DevW, ECS_W_ADDRESS_RVD_REQUEST_CEID, 8, m_wRecipeVariousData, 8);
		m_RequestRecipeVariousData.nRecipeCEID = m_wRecipeVariousData[idx++];
		m_RequestRecipeVariousData.nRecipeCommand = m_wRecipeVariousData[idx++];
		m_RecipeRMSData.nRecipeNum = m_RequestRecipeVariousData.nRecipeNumber = m_wRecipeVariousData[idx++];
		m_RecipeRMSData.nRecipeLevel = m_RequestRecipeVariousData.nRecipeLevel = m_wRecipeVariousData[idx++];
		m_RecipeRMSData.cRecipeType = m_RequestRecipeVariousData.cRecipeType = m_wRecipeVariousData[idx++];
		m_RecipeRMSData.RecipeVersion[0] = m_RequestRecipeVariousData.nRecipeVersion[0] = m_wRecipeVariousData[idx++];
		m_RecipeRMSData.RecipeVersion[1] = m_RequestRecipeVariousData.nRecipeVersion[1] = m_wRecipeVariousData[idx++];
		m_RecipeRMSData.RecipeVersion[2] = m_RequestRecipeVariousData.nRecipeVersion[2] = m_wRecipeVariousData[idx++];
		m_RequestRecipeVariousData.nUnitNumber = m_wRecipeVariousData[idx++];

		if(m_RequestRecipeVariousData.nRecipeCommand != 3)
		{
			m_PLCComm->Read(DevW, ECS_W_ADDRESS_RVD_EQUIPMENT_SPECIFIC_DATA, 16, m_wRecipeVariousData, 16);
			idx = 0;
			for(int i = 0; i < 16; i++)
			{
				m_RecipeRMSData.SpectificData[i] = m_wRecipeVariousData[i];
			}
			m_PLCComm->Read(DevW, ECS_W_ADDRESS_RVD_RECIPE_PARAMETER, 992, m_wRecipeVariousData, 992);
			idx = 0;
			for(int i = 0; i < 992; i++)
			{
				m_RecipeRMSData.Parameter[i] = m_wRecipeVariousData[i];
			}
		}
		
		if(m_RequestRecipeVariousData.nRecipeCommand == 1)//built
		{
			//recipe 생성
			strLog.Format(_T("Recipe Built, RecipeNum = %d, RecipeLevel = %d, RecipeType = %c"), m_RecipeRMSData.nRecipeNum, m_RecipeRMSData.nRecipeLevel, m_RecipeRMSData.cRecipeType);
			WriteLog(strLog);
		}
		else if(m_RequestRecipeVariousData.nRecipeCommand == 2)//change
		{
			//recipe 변경
			strLog.Format(_T("Recipe Change, RecipeNum = %d, RecipeLevel = %d, RecipeType = %c"), m_RecipeRMSData.nRecipeNum, m_RecipeRMSData.nRecipeLevel, m_RecipeRMSData.cRecipeType);
			WriteLog(strLog);
		}
		else if(m_RequestRecipeVariousData.nRecipeCommand == 3)//delete
		{
			//recipe 삭제
			strLog.Format(_T("Recipe Delete, RecipeNum = %d, RecipeLevel = %d, RecipeType = %c"), m_RecipeRMSData.nRecipeNum, m_RecipeRMSData.nRecipeLevel, m_RecipeRMSData.cRecipeType);
			WriteLog(strLog);
		}
		else if(m_RequestRecipeVariousData.nRecipeCommand == 4)//rename
		{
			//recipe 이름 변경
			strLog.Format(_T("Recipe Rename, RecipeNum = %d, RecipeLevel = %d, RecipeType = %c"), m_RecipeRMSData.nRecipeNum, m_RecipeRMSData.nRecipeLevel, m_RecipeRMSData.cRecipeType);
			WriteLog(strLog);
		}
	}
	else if(m_RequestRecipeVariousData.nRecipeCEID == 5)//recipe Change report
	{
		m_wRecipeVariousData[idx++] = 3;//ceid
		idx+=4;
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].nRecipeNumber;//recipe number
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].nRecipeLevel;//recipe level
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].cRecipeType;//recipe type
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].wRecipeVersion[0];//recipe version0
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].wRecipeVersion[1];//recipe version1
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].wRecipeVersion[2];//recipe version2
		m_wRecipeVariousData[idx++] = 0;
		for(int i = 0; i < 16; i++)
		{
			m_wRecipeVariousData[idx++] = m_wRequestSpecificData[i];
		}
		m_PLCComm->SetWORDS(PLC_DEVW_RCR_RECIPE_REPORT_CEID, idx - 1, m_wRecipeVariousData);

		WriteRecipe(m_RequestRecipeVariousData.nRecipeCEID, 1);
#ifndef _ECS_TEST
		m_MelsecListPara[DEVB_RECIPE_VARIOUS_DATA_REPORT].bValue = TRUE;
		SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_RECIPE_VARIOUS_DATA_REPORT);	
#endif
	}
	else if(m_RequestRecipeVariousData.nRecipeCEID == 6)//EQP Constant Recipe Parameter(online)
	{
		m_wRecipeVariousData[idx++] = 6;//ceid
		idx+=4;
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].nRecipeNumber;//recipe number
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].nRecipeLevel;//recipe level
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].cRecipeType;//recipe type
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].wRecipeVersion[0];//recipe version0
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].wRecipeVersion[1];//recipe version1
		m_wRecipeVariousData[idx++] = m_RecipeReport[m_SelectRecipeData.RecipeData.nRecipeNum].wRecipeVersion[2];//recipe version2
		m_wRecipeVariousData[idx++] = 0;
		for(int i = 0; i < 16; i++)
		{
			m_wRecipeVariousData[idx++] = m_wRequestSpecificData[i];
		}
		m_PLCComm->SetWORDS(PLC_DEVW_RCR_RECIPE_REPORT_CEID, idx - 1, m_wRecipeVariousData);

		WriteRecipe(m_RequestRecipeVariousData.nRecipeCEID, 1);
	}
	else if(m_RequestRecipeVariousData.nRecipeCEID == 7)//EQP Constant Recipe Parameter(request)
	{

	}

	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_RECIPE_VARIOUS_DATA_REPORT);
	WriteLog(_T("WriteRecipeVariousData End"));
}

void CDialogInline::GetRecipeVariousRead()
{
	WriteLog(_T("GetRecipeVariousRead Start"));
	SetTimer(TIMER_RECIPE_VARIOUS_DATA_REQUEST, 100, NULL);
	WriteLog(_T("GetRecipeVariousRead End"));
}

void CDialogInline::OnBnClickedButtonRecipeVariousRequest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//this->ReadRecipeVariousData();
	GetRecipeVariousRead();
}

void CDialogInline::OnBnClickedButtonRecipeVariousReport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	WriteRecipeVariousData(m_RequestRecipeVariousData.nRecipeCEID);
}

void CDialogInline::SetScrapGlassDataReport()
{
	WriteLog(_T("SetScrapGlassDataReport Start"));
	SetTimer(TIMER_SCRAP_GLASS_DATA_REPORT, 100, NULL);
	WriteLog(_T("SetScrapGlassDataReport End"));
}

void CDialogInline::OnBnClickedButtonScrapGlassData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlassDataUpdate(TRUE);
	m_bScrapGlass = TRUE;
	if(m_bScrapGlass)
	{
		SetScrapGlassDataReport();
	}
}

void CDialogInline::SetLostGlassDataReport(BOOL on_off)
{
	WriteLog(_T("SetLostGlassDataReport Start"));
	m_MelsecListPara[DEVB_LOST_GLASS_DATA_REQUEST].bValue = on_off;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_LOST_GLASS_DATA_REQUEST);
	if(on_off)
	{
		m_nTimeCheck[TIMEOUT_LOST_GLASS_DATA_REQUEST] = 0;
		SetTimer(TIMER_LOST_GLASS_DATA_REPORT, 100, NULL);
	}
	WriteLog(_T("SetLostGlassDataReport End"));
}

void CDialogInline::SetChangeGlassDataReport()
{
	WriteLog(_T("SetChangeGlassDataReport Start"));
	SetTimer(TIMER_CHANGE_GLASS_DATA_REPORT, 100, NULL);
	WriteLog(_T("SetChangeGlassDataReport End"));
}

void CDialogInline::OnBnClickedButtonLostGlassData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlassDataUpdate(TRUE);
	typeLostGlassDataPara lostGlass;

	lostGlass.nGlassOption = m_radio_lost_glass_data_option;
	sprintf(lostGlass.strGlassID, "%s", m_UpdateGlassData.strGlassID);
	lostGlass.nGlassCode = m_UpdateGlassData.nGlassCode;
	lostGlass.GlassCode = m_UpdateGlassData.GlassCode;
	m_nLoastGlassDataOption = m_radio_lost_glass_data_option;

	SetLostGlassDataReport(lostGlass);
	WriteLostGlassData(m_nLoastGlassDataOption);
	SetLostGlassDataReport(TRUE);
}


void CDialogInline::OnBnClickedButtonChangeGlassData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlassDataUpdate(TRUE);
	SetChangeGlassData(GetUnloaderGlassData());
	SetChangeGlassDataReport();
}

BOOL CDialogInline::WriteLog(CString text)
{
	EnterCriticalSection(&m_CSInline);

	FILE*	stream;
	char	strFile[MAX_PATH], strToFile[MAX_PATH];
    long	result, line;
	char	szTime[DAT_STR_LENG]; 
	CString bak_file, make_path;
	int limit_line = 500000;
	CString path;
	CString strTime;
	CString log;

	char buffer[DAT_STR_LENG];

	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);
	strTime.Format(_T("%04d%02d%02d"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
	log.Format(_T("%04d-%02d-%02d-%02d:%02d:%02d:%03d->%s"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay, lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond, lpSystemTime.wMilliseconds, text);
	make_path.Format(_T("D:\\DATABASE\\LOG\\%s"), strTime);
	_MakeDirectories(make_path);
	sprintf(strFile, "%S\\%S_Inline_Log.log", make_path, strTime);

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

	memset(szTime, NULL, sizeof(szTime));
	sprintf_s(szTime, log.GetLength() + 1, "%S", log);
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

void CDialogInline::SetTPD(typeTPDDataPara tpdData)
{
	m_TPDData = tpdData;
}

void CDialogInline::SetTPDData()
{
	WriteLog(_T("SetTPDData Start"));
	int nTPDNum = m_nTPDNum;
	int nTPDIdx = DEVW_TPD_DATA_V1;
	int idx = 0;
	WORD wTemp;
	WORD wWord[992];

	m_MelsecListPara[DEVW_USD_TPD_REPORT_NUMBER].wValue = nTPDNum;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_USD_TPD_REPORT_NUMBER);
	m_MelsecListPara[DEVW_USD_TPD_UPDATE_TIME].wValue = m_nTrigger[TRIGGER_TPD_DATA_DELAY];
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_USD_TPD_UPDATE_TIME);

	wWord[idx++] = (m_APDData.nAlignMarkX1 << 16) >> 16;
	wWord[idx++] = m_APDData.nAlignMarkX1 >> 16;

	m_MelsecListPara[nTPDIdx++].wValue = m_TPDData.nSolBlw;
	wWord[idx++] = ((m_TPDData.nSolBlw << 16) >> 16);
	wWord[idx++] = m_TPDData.nSolBlw >> 16;
	m_MelsecListPara[nTPDIdx++].wValue = m_TPDData.nChuckVac;
	wWord[idx++] = (m_TPDData.nChuckVac << 16) >> 16;
	wWord[idx++] = m_TPDData.nChuckVac >> 16;
	m_MelsecListPara[nTPDIdx++].wValue = m_TPDData.nScan_X_Axis_TQ;
	wWord[idx++] = (m_TPDData.nScan_X_Axis_TQ << 16) >> 16;
	wWord[idx++] = m_TPDData.nScan_X_Axis_TQ >> 16;
	m_MelsecListPara[nTPDIdx++].wValue = m_TPDData.nRV_Y_Axis_TQ;
	wWord[idx++] = (m_TPDData.nRV_Y_Axis_TQ << 16) >> 16;
	wWord[idx++] = m_TPDData.nRV_Y_Axis_TQ >> 16;
	m_MelsecListPara[nTPDIdx++].wValue = m_TPDData.nLIFT_AXIS_TQ;
	wWord[idx++] = (m_TPDData.nLIFT_AXIS_TQ << 16) >> 16;
	wWord[idx++] = m_TPDData.nLIFT_AXIS_TQ >> 16;
	m_MelsecListPara[nTPDIdx++].wValue = m_TPDData.nTPDNumber = nTPDNum;
	wWord[idx++] = m_TPDData.nTPDNumber;

	WriteTPDData(PLC_DEVW_TPD_DATA_V1, idx - 1, wWord);
	m_nTPDNum++;
	if(m_nTPDNum == 32767)
	{
		m_nTPDNum = -32768;
	}
	WriteLog(_T("SetTPDData End"));
}

void CDialogInline::SetAPD(typeAPDDataPara apdData)
{
	m_APDData = apdData;
}

typeAPDDataPara CDialogInline::GetAPD()
{
	return m_APDData;
}

void CDialogInline::SetAPDData()
{
	WriteLog(_T("SetAPDData Start"));
	int nAPDIdx = DEVW_APD_DATA_V1;
	int idx = 0;
	WORD wTemp;
	WORD wWord[992];
	char *cTemp;
	cTemp = new char[3];
	CString temp1, temp2;
	//m_APDData = apdData;
	
	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.GlassStartTime.wYear;
	wWord[idx++] = m_APDData.GlassStartTime.wYear;
	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.GlassStartTime.wMonth;
	wWord[idx++] = m_APDData.GlassStartTime.wMonth;
	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.GlassStartTime.wDay;
	wWord[idx++] = m_APDData.GlassStartTime.wDay;
	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.GlassStartTime.wHour;
	wWord[idx++] = m_APDData.GlassStartTime.wHour;
	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.GlassStartTime.wMinute;
	wWord[idx++] = m_APDData.GlassStartTime.wMinute;
	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.GlassStartTime.wSecond;
	wWord[idx++] = m_APDData.GlassStartTime.wSecond;

	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.GlassEndTime.wYear;
	wWord[idx++] = m_APDData.GlassEndTime.wYear;
	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.GlassEndTime.wMonth;
	wWord[idx++] = m_APDData.GlassEndTime.wMonth;
	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.GlassEndTime.wDay;
	wWord[idx++] = m_APDData.GlassEndTime.wDay;
	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.GlassEndTime.wHour;
	wWord[idx++] = m_APDData.GlassEndTime.wHour;
	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.GlassEndTime.wMinute;
	wWord[idx++] = m_APDData.GlassEndTime.wMinute;
	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.GlassEndTime.wSecond;
	wWord[idx++] = m_APDData.GlassEndTime.wSecond;

	temp1.Empty();
	temp2.Empty();
	for(int i = 0; i < 12; i++)
	{
		if(m_APDData.cGlassID[i] == 0x00)
		{
			nAPDIdx = DEVW_APD_DATA_V1 + 12 + 12 + 12;
			idx = 6 + 6 + 6;
			break;
		}

		temp1.Format(_T("%s%c"), temp2, m_APDData.cGlassID[i]);
		temp2 = temp1;
		if(i % 2 != 0)
		{
			if(temp2 == "0")
			{
				break;
			}
			sprintf(cTemp, "%S", temp2.Mid(1,2));
			StringToWord(cTemp, &wTemp);
			wWord[idx++] = wTemp;
			temp2.Empty();
		}
	}

	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.cAlignSuccess;
	wWord[idx++] = m_APDData.cAlignSuccess;
	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.nInspMode;
	wWord[idx++] = m_APDData.nInspMode;

	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.nAlignMarkX1;
	wWord[idx++] = (m_APDData.nAlignMarkX1 << 16) >> 16;
	wWord[idx++] = m_APDData.nAlignMarkX1 >> 16;
	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.nAlignMarkX2;
	wWord[idx++] = (m_APDData.nAlignMarkX2 << 16) >> 16;
	wWord[idx++] = m_APDData.nAlignMarkX2 >> 16;
	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.nAlignMarkY1;
	wWord[idx++] = (m_APDData.nAlignMarkY1 << 16) >> 16;
	wWord[idx++] = m_APDData.nAlignMarkY1 >> 16;
	m_MelsecListPara[nAPDIdx++].wValue = m_APDData.nAlignMarkY2;
	wWord[idx++] = (m_APDData.nAlignMarkY2 << 16) >> 16;
	wWord[idx++] = m_APDData.nAlignMarkY2 >> 16;
	
	WriteAPDData(PLC_DEVW_APD_DATA_V1, idx - 1, wWord);
	
	//delete cTemp;
	WriteLog(_T("SetAPDData End"));
}

void CDialogInline::SetGlassPositionData(WORD *glassPosition)
{
	WriteLog(_T("SetGlassPositionData Start"));
	for(int i = 0; i < 2; i++)
	{
		m_wPositionData[i] = glassPosition[i];
		m_MelsecListPara[DEVW_POG_POSITION_1_GLASS_CODE + i].wValue = m_wPositionData[i];
	}
	WriteGlassPoitionData();
	WriteLog(_T("SetGlassPositionData End"));
}

void CDialogInline::WriteTPDData(int nStartAddress, int nSize, WORD *word)
{
	WriteLog(_T("WriteTPDData Start"));
	m_PLCComm->SetWORDS(nStartAddress, nSize, word);
	WriteLog(_T("WriteTPDData End"));
}

void CDialogInline::WriteAPDData(int nStartAddress, int nSize, WORD *word)
{
	WriteLog(_T("WriteAPDData Start"));
	m_PLCComm->SetWORDS(nStartAddress, nSize, word);
	m_MelsecListPara[DEVB_GLASS_APD_DATA_REPORT].bValue = TRUE;
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVB_GLASS_APD_DATA_REPORT);
	WriteLog(_T("WriteAPDData End"));
}

void CDialogInline::WriteGlassPoitionData()
{
	WriteLog(_T("WriteGlassPoitionData Start"));
	SendMessage(USE_MSG_INLINE_CTRL, MSG_WRITE_ADDRS, DEVW_POG_POSITION_1_GLASS_CODE);
	WriteLog(_T("WriteGlassPoitionData End"));
}

void CDialogInline::SetRecipeList(typeRecipeReportDataPara *recipeListPara)
{
	m_RecipeReport = recipeListPara;
	for(int i = 0; i < 992; i++)
	{
		m_RecipeVariousData.RecipeReportData[i] = m_RecipeReport[i];
	}
}

void CDialogInline::WriteRecipeList(int ceid, int start_report_cnt, int end_report_cnt)
{
	WORD *write_word;
	write_word = new WORD[992];
	memset(write_word, NULL, sizeof(WORD) * 992);
	int writeCnt = 0;
	write_word[writeCnt++] = 1;

	switch(ceid)
	{
	case RMS_RECIPE_LIST_FIRST_REPORT:
		write_word[writeCnt++] = RMS_RECIPE_LIST_FIRST_REPORT;
		break;
	case RMS_RECIPE_LIST_EXCEPT_REPORT:
		write_word[writeCnt++] = RMS_RECIPE_LIST_EXCEPT_REPORT;
		break;
	case RMS_RECIPE_LIST_LAST_REPORT:
		write_word[writeCnt++] = RMS_RECIPE_LIST_LAST_REPORT;
		break;
	}

	write_word[writeCnt++] = m_RecipeVariousData.nRecipeTotalCount;
	write_word[writeCnt++] = m_RecipeVariousData.nRecipeReportCount;

	//m_PLCComm->SetWORDS(DEVW_RCR_RECIPE_REPORT_CEID, writeCnt - 1, write_word);
	m_PLCComm->Write(DevW, PLC_DEVW_RCR_RECIPE_REPORT_CEID, PLC_DEVW_RCR_RECIPE_REPORT_CEID + writeCnt, write_word, writeCnt);


	memset(write_word, NULL, sizeof(WORD) * 992);
	writeCnt = 0;
	for(int i = 0; i < 16; i++)
	{
		write_word[writeCnt++] = m_RecipeVariousData.wEquipmentSpecificDataV[i];
	}
	m_PLCComm->Write(DevW, PLC_DEVW_ESD_EQUIPMENT_SPECIFIC_DATA_V1, PLC_DEVW_ESD_EQUIPMENT_SPECIFIC_DATA_V1 + writeCnt, write_word, writeCnt);
	//m_PLCComm->SetWORDS(DEVW_ESD_EQUIPMENT_SPECIFIC_DATA_V1, writeCnt - 1, write_word);

	memset(write_word, NULL, sizeof(WORD) * 992);
	writeCnt = 0;
	int recipe_cnt = 0;
	for(int i = start_report_cnt; i < (end_report_cnt); i++)
	{
		write_word[writeCnt++] = m_RecipeVariousData.RecipeReportData[i].nRecipeNumber;
		write_word[writeCnt++] = m_RecipeVariousData.RecipeReportData[i].nRecipeLevel;
		write_word[writeCnt++] = m_RecipeVariousData.RecipeReportData[i].cRecipeType;
		write_word[writeCnt++] = m_RecipeVariousData.RecipeReportData[i].wRecipeVersion[0];
		write_word[writeCnt++] = m_RecipeVariousData.RecipeReportData[i].wRecipeVersion[1];
		write_word[writeCnt++] = m_RecipeVariousData.RecipeReportData[i].wRecipeVersion[2];
		write_word[writeCnt++] = m_RecipeVariousData.RecipeReportData[i].nRecipeStatus;
		write_word[writeCnt++] = m_RecipeVariousData.RecipeReportData[i].nUnitNumber;
		recipe_cnt++;
	}
	m_nReportStart_RecipeListCnt = end_report_cnt;
	m_nReportEnd_RecipeListCnt = end_report_cnt;
	m_PLCComm->Write(DevW, PLC_DEVW_RP_RECIPE_PARAMETER_V1, PLC_DEVW_RP_RECIPE_PARAMETER_V1 + writeCnt, write_word, writeCnt);
	//m_PLCComm->SetWORDS(DEVW_RP_RECIPE_PARAMETER_V1, writeCnt - 1, write_word);
}

void CDialogInline::SetRecipe(typeRecipePara recipePara)
{
	m_Recipe = recipePara;

	//WriteRecipe
	m_RecipeBodyData.nRecipeNumber = m_Recipe.nRecipeNumber;
	m_RecipeBodyData.nRecipeLevel = 2;
	m_RecipeBodyData.cRecipeType = 'M';
	m_RecipeBodyData.wRecipeVersion[0] = 0;
	m_RecipeBodyData.wRecipeVersion[1] = 0;
	m_RecipeBodyData.wRecipeVersion[2] = 0;
}

void CDialogInline::WriteRecipe(int ceid, int nBody_Parameter)
{
	WriteLog(_T("WriteRecipe Start"));
	int nRecipeIdx = 0;
	if(nBody_Parameter == 0)
	{
		nRecipeIdx = DEVW_RBD_RECIPE_BODY_V1;
	}
	else if(nBody_Parameter == 1)
	{
		nRecipeIdx = DEVW_RP_RECIPE_PARAMETER_V1;
	}

	int idx = 0;
	int nRecipePanel = 0;
	WORD wTemp;
	WORD *wWord;
	CString temp1, temp2, strPanel_name;
	char *cTemp;
	cTemp = new char[3];
	wWord = new WORD[992];
	memset(wWord, NULL, sizeof(WORD)*992);
	memset(cTemp, NULL, sizeof(char)*3);

	if(ceid == 3 || ceid == 5 || ceid == 6)
	{
		//recipe cell
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dCell_pos_x;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dCell_pos_x * 10000) << 16) >> 16;;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dCell_pos_x * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dCell_pos_y;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dCell_pos_y * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dCell_pos_y * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dCell_activesize_x;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dCell_activesize_x * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dCell_activesize_x * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dCell_activesize_y;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dCell_activesize_y * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dCell_activesize_y * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dRecipe_film_size_x0;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dRecipe_film_size_x0 * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dRecipe_film_size_x0 * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dRecipe_film_size_y0;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dRecipe_film_size_y0 * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dRecipe_film_size_y0 * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dRecipe_film_size_x1;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dRecipe_film_size_x1 * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dRecipe_film_size_x1 * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dRecipe_film_size_y1;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dRecipe_film_size_y1 * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dRecipe_film_size_y1 * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dRecipe_film_cnt_x;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dRecipe_film_cnt_x * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dRecipe_film_cnt_x * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dRecipe_film_cnt_y;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dRecipe_film_cnt_y * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dRecipe_film_cnt_y * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dRecipe_film_pitch_x;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dRecipe_film_pitch_x * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dRecipe_film_pitch_x * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dRecipe_film_pitch_y;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dRecipe_film_pitch_y * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dRecipe_film_pitch_y * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dRecipe_film_grpcnt_x;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dRecipe_film_grpcnt_x * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dRecipe_film_grpcnt_x * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dRecipe_film_grpcnt_y;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dRecipe_film_grpcnt_y * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dRecipe_film_grpcnt_y * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dRecipe_film_grppitch_x;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dRecipe_film_grppitch_x * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dRecipe_film_grppitch_x * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dRecipe_film_grppitch_y;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dRecipe_film_grppitch_y * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dRecipe_film_grppitch_y * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dRecipe_film_pos_x;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dRecipe_film_pos_x * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dRecipe_film_pos_x * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeCell.dRecipe_film_pos_y;
		wWord[idx++] = ((int)(m_Recipe.RecipeCell.dRecipe_film_pos_y * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipeCell.dRecipe_film_pos_y * 10000) >> 16;


		//recipe panel
		temp1.Empty();
		temp2.Empty();
		nRecipePanel = idx - 1;
		//sprintf(m_Recipe.RecipePanel.cPanel_name, "%s", m_Recipe.cRecipeName);
		for(int i = 0; i < 20; i++)
		{
			if(m_Recipe.RecipePanel.cPanel_name[i] == 0x00)
			{
				nRecipeIdx = DEVW_APD_DATA_V1 + nRecipePanel + 20;
				idx = nRecipePanel + 20;
				break;
			}
			else
			{
				temp1.Format(_T("%s%c"), temp2, m_Recipe.RecipePanel.cPanel_name[i]);
				temp2 = temp1;
				if(i % 2 != 0)
				{
					if(temp1.IsEmpty())
					{
						m_MelsecListPara[nRecipeIdx++].wValue = 0;
					}
					sprintf(cTemp, "%S", temp1);
					StringToWord(cTemp, &wTemp);
					m_MelsecListPara[nRecipeIdx++].wValue = wTemp;
					wWord[idx++] = wTemp;
					temp1.Empty();
					temp2.Empty();
				}
			}
		}

		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_size_x;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_size_x * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_size_x * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_size_y;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_size_y * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_size_y * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_load_y_pos;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_load_y_pos * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_load_y_pos * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_load_t_pos;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_load_t_pos * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_load_t_pos * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_load_y_pos_manu;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_load_y_pos_manu * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_load_y_pos_manu * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_load_t_pos_manu;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_load_t_pos_manu * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_load_t_pos_manu * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_start_x_pos;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_start_x_pos * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_start_x_pos * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_start_y_pos;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_start_y_pos * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_start_y_pos * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_film_start_x_pos;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_film_start_x_pos * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_film_start_x_pos * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_film_start_y_pos;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_film_start_y_pos * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_film_start_y_pos * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPenel_cell_cnt_y;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPenel_cell_cnt_y * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPenel_cell_cnt_y * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPenel_cell_cnt_x;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPenel_cell_cnt_x * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPenel_cell_cnt_x * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPenel_cell_grpcnt_y;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPenel_cell_grpcnt_y * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPenel_cell_grpcnt_y * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPenel_cell_grpcnt_x;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPenel_cell_grpcnt_x * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPenel_cell_grpcnt_x * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_mark_size0;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_mark_size0 * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_mark_size0 * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_mark_size1;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_mark_size1 * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_mark_size1 * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_mark_to_panel_pos_y0;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_mark_to_panel_pos_y0 * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_mark_to_panel_pos_y0 * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_mark_to_panel_pos_x0;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_mark_to_panel_pos_x0 * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_mark_to_panel_pos_x0 * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_mark_to_panel_pos_y1;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_mark_to_panel_pos_y1 * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_mark_to_panel_pos_y1 * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_mark_to_panel_pos_x1;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_mark_to_panel_pos_x1 * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_mark_to_panel_pos_x1 * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_cell_pitch_x;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_cell_pitch_x * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_cell_pitch_x * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_cell_pitch_y;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_cell_pitch_y * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_cell_pitch_y * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_cell_grppitch_x;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_cell_grppitch_x * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_cell_grppitch_x * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dPanel_cell_grppitch_y;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dPanel_cell_grppitch_y * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dPanel_cell_grppitch_y * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dLoad_lift_up_pos;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dLoad_lift_up_pos * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dLoad_lift_up_pos * 10000) >> 16;
		m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipePanel.dLoad_lift_dn_pos;
		wWord[idx++] = ((int)(m_Recipe.RecipePanel.dLoad_lift_dn_pos * 10000) << 16) >> 16;
		wWord[idx++] = (int)(m_Recipe.RecipePanel.dLoad_lift_dn_pos * 10000) >> 16;

	
		//recipe scan
		for(int i = 0; i < 3; i++)
		{
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dScan_start_x;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dScan_start_x * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dScan_start_x * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dScan_start_y;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dScan_start_x * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dScan_start_x * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dScan_leng;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dScan_leng * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dScan_leng * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dScan_pitch;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dScan_pitch * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dScan_pitch * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dScan_pixel_x;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dScan_pixel_x * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dScan_pixel_x * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dScan_pixel_y;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dScan_pixel_y * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dScan_pixel_y * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dScan_cnt;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dScan_cnt * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dScan_cnt * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dScan_coord_x;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dScan_coord_x * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dScan_coord_x * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dScan_coord_y;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dScan_coord_y * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dScan_coord_y * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dScan_glass_x;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dScan_glass_x * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dScan_glass_x * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dScan_glass_y;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dScan_glass_y * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dScan_glass_y * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dThrsh0;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dThrsh0 * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dThrsh0 * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dThrsh1;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dThrsh1 * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dThrsh1 * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dThrsh2;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dThrsh2 * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dThrsh2 * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dThrsh3;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dThrsh3 * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dThrsh3 * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dThrsh4;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dThrsh4 * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dThrsh4 * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dDefect_size0;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dDefect_size0 * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dDefect_size0 * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dDefect_size1;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dDefect_size1 * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dDefect_size1 * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dDefect_size2;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dDefect_size2 * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dDefect_size2 * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dDefect_size3;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dDefect_size3 * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dDefect_size3 * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dDefect_size4;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dDefect_size4 * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dDefect_size4 * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dDefect_size_small;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dDefect_size_small * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dDefect_size_small * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dDefect_size_medium;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dDefect_size_medium * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dDefect_size_medium * 10000) >> 16;
			m_MelsecListPara[nRecipeIdx++].wValue = m_Recipe.RecipeScan[i].dDefect_size_large;
			wWord[idx++] = ((int)(m_Recipe.RecipeScan[i].dDefect_size_large * 10000) << 16) >> 16;
			wWord[idx++] = (int)(m_Recipe.RecipeScan[i].dDefect_size_large * 10000) >> 16;
		}

		if(nBody_Parameter == 0)
		{
			m_PLCComm->SetWORDS(PLC_DEVW_RBD_RECIPE_BODY_V1, idx - 1, wWord);
		}
		else if(nBody_Parameter == 1)
		{
			m_PLCComm->SetWORDS(PLC_DEVW_RP_RECIPE_PARAMETER_V1, idx - 1, wWord);
		}
		
	}
	//delete cTemp;
	delete wWord;
	WriteLog(_T("WriteRecipe End"));
}

void CDialogInline::OnBnClickedButtonGlassPosition()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetGlassPositionData(m_wPositionData);
}

void CDialogInline::OnBnClickedButtonGlassApdData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	typeAPDDataPara apdData;
	CString temp;
	SYSTEMTIME time;
	GetLocalTime(&time);
	temp.Format(_T("20160127_101213"));
	m_APDData.GlassStartTime.wYear = time.wYear;
	m_APDData.GlassStartTime.wMonth = time.wMonth;
	m_APDData.GlassStartTime.wDay = time.wDay;
	m_APDData.GlassStartTime.wHour = time.wHour;
	m_APDData.GlassStartTime.wMinute = time.wMinute;
	m_APDData.GlassStartTime.wSecond = time.wSecond;
	//sprintf(apdData.cGlassStartTime, "%S", temp);
	temp.Format(_T("20160127_111314"));
	m_APDData.GlassStartTime.wYear = time.wYear;
	m_APDData.GlassStartTime.wMonth = time.wMonth;
	m_APDData.GlassStartTime.wDay = time.wDay;
	m_APDData.GlassStartTime.wHour = time.wHour + 1;
	m_APDData.GlassStartTime.wMinute = time.wMinute + 30;
	m_APDData.GlassStartTime.wSecond = time.wSecond + 10;
	//sprintf(apdData.cGlassEndTime, "%S", temp);
	temp.Format(_T("TEST_GLASS"));
	sprintf(m_APDData.cGlassID, "%S", temp);
	m_APDData.cAlignSuccess = 'T';
	m_APDData.nAlignMarkX1 = 1;
	m_APDData.nAlignMarkX2 = 2;
	m_APDData.nAlignMarkY1 = 3;
	m_APDData.nAlignMarkY2 = 4;
	m_APDData.nInspMode = 5;
	//SetAPD(apdData);
	SetAPDData();
}

void CDialogInline::SetStatus(typeEQPStatusDataPara statusData)
{
	WriteLog(_T("SetStatus Start"));
	m_EQPStatus = statusData;
	SetBitOModeOnline(m_EQPStatus.bOnline);
	SetEStatus(m_EQPStatus.nStatusRunIdleDown);
	SetBitEStatusGlassInPrcs(m_EQPStatus.bGlassInProcess);
	SetBitEStatusGlassExist(m_EQPStatus.bGlassExistInUnit);
	SetBitEStatusGlassPosition(m_EQPStatus.bGlassExistInUnit);

	if(m_EQPStatus.bGlassExistInUnit)
	{
		SetEStatus(0);
	}

	SetBitEStatusIonStatus(m_EQPStatus.bIonizer);
	SetBitOModeUnitAuto(m_EQPStatus.bUnitAutoMode);
	SetBitOModeOperation_AutoOrManual(!m_EQPStatus.nUnitOpeationMode);
	SetCurrentEquipmentRecipe(m_EQPStatus.nCurrenRecipeNum);
	SetBitEStatusGlassCnt(m_EQPStatus.nGlassCountInUnit);
	SetBitEStatusGlassPosiCnt(m_EQPStatus.nPutInfoPossibleCount);
	m_SelectRecipeData.RecipeData.nRecipeNum = m_EQPStatus.nCurrenRecipeNum;
	WriteLog(_T("SetStatus End"));
}

void CDialogInline::SetStatusReport(BOOL isOn)
{
	WriteLog(_T("SetStatusReport Start"));
	if(isOn)
	{
		m_EQPStatus.bECSConnect = TRUE;
		for(int i = 0; i < 999; i++)
		{
			if(m_RecipeData[i].bRecipeNum)
			{
				SetRecipeNum(i + 1, TRUE);
			}
			else
			{
				SetRecipeNum(i + 1, FALSE);
			}
		}
		SetRecipeListChange(TRUE);
	}
	else
	{
		m_EQPStatus.bECSConnect = FALSE;
	}

	SetBitOModeOnline(m_EQPStatus.bOnline);
	SetEStatus(m_EQPStatus.nStatusRunIdleDown);
	SetBitEStatusGlassInPrcs(m_EQPStatus.bGlassInProcess);
	SetBitEStatusGlassExist(m_EQPStatus.bGlassExistInUnit);
	SetBitEStatusIonStatus(m_EQPStatus.bIonizer);
	SetBitOModeUnitAuto(m_EQPStatus.bUnitAutoMode);
	SetBitOModeOperation_AutoOrManual(m_EQPStatus.nUnitOpeationMode);
	SetCurrentEquipmentRecipe(m_EQPStatus.nCurrenRecipeNum);
	SetBitEStatusGlassCnt(m_EQPStatus.nGlassCountInUnit);
	SetBitEStatusGlassPosiCnt(m_EQPStatus.nPutInfoPossibleCount);
	WriteLog(_T("SetStatusReport End"));
}

CString CDialogInline::GetErrorString()
{
	return m_strErrorString;
}

void CDialogInline::UpdateTimeoutTrigger(BOOL isUpdate)
{
	if(isUpdate)
	{
		UpdateData(TRUE);
		m_nTimeout[TIMEOUT_RECIPE_BODY_DATA_REQUEST] = m_edit_nTimeout_0;
		m_nTimeout[TIMEOUT_EQP_RECIPE_REQUEST_CONFIRM] = m_edit_nTimeout_1;
		m_nTimeout[TIMEOUT_RECIPE_VARIOUS_DATA_REQUEST] = m_edit_nTimeout_2;
		m_nTimeout[TIMEOUT_RECIPE_VARIOUS_DATA_REPORT] = m_edit_nTimeout_3;
		m_nTimeout[TIMEOUT_LOST_GLASS_DATA_REQUEST] = m_edit_nTimeout_4;
		m_nTimeout[TIMEOUT_GLASS_DATA_REQUEST] = m_edit_nTimeout_5;
		m_nTimeout[TIMEOUT_SEQUENCE] = m_edit_nTimeout_6;
		m_nTimeout[TIMEOUT_ROBOTBUSY] = m_edit_nTimeout_7;

		m_nTrigger[TRIGGER_RECIPE_NUMBER_LIST_CHANGE] = m_edit_nTrigger_0;
		m_nTrigger[TRIGGER_SCRAP_GLASS_DATA_REPORT] = m_edit_nTrigger_1;
		m_nTrigger[TRIGGER_GLASS_DATA_CHANGE_REPORT] = m_edit_nTrigger_2;
		m_nTrigger[TRIGGER_GLASS_APD_DATA_REPORT] = m_edit_nTrigger_3;
		m_nTrigger[TRIGGER_LIGHT_ALARM] = m_edit_nTrigger_4;
		m_nTrigger[TRIGGER_HEAVY_ALARM] = m_edit_nTrigger_5;
		m_nTrigger[TRIGGER_SEQUENCE_DELAY] = m_edit_nTrigger_6;
		m_nTrigger[TRIGGER_TPD_DATA_DELAY] = m_edit_nTrigger_7;

		m_nReport_RecipeCnt = m_edit_nReipeReportCnt;
		m_bUnloadOnly_Exchange = m_check_bUnload_Exchang;
	}
	else
	{
		m_edit_nTimeout_0 = m_nTimeout[TIMEOUT_RECIPE_BODY_DATA_REQUEST];
		m_edit_nTimeout_1 = m_nTimeout[TIMEOUT_EQP_RECIPE_REQUEST_CONFIRM];
		m_edit_nTimeout_2 = m_nTimeout[TIMEOUT_RECIPE_VARIOUS_DATA_REQUEST];
		m_edit_nTimeout_3 = m_nTimeout[TIMEOUT_RECIPE_VARIOUS_DATA_REPORT];
		m_edit_nTimeout_4 = m_nTimeout[TIMEOUT_LOST_GLASS_DATA_REQUEST];
		m_edit_nTimeout_5 = m_nTimeout[TIMEOUT_GLASS_DATA_REQUEST];
		m_edit_nTimeout_6 = m_nTimeout[TIMEOUT_SEQUENCE];
		m_edit_nTimeout_7 = m_nTimeout[TIMEOUT_ROBOTBUSY];

		m_edit_nTrigger_0 = m_nTrigger[TRIGGER_RECIPE_NUMBER_LIST_CHANGE];
		m_edit_nTrigger_1 = m_nTrigger[TRIGGER_SCRAP_GLASS_DATA_REPORT];
		m_edit_nTrigger_2 = m_nTrigger[TRIGGER_GLASS_DATA_CHANGE_REPORT];;
		m_edit_nTrigger_3 = m_nTrigger[TRIGGER_GLASS_APD_DATA_REPORT];;
		m_edit_nTrigger_4 = m_nTrigger[TRIGGER_LIGHT_ALARM];
		m_edit_nTrigger_5 = m_nTrigger[TRIGGER_HEAVY_ALARM];
		m_edit_nTrigger_6 = m_nTrigger[TRIGGER_SEQUENCE_DELAY];
		m_edit_nTrigger_7 = m_nTrigger[TRIGGER_TPD_DATA_DELAY];

		m_edit_nReipeReportCnt = m_nReport_RecipeCnt;
		m_check_bUnload_Exchang = m_bUnloadOnly_Exchange;
		UpdateData(FALSE);
	}
}

BOOL CDialogInline::SaveTemp()
{
	WriteLog(_T("SaveTemp Start!"));
	BOOL rslt = TRUE;
	CString key, dat;
	CString file_path, temp_path;
	
	file_path.Format(_T("%S\\Inline_temp.temp"), DATABASE_PATH);  //pkh
	
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
		WriteLog(_T("SaveTemp Fail!"));
		return FALSE;
	}
	else
	{
		CloseHandle(fd);

		dat.Format(_T(""));
		key.Format(_T(""));
		key.Format(_T("TEMP_PARAMETER_0"));
		dat.Format(_T("%d"), m_LoaderGlassData.nGlassCode);
		WritePrivateProfileString(_T("TEMP_PARAMETER"), key, dat, file_path);

		dat.Format(_T(""));
		key.Format(_T(""));
		key.Format(_T("TEMP_PARAMETER_1"));
		dat.Format(_T("%S"), m_LoaderGlassData.strLotID);
		WritePrivateProfileString(_T("TEMP_PARAMETER"), key, dat, file_path);

		dat.Format(_T(""));
		key.Format(_T(""));
		key.Format(_T("TEMP_PARAMETER_2"));
		dat.Format(_T("%S"), m_LoaderGlassData.strGlassID);
		WritePrivateProfileString(_T("TEMP_PARAMETER"), key, dat, file_path);
	}
	fd = 0;
	WriteLog(_T("SaveTemp End!"));
	return TRUE;
}

BOOL CDialogInline::SaveTimeoutTrigger()
{
	WriteLog(_T("SaveTimeoutTrigger Start!"));
	BOOL rslt = TRUE;
	CString key, dat;
	CString file_path, temp_path;
	
	file_path.Format(_T("%S\\Inline_TimeoutTrigger.par"), DATABASE_PATH);
	
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
		WriteLog(_T("SaveTimeoutTrigger Fail!"));
		return FALSE;
	}
	else
	{
		CloseHandle(fd);

		for(int i = 0; i < 20; i++)
		{
			key.Empty();
			key.Format(_T("INLINE_TIMEOUT_%02d"), i);
			dat.Empty();
			dat.Format(_T("%d"), m_nTimeout[i]);
			WritePrivateProfileString(_T("INLINE_TIMEOUT_TRIGGER"), key, dat, file_path);

			key.Empty();
			key.Format(_T("INLINE_TRIGGER_%02d"), i);
			dat.Empty();
			dat.Format(_T("%d"), m_nTrigger[i]);
			WritePrivateProfileString(_T("INLINE_TIMEOUT_TRIGGER"), key, dat, file_path);
		}

		key.Empty();
		key.Format(_T("INLINE_RECIPE_REPORT_CNT"));
		dat.Empty();
		dat.Format(_T("%d"), m_nReport_RecipeCnt);
		WritePrivateProfileString(_T("INLINE_PARAMETER"), key, dat, file_path);

		key.Empty();
		key.Format(_T("INLINE_UNLOAD_ONLY_EXCHANGE"));
		dat.Empty();
		dat.Format(_T("%d"), m_bUnloadOnly_Exchange);
		WritePrivateProfileString(_T("INLINE_PARAMETER"), key, dat, file_path);
	}
	fd = 0;
	WriteLog(_T("SaveTimeoutTrigger End!"));
	return TRUE;
}

BOOL CDialogInline::LoadTemp()
{
	WriteLog(_T("LoadTemp Start!"));
	BOOL rslt = TRUE;
	CString key, dat, val;
	CString file_path, temp_path;
	char temp[256];

	file_path.Format(_T("%S\\Inline_temp.temp"), DATABASE_PATH); //pkh

	
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
		WriteLog(_T("LoadTemp Fail!"));
		return FALSE;
	}
	else
	{
		CloseHandle(fd);

		key.Empty();
		key.Format(_T("TEMP_PARAMETER_0"));
		//GetPrivateProfileString(_T("TEMP_PARAMETER"), key, _T("0"), (LPWSTR)(LPCWSTR)temp, 256, file_path);
		//dat.Format(_T("%S"), temp);
		//m_LoaderGlassData.nGlassCode = _wtoi(dat);
		m_LoaderGlassData.nGlassCode = GetPrivateProfileInt (_T("TEMP_PARAMETER"), key, NULL, file_path);
		
		key.Empty();
		key.Format(_T("TEMP_PARAMETER_1"));
		GetPrivateProfileString(_T("TEMP_PARAMETER"), key, _T("0"), (LPWSTR)(LPCWSTR)temp, 256, file_path);
		//dat.Format(_T("%S"), temp);
		sprintf(m_LoaderGlassData.strLotID, "%S", temp);

		key.Empty();
		key.Format(_T("TEMP_PARAMETER_2"));
		GetPrivateProfileString(_T("TEMP_PARAMETER"), key, _T("0"), (LPWSTR)(LPCWSTR)temp, 256, file_path);
		//dat.Format(_T("%S"), temp);
		sprintf(m_LoaderGlassData.strGlassID, "%S", temp);
	}
	fd = 0;
	WriteLog(_T("LoadTemp End!"));
	return TRUE;
}

BOOL CDialogInline::LoadTimeoutTrigger()
{
	WriteLog(_T("LoadTimeoutTrigger Start!"));
	BOOL rslt = TRUE;
	CString key, dat;
	CString file_path, temp_path;
	char temp[256];

	file_path.Format(_T("%S\\Inline_TimeoutTrigger.par"), DATABASE_PATH);

	
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
		WriteLog(_T("LoadTimeoutTrigger Fail!"));
		return FALSE;
	}
	else
	{
		CloseHandle(fd);

		for(int i = 0; i < 20; i++)
		{
			key.Empty();
			key.Format(_T("INLINE_TIMEOUT_%02d"), i);
			GetPrivateProfileString(_T("INLINE_TIMEOUT_TRIGGER"), key, _T("0"), (LPWSTR)(LPCWSTR)dat, 256, file_path);
			//dat.Format(_T("%S"), temp);
			m_nTimeout[i] = _wtoi(dat);

			key.Empty();
			key.Format(_T("INLINE_TRIGGER_%02d"), i);
			GetPrivateProfileString(_T("INLINE_TIMEOUT_TRIGGER"), key, _T("0"), (LPWSTR)(LPCWSTR)dat, 256, file_path);
			//dat.Format(_T("%S"), temp);
			m_nTrigger[i] = _wtoi(dat);
		}

		key.Empty();
		key.Format(_T("INLINE_RECIPE_REPORT_CNT"));
		GetPrivateProfileString(_T("INLINE_PARAMETER"), key, _T("0"), (LPWSTR)(LPCWSTR)dat, 256, file_path);
		//dat.Format(_T("%S"), temp);
		m_nReport_RecipeCnt = _wtoi(dat);

		key.Empty();
		key.Format(_T("INLINE_UNLOAD_ONLY_EXCHANGE"));
		GetPrivateProfileString(_T("INLINE_PARAMETER"), key, _T("0"), (LPWSTR)(LPCWSTR)dat, 256, file_path);
		//dat.Format(_T("%S"), temp);
		m_bUnloadOnly_Exchange = _wtoi(dat);
	}
	fd = 0;
	WriteLog(_T("LoadTimeoutTrigger End!"));
	return TRUE;
}

void CDialogInline::InitSeqBit(int type)
{
	m_nInterLockError = -1;
	if(type == LOAD_ONLY)
	{
		m_PLCComm->SetBit(PLC_DEVB_LOAD_ENABLE, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_ENABLE, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_LOAD_NORMAL_STATUS, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_NORMAL_STATUS, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_LOAD_NORMAL_STATUS, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_REQUEST, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_LOAD_COMPLETE_CONFIRM, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_COMPLETE_CONFIRM, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_LOAD_WAIT_OPTION, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_WAIT_OPTION, FALSE);
	}
	else if(type == UNLOAD_ONLY)
	{
		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_ENABLE, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_ENABLE, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_NORMAL_STATUS, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_NORMAL_STATUS, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_REQUEST, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_REQUEST, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_COMPLETE_CONFIRM, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_COMPLETE_CONFIRM, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_WAIT_OPTION, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_WAIT_OPTION, FALSE);
	}
	else if(type == EXCHANGE)
	{
		m_PLCComm->SetBit(PLC_DEVB_LOAD_ENABLE, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_ENABLE, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_LOAD_NORMAL_STATUS, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_NORMAL_STATUS, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_LOAD_REQUEST, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_REQUEST, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_LOAD_COMPLETE_CONFIRM, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_COMPLETE_CONFIRM, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_LOAD_WAIT_OPTION, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_WAIT_OPTION, FALSE);

		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_ENABLE, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_ENABLE, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_NORMAL_STATUS, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_NORMAL_STATUS, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_REQUEST, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_REQUEST, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_COMPLETE_CONFIRM, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_COMPLETE_CONFIRM, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_WAIT_OPTION, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_WAIT_OPTION, FALSE);
	}
	else if(type == DOWN_ALARM)
	{
		m_PLCComm->SetBit(PLC_DEVB_LOAD_ENABLE, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_ENABLE, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_GLASS_DATA_REQUEST, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_LOAD_NORMAL_STATUS, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_NORMAL_STATUS, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_LOAD_REQUEST, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_REQUEST, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_LOAD_COMPLETE_CONFIRM, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_COMPLETE_CONFIRM, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_LOAD_WAIT_OPTION, FALSE);
		SetLoaderBtn(PLC_DEVB_LOAD_WAIT_OPTION, FALSE);

		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_ENABLE, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_ENABLE, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_GLASS_DATA_REPORT, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_NORMAL_STATUS, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_NORMAL_STATUS, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_REQUEST, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_REQUEST, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_COMPLETE_CONFIRM, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_COMPLETE_CONFIRM, FALSE);
		m_PLCComm->SetBit(PLC_DEVB_UNLOAD_WAIT_OPTION, FALSE);
		SetUnLoaderBtn(PLC_DEVB_UNLOAD_WAIT_OPTION, FALSE);
	}
}

int CDialogInline::GetInterLockError()
{
	return m_nInterLockError + 240;
}

int CDialogInline::GetInterLockErrorLevel()
{
	return m_nInterLockErrorLevel;
}


void CDialogInline::OnBnClickedButtonGlassTpdData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nTPDWriteDelay = 0;
	if(!m_bTest)
	{
		m_TPDData.nChuckVac = 65530;
		m_TPDData.nLIFT_AXIS_TQ = 65530;
		m_TPDData.nRV_Y_Axis_TQ = 65530;
		m_TPDData.nScan_X_Axis_TQ = 65530;
		m_TPDData.nSolBlw = 65530;
		m_bTest = TRUE;
		SetTimer(TIMER_TPD_DATA_REPORT, 100, NULL);
	}
	else
	{
		m_bTest= FALSE;
		KillTimer(TIMER_TPD_DATA_REPORT);
	}
}

void CDialogInline::OnBnClickedButtonLightAlarm2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetLightAlarmReport(FALSE, 99);
}

void CDialogInline::OnBnClickedButtonHevyAlarm2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetHeavyAlarmReport(FALSE, 100);
}


void CDialogInline::OnBnClickedButtonRecipeBody()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_RequestRecipeVariousData.nRecipeCEID = 5;
	for(int i = 0; i < 10; i++)
	{
		m_RecipeReport[i].nRecipeNumber = i;
		m_RecipeReport[i].nRecipeLevel = 3;
		m_RecipeReport[i].cRecipeType = 'M';
		m_RecipeReport[i].wRecipeVersion[0] = 0;
		m_RecipeReport[i].wRecipeVersion[1] = 0;
		m_RecipeReport[i].wRecipeVersion[2] = 1;
		m_RecipeReport[i].nRecipeStatus = (i % 3) + 1;
		m_RecipeReport[i].nUnitNumber = 0;
	}
	int nRecipeIdx = DEVW_RBD_RECIPE_BODY_V1;
	for(int i = 0; i< 992; i++)
	{
		m_MelsecListPara[i + nRecipeIdx].wValue = 1 + i;
	}
	WriteRecipeVariousData(m_RequestRecipeVariousData.nRecipeCEID);
}


void CDialogInline::OnBnClickedButtonRecipeParameter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_RequestRecipeVariousData.nRecipeCEID = 3;
	int nRecipeIdx = DEVW_RP_RECIPE_PARAMETER_V1;
	for(int i = 0; i< 992; i++)
	{
		m_MelsecListPara[i + nRecipeIdx].wValue = 1 + i;
	}
	WriteRecipeVariousData(m_RequestRecipeVariousData.nRecipeCEID);
}



void CDialogInline::OnBnClickedButton1()
{
	int i = 0;

	switch(m_nTest)
	{
	case 0:
		i = PLC_DEVB_LOAD_ENABLE;
		break;
	case 1:
		i = PLC_DEVB_LOAD_GLASS_DATA_REQUEST;
		break;
	case 2:
		i = PLC_DEVB_LOAD_NORMAL_STATUS;
		break;
	case 3:
		i = PLC_DEVB_LOAD_REQUEST;
		break;
	case 4:
		i = PLC_DEVB_LOAD_COMPLETE_CONFIRM;
		break;
	case 5:
		i = PLC_DEVB_LOAD_WAIT_OPTION;
		break;
	case 6:
		i = PLC_DEVB_UNLOAD_ENABLE;
		break;
	case 7:
		i = PLC_DEVB_UNLOAD_GLASS_DATA_REPORT;
		break;
	case 8:
		i = PLC_DEVB_UNLOAD_NORMAL_STATUS;
		break;
	case 9:
		i = PLC_DEVB_UNLOAD_REQUEST;
		break;
	case 10:
		i = PLC_DEVB_UNLOAD_COMPLETE_CONFIRM;
		break;
	case 11:
		i = PLC_DEVB_UNLOAD_WAIT_OPTION;
		break;
	}
	
	if(m_nTest < 6)
	{
		SetLoaderBtn(i, m_bTest);
	}
	else
	{
		SetUnLoaderBtn(i, m_bTest);
	}
	m_nTest++;
	if(m_nTest > 11)
	{
		m_nTest = 0;
		m_bTest = !m_bTest;
	}

}

int CDialogInline::GlassDataComp(BOOL *glass_exist)
{
	int rslt = 0;
	CString strLot_id, strGlass_id, strMainLot_id, strMainGlass_id;
	typeGlassDataPara loaderGlass;
	
#ifdef MACHINE
	//ReadLoaderGlassData(&loaderGlass);
	loaderGlass = GetUpdateGlassData();
#else
	loaderGlass = GetUpdateGlassData();
#endif

	strMainLot_id.Format(_T("%S"), loaderGlass.strLotID);
	strMainGlass_id.Format(_T("%S"), loaderGlass.strGlassID);

	strLot_id.Format(_T("%S"), m_UnloaderGlassData.strLotID);
	strGlass_id.Format(_T("%S"), m_UnloaderGlassData.strGlassID);

	
	if(*glass_exist)//glass 센서 감지 O
	{
		if(!strMainLot_id.Compare(strLot_id) && !strMainGlass_id.Compare(strGlass_id) && rslt == 0)
		{
			rslt = 0;//Glass Data 있음
		}

		if((strMainLot_id.IsEmpty() || !strMainLot_id.Compare(_T("0"))) && ((strMainGlass_id.IsEmpty() || !strMainLot_id.Compare(_T("0")))
		&& strLot_id.IsEmpty() && strGlass_id.IsEmpty()) && rslt == 0)
		{
			rslt = 2;//Glass Data 전부 없음
		}

		if((strMainLot_id.IsEmpty() || strMainGlass_id.IsEmpty() || strLot_id.IsEmpty() || strGlass_id.IsEmpty()) && rslt == 0)
		{
			rslt = 3;//Glass Data 일부 없음
		}

		if((0 != strMainLot_id.Compare(strLot_id)) || (0 != strMainGlass_id.Compare(strGlass_id)) && rslt == 0)
		{
			rslt = 4;//Glass Data 다름
		}
	}
	else//glass 센서 감지 X
	{
		if((strMainLot_id.IsEmpty() || !strMainLot_id.Compare(_T("0"))) && ((strMainGlass_id.IsEmpty() || !strMainLot_id.Compare(_T("0")))
		&& strLot_id.IsEmpty() && strGlass_id.IsEmpty()) && rslt == 0)
		{
			rslt = 1;//Glass Data 전부 없음
		}

		if(!strMainLot_id.Compare(strLot_id) && !strMainGlass_id.Compare(strGlass_id) && rslt == 0)
		{
			rslt = 5;//Glass Data 전부 있음
		}

		if((!strMainLot_id.IsEmpty() || !strMainGlass_id.IsEmpty() || !strLot_id.IsEmpty() || !strGlass_id.IsEmpty()) && rslt == 0)
		{
			rslt = 6;//Glass Data 일부 있음
		}
	}

	//rslt 0~1 OK
	//rslt 2~6 NG

	return rslt;
}

void CDialogInline::SetLoaderBtn(int idx, BOOL val)
{
	switch(idx)
	{
	case PLC_DEVB_LOAD_ENABLE:
		m_btn_LoaderEnable.SetValue(val);
		break;
	case PLC_DEVB_LOAD_GLASS_DATA_REQUEST:
		m_btn_LoaderGlassDataRequest.SetValue(val);
		break;
	case PLC_DEVB_LOAD_NORMAL_STATUS:
		m_btn_LoaderNormalStatus.SetValue(val);
		break;
	case PLC_DEVB_LOAD_REQUEST:
		m_btn_LoaderLoadRequest.SetValue(val);
		break;
	case PLC_DEVB_LOAD_COMPLETE_CONFIRM:
		m_btn_LoaderLoadCompleteConfirm.SetValue(val);
		break;
	case PLC_DEVB_LOAD_WAIT_OPTION:
		m_btn_LoaderLoadWait.SetValue(val);
		break;
	}	
}

void CDialogInline::SetUnLoaderBtn(int idx, BOOL val)
{
	switch(idx)
	{
	case PLC_DEVB_UNLOAD_ENABLE:
		m_btn_UnLoaderEnable.SetValue(val);
		break;
	case PLC_DEVB_UNLOAD_GLASS_DATA_REPORT:
		m_btn_UnLoaderGlassDataReport.SetValue(val);
		break;
	case PLC_DEVB_UNLOAD_NORMAL_STATUS:
		m_btn_UnLoaderNormalStatus.SetValue(val);
		break;
	case PLC_DEVB_UNLOAD_REQUEST:
		m_btn_UnLoaderUnLoadRequest.SetValue(val);
		break;
	case PLC_DEVB_UNLOAD_COMPLETE_CONFIRM:
		m_btn_UnLoaderUnloadCompleteConfirm.SetValue(val);
		break;
	case PLC_DEVB_UNLOAD_WAIT_OPTION:
		m_btn_UnLoaderUnLoadWait.SetValue(val);
		break;
	}
}

void CDialogInline::TimeOutParameterEnable(int access_type)
{
	switch(access_type)
	{
	case 0:
	case 1:
		GetDlgItem(IDC_EDIT_TIMEOUT_0)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TIMEOUT_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TIMEOUT_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TIMEOUT_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TIMEOUT_4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TIMEOUT_5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TIMEOUT_6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TIMEOUT_7)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_TRIGGER_0)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TRIGGER_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TRIGGER_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TRIGGER_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TRIGGER_4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TRIGGER_5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TRIGGER_6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TRIGGER_7)->EnableWindow(FALSE);
		break;
	case 2:
	case 3:
		GetDlgItem(IDC_EDIT_TIMEOUT_0)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TIMEOUT_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TIMEOUT_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TIMEOUT_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TIMEOUT_4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TIMEOUT_5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TIMEOUT_6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TIMEOUT_7)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_TRIGGER_0)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TRIGGER_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TRIGGER_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TRIGGER_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TRIGGER_4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TRIGGER_5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TRIGGER_6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TRIGGER_7)->EnableWindow(TRUE);
		break;
	}
}

void CDialogInline::OnBnClickedButtonDeleteGlassData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	memset(&m_UpdateGlassData, NULL, sizeof(typeGlassDataPara));

	m_LoaderGlassData = m_UpdateGlassData;
	m_UnloaderGlassData = m_UpdateGlassData;
	GlassDataUpdate(FALSE);
}

//20160725 sc
CString CDialogInline::GetLotInTime(){return m_LotInTime.strInTime;}

int CDialogInline::GetSlotNumber()
{
	int rslt = 0;
	WORD temp1 , temp3;;
	BOOL temp2[16];
	temp1 = m_UpdateGlassData.nGlassCode;

	temp3 = temp1 << 8;
	rslt = temp3 >> 8;

	return rslt;
}

