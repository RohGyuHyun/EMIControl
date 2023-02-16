// Dialog/DialogGlass.cpp : 구현 파일입니다.
//


#include "stdafx.h"
#include "EMIControl.h"
//#include "xls\ExcelFormat.h"
//using namespace ExcelFormat;
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


// CDialogGlass 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogGlass, CDialogEx)



CDialogGlass::CDialogGlass(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogGlass::IDD, pParent)
{
	int i;

	memset(&m_GlassDat, NULL, sizeof(typeGlassData));

	//m_CvdCad = new typeCadData[CVD_CAD_CNT];
	memset(m_CvdCad, NULL, sizeof(typeCadData) * (CVD_CAD_CNT));

	//m_DamCad = new typeCadData[CVD_CAD_CNT];
	memset(m_DamCad, NULL, sizeof(typeCadData) * (CVD_CAD_CNT));

	for(i = 0; i < RECIPE_SCAN_TYPE_MAX_NUM; i++)
	{
		m_DftGlassDat_coordXY[i] = new typeDefectData[DEFECT_DATA_MAX_NUM];
		memset(m_DftGlassDat_coordXY[i], NULL, sizeof(typeDefectData) * DEFECT_DATA_MAX_NUM);
		
		m_DftScanDat_scan[i] = new typeDefectData[DEFECT_DATA_MAX_NUM];
		memset(m_DftScanDat_scan[i], NULL, sizeof(typeDefectData) * DEFECT_DATA_MAX_NUM);

		memset(&m_ScanManage[i], NULL, sizeof(typeScanManage));

		m_DftScanData_before[i] = new typeDefectData[DEFECT_DATA_MAX_NUM];
		memset(m_DftScanData_before[i], NULL, sizeof(typeDefectData) * DEFECT_DATA_MAX_NUM);

		DefectCompareIdx = new int[DEFECT_DATA_MAX_NUM];
		memset(DefectCompareIdx, NULL, sizeof(int) * DEFECT_DATA_MAX_NUM);

		BeforeDefectCnt[i] = 0;
	}

	m_dDestRobot_y = 200.;
	m_dDestRobot_x = -200;
}

CDialogGlass::~CDialogGlass()
{
	int i;
	for(i = 0; i < RECIPE_SCAN_TYPE_MAX_NUM; i++)
	{
		delete m_DftGlassDat_coordXY[i];
		delete m_DftScanDat_scan[i];
	}
}

void CDialogGlass::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogGlass, CDialogEx)
	ON_MESSAGE(USER_MSG_IMG_RECT, OnImageRectMessage)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogGlass 메시지 처리기입니다.

BOOL CDialogGlass::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Herror herr;	
	CRect rect;
	int i;
	HTuple wd, ht;
	long width, height;
	
	for(i = 0; i < GLASS_IMG_RECT_MAX_NUM; i++)
	{
		m_dlgImgRect[i] = new C_CtrlImageRect;
		m_dlgImgRect[i]->Create(IMAGE_RECT_DIALOG, this);

		switch(i)
		{
		case 0:
			GetDlgItem(IDC_STATIC_GLASS0)->GetClientRect(rect);
			GetDlgItem(IDC_STATIC_GLASS0)->ClientToScreen(rect);

			m_dlgImgRect[i]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);
			m_dlgImgRect[i]->SetCamIndex(0, _T("                                                         GLASS WINDOW"), this->m_hWnd);
			break;
		case 1:
		default:
			AfxMessageBox(_T("Error : invalid index(CDialogGlass::OnInitDialog())!"));
			break;
		}

		m_dlgImgRect[i]->ShowWindow(SW_SHOW);

	}

	//Halcon image initialize
	for(i = 0; i < GLASS_IMG_RECT_MAX_NUM; i++)
	{
		gen_image_const(&m_HalImage[i], HTuple("byte"), GLASS_IMAGE_WIDTH, GLASS_IMAGE_HEIGHT);

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
		m_iFilm_id_a = 1;
		m_iFilm_id_b = 1;


		m_dlgImgRect[i]->ShowWindow(SW_HIDE);
	}

	SetDrawCrossRobot(0,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogGlass::MakeCellID()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString row_idx,cell_idx;
	int idx_0 = 0;
	int idx_1 = 0;
	int count = 0;
	//strCell_table[GLASS_MAX_CELL_NUM][GLASS_MAX_CELL_NUM].Empty();

	for(int i=0; i < pdlg->m_RecipePanel->GetCellCntY_coordXY(); i++)
	{
		for(int j=0; j<pdlg->m_RecipePanel->GetCellCntX_coordXY(); j++)
		{
			m_DfCount[i][j].idx = 0;
		}
	}

	for(int i=0; i < pdlg->m_RecipePanel->GetCellCntY_coordXY(); i++)
	{
		if(idx_0 + 65 == 73)
			++idx_0;
		if(idx_0 + 65 == 79)
			++idx_0;

		row_idx.Format(_T("%c"),idx_0 + 65);

		for(int j=0; j<pdlg->m_RecipePanel->GetCellCntX_coordXY(); j++)
		{
			if(idx_1 < 9)
			{
				cell_idx.Format(_T("%s%d"),row_idx,idx_1 + 1);
				m_strCell_table[i][j] = cell_idx;
				m_DfCount[i][j].idx = count;
			}
			else
			{

				if(idx_1 + 56 == 73)
					++idx_1; 
				if(idx_1 + 56 == 79)
					++idx_1;
				cell_idx.Format(_T("%s%c"),row_idx,idx_1 + 56);
				m_strCell_table[i][j] = cell_idx;
				m_DfCount[i][j].idx = count;
			}

			idx_1++;
			count++;
		}
		idx_0++;
		idx_1 = 0;
	}
}

LRESULT CDialogGlass::OnImageRectMessage(WPARAM para0, LPARAM para1)
{
	
	// TODO: Add your control notification handler code here
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	Hobject OutputImage;
	HTuple wd, ht;
	Herror herr;
	CString status;
	double in_glass_x, in_glass_y;
	double glass_x, glass_y;
	double robot_x, robot_y;
	int cam_idx;
	cam_idx = para0;
	long x, y;
	x = 0;
	y = 0;
	int scan_idx;
	CString strTemp;
	CFileDialog dlgFile(FALSE,_T("Image"),_T("*.bmp"),
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			_T("BMP Image(*.bmp)|*.bmp|JPEG(*.jpg)|*.jpg||"));;
	CString tmpPath_Name = _T("");
	char temp[512];
	wchar_t *tmpfilename = NULL;

	switch((int)para1)
	{
	//Display
	case 0:
		disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
		m_dlgImgRect[cam_idx]->Display();
		DispGlassDat();

		SetDispDefectGlassData(pdlg->GetScanTypeIdx());
		SetDispCompareDefectGlassData(pdlg->GetScanTypeIdx());

		m_dDestRobot_y = 200.;
		m_dDestRobot_x = -200;

		robot_y = pdlg->m_Robot->GetRobotPos_mm(MOTOR_0);
		robot_x = pdlg->m_Robot->GetRobotPos_mm(MOTOR_1);

		SetDrawCrossReviewX(robot_y, robot_x);
		SetDrawCrossReviewY(robot_y, robot_x);
		break;
	//Live on
	case 1:
		DispGlassDat();
		break;
	//Live off
	case 2:
		DispGlassDat();
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
				dump_window(m_dlgImgRect[0]->GetView(), HTuple("bmp"), temp);
				//write_image(OutputImage, HTuple("bmp"), NULL, temp);
			}
			else if(_T("jpg") == strTemp || _T("JPG") == strTemp)
			{
				//char temp[100];
				sprintf_s(temp, tmpPath_Name.GetLength() + 1, "%S", tmpPath_Name);
				dump_window(m_dlgImgRect[0]->GetView(), HTuple("jpeg"), temp);
				//write_image(OutputImage, HTuple("jpeg"), NULL, temp);
			}
			else
			{
			
			}
		}
		break;
	//Gray value
	case 5:
		m_dlgImgRect[cam_idx]->GetGrayPos(&x, &y);
		if(pdlg->GetEnableMove())
		{

			if(pdlg->GetEnableDefectMove())
			{
				GetCoordDispToGlass(x, y, &in_glass_x, &in_glass_y);
				
				//Find defect
				scan_idx = GetInRectInGlass(pdlg->GetScanTypeIdx(), in_glass_x, in_glass_y, &glass_x, &glass_y);
				if(0 <= scan_idx)
				{
					CString temp;
					temp.Format(_T("Defect Index:%d View"), scan_idx + 1);
					pdlg->m_btn_MainMsg.SetWindowTextW(temp);

					//Glass to robot
					GetCoordGlassToRobot(glass_x, glass_y, &robot_y, &robot_x);
					//Move
					pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, robot_x + pdlg->m_Review->GetOffsetY());
					pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, robot_y + pdlg->m_Review->GetOffsetX());
			
					disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
					SetDispGlassDat();

					//20160324 ngh pdlg->m_Glass->SetDrawCrossRobot(y, x); -> 변경
					SetDrawCrossRobot(robot_y, robot_x);

					//Scan image and width, height disply
					pdlg->m_CCD->SetReadImage(m_DftGlassDat_coordXY[pdlg->GetScanTypeIdx()][scan_idx].m_sImgPath, 
						                      m_DftGlassDat_coordXY[pdlg->GetScanTypeIdx()][scan_idx].m_dCntX, 
											  m_DftGlassDat_coordXY[pdlg->GetScanTypeIdx()][scan_idx].m_dCntY, 
											  m_DftGlassDat_coordXY[pdlg->GetScanTypeIdx()][scan_idx].m_dWidth, 
											  m_DftGlassDat_coordXY[pdlg->GetScanTypeIdx()][scan_idx].m_dHeight, 
											  m_DftGlassDat_coordXY[pdlg->GetScanTypeIdx()][scan_idx].m_dArea,
											  m_DftGlassDat_coordXY[pdlg->GetScanTypeIdx()][scan_idx].m_cCellID);
				}
			}
			else
			{
				//Disp to galss
				GetCoordDispToGlass(x, y, &glass_x, &glass_y);


				//Glass to robot
				GetCoordGlassToRobot(glass_x, glass_y, &robot_y, &robot_x);
				//Move
				pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, robot_x);
				pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, robot_y);
			
				disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
				SetDispGlassDat();
				SetDrawCrossRobot(robot_y, robot_x);
			}

			SetDispDefectGlassData(pdlg->GetScanTypeIdx());
		}
		break;
	//Region confirm
	case 6:
			
		break;
	//Test
	case 7:
	
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogGlass::OnImageRectMessage)!"));
		break;
	}

	return 0;
}

void CDialogGlass::GetDefectGlassXY(int scan_type, int idx, double *cent_x, double *cent_y, int offset)
{
	double cx, cy, sx, sy, ex, ey;
	cx = 0.;
	cy = 0.;
	sx = 0.;
	sy = 0.;
	ex = 0.;
	ey = 0.;

	*cent_x = 0.;
	*cent_y = 0.;

	cx = m_DftGlassDat_coordXY[scan_type][idx].m_dCntX;
	cy = m_DftGlassDat_coordXY[scan_type][idx].m_dCntY;
	sx = cx - (m_DftGlassDat_coordXY[scan_type][idx].m_dWidth * 0.5) - offset;
	sy = cy + (m_DftGlassDat_coordXY[scan_type][idx].m_dHeight * 0.5) + offset;	
	ex = cx + (m_DftGlassDat_coordXY[scan_type][idx].m_dWidth * 0.5) + offset;
	ey = cy - (m_DftGlassDat_coordXY[scan_type][idx].m_dHeight * 0.5) - offset;	
	
	/*DistXY(double rx, double ry,				
				double ax, double ay,
				double *distX, double *distY);*/
	*cent_x = cx;
	*cent_y = cy;
}

void CDialogGlass::DefectMove(int idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	double in_glass_x, in_glass_y, glass_x, glass_y, robot_x, robot_y;
	int cam_idx = 0;
	int scan_idx = idx;
	if(pdlg->GetEnableMove())
	{
		if(pdlg->GetEnableDefectMove())
		{
			CString temp;
			temp.Format(_T("Defect Index:%d View"), scan_idx + 1);
			pdlg->m_btn_MainMsg.SetWindowTextW(temp);

			GetDefectGlassXY(pdlg->GetScanTypeIdx(), idx, &glass_x, &glass_y);

			//Glass to robot
			GetCoordGlassToRobot(glass_x, glass_y, &robot_y, &robot_x);
			//Move
			pdlg->m_Robot->SetRobotMove_mm(MOTOR_1, robot_x + pdlg->m_Review->GetOffsetY());
			pdlg->m_Robot->SetRobotMove_mm(MOTOR_0, robot_y + pdlg->m_Review->GetOffsetX());
			
			disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());
			SetDispGlassDat();

			//Scan image and width, height disply
			pdlg->m_CCD->SetReadImage(m_DftGlassDat_coordXY[pdlg->GetScanTypeIdx()][scan_idx].m_sImgPath, 
						                m_DftGlassDat_coordXY[pdlg->GetScanTypeIdx()][scan_idx].m_dCntX, 
										m_DftGlassDat_coordXY[pdlg->GetScanTypeIdx()][scan_idx].m_dCntY, 
										m_DftGlassDat_coordXY[pdlg->GetScanTypeIdx()][scan_idx].m_dWidth, 
										m_DftGlassDat_coordXY[pdlg->GetScanTypeIdx()][scan_idx].m_dHeight, 
										m_DftGlassDat_coordXY[pdlg->GetScanTypeIdx()][scan_idx].m_dArea,
										m_DftGlassDat_coordXY[pdlg->GetScanTypeIdx()][scan_idx].m_cCellID);

			SendMessage(USER_MSG_IMG_RECT, 0, 0);
			//20160324 ngh pdlg->m_Glass->SetDrawCrossRobot(y, x); -> 변경
			SetDrawCrossRobot(robot_y, robot_x);
		}
	}
}


void CDialogGlass::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int i;
	CRect rect;

	if(bShow)
	{
		for(i = 0; i < GLASS_IMG_RECT_MAX_NUM; i++)
		{

			GetDlgItem(IDC_STATIC_GLASS0)->GetClientRect(rect);
			GetDlgItem(IDC_STATIC_GLASS0)->ClientToScreen(rect);

			m_dlgImgRect[i]->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), FALSE);

			m_dlgImgRect[i]->ShowWindow(SW_SHOW);
		}

		DispGlassDat();
	}
	else
	{
		for(int i = 0; i < GLASS_IMG_RECT_MAX_NUM; i++)
		{
			m_dlgImgRect[i]->ShowWindow(SW_HIDE);
		}
	}
}

//Glass : 오른쪽 아래가 (0, 0)이다(X, Y 수학 좌표계)
void CDialogGlass::GetCoordGlassToDisp(double gx, double gy, double *dx, double *dy)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	//::GetCoordGlassToDisp(pdlg->m_RecipePanel->GetPanelSizeX_coordXY(), pdlg->m_RecipePanel->GetPanelSizeY_coordXY(), 
	//	GLASS_START_OFFSET_X_MM, GLASS_START_OFFSET_Y_MM, GLASS_PIXEL_RATIO, GLASS_PIXEL_RATIO, gx, gy, dx, dy);

	double panel_size_x, panel_size_y;
	double img_x, img_y;

	gx = gx * 1000.;
	gy = gy * 1000.;

	panel_size_x = pdlg->m_RecipePanel->GetPanelSizeX_coordXY() * 1000.;
	panel_size_y = pdlg->m_RecipePanel->GetPanelSizeY_coordXY() * 1000.;

	*dx = (gx + (GLASS_START_OFFSET_X_MM * 1000.) + panel_size_x) * GLASS_PIXEL_RATIO;	
	*dy = (-gy + (GLASS_START_OFFSET_Y_MM * 1000.) + panel_size_y) * GLASS_PIXEL_RATIO;
}

void CDialogGlass::GetCoordDispToGlass(double dx, double dy, double *gx, double *gy)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	::GetCoordDispToGlass(pdlg->m_RecipePanel->GetPanelSizeX_coordXY(), pdlg->m_RecipePanel->GetPanelSizeY_coordXY(),
		GLASS_START_OFFSET_X_MM, GLASS_START_OFFSET_Y_MM, GLASS_PIXEL_RATIO, GLASS_PIXEL_RATIO, dx, dy, gx, gy);

	/*double panel_size_x, panel_size_y;
	double img_x, img_y;

	panel_size_x = pdlg->m_RecipePanel->GetPanelSizeX_coordXY() * 1000.;
	panel_size_y = pdlg->m_RecipePanel->GetPanelSizeY_coordXY() * 1000.;

	*gx = (dx / GLASS_PIXEL_RATIO) - (1000. * GLASS_START_OFFSET_X_MM) - (panel_size_x);	
	*gy = (-dy / GLASS_PIXEL_RATIO) + (1000. * GLASS_START_OFFSET_Y_MM) + (panel_size_y);

	*gx = *gx * 0.001;
	*gy = *gy * 0.001;*/
}

void CDialogGlass::DispGlassDat()
{
	MakeGlassDat(TRUE);
	SetDispRegion();
}

