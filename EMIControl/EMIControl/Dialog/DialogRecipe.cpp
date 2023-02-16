// Dialog/DialogRecipe.cpp : ���� �����Դϴ�.
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


// CDialogRecipe ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogRecipe, CDialogEx)

CDialogRecipe::CDialogRecipe(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogRecipe::IDD, pParent)
{
	m_sRecipeName.Empty();
	m_iRecipeNo = 0;
}

CDialogRecipe::~CDialogRecipe()
{

}

void CDialogRecipe::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNENHCTRL_RECIPE_NAME, m_btn_recipe_name);
	DDX_Control(pDX, IDC_BTNENHCTRL_RECIPE_NO, m_btn_recipe_no);
	DDX_Control(pDX, IDC_BTNENHCTRL_RECIPE_NEW, m_btn_recipe_new);
	DDX_Control(pDX, IDC_BTNENHCTRL_RECIPE_SELECT, m_btn_recipe_select);
	DDX_Control(pDX, IDC_BTNENHCTRL_RECIPE_DELETE, m_btn_recipe_delete);
	DDX_Control(pDX, IDC_BTNENHCTRL_RECIPE_COPY, m_btn_recipe_copy);
}


BEGIN_MESSAGE_MAP(CDialogRecipe, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogRecipe �޽��� ó�����Դϴ�.
BEGIN_EVENTSINK_MAP(CDialogRecipe, CDialogEx)
	ON_EVENT(CDialogRecipe, IDC_BTNENHCTRL_RECIPE_PANEL, DISPID_CLICK, CDialogRecipe::ClickBtnenhctrlRecipePanel, VTS_NONE)
	ON_EVENT(CDialogRecipe, IDC_BTNENHCTRL_RECIPE_ALIGN, DISPID_CLICK, CDialogRecipe::ClickBtnenhctrlRecipeAlign, VTS_NONE)
	ON_EVENT(CDialogRecipe, IDC_BTNENHCTRL_RECIPE_CELL, DISPID_CLICK, CDialogRecipe::ClickBtnenhctrlRecipeCell, VTS_NONE)
	ON_EVENT(CDialogRecipe, IDC_BTNENHCTRL_RECIPE_FILM_ALIGN, DISPID_CLICK, CDialogRecipe::ClickBtnenhctrlRecipeFilmAlign, VTS_NONE)
	ON_EVENT(CDialogRecipe, IDC_BTNENHCTRL_RECIPE_SCAN, DISPID_CLICK, CDialogRecipe::ClickBtnenhctrlRecipeScan, VTS_NONE)
	ON_EVENT(CDialogRecipe, IDC_BTNENHCTRL_RECIPE_LOT_RESTART, DISPID_CLICK, CDialogRecipe::ClickBtnenhctrlRecipeLotRestart, VTS_NONE)
	ON_EVENT(CDialogRecipe, IDC_BTNENHCTRL_RECIPE_NEW, DISPID_CLICK, CDialogRecipe::ClickBtnenhctrlRecipeNew, VTS_NONE)
	ON_EVENT(CDialogRecipe, IDC_BTNENHCTRL_RECIPE_SAVE, DISPID_CLICK, CDialogRecipe::ClickBtnenhctrlRecipeSave, VTS_NONE)
	ON_EVENT(CDialogRecipe, IDC_BTNENHCTRL_RECIPE_CELL_PCL, DISPID_CLICK, CDialogRecipe::ClickBtnenhctrlRecipeCellPcl, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogRecipe::ClickBtnenhctrlRecipePanel()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	pdlg->RecipePanel();
}

void CDialogRecipe::ClickBtnenhctrlRecipeAlign()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	pdlg->MainAlign();
}

void CDialogRecipe::ClickBtnenhctrlRecipeCell()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	pdlg->RecipeCell();
}

void CDialogRecipe::ClickBtnenhctrlRecipeCellPcl()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	pdlg->RecipeCellPCL();
}


