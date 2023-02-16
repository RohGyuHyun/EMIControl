// Dialog/DialogCCD.cpp : 구현 파일입니다.
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
#include "SerialPort.h"


#include "afxdialogex.h"


// CDialogCCD 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogCCD, CDialogEx)

CDialogCCD::CDialogCCD(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogCCD::IDD, pParent)
	, m_edit_ccd_x(0)
	, m_edit_ccd_y(0)
	, m_edit_ccd_width(0)
	, m_edit_ccd_height(0)
	, m_edit_ccd_area(0)
	, m_edit_strCellID(_T(""))
{

}

CDialogCCD::~CDialogCCD()
{
}

void CDialogCCD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CCD_X, m_edit_ccd_x);
	DDX_Text(pDX, IDC_EDIT_CCD_Y, m_edit_ccd_y);
	DDX_Text(pDX, IDC_EDIT_CCD_WIDTH, m_edit_ccd_width);
	DDX_Text(pDX, IDC_EDIT_CCD_HEIGHT, m_edit_ccd_height);
	DDX_Text(pDX, IDC_EDIT_CCD_AREA, m_edit_ccd_area);
	DDX_Text(pDX, IDC_EDIT_CCD_CELL_ID, m_edit_strCellID);
}


BEGIN_MESSAGE_MAP(CDialogCCD, CDialogEx)
	ON_WM_SHOWWINDOW()
//	ON_WM_MOVE()
//	ON_WM_PAINT()
//	ON_WM_MOVING()
END_MESSAGE_MAP()


// CDialogCCD 메시지 처리기입니다.


void CDialogCCD::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int i;
	CRect rect;

	if(bShow)
	{
		for(i = 0; i < CCD_IMG_RECT_MAX_NUM; i++)
		{

			GetDlgItem(IDC_STATIC_CCD)->GetClientRect(rect);
			GetDlgItem(IDC_STATIC_CCD)->ClientToScreen(rect);

			m_dlgImgRect[i]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);

			m_dlgImgRect[i]->ShowWindow(SW_SHOW);
		}

		UpdateData(FALSE);
	}
	else
	{
		for(int i = 0; i < CCD_IMG_RECT_MAX_NUM; i++)
		{
			m_dlgImgRect[i]->ShowWindow(SW_HIDE);
		}
	}
}


BOOL CDialogCCD::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Herror herr;	
	CRect rect;
	int i;
	HTuple wd, ht;
	long width, height;
	
	for(i = 0; i < CCD_IMG_RECT_MAX_NUM; i++)
	{
		m_dlgImgRect[i] = new C_CtrlImageRect;
		m_dlgImgRect[i]->Create(IMAGE_RECT_DIALOG, this);

		switch(i)
		{
		case 0:
			GetDlgItem(IDC_STATIC_CCD)->GetClientRect(rect);
			GetDlgItem(IDC_STATIC_CCD)->ClientToScreen(rect);

			m_dlgImgRect[i]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);
			m_dlgImgRect[i]->SetCamIndex(0, _T("                                             CCD IMAGE"), this->m_hWnd);
			break;
		case 1:
		default:
			AfxMessageBox(_T("Error : invalid index(CDialogCCD::OnInitDialog())!"));
			break;
		}

		m_dlgImgRect[i]->ShowWindow(SW_SHOW);

	}

	//Halcon image initialize
	for(i = 0; i < CCD_IMG_RECT_MAX_NUM; i++)
	{
		gen_image_const(&m_HalImage[i], HTuple("byte"), CCD_IMAGE_WIDTH, CCD_IMAGE_HEIGHT);

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
		m_dlgImgRect[i]->SetActiveGetPixel(TRUE);

		set_font(m_dlgImgRect[i]->GetView(), "-Arial-12-*-*-*-*-*-1-");
		set_draw(m_dlgImgRect[i]->GetView(), HTuple("margin"));

		disp_obj(m_HalImage[i], m_dlgImgRect[i]->GetView());	

		m_dlgImgRect[i]->ShowWindow(SW_HIDE);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogCCD::SetReadImage(char *path, double x, double y, double width, double height, double area, char* cell_id)
{
	int cam_idx = 0;
	Herror Herr;

	clear_window(m_dlgImgRect[cam_idx]->GetView());
	set_check("~give_error");
	Herr = read_image(&m_HalImage[cam_idx], path);
	set_check("give_error");
	if(H_MSG_TRUE == Herr)
	{
		disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
	}

	m_edit_ccd_x = x;
	m_edit_ccd_y = y;
	m_edit_ccd_width = width;
	m_edit_ccd_height = height;
	m_edit_ccd_area = area;
	m_edit_strCellID.Format(_T("%S"), cell_id);
	
	if(IsWindowVisible())
	{
		UpdateData(FALSE);
	}
	
	//set_part(m_lNGWindowID[m_iNGSubViewIdx[0]], sy, sx, ey, ex);
}




//void CDialogCCD::OnMove(int x, int y)
//{
//	CDialogEx::OnMove(x, y);
//
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//
//}

//void CDialogCCD::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
//
//
//}

//void CDialogCCD::OnMoving(UINT fwSide, LPRECT pRect)
//{
//	CDialogEx::OnMoving(fwSide, pRect);
//
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//
//}



BOOL CDialogCCD::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
