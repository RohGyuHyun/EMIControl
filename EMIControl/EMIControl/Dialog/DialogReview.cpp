// Dialog/DialogReview.cpp : 구현 파일입니다.
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


// CDialogReview 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogReview, CDialogEx)

CDialogReview::CDialogReview(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogReview::IDD, pParent)
	, m_edit_rv_cell_row(1)
	, m_edit_rv_cell_col(1)
	, m_check_rv_cell_nevi_move(FALSE)
	, m_edit_rv_light(0)
	, m_edit_offset_x(0)
	, m_edit_offset_y(0)
	, m_edit_dGlass_x(0)
	, m_edit_dGlass_y(0)
{
	MilApplication = NULL;

	int i;

	for(i = 0; i < REVIEW_IMG_RECT_MAX_NUM; i++)
	{
		MilWBCoefficients[i] = NULL;
		MilDigitizer[i] = NULL;
		MilSystem[i] = NULL;

		MilBayerBuf_R[i] = NULL;
		MilBayerBuf_G[i] = NULL;
		MilBayerBuf_B[i] = NULL;
		MilBayerBuf[i] = NULL;
		MilGrabBuf[i] = NULL;
	}

	m_iJogSpdSelect = 0;
	m_edit_rv_light_old = 0.;
	m_nZoomNum = 0;
}

CDialogReview::~CDialogReview()
{
	int i;

	SaveReview(_T(REVIEW_PATH));

	for(i = 0; i < REVIEW_IMG_RECT_MAX_NUM; i++)
	{
		if(MilWBCoefficients[i] != NULL)
			MbufFree(MilWBCoefficients[i]);
	}

	for(i = 0; i < REVIEW_IMG_RECT_MAX_NUM; i++)
	{

		if(NULL != MilGrabBuf[i])
		{
			if(MilBayerBuf_R[i] != NULL){
				MbufFree(MilBayerBuf_R[i]);	
				MilBayerBuf_R[i] = NULL;
			}
			if(MilBayerBuf_G[i] != NULL){
				MbufFree(MilBayerBuf_G[i]);	
				MilBayerBuf_G[i] = NULL;
			}
			if(MilBayerBuf_B[i] != NULL){
				MbufFree(MilBayerBuf_B[i]);	
				MilBayerBuf_B[i] = NULL;
			}

			if(MilBayerBuf[i] != NULL){
				MbufFree(MilBayerBuf[i]);	
				MilBayerBuf[i] = NULL;
			}

			if(MilGrabBuf[i] != NULL){
				MbufFree(MilGrabBuf[i]);	
				MilGrabBuf[i] = NULL;
			}
		}

	}

	for(i = 0; i < REVIEW_IMG_RECT_MAX_NUM; i++)
	{
		if(NULL != MilDigitizer[i])
		{
			MdigFree(MilDigitizer[i]);
			MilDigitizer[i] = NULL;
		}
	}

	for(i = 0; i < REVIEW_IMG_RECT_MAX_NUM; i++)
	{
		if(NULL != MilSystem[i])
		{
			MsysFree(MilSystem[i]);
			MilSystem[i] = NULL;
		}
	}

	if(NULL != MilApplication)
	{
		MappFree(MilApplication);
		MilApplication = NULL;
	}
}

void CDialogReview::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNENHCTRL_RV_JOG_SPD1, m_btn_jog_spd0);
	DDX_Control(pDX, IDC_BTNENHCTRL_RV_JOG_SPD2, m_btn_jog_spd1);
	DDX_Control(pDX, IDC_BTNENHCTRL_RV_JOG_SPD3, m_btn_jog_spd2);
	DDX_Text(pDX, IDC_EDIT_RV_Y, m_edit_rv_cell_row);
	DDX_Text(pDX, IDC_EDIT_RV_X, m_edit_rv_cell_col);
	DDX_Check(pDX, IDC_CHECK_RV_CELL_NEVI2, m_check_rv_cell_nevi_move);
	DDX_Control(pDX, IDC_BTNENHCTRL_RV_ZOOM_1, m_btn_rv_zoom_1);
	DDX_Control(pDX, IDC_BTNENHCTRL_RV_ZOOM_MAG2, m_btn_rv_zoom_2);
	DDX_Control(pDX, IDC_BTNENHCTRL_RV_ZOOM_MAG3, m_btn_rv_zoom_3);
	DDX_Text(pDX, IDC_EDIT_RV_LIGHT, m_edit_rv_light);
	DDX_Control(pDX, IDC_SLIDER_RV_LIGHT, m_slider_rv_light);
	DDX_Control(pDX, IDC_BTNENHCTRL_RV_AF_USE, m_btn_rv_af_use);
	DDX_Control(pDX, IDC_BTNENHCTRL_RV_AF_USE2, m_btn_light_use);
	DDX_Control(pDX, IDC_COMBO_RV_AF_STEP, m_combo_rv_af_step);
	DDX_Control(pDX, IDC_BTNENHCTRL_RV_CAMERA_SCAN_IMAGE, m_btn_check_ccd);
	DDX_Text(pDX, IDC_EDIT_RV_OFFSET_X, m_edit_offset_x);
	DDX_Text(pDX, IDC_EDIT_RV_OFFSET_Y, m_edit_offset_y);
	DDX_Text(pDX, IDC_EDIT_RV_GLASS_X, m_edit_dGlass_x);
	DDX_Text(pDX, IDC_EDIT_RV_GLASS_Y, m_edit_dGlass_y);
}


BEGIN_MESSAGE_MAP(CDialogReview, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(USER_MSG_IMG_RECT, OnImageRectMessage)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_RV_LIGHT, &CDialogReview::OnNMCustomdrawSliderRvLight)
//	ON_EN_CHANGE(IDC_EDIT_RV_LIGHT, &CDialogReview::OnEnChangeEditRvLight)
ON_EN_CHANGE(IDC_EDIT_RV_LIGHT, &CDialogReview::OnEnChangeEditRvLight)
ON_CBN_SELCHANGE(IDC_COMBO_RV_AF_STEP, &CDialogReview::OnCbnSelchangeComboRvAfStep)
END_MESSAGE_MAP()


// CDialogReview 메시지 처리기입니다.


