// Dialog/DialogIOCtrl.cpp : 구현 파일입니다.
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
#include "Dialog/DialogGlassManuCtrl.h"
#include "Dialog/DialogFilmAlign.h"

#include "Dialog/DialogLightCtrl.h"
#include "afxdialogex.h"
#include "SerialPort.h"
#include "Dialog/DialogLogAlarm.h"

#include "EMIControlDlg.h"



// CDialogIOCtrl 대화 상자입니다.

DIO_CFG g_pDI_DEV_NO[] = {
	{1, 0x000},
	{1, 0x010},
	{1, 0x020},
	{1, 0x030},
	{1, 0x040}, 
	{1, 0x050},
	{1, 0x060},
	{1, 0x070},
	{1, 0x080},
	{1, 0x090},
	{1, 0x0A0},
	{1, 0x0B0},
	{1, 0x0C0},
	{1, 0x0D0},
};

DIO_CFG g_pDO_DEV_NO[] = {
	{1, 0x100},
	{1, 0x110},
	{1, 0x120},
	{1, 0x130},
	{1, 0x140},
	{1, 0x150},
	{1, 0x160},
	{1, 0x170},
	{1, 0x180},
	{1, 0x190},
	{1, 0x1A0},
	{1, 0x1B0},
};

IMPLEMENT_DYNAMIC(CDialogIOCtrl, CDialogEx)

CDialogIOCtrl::CDialogIOCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogIOCtrl::IDD, pParent)
{
	int size = sizeof(char);
	memset(m_sDILabel, NULL, sizeof(wchar_t) * DI_MAX_NUM * 100);
	memset(m_sDOLabel, NULL, sizeof(wchar_t) * DO_MAX_NUM * 100);

	memset(m_bDI, NULL, sizeof(BOOL) * DI_MAX_NUM);
	memset(m_bDO, NULL, sizeof(BOOL) * DO_MAX_NUM);
	memset(m_bDI_list, NULL, sizeof(BOOL) * DI_MAX_NUM);
	memset(m_bDO_list, NULL, sizeof(BOOL) * DO_MAX_NUM);

	m_bTest = TRUE;
}

CDialogIOCtrl::~CDialogIOCtrl()
{
	//Detect sensor power on
	SetWriteDO(DO_GLASS_DETECT_SENSOR_RELAY_ON_OFF, TRUE);

	KillTimer(0);
	KillTimer(1);
	CloseCCLink();
}

void CDialogIOCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IO_INPUT, m_list_input);
	DDX_Control(pDX, IDC_LIST_IO_OUTPUT, m_list_output);
}


BEGIN_MESSAGE_MAP(CDialogIOCtrl, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_IO_OUTPUT, &CDialogIOCtrl::OnLvnItemchangedListIoOutput)
ON_NOTIFY(NM_DBLCLK, IDC_LIST_IO_OUTPUT, &CDialogIOCtrl::OnNMDblclkListIoOutput)
END_MESSAGE_MAP()


// CDialogIOCtrl 메시지 처리기입니다.

