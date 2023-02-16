// Dialog/DialogFilmAlign.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EMIControl.h"

#include "Include/_CtrlImageRect.h"
#include "include/_FuncModuleHal.h"
#include "include/_FuncModuleBasic.h"

#include "include/Runtime.h"
#include "include/PMACComm.h"

#include "Dialog/DialogRecipePanel.h"
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
#include "Dialog/DialogGlassManuCtrl.h"
#include "Dialog/DialogFilmAlign.h"
#include "EMIControlDlg.h"
#include "afxdialogex.h"



// CDialogFilmAlign 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogFilmAlign, CDialogEx)

CDialogFilmAlign::CDialogFilmAlign(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogFilmAlign::IDD, pParent)
	, m_check_film_move(FALSE)
	, m_check_rgn_move(FALSE)
	, m_radio_scan_ud(0)
	, m_radio_scan_lr(0)
	, m_edit_thrsh_ud(0)
	, m_edit_thrsh_lr(0)
	, m_edit_film_num_x(1)
	, m_edit_film_num_y(1)
{
	memset(&m_FilmAlignData, NULL, sizeof(typeFilmAlignData));
	memset(&m_FilmAlignRslt, NULL, sizeof(typeFilmAlignResult));

	m_iSelectRgnNum = 0;
}

CDialogFilmAlign::~CDialogFilmAlign()
{
	SaveFilmAlignPara(_T(FILM_ALIGN_PATH));
}

void CDialogFilmAlign::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNENHCTRL_FILM_ALIGN_POS0, m_btn_rgn_pos0);
	DDX_Control(pDX, IDC_BTNENHCTRL_FILM_ALIGN_POS1, m_btn_rgn_pos1);
	DDX_Control(pDX, IDC_BTNENHCTRL_FILM_ALIGN_POS2, m_btn_rgn_pos2);
	DDX_Control(pDX, IDC_BTNENHCTRL_FILM_ALIGN_POS3, m_btn_rgn_pos3);
	DDX_Control(pDX, IDC_BTNENHCTRL_FILM_ALIGN_POS4, m_btn_rgn_pos4);
	DDX_Control(pDX, IDC_BTNENHCTRL_FILM_ALIGN_POS5, m_btn_rgn_pos5);
	DDX_Check(pDX, IDC_CHECK_FILM_ALIGN_FILM_MOVE, m_check_film_move);
	DDX_Check(pDX, IDC_CHECK_FILM_ALIGN_FILM_MOVE2, m_check_rgn_move);
	DDX_Radio(pDX, IDC_RADIO_FILM_ALIGN_UD0, m_radio_scan_ud);
	DDX_Radio(pDX, IDC_RADIO_FILM_ALIGN_LR0, m_radio_scan_lr);
	DDX_Text(pDX, IDC_EDIT_FILM_ALIGN_UD_THRSH, m_edit_thrsh_ud);
	DDX_Text(pDX, IDC_EDIT_FILM_ALIGN_LR_THRSH, m_edit_thrsh_lr);
	DDX_Text(pDX, IDC_EDIT_FILM_ALIGN_X, m_edit_film_num_x);
	DDX_Text(pDX, IDC_EDIT_FILM_ALIGN_Y, m_edit_film_num_y);
}


