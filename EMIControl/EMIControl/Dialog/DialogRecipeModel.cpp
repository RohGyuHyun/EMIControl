// Dialog/DialogRecipeModel.cpp : ���� �����Դϴ�.
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


// Src ������ ��� ���ϰ� ���� ���丮�� Dest�� �����Ѵ�. 
// XCopy(Dir1, Dir2)���·� ����Ѵ�.
BOOL XCopy(LPCTSTR Src, LPCTSTR Dest)
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	BOOL bResult = TRUE;
	TCHAR WildCard[MAX_PATH];
	TCHAR SrcFile[MAX_PATH];
	TCHAR DestFile[MAX_PATH];

	wsprintf(WildCard, _T("%s\\*.*"), Src);
	CreateDirectory(Dest, NULL);
	hSrch=FindFirstFile(WildCard, &wfd);
	if (hSrch == INVALID_HANDLE_VALUE) 
	{
		return FALSE;
	}

	while(bResult)
	{
		wsprintf(SrcFile, _T("%s\\%s"), Src, wfd.cFileName);
		wsprintf(DestFile, _T("%s\\%s"), Dest, wfd.cFileName);
		// ���� ���丮�� �߰ߵǸ� ���� ���丮�� �����Ѵ�.
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (lstrcmp(wfd.cFileName,_T(".")) && lstrcmp(wfd.cFileName,_T("..")))
			{
				XCopy(SrcFile, DestFile);
			}
		} 
		else 
		{
			CopyFile(SrcFile,DestFile,FALSE);
		}
		bResult=FindNextFile(hSrch,&wfd);
	}
	FindClose(hSrch);

	return TRUE;
}


// CDialogRecipeModel ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogRecipeModel, CDialogEx)

CDialogRecipeModel::CDialogRecipeModel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogRecipeModel::IDD, pParent)
	, m_edit_name(_T(""))
	, m_edit_desc(_T(""))
	, m_edit_recipe_model_recipe_no(0)
	, m_edit_recipe_no_new(0)
	, m_edit_recipe_name_new(_T(""))
	, m_edit_recipe_no_default(0)
	, m_edit_recipe_name_default(_T(""))
	, m_edit_recipe_no_new2(0)
	, m_edit_recipe_name_new2(_T(""))
	, m_edit_recipe_no_copy(0)
{
	memset(m_sRecipeName, NULL, sizeof(m_sRecipeName));
	memset(m_sRecipePath, NULL, sizeof(m_sRecipePath));
	
	m_iRecipeNo = 0;
	m_bRecipeCopy = FALSE;
	m_nCopyRecipeNo = -1;
	m_bDefaultRecipeCopy = FALSE;
	m_bSelectRecipeCopy = FALSE;
}

CDialogRecipeModel::~CDialogRecipeModel()
{
	SaveRecipeLabelPara(_T(RECIPE_PATH));
	SaveRecipeNoPara(_T(RECIPE_PATH));
}

void CDialogRecipeModel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MODEL, m_list_model);
	DDX_Text(pDX, IDC_EDIT_RECIPE_MODEL_RECIPE_NAME, m_edit_name);
	DDX_Text(pDX, IDC_EDIT_RECIPE_MODEL_RECIPE_DESC, m_edit_desc);
	DDX_Control(pDX, IDC_BTNENHCTRL_RECIPE_MODEL_NEW, m_btn_new);
	DDX_Control(pDX, IDC_BTNENHCTRL_RECIPE_MODEL_SELECT, m_btn_select);
	DDX_Control(pDX, IDC_BTNENHCTRL_RECIPE_MODEL_DELETE, m_btn_delete);
	DDX_Text(pDX, IDC_EDIT_RECIPE_MODEL_RECIPE_NO, m_edit_recipe_model_recipe_no);
	DDX_Text(pDX, IDC_EDIT_RECIPE_MODEL_RECIPE_NEW, m_edit_recipe_no_new);
	DDX_Text(pDX, IDC_EDIT_RECIPE_MODEL_RECIPE_NAME_NEW, m_edit_recipe_name_new);
	DDX_Text(pDX, IDC_EDIT_RECIPE_MODEL_RECIPE_DEFAULT, m_edit_recipe_no_default);
	DDX_Text(pDX, IDC_EDIT_RECIPE_MODEL_RECIPE_NAME_DEFAULT, m_edit_recipe_name_default);
	DDX_Text(pDX, IDC_EDIT_RECIPE_MODEL_RECIPE_NEW2, m_edit_recipe_no_new2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_MODEL_RECIPE_NAME_NEW2, m_edit_recipe_name_new2);
	DDX_Text(pDX, IDC_EDIT_RECIPE_MODEL_RECIPE_COPY, m_edit_recipe_no_copy);
}


