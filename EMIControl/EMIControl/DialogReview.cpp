// Dialog/DialogReview.cpp : 구현 파일입니다.
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


// CDialogReview 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogReview, CDialogEx)

CDialogReview::CDialogReview(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogReview::IDD, pParent)
	, m_edit_rv_cell_row(1)
	, m_edit_rv_cell_col(1)
	, m_check_rv_cell_nevi_move(FALSE)
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
}

CDialogReview::~CDialogReview()
{
	int i;

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
}


BEGIN_MESSAGE_MAP(CDialogReview, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(USER_MSG_IMG_RECT, OnImageRectMessage)
	ON_WM_TIMER()
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
			m_dlgImgRect[i]->SetCamIndex(0, _T("                                    REVEIW CAMERA"), this->m_hWnd);
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
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_AF_UP, DISPID_MOUSEDOWN, CDialogReview::MouseDownBtnenhctrlRvAfUp, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDialogReview, IDC_BTNENHCTRL_RV_AF_DN, DISPID_MOUSEDOWN, CDialogReview::MouseDownBtnenhctrlRvAfDn, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
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
		//Hal_SaveHalImage(m_VisMod->GetHalImage((int)para0, m_iBufIdx[0], m_iMapIdx[0]), "", "bmp");
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
		AfxMessageBox(_T("Error : invalid index(CDialogReview::OnImageRectMessage)!"));
		break;
	}

	return 0;
}

void CDialogReview::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case 0:
		Grab(0, TRUE);
		DrawCross();
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
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_Y, DIR_POSITIVE, m_iJogSpdSelect);
}


void CDialogReview::MouseUpBtnenhctrlRvJogL(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();

	pdlg->m_Robot->SetRobotJogStop(MOTOR_Y);
}

void CDialogReview::MouseDownBtnenhctrlRvJogR(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_Y, DIR_NEGATIVE, m_iJogSpdSelect);
}


void CDialogReview::MouseUpBtnenhctrlRvJogR(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();

	pdlg->m_Robot->SetRobotJogStop(MOTOR_Y);
}


void CDialogReview::MouseDownBtnenhctrlRvJogU(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_X, DIR_POSITIVE, m_iJogSpdSelect);
}


void CDialogReview::MouseUpBtnenhctrlRvJogU(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();

	pdlg->m_Robot->SetRobotJogStop(MOTOR_X);
}


void CDialogReview::MouseDownBtnenhctrlRvJogD(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_X, DIR_NEGATIVE, m_iJogSpdSelect);
}


void CDialogReview::MouseUpBtnenhctrlRvJogD(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();

	pdlg->m_Robot->SetRobotJogStop(MOTOR_X);
}


void CDialogReview::MouseDownBtnenhctrlRvJogCcw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_T, DIR_POSITIVE, m_iJogSpdSelect);
}


void CDialogReview::MouseUpBtnenhctrlRvJogCcw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();

	pdlg->m_Robot->SetRobotJogStop(MOTOR_T);
}


void CDialogReview::MouseDownBtnenhctrlRvJogCw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();

	UpdateData(TRUE);

	pdlg->m_Robot->SetRobotJogMove(MOTOR_T, DIR_NEGATIVE, m_iJogSpdSelect);
}


void CDialogReview::MouseUpBtnenhctrlRvJogCw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();

	pdlg->m_Robot->SetRobotJogStop(MOTOR_T);
}


void CDialogReview::DrawCross()
{
	int cam_idx = 0;
	set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
	disp_line(m_dlgImgRect[cam_idx]->GetView(), REVIEW_IMAGE_HEIGHT * 0.5, 0, REVIEW_IMAGE_HEIGHT * 0.5, REVIEW_IMAGE_WIDTH);
	disp_line(m_dlgImgRect[cam_idx]->GetView(), 0, REVIEW_IMAGE_WIDTH * 0.5,REVIEW_IMAGE_HEIGHT, REVIEW_IMAGE_WIDTH * 0.5);
}

void CDialogReview::SetDrawCross(int x, int y)
{
	int cam_idx = 0;
	set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
	disp_line(m_dlgImgRect[cam_idx]->GetView(), y, 0, y, REVIEW_IMAGE_WIDTH);
	disp_line(m_dlgImgRect[cam_idx]->GetView(), 0, x,REVIEW_IMAGE_HEIGHT, x);
}

//Move cell position
//X, Y좌표계
void CDialogReview::ClickBtnenhctrlRvCellMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();

	double x, y;

	UpdateData(TRUE);

	//Cell limit조건
	if(1 > m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = 1;
	}
	if((pdlg->m_RecipePanel->GetCellCntX_glass() + 1) < m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = pdlg->m_RecipePanel->GetCellCntX() + 1;
	}
	if(1 > m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = 1;
	}
	if((pdlg->m_RecipePanel->GetCellCntY_glass() + 1) < m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = pdlg->m_RecipePanel->GetCellCntY() + 1;
	}
	if(GLASS_MAX_CELL_NUM <= (pdlg->m_RecipePanel->GetCellCntX() * pdlg->m_RecipePanel->GetCellCntY()) )
	{
		//Error log

		return;
	}

	//X(열), Y(행)
	x = pdlg->m_Glass->GetCellRobotPosX(m_edit_rv_cell_row, m_edit_rv_cell_col);
	y = pdlg->m_Glass->GetCellRobotPosY(m_edit_rv_cell_row, m_edit_rv_cell_col);

	pdlg->m_Robot->SetRobotMove_mm(MOTOR_X, x);
	pdlg->m_Robot->SetRobotMove_mm(MOTOR_Y, y);

	pdlg->m_Glass->SetDrawCrossRobot(x, y);
}

