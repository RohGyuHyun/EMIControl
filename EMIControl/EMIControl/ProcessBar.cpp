// ProcessBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EMIControl.h"
#include "ProcessBar.h"
#include "afxdialogex.h"
#include "EMIControlDlg.h"


// CProcessBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CProcessBar, CDialogEx)

CProcessBar::CProcessBar(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProcessBar::IDD, pParent)
{

}

CProcessBar::~CProcessBar()
{
	KillTimer(0);
}

void CProcessBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_ctrlProcessBar);
	//ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CProcessBar::OnBnClickedButtonCancel)
}

BEGIN_MESSAGE_MAP(CProcessBar, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	//ON_NOTIFY(BCN_DROPDOWN, IDC_BUTTON_CANCEL, &CProcessBar::OnBnDropDownButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CProcessBar::OnBnClickedButtonCancel)
END_MESSAGE_MAP()

// CProcessBar 메시지 처리기입니다.
BOOL CProcessBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_bProcessCancel = FALSE;
	m_ctrlProcessBar.SetRange(0, 99);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CProcessBar::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialog::PreTranslateMessage(pMsg);
}

void CProcessBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	CString temp;
	if(bShow)
	{
		m_bProcessComp = FALSE;
		m_bProcessCancel = FALSE;
		GetDlgItem(IDC_STATIC_NAME)->SetWindowTextW(m_ProcessBar.m_strProcess);
		GetDlgItem(IDC_STATIC_TIME)->SetWindowTextW(m_ProcessBar.m_strProcessTime);
		temp.Format(_T("%d %%"), m_ProcessBar.m_nProcessPer);
		GetDlgItem(IDC_STATIC_PAR)->SetWindowTextW(temp);
		m_iStep = m_ProcessBar.m_nProcessPer;
		m_ctrlProcessBar.SetPos(m_iStep);
		SetTimer(0, 10, NULL);
	}
	else
	{
		//KillTimer(0);
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CProcessBar::SetProcess(typeProcessBar process_bar)
{
	m_ProcessBar = process_bar;
	GetDlgItem(IDC_STATIC_NAME)->SetWindowTextW(m_ProcessBar.m_strProcess);
	GetDlgItem(IDC_STATIC_PAR)->SetWindowTextW(_T("0 %%"));
}

void CProcessBar::SetProcessPer(int percent)
{
	CString strPer;
	m_ProcessBar.m_nProcessPer = percent;
	m_ctrlProcessBar.SetPos(percent);
	strPer.Format(_T("%d %%"), percent);
	GetDlgItem(IDC_STATIC_PAR)->SetWindowTextW(strPer);
}

void CProcessBar::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case 0:
		m_iStep++;
		if((m_iStep % 100) == 0)
		{
			m_ProcessBar.m_strProcessTime.Format(_T("%d sec"), m_iStep / 100);
			GetDlgItem(IDC_STATIC_TIME)->SetWindowTextW(m_ProcessBar.m_strProcessTime);
		}
		
		if(m_bProcessComp)
		{
			KillTimer(0);
			m_bProcessComp = FALSE;
			ShowWindow(SW_HIDE);
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}
void CProcessBar::OnBnClickedButtonCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	pdlg->SetDefectFileWriteBreak(TRUE);
	m_bProcessCancel = TRUE;
	KillTimer(0);
	ShowWindow(SW_HIDE);
}


//void CProcessBar::OnBnClickedButtonCancel()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}