BEGIN_MESSAGE_MAP(CDialogRecipeModel, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MODEL, &CDialogRecipeModel::OnDblclkListModel)
END_MESSAGE_MAP()


// CDialogRecipeModel �޽��� ó�����Դϴ�.
BEGIN_EVENTSINK_MAP(CDialogRecipeModel, CDialogEx)
	ON_EVENT(CDialogRecipeModel, IDC_BTNENHCTRL_RECIPE_MODEL_OK, DISPID_CLICK, CDialogRecipeModel::ClickBtnenhctrlRecipeModelOk, VTS_NONE)
	ON_EVENT(CDialogRecipeModel, IDC_BTNENHCTRL_RECIPE_MODEL_CANCEL, DISPID_CLICK, CDialogRecipeModel::ClickBtnenhctrlRecipeModelCancel, VTS_NONE)
	ON_EVENT(CDialogRecipeModel, IDC_BTNENHCTRL_RECIPE_MODEL_NEW, DISPID_CLICK, CDialogRecipeModel::ClickBtnenhctrlRecipeModelNew, VTS_NONE)
	ON_EVENT(CDialogRecipeModel, IDC_BTNENHCTRL_RECIPE_MODEL_SELECT, DISPID_CLICK, CDialogRecipeModel::ClickBtnenhctrlRecipeModelSelect, VTS_NONE)
	ON_EVENT(CDialogRecipeModel, IDC_BTNENHCTRL_RECIPE_MODEL_DELETE, DISPID_CLICK, CDialogRecipeModel::ClickBtnenhctrlRecipeModelDelete, VTS_NONE)
	ON_EVENT(CDialogRecipeModel, IDC_BTNENHCTRL_RECIPE_MODEL_DEFAULT, DISPID_CLICK, CDialogRecipeModel::ClickBtnenhctrlRecipeModelDefault, VTS_NONE)
	ON_EVENT(CDialogRecipeModel, IDC_BTNENHCTRL_RECIPE_MODEL_NEW2, DISPID_CLICK, CDialogRecipeModel::ClickBtnenhctrlRecipeModelNew2, VTS_NONE)
	ON_EVENT(CDialogRecipeModel, IDC_BTNENHCTRL_RECIPE_MODEL_COPY, DISPID_CLICK, CDialogRecipeModel::ClickBtnenhctrlRecipeModelCopy, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogRecipeModel::ClickBtnenhctrlRecipeModelOk()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CEMIControlDlg *pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strLog;
	SaveRecipeLabelPara(_T(RECIPE_PATH));
	SaveRecipeNoPara(_T(RECIPE_PATH));

	if(m_bSelectRecipeCopy)
	{
		if(!XCopy(m_strCopySrcFilePath, m_strCopyDesFilePath))
		{
			strLog.Format(_T("���� Recipe No %d ������ �����ϴ�."), m_nSelectRecipeNo);
			pdlg->WriteLog(MAIN_LOG, strLog);
		}
	}

	if(m_bDefaultRecipeCopy)
	{
		if(!XCopy(m_strCopySrcFilePath, m_strCopyDesFilePath))
		{
			strLog.Format(_T("�⺻ Recipe No %d ������ �����ϴ�."), m_nDefaultRecipeNo);
			pdlg->WriteLog(MAIN_LOG, strLog);
		}
	}

	if(m_bRecipeCopy && (m_nCopyRecipeNo >= 0))
	{
		if(!XCopy(m_strCopySrcFilePath, m_strCopyDesFilePath))
		{
			strLog.Format(_T("���� �� Recipe No %d ������ �����ϴ�."), m_nCopyRecipeNo);
			pdlg->WriteLog(MAIN_LOG, strLog);
		}
	}

	EndDialog(1);
}