BEGIN_MESSAGE_MAP(CDialogFilmAlign, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogFilmAlign 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDialogFilmAlign, CDialogEx)
//	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_POS0, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos0, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_POS1, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos1, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_POS2, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos2, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_POS5, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos5, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_POS4, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos4, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_POS3, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos3, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_MOVE, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignMove, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_LF, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignLf, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_RT, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignRt, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_UP, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignUp, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_DN, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignDn, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_MOVEPOS, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignMovepos, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_INSPECT, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignInspect, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_APPLY, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignApply, VTS_NONE)
	ON_EVENT(CDialogFilmAlign, IDC_BTNENHCTRL_FILM_ALIGN_POS0, DISPID_CLICK, CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos0, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_btn_rgn_pos0.SetValue(FALSE);
	m_btn_rgn_pos1.SetValue(FALSE);
	m_btn_rgn_pos2.SetValue(FALSE);
	m_btn_rgn_pos3.SetValue(FALSE);
	m_btn_rgn_pos4.SetValue(FALSE);
	m_btn_rgn_pos5.SetValue(FALSE);

	m_btn_rgn_pos0.SetValue(TRUE);
	m_iSelectRgnNum = 0;

	Update(FALSE);
	UpdateData(FALSE);
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_btn_rgn_pos0.SetValue(FALSE);
	m_btn_rgn_pos1.SetValue(FALSE);
	m_btn_rgn_pos2.SetValue(FALSE);
	m_btn_rgn_pos3.SetValue(FALSE);
	m_btn_rgn_pos4.SetValue(FALSE);
	m_btn_rgn_pos5.SetValue(FALSE);

	m_btn_rgn_pos1.SetValue(TRUE);
	m_iSelectRgnNum = 1;

	Update(FALSE);
	UpdateData(FALSE);
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_btn_rgn_pos0.SetValue(FALSE);
	m_btn_rgn_pos1.SetValue(FALSE);
	m_btn_rgn_pos2.SetValue(FALSE);
	m_btn_rgn_pos3.SetValue(FALSE);
	m_btn_rgn_pos4.SetValue(FALSE);
	m_btn_rgn_pos5.SetValue(FALSE);

	m_btn_rgn_pos2.SetValue(TRUE);
	m_iSelectRgnNum = 2;

	Update(FALSE);
	UpdateData(FALSE);
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos5()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_btn_rgn_pos0.SetValue(FALSE);
	m_btn_rgn_pos1.SetValue(FALSE);
	m_btn_rgn_pos2.SetValue(FALSE);
	m_btn_rgn_pos3.SetValue(FALSE);
	m_btn_rgn_pos4.SetValue(FALSE);
	m_btn_rgn_pos5.SetValue(FALSE);

	m_btn_rgn_pos5.SetValue(TRUE);
	m_iSelectRgnNum = 5;

	Update(FALSE);
	UpdateData(FALSE);
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos4()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_btn_rgn_pos0.SetValue(FALSE);
	m_btn_rgn_pos1.SetValue(FALSE);
	m_btn_rgn_pos2.SetValue(FALSE);
	m_btn_rgn_pos3.SetValue(FALSE);
	m_btn_rgn_pos4.SetValue(FALSE);
	m_btn_rgn_pos5.SetValue(FALSE);

	m_btn_rgn_pos4.SetValue(TRUE);
	m_iSelectRgnNum = 4;

	Update(FALSE);
	UpdateData(FALSE);
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignPos3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_btn_rgn_pos0.SetValue(FALSE);
	m_btn_rgn_pos1.SetValue(FALSE);
	m_btn_rgn_pos2.SetValue(FALSE);
	m_btn_rgn_pos3.SetValue(FALSE);
	m_btn_rgn_pos4.SetValue(FALSE);
	m_btn_rgn_pos5.SetValue(FALSE);

	m_btn_rgn_pos3.SetValue(TRUE);
	m_iSelectRgnNum = 3;

	Update(FALSE);
	UpdateData(FALSE);
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignLf()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignRt()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignUp()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignDn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignMovepos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CDialogFilmAlign::ClickBtnenhctrlFilmAlignInspect()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	int cam_idx;
	HTuple RowEdgeFirst, ColumnEdgeFirst, AmplitudeFirst, RowEdgeSecond, ColumnEdgeSecond, AmplitudeSecond, MeasureHandle, IntraDistance, InterDistance;
	Hlong row, col, leng, height;
	Hlong edgeCnt;
	HTuple wd, ht;
	Herror herr;
	cam_idx = 0;

	UpdateData(TRUE);
	Update(TRUE);

	int film_idx = 0;
	int rgn_idx = m_iSelectRgnNum;

	/*
	m_FilmAlignData.m_dThrshLR[film_idx][rgn_idx] = m_edit_thrsh_lr;
	m_FilmAlignData.m_dThrshUD[film_idx][rgn_idx] = m_edit_thrsh_ud;

	m_FilmAlignData.m_iScanUD[film_idx][rgn_idx] = m_radio_scan_ud;
	m_FilmAlignData.m_iScanRD[film_idx][rgn_idx] = m_radio_scan_lr;

	pdlg->m_Review->GetFilmAlignRect(film_idx, rgn_idx, 0, &m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx]);
	pdlg->m_Review->GetFilmAlignRect(film_idx, rgn_idx, 1, &m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx]);
	*/

	disp_obj(*pdlg->m_Review->GetImage(), pdlg->m_Review->GetView());

	double mark_width, mark_height;
	mark_width = 0.;
	mark_height = 0.;


	set_check("~give_error");
	herr = get_image_pointer1(*pdlg->m_Review->GetImage(), NULL, NULL, &wd, &ht);
	set_check("give_error");
	if(H_MSG_TRUE == herr)
	{
	}
	else
	{
		AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1!"));
		return;
	}

	//Left right
	row = (Hlong)((m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx].bottom + m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx].top) * 0.5);
	col = (Hlong)((m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx].right + m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx].left) * 0.5);
	leng = (m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx].right - m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx].left) * 0.5;
	gen_measure_rectangle2(row,col,HTuple(0),leng,ALIGN_MEASURE_LENG,wd,ht,HTuple("nearest_neighbor"),&MeasureHandle);

	measure_pairs(*pdlg->m_Review->GetImage(), MeasureHandle, HTuple(10), HTuple(m_edit_thrsh_lr), HTuple("negative_strongest"), HTuple("all"),
			            &RowEdgeFirst,&ColumnEdgeFirst,&AmplitudeFirst,  
						&RowEdgeSecond,&ColumnEdgeSecond,&AmplitudeSecond,
						&IntraDistance, &InterDistance);
	
	edgeCnt = RowEdgeFirst.Num();
	if(0 < edgeCnt)
	{
		set_color(pdlg->m_Review->GetView(), HTuple("green"));
		//disp_line(pdlg->m_Review->GetView(), RowEdgeFirst[0], ColumnEdgeFirst[0], RowEdgeSecond[0], ColumnEdgeSecond[0]);
		
		
		disp_line(pdlg->m_Review->GetView(), 0, ColumnEdgeFirst[0], ht, ColumnEdgeFirst[0]);
		disp_line(pdlg->m_Review->GetView(), 0,ColumnEdgeSecond[0], ht, ColumnEdgeSecond[0]);
	}
	else
	{
		set_color(pdlg->m_Review->GetView(), HTuple("red"));
		set_tposition(pdlg->m_Review->GetView(), 10, 10);
		write_string(pdlg->m_Review->GetView(), HTuple("Can't find!"));
	}

	//Up dn
	row = (Hlong)((m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].bottom + m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].top) * 0.5);
	col = (Hlong)((m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].right + m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].left) * 0.5);
	//leng = (m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].right - m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].left) * 0.5;
	//gen_measure_rectangle2(row,col,HTuple(0),leng,ALIGN_MEASURE_LENG,wd,ht,HTuple("nearest_neighbor"),&MeasureHandle);

	leng = (m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].bottom - m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx].top) * 0.5;
	gen_measure_rectangle2(row,col,HTuple(-1.57079632679),leng,ALIGN_MEASURE_LENG,wd,ht,HTuple("nearest_neighbor"),&MeasureHandle);

	measure_pairs(*pdlg->m_Review->GetImage(), MeasureHandle, HTuple(10), HTuple(m_edit_thrsh_ud), HTuple("negative_strongest"), HTuple("all"),
			            &RowEdgeFirst,&ColumnEdgeFirst,&AmplitudeFirst,  
						&RowEdgeSecond,&ColumnEdgeSecond,&AmplitudeSecond,
						&IntraDistance, &InterDistance);
	
	edgeCnt = RowEdgeFirst.Num();
	if(0 < edgeCnt)
	{
		set_color(pdlg->m_Review->GetView(), HTuple("green"));
		//disp_line(pdlg->m_Review->GetView(), RowEdgeFirst[0], ColumnEdgeFirst[0], RowEdgeSecond[0], ColumnEdgeSecond[0]);
		
		
		disp_line(pdlg->m_Review->GetView(), RowEdgeFirst[0], 0, RowEdgeFirst[0], wd);
		disp_line(pdlg->m_Review->GetView(), RowEdgeSecond[0], 0, RowEdgeSecond[0], wd);
	}
	else
	{
		set_color(pdlg->m_Review->GetView(), HTuple("red"));
		set_tposition(pdlg->m_Review->GetView(), 10, 10);
		write_string(pdlg->m_Review->GetView(), HTuple("Can't find!"));
	}
}