void CDialogGlass::MakeGlassDat(BOOL disp)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	double sx, sy, ex, ey;
	double size;
	double dist_inter_mark_x, dist_inter_mark_y;
	long l_sx, l_sy, l_ex, l_ey;
	int cam_idx, mark_idx, i, j;
	double x, y;
	int idx, grp_idx_x, grp_idx_y;
	

	memset(&m_GlassDat, NULL, sizeof(typeGlassData));

	cam_idx = 0;
	disp_obj(m_HalImage[cam_idx], m_dlgImgRect[cam_idx]->GetView());	

	//Draw glass///////////////////////////////////////////////////////////////////////////////////////////
	cam_idx = 0;
	GetCoordGlassToDisp(0, 0, &sx, &sy);
	GetCoordGlassToDisp(-pdlg->m_RecipePanel->GetPanelSizeX_coordXY(), pdlg->m_RecipePanel->GetPanelSizeY_coordXY(), &ex, &ey);
	l_sx = sx + 0.5;
	l_sy = sy + 0.5;
	l_ex = ex + 0.5;
	l_ey = ey + 0.5;
	set_color(m_dlgImgRect[cam_idx]->GetView(), "blue");
	set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
	disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
	disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
	disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
	disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);

	//Draw glass Align mark///////////////////////////////////////////////////////////////////////////////////////
	//Mark0//////////////////////
	cam_idx = 0;
	mark_idx = 0;
	size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

	m_GlassDat.m_dGlassAlignMarkPoxX0 = pdlg->m_RecipePanel->GetMarkPosX_coordXY(mark_idx);
	m_GlassDat.m_dGlassAlignMarkPoxY0 = pdlg->m_RecipePanel->GetMarkPosY_coordXY(mark_idx);
	//Horz
	GetCoordGlassToDisp(m_GlassDat.m_dGlassAlignMarkPoxX0 - size,
		             m_GlassDat.m_dGlassAlignMarkPoxY0,
					 &sx, &sy);
	GetCoordGlassToDisp(m_GlassDat.m_dGlassAlignMarkPoxX0 + size,
		             m_GlassDat.m_dGlassAlignMarkPoxY0,
					 &ex, &ey);

	if(disp)
	{
		l_sx = sx + 0.5;
		l_sy = sy + 0.5;
		l_ex = ex + 0.5;
		l_ey = ey + 0.5;
		set_color(m_dlgImgRect[cam_idx]->GetView(), "blue");
		set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
		disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);
	}
	//Vert
	GetCoordGlassToDisp(m_GlassDat.m_dGlassAlignMarkPoxX0,
		             m_GlassDat.m_dGlassAlignMarkPoxY0 - size,
					 &sx, &sy);
	GetCoordGlassToDisp(m_GlassDat.m_dGlassAlignMarkPoxX0,
		             m_GlassDat.m_dGlassAlignMarkPoxY0 + size,
					 &ex, &ey);
	l_sx = sx + 0.5;
	l_sy = sy + 0.5;
	l_ex = ex + 0.5;
	l_ey = ey + 0.5;
	set_color(m_dlgImgRect[cam_idx]->GetView(), "blue");
	set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
	disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);

	//Mark1//////////////////////
	cam_idx = 0;
	mark_idx = 1;
	size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

	m_GlassDat.m_dGlassAlignMarkPoxX1 = pdlg->m_RecipePanel->GetMarkPosX_coordXY(mark_idx);
	m_GlassDat.m_dGlassAlignMarkPoxY1 = fabs(pdlg->m_RecipePanel->GetPanelSizeY_coordXY()) + pdlg->m_RecipePanel->GetMarkPosY_coordXY(mark_idx);
	//Horz
	if(disp)
	{
		GetCoordGlassToDisp(m_GlassDat.m_dGlassAlignMarkPoxX1 - size,
						 m_GlassDat.m_dGlassAlignMarkPoxY1,
						 &sx, &sy);
		GetCoordGlassToDisp(m_GlassDat.m_dGlassAlignMarkPoxX1 + size,
						 m_GlassDat.m_dGlassAlignMarkPoxY1,
						 &ex, &ey);

		l_sx = sx + 0.5;
		l_sy = sy + 0.5;
		l_ex = ex + 0.5;
		l_ey = ey + 0.5;
		set_color(m_dlgImgRect[cam_idx]->GetView(), "blue");
		set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
		disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);
	}
	//Vert
	if(disp)
	{
		GetCoordGlassToDisp(m_GlassDat.m_dGlassAlignMarkPoxX1,
						 m_GlassDat.m_dGlassAlignMarkPoxY1 - size,
						 &sx, &sy);
		GetCoordGlassToDisp(m_GlassDat.m_dGlassAlignMarkPoxX1,
						 m_GlassDat.m_dGlassAlignMarkPoxY1 + size,
						 &ex, &ey);

		l_sx = sx + 0.5;
		l_sy = sy + 0.5;
		l_ex = ex + 0.5;
		l_ey = ey + 0.5;
		set_color(m_dlgImgRect[cam_idx]->GetView(), "blue");
		set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
		disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);
	}

	//20160717 ngh
	//glass front
	long f_x,f_y;
	char cFront[6];
	char cTemp[2];
	sprintf(cFront, "FRONT");

	GetCoordGlassToDisp(0, 0, &ex, &ey);
	f_y = ((ey + 0.5) / 2) - 600;
	f_x = ((ex + 0.5) - 170);

	set_color(m_dlgImgRect[cam_idx]->GetView(), "red");
	set_font(m_dlgImgRect[cam_idx]->GetView(), "-Arial-15-*-*-*-*-*-1-");

	set_tposition(m_dlgImgRect[cam_idx]->GetView(), f_y, f_x);
	sprintf(cTemp, "F");
	write_string(m_dlgImgRect[cam_idx]->GetView(), (HTuple)cTemp);

	f_y+=300;
	set_tposition(m_dlgImgRect[cam_idx]->GetView(), f_y, f_x);
	sprintf(cTemp, "R");
	write_string(m_dlgImgRect[cam_idx]->GetView(), (HTuple)cTemp);

	f_y+=300;
	set_tposition(m_dlgImgRect[cam_idx]->GetView(), f_y, f_x);
	sprintf(cTemp, "O");
	write_string(m_dlgImgRect[cam_idx]->GetView(), (HTuple)cTemp);

	f_y+=300;
	set_tposition(m_dlgImgRect[cam_idx]->GetView(), f_y, f_x);
	sprintf(cTemp, "N");
	write_string(m_dlgImgRect[cam_idx]->GetView(), (HTuple)cTemp);

	f_y+=300;
	set_tposition(m_dlgImgRect[cam_idx]->GetView(), f_y, f_x);
	sprintf(cTemp, "T");
	write_string(m_dlgImgRect[cam_idx]->GetView(), (HTuple)cTemp);





	//Draw cell Align mark///////////////////////////////////////////////////////////////////////////////////////
	cam_idx = 0;
	mark_idx = 0;
	size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

	idx = 0;
	if(pdlg->m_RecipePanel->GetCellCntX_coordXY() > pdlg->m_RecipePanel->GetCellGrpCntX_coordXY())
	{
		grp_idx_x = (int)pdlg->m_RecipePanel->GetCellGrpCntX_coordXY();
	}
	else
	{
		grp_idx_x = 0;
	}
	if(pdlg->m_RecipePanel->GetCellCntY_coordXY() > pdlg->m_RecipePanel->GetCellGrpCntY_coordXY())
	{
		grp_idx_y = (int)pdlg->m_RecipePanel->GetCellGrpCntY_coordXY();
	}
	else
	{
		grp_idx_y = 0;
	}

	x = 0;
	y = 0;
	int ins_type = 0, rect_type = 0;;
	for(i = 0; i < pdlg->m_RecipePanel->GetCellCntY_coordXY(); i++)
	{
		if(0 == i)
		{
			y = pdlg->m_RecipePanel->GetMarkPosY_coordXY(mark_idx) + pdlg->m_RecipePanel->GetCellFirstPosY_coordXY();
		}
		else if(i == grp_idx_y)
		{
			y =  y + pdlg->m_RecipePanel->GetCellGrpPitchY_coordXY();
		}
		else
		{
			y =  y + pdlg->m_RecipePanel->GetCellPitchY_coordXY();
		}

		for(j = 0; j < pdlg->m_RecipePanel->GetCellCntX_coordXY();j++)
		{
			if(GLASS_MAX_CELL_NUM <= idx)
			{
				break;
			}

			if(0 == j)
			{
				x = pdlg->m_RecipePanel->GetMarkPosX_coordXY(mark_idx) + pdlg->m_RecipePanel->GetCellFirstPosX_coordXY();
			}
			else if(j == grp_idx_x)
			{
				x =  x + pdlg->m_RecipePanel->GetCellGrpPitchX_coordXY();
			}
			else
			{
				x =  x + pdlg->m_RecipePanel->GetCellPitchX_coordXY();
			}

			m_GlassDat.m_dCellAlignMarkPoxX[idx] = x;
			m_GlassDat.m_dCellAlignMarkPoxY[idx] = y;

			rect_type = 2;
			ins_type = 0;
			m_GlassDat.m_dPCLAlignMarkPosX[rect_type][ins_type][idx] = x;
			m_GlassDat.m_dPCLAlignMarkPosY[rect_type++][ins_type++][idx] = y;
			m_GlassDat.m_dPCLAlignMarkPosX[rect_type][ins_type][idx] = x;
			m_GlassDat.m_dPCLAlignMarkPosY[rect_type++][ins_type++][idx] = y;

			//Horz
			if(disp)
			{
				GetCoordGlassToDisp(x - size,
								 y,
								 &sx, &sy);
				GetCoordGlassToDisp(x + size,
								 y,
								 &ex, &ey);
				l_sx = sx + 0.5;
				l_sy = sy + 0.5;
				l_ex = ex + 0.5;
				l_ey = ey + 0.5;
				set_color(m_dlgImgRect[cam_idx]->GetView(), "blue");
				set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);
			}
			//Vert
			if(disp)
			{
				GetCoordGlassToDisp(x,
								 y - size,
								 &sx, &sy);
				GetCoordGlassToDisp(x,
								 y + size,
								 &ex, &ey);
				l_sx = sx + 0.5;
				l_sy = sy + 0.5;
				l_ex = ex + 0.5;
				l_ey = ey + 0.5;
				set_color(m_dlgImgRect[cam_idx]->GetView(), "blue");
				set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);
			}



			idx++;

		}


	}

	//Draw cell active area////////////////////////////////////////////////////////////////////////////
	cam_idx = 0;
	mark_idx = 0;
	size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

	idx = 0;
	for(i = 0; i < pdlg->m_RecipePanel->GetCellCntY_coordXY(); i++)
	{
		for(j = 0; j < pdlg->m_RecipePanel->GetCellCntX_coordXY();j++)
		{
			if(GLASS_MAX_CELL_NUM <= idx)
			{
				break;
			}

			m_GlassDat.m_dCellActiveRectSX[idx] = m_GlassDat.m_dCellAlignMarkPoxX[idx] + pdlg->m_RecipeCell->GetCellPosFromCellAlignX_coordXY();
			m_GlassDat.m_dCellActiveRectSY[idx] = m_GlassDat.m_dCellAlignMarkPoxY[idx] + pdlg->m_RecipeCell->GetCellPosFromCellAlignY_coordXY();

			m_GlassDat.m_dCellActiveRectEX[idx] = m_GlassDat.m_dCellActiveRectSX[idx] + fabs(pdlg->m_RecipeCell->GetCellActiveSizeX_coordXY());
			m_GlassDat.m_dCellActiveRectEY[idx] = m_GlassDat.m_dCellActiveRectSY[idx] - fabs(pdlg->m_RecipeCell->GetCellActiveSizeY_coordXY());

			GetCoordGlassToDisp(m_GlassDat.m_dCellActiveRectSX[idx],
							m_GlassDat.m_dCellActiveRectSY[idx],
							&sx, &sy);
			GetCoordGlassToDisp(m_GlassDat.m_dCellActiveRectEX[idx],
							m_GlassDat.m_dCellActiveRectEY[idx],
							&ex, &ey);
			l_sx = sx + 0.5;
			l_sy = sy + 0.5;
			l_ex = ex + 0.5;
			l_ey = ey + 0.5;
			set_color(m_dlgImgRect[cam_idx]->GetView(), "blue");
			set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);

			idx++;
		}
	}

	set_color(m_dlgImgRect[cam_idx]->GetView(), "white");
	set_font(m_dlgImgRect[cam_idx]->GetView(), "-Arial-27-*-*-*-*-*-1-");
	double map_id_x, map_id_y;
	int idx_0 = 0;
	idx = 0;
	char strMapIdx1[5];
	CString temp;
	for(i = 0; i < pdlg->m_RecipePanel->GetCellCntY_coordXY(); i++)//y
	{
		if(GLASS_MAX_CELL_NUM <= idx)
		{
			break;
		}
		idx = (pdlg->m_RecipePanel->GetCellCntX_coordXY() * i);

		if(idx_0 + 65 == 73)
			++idx_0;
		if(idx_0 + 65 == 79)
			++idx_0;

		map_id_x = m_GlassDat.m_dCellAlignMarkPoxX[idx] + pdlg->m_RecipeCell->GetCellPosFromCellAlignX_coordXY();
		map_id_y = m_GlassDat.m_dCellAlignMarkPoxY[idx] + pdlg->m_RecipeCell->GetCellPosFromCellAlignY_coordXY();

		GetCoordGlassToDisp(map_id_x,
							map_id_y,
							&sx, &sy);

		l_sx = sx + 0.5;
		l_sy = sy + 0.5;

		set_tposition(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx - 270);
		temp.Format(_T("%c"), idx_0 + 65);
		sprintf(strMapIdx1, "%S", temp);

		write_string(m_dlgImgRect[cam_idx]->GetView(), (HTuple)strMapIdx1);

		
		idx_0++;
	}

	idx_0 = 0;
	idx = 0;
	for(j = 0; j < pdlg->m_RecipePanel->GetCellCntX_coordXY();j++)//x
	{
		if(GLASS_MAX_CELL_NUM <= idx)
		{
			break;
		}

		if(j < 9)
		{
			idx_0 = j + 49;
		}
		else
		{
			if(j == 9)
				idx_0 = 65;
			if(idx_0 == 73)
				++idx_0; 
			if(idx_0 == 79)
				++idx_0;
		}

		map_id_x = m_GlassDat.m_dCellAlignMarkPoxX[j] + pdlg->m_RecipeCell->GetCellPosFromCellAlignX_coordXY();
		map_id_y = m_GlassDat.m_dCellAlignMarkPoxY[j] + pdlg->m_RecipeCell->GetCellPosFromCellAlignY_coordXY();

		GetCoordGlassToDisp(map_id_x,
							map_id_y,
							&sx, &sy);

		l_sx = sx + 0.5;
		l_sy = sy + 0.5;

		set_tposition(m_dlgImgRect[cam_idx]->GetView(), l_sy - 320, l_sx);
		temp.Format(_T("%c"), idx_0);
		sprintf(strMapIdx1, "%S", temp);

		write_string(m_dlgImgRect[cam_idx]->GetView(), (HTuple)strMapIdx1);

		idx_0++;
	}

	//Draw film align mark////////////////////////////////////////////////////////////////////////////////////
	cam_idx = 0;
	mark_idx = 0;
	size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

	idx = 0;
	if(pdlg->m_RecipeCell->GetFilmCntX_coordXY() > pdlg->m_RecipeCell->GetFilmGrpCntX_coordXY())
	{
		grp_idx_x = (int)pdlg->m_RecipeCell->GetFilmGrpCntX_coordXY();
	}
	else
	{
		grp_idx_x = 0;
	}
	if(pdlg->m_RecipeCell->GetFilmCntY_coordXY() > pdlg->m_RecipeCell->GetFilmGrpCntY_coordXY())
	{
		grp_idx_y = (int)pdlg->m_RecipeCell->GetFilmGrpCntY_coordXY();
	}
	else
	{
		grp_idx_y = 0;
	}

	x = 0;
	y = 0;
	for(i = 0; i < pdlg->m_RecipeCell->GetFilmCntY_coordXY(); i++)
	{
		if(0 == i)
		{
			y = pdlg->m_RecipePanel->GetMarkPosY_coordXY(mark_idx) + pdlg->m_RecipePanel->GetFilmFirstPosY_coordXY();
		}
		else if(i == grp_idx_y)
		{
			y =  y + pdlg->m_RecipeCell->GetFilmGrpPitchY_coordXY();
		}
		else
		{
			y =  y + pdlg->m_RecipeCell->GetFilmPitchY_coordXY();
		}

		for(j = 0; j < pdlg->m_RecipeCell->GetFilmCntX_coordXY();j++)
		{
			if(GLASS_MAX_CELL_NUM <= idx)
			{
				break;
			}

			if(0 == j)
			{
				x = pdlg->m_RecipePanel->GetMarkPosX_coordXY(mark_idx) + pdlg->m_RecipePanel->GetFilmFirstPosX_coordXY();
			}
			else if(j == grp_idx_y)
			{
				x =  x + pdlg->m_RecipeCell->GetFilmGrpPitchX_coordXY();
			}
			else
			{
				x =  x + pdlg->m_RecipeCell->GetFilmPitchX_coordXY();
			}

			m_GlassDat.m_dFilmAlignMarkPosX[idx] = x;
			m_GlassDat.m_dFilmAlignMarkPosY[idx] = y;

			//Horz
			if(disp)
			{
				GetCoordGlassToDisp(x - size,
								 y,
								 &sx, &sy);
				GetCoordGlassToDisp(x + size,
								 y,
								 &ex, &ey);
				l_sx = sx + 0.5;
				l_sy = sy + 0.5;
				l_ex = ex + 0.5;
				l_ey = ey + 0.5;
				set_color(m_dlgImgRect[cam_idx]->GetView(), "gray");
				set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);
			}
			//Vert
			if(disp)
			{
				GetCoordGlassToDisp(x,
								 y - size,
								 &sx, &sy);
				GetCoordGlassToDisp(x,
								 y + size,
								 &ex, &ey);
				l_sx = sx + 0.5;
				l_sy = sy + 0.5;
				l_ex = ex + 0.5;
				l_ey = ey + 0.5;
				set_color(m_dlgImgRect[cam_idx]->GetView(), "gray");
				set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);
			}
			idx++;
		}
	}

	//Draw film active area////////////////////////////////////////////////////////////////////////////
	cam_idx = 0;
	mark_idx = 0;
	size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

	idx = 0;
	for(i = 0; i < pdlg->m_RecipeCell->GetFilmCntY_coordXY(); i++)
	{
		for(j = 0; j < pdlg->m_RecipeCell->GetFilmCntX_coordXY();j++)
		{
			if(GLASS_MAX_CELL_NUM <= idx)
			{
				break;
			}

			m_GlassDat.m_dFilmActiveRectSX[idx] = m_GlassDat.m_dFilmAlignMarkPosX[idx] + pdlg->m_RecipeCell->GetFilmPosFromFilmAlignX_coordXY();
			m_GlassDat.m_dFilmActiveRectSY[idx] = m_GlassDat.m_dFilmAlignMarkPosY[idx] + pdlg->m_RecipeCell->GetFilmPosFromFilmAlignY_coordXY();

			if(pdlg->m_RecipeCell->GetFilmGrpCntX_coordXY() > j)
			{
				m_GlassDat.m_dFilmActiveRectEX[idx] = m_GlassDat.m_dFilmActiveRectSX[idx] + pdlg->m_RecipeCell->GetFilmSizeX0_coordXY();
				m_GlassDat.m_dFilmActiveRectEY[idx] = m_GlassDat.m_dFilmActiveRectSY[idx] - pdlg->m_RecipeCell->GetFilmSizeY0_coordXY();
			}
			else
			{
				m_GlassDat.m_dFilmActiveRectEX[idx] = m_GlassDat.m_dFilmActiveRectSX[idx] + pdlg->m_RecipeCell->GetFilmSizeX1_coordXY();
				m_GlassDat.m_dFilmActiveRectEY[idx] = m_GlassDat.m_dFilmActiveRectSY[idx] - pdlg->m_RecipeCell->GetFilmSizeY1_coordXY();
			}

			GetCoordGlassToDisp(m_GlassDat.m_dFilmActiveRectSX[idx],
							m_GlassDat.m_dFilmActiveRectSY[idx],
							&sx, &sy);
			GetCoordGlassToDisp(m_GlassDat.m_dFilmActiveRectEX[idx],
							m_GlassDat.m_dFilmActiveRectEY[idx],
							&ex, &ey);
			l_sx = sx + 0.5;
			l_sy = sy + 0.5;
			l_ex = ex + 0.5;
			l_ey = ey + 0.5;
			set_color(m_dlgImgRect[cam_idx]->GetView(), "gray");
			set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);

			idx++;
		}
	}

	
	//Draw CVD align mark////////////////////////////////////////////////////////////////////////////////////
	cam_idx = 0;
	mark_idx = 0;
	size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

	idx = 0;
	if(pdlg->m_RecipeCell->GetCVDCntX_coordXY() > pdlg->m_RecipeCell->GetCVDGrpCntX_coordXY())
	{
		grp_idx_x = (int)pdlg->m_RecipeCell->GetCVDGrpCntX_coordXY();
	}
	else
	{
		grp_idx_x = 0;
	}
	if(pdlg->m_RecipeCell->GetCVDCntY_coordXY() > pdlg->m_RecipeCell->GetCVDGrpCntY_coordXY())
	{
		grp_idx_y = (int)pdlg->m_RecipeCell->GetCVDGrpCntY_coordXY();
	}
	else
	{
		grp_idx_y = 0;
	}

	x = 0;
	y = 0;
	rect_type = 0;
	ins_type = 0;
	for(i = 0; i < pdlg->m_RecipeCell->GetCVDCntY_coordXY(); i++)
	{
		if(rect_type == 2)
		{
			rect_type = 0;
		}
		

		if(0 == i)
		{
			y = pdlg->m_RecipePanel->GetMarkPosY_coordXY(mark_idx) + pdlg->m_RecipePanel->GetCVDFirstPosY_coordXY();
		}
		else if(i == grp_idx_y)
		{
			y =  y + pdlg->m_RecipeCell->GetCVDGrpPitchY_coordXY();
		}
		else
		{
			y =  y + pdlg->m_RecipeCell->GetCVDPitchY_coordXY();
		}

		for(j = 0; j < pdlg->m_RecipeCell->GetCVDCntX_coordXY();j++)
		{
			if(GLASS_MAX_CELL_NUM <= idx)
			{
				break;
			}

			if(0 == j)
			{
				x = pdlg->m_RecipePanel->GetMarkPosX_coordXY(mark_idx) + pdlg->m_RecipePanel->GetCVDFirstPosX_coordXY();
			}
			else if(j == grp_idx_y)
			{
				x =  x + pdlg->m_RecipeCell->GetCVDGrpPitchX_coordXY();
			}
			else
			{
				x =  x + pdlg->m_RecipeCell->GetCVDPitchX_coordXY();
			}

			m_GlassDat.m_dCVDAlignMarkPosX[idx] = x;
			m_GlassDat.m_dCVDAlignMarkPosY[idx] = y;

			//공용
			m_GlassDat.m_dCVDDNAlignMarkPosX[idx] = x;
			m_GlassDat.m_dCVDDNAlignMarkPosY[idx] = y;

			if(ins_type == 2)
			{
				ins_type = 0;
			}

			m_GlassDat.m_dPCLAlignMarkPosX[rect_type][ins_type][idx] = x;
			m_GlassDat.m_dPCLAlignMarkPosY[rect_type][ins_type][idx] = y;
			ins_type++;
			//Horz
			if(disp)
			{
				GetCoordGlassToDisp(x - size,
								 y,
								 &sx, &sy);
				GetCoordGlassToDisp(x + size,
								 y,
								 &ex, &ey);
				l_sx = sx + 0.5;
				l_sy = sy + 0.5;
				l_ex = ex + 0.5;
				l_ey = ey + 0.5;
				set_color(m_dlgImgRect[cam_idx]->GetView(), "dim gray");
				set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);
			}
			//Vert
			if(disp)
			{
				GetCoordGlassToDisp(x,
								 y - size,
								 &sx, &sy);
				GetCoordGlassToDisp(x,
								 y + size,
								 &ex, &ey);
				l_sx = sx + 0.5;
				l_sy = sy + 0.5;
				l_ex = ex + 0.5;
				l_ey = ey + 0.5;
				set_color(m_dlgImgRect[cam_idx]->GetView(), "dim gray");
				set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_ey, l_ex);
			}
			idx++;
		}
		rect_type++;
		
	}

	//Draw cvd active area////////////////////////////////////////////////////////////////////////////
	cam_idx = 0;
	mark_idx = 0;
	size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

	idx = 0;
	for(i = 0; i < pdlg->m_RecipeCell->GetCVDCntY_coordXY(); i++)
	{
		for(j = 0; j < pdlg->m_RecipeCell->GetCVDCntX_coordXY();j++)
		{
			if(GLASS_MAX_CELL_NUM <= idx)
			{
				break;
			}

			m_GlassDat.m_dCVDActiveRectSX[idx] = m_GlassDat.m_dCVDAlignMarkPosX[idx] + pdlg->m_RecipeCell->GetCVDPosFromCVDAlignX_coordXY();
			m_GlassDat.m_dCVDActiveRectSY[idx] = m_GlassDat.m_dCVDAlignMarkPosY[idx] + pdlg->m_RecipeCell->GetCVDPosFromCVDAlignY_coordXY();

			if(pdlg->m_RecipeCell->GetCVDGrpCntX_coordXY() > j)
			{
				m_GlassDat.m_dCVDActiveRectEX[idx] = m_GlassDat.m_dCVDActiveRectSX[idx] + pdlg->m_RecipeCell->GetCVDSizeX0_coordXY();
				m_GlassDat.m_dCVDActiveRectEY[idx] = m_GlassDat.m_dCVDActiveRectSY[idx] - pdlg->m_RecipeCell->GetCVDSizeY0_coordXY();
			}
			else
			{
				m_GlassDat.m_dCVDActiveRectEX[idx] = m_GlassDat.m_dCVDActiveRectSX[idx] + pdlg->m_RecipeCell->GetCVDSizeX1_coordXY();
				m_GlassDat.m_dCVDActiveRectEY[idx] = m_GlassDat.m_dCVDActiveRectSY[idx] - pdlg->m_RecipeCell->GetCVDSizeY1_coordXY();
			}

			GetCoordGlassToDisp(m_GlassDat.m_dCVDActiveRectSX[idx],
							m_GlassDat.m_dCVDActiveRectSY[idx],
							&sx, &sy);
			GetCoordGlassToDisp(m_GlassDat.m_dCVDActiveRectEX[idx],
							m_GlassDat.m_dCVDActiveRectEY[idx],
							&ex, &ey);
			l_sx = sx + 0.5;
			l_sy = sy + 0.5;
			l_ex = ex + 0.5;
			l_ey = ey + 0.5;
			set_color(m_dlgImgRect[cam_idx]->GetView(), "dim gray");
			set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);

			idx++;
		}
	}
	
	//CVDDN : Align은 cvd와 공용

	//Draw cvddn active area////////////////////////////////////////////////////////////////////////////
	cam_idx = 0;
	mark_idx = 0;
	size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;

	idx = 0;
	for(i = 0; i < pdlg->m_RecipeCell->GetCVDDNCntY_coordXY(); i++)
	{
		for(j = 0; j < pdlg->m_RecipeCell->GetCVDDNCntX_coordXY();j++)
		{
			if(GLASS_MAX_CELL_NUM <= idx)
			{
				break;
			}

			m_GlassDat.m_dCVDDNActiveRectSX[idx] = m_GlassDat.m_dCVDDNAlignMarkPosX[idx] + pdlg->m_RecipeCell->GetCVDDNPosFromCVDDNAlignX_coordXY();
			m_GlassDat.m_dCVDDNActiveRectSY[idx] = m_GlassDat.m_dCVDDNAlignMarkPosY[idx] + pdlg->m_RecipeCell->GetCVDDNPosFromCVDDNAlignY_coordXY();

			if(pdlg->m_RecipeCell->GetCVDDNGrpCntX_coordXY() > j)
			{
				m_GlassDat.m_dCVDDNActiveRectEX[idx] = m_GlassDat.m_dCVDDNActiveRectSX[idx] + pdlg->m_RecipeCell->GetCVDDNSizeX0_coordXY();
				m_GlassDat.m_dCVDDNActiveRectEY[idx] = m_GlassDat.m_dCVDDNActiveRectSY[idx] - pdlg->m_RecipeCell->GetCVDDNSizeY0_coordXY();
			}
			else
			{
				m_GlassDat.m_dCVDDNActiveRectEX[idx] = m_GlassDat.m_dCVDDNActiveRectSX[idx] + pdlg->m_RecipeCell->GetCVDDNSizeX1_coordXY();
				m_GlassDat.m_dCVDDNActiveRectEY[idx] = m_GlassDat.m_dCVDDNActiveRectSY[idx] - pdlg->m_RecipeCell->GetCVDDNSizeY1_coordXY();
			}

			GetCoordGlassToDisp(m_GlassDat.m_dCVDDNActiveRectSX[idx],
							m_GlassDat.m_dCVDDNActiveRectSY[idx],
							&sx, &sy);
			GetCoordGlassToDisp(m_GlassDat.m_dCVDDNActiveRectEX[idx],
							m_GlassDat.m_dCVDDNActiveRectEY[idx],
							&ex, &ey);
			l_sx = sx + 0.5;
			l_sy = sy + 0.5;
			l_ex = ex + 0.5;
			l_ey = ey + 0.5;
			set_color(m_dlgImgRect[cam_idx]->GetView(), "dim gray");
			set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);

			idx++;
		}
	}
	
	//top down
	ins_type = 0;
	for(int k = 0; k < 2; k++)
	{
		//ins_type = 0;
		cam_idx = 0;
		mark_idx = 0;
		size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;
		idx = 0;
		for(i = 0; i < pdlg->m_RecipeCellPCL->GetPCLCntY_coordXY(k); i++)
		{
			for(j = 0; j < pdlg->m_RecipeCellPCL->GetPCLCntX_coordXY(k);j++)
			{
				if(GLASS_MAX_CELL_NUM <= idx)
				{
					break;
				}

				m_GlassDat.m_dPCLActiveRectSX[k][ins_type][idx] = m_GlassDat.m_dCVDAlignMarkPosX[idx] + pdlg->m_RecipeCellPCL->GetPCLPosFromPCLAlignX_coordXY(k);
				m_GlassDat.m_dPCLActiveRectSY[k][ins_type][idx] = m_GlassDat.m_dCVDAlignMarkPosY[idx] + pdlg->m_RecipeCellPCL->GetPCLPosFromPCLAlignY_coordXY(k);

				if(pdlg->m_RecipeCellPCL->GetPCLGrpCntX_coordXY(k) > j)
				{
					m_GlassDat.m_dPCLActiveRectEX[k][ins_type][idx] = m_GlassDat.m_dPCLActiveRectSX[k][ins_type][idx] + pdlg->m_RecipeCellPCL->GetPCLSizeX0_coordXY(k);
					m_GlassDat.m_dPCLActiveRectEY[k][ins_type][idx] = m_GlassDat.m_dPCLActiveRectSY[k][ins_type][idx] - pdlg->m_RecipeCellPCL->GetPCLSizeY0_coordXY(k);
				}
				else
				{
					m_GlassDat.m_dPCLActiveRectEX[k][ins_type][idx] = m_GlassDat.m_dPCLActiveRectSX[k][ins_type][idx] + pdlg->m_RecipeCellPCL->GetPCLSizeX1_coordXY(k);
					m_GlassDat.m_dPCLActiveRectEY[k][ins_type][idx] = m_GlassDat.m_dPCLActiveRectSY[k][ins_type][idx] - pdlg->m_RecipeCellPCL->GetPCLSizeY1_coordXY(k);
				}

				GetCoordGlassToDisp(m_GlassDat.m_dPCLActiveRectSX[k][ins_type][idx],
								m_GlassDat.m_dPCLActiveRectSY[k][ins_type][idx],
								&sx, &sy);
				GetCoordGlassToDisp(m_GlassDat.m_dPCLActiveRectEX[k][ins_type][idx],
								m_GlassDat.m_dPCLActiveRectEY[k][ins_type][idx],
								&ex, &ey);
				l_sx = sx + 0.5;
				l_sy = sy + 0.5;
				l_ex = ex + 0.5;
				l_ey = ey + 0.5;
				set_color(m_dlgImgRect[cam_idx]->GetView(), "dark olive green");
				set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);

				idx++;
			}
		}
		ins_type++;
	}
	ins_type = 0;
	//left right
	for(int k = 2; k < 4; k++)
	{
		
		cam_idx = 0;
		mark_idx = 0;
		size = pdlg->m_RecipePanel->GetMarkSize_coordXY(mark_idx) * 0.5;
		idx = 0;
		for(i = 0; i < pdlg->m_RecipeCellPCL->GetPCLCntY_coordXY(k); i++)
		{
			for(j = 0; j < pdlg->m_RecipeCellPCL->GetPCLCntX_coordXY(k);j++)
			{
				if(GLASS_MAX_CELL_NUM <= idx)
				{
					break;
				}

				m_GlassDat.m_dPCLActiveRectSX[k][ins_type][idx] = m_GlassDat.m_dCellAlignMarkPoxX[idx] + pdlg->m_RecipeCellPCL->GetPCLPosFromPCLAlignX_coordXY(k);
				m_GlassDat.m_dPCLActiveRectSY[k][ins_type][idx] = m_GlassDat.m_dCellAlignMarkPoxY[idx] + pdlg->m_RecipeCellPCL->GetPCLPosFromPCLAlignY_coordXY(k);

				if(pdlg->m_RecipeCellPCL->GetPCLGrpCntX_coordXY(k) > j)
				{
					m_GlassDat.m_dPCLActiveRectEX[k][ins_type][idx] = m_GlassDat.m_dPCLActiveRectSX[k][ins_type][idx] + pdlg->m_RecipeCellPCL->GetPCLSizeX0_coordXY(k);
					m_GlassDat.m_dPCLActiveRectEY[k][ins_type][idx] = m_GlassDat.m_dPCLActiveRectSY[k][ins_type][idx] - pdlg->m_RecipeCellPCL->GetPCLSizeY0_coordXY(k);
				}
				else
				{
					m_GlassDat.m_dPCLActiveRectEX[k][ins_type][idx] = m_GlassDat.m_dPCLActiveRectSX[k][ins_type][idx] + pdlg->m_RecipeCellPCL->GetPCLSizeX1_coordXY(k);
					m_GlassDat.m_dPCLActiveRectEY[k][ins_type][idx] = m_GlassDat.m_dPCLActiveRectSY[k][ins_type][idx] - pdlg->m_RecipeCellPCL->GetPCLSizeY1_coordXY(k);
				}

				GetCoordGlassToDisp(m_GlassDat.m_dPCLActiveRectSX[k][ins_type][idx],
								m_GlassDat.m_dPCLActiveRectSY[k][ins_type][idx],
								&sx, &sy);
				GetCoordGlassToDisp(m_GlassDat.m_dPCLActiveRectEX[k][ins_type][idx],
								m_GlassDat.m_dPCLActiveRectEY[k][ins_type][idx],
								&ex, &ey);
				l_sx = sx + 0.5;
				l_sy = sy + 0.5;
				l_ex = ex + 0.5;
				l_ey = ey + 0.5;
				set_color(m_dlgImgRect[cam_idx]->GetView(), "dark olive green");
				set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);

				idx++;
			}
		}
		ins_type++;
	}

	
	//Glass->Robot
	CoordGlassToRobotAll();

	//20160608 ngh
	CvdCadResult();
	DamCadResult();
	MakeCellID();
}

