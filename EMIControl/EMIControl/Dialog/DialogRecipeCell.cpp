// Dialog/DialogRecipeCell.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EMIControl.h"


#include "Include/_CtrlImageRect.h"
#include "include/_FuncModuleHal.h"
#include "include/_FuncModuleBasic.h"

#include "include/Runtime.h"
#include "include/PMACComm.h"

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
#include "Dialog/DialogRecipeScan.h"
#include "Dialog/DialogGlassManuCtrl.h"
#include "Dialog/DialogFilmAlign.h"
#include "Dialog/DialogLogAlarm.h"
#include "DialogLogWindow.h"

#include "EMIControlDlg.h"

#include "afxdialogex.h"



// CDialogRecipeCell 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogRecipeCell, CDialogEx)

CDialogRecipeCell::CDialogRecipeCell(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogRecipeCell::IDD, pParent)
	, m_edit_cell_pos_x(0)
	, m_edit_cell_pos_y(0)
	, m_edit_cell_activesize_x(0)
	, m_edit_cell_activesize_y(0)
	, m_edit_recipe_film_size_x0(0)
	, m_edit_recipe_film_size_y0(0)
	, m_edit_recipe_film_size_x1(0)
	, m_edit_recipe_film_size_y1(0)
	, m_edit_recipe_film_cnt_x(0)
	, m_edit_recipe_film_cnt_y(0)
	, m_edit_recipe_film_pitch_x(0)
	, m_edit_recipe_film_pitch_y(0)
	, m_edit_recipe_film_grpcnt_x(0)
	, m_edit_recipe_film_grpcnt_y(0)
	, m_edit_recipe_film_grppitch_x(0)
	, m_edit_recipe_film_grppitch_y(0)
	, m_edit_recipe_film_pos_x(0)
	, m_edit_recipe_film_pos_y(0)
	, m_edit_recipe_cvd_size_x0(0)
	, m_edit_recipe_cvd_size_y0(0)
	, m_edit_recipe_cvd_size_x1(0)
	, m_edit_recipe_cvd_size_y1(0)
	, m_edit_recipe_cvd_cnt_x(0)
	, m_edit_recipe_cvd_cnt_y(0)
	, m_edit_recipe_cvd_pitch_x(0)
	, m_edit_recipe_cvd_pitch_y(0)
	, m_edit_recipe_cvd_grpcnt_x(0)
	, m_edit_recipe_cvd_grpcnt_y(0)
	, m_edit_recipe_cvd_grppitch_x(0)
	, m_edit_recipe_cvd_grppitch_y(0)
	, m_edit_recipe_cvd_pos_x(0)
	, m_edit_recipe_cvd_pos_y(0)
	, m_edit_recipe_cvddn_size_x0(0)
	, m_edit_recipe_cvddn_size_y0(0)
	, m_edit_recipe_cvddn_size_x1(0)
	, m_edit_recipe_cvddn_size_y1(0)
	, m_edit_recipe_cvddn_cnt_x(0)
	, m_edit_recipe_cvddn_cnt_y(0)
	, m_edit_recipe_cvddn_pitch_x(0)
	, m_edit_recipe_cvddn_pitch_y(0)
	, m_edit_recipe_cvddn_grpcnt_x(0)
	, m_edit_recipe_cvddn_grpcnt_y(0)
	, m_edit_recipe_cvddn_grppitch_x(0)
	, m_edit_recipe_cvddn_grppitch_y(0)
	, m_edit_recipe_cvddn_pos_x(0)
	, m_edit_recipe_cvddn_pos_y(0)
	, m_edit_align_cvd(0)
	, m_edit_align_cvd_y(0)
	, m_edit_up_dn_dis(0)
	, m_edit_cvd_grp_dis_x(0)
	, m_edit_cvd_grp_dis_y(0)
{

}

CDialogRecipeCell::~CDialogRecipeCell()
{
	SaveRecipeCellPara(_T(RECIPE_CELL_PATH));
}