void CDialogRecipeModel::SetOpen()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	LoadRecipeLabelPara(_T(RECIPE_PATH));
	LoadRecipeNoPara(_T(RECIPE_PATH));

}

BOOL CDialogRecipeModel::GetOpenList(int idx)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	//�����ϸ� FALSE
	CString str;
	str.Format(_T("%S"), m_sRecipeName[idx]);
	if(str.IsEmpty())
	{
		return TRUE;
	}
	else
	{
		//�𵨸��� �����ϸ�
		return FALSE;
	}
}


void CDialogRecipeModel::ClickBtnenhctrlRecipeModelCancel()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	EndDialog(0);
}

BOOL CDialogRecipeModel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	int i;
	long setstyle;
	LV_COLUMN column;
	CImageList *img_list_slct;
	CBitmap bitmap;

	CString str;

	SetWindowPos(&wndTopMost, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE); 

	LoadRecipeLabelPara(_T(RECIPE_PATH));
	LoadRecipeNoPara(_T(RECIPE_PATH));

	//List
	img_list_slct = new CImageList;
	img_list_slct->Create(16, 16, ILC_COLOR4, 3, 3);

	bitmap.LoadBitmap(IDB_BITMAP_NOTSELECTED);
	img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_BITMAP_SELECTED);
	img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	m_list_model.SetImageList(img_list_slct, LVSIL_SMALL);

	setstyle = GetWindowLong(m_list_model.m_hWnd, GWL_STYLE);
	setstyle |= LVS_EDITLABELS;
	SetWindowLong(m_list_model.m_hWnd, GWL_STYLE, setstyle);

	m_list_model.RedrawWindow();


	WCHAR* list[3]={_T("NO."), _T("NAME"), _T("PATH")};
	int width[4]={80, 300, 500};
	for(i=0; i<3; i++)
	{
		column.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		column.fmt=LVCFMT_LEFT;
		column.pszText=list[i];
		column.iSubItem=i;
		column.cx=width[i];
		m_list_model.InsertColumn(i, &column);
	}
	m_list_model.DeleteAllItems();

	//ADD List
	for(i = 0; i < RECIPE_MAX_NUM; i++)
	{
		
		str.Format(_T("%04d"), i + 1);
		m_list_model.InsertItem(i, str);
		m_list_model.SetItemText(i, 1, m_sRecipeName[i]);
		m_list_model.SetItemText(i, 2, m_sRecipePath[i]);
	}

	//Set
	LV_ITEM lvitem;
	for(i = 0; i < RECIPE_MAX_NUM; i++)
	{
		str.Format(_T("%04d"), i + 1);

		if(i == m_iRecipeNo)
		{
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage = 1;
			lvitem.pszText=(LPWSTR)(LPCWSTR)str;
			m_list_model.SetItem(&lvitem);
		}
		else
		{
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage = 0;
			lvitem.pszText=(LPWSTR)(LPCWSTR)str;
			m_list_model.SetItem(&lvitem);
		}

	}

	m_edit_recipe_model_recipe_no = m_iRecipeNo + 1;
	m_edit_name = m_sRecipeName[m_iRecipeNo];

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDialogRecipeModel::ClickBtnenhctrlRecipeModelNew()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	int idx;
	int i;
	LV_ITEM lvitem;
	CString str, strRecipeName;

	UpdateData(TRUE);

	idx = m_edit_recipe_no_new - 1;

	CDialogMSG dlg;
	str.Format(_T("���� ���õ� RECIPE NO %d�� RECIPE NO %d���� ���ο� ���� �����ϰڽ��ϱ�?"), m_edit_recipe_model_recipe_no, m_edit_recipe_no_new);
	dlg.SetCaption(str);
	if(0 == dlg.DoModal())
	{
		return;
	}

	if( (0 <= idx) && (RECIPE_MAX_NUM > idx) )
	{

	}
	else
	{
		idx = 0;
		AfxMessageBox(_T("Error : Recipe No�� �߸� �Ǿ����ϴ�!"));
		return;
	}

	if(m_edit_recipe_name_new.IsEmpty())
	{
		AfxMessageBox(_T("Error : Recipe �̸��� �Է¹ٶ��ϴ�..!"));
		return;
	}

	strRecipeName.Format(_T("%s"), m_sRecipeName[idx]);
	if(!strRecipeName.IsEmpty())
	{
		AfxMessageBox(_T("Error : �̹� ������ Recipe No �Դϴ�.!"));
		return;
	}

	m_iRecipeNo = idx;

	m_bSelectRecipeCopy = TRUE;
	m_strCopySrcFilePath.Format(_T("%s"), m_sRecipePath[m_edit_recipe_model_recipe_no - 1]);
	m_strCopyDesFilePath.Format(_T("%s"), m_sRecipePath[idx]);

	wsprintf(m_sRecipeName[m_iRecipeNo], _T("%s"), m_edit_recipe_name_new);


	m_list_model.DeleteAllItems();

	//ADD List
	for(i = 0; i < RECIPE_MAX_NUM; i++)
	{
		
		str.Format(_T("%04d"), i + 1);
		m_list_model.InsertItem(i, str);
		m_list_model.SetItemText(i, 1, m_sRecipeName[i]);
		m_list_model.SetItemText(i, 2, m_sRecipePath[i]);
	}

	for(i = 0; i < RECIPE_MAX_NUM; i++)
	{
		str.Format(_T("%04d"), i + 1);

		if(i == m_iRecipeNo)
		{
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage = 1;
			lvitem.pszText=(LPWSTR)(LPCWSTR)str;
			m_list_model.SetItem(&lvitem);
		}
		else
		{
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage = 0;
			lvitem.pszText=(LPWSTR)(LPCWSTR)str;
			m_list_model.SetItem(&lvitem);
		}

	}

	UpdateData(FALSE);
}