/*
/*
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Hobject modimage;
	HTuple wd, ht;
	Herror herr;
	int cam_idx;
	int thresh;
	CRect rect;
	RECT pat_rect;
	thresh = ALIGN_THRESH;
	double rx, ry;
	rx = 0.;
	ry = 0.;
	
	UpdateData(TRUE);
	Update(TRUE);
	cam_idx = 0;

	disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
	m_dlgImgRect[cam_idx]->Display();


	set_check("~give_error");
	herr = get_image_pointer1(m_HalImage[cam_idx], NULL, NULL, &wd, &ht);
	set_check("give_error");
	if(H_MSG_TRUE == herr)
	{
	}
	else
	{
		AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1!"));
		return;
	}

	//Mark size find////////////////////////////////////////////
	HTuple RowEdgeFirst, ColumnEdgeFirst, AmplitudeFirst, RowEdgeSecond, ColumnEdgeSecond, AmplitudeSecond, MeasureHandle, IntraDistance, InterDistance;
	Hlong row, col, leng, height;
	Hlong edgeCnt;
	double mark_width, mark_height;
	mark_width = 0.;
	mark_height = 0.;

	//Width
	row = (Hlong)((m_AlignData.m_PatRect[cam_idx].bottom + m_AlignData.m_PatRect[cam_idx].top) * 0.5);
	col = (Hlong)((m_AlignData.m_PatRect[cam_idx].right + m_AlignData.m_PatRect[cam_idx].left) * 0.5);
	leng = (m_AlignData.m_PatRect[cam_idx].right - m_AlignData.m_PatRect[cam_idx].left) * 0.5;
	gen_measure_rectangle2(row,col,HTuple(0),leng,ALIGN_MEASURE_LENG,wd,ht,HTuple("nearest_neighbor"),&MeasureHandle);

	measure_pairs(m_HalImage[cam_idx], MeasureHandle, HTuple(1), HTuple(ALIGN_THRESH), HTuple("positive_strongest"), HTuple("all"),
			            &RowEdgeFirst,&ColumnEdgeFirst,&AmplitudeFirst,  
						&RowEdgeSecond,&ColumnEdgeSecond,&AmplitudeSecond,
						&IntraDistance, &InterDistance);
	
	edgeCnt = RowEdgeFirst.Num();
	if(0 < edgeCnt)
	{
		set_color(m_dlgImgRect[cam_idx]->GetView(), HTuple("blue"));
		disp_line(m_dlgImgRect[cam_idx]->GetView(), RowEdgeFirst[0], ColumnEdgeFirst[0], RowEdgeSecond[0], ColumnEdgeSecond[0]);
		pat_rect.left = (int)((double)ColumnEdgeFirst[0] - 0.5);
		pat_rect.right = (int)((double)ColumnEdgeSecond[0] + 0.5);
		mark_width = IntraDistance[0];
	}
	else
	{
		m_AlignData.m_bCreatPatIDAlign[cam_idx] = FALSE;
		
		set_color(m_dlgImgRect[cam_idx]->GetView(), HTuple("red"));
		set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
		write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("Teach(rect is not center) fail!"));
		return;
	}

	//Height
	leng = (m_AlignData.m_PatRect[cam_idx].bottom - m_AlignData.m_PatRect[cam_idx].top) * 0.5;
	gen_measure_rectangle2(row,col,HTuple(-1.57079632679),leng,ALIGN_MEASURE_LENG,wd,ht,HTuple("nearest_neighbor"),&MeasureHandle);

	measure_pairs(m_HalImage[cam_idx], MeasureHandle, HTuple(1), HTuple(ALIGN_THRESH), HTuple("positive_strongest"), HTuple("all"),
			            &RowEdgeFirst,&ColumnEdgeFirst,&AmplitudeFirst,  
						&RowEdgeSecond,&ColumnEdgeSecond,&AmplitudeSecond,
						&IntraDistance, &InterDistance);
	
	edgeCnt = RowEdgeFirst.Num();
	if(0 < edgeCnt)
	{
		set_color(m_dlgImgRect[cam_idx]->GetView(), HTuple("blue"));
		disp_line(m_dlgImgRect[cam_idx]->GetView(), RowEdgeFirst[0], ColumnEdgeFirst[0], RowEdgeSecond[0], ColumnEdgeSecond[0]);
		pat_rect.top = (int)((double)RowEdgeFirst[0] - 0.5);
		pat_rect.bottom = (int)((double)RowEdgeSecond[0] + 0.5);
		mark_height = IntraDistance[0];
	}
	else
	{
		m_AlignData.m_bCreatPatIDAlign[cam_idx] = FALSE;

		set_color(m_dlgImgRect[cam_idx]->GetView(), HTuple("red"));
		set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
		write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("Teach(height) fail!"));
		return;
	}

	//Teach////////////////////////////////////////////////////////////////
	if(Hal_ModelTeachingRect1(m_dlgImgRect[cam_idx]->GetView(), 
							 &m_HalImage[cam_idx], 
							 &pat_rect,
							 (Hlong)wd[0],
							 (Hlong)ht[0],
							 m_AlignPara[cam_idx], 
							 thresh,								  //threshold
							 thresh * 0.5,							  //min threshold
							 &rx,  //pattern cx 
							 &ry,  //pattern cy
							 &modimage, 
							 &m_AlignData.m_PatIDAlign[cam_idx])
    	)
	{
		m_AlignData.m_bCreatPatIDAlign[cam_idx] = TRUE;

		if( (0 < mark_width) && (0 < mark_height) )
		{
			m_edit_align_mark_res0 = ((m_edit_align_mark_size0 / mark_width) + (m_edit_align_mark_size0 / mark_height)) * 0.5;
			m_edit_align_pixel_size = (m_edit_align_mark_res0 + m_edit_align_mark_res1) * 0.5;
		}
		else
		{
			m_edit_align_mark_res0 = 1;
			m_edit_align_pixel_size = 1;

		}
	}
	else
	{
		m_AlignData.m_bCreatPatIDAlign[cam_idx] = FALSE;
	}

	UpdateData(FALSE);
*/