void CDialogRecipeCell::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CELL_POS_X, m_edit_cell_pos_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CELL_POS_Y, m_edit_cell_pos_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CELL_ACTIVE_AREA_X, m_edit_cell_activesize_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CELL_ACTIVE_AREA_Y, m_edit_cell_activesize_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_FILM_SIZE_X0, m_edit_recipe_film_size_x0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_FILM_SIZE_Y0, m_edit_recipe_film_size_y0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_FILM_SIZE_X1, m_edit_recipe_film_size_x1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_FILM_SIZE_Y1, m_edit_recipe_film_size_y1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_FILM_CNT_X, m_edit_recipe_film_cnt_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_FILM_CNT_Y, m_edit_recipe_film_cnt_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_FILM_PITCH_X2, m_edit_recipe_film_pitch_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_FILM_PITCH_Y2, m_edit_recipe_film_pitch_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_FILM_GRPCNT_X, m_edit_recipe_film_grpcnt_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_FILM_GRPCNT_Y, m_edit_recipe_film_grpcnt_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_FILM_GRPPITCH_X, m_edit_recipe_film_grppitch_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_FILM_GRPPITCH_Y, m_edit_recipe_film_grppitch_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_FILM_POS_X, m_edit_recipe_film_pos_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_FILM_POS_Y, m_edit_recipe_film_pos_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVD_SIZE_X0, m_edit_recipe_cvd_size_x0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVD_SIZE_Y0, m_edit_recipe_cvd_size_y0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVD_SIZE_X1, m_edit_recipe_cvd_size_x1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVD_SIZE_Y1, m_edit_recipe_cvd_size_y1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVD_CNT_X, m_edit_recipe_cvd_cnt_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVD_CNT_Y, m_edit_recipe_cvd_cnt_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVD_PITCH_X, m_edit_recipe_cvd_pitch_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVD_PITCH_Y, m_edit_recipe_cvd_pitch_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVD_GRPCNT_X, m_edit_recipe_cvd_grpcnt_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVD_GRPCNT_Y, m_edit_recipe_cvd_grpcnt_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVD_GRPPITCH_X, m_edit_recipe_cvd_grppitch_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVD_GRPPITCH_Y, m_edit_recipe_cvd_grppitch_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVD_POS_X, m_edit_recipe_cvd_pos_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVD_POS_Y, m_edit_recipe_cvd_pos_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVDDN_SIZE_X0, m_edit_recipe_cvddn_size_x0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVDDN_SIZE_Y0, m_edit_recipe_cvddn_size_y0);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVDDN_SIZE_X1, m_edit_recipe_cvddn_size_x1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVDDN_SIZE_Y1, m_edit_recipe_cvddn_size_y1);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVDDN_CNT_X, m_edit_recipe_cvddn_cnt_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVDDN_CNT_Y, m_edit_recipe_cvddn_cnt_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVDDN_PITCH_X, m_edit_recipe_cvddn_pitch_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVDDN_PITCH_Y, m_edit_recipe_cvddn_pitch_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVDDN_GRPCNT_X, m_edit_recipe_cvddn_grpcnt_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVDDN_GRPCNT_Y, m_edit_recipe_cvddn_grpcnt_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVDDN_GRPPITCH_X, m_edit_recipe_cvddn_grppitch_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVDDN_GRPPITCH_Y, m_edit_recipe_cvddn_grppitch_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVDDN_POS_X, m_edit_recipe_cvddn_pos_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVDDN_POS_Y, m_edit_recipe_cvddn_pos_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_ALIGN_CVD_X, m_edit_align_cvd);
	DDX_Text(pDX, IDC_EDIT_RECIPE_ALIGN_CVD_Y, m_edit_align_cvd_y);
	DDX_Text(pDX, IDC_EDIT_RECIPE_UP_DOWN_DIS, m_edit_up_dn_dis);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVD_GRP_DIS_X, m_edit_cvd_grp_dis_x);
	DDX_Text(pDX, IDC_EDIT_RECIPE_CVD_GRP_DIS_Y, m_edit_cvd_grp_dis_y);
}


BEGIN_MESSAGE_MAP(CDialogRecipeCell, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogRecipeCell 메시지 처리기입니다.


BOOL CDialogRecipeCell::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	LoadRecipeCellPara(_T(RECIPE_CELL_PATH));

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogRecipeCell::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow)
	{

	}
	else
	{

	}
}

//2010412 ngh
void CDialogRecipeCell::SetUpdate(BOOL isUpdate)
{
	UpdateData(isUpdate);
}

BOOL CDialogRecipeCell::SetLoadRecipeCellPara(CString path)
{
	return LoadRecipeCellPara(path);
}