void CDialogRecipeModel::ClickBtnenhctrlRecipeModelSelect()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	int idx;
	int i;
	LV_ITEM lvitem;
	CString str;

	UpdateData(TRUE);

	idx = m_edit_recipe_model_recipe_no - 1;

	CDialogMSG dlg;
	str.Format(_T("RECIPE NO %d���� �̸��� %s �� �����ϰڽ��ϱ�?"), m_edit_recipe_model_recipe_no, m_edit_name);
	dlg.SetCaption(str);
	if(0 == dlg.DoModal())
	{
		return;
	}

	if( (0 < idx) && (RECIPE_MAX_NUM > idx) )
	{

	}
	else
	{
		idx = 0;
	}

	if(m_edit_name.IsEmpty())
	{
		AfxMessageBox(_T("Error : Recipe �̸��� �Է¹ٶ��ϴ�..!"));
		return;
	}

	//m_iRecipeNo = idx;
	wsprintf(m_sRecipeName[m_iRecipeNo], _T("%s"), m_edit_name);


	m_list_model.DeleteAllItems();

	//ADD List
	for(i = 0; i < RECIPE_MAX_NUM; i++)
	{
		
		str.Format(_T("%04d"), i + 1);
		m_list_model.InsertItem(i, str);
		m_list_model.SetItemText(i, 1, m_sRecipeName[i]);
		m_list_model.SetItemText(i, 2, m_sRecipePath[i]);
	}

	for(i = 0; i < RECIPE_MAX_NUM; i++)
	{
		str.Format(_T("%04d"), i + 1);

		if(i == m_iRecipeNo)
		{
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage = 1;
			lvitem.pszText=(LPWSTR)(LPCWSTR)str;
			m_list_model.SetItem(&lvitem);
		}
		else
		{
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage = 0;
			lvitem.pszText=(LPWSTR)(LPCWSTR)str;
			m_list_model.SetItem(&lvitem);
		}

	}



	UpdateData(FALSE);
}


