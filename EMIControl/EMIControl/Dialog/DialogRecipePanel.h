#pragma once


// CDialogRecipePanel 대화 상자입니다.
#define RECIPE_PANEL_PATH			"D:\\DATABASE\\SYSTEM"			

class CDialogRecipePanel : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogRecipePanel)

public:
	CDialogRecipePanel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogRecipePanel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_RECIPE_PANEL };

public:
	double GetMarkSize(int cam_idx);
	double GetMarkPosX(int cam_idx);
	double GetMarkPosY(int cam_idx);
	double GetPanelSizeX();
	double GetPanelSizeY();
	double GetCellCntX();
	double GetCellCntY();
	double GetCellPitchX();
	double GetCellPitchY();
	double GetCellGrpCntX();
	double GetCellGrpCntY();
	double GetCellGrpPitchX();
	double GetCellGrpPitchY();
	double GetCellFirstPosX();
	double GetCellFirstPosY();
	double GetFilmFirstPosX();
	double GetFilmFirstPosY();
	double GetCVDFirstPosX();
	double GetCVDFirstPosY();
	double GetPanelLoadPosY();
	double GetPanelLoadPosT();
	double GetPanelManuLoadPosY();
	double GetPanelManuLoadPosT();
	double GetPanelLoadPosUP();
	double GetPanelLoadPosDN();
	//20160418 sc
	double GetCellTotalCnt();

	//Glass
	double GetMarkSize_coordXY(int cam_idx);
	double GetMarkPosX_coordXY(int cam_idx);
	double GetMarkPosY_coordXY(int cam_idx);
	double GetPanelSizeX_coordXY();
	double GetPanelSizeY_coordXY();
	double GetCellCntX_coordXY();
	double GetCellCntY_coordXY();
	double GetCellPitchX_coordXY();
	double GetCellPitchY_coordXY();
	double GetCellGrpCntX_coordXY();
	double GetCellGrpCntY_coordXY();
	double GetCellGrpPitchX_coordXY();
	double GetCellGrpPitchY_coordXY();
	double GetCellFirstPosX_coordXY();
	double GetCellFirstPosY_coordXY();
	double GetFilmFirstPosX_coordXY();
	double GetFilmFirstPosY_coordXY();
	double GetCVDFirstPosX_coordXY();
	double GetCVDFirstPosY_coordXY();

	BOOL SetLoadRecipePanelPara(CString path);
	BOOL SetSaveRecipePanelPara(CString path);

	typeRecipePanel GetRecipe();

	//2010412 ngh
	void SetUpdate(BOOL isUpdate);

	//20160522 ngh
	CString m_strPanelName;
	double m_dRecipePanelPara[100];
	void ParameterCompare();
	void Update(BOOL isUpdate);
protected:
	BOOL LoadRecipePanelPara(CString path);
	BOOL SaveRecipePanelPara(CString path);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();
	CString m_edit_panel_name;
	double m_edit_panel_size_x;
	double m_edit_panel_size_y;
	double m_edit_panel_load_y_pos;
	double m_edit_panel_load_t_pos;
	double m_edit_panel_load_y_pos_manu;
	double m_edit_panel_load_t_pos_manu;
	double m_edit_panel_start_x_pos;
	double m_edit_panel_start_y_pos;
	double m_edit_panel_film_start_x_pos;
	double m_edit_panel_film_start_y_pos;
	double m_edit_panel_cvd_start_x_pos;
	double m_edit_panel_cvd_start_y_pos;
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlPanelSetLoadPos();
	void ClickBtnenhctrlPanelSetLoadMove();
	void ClickBtnenhctrlPanelSetLoadPosM();
	void ClickBtnenhctrlPanelSetLoadMoveM();
	void ClickBtnenhctrlRecipePanelSave();
	void ClickBtnenhctrlRecipePanelApply();
	void ClickBtnenhctrlRecipePanelCancel();
	double m_edit_penel_cell_cnt_y;
	double m_edit_penel_cell_cnt_x;
	double m_edit_penel_cell_grpcnt_y;
	double m_edit_penel_cell_grpcnt_x;
	double m_edit_panel_mark_size0;
	double m_edit_panel_mark_size1;
	double m_edit_panel_mark_to_panel_pos_y0;
	double m_edit_panel_mark_to_panel_pos_x0;
	double m_edit_panel_mark_to_panel_pos_y1;
	double m_edit_panel_mark_to_panel_pos_x1;
public:
	double m_edit_panel_cell_pitch_x;
	double m_edit_panel_cell_pitch_y;
	double m_edit_panel_cell_grppitch_x;
	double m_edit_panel_cell_grppitch_y;
	double m_edit_load_lift_up_pos;
	double m_edit_load_lift_dn_pos;
	void ClickBtnenhctrlPanelLoadSetLiftUpPos();
	void ClickBtnenhctrlPanelLoadLiftUpMove();
	void ClickBtnenhctrlPanelLoadLiftDnPos();
	void ClickBtnenhctrlPanelLoadLiftDnMove();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
