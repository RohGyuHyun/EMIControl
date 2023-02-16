
typedef struct{
	int nType;
	int	nStartAddress;
	int	nEndAddress;
}PLC_MEMORY_MAP;

typedef struct{
	int nType;
	int	nAddress;
	int nSize;
}PLC_MEMORY_ITEM; 

typedef struct MelsecListPara{
	int nAddrType;
	int nSize;
	char strName[256];
	char strAddr[256];
	int nAddr;
	WORD wValue;
	BOOL bValue;
	BOOL bWORD_BIT;
	char strValue[100];
}typeMelsecListPara;

typedef struct EQPStatusData{
	BOOL bECSConnect;
	BOOL bOnline;
	BOOL bUnitAutoMode;
	int nUnitOpeationMode;
	int nStatusRunIdleDown;
	BOOL bDownAlarm;
	BOOL bLightAlarm;
	BOOL bHeavyAlarm;
	BOOL bGlassInProcess;
	BOOL bGlassExistInUnit;
	int nCurrenRecipeNum;
	int nHostRecipeNum;
	int nGlassCountInUnit;
	int nPutInfoPossibleCount;
	BOOL bIonizer;
	BOOL bGlassPosition;
}typeEQPStatusDataPara;

typedef struct GlassCodePara{
	int nLotNumber;
	int nSlotNumber;
}typeGlassCodePara;

typedef struct GlassDataPara{
	char strLotID[16];
	char strProcessingCode[8];
	char strLotSpecificData[8];
	int nHostRecipeNumber;
	int nGlassType;
	typeGlassCodePara GlassCode;
	int nGlassCode;
	char strGlassID[16];
	char strGlassJudge[2];
	int nGlassSpecificData[4];
	int nGlassAddData[6];
	int nPreviousUnitProcessingData[4];
	int nGlassProcessingStatus[3];
	int nGlassRoutingData[3];
}typeGlassDataPara;

typedef struct LostGlassData{
	char strGlassID[16];
	typeGlassCodePara GlassCode;
	int nGlassCode;
	int nGlassOption;
}typeLostGlassDataPara;

typedef struct RecipeReportData{
	int nRecipeNumber;
	int nRecipeLevel;
	char cRecipeType;
	WORD wRecipeVersion[3];
	int nRecipeStatus;
	int nUnitNumber;
}typeRecipeReportDataPara;

typedef struct RecipeData{
	int nRecipeNum;
	BOOL bRecipeNum;
	BOOL bRecipeBody;
	int nVersion[3];
}typeRecipeDataPara;

typedef struct RecipeVariousData{
	int nRecipeCEID;
	int nRecipeCEIDOrder;
	int nRecipeTotalCount;
	int nRecipeReportCount;
	int nRecipeCommand;
	int nRecipeNumber;
	int nRecipeLevel;
	char cRecipeType;
	int nUnitNumber;
	WORD nRecipeVersion[3];
	WORD wEquipmentSpecificDataV[16];
	//WORD wRecipeParameterV[992];
	typeRecipeReportDataPara RecipeReportData[992];
}typeRecipeVariousDataPara;

typedef struct RecipeVariousDataRequestPara{
	int nRecipeCEID;
	int nRecipeCommand;
	int nRecipeNumber;
	int nRecipeLevel;
	char cRecipeType;
	WORD nRecipeVersion[3];
	int nUnitNumber;
}typeRecipeVariousDataRequestPara;

typedef struct RecipeRMSData{
	int nRecipeNum;
	int nRecipeLevel;
	char cRecipeType;
	WORD RecipeVersion[3];
	WORD SpectificData[16];
	WORD Parameter[992];
}typeRecipeRMSDataPara;

typedef struct RecipeVariousAck{
	int nRequestConfirmCEID;
	int RequestConfirmACK;
}typeRecipeVariousAckPara;

typedef struct RecipeBodyData{
	int nDataCEID;
	int nRecipeNumber;
	int nConfirmAck;
	int nRecipeLevel;
	char cRecipeType;
	WORD wRecipeVersion[3];
	//WORD wEQPSpecificData[16];
	//WORD wRecipeBodyData[992];
}typeRecipeBodyDataPara;

typedef struct ReportConfirmData{
	WORD wConfirmCEID;
	WORD wConfirmACK;
}typeReportConfirmDataPara;

typedef struct SelectRecipeData{
	typeRecipeDataPara RecipeData;
	WORD *RecipeBody;
}typeSelectRecipeDataPara;

