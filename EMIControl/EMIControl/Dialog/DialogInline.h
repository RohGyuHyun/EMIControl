/*
Test 사항
various data write
-> recipe body
-> recipe parameter

scrap glass data
lost glass data
change glass data
apd data
tpd data
*/

#pragma once
#include "Inline_define.h"
#include "Inline_typedef.h"
#include "include\PLCComm.h"
#include "share.h"
#include "afxwin.h"
#include "btnenhctrl.h"
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

enum JUDGE_CODE {

};
// DEV_W
enum PLC_DEVW_ADDRESS_AREA_1 {
	PLC_DEVW_BASE_ADDRESS1								= 0x2000,
	
	PLC_DEVW_USD_TPD_REPORT_NUMBER						= PLC_DEVW_BASE_ADDRESS1,
	PLC_DEVW_USD_TPD_UPDATE_TIME						= 0x2001,
	PLC_DEVW_USD_IONIZER_STATUS_0_15					= 0x2006,
	PLC_DEVW_USD_IONIZER_STATUS_16_31					= 0x2007,
	PLC_DEVW_USD_LOT_JUDGE_CODE							= 0x2008,
	PLC_DEVW_USD_UNIT_DOWN_ALARM_CODE					= 0x2009,
	PLC_DEVW_USD_REQUEST_HOST_RECIPE_NUMBER				= 0x200A,
	PLC_DEVW_USD_CURRENT_EQUIPMENT_RECIPE_NUMBER		= 0x200B,
	PLC_DEVW_USD_GLASS_COUNT_IN_UNIT					= 0x2001C,
	PLC_DEVW_USD_PUT_INTO_POSSIBLE_COUNT				= 0x200D,
	PLC_DEVW_USD_LIGHT_ALARM_CODE						= 0x200E,
	PLC_DEVW_USD_HEAVY_ALARM_CODE						= 0x200F,

	PLC_DEVW_RGD_LOT_ID									= 0x2010, //8WORD
	PLC_DEVW_RGD_PROCESSING_CODE						= 0x2018, //4WORD
	PLC_DEVW_RGD_LOT_SPECIFIC_DATA						= 0x201C, //4WORD

	PLC_DEVW_RGD_HOST_RECIPE_NUMBER						= 0x2020,
	PLC_DEVW_RGD_GLASS_TYPE								= 0x2021,
	PLC_DEVW_RGD_GLASS_CODE_LOT_SLOT					= 0x2022,
	PLC_DEVW_RGD_GLASS_ID								= 0x2023, //8WORD
	PLC_DEVW_RGD_GLASS_JUDGE							= 0x202B,
	PLC_DEVW_RGD_GLASS_SPECIFIC_DATA					= 0x202C, //4WORD

	PLC_DEVW_RGD_GLASS_ADD_DATA							= 0x2030, //6WORD
	PLC_DEVW_RGD_PREVIOUS_UNIT_PROCESSING_DATA			= 0x2036, //4WORD
	PLC_DEVW_RGD_GLASS_PROCESSING_DATA					= 0x203A, //3WORD
	PLC_DEVW_RGD_GLASS_ROUTING_DATA						= 0x203D, //3WORD
	
	PLC_DEVW_RP_REPORT_PARAMETER_CEID					= 0x2040,
	PLC_DEVW_RP_REPORT_PARAMETER_V1						= 0x2041,
	PLC_DEVW_RP_REPORT_PARAMETER_V2						= 0x2042,
	PLC_DEVW_RP_REPORT_PARAMETER_V3						= 0x2043,
	PLC_DEVW_RP_REPORT_PARAMETER_V4						= 0x2044,
	PLC_DEVW_RP_REPORT_PARAMETER_V5						= 0x2045,
	PLC_DEVW_RP_REPORT_PARAMETER_V6						= 0x2046,
	PLC_DEVW_RP_REPORT_PARAMETER_V7						= 0x2047,
	PLC_DEVW_RP_REPORT_PARAMETER_V8						= 0x2048,
	PLC_DEVW_RP_REPORT_PARAMETER_V9						= 0x2049,
	PLC_DEVW_RP_REPORT_PARAMETER_V10					= 0x204A,
	PLC_DEVW_RP_REPORT_PARAMETER_V11					= 0x204B,
	PLC_DEVW_RP_REPORT_PARAMETER_V12					= 0x204C,
	PLC_DEVW_RP_REPORT_PARAMETER_V13					= 0x204D,
	PLC_DEVW_RP_REPORT_PARAMETER_V14					= 0x204E,
	PLC_DEVW_RP_REPORT_PARAMETER_V15					= 0x204F,

