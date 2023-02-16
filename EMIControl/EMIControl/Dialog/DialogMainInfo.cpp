// Dialog/DialogMainInfo.cpp : 구현 파일입니다.
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


// CDialogMainInfo 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogMainInfo, CDialogEx)

CDialogMainInfo::CDialogMainInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogMainInfo::IDD, pParent)
{

}

CDialogMainInfo::~CDialogMainInfo()
{
	KillTimer(0);
}

void CDialogMainInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_ORIGIN, m_main_info_origin);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_LOAD, m_main_info_load);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_ALIGN, m_main_info_align);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_SCAN_8K, m_btn_main_info_scan8k);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_SCAN_16K, m_btn_main_info_scan16k);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_FILM, m_btn_main_info_film);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_SHADOW, m_btn_main_info_shadow);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_JUDGE, m_btn_main_info_judge);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_REVIEW, m_btn_main_info_review);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_UNLOAD, m_btn_main_info_unload);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_LABEL_LOT, m_btn_main_info_lot);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_LABEL_GLASS, m_btn_main_info_glass);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_LABEL_RECIPE, m_btn_main_info_recipe);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_LABEL_OPERATOR, m_btn_main_info_operator);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_LABEL_TIME, m_btn_main_info_time);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_LABEL_CASSETTE, m_btn_main_info_cassete);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_LABEL_SLOT, m_btn_main_info_slot);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_LABEL_RECIPE_NO, m_btn_main_info_recipe_no);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_LOADOFF, m_btn_loader_use);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_IPS_CONNECT, b_btn_ips_online);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_8K_USE, m_btn_8k_use);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_8K90_USE, m_btn_8k90_use);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_CVD_USE, m_btn_16k_use);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_FILM_USE, m_btn_filmalign_use);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_SHADOW_USE, m_btn_shadow_use);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_CYCLE_ONOFF, m_btn_cycle_use);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_DEFECT_SERVER, m_btn_DefServer);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_PCL_S_USE, m_btn_16k_pcl_s_use);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_PCL_L_USE, m_btn_16k_pcl_l_use);
	DDX_Control(pDX, IDC_BTNENHCTRL_INFO_PRCS_CVD_USE2, m_btn_ips_autorun_use);
}


BEGIN_MESSAGE_MAP(CDialogMainInfo, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogMainInfo 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDialogMainInfo, CDialogEx)
	ON_EVENT(CDialogMainInfo, IDC_BTNENHCTRL_INFO_PRCS_ORIGIN, DISPID_CLICK, CDialogMainInfo::ClickBtnenhctrlInfoPrcsOrigin, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogMainInfo::ClickBtnenhctrlInfoPrcsOrigin()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}

void CDialogMainInfo::SetUpdate()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	CString str;
	typeGlassDataPara para;

	para = pdlg->m_Inline->GetUnloaderGlassData();

	str.Format(_T("%S"), para.strLotID);
	m_btn_main_info_lot.SetCaption(str);

	str.Format(_T("%S"), para.strGlassID);
	m_btn_main_info_glass.SetCaption(str);

	m_btn_main_info_recipe.SetCaption(pdlg->m_Recipe->GetRecipeName());

	//m_btn_main_info_operator.SetCaption(str);
	//m_btn_main_info_time.SetCaption(str);
	//m_btn_main_info_cassete.SetCaption(str);

	str.Format(_T("%d"), para.GlassCode.nSlotNumber);
	m_btn_main_info_slot.SetCaption(str);

	str.Format(_T("%d"), pdlg->m_Recipe->GetRecipeNo() + 1);
	m_btn_main_info_recipe_no.SetCaption(str);

	/*
	m_main_info_origin;
	m_main_info_load;
	m_main_info_align;
	m_btn_main_info_scan8k;
	m_btn_main_info_scan16k;
	m_btn_main_info_film;
	m_btn_main_info_shadow;
	m_btn_main_info_judge;
	m_btn_main_info_review;
	m_btn_main_info_unload;
	*/
}