void CDialogFilmAlign::ClickBtnenhctrlFilmAlignApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Update(TRUE);
}

BOOL CDialogFilmAlign::LoadFilmAlignPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	char path_temp[MAX_PATH];
	size_t getval;
	getval = 0;
	int i, j, idx;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	
	file_path = path + _T("\\FilmAlign.par");

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

		for(i = 0; i < FILM_ALIGN_MAX_NUM; i++)
		{
			for(j = 0; j < FILM_ALIGN_RGN_POS_MAX_MUM; j++)
			{
				//ins rect0//////////////////////////////////////////////////////////////
				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectLR_left%d_%d"), i, j);
				m_FilmAlignData.m_InsRectLR[i][j].left = GetPrivateProfileInt(_T("FILM ALIGN"), key, 0, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectLR_top%d_%d"), i, j);
				m_FilmAlignData.m_InsRectLR[i][j].top = GetPrivateProfileInt(_T("FILM ALIGN"), key, 0, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectLR_right%d_%d"), i, j);
				m_FilmAlignData.m_InsRectLR[i][j].right = GetPrivateProfileInt(_T("FILM ALIGN"), key, 0, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectLR_bottom%d_%d"), i, j);
				m_FilmAlignData.m_InsRectLR[i][j].bottom = GetPrivateProfileInt(_T("FILM ALIGN"), key, 0, file_path);

				if( (0 >= m_FilmAlignData.m_InsRectLR[i][j].left) ||
					(REVIEW_IMAGE_WIDTH < m_FilmAlignData.m_InsRectLR[i][j].left) ||
					(0 >= m_FilmAlignData.m_InsRectLR[i][j].top) ||
					(REVIEW_IMAGE_HEIGHT <m_FilmAlignData.m_InsRectLR[i][j].top) ||
					(0 >= m_FilmAlignData.m_InsRectLR[i][j].right) ||
					(REVIEW_IMAGE_WIDTH < m_FilmAlignData.m_InsRectLR[i][j].right) ||
					(0 >= m_FilmAlignData.m_InsRectLR[i][j].bottom) ||
					(REVIEW_IMAGE_HEIGHT < m_FilmAlignData.m_InsRectLR[i][j].bottom)
				  )
				{
					m_FilmAlignData.m_InsRectLR[i][j] = CRect(100, 100, 500, 500);
				}

				//ins rect1//////////////////////////////////////////////////////////////
				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectUD_left%d_%d"), i, j);
				m_FilmAlignData.m_InsRectUD[i][j].left = GetPrivateProfileInt(_T("FILM ALIGN"), key, 0, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectUD_top%d_%d"), i, j);
				m_FilmAlignData.m_InsRectUD[i][j].top = GetPrivateProfileInt(_T("FILM ALIGN"), key, 0, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectUD_right%d_%d"), i, j);
				m_FilmAlignData.m_InsRectUD[i][j].right = GetPrivateProfileInt(_T("FILM ALIGN"), key, 0, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectUD_bottom%d_%d"), i, j);
				m_FilmAlignData.m_InsRectUD[i][j].bottom = GetPrivateProfileInt(_T("FILM ALIGN"), key, 0, file_path);

				if( (0 >= m_FilmAlignData.m_InsRectUD[i][j].left) ||
					(REVIEW_IMAGE_WIDTH < m_FilmAlignData.m_InsRectUD[i][j].left) ||
					(0 >= m_FilmAlignData.m_InsRectUD[i][j].top) ||
					(REVIEW_IMAGE_HEIGHT <m_FilmAlignData.m_InsRectUD[i][j].top) ||
					(0 >= m_FilmAlignData.m_InsRectUD[i][j].right) ||
					(REVIEW_IMAGE_WIDTH < m_FilmAlignData.m_InsRectUD[i][j].right) ||
					(0 >= m_FilmAlignData.m_InsRectUD[i][j].bottom) ||
					(REVIEW_IMAGE_HEIGHT < m_FilmAlignData.m_InsRectUD[i][j].bottom)
				  )
				{
					m_FilmAlignData.m_InsRectUD[i][j] = CRect(700, 100, 1100, 500);
				}

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_dThrshLR%d_%d"), i, j);
				GetPrivateProfileString(_T("FILM ALIGN"), key, _T("0.0"), temp, sizeof(temp), file_path);
				dat = temp;
				wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
				m_FilmAlignData.m_dThrshLR[i][j] = atof(ctemp);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_dThrshUD%d_%d"), i, j);
				GetPrivateProfileString(_T("FILM ALIGN"), key, _T("0.0"), temp, sizeof(temp), file_path);
				dat = temp;
				wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
				m_FilmAlignData.m_dThrshUD[i][j] = atof(ctemp);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_iScanUD%d_%d"), i, j);
				GetPrivateProfileString(_T("FILM ALIGN"), key, _T("0.0"), temp, sizeof(temp), file_path);
				dat = temp;
				wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
				m_FilmAlignData.m_iScanUD[i][j] = atof(ctemp);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_iScanRD%d_%d"), i, j);
				GetPrivateProfileString(_T("FILM ALIGN"), key, _T("0.0"), temp, sizeof(temp), file_path);
				dat = temp;
				wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
				m_FilmAlignData.m_iScanRD[i][j] = atof(ctemp);
			}

		}

	}
	fd=0;

	return rslt;
}

BOOL CDialogFilmAlign::SaveFilmAlignPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	char path_temp[MAX_PATH];
	size_t getval;
	getval = 0;
	int i, j, idx;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	
	file_path = path + _T("\\FilmAlign.par");

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

		for(i = 0; i < FILM_ALIGN_MAX_NUM; i++)
		{
			for(j = 0; j < FILM_ALIGN_RGN_POS_MAX_MUM; j++)
			{
				//ins rect0//////////////////////////////////////////////////////////////
				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectLR_left%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_FilmAlignData.m_InsRectLR[i][j].left);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectLR_top%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_FilmAlignData.m_InsRectLR[i][j].top);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectLR_right%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_FilmAlignData.m_InsRectLR[i][j].right);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectLR_bottom%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_FilmAlignData.m_InsRectLR[i][j].bottom);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);


				//ins rect1//////////////////////////////////////////////////////////////
				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectUD_left%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_FilmAlignData.m_InsRectUD[i][j].left);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectUD_top%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_FilmAlignData.m_InsRectUD[i][j].top);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectUD_right%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_FilmAlignData.m_InsRectUD[i][j].right);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				idx = 0;
				key.Empty();
				key.Format(_T("m_FilmAlignData.m_InsRectUD_bottom%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%d"), m_FilmAlignData.m_InsRectUD[i][j].bottom);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_dThrshLR%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%f"), m_FilmAlignData.m_dThrshLR[i][j]);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_dThrshUD%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%f"), m_FilmAlignData.m_dThrshUD[i][j]);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);

				key.Empty();
				key.Format(_T("m_FilmAlignData.m_iScanUD%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%f"), m_FilmAlignData.m_iScanUD[i][j]);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);


				key.Empty();
				key.Format(_T("m_FilmAlignData.m_iScanRD%d_%d"), i, j);
				dat.Empty();
				dat.Format(_T("%f"), m_FilmAlignData.m_iScanRD[i][j]);
				WritePrivateProfileString(_T("FILM ALIGN"), key, dat, file_path);


			}

		}
	}
	fd=0;
	
	return rslt;
}