BOOL CDialogRecipeCell::LoadRecipeCellPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	char path_temp[MAX_PATH];
	size_t getval;
	getval = 0;
	int i, idx;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;


	file_path = path + _T("\\RecipeCell.par");

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
		rslt = FALSE;
	}
	else
	{
		CloseHandle(fd);
		
		key.Empty();
		key.Format(_T("m_edit_cell_pos_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_cell_pos_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_cell_pos_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_cell_pos_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_cell_activesize_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_cell_activesize_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_cell_activesize_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_cell_activesize_y = atof(ctemp);




		key.Empty();
		key.Format(_T("m_edit_recipe_film_size_x0"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_film_size_x0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_size_y0"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_film_size_y0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_size_x1"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_film_size_x1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_size_y1"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_film_size_y1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_cnt_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_film_cnt_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_cnt_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_film_cnt_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_pitch_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_film_pitch_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_pitch_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_film_pitch_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_grpcnt_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_film_grpcnt_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_grpcnt_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_film_grpcnt_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_grppitch_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_film_grppitch_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_grppitch_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_film_grppitch_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_pos_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_film_pos_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_pos_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_film_pos_y = atof(ctemp);









		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_size_x0"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvd_size_x0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_size_y0"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvd_size_y0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_size_x1"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvd_size_x1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_size_y1"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvd_size_y1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_cnt_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvd_cnt_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_cnt_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvd_cnt_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_pitch_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvd_pitch_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_pitch_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvd_pitch_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_grpcnt_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvd_grpcnt_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_grpcnt_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvd_grpcnt_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_grppitch_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvd_grppitch_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_grppitch_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvd_grppitch_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_pos_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvd_pos_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_pos_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvd_pos_y = atof(ctemp);


		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_size_x0"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvddn_size_x0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_size_y0"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvddn_size_y0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_size_x1"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvddn_size_x1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_size_y1"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvddn_size_y1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_cnt_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvddn_cnt_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_cnt_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvddn_cnt_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_pitch_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvddn_pitch_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_pitch_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvddn_pitch_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_grpcnt_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvddn_grpcnt_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_grpcnt_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvddn_grpcnt_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_grppitch_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvddn_grppitch_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_grppitch_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvddn_grppitch_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_pos_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvddn_pos_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_pos_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_cvddn_pos_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_cvd"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_cvd = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_cvd_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_cvd_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_up_dn_dis"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_up_dn_dis = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_cvd_grp_dis_x"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_cvd_grp_dis_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_cvd_grp_dis_y"));
		GetPrivateProfileString(_T("RECIPE CELL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_cvd_grp_dis_y = atof(ctemp);

		
	}
	fd=0;

	return rslt;
}

BOOL CDialogRecipeCell::SetSaveRecipeCellPara(CString path)
{
	return SaveRecipeCellPara(path);
}

BOOL CDialogRecipeCell::SaveRecipeCellPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	char path_temp[MAX_PATH];
	size_t getval;
	getval = 0;
	int i, idx;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	
	file_path = path + _T("\\RecipeCell.par");

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
		rslt = FALSE;
	}
	else
	{
		CloseHandle(fd);

		key.Empty();
		key.Format(_T("m_edit_cell_pos_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_cell_pos_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_cell_pos_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_cell_pos_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_cell_activesize_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_cell_activesize_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_cell_activesize_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_cell_activesize_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_size_x0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_film_size_x0);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_size_y0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_film_size_y0);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_size_x1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_film_size_x1);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_size_y1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_film_size_y1);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_cnt_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_film_cnt_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_cnt_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_film_cnt_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_pitch_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_film_pitch_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_pitch_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_film_pitch_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_grpcnt_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_film_grpcnt_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_grpcnt_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_film_grpcnt_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_grppitch_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_film_grppitch_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_grppitch_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_film_grppitch_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_pos_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_film_pos_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_film_pos_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_film_pos_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);







		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_size_x0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvd_size_x0);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_size_y0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvd_size_y0);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_size_x1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvd_size_x1);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_size_y1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvd_size_y1);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_cnt_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvd_cnt_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_cnt_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvd_cnt_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_pitch_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvd_pitch_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_pitch_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvd_pitch_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_grpcnt_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvd_grpcnt_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_grpcnt_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvd_grpcnt_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_grppitch_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvd_grppitch_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_grppitch_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvd_grppitch_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_pos_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvd_pos_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvd_pos_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvd_pos_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);





		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_size_x0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvddn_size_x0);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_size_y0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvddn_size_y0);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_size_x1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvddn_size_x1);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_size_y1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvddn_size_y1);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_cnt_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvddn_cnt_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_cnt_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvddn_cnt_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_pitch_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvddn_pitch_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_pitch_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvddn_pitch_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_grpcnt_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvddn_grpcnt_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_grpcnt_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvddn_grpcnt_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_grppitch_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvddn_grppitch_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_grppitch_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvddn_grppitch_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);


		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_pos_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvddn_pos_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_recipe_cvddn_pos_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_recipe_cvddn_pos_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_cvd"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_cvd);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_cvd_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_cvd_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_up_dn_dis"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_up_dn_dis);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_cvd_grp_dis_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_cvd_grp_dis_x);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_cvd_grp_dis_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_cvd_grp_dis_y);
		WritePrivateProfileString(_T("RECIPE CELL"), key, dat, file_path);
	

	}
	fd=0;
	
	return rslt;
}