	PLC_DEVW_GSD_GLASS_DATA_REQUEST_GLASS_ID			= 0x2070, //8WORD
	PLC_DEVW_GSD_GLASS_DATA_REQUEST_GLASS_CODE			= 0x2078,
	PLC_DEVW_GSD_GLASS_DATA_REQUEST_OPTION				= 0x2079,

	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_1_15			= 0x2080,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_16_31			= 0x2081,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_32_47			= 0x2082,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_48_63			= 0x2083,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_64_79			= 0x2084,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_80_95			= 0x2085,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_96_111			= 0x2086,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_112_127		= 0x2087,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_128_143		= 0x2088,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_144_159		= 0x2089,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_160_175		= 0x208A,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_176_191		= 0x208B,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_192_207		= 0x208C,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_208_223		= 0x208D,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_224_239		= 0x208E,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_240_255		= 0x208F,

	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_256_271		= 0x2090,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_272_287		= 0x2091,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_288_303		= 0x2092,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_304_319		= 0x2093,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_320_335		= 0x2094,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_336_351		= 0x2095,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_352_367		= 0x2096,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_368_383		= 0x2097,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_384_399		= 0x2098,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_400_415		= 0x2099,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_416_431		= 0x209A,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_432_447		= 0x209B,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_448_463		= 0x209C,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_464_479		= 0x209D,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_480_495		= 0x209E,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_496_511		= 0x209F,

	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_512_527		= 0x20A0,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_528_543		= 0x20A1,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_544_559		= 0x20A2,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_560_575		= 0x20A3,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_576_591		= 0x20A4,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_592_607		= 0x20A5,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_608_623		= 0x20A6,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_624_639		= 0x20A7,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_640_655		= 0x20A8,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_656_671		= 0x20A9,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_672_687		= 0x20AA,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_688_703		= 0x20AB,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_704_719		= 0x20AC,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_720_735		= 0x20AD,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_736_751		= 0x20AE,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_752_767		= 0x20AF,

	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_768_783		= 0x20B0,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_784_799		= 0x20B1,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_800_815		= 0x20B2,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_816_831		= 0x20B3,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_832_847		= 0x20B4,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_848_863		= 0x20B5,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_864_879		= 0x20B6,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_880_895		= 0x20B7,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_896_911		= 0x20B8,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_912_927		= 0x20B9,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_928_943		= 0x20BA,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_944_959		= 0x20BB,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_960_975		= 0x20BC,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_976_991		= 0x20BD,
	PLC_DEVW_ERN_EQUIPMENT_RECIPE_NUMBER_992_999		= 0x20BE,

	PLC_DEVW_POG_POSITION_1_GLASS_CODE					= 0x20C0,
	PLC_DEVW_POG_POSITION_2_GLASS_CODE					= 0x20C1,

	PLC_DEVW_UIGD_LOT_ID								= 0x2110, //8WORD
	PLC_DEVW_UIGD_PROCESSING_CODE						= 0x2118, //4WORD
	PLC_DEVW_UIGD_LOT_SPECIFIC_DATA						= 0x211C, //4WORD

	PLC_DEVW_UIGD_HOST_RECIPE_NUMBER					= 0x2120,
	PLC_DEVW_UIGD_GLASS_TYPE							= 0x2121,
	PLC_DEVW_UIGD_GLASS_CODE_LOT_SLOT					= 0x2122,
	PLC_DEVW_UIGD_GLASS_ID								= 0x2123, //8WORD
	PLC_DEVW_UIGD_GLASS_JUDGE							= 0x212B,
	PLC_DEVW_UIGD_GLASS_SPECIFIC_DATA					= 0x212C, //4WORD

	PLC_DEVW_UIGD_GLASS_ADD_DATA						= 0x2130, //6WORD
	PLC_DEVW_UIGD_PREVIOUS_UNIT_PROCESSING_DATA			= 0x2136, //4WORD
	PLC_DEVW_UIGD_GLASS_PROCESSING_DATA					= 0x213A, //3WORD
	PLC_DEVW_UIGD_GLASS_ROUTING_DATA					= 0x213D, //3WORD