BOOL CDialogReview::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Herror herr;	
	CRect rect;
	int i;
	HTuple wd, ht;
	long width, height;


	
	for(i = 0; i < REVIEW_IMG_RECT_MAX_NUM; i++)
	{
		m_dlgImgRect[i] = new C_CtrlImageRect;
		m_dlgImgRect[i]->Create(IMAGE_RECT_DIALOG, this);

		switch(i)
		{
		case 0:
			GetDlgItem(IDC_STATIC_REVIEW0)->GetClientRect(rect);
			GetDlgItem(IDC_STATIC_REVIEW0)->ClientToScreen(rect);

			m_dlgImgRect[i]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);
			m_dlgImgRect[i]->SetCamIndex(0, _T("                                             REVIEW CAMERA"), this->m_hWnd);
			break;
		case 1:
		default:
			AfxMessageBox(_T("Error : invalid index(CDialogReview::OnInitDialog())!"));
			break;
		}

		m_dlgImgRect[i]->ShowWindow(SW_SHOW);

	}

	//Halcon image initialize
	for(i = 0; i < REVIEW_IMG_RECT_MAX_NUM; i++)
	{
		gen_image_const(&m_HalImage[i], HTuple("byte"), REVIEW_IMAGE_WIDTH, REVIEW_IMAGE_HEIGHT);

		set_check("~give_error");
			herr = get_image_pointer1(m_HalImage[i], NULL, NULL, &wd, &ht);
		set_check("give_error");

		if(H_MSG_TRUE == herr)
		{
		}
		else
		{
			AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1_00!"));
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

	//Matrox/////////////////////////////////////
	MIL_INT64 buf_width[REVIEW_IMG_RECT_MAX_NUM], buf_height[REVIEW_IMG_RECT_MAX_NUM];
	MIL_INT SizeBand;

#ifdef CAMERA
	//Application
	MappAlloc(M_DEFAULT, &MilApplication); 
	
	//Framegrabber
	for(i = 0; i < REVIEW_IMG_RECT_MAX_NUM; i++)
	{
		MsysAlloc(M_SYSTEM_SOLIOS, i, M_DEFAULT, &MilSystem[i]);

	}

	//Digitizer allocation
	for(i = 0; i < REVIEW_IMG_RECT_MAX_NUM; i++)
	{
		MdigAlloc(MilSystem[i], (long)i, (MIL_API_CONST_TEXT_PTR)_T("D:\\Project\\dcf\\S_KP_FD510WCL.dcf"), M_DEFAULT, &MilDigitizer[i]);
		
		//HW Trigger Grab Mode 설정시
		MdigControl(MilDigitizer[i], M_GRAB_MODE, M_ASYNCHRONOUS); 
		MdigControl(MilDigitizer[i], M_GRAB_EXPOSURE_SOURCE, M_SOFTWARE);
		MdigControl(MilDigitizer[i], M_GRAB_TIMEOUT, M_INFINITE);
		
		SizeBand = MdigInquire(MilDigitizer[i], M_SIZE_BAND, M_NULL);
	}
	
	for(i = 0; i < REVIEW_IMG_RECT_MAX_NUM; i++)
	{
		buf_width[i] = width;
		buf_height[i] = height;

		MbufAllocColor(MilSystem[i], 3, buf_width[i], buf_height[i], 8L+M_UNSIGNED, M_IMAGE+M_GRAB, &MilGrabBuf[i]);	
		MbufClear(MilGrabBuf[i], M_NULL);
		MbufAllocColor(MilSystem[i], 3, buf_width[i], buf_height[i], 8L+M_UNSIGNED, M_IMAGE, &MilBayerBuf[i]);
		MbufClear(MilBayerBuf[i], M_NULL);

		MbufChildColor(MilBayerBuf[i], M_RED,   &MilBayerBuf_R[i]);
		MbufChildColor(MilBayerBuf[i], M_GREEN, &MilBayerBuf_G[i]);
		MbufChildColor(MilBayerBuf[i], M_BLUE,  &MilBayerBuf_B[i]);
	}

	for(i = 0; i < REVIEW_IMG_RECT_MAX_NUM; i++)
	{
		//White Balance 초기화
		MbufAlloc1d(MilSystem[i], 3, 32+M_FLOAT, M_ARRAY, &MilWBCoefficients[i]);

		WBCoefficients[i][0] = (float)1.2955;
		WBCoefficients[i][1] = (float)0.90164;
		WBCoefficients[i][2] = (float)0.96474;

		MbufPut(MilWBCoefficients[i], (void *)&WBCoefficients[i][0]);
	}
#endif

	LoadReview(_T(REVIEW_PATH));
	m_iAFJogStep = 0;
	m_slider_rv_light.SetRange(0, 100);
	m_slider_rv_light.SetPos(m_edit_rv_light);
	m_combo_rv_af_step.AddString(_T("16"));
	m_combo_rv_af_step.AddString(_T("50"));
	m_combo_rv_af_step.AddString(_T("100"));
	m_combo_rv_af_step.AddString(_T("200"));
	m_combo_rv_af_step.AddString(_T("500"));
	m_combo_rv_af_step.SetCurSel(0);
	OnCbnSelchangeComboRvAfStep();
	UpdateData(FALSE);

	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogReview::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int i;
	CRect rect;

	if(bShow)
	{
		for(i = 0; i < REVIEW_IMG_RECT_MAX_NUM; i++)
		{

			GetDlgItem(IDC_STATIC_REVIEW0)->GetClientRect(rect);
			GetDlgItem(IDC_STATIC_REVIEW0)->ClientToScreen(rect);

			m_dlgImgRect[i]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);

			m_dlgImgRect[i]->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		for(int i = 0; i < REVIEW_IMG_RECT_MAX_NUM; i++)
		{
			m_dlgImgRect[i]->ShowWindow(SW_HIDE);
		}
	}
}
BEGIN_EVENTSINK_MAP(CDialogReview, CDialogEx)
	ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_ZOOM_1, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvZoom1, VTS_NONE)
	ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_SPD1, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvJogSpd1, VTS_NONE)
	ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_L, DISPID_MOUSEDOWN, CDialogReview::MouseDownBtnenhctrlRvJogL, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_L, DISPID_MOUSEUP, CDialogReview::MouseUpBtnenhctrlRvJogL, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_SPD2, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvJogSpd2, VTS_NONE)
	ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_SPD3, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvJogSpd3, VTS_NONE)
	ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_R, DISPID_MOUSEDOWN, CDialogReview::MouseDownBtnenhctrlRvJogR, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_R, DISPID_MOUSEUP, CDialogReview::MouseUpBtnenhctrlRvJogR, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_U, DISPID_MOUSEDOWN, CDialogReview::MouseDownBtnenhctrlRvJogU, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_U, DISPID_MOUSEUP, CDialogReview::MouseUpBtnenhctrlRvJogU, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