BOOL CDialogFilmAlign::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	LoadFilmAlignPara(_T(FILM_ALIGN_PATH));

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogFilmAlign::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow)
	{
		m_btn_rgn_pos0.SetValue(FALSE);
		m_btn_rgn_pos1.SetValue(FALSE);
		m_btn_rgn_pos2.SetValue(FALSE);
		m_btn_rgn_pos3.SetValue(FALSE);
		m_btn_rgn_pos4.SetValue(FALSE);
		m_btn_rgn_pos5.SetValue(FALSE);
		switch(m_iSelectRgnNum)
		{
		case 0:
			m_btn_rgn_pos0.SetValue(TRUE);
			break;
		case 1:
			m_btn_rgn_pos1.SetValue(TRUE);
			break;
		case 2:
			m_btn_rgn_pos2.SetValue(TRUE);
			break;
		case 3:
			m_btn_rgn_pos3.SetValue(TRUE);
			break;
		case 4:
			m_btn_rgn_pos4.SetValue(TRUE);
			break;
		case 5:
			m_btn_rgn_pos5.SetValue(TRUE);
			break;
		default:
			AfxMessageBox(_T("Error : invalid index(CDialogFilmAlign::OnShowWindow)!"));
			break;
		}

		Update(FALSE);

		UpdateData(FALSE);
	}
	else
	{


	}
}