	PLC_DEVW_TPD_DATA_V1								= 0x2200,
	PLC_DEVW_TPD_REPORT_NUMBER							= 0x22FF,

	PLC_DEVW_APD_DATA_V1								= 0x2300, //V1280
	

	PLC_DEVW_END_ADDRESS1								= 0x2FFF,
};

enum PLC_DEVW_ADDRESS_AREA_2 {
	PLC_DEVW_BASE_ADDRESS2								= 0x14000,
	
	PLC_DEVW_RBR_RECIPE_REPORT_CEID						= PLC_DEVW_BASE_ADDRESS2,
	PLC_DEVW_RBR_RECIPE_NUMBER							= 0x14001,
	PLC_DEVW_RBR_RECIPE_CONFIRM_ACK						= 0x14002,
	PLC_DEVW_RBR_RECIPE_LEVEL							= 0x14003,
	PLC_DEVW_RBR_RECIPE_TYPE							= 0x14004,
	PLC_DEVW_RBR_RECIPE_VERSION							= 0x14005, //3WORD

	PLC_DEVW_ESP_EQUIPMENT_SPECIFIC_DATA_V1				= 0x14010, //V16

	PLC_DEVW_RBD_RECIPE_BODY_V1							= 0x14020, //V992

	PLC_DEVW_RCR_REQUEST_CONFIRM_CEID					= 0x14400,
	PLC_DEVW_RCR_REQUEST_CONFIRM_ACK					= 0x14401,
	PLC_DEVW_RCR_RECIPE_REPORT_CEID						= 0x14404,
	PLC_DEVW_RCR_RECIPE_REPORT_CEID_ORDER				= 0x14405,
	PLC_DEVW_RCR_RECIPE_TOTAL_COUNT						= 0x14406,
	PLC_DEVW_RCR_RECIPE_REPORT_COUNT					= 0x14407,
	PLC_DEVW_RCR_RECIPE_COMMAND							= 0x14408,
	PLC_DEVW_RCR_RECIPE_NUMBER							= 0x14409,
	PLC_DEVW_RCR_RECIPE_LEVEL							= 0x1440A,
	PLC_DEVW_RCR_RECIPE_TYPE							= 0x1440B,
	PLC_DEVW_RCR_RECIPE_VERSION							= 0x1440C, //3WORD

	PLC_DEVW_ESD_EQUIPMENT_SPECIFIC_DATA_V1				= 0x14410, //V16

	PLC_DEVW_RP_RECIPE_PARAMETER_V1						= 0x14420, //V992
	
	PLC_DEVW_END_ADDRESS2								= 0x147FF,
};

// DEV_B
enum PLC_DEVB_ADDRESS {
	PLC_DEVB_BASE_ADDRESS								= 0x1000,

	PLC_DEVB_ONLINE										= PLC_DEVB_BASE_ADDRESS,
	PLC_DEVB_AUTO_MODE									= 0x1005,
	PLC_DEVB_OPERATION_CYCLE_STOP						= 0x1007,
	PLC_DEVB_RUN										= 0x1008,
	PLC_DEVB_IDLE										= 0x1009,
	PLC_DEVB_DOWN										= 0x100A,
	PLC_DEVB_GLASS_IN_PROCESSING						= 0x100B,
	PLC_DEVB_GLASS_EXIST_IN_UNIT						= 0x100C,
	PLC_DEVB_PREVIOUS_TRANSFER_ENABLE					= 0x100D,
	PLC_DEVB_LIGHT_ALARM_REPORT							= 0x100E,
	PLC_DEVB_HEAVY_ALARM_REPORT							= 0x100F,

	PLC_DEVB_MODE_AUTO									= 0x1010,
	PLC_DEVB_MODE_MANUAL								= 0x1011,