//	ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_D, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvJogD, VTS_NONE)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_D, DISPID_MOUSEDOWN, CDialogReview::MouseDownBtnenhctrlRvJogD, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_D, DISPID_MOUSEUP, CDialogReview::MouseUpBtnenhctrlRvJogD, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_CCW, DISPID_MOUSEDOWN, CDialogReview::MouseDownBtnenhctrlRvJogCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_CCW, DISPID_MOUSEUP, CDialogReview::MouseUpBtnenhctrlRvJogCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_CW, DISPID_MOUSEDOWN, CDialogReview::MouseDownBtnenhctrlRvJogCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_CW, DISPID_MOUSEUP, CDialogReview::MouseUpBtnenhctrlRvJogCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_CELL_MOVE, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvCellMove, VTS_NONE)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_CELL_LF, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvCellLf, VTS_NONE)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_CELL_RT, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvCellRt, VTS_NONE)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_CELL_U, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvCellU, VTS_NONE)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_CELL_DN, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvCellDn, VTS_NONE)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_ZOOM_MAG2, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvZoomMag2, VTS_NONE)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_ZOOM_MAG3, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvZoomMag3, VTS_NONE)
//ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_AF_UP, DISPID_MOUSEDOWN, CDialogReview::MouseDownBtnenhctrlRvAfUp, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
//ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_AF_DN, DISPID_MOUSEDOWN, CDialogReview::MouseDownBtnenhctrlRvAfDn, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_AF_USE2, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvAfUse2, VTS_NONE)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_AF_USE, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvAfUse, VTS_NONE)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_CAMERA_SCAN_IMAGE, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvCameraScanImage, VTS_NONE)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_SET, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvSet, VTS_NONE)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_AF_UP, DISPID_MOUSEDOWN, CDialogReview::MouseDownBtnenhctrlRvAfUp, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_AF_UP, DISPID_MOUSEUP, CDialogReview::MouseUpBtnenhctrlRvAfUp, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_AF_DN, DISPID_MOUSEDOWN, CDialogReview::MouseDownBtnenhctrlRvAfDn, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_AF_DN, DISPID_MOUSEUP, CDialogReview::MouseUpBtnenhctrlRvAfDn, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_LU, DISPID_MOUSEDOWN, CDialogReview::MouseDownBtnenhctrlRvJogLu, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_LU, DISPID_MOUSEUP, CDialogReview::MouseUpBtnenhctrlRvJogLu, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
//ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_RU, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvJogRu, VTS_NONE)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_RU, DISPID_MOUSEDOWN, CDialogReview::MouseDownBtnenhctrlRvJogRu, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_RU, DISPID_MOUSEUP, CDialogReview::MouseUpBtnenhctrlRvJogRu, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_DIRLD, DISPID_MOUSEDOWN, CDialogReview::MouseDownBtnenhctrlRvJogDirld, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_DIRLD, DISPID_MOUSEUP, CDialogReview::MouseUpBtnenhctrlRvJogDirld, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_RD, DISPID_MOUSEDOWN, CDialogReview::MouseDownBtnenhctrlRvJogRd, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_RD, DISPID_MOUSEUP, CDialogReview::MouseUpBtnenhctrlRvJogRd, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_JOG_KBD, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvJogKbd, VTS_NONE)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_MOVE, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvMove, VTS_NONE)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_STOP, DISPID_CLICK, CDialogReview::ClickBtnenhctrlRvStop, VTS_NONE)
END_EVENTSINK_MAP()




void CDialogReview::Grab(int cam_idx, BOOL disp)
{
	MIL_INT64 width, height;
	BYTE *image_ptr;
	Hobject MultiChannelImage;

	//Get image size & set grab buffer
	MdigInquire(MilDigitizer[cam_idx], M_SIZE_X, &width);
	MdigInquire(MilDigitizer[cam_idx], M_SIZE_Y, &height);	
		
	MdigGrab(MilDigitizer[cam_idx], MilGrabBuf[cam_idx]);	

	//HW Trigger 설정시 
	//MdigControl(MilDigitizer[cam_idx], M_GRAB_EXPOSURE+M_TIMER1, M_ACTIVATE);
	//MdigGrabWait(MilDigitizer[cam_idx], M_GRAB_END);
	
	BYTE *Rimage_ptr, *Gimage_ptr, *Bimage_ptr;

	//MbufBayer(MilGrabBuf[cam_idx], MilBayerBuf[cam_idx], MilWBCoefficients[cam_idx], M_BAYER_GR);
	MbufCopy(MilGrabBuf[cam_idx], MilBayerBuf[cam_idx]);


	MbufInquire(MilBayerBuf_R[cam_idx], M_HOST_ADDRESS, &Rimage_ptr);
	MbufInquire(MilBayerBuf_G[cam_idx], M_HOST_ADDRESS, &Gimage_ptr);
	MbufInquire(MilBayerBuf_B[cam_idx], M_HOST_ADDRESS, &Bimage_ptr);

	gen_image1_extern(&m_HalImageR[cam_idx], "byte", width, height, (Hlong)Rimage_ptr, (Hlong) 0);
	gen_image1_extern(&m_HalImageG[cam_idx], "byte", width, height, (Hlong)Gimage_ptr, (Hlong) 0);
	gen_image1_extern(&m_HalImageB[cam_idx], "byte", width, height, (Hlong)Bimage_ptr, (Hlong) 0);

	compose3(m_HalImageR[cam_idx], m_HalImageG[cam_idx], m_HalImageB[cam_idx], &m_HalImage[cam_idx]);

	if(disp)
	{
		disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
	}
}