BOOL CDialogIOCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	int i;
	CString str;

	//IO label loading//////////////////////////////////////////////////////////
	LoadDILabelPara(_T(IO_PATH));

	//List//////////////////////////////////////////////////////////////////////
	LV_COLUMN m_lvColumn;
	CImageList *img_list_slct;
	CBitmap bitmap;

	img_list_slct = new CImageList;
	img_list_slct->Create(16, 16, ILC_COLOR4, 3, 3);
	
	bitmap.LoadBitmap(IDB_BITMAP_IO_OFF);
	img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_BITMAP_IO_ON);
	img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	m_list_input.SetImageList(img_list_slct, LVSIL_SMALL);
	m_list_output.SetImageList(img_list_slct, LVSIL_SMALL);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_LEFT;
	m_lvColumn.pszText=_T("INPUT NO.");
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=120;
	m_list_input.InsertColumn(0, &m_lvColumn);
	m_lvColumn.pszText=_T("OUTPUT NO.");
	m_list_output.InsertColumn(0, &m_lvColumn);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_LEFT;
	m_lvColumn.pszText=_T("NAME");
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=900;
	m_list_input.InsertColumn(1, &m_lvColumn);
	m_list_output.InsertColumn(1, &m_lvColumn);

	m_font.CreatePointFont(120, _T("굴림"));
	m_list_input.SetFont(&m_font, TRUE);
	m_list_output.SetFont(&m_font, TRUE);

	for(i = 0; i < DI_MAX_NUM; i++)
	{
		str.Format(_T("X%03x"), g_pDI_DEV_NO[0].nDevNo + i);
		m_list_input.InsertItem(i, str);

		m_list_input.SetItemText(i, 1, m_sDILabel[i]);
	}

	for(i = 0; i < DO_MAX_NUM; i++)
	{
		str.Format(_T("Y%03x"), g_pDO_DEV_NO[0].nDevNo + i);
		m_list_output.InsertItem(i, str);

		m_list_output.SetItemText(i, 1, m_sDOLabel[i]);
	}

	//CClink////////////////////////////////////////////////////////////////////
#ifdef MACHINE
	if(OpenCCLink(81) != 0)
	{
		AfxMessageBox(_T("Error : CCLink open fail!"));
	}
	else
	{
		if(InitCCLink(81) == -1)
		{
			AfxMessageBox(_T("Error : CCLink initialize fail!"));
		}
	}
#endif
	//SetTimer IO monitoring
	int io = 3;

	//ReadDIO(7);
	//WriteDO(113,TRUE);

	SetTimer(0, 10, NULL);
	SetTimer(1, 100, NULL);

	//초기 가동 IO
	SetWriteDO(DO_IONIZER_ION_ON_OFF, TRUE);
	SetWriteDO(DO_IONIZER_CDA_SOL_ON_OFF, TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//CCLink
short CDialogIOCtrl::OpenCCLink(short nChen)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
    nChen = 81;
	m_strlog.Format(_T("OpenCCLink(chen : %d"),nChen);
	pdlg->WriteLog(IO_LOG,m_strlog); 
    return(mdOpen(nChen, -1, &m_lPath));
}

short CDialogIOCtrl::CloseCCLink()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	m_strlog.Format(_T("CloseCCLink()"));
	pdlg->WriteLog(IO_LOG,m_strlog); 
    return(mdClose(m_lPath));
}

int CDialogIOCtrl::InitCCLink(short nChen)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	m_strlog.Format(_T("InitCCLink(nChen : %d)"),nChen);
	pdlg->WriteLog(IO_LOG,m_strlog); 
    return mdInit(nChen);
}

BOOL CDialogIOCtrl::ReadDIO(int io)
{
#ifdef MACHINE
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	 
	BOOL bRet = FALSE;

	if(io>=DI_START && io<=DI_END) 
	{
		int addr = (io-DI_START)/16;
		int bit = (io-DI_START)%16;
		
		int nDI_DEV_NO_cnt = sizeof(g_pDI_DEV_NO)/sizeof(g_pDI_DEV_NO[0]);
		addr = min(nDI_DEV_NO_cnt-1, addr);

		if(!g_pDI_DEV_NO[addr].bUse) return FALSE;

		short  Size    = 2;
		short  lReadBuffer1[2]={0,};
		short res = mdReceive(m_lPath, 0xFF, DevX, g_pDI_DEV_NO[addr].nDevNo, &Size, &lReadBuffer1);
		if(res!=0) return FALSE;
		bRet = (lReadBuffer1[0] & (1<<bit)) ? TRUE:FALSE;
		//m_strlog.Format(_T("ReadDIO(io : %s)"),m_strDILabel[io]);
		//pdlg->WriteLog(IO_LOG,m_strlog);
	} 
	else if(io>=DO_START && io<=DO_END)
	{
		int addr = (io-DO_START)/16;
		int bit = (io-DO_START)%16;
		
		int nDO_DEV_NO_cnt = sizeof(g_pDO_DEV_NO)/sizeof(g_pDO_DEV_NO[0]);
		addr = min(nDO_DEV_NO_cnt-1, addr);
		
		if(!g_pDO_DEV_NO[addr].bUse) return FALSE;
		
		short  Size    = 2;
		short  lReadBuffer1[2]={0,};
		short res = mdReceive(m_lPath, 0xFF, DevY, g_pDO_DEV_NO[addr].nDevNo, &Size, &lReadBuffer1);
		if(res!=0) return FALSE;
		bRet = (lReadBuffer1[0] & (1<<bit)) ? TRUE:FALSE;
		//m_strlog.Format(_T("ReadDIO(io : %s)"),m_strDOLabel[io - (DI_MAX_NUM + 1)]);
		//pdlg->WriteLog(IO_LOG,m_strlog);
	}
	return bRet;
#else
	return TRUE;
#endif
}