	PLC_DEVB_GLASS_APD_DATA_REPORT						= 0x1021,
	PLC_DEVB_SCRAP_GLASS_DATA_REPORT					= 0x1022,
	PLC_DEVB_LOST_GLASS_DATA_REQUEST					= 0x1023,
	PLC_DEVB_GLASS_DATA_CHANGE_REPORT					= 0x1024,
	PLC_DEVB_RECIPE_BODY_DATA_REPORT					= 0x1028,
	PLC_DEVB_EQIPMENT_RECIPE_REQUEST					= 0x102A,
	PLC_DEVB_EQP_RECIPE_LIST_CHANGE						= 0x102B,
	PLC_DEVB_RECIPE_VARIOUS_REQUEST_CONFIRM				= 0x102D,
	PLC_DEVB_RECIPE_VARIOUS_DATA_REPORT					= 0x102E,

	PLC_DEVB_POSITION1_GLASS_EXIST						= 0x10C0,
	PLC_DEVB_POSITION2_GLASS_EXIST						= 0x10C1,

	PLC_DEVB_LOAD_ENABLE								= 0x1100,
	PLC_DEVB_LOAD_GLASS_DATA_REQUEST					= 0x1101,
	PLC_DEVB_LOAD_NORMAL_STATUS							= 0x1104,
	PLC_DEVB_LOAD_REQUEST								= 0x1105,
	PLC_DEVB_LOAD_COMPLETE_CONFIRM						= 0x1106,
	PLC_DEVB_LOAD_WAIT_OPTION							= 0x110B,

	PLC_DEVB_UNLOAD_ENABLE								= 0x1110,
	PLC_DEVB_UNLOAD_GLASS_DATA_REPORT					= 0x1111,
	PLC_DEVB_UNLOAD_NORMAL_STATUS						= 0x1114,
	PLC_DEVB_UNLOAD_REQUEST								= 0x1115,
	PLC_DEVB_UNLOAD_COMPLETE_CONFIRM					= 0x1116,
	PLC_DEVB_UNLOAD_WAIT_OPTION							= 0x111B,

	PLC_DEVB_END_ADDRESS								= 0x2FFF,
};


// CDialogInline 대화 상자입니다.
class CDialogInline : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogInline)

public:
	CDialogInline(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogInline();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_INLINE_CTRL };