BEGIN_EVENTSINK_MAP(CDialogRecipeCell, CDialogEx)
	ON_EVENT(CDialogRecipeCell, IDC_BTNENHCTRL_RECIPE_CELL_APPLY, DISPID_CLICK, CDialogRecipeCell::ClickBtnenhctrlRecipeCellApply, VTS_NONE)
	ON_EVENT(CDialogRecipeCell, IDC_BTNENHCTRL_RECIPE_CELL_SAVE, DISPID_CLICK, CDialogRecipeCell::ClickBtnenhctrlRecipeCellSave, VTS_NONE)
END_EVENTSINK_MAP()

void CDialogRecipeCell::ClickBtnenhctrlRecipeCellApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	ParameterCompare();
	Update(TRUE);
	pdlg->m_Glass->SetUpdate();
}

double CDialogRecipeCell::GetCellPosFromCellAlignX()
{
	return m_edit_cell_pos_x;
}


double CDialogRecipeCell::GetCellPosFromCellAlignY()
{
	return m_edit_cell_pos_y;
}

double CDialogRecipeCell::GetCellActiveSizeX()
{
	return m_edit_cell_activesize_x;
}

double CDialogRecipeCell::GetCellActiveSizeY()
{
	return m_edit_cell_activesize_y;
}

double CDialogRecipeCell::GetFilmSizeX0()
{	
	return m_edit_recipe_film_size_x0;
}

double CDialogRecipeCell::GetFilmSizeY0()
{	
	return m_edit_recipe_film_size_y0;
}

double CDialogRecipeCell::GetFilmSizeX1()
{	
	return m_edit_recipe_film_size_x1;
}

double CDialogRecipeCell::GetFilmSizeY1()
{	
	return m_edit_recipe_film_size_y1;
}

double CDialogRecipeCell::GetFilmCntX()
{	
	return m_edit_recipe_film_cnt_x;
}

double CDialogRecipeCell::GetFilmCntY()
{	
	return m_edit_recipe_film_cnt_y;
}

double CDialogRecipeCell::GetFilmPitchX()
{	
	return m_edit_recipe_film_pitch_x;
}

double CDialogRecipeCell::GetFilmPitchY()
{	
	return m_edit_recipe_film_pitch_y;
}

double CDialogRecipeCell::GetFilmGrpCntX()
{	
	return m_edit_recipe_film_grpcnt_x;
}

double CDialogRecipeCell::GetFilmGrpCntY()
{	
	return m_edit_recipe_film_grpcnt_y;
}

double CDialogRecipeCell::GetFilmGrpPitchX()
{	
	return m_edit_recipe_film_grppitch_x;
}

double CDialogRecipeCell::GetFilmGrpPitchY()
{	
	return m_edit_recipe_film_grppitch_y;
}

double CDialogRecipeCell::GetFilmPosFromFilmAlignX()
{
	return m_edit_recipe_film_pos_x;
}

double CDialogRecipeCell::GetFilmPosFromFilmAlignY()
{
	return m_edit_recipe_film_pos_y;
}




double CDialogRecipeCell::GetCVDSizeX0()
{	
	return m_edit_recipe_cvd_size_x0;
}

double CDialogRecipeCell::GetCVDSizeY0()
{	
	return m_edit_recipe_cvd_size_y0;
}

double CDialogRecipeCell::GetCVDSizeX1()
{	
	return m_edit_recipe_cvd_size_x1;
}

double CDialogRecipeCell::GetCVDSizeY1()
{	
	return m_edit_recipe_cvd_size_y1;
}

double CDialogRecipeCell::GetCVDCntX()
{	
	return m_edit_recipe_cvd_cnt_x;
}

double CDialogRecipeCell::GetCVDCntY()
{	
	return m_edit_recipe_cvd_cnt_y;
}

double CDialogRecipeCell::GetCVDPitchX()
{	
	return m_edit_recipe_cvd_pitch_x;
}

double CDialogRecipeCell::GetCVDPitchY()
{	
	return m_edit_recipe_cvd_pitch_y;
}

double CDialogRecipeCell::GetCVDGrpCntX()
{	
	return m_edit_recipe_cvd_grpcnt_x;
}

double CDialogRecipeCell::GetCVDGrpCntY()
{	
	return m_edit_recipe_cvd_grpcnt_y;
}

double CDialogRecipeCell::GetCVDGrpPitchX()
{	
	return m_edit_recipe_cvd_grppitch_x;
}

double CDialogRecipeCell::GetCVDGrpPitchY()
{	
	return m_edit_recipe_cvd_grppitch_y;
}

double CDialogRecipeCell::GetCVDPosFromCVDAlignX()
{
	return m_edit_recipe_cvd_pos_x;
}

