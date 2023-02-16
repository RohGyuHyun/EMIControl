// Dialog/DialogAlign.cpp : 구현 파일입니다.
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
#include "EMIControlDlg.h"
#include "SerialPort.h"

#include "afxdialogex.h"


// CDialogAlign 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogAlign, CDialogEx)

CDialogAlign::CDialogAlign(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogAlign::IDD, pParent)
	, m_edit_align_y_axi_pos(0)
	, m_edit_align_t_axi_pos(0)
	, m_edit_align_offset_y(0)
	, m_edit_align_offset_x(0)
	, m_edit_align_mark_size0(0)
	, m_edit_align_mark_size1(0)
	, m_edit_align_mark_res0(0)
	, m_edit_align_mark_res1(0)
	, m_edit_align_posx0(0)
	, m_edit_align_posy0(0)
	, m_edit_align_posx1(0)
	, m_edit_align_posy1(0)
	, m_edit_align_shaftx(0)
	, m_edit_align_shafty(0)
	, m_edit_align_light0(0)
	, m_edit_align_light1(0)
	, m_edit_offset_x_rat(0)
	, m_edit_offset_y_rat(0)
	, m_edit_align_pixel_size(1)
	, m_edit_align_offset_t(0)
	, m_edit_align_jog_pitch(0)
	, m_edit_align_shaft_offset_x(0)
	, m_edit_align_shaft_offset_y(0)
	, m_edit_align_x_axi_pos(0)
{
	int i;

	m_iCamNum = 0;

	for(i = 0; i < ALIGN_CAM_MAX_NUM; i++)
	{
		m_bIsDisp[i] = FALSE;
		m_bIsSnap[i] = FALSE;
	}

	memset(&m_AlignData, NULL, sizeof(typeAlignData));
	memset(&m_AlignRslt, NULL, sizeof(typeAlignResult));

	for(i = 0; i < ALIGN_CAM_MAX_NUM; i++)
	{
		m_AlignData.m_InsRect[i] = CRect(100, 100, 500, 500);
		m_AlignData.m_PatRect[i] = CRect(200, 200, 400, 400);
	}

	//fixed
	for(i = 0; i < ALIGN_CAM_MAX_NUM; i++)
	{
		m_AlignPara[i].m_shType = VISIONHALFUNC_MOD_RECT1;
		m_AlignPara[i].m_AngleStart = 0;
		m_AlignPara[i].m_AngleExtend = 0;
		m_AlignPara[i].m_AngleStep = 0;
		sprintf(m_AlignPara[i].m_Optimize, "none");
		sprintf(m_AlignPara[i].m_Matric, "ignore_global_polarity");
		sprintf(m_AlignPara[i].m_SupPix, "interpolation");
		m_AlignPara[i].m_Overlap = 0;

		m_AlignPara[i].m_MatchNum = 1;
		m_AlignPara[i].m_Greed = 0.5;
		m_AlignPara[i].m_ScaleMin = 1.0;
		m_AlignPara[i].m_ScaleMax = 1.0;
		m_AlignPara[i].m_ScaleStep = 0;
	}
	m_iJogSpdSelect = 0;
	m_bAlignInspection = FALSE;
	m_bAlignTestInspection1 = FALSE;
	m_bAlignTestInspection2 = FALSE;
}

CDialogAlign::~CDialogAlign()
{
	SaveAlignPara(_T(ALIGN_PATH));

	KillTimer(0);

	//Teli
	CloseStream();
	CloseAllCameras();
	Sys_Terminate();

}

void CDialogAlign::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ALIGN_SCAN_AXI_POS0, m_edit_align_y_axi_pos);
	DDX_Text(pDX, IDC_EDIT_ALIGN_THETA_AXI_POS0, m_edit_align_t_axi_pos);
	DDX_Text(pDX, IDC_EDIT_ALIGN_OFFSET0, m_edit_align_offset_y);
	DDX_Text(pDX, IDC_EDIT_ALIGN_OFFSET1, m_edit_align_offset_x);
	DDX_Text(pDX, IDC_EDIT_ALIGN_MARK_SIZE0, m_edit_align_mark_size0);
	DDX_Text(pDX, IDC_EDIT_ALIGN_MARK_SIZE1, m_edit_align_mark_size1);
	DDX_Text(pDX, IDC_EDIT_ALIGN_MARK_RES0, m_edit_align_mark_res0);
	DDX_Text(pDX, IDC_EDIT_ALIGN_MARK_RES1, m_edit_align_mark_res1);
	DDX_Text(pDX, IDC_EDIT_ALIGN_POSX0, m_edit_align_posx0);
	DDX_Text(pDX, IDC_EDIT_ALIGN_POSY0, m_edit_align_posy0);
	DDX_Text(pDX, IDC_EDIT_ALIGN_POSX1, m_edit_align_posx1);
	DDX_Text(pDX, IDC_EDIT_ALIGN_POSY1, m_edit_align_posy1);
	DDX_Text(pDX, IDC_EDIT_ALIGN_SHAFTX, m_edit_align_shaftx);
	DDX_Text(pDX, IDC_EDIT_ALIGN_SHAFTY, m_edit_align_shafty);
	DDX_Text(pDX, IDC_EDIT_ALIGN_LIGHT0, m_edit_align_light0);
	DDX_Text(pDX, IDC_EDIT_ALIGN_LIGHT1, m_edit_align_light1);
	DDX_Control(pDX, IDC_SLIDER_ALIGN_LIGHT0, m_slider_align_light0);
	DDX_Control(pDX, IDC_SLIDER_ALIGN_LIGHT1, m_slider_align_light1);
	DDX_Text(pDX, IDC_EDIT_ALIGN_OFFSET_X_RAT, m_edit_offset_x_rat);
	DDX_Text(pDX, IDC_EDIT_ALIGN_OFFSET_Y_RAT, m_edit_offset_y_rat);
	DDX_Text(pDX, IDC_EDIT_ALIGN_PIXEL_SIZE, m_edit_align_pixel_size);
	DDX_Text(pDX, IDC_EDIT_ALIGN_OFFSET_T, m_edit_align_offset_t);
	DDX_Control(pDX, IDC_BTNENHCTRL_ALIGN_JOG_SPD1, m_btn_jog_spd0);
	DDX_Control(pDX, IDC_BTNENHCTRL_ALIGN_JOG_SPD2, m_btn_jog_spd1);
	DDX_Control(pDX, IDC_BTNENHCTRL_ALIGN_JOG_SPD3, m_btn_jog_spd2);
	DDX_Control(pDX, IDC_BTNENHCTRL_ALIGN_LIGHT_SET0, m_btn_align_light_set0);
	DDX_Control(pDX, IDC_BTNENHCTRL_ALIGN_LIGHT_SET1, m_btn_align_light_set1);
	DDX_Text(pDX, IDC_EDIT_ALIGN_SCAN_PITCH, m_edit_align_jog_pitch);
	DDX_Text(pDX, IDC_EDIT_ALIGN_SHIFTOFFSETX, m_edit_align_shaft_offset_x);
	DDX_Text(pDX, IDC_EDIT_ALIGN_SHIFTOFFSETY, m_edit_align_shaft_offset_y);
	DDX_Text(pDX, IDC_EDIT_ALIGN_SHIFT_AXI_POS, m_edit_align_x_axi_pos);
}


BEGIN_MESSAGE_MAP(CDialogAlign, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(USER_MSG_IMG_RECT, OnImageRectMessage)
	ON_WM_TIMER()
//	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ALIGN_LIGHT0, &CDialogAlign::OnNMCustomdrawSliderAlignLight0)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ALIGN_LIGHT0, &CDialogAlign::OnNMCustomdrawSliderAlignLight0)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ALIGN_LIGHT1, &CDialogAlign::OnNMCustomdrawSliderAlignLight1)
END_MESSAGE_MAP()


// CDialogAlign 메시지 처리기입니다.


BOOL CDialogAlign::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	
	Herror herr;	
	CRect rect;
	int i;
	HTuple wd, ht;
	long width, height;
	
	for(i = 0; i < ALIGN_IMG_RECT_MAX_NUM; i++)
	{
		m_dlgImgRect[i] = new C_CtrlImageRect;
		m_dlgImgRect[i]->Create(IMAGE_RECT_DIALOG, this);

		switch(i)
		{
		case 0:
			GetDlgItem(IDC_STATIC_ALIGN0)->GetClientRect(rect);
			GetDlgItem(IDC_STATIC_ALIGN0)->ClientToScreen(rect);

			m_dlgImgRect[i]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);
			m_dlgImgRect[i]->SetCamIndex(i, _T("                                         Camera #1"), this->m_hWnd);
			break;
		case 1:
			GetDlgItem(IDC_STATIC_ALIGN1)->GetClientRect(rect);
			GetDlgItem(IDC_STATIC_ALIGN1)->ClientToScreen(rect);
			m_dlgImgRect[i]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);
			m_dlgImgRect[i]->SetCamIndex(i, _T("                                         Camera #2"), this->m_hWnd);
			break;
		default:
			AfxMessageBox(_T("Error : invalid index(CDialogAlign::OnInitDialog())!"));
			strlog.Format(_T("Error : invalid index(CDialogAlign::OnInitDialog())!"));
			pdlg->WriteLog(ALIGN_LOG,strlog);
			break;
		}

		m_dlgImgRect[i]->ShowWindow(SW_SHOW);

	}

	//Halcon image initialize
	for(i = 0; i < ALIGN_IMG_RECT_MAX_NUM; i++)
	{
		gen_image_const(&m_HalImage[i], HTuple("byte"), ALIGN_IMAGE_WIDTH, ALIGN_IMAGE_HEIGHT);

		set_check("~give_error");
			herr = get_image_pointer1(m_HalImage[i], NULL, NULL, &wd, &ht);
		set_check("give_error");

		if(H_MSG_TRUE == herr)
		{
		}
		else
		{
			AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1_00!"));
			strlog.Format(_T("Halcon Error Occured at get_image_pointer1_00!"));
			pdlg->WriteLog(ALIGN_LOG,strlog);
			return FALSE;
		}			

		width = (Hlong)wd[0];
		height = (Hlong)ht[0];
		
		m_dlgImgRect[i]->SetZoom(0, (long)width, (long)height);

		set_font(m_dlgImgRect[i]->GetView(), "-Arial-12-*-*-*-*-*-1-");
		set_draw(m_dlgImgRect[i]->GetView(), HTuple("margin"));

		disp_obj(m_HalImage[i], m_dlgImgRect[i]->GetView());	

		m_dlgImgRect[i]->ShowWindow(SW_HIDE);
	}