void CDialogGlass::SetUpdate()
{
	MakeGlassDat(TRUE);
}

//현재 glass좌표는 X, Y 수학 좌표이다. Robot(align)은 Y, X좌표계 왼쪽으로 +Y, 위로 +X
//현재 Align은 robot좌표계와 동일하다, 카메라가 90도 회전되어 있기때문임
void CDialogGlass::CoordGlassToRobotAll()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	double offset_x, offset_y;
	int mark_idx, i;
	mark_idx = 0;//첫번째 마크가지고 좌표변환

	//Glass는 X, Y 좌표계(수학)
	offset_x = pdlg->m_Align->GetAlignOffsetPosX_coordXY() - pdlg->m_RecipePanel->GetMarkPosX_coordXY(mark_idx);
	offset_y = pdlg->m_Align->GetAlignOffsetPosY_coordXY() - pdlg->m_RecipePanel->GetMarkPosY_coordXY(mark_idx);

	::CoordGlassToRobotAll(offset_x, offset_y, 
		m_GlassDat.m_dGlassAlignMarkPoxX0, m_GlassDat.m_dGlassAlignMarkPoxY0, m_GlassDat.m_dGlassAlignMarkPoxX1, m_GlassDat.m_dGlassAlignMarkPoxY1,
		&m_RobotDat.m_dGlassAlignMarkPoxX0, &m_RobotDat.m_dGlassAlignMarkPoxY0, &m_RobotDat.m_dGlassAlignMarkPoxX1, &m_RobotDat.m_dGlassAlignMarkPoxY1,
		GLASS_MAX_CELL_NUM,
		m_GlassDat.m_dCellAlignMarkPoxX, m_GlassDat.m_dCellAlignMarkPoxY, m_GlassDat.m_dFilmAlignMarkPosX, m_GlassDat.m_dFilmAlignMarkPosY,
		m_GlassDat.m_dFilmActiveRectSX, m_GlassDat.m_dFilmActiveRectSY, m_GlassDat.m_dFilmActiveRectEX, m_GlassDat.m_dFilmActiveRectEY,
		m_RobotDat.m_dCellAlignMarkPoxX, m_RobotDat.m_dCellAlignMarkPoxY, m_RobotDat.m_dFilmAlignMarkPosX, m_RobotDat.m_dFilmAlignMarkPosY,
		m_RobotDat.m_dFilmActiveRectSX, m_RobotDat.m_dFilmActiveRectSY, m_RobotDat.m_dFilmActiveRectEX, m_RobotDat.m_dFilmActiveRectEY);


	//m_RobotDat.m_dGlassAlignMarkPoxX0 = m_GlassDat.m_dGlassAlignMarkPoxX0 + offset_x;//Glass X축은 -위치이다(축이움직이므로)
	//m_RobotDat.m_dGlassAlignMarkPoxY0 = m_GlassDat.m_dGlassAlignMarkPoxY0 + offset_y;
	//m_RobotDat.m_dGlassAlignMarkPoxX1 = m_GlassDat.m_dGlassAlignMarkPoxX1 + offset_x;//Glass X축은 -위치이다(축이움직이므로)
	//m_RobotDat.m_dGlassAlignMarkPoxY1 = m_GlassDat.m_dGlassAlignMarkPoxY1 + offset_y;

	//for(i = 0; i < GLASS_MAX_CELL_NUM; i++)
	//{
	//	m_RobotDat.m_dCellAlignMarkPoxX[i] = m_GlassDat.m_dCellAlignMarkPoxX[i] + offset_x;//Glass X축은 -위치이다(축이움직이므로)
	//	m_RobotDat.m_dCellAlignMarkPoxY[i] = m_GlassDat.m_dCellAlignMarkPoxY[i] + offset_y;

	//	m_RobotDat.m_dFilmAlignMarkPosX[i] = m_GlassDat.m_dFilmAlignMarkPosX[i] + offset_x;//Glass X축은 -위치이다(축이움직이므로)
	//	m_RobotDat.m_dFilmAlignMarkPosY[i] = m_GlassDat.m_dFilmAlignMarkPosY[i] + offset_y;

	//	m_RobotDat.m_dFilmActiveRectSX[i] = m_GlassDat.m_dFilmActiveRectSX[i] + offset_x;//Glass X축은 -위치이다(축이움직이므로)
	//	m_RobotDat.m_dFilmActiveRectSY[i] = m_GlassDat.m_dFilmActiveRectSY[i] + offset_y;
	//	m_RobotDat.m_dFilmActiveRectEX[i] = m_GlassDat.m_dFilmActiveRectEX[i] + offset_x;//Glass X축은 -위치이다(축이움직이므로)
	//	m_RobotDat.m_dFilmActiveRectEY[i] = m_GlassDat.m_dFilmActiveRectEY[i] + offset_y;
	//}
}