LRESULT CDialogReview::OnImageRectMessage(WPARAM para0, LPARAM para1)
{
	
	// TODO: Add your control notification handler code here
	HTuple wd, ht;
	Herror herr;
	CString status;
	int cam_idx;
	cam_idx = 0;
	double pixel_size = 0.;
	CPoint s_p, e_p;
	long l,t,r,b;
	long sx,sy;
	char temp[512];
	double w,h;

	//임시
	int nIdx;
	CString strTemp;
	CFileDialog dlgFile(FALSE,_T("Image"),_T("*.bmp"),
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			_T("BMP Image(*.bmp)|*.bmp|JPEG(*.jpg)|*.jpg||"));;
	CString tmpPath_Name = _T("");
	wchar_t *tmpfilename = NULL;

	switch((int)para1)
	{
	//Display
	case 0:
		disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
		m_dlgImgRect[cam_idx]->Display();
		break;
	
	//Live on
	case 1:
		SetTimer(0, 100, NULL);
		break;
	//Live off
	case 2:
		KillTimer(0);
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
			return 0;
		}

		disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
		m_dlgImgRect[cam_idx]->Display();
		break;
		//Image save
	case 4:
		//8bit : BMP, JPEG
		if(dlgFile.DoModal() == IDOK)
		{
			tmpPath_Name = dlgFile.GetPathName();
			tmpfilename = tmpPath_Name.GetBuffer(tmpPath_Name.GetLength());
			 
			// BMP or JPG
			strTemp = tmpPath_Name.Right(3);

			if(_T("bmp") == strTemp || _T("BMP") == strTemp)
			{
				//char temp[100];
				sprintf_s(temp, tmpPath_Name.GetLength() + 1, "%S", tmpPath_Name);
				write_image(m_HalImage[cam_idx], HTuple("bmp"), NULL, temp);
			}
			else if(_T("jpg") == strTemp || _T("JPG") == strTemp)
			{
				//char temp[100];
				sprintf_s(temp, tmpPath_Name.GetLength() + 1, "%S", tmpPath_Name);
				write_image(m_HalImage[cam_idx], HTuple("jpeg"), NULL, temp);
			}
			else
			{
			
			}
		}
		//Hal_SaveHalImage(m_VisMod->GetHalImage((int)para0, m_iBufIdx[0], m_iMapIdx[0]), "", "bmp");
		//write_image(m_HalImage[cam_idx],HTuple("bmp"),NULL,HTuple("D:\\aaa.bmp"));
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
		if(m_dlgImgRect[cam_idx]->GetStartPitch())
		{
			switch(m_nZoomNum)
			{
			case 0:
				pixel_size = 0.001683168 * 1;
				break;
			case 1:
				pixel_size = 0.001683168 / 5;
				break;
			case 2:
				pixel_size = 0.001683168 / 10;
				break;
			}

			s_p = m_dlgImgRect[cam_idx]->GetStartPoint();
			e_p = m_dlgImgRect[cam_idx]->GetEndPoint();

			long z,i_w,i_h,sx,sy,ex,ey;

			if(m_dlgImgRect[cam_idx]->GetStartP())
			{
				set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
				set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("fill"));
				disp_rectangle1(m_dlgImgRect[cam_idx]->GetView(), s_p.y - 4, s_p.x - 4, s_p.y + 4, s_p.x + 4);

				set_color(m_dlgImgRect[cam_idx]->GetView(), "red");
				set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
				disp_line(m_dlgImgRect[cam_idx]->GetView(), s_p.y, s_p.x, e_p.y, e_p.x);
				m_dlgImgRect[cam_idx]->GetZoom(&z, &i_w, &i_h, &sx, &sy, &ex, &ey);

				sx = s_p.x;
				sy = s_p.y + (70 / (z + 1));
				w = (e_p.x - s_p.x) * pixel_size;
				h = (e_p.y - s_p.y) * pixel_size;
				set_color(m_dlgImgRect[cam_idx]->GetView(), "red");
				set_tposition(m_dlgImgRect[cam_idx]->GetView(), (Hlong)(sy), (Hlong)(sx));
				sprintf(temp, "width : %.5fmm, height : %.5fmm", abs(w), abs(h));
				write_string(m_dlgImgRect[cam_idx]->GetView(), (HTuple)temp);
			}
			else
			{
				disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
				m_dlgImgRect[cam_idx]->Display();
				set_color(m_dlgImgRect[cam_idx]->GetView(), "blue");
				set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("fill"));
				disp_rectangle1(m_dlgImgRect[cam_idx]->GetView(), e_p.y - 4, e_p.x - 4, e_p.y + 4, e_p.x + 4);
			}
		}
		break;
		//Region confirm
	case 6:
		
		switch(m_nZoomNum)
		{
		case 0:
			pixel_size = 0.001683168 * 1;
			break;
		case 1:
			pixel_size = 0.001683168 / 5;
			break;
		case 2:
			pixel_size = 0.001683168 / 10;
			break;
		}
			
		s_p = m_dlgImgRect[cam_idx]->GetStartPoint();
		e_p = m_dlgImgRect[cam_idx]->GetEndPoint();

		sx = s_p.x;
		sy = e_p.y + 70;
		w = (e_p.x - s_p.x) * pixel_size;
		h = (e_p.y - s_p.y) * pixel_size;
		set_color(m_dlgImgRect[cam_idx]->GetView(), "red");
		set_tposition(m_dlgImgRect[cam_idx]->GetView(), (Hlong)(sy), (Hlong)(sx));
		sprintf(temp, "width : %.5fmm, height : %.5fmm", w, h);
		write_string(m_dlgImgRect[cam_idx]->GetView(), (HTuple)temp);
		break;
	//Test
	case 7:
	
	break;
	//거리 측정
	case 8:
		//SetLine(m_dlgImgRect[cam_idx]->GetActiveGrid());
		if(m_dlgImgRect[cam_idx]->GetStartPitch())
		{
			m_dlgImgRect[cam_idx]->SetStartPitch(FALSE);
			disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
			m_dlgImgRect[cam_idx]->Display();
		}
		else
		{
			m_dlgImgRect[cam_idx]->SetStartPitch(TRUE);
		}
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogReview::OnImageRectMessage)!"));
		break;
	}

	return 0;
}

void CDialogReview::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	double glass_x, glass_y;
	double e2_x, e2_y;
	double robot_x, robot_y;
	double offset_x, offset_y;
	int mark_idx, i;
	mark_idx = 0;//첫번째 마크가지고 좌표변환

	switch(nIDEvent)
	{
	case 0:
#ifdef CAMERA
		Grab(0, TRUE);
#endif
		DrawCross();
		break;
	case 1:
		KillTimer(nIDEvent);

		pdlg->m_AFCtrl->SetReviewLed((unsigned short) m_edit_rv_light);

		break;
	case 100://20160729 ngh
		e2_x = m_edit_dGlass_y;
		e2_y = m_edit_dGlass_x;
		glass_x = e2_x;
		glass_y = e2_y;

		if(e2_x > 0)
		{
			glass_x = -(365 + abs(e2_x));
		}
		else if(e2_x < 0)
		{
			glass_x = -(365 - abs(e2_x));
		}

		//Glass는 X, Y 좌표계(수학)
		offset_x = pdlg->m_Align->GetAlignOffsetPosX_coordXY() - pdlg->m_RecipePanel->GetMarkPosX_coordXY(mark_idx);
		offset_y = pdlg->m_Align->GetAlignOffsetPosY_coordXY() - pdlg->m_RecipePanel->GetMarkPosY_coordXY(mark_idx);
		::GetCoordGlassToRobot(offset_x, offset_y, m_edit_dGlass_x, m_edit_dGlass_y, &robot_y, &robot_x);

		if(pdlg->m_Robot->GetIsMoveComplete(MOTOR_0, robot_y))
		{
			if(pdlg->m_Robot->GetIsInposition(MOTOR_0) && !m_bReview_MoveEnd_X)
			{
				m_bReview_MoveEnd_X = TRUE;
			}
		}

		if(pdlg->m_Robot->GetIsMoveComplete(MOTOR_1, robot_x))
		{
			if(pdlg->m_Robot->GetIsInposition(MOTOR_1) && !m_bReview_MoveEnd_Y)
			{
				m_bReview_MoveEnd_Y = TRUE;
			}
		}

		if(m_bReview_MoveEnd_X && m_bReview_MoveEnd_Y)
		{
			KillTimer(100);
			pdlg->m_Robot->m_bMotionJogDrawCrowsStart = FALSE;
		}
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogReview::OnTimer)!"));
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CDialogReview::ClickBtnenhctrlRvJogSpd1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	m_iJogSpdSelect = 0;
	m_btn_jog_spd0.SetValue(TRUE);
	m_btn_jog_spd1.SetValue(FALSE);
	m_btn_jog_spd2.SetValue(FALSE);
}

