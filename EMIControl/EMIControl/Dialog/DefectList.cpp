// Dialog\DefectList.cpp : ���� �����Դϴ�.
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
#include "Dialog\DefectList.h"


#include "afxdialogex.h"


// CDefectList ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDefectList, CDialogEx)

CDefectList::CDefectList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDefectList::IDD, pParent)
	, m_chk_Pat(TRUE)
	, m_chk_White(TRUE)
	, m_chk_Small(TRUE)
	, m_chk_Medium(TRUE)
	, m_chk_Large(TRUE)
	, m_chk_OverLarge(TRUE)
{
	m_iListIdx = -1;
	m_bSortAscending = TRUE;
	m_nSortColumn = 0;
}

CDefectList::~CDefectList()
{
}

void CDefectList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DEFECT, m_list_defect);
	DDX_Check(pDX, IDC_CHECK_DEFECT_SELECT_0, m_chk_Pat);
	DDX_Check(pDX, IDC_CHECK_DEFECT_SELECT_1, m_chk_White);
	DDX_Check(pDX, IDC_CHECK_DEFECT_SELECT_2, m_chk_Small);
	DDX_Check(pDX, IDC_CHECK_DEFECT_SELECT_3, m_chk_Medium);
	DDX_Check(pDX, IDC_CHECK_DEFECT_SELECT_4, m_chk_Large);
	DDX_Check(pDX, IDC_CHECK_DEFECT_SELECT_5, m_chk_OverLarge);
	DDX_Control(pDX, IDC_CHECK_DEFECT_SELECT_0, m_chk_Defect0);
	DDX_Control(pDX, IDC_CHECK_DEFECT_SELECT_1, m_chk_Defect1);
	DDX_Control(pDX, IDC_CHECK_DEFECT_SELECT_2, m_chk_Defect2);
	DDX_Control(pDX, IDC_CHECK_DEFECT_SELECT_3, m_chk_Defect3);
	DDX_Control(pDX, IDC_CHECK_DEFECT_SELECT_4, m_chk_Defect4);
	DDX_Control(pDX, IDC_CHECK_DEFECT_SELECT_5, m_chk_Defect5);
}


BEGIN_MESSAGE_MAP(CDefectList, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHECK_DEFECT_SELECT_0, &CDefectList::OnBnClickedCheckDefectSelect0)
	ON_BN_CLICKED(IDC_CHECK_DEFECT_SELECT_1, &CDefectList::OnBnClickedCheckDefectSelect1)
	ON_BN_CLICKED(IDC_CHECK_DEFECT_SELECT_2, &CDefectList::OnBnClickedCheckDefectSelect2)
	ON_BN_CLICKED(IDC_CHECK_DEFECT_SELECT_3, &CDefectList::OnBnClickedCheckDefectSelect3)
	ON_BN_CLICKED(IDC_CHECK_DEFECT_SELECT_4, &CDefectList::OnBnClickedCheckDefectSelect4)
	ON_BN_CLICKED(IDC_CHECK_DEFECT_SELECT_5, &CDefectList::OnBnClickedCheckDefectSelect5)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DEFECT, &CDefectList::OnNMDblclkListDefect)
	ON_NOTIFY(HDN_ITEMCLICKA, 0, &CDefectList::OnHdnItemclickListDefect)
	ON_NOTIFY(HDN_ITEMCLICKW, 0, &CDefectList::OnHdnItemclickListDefect)
END_MESSAGE_MAP()


// CDefectList �޽��� ó�����Դϴ�.
CListCtrl *CDefectList::GetList()
{
	return &m_list_defect;
}