double CDialogRecipeCell::GetCVDPosFromCVDAlignY()
{
	return m_edit_recipe_cvd_pos_y;
}




double CDialogRecipeCell::GetCVDDNSizeX0()
{	
	return m_edit_recipe_cvddn_size_x0;
}

double CDialogRecipeCell::GetCVDDNSizeY0()
{	
	return m_edit_recipe_cvddn_size_y0;
}

double CDialogRecipeCell::GetCVDDNSizeX1()
{	
	return m_edit_recipe_cvddn_size_x1;
}

double CDialogRecipeCell::GetCVDDNSizeY1()
{	
	return m_edit_recipe_cvddn_size_y1;
}

double CDialogRecipeCell::GetCVDDNCntX()
{	
	return m_edit_recipe_cvddn_cnt_x;
}

double CDialogRecipeCell::GetCVDDNCntY()
{	
	return m_edit_recipe_cvddn_cnt_y;
}

double CDialogRecipeCell::GetCVDDNPitchX()
{	
	return m_edit_recipe_cvddn_pitch_x;
}

double CDialogRecipeCell::GetCVDDNPitchY()
{	
	return m_edit_recipe_cvddn_pitch_y;
}

double CDialogRecipeCell::GetCVDDNGrpCntX()
{	
	return m_edit_recipe_cvddn_grpcnt_x;
}

double CDialogRecipeCell::GetCVDDNGrpCntY()
{	
	return m_edit_recipe_cvddn_grpcnt_y;
}

double CDialogRecipeCell::GetCVDDNGrpPitchX()
{	
	return m_edit_recipe_cvddn_grppitch_x;
}

double CDialogRecipeCell::GetCVDDNGrpPitchY()
{	
	return m_edit_recipe_cvddn_grppitch_y;
}



//20160526 sh
double CDialogRecipeCell::GetCVDAlignFirstCVDX()
{
	return m_edit_align_cvd;
}

double CDialogRecipeCell::GetCVDAlignFirstCVDY()
{
	return m_edit_align_cvd_y;
}

double CDialogRecipeCell::GetCVDUpDnDis()
{
	return m_edit_up_dn_dis;
}

double CDialogRecipeCell::GetCVDGrpDisX()
{
	return m_edit_cvd_grp_dis_x;
}

double CDialogRecipeCell::GetCVDGrpDisY()
{
	return m_edit_cvd_grp_dis_y;
}
/*
공용
double CDialogRecipeCell::GetCVDDNPosFromCVDAlignX()
{
	return m_edit_recipe_cvddn_pos_x;
}

double CDialogRecipeCell::GetCVDDNPosFromCVDAlignY()
{
	return m_edit_recipe_cvddn_pos_y;
}
*/


//Glass///////////////////////////////////////
double CDialogRecipeCell::GetCellPosFromCellAlignX_coordXY()
{
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return -m_edit_cell_pos_y;
}


double CDialogRecipeCell::GetCellPosFromCellAlignY_coordXY()
{
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_cell_pos_x;
}

double CDialogRecipeCell::GetCellActiveSizeX_coordXY()
{
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_cell_activesize_y;
}

double CDialogRecipeCell::GetCellActiveSizeY_coordXY()
{
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_cell_activesize_x;
}







double CDialogRecipeCell::GetFilmSizeX0_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_film_size_y0;
}

double CDialogRecipeCell::GetFilmSizeY0_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_film_size_x0;
}

double CDialogRecipeCell::GetFilmSizeX1_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_film_size_y1;
}

double CDialogRecipeCell::GetFilmSizeY1_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_film_size_x1;
}

double CDialogRecipeCell::GetFilmCntX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_film_cnt_y;
}

double CDialogRecipeCell::GetFilmCntY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_film_cnt_x;
}

double CDialogRecipeCell::GetFilmPitchX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return -m_edit_recipe_film_pitch_y;
}

double CDialogRecipeCell::GetFilmPitchY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_film_pitch_x;
}

double CDialogRecipeCell::GetFilmGrpCntX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_film_grpcnt_y;
}

double CDialogRecipeCell::GetFilmGrpCntY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_film_grpcnt_x;
}

double CDialogRecipeCell::GetFilmGrpPitchX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return -m_edit_recipe_film_grppitch_y;
}

double CDialogRecipeCell::GetFilmGrpPitchY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_film_grppitch_x;
}

double CDialogRecipeCell::GetFilmPosFromFilmAlignX_coordXY()
{
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return -m_edit_recipe_film_pos_y;
}

double CDialogRecipeCell::GetFilmPosFromFilmAlignY_coordXY()
{
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_film_pos_x;
}






double CDialogRecipeCell::GetCVDSizeX0_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvd_size_y0;
}