void CDialogReview::ClickBtnenhctrlRvJogSpd2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_iJogSpdSelect = 1;
	m_btn_jog_spd0.SetValue(FALSE);
	m_btn_jog_spd1.SetValue(TRUE);
	m_btn_jog_spd2.SetValue(FALSE);
}


void CDialogReview::ClickBtnenhctrlRvJogSpd3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_iJogSpdSelect = 2;
	m_btn_jog_spd0.SetValue(FALSE);
	m_btn_jog_spd1.SetValue(FALSE);
	m_btn_jog_spd2.SetValue(TRUE);
}

void CDialogReview::MouseDownBtnenhctrlRvJogL(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_0, DIR_NEGATIVE, m_iJogSpdSelect);
}


void CDialogReview::MouseUpBtnenhctrlRvJogL(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_0);
}

void CDialogReview::MouseDownBtnenhctrlRvJogR(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_0, DIR_POSITIVE, m_iJogSpdSelect);
}


void CDialogReview::MouseUpBtnenhctrlRvJogR(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_0);
}


void CDialogReview::MouseDownBtnenhctrlRvJogU(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_1, DIR_POSITIVE, m_iJogSpdSelect);
}


void CDialogReview::MouseUpBtnenhctrlRvJogU(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_1);
}


void CDialogReview::MouseDownBtnenhctrlRvJogD(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_1, DIR_NEGATIVE, m_iJogSpdSelect);
}


void CDialogReview::MouseUpBtnenhctrlRvJogD(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_1);
}


void CDialogReview::MouseDownBtnenhctrlRvJogCcw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_2, DIR_POSITIVE, m_iJogSpdSelect);
}


void CDialogReview::MouseUpBtnenhctrlRvJogCcw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_2);
}


void CDialogReview::MouseDownBtnenhctrlRvJogCw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_2, DIR_NEGATIVE, m_iJogSpdSelect);
}


void CDialogReview::MouseUpBtnenhctrlRvJogCw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_2);
}


void CDialogReview::DrawCross()
{
	int cam_idx = 0;
	set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
	set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
	disp_line(m_dlgImgRect[cam_idx]->GetView(), REVIEW_IMAGE_HEIGHT * 0.5, 0, REVIEW_IMAGE_HEIGHT * 0.5, REVIEW_IMAGE_WIDTH);
	disp_line(m_dlgImgRect[cam_idx]->GetView(), 0, REVIEW_IMAGE_WIDTH * 0.5,REVIEW_IMAGE_HEIGHT, REVIEW_IMAGE_WIDTH * 0.5);
}

void CDialogReview::SetDrawCross(int x, int y)
{
	int cam_idx = 0;
	set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
	set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
	disp_line(m_dlgImgRect[cam_idx]->GetView(), y, 0, y, REVIEW_IMAGE_WIDTH);
	disp_line(m_dlgImgRect[cam_idx]->GetView(), 0, x,REVIEW_IMAGE_HEIGHT, x);
}

//Move cell position
//X, Y좌표계
void CDialogReview::ClickBtnenhctrlRvCellMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	double x, y;
	int cam_idx = 0;

	UpdateData(TRUE);

	//Cell limit조건
	if(1 > m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = 1;
	}
	if((pdlg->m_RecipePanel->GetCellCntX_coordXY() + 1) < m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = pdlg->m_RecipePanel->GetCellCntX() + 1;
	}
	if(1 > m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = 1;
	}
	if((pdlg->m_RecipePanel->GetCellCntY_coordXY() + 1) < m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = pdlg->m_RecipePanel->GetCellCntY() + 1;
	}
	if(GLASS_MAX_CELL_NUM <= (pdlg->m_RecipePanel->GetCellCntX() * pdlg->m_RecipePanel->GetCellCntY()) )
	{
		//Error log

		return;
	}

	//X(열), Y(행)
	y = pdlg->m_Glass->GetCellRobotPosX_coordXY(m_edit_rv_cell_row, m_edit_rv_cell_col);
	x = pdlg->m_Glass->GetCellRobotPosY_coordXY(m_edit_rv_cell_row, m_edit_rv_cell_col);

	pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, x);
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, y);

	disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
	pdlg->m_Glass->SetDispGlassDat();
	pdlg->m_Glass->SetDrawCrossRobot(y, x);
}

void CDialogReview::ClickBtnenhctrlRvCellLf()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;	
	double x, y;
	int cam_idx;
	cam_idx = 0;

	UpdateData(TRUE);

	//열
	m_edit_rv_cell_col++;

	//Cell limit조건
	if(1 > m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = 1;
	}
	if(pdlg->m_RecipePanel->GetCellCntX_coordXY() < m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = pdlg->m_RecipePanel->GetCellCntX_coordXY();
	}
	if(1 > m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = 1;
	}
	if(pdlg->m_RecipePanel->GetCellCntY_coordXY() < m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = pdlg->m_RecipePanel->GetCellCntY_coordXY();
	}
	if(GLASS_MAX_CELL_NUM <= (pdlg->m_RecipePanel->GetCellCntX() * pdlg->m_RecipePanel->GetCellCntY()) )
	{
		//Error log

		return;
	}

	if(m_check_rv_cell_nevi_move)
	{
		//Move
		//X(열), Y(행)
		y = pdlg->m_Glass->GetCellRobotPosX_coordXY(m_edit_rv_cell_row, m_edit_rv_cell_col);
		x = pdlg->m_Glass->GetCellRobotPosY_coordXY(m_edit_rv_cell_row, m_edit_rv_cell_col);

		pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, x);
		pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, y);

		disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
		pdlg->m_Glass->SetDispGlassDat();
		pdlg->m_Glass->SetDrawCrossRobot(y, x);
	}

	UpdateData(FALSE);
}


