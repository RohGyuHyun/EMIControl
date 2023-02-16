// ProcessBar.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EMIControl.h"
#include "ProcessBar.h"
#include "afxdialogex.h"
#include "EMIControlDlg.h"


// CProcessBar ��ȭ �����Դϴ�.

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

// CProcessBar �޽��� ó�����Դϴ�.
BOOL CProcessBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_bProcessCancel = FALSE;
	m_ctrlProcessBar.SetRange(0, 99);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

BOOL CProcessBar::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	pdlg->SetDefectFileWriteBreak(TRUE);
	m_bProcessCancel = TRUE;
	KillTimer(0);
	ShowWindow(SW_HIDE);
}


//void CProcessBar::OnBnClickedButtonCancel()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//}