BOOL CDefectList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	InitList(0, TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDefectList::InitList(int iSotColumm, BOOL bSortDirect)
{
	LV_COLUMN m_lvColumn;
	CFont m_font;
	CString strTemp, strText[7];
	int idx = 0;

	strText[0].Format(_T("Index"));
	strText[1].Format(_T("Cell_ID"));
	strText[2].Format(_T("Width(mm)"));
	strText[3].Format(_T("Height(mm)"));
	strText[4].Format(_T("Area(um^2)"));
	strText[5].Format(_T("Size(S,M,L,OL)"));
	strText[6].Format(_T("�̹� & ���̼�"));

	for(idx = 0; idx < 7; idx++)
	{
		if(idx == iSotColumm)
		{
			if(bSortDirect)
			{
				strText[idx]+=_T("��");
			}
			else
			{
				strText[idx]+=_T("��");
			}
		}
	}

	m_list_defect.SetExtendedStyle(LVS_EX_GRIDLINES);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_LEFT;
	m_lvColumn.pszText=(LPWSTR)(LPCWSTR)strText[0];
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=80;
	m_list_defect.InsertColumn(0, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=(LPWSTR)(LPCWSTR)strText[1];
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=110;
	m_list_defect.InsertColumn(1, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=(LPWSTR)(LPCWSTR)strText[2];
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=110;
	m_list_defect.InsertColumn(2, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=(LPWSTR)(LPCWSTR)strText[3];
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=110;
	m_list_defect.InsertColumn(3, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=(LPWSTR)(LPCWSTR)strText[4];
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=110;
	m_list_defect.InsertColumn(4, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=(LPWSTR)(LPCWSTR)strText[5];
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=130;
	m_list_defect.InsertColumn(5, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=(LPWSTR)(LPCWSTR)strText[6];
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=130;
	m_list_defect.InsertColumn(6, &m_lvColumn);

	m_font.CreatePointFont(120, _T("����"));
	m_list_defect.SetFont(&m_font, TRUE);
}

void CDefectList::ChangeListCol(int iSotColumm, BOOL bSortDirect)
{
	LV_COLUMN m_lvColumn;
	CFont m_font;
	CString strTemp, strText[7];
	int idx = 0;

	strText[0].Format(_T("Index"));
	strText[1].Format(_T("Cell_ID"));
	strText[2].Format(_T("Width(mm)"));
	strText[3].Format(_T("Height(mm)"));
	strText[4].Format(_T("Area(um^2)"));
	strText[5].Format(_T("Size(S,M,L,OL)"));
	strText[6].Format(_T("�̹� & ���̼�"));

	for(idx = 0; idx < 7; idx++)
	{
		if(idx == iSotColumm)
		{
			if(bSortDirect)
			{
				strText[idx]+=_T("��");
			}
			else
			{
				strText[idx]+=_T("��");
			}
		}
	}

	m_list_defect.SetExtendedStyle(LVS_EX_GRIDLINES);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_LEFT;
	m_lvColumn.pszText=(LPWSTR)(LPCWSTR)strText[0];
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=80;
	m_list_defect.SetColumn(0, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=(LPWSTR)(LPCWSTR)strText[1];
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=110;
	m_list_defect.SetColumn(1, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=(LPWSTR)(LPCWSTR)strText[2];
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=110;
	m_list_defect.SetColumn(2, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=(LPWSTR)(LPCWSTR)strText[3];
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=110;
	m_list_defect.SetColumn(3, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=(LPWSTR)(LPCWSTR)strText[4];
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=110;
	m_list_defect.SetColumn(4, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=(LPWSTR)(LPCWSTR)strText[5];
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=130;
	m_list_defect.SetColumn(5, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_CENTER;
	m_lvColumn.pszText=(LPWSTR)(LPCWSTR)strText[6];
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=130;
	m_list_defect.SetColumn(6, &m_lvColumn);

	m_font.CreatePointFont(120, _T("����"));
	m_list_defect.SetFont(&m_font, TRUE);
}

void CDefectList::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CDefectList::SetDefectListInfo(int idx, CString strData)
{
	m_iListIdx = -1;
	if(idx == 0)
	{
		m_list_defect.DeleteAllItems();
		m_chk_Pat = TRUE;
		m_chk_White = TRUE;
		m_chk_Small = TRUE;
		m_chk_Medium = TRUE;
		m_chk_Large = TRUE;
		m_chk_OverLarge = TRUE;

		m_chk_Defect0.SetCheck(TRUE);
		m_chk_Defect1.SetCheck(TRUE);
		m_chk_Defect2.SetCheck(TRUE);
		m_chk_Defect3.SetCheck(TRUE);
		m_chk_Defect4.SetCheck(TRUE);
		m_chk_Defect5.SetCheck(TRUE);

		if(m_strDefectListInfo.size() > 0)
		{
			m_strDefectListInfo.clear();
		}
	}
	m_strDefectListInfo.push_back(strData);
	m_iListIdx = idx;
	WriteList();
}

void CDefectList::WriteList()
{
	CString strIndex, strCellid, strWidth, strHeight, strArea, strSize, strBW;
	int subItem = 1;

	if(m_strDefectListInfo.size() < 1)
	{
		return;
	}

	AfxExtractSubString(strIndex, m_strDefectListInfo[m_iListIdx], 0, ',');
	AfxExtractSubString(strCellid, m_strDefectListInfo[m_iListIdx], 1, ',');
	AfxExtractSubString(strWidth, m_strDefectListInfo[m_iListIdx], 2, ',');
	AfxExtractSubString(strHeight, m_strDefectListInfo[m_iListIdx], 3, ',');
	AfxExtractSubString(strArea, m_strDefectListInfo[m_iListIdx], 4, ',');
	AfxExtractSubString(strSize, m_strDefectListInfo[m_iListIdx], 5, ',');
	AfxExtractSubString(strBW, m_strDefectListInfo[m_iListIdx], 6, ',');

	m_list_defect.InsertItem(m_iListIdx + 1, strIndex);
	m_list_defect.SetItemText(m_iListIdx, subItem++, strCellid);
	m_list_defect.SetItemText(m_iListIdx, subItem++, strWidth);
	m_list_defect.SetItemText(m_iListIdx, subItem++, strHeight);
	m_list_defect.SetItemText(m_iListIdx, subItem++, strArea);
	m_list_defect.SetItemText(m_iListIdx, subItem++, strSize);
	m_list_defect.SetItemText(m_iListIdx, subItem++, strBW);
}

void CDefectList::ListUpdate()
{
	CString strIndex, strCellid, strWidth, strHeight, strArea, strSize, strBW;
	int subItem = 1, nItem = 0;;
	m_list_defect.DeleteAllItems();
	UpdateData(TRUE);

	if(!m_chk_White && !m_chk_Pat)
	{
		return;
	}

	for(int i = 0; i < (m_iListIdx + 1); i++)
	{
		AfxExtractSubString(strIndex, m_strDefectListInfo[i], 0, ',');
		AfxExtractSubString(strCellid, m_strDefectListInfo[i], 1, ',');
		AfxExtractSubString(strWidth, m_strDefectListInfo[i], 2, ',');
		AfxExtractSubString(strHeight, m_strDefectListInfo[i], 3, ',');
		AfxExtractSubString(strArea, m_strDefectListInfo[i], 4, ',');
		AfxExtractSubString(strSize, m_strDefectListInfo[i], 5, ',');
		AfxExtractSubString(strBW, m_strDefectListInfo[i], 6, ',');
		if(m_chk_White && !m_chk_Pat)
		{
			if(!strBW.Compare(_T("BW")))
			{
				if(m_chk_Small && (!strSize.Compare(_T("S"))))
				{
					subItem = 1;
					m_list_defect.InsertItem(nItem + 1, strIndex);
					m_list_defect.SetItemText(nItem, subItem++, strCellid);
					m_list_defect.SetItemText(nItem, subItem++, strWidth);
					m_list_defect.SetItemText(nItem, subItem++, strHeight);
					m_list_defect.SetItemText(nItem, subItem++, strArea);
					m_list_defect.SetItemText(nItem, subItem++, strSize);
					m_list_defect.SetItemText(nItem++, subItem++, strBW);
				}
				else if(m_chk_Medium && (!strSize.Compare(_T("M"))))
				{
					subItem = 1;
					m_list_defect.InsertItem(nItem + 1, strIndex);
					m_list_defect.SetItemText(nItem, subItem++, strCellid);
					m_list_defect.SetItemText(nItem, subItem++, strWidth);
					m_list_defect.SetItemText(nItem, subItem++, strHeight);
					m_list_defect.SetItemText(nItem, subItem++, strArea);
					m_list_defect.SetItemText(nItem, subItem++, strSize);
					m_list_defect.SetItemText(nItem++, subItem++, strBW);
				}
				else if(m_chk_Large && (!strSize.Compare(_T("L"))))
				{
					subItem = 1;
					m_list_defect.InsertItem(nItem + 1, strIndex);
					m_list_defect.SetItemText(nItem, subItem++, strCellid);
					m_list_defect.SetItemText(nItem, subItem++, strWidth);
					m_list_defect.SetItemText(nItem, subItem++, strHeight);
					m_list_defect.SetItemText(nItem, subItem++, strArea);
					m_list_defect.SetItemText(nItem, subItem++, strSize);
					m_list_defect.SetItemText(nItem++, subItem++, strBW);
				}
				else if(m_chk_OverLarge && (!strSize.Compare(_T("OL"))))
				{
					subItem = 1; 
					m_list_defect.InsertItem(nItem + 1, strIndex);
					m_list_defect.SetItemText(nItem, subItem++, strCellid);
					m_list_defect.SetItemText(nItem, subItem++, strWidth);
					m_list_defect.SetItemText(nItem, subItem++, strHeight);
					m_list_defect.SetItemText(nItem, subItem++, strArea);
					m_list_defect.SetItemText(nItem, subItem++, strSize);
					m_list_defect.SetItemText(nItem++, subItem++, strBW);
				}
			}
		}
		else if(!m_chk_White && m_chk_Pat)
		{
			if(!strBW.Compare(_T("B")))
			{
				if(m_chk_Small && (!strSize.Compare(_T("S"))))
				{
					subItem = 1;
					m_list_defect.InsertItem(nItem + 1, strIndex);
					m_list_defect.SetItemText(nItem, subItem++, strCellid);
					m_list_defect.SetItemText(nItem, subItem++, strWidth);
					m_list_defect.SetItemText(nItem, subItem++, strHeight);
					m_list_defect.SetItemText(nItem, subItem++, strArea);
					m_list_defect.SetItemText(nItem, subItem++, strSize);
					m_list_defect.SetItemText(nItem++, subItem++, strBW);
				}
				else if(m_chk_Medium && (!strSize.Compare(_T("M"))))
				{
					subItem = 1;
					m_list_defect.InsertItem(nItem + 1, strIndex);
					m_list_defect.SetItemText(nItem, subItem++, strCellid);
					m_list_defect.SetItemText(nItem, subItem++, strWidth);
					m_list_defect.SetItemText(nItem, subItem++, strHeight);
					m_list_defect.SetItemText(nItem, subItem++, strArea);
					m_list_defect.SetItemText(nItem, subItem++, strSize);
					m_list_defect.SetItemText(nItem++, subItem++, strBW);
				}
				else if(m_chk_Large && (!strSize.Compare(_T("L"))))
				{
					subItem = 1;
					m_list_defect.InsertItem(nItem + 1, strIndex);
					m_list_defect.SetItemText(nItem, subItem++, strCellid);
					m_list_defect.SetItemText(nItem, subItem++, strWidth);
					m_list_defect.SetItemText(nItem, subItem++, strHeight);
					m_list_defect.SetItemText(nItem, subItem++, strArea);
					m_list_defect.SetItemText(nItem, subItem++, strSize);
					m_list_defect.SetItemText(nItem++, subItem++, strBW);
				}
				else if(m_chk_OverLarge && (!strSize.Compare(_T("OL"))))
				{
					subItem = 1; 
					m_list_defect.InsertItem(nItem + 1, strIndex);
					m_list_defect.SetItemText(nItem, subItem++, strCellid);
					m_list_defect.SetItemText(nItem, subItem++, strWidth);
					m_list_defect.SetItemText(nItem, subItem++, strHeight);
					m_list_defect.SetItemText(nItem, subItem++, strArea);
					m_list_defect.SetItemText(nItem, subItem++, strSize);
					m_list_defect.SetItemText(nItem++, subItem++, strBW);
				}
			}
		}
		else if(m_chk_White && m_chk_Pat)
		{
			if(m_chk_Small && (!strSize.Compare(_T("S"))))
			{
				subItem = 1;
				m_list_defect.InsertItem(nItem + 1, strIndex);
				m_list_defect.SetItemText(nItem, subItem++, strCellid);
				m_list_defect.SetItemText(nItem, subItem++, strWidth);
				m_list_defect.SetItemText(nItem, subItem++, strHeight);
				m_list_defect.SetItemText(nItem, subItem++, strArea);
				m_list_defect.SetItemText(nItem, subItem++, strSize);
				m_list_defect.SetItemText(nItem++, subItem++, strBW);
			}
			else if(m_chk_Medium && (!strSize.Compare(_T("M"))))
			{
				subItem = 1;
				m_list_defect.InsertItem(nItem + 1, strIndex);
				m_list_defect.SetItemText(nItem, subItem++, strCellid);
				m_list_defect.SetItemText(nItem, subItem++, strWidth);
				m_list_defect.SetItemText(nItem, subItem++, strHeight);
				m_list_defect.SetItemText(nItem, subItem++, strArea);
				m_list_defect.SetItemText(nItem, subItem++, strSize);
				m_list_defect.SetItemText(nItem++, subItem++, strBW);
			}
			else if(m_chk_Large && (!strSize.Compare(_T("L"))))
			{
				subItem = 1;
				m_list_defect.InsertItem(nItem + 1, strIndex);
				m_list_defect.SetItemText(nItem, subItem++, strCellid);
				m_list_defect.SetItemText(nItem, subItem++, strWidth);
				m_list_defect.SetItemText(nItem, subItem++, strHeight);
				m_list_defect.SetItemText(nItem, subItem++, strArea);
				m_list_defect.SetItemText(nItem, subItem++, strSize);
				m_list_defect.SetItemText(nItem++, subItem++, strBW);
			}
			else if(m_chk_OverLarge && (!strSize.Compare(_T("OL"))))
			{
				subItem = 1; 
				m_list_defect.InsertItem(nItem + 1, strIndex);
				m_list_defect.SetItemText(nItem, subItem++, strCellid);
				m_list_defect.SetItemText(nItem, subItem++, strWidth);
				m_list_defect.SetItemText(nItem, subItem++, strHeight);
				m_list_defect.SetItemText(nItem, subItem++, strArea);
				m_list_defect.SetItemText(nItem, subItem++, strSize);
				m_list_defect.SetItemText(nItem++, subItem++, strBW);
			}
		}
	}
}

void CDefectList::OnBnClickedCheckDefectSelect0()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ListUpdate();
}


void CDefectList::OnBnClickedCheckDefectSelect1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ListUpdate();
}


void CDefectList::OnBnClickedCheckDefectSelect2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ListUpdate();
}


void CDefectList::OnBnClickedCheckDefectSelect3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ListUpdate();
}


void CDefectList::OnBnClickedCheckDefectSelect4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ListUpdate();
}


void CDefectList::OnBnClickedCheckDefectSelect5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ListUpdate();
}


void CDefectList::OnNMDblclkListDefect(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int idx = pNMListView->iItem;

	if(idx > -1)
	{
#ifdef DEFECT_MOVE
		pdlg->m_Glass->DefectMove(_wtoi(m_list_defect.GetItemText(idx, 0)) - 1);
#endif
	}
	*pResult = 0;
}


BOOL CDefectList::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

int CALLBACK CDefectList::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	BOOL bSortAscending  = ((SORTPARAM*)lParamSort)->bSortDirect; // ���� ���
	int iColumn = ((SORTPARAM*)lParamSort)->iSotColumm;
	int result = 0;

	if(iColumn == 0 || iColumn == 1 || iColumn == 5 || iColumn == 6)
	{
		CString* pStr1 = (CString*)lParam1; // ItemData�� ���ڿ��� �� ��ȯ
		CString* pStr2 = (CString*)lParam2;
		result = wcscmp(*pStr1, *pStr2); // strcmp �� ���ؼ�
		if (bSortAscending == TRUE)
			return result;
		else
			return -result;
	}
	else if(iColumn == 2 || iColumn == 3 || iColumn == 4)
	{
		DWORD dWord1 = (DWORD)lParam1;
		DWORD dWord2 = (DWORD)lParam2;

		if(dWord1 > dWord2)
			result = 1;
		
		if (bSortAscending == TRUE)
			return result;
		else
			return !result;
	}
	 
     return 0;
}

void CDefectList::OnHdnItemclickListDefect(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	 NM_LISTVIEW* pNMListView = (NM_LISTVIEW*) pNMHDR;
	 int iColumn = pNMListView->iItem; // iSubItem �� ���õ� Column ��ȣ�̴�.

#ifdef DEFECT_SORT
     if (iColumn == m_nSortColumn)
	 { // m_nSortColumn �� ��� ������ ���� �����ߴ� Column ��ȣ�̴�.
         m_bSortAscending = !m_bSortAscending; // ���� �� Columns�� ������������ �����ѰŸ�, ������������ �����Ѵ�.
     }
     else 
	 { // ���� �ٸ� Column�̸� �������� ����
         m_bSortAscending = TRUE;
     }
     m_nSortColumn = iColumn; // ������ Column�� ��������� ����.
	 SORTPARAM SortPara;
	 SortPara.bSortDirect = m_bSortAscending;
	 SortPara.iSotColumm = iColumn;

     // �� ItemData�� ���ڿ��� �־���� �ϹǷ�

    int nItem = m_list_defect.GetItemCount(); // Item ������ ���ϰ�
	CString** arStr = new CString*[nItem]; // ���ڿ� ��������� Item���� ��ŭ �����.
	DWORD arDw;
	if(iColumn == 0 || iColumn == 1 || iColumn == 5 || iColumn == 6)
	{
		for (int i=0; i<nItem; i++) 
		{
			arStr[i] = new CString(m_list_defect.GetItemText(i, iColumn)); // ���ڿ��� �� Item�� ������ �־��ְ�
			m_list_defect.SetItemData(i, (LPARAM)arStr[i]); // ItemData�� ���ش�.
		}
	}
	else if(iColumn == 2 || iColumn == 3 || iColumn == 4)
	{
		for (int i=0; i<nItem; i++) 
		{
			arDw = (int)(_wtof(m_list_defect.GetItemText(i, iColumn)) * 10000);
			m_list_defect.SetItemData(i, (LPARAM)arDw);
		}
	}

    // ù ��° �μ��� ������ ���� �Լ�, �� ��° �μ��� ���� ���
    m_list_defect.SortItems(CompareFunc, (LPARAM)&SortPara); // ����!

	if(iColumn == 0 || iColumn == 1 || iColumn == 5 || iColumn == 6)
	{
		for (int i=0; i<nItem; i++) // ���� �������� �Ʊ� �Ҵ��� ���ڿ� �޸𸮸� �ٽ� ����
			delete arStr[i];
		delete []arStr;
	}
	else
	{
		delete []arStr;
	}

	ChangeListCol(iColumn, m_bSortAscending);
#endif
	*pResult = 0;
}