public:
	BOOL SetBitEStatusRun(BOOL bit);
	BOOL GetBitEStatusRun();
	BOOL SetBitEStatusIdle(BOOL bit);
	BOOL GetBitEStatusIdle();
	BOOL SetBitEStatusDown(BOOL bit);
	BOOL GetBitEStatusDown();
	BOOL SetBitEStatusGlassInPrcs(BOOL bit);
	BOOL GetBitEStatusGlassInPrcs();
	BOOL SetBitEStatusGlassExist(BOOL bit);
	BOOL GetBitEStatusGlassExist();
	BOOL SetBitOModeOnline(BOOL bit);
	BOOL GetBitOModeOnline();
	BOOL SetBitOModeUnitAuto(BOOL bit);
	BOOL GetBitOModeAuto();
	BOOL SetBitOModeOperation_AutoOrManual(BOOL bit);
	BOOL GetBitOModeOperation();
	BOOL SetBitEStatusIonStatus(BOOL bit);
	BOOL GetBitEStatusIonStatus();
	BOOL SetBitEStatusRecipeNo(int dat);
	int GetBitEStatusRecipeNo();
	BOOL SetBitEStatusGlassCnt(int dat);
	int GetBitEStatusGlassCnt();
	BOOL SetBitEStatusGlassPosiCnt(int dat);
	int GetBitEStatusGlassPosiCnt();
	BOOL GetBitEStatusGlassPosition();
	BOOL SetBitEStatusGlassPosition(BOOL bit);

	void SetEStatus(int type);
	void SetNormalStatus(int nLoader_UnLoader, BOOL isOn);
	void SetGlassInProcessing(BOOL isOn);
	void SetGlassExistInUnit(BOOL isOn);
	void SetCurrentEquipmentRecipe(int nRecipeNum);
	void GetEcsTime();
	void GetRecipeVariousRead();

	void SetLightAlarmCode(int dat, BOOL on_off);
	void SetHeavyAlarmCode(int dat, BOOL on_off);
	BOOL SetLightAlarmReport(BOOL bit, int dat);
	BOOL SetHeavyAlarmReport(BOOL bit, int dat);
	BOOL SetDownAlarmCode(int dat);

	BOOL SetRecipeNum(int recipeNum, BOOL bDelete);
	BOOL SetRecipeListChange(BOOL bit);
	void RecipeChange();
	BOOL RecipeNumCheck(int nHostRecipeNum);

	void SetRecipeBodyRequest(BOOL on_off);
	BOOL SetRecipeBodyDataWrite(BOOL isEmpty);

	void SetHostRecipeRequest(WORD wRecipeNum, BOOL on_off);
	void SetHostRecipeWrite(BOOL isNULL);

	void WriteUnLoaderGlassData();
	void ReadLoaderGlassData();
	void ReadLoaderGlassData(typeGlassDataPara *glassData);
	void ReadLostGlassData();
	void WriteScrapGlassData();
	void WriteLostGlassData(int option);
	void WriteChangeGlassData();
	void WriteReportGlassData(int type);

	void GlassDataUpdate(BOOL isUpdate);
	typeGlassDataPara GetLoaderGlassData();
	typeGlassDataPara GetUnloaderGlassData();
	typeGlassDataPara GetUpdateGlassData();
	void SetUnloaderGlassData(typeGlassDataPara glassData);
	void SetScrapGlassData(typeGlassDataPara glassData);
	void SetScrapGlass(BOOL on_off);

	void SetChangeGlassData(typeGlassDataPara glassData);
	void SetChangeGlass(BOOL on_off);

	void SetUpdateGlassData(typeGlassDataPara glassData);
	
	CString GetTime();	

	void SetEquipmentRecipeChange(typeRecipeDataPara recipeData);
	BOOL EquipmentRecipeChangeCheck(typeRecipeDataPara recipeData);
	void SetEquipmentRecipeChange(BOOL on_off);
	BOOL m_bEquipmentRecipeChange;

	void SetScrapGlassDataReport();
	void SetLostGlassDataReport(BOOL on_off);
	void SetChangeGlassDataReport();

	void ReadRecipeVariousData();
	void WriteRecipeVariousAckData();
	void WriteRecipeVariousData(int ceid);
	void SetReipeVariousData(typeRecipeVariousDataPara variousData);

	void SetGlassLdUldEx(int type);

	typeMelsecListPara GetMelecListPara(int idx);

	void SetRecipeList(typeRecipeReportDataPara *recipeListPara);
	void WriteRecipeList(int ceid, int start_report_cnt, int end_report_cnt);

	void SetRecipe(typeRecipePara recipePara);
	void WriteRecipe(int ceid, int nBody_Parameter);

	void SetTPD(typeTPDDataPara tpdData);
	void SetTPDData();
	typeAPDDataPara GetAPD();
	void SetAPD(typeAPDDataPara apdData);
	void SetAPDData();
	void SetGlassPositionData(WORD *glassPosition);

	void WriteTPDData(int nStartAddress, int nSize, WORD *word);//comp
	void WriteAPDData(int nStartAddres, int nSize, WORD *word);//comp
	void WriteGlassPoitionData();

	void SetStatusReport(BOOL isOn);
	typeEQPStatusDataPara GetStatusReport(){return m_EQPStatus;};
	void SetStatus(typeEQPStatusDataPara statusData);

	int GetHostRecipeNumber();
	

	void InitSeqBit(int type);

	BOOL GetLoadingComplete(){return m_bLoadingComplete;};
	void InitLoadingComplete(){m_bLoadingComplete = FALSE;};

	BOOL FileDelete(CString FilePath, CString FileName);
	BOOL FileSearch(CString FilePath, CString FileName);

	CString GetErrorString();

	void SetRequestRecipePara(typeRecipePara recipePara);
	typeRecipePara GetRequestRecipePara();

	void SetLostGlassDataReport(typeLostGlassDataPara lostGlass);
	void InitGlassPosition();


	int GetInterLockError();
	int m_nInterLockError;

	//20160401 ngh
	int GlassDataComp(BOOL *glass_exist);
	BOOL GetGlassExist();

	int GetInterLockErrorLevel();
	int m_nInterLockErrorLevel;

	BOOL WriteLog(CString text);

	//20160717 ngh
	void SetLoaderBtn(int idx, BOOL val);
	void SetUnLoaderBtn(int idx, BOOL val);
	//20160725 sc
	CString GetLotInTime();

	//20160727 ngh
	int GetSlotNumber();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
