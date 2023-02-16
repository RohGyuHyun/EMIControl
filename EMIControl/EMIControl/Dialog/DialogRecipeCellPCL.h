#pragma once
#define RECIPE_CELL_PCL_PATH	_T("D:\\DATABASE\\SYSTEM")

// CDialogRecipeCellPCL 대화 상자입니다.

class CDialogRecipeCellPCL : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogRecipeCellPCL)

public:
	CDialogRecipeCellPCL(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogRecipeCellPCL();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_RECIPE_CELL_PCL };


public:
	double GetPCLSizeX0(int type);
	double GetPCLSizeY0(int type);
	double GetPCLSizeX1(int type);
	double GetPCLSizeY1(int type);
	double GetPCLCntX(int type);
	double GetPCLCntY(int type);
	double GetPCLPitchX(int type);
	double GetPCLPitchY(int type);
	double GetPCLGrpCntX(int type);
	double GetPCLGrpCntY(int type);
	double GetPCLGrpPitchX(int type);
	double GetPCLGrpPitchY(int type);
	double GetPCLPosFromPCLAlignX(int type);
	double GetPCLPosFromPCLAlignY(int type);

	double GetPCLSizeX0_coordXY(int type);
	double GetPCLSizeY0_coordXY(int type);
	double GetPCLSizeX1_coordXY(int type);
	double GetPCLSizeY1_coordXY(int type);
	double GetPCLCntX_coordXY(int type);
	double GetPCLCntY_coordXY(int type);
	double GetPCLPitchX_coordXY(int type);
	double GetPCLPitchY_coordXY(int type);
	double GetPCLGrpCntX_coordXY(int type);
	double GetPCLGrpCntY_coordXY(int type);
	double GetPCLGrpPitchX_coordXY(int type);
	double GetPCLGrpPitchY_coordXY(int type);
	double GetPCLPosFromPCLAlignX_coordXY(int type);
	double GetPCLPosFromPCLAlignY_coordXY(int type);

	BOOL SetLoadRecipeCellPCLPara(CString path);
	BOOL SetSaveRecipeCellPCLPara(CString path);

	//20160322 ngh
	void ParameterCompare();
	void Update(BOOL isUpdate);
	double m_dRecipeCellPCLPara[100];

protected:
	BOOL LoadRecipeCellPCLPara(CString path);
	BOOL SaveRecipeCellPCLPara(CString path);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlRecipeCellSave();
	void ClickBtnenhctrlRecipeCellApply();
	void ClickBtnenhctrlCellPanelCancel();

	double m_edit_recipe_pcl_size_x0_0;
	double m_edit_recipe_pcl_size_y0_0;
	double m_edit_recipe_pcl_size_x1_0;
	double m_edit_recipe_pcl_size_y1_0;
	double m_edit_recipe_pcl_cnt_x_0;
	double m_edit_recipe_pcl_cnt_y_0;
	double m_edit_recipe_pcl_pitch_x_0;
	double m_edit_recipe_pcl_pitch_y_0;
	double m_edit_recipe_pcl_grpcnt_x_0;
	double m_edit_recipe_pcl_grpcnt_y_0;
	double m_edit_recipe_pcl_grppitch_x_0;
	double m_edit_recipe_pcl_grppitch_y_0;
	double m_edit_recipe_pcl_pos_x_0;
	double m_edit_recipe_pcl_pos_y_0;

	double m_edit_recipe_pcl_size_x0_1;
	double m_edit_recipe_pcl_size_y0_1;
	double m_edit_recipe_pcl_size_x1_1;
	double m_edit_recipe_pcl_size_y1_1;
	double m_edit_recipe_pcl_cnt_x_1;
	double m_edit_recipe_pcl_cnt_y_1;
	double m_edit_recipe_pcl_pitch_x_1;
	double m_edit_recipe_pcl_pitch_y_1;
	double m_edit_recipe_pcl_grpcnt_x_1;
	double m_edit_recipe_pcl_grpcnt_y_1;
	double m_edit_recipe_pcl_grppitch_x_1;
	double m_edit_recipe_pcl_grppitch_y_1;
	double m_edit_recipe_pcl_pos_x_1;
	double m_edit_recipe_pcl_pos_y_1;

	double m_edit_recipe_pcl_size_x0_2;
	double m_edit_recipe_pcl_size_y0_2;
	double m_edit_recipe_pcl_size_x1_2;
	double m_edit_recipe_pcl_size_y1_2;
	double m_edit_recipe_pcl_cnt_x_2;
	double m_edit_recipe_pcl_cnt_y_2;
	double m_edit_recipe_pcl_pitch_x_2;
	double m_edit_recipe_pcl_pitch_y_2;
	double m_edit_recipe_pcl_grpcnt_x_2;
	double m_edit_recipe_pcl_grpcnt_y_2;
	double m_edit_recipe_pcl_grppitch_x_2;
	double m_edit_recipe_pcl_grppitch_y_2;
	double m_edit_recipe_pcl_pos_x_2;
	double m_edit_recipe_pcl_pos_y_2;

	double m_edit_recipe_pcl_size_x0_3;
	double m_edit_recipe_pcl_size_y0_3;
	double m_edit_recipe_pcl_size_x1_3;
	double m_edit_recipe_pcl_size_y1_3;
	double m_edit_recipe_pcl_cnt_x_3;
	double m_edit_recipe_pcl_cnt_y_3;
	double m_edit_recipe_pcl_pitch_x_3;
	double m_edit_recipe_pcl_pitch_y_3;
	double m_edit_recipe_pcl_grpcnt_x_3;
	double m_edit_recipe_pcl_grpcnt_y_3;
	double m_edit_recipe_pcl_grppitch_x_3;
	double m_edit_recipe_pcl_grppitch_y_3;
	double m_edit_recipe_pcl_pos_x_3;
	double m_edit_recipe_pcl_pos_y_3;
};