typedef struct RecipeCellData{
	double dCell_pos_x;
	double dCell_pos_y;
	double dCell_activesize_x;
	double dCell_activesize_y;
	double dRecipe_film_size_x0;
	double dRecipe_film_size_y0;
	double dRecipe_film_size_x1;
	double dRecipe_film_size_y1;
	double dRecipe_film_cnt_x;
	double dRecipe_film_cnt_y;
	double dRecipe_film_pitch_x;
	double dRecipe_film_pitch_y;
	double dRecipe_film_grpcnt_x;
	double dRecipe_film_grpcnt_y;
	double dRecipe_film_grppitch_x;
	double dRecipe_film_grppitch_y;
	double dRecipe_film_pos_x;
	double dRecipe_film_pos_y;
}typeRecipeCell;

typedef struct RecipePanel{
	char cPanel_name[20];
	double dPanel_size_x;
	double dPanel_size_y;
	double dPanel_load_y_pos;
	double dPanel_load_t_pos;
	double dPanel_load_y_pos_manu;
	double dPanel_load_t_pos_manu;
	double dPanel_start_x_pos;
	double dPanel_start_y_pos;
	double dPanel_film_start_x_pos;
	double dPanel_film_start_y_pos;
	double dPenel_cell_cnt_y;
	double dPenel_cell_cnt_x;
	double dPenel_cell_grpcnt_y;
	double dPenel_cell_grpcnt_x;
	double dPanel_mark_size0;
	double dPanel_mark_size1;
	double dPanel_mark_to_panel_pos_y0;
	double dPanel_mark_to_panel_pos_x0;
	double dPanel_mark_to_panel_pos_y1;
	double dPanel_mark_to_panel_pos_x1;
	double dPanel_cell_pitch_x;
	double dPanel_cell_pitch_y;
	double dPanel_cell_grppitch_x;
	double dPanel_cell_grppitch_y;
	double dLoad_lift_up_pos;
	double dLoad_lift_dn_pos;
}typeRecipePanel;

typedef struct RecipeScan{
	double dScan_start_x;
	double dScan_start_y;
	double dScan_leng;
	double dScan_pitch;
	double dScan_pixel_x;
	double dScan_pixel_y;
	double dScan_cnt;
	double dScan_coord_x;
	double dScan_coord_y;
	double dScan_glass_x;
	double dScan_glass_y;
	double dThrsh0;
	double dThrsh1;
	double dThrsh2;
	double dThrsh3;
	double dThrsh4;
	double dDefect_size0;
	double dDefect_size1;
	double dDefect_size2;
	double dDefect_size3;
	double dDefect_size4;
	double dDefect_size_small;
	double dDefect_size_medium;
	double dDefect_size_large;
}typeRecipeScan;

typedef struct RecipeParaData{
	int nRecipeNumber;
	char cRecipeName[100];
	typeRecipeReportDataPara RecipeReport;
	typeRecipeCell RecipeCell;
	typeRecipePanel RecipePanel;
	typeRecipeScan RecipeScan[RECIPE_SCAN_TYPE_MAX_NUM];
}typeRecipePara;

typedef struct TPDReoprt{
	int nSolBlw;
	int nChuckVac;
	int nScan_X_Axis_TQ;
	int nRV_Y_Axis_TQ;
	int nLIFT_AXIS_TQ;
	int nTPDNumber;
}typeTPDDataPara;

typedef struct TimeData{
	WORD wYear;
	WORD wMonth;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
}typeTimeDataPara;

typedef struct APDReport{
	typeTimeDataPara GlassStartTime;
	typeTimeDataPara GlassEndTime;
	char cGlassID[12];
	char cAlignSuccess;
	int nInspMode;
	int nAlignMarkX1;
	int nAlignMarkX2;
	int nAlignMarkY1;
	int nAlignMarkY2;
}typeAPDDataPara;

typedef struct EMILoaderStatusData{
	BOOL bEnable;
	BOOL bGlassDataRequest;
	BOOL bNormalStatus;
	BOOL bRequest;
	BOOL bCompleteConfirm;
	BOOL bWait;
}typeEMILoaderStatusDataPara;

typedef struct EMIUnloaderStatusData{
	BOOL bEnable;
	BOOL bGlassDataRequest;
	BOOL bNormalStatus;
	BOOL bRequest;
	BOOL bCompleteConfirm;
	BOOL bWait;
}typeEMIUnloaderStatusDataPara;

typedef struct RobotLoaderStatusData{
	BOOL bNoninterference;
	BOOL bGlassDataReport;
	BOOL bRobotBusy;
	BOOL bcomplete;
	BOOL bNormalStatus;
	BOOL bReady;
}typeRobotLoaderStatusDataPara;

typedef struct RobotUnloaderStatusData{
	BOOL bNoninterference;
	BOOL bRobotBusy;
	BOOL bcomplete;
	BOOL bNormalStatus;
	BOOL bReady;
}typeRobotUnloaderStatusDataPara;

typedef struct LotFirstGlassInTime{   ////20160725 sc
	CString strInTime;
}typeLotFirstGlassInTime;