void CDialogGlass::GetCoordGlassToRobot(double gx, double gy, double *rx, double *ry)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	double offset_x, offset_y;
	int mark_idx, i;
	mark_idx = 0;//첫번째 마크가지고 좌표변환

	//Glass는 X, Y 좌표계(수학)
	offset_x = pdlg->m_Align->GetAlignOffsetPosX_coordXY() - pdlg->m_RecipePanel->GetMarkPosX_coordXY(mark_idx);
	offset_y = pdlg->m_Align->GetAlignOffsetPosY_coordXY() - pdlg->m_RecipePanel->GetMarkPosY_coordXY(mark_idx);

	::GetCoordGlassToRobot(offset_x, offset_y, gx, gy, rx, ry);
	//*rx = gx + offset_x;//Glass X축은 -위치이다(축이움직이므로)
	//*ry = gy + offset_y;
}

void CDialogGlass::CoordRobotToDisp(double robo_x, double robo_y, double *rx, double *ry)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	double x, y;
	double offset_x, offset_y;
	int mark_idx;
	mark_idx = 0;//첫번째 마크가지고 좌표변환

	//Glass는 X, Y 좌표계(수학)
	offset_x = pdlg->m_Align->GetAlignOffsetPosX_coordXY() - pdlg->m_RecipePanel->GetMarkPosX_coordXY(mark_idx);
	offset_y = pdlg->m_Align->GetAlignOffsetPosY_coordXY() - pdlg->m_RecipePanel->GetMarkPosY_coordXY(mark_idx);

	//::CoordRobotToDisp(pdlg->m_RecipePanel->GetPanelSizeX_coordXY(), pdlg->m_RecipePanel->GetPanelSizeY_coordXY(),
	//	GLASS_START_OFFSET_X_MM, GLASS_START_OFFSET_Y_MM, offset_x, offset_y, GLASS_PIXEL_RATIO, GLASS_PIXEL_RATIO, robo_x, robo_y, rx, ry);

	x = robo_x - offset_x;
	y = robo_y - offset_y;

	GetCoordGlassToDisp(x, y, rx, ry);
}

//X, Y좌표계(수학좌표계)
double CDialogGlass::GetCellRobotPosX_coordXY(int cell_col_idx, int cell_row_idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	//인자는 1부터임.
	double pos;

	int idx;

	CoordGlassToRobotAll();

	//열
	cell_col_idx--;
	if(0 > cell_col_idx)
	{
		cell_col_idx = 0;
		//Error log

	}

	//행
	cell_row_idx--;
	if(0 > cell_row_idx)
	{
		cell_row_idx = 0;
		//Error log

	}

	idx = pdlg->m_RecipePanel->GetCellCntX_coordXY() * cell_col_idx + cell_row_idx;
	if(GLASS_MAX_CELL_NUM <= idx)
	{
		idx = GLASS_MAX_CELL_NUM - 1;
	}

	pos  = (m_RobotDat.m_dCellAlignMarkPoxX[idx]);

	return pos;
}

double CDialogGlass::GetCellRobotPosY_coordXY(int cell_col_idx, int cell_row_idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	//인자는 1부터임.
	double pos;

	int idx;

	CoordGlassToRobotAll();

	//열
	cell_col_idx--;
	if(0 > cell_col_idx)
	{
		cell_col_idx = 0;
		//Error log

	}

	//행
	cell_row_idx--;
	if(0 > cell_row_idx)
	{
		cell_row_idx = 0;
		//Error log

	}

	idx = pdlg->m_RecipePanel->GetCellCntX_coordXY() * cell_col_idx + cell_row_idx;
	if(GLASS_MAX_CELL_NUM <= idx)
	{
		idx = GLASS_MAX_CELL_NUM - 1;
	}

	pos  = (m_RobotDat.m_dCellAlignMarkPoxY[idx]);

	return pos;
}

double CDialogGlass::GetCellGrpRobotPosX_coordXY(int cell_col_idx, int cell_row_idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	//인자는 1부터임.
	double pos;
	int num = 0;
	int idx = 0;

	CoordGlassToRobotAll();

	//열
	cell_col_idx--;
	if(0 > cell_col_idx)
	{
		cell_col_idx = 0;
		//Error log

	}

	//행
	cell_row_idx--;
	if(0 > cell_row_idx)
	{
		cell_row_idx = 0;
		//Error log

	}

	if(0 < pdlg->m_RecipePanel->GetCellGrpCntX_coordXY())
	{
		num = pdlg->m_RecipePanel->GetCellCntX_coordXY() / pdlg->m_RecipePanel->GetCellGrpCntX_coordXY(); 
	}
	idx = num * cell_col_idx + cell_row_idx;
	if(GLASS_MAX_CELL_NUM <= idx)
	{
		idx = GLASS_MAX_CELL_NUM - 1;
	}
	
	pos  = (m_RobotDat.m_dFilmAlignMarkPosX[idx]);

	return pos;
}

double CDialogGlass::GetCellGrpRobotPosY_coordXY(int cell_col_idx, int cell_row_idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	//인자는 1부터임.
	double pos;
	int num = 0;
	int idx = 0;

	CoordGlassToRobotAll();

	//열
	cell_col_idx--;
	if(0 > cell_col_idx)
	{
		cell_col_idx = 0;
		//Error log

	}

	//행
	cell_row_idx--;
	if(0 > cell_row_idx)
	{
		cell_row_idx = 0;
		//Error log

	}

	if(0 < pdlg->m_RecipePanel->GetCellGrpCntX_coordXY())
	{
		num = pdlg->m_RecipePanel->GetCellCntX_coordXY() / pdlg->m_RecipePanel->GetCellGrpCntX_coordXY(); 
	}
	idx = num * cell_col_idx + cell_row_idx;
	if(GLASS_MAX_CELL_NUM <= idx)
	{
		idx = GLASS_MAX_CELL_NUM - 1;
	}

	pos  = (m_RobotDat.m_dFilmAlignMarkPosY[idx]);

	return pos;
}


void CDialogGlass::GetCellGrpRegionRobotPos_coordXY(int cell_col_idx, int cell_row_idx, double *sx, double *sy, double *ex, double *ey)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	//인자는 1부터임.
	int num = 0;
	int idx = 0;

	CoordGlassToRobotAll();

	//열
	cell_col_idx--;
	if(0 > cell_col_idx)
	{
		cell_col_idx = 0;
		//Error log

	}

	//행
	cell_row_idx--;
	if(0 > cell_row_idx)
	{
		cell_row_idx = 0;
		//Error log

	}

	if(0 < pdlg->m_RecipePanel->GetCellGrpCntX_coordXY())
	{
		//20160412 ngh 수정
		//num = pdlg->m_RecipePanel->GetCellCntX_coordXY() / pdlg->m_RecipePanel->GetCellGrpCntX_coordXY(); 
		num = pdlg->m_RecipeCell->GetFilmCntY();
	}
	
	idx = num * cell_col_idx + cell_row_idx;
	if(GLASS_MAX_CELL_NUM <= idx)
	{
		idx = GLASS_MAX_CELL_NUM - 1;
	}
	
	*sx  = (m_RobotDat.m_dFilmActiveRectSX[idx]);
	*sy  = (m_RobotDat.m_dFilmActiveRectSY[idx]);
	*ex  = (m_RobotDat.m_dFilmActiveRectEX[idx]);
	*ey  = (m_RobotDat.m_dFilmActiveRectEY[idx]);
}

void CDialogGlass::SetDrawCross(int x, int y)
{
	int cam_idx = 0;
	set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
	set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
	disp_line(m_dlgImgRect[cam_idx]->GetView(), y, 0, y, REVIEW_IMAGE_WIDTH);
	disp_line(m_dlgImgRect[cam_idx]->GetView(), 0, x,REVIEW_IMAGE_HEIGHT, x);
}

void CDialogGlass::SetLotInit()
{
	int i;

	for(i = 0; i < RECIPE_SCAN_TYPE_MAX_NUM; i++)
	{
		if(m_DftGlassDat_coordXY[i] != NULL)
		{
			delete m_DftGlassDat_coordXY[i];
		}

		m_DftGlassDat_coordXY[i] = new typeDefectData[DEFECT_DATA_MAX_NUM];
		memset(m_DftGlassDat_coordXY[i], NULL, sizeof(typeDefectData) * DEFECT_DATA_MAX_NUM);

		if(m_DftScanDat_scan[i] != NULL)
		{
			delete m_DftScanDat_scan[i];
		}
		
		m_DftScanDat_scan[i] = new typeDefectData[DEFECT_DATA_MAX_NUM];
		memset(m_DftScanDat_scan[i], NULL, sizeof(typeDefectData) * DEFECT_DATA_MAX_NUM);

		memset(&m_ScanManage[i], NULL, sizeof(typeScanManage));
	}

	DispGlassDat();
}


void CDialogGlass::SetDrawCrossRobot(int x, int y)
{
	int cam_idx = 0;
	double rx, ry;

	CoordRobotToDisp(x, y, &rx, &ry);

	set_insert(m_dlgImgRect[cam_idx]->GetView(), "xor");
	set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
	set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
	disp_line(m_dlgImgRect[cam_idx]->GetView(), ry, 0, ry, GLASS_IMAGE_WIDTH);
	disp_line(m_dlgImgRect[cam_idx]->GetView(), 0, rx, GLASS_IMAGE_HEIGHT, rx);
}

void CDialogGlass::SetDrawCrossReviewX(int x, int y)
{
	int cam_idx = 0;
	double rx, ry;

	set_insert(m_dlgImgRect[cam_idx]->GetView(), "xor");
	set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
	set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));

	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	m_dDestRobot_y = x;

	CoordRobotToDisp(x, y, &rx, &ry);

	disp_line(m_dlgImgRect[cam_idx]->GetView(), ry, 0, ry, GLASS_IMAGE_WIDTH);
}

void CDialogGlass::SetDrawCrossReviewY(int x, int y)
{
	int cam_idx = 0;
	double rx, ry;

	set_insert(m_dlgImgRect[cam_idx]->GetView(), "xor");
	set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
	set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	m_dDestRobot_x = y;
	CoordRobotToDisp(x, y, &rx, &ry);

	disp_line(m_dlgImgRect[cam_idx]->GetView(), 0, rx, GLASS_IMAGE_HEIGHT, rx);
}

double CDialogGlass::GetCellActiveRectSX_coordXY(int idx)
{
	return m_GlassDat.m_dCellActiveRectSX[idx];
}

double CDialogGlass::GetCellActiveRectSY_coordXY(int idx)
{
	return m_GlassDat.m_dCellActiveRectSY[idx];
}

double CDialogGlass::GetCellActiveRectEX_coordXY(int idx)
{
	return m_GlassDat.m_dCellActiveRectEX[idx];
}

double CDialogGlass::GetCellActiveRectEY_coordXY(int idx)
{
	return m_GlassDat.m_dCellActiveRectEY[idx];
}



double CDialogGlass::GetCVDActiveRectSX_coordXY(int idx)
{
	return m_GlassDat.m_dCVDActiveRectSX[idx];
}

double CDialogGlass::GetCVDActiveRectSY_coordXY(int idx)
{
	return m_GlassDat.m_dCVDActiveRectSY[idx];
}

double CDialogGlass::GetCVDActiveRectEX_coordXY(int idx)
{
	return m_GlassDat.m_dCVDActiveRectEX[idx];
}

double CDialogGlass::GetCVDActiveRectEY_coordXY(int idx)
{
	return m_GlassDat.m_dCVDActiveRectEY[idx];
}

double CDialogGlass::GetCVDDNActiveRectSX_coordXY(int idx)
{
	return m_GlassDat.m_dCVDDNActiveRectSX[idx];
}

double CDialogGlass::GetCVDDNActiveRectSY_coordXY(int idx)
{
	return m_GlassDat.m_dCVDDNActiveRectSY[idx];
}

double CDialogGlass::GetCVDDNActiveRectEX_coordXY(int idx)
{
	return m_GlassDat.m_dCVDDNActiveRectEX[idx];
}

double CDialogGlass::GetCVDDNActiveRectEY_coordXY(int idx)
{
	return m_GlassDat.m_dCVDDNActiveRectEY[idx];
}




double CDialogGlass::GetPCLActiveRectSX_coordXY(int rect_idx, int type, int idx)
{
	return m_GlassDat.m_dPCLActiveRectSX[rect_idx][type][idx];
}

double CDialogGlass::GetPCLActiveRectSY_coordXY(int rect_idx, int type, int idx)
{
	return m_GlassDat.m_dPCLActiveRectSY[rect_idx][type][idx];
}

double CDialogGlass::GetPCLActiveRectEX_coordXY(int rect_idx, int type, int idx)
{
	return m_GlassDat.m_dPCLActiveRectEX[rect_idx][type][idx];
}

double CDialogGlass::GetPCLActiveRectEY_coordXY(int rect_idx, int type, int idx)
{
	return m_GlassDat.m_dPCLActiveRectEY[rect_idx][type][idx];
}





void CDialogGlass::SetDefectData_scan(int scan_type_idx, typeDefectData dat)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	int base_defect_idx;
	int defect_idx;
	int i;

	double offset_x, offset_y;
	double ratio_x, ratio_y;

	//Index///////////////////////////////////////////////////////
	if(0 > dat.m_iScanIdx)
	{
		dat.m_iScanIdx = 0;
	}
	else if(DEFECT_MAX_SCAN_NUM <= dat.m_iScanIdx)
	{
		dat.m_iScanIdx = DEFECT_MAX_SCAN_NUM - 1;
	}
	else
	{
		dat.m_iScanIdx = dat.m_iScanIdx;
	}

	if(0 > dat.m_iDefectIdxPerScan)
	{
		dat.m_iDefectIdxPerScan = 0;
	}
	else if(DEFECT_DATA_MAX_NUM <= dat.m_iDefectIdxPerScan)
	{
		dat.m_iDefectIdxPerScan = DEFECT_DATA_MAX_NUM - 1;
	}
	else
	{
		dat.m_iDefectIdxPerScan = dat.m_iDefectIdxPerScan;
	}

	//Manage index
	if(m_ScanManage[scan_type_idx].m_iLastDefectIdxPerScan[dat.m_iScanIdx] < dat.m_iDefectIdxPerScan)
	{
		m_ScanManage[scan_type_idx].m_iLastDefectIdxPerScan[dat.m_iScanIdx] = dat.m_iDefectIdxPerScan;
	}
	if(m_ScanManage[scan_type_idx].m_iLastScanNum < dat.m_iScanIdx)
	{
		m_ScanManage[scan_type_idx].m_iLastScanNum = dat.m_iScanIdx;
	}
	
	base_defect_idx = 0;
	for(i = 0; i < dat.m_iScanIdx; i++)
	{
		base_defect_idx = base_defect_idx + m_ScanManage[scan_type_idx].m_iLastDefectIdxPerScan[i];
	}

	
	defect_idx = base_defect_idx + dat.m_iDefectIdx;
	if(DEFECT_DATA_MAX_NUM <= defect_idx)
	{
		defect_idx = DEFECT_DATA_MAX_NUM - 1;
	}

	//입력
	m_DftScanDat_scan[scan_type_idx][defect_idx] = dat;
	m_DftScanDat_scan[scan_type_idx][defect_idx].m_iDefectIdx = defect_idx;
	m_DftScanDat_scan[scan_type_idx][defect_idx].m_iScanIdx = dat.m_iScanIdx;
	m_DftScanDat_scan[scan_type_idx][defect_idx].m_bBlackWhite = dat.m_bBlackWhite;
	m_DftScanDat_scan[scan_type_idx][defect_idx].m_bBeforeAfterCompare = FALSE;
	m_DftScanDat_scan[scan_type_idx][defect_idx].m_bOFDefect = FALSE;

	if(m_ScanManage[scan_type_idx].m_iLastDefectIdx < defect_idx)
	{
		m_ScanManage[scan_type_idx].m_iLastDefectIdx = defect_idx;
	}
	
	//Glass data로 변환해서입력필요

}