BOOL CDialogIOCtrl::WriteDO(int io, BOOL bValue)
{
#ifdef MACHINE
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;	
	if(io == (DO_STAGE_AIR_BLOW_SOL - 1) && m_bTest && bValue)
	{
		m_bTest = FALSE;
		m_strlog.Format(_T("blow on"));
		pdlg->WriteLog(IO_LOG,m_strlog);
	}
	else if(io == (DO_STAGE_AIR_BLOW_SOL - 1) && !m_bTest && !bValue)
	{
		m_bTest = TRUE;
		m_strlog.Format(_T("blow off"));
		pdlg->WriteLog(IO_LOG,m_strlog);
	}

	if(io>=DO_START && io<=DO_END) 
	{
		int addr = (io-DO_START)/16;
		int bit = (io-DO_START)%16;
		
		int nDO_DEV_NO_cnt = sizeof(g_pDO_DEV_NO)/sizeof(g_pDO_DEV_NO[0]);
		addr = min(nDO_DEV_NO_cnt-1, addr);
		
		if(!g_pDO_DEV_NO[addr].bUse) return FALSE;
		
		short  Size    = 2;
		short  lReadBuffer[2]={0,};
		short res = mdReceive(m_lPath, 0xFF, DevY, g_pDO_DEV_NO[addr].nDevNo, &Size, &lReadBuffer);
		if(res!=0) return FALSE;

		short nData = 1<<bit;
		if(bValue) {
			lReadBuffer[0] |= nData;
		} else {
			lReadBuffer[0] &= ~nData;
		}

		res = mdSend( m_lPath, 0xFF, DevY, g_pDO_DEV_NO[addr].nDevNo, &Size, &lReadBuffer);
		if(res !=0) return FALSE;

		//m_strlog.Format(_T("WriteDO(io : %s , bValue : %d)"),m_strDOLabel[io - (DI_MAX_NUM + 1)],bValue);
		//pdlg->WriteLog(IO_LOG,m_strlog); 
		return TRUE;
	}

	return FALSE;
#else
	return TRUE;
#endif
	
}

BOOL CDialogIOCtrl::LoadDILabelPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	int idx = 0;
	
	file_path = path + _T("\\DIOLabel.par");

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
		
		for(i = 0; i < DI_MAX_NUM; i++)
		{
			key.Empty();
			key.Format(_T("X%03x"), g_pDI_DEV_NO[0].nDevNo + i);
			GetPrivateProfileString(_T("DIGITAL INPUT LABEL"), key, _T("0.0"), m_sDILabel[i], sizeof(m_sDILabel), file_path);
			m_strDILabel[i] = m_strDILabel[i] + m_sDILabel[i];
		}

		for(i = 0; i < DO_MAX_NUM; i++)
		{
			key.Empty();
			key.Format(_T("Y%03x"), g_pDO_DEV_NO[0].nDevNo + i);
			GetPrivateProfileString(_T("DIGITAL OUTPUT LABEL"), key, _T("0.0"), m_sDOLabel[i], sizeof(m_sDOLabel), file_path);
			m_strDOLabel[i] = m_strDOLabel[i] + m_sDOLabel[i];
		}
		
	}
	fd=0;
	
	return rslt;
}