//X, Y 좌표계(수학)
void CDialogFilmAlign::Update(BOOL type)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();

	int i;
	int img_idx;
	int film_idx;
	img_idx = 0;
	film_idx = 0;

	int cell_x_idx, cell_y_idx, rgn_idx;
	cell_x_idx = m_edit_film_num_x;
	cell_y_idx = m_edit_film_num_y;

	rgn_idx = m_iSelectRgnNum;

	/*
	cell_x_idx--;
	if(0 > cell_x_idx)
	{
		cell_x_idx = 0;
		//Error log

	}
	cell_y_idx--;
	if(0 > cell_y_idx)
	{
		cell_y_idx = 0;
		//Error log

	}
	
	film_idx = (cell_y_idx * pdlg->m_RecipeCell->GetFilmCntX_glass()) + cell_x_idx;
	*/

	if(type)
	{
		m_FilmAlignData.m_dThrshLR[film_idx][rgn_idx] = m_edit_thrsh_lr;
		m_FilmAlignData.m_dThrshUD[film_idx][rgn_idx] = m_edit_thrsh_ud;

		m_FilmAlignData.m_iScanUD[film_idx][rgn_idx] = m_radio_scan_ud;
		m_FilmAlignData.m_iScanRD[film_idx][rgn_idx] = m_radio_scan_lr;

		pdlg->m_Review->GetFilmAlignRect(film_idx, rgn_idx, 0, &m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx]);

		pdlg->m_Review->GetFilmAlignRect(film_idx, rgn_idx, 1, &m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx]);
	}
	else
	{
		m_edit_thrsh_lr = m_FilmAlignData.m_dThrshLR[film_idx][rgn_idx];
		m_edit_thrsh_ud = m_FilmAlignData.m_dThrshUD[film_idx][rgn_idx];

		m_radio_scan_ud = m_FilmAlignData.m_iScanUD[film_idx][rgn_idx];
		m_radio_scan_lr = m_FilmAlignData.m_iScanRD[film_idx][rgn_idx];

		

		pdlg->m_Review->SetFilmAlignRect(TRUE);
		//Region LR
		pdlg->m_Review->SetFilmAlignRect(cell_x_idx, cell_y_idx, film_idx, rgn_idx, 0, m_FilmAlignData.m_InsRectLR[film_idx][rgn_idx]);
		//Region UD
		pdlg->m_Review->SetFilmAlignRect(cell_x_idx, cell_y_idx, film_idx, rgn_idx, 1, m_FilmAlignData.m_InsRectUD[film_idx][rgn_idx]);
	}
}