void CDialogGlass::SetMakeDefectGlassData(int scan_type_idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	typedefectinfo defInfo;
	CString strLog;
	CString file_path, data, file_name, temp_data, temp;
	CString strCell_id ,strGlass_id;  //sh	
	strGlass_id.Format(_T(""));  //sh
	strCell_id.Format(_T(""));  //sh
	int i;
	int scan_idx = 0;
	int a_measure_y = -(pdlg->m_RecipePanel->GetPanelSizeX_coordXY() * 0.5); //sh A-cut 일때 기준 glass 좌표 y값 (E2좌표계 기준)
	int size_range_min = 0, size_range_max = 0; //sh defect size 구분 경계값
	CRect rect;
	CTime m_time = CTime::GetCurrentTime();  //sh
	CString strTime;  //sh
	CString strSizeret ,strRank ,strInout; //sh size 구분값 
	double size_ret_small = 0;  //size 구분 리턴값
	double size_ret_medium = 0;
	double size_ret_large = 0;
	double cx, cy, glass_x, glass_y, width, height, area;
	double measure_y_e2, measure_x_e2, measure_y_ap2, measure_x_ap2;  //sh  E2좌표 AP2좌표 변수
	double gray_25,gray_median,gray_75;
	double size_long; //sh 장축 길이 변수
	double dret = 0;
	double area_per = 0.;
	BOOL bGlassinfo = TRUE; //sh Glass A-cut,B-cut 판단 유무 A-cut이면 TRUE 
	CString strUpDw;
	CString strDefectList;
	//typeGlassDataPara glassData;
	//glassData = pdlg->m_Inline->GetLoaderGlassData();
	temp.Format(_T("%S"), pdlg->m_Inline->GetUnloaderGlassData().strGlassID);
	AfxExtractSubString(strGlass_id, temp, 0, ' ');

	typeGlassDataPara glass_data = pdlg->m_Inline->GetLoaderGlassData();

	double mark_x = 0., mark_y = 0., cad_x = 0., cad_y = 0.;
	double spec_x, spec_y, delta_x = 0., delta_y = 0., spec;
	double def_width = 0., def_height = 0.;
	CString strResult_x, strResult_y, strBlackWhite;
	char strFile[512];
	FILE *fp;
	CTime time;
	time = CTime::GetCurrentTime();
	strLog.Format(_T("SetMakeDefectGlassData Start(%d)"), scan_type_idx);
	pdlg->WriteLog(GLASS_LOG, strLog);
	spec_x = 200.;
	spec_y = 200.;

	
	//20160811 ngh
	double dam[2];

	cx = 0.;
	cy = 0.;
	glass_x = 0.;
	glass_y = 0.;
	width = 0.;
	height = 0.;
	area = 0.;

	int def_idx = 0;
	
	int compare_cnt = 0;
	double pixel_size_x, pixel_size_y, pixel_size;
	double dSpec_x;
	double dSpec_y;
	double dSpec;

	//20160911 trisys
	CString strTotalData;
	int total_idx = 0;


	strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),m_time.GetYear(),m_time.GetMonth(),m_time.GetDay(),m_time.GetHour(),m_time.GetMinute(),m_time.GetSecond()); //sh

	switch (scan_type_idx)
	{
		case 0:						//8k
		case 1:						//8k 90
			file_path.Format(_T("%s0_8K_%d.csv"), pdlg->GetResultFilePath(), scan_type_idx);
			//data.Format(_T("GLASS_ID,TIME,INDEX,CELL_ID,MEASURE_X_E2(mm),MEASURE_Y_E2(mm),MEASURE_X_AP2(mm),MEASURE_Y_AP2(mm),SIZE_LONG(um),AREA(um^2),SIZE_RANGE,RANK,IN_OUT,GRAY_MIN,GRAY_25,GRAY_MEDIEAN,GRAY_75,GRAY_MAX,GRAY_MEAN,GRAY_DEVIATION,BlackWhite,Image_Path\n"));//sh data.Format(_T("INDEX,WIDTH(mm),HEIGHT(mm),CENTER X(mm),CENTER Y(mm),AREA(mm)"));
			data.Format(_T("GLASS_ID,TIME,INDEX,CELL_ID,DEFECT_WIDTH(mm),DEFECT_HEIGHT(mm),MEASURE_X_E2(mm),MEASURE_Y_E2(mm),MEASURE_X_AP2(mm),MEASURE_Y_AP2(mm),SIZE_LONG(um),AREA(um^2),AREA(%%),SIZE_RANGE,RANK,IN_OUT,GRAY_MIN,GRAY_25,GRAY_MEDIEAN,GRAY_75,GRAY_MAX,GRAY_MEAN,GRAY_DEVIATION,BlackWhite,Image_Path\n"));//sh data.Format(_T("INDEX,WIDTH(mm),HEIGHT(mm),CENTER X(mm),CENTER Y(mm),AREA(mm)"));

			file_name.Format(_T("0_8K_%d.csv"), scan_type_idx);

			
			//20160126 ngh
			if(pdlg->m_Inline->FileSearch(pdlg->GetResultFilePath(), file_name))
			{
				pdlg->m_Inline->FileDelete(pdlg->GetResultFilePath(), file_name);
			}

			sprintf(strFile, "%S", file_path);
			fp = fopen( strFile, "a+" ); 

			if(fp == NULL)
			{
				return;
			}

			pixel_size_x = pdlg->m_RecipeScan->GetScanPixelX(scan_type_idx);
			pixel_size_y = pdlg->m_RecipeScan->GetScanPixelY(scan_type_idx);

			pixel_size = (pixel_size_x + pixel_size_y) / 2;

			//20160911 trisys
			if(scan_type_idx == 1)
			{
				total_idx = m_ScanManage[0].m_iLastDefectIdx + 1;
			}
		
			for(i = 0; (i <= m_ScanManage[scan_type_idx].m_iLastDefectIdx) && (i < DEFECT_DATA_MAX_NUM); i++)
			{
				//def_idx = i;
				cx = m_DftScanDat_scan[scan_type_idx][i].m_dCntX;
				cy = m_DftScanDat_scan[scan_type_idx][i].m_dCntY;

				width = m_DftScanDat_scan[scan_type_idx][i].m_dWidth;
				height = m_DftScanDat_scan[scan_type_idx][i].m_dHeight;
				area = m_DftScanDat_scan[scan_type_idx][i].m_dArea * pixel_size_x;

				

				scan_idx = m_DftScanDat_scan[scan_type_idx][i].m_iScanIdx;
				pdlg->m_RecipeScan->GetCoordGlassXYfromScan_coordXY(scan_type_idx, scan_idx, pdlg->m_RecipeScan->GetScanPitch_coordXY(scan_type_idx), cx, cy, &glass_x, &glass_y);

				m_DftGlassDat_coordXY[scan_type_idx][def_idx] = m_DftScanDat_scan[scan_type_idx][i];

				m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dCntX = glass_x;
				m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dCntY = glass_y;

				def_width = m_DftScanDat_scan[scan_type_idx][i].m_dWidth * pdlg->m_RecipeScan->GetScanPixelX(scan_type_idx);
				def_height = m_DftScanDat_scan[scan_type_idx][i].m_dHeight * pdlg->m_RecipeScan->GetScanPixelY(scan_type_idx);

				if(i == 34)
				{
					i = 34;
				}
				//strCell_id = GetInsActive_id(glass_x,glass_y,CELL_ID_8K,0);
				strCell_id.Format(_T("%s"), GetInsActive_id(glass_x,glass_y,CELL_ID_8K,0, 0));

				sprintf(m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_cCellID, "%S", strCell_id);

				//글레스id에서 a,b cut 확인 //20160711 ngh
				CString cuttemp; //20160728 sc
				cuttemp = strGlass_id.Right(1); //20160728 sc
				if(cuttemp == 'A') //20160728 sc
				{
					bGlassinfo = TRUE;
				}
				else
				{
					bGlassinfo = FALSE;
				}

				if(bGlassinfo)  //sh  A-cut
				{
					measure_x_e2 = m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dCntY;  //sh
					measure_x_ap2 = measure_x_e2;		//sh Glass가 A-cut일 경우에 E2 좌표와 AP2 좌표가 같음


					if((m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dCntX < 0)&&(m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dCntX >= a_measure_y))  //sh
					{	
						measure_y_e2 = abs(m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dCntX) + a_measure_y;  //sh
						measure_y_ap2 = measure_y_e2;
					}
					else if(m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dCntX < a_measure_y)  //sh
					{
						measure_y_e2 = abs(m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dCntX) + a_measure_y;  //sh
						measure_y_ap2 = measure_y_e2;
					}
					else
					{

					}
				}
				else    //sh B-cut
				{
					measure_x_e2 = m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dCntY;  //sh
					measure_x_ap2 = -(pdlg->m_RecipePanel->GetPanelSizeY_coordXY() - measure_x_e2);		//sh 


					if((m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dCntX < 0)&&(m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dCntX >= a_measure_y))  //sh
					{	
						measure_y_e2 = abs(m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dCntX) + a_measure_y;  //sh
						measure_y_ap2 = measure_y_e2;
					}
					else if(m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dCntX < a_measure_y)  //sh
					{
						measure_y_e2 = abs(m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dCntX) + a_measure_y;  //sh
						measure_y_ap2 = measure_y_e2;
					}
					else
					{

					}
				}


				//20160720 ngh 수정 필요
				//defect compare
				CdRect long_dRect, short_dRect;
				BOOL bCompare = FALSE;
				if(scan_type_idx == 1)
				{
					short_dRect.SetdRectCWH(m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dCntX, m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dCntY,
							def_width + ((pdlg->m_RecipeScan->GetDefectCompareDistance(scan_type_idx) * 2.0)),
							def_height + ((pdlg->m_RecipeScan->GetDefectCompareDistance(scan_type_idx) * 2.0)));

					for(int def_cnt = 0; def_cnt < (m_ScanManage[0].m_iLastDefectIdx); def_cnt++)
					{
						long_dRect.SetdRectCWH(m_DftGlassDat_coordXY[0][def_cnt].m_dCntX, m_DftGlassDat_coordXY[0][def_cnt].m_dCntY,
							m_DftGlassDat_coordXY[0][def_cnt].m_dWidth + ((pdlg->m_RecipeScan->GetDefectCompareDistance(scan_type_idx) * 2.0)),
							m_DftGlassDat_coordXY[0][def_cnt].m_dHeight + ((pdlg->m_RecipeScan->GetDefectCompareDistance(scan_type_idx) * 2.0)));
						
						if(long_dRect.IntersectRect(short_dRect))
						{
							bCompare = TRUE;
							compare_cnt++;
							break;
						}
					}
				}
				if(bCompare)
				{
					continue;
				}

				
				m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_iDefectIdx = def_idx;
				m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_iDefectIdxPerScan = def_idx + 1;
				m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dWidth = width * pdlg->m_RecipeScan->GetScanPixelX_coordXY(scan_type_idx);
				m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dHeight = height * pdlg->m_RecipeScan->GetScanPixelY_coordXY(scan_type_idx);
				if(m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dWidth > m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dHeight) //sh
				{
					size_long = m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dWidth * 1000;
				}
				else
				{
					size_long = m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dHeight * 1000;					//sh
				}
				m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dArea = area * ((pdlg->m_RecipeScan->GetScanPixelX_coordXY(scan_type_idx) * pdlg->m_RecipeScan->GetScanPixelY_coordXY(scan_type_idx)) * 1000000);
				m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_bBlackWhite = m_DftScanDat_scan[scan_type_idx][i].m_bBlackWhite;

				//20160328 ngh
				if(m_DftScanDat_scan[scan_type_idx][def_idx].m_bBlackWhite)
				{
					pdlg->SetRsltSizeCntView(GetRsltSizeCntTypeArea(scan_type_idx, area), DEFECT_SIZE_BLACK_WHITE);
				}
				else
				{
					pdlg->SetRsltSizeCntView(GetRsltSizeCntTypeArea(scan_type_idx, area));
				}

				//20151210 ngh
				//sh pdlg->SetRsltSizeCntView(GetRsltSizeCntType(scan_type_idx, m_DftGlassDat_coordXY[scan_type_idx][i].m_dWidth, m_DftGlassDat_coordXY[scan_type_idx][i].m_dHeight));
				//pdlg->SetRsltSizeCntView(GetRsltSizeCntTypeArea(scan_type_idx, area)); //sh

				m_defect_size_rslt[def_idx] = GetRsltSizeCntTypeArea(scan_type_idx, area);

				size_ret_small = pdlg->m_RecipeScan->GetDefectSmall(scan_type_idx);
				size_ret_medium = pdlg->m_RecipeScan->GetDefectMedium(scan_type_idx);
				size_ret_large = pdlg->m_RecipeScan->GetDefectLarge(scan_type_idx);



				if(m_defect_size_rslt[def_idx] == DEFECT_SIZE_SMALL)
				{
					strSizeret.Format(_T("%.4f<AREA"),size_ret_small);
					strRank.Format(_T("S"));
				}
				else if(m_defect_size_rslt[def_idx] == DEFECT_SIZE_MEDIUM)
				{
					strSizeret.Format(_T("%.4f<=AREA<%.4f"),size_ret_small,size_ret_medium);
					strRank.Format(_T("M"));
				}
				else if(m_defect_size_rslt[def_idx] == DEFECT_SIZE_LARGE)
				{
					strSizeret.Format(_T("%.4f<=AREA<%.4f"),size_ret_medium,size_ret_large);
					strRank.Format(_T("L"));
				}
				else if(m_defect_size_rslt[def_idx] == DEFECT_SIZE_OVER_FLOW)
				{
					strSizeret.Format(_T("%.4f<=AREA"),size_ret_large);
					strRank.Format(_T("OL"));
				}

				if(scan_type_idx == 0)		//sh 8k
					strInout.Format(_T("IN"));

				gray_median = (m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymax - m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymin) / 2 + m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymin;  //sh
				gray_25 = (gray_median - m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymin) / 2 + m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymin;  //sh
				gray_75 = (m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymax - gray_median) / 2 + gray_median;  //sh

				//20160401 ngh
				switch(m_DftScanDat_scan[scan_type_idx][def_idx].m_bBlackWhite)
				{
				case BLACK_DEFECT:
					strBlackWhite.Format(_T("B"));
					break;
				case BLACK_WHITE_DEFECT:
					strBlackWhite.Format(_T("BW"));
					break;
				}

				//area = (def_width * 1000) * (def_height * 1000);
				area = area * 1000;
				area_per = ((area) / ((def_width * 1000) * (def_height * 1000))) * 100.0;

				CString imagefilename, temp_image_path, image_path;
				//lotid,glassid,panelid,unitid,procid,x,y,no);
				//image_path.Format(_T("\\\\%s\\result\\%04d%02d%02d\\%S\\%S\\Image\\8K_IMAGE\\"), pdlg->m_IPSCtrl->GetIPSIP(), time.GetYear(), time.GetMonth(), time.GetDay(), glass_data.strLotID, glass_data.strGlassID);
				image_path.Format(_T("%sImage\\8K_IMAGE\\"),pdlg->GetResultFilePath());  //20160725 pkh
				_MakeDirectories(image_path);
				imagefilename.Format(_T("%s%S_%S_%s_%s_%s_%.3f_%.3f_%d.JPG"), image_path, glass_data.strLotID, glass_data.strGlassID, strCell_id, _T("RRRRRRR"),
									_T("AAAAAAA"), measure_x_e2, measure_y_e2, def_idx + 1);

				temp_image_path.Format(_T("%S"), m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_sImgPath);

				pdlg->m_Dfserver->CopyFile_Df(temp_image_path, imagefilename);

				defInfo.glsid.Format(_T("%s"), strGlass_id);
				defInfo.time.Format(_T("%s"), strTime);
				defInfo.index.Format(_T("%d"), def_idx + 1);
				defInfo.cellid.Format(_T("%s"), strCell_id);
				defInfo.width = def_width;
				defInfo.height = def_height;
				defInfo.measure_x_e2 = measure_x_e2;
				defInfo.measure_y_e2 = measure_y_e2;
				defInfo.measure_x_ap2 = measure_x_ap2;
				defInfo.measure_y_ap2 = measure_y_ap2;
				defInfo.size_long = size_long;
				defInfo.area = area;
				//defInfo.areaper;
				defInfo.size_range.Format(_T("%s"), strSizeret);
				defInfo.rank.Format(_T("%s"), strRank);
				defInfo.in_out.Format(_T("%s"), strInout);
				defInfo.gray_min = m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymin;
				defInfo.gray_25 = gray_25;
				defInfo.gray_mediean = gray_median;
				defInfo.gray_75 = gray_75;
				defInfo.gray_max = m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymax;
				defInfo.gray_mean = m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymean;
				defInfo.gray_deviation = m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraydeviation;
				defInfo.black_white.Format(_T("%s"), strBlackWhite);
				defInfo.image_path.Format(_T("%s"), imagefilename);
				defInfo.width_pix = (int)width;  //20160728 sc
				defInfo.height_pix = (int)height; //20160728 sc
				defInfo.area_pix = (int)m_DftScanDat_scan[scan_type_idx][i].m_dArea; //20160728 sc
#ifdef DEFECT
				pdlg->m_Dfserver->SetDefectInfo(defInfo, def_idx);
#endif

				
				
				//sh data.Format(_T("%d,%.3f,%.3f,%.3f,%.3f,%.2f"),  i, m_DftGlassDat_coordXY[scan_type_idx][i].m_dWidth, m_DftGlassDat_coordXY[scan_type_idx][i].m_dHeight,m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX, m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntY,m_DftGlassDat_coordXY[scan_type_idx][i].m_dArea);
				//temp_data.Format(_T("%s%s,%s,%d,%s,%.3f,%.3f,%.3f,%.3f,%.2f,%.2f,%s,%s,%s,%d,%.2f,%.2f,%.2f,%d,%.2f,%.2f,%s,%S\n"), data, strGlass_id, strTime, i + 1, strCell_id, measure_x_e2, measure_y_e2, measure_x_ap2, measure_y_ap2, size_long, m_DftGlassDat_coordXY[scan_type_idx][i].m_dArea,strSizeret,strRank,strInout, (int)m_DftGlassDat_coordXY[scan_type_idx][i].m_dgraymin,gray_25,
				//	//data.Format(_T("%s,%s,%d,%.3f,%.3f,%.3f,%.3f,%.2f,%.2f,%s,%s,%s,%d,%.2f,%.2f,%.2f,%d,%.2f,%.2f"), strGlass_id, strTime, i, measure_x_e2, measure_y_e2, measure_x_ap2, measure_y_ap2, size_long, m_DftGlassDat_coordXY[scan_type_idx][i].m_dArea,strSizeret,strRank,strInout, (int)m_DftGlassDat_coordXY[scan_type_idx][i].m_dgraymin,gray_25,
				//	gray_median,gray_75,(int)m_DftGlassDat_coordXY[scan_type_idx][i].m_dgraymax,m_DftGlassDat_coordXY[scan_type_idx][i].m_dgraymean,m_DftGlassDat_coordXY[scan_type_idx][i].m_dgraydeviation, strBlackWhite, m_DftGlassDat_coordXY[scan_type_idx][i].m_sImgPath);
				temp_data.Format(_T("%s%s,%s,%d,%s,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.2f,%.2f,%.2f,%s,%s,%s,%d,%.2f,%.2f,%.2f,%d,%.2f,%.2f,%s,%s\n"), data, strGlass_id, strTime, def_idx + 1, strCell_id, def_width, def_height, measure_x_e2, measure_y_e2, measure_x_ap2, measure_y_ap2, size_long, area, area_per,strSizeret,strRank,strInout, (int)m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymin,gray_25,
					//data.Format(_T("%s,%s,%d,%.3f,%.3f,%.3f,%.3f,%.2f,%.2f,%s,%s,%s,%d,%.2f,%.2f,%.2f,%d,%.2f,%.2f"), strGlass_id, strTime, i, measure_x_e2, measure_y_e2, measure_x_ap2, measure_y_ap2, size_long, m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dArea,strSizeret,strRank,strInout, (int)m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymin,gray_25,
					gray_median,gray_75,(int)m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymax,m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymean,m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraydeviation, strBlackWhite, imagefilename);
				data.Format(_T("%s"), temp_data);


				//20160911 trisys
				if(scan_type_idx == 1)
				{
					if(strTotalData.IsEmpty())
					{
						temp_data.Format(_T("%s,%s,%d,%s,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.2f,%.2f,%.2f,%s,%s,%s,%d,%.2f,%.2f,%.2f,%d,%.2f,%.2f,%s,%s\n"), strGlass_id, strTime, total_idx + 1, strCell_id, def_width, def_height, measure_x_e2, measure_y_e2, measure_x_ap2, measure_y_ap2, size_long, area, area_per,strSizeret,strRank,strInout, (int)m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymin,gray_25,
						//data.Format(_T("%s,%s,%d,%.3f,%.3f,%.3f,%.3f,%.2f,%.2f,%s,%s,%s,%d,%.2f,%.2f,%.2f,%d,%.2f,%.2f"), strGlass_id, strTime, i, measure_x_e2, measure_y_e2, measure_x_ap2, measure_y_ap2, size_long, m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dArea,strSizeret,strRank,strInout, (int)m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymin,gray_25,
						gray_median,gray_75,(int)m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymax,m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymean,m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraydeviation, strBlackWhite, imagefilename);
					}
					else
					{
						temp_data.Format(_T("%s%s,%s,%d,%s,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.2f,%.2f,%.2f,%s,%s,%s,%d,%.2f,%.2f,%.2f,%d,%.2f,%.2f,%s,%s\n"), strTotalData, strGlass_id, strTime, total_idx + 1, strCell_id, def_width, def_height, measure_x_e2, measure_y_e2, measure_x_ap2, measure_y_ap2, size_long, area, area_per,strSizeret,strRank,strInout, (int)m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymin,gray_25,
						//data.Format(_T("%s,%s,%d,%.3f,%.3f,%.3f,%.3f,%.2f,%.2f,%s,%s,%s,%d,%.2f,%.2f,%.2f,%d,%.2f,%.2f"), strGlass_id, strTime, i, measure_x_e2, measure_y_e2, measure_x_ap2, measure_y_ap2, size_long, m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dArea,strSizeret,strRank,strInout, (int)m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymin,gray_25,
						gray_median,gray_75,(int)m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymax,m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraymean,m_DftGlassDat_coordXY[scan_type_idx][def_idx].m_dgraydeviation, strBlackWhite, imagefilename);
					}
					strTotalData.Format(_T("%s"), temp_data);
					total_idx++;
				}
				

				strDefectList.Format(_T("%04d,%s,%f,%f,%f,%s,%s"), def_idx, strCell_id, def_width, def_height, area, strRank, strBlackWhite);
				pdlg->m_DefectList->SetDefectListInfo(def_idx, strDefectList);

				def_idx++;

				int percent;

				percent = (int)((double)((double)i / (double)m_ScanManage[scan_type_idx].m_iLastDefectIdx) * 100.0);

				pdlg->m_ProcessBar->SetProcessPer(percent);

				if((i % 1000) == 0 && i != 0)
				{
					if(!pdlg->WriteInsResultFile(fp, file_path, data))
					{
						if(fp != NULL)
						{
							fclose(fp);
							return;
						}
					}

					data.Format(_T(""));
				}

				if(pdlg->GetDefectFileWriteBreak())
				{
					strLog.Format(_T("SetMakeDefectGlassData Break(%d)"), scan_type_idx);
					pdlg->WriteLog(GLASS_LOG, strLog);
					pdlg->m_strMainMsg.Format(_T("DefectFile Load & Save Break"));
					pdlg->m_btn_MainMsg.SetWindowTextW(pdlg->m_strMainMsg);

					if(!pdlg->WriteInsResultFile(fp, file_path, data))
					{
						if(fp != NULL)
						{
							fclose(fp);
							return;
						}
					}

					if(fp != NULL)
					{
						fclose(fp);
						return;
					}

				}
			}
			break;
		case 2:			//16k
		case 3:
		case 4:
			//20160811 ngh
			pdlg->m_RecipeScan->GetFactor(2, dam);


			if(pdlg->m_RecipeScan->GetInspect90Degree(scan_type_idx))
			{
				file_name.Format(_T("0_16K_1.csv"));
			}
			else
			{
				file_name.Format(_T("0_16K_0.csv"));
			}
			file_path.Format(_T("%s%s"), pdlg->GetResultFilePath(), file_name);

			//20160126 ngh
			if(pdlg->m_Inline->FileSearch(pdlg->GetResultFilePath(), file_name))
			{
				pdlg->m_Inline->FileDelete(pdlg->GetResultFilePath(), file_name);
			}

			sprintf(strFile, "%S", file_path);
			fp = fopen( strFile, "a+" ); 

			if(fp == NULL)
			{
				return;
			}

			//20160811 ngh
			data.Format(_T("GLASS_ID,TIME,INDEX,ID,2Dam<->CVD(up)(mm),2Dam<->CVD(dw)(mm),2DAM_CAD_X(mm),2DAM_CAD_Y(mm),MEASURE_X(mm),MEASURE_Y(mm),SPEC_X(mm),SPEC_Y(mm),DELTA_X(mm),DELTA_Y(mm),RESULT_X(mm),RESULT_Y(mm),POINT_X_E2(mm),POINT_Y_E2(mm),ORIGIN_POINT_X(mm),ORIGIN_POINT_X(mm),Image_Path\n"));//sh data.Format(_T("INDEX,WIDTH(mm),HEIGHT(mm),CENTER X(mm),CENTER Y(mm),AREA(mm)"));

			//data.Format(_T("GLASS_ID,TIME,INDEX,MEASURE_X_E2(mm),MEASURE_Y_E2(mm),MEASURE_X_AP2(mm),MEASURE_Y_AP2(mm),SIZE_LONG(um),AREA(um^2),SIZE_RANGE,RANK,IN_OUT,GRAY_MIN,GRAY_25,GRAY_MEDIEAN,GRAY_75,GRAY_MAX,GRAY_MEAN,GRAY_DEVIATION"));//sh data.Format(_T("INDEX,WIDTH(mm),HEIGHT(mm),CENTER X(mm),CENTER Y(mm),AREA(mm)"));
			int a, b;
			int j;
			m_ScanManage[scan_type_idx].m_iCVDNgCnt = 0;

			
			//if(bGlassinfo)  //sh
			for(i = 0; (i <= m_ScanManage[scan_type_idx].m_iLastDefectIdx) && (i < DEFECT_DATA_MAX_NUM); i++)
			{
				cx = m_DftScanDat_scan[scan_type_idx][i].m_dCntX;
				cy = m_DftScanDat_scan[scan_type_idx][i].m_dCntY;

				width = m_DftScanDat_scan[scan_type_idx][i].m_dWidth;
				height = m_DftScanDat_scan[scan_type_idx][i].m_dHeight;
				area = m_DftScanDat_scan[scan_type_idx][i].m_dArea;

				scan_idx = m_DftScanDat_scan[scan_type_idx][i].m_iScanIdx;
				pdlg->m_RecipeScan->GetCoordGlassXYfromScan_coordXY(scan_type_idx, scan_idx, pdlg->m_RecipeScan->GetScanPitch_coordXY(scan_type_idx), cx, cy, &glass_x, &glass_y);

				m_DftGlassDat_coordXY[scan_type_idx][i] = m_DftScanDat_scan[scan_type_idx][i];

				m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX = glass_x;// + 14.0;
				m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntY = glass_y;// + 14.0;

				measure_x_e2 = m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntY;  //sh
				measure_x_ap2 = measure_x_e2;		//sh Glass가 A-cut일 경우에 E2 좌표와 AP2 좌표가 같음
			

				//20160811 ngh
				//////////////////////////////////////////////////////////////////
				//글레스id에서 a,b cut 확인 //20160711 ngh
				CString cuttemp; //20160728 sc
				cuttemp = strGlass_id.Right(1); //20160728 sc
				if(cuttemp == 'A') //20160728 sc
				{
					bGlassinfo = TRUE;
				}
				else
				{
					bGlassinfo = FALSE;
				}

				if(bGlassinfo)  //sh  A-cut
				{
					measure_x_e2 = m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntY;  //sh
					measure_x_ap2 = measure_x_e2;		//sh Glass가 A-cut일 경우에 E2 좌표와 AP2 좌표가 같음

					if((m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX < 0)&&(m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX >= a_measure_y))  //sh
					{	
						measure_y_e2 = abs(m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX) + a_measure_y;  //sh
						measure_y_ap2 = measure_y_e2;
					}
					else if(m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX < a_measure_y)  //sh
					{
						measure_y_e2 = abs(m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX) + a_measure_y;  //sh
						measure_y_ap2 = measure_y_e2;
					}
					else
					{

					}
				}
				else    //sh B-cut
				{
					measure_x_e2 = m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntY;  //sh
					measure_x_ap2 = -(pdlg->m_RecipePanel->GetPanelSizeY_coordXY() - measure_x_e2);		//sh 


					if((m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX < 0)&&(m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX >= a_measure_y))  //sh
					{	
						measure_y_e2 = abs(m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX) + a_measure_y;  //sh
						measure_y_ap2 = measure_y_e2;
					}
					else if(m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX < a_measure_y)  //sh
					{
						measure_y_e2 = abs(m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX) + a_measure_y;  //sh
						measure_y_ap2 = measure_y_e2;
					}
					else
					{

					}
				}
				//////////////////////////////////////////////////////////////////

				strUpDw.Format(_T(""));
				strUpDw = GetCVDIndex(scan_type_idx, i);

				strCell_id.Format(_T("%s"), strUpDw.Mid(0, strUpDw.GetLength() - 2));

				sprintf(m_DftGlassDat_coordXY[scan_type_idx][i].m_cCellID, "%S", strCell_id);
					
				cad_x = 0.;
				cad_y = 0.;
				
				GetCVDCadValue(scan_type_idx, i, &cad_x, &cad_y, TRUE);
				//GetCVDCadValue(scan_type_idx, i, &cad_x, &cad_y, TRUE);
				cad_x = m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX;

				//결과값 정렬 필요
				dSpec = 0.;
				spec = 0.;
				delta_x = 0.;
				delta_y = 0.;
				if(!strUpDw.Mid(4,1).Compare(_T("T")))
				{
					spec = (double)pdlg->m_RecipeScan->GetDefectSize(scan_type_idx, 0);// / 1000.0;
					dSpec = pdlg->m_RecipeScan->GetDefectSmall(scan_type_idx);
					delta_x = abs(m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX) - abs(cad_x);
					delta_y = abs(m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntY) - abs(cad_y);
				}
				else
				{
					spec = (double)pdlg->m_RecipeScan->GetDefectSize(scan_type_idx, 1);// / 1000.0;
					dSpec = pdlg->m_RecipeScan->GetDefectMedium(scan_type_idx);
					delta_x = abs(cad_x) - abs(m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX);
					delta_y = abs(cad_y) - abs(m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntY);
				}

				if(dSpec >= delta_x)
				{
					strResult_x.Format(_T("OK"));
				}
				else
				{
					strResult_x.Format(_T("NG"));
				}

				if(dSpec >= delta_y)
				{
					strResult_y.Format(_T("OK"));
				}
				else
				{
					strResult_y.Format(_T("NG"));
					m_ScanManage[scan_type_idx].m_iCVDNgCnt++;
				}

				//20160527 ngh
				if(!strUpDw.Mid(4,1).Compare(_T("T")))
				{
					m_DftGlassDat_coordXY[scan_type_idx][i].m_CVDRslt = FALSE;
					m_DftGlassDat_coordXY[scan_type_idx][i].m_CVDUpDw = TRUE;
					m_DftGlassDat_coordXY[scan_type_idx][i].m_CVDUpRsltX = FALSE;
					m_DftGlassDat_coordXY[scan_type_idx][i].m_CVDUpRsltY = FALSE;
					if(!strResult_x.Compare(_T("OK")) && !strResult_y.Compare(_T("OK")))
					{
						m_DftGlassDat_coordXY[scan_type_idx][i].m_CVDUpRsltX = TRUE;
						m_DftGlassDat_coordXY[scan_type_idx][i].m_CVDUpRsltY = TRUE;
						m_DftGlassDat_coordXY[scan_type_idx][i].m_CVDRslt = TRUE;
					}
				}
				else
				{
					m_DftGlassDat_coordXY[scan_type_idx][i].m_CVDRslt = FALSE;
					m_DftGlassDat_coordXY[scan_type_idx][i].m_CVDUpDw = FALSE;
					m_DftGlassDat_coordXY[scan_type_idx][i].m_CVDDwRsltX = FALSE;
					m_DftGlassDat_coordXY[scan_type_idx][i].m_CVDDwRsltY = FALSE;
					if(!strResult_x.Compare(_T("OK")) && !strResult_y.Compare(_T("OK")))
					{
						m_DftGlassDat_coordXY[scan_type_idx][i].m_CVDDwRsltX = TRUE;
						m_DftGlassDat_coordXY[scan_type_idx][i].m_CVDDwRsltY = TRUE;
						m_DftGlassDat_coordXY[scan_type_idx][i].m_CVDRslt = TRUE;
					}
				}

				//20160711 ngh
				CString imagefilename, temp_image_path, image_path;
				//lotid,glassid,panelid,unitid,procid,x,y,no);
				if(scan_type_idx == 2)
				{
					//image_path.Format(_T("\\\\%s\\result\\%04d%02d%02d\\%S\\%S\\Image\\16K_IMAGE_CVD\\"), pdlg->m_IPSCtrl->GetIPSIP(), time.GetYear(), time.GetMonth(), time.GetDay(), glass_data.strLotID, glass_data.strGlassID);
					image_path.Format(_T("%sImage\\16K_IMAGE_CVD\\"),pdlg->GetResultFilePath());  //20160725 pkh
				}
				else
				{
					//image_path.Format(_T("\\\\%s\\result\\%04d%02d%02d\\%S\\%S\\Image\\16K_IMAGE_PCL\\"), pdlg->m_IPSCtrl->GetIPSIP(), time.GetYear(), time.GetMonth(), time.GetDay(), glass_data.strLotID, glass_data.strGlassID);
					image_path.Format(_T("%sImage\\16K_IMAGE_PCL\\"),pdlg->GetResultFilePath());  //20160725 pkh
				}
				_MakeDirectories(image_path);

				imagefilename.Format(_T("%s%S_%S_%s_%s_%s_%.3f_%.3f_%s.JPG"), image_path, glass_data.strLotID, glass_data.strGlassID, strUpDw, _T("RRRRRRR"),
										_T("AAAAAAA"), measure_x_e2, measure_y_e2, defInfo.index);
				temp_image_path.Format(_T("%S"), m_DftGlassDat_coordXY[scan_type_idx][i].m_sImgPath);

				if(!strUpDw.Mid(0, strUpDw.GetLength() - 2).Compare(_T("CB1")))
				{
					temp_image_path;
				}
				pdlg->m_Dfserver->CopyFile_Df(temp_image_path, imagefilename);

				sprintf(m_DftGlassDat_coordXY[scan_type_idx][i].m_sImgPath, "%S", imagefilename);

				mark_x = 0.;
				mark_y = 0.;

				//20160811 ngh
				//////////////////////////////////////////////////////////////////
				//"GLASS_ID,TIME,INDEX,ID,MARK_X,MARK_Y,CAD_X,CAD_Y,MEASURE_X,MEASURE_Y,SPEC_X,SPEC_Y,DELTA_X,DELTA_Y,RESULT_X,RESULT_Y,POINT_X_E2,POINT_Y_E2,ORIGIN_POINT_X,ORIGIN_POINT_X"
				temp_data.Format(_T("%s%s,%s,%d,%s,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%s,%s,%.3f,%.3f,%.3f,%.3f,%s\n"), 
					data, strGlass_id, strTime, i + 1, strUpDw, dam[0], dam[1], abs(cad_x), abs(cad_y), abs(m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX), abs(m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntY), 
					dSpec, dSpec, delta_x, delta_y, strResult_x, strResult_y, measure_x_e2, measure_y_e2, measure_x_ap2, measure_y_ap2,imagefilename);//m_DftGlassDat_coordXY[scan_type_idx][i].m_sImgPath);//결과
				//////////////////////////////////////////////////////////////////
				data.Format(_T("%s"), temp_data);

				int percent;

				percent = (int)((double)((double)i / (double)m_ScanManage[scan_type_idx].m_iLastDefectIdx) * 100.0);

				pdlg->m_ProcessBar->SetProcessPer(percent);

				if((i % 1000) == 0 && i != 0)
				{
					if(!pdlg->WriteInsResultFile(fp, file_path, data))
					{
						if(fp != NULL)
						{
							fclose(fp);
							return;
						}
					}
					data.Format(_T(""));
				}

				if(pdlg->GetDefectFileWriteBreak())
				{
					strLog.Format(_T("SetMakeDefectGlassData Break(%d)"), scan_type_idx);
					pdlg->WriteLog(GLASS_LOG, strLog);
					pdlg->m_strMainMsg.Format(_T("DefectFile Load & Save Break"));
					pdlg->m_btn_MainMsg.SetWindowTextW(pdlg->m_strMainMsg);
					if(!pdlg->WriteInsResultFile(fp, file_path, data))
					{
						if(fp != NULL)
						{
							fclose(fp);
							return;
						}
					}

					if(fp != NULL)
					{
						fclose(fp);
						return;
					}
				}
			}
		break;
	}

	if(!pdlg->WriteInsResultFile(fp, file_path, data))
	{
		if(fp != NULL)
		{
			fclose(fp);
			return;
		}
	}

	if(fp != NULL)
	{
		fclose(fp);
	}


	//20160911 trisys
	if(scan_type_idx == 1)
	{
		file_path.Format(_T("%s0_8K_0.csv"), pdlg->GetResultFilePath());

		sprintf(strFile, "%S", file_path);
		fp = fopen( strFile, "a+" ); 

		if(fp == NULL)
		{
			return;
		}

		if(!pdlg->WriteInsResultFile(fp, file_path, strTotalData))
		{
			if(fp != NULL)
			{
				fclose(fp);
				return;
			}
		}

		if(fp != NULL)
		{
			fclose(fp);
		}
	}

	m_ScanManage[scan_type_idx].m_iLastDefectIdx-=compare_cnt;

	strLog.Format(_T("SetMakeDefectGlassData End(%d)"), scan_type_idx);
	pdlg->WriteLog(GLASS_LOG, strLog);
}

