#pragma once

#define RECIPE_CELL_PATH				"D:\\DATABASE\\SYSTEM"
// CDialogRecipeCell 대화 상자입니다.

class CDialogRecipeCell : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogRecipeCell)

public:
	CDialogRecipeCell(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogRecipeCell();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_RECIPE_CELL };

public:
	double GetCellPosFromCellAlignX();
	double GetCellPosFromCellAlignY();
	double GetCellActiveSizeX();
	double GetCellActiveSizeY();

	double GetFilmSizeX0();
	double GetFilmSizeY0();
	double GetFilmSizeX1();
	double GetFilmSizeY1();
	double GetFilmCntX();
	double GetFilmCntY();
	double GetFilmPitchX();
	double GetFilmPitchY();
	double GetFilmGrpCntX();
	double GetFilmGrpCntY();
	double GetFilmGrpPitchX();
	double GetFilmGrpPitchY();
	double GetFilmPosFromFilmAlignX();
	double GetFilmPosFromFilmAlignY();

	double GetCVDSizeX0();
	double GetCVDSizeY0();
	double GetCVDSizeX1();
	double GetCVDSizeY1();
	double GetCVDCntX();
	double GetCVDCntY();
	double GetCVDPitchX();
	double GetCVDPitchY();
	double GetCVDGrpCntX();
	double GetCVDGrpCntY();
	double GetCVDGrpPitchX();
	double GetCVDGrpPitchY();
	double GetCVDPosFromCVDAlignX();
	double GetCVDPosFromCVDAlignY();

	double GetCVDDNSizeX0();
	double GetCVDDNSizeY0();
	double GetCVDDNSizeX1();
	double GetCVDDNSizeY1();
	double GetCVDDNCntX();
	double GetCVDDNCntY();
	double GetCVDDNPitchX();
	double GetCVDDNPitchY();
	double GetCVDDNGrpCntX();
	double GetCVDDNGrpCntY();
	double GetCVDDNGrpPitchX();
	double GetCVDDNGrpPitchY();
	double GetCVDDNPosFromCVDDNAlignX();
	double GetCVDDNPosFromCVDDNAlignY();

	//Glass
	double GetCellPosFromCellAlignX_coordXY();
	double GetCellPosFromCellAlignY_coordXY();
	double GetCellActiveSizeX_coordXY();
	double GetCellActiveSizeY_coordXY();

	double GetFilmSizeX0_coordXY();
	double GetFilmSizeY0_coordXY();
	double GetFilmSizeX1_coordXY();
	double GetFilmSizeY1_coordXY();
	double GetFilmCntX_coordXY();
	double GetFilmCntY_coordXY();
	double GetFilmPitchX_coordXY();
	double GetFilmPitchY_coordXY();
	double GetFilmGrpCntX_coordXY();
	double GetFilmGrpCntY_coordXY();
	double GetFilmGrpPitchX_coordXY();
	double GetFilmGrpPitchY_coordXY();
	double GetFilmPosFromFilmAlignX_coordXY();
	double GetFilmPosFromFilmAlignY_coordXY();

	double GetCVDSizeX0_coordXY();
	double GetCVDSizeY0_coordXY();
	double GetCVDSizeX1_coordXY();
	double GetCVDSizeY1_coordXY();
	double GetCVDCntX_coordXY();
	double GetCVDCntY_coordXY();
	double GetCVDPitchX_coordXY();
	double GetCVDPitchY_coordXY();
	double GetCVDGrpCntX_coordXY();
	double GetCVDGrpCntY_coordXY();
	double GetCVDGrpPitchX_coordXY();
	double GetCVDGrpPitchY_coordXY();
	double GetCVDPosFromCVDAlignX_coordXY();
	double GetCVDPosFromCVDAlignY_coordXY();

	double GetCVDDNSizeX0_coordXY();
	double GetCVDDNSizeY0_coordXY();
	double GetCVDDNSizeX1_coordXY();
	double GetCVDDNSizeY1_coordXY();
	double GetCVDDNCntX_coordXY();
	double GetCVDDNCntY_coordXY();
	double GetCVDDNPitchX_coordXY();
	double GetCVDDNPitchY_coordXY();
	double GetCVDDNGrpCntX_coordXY();
	double GetCVDDNGrpCntY_coordXY();
	double GetCVDDNGrpPitchX_coordXY();
	double GetCVDDNGrpPitchY_coordXY();
	double GetCVDDNPosFromCVDDNAlignX_coordXY();
	double GetCVDDNPosFromCVDDNAlignY_coordXY();

	//20160526 sh
	double GetCVDAlignFirstCVDX();
	double GetCVDAlignFirstCVDY();
	double GetCVDUpDnDis();
	double GetCVDGrpDisX();
	double GetCVDGrpDisY();

	typeRecipeCell GetRecipe();

	BOOL SetLoadRecipeCellPara(CString path);
	BOOL SetSaveRecipeCellPara(CString path);

	//2010412 ngh
	void SetUpdate(BOOL isUpdate);
	//20160322 ngh
	void ParameterCompare();
	void Update(BOOL isUpdate);
	double m_dRecipeCellPara[100];
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