void CDialogReview::ClickBtnenhctrlRvCellLf()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();	
	
	//열
	m_edit_rv_cell_col++;

	//Cell limit조건
	if(1 > m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = 1;
	}
	if((pdlg->m_RecipePanel->GetCellCntX_glass() + 1) < m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = pdlg->m_RecipePanel->GetCellCntX() + 1;
	}
	if(1 > m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = 1;
	}
	if((pdlg->m_RecipePanel->GetCellCntY_glass() + 1) < m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = pdlg->m_RecipePanel->GetCellCntY() + 1;
	}
	if(GLASS_MAX_CELL_NUM <= (pdlg->m_RecipePanel->GetCellCntX() * pdlg->m_RecipePanel->GetCellCntY()) )
	{
		//Error log

		return;
	}

	if(m_check_rv_cell_nevi_move)
	{
		//Move

	}

	UpdateData(FALSE);
}


void CDialogReview::ClickBtnenhctrlRvCellRt()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();	
	

	m_edit_rv_cell_col--;

	//Cell limit조건
	if(1 > m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = 1;
	}
	if((pdlg->m_RecipePanel->GetCellCntX_glass() + 1) < m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = pdlg->m_RecipePanel->GetCellCntX() + 1;
	}
	if(1 > m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = 1;
	}
	if((pdlg->m_RecipePanel->GetCellCntY_glass() + 1) < m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = pdlg->m_RecipePanel->GetCellCntY() + 1;
	}
	if(GLASS_MAX_CELL_NUM <= (pdlg->m_RecipePanel->GetCellCntX() * pdlg->m_RecipePanel->GetCellCntY()) )
	{
		//Error log

		return;
	}

	if(m_check_rv_cell_nevi_move)
	{
		//Move

	}

	UpdateData(FALSE);

}


void CDialogReview::ClickBtnenhctrlRvCellU()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();	
	
	//행
	m_edit_rv_cell_row++;

	//Cell limit조건
	if(1 > m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = 1;
	}
	if((pdlg->m_RecipePanel->GetCellCntX_glass() + 1) < m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = pdlg->m_RecipePanel->GetCellCntX() + 1;
	}
	if(1 > m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = 1;
	}
	if((pdlg->m_RecipePanel->GetCellCntY_glass() + 1) < m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = pdlg->m_RecipePanel->GetCellCntY() + 1;
	}
	if(GLASS_MAX_CELL_NUM <= (pdlg->m_RecipePanel->GetCellCntX() * pdlg->m_RecipePanel->GetCellCntY()) )
	{
		//Error log

		return;
	}

	if(m_check_rv_cell_nevi_move)
	{
		//Move

	}

	UpdateData(FALSE);

}


void CDialogReview::ClickBtnenhctrlRvCellDn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();	
	
	//행
	m_edit_rv_cell_row--;

	//Cell limit조건
	if(1 > m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = 1;
	}
	if((pdlg->m_RecipePanel->GetCellCntX_glass() + 1) < m_edit_rv_cell_col)
	{
		m_edit_rv_cell_col = pdlg->m_RecipePanel->GetCellCntX() + 1;
	}
	if(1 > m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = 1;
	}
	if((pdlg->m_RecipePanel->GetCellCntY_glass() + 1) < m_edit_rv_cell_row)
	{
		m_edit_rv_cell_row = pdlg->m_RecipePanel->GetCellCntY() + 1;
	}
	if(GLASS_MAX_CELL_NUM <= (pdlg->m_RecipePanel->GetCellCntX() * pdlg->m_RecipePanel->GetCellCntY()) )
	{
		//Error log

		return;
	}

	if(m_check_rv_cell_nevi_move)
	{
		//Move

	}

	UpdateData(FALSE);
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
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();
	
	UpdateData(TRUE);
	int iRet = 0;
	iRet = pdlg->m_AFCtrl->SetAfLlc(0);
	

}

void CDialogReview::ClickBtnenhctrlRvZoomMag2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();
	
	UpdateData(TRUE);
	int iRet = 0;
	iRet = pdlg->m_AFCtrl->SetAfLlc(1);
	
}


void CDialogReview::ClickBtnenhctrlRvZoomMag3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();

	UpdateData(TRUE);
	int iRet = 0;
	iRet = pdlg->m_AFCtrl->SetAfLlc(2);

}


void CDialogReview::MouseDownBtnenhctrlRvAfUp(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();

	UpdateData(TRUE);

	pdlg->m_AFCtrl->SetAfMove(FALSE);
}


void CDialogReview::MouseDownBtnenhctrlRvAfDn(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)GetParent();

	UpdateData(TRUE);

	pdlg->m_AFCtrl->SetAfMove(TRUE);
}

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