void CDialogGlass::SetDispDefectGlassData(int scan_type_idx)
{
	DispGlassDat();

	Hobject region;
	int i;
	int cam_idx = 0;
	double cx, cy, sx, sy, ex, ey;
	cx = 0.;
	cy = 0.;
	sx = 0.;
	sy = 0.;
	ex = 0.;
	ey = 0.;

	double d_cx, d_cy, d_sx, d_sy, d_ex, d_ey;
	d_cx = 0.;
	d_cy = 0.;
	d_sx = 0.;
	d_sy = 0.;
	d_ex = 0.;
	d_ey = 0.;

	double l_sx, l_sy, l_ex, l_ey;
	l_sx = 0.;
	l_sy = 0.;
	l_ex = 0.;
	l_ey = 0.;

	if(scan_type_idx < 2)
	{
		for(i = 0; (i < m_ScanManage[scan_type_idx].m_iLastDefectIdx + 1) && (i < DEFECT_DATA_MAX_NUM); i++)
		{
			cx = m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX;
			cy = m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntY;
			sx = cx - (m_DftGlassDat_coordXY[scan_type_idx][i].m_dWidth * 0.5);
			sy = cy + (m_DftGlassDat_coordXY[scan_type_idx][i].m_dHeight * 0.5);	
			ex = cx + (m_DftGlassDat_coordXY[scan_type_idx][i].m_dWidth * 0.5);
			ey = cy - (m_DftGlassDat_coordXY[scan_type_idx][i].m_dHeight * 0.5);	

			GetCoordGlassToDisp(sx, sy, &d_sx, &d_sy);
			GetCoordGlassToDisp(ex, ey, &d_ex, &d_ey);

			//Display
			l_sx = d_sx + 0.5;
			l_sy = d_sy + 0.5;
			l_ex = d_ex + 0.5;
			l_ey = d_ey + 0.5;

			if(m_defect_size_rslt[i] == DEFECT_SIZE_SMALL)
			{
				set_color(m_dlgImgRect[cam_idx]->GetView(), "yellow");
				set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("fill"));
				gen_rectangle1(&region, l_sy, l_sx, l_ey, l_ex);
				disp_obj(region, m_dlgImgRect[cam_idx]->GetView());
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);
			}
			else if(m_defect_size_rslt[i] == DEFECT_SIZE_MEDIUM)
			{
				set_color(m_dlgImgRect[cam_idx]->GetView(), "blue");
				set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("fill"));
				gen_rectangle1(&region, l_sy, l_sx, l_ey, l_ex);
				disp_obj(region, m_dlgImgRect[cam_idx]->GetView());
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);
			}
			else if(m_defect_size_rslt[i] == DEFECT_SIZE_LARGE)
			{
				set_color(m_dlgImgRect[cam_idx]->GetView(), "red");
				set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("fill"));
				gen_rectangle1(&region, l_sy, l_sx, l_ey, l_ex);
				disp_obj(region, m_dlgImgRect[cam_idx]->GetView());
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);
			}
			else if(m_defect_size_rslt[i] == DEFECT_SIZE_OVER_FLOW)
			{
				set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
				set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("fill"));
				gen_rectangle1(&region, l_sy, l_sx, l_ey, l_ex);
				disp_obj(region, m_dlgImgRect[cam_idx]->GetView());
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);
			}
			else
			{
				set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
				set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("fill"));
				gen_rectangle1(&region, l_sy, l_sx, l_ey, l_ex);
				disp_obj(region, m_dlgImgRect[cam_idx]->GetView());
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
				disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);
			}

			if(m_DftScanDat_scan[scan_type_idx][i].m_bBlackWhite)
			{
				double l_bw_sx, l_bw_sy, l_bw_ex, l_bw_ey;

				l_bw_sx = l_sx - 3.0;
				l_bw_sy = l_sy - 3.0;
				l_bw_ex = l_ex + 3.0;
				l_bw_ey = l_ey + 3.0;

				set_color(m_dlgImgRect[cam_idx]->GetView(), "pink");
				set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
				gen_rectangle1(&region, l_bw_sy, l_bw_sx, l_bw_ey, l_bw_ex);
				disp_obj(region, m_dlgImgRect[cam_idx]->GetView());
			}
		}
	}
	else
	{
		//20160523 ngh
		for(i = 0; (i < m_ScanManage[scan_type_idx].m_iLastDefectIdx) && (i < DEFECT_DATA_MAX_NUM); i++)
		{
			cx = m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX;
			cy = m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntY;
			sx = cx - (m_DftGlassDat_coordXY[scan_type_idx][i].m_dWidth * 0.5);
			sy = cy + (m_DftGlassDat_coordXY[scan_type_idx][i].m_dHeight * 0.5);	
			ex = cx + (m_DftGlassDat_coordXY[scan_type_idx][i].m_dWidth * 0.5);
			ey = cy - (m_DftGlassDat_coordXY[scan_type_idx][i].m_dHeight * 0.5);	

			GetCoordGlassToDisp(sx, sy, &d_sx, &d_sy);
			GetCoordGlassToDisp(ex, ey, &d_ex, &d_ey);

			//Display
			l_sx = d_sx + 0.5;
			l_sy = d_sy + 0.5;
			l_ex = d_ex + 0.5;
			l_ey = d_ey + 0.5;

			if(m_DftGlassDat_coordXY[scan_type_idx][i].m_CVDRslt)
			{
				set_color(m_dlgImgRect[cam_idx]->GetView(), "green");
			}
			else
			{
				set_color(m_dlgImgRect[cam_idx]->GetView(), "red");
			}
			
			set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("fill"));
			gen_rectangle1(&region, l_sy, l_sx, l_ey, l_ex);
			disp_obj(region, m_dlgImgRect[cam_idx]->GetView());
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_sx, l_sy, l_ex);
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_sy, l_ex, l_ey, l_ex);
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_ex, l_ey, l_sx);
			disp_line(m_dlgImgRect[cam_idx]->GetView(), l_ey, l_sx, l_sy, l_sx);
		}
	}
}