void CDialogReview::ClickBtnenhctrlRvCellRt()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;	
	double x, y;	
	int cam_idx;
	cam_idx = 0;

	UpdateData(TRUE);

	m_edit_rv_cell_col--;

	//Cell limit조건
	if(1 > m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = 1;
	}
	if(pdlg->m_RecipePanel->GetCellCntX_coordXY() < m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = pdlg->m_RecipePanel->GetCellCntX_coordXY();
	}
	if(1 > m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = 1;
	}
	if(pdlg->m_RecipePanel->GetCellCntY_coordXY() < m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = pdlg->m_RecipePanel->GetCellCntY_coordXY();
	}
	if(GLASS_MAX_CELL_NUM <= (pdlg->m_RecipePanel->GetCellCntX() * pdlg->m_RecipePanel->GetCellCntY()) )
	{
		//Error log

		return;
	}

	if(m_check_rv_cell_nevi_move)
	{
		//Move
		//X(열), Y(행)
		y = pdlg->m_Glass->GetCellRobotPosX_coordXY(m_edit_rv_cell_row, m_edit_rv_cell_col);
		x = pdlg->m_Glass->GetCellRobotPosY_coordXY(m_edit_rv_cell_row, m_edit_rv_cell_col);

		pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, x);
		pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, y);

		disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
		pdlg->m_Glass->SetDispGlassDat();
		pdlg->m_Glass->SetDrawCrossRobot(y, x);
	}

	UpdateData(FALSE);

}


void CDialogReview::ClickBtnenhctrlRvCellU()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;	
	double x, y;
	int cam_idx;
	cam_idx = 0;

	UpdateData(TRUE);

	//행
	m_edit_rv_cell_row++;

	//Cell limit조건
	if(1 > m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = 1;
	}
	if(pdlg->m_RecipePanel->GetCellCntX_coordXY() < m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = pdlg->m_RecipePanel->GetCellCntX_coordXY();
	}
	if(1 > m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = 1;
	}
	if(pdlg->m_RecipePanel->GetCellCntY_coordXY() < m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = pdlg->m_RecipePanel->GetCellCntY_coordXY();
	}
	if(GLASS_MAX_CELL_NUM <= (pdlg->m_RecipePanel->GetCellCntX() * pdlg->m_RecipePanel->GetCellCntY()) )
	{
		//Error log

		return;
	}

	if(m_check_rv_cell_nevi_move)
	{
		//Move
		//X(열), Y(행)
		y = pdlg->m_Glass->GetCellRobotPosX_coordXY(m_edit_rv_cell_row, m_edit_rv_cell_col);
		x = pdlg->m_Glass->GetCellRobotPosY_coordXY(m_edit_rv_cell_row, m_edit_rv_cell_col);

		pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, x);
		pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, y);

		disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
		pdlg->m_Glass->SetDispGlassDat();
		pdlg->m_Glass->SetDrawCrossRobot(y, x);
	}

	UpdateData(FALSE);

}


void CDialogReview::ClickBtnenhctrlRvCellDn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;	
	double x, y;	
	int cam_idx;
	cam_idx = 0;

	UpdateData(TRUE);

	//행
	m_edit_rv_cell_row--;

	//Cell limit조건
	if(1 > m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = 1;
	}
	if(pdlg->m_RecipePanel->GetCellCntX_coordXY() < m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = pdlg->m_RecipePanel->GetCellCntX_coordXY();
	}
	if(1 > m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = 1;
	}
	if(pdlg->m_RecipePanel->GetCellCntY_coordXY() < m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = pdlg->m_RecipePanel->GetCellCntY_coordXY();
	}
	if(GLASS_MAX_CELL_NUM <= (pdlg->m_RecipePanel->GetCellCntX() * pdlg->m_RecipePanel->GetCellCntY()) )
	{
		//Error log

		return;
	}

	if(m_check_rv_cell_nevi_move)
	{
		//Move
		//X(열), Y(행)
		y = pdlg->m_Glass->GetCellRobotPosX_coordXY(m_edit_rv_cell_row, m_edit_rv_cell_col);
		x = pdlg->m_Glass->GetCellRobotPosY_coordXY(m_edit_rv_cell_row, m_edit_rv_cell_col);

		pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, x);
		pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, y);

		disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
		pdlg->m_Glass->SetDispGlassDat();
		pdlg->m_Glass->SetDrawCrossRobot(y, x);
	}

	UpdateData(FALSE);
}

void CDialogReview::SetLine(BOOL isView)
{
	int img_idx = 0;
	if(isView)
	{
		m_dlgImgRect[img_idx]->DeleteAll();
		m_dlgImgRect[img_idx]->SetActiveTracker(TRUE);	
		m_dlgImgRect[img_idx]->AddTracker(1, 100, 100, 200, 200, RGB(255, 0, 0), "LineSize");
	}
	else
	{
		m_dlgImgRect[img_idx]->DeleteAll();
		m_dlgImgRect[img_idx]->SetActiveTracker(FALSE);
	}
	m_dlgImgRect[img_idx]->Display();

	disp_obj(m_HalImage[img_idx], m_dlgImgRect[img_idx]->GetView());
}

void CDialogReview::SetFilmAlignRect(BOOL type)
{
	int img_idx;
	img_idx = 0;
	if(type)
	{
		//Start
		m_dlgImgRect[img_idx]->DeleteAll();
		m_dlgImgRect[img_idx]->SetActiveTracker(TRUE);

	}
	else
	{
		//End
		m_dlgImgRect[img_idx]->DeleteAll();
		m_dlgImgRect[img_idx]->SetActiveTracker(FALSE);
	}

	disp_obj(m_HalImage[img_idx], m_dlgImgRect[img_idx]->GetView());
}

void CDialogReview::SetFilmAlignRectShow(BOOL type)
{
	int img_idx;
	img_idx = 0;
	if(type)
	{
		//Start
		m_dlgImgRect[img_idx]->SetActiveTracker(TRUE);

	}
	else
	{
		//End
		m_dlgImgRect[img_idx]->SetActiveTracker(FALSE);
	}

	disp_obj(m_HalImage[img_idx], m_dlgImgRect[img_idx]->GetView());
}