double CDialogRecipeCell::GetCVDSizeY0_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvd_size_x0;
}

double CDialogRecipeCell::GetCVDSizeX1_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvd_size_y1;
}

double CDialogRecipeCell::GetCVDSizeY1_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvd_size_x1;
}

double CDialogRecipeCell::GetCVDCntX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvd_cnt_y;
}

double CDialogRecipeCell::GetCVDCntY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvd_cnt_x;
}

double CDialogRecipeCell::GetCVDPitchX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return -m_edit_recipe_cvd_pitch_y;
}

double CDialogRecipeCell::GetCVDPitchY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvd_pitch_x;
}

double CDialogRecipeCell::GetCVDGrpCntX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvd_grpcnt_y;
}

double CDialogRecipeCell::GetCVDGrpCntY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvd_grpcnt_x;
}

double CDialogRecipeCell::GetCVDGrpPitchX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return -m_edit_recipe_cvd_grppitch_y;
}

double CDialogRecipeCell::GetCVDGrpPitchY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvd_grppitch_x;
}

double CDialogRecipeCell::GetCVDPosFromCVDAlignX_coordXY()
{
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return -m_edit_recipe_cvd_pos_y;
}

double CDialogRecipeCell::GetCVDPosFromCVDAlignY_coordXY()
{
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvd_pos_x;
}



double CDialogRecipeCell::GetCVDDNSizeX0_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvddn_size_y0;
}

double CDialogRecipeCell::GetCVDDNSizeY0_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvddn_size_x0;
}

double CDialogRecipeCell::GetCVDDNSizeX1_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvddn_size_y1;
}

double CDialogRecipeCell::GetCVDDNSizeY1_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvddn_size_x1;
}

double CDialogRecipeCell::GetCVDDNCntX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvddn_cnt_y;
}

double CDialogRecipeCell::GetCVDDNCntY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvddn_cnt_x;
}

double CDialogRecipeCell::GetCVDDNPitchX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return -m_edit_recipe_cvddn_pitch_y;
}

double CDialogRecipeCell::GetCVDDNPitchY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvddn_pitch_x;
}

double CDialogRecipeCell::GetCVDDNGrpCntX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvddn_grpcnt_y;
}

double CDialogRecipeCell::GetCVDDNGrpCntY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvddn_grpcnt_x;
}

double CDialogRecipeCell::GetCVDDNGrpPitchX_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return -m_edit_recipe_cvddn_grppitch_y;
}

double CDialogRecipeCell::GetCVDDNGrpPitchY_coordXY()
{	
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvddn_grppitch_x;
}

double CDialogRecipeCell::GetCVDDNPosFromCVDDNAlignX_coordXY()
{
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return -m_edit_recipe_cvddn_pos_y;
}

double CDialogRecipeCell::GetCVDDNPosFromCVDDNAlignY_coordXY()
{
	//축이 x, y 바뀌어 있다. 반대로 리턴
	return m_edit_recipe_cvddn_pos_x;
}




typeRecipeCell CDialogRecipeCell::GetRecipe()
{
	typeRecipeCell dat;

	dat.dCell_pos_x = GetCellPosFromCellAlignX();
	dat.dCell_pos_y = GetCellPosFromCellAlignY();
	dat.dCell_activesize_x = GetCellActiveSizeX();
	dat.dCell_activesize_y = GetCellActiveSizeY();
	dat.dRecipe_film_size_x0 = GetFilmSizeX0();
	dat.dRecipe_film_size_y0 = GetFilmSizeY0();
	dat.dRecipe_film_size_x1 = GetFilmSizeX1();
	dat.dRecipe_film_size_y1 = GetFilmSizeY1();
	dat.dRecipe_film_cnt_x = GetFilmCntX();
	dat.dRecipe_film_cnt_y = GetFilmCntY();
	dat.dRecipe_film_pitch_x = GetFilmPitchX();
	dat.dRecipe_film_pitch_y = GetFilmPitchY();;
	dat.dRecipe_film_grpcnt_x = GetFilmGrpCntX();
	dat.dRecipe_film_grpcnt_y = GetFilmGrpCntY();
	dat.dRecipe_film_grppitch_x = GetFilmGrpPitchX();
	dat.dRecipe_film_grppitch_y = GetFilmGrpPitchY();
	dat.dRecipe_film_pos_x = GetFilmPosFromFilmAlignX();
	dat.dRecipe_film_pos_y = GetFilmPosFromFilmAlignY();

	return dat;
}