int CDialogGlass::GetInRectInGlass(int scan_type_idx, double x, double y, double *cent_x, double *cent_y)
{
	int i;
	CdRect rect;
	CdPoint point;
	double cx, cy, sx, sy, ex, ey;
	cx = 0.;
	cy = 0.;
	sx = 0.;
	sy = 0.;
	ex = 0.;
	ey = 0.;

	*cent_x = 0.;
	*cent_y = 0.;

	for(i = 0; (i < m_ScanManage[scan_type_idx].m_iLastDefectIdx) && (i < DEFECT_DATA_MAX_NUM); i++)
	{
		rect.SetdRectCWH(m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX, m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntY,
			m_DftGlassDat_coordXY[scan_type_idx][i].m_dWidth, m_DftGlassDat_coordXY[scan_type_idx][i].m_dHeight, 2.0);
		if(rect.PointInRect(x, y))
		{
			point = rect.GetCenter();
			*cent_x = point.GetX();
			*cent_y = point.GetY();
			return i;
		}
		/*cx = m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX;
		cy = m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntY;
		sx = cx - (m_DftGlassDat_coordXY[scan_type_idx][i].m_dWidth * 0.5);
		sy = cy + (m_DftGlassDat_coordXY[scan_type_idx][i].m_dHeight * 0.5);	
		ex = cx + (m_DftGlassDat_coordXY[scan_type_idx][i].m_dWidth * 0.5);
		ey = cy - (m_DftGlassDat_coordXY[scan_type_idx][i].m_dHeight * 0.5);	
		
		DistXY(double rx, double ry,				
				  double ax, double ay,
				  double *distX, double *distY);
		if( (sx < x) && (ex > x) && (sy > y) && (ey < y))
		{
			*cent_x = cx;
			*cent_y = cy;
			return m_DftGlassDat_coordXY[scan_type_idx][i].m_iDefectIdx;
		}
		else
		{

		}*/
	}
	return -1;
}
		
typeDefectData CDialogGlass::GetDefectData_scan(int scan_type_idx, int idx)
{
	return m_DftScanDat_scan[scan_type_idx][idx];
}

typeDefectData CDialogGlass::GetDefectData_coordXY(int scan_type_idx, int idx)
{
	return m_DftGlassDat_coordXY[scan_type_idx][idx];
}

void CDialogGlass::SetDispRegion()
{
	int cam_idx;
	cam_idx = 0;

	set_color(m_dlgImgRect[cam_idx]->GetView(), "yellow");
	set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
	disp_obj(m_InspectRegion, m_dlgImgRect[cam_idx]->GetView());	
	set_color(m_dlgImgRect[cam_idx]->GetView(), "red");
	set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
	disp_obj(m_DefectRegion, m_dlgImgRect[cam_idx]->GetView());	
}

Hobject *CDialogGlass::GetDefectRgn()
{
	return &m_DefectRegion;
}

Hobject *CDialogGlass::GetInspectRgn()
{
	return &m_InspectRegion;
}

void CDialogGlass::SetDefectRgn(Hobject *region)
{
	copy_obj(*region, &m_DefectRegion, 1, -1);
}

void CDialogGlass::SetDispGlassDat()
{
	DispGlassDat();
}
/*
//20151210 ngh
int CDialogGlass::GetRsltSizeCntType(int type_idx, double width, double height)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	double ismall = pdlg->m_RecipeScan->GetDefectSmall(type_idx);
	double imedium = pdlg->m_RecipeScan->GetDefectMedium(type_idx);
	double ilarge = pdlg->m_RecipeScan->GetDefectLarge(type_idx);
	

	if((0 < width && width <= ismall) || (0 < height && height <= ismall))
	{
		return DEFECT_SIZE_SMALL;
	}
	else if((ismall < width && width <= imedium) || (ismall < height && height <= imedium))
	{
		return DEFECT_SIZE_MEDIUM;
	}
	else if((imedium  < width && width <= ilarge) || (imedium < height && height <= ilarge))
	{
		return DEFECT_SIZE_LARGE;
	}
	return DEFECT_SIZE_OVER_FLOW;
}
*/

double CDialogGlass::GetRsltSizeCntTypeArea(int type_idx, double Area)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	double ismall = pdlg->m_RecipeScan->GetDefectSmall(type_idx);
	double imedium = pdlg->m_RecipeScan->GetDefectMedium(type_idx);
	double ilarge = pdlg->m_RecipeScan->GetDefectLarge(type_idx);
	
	if(Area < ismall) 
	{
		return DEFECT_SIZE_SMALL;
	}
	else if((ismall <= Area && Area < imedium))
	{
		return DEFECT_SIZE_MEDIUM;
	}
	else if((imedium  <= Area && Area < ilarge))
	{
		return DEFECT_SIZE_LARGE;
	}
	return DEFECT_SIZE_OVER_FLOW;
}


BOOL CDialogGlass::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


CString CDialogGlass::GetInsActive_id(double x, double y, int scan_id_idx, int film_idx_x, int film_idx_y)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	int idx, sub_idx;
	int i,j;
	
	double offset_x = 1.2,offset_y = 1.2;
	//char *low , *col;
	idx = 0;
	sub_idx = 0;
	CString strId;
	//strId.Empty();
	strId.Format(_T(""));

	int idx_0 = 0;
	int idx_1 = 0;

	switch (scan_id_idx)
	{
	case CELL_ID_8K:
		for(i = 0; i < pdlg->m_RecipePanel->GetCellCntY_coordXY(); i++)
		{
			for(j = 0; j < pdlg->m_RecipePanel->GetCellCntX_coordXY();j++)
			{
				if(GLASS_MAX_CELL_NUM <= idx)
				{
					break;
				}

				m_GlassDat.m_dCellActiveRectSX[idx] = m_GlassDat.m_dCellAlignMarkPoxX[idx] + pdlg->m_RecipeCell->GetCellPosFromCellAlignX_coordXY();
				m_GlassDat.m_dCellActiveRectSY[idx] = m_GlassDat.m_dCellAlignMarkPoxY[idx] + pdlg->m_RecipeCell->GetCellPosFromCellAlignY_coordXY();

				m_GlassDat.m_dCellActiveRectEX[idx] = m_GlassDat.m_dCellActiveRectSX[idx] + fabs(pdlg->m_RecipeCell->GetCellActiveSizeX_coordXY());
				m_GlassDat.m_dCellActiveRectEY[idx] = m_GlassDat.m_dCellActiveRectSY[idx] - fabs(pdlg->m_RecipeCell->GetCellActiveSizeY_coordXY());

				if((m_GlassDat.m_dCellActiveRectSX[idx] - offset_x <= x) && (m_GlassDat.m_dCellActiveRectEX[idx] + offset_x >= x) && (m_GlassDat.m_dCellActiveRectSY[idx] + offset_y >= y) && (m_GlassDat.m_dCellActiveRectEY[idx] - offset_y <= y))
				{
					strId.Format(_T("%s"),m_strCell_table[idx_0][idx_1]);
					return strId;
				}
				idx++;
				idx_1++;

			}
			idx_1 = 0;
			idx_0++;
		//	sub_idx = sub_idx + pdlg->m_RecipePanel->GetCellCntX_coordXY();
		}
		
		break;
	case CELL_ID_8K_90:
		break;
	case CELL_ID_16K:
		break;
	case CELL_ID_16K_90:
		strId.Format(_T("C%s"),m_strCell_table[film_idx_x][film_idx_y]);
		break;
	case FILM_ID:
		strId.Format(_T("F%s"),m_strCell_table[film_idx_x][film_idx_y]);
		
		
				/*
				m_GlassDat.m_dFilmActiveRectSX[idx] = m_GlassDat.m_dFilmAlignMarkPosX[idx] + pdlg->m_RecipeCell->GetFilmPosFromFilmAlignX_coordXY();
				m_GlassDat.m_dFilmActiveRectSY[idx] = m_GlassDat.m_dFilmAlignMarkPosY[idx] + pdlg->m_RecipeCell->GetFilmPosFromFilmAlignY_coordXY();

				if(pdlg->m_RecipeCell->GetFilmGrpCntX_coordXY() > j)
				{
					m_GlassDat.m_dFilmActiveRectEX[idx] = m_GlassDat.m_dFilmActiveRectSX[idx] + pdlg->m_RecipeCell->GetFilmSizeX0_coordXY();
					m_GlassDat.m_dFilmActiveRectEY[idx] = m_GlassDat.m_dFilmActiveRectSY[idx] - pdlg->m_RecipeCell->GetFilmSizeY0_coordXY();
				}
				else
				{
					m_GlassDat.m_dFilmActiveRectEX[idx] = m_GlassDat.m_dFilmActiveRectSX[idx] + pdlg->m_RecipeCell->GetFilmSizeX1_coordXY();
					m_GlassDat.m_dFilmActiveRectEY[idx] = m_GlassDat.m_dFilmActiveRectSY[idx] - pdlg->m_RecipeCell->GetFilmSizeY1_coordXY();
				}*/
		
	
		break;
	}

	return strId;
}

double CDialogGlass::GetFilmActiveRectSX(int idx)
{
	return m_GlassDat.m_dFilmActiveRectSX[idx];
}

double CDialogGlass::GetFilmActiveRectSY(int idx)
{
	return m_GlassDat.m_dFilmActiveRectSY[idx];
}