protected:
	CRITICAL_SECTION	m_CSInline;

	CString m_strLogString;
	CString m_strErrorString;
	CString m_strStatusString;
	BOOL m_bLogWrite;

	//Sequence
	int m_iSeqIdx;
	int m_iTimeout100mm;
	int m_nSeqDelay100mm;
	int m_nTime;
	int m_nTPDWriteDelay;
	int m_nTimeCheck[MAX_TIMEOUT_CNT];
	int m_nTimeout[MAX_TIMEOUT_CNT];
	int m_nTrigger[MAX_TRIGGER_CNT];

	BOOL m_chk_e_sts_run;
	BOOL m_chk_e_sts_idle;
	BOOL m_chk_e_sts_down;
	BOOL m_chk_glass_in_process;
	BOOL m_chk_glass_exist;
	
	BOOL m_chk_o_mode_online;
	BOOL m_chk_o_mode_auto;
	BOOL m_chk_o_mode_operation;
	BOOL m_bUnloadOnly_Exchange;

	CString m_strECSTime;
	BOOL m_bECSStart;
	BOOL m_bAPDDataReport;
	BOOL m_bLoadingComplete;
	int m_nLoadingType;

	CPLCComm *m_PLCComm; 
	BOOL m_bOpened;
	long m_Path;

	int m_nLightAlarmCode;
	int m_nHeavyAlarmCode;
	int m_nDownAlarmCode;

	int m_nRequestRecipeNum;

	int m_nTPDNum;

	int m_nOrderCEID;
	int m_nReport_RecipeCnt;
	int m_nReportStart_RecipeListCnt;
	int m_nReportEnd_RecipeListCnt;

	typeEQPStatusDataPara m_EQPStatus;
	typeEMILoaderStatusDataPara m_EMILoaderStatus;
	typeEMIUnloaderStatusDataPara m_EMIUnloaderStatus;
	typeRobotLoaderStatusDataPara m_RobotLoaderStatus;
	typeRobotUnloaderStatusDataPara m_RobotUnloaderStatus;
	typeMelsecListPara *m_MelsecListPara;
	typeGlassDataPara m_LoaderGlassData;
	typeGlassDataPara m_UnloaderGlassData;
	typeGlassDataPara m_UpdateGlassData;

	typeGlassDataPara m_ScrapGlassData;
	typeGlassDataPara m_LostGlassData;
	typeGlassDataPara m_ChangeGlassData;
	typeLostGlassDataPara m_ReportLostGlassData;
	typeRecipeBodyDataPara m_RecipeBodyData;
	
	typeRecipeReportDataPara *m_RecipeReport;
	typeRecipeDataPara *m_RecipeData;
	typeSelectRecipeDataPara m_SelectRecipeData;
	typeRecipeVariousDataRequestPara m_RequestRecipeVariousData;
	typeRecipeVariousDataPara m_RecipeVariousData;
	typeRecipeVariousAckPara m_RecipeVariousAck;
	typeRecipeRMSDataPara m_RecipeRMSData;
	typeRecipePara m_RequestRecipeData;
	
	typeRecipePara m_Recipe;
	typeAPDDataPara m_APDData;
	typeTPDDataPara m_TPDData;

	typeLotFirstGlassInTime m_LotInTime; //20160725 sc

	WORD m_wLoaderGlassData[48];
	WORD m_wUnloaderGlassData[48];
	WORD m_wScrapGlassData[48];
	WORD m_wLostGlassData[48];
	WORD m_wReportLostGlassData[10];
	WORD m_wChangGlassData[48];
	WORD m_wReportGlassData[16];
	WORD m_wRecipeVariousData[1024];
	WORD m_wRequestSpecificData[16];
	WORD m_wRecipeParameter[992];
	WORD *m_wAPDData;
	WORD *m_wPositionData;
	
	int m_nRecipeTotalCount;
	int m_nLoastGlassDataOption;
	BOOL m_bLostGlassDataRequest;
	BOOL m_bScrapGlass;
	BOOL m_bEQPLoaderNormalStatus;
	BOOL m_bEQPUnloaderNormalStatus;
	BOOL m_bRobotLoaderNormalStatus;
	BOOL m_bRobotUnloaderNormalStatus;
	BOOL m_bEQPLoaderEnable;
	BOOL m_bEQPUnloaderEnable;
	BOOL m_bMelsecConnect;
	PLC_MEMORY_ITEM *m_MelsecItem;
	PLC_MEMORY_MAP *m_MelsecAddr;
	int m_nAllMelsecListCnt;
	BOOL m_bAllValueRead;
	BOOL m_bWord1ValueRead;
	BOOL m_bWord2ValueRead;
	BOOL m_bBitValueRead;
	BOOL m_bBackAreaType;
	BOOL m_bAreaType;
	int m_iProcessMoveIdx;

	void InitInterLockStatus();
	CString GetMCSCTime(int nAddress);
	void Update();
	void UpdateTimeoutTrigger(BOOL isUpdate);
	void ListInit();
	BOOL MelsecListParaLoad();
	void MelsecListView(int type, int line_cnt, CString strAddr, CString strName);
	void InitMelsecItem();
	void SetMemoryItem(int idx, int nType, int nAddr, int nSize);
	BOOL MelsecPLCInit();
	void UpdateMelsecList();
	void UpdateValue();
	void UpdateListValue();
	int GetMelsecErrValue();
	CString GetMelsecErrorMsg();
	void ReadMemory(int type);
	void TimerReadMemory(int type, BOOL isRead, int millisec = 500);
	void WordToBit(WORD word, BOOL *bit);
	void BitToWord(BOOL *bit, WORD *word);
	void WordToString(WORD word, char *str);
	void StringToWord(char str[2], WORD *word);
	void DWordToDec(WORD *word, int idx, double *dat);

	BOOL SaveTemp();
	BOOL LoadTemp();

	BOOL SaveTimeoutTrigger();
	BOOL LoadTimeoutTrigger();

	LRESULT OnInLineCtrlMessage(WPARAM para0, LPARAM para1);

	//20160721 ngh
	void TimeOutParameterEnable(int access_type);
	
	//사용 안함
	void ReadData(int idx, int type);
	void WriteData(int idx, BOOL nBit, int type = 2);
	void WriteData(int idx, WORD wWord, int type = 0);
	void ComboInit();
	void AddressItemComboUpdate(BOOL isUpdate);
	void ValueTypeItemCommboUpdate(int idx, int nValue_type);
	void UpdateEditData(int idx, int nValue_type);
	int HexToDec(CString nHex, int size);
	void SetValue(int idx, WORD wValue);
	void SetValue(int idx, BOOL bValue);
	WORD GetWordValue(int idx);
	BYTE GetBitValue(int idx);
	void Write(CString data, int nAreaType, int AddrItemIdx);