#ifdef CAMERA
	//Teli
	if(Open())
	{
		OpenStream();
	}
	else
	{
		AfxMessageBox(_T("Error : teli camera open fail!"));
		strlog.Format(_T("Error : teli camera open fail!"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
	}
#endif

	//Load data
	LoadAlignPara(_T(ALIGN_PATH));

	m_slider_align_light0.SetRange(0,255);
	if(0 > m_edit_align_light0)
	{
		m_edit_align_light0 = 0;
	}
	if(255 < m_edit_align_light0)
	{
		m_edit_align_light0 = 255;
	}
	m_slider_align_light0.SetPos(m_edit_align_light0);

	m_slider_align_light1.SetRange(0,255);
	if(0 > m_edit_align_light1)
	{
		m_edit_align_light1 = 0;
	}
	if(255 < m_edit_align_light1)
	{
		m_edit_align_light1 = 255;
	}
	m_slider_align_light1.SetPos(m_edit_align_light1);

	UpdateData(FALSE);

	uint32_t    status = 0;
	// Send acquisition command to camera
#ifdef CAMERA
	for(int cam_idx = 0; cam_idx < 2; cam_idx++)
	{
		status = Strm_Start(m_hStrm[cam_idx]);
		if (status != CAM_API_STS_SUCCESS)
		{
			AfxMessageBox(_T("Error : grab fail(CDialogAlign::Grab(CDialogAlign::Grab)!"));
			strlog.Format(_T("Error : grab fail(CDialogAlign::Grab(CDialogAlign::Grab)!"));
			pdlg->WriteLog(ALIGN_LOG,strlog);
			return TRUE;
		}
	}
#endif
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogAlign::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	CRect rect;
	int i;
	double panel_size_x, panel_size_y;
	double mark_pos_x0, mark_pos_y0, mark_pos_x1, mark_pos_y1;

	if(bShow)
	{
		for(i = 0; i < ALIGN_IMG_RECT_MAX_NUM; i++)
		{
			switch(i)
			{
			case 0:
				GetDlgItem(IDC_STATIC_ALIGN0)->GetClientRect(rect);
				GetDlgItem(IDC_STATIC_ALIGN0)->ClientToScreen(rect);
				m_dlgImgRect[i]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);

				m_edit_align_mark_size0 = pdlg->m_RecipePanel->GetMarkSize(i);

				mark_pos_x0 = pdlg->m_RecipePanel->GetMarkPosX(i);
				mark_pos_y0 = pdlg->m_RecipePanel->GetMarkPosY(i);
				break;
			case 1:
				GetDlgItem(IDC_STATIC_ALIGN1)->GetClientRect(rect);
				GetDlgItem(IDC_STATIC_ALIGN1)->ClientToScreen(rect);
				m_dlgImgRect[i]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);

				m_edit_align_mark_size1 = pdlg->m_RecipePanel->GetMarkSize(i);

				mark_pos_x1 = pdlg->m_RecipePanel->GetMarkPosX(i);
				mark_pos_y1 = pdlg->m_RecipePanel->GetMarkPosY(i);
				break;
			default:
				AfxMessageBox(_T("Error : invalid index(CDialogAlign::OnInitDialog())!"));
				strlog.Format(_T("Error : invalid index(CDialogAlign::OnInitDialog())!"));
				pdlg->WriteLog(ALIGN_LOG,strlog);
				break;
			}

			m_dlgImgRect[i]->ShowWindow(SW_SHOW);

		}

		//Shift축 중심
		panel_size_x = pdlg->m_RecipePanel->GetPanelSizeX();
		panel_size_y = pdlg->m_RecipePanel->GetPanelSizeY();

		m_edit_align_shaftx = (panel_size_x * 0.5) - mark_pos_x0;
		m_edit_align_shafty = (panel_size_y * 0.5) - mark_pos_y0;

		Update(FALSE);

		SetTimer(0, 200, NULL);
	}
	else
	{
		ClickBtnenhctrlAlignStop();

		for(int i = 0; i < ALIGN_IMG_RECT_MAX_NUM; i++)
		{
			m_dlgImgRect[i]->ShowWindow(SW_HIDE);
		}
		KillTimer(0);
	}

}
BEGIN_EVENTSINK_MAP(CDialogAlign, CDialogEx)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_SNAP, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignSnap, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_LIVE, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignLive, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_STOP, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignStop, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_FINDPOS, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignFindpos, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_MOVEPOS, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignMovepos, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_MARK_TEACH0, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignMarkTeach0, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_MARK_TEACH1, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignMarkTeach1, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_SCAN_JOG_P, DISPID_MOUSEDOWN, CDialogAlign::MouseDownBtnenhctrlAlignScanJogP, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_SCAN_JOG_P, DISPID_MOUSEUP, CDialogAlign::MouseUpBtnenhctrlAlignScanJogP, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_SCAN_JOG_M, DISPID_MOUSEDOWN, CDialogAlign::MouseDownBtnenhctrlAlignScanJogM, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_SCAN_JOG_M, DISPID_MOUSEUP, CDialogAlign::MouseUpBtnenhctrlAlignScanJogM, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_JOG_CCW, DISPID_MOUSEUP, CDialogAlign::MouseUpBtnenhctrlAlignJogCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_JOG_CW, DISPID_MOUSEDOWN, CDialogAlign::MouseDownBtnenhctrlAlignJogCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_JOG_CW, DISPID_MOUSEUP, CDialogAlign::MouseUpBtnenhctrlAlignJogCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_JOG_SPD1, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignJogSpd1, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_JOG_SPD2, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignJogSpd2, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_JOG_SPD3, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignJogSpd3, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_APPLY, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignApply, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_MARK_COPY_0_TO_1, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignMarkCopy0To1, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_JOG_CCW, DISPID_MOUSEDOWN, CDialogAlign::MouseDownBtnenhctrlAlignJogCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_MARK_FIND0, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignMarkFind0, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_MARK_POSSET0, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignMarkPosset0, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_MARK_POSSET1, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignMarkPosset1, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_LIGHT_SET0, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignLightSet0, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_LIGHT_SET1, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignLightSet1, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_MARK_FIND1, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignMarkFind1, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_SAVE, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignSave, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_SET_ALIGN_POS, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignSetAlignPos, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_ALIGN_MOVE, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignAlignMove, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_REVIEW_MOVE_POS0, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignReviewMovePos0, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_REVIEW_MOVE_POS1, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignReviewMovePos1, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_SET_ALIGN_POS_SHIFT, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignSetAlignPosShift, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_ALIGN_MOVE_SHIFT, DISPID_CLICK, CDialogAlign::ClickBtnenhctrlAlignAlignMoveShift, VTS_NONE)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_JOG_L, DISPID_MOUSEDOWN, CDialogAlign::MouseDownBtnenhctrlAlignJogL, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_JOG_L, DISPID_MOUSEUP, CDialogAlign::MouseUpBtnenhctrlAlignJogL, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_JOG_R, DISPID_MOUSEDOWN, CDialogAlign::MouseDownBtnenhctrlAlignJogR, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogAlign, IDC_BTNENHCTRL_ALIGN_JOG_R, DISPID_MOUSEUP, CDialogAlign::MouseUpBtnenhctrlAlignJogR, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CDialogAlign::ClickBtnenhctrlAlignSnap()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	for (int i = 0; i < m_iCamNum; ++i)
	{
		Grab(i, TRUE);
	}
}


void CDialogAlign::ClickBtnenhctrlAlignLive()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	uint32_t    status = 0;
	// Send acquisition command to camera
	for (int i = 0; i < m_iCamNum; ++i)
	{
		Live(i, TRUE);
	}


}


BOOL CDialogAlign::Open()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	strlog.Format(_T("Open()"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	BOOL rslt = FALSE;

	uint32_t    status = 0;
	CString  str;

	// Initialize Api
	Sys_Initialize();

	// Stop receiving image
	CloseAllCameras();
	
	rslt = Initialize();
	if (!rslt)
	{
		CloseAllCameras();
	}

	return rslt;
}

void CDialogAlign::CloseAllCameras()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	strlog.Format(_T("CloseAllCameras()"));
    for (uint32_t i = 0; i < ALIGN_CAM_MAX_NUM; i++)
    {
        if (m_hCam[i] != NULL)
        {
            Cam_Close(m_hCam[i]);
            m_hCam[i] = NULL;
        }
    }
	
}