void CDialogReview::SetFilmAlignRect(int cell_x_idx, int cell_y_idx, int film_idx, int rgn_idx, int sub_rgn_idx, RECT rect)
{
	char temp[100];
	int img_idx;
	img_idx = 0;

	switch(sub_rgn_idx)
	{
	case 0:
		sprintf(temp, "FILM ALIGN (%d,%d)-%d-LR", cell_x_idx + 1, cell_y_idx + 1, rgn_idx + 1);
		break;
	case 1:
		sprintf(temp, "FILM ALIGN (%d,%d)-%d-UD", cell_x_idx + 1, cell_y_idx + 1, rgn_idx + 1);
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogReview::SetFilmAlignRect)!"));
		break;
	}

	m_dlgImgRect[img_idx]->AddTracker(0, rect.left, rect.top, rect.right, rect.bottom, RGB(0, 0, 255), temp);
	m_dlgImgRect[img_idx]->Display();
}

void CDialogReview::GetFilmAlignRect(int film_idx, int rgn_idx, int sub_rgn_idx, RECT *rect)
{
	int img_idx;
	img_idx = 0;

	m_dlgImgRect[img_idx]->GetTracker(sub_rgn_idx, &rect->left, &rect->top, &rect->right, &rect->bottom);
}



void CDialogReview::ClickBtnenhctrlRvZoom1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	UpdateData(TRUE);
	m_nZoomNum = 0;

	m_btn_rv_zoom_1.SetValue(TRUE);
	m_btn_rv_zoom_2.SetValue(FALSE);
	m_btn_rv_zoom_3.SetValue(FALSE);

	int iRet = 0;
	iRet = pdlg->m_AFCtrl->SetAfLlc(0);
}

void CDialogReview::ClickBtnenhctrlRvZoomMag2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	UpdateData(TRUE);
	m_nZoomNum = 1;

	m_btn_rv_zoom_1.SetValue(FALSE);
	m_btn_rv_zoom_2.SetValue(TRUE);
	m_btn_rv_zoom_3.SetValue(FALSE);

	int iRet = 0;
	iRet = pdlg->m_AFCtrl->SetAfLlc(1);
}


void CDialogReview::ClickBtnenhctrlRvZoomMag3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);
	m_nZoomNum = 2;

	m_btn_rv_zoom_1.SetValue(FALSE);
	m_btn_rv_zoom_2.SetValue(FALSE);
	m_btn_rv_zoom_3.SetValue(TRUE);

	int iRet = 0;
	iRet = pdlg->m_AFCtrl->SetAfLlc(2);
}


//void CDialogReview::MouseDownBtnenhctrlRvAfUp(short Button, short Shift, long x, long y)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
//
//	UpdateData(TRUE);
//
//	pdlg->m_AFCtrl->SetAfMove(FALSE);
//}


//void CDialogReview::MouseDownBtnenhctrlRvAfDn(short Button, short Shift, long x, long y)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
//
//	UpdateData(TRUE);
//
//	pdlg->m_AFCtrl->SetAfMove(TRUE);
//}

Hlong CDialogReview::GetView()
{
	int cam_idx = 0;
	return m_dlgImgRect[cam_idx]->GetView();
}

Hobject * CDialogReview::GetImage()
{
	int cam_idx = 0;
	return &m_HalImage[cam_idx];
}


void CDialogReview::SetReviewLight()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	unsigned short LedPwm = 0;

	m_edit_rv_light = m_slider_rv_light.GetPos();
	
	LedPwm = m_edit_rv_light;

	pdlg->m_AFCtrl->SetReviewLed(LedPwm);
	UpdateData(FALSE);

	if(LedPwm > 0)
	{
		pdlg->m_Light->SetLightTime(4, TRUE);
	}
	else
	{
		pdlg->m_Light->SetLightTime(4, FALSE);
	}
}

void CDialogReview::ClickBtnenhctrlRvAfUse2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);

	if(m_btn_light_use.GetValue())
	{
		if((m_edit_rv_light < 0) || (m_edit_rv_light > 100))
		{ 
			::AfxMessageBox(_T("Light value 0~100"));
			m_edit_rv_light = 0;
		}
		pdlg->m_AFCtrl->SetReviewLed((unsigned short) m_edit_rv_light);

		m_slider_rv_light.SetPos(m_edit_rv_light);

		if(m_edit_rv_light > 0)
		{
			pdlg->m_Light->SetLightTime(4, TRUE);
		}
		else
		{
			pdlg->m_Light->SetLightTime(4, FALSE);
		}
	}
	else
	{
		m_edit_rv_light_old = m_edit_rv_light;
		pdlg->m_AFCtrl->SetReviewLed((unsigned short) 0);

		if(m_edit_rv_light_old > 0)
		{
			pdlg->m_Light->SetLightTime(4, TRUE);
		}
		else
		{
			pdlg->m_Light->SetLightTime(4, FALSE);
		}
	}

	
}


void CDialogReview::OnNMCustomdrawSliderRvLight(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	unsigned short LedPwm = 0;

	m_edit_rv_light = m_slider_rv_light.GetPos();
	
	LedPwm = m_edit_rv_light;

	pdlg->m_AFCtrl->SetReviewLed(LedPwm);

	if(LedPwm > 0)
	{
		pdlg->m_Light->SetLightTime(4, TRUE);
	}
	else
	{
		pdlg->m_Light->SetLightTime(4, FALSE);
	}
	UpdateData(FALSE);

	*pResult = 0;
}