//Read input
void CDialogIOCtrl::ReadDisplayInputStatus(BOOL disp)
{
	int i;
	LV_ITEM lvitem;

	CString str;
	BOOL in;

	for(i = 0; i < DI_MAX_NUM; i++)
	{
		str.Format(_T("X%03x"), g_pDI_DEV_NO[0].nDevNo + i);
		
		//IO reading
		in = ReadDIO(i);
		if(in)
		{
			if((m_bDI_list[i] != in))
			{
				if(disp)
				{
					lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
					lvitem.iItem=i;
					lvitem.iSubItem=0;
					lvitem.iImage = 1;
					lvitem.pszText=(LPWSTR)(LPCWSTR)str;
					m_list_input.SetItem(&lvitem);
					m_bDI_list[i] = in;
				}
				
			}
			m_bDI[i] = in;
		}
		else
		{
			if((m_bDI_list[i] != in))
			{
				if(disp)
				{
					lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
					lvitem.iItem=i;
					lvitem.iSubItem=0;
					lvitem.iImage = 0;
					lvitem.pszText=(LPWSTR)(LPCWSTR)str;
					m_list_input.SetItem(&lvitem);
					m_bDI_list[i] = in;
				}
				
			}
			m_bDI[i] = in;
		}
	}
}

//Read output
void CDialogIOCtrl::ReadDisplayOutputStatus(BOOL disp)
{
	int i;
	LV_ITEM lvitem;

	CString str;
	BOOL out;

	for(i = 0; i < DO_MAX_NUM; i++)
	{
		str.Format(_T("Y%03x"), g_pDO_DEV_NO[0].nDevNo + i);
		
		//IO reading
		out = ReadDIO(i + DO_START);
		if(out)
		{
			if((m_bDO_list[i] != out))
			{
				if(disp)
				{
					lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
					lvitem.iItem=i;
					lvitem.iSubItem=0;
					lvitem.iImage = 1;
					lvitem.pszText=(LPWSTR)(LPCWSTR)str;
					m_list_output.SetItem(&lvitem);
					m_bDO_list[i] = out;
				}
				
			}
			m_bDO[i] = out;
			
		}
		else
		{
			if((m_bDO_list[i] != out))
			{
				if(disp)
				{
					lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
					lvitem.iItem=i;
					lvitem.iSubItem=0;
					lvitem.iImage = 0;
					lvitem.pszText=(LPWSTR)(LPCWSTR)str;
					m_list_output.SetItem(&lvitem);
					m_bDO_list[i] = out;
				}
				
			}
			m_bDO[i] = out;		
		}
		
	}
}

void CDialogIOCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow)
	{

	}
	else
	{

	}
}

void CDialogIOCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	switch(nIDEvent)
	{
	case 0:
		ReadDisplayInputStatus(IsWindowVisible());
		break;
	case 1:
		ReadDisplayOutputStatus(IsWindowVisible());
		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogIOCtrl::OnTimer)!"));
		m_strlog.Format(_T("Error : invalid index(CDialogIOCtrl::OnTimer)!"));
		pdlg->WriteLog(IO_LOG,m_strlog);
		
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDialogIOCtrl::OnNMDblclkListIoOutput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int idx = pNMListView->iItem;

	if(m_bDO[idx])
	{
		WriteDO(idx + DO_START, FALSE);
	}
	else
	{
		WriteDO(idx + DO_START, TRUE);
	}

	*pResult = 0;
}

void CDialogIOCtrl::SetWriteDO(int item, BOOL status)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	if(status)
	{
		WriteDO(item - 1, TRUE);
		m_strlog.Format(_T("SetWriteDO(item : %s , bstatus : %d)"),m_strDOLabel[item - (DI_MAX_NUM + 1)],status);	
		//pdlg->WriteLog(IO_LOG,m_strlog); 
	}
	else
	{
		WriteDO(item - 1, FALSE);
		m_strlog.Format(_T("SetWriteDO(item : %s , bstatus : %d)"),m_strDOLabel[item - (DI_MAX_NUM + 1)],status);
		//pdlg->WriteLog(IO_LOG,m_strlog); 
	}
}