BOOL CDialogAlign::Initialize()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	strlog.Format(_T("Initialize() Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	uint32_t cam_count = 0;	
	CAM_API_STATUS status = 0;

    // Get number of cameras
    Sys_GetNumOfCameras(&cam_count);
	m_iCamNum = cam_count;
    if (status != CAM_API_STS_SUCCESS)
	{	// Failed to get count of cameras.
		strlog.Format(_T("Failed to get count of cameras"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
		return FALSE;
	}

    if (cam_count == 0)
	{
		strlog.Format(_T("Error : cam_count == 0"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
		return FALSE;
	}

    if (cam_count > ALIGN_CAM_MAX_NUM)
	{	// if detected too much cameras. use first MAX_CAMERA cameras.
		cam_count = ALIGN_CAM_MAX_NUM;
	}

	// Show camera information in EDIT_CAMn
	CAM_INFO sCameraInfo;
    for (uint32_t i = 0; i < cam_count; i++)
    {	
        // Get camera information
        status = Cam_GetInformation(NULL, i, &sCameraInfo);
        if (status != CAM_API_STS_SUCCESS)
		{	//Failed to get camera information.
			strlog.Format(_T("Failed to get camera information."));
			pdlg->WriteLog(ALIGN_LOG,strlog);
            return FALSE;
		}

        // Open camera. 
        status = Cam_Open(i,&m_hCam[i]);
        if (status != CAM_API_STS_SUCCESS)
		{	//Failed to open camera.
			strlog.Format(_T("Failed to open camera."));
			pdlg->WriteLog(ALIGN_LOG,strlog);
            return FALSE;
		}
    }

    //
    if (cam_count == 0)
    {
		strlog.Format(_T("Error : cam_count == 0"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
        return FALSE;
    }
	strlog.Format(_T("Initialize() End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);	
    return TRUE;
}

BOOL CDialogAlign::OpenStream()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	strlog.Format(_T("OpenStream()"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	uint32_t  uiPyld = 0;	
	CAM_API_STATUS status = 0;

	for (int i = 0; i < m_iCamNum; i++)
	{
		// Open stream channel.
		//  Use default Payload size (pyld = 0) and  MaxPacketSize(U3v : 65536, GEV : 1500)
		//
		//  If 0x0000100C(CAM_API_STS_TOO_MANY_PACKET_MISSING) is returned from Strm_DequeueRequest() in ImgRcvThread,
		//  try enabling Jumbo-Packet feature of the network adapter and changing MaxPacketSize value of the opening stream, in the case of GigE Vision camera.
		//
		//  If Junmbo-frame value of network adapter is 9014 or 9000 or 9k, Use 9000 for MaxPacketSize.
		//  Value "9014" includes ehernet header length 14 bytes. (Source MAC address 6 bytes + Destination MAC address 6 bytes + Protocol ID 2 bytes)
		//
		//  If Junmbo-frame value of network adapter is 4088 or 4074 or 4k, Use 4074 for MaxPacketSize. 
		//
		//  If too large value is set to MaxPacketSize argument, 0xFFFFFFFF (CAM_API_STS_UNSUCCESSFUL) will be returned from Strm_Open() API. 
		//
		//  The following is code of opening stream with MAxPacketsIze 9000.
		//
		// m_uiStatus = Strm_OpenSimple(m_hCam[i], &m_hStrm[i], &uiPyld, NULL, 5, 9000);
		//
		//
		status = Strm_OpenSimple(m_hCam[i], &m_hStrm[i], &uiPyld, NULL);
		if (status != CAM_API_STS_SUCCESS)
		{
			return FALSE;
		}

		// Allocate memory for showing image in main form. (32bit BGRA)
		m_pvImgBGRA[i] = VirtualAlloc(NULL, 
									  ALIGN_IMAGE_WIDTH * ALIGN_IMAGE_HEIGHT,
									  MEM_RESERVE | MEM_COMMIT,
									  PAGE_EXECUTE_READWRITE);
		if (m_pvImgBGRA[i] == NULL)
		{
			return FALSE;
		}

		// Set callback for image acquired event 
		status = Strm_SetCallbackImageAcquired(m_hStrm[i], this, &CDialogAlign::CallBackImageAcquired);

		// Set callback for stream error event 
		//status = Strm_SetCallbackImageError(m_hStrm[i], this, &CDialogAlign::CallBackErrorImage);
	}



	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// Close Stream
//
//
////////////////////////////////////////////////////////////////////////////////
BOOL CDialogAlign::CloseStream()
{
	for (uint32_t i = 0; i < m_iCamNum; i++)
	{
		// Close stream.
		if (m_hStrm[i] != NULL)
		{
			Strm_Stop(m_hStrm[i]);
			Strm_Close(m_hStrm[i]);
			m_hStrm[i] = NULL;
		}

		// Release BGRA image data of each camera..
		if (m_pvImgBGRA[i] != NULL)
		{
			VirtualFree(m_pvImgBGRA[i], 0, MEM_RELEASE);
			m_pvImgBGRA[i] = NULL;
		}

		//m_uiFrmCount[i] = 0;
	}
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	strlog.Format(_T("CloseStream()"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	return TRUE;
}

void CDialogAlign::Grab(int cam_idx, BOOL disp)
{
	//uint32_t    status = 0;
	//// Send acquisition command to camera
	//
	//status = Strm_Start(m_hStrm[cam_idx]);
	//if (status != CAM_API_STS_SUCCESS)
	//{
	//	AfxMessageBox(_T("Error : grab fail(CDialogAlign::Grab(CDialogAlign::Grab)!"));
	//	return;
	//}
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	strlog.Format(_T("Grab(cam_idx : %d , disp : %d) Start"),cam_idx,disp);
	pdlg->WriteLog(ALIGN_LOG,strlog);

	m_bIsDisp[cam_idx] = disp;
	m_bIsSnap[cam_idx] = TRUE;
	strlog.Format(_T("Grab(cam_idx : %d , disp : %d) End"),cam_idx,disp);
	pdlg->WriteLog(ALIGN_LOG,strlog);
}

void CDialogAlign::Live(int cam_idx, BOOL disp)
{
	//uint32_t    status = 0;
	//// Send acquisition command to camera
	//
	//status = Strm_Start(m_hStrm[cam_idx]);
	//if (status != CAM_API_STS_SUCCESS)
	//{
	//	AfxMessageBox(_T("Error : grab fail(CDialogAlign::Grab(CDialogAlign::Live)!"));
	//	return;
	//}


	m_bIsDisp[cam_idx] = disp;
	m_bIsSnap[cam_idx] = FALSE;

	Sleep(200);
}

void CDialogAlign::Stop(int cam_idx)
{
	//CAM_API_STATUS status = 0;

	//// send acquisition stop command to camera
	//if (m_hStrm[cam_idx] != NULL)
	//{
	//	status = Strm_Stop(m_hStrm[cam_idx]);
	//	if (status != CAM_API_STS_SUCCESS)
	//	{
	//		// Set Acquisition active flag.
	//		return;
	//	}
	//}
	

	m_bIsDisp[cam_idx] = FALSE;
	m_bIsSnap[cam_idx] = FALSE;

	Sleep(200);
}

//2010412 ngh
void CDialogAlign::SetUpdate(BOOL isUpdate)
{
	Update(isUpdate);
}

void CDialogAlign::Update(BOOL flag)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	strlog.Format(_T("Update(flag : %d ) Start"),flag);
	pdlg->WriteLog(ALIGN_LOG,strlog);

	CRect rect;
	int i, idx;
	int z = 0;

	if(flag)
	{
		m_dRecipeAlignPara[z++] = m_edit_align_y_axi_pos;
		m_dRecipeAlignPara[z++] = m_edit_align_t_axi_pos;
		m_dRecipeAlignPara[z++] = m_edit_align_offset_x;
		m_dRecipeAlignPara[z++] = m_edit_align_offset_y;
		m_dRecipeAlignPara[z++] = m_edit_align_mark_size0;
		m_dRecipeAlignPara[z++] = m_edit_align_mark_size1;
		m_dRecipeAlignPara[z++] = m_edit_align_mark_res0;
		m_dRecipeAlignPara[z++] = m_edit_align_mark_res1;
		m_dRecipeAlignPara[z++] = m_edit_align_posx0;
		m_dRecipeAlignPara[z++] = m_edit_align_posy0;
		m_dRecipeAlignPara[z++] = m_edit_align_posx1;
		m_dRecipeAlignPara[z++] = m_edit_align_posy1;
		m_dRecipeAlignPara[z++] = m_edit_align_shaftx;
		m_dRecipeAlignPara[z++] = m_edit_align_shafty;
		m_dRecipeAlignPara[z++] = m_edit_align_light0;
		m_dRecipeAlignPara[z++] = m_edit_align_light1;
		m_dRecipeAlignPara[z++] = m_edit_offset_x_rat;
		m_dRecipeAlignPara[z++] = m_edit_offset_y_rat;
		m_dRecipeAlignPara[z++] = m_edit_align_pixel_size;
		m_dRecipeAlignPara[z++] = m_edit_align_offset_t;
		m_dRecipeAlignPara[z++] = m_edit_align_jog_pitch;
		m_dRecipeAlignPara[z++] = m_edit_align_shaft_offset_x;
		m_dRecipeAlignPara[z++] = m_edit_align_shaft_offset_y;
		m_dRecipeAlignPara[z++] = m_edit_align_x_axi_pos;
		//Alignment//////////////////////////////
		//Align region
		for(i = 0; i < ALIGN_IMG_RECT_MAX_NUM; i++)
		{
			idx = 0;
			m_dlgImgRect[i]->GetTracker(idx, &rect.left, &rect.top, &rect.right, &rect.bottom); 
			m_AlignData.m_InsRect[i] = rect;

			idx = 1;
			m_dlgImgRect[i]->GetTracker(idx, &rect.left, &rect.top, &rect.right, &rect.bottom); 
			m_AlignData.m_PatRect[i] = rect;
		}
	}
	else
	{
		//Align region
		for(i = 0; i < ALIGN_IMG_RECT_MAX_NUM; i++)
		{
			m_dlgImgRect[i]->DeleteAll();
			m_dlgImgRect[i]->SetActiveTracker(TRUE);

			//inspect region
			rect = m_AlignData.m_InsRect[i];
			m_dlgImgRect[i]->AddTracker(0, rect.left, rect.top, rect.right, rect.bottom, RGB(128, 128, 128), "ALIGN-SEARCH REGION");
			m_dlgImgRect[i]->Display();

			//mark region
			rect = m_AlignData.m_PatRect[i];
			m_dlgImgRect[i]->AddTracker(0, rect.left, rect.top, rect.right, rect.bottom, RGB(0, 0, 255), "ALIGN-SEARCH REGION");
			m_dlgImgRect[i]->Display();
		}
	}

	strlog.Format(_T("Update(flag : %d ) End"),flag);
	pdlg->WriteLog(ALIGN_LOG,strlog);
}

void CDialogAlign::CallBackImageAcquired(CAM_HANDLE hCam, CAM_STRM_HANDLE hStrm, CAM_IMAGE_INFO *psImageInfo, uint32_t uiBufferIndex, void *pvContext)
{
	CDialogAlign *pDlg = reinterpret_cast<CDialogAlign *>(pvContext);
	// Get camera index from camera handle.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;

	strlog.Format(_T("CallBackImageAcquired() Start"));
	//pdlg->WriteLog(ALIGN_LOG,strlog);
	uint32_t uiIndexCam;
	CAM_API_STATUS uiSts = GetCamIndexFromCamHandle(hCam, &uiIndexCam);
	if (uiSts != CAM_API_STS_SUCCESS)
	{
		AfxMessageBox(_T("Error : grab fail(CDialogAlign::CallBackImageAcquired sucess fail)!"));	
		strlog.Format(_T("Error : grab fail(CDialogAlign::CallBackImageAcquired sucess fail)!"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
		return;
	}

	//gen_image1_extern(&pDlg->m_HalImage[uiIndexCam], "byte", ALIGN_IMAGE_WIDTH, ALIGN_IMAGE_HEIGHT, (Hlong)psImageInfo->pvBuf, (Hlong) 0);

	if(pDlg->m_bIsDisp[uiIndexCam])
	{
		gen_image1_extern(&pDlg->m_HalImage[uiIndexCam], "byte", ALIGN_IMAGE_WIDTH, ALIGN_IMAGE_HEIGHT, (Hlong)psImageInfo->pvBuf, (Hlong) 0);
		if(pDlg->m_bIsSnap[uiIndexCam])
		{
			pDlg->m_bIsDisp[uiIndexCam] = FALSE;
			pDlg->m_bIsSnap[uiIndexCam] = FALSE;
			strlog.Format(_T("CallBackImageAcquired() Snap"));
			pdlg->WriteLog(ALIGN_LOG,strlog);
		}
		disp_obj(pDlg->m_HalImage[uiIndexCam], pDlg->m_dlgImgRect[uiIndexCam]->GetView());
		pDlg->m_dlgImgRect[uiIndexCam]->Display();
		pDlg->DrawCross(uiIndexCam);
	}

	CAM_API_STATUS status = 0;
	if(pDlg->m_bIsSnap[uiIndexCam])
	{
		
		// send acquisition stop command to camera
		if (pDlg->m_hStrm[uiIndexCam] != NULL)
		{
			Sleep(200);
			//status = Strm_Stop(pDlg->m_hStrm[uiIndexCam]);
			//if (status != CAM_API_STS_SUCCESS)
			//{
			//	// Set Acquisition active flag.
			//	AfxMessageBox(_T("Error : grab fail(CDialogAlign::CallBackImageAcquired stop fail)!"));	
			//	return;
			//}
		}
	}
}

void CDialogAlign::ClickBtnenhctrlAlignStop()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CAM_API_STATUS status = 0;

	// send acquisition stop command to camera
	for (int i = 0; i < m_iCamNum; ++i)
	{
		Stop(i);
	}
}

void CDialogAlign::DrawCross(int cam_idx)
{
	set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
	set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
	disp_line(m_dlgImgRect[cam_idx]->GetView(), ALIGN_IMAGE_HEIGHT * 0.5, 0, ALIGN_IMAGE_HEIGHT * 0.5, ALIGN_IMAGE_WIDTH);
	disp_line(m_dlgImgRect[cam_idx]->GetView(), 0, ALIGN_IMAGE_WIDTH * 0.5, ALIGN_IMAGE_HEIGHT, ALIGN_IMAGE_WIDTH * 0.5);

	set_tposition(m_dlgImgRect[cam_idx]->GetView(),ALIGN_IMAGE_HEIGHT * 0.5 + 10, 10);
	write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("-X"));
	set_tposition(m_dlgImgRect[cam_idx]->GetView(),ALIGN_IMAGE_HEIGHT * 0.5 + 10, ALIGN_IMAGE_WIDTH - 30);
	write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("+X"));
	set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, ALIGN_IMAGE_WIDTH * 0.5 - 30);
	write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("+Y"));
	set_tposition(m_dlgImgRect[cam_idx]->GetView(), ALIGN_IMAGE_HEIGHT - 30, ALIGN_IMAGE_WIDTH * 0.5 - 30);
	write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("-Y"));
}

LRESULT CDialogAlign::OnImageRectMessage(WPARAM para0, LPARAM para1)
{
	
	// TODO: Add your control notification handler code here
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	HTuple wd, ht;
	Herror herr;
	CString status;
	int cam_idx;
	cam_idx = para0;

	switch((int)para1)
	{
	//Display
	case 0:
		disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
		m_dlgImgRect[cam_idx]->Display();

		//십자마크
		DrawCross(cam_idx);
		break;
	//Live on
	case 1:
		Live(cam_idx, TRUE);
		break;
	//Live off
	case 2:
		Stop(cam_idx);
		break;
	//Image load
	case 3:
		Hal_OpenHalImage(&m_HalImage[cam_idx], CString(""), FALSE);

		set_check("~give_error");
	
		herr = get_image_pointer1(m_HalImage[cam_idx], NULL, NULL, &wd, &ht);
		set_check("give_error");
		
		if(H_MSG_TRUE == herr)
		{
		}
		else
		{
			AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1!"));
			strlog.Format(_T("Halcon Error Occured at get_image_pointer1!"));
			pdlg->WriteLog(ALIGN_LOG,strlog);
			return 0;
		}

		disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
		m_dlgImgRect[cam_idx]->Display();
		break;
		//Image save
	case 4:
		Hal_SaveHalImage(&m_HalImage[cam_idx], CString(""), CString("bmp"));
		//임시
		write_image(m_HalImage[cam_idx],HTuple("bmp"),NULL,HTuple("D:\\aaa.bmp"));
		break;
	//Gray value
	case 5:
		/*
		switch(para0)
		{
		case 0:
			m_dlgImgRect[para0]->GetGrayPos(&x, &y);
			//Get gray 
			get_grayval(*m_VisMod->GetHalImage((int)para0, m_iBufIdx[0], m_iMapIdx[0]), y, x, &gray);
			m_dlgImgRect[para0]->SetGray((Hlong)gray[0]);

			set_color(m_dlgImgRect[para0]->GetView(), HTuple("blue"));
			disp_obj(*m_VisMod->GetHalImage((int)para0, m_iBufIdx[0], m_iMapIdx[0]), m_dlgImgRect[para0]->GetView());
			 //20110418 lee
			if(m_lProfileMode == 1)
			{
				for(loop=0;loop<m_lWidth[para0]-1;loop++)
				{
					get_grayval(*m_VisMod->GetHalImage((int)para0, m_iBufIdx[0], m_iMapIdx[0]), y, loop, &gray);
					get_grayval(*m_VisMod->GetHalImage((int)para0, m_iBufIdx[0], m_iMapIdx[0]), y, loop+1, &gray2);
					disp_line(m_dlgImgRect[(int)para0]->GetView(), y - (Hlong)gray[0], loop, y - (Hlong)gray2[0], loop+1);				
				}
			}
 	        else if(m_lProfileMode == 2)
			{
				set_color(m_dlgImgRect[para0]->GetView(), HTuple("yellow")); 
				for(loop=0;loop<m_lHeight[para0]-1;loop++)
				{
					get_grayval(*m_VisMod->GetHalImage((int)para0, m_iBufIdx[0], m_iMapIdx[0]), loop, x, &gray);
					get_grayval(*m_VisMod->GetHalImage((int)para0, m_iBufIdx[0], m_iMapIdx[0]), loop+1, x, &gray2);
					disp_line(m_dlgImgRect[(int)para0]->GetView(), loop, x + (Hlong)gray[0], loop+1, x + (Hlong)gray2[0]);				
				}
			}
			else if(m_lProfileMode == 3)
			{
				for(loop=0;loop<m_lWidth[para0]-1;loop++)
				{
					get_grayval(*m_VisMod->GetHalImage((int)para0, m_iBufIdx[0], m_iMapIdx[0]), y, loop, &gray);
					get_grayval(*m_VisMod->GetHalImage((int)para0, m_iBufIdx[0], m_iMapIdx[0]), y, loop+1, &gray2);
					disp_line(m_dlgImgRect[(int)para0]->GetView(), y - (Hlong)gray[0], loop, y - (Hlong)gray2[0], loop+1);				
				}
				set_color(m_dlgImgRect[para0]->GetView(), HTuple("yellow")); 
				for(loop=0;loop<m_lHeight[para0]-1;loop++)
				{
					get_grayval(*m_VisMod->GetHalImage((int)para0, m_iBufIdx[0], m_iMapIdx[0]), loop, x, &gray);
					get_grayval(*m_VisMod->GetHalImage((int)para0, m_iBufIdx[0], m_iMapIdx[0]), loop+1, x, &gray2);
					disp_line(m_dlgImgRect[(int)para0]->GetView(), loop,x + (Hlong)gray[0], loop+1, x + (Hlong)gray2[0]);				
				}
			}
			break;
		}
		*/
		break;
		//Region confirm
		case 6:
			
			break;
		//Test
		case 7:
	
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogAlign::OnImageRectMessage)!"));
		strlog.Format(_T("Error : invalid index(CDialogAlign::OnImageRectMessage)!"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
		break;
	}

	return 0;
}


void CDialogAlign::ClickBtnenhctrlAlignFindpos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_bAlignInspection)
	{
		return;
	}
	m_bAlignInspection = TRUE;
	AlignInspectFindPos();
	m_bAlignInspection = FALSE;
}

//20160720 ngh
BOOL CDialogAlign::CamReconnect()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	uint32_t status = 0;
	BOOL rslt = TRUE;

#ifdef CAMERA
	CloseStream();
	CloseAllCameras();
	Sys_Terminate();

	Sleep(1000);

	if(Open())
	{
		OpenStream();
	}

	for(int cam_idx = 0; cam_idx < 2; cam_idx++)
	{
		status = Strm_Start(m_hStrm[cam_idx]);
		if (status != CAM_API_STS_SUCCESS)
		{
			AfxMessageBox(_T("Error : grab fail(CDialogAlign::Grab(CDialogAlign::Grab)!"));
			strlog.Format(_T("Error : grab fail(CDialogAlign::Grab(CDialogAlign::Grab)!"));
			pdlg->WriteLog(ALIGN_LOG,strlog);
			rslt = FALSE;
			break;
		}
	}
#endif
	return rslt;
}

//20160711 ngh
int CDialogAlign::AlignInspectFindPos()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	CString strlog;
	

	int rslt = 0;

	char result0[DAT_STR_LENG], result1[DAT_STR_LENG];

	int i, cam_idx;
	cam_idx = 0;
	double dx, dy;
	double x, y, t;

	strlog.Format(_T("AlignInspectFindPos() Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	memset(&m_AlignRslt, NULL, sizeof(typeAlignResult));

	m_nFindAlignCnt = 0;
	m_nFindWhileCnt = 0;

	ClickBtnenhctrlAlignMarkFind0();
	ClickBtnenhctrlAlignMarkFind1();

	while(TRUE)
	{
		if(m_nFindAlignCnt < 2)
		{
			if(CamReconnect())
			{
				strlog.Format(_T("AlignInspectFindPos() Camera Reconnect - %d"), m_nFindWhileCnt);
				pdlg->WriteLog(ALIGN_LOG,strlog);
			
#ifdef CAMERA
				SetGrab();
#endif

				ClickBtnenhctrlAlignMarkFind0();
				ClickBtnenhctrlAlignMarkFind1();

				strlog.Format(_T("AlignInspectFindPos() Align Refind - %d"), m_nFindWhileCnt);
				pdlg->WriteLog(ALIGN_LOG,strlog);
			}

			m_nFindWhileCnt++;
		}

		if(m_nFindAlignCnt == 2)
		{
			break;
		}

		if(m_nFindWhileCnt == 3)
		{
			rslt = 1;
			strlog.Format(_T("AlignInspectFindPos() Find Error"));
			pdlg->WriteLog(ALIGN_LOG,strlog);
			return rslt;
		}
	}

	for(i = 0; i < ALIGN_CAM_MAX_NUM; i++)
	{
		if(1 != m_AlignRslt.m_dFindNum[i])
		{
			//Align fail
			rslt = 1;
			strlog.Format(_T("AlignInspectFindPos() Find Error"));
			pdlg->WriteLog(ALIGN_LOG,strlog);
			return rslt;
		}
	}

	//Cam0////////////////////////////////////////////////////////////////////////
	//티칭점과의 차이거리
	cam_idx = 0;
	m_AlignRslt.m_dTeachPosX_mm[cam_idx] = m_edit_align_posx0 * m_edit_align_pixel_size;
	m_AlignRslt.m_dTeachPosY_mm[cam_idx] = m_edit_align_posy0 * m_edit_align_pixel_size;

	m_AlignRslt.m_dFindPosX_mm[cam_idx] =  m_AlignRslt.m_dFindPosX_pixel[cam_idx] * m_edit_align_pixel_size;
	m_AlignRslt.m_dFindPosY_mm[cam_idx] =  m_AlignRslt.m_dFindPosY_pixel[cam_idx] * m_edit_align_pixel_size;

	DistXY(m_AlignRslt.m_dFindPosX_pixel[cam_idx], m_AlignRslt.m_dFindPosY_pixel[cam_idx], 
		   m_edit_align_posx0, m_edit_align_posy0, 
		   &m_AlignRslt.m_dFindDiffX_pixel[cam_idx],&m_AlignRslt.m_dFindDiffY_pixel[cam_idx]);

	m_AlignRslt.m_dFindDiffX_mm[cam_idx] = m_AlignRslt.m_dFindDiffX_pixel[cam_idx] * m_edit_align_pixel_size;
	m_AlignRslt.m_dFindDiffY_mm[cam_idx] = m_AlignRslt.m_dFindDiffY_pixel[cam_idx] * m_edit_align_pixel_size;

	//카메라 중심과의 차이거리
	DistXY(m_AlignRslt.m_dFindPosX_pixel[cam_idx], m_AlignRslt.m_dFindPosY_pixel[cam_idx], 
		   (ALIGN_IMAGE_WIDTH * 0.5), (-(ALIGN_IMAGE_HEIGHT * 0.5)), 
		   &m_AlignRslt.m_dFindDiffCentX_pixel[cam_idx],&m_AlignRslt.m_dFindDiffCentY_pixel[cam_idx]);

	m_AlignRslt.m_dFindDiffCentX_mm[cam_idx] = m_AlignRslt.m_dFindDiffCentX_pixel[cam_idx] * m_edit_align_pixel_size;
	m_AlignRslt.m_dFindDiffCentY_mm[cam_idx] = m_AlignRslt.m_dFindDiffCentY_pixel[cam_idx] * m_edit_align_pixel_size;

	//Display
	dx = m_AlignRslt.m_dFindDiffCentX_mm[cam_idx]; 
	if(0.001 > fabs(dx))
	{
		dx = 0.;
	}
	dy = m_AlignRslt.m_dFindDiffCentY_mm[cam_idx];
	if(0.001 > fabs(dy))
	{
		dy = 0.;
	}

	sprintf(result0, "%0.3f", dx);
	sprintf(result1, "%0.3f", dy);
	set_color(m_dlgImgRect[cam_idx]->GetView(), "blue");
	set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
	set_tposition(m_dlgImgRect[cam_idx]->GetView(),ALIGN_IMAGE_HEIGHT - 100, 10);
	write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("Distance X from center pos. : ") + HTuple(result0) + HTuple("mm"));
	set_tposition(m_dlgImgRect[cam_idx]->GetView(),ALIGN_IMAGE_HEIGHT - 80, 10);
	write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("Distance Y from center pos.  : ") + HTuple(result1) + HTuple("mm"));

	dx = m_AlignRslt.m_dFindDiffX_mm[cam_idx]; 
	if(0.001 > fabs(dx))
	{
		dx = 0.;
	}
	dy = m_AlignRslt.m_dFindDiffY_mm[cam_idx];
	if(0.001 > fabs(dy))
	{
		dy = 0.;
	}

	sprintf(result0, "%0.3f", dx);
	sprintf(result1, "%0.3f", dy);
	set_tposition(m_dlgImgRect[cam_idx]->GetView(),ALIGN_IMAGE_HEIGHT - 60, 10);
	write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("Distance X from teaching pos. : ") + HTuple(result0) + HTuple("mm"));
	set_tposition(m_dlgImgRect[cam_idx]->GetView(),ALIGN_IMAGE_HEIGHT - 40, 10);
	write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("Distance Y from teaching pos. : ") + HTuple(result1) + HTuple("mm"));

	DrawCross(cam_idx);

	//Cam1////////////////////////////////////////////////////////////////////////
	cam_idx = 1;
	m_AlignRslt.m_dTeachPosX_mm[cam_idx] = m_edit_align_posx1 * m_edit_align_pixel_size;
	m_AlignRslt.m_dTeachPosY_mm[cam_idx] = m_edit_align_posy1 * m_edit_align_pixel_size;

	m_AlignRslt.m_dFindPosX_mm[cam_idx] =  m_AlignRslt.m_dFindPosX_pixel[cam_idx] * m_edit_align_pixel_size;
	m_AlignRslt.m_dFindPosY_mm[cam_idx] =  m_AlignRslt.m_dFindPosY_pixel[cam_idx] * m_edit_align_pixel_size;

	DistXY(m_AlignRslt.m_dFindPosX_pixel[cam_idx], m_AlignRslt.m_dFindPosY_pixel[cam_idx], 
		   m_edit_align_posx1, m_edit_align_posy1, 
		   &m_AlignRslt.m_dFindDiffX_pixel[cam_idx],&m_AlignRslt.m_dFindDiffY_pixel[cam_idx]);

	m_AlignRslt.m_dFindDiffX_mm[cam_idx] = m_AlignRslt.m_dFindDiffX_pixel[cam_idx] * m_edit_align_pixel_size;
	m_AlignRslt.m_dFindDiffY_mm[cam_idx] = m_AlignRslt.m_dFindDiffY_pixel[cam_idx] * m_edit_align_pixel_size;

	//카메라 중심과의 차이거리
	DistXY(m_AlignRslt.m_dFindPosX_pixel[cam_idx], m_AlignRslt.m_dFindPosY_pixel[cam_idx], 
		   (ALIGN_IMAGE_WIDTH * 0.5), (-(ALIGN_IMAGE_HEIGHT * 0.5)), 
		   &m_AlignRslt.m_dFindDiffCentX_pixel[cam_idx],&m_AlignRslt.m_dFindDiffCentY_pixel[cam_idx]);

	m_AlignRslt.m_dFindDiffCentX_mm[cam_idx] = m_AlignRslt.m_dFindDiffCentX_pixel[cam_idx] * m_edit_align_pixel_size;
	m_AlignRslt.m_dFindDiffCentY_mm[cam_idx] = m_AlignRslt.m_dFindDiffCentY_pixel[cam_idx] * m_edit_align_pixel_size;

	//Display
	dx = m_AlignRslt.m_dFindDiffCentX_mm[cam_idx]; 
	if(0.001 > fabs(dx))
	{
		dx = 0.;
	}
	dy = m_AlignRslt.m_dFindDiffCentY_mm[cam_idx];
	if(0.001 > fabs(dy))
	{
		dy = 0.;
	}

	sprintf(result0, "%0.3f", dx);
	sprintf(result1, "%0.3f", dy);
	set_color(m_dlgImgRect[cam_idx]->GetView(), "blue");
	set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
	set_tposition(m_dlgImgRect[cam_idx]->GetView(),ALIGN_IMAGE_HEIGHT - 100, 10);
	write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("Distance X from center pos. : ") + HTuple(result0) + HTuple("mm"));
	set_tposition(m_dlgImgRect[cam_idx]->GetView(),ALIGN_IMAGE_HEIGHT - 80, 10);
	write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("Distance Y from center pos.  : ") + HTuple(result1) + HTuple("mm"));

	dx = m_AlignRslt.m_dFindDiffX_mm[cam_idx]; 
	if(0.001 > fabs(dx))
	{
		dx = 0.;
	}
	dy = m_AlignRslt.m_dFindDiffY_mm[cam_idx];
	if(0.001 > fabs(dy))
	{
		dy = 0.;
	}

	sprintf(result0, "%0.3f", dx);
	sprintf(result1, "%0.3f", dy);
	set_tposition(m_dlgImgRect[cam_idx]->GetView(),ALIGN_IMAGE_HEIGHT - 60, 10);
	write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("Distance X from teaching pos. : ") + HTuple(result0) + HTuple("mm"));
	set_tposition(m_dlgImgRect[cam_idx]->GetView(),ALIGN_IMAGE_HEIGHT - 40, 10);
	write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("Distance Y from teaching pos. : ") + HTuple(result1) + HTuple("mm"));

	DrawCross(cam_idx);

	//Coord/////////////////////////////////////////////////////////////////////////////
	double camera_cent_x, camera_cent_y, dist_inter_mark_x, dist_inter_mark_y;

	camera_cent_x = (ALIGN_IMAGE_WIDTH * 0.5) * m_edit_align_pixel_size;
	camera_cent_y = (-(ALIGN_IMAGE_HEIGHT * 0.5)) * m_edit_align_pixel_size;
	dist_inter_mark_x = (pdlg->m_RecipePanel->GetPanelSizeX()) - (pdlg->m_RecipePanel->GetMarkPosX(0)) + (pdlg->m_RecipePanel->GetMarkPosX(1));
	dist_inter_mark_y = (pdlg->m_RecipePanel->GetMarkPosY(1)) - (pdlg->m_RecipePanel->GetMarkPosY(0));
	if(MakeRsltCoord(m_AlignRslt.m_dTeachPosX_mm[0], m_AlignRslt.m_dTeachPosY_mm[0], m_AlignRslt.m_dTeachPosX_mm[1], m_AlignRslt.m_dTeachPosY_mm[1], 
					 m_AlignRslt.m_dFindPosX_mm[0], m_AlignRslt.m_dFindPosY_mm[0], m_AlignRslt.m_dFindPosX_mm[1], m_AlignRslt.m_dFindPosY_mm[1],
					 m_edit_align_shaftx + m_edit_align_shaft_offset_x, m_edit_align_shafty + m_edit_align_shaft_offset_y, 
					 camera_cent_x, camera_cent_y,
					 dist_inter_mark_x, dist_inter_mark_y
					 ))
	{

		//Mark align//////////////////////////////////////////////
		if( (0 == m_AlignRslt.m_dRobotPosX_afteralign) || 
			(0 == m_AlignRslt.m_dRobotPosY_afteralign) ||
			(0 == m_AlignRslt.m_dRobotPosT_afteralign) )
		{
			//x = pdlg->m_Robot->GetRobotPos_mm(MOTOR_1);;
			y = pdlg->m_Robot->GetRobotPos_mm(MOTOR_0);
			t = pdlg->m_Robot->GetRobotPos_mm(MOTOR_2);

			m_AlignRslt.m_dRobotPosX_afteralign = m_edit_align_x_axi_pos - m_AlignRslt.m_dCoordX_diff;//X는 티칭한 위치 + difference
			m_AlignRslt.m_dRobotPosY_afteralign = y + m_AlignRslt.m_dCoordY_diff;
			m_AlignRslt.m_dRobotPosT_afteralign = t + m_AlignRslt.m_dCoordAng_diff;

			//Reveiw align

			x = m_AlignRslt.m_dRobotPosX_afteralign;
			y = m_AlignRslt.m_dRobotPosY_afteralign;
			t = m_AlignRslt.m_dRobotPosT_afteralign;

			x = x + m_edit_align_offset_x;
			y = y + m_edit_align_offset_y;
			t = t + m_edit_align_offset_t;

			m_AlignRslt.m_dRobotPosX_afteroffset = x;
			m_AlignRslt.m_dRobotPosY_afteroffset = y;
			m_AlignRslt.m_dRobotPosT_afteroffset = t;

		}
		
	}
	else
	{

	}
	strlog.Format(_T("AlignInspectFindPos() End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);

	return rslt;

}

void CDialogAlign::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString temp;
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;

	switch(nIDEvent)
	{
	case 0:
		if(IsWindowVisible())
		{
			temp.Format(_T("%f"), pdlg->m_Robot->GetRobotPos_mm(MOTOR_1));
			GetDlgItem(IDC_STATIC_ALIGN_POS_X)->SetWindowTextW(temp);

			temp.Format(_T("%f"), pdlg->m_Robot->GetRobotPos_mm(MOTOR_0));
			GetDlgItem(IDC_STATIC_ALIGN_POS_Y)->SetWindowTextW(temp);

			temp.Format(_T("%f"), pdlg->m_Robot->GetRobotPos_mm(MOTOR_2));
			GetDlgItem(IDC_STATIC_ALIGN_POS_T)->SetWindowTextW(temp);
		}
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogAlign::OnTimer)!"));
		strlog.Format(_T("Error : invalid index(CDialogAlign::OnTimer)!"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CDialogAlign::ClickBtnenhctrlAlignMarkTeach0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
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
	if(IDCANCEL == AfxMessageBox(_T("정말 변경 하시겠습까?"), MB_OKCANCEL | MB_ICONQUESTION, 0))
	{
		return;
	}
	strlog.Format(_T("ClickBtnenhctrlAlignMarkTeach0() Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
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
		strlog.Format(_T("Halcon Error Occured at get_image_pointer1!"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
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
	
	close_measure(MeasureHandle);

	edgeCnt = RowEdgeFirst.Num();
	if(0 < edgeCnt)
	{
		set_color(m_dlgImgRect[cam_idx]->GetView(), HTuple("blue"));
		set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
		disp_line(m_dlgImgRect[cam_idx]->GetView(), RowEdgeFirst[0], ColumnEdgeFirst[0], RowEdgeSecond[0], ColumnEdgeSecond[0]);
		pat_rect.left = (int)((double)ColumnEdgeFirst[0] - 0.5);
		pat_rect.right = (int)((double)ColumnEdgeSecond[0] + 0.5);
		mark_width = IntraDistance[0];
	}
	else
	{
		m_AlignData.m_bCreatPatIDAlign[cam_idx] = FALSE;
		
		set_color(m_dlgImgRect[cam_idx]->GetView(), HTuple("red"));
		set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
		set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
		write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("Teach(rect is not center) fail!"));
		strlog.Format(_T("Teach(rect is not center) fail!"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
		return;
	}

	//Height
	leng = (m_AlignData.m_PatRect[cam_idx].bottom - m_AlignData.m_PatRect[cam_idx].top) * 0.5;
	gen_measure_rectangle2(row,col,HTuple(-1.57079632679),leng,ALIGN_MEASURE_LENG,wd,ht,HTuple("nearest_neighbor"),&MeasureHandle);

	measure_pairs(m_HalImage[cam_idx], MeasureHandle, HTuple(1), HTuple(ALIGN_THRESH), HTuple("positive_strongest"), HTuple("all"),
			            &RowEdgeFirst,&ColumnEdgeFirst,&AmplitudeFirst,  
						&RowEdgeSecond,&ColumnEdgeSecond,&AmplitudeSecond,
						&IntraDistance, &InterDistance);

	close_measure(MeasureHandle);
	
	edgeCnt = RowEdgeFirst.Num();
	if(0 < edgeCnt)
	{
		set_color(m_dlgImgRect[cam_idx]->GetView(), HTuple("blue"));
		set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
		disp_line(m_dlgImgRect[cam_idx]->GetView(), RowEdgeFirst[0], ColumnEdgeFirst[0], RowEdgeSecond[0], ColumnEdgeSecond[0]);
		pat_rect.top = (int)((double)RowEdgeFirst[0] - 0.5);
		pat_rect.bottom = (int)((double)RowEdgeSecond[0] + 0.5);
		mark_height = IntraDistance[0];
	}
	else
	{
		m_AlignData.m_bCreatPatIDAlign[cam_idx] = FALSE;

		set_color(m_dlgImgRect[cam_idx]->GetView(), HTuple("red"));
		set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
		set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
		write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("Teach(height) fail!"));
		strlog.Format(_T("Teach(height) fail!"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
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
	
	strlog.Format(_T("ClickBtnenhctrlAlignMarkTeach0() End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
}


void CDialogAlign::ClickBtnenhctrlAlignMarkTeach1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
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
	if(IDCANCEL == AfxMessageBox(_T("정말 변경 하시겠습까?"), MB_OKCANCEL | MB_ICONQUESTION, 0))
	{
		return;
	}
	strlog.Format(_T("ClickBtnenhctrlAlignMarkTeach1() Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	UpdateData(TRUE);
	Update(TRUE);
	cam_idx = 1;

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
		strlog.Format(_T("Halcon Error Occured at get_image_pointer1!"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
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

	close_measure(MeasureHandle);
	
	edgeCnt = RowEdgeFirst.Num();
	if(0 < edgeCnt)
	{
		set_color(m_dlgImgRect[cam_idx]->GetView(), HTuple("blue"));
		set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
		disp_line(m_dlgImgRect[cam_idx]->GetView(), RowEdgeFirst[0], ColumnEdgeFirst[0], RowEdgeSecond[0], ColumnEdgeSecond[0]);
		pat_rect.left = (int)((double)ColumnEdgeFirst[0] - 0.5);
		pat_rect.right = (int)((double)ColumnEdgeSecond[0] + 0.5);
		mark_width = IntraDistance[0];
	}
	else
	{
		m_AlignData.m_bCreatPatIDAlign[cam_idx] = FALSE;
		
		set_color(m_dlgImgRect[cam_idx]->GetView(), HTuple("red"));
		set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
		set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
		write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("Teach(rect is not center) fail!"));
		strlog.Format(_T("Teach(rect is not center) fail!"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
		return;
	}

	//Height
	leng = (m_AlignData.m_PatRect[cam_idx].bottom - m_AlignData.m_PatRect[cam_idx].top) * 0.5;
	gen_measure_rectangle2(row,col,HTuple(-1.57079632679),leng,ALIGN_MEASURE_LENG,wd,ht,HTuple("nearest_neighbor"),&MeasureHandle);

	measure_pairs(m_HalImage[cam_idx], MeasureHandle, HTuple(1), HTuple(ALIGN_THRESH), HTuple("positive_strongest"), HTuple("all"),
			            &RowEdgeFirst,&ColumnEdgeFirst,&AmplitudeFirst,  
						&RowEdgeSecond,&ColumnEdgeSecond,&AmplitudeSecond,
						&IntraDistance, &InterDistance);

	close_measure(MeasureHandle);
	
	edgeCnt = RowEdgeFirst.Num();
	if(0 < edgeCnt)
	{
		set_color(m_dlgImgRect[cam_idx]->GetView(), HTuple("blue"));
		set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
		disp_line(m_dlgImgRect[cam_idx]->GetView(), RowEdgeFirst[0], ColumnEdgeFirst[0], RowEdgeSecond[0], ColumnEdgeSecond[0]);
		pat_rect.top = (int)((double)RowEdgeFirst[0] - 0.5);
		pat_rect.bottom = (int)((double)RowEdgeSecond[0] + 0.5);
		mark_height = IntraDistance[0];
	}
	else
	{
		m_AlignData.m_bCreatPatIDAlign[cam_idx] = FALSE;

		set_color(m_dlgImgRect[cam_idx]->GetView(), HTuple("red"));
		set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
		set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
		write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("Teach(height) fail!"));
		strlog.Format(_T("Teach(height) fail!"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
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
			m_edit_align_mark_res1 = ((m_edit_align_mark_size1 / mark_width) + (m_edit_align_mark_size1 / mark_height)) * 0.5;
			m_edit_align_pixel_size = (m_edit_align_mark_res0 + m_edit_align_mark_res1) * 0.5;
		}
		else
		{
			m_edit_align_mark_res1 = 1;
			m_edit_align_pixel_size = 1;
		}
	}
	else
	{
		m_AlignData.m_bCreatPatIDAlign[cam_idx] = FALSE;
	}

	UpdateData(FALSE);
	strlog.Format(_T("ClickBtnenhctrlAlignMarkTeach1() End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);

}

void CDialogAlign::OnNMCustomdrawSliderAlignLight0(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	m_edit_align_light0 = m_slider_align_light0.GetPos();
	pdlg->m_Light->SetAlignLight(0, (int)m_edit_align_light0);
	
	UpdateData(FALSE);

	if(m_edit_align_light0 > 0)
	{
		pdlg->m_Light->SetLightTime(0, TRUE);
	}
	else
	{
		pdlg->m_Light->SetLightTime(0, FALSE);
	}

	*pResult = 0;
}


void CDialogAlign::OnNMCustomdrawSliderAlignLight1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	m_edit_align_light1 = m_slider_align_light1.GetPos();
	pdlg->m_Light->SetAlignLight(1, (int)m_edit_align_light1);

	UpdateData(FALSE);

	if(m_edit_align_light1 > 0)
	{
		pdlg->m_Light->SetLightTime(1, TRUE);
	}
	else
	{
		pdlg->m_Light->SetLightTime(1, FALSE);
	}

	*pResult = 0;
}


void CDialogAlign::MouseDownBtnenhctrlAlignScanJogM(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_0, DIR_NEGATIVE, m_iJogSpdSelect);
}


void CDialogAlign::MouseUpBtnenhctrlAlignScanJogM(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_0);
}


void CDialogAlign::MouseDownBtnenhctrlAlignScanJogP(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_0, DIR_POSITIVE, m_iJogSpdSelect);
}


void CDialogAlign::MouseUpBtnenhctrlAlignScanJogP(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_0);
}


void CDialogAlign::MouseDownBtnenhctrlAlignJogCcw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_2, DIR_POSITIVE, m_iJogSpdSelect);
}


void CDialogAlign::MouseUpBtnenhctrlAlignJogCcw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_2);
}


void CDialogAlign::MouseDownBtnenhctrlAlignJogCw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_2, DIR_NEGATIVE, m_iJogSpdSelect);
}


void CDialogAlign::MouseUpBtnenhctrlAlignJogCw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_2);
}

void CDialogAlign::ClickBtnenhctrlAlignJogSpd1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_iJogSpdSelect = 0;

	m_btn_jog_spd0.SetValue(TRUE);
	m_btn_jog_spd1.SetValue(FALSE);
	m_btn_jog_spd2.SetValue(FALSE);
}


void CDialogAlign::ClickBtnenhctrlAlignJogSpd2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_iJogSpdSelect = 1;

	m_btn_jog_spd0.SetValue(FALSE);
	m_btn_jog_spd1.SetValue(TRUE);
	m_btn_jog_spd2.SetValue(FALSE);
}


void CDialogAlign::ClickBtnenhctrlAlignJogSpd3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_iJogSpdSelect = 2;

	m_btn_jog_spd0.SetValue(FALSE);
	m_btn_jog_spd1.SetValue(FALSE);
	m_btn_jog_spd2.SetValue(TRUE);
}

BOOL CDialogAlign::SetLoadAlignPara(CString path)
{
	return LoadAlignPara(path);
}

BOOL CDialogAlign::LoadAlignPara(CString path)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	strlog.Format(_T("LoadAlignPara() Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
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


	file_path = path + _T("\\AlignCtrl.par");

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
		
		//ins rect0//////////////////////////////////////////////////////////////
		idx = 0;
		key.Empty();
		key.Format(_T("m_AlignData.m_InsRect_left%d"), idx);
		m_AlignData.m_InsRect[idx].left = GetPrivateProfileInt(_T("ALIGN CTRL"), key, 0, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_InsRect_top%d"), idx);
		m_AlignData.m_InsRect[idx].top = GetPrivateProfileInt(_T("ALIGN CTRL"), key, 0, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_InsRect_right%d"), idx);
		m_AlignData.m_InsRect[idx].right = GetPrivateProfileInt(_T("ALIGN CTRL"), key, 0, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_InsRect_bottom%d"), idx);
		m_AlignData.m_InsRect[idx].bottom = GetPrivateProfileInt(_T("ALIGN CTRL"), key, 0, file_path);

		if( (0 >= m_AlignData.m_InsRect[idx].left) ||
			(ALIGN_IMAGE_WIDTH < m_AlignData.m_InsRect[idx].left) ||
			(0 >= m_AlignData.m_InsRect[idx].top) ||
			(ALIGN_IMAGE_HEIGHT < m_AlignData.m_InsRect[idx].top) ||
			(0 >= m_AlignData.m_InsRect[idx].right) ||
			(ALIGN_IMAGE_WIDTH < m_AlignData.m_InsRect[idx].right) ||
			(0 >= m_AlignData.m_InsRect[idx].bottom) ||
			(ALIGN_IMAGE_HEIGHT < m_AlignData.m_InsRect[idx].bottom)
		  )
		{
			m_AlignData.m_InsRect[idx] = CRect(100, 100, 500, 500);
		}

		//ins rect1//////////////////////////////////////////////////////////////
		idx = 1;
		key.Empty();
		key.Format(_T("m_AlignData.m_InsRect_left%d"), idx);
		m_AlignData.m_InsRect[idx].left = GetPrivateProfileInt(_T("ALIGN CTRL"), key, 0, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_InsRect_top%d"), idx);
		m_AlignData.m_InsRect[idx].top = GetPrivateProfileInt(_T("ALIGN CTRL"), key, 0, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_InsRect_right%d"), idx);
		m_AlignData.m_InsRect[idx].right = GetPrivateProfileInt(_T("ALIGN CTRL"), key, 0, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_InsRect_bottom%d"), idx);
		m_AlignData.m_InsRect[idx].bottom = GetPrivateProfileInt(_T("ALIGN CTRL"), key, 0, file_path);

		if( (0 >= m_AlignData.m_InsRect[idx].left) ||
			(ALIGN_IMAGE_WIDTH < m_AlignData.m_InsRect[idx].left) ||
			(0 >= m_AlignData.m_InsRect[idx].top) ||
			(ALIGN_IMAGE_HEIGHT < m_AlignData.m_InsRect[idx].top) ||
			(0 >= m_AlignData.m_InsRect[idx].right) ||
			(ALIGN_IMAGE_WIDTH < m_AlignData.m_InsRect[idx].right) ||
			(0 >= m_AlignData.m_InsRect[idx].bottom) ||
			(ALIGN_IMAGE_HEIGHT < m_AlignData.m_InsRect[idx].bottom)
		  )
		{
			m_AlignData.m_InsRect[idx] = CRect(100, 100, 500, 500);
		}

		//pat rect0//////////////////////////////////////////////////////////////
		idx = 0;
		key.Empty();
		key.Format(_T("m_AlignData.m_PatRect_left%d"), idx);
		m_AlignData.m_PatRect[idx].left = GetPrivateProfileInt(_T("ALIGN CTRL"), key, 0, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_PatRect_top%d"), idx);
		m_AlignData.m_PatRect[idx].top = GetPrivateProfileInt(_T("ALIGN CTRL"), key, 0, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_PatRect_right%d"), idx);
		m_AlignData.m_PatRect[idx].right = GetPrivateProfileInt(_T("ALIGN CTRL"), key, 0, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_PatRect_bottom%d"), idx);
		m_AlignData.m_PatRect[idx].bottom = GetPrivateProfileInt(_T("ALIGN CTRL"), key, 0, file_path);

		if( (0 >= m_AlignData.m_PatRect[idx].left) ||
			(ALIGN_IMAGE_WIDTH < m_AlignData.m_PatRect[idx].left) ||
			(0 >= m_AlignData.m_PatRect[idx].top) ||
			(ALIGN_IMAGE_HEIGHT < m_AlignData.m_PatRect[idx].top) ||
			(0 >= m_AlignData.m_PatRect[idx].right) ||
			(ALIGN_IMAGE_WIDTH < m_AlignData.m_PatRect[idx].right) ||
			(0 >= m_AlignData.m_PatRect[idx].bottom) ||
			(ALIGN_IMAGE_HEIGHT < m_AlignData.m_PatRect[idx].bottom)
		  )
		{
			m_AlignData.m_PatRect[idx] = CRect(200, 200, 400, 400);
		}


		//pat rect1//////////////////////////////////////////////////////////////
		idx = 1;
		key.Empty();
		key.Format(_T("m_AlignData.m_PatRect_left%d"), idx);
		m_AlignData.m_PatRect[idx].left = GetPrivateProfileInt(_T("ALIGN CTRL"), key, 0, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_PatRect_top%d"), idx);
		m_AlignData.m_PatRect[idx].top = GetPrivateProfileInt(_T("ALIGN CTRL"), key, 0, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_PatRect_right%d"), idx);
		m_AlignData.m_PatRect[idx].right = GetPrivateProfileInt(_T("ALIGN CTRL"), key, 0, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_PatRect_bottom%d"), idx);
		m_AlignData.m_PatRect[idx].bottom = GetPrivateProfileInt(_T("ALIGN CTRL"), key, 0, file_path);

		if( (0 >= m_AlignData.m_PatRect[idx].left) ||
			(ALIGN_IMAGE_WIDTH < m_AlignData.m_PatRect[idx].left) ||
			(0 >= m_AlignData.m_PatRect[idx].top) ||
			(ALIGN_IMAGE_HEIGHT < m_AlignData.m_PatRect[idx].top) ||
			(0 >= m_AlignData.m_PatRect[idx].right) ||
			(ALIGN_IMAGE_WIDTH < m_AlignData.m_PatRect[idx].right) ||
			(0 >= m_AlignData.m_PatRect[idx].bottom) ||
			(ALIGN_IMAGE_HEIGHT < m_AlignData.m_PatRect[idx].bottom)
		  )
		{
			m_AlignData.m_PatRect[idx] = CRect(200, 200, 400, 400);
		}

		//////////////////////////////////////////////////////////////



		key.Empty();
		key.Format(_T("m_edit_align_light0"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_light0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_light1"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_light1 = atof(ctemp);
		
		key.Empty();
		key.Format(_T("m_edit_align_x_axi_pos"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_x_axi_pos = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_y_axi_pos"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_y_axi_pos = atof(ctemp);
		
		key.Empty();
		key.Format(_T("m_edit_align_t_axi_pos"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_t_axi_pos = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_offset_x"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_offset_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_offset_y"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_offset_y = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_offset_t"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_offset_t = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_mark_size0"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_mark_size0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_mark_size1"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_mark_size1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_mark_res0"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_mark_res0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_mark_res1"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_mark_res1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_posx0"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_posx0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_posy0"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_posy0 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_posx1"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_posx1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_posy1"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_posy1 = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_shaftx"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_shaftx = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_shafty"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_shafty = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_pixel_size"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_pixel_size = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_jog_pitch"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_jog_pitch = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_shaft_offset_x"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_shaft_offset_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_align_shaft_offset_y"));
		GetPrivateProfileString(_T("ALIGN CTRL"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_align_shaft_offset_y = atof(ctemp);

		//Read shape region0/////////////////////////////////////////////////////////
		idx = 0;
		pat_path = path + _T("\\PatternAlign."); 
		extention.Format(_T("m%d"), idx);
		pat_path = pat_path + extention;
		getval = 0;
		wcstombs_s(&getval, path_temp, pat_path.GetLength() + 1, pat_path, _TRUNCATE);
		set_check("~give_error");
		Herr = read_shape_model(HTuple(path_temp), &m_AlignData.m_PatIDAlign[idx]);
		set_check("give_error");
		if(H_MSG_TRUE == Herr)
		{
			m_AlignData.m_bCreatPatIDAlign[idx] = TRUE;
		}
		else
		{
			m_AlignData.m_bCreatPatIDAlign[idx] = FALSE;
		}
		////////////////////////////////////////////////////////////////////////////

		//Read shape region1/////////////////////////////////////////////////////////
		idx = 1;
		pat_path = path + _T("\\PatternAlign."); 
		extention.Format(_T("m%d"), idx);
		pat_path = pat_path + extention;
		getval = 0;
		wcstombs_s(&getval, path_temp, pat_path.GetLength() + 1, pat_path, _TRUNCATE);
		set_check("~give_error");
		Herr = read_shape_model(HTuple(path_temp), &m_AlignData.m_PatIDAlign[idx]);
		set_check("give_error");
		if(H_MSG_TRUE == Herr)
		{
			m_AlignData.m_bCreatPatIDAlign[idx] = TRUE;
		}
		else
		{
			m_AlignData.m_bCreatPatIDAlign[idx] = FALSE;
		}
		////////////////////////////////////////////////////////////////////////////
	}
	fd=0;
	strlog.Format(_T("LoadAlignPara() End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	return rslt;
}

BOOL CDialogAlign::SetSaveAlignPara(CString path)
{
	return SaveAlignPara(path);
}

BOOL CDialogAlign::SaveAlignPara(CString path)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	strlog.Format(_T("SaveAlignPara() Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
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
	
	file_path = path + _T("\\AlignCtrl.par");

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

		//m_AlignData.m_InsRect_left0

		//Ins rect0/////////////////////////////////////////////////////////
		idx = 0;
		key.Empty();
		key.Format(_T("m_AlignData.m_InsRect_left%d"), idx);
		dat.Empty();
		dat.Format(_T("%d"), m_AlignData.m_InsRect[idx].left);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_InsRect_top%d"), idx);
		dat.Empty();
		dat.Format(_T("%d"), m_AlignData.m_InsRect[idx].top);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_InsRect_right%d"), idx);
		dat.Empty();
		dat.Format(_T("%d"), m_AlignData.m_InsRect[idx].right);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_InsRect_bottom%d"), idx);
		dat.Empty();
		dat.Format(_T("%d"), m_AlignData.m_InsRect[idx].bottom);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		//Ins rect1//////////////////////////////////////////////////////////////////////
		idx = 1;
		key.Empty();
		key.Format(_T("m_AlignData.m_InsRect_left1"));
		dat.Empty();
		dat.Format(_T("%d"), m_AlignData.m_InsRect[idx].left);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_InsRect_top1"));
		dat.Empty();
		dat.Format(_T("%d"), m_AlignData.m_InsRect[idx].top);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_InsRect_right1"));
		dat.Empty();
		dat.Format(_T("%d"), m_AlignData.m_InsRect[idx].right);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_InsRect_bottom1"));
		dat.Empty();
		dat.Format(_T("%d"), m_AlignData.m_InsRect[idx].bottom);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		//Pat rect0/////////////////////////////////////////////////////////
		idx = 0;
		key.Empty();
		key.Format(_T("m_AlignData.m_PatRect_left%d"), idx);
		dat.Empty();
		dat.Format(_T("%d"), m_AlignData.m_PatRect[idx].left);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_PatRect_top%d"), idx);
		dat.Empty();
		dat.Format(_T("%d"), m_AlignData.m_PatRect[idx].top);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_PatRect_right%d"), idx);
		dat.Empty();
		dat.Format(_T("%d"), m_AlignData.m_PatRect[idx].right);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_PatRect_bottom%d"), idx);
		dat.Empty();
		dat.Format(_T("%d"), m_AlignData.m_PatRect[idx].bottom);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		//Pat rect1//////////////////////////////////////////////////////////////////////
		idx = 1;
		key.Empty();
		key.Format(_T("m_AlignData.m_PatRect_left1"));
		dat.Empty();
		dat.Format(_T("%d"), m_AlignData.m_PatRect[idx].left);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_PatRect_top1"));
		dat.Empty();
		dat.Format(_T("%d"), m_AlignData.m_PatRect[idx].top);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_PatRect_right1"));
		dat.Empty();
		dat.Format(_T("%d"), m_AlignData.m_PatRect[idx].right);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_AlignData.m_PatRect_bottom1"));
		dat.Empty();
		dat.Format(_T("%d"), m_AlignData.m_PatRect[idx].bottom);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		///////////////////////////////////////////////////////////////////

		
		key.Empty();
		key.Format(_T("m_edit_align_light0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_light0);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_x_axi_pos"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_x_axi_pos);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_y_axi_pos"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_y_axi_pos);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_t_axi_pos"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_t_axi_pos);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_offset_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_offset_x);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_offset_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_offset_y);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_mark_size0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_mark_size0);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_mark_size1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_mark_size1);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_mark_res0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_mark_res0);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_mark_res1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_mark_res1);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_posx0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_posx0);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_posy0"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_posy0);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_posx1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_posx1);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_posy1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_posy1);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_shaftx"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_shaftx);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_shafty"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_shafty);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_light1"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_light1);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_pixel_size"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_pixel_size);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_offset_t"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_offset_t);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_jog_pitch"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_jog_pitch);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_shaft_offset_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_shaft_offset_x);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_align_shaft_offset_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_align_shaft_offset_y);
		WritePrivateProfileString(_T("ALIGN CTRL"), key, dat, file_path);

		//Write shape region0////////////////////////////////////////////////////
		idx = 0;
		if(m_AlignData.m_bCreatPatIDAlign[idx])
		{
			pat_path = path + _T("\\PatternAlign."); 
			extention.Format(_T("m%d"), idx);
			pat_path = pat_path + extention;
			getval = 0;
			wcstombs_s(&getval, path_temp, pat_path.GetLength() + 1, pat_path, _TRUNCATE);
			set_check("~give_error");
			Herr = write_shape_model(m_AlignData.m_PatIDAlign[idx], HTuple(path_temp));
			set_check("give_error");
			if(H_MSG_TRUE == Herr)
			{

			}
			else
			{
				m_AlignData.m_bCreatPatIDAlign[idx] = FALSE;
			}
		}

		//Write shape region1////////////////////////////////////////////////////
		idx = 1;
		if(m_AlignData.m_bCreatPatIDAlign[idx])
		{
			pat_path = path + _T("\\PatternAlign."); 
			extention.Format(_T("m%d"), idx);
			pat_path = pat_path + extention;
			getval = 0;
			wcstombs_s(&getval, path_temp, pat_path.GetLength() + 1, pat_path, _TRUNCATE);
			set_check("~give_error");
			Herr = write_shape_model(m_AlignData.m_PatIDAlign[idx], HTuple(path_temp));
			set_check("give_error");
			if(H_MSG_TRUE == Herr)
			{

			}
			else
			{
				m_AlignData.m_bCreatPatIDAlign[idx] = FALSE;
			}
		}
		//////////////////////////////////////////////////////////////////////////
	}
	fd=0;
	strlog.Format(_T("SaveAlignPara() End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	return rslt;
}

void CDialogAlign::ClickBtnenhctrlAlignSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}

void CDialogAlign::ClickBtnenhctrlAlignApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ParameterCompare();
	Update(TRUE);
}


void CDialogAlign::ClickBtnenhctrlAlignMarkCopy0To1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}


void CDialogAlign::ClickBtnenhctrlAlignMarkFind0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	if(m_bAlignTestInspection1)
	{
		return;
	}
	m_bAlignTestInspection1 = TRUE;
	CString strlog;
	HTuple wd, ht;
	Herror herr;
	int cam_idx;
	int thresh;
	CRect rect;
	thresh = ALIGN_THRESH;

	double rx, ry, ra, rs, rscore; 
	Hlong rn;
	Hobject modimage, shape, Region, ImageReduced;

	double pat_score;
	pat_score = ALIGN_PAT_SCORE;

	strlog.Format(_T("ClickBtnenhctrlAlignMarkFind1() : Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	//UpdateData(TRUE);
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
		strlog.Format(_T("Halcon Error Occured at get_image_pointer1!"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
		return;
	}

	if(m_AlignData.m_bCreatPatIDAlign[cam_idx])
	{
		rect = m_AlignData.m_InsRect[cam_idx];
		gen_rectangle1(&Region, rect.top, rect.left, rect.bottom, rect.right);
			
		reduce_domain(m_HalImage[cam_idx] ,Region, &ImageReduced);

		Hal_ShapePatternFind(m_dlgImgRect[cam_idx]->GetView(), 
							&m_HalImage[cam_idx], 
							m_AlignData.m_PatIDAlign[cam_idx], 
							m_AlignPara[cam_idx], 
							m_AlignData.m_InsRect[cam_idx],
							(Hlong)wd[0],
							(Hlong)ht[0],
							0,
							0,
							pat_score,//Score
							&modimage, 
							&shape, 
							&rx, 
							&ry, 
							&ra, 
							&rs, 
							&rscore, 
							&rn);

		if(rn)
		{
			m_AlignRslt.m_dFindPosX_pixel[cam_idx] = rx;
			m_AlignRslt.m_dFindPosY_pixel[cam_idx] = -ry;
			m_AlignRslt.m_dFindPosA_pixel[cam_idx] = ra;
			m_AlignRslt.m_dFindScore[cam_idx] = rscore;
			m_AlignRslt.m_dFindNum[cam_idx] = rn;

			set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
			set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("SCORE : ") + HTuple(rscore * 100)));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 40, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("X : ") + HTuple(rx)));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 70, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("Y : ") + HTuple(ry)));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 100, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("ANGLE : ") + HTuple(ra)));

			m_nFindAlignCnt++;
		}
		else
		{
			set_color(m_dlgImgRect[cam_idx]->GetView(), "red");
			set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
			set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("FAIL : can't find model!")));
			strlog.Format(_T("FAIL : can't find model!"));
			pdlg->WriteLog(ALIGN_LOG,strlog);
		}
	}
	else
	{
		set_color(m_dlgImgRect[cam_idx]->GetView(), "red");
		set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
		write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("FAIL : no pattern!")));
		strlog.Format(_T("FAIL : no pattern!"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
	}
	m_bAlignTestInspection1 = FALSE;
	strlog.Format(_T("ClickBtnenhctrlAlignMarkFind1() : End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);

}

void CDialogAlign::ClickBtnenhctrlAlignMarkPosset0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	HTuple wd, ht;
	Herror herr;
	int cam_idx;
	int thresh;
	CRect rect;
	thresh = ALIGN_THRESH;

	double rx, ry, ra, rs, rscore; 
	Hlong rn;
	Hobject modimage, shape, Region, ImageReduced;

	double pat_score;
	pat_score = ALIGN_PAT_SCORE;
	if(IDCANCEL == AfxMessageBox(_T("정말 변경 하시겠습까?"), MB_OKCANCEL | MB_ICONQUESTION, 0))
	{
		return;
	}
	strlog.Format(_T("ClickBtnenhctrlAlignMarkPosset0() Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
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
		strlog.Format(_T("Halcon Error Occured at get_image_pointer1!"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
		return;
	}

	if(m_AlignData.m_bCreatPatIDAlign[cam_idx])
	{
		rect = m_AlignData.m_InsRect[cam_idx];
		gen_rectangle1(&Region, rect.top, rect.left, rect.bottom, rect.right);
			
		reduce_domain(m_HalImage[cam_idx] ,Region, &ImageReduced);

		Hal_ShapePatternFind(m_dlgImgRect[cam_idx]->GetView(), 
							&m_HalImage[cam_idx], 
							m_AlignData.m_PatIDAlign[cam_idx], 
							m_AlignPara[cam_idx], 
							m_AlignData.m_InsRect[cam_idx],
							(Hlong)wd[0],
							(Hlong)ht[0],
							0,
							0,
							pat_score,//Score
							&modimage, 
							&shape, 
							&rx, 
							&ry, 
							&ra, 
							&rs, 
							&rscore, 
							&rn);

		if(rn)
		{
			set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
			set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("SCORE : ") + HTuple(rscore * 100)));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 40, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("X : ") + HTuple(rx)));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 70, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("Y : ") + HTuple(ry)));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 100, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("ANGLE : ") + HTuple(ra)));

			//Position set
			m_edit_align_posx0 = rx;
			m_edit_align_posy0 = -ry;

		}
		else
		{
			set_color(m_dlgImgRect[cam_idx]->GetView(), "red");
			set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("FAIL : can't find model!")));
			strlog.Format(_T("FAIL : can't find model!"));
			pdlg->WriteLog(ALIGN_LOG,strlog);

			//Position set
			m_edit_align_posx0 = 0.;
			m_edit_align_posy0 = 0.;
		}
	}
	else
	{
		set_color(m_dlgImgRect[cam_idx]->GetView(), "red");
		set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
		set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
		write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("FAIL : no pattern!")));
		strlog.Format(_T("FAIL : no pattern!"));
		pdlg->WriteLog(ALIGN_LOG,strlog);

		//Position set
		m_edit_align_posx0 = 0.;
		m_edit_align_posy0 = 0.;
	
	}

	UpdateData(FALSE);
	
	strlog.Format(_T("ClickBtnenhctrlAlignMarkPosset0() End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);

}


void CDialogAlign::ClickBtnenhctrlAlignMarkPosset1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	HTuple wd, ht;
	Herror herr;
	int cam_idx;
	int thresh;
	CRect rect;
	thresh = ALIGN_THRESH;

	double rx, ry, ra, rs, rscore; 
	Hlong rn;
	Hobject modimage, shape, Region, ImageReduced;

	double pat_score;
	pat_score = ALIGN_PAT_SCORE;
	if(IDCANCEL == AfxMessageBox(_T("정말 변경 하시겠습까?"), MB_OKCANCEL | MB_ICONQUESTION, 0))
	{
		return;
	}
	strlog.Format(_T("ClickBtnenhctrlAlignMarkPosset1() Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	UpdateData(TRUE);
	Update(TRUE);
	cam_idx = 1;

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
		strlog.Format(_T("ClickBtnenhctrlAlignMarkPosset1() Start"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
		return;
	}

	if(m_AlignData.m_bCreatPatIDAlign[cam_idx])
	{
		rect = m_AlignData.m_InsRect[cam_idx];
		gen_rectangle1(&Region, rect.top, rect.left, rect.bottom, rect.right);
			
		reduce_domain(m_HalImage[cam_idx] ,Region, &ImageReduced);

		Hal_ShapePatternFind(m_dlgImgRect[cam_idx]->GetView(), 
							&m_HalImage[cam_idx], 
							m_AlignData.m_PatIDAlign[cam_idx], 
							m_AlignPara[cam_idx], 
							m_AlignData.m_InsRect[cam_idx],
							(Hlong)wd[0],
							(Hlong)ht[0],
							0,
							0,
							pat_score,//Score
							&modimage, 
							&shape, 
							&rx, 
							&ry, 
							&ra, 
							&rs, 
							&rscore, 
							&rn);

		if(rn)
		{
			set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
			set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("SCORE : ") + HTuple(rscore * 100)));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 40, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("X : ") + HTuple(rx)));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 70, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("Y : ") + HTuple(ry)));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 100, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("ANGLE : ") + HTuple(ra)));

			//Position set
			m_edit_align_posx1 = rx;
			m_edit_align_posy1 = -ry;

		}
		else
		{
			set_color(m_dlgImgRect[cam_idx]->GetView(), "red");
			set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("FAIL : can't find model!")));
			strlog.Format(_T("FAIL : can't find model!"));
			pdlg->WriteLog(ALIGN_LOG,strlog);
			//Position set
			m_edit_align_posx1 = 0.;
			m_edit_align_posy1 = 0.;
		}
	}
	else
	{
		set_color(m_dlgImgRect[cam_idx]->GetView(), "red");
		set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
		set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
		write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("FAIL : no pattern!")));
		strlog.Format(_T("FAIL : no pattern!"));
			pdlg->WriteLog(ALIGN_LOG,strlog);
		//Position set
		m_edit_align_posx1 = 0.;
		m_edit_align_posy1 = 0.;
	
	}
	strlog.Format(_T("ClickBtnenhctrlAlignMarkPosset1() End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	UpdateData(FALSE);
}


void CDialogAlign::ClickBtnenhctrlAlignLightSet0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_slider_align_light0.SetPos(m_edit_align_light0);
}