BOOL CDialogReview::LoadReview(CString path)
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


	file_path = path + _T("\\Review.par");

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
		key.Format(_T("m_edit_rv_light"));
		GetPrivateProfileString(_T("REVIEW"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_rv_light = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_offset_x"));
		GetPrivateProfileString(_T("REVIEW"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_offset_x = atof(ctemp);

		key.Empty();
		key.Format(_T("m_edit_offset_y"));
		GetPrivateProfileString(_T("REVIEW"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_offset_y = atof(ctemp);
		////////////////////////////////////////////////////////////////////////////
	}
	fd=0;

	return rslt;
}

BOOL CDialogReview::SaveReview(CString path)								
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\Review.par");

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
		key.Format(_T("m_edit_rv_light"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_rv_light);
		WritePrivateProfileString(_T("REVIEW"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_offset_x"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_offset_x);
		WritePrivateProfileString(_T("REVIEW"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_offset_y"));
		dat.Empty();
		dat.Format(_T("%f"), m_edit_offset_y);
		WritePrivateProfileString(_T("REVIEW"), key, dat, file_path);
	}
	fd=0;
	
	return rslt;
}

void CDialogReview::OnEnChangeEditRvLight()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	m_slider_rv_light.SetPos(m_edit_rv_light);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDialogReview::ClickBtnenhctrlRvAfUse()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	BOOL bRet = FALSE;

	bRet = m_btn_rv_af_use.GetValue();
	if(bRet)
	{
		bRet = pdlg->m_AFCtrl->SetAFTrackRun(TRUE);
		/*if(bRet)
		{
			SetTimer(2,1000,NULL);
		}*/
	}
	else
	{
		pdlg->m_AFCtrl->SetAFTrackRun(FALSE);
	}
}


void CDialogReview::OnCbnSelchangeComboRvAfStep()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIndex;
	CString Ch_type;
	Ch_type.Empty();


	nIndex = m_combo_rv_af_step.GetCurSel();

	m_combo_rv_af_step.GetLBText(nIndex,Ch_type);

	SetDlgItemText(IDC_COMBO_RV_AF_STEP,Ch_type);

	if(Ch_type == (_T("16")))
	{
		m_iAFJogStep = 16;
	}
	else if(Ch_type == (_T("50")))
	{
		m_iAFJogStep = 50;
	}
	else if(Ch_type == (_T("100")))
	{
		m_iAFJogStep = 100;
	}
	else if(Ch_type == (_T("200")))
	{
		m_iAFJogStep = 200;
	}
	else if(Ch_type == (_T("500")))
	{
		m_iAFJogStep = 500;
	}
	else
		return;
}


void CDialogReview::ClickBtnenhctrlRvCameraScanImage()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	if(m_btn_check_ccd.GetValue())
	{
		pdlg->ReviewCCD();
	}
	else
	{
		pdlg->m_CCD->ShowWindow(SW_HIDE);
	}

}

void CDialogReview::SetGrab()
{
	KillTimer(0);
	Grab(0, TRUE);
	Grab(0, TRUE);
}

void CDialogReview::StopLive()
{
	KillTimer(0);
}

void CDialogReview::DrawCeterLine()
{
	DrawCross();
}

void CDialogReview::ClickBtnenhctrlRvSet()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

double CDialogReview::GetOffsetX()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	return m_edit_offset_x;
}

double CDialogReview::GetOffsetY()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	return m_edit_offset_y;
}


void CDialogReview::MouseDownBtnenhctrlRvAfUp(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_AFCtrl->SetAfMove(TRUE,m_iAFJogStep);
}


void CDialogReview::MouseUpBtnenhctrlRvAfUp(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

//	pdlg->m_AFCtrl->SetAfMove(FALSE,m_iAFJogStep);
}


void CDialogReview::MouseDownBtnenhctrlRvAfDn(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_AFCtrl->SetAfMove(FALSE,m_iAFJogStep);
}


void CDialogReview::MouseUpBtnenhctrlRvAfDn(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

//	pdlg->m_AFCtrl->SetAfMove(FALSE,m_iAFJogStep);
}


void CDialogReview::MouseDownBtnenhctrlRvJogLu(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_0, DIR_NEGATIVE, m_iJogSpdSelect);
	pdlg->m_Robot->SetRobotJogMove(MOTOR_1, DIR_POSITIVE, m_iJogSpdSelect);
}

void CDialogReview::MouseUpBtnenhctrlRvJogLu(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_0);
	pdlg->m_Robot->SetRobotJogStop(MOTOR_1);
}

void CDialogReview::MouseDownBtnenhctrlRvJogRu(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_0, DIR_POSITIVE, m_iJogSpdSelect);
	pdlg->m_Robot->SetRobotJogMove(MOTOR_1, DIR_POSITIVE, m_iJogSpdSelect);
}


void CDialogReview::MouseUpBtnenhctrlRvJogRu(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_0);
	pdlg->m_Robot->SetRobotJogStop(MOTOR_1);
}


void CDialogReview::MouseDownBtnenhctrlRvJogDirld(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_0, DIR_NEGATIVE, m_iJogSpdSelect);
	pdlg->m_Robot->SetRobotJogMove(MOTOR_1, DIR_NEGATIVE, m_iJogSpdSelect);
}


void CDialogReview::MouseUpBtnenhctrlRvJogDirld(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_0);
	pdlg->m_Robot->SetRobotJogStop(MOTOR_1);
}


void CDialogReview::MouseDownBtnenhctrlRvJogRd(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_0, DIR_POSITIVE, m_iJogSpdSelect);
	pdlg->m_Robot->SetRobotJogMove(MOTOR_1, DIR_NEGATIVE, m_iJogSpdSelect);
}


void CDialogReview::MouseUpBtnenhctrlRvJogRd(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_Robot->SetRobotJogStop(MOTOR_0);
	pdlg->m_Robot->SetRobotJogStop(MOTOR_1);
}


void CDialogReview::ClickBtnenhctrlRvJogKbd()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CDialogReview::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//20160729 ngh
void CDialogReview::ClickBtnenhctrlRvMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	double glass_x, glass_y;
	double e2_x, e2_y;
	double robot_x, robot_y;
	double offset_x, offset_y;
	int mark_idx, i;
	mark_idx = 0;//첫번째 마크가지고 좌표변환

	e2_x = m_edit_dGlass_y;
	e2_y = m_edit_dGlass_x;
	glass_x = e2_x;
	glass_y = e2_y;


	if(e2_x > 0)
	{
		glass_x = -(365 + abs(e2_x));
	}
	else if(e2_x < 0)
	{
		glass_x = -(365 - abs(e2_x));
	}

	//Glass는 X, Y 좌표계(수학)
	offset_x = pdlg->m_Align->GetAlignOffsetPosX_coordXY() - pdlg->m_RecipePanel->GetMarkPosX_coordXY(mark_idx);
	offset_y = pdlg->m_Align->GetAlignOffsetPosY_coordXY() - pdlg->m_RecipePanel->GetMarkPosY_coordXY(mark_idx);
	
	::GetCoordGlassToRobot(offset_x, offset_y, glass_x, glass_y, &robot_y, &robot_x);

	m_bReview_MoveEnd_X = FALSE;
	m_bReview_MoveEnd_Y = FALSE;

	//Move
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, robot_x + GetOffsetY());
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, robot_y + GetOffsetX());

	pdlg->m_Robot->m_bMotionJogDrawCrowsStart = TRUE;
	SetTimer(100, 100, NULL);
}


void CDialogReview::ClickBtnenhctrlRvStop()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	KillTimer(100);
	pdlg->m_Robot->m_bMotionJogDrawCrowsStart = FALSE;
	pdlg->m_Robot->SetMotorStop(MOTOR_0);
	pdlg->m_Robot->SetMotorStop(MOTOR_1);
}