void CDialogGlass::DamCadResult()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	int i,j;
	long idx;
	double rslt_x, rslt_y;

	double dam[2];
	pdlg->m_RecipeScan->GetFactor(2, dam);

	for(i = 0; i < m_lCVDUpPointCnt; i++)
	{
		m_DamCad[i].m_dCVDCADResult_x = m_CvdCad[i].m_dCVDCADResult_x;
		m_DamCad[i].m_dCVDCADResult_y = m_CvdCad[i].m_dCVDCADResult_y - dam[0];
	}

	for(i = m_lCVDUpPointCnt; i < (m_lCVDUpPointCnt + m_lCVDDwPointCnt); i++)
	{
		m_DamCad[i].m_dCVDCADResult_x = m_CvdCad[i].m_dCVDCADResult_x;
		m_DamCad[i].m_dCVDCADResult_y = m_CvdCad[i].m_dCVDCADResult_y - dam[1];
	}
	
//	int temp = 0;
//	double cvd_line_a = pdlg->m_RecipeCell->GetCVDSizeY0(); //330.436;//
//	double cvd_line_b = pdlg->m_RecipeCell->GetCVDSizeY1(); //330.436;//
//	double cvd_line_top_bot_dis = pdlg->m_RecipeCell->GetCVDUpDnDis();  //33.060000;
//	double cvd_line_grp_dis_x = pdlg->m_RecipeCell->GetCVDGrpDisY();  //11.132; 
//	double cvd_line_grp_dis_y = pdlg->m_RecipeCell->GetCVDGrpDisY();  //5.37;  //
//	
//	double cvd_line_float_a,cvd_line_float_b;
//	rslt_x = pdlg->m_RecipeCell->GetCVDAlignFirstCVDY() + 12;//m_Recipe->GetCvdAlignX();
//	rslt_y = pdlg->m_RecipeCell->GetCVDAlignFirstCVDX() + 7;//m_Recipe->GetCvdAlignY();
///*	double cvd_line_a = 330.436;//pdlg->m_RecipeCell->GetCVDSizeY0_coordXY();
//	double cvd_line_b = 330.436;//pdlg->m_RecipeCell->GetCVDSizeY0_coordXY();
//	double cvd_line_grp_dis_x = 11.132;
//	double cvd_line_grp_dis_y = 5.37;
//	double cvd_line_top_bot_dis = 33.060000 + 0.322;
//	double cvd_line_float_a,cvd_line_float_b;
//	rslt_x = 689.002 + 12;//m_Recipe->GetCvdAlignX();
//	rslt_y = 431.630 + 7 - 0.302;//m_Recipe->GetCvdAlignY(); */
//
//	temp = cvd_line_a + cvd_line_b;
//	idx = 0;	
//	CString ii;
//
//	if((int)cvd_line_a < cvd_line_a)
//	{
//		cvd_line_float_a  = cvd_line_a - (int)cvd_line_a;
//	}
//
//	if((int)cvd_line_b < cvd_line_b)
//	{
//		cvd_line_float_a  = cvd_line_a - (int)cvd_line_a;
//	}
//	
//	//i = 22, j = 662;
//
//	/*for(i = 0; i < 22; i++)
//	{
//		for(j = 0; j < (int)(cvd_line_a + cvd_line_b); j++)
//		{
//			m_CvdCad[(i * 660) + j] ;
//		}
//	}*/
//
//	for(i = 0; i < 22; i++)
//	{
//		for(j = 0; j < (int)(cvd_line_a + cvd_line_b); j++)
//		{		
//			if(j == (int)cvd_line_a - 1)
//			{
//				rslt_x = rslt_x  - cvd_line_grp_dis_x;
//			}
//			
//			m_DamCad[(i*temp)+j].m_dCVDCADResult_x = rslt_x - j;
//			m_DamCad[(i*temp)+j].m_dCVDCADResult_y = rslt_y;		
//			idx++;
//			//ii.Format(_T("%d"),i);			
//		}
//		
//		if(i % 2 == 0)
//		{	
//			rslt_y = rslt_y - cvd_line_top_bot_dis;
//		}
//		else 
//		{
//			
//			rslt_y = rslt_y - cvd_line_grp_dis_y;
//		}
//	}
}


void CDialogGlass::CvdCadResult()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	int i, j, k;
	long idx = 0;
	long idx1 = 0;
	double rslt_x, rslt_y;
	CRect rect;
	double sx, sy, ex, ey, cx, cy, dsx, dsy, dex, dey;
	CdRect dRect;


	for(i = 0; i < pdlg->m_RecipeCell->GetCVDCntY_coordXY(); i++)
	{
		for(j = 0; j < pdlg->m_RecipeCell->GetCVDCntX_coordXY();j++)
		{
			if(GLASS_MAX_CELL_NUM <= idx1)
			{
				break;
			}

			dRect.SetdRectLTRB(m_GlassDat.m_dCVDActiveRectSX[idx1], m_GlassDat.m_dCVDActiveRectSY[idx1], 
				m_GlassDat.m_dCVDActiveRectEX[idx1], m_GlassDat.m_dCVDActiveRectEY[idx1]);

			cx = dRect.GetCenter().GetY();
			for(k = 0; k < (int)abs(dRect.GetWidth()) + 2; k++)
			{
				m_CvdCad[idx].m_dCVDCADResult_x = cx;
				m_CvdCad[idx++].m_dCVDCADResult_y = dRect.GetLeft() - 1 + k;
			}

			dRect.SetdRectLTRB(m_GlassDat.m_dCVDDNActiveRectSX[idx1], m_GlassDat.m_dCVDDNActiveRectSY[idx1], 
				m_GlassDat.m_dCVDDNActiveRectEX[idx1], m_GlassDat.m_dCVDDNActiveRectEY[idx1]);

			cx = dRect.GetCenter().GetY();
			for(k = 0; k < (int)abs(dRect.GetWidth()) + 2; k++)
			{
				m_CvdCad[idx].m_dCVDCADResult_x = cx;
				m_CvdCad[idx++].m_dCVDCADResult_y = dRect.GetLeft() - 1 + k;
			}

			idx1++;
			m_lCVDUpPointCnt = idx/2;
			m_lCVDDwPointCnt = idx/2;
		}
	}
}

CString CDialogGlass::GetCVDIndex(int scan_type_idx, int defect_idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	//20160303 ngh
	double sx, sy, ex, ey, cx, cy;
	CString temp, temp1;
	int idx = 0, j, k;
	int cvd_idx = 0;
	int temp_idx1 = 0, temp_idx2 = 0;
	CString strUpDw;

	strUpDw.Format(_T(""));

	for(j = 0; j < pdlg->m_RecipeCell->GetCVDCntY_coordXY(); j++)
	{
		for(k = 0; k < pdlg->m_RecipeCell->GetCVDCntX_coordXY(); k++)
		{
			temp_idx1 = j;
			temp_idx2 = k;
			sx = m_GlassDat.m_dCVDActiveRectSX[idx];
			sy = m_GlassDat.m_dCVDActiveRectSY[idx];
			ex = m_GlassDat.m_dCVDDNActiveRectEX[idx];
			ey = m_GlassDat.m_dCVDDNActiveRectEY[idx];
			cy = m_GlassDat.m_dFilmActiveRectEY[idx] + ((m_GlassDat.m_dFilmActiveRectSY[idx] - m_GlassDat.m_dFilmActiveRectEY[idx]) * 0.5);
			temp1 = GetInsActive_id(0,0,3,j,k);
							
			if((sx <= m_DftGlassDat_coordXY[scan_type_idx][defect_idx].m_dCntX) && (m_DftGlassDat_coordXY[scan_type_idx][defect_idx].m_dCntX <= ex))
			{
				if((cy <= m_DftGlassDat_coordXY[scan_type_idx][defect_idx].m_dCntY) && (m_DftGlassDat_coordXY[scan_type_idx][defect_idx].m_dCntY <= sy))
				{
					strUpDw.Format(_T("%s_T"), temp1);
					j = pdlg->m_RecipeCell->GetCVDCntY_coordXY();
					return strUpDw;
				}
				else if((cy >= m_DftGlassDat_coordXY[scan_type_idx][defect_idx].m_dCntY) && (m_DftGlassDat_coordXY[scan_type_idx][defect_idx].m_dCntY >= ey))
				{
					cvd_idx = j;
					strUpDw.Format(_T("%s_B"), temp1);
					j = pdlg->m_RecipeCell->GetCVDCntY_coordXY();
					return strUpDw;
				}
			}
			idx++;
		}
	}
	return strUpDw;
}

void CDialogGlass::GetCVDCadValue(int scan_type_idx, int defect_idx, double *x, double *y, BOOL XorY)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	//결과값 정렬 필요
	BOOL ok1 = FALSE, ok2 = FALSE;
	double g_x, g_y, c_x, c_y;

	if(XorY == 0)
	{
		ok2 = TRUE;
	}
	else
	{
		ok1 = TRUE;
	}

	for(int i = 0; i < (m_lCVDUpPointCnt + m_lCVDDwPointCnt); i++)
	{
		g_x = m_DftGlassDat_coordXY[scan_type_idx][defect_idx].m_dCntX;
		g_y = m_DftGlassDat_coordXY[scan_type_idx][defect_idx].m_dCntY;
		c_x = m_DamCad[i].m_dCVDCADResult_y;
		c_y = m_DamCad[i].m_dCVDCADResult_x;

		if(((abs(c_x) - 0.5) < abs(g_x)) && (abs(g_x) < (abs(c_x) + 0.5)) && (ok1 == FALSE))
		{
			*x = abs(c_x);
			ok1 = TRUE;
		}

		if(((abs(c_y) - 0.9) < abs(g_y)) && (abs(g_y) < (abs(c_y) + 0.9)) && (ok2 == FALSE))
		{
			*y = abs(c_y);
			ok2 = TRUE;
		}

		if(ok1 && ok2)
		{
			return;
		}
	}
}

//20160405 ngh
void CDialogGlass::InitDefectData(int scan_type, BOOL befor_after)
{
	for(int i = 0; i < 30000; i++)
	{
		if(befor_after)
		{
			memset(&m_DftScanDat_scan[scan_type][i], NULL, sizeof(typeDefectData));
		}
		else
		{
			memset(&m_DftScanData_before[scan_type][i], NULL, sizeof(typeDefectData));
		}
	}
}

//20160405 ngh
BOOL CDialogGlass::DefectCompare(int scan_type)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	BOOL rslt = TRUE;
	int CompareCnt = 0;

	if(pdlg->m_RecipeScan->GetSelectDefectErrorSize(scan_type, 5))
	{
		CompareCnt = DefectBeforeAfterCompare(scan_type, BeforeDefectCnt[scan_type] + 1, m_ScanManage[scan_type].m_iLastDefectIdx + 1, pdlg->m_RecipeScan->GetMaskDefectDistance(scan_type));

		if(CompareCnt > 0)
		{
			rslt = FALSE;
		}
	}

	SetBeforeDefect(scan_type);

	return rslt;
}

//20160405 ngh
int CDialogGlass::DefectBeforeAfterCompare(int scan_type, int beforeCnt, int afterCnt, double distance)
{
	int CompareCnt = 0;
	double dCompareLeft = 0.;
	double dCompareRight = 0.;
	double dCompareTop = 0.;
	double dCompareBottom = 0.;
	for(int i = 0; i < beforeCnt; i++)
	{
		dCompareLeft = m_DftScanData_before[scan_type][i].m_dCntX - distance;
		dCompareRight = m_DftScanData_before[scan_type][i].m_dCntX + distance;
		dCompareTop = m_DftScanData_before[scan_type][i].m_dCntY - distance;
		dCompareBottom = m_DftScanData_before[scan_type][i].m_dCntY + distance;
		for(int j = 0; j < afterCnt; j++)
		{
			if((dCompareLeft <= m_DftScanDat_scan[scan_type][j].m_dCntX) && (m_DftScanDat_scan[scan_type][j].m_dCntX <= dCompareRight) &&
				(dCompareTop <= m_DftScanDat_scan[scan_type][j].m_dCntY) && (m_DftScanDat_scan[scan_type][j].m_dCntY <= dCompareBottom))
			{
				m_DftScanDat_scan[scan_type][j].m_bBeforeAfterCompare = TRUE;
				//DefectCompareIdx[CompareCnt] = j + 1;
				CompareCnt++;
				break;
			}
		}
	}
	if(CompareCnt == 0)
	{
		m_nCompareDefectCnt[scan_type] = 0;
	}
	else
	{
		m_nCompareDefectCnt[scan_type] = CompareCnt;
	}
	return CompareCnt;
}

//20160405 ngh
int CDialogGlass::GetDefectCompareCnt(int scan_type)
{
	return m_nCompareDefectCnt[scan_type];
}

//20160405 ngh
void CDialogGlass::SetBeforeDefect(int scan_type)
{
	m_DftScanData_before[scan_type] = m_DftScanDat_scan[scan_type];

	BeforeDefectCnt[scan_type] = m_ScanManage[scan_type].m_iLastDefectIdx;
}

//20160405 ngh
void CDialogGlass::SetDispCompareDefectGlassData(int scan_type_idx)
{
	Hobject region;
	int i;
	int cam_idx = 0;
	double cx, cy, sx, sy, ex, ey;
	cx = 0.;
	cy = 0.;
	sx = 0.;
	sy = 0.;
	ex = 0.;
	ey = 0.;

	double d_cx, d_cy, d_sx, d_sy, d_ex, d_ey;
	d_cx = 0.;
	d_cy = 0.;
	d_sx = 0.;
	d_sy = 0.;
	d_ex = 0.;
	d_ey = 0.;

	double l_sx, l_sy, l_ex, l_ey;
	l_sx = 0.;
	l_sy = 0.;
	l_ex = 0.;
	l_ey = 0.;

	double radius = 0.;
	double l_cx, l_cy, l_wd, l_ht;
	l_cx = 0.;
	l_cy = 0.;
	l_wd = 0.;
	l_ht = 0.;

	for(i = 0; (i < m_ScanManage[scan_type_idx].m_iLastDefectIdx) && (i < DEFECT_DATA_MAX_NUM); i++)
	{
		if(m_DftScanDat_scan[scan_type_idx][i].m_bBeforeAfterCompare)
		{
			cx = m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntX;
			cy = m_DftGlassDat_coordXY[scan_type_idx][i].m_dCntY;
			sx = cx - (m_DftGlassDat_coordXY[scan_type_idx][i].m_dWidth * 0.5);
			sy = cy + (m_DftGlassDat_coordXY[scan_type_idx][i].m_dHeight * 0.5);	
			ex = cx + (m_DftGlassDat_coordXY[scan_type_idx][i].m_dWidth * 0.5);
			ey = cy - (m_DftGlassDat_coordXY[scan_type_idx][i].m_dHeight * 0.5);	

			GetCoordGlassToDisp(sx, sy, &d_sx, &d_sy);
			GetCoordGlassToDisp(ex, ey, &d_ex, &d_ey);

			//Display
			l_sx = d_sx + 0.5 - 15;
			l_sy = d_sy + 0.5 - 15;
			l_ex = d_ex + 0.5 + 15;
			l_ey = d_ey + 0.5 + 15;
			
			l_wd = l_ex - l_sx;
			l_ht = l_ey - l_sy;
			l_cx = l_sx + (l_wd / 2);
			l_cy = l_sy + (l_ht / 2);

			set_color(m_dlgImgRect[cam_idx]->GetView(), "white");
			set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));

			for(int j = 0; j < 1; j++)
			{
				gen_ellipse(&region, l_cy, l_cx, 0, (l_wd / 2) + j, (l_ht / 2) + j);
				disp_obj(region, m_dlgImgRect[cam_idx]->GetView());
			}
		}
	}
}

BOOL CDialogGlass::OFDefectError(int scan_type, double width, double height)
{
	BOOL rslt = TRUE;
	int of_cnt = 0;
	for(int i = 0; (i < m_ScanManage[scan_type].m_iLastDefectIdx) && (i < DEFECT_DATA_MAX_NUM); i++)
	{
		if((m_DftScanDat_scan[scan_type][i].m_dWidth >= width) || (m_DftScanDat_scan[scan_type][i].m_dHeight >= height))
		{
			m_DftScanDat_scan[scan_type][i].m_bOFDefect = TRUE;
			of_cnt++;
			rslt = FALSE;
		}
	}

	m_OFDefectCnt[scan_type] = of_cnt;

	return rslt;
}

int CDialogGlass::GetOFDefectCnt(int scantype)
{
	return m_OFDefectCnt[scantype];
}

BOOL CDialogGlass::GlassWindowMapSave(int scan_type, CString path)
{
	Herror herr;
	char temp[512];
	sprintf(temp, "%S\\GlassWindowMap_%d.bmp", path,scan_type);

	set_check("~give_error");
	herr = dump_window(m_dlgImgRect[0]->GetView(), HTuple("bmp"), temp);
	set_check("give_error");

	if(H_MSG_TRUE == herr)
	{

	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

int CDialogGlass::GetDfPanelidx(CString cellid,int type)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	int idx = 0;
	CString temp;
	BOOL compar = FALSE;
#ifdef DEFECT
	temp.Format(_T("%s"),cellid.Right(2));
	for(int i=0; i < pdlg->m_RecipePanel->GetCellCntY_coordXY(); i++)
	{
		for(int j=0; j<pdlg->m_RecipePanel->GetCellCntX_coordXY(); j++)
		{
			if(!temp.Compare(m_strCell_table[i][j]))
			{
				idx = m_DfCount[i][j].idx;
				m_DfCount[i][j].count += 1;
				if(type == 0)
				{
					pdlg->m_Dfserver->m_idf_count_ = m_DfCount[i][j].count;
					pdlg->m_Dfserver->m_ii = i;
					pdlg->m_Dfserver->m_ij = j;
				}
				
				compar = TRUE;
				break;
			}
		}
		if(compar)
		{
			break;
		}
	}
#endif
	return idx;
}


//20160524 ngh
BOOL CDialogGlass::GetCVDRslt()
{
	BOOL rslt = TRUE;
	int scan_type = 2;
	if(m_ScanManage[scan_type].m_iCVDNgCnt > 0)
	{
		m_nCvdDefectCnt = m_ScanManage[scan_type].m_iCVDNgCnt;
		rslt = FALSE;
	}

	return rslt;
}

int CDialogGlass::GetCVDDefectCnt()
{
	return m_nCvdDefectCnt;
}

C_CtrlImageRect* CDialogGlass::GetImageRect()
{
	return m_dlgImgRect[0];
}