BOOL CDialogMainInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetTimer(0, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogMainInfo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	SYSTEMTIME	lpSystemTime;
	CString str;

	typeGlassDataPara glass_data;

	switch(nIDEvent)
	{
	case 0:
		//시간표시
		GetLocalTime(&lpSystemTime);
		str.Format(_T("%04d:%02d:%02d:%02d:%02d:%02d"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay, lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond);
		m_btn_main_info_time.SetCaption(str);

		m_btn_main_info_recipe.SetCaption(pdlg->m_Recipe->GetRecipeName());

		str.Format(_T("%d"), pdlg->m_Recipe->GetRecipeNo() + 1);
		m_btn_main_info_recipe_no.SetCaption(str);

		//Glass data
		glass_data = pdlg->m_Inline->GetUnloaderGlassData();//Unloader것을 읽어야한다.

		str.Format(_T("%S"), glass_data.strLotID);
		if(str.IsEmpty())
		{
			str.Format(_T("NULL"));
		}
		m_btn_main_info_lot.SetCaption(str);

		str.Format(_T("%S"), glass_data.strGlassID);
		if(str.IsEmpty())
		{
			str.Format(_T("NULL"));
		}
		m_btn_main_info_glass.SetCaption(str);

		str.Format(_T("%d"), glass_data.GlassCode.nSlotNumber);
		m_btn_main_info_slot.SetCaption(str);

		str.Format(_T("%s"), pdlg->GetOperater());
		m_btn_main_info_operator.SetCaption(str);
		
		//m_btn_main_info_operator;
		
		if(pdlg->m_GlassManu->GetLoaderUse())
		{
			if(!m_btn_loader_use.GetValue())
			{
				m_btn_loader_use.SetValue(TRUE);
				m_btn_loader_use.SetCaption(_T("LOADER사용"));
				m_btn_loader_use.SetBackColor(RGB(0, 255, 0));
			}
		}
		else
		{
			if(m_btn_loader_use.GetValue())
			{
				m_btn_loader_use.SetValue(FALSE);
				m_btn_loader_use.SetCaption(_T("LOADER미사용"));
				m_btn_loader_use.SetBackColor(RGB(1, 1, 1));
			}
		}

		if(pdlg->m_GlassManu->GetCycleUse())
		{
			if(!m_btn_cycle_use.GetValue())
			{
				m_btn_cycle_use.SetValue(TRUE);
				m_btn_cycle_use.SetCaption(_T("CYCLE 진행"));
				m_btn_cycle_use.SetBackColor(RGB(0, 255, 0));
			}
		}
		else
		{
			if(m_btn_cycle_use.GetValue())
			{
				m_btn_cycle_use.SetValue(FALSE);
				m_btn_cycle_use.SetCaption(_T("CYCLE 정지"));
				m_btn_cycle_use.SetBackColor(RGB(1, 1, 1));
			}
		}
		

		if(pdlg->GetIPSConnect())
		{
			if(!b_btn_ips_online.GetValue())
			{
				b_btn_ips_online.SetValue(TRUE);
				b_btn_ips_online.SetCaption(_T("IPS ON-LINE"));
				b_btn_ips_online.SetBackColor(RGB(0, 255, 0));
			}
		}
		else
		{
			if(b_btn_ips_online.GetValue())
			{
				b_btn_ips_online.SetValue(FALSE);
				b_btn_ips_online.SetCaption(_T("IPS OFF-LINE"));
				b_btn_ips_online.SetBackColor(RGB(255, 0, 0));
			}
		}

		if(pdlg->m_RecipeScan->GetInspectUse(0))
		{
			if(!m_btn_8k_use.GetValue())
			{
				m_btn_8k_use.SetValue(TRUE);
				m_btn_8k_use.SetCaption(_T("8K 사용"));
				m_btn_8k_use.SetBackColor(RGB(0, 255, 0));
			}
		}
		else
		{
			if(m_btn_8k_use.GetValue())
			{
				m_btn_8k_use.SetValue(FALSE);
				m_btn_8k_use.SetCaption(_T("8K 미사용"));
				m_btn_8k_use.SetBackColor(RGB(1, 1, 1));
			}
		}

		if(pdlg->m_RecipeScan->GetInspectUse(1))
		{
			if(!m_btn_8k90_use.GetValue())
			{
				m_btn_8k90_use.SetValue(TRUE);
				m_btn_8k90_use.SetCaption(_T("8K 90도 사용"));
				m_btn_8k90_use.SetBackColor(RGB(0, 255, 0));
			}
		}
		else
		{
			if(m_btn_8k90_use.GetValue())
			{
				m_btn_8k90_use.SetValue(FALSE);
				m_btn_8k90_use.SetCaption(_T("8K 90도 미사용"));
				m_btn_8k90_use.SetBackColor(RGB(1, 1, 1));
			}
		}

		if(pdlg->m_RecipeScan->GetInspectUse(2))
		{
			if(!m_btn_16k_use.GetValue())
			{
				m_btn_16k_use.SetValue(TRUE);
				m_btn_16k_use.SetCaption(_T("CVD 사용"));
				m_btn_16k_use.SetBackColor(RGB(0, 255, 0));
			}
		}
		else
		{
			if(m_btn_16k_use.GetValue())
			{
				m_btn_16k_use.SetValue(FALSE);
				m_btn_16k_use.SetCaption(_T("CVD 미사용"));
				m_btn_16k_use.SetBackColor(RGB(1, 1, 1));
			}
		}

		if(pdlg->m_RecipeScan->GetInspectUse(3))
		{
			if(!m_btn_16k_pcl_s_use.GetValue())
			{
				m_btn_16k_pcl_s_use.SetValue(TRUE);
				m_btn_16k_pcl_s_use.SetCaption(_T("PCL 단축 사용"));
				m_btn_16k_pcl_s_use.SetBackColor(RGB(0, 255, 0));
			}
		}
		else
		{
			if(m_btn_16k_pcl_s_use.GetValue())
			{
				m_btn_16k_pcl_s_use.SetValue(FALSE);
				m_btn_16k_pcl_s_use.SetCaption(_T("PCL 장축 미사용"));
				m_btn_16k_pcl_s_use.SetBackColor(RGB(1, 1, 1));
			}
		}

		if(pdlg->m_RecipeScan->GetInspectUse(4))
		{
			if(!m_btn_16k_pcl_l_use.GetValue())
			{
				m_btn_16k_pcl_l_use.SetValue(TRUE);
				m_btn_16k_pcl_l_use.SetCaption(_T("PCL 장축 사용"));
				m_btn_16k_pcl_l_use.SetBackColor(RGB(0, 255, 0));
			}
		}
		else
		{
			if(m_btn_16k_pcl_l_use.GetValue())
			{
				m_btn_16k_pcl_l_use.SetValue(FALSE);
				m_btn_16k_pcl_l_use.SetCaption(_T("PCL 단축 미사용"));
				m_btn_16k_pcl_l_use.SetBackColor(RGB(1, 1, 1));
			}
		}

		if(pdlg->m_FilmAlign->GetInspectUse())
		{
			if(!m_btn_filmalign_use.GetValue())
			{
				m_btn_filmalign_use.SetValue(TRUE);
				m_btn_filmalign_use.SetCaption(_T("FILM ALIGN사용"));
				m_btn_filmalign_use.SetBackColor(RGB(0, 255, 0));
			}
		}
		else
		{
			if(m_btn_filmalign_use.GetValue())
			{
				m_btn_filmalign_use.SetValue(FALSE);
				m_btn_filmalign_use.SetCaption(_T("FILM ALIGN미사용"));
				m_btn_filmalign_use.SetBackColor(RGB(1, 1, 1));
			}
		}
#ifdef DEFECT
		if(pdlg->m_Dfserver->GetServerConnectStatus())
		{
			if(!m_btn_DefServer.GetValue())
			{
				m_btn_DefServer.SetValue(TRUE);
				//m_btn_filmalign_use.SetCaption(_T("FILM ALIGN사용"));
				m_btn_DefServer.SetBackColor(RGB(0, 255, 0));
			}
		}
		else
		{
			if(m_btn_DefServer.GetValue())
			{
				m_btn_DefServer.SetValue(FALSE);
				//m_btn_filmalign_use.SetCaption(_T("FILM ALIGN미사용"));
				m_btn_DefServer.SetBackColor(RGB(1, 1, 1));
			}
		}
#endif
		/*
		if()
		{
			m_btn_shadow_use.SetValue(TRUE);
			m_btn_shadow_use.SetCaption(_T("CVD 사용"));
			m_btn_shadow_use.SetBackColor(RGB(0, 255, 0));
		}
		else
		{
			m_btn_shadow_use.SetValue(FALSE);
			m_btn_shadow_use.SetCaption(_T("CVD 미사용"));
			m_btn_shadow_use.SetBackColor(RGB(1, 1, 1));
		}
		*/

		if(pdlg->GetIPSAutoRunState())
		{
			if(!m_btn_ips_autorun_use.GetValue())
			{
				m_btn_ips_autorun_use.SetValue(TRUE);
				m_btn_ips_autorun_use.SetCaption(_T("IPS_AUTO"));
				m_btn_ips_autorun_use.SetBackColor(RGB(0, 255, 0));
			}
		}
		else
		{
			if(m_btn_ips_autorun_use.GetValue())
			{
				m_btn_ips_autorun_use.SetValue(FALSE);
				m_btn_ips_autorun_use.SetCaption(_T("IPS_MANUAL"));
				m_btn_ips_autorun_use.SetBackColor(RGB(1,1,1));
			}
		}

		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CDialogMainInfo::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CDialogMainInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