void CDialogIOCtrl::SetGlassCtrlDO(int item,BOOL status)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	//20160715 ngh
	if((item == DO_STAGE_ALIGN_FRONT1_FW_SOL) || (item == DO_STAGE_SIDE_RIGHT1_FW_SOL) || 
		(item == DO_STAGE_SIDE_LIFT1_FW_SOL) || (item == DO_STAGE_ALIGN_REAR1_FW_SOL))
	{
		//20160729 pkh 버큠 인터락
		/*if(GetVacCheck())
		{
			return;
		}*/
	}

	if(status)
	{
		WriteDO(item - 1, TRUE);
		m_strlog.Format(_T("SetWriteDO(item : %s , bstatus : %d)"),m_strDOLabel[item - (DI_MAX_NUM + 1)],status);
		//pdlg->WriteLog(IO_LOG,m_strlog); 
	}
	else
	{
		WriteDO(item - 1, FALSE);
		m_strlog.Format(_T("SetWriteDO(item : %s , bstatus : %d)"),m_strDOLabel[item - (DI_MAX_NUM + 1)],status);
		//pdlg->WriteLog(IO_LOG,m_strlog); 
	}
}

BOOL CDialogIOCtrl::GetGlassCtrlReadDI(int in_idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	m_strlog.Format(_T("GetReadDI(in_idx : %s)"),m_strDILabel[in_idx]);
	//pdlg->WriteLog(IO_LOG,m_strlog);
	return m_bDI[in_idx];
}
BOOL CDialogIOCtrl::GetGlassCtrlReadDO(int out_idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	m_strlog.Format(_T("GetReadDO(in_idx : %s)"),m_strDOLabel[out_idx - (DO_MAX_NUM + 1)]);
	//pdlg->WriteLog(IO_LOG,m_strlog);
	return m_bDO[out_idx];
}

//ReadDIO(int io)

BOOL CDialogIOCtrl::GetReadDI(int in_idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	m_strlog.Format(_T("GetReadDI(in_idx : %s , status : %d)"),m_strDILabel[in_idx], m_bDI[in_idx]);
	//pdlg->WriteLog(IO_LOG,m_strlog);
	return m_bDI[in_idx];
}

BOOL CDialogIOCtrl::GetReadDO(int in_idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	m_strlog.Format(_T("GetReadDO(in_idx : %s , status : %d)"),m_strDOLabel[in_idx - (DO_MAX_NUM + 1)],m_bDO[in_idx]);
	//pdlg->WriteLog(IO_LOG,m_strlog);
	return m_bDO[in_idx];
}


BOOL CDialogIOCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CDialogIOCtrl::GetMotorCtrlReadDI(int in_idx)
{
#ifdef MACHINE
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	m_strlog.Format(_T("GetReadDI(in_idx : %s)"),m_strDILabel[in_idx]);
	//pdlg->WriteLog(IO_LOG,m_strlog);
#endif
	return m_bDI[in_idx];
}

BOOL CDialogIOCtrl::GetMotorCtrlReadDO(int out_idx)
{
#ifdef MACHINE
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	m_strlog.Format(_T("GetReadDO(in_idx : %s)"),m_strDOLabel[out_idx - (DO_MAX_NUM + 1)]);
	//pdlg->WriteLog(IO_LOG,m_strlog);
#endif
	return m_bDO[out_idx];
}

//20160715 ngh
BOOL CDialogIOCtrl::GetVacCheck()
{
	return GetReadDI(DI_CHUCK_VACUUM_ON_DETECT);
}

BOOL CDialogIOCtrl::GetGlassEdgeStatus()
{
	BOOL rslt = TRUE;

	if(!GetReadDI(DI_STAGE_GLS_EDGE_LEFT_FRONT) || !GetReadDI(DI_STAGE_GLS_EDGE_RIGHT_FRONT) || 
		!GetReadDI(DI_STAGE_GLS_EDGE_LEFT_REAR) || !GetReadDI(DI_STAGE_GLS_EDGE_RIGHT_REAR))
	{
		rslt = FALSE;
	}

	return rslt;
}