void CDialogRecipe::ClickBtnenhctrlRecipeFilmAlign()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	pdlg->FilmAlign();
}

void CDialogRecipe::ClickBtnenhctrlRecipeScan()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
	pdlg->RecipeScan();
}

void CDialogRecipe::SetLotRestart()
{
	ClickBtnenhctrlRecipeLotRestart();
}

void CDialogRecipe::ClickBtnenhctrlRecipeLotRestart()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	CDialogMSG dlg;
	dlg.SetCaption(_T("����� �Ͻðڽ��ϱ�? ����Ÿ�� �ʱ�ȭ �˴ϴ�."));
	if(0 == dlg.DoModal())
	{
		return;
	}

	pdlg->SetLotInit();
}

BOOL CDialogRecipe::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CString strValue;

	//Load recipe file///////////////////////////////////////////////////////////////////////
	

	m_btn_recipe_name.SetCaption(m_sRecipeName);
	strValue.Format(_T("%d"), m_iRecipeNo);
	m_btn_recipe_no.SetCaption(strValue);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDialogRecipe::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	CString strValue;
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if(bShow)
	{
		m_btn_recipe_name.SetCaption(m_sRecipeName);
		strValue.Format(_T("%d"), m_iRecipeNo + 1);
		m_btn_recipe_no.SetCaption(strValue);		
	}
	else
	{

	}
}

CString CDialogRecipe::GetRecipeName()
{
	return m_sRecipeName;
}

int CDialogRecipe::GetRecipeNo()
{
	return m_iRecipeNo;
}

void CDialogRecipe::SetRecipeName(CString name)
{
	m_sRecipeName = name;
}

void CDialogRecipe::SetRecipeNo(int no)
{
	m_iRecipeNo = no;
}

void CDialogRecipe::ClickBtnenhctrlRecipeNew()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CDialogRecipeModel dlg;
	CString strValue;

	if(dlg.DoModal())
	{
		//Recipe ����
		m_sRecipeName = dlg.GetRecipeName();
		m_iRecipeNo = dlg.GetRecipeNo();

		if(!dlg.GetOpenList(dlg.GetRecipeNo()))
		{
			//Load recipe
			pdlg->SetLoadRecipeFile(dlg.GetRecipePath());

			//20160412 ngh
			pdlg->m_Glass->SetDispGlassDat();

			//20160420 ngh
			pdlg->m_Glass->MakeCellID();

			//20160608 ngh
			pdlg->m_Glass->CvdCadResult();
			pdlg->m_Glass->DamCadResult();
		}
		else
		{
			AfxMessageBox(_T("Error : Recipe Name�� ���� ���� �ʽ��ϴ�. ������ Recipe�� �ƴմϴ�!"));
			return;

		}

		m_btn_recipe_name.SetCaption(m_sRecipeName);
		strValue.Format(_T("%d"), m_iRecipeNo + 1);
		m_btn_recipe_no.SetCaption(strValue);

		//ECS set recipe list
		int i;
		pdlg->m_Inline->WriteLog(_T("SetRecipeNum List Write Start"));
		for(i = 0; i < RECIPE_MAX_NUM; i++)
		{
			//FALSE:������ false ������ true
			if(!dlg.GetOpenList(i))
			{
				pdlg->m_Inline->SetRecipeNum(i + 1, FALSE);
			}
			else
			{
				pdlg->m_Inline->SetRecipeNum(i + 1, TRUE);
			}
		}
		pdlg->m_Inline->WriteLog(_T("SetRecipeNum List Write End"));
		pdlg->m_Inline->SetRecipeListChange(TRUE);

		pdlg->SetRecipeToECS();
	}
	else
	{


	}
}

void CDialogRecipe::ClickBtnenhctrlRecipeSave()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	//Save recipe///////////////////////////////////////////////////////////////
	CDialogRecipeModel dlg;
	dlg.SetOpen();
	pdlg->SetSaveRecipeFile(dlg.GetRecipePath());
}


BOOL CDialogRecipe::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}