BOOL CDialogRecipeCell::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogRecipeCell::Update(BOOL isUpdate)
{
	int idx = 0;
	if(isUpdate)
	{
		m_dRecipeCellPara[idx++] = m_edit_cell_pos_x;
		m_dRecipeCellPara[idx++] = m_edit_cell_pos_y;
		m_dRecipeCellPara[idx++] = m_edit_cell_activesize_x;
		m_dRecipeCellPara[idx++] = m_edit_cell_activesize_y;
		m_dRecipeCellPara[idx++] = m_edit_recipe_film_size_x0;
		m_dRecipeCellPara[idx++] = m_edit_recipe_film_size_y0;
		m_dRecipeCellPara[idx++] = m_edit_recipe_film_size_x1;
		m_dRecipeCellPara[idx++] = m_edit_recipe_film_size_y1;
		m_dRecipeCellPara[idx++] = m_edit_recipe_film_cnt_x;
		m_dRecipeCellPara[idx++] = m_edit_recipe_film_cnt_y;
		m_dRecipeCellPara[idx++] = m_edit_recipe_film_pitch_x;
		m_dRecipeCellPara[idx++] = m_edit_recipe_film_pitch_y;
		m_dRecipeCellPara[idx++] = m_edit_recipe_film_grpcnt_x;
		m_dRecipeCellPara[idx++] = m_edit_recipe_film_grpcnt_y;
		m_dRecipeCellPara[idx++] = m_edit_recipe_film_grppitch_x;
		m_dRecipeCellPara[idx++] = m_edit_recipe_film_grppitch_y;
		m_dRecipeCellPara[idx++] = m_edit_recipe_film_pos_x;
		m_dRecipeCellPara[idx++] = m_edit_recipe_film_pos_y;

		m_dRecipeCellPara[idx++] = m_edit_recipe_cvd_size_x0;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvd_size_y0;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvd_size_x1;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvd_size_y1;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvd_cnt_x;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvd_cnt_y;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvd_pitch_x;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvd_pitch_y;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvd_grpcnt_x;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvd_grpcnt_y;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvd_grppitch_x;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvd_grppitch_y;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvd_pos_x;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvd_pos_y;

		m_dRecipeCellPara[idx++] = m_edit_recipe_cvddn_size_x0;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvddn_size_y0;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvddn_size_x1;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvddn_size_y1;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvddn_cnt_x;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvddn_cnt_y;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvddn_pitch_x;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvddn_pitch_y;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvddn_grpcnt_x;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvddn_grpcnt_y;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvddn_grppitch_x;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvddn_grppitch_y;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvddn_pos_x;
		m_dRecipeCellPara[idx++] = m_edit_recipe_cvddn_pos_y;
	}
	else
	{
		m_edit_cell_pos_x = m_dRecipeCellPara[idx++];
		m_edit_cell_pos_y = m_dRecipeCellPara[idx++];
		m_edit_cell_activesize_x = m_dRecipeCellPara[idx++];
		m_edit_cell_activesize_y = m_dRecipeCellPara[idx++];
		m_edit_recipe_film_size_x0 = m_dRecipeCellPara[idx++];
		m_edit_recipe_film_size_y0 = m_dRecipeCellPara[idx++];
		m_edit_recipe_film_size_x1 = m_dRecipeCellPara[idx++];
		m_edit_recipe_film_size_y1 = m_dRecipeCellPara[idx++];
		m_edit_recipe_film_cnt_x = m_dRecipeCellPara[idx++];
		m_edit_recipe_film_cnt_y = m_dRecipeCellPara[idx++];
		m_edit_recipe_film_pitch_x = m_dRecipeCellPara[idx++];
		m_edit_recipe_film_pitch_y = m_dRecipeCellPara[idx++];
		m_edit_recipe_film_grpcnt_x = m_dRecipeCellPara[idx++];
		m_edit_recipe_film_grpcnt_y = m_dRecipeCellPara[idx++];
		m_edit_recipe_film_grppitch_x = m_dRecipeCellPara[idx++];
		m_edit_recipe_film_grppitch_y = m_dRecipeCellPara[idx++];
		m_edit_recipe_film_pos_x = m_dRecipeCellPara[idx++];
		m_edit_recipe_film_pos_y = m_dRecipeCellPara[idx++];
		
		m_edit_recipe_cvd_size_x0 = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvd_size_y0 = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvd_size_x1 = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvd_size_y1 = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvd_cnt_x = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvd_cnt_y = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvd_pitch_x = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvd_pitch_y = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvd_grpcnt_x = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvd_grpcnt_y = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvd_grppitch_x = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvd_grppitch_y = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvd_pos_x = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvd_pos_y = m_dRecipeCellPara[idx++];
		
		m_edit_recipe_cvddn_size_x0 = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvddn_size_y0 = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvddn_size_x1 = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvddn_size_y1 = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvddn_cnt_x = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvddn_cnt_y = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvddn_pitch_x = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvddn_pitch_y = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvddn_grpcnt_x = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvddn_grpcnt_y = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvddn_grppitch_x = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvddn_grppitch_y = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvddn_pos_x = m_dRecipeCellPara[idx++];
		m_edit_recipe_cvddn_pos_y = m_dRecipeCellPara[idx++];
	}
}