void CDialogRecipeModel::ClickBtnenhctrlRecipeModelDelete()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CDialogMSG dlg;

	int idx;
	int i;
	LV_ITEM lvitem;
	CString str;

	UpdateData(TRUE);

	idx = m_edit_recipe_model_recipe_no - 1;

	str.Format(_T("RECIPE NO %d���� �����ϰڽ��ϱ�?"), m_edit_recipe_model_recipe_no, m_edit_name);
	dlg.SetCaption(str);
	if(0 == dlg.DoModal())
	{
		return;
	}

	if( (0 < idx) && (RECIPE_MAX_NUM > idx) )
	{

	}
	else
	{
		idx = 0;
	}

	//m_iRecipeNo = idx;
	wsprintf(m_sRecipeName[m_iRecipeNo], _T(""));
	m_edit_name.Empty();

	m_list_model.DeleteAllItems();

	//ADD List
	for(i = 0; i < RECIPE_MAX_NUM; i++)
	{
		
		str.Format(_T("%04d"), i + 1);
		m_list_model.InsertItem(i, str);
		m_list_model.SetItemText(i, 1, m_sRecipeName[i]);
		m_list_model.SetItemText(i, 2, m_sRecipePath[i]);
	}

	for(i = 0; i < RECIPE_MAX_NUM; i++)
	{
		str.Format(_T("%04d"), i + 1);

		if(i == m_iRecipeNo)
		{
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage = 1;
			lvitem.pszText=(LPWSTR)(LPCWSTR)str;
			m_list_model.SetItem(&lvitem);
		}
		else
		{
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage = 0;
			lvitem.pszText=(LPWSTR)(LPCWSTR)str;
			m_list_model.SetItem(&lvitem);
		}

	}



	UpdateData(FALSE);
}

BOOL CDialogRecipeModel::LoadRecipeLabelPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	
	_MakeDirectories(path);

	file_path = path + _T("\\RecipeLabel.par");

	//file ���� Ȯ�� 
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
		
		for(i = 0; i < RECIPE_MAX_NUM; i++)
		{
			key.Empty();
			key.Format(_T("RECIPE NAME_%d"), i + 1);
			GetPrivateProfileString(_T("RECIPE LABEL"), key, _T(" "), m_sRecipeName[i], sizeof(m_sRecipeName), file_path);

			/*
			key.Empty();
			key.Format(_T("RECIPE PATH_%d"), i + 1);
			GetPrivateProfileString(_T("RECIPE LABEL"), key, _T(" "), m_sRecipePath[i], sizeof(m_sRecipePath), file_path);
			*/
			wsprintf(m_sRecipePath[i], _T("%S\\%04d"), RECIPE_PATH, i + 1);
		}

	}
	fd=0;
	
	return rslt;
}

BOOL CDialogRecipeModel::LoadRecipeNoPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	int i;
	char ctemp[DAT_STR_LENG];
	size_t getval;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	
	_MakeDirectories(path);

	file_path = path + _T("\\RecipeNo.par");

	//file ���� Ȯ�� 
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
		key.Format(_T("RECIPE NAME_NO"));
		GetPrivateProfileString(_T("RECIPE NO"), key, _T("0.0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_iRecipeNo = (int)(atof(ctemp));

		key.Empty();
		key.Format(_T("DEFAULT_RECIPE NAME_NO"));
		GetPrivateProfileString(_T("RECIPE NO"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		wcstombs_s(&getval, ctemp, dat.GetLength() + 1, dat, _TRUNCATE);
		m_edit_recipe_no_default = (int)(atof(ctemp));

		key.Empty();
		key.Format(_T("DEFAULT_RECIPE NAME_NAME"));
		GetPrivateProfileString(_T("RECIPE NO"), key, _T("NULL"), temp, sizeof(temp), file_path);
		dat = temp;
		m_edit_recipe_name_default = dat;
	}
	fd=0;

	return rslt;
}


BOOL CDialogRecipeModel::SaveRecipeLabelPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	
	file_path = path + _T("\\RecipeLabel.par");

	//file ���� Ȯ�� 
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
		
		for(i = 0; i < RECIPE_MAX_NUM; i++)
		{
			key.Empty();
			key.Format(_T("RECIPE NAME_%d"), i + 1);
			dat.Empty();
			dat.Format(_T("%s"), m_sRecipeName[i]);
			WritePrivateProfileString(_T("RECIPE LABEL"), key, dat, file_path);		

			/*
			key.Empty();
			key.Format(_T("RECIPE PATH_%d"), i + 1);
			dat.Empty();
			dat.Format(_T("%s"), m_sRecipePath[i]);
			WritePrivateProfileString(_T("RECIPE LABEL"), key, dat, file_path);		
			*/
		}

	}
	fd=0;
	
	return rslt;
}