void CDialogAlign::ClickBtnenhctrlAlignLightSet1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_slider_align_light1.SetPos(m_edit_align_light1);
}


void CDialogAlign::ClickBtnenhctrlAlignMarkFind1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	if(m_bAlignTestInspection2)
	{
		return;
	}
	m_bAlignTestInspection2 = TRUE;
	CString strlog;
	HTuple wd, ht;
	Herror herr;
	int cam_idx;
	int thresh;
	CRect rect;
	thresh = ALIGN_THRESH;

	double rx, ry, ra, rs, rscore; 
	Hlong rn;
	Hobject modimage, shape, Region, ImageReduced;

	double pat_score;
	pat_score = ALIGN_PAT_SCORE;

	strlog.Format(_T("ClickBtnenhctrlAlignMarkFind1() : Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	//UpdateData(TRUE);
	Update(TRUE);
	cam_idx = 1;

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
		strlog.Format(_T("Halcon Error Occured at get_image_pointer1!"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
		return;
	}

	if(m_AlignData.m_bCreatPatIDAlign[cam_idx])
	{
		rect = m_AlignData.m_InsRect[cam_idx];
		gen_rectangle1(&Region, rect.top, rect.left, rect.bottom, rect.right);
			
		reduce_domain(m_HalImage[cam_idx] ,Region, &ImageReduced);

		Hal_ShapePatternFind(m_dlgImgRect[cam_idx]->GetView(), 
							&m_HalImage[cam_idx], 
							m_AlignData.m_PatIDAlign[cam_idx], 
							m_AlignPara[cam_idx], 
							m_AlignData.m_InsRect[cam_idx],
							(Hlong)wd[0],
							(Hlong)ht[0],
							0,
							0,
							pat_score,//Score
							&modimage, 
							&shape, 
							&rx, 
							&ry, 
							&ra, 
							&rs, 
							&rscore, 
							&rn);

		if(rn)
		{
			m_AlignRslt.m_dFindPosX_pixel[cam_idx] = rx;
			m_AlignRslt.m_dFindPosY_pixel[cam_idx] = -ry;
			m_AlignRslt.m_dFindPosA_pixel[cam_idx] = ra;
			m_AlignRslt.m_dFindScore[cam_idx] = rscore;
			m_AlignRslt.m_dFindNum[cam_idx] = rn;

			set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
			set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("SCORE : ") + HTuple(rscore * 100)));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 40, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("X : ") + HTuple(rx)));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 70, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("Y : ") + HTuple(ry)));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 100, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("ANGLE : ") + HTuple(ra)));

			m_nFindAlignCnt++;
		}
		else
		{
			set_color(m_dlgImgRect[cam_idx]->GetView(), "red");
			set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
			set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
			set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
			write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("FAIL : can't find model!")));
			strlog.Format(_T("FAIL : can't find model!"));
			pdlg->WriteLog(ALIGN_LOG,strlog);

		}
	}
	else
	{
		set_color(m_dlgImgRect[cam_idx]->GetView(), "red");
		set_tposition(m_dlgImgRect[cam_idx]->GetView(), 10, 10);
		write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple(HTuple("FAIL : no pattern!")));
		strlog.Format(_T("FAIL : no pattern!"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
	}
	m_bAlignTestInspection2 = FALSE;
	strlog.Format(_T("ClickBtnenhctrlAlignMarkFind1() : End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
}

BOOL CDialogAlign::MakeRsltCoord(double tx0, double ty0, double tx1, double ty1, 
								 double rx0, double ry0, double rx1, double ry1,
								 double shift_x, double shift_y, 
								 double camera_cnt_x, double camera_cnt_y,
								 double dist_inter_mark_x, double dist_inter_mark_y
								)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	BOOL rslt = TRUE;
	int cam_idx;

	strlog.Format(_T("MakeRsltCoord() Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	m_AlignRslt.m_dCoordCntX = camera_cnt_x;
	m_AlignRslt.m_dCoordCntY = camera_cnt_y;

	m_AlignRslt.m_dDistInterMarkX = dist_inter_mark_x;
	m_AlignRslt.m_dDistInterMarkY = dist_inter_mark_y;

	//Teach위치
	DistXY(tx0, ty0, m_AlignRslt.m_dCoordCntX, m_AlignRslt.m_dCoordCntY, &m_AlignRslt.m_dCoordMarkCntX_teach[0], &m_AlignRslt.m_dCoordMarkCntY_teach[0]);
	DistXY(tx1, ty1, m_AlignRslt.m_dCoordCntX, m_AlignRslt.m_dCoordCntY, &m_AlignRslt.m_dCoordMarkCntX_teach[1], &m_AlignRslt.m_dCoordMarkCntY_teach[1]);
	m_AlignRslt.m_dCoordMarkCntX_teach[1] += m_AlignRslt.m_dDistInterMarkX;
	m_AlignRslt.m_dCoordMarkCntY_teach[1] += m_AlignRslt.m_dDistInterMarkY;
	//Find위치
	DistXY(rx0, ry0, m_AlignRslt.m_dCoordCntX, m_AlignRslt.m_dCoordCntY, &m_AlignRslt.m_dCoordMarkCntX_rslt[0], &m_AlignRslt.m_dCoordMarkCntY_rslt[0]);
	DistXY(rx1, ry1, m_AlignRslt.m_dCoordCntX, m_AlignRslt.m_dCoordCntY, &m_AlignRslt.m_dCoordMarkCntX_rslt[1], &m_AlignRslt.m_dCoordMarkCntY_rslt[1]);
	m_AlignRslt.m_dCoordMarkCntX_rslt[1] += m_AlignRslt.m_dDistInterMarkX;
	m_AlignRslt.m_dCoordMarkCntY_rslt[1] += m_AlignRslt.m_dDistInterMarkY;

	//0번과 1번의 각도
	AngBetween(m_AlignRslt.m_dCoordMarkCntX_teach[0], m_AlignRslt.m_dCoordMarkCntY_teach[0], m_AlignRslt.m_dCoordMarkCntX_teach[1], m_AlignRslt.m_dCoordMarkCntY_teach[1], 
		       m_AlignRslt.m_dCoordMarkCntX_rslt[0], m_AlignRslt.m_dCoordMarkCntY_rslt[0], m_AlignRslt.m_dCoordMarkCntX_rslt[1], m_AlignRslt.m_dCoordMarkCntY_rslt[1],
			   &m_AlignRslt.m_dCoordAng_rslt);

	m_AlignRslt.m_dCoordShiftX = shift_x;
	m_AlignRslt.m_dCoordShiftY = shift_y;

	//결과 Ang
	m_AlignRslt.m_dCoordAng_diff = -m_AlignRslt.m_dCoordAng_rslt;

	//X, Y 결과는 cam0만 처리하면된다. cam1번것은 각도만 찾기 위해 사용한다.
	cam_idx = 0;
	RotatedPt(m_AlignRslt.m_dCoordMarkCntX_rslt[cam_idx], m_AlignRslt.m_dCoordMarkCntY_rslt[cam_idx],				
			  m_AlignRslt.m_dCoordShiftX, m_AlignRslt.m_dCoordShiftY,	
			  m_AlignRslt.m_dCoordAng_diff,						
			  &m_AlignRslt.m_dCoordMarkCntX_rslt_afterRot[cam_idx], &m_AlignRslt.m_dCoordMarkCntY_rslt_afterRot[cam_idx]);

	//X, Y(차이값) 결과
	DistXY(m_AlignRslt.m_dCoordMarkCntX_rslt_afterRot[cam_idx], m_AlignRslt.m_dCoordMarkCntY_rslt_afterRot[cam_idx],
		   m_AlignRslt.m_dCoordMarkCntX_teach[cam_idx], m_AlignRslt.m_dCoordMarkCntY_teach[cam_idx], 
		   &m_AlignRslt.m_dCoordX_rslt, &m_AlignRslt.m_dCoordY_rslt);

	m_AlignRslt.m_dCoordX_diff = -m_AlignRslt.m_dCoordX_rslt;
	m_AlignRslt.m_dCoordY_diff = -m_AlignRslt.m_dCoordY_rslt;

	char result0[DAT_STR_LENG], result1[DAT_STR_LENG], result2[DAT_STR_LENG];
	//Cam0//////////////////////////////////////////////////////////////////////////////
	cam_idx = 0;
	set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
	set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
	set_tposition(m_dlgImgRect[cam_idx]->GetView(),ALIGN_IMAGE_HEIGHT - 200, 10);

	sprintf(result0, "%0.3f", m_AlignRslt.m_dCoordAng_diff);
	sprintf(result1, "%0.3f", m_AlignRslt.m_dCoordAng_rslt);
	write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("Result angle : ") + HTuple(result0) + HTuple("deg")
		        + HTuple("(FIND ANGLE : ") + HTuple(result1) + HTuple("deg)") );

	sprintf(result0, "%0.3f", m_AlignRslt.m_dCoordX_diff);
	sprintf(result1, "%0.3f", m_AlignRslt.m_dCoordMarkCntX_rslt[cam_idx]);
	sprintf(result2, "%0.3f", m_AlignRslt.m_dCoordMarkCntX_rslt_afterRot[cam_idx]);
	set_tposition(m_dlgImgRect[cam_idx]->GetView(),ALIGN_IMAGE_HEIGHT - 180, 10);
	write_string( m_dlgImgRect[cam_idx]->GetView(), HTuple("Result X : ") + HTuple(result0) + HTuple("mm")
				+ HTuple("(FIND POS. X : ") + HTuple(result1) + HTuple("mm)") 
				+ HTuple("(ROTATE POS. X : ") + HTuple(result2) + HTuple("mm)")
				);

	sprintf(result0, "%0.3f", m_AlignRslt.m_dCoordY_diff);
	sprintf(result1, "%0.3f", m_AlignRslt.m_dCoordMarkCntY_rslt[cam_idx]);
	sprintf(result2, "%0.3f", m_AlignRslt.m_dCoordMarkCntY_rslt_afterRot[cam_idx]);
	set_tposition(m_dlgImgRect[cam_idx]->GetView(),ALIGN_IMAGE_HEIGHT - 160, 10);
	write_string( m_dlgImgRect[cam_idx]->GetView(), HTuple("Result Y : ") + HTuple(result0) + HTuple("mm")
				+ HTuple("(FIND POS. Y : ") + HTuple(result1) + HTuple("mm)") 
				+ HTuple("(ROTATE POS. Y : ") + HTuple(result2) + HTuple("mm)")
				);

	//Cam1//////////////////////////////////////////////////////////////////////////////
	cam_idx = 1;
	set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
	set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
	set_tposition(m_dlgImgRect[cam_idx]->GetView(),ALIGN_IMAGE_HEIGHT - 200, 10);

	sprintf(result0, "%0.3f", m_AlignRslt.m_dCoordAng_diff);
	sprintf(result1, "%0.3f", m_AlignRslt.m_dCoordAng_rslt);
	write_string(m_dlgImgRect[cam_idx]->GetView(), HTuple("Result angle : ") + HTuple(result0) + HTuple("deg")
		        + HTuple("(FIND ANGLE : ") + HTuple(result1) + HTuple("deg)") );

	sprintf(result0, "%0.3f", m_AlignRslt.m_dCoordX_diff);
	sprintf(result1, "%0.3f", m_AlignRslt.m_dCoordMarkCntX_rslt[cam_idx]);
	sprintf(result2, "%0.3f", m_AlignRslt.m_dCoordMarkCntX_rslt_afterRot[cam_idx]);
	set_tposition(m_dlgImgRect[cam_idx]->GetView(),ALIGN_IMAGE_HEIGHT - 180, 10);
	write_string( m_dlgImgRect[cam_idx]->GetView(), HTuple("Result X : ") + HTuple(result0) + HTuple("mm")
				+ HTuple("(FIND POS. X : ") + HTuple(result1) + HTuple("mm)") 
				+ HTuple("(ROTATE POS. X : ") + HTuple(result2) + HTuple("mm)")
				);

	sprintf(result0, "%0.3f", m_AlignRslt.m_dCoordY_diff);
	sprintf(result1, "%0.3f", m_AlignRslt.m_dCoordMarkCntY_rslt[cam_idx]);
	sprintf(result2, "%0.3f", m_AlignRslt.m_dCoordMarkCntY_rslt_afterRot[cam_idx]);
	set_tposition(m_dlgImgRect[cam_idx]->GetView(),ALIGN_IMAGE_HEIGHT - 160, 10);
	write_string( m_dlgImgRect[cam_idx]->GetView(), HTuple("Result Y : ") + HTuple(result0) + HTuple("mm")
				+ HTuple("(FIND POS. Y : ") + HTuple(result1) + HTuple("mm)") 
				+ HTuple("(ROTATE POS. Y : ") + HTuple(result2) + HTuple("mm)")
				);
	strlog.Format(_T("MakeRsltCoord() End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	return rslt;
}

void CDialogAlign::ClickBtnenhctrlAlignSetAlignPos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	//20160715 ngh
	if(IDCANCEL == AfxMessageBox(_T("정말 변경 하시겠습까?"), MB_OKCANCEL | MB_ICONQUESTION, 0))
	{
		return;
	}
	strlog.Format(_T("ClickBtnenhctrlAlignSetAlignPos() Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	
	m_edit_align_y_axi_pos = pdlg->m_Robot->GetRobotPos_mm(MOTOR_0);
	m_edit_align_t_axi_pos = pdlg->m_Robot->GetRobotPos_mm(MOTOR_2);

	UpdateData(FALSE);
	strlog.Format(_T("ClickBtnenhctrlAlignSetAlignPos() End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
}


void CDialogAlign::ClickBtnenhctrlAlignAlignMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	strlog.Format(_T("ClickBtnenhctrlAlignAlignMove() Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, m_edit_align_y_axi_pos);
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_2, m_edit_align_t_axi_pos);

	strlog.Format(_T("ClickBtnenhctrlAlignAlignMove() End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
}

void CDialogAlign::ClickBtnenhctrlAlignMovepos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	CString strlog;
	strlog.Format(_T("ClickBtnenhctrlAlignMovepos() Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	//Align적용
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, m_AlignRslt.m_dRobotPosX_afteralign);
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, m_AlignRslt.m_dRobotPosY_afteralign);
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_2, m_AlignRslt.m_dRobotPosT_afteralign);

	strlog.Format(_T("ClickBtnenhctrlAlignMovepos() End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
}

void CDialogAlign::ClickBtnenhctrlAlignReviewMovePos0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	strlog.Format(_T("ClickBtnenhctrlAlignReviewMovePos0() Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);

	pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, m_AlignRslt.m_dRobotPosX_afteroffset);
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, m_AlignRslt.m_dRobotPosY_afteroffset);
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_2, m_AlignRslt.m_dRobotPosT_afteroffset);

	strlog.Format(_T("ClickBtnenhctrlAlignReviewMovePos0() End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
}

void CDialogAlign::ClickBtnenhctrlAlignReviewMovePos1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	strlog.Format(_T("ClickBtnenhctrlAlignReviewMovePos1() Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	double x, y, t;

	x = m_AlignRslt.m_dRobotPosX_afteroffset + m_AlignRslt.m_dDistInterMarkX;
	y = m_AlignRslt.m_dRobotPosY_afteroffset + m_AlignRslt.m_dDistInterMarkY;
	t = m_AlignRslt.m_dRobotPosT_afteroffset;

	pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, x);
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, y);
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_2, t);

	strlog.Format(_T("ClickBtnenhctrlAlignReviewMovePos1() End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
}


void CDialogAlign::ClickBtnenhctrlAlignSetAlignPosShift()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	//20160715 ngh
	if(IDCANCEL == AfxMessageBox(_T("정말 변경 하시겠습까?"), MB_OKCANCEL | MB_ICONQUESTION, 0))
	{
		return;
	}
	strlog.Format(_T("ClickBtnenhctrlAlignSetAlignPosShift() Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
	
	m_edit_align_x_axi_pos = pdlg->m_Robot->GetRobotPos_mm(MOTOR_1);

	UpdateData(FALSE);
	strlog.Format(_T("ClickBtnenhctrlAlignSetAlignPosShift() End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
}


void CDialogAlign::ClickBtnenhctrlAlignAlignMoveShift()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	strlog.Format(_T("ClickBtnenhctrlAlignAlignMoveShift() Start"));
	pdlg->WriteLog(ALIGN_LOG,strlog);

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, m_edit_align_x_axi_pos);

	strlog.Format(_T("ClickBtnenhctrlAlignAlignMoveShift() End"));
	pdlg->WriteLog(ALIGN_LOG,strlog);
}

double CDialogAlign::GetAlignPosX()
{
	double x;

	x = m_AlignRslt.m_dRobotPosX_afteralign;

	x = x + m_edit_align_offset_x;

	return x;
}

double CDialogAlign::GetAlignPosY()
{
	double y;

	y = m_AlignRslt.m_dRobotPosY_afteralign;

	y = y - m_edit_align_offset_y;

	return y;
}

double CDialogAlign::GetAlignPosT()
{
	double t;

	t = m_AlignRslt.m_dRobotPosT_afteralign;

	t = t + m_edit_align_offset_t;

	return t;
}

double CDialogAlign::GetAlignOffsetPosX()
{
	double x;

	x = m_AlignRslt.m_dRobotPosX_afteroffset;

	return x;
}

double CDialogAlign::GetAlignOffsetPosY()
{
	double y;

	y = m_AlignRslt.m_dRobotPosY_afteroffset;

	return y;
}

double CDialogAlign::GetAlignOffsetPosX_coordXY()
{
	double y;
	//축이 x, y 바뀌어 있다. 반대로 리턴
	y = m_AlignRslt.m_dRobotPosY_afteroffset;

	return y;
}

double CDialogAlign::GetAlignOffsetPosY_coordXY()
{
	double x;
	//축이 x, y 바뀌어 있다. 반대로 리턴
	x = m_AlignRslt.m_dRobotPosX_afteroffset;

	return x;
}


double CDialogAlign::GetAlignOffsetPosT()
{
	double t;

	t = m_AlignRslt.m_dRobotPosT_afteroffset;

	return t;
}

int CDialogAlign::GetLight(int idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	int val;
	val = 0;
	switch(idx)
	{
	case 0:
		val = (int)(m_edit_align_light0);
		break;
	case 1:
		val = (int)(m_edit_align_light1);
		break;
	default:
		AfxMessageBox(_T("Error : invalid index!(CDialogAlign::GetLight)"));
		strlog.Format(_T("Error : invalid index!(CDialogAlign::GetLight)"));
		pdlg->WriteLog(ALIGN_LOG,strlog);
		break;
	}

	return val;
}

void CDialogAlign::MouseDownBtnenhctrlAlignJogL(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_1, DIR_NEGATIVE, m_iJogSpdSelect);
}


void CDialogAlign::MouseUpBtnenhctrlAlignJogL(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_1);
}


void CDialogAlign::MouseDownBtnenhctrlAlignJogR(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_1, DIR_POSITIVE, m_iJogSpdSelect);
}

void CDialogAlign::MouseUpBtnenhctrlAlignJogR(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_1);

}

double CDialogAlign::GetAlignReadyPosY()
{
	return m_edit_align_y_axi_pos;
}

double CDialogAlign::GetAlignReadyPosT()
{
	return m_edit_align_t_axi_pos;
}

void CDialogAlign::SetGrab()
{
	ClickBtnenhctrlAlignSnap();
}

int CDialogAlign::SetFind()
{
	int rslt = 0;
	if(!m_bAlignInspection)
	{
		m_bAlignInspection = TRUE;
		rslt = AlignInspectFindPos();
		m_bAlignInspection = FALSE;
	}
	return rslt;
}

double CDialogAlign::GetAlignAfterPosX()
{
	return m_AlignRslt.m_dRobotPosX_afteralign;
}

double CDialogAlign::GetAlignAfterPosY()
{
	return m_AlignRslt.m_dRobotPosY_afteralign;
}

double CDialogAlign::GetAlignAfterPosT()
{
	return m_AlignRslt.m_dRobotPosT_afteralign;
}





BOOL CDialogAlign::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}



void CDialogAlign::ParameterCompare()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strType;
	strType.Format(_T("RecipeAlign_Para_"));
	int idx = 0;

	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_y_axi_pos, strType + _T("m_edit_align_y_axi_pos"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_t_axi_pos, strType + _T("m_edit_align_t_axi_pos"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_offset_x, strType + _T("m_edit_align_offset_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_offset_y, strType + _T("m_edit_align_offset_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_mark_size0, strType + _T("m_edit_align_mark_size0"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_mark_size1, strType + _T("m_edit_align_mark_size1"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_mark_res0, strType + _T("m_edit_align_mark_res0"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_mark_res1, strType + _T("m_edit_align_mark_res1"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_posx0, strType + _T("m_edit_align_posx0"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_posy0, strType + _T("m_edit_align_posy0"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_posx1, strType + _T("m_edit_align_posx1"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_posy1, strType + _T("m_edit_align_posy1"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_shaftx, strType + _T("m_edit_align_shaftx"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_shafty, strType + _T("m_edit_align_shafty"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_light0, strType + _T("m_edit_align_light0"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_light1, strType + _T("m_edit_align_light1"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_offset_x_rat, strType + _T("m_edit_offset_x_rat"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_offset_y_rat, strType + _T("m_edit_offset_y_rat"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_pixel_size, strType + _T("m_edit_align_pixel_size"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_offset_t, strType + _T("m_edit_align_offset_t"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_jog_pitch, strType + _T("m_edit_align_jog_pitch"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_shaft_offset_x, strType + _T("m_edit_align_shaft_offset_x"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_shaft_offset_y, strType + _T("m_edit_align_shaft_offset_y"));
	pdlg->ParaCompareLogWrite(m_dRecipeAlignPara[idx++], m_edit_align_x_axi_pos, strType + _T("m_edit_align_x_axi_pos"));
}