public:
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlInlineApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void ClickBtnenhctrlInlineCancel();
	afx_msg void OnBnClickedButtonEStatusRecipeNoSend();
	afx_msg void OnBnClickedButtonEStatusGlassCntSend();
	afx_msg void OnBnClickedButtonEStatusGlassPosiCntSend();
	afx_msg void OnBnClickedButtonEStatusIonStatusSend();
	afx_msg void OnBnClickedButtonOModeUnitOperationSend();
	BOOL m_chk_e_status_ion_status;
	int m_edit_e_status_recipe_no;
	int m_edit_e_status_glass_cnt;
	int m_edit_e_status_glass_posi_cnt;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ClickBtnenhctrlInlineLoaderGlassRead();
	void ClickBtnenhctrlInlineLoaderGlassRead2();
	CString m_edit_strLotID;
	CString m_edit_strProcessingCode;
	CString m_edit_strLotSpecificData1;
	CString m_edit_strLotSpecificData2;
	CString m_edit_strLotSpecificData3;
	CString m_edit_strLotSpecificData4;
	int m_edit_nHostRecipeNumber;
	int m_edit_nGlassType;
	int m_edit_nGlassCodeLot;
	int m_edit_nGlassCodeSLot;
	CString m_edit_strGlassID;
	int m_edit_nGlassSpecificData1;
	int m_edit_nGlassSpecificData2;
	int m_edit_nGlassSpecificData3;
	int m_edit_nGlassSpecificData4;
	int m_edit_nGlassAddData1;
	int m_edit_nGlassAddData2;
	int m_edit_nGlassAddData3;
	int m_edit_nGlassAddData4;
	int m_edit_nGlassAddData5;
	int m_edit_nGlassAddData6;
	int m_edit_nPreviousEqpProcessingData1;
	int m_edit_nPreviousEqpProcessingData2;
	int m_edit_nPreviousEqpProcessingData3;
	int m_edit_nPreviousEqpProcessingData4;
	int m_edit_nGlassProcessingData1;
	int m_edit_nGlassProcessingData2;
	int m_edit_nGlassProcessingData3;
	int m_edit_nGlassRoutingData1;
	int m_edit_nGlassRoutingData2;
	int m_edit_nGlassRoutingData3;
	CString m_edit_strGlassJudge;
	afx_msg void OnBnClickedButtonExchange();
	afx_msg void OnBnClickedButtonTimeData();
	afx_msg void OnBnClickedButtonLightAlarm();
	afx_msg void OnBnClickedButtonHevyAlarm();
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedButtonIdle();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedCheckEStatusGlassInprocess();
	afx_msg void OnBnClickedCheckEStatusGlassExist();
	afx_msg void OnBnClickedButtonLoaderGlassDataRead();
	afx_msg void OnBnClickedButtonUnloaderGlassDataWrite();
	afx_msg void OnBnClickedButtonEqupmentRecipeChange();
	int m_edit_nRecipeChangeNum;
	afx_msg void OnBnClickedButtonRecipeVariousRequest();
	afx_msg void OnBnClickedButtonRecipeVariousReport();
	afx_msg void OnBnClickedButtonScrapGlassData();
	afx_msg void OnBnClickedButtonLostGlassData();
	afx_msg void OnBnClickedButtonChangeGlassData();
	CBtnenhctrl m_label_eqp_loader_normal_status;
	CBtnenhctrl m_label_eqp_unloader_normal_status;
	CBtnenhctrl m_label_robot_loader_normal_status;
	CBtnenhctrl m_label_robot_unloader_normal_status;
	CBtnenhctrl m_label_eqp_loader_enable;
	CBtnenhctrl m_label_eqp_unloader_enable;
	afx_msg void OnBnClickedButtonGlassPosition();
	afx_msg void OnBnClickedButtonGlassApdData();
	afx_msg void OnBnClickedButtonGlassTpdData();
	CString m_edit_strEcs_Time;
	afx_msg void OnBnClickedButtonLightAlarm2();
	afx_msg void OnBnClickedButtonHevyAlarm2();
	int m_edit_nTimeout_0;
	int m_edit_nTimeout_1;
	int m_edit_nTimeout_2;
	int m_edit_nTimeout_3;
	int m_edit_nTimeout_4;
	int m_edit_nTimeout_5;
	int m_edit_nTimeout_6;
	int m_edit_nTimeout_7;
	int m_edit_nTrigger_0;
	int m_edit_nTrigger_1;
	int m_edit_nTrigger_2;
	int m_edit_nTrigger_3;
	int m_edit_nTrigger_4;
	int m_edit_nTrigger_5;
	int m_edit_nTrigger_6;
	int m_edit_nTrigger_7;
	BOOL m_radio_lost_glass_data_option;
	afx_msg void OnBnClickedButtonRecipeBody();
	afx_msg void OnBnClickedButtonRecipeParameter();
	int m_edit_nGlassCode;
	afx_msg void OnBnClickedButton1();
	int m_edit_nReipeReportCnt;
	BOOL m_check_bUnload_Exchang;
	CBtnenhctrl m_btn_LoaderEnable;
	CBtnenhctrl m_btn_LoaderGlassDataRequest;
	CBtnenhctrl m_btn_LoaderNormalStatus;
	CBtnenhctrl m_btn_LoaderLoadRequest;
	CBtnenhctrl m_btn_LoaderLoadCompleteConfirm;
	CBtnenhctrl m_btn_LoaderLoadWait;
	CBtnenhctrl m_btn_UnLoaderEnable;
	CBtnenhctrl m_btn_UnLoaderGlassDataReport;
	CBtnenhctrl m_btn_UnLoaderNormalStatus;
	CBtnenhctrl m_btn_UnLoaderUnLoadRequest;
	CBtnenhctrl m_btn_UnLoaderUnloadCompleteConfirm;
	CBtnenhctrl m_btn_UnLoaderUnLoadWait;

	int m_nTest;
	BOOL m_bTest;
	afx_msg void OnBnClickedButtonDeleteGlassData();
};