BOOL CDialogRecipeModel::SaveRecipeNoPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	
	file_path = path + _T("\\RecipeNo.par");

	//file ���� Ȯ�� 
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
		key.Format(_T("RECIPE NAME_NO"));
		dat.Empty();
		dat.Format(_T("%d"), m_iRecipeNo);
		WritePrivateProfileString(_T("RECIPE NO"), key, dat, file_path);	

		key.Empty();
		key.Format(_T("DEFAULT_RECIPE NAME_NO"));
		dat.Empty();
		dat.Format(_T("%d"), m_edit_recipe_no_default);
		WritePrivateProfileString(_T("RECIPE NO"), key, dat, file_path);

		key.Empty();
		key.Format(_T("DEFAULT_RECIPE NAME_NAME"));
		dat.Empty();
		dat.Format(_T("%s"), m_edit_recipe_name_default);
		WritePrivateProfileString(_T("RECIPE NO"), key, dat, file_path);

	}
	fd=0;
	
	return rslt;
}

int CDialogRecipeModel::GetRecipeNo()
{
	return m_iRecipeNo;
}

CString CDialogRecipeModel::GetRecipeName()
{
	CString str;

	m_sRecipeName[RECIPE_MAX_NUM][MAX_MOD_NAME];
	str.Format(_T("%s"), m_sRecipeName[m_iRecipeNo]);

	return str;
}

CString CDialogRecipeModel::GetRecipePath()
{
	CString str;

	str.Format(_T("%s"), m_sRecipePath[m_iRecipeNo]);

	return str;
}


void CDialogRecipeModel::OnDblclkListModel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int idx = pNMListView->iItem;
	int i;
	LV_ITEM lvitem;
	CString str;

	CDialogMSG dlg;
	str.Format(_T("RECIPE NO %d���� �����ϰڽ��ϱ�?"), idx + 1);
	dlg.SetCaption(str);
	if(0 == dlg.DoModal())
	{
		
	}
	else
	{
		
		if(GetOpenList(idx))
		{
			AfxMessageBox(_T("���� �Ҽ� ���� RECIPE�Դϴ�. ���� �� ����ٶ��ϴ�!"));
			return;
		}
		m_iRecipeNo = idx;

		m_edit_recipe_model_recipe_no = m_iRecipeNo + 1;
		m_edit_name = m_sRecipeName[m_iRecipeNo];

		for(i = 0; i < RECIPE_MAX_NUM; i++)
		{
			str.Format(_T("%04d"), i + 1);

			if(i == m_iRecipeNo)
			{
				lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
				lvitem.iItem=i;
				lvitem.iSubItem=0;
				lvitem.iImage = 1;
				lvitem.pszText=(LPWSTR)(LPCWSTR)str;
				m_list_model.SetItem(&lvitem);
			}
			else
			{
				lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
				lvitem.iItem=i;
				lvitem.iSubItem=0;
				lvitem.iImage = 0;
				lvitem.pszText=(LPWSTR)(LPCWSTR)str;
				m_list_model.SetItem(&lvitem);
			}

		}

		UpdateData(FALSE);
	}

	*pResult = 0;
}


BOOL CDialogRecipeModel::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogRecipeModel::ClickBtnenhctrlRecipeModelDefault()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	int idx;
	CDialogMSG dlg;
	CString strRecipeName, str;
	idx = m_edit_recipe_no_default - 1;

	
	str.Format(_T("RECIPE NO %d���� �⺻ Recipe�� �����ϰڽ��ϱ�?"), m_edit_recipe_no_default);
	dlg.SetCaption(str);
	if(0 == dlg.DoModal())
	{
		return;
	}

	if( (0 <= idx) && (RECIPE_MAX_NUM > idx) )
	{

	}
	else
	{
		idx = 0;
		AfxMessageBox(_T("Error : Recipe No�� �߸� �Ǿ����ϴ�!"));
		return;
	}

	m_edit_recipe_name_default.Format(_T("%s"), m_sRecipeName[idx]);

	if(m_edit_recipe_name_default.IsEmpty())
	{
		AfxMessageBox(_T("Error : ���� ���� ���� Recipe No�Դϴ�.!"));
		return;
	}

	UpdateData(FALSE);
}