void CDialogRecipeCell::ParameterCompare()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strType;
	strType.Format(_T("RecipeCell_Para_"));
	int idx = 0;

	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_cell_pos_x, strType + _T("m_edit_cell_pos_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_cell_pos_y, strType + _T("m_edit_cell_pos_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_cell_activesize_x, strType + _T("m_edit_cell_activesize_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_cell_activesize_y, strType + _T("m_edit_cell_activesize_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_film_size_x0, strType + _T("m_edit_recipe_film_size_x0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_film_size_y0, strType + _T("m_edit_recipe_film_size_y0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_film_size_x1, strType + _T("m_edit_recipe_film_size_x1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_film_size_y1, strType + _T("m_edit_recipe_film_size_y1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_film_cnt_x, strType + _T("m_edit_recipe_film_cnt_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_film_cnt_y, strType + _T("m_edit_recipe_film_cnt_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_film_pitch_x, strType + _T("m_edit_recipe_film_pitch_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_film_pitch_y, strType + _T("m_edit_recipe_film_pitch_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_film_grpcnt_x, strType + _T("m_edit_recipe_film_grpcnt_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_film_grpcnt_y, strType + _T("m_edit_recipe_film_grpcnt_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_film_grppitch_x, strType + _T("m_edit_recipe_film_grppitch_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_film_grppitch_y, strType + _T("m_edit_recipe_film_grppitch_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_film_pos_x, strType + _T("m_edit_recipe_film_pos_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_film_pos_y, strType + _T("m_edit_recipe_film_pos_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvd_size_x0, strType + _T("m_edit_recipe_cvd_size_x0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvd_size_y0, strType + _T("m_edit_recipe_cvd_size_y0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvd_size_x1, strType + _T("m_edit_recipe_cvd_size_x1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvd_size_y1, strType + _T("m_edit_recipe_cvd_size_y1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvd_cnt_x, strType + _T("m_edit_recipe_cvd_cnt_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvd_cnt_y, strType + _T("m_edit_recipe_cvd_cnt_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvd_pitch_x, strType + _T("m_edit_recipe_cvd_pitch_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvd_pitch_y, strType + _T("m_edit_recipe_cvd_pitch_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvd_grpcnt_x, strType + _T("m_edit_recipe_cvd_grpcnt_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvd_grpcnt_y, strType + _T("m_edit_recipe_cvd_grpcnt_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvd_grppitch_x, strType + _T("m_edit_recipe_cvd_grppitch_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvd_grppitch_y, strType + _T("m_edit_recipe_cvd_grppitch_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvd_pos_x, strType + _T("m_edit_recipe_cvd_pos_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvd_pos_y, strType + _T("m_edit_recipe_cvd_pos_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvddn_size_x0, strType + _T("m_edit_recipe_cvddn_size_x0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvddn_size_y0, strType + _T("m_edit_recipe_cvddn_size_y0"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvddn_size_x1, strType + _T("m_edit_recipe_cvddn_size_x1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvddn_size_y1, strType + _T("m_edit_recipe_cvddn_size_y1"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvddn_cnt_x, strType + _T("m_edit_recipe_cvddn_cnt_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvddn_cnt_y, strType + _T("m_edit_recipe_cvddn_cnt_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvddn_pitch_x, strType + _T("m_edit_recipe_cvddn_pitch_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvddn_pitch_y, strType + _T("m_edit_recipe_cvddn_pitch_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvddn_grpcnt_x, strType + _T("m_edit_recipe_cvddn_grpcnt_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvddn_grpcnt_y, strType + _T("m_edit_recipe_cvddn_grpcnt_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvddn_grppitch_x, strType + _T("m_edit_recipe_cvddn_grppitch_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvddn_grppitch_y, strType + _T("m_edit_recipe_cvddn_grppitch_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvddn_pos_x, strType + _T("m_edit_recipe_cvddn_pos_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeCellPara[idx++], m_edit_recipe_cvddn_pos_y, strType + _T("m_edit_recipe_cvddn_pos_y"));
}

void CDialogRecipeCell::ClickBtnenhctrlRecipeCellSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CDialogRecipeModel mod_dlg;
	mod_dlg.SetOpen();
	UpdateData(TRUE);
	ParameterCompare();
	Update(TRUE);
	SetSaveRecipeCellPara(RECIPE_CELL_PCL_PATH);
	SetSaveRecipeCellPara(mod_dlg.GetRecipePath());
}