protected:
	BOOL LoadRecipeCellPara(CString path);
	BOOL SaveRecipeCellPara(CString path);

	DECLARE_MESSAGE_MAP()
public:
	double m_edit_cell_pos_x;
	double m_edit_cell_pos_y;
	double m_edit_cell_activesize_x;
	double m_edit_cell_activesize_y;
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlRecipeCellApply();
	double m_edit_recipe_film_size_x0;
	double m_edit_recipe_film_size_y0;
	double m_edit_recipe_film_size_x1;
	double m_edit_recipe_film_size_y1;
	double m_edit_recipe_film_cnt_x;
	double m_edit_recipe_film_cnt_y;
	double m_edit_recipe_film_pitch_x;
	double m_edit_recipe_film_pitch_y;
	double m_edit_recipe_film_grpcnt_x;
	double m_edit_recipe_film_grpcnt_y;
	double m_edit_recipe_film_grppitch_x;
	double m_edit_recipe_film_grppitch_y;
	double m_edit_recipe_film_pos_x;
	double m_edit_recipe_film_pos_y;

	double m_edit_recipe_cvd_size_x0;
	double m_edit_recipe_cvd_size_y0;
	double m_edit_recipe_cvd_size_x1;
	double m_edit_recipe_cvd_size_y1;
	double m_edit_recipe_cvd_cnt_x;
	double m_edit_recipe_cvd_cnt_y;
	double m_edit_recipe_cvd_pitch_x;
	double m_edit_recipe_cvd_pitch_y;
	double m_edit_recipe_cvd_grpcnt_x;
	double m_edit_recipe_cvd_grpcnt_y;
	double m_edit_recipe_cvd_grppitch_x;
	double m_edit_recipe_cvd_grppitch_y;
	double m_edit_recipe_cvd_pos_x;
	double m_edit_recipe_cvd_pos_y;

	double m_edit_recipe_cvddn_size_x0;
	double m_edit_recipe_cvddn_size_y0;
	double m_edit_recipe_cvddn_size_x1;
	double m_edit_recipe_cvddn_size_y1;
	double m_edit_recipe_cvddn_cnt_x;
	double m_edit_recipe_cvddn_cnt_y;
	double m_edit_recipe_cvddn_pitch_x;
	double m_edit_recipe_cvddn_pitch_y;
	double m_edit_recipe_cvddn_grpcnt_x;
	double m_edit_recipe_cvddn_grpcnt_y;
	double m_edit_recipe_cvddn_grppitch_x;
	double m_edit_recipe_cvddn_grppitch_y;
	double m_edit_recipe_cvddn_pos_x;
	double m_edit_recipe_cvddn_pos_y;
	double m_edit_align_cvd;
	double m_edit_align_cvd_y;
	double m_edit_up_dn_dis;
	double m_edit_cvd_grp_dis_x;
	double m_edit_cvd_grp_dis_y;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	void ClickBtnenhctrlRecipeCellSave();
};