void CDialogRecipeModel::ClickBtnenhctrlRecipeModelNew2()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	int idx;
	int i;
	LV_ITEM lvitem;
	CString str, strRecipeName;

	UpdateData(TRUE);

	idx = m_edit_recipe_no_new2 - 1;

	CDialogMSG dlg;
	str.Format(_T("�⺻ RECIPE�� RECIPE NO %d���� ���ο� ���� �����ϰڽ��ϱ�?"), m_edit_recipe_no_new2);
	dlg.SetCaption(str);
	if(0 == dlg.DoModal())
	{
		return;
	}

	if( (0 <= idx) && (RECIPE_MAX_NUM > idx) )
	{

	}
	else
	{
		idx = 0;
		AfxMessageBox(_T("Error : Recipe No�� �߸� �Ǿ����ϴ�!"));
		return;
	}

	if(m_edit_recipe_name_new2.IsEmpty())
	{
		AfxMessageBox(_T("Error : Recipe �̸��� �Է¹ٶ��ϴ�..!"));
		return;
	}

	m_iRecipeNo = idx;
	strRecipeName.Format(_T("%s"), m_sRecipeName[m_iRecipeNo]);
	if(!strRecipeName.IsEmpty())
	{
		AfxMessageBox(_T("Error : �̹� ������ Recipe No �Դϴ�.!"));
		return;
	}



	wsprintf(m_sRecipeName[m_iRecipeNo], _T("%s"), m_edit_recipe_name_new2);
	m_bDefaultRecipeCopy = TRUE;
	m_nDefaultRecipeNo = m_edit_recipe_no_default;
	m_strCopySrcFilePath.Format(_T("%s"), m_sRecipePath[m_nDefaultRecipeNo - 1]);
	m_strCopyDesFilePath.Format(_T("%s"), m_sRecipePath[m_edit_recipe_no_new2 - 1]);

	m_list_model.DeleteAllItems();

	//ADD List
	for(i = 0; i < RECIPE_MAX_NUM; i++)
	{
		
		str.Format(_T("%04d"), i + 1);
		m_list_model.InsertItem(i, str);
		m_list_model.SetItemText(i, 1, m_sRecipeName[i]);
		m_list_model.SetItemText(i, 2, m_sRecipePath[i]);
	}

	for(i = 0; i < RECIPE_MAX_NUM; i++)
	{
		str.Format(_T("%04d"), i + 1);

		if(i == m_iRecipeNo)
		{
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage = 1;
			lvitem.pszText=(LPWSTR)(LPCWSTR)str;
			m_list_model.SetItem(&lvitem);
		}
		else
		{
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage = 0;
			lvitem.pszText=(LPWSTR)(LPCWSTR)str;
			m_list_model.SetItem(&lvitem);
		}

	}

	UpdateData(FALSE);
}


void CDialogRecipeModel::ClickBtnenhctrlRecipeModelCopy()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	int idx = m_edit_recipe_no_copy - 1;
	CString strRecipeName;

	strRecipeName.Format(_T("%s"), m_sRecipeName[idx]);
	if(strRecipeName.IsEmpty())
	{
		AfxMessageBox(_T("Error : �������� ���� Recipe No �Դϴ�.!"));
		return;
	}

	m_bRecipeCopy = TRUE;
	m_nCopyRecipeNo = idx;
	m_strCopyDesFilePath.Format(_T("%s"), m_sRecipePath[m_iRecipeNo]);
	m_strCopySrcFilePath.Format(_T("%s"), m_sRecipePath[m_nCopyRecipeNo]);
}

void CDialogRecipeModel::RecipeFileCopy()
{
	m_strCopyDesFilePath, m_strCopySrcFilePath;
}