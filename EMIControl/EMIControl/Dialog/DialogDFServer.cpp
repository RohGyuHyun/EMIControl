// DialogDFServer.cpp : 구현 파일입니다.
//

#include "stdafx.h"

#include "EMIControl.h"

#include "Dialog\DialogInline.h"
#include "DialogDFServer.h"
#include "EMIControlDlg.h"

#include "afxdialogex.h"


BOOL RemoveDirectoryFile(LPCTSTR PathDir)
{
	if(PathDir == NULL)
	{
		return FALSE;
	}

	BOOL bReturnval = FALSE;

	CString szNextDirPath = _T("");
	CString szRoot = _T("");

	szRoot.Format(_T("%s\\*.*"), PathDir);
	CFileFind find;
	bReturnval = find.FindFile(szRoot);
	if(bReturnval == FALSE)
	{
		return bReturnval;
	}

	while(bReturnval)
	{
		bReturnval = find.FindNextFileW();

		if(find.IsDots() == FALSE)
		{
			if(find.IsDirectory())
			{
				RemoveDirectoryFile(find.GetFilePath());
			}
			else
			{
				bReturnval = DeleteFile(find.GetFilePath());
			}
		}
	}

	find.Close();
	bReturnval = RemoveDirectory(PathDir);
	
	return bReturnval;
}


// CDialogDFServer 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogDFServer, CDialogEx)

CDialogDFServer::CDialogDFServer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogDFServer::IDD, pParent)
	, m_edit_nWriteTimeOut(0)
{
	m_DefectInfo = new typeDefectInfo[DEFECT_DATA_MAX_NUM];
	memset(m_DefectInfo, NULL, sizeof(typeDefectInfo) * DEFECT_DATA_MAX_NUM);
}

CDialogDFServer::~CDialogDFServer()
{
	delete m_DefectInfo;
}

void CDialogDFServer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_WRITE_ON_CHECK, m_check_write_on_check);
	DDX_Control(pDX, IDC_IPADDRESS_SERVER, m_IPAddress_Server);
	DDX_Text(pDX, IDC_EDIT_ID, m_edit_strID);
	DDX_Text(pDX, IDC_EDIT_PW, m_edit_strPW);
	DDX_Text(pDX, IDC_EDIT_LOCAL_DIR, m_edit_strLocalDir);
	DDX_Text(pDX, IDC_EDIT_REMOTE_DIR, m_edit_strRemoteDir);
	DDX_Control(pDX, IDC_BTNENHCTRL_DF_CONNECT, m_btn_DFServerConnect);
	DDX_Control(pDX, IDC_CHECK_UP_LOAD_FILE_DEL, m_check_up_load_file_del);
	DDX_Text(pDX, IDC_EDIT_TIME_OUT, m_edit_nWriteTimeOut);
}


BEGIN_MESSAGE_MAP(CDialogDFServer, CDialogEx)
	ON_MESSAGE(USER_MSG_DEFECT_SERVER, OnDefectMessage)
END_MESSAGE_MAP()


// CDialogDFServer 메시지 처리기입니다.


BOOL CDialogDFServer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_bDefectInfo = FALSE;
	m_bXml = FALSE;
	m_ii = 0;
	m_ij = 0;
	//CreateXmlfile(_T("AA"), 1);

	CTime time;
	time = CTime::GetCurrentTime();
	
	
	m_timedata.month.Format(_T("%02d"),time.GetMonth());
	m_timedata.day.Format(_T("%02d"),time.GetDay());
	
	LoadDFServerPara(_T(IPS_PATH));
	UpdateData(FALSE);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
BEGIN_EVENTSINK_MAP(CDialogDFServer, CDialogEx)
	ON_EVENT(CDialogDFServer, IDC_BTNENHCTRL_IPS_SAVE, DISPID_CLICK, CDialogDFServer::ClickBtnenhctrlIpsSave, VTS_NONE)
	ON_EVENT(CDialogDFServer, IDC_BTNENHCTRL_DF_CONNECT, DISPID_CLICK, CDialogDFServer::ClickBtnenhctrlDfConnect, VTS_NONE)
	ON_EVENT(CDialogDFServer, IDC_BTNENHCTRL_DF_UP_LOAD, DISPID_CLICK, CDialogDFServer::ClickBtnenhctrlDfUpLoad, VTS_NONE)
	ON_EVENT(CDialogDFServer, IDC_BTNENHCTRL_DF_DOWN_LOAD, DISPID_CLICK, CDialogDFServer::ClickBtnenhctrlDfDownLoad, VTS_NONE)
	ON_EVENT(CDialogDFServer, IDC_BTNENHCTRL_DF_APPLY, DISPID_CLICK, CDialogDFServer::ClickBtnenhctrlDfApply, VTS_NONE)
	ON_EVENT(CDialogDFServer, IDC_BTNENHCTRL_DF_SAVE, DISPID_CLICK, CDialogDFServer::ClickBtnenhctrlDfSave, VTS_NONE)
	ON_EVENT(CDialogDFServer, IDC_BTNENHCTRL_DF_FIND, DISPID_CLICK, CDialogDFServer::ClickBtnenhctrlDfFind, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogDFServer::ClickBtnenhctrlIpsSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//DfHeaderUpdate();
	BOOL bXml = FALSE;
//	m_Xml->Create_Xml_File(_T("aaa"));
//	bXml = m_Xml->Xml_Format();
//	ReadDefect(0);
//	WriteXmlHeader(bXml);
	
}

CString CDialogDFServer::R_DefectFileName_a(CString glassid,CString procid,CString time,CString unitid)
{
	CString dfn,temp,temp2;
	temp.Format(_T(""));
	temp2.Format(_T(""));
	temp = time.Left(8);
	temp2 = time.Right(6);
	dfn.Format(_T("%s_%s_%s_%s.%s"),glassid,procid,temp,temp2,unitid);
	return dfn;
}

CString CDialogDFServer::H_Key_id_e(CString glassid,CString time)
{
	CString key_id;
	//key_id.Format(_T("%s_%s"),glassid,time);
	key_id.Format(_T(""));
	return key_id;
}

CString CDialogDFServer::H_Lot_id_e(char * lotid)
{
	CString temp;
	temp.Format(_T("%S"),lotid);
	return temp;
}

CString CDialogDFServer::H_Cst_id_e(char * cstid)
{
	CString temp;
	temp.Format(_T("%S"),cstid);
	return temp;
}

CString CDialogDFServer::H_Gls_id_e(CString glassid)
{
	CString temp;
	temp.Format(_T("%s"),glassid);
	return temp;
}

CString CDialogDFServer::H_Mo_gls_id_e2_e(CString glassid)
{
	CString mo_gls_id,glass;
	int length = 0;
	mo_gls_id.Format(_T(""));
	glass.Format(_T("%s"),glassid);
	length = glass.GetLength();
	mo_gls_id = glass.Left(length - 1);
	return mo_gls_id;
}

CString CDialogDFServer::H_Mo_gls_id_m1_e(CString glassid)
{
	CString mo_gls_id,glass;
	int length = 0;
	mo_gls_id.Format(_T(""));
	glass.Format(_T("%s"),glassid);
	length = glass.GetLength();
	mo_gls_id = glass.Left(length - 1);
	return mo_gls_id;
}

CString CDialogDFServer::H_Half_gls_id_e2_e(CString glassid)
{
	//CString half_gls_id,glass;
	//half_gls_id.Format(_T(""));
	//glass.Format(_T("%s"),glassid);
	//half_gls_id = glass.Left(12);
	return glassid;
}

CString CDialogDFServer::H_Half_gls_id_m1_e(CString glassid)
{
	//CString half_gls_id,glass;
	//half_gls_id.Format(_T(""));
	//glass.Format(_T("%s"),glassid);
	//half_gls_id = glass.Left(13);
	return glassid;
}

CString CDialogDFServer::H_Cf_id_e(char * cfid)
{
	CString temp;
	temp.Format(_T("%S"),cfid);
	return temp;
}

CString CDialogDFServer::H_Gls_cut_e2_e(CString glassid)
{
	CString gls_cut,glass;
	gls_cut.Format(_T(""));
	glass.Format(_T("%s"),glassid);
	gls_cut = glass.Right(1);
	return gls_cut;
}

CString CDialogDFServer::H_Gls_cut_m1_e(CString glassid)
{
	CString gls_cut,glass;
	gls_cut.Format(_T(""));
	glass.Format(_T("%s"),glassid);
	gls_cut = glass.Right(1);
	return gls_cut;
}

CString CDialogDFServer::H_Slot_no_e(CString glassid)
{
	CString slot_no,glass;
	slot_no.Format(_T(""));
	glass.Format(_T("%s"),glassid);
	slot_no = glass.Right(1);
	return slot_no;
}

CString CDialogDFServer::H_Eqp_id_e(char * eqpid)
{
	CString temp;
	temp.Format(_T("%S"),eqpid);
	return temp;
}

CString CDialogDFServer::H_Machine_id_e(char * machineid)
{
	CString temp;
	temp.Format(_T("%S"),machineid);
	return temp;
}

CString CDialogDFServer::H_Unit_id_e(char * unitid)
{
	CString temp;
	temp.Format(_T("%S"),unitid);
	return temp;
}

CString CDialogDFServer::H_Oper_id_e(char * operid)
{
	CString temp;
	temp.Format(_T("%S"),operid);
	return temp;
}

CString CDialogDFServer::H_Proc_id_e(char * procid)
{
	CString temp;
	temp.Format(_T("%S"),procid);
	return temp;
}

CString CDialogDFServer::H_Recipe_id_e(int recipeid)
{
	CString temp;
	temp.Format(_T("%d"),recipeid);
	return temp;
}



void CDialogDFServer::DfHeaderUpdate()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	//UpdateData(TRUE);
	typeAPDDataPara apdData;
	CString strlog;
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() Start!")); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	apdData = pdlg->m_Inline->GetAPD();
#ifndef CAMERA
	SYSTEMTIME cTime;
	//::GetLocalTime(&cTime);
	/*apdData.GlassStartTime.wYear = cTime.wYear;
	apdData.GlassStartTime.wMonth = cTime.wMonth;
	apdData.GlassStartTime.wDay = cTime.wDay;
	apdData.GlassStartTime.wHour = cTime.wHour;
	apdData.GlassStartTime.wMinute = cTime.wMinute;
	apdData.GlassStartTime.wSecond = cTime.wSecond;

	apdData.GlassEndTime.wYear = cTime.wYear;
	apdData.GlassEndTime.wMonth = cTime.wMonth;
	apdData.GlassEndTime.wDay = cTime.wDay;
	apdData.GlassEndTime.wHour = cTime.wHour;
	apdData.GlassEndTime.wMinute = cTime.wMinute;
	apdData.GlassEndTime.wSecond = cTime.wSecond;*/
#endif


	CString inspec_start_time,inspec_end_time,temp1,lotintime;
	char temp[] = {"ABCD"};
	CString glass_id;
	glass_id.Format(_T("%S"), pdlg->m_Inline->GetLoaderGlassData().strGlassID);
	inspec_start_time.Format(_T("%04d%02d%02d%02d%02d%02d"),apdData.GlassStartTime.wYear,apdData.GlassStartTime.wMonth,apdData.GlassStartTime.wDay,apdData.GlassStartTime.wHour,apdData.GlassStartTime.wMinute,apdData.GlassStartTime.wSecond); //* 착공시간으로 변경 해야함
	inspec_end_time.Format(_T("%04d%02d%02d%02d%02d%02d"),apdData.GlassEndTime.wYear,apdData.GlassEndTime.wMonth,apdData.GlassEndTime.wDay,apdData.GlassEndTime.wHour,apdData.GlassEndTime.wMinute,apdData.GlassEndTime.wSecond);
	lotintime.Format(_T("%04d%02d%02d%02d%02d%02d"),m_lotintime.year,m_lotintime.month,m_lotintime.day,m_lotintime.hour,m_lotintime.min,m_lotintime.second);
		
	m_header_e.key_id = H_Key_id_e(glass_id,lotintime);
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() key_id : %s "),m_header_e.key_id); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_e.lot_id = H_Lot_id_e(pdlg->m_Inline->GetLoaderGlassData().strLotID);
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() lot_id : %s "),m_header_e.lot_id); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_e.cst_id = H_Cst_id_e(temp);		// *
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() cst_id : %s "),m_header_e.cst_id); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_e.gls_id = H_Gls_id_e(glass_id);
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() gls_id : %s "),m_header_e.gls_id); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_e.mo_gls_id_e2 = H_Mo_gls_id_e2_e(glass_id);
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() mo_gls_id_e2 : %s "),m_header_e.mo_gls_id_e2); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_e.mo_gls_id_m1 = H_Mo_gls_id_m1_e(glass_id);
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() mo_gls_id_m1 : %s "),m_header_e.mo_gls_id_m1); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_e.half_gls_id_e2 = H_Half_gls_id_e2_e(glass_id);
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() half_gls_id_e2 : %s "),m_header_e.half_gls_id_e2); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_e.half_gls_id_m1 = H_Half_gls_id_m1_e(glass_id);
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() half_gls_id_m1 : %s "),m_header_e.half_gls_id_m1); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_e.cf_id = H_Cf_id_e(temp);			//  *
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() cf_id : %s "),m_header_e.cf_id); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_e.gls_cut_e2 = H_Gls_cut_e2_e(glass_id);
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() gls_cut_e2 : %s "),m_header_e.gls_cut_e2); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_e.gls_cut_m1 = H_Gls_cut_m1_e(glass_id);
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() gls_cut_m1 : %s "),m_header_e.gls_cut_m1); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_e.slot_no = H_Slot_no_e(glass_id);
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() slot_no : %s "),m_header_e.slot_no); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);
	
	m_header_e.eqp_id.Format(_T("CCCCCC")); //  *
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() eqp_id : %s "),m_header_e.eqp_id); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_e.machine_id = H_Machine_id_e(temp); //  *
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() machine_id : %s "),m_header_e.machine_id); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_e.unit_id.Format(_T("AAAAAAA"));// = H_Unit_id_e(temp);  //  *
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() unit_id : %s "),m_header_e.unit_id); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_e.oper_id.Format(_T("%s"),pdlg->GetOperater()); //= H_Oper_id_e(temp);	 //  *
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() oper_id : %s "),m_header_e.oper_id); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_e.proc_id.Format(_T("PPPPPPP"));// = H_Proc_id_e(pdlg->m_Inline->GetLoaderGlassData().strProcessingCode);
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() proc_id : %s "),m_header_e.proc_id); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_e.recipe_id.Format(_T("%s"),pdlg->m_Recipe->GetRecipeName());// = H_Recipe_id_e(pdlg->m_Inline->GetLoaderGlassData().nHostRecipeNumber);
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() recipe_id : %s "),m_header_e.recipe_id); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	//temp1.Format(_T(""));
	//m_header_e.insp_time = H_Insp_time_e(temp1);  //  *
	m_header_e.insp_time.Format(_T(""));
//	strlog.Format(_T("Defect File Create : DfHeaderUpdate() insp_time : %s "),m_header_e.insp_time); 
//	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_a.insp_time_start = inspec_start_time;  //*
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() insp_time_start : %s "),m_header_a.insp_time_start); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	m_header_a.insp_time_end = inspec_end_time;  //*
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() insp_time_end : %s "),m_header_a.insp_time_end); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);


	m_defect_file_a.name.Format(_T("%s_%s_%04d%02d%02d_%02d%02d%02d.%s"),glass_id,m_header_e.proc_id,apdData.GlassStartTime.wYear,apdData.GlassStartTime.wMonth,apdData.GlassStartTime.wDay,apdData.GlassStartTime.wHour,apdData.GlassStartTime.wMinute,apdData.GlassStartTime.wSecond,m_header_e.unit_id); //R_DefectFileName_a(glass_id,(LPWSTR)pdlg->m_Inline->GetLoaderGlassData().strProcessingCode,time,m_header_e.unit_id);
	strlog.Format(_T("Defect File Create : DfHeaderUpdate() defectfilename : %s "),m_defect_file_a.name); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

}

void CDialogDFServer::DFBodyUpdate()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	//UpdateData(TRUE);

	m_body_a.main_IN.Format(_T(""));
	m_body_a.main_G.Format(_T(""));
	m_body_a.main_N.Format(_T(""));
	m_body_a.main_R.Format(_T(""));

	m_body_a.sub_IN.Format(_T(""));
	m_body_a.sub_G.Format(_T(""));
	m_body_a.sub_N.Format(_T(""));
	m_body_a.sub_R.Format(_T(""));

	m_body_a.latest_judge.Format(_T(""));
	m_body_a.latest_reason.Format(_T(""));

	m_body_a.fdi_judge.Format(_T(""));
	m_body_a.fdi_reason.Format(_T(""));

	m_body_a.frt_judge.Format(_T(""));
	m_body_a.frt_reason.Format(_T(""));

	m_body_a.ptl_judge.Format(_T(""));
	m_body_a.ptl_reason.Format(_T(""));

	m_body_a.gcl_judge.Format(_T(""));
	m_body_a.gcl_reason.Format(_T(""));

	m_body_a.cum.Format(_T("%d"),pdlg->GetRsltDefectSumCnt(DF_TOTAL));
	m_body_a.step.Format(_T("%d"),pdlg->GetRsltDefectSumCnt(DF_TOTAL));
	m_body_a.new_.Format(_T("%d"),pdlg->GetRsltDefectSumCnt(DF_TOTAL));

	m_body_a.step_s_rb.Format(_T("%d"),pdlg->GetRsltDefectCnt(DF_SMALL));
	m_body_a.step_m_rb.Format(_T("%d"),pdlg->GetRsltDefectCnt(DF_MEDIUM));
	m_body_a.step_l_rb.Format(_T("%d"),pdlg->GetRsltDefectCnt(DF_LARGE));
	m_body_a.step_o_rb.Format(_T("%d"),pdlg->GetRsltDefectCnt(DF_OVERFLOW));

	m_body_a.cum_s_rb.Format(_T("%d"),pdlg->GetRsltDefectCnt(DF_SMALL));
	m_body_a.cum_m_rb.Format(_T("%d"),pdlg->GetRsltDefectCnt(DF_MEDIUM));
	m_body_a.cum_l_rb.Format(_T("%d"),pdlg->GetRsltDefectCnt(DF_LARGE));
	m_body_a.cum_o_rb.Format(_T("%d"),pdlg->GetRsltDefectCnt(DF_OVERFLOW));

	m_body_a.step_s_rw.Format(_T("%d"),pdlg->GetRsltDefectWhiteCnt(DF_SMALL));
	m_body_a.step_m_rw.Format(_T("%d"),pdlg->GetRsltDefectWhiteCnt(DF_MEDIUM));
	m_body_a.step_l_rw.Format(_T("%d"),pdlg->GetRsltDefectWhiteCnt(DF_LARGE));
	m_body_a.step_o_rw.Format(_T("%d"),pdlg->GetRsltDefectWhiteCnt(DF_OVERFLOW));

	m_body_a.cum_s_rw.Format(_T("%d"),pdlg->GetRsltDefectWhiteCnt(DF_SMALL));
	m_body_a.cum_m_rw.Format(_T("%d"),pdlg->GetRsltDefectWhiteCnt(DF_MEDIUM));
	m_body_a.cum_l_rw.Format(_T("%d"),pdlg->GetRsltDefectWhiteCnt(DF_LARGE));
	m_body_a.cum_o_rw.Format(_T("%d"),pdlg->GetRsltDefectWhiteCnt(DF_OVERFLOW));

	m_body_e.ref.Format(_T(""));
	m_body_e.ptn.Format(_T(""));
}

CString CDialogDFServer::B_Panel_qty_e(int count)
{
	CString temp;
	temp.Format(_T("%d"),count);
	return temp;
}

CString CDialogDFServer::B_Main_e(int count,int type)
{
	CString temp;
	temp.Format(_T("%d"),count);

	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_IN:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_G:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_N:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_R:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::B_Sub_e(int count,int type)
{
	CString temp;
	temp.Format(_T("%d"),count);

	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_IN:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_G:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_N:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_R:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::B_Defect_no_e(int count)
{
	CString temp;
	temp.Format(_T("%d"),count);
	return temp;
}
CString	CDialogDFServer::B_Defect_no_total_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		m_body_e.total.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_CUM:
		temp.Format(_T("%d"),count);
		m_body_a.cum.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_STEP:
		temp.Format(_T("%d"),count);
		m_body_a.step.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_NEW:
		temp.Format(_T("%d"),count);
		m_body_a.new_.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::B_Defect_no_step_s_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RB:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RW:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::B_Defect_no_step_m_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RB:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RW:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::B_Defect_no_step_l_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RB:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RW:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::B_Defect_no_step_o_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RB:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RW:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}   

CString CDialogDFServer::B_Defect_no_cum_s_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RB:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RW:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::B_Defect_no_cum_m_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RB:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RW:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::B_Defect_no_cum_l_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RB:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RW:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::B_Defect_no_cum_o_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RB:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RW:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::B_Image_file_e(CString path)
{
	CString temp;
	temp.Format(_T("%s"),path);
	return temp;
}

CString CDialogDFServer::B_Image_file_ref_e(CString path)
{
	CString temp;
	temp.Format(_T("%s"),path);
	return temp;
}
CString CDialogDFServer::B_Image_file_ptn_e(CString path)
{
	CString temp;
	temp.Format(_T("%s"),path);
	return temp;
}

CString CDialogDFServer::P_H_Key_id_e(char *glassid,CString procid,CString time)
{
	CString key_id;
	key_id.Format(_T("%S_%s"),glassid,time);
	return key_id;
}

CString CDialogDFServer::P_H_Panel_id_e(char *glassid,CString cellid)
{
	CString panel_id;
	panel_id.Format(_T("%S_%s"),glassid,cellid);
	return panel_id;
}

CString CDialogDFServer::P_H_Main_sub_info()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString temp;
	temp.Format(_T("%s"),pdlg->m_RecipeScan->GetGlassMainsub());
	return temp;
}

CString CDialogDFServer::P_B_Defect_no_e(int count)
{
	CString temp;
	temp.Format(_T("%d"),count);
	return temp;
}

CString CDialogDFServer::P_B_Defect_no_total_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ATTRIBUTE_CUM:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_STEP:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::P_B_Defect_no_step_s_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RB:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RW:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::P_B_Defect_no_step_m_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RB:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RW:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::P_B_Defect_no_step_l_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RB:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RW:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::P_B_Defect_no_step_o_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RB:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RW:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::P_B_Defect_no_cum_s_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RB:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RW:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::P_B_Defect_no_cum_m_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RB:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RW:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::P_B_Defect_no_cum_l_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RB:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RW:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

CString CDialogDFServer::P_B_Defect_no_cum_o_e(int count,int type)
{
	CString temp;
	switch (type)
	{
	case ELEMENT:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RB:
		temp.Format(_T("%d"),count);
		break;
	case ATTRIBUTE_RW:
		temp.Format(_T("%d"),count);
		break;
	}
	return temp;
}

void CDialogDFServer::P_B_Defect_info_defect_e()
{

}



BOOL CDialogDFServer::ReadIDXFile(CString idxfilepath)
{
	char buffer[DAT_STR_LENG], *ps, *context,*strtemp;
	CString strName, strNum, temp;
	CString strNgInfo, strlog, lot_id,stridxcmpday;

	strlog.Format(_T("IDX file search : ReadIDXFile(idx file path : %s); Start!"),idxfilepath);
	DFWriteLog(strlog,DEFECT_SERVER_LOG);
	int nidxcmpday;
	int i,line_cnt = 0;
	int k;
	size_t l;
	BOOL rslt = FALSE;
	
	char strFile[260];
	BOOL bXml = FALSE;

	CString temp_data;

	for(i = 0;i < 100;i++)
	{
		m_idxfilecmp[i].month.Format(_T(""));
		m_idxfilecmp[i].day.Format(_T(""));
		m_idxfilecmp[i].unitid.Format(_T(""));
		m_idxfilecmp[i].lotid.Format(_T(""));
		m_idxfilecmp[i].recipe.Format(_T(""));
		m_idxfilecmp[i].proid.Format(_T(""));
		m_idxfilecmp[i].path.Format(_T(""));
		m_idxfilecmp[i].defectfilename.Format(_T(""));
	}
	

	sprintf_s(strFile,"%S",idxfilepath);
	FILE *stream;
	if((stream = _fsopen( strFile, "rb" , _SH_DENYNO)) == NULL)
	{
		for(int i = 0; i < 3; i++)
		{
			stream = _fsopen( strFile, "rb" , _SH_DENYNO);
			if(stream != NULL)
			{
				break;
			}
			//Sleep(100);
			if(i == 2)
			{
				strlog.Format(_T("IDX file search : ReadIDXFile(); %S file open fail!"),strFile);
				DFWriteLog(strlog,DEFECT_SERVER_LOG);
				return FALSE;
			}
		}
	}
	 
	while (fgets(buffer, MAX_STRING_LINE, stream) != NULL)  // 텍스트파일을 한줄씩 읽습니다. 
	{
		ps = strchr(buffer, '\n');							// 제일 뒤쪽의 new line의 char을 찿아.
		if (ps != NULL) *ps = '\0';							// new line의 char을 null문자로 바꿉니다. 

		strNgInfo.Format(_T("%S"),buffer);
		m_idxfilecmp[line_cnt].path.Format(_T("strNgInfo"));
		
		
		
		AfxExtractSubString(temp, strNgInfo, 3, '\\');
		m_idxfilecmp[line_cnt].month.Format(_T("%s"),temp);

		AfxExtractSubString(temp, strNgInfo, 4, '\\');
		m_idxfilecmp[line_cnt].day.Format(_T("%s"),temp);

		AfxExtractSubString(temp, strNgInfo, 5, '\\');
		m_idxfilecmp[line_cnt].recipe.Format(_T("%s"),temp);

		AfxExtractSubString(temp, strNgInfo, 6, '\\');
		m_idxfilecmp[line_cnt].unitid.Format(_T("%s"),temp);

		AfxExtractSubString(temp, strNgInfo, 7, '\\');
		m_idxfilecmp[line_cnt].lotid.Format(_T("%s"),temp);

		AfxExtractSubString(temp, strNgInfo, 8, '\\');
		m_idxfilecmp[line_cnt].defectfilename.Format(_T("%s"),temp);

		AfxExtractSubString(temp, m_idxfilecmp[line_cnt].defectfilename, 1, '_');
		m_idxfilecmp[line_cnt].proid.Format(_T("%s"),temp);

			
		int cn = 0;

		ps = strtok_s(buffer, ",", &context);
		while (ps) {
			if (cn == 0) strName.Format(_T("%s"), ps);
			else strNum.Format(_T("%s"), ps);
			ps = strtok_s(NULL, ",", &context);
			cn++;
		}
		TRACE("%s\r\n",buffer);
		
		line_cnt++;
	}
	if(ps == NULL)
		delete ps;

	fclose(stream);

	if(DefectFileDownload(m_idxfilecmp[line_cnt - 1]))
	{
		strlog.Format(_T("IDX file search : ReadIDXFile(); DefectFileDownload() Successful!"));
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
		strlog.Format(_T("IDX file search : ReadIDXFile(); Return TRUE End"));
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
		return TRUE;
	}
	else
	{
		strlog.Format(_T("IDX file search : ReadIDXFile();  DefectFileDownload() fail!"));
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
		strlog.Format(_T("IDX file search : ReadIDXFile(); Return FALSE End"));
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
		return FALSE;
	}
}

BOOL CDialogDFServer::DefectFileDownload(typeidxcmp idxfileinfo)
{

	CString temp,strlog;
	strlog.Format(_T("IDX file search : DefectFileDownload(typeidxcmp idxfileinfo); Start!"));
	DFWriteLog(strlog,DEFECT_SERVER_LOG);
	BOOL rslt = FALSE;
	if(!m_cFPTCilent.m_bServerConnect)
	{
		rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
	}
	else
	{
		m_cFPTCilent.Close();
		rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
	}
	if(m_cFPTCilent.DefectFileFind(idxfileinfo.month,idxfileinfo.day,idxfileinfo.recipe,idxfileinfo.unitid,idxfileinfo.lotid,idxfileinfo.defectfilename))
	{
		m_strBeforeDfFilePath.Format(_T("%s"),m_cFPTCilent.m_temppath);
		strlog.Format(_T("IDX file search : DefectFileFind(); return TRUE End!"));
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
		return TRUE;
		/*AfxExtractSubString(temp,m_cFPTCilent.m_temppath,0,'.');
		temp += _T(".xml");
		if(CopyFile_Df(m_cFPTCilent.m_temppath,temp))
		{
			m_strBeforeDfFilePath.Format(_T("%s"),temp);
			DeleteFile(m_cFPTCilent.m_temppath);			
		}
		*/
	}
	else
	{
		strlog.Format(_T("IDX file search : DefectFileFind(); return FALSE End!"));
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
		return FALSE;
	}
}

CString CDialogDFServer::GetBeforeDfFilePath(){return m_strBeforeDfFilePath;}
	

BOOL CDialogDFServer::CreateXmlfile(CString xmlpath,int cnt)
{
	CString strlog;
	strlog.Format(_T("Defect File Create : CreateXmlfile(xmlpath : %s) Start!"),xmlpath);
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	if(m_Xml->Create_Xml_File(xmlpath,cnt))
	{
		m_Xml->Xml_Format();
		m_Xml->SetAttribute(_T("DEFECT_FILE"),_T("NAME"),m_defect_file_a.name,DEFAULT);
		return TRUE;
	}
	else
	{
		strlog.Format(_T("Create_Xml_File() faild !!"));
		return FALSE;
	}
}

CString CDialogDFServer::DefectFilepath(CString def_path)
{
	CString temp;
	AfxExtractSubString(temp, def_path, 0, '.');
	m_strDfFilePath.Format(_T("%s.xml"),temp);
	return m_strDfFilePath;
}

BOOL CDialogDFServer::WriteXmlHeader(BOOL bRet)
{
	
	CString strlog;
	strlog.Format(_T("IDX file search : WriteXmlHeader() Start!"));
	DFWriteLog(strlog,DEFECT_SERVER_LOG);
	if(bRet)
	{
		m_Xml->PutText(_T("KEY_ID"),m_header_e.key_id,HEADER);
		m_Xml->PutText(_T("LOT_ID"),m_header_e.lot_id,HEADER);
		m_Xml->PutText(_T("CST_ID"),m_header_e.cst_id,HEADER);
		m_Xml->PutText(_T("GLS_ID"),m_header_e.gls_id,HEADER);
		m_Xml->PutText(_T("MO_GLS_ID"),m_header_e.mo_gls_id_e2,HEADER);
		m_Xml->PutText(_T("HALF_GLS_ID"),m_header_e.half_gls_id_e2,HEADER);
		m_Xml->PutText(_T("CF_ID"),m_header_e.cf_id,HEADER);
		m_Xml->PutText(_T("GLS_CUT"),m_header_e.gls_cut_e2,HEADER);
		m_Xml->PutText(_T("SLOT_NO"),m_header_e.slot_no,HEADER);
		m_Xml->PutText(_T("MACHINE_ID"),m_header_e.machine_id,HEADER);
		m_Xml->PutText(_T("UNIT_ID"),m_header_e.unit_id,HEADER);
		m_Xml->PutText(_T("OPER_ID"),m_header_e.oper_id,HEADER);
		m_Xml->PutText(_T("PROC_ID"),m_header_e.proc_id,HEADER);
		m_Xml->PutText(_T("RECIPE_ID"),m_header_e.recipe_id,HEADER);
		m_Xml->SetAttribute(_T("INSP_TIME"),_T("START"),m_header_a.insp_time_start,HEADER);
		m_Xml->SetAttribute(_T("INSP_TIME"),_T("END"),m_header_a.insp_time_end,HEADER);
	}
	else
	{
		strlog.Format(_T("CreateXmlfile() faild !!"));
		return FALSE;
	}
	return TRUE;
}
BOOL CDialogDFServer::WriteXmlBody(BOOL bXml)
{
	
	CString strlog,temp2;
	temp2.Format(_T("0"));
	if(bXml)
	{
/*		m_Xml->PutText(_T("PANEL_QTY"),m_body_e.panel_qty,BODY);
		m_Xml->PutText(_T("MAIN"),m_body_e.main,BODY);
		m_Xml->PutText(_T("SUB"),m_body_e.sub,BODY);
		m_Xml->PutText(_T("DEFECT_NO"),m_body_e.defect_no,BODY);
		m_Xml->PutText(_T("TOTAL"),m_body_e.total,BODY);
		m_Xml->PutText(_T("STEP_S"),m_body_e.step_s,BODY);
		m_Xml->PutText(_T("STEP_M"),m_body_e.step_m,BODY);
		m_Xml->PutText(_T("STEP_L"),m_body_e.step_l,BODY);
		m_Xml->PutText(_T("STEP_O"),m_body_e.step_o,BODY);
		m_Xml->PutText(_T("CUM_S"),m_body_e.cum_s,BODY);
		m_Xml->PutText(_T("CUM_M"),m_body_e.cum_m,BODY);
		m_Xml->PutText(_T("CUM_L"),m_body_e.cum_l,BODY);
		m_Xml->PutText(_T("CUM_O"),m_body_e.cum_o,BODY);
		m_Xml->PutText(_T("IMAGE_FILE"),m_body_e.image_file,BODY);		*/
//		m_Xml->PutText(_T("REF"),m_body_e.ref,BODY);
		

		m_Xml->SetAttribute(_T("MAIN"),_T("IN"),m_body_a.main_IN,BODY);
		m_Xml->SetAttribute(_T("MAIN"),_T("G"),m_body_a.main_G,BODY);
		m_Xml->SetAttribute(_T("MAIN"),_T("N"),m_body_a.main_N,BODY);
		m_Xml->SetAttribute(_T("MAIN"),_T("R"),m_body_a.main_R,BODY);
		m_Xml->SetAttribute(_T("SUB"),_T("IN"),m_body_a.sub_IN,BODY);
		m_Xml->SetAttribute(_T("SUB"),_T("G"),m_body_a.sub_G,BODY);
		m_Xml->SetAttribute(_T("SUB"),_T("N"),m_body_a.sub_N,BODY);
		m_Xml->SetAttribute(_T("SUB"),_T("R"),m_body_a.sub_R,BODY);

		CString temp;
		temp.Format(_T(""));
		m_Xml->SetAttribute(_T("LATEST"),_T("JUDGE"),temp,BODY);
		m_Xml->SetAttribute(_T("LATEST"),_T("REASON"),temp,BODY);
		m_Xml->SetAttribute(_T("FDI"),_T("JUDGE"),temp,BODY);
		m_Xml->SetAttribute(_T("FDI"),_T("REASON"),temp,BODY);
		m_Xml->SetAttribute(_T("FRT"),_T("JUDGE"),temp,BODY);
		m_Xml->SetAttribute(_T("FRT"),_T("REASON"),temp,BODY);
		m_Xml->SetAttribute(_T("PTL"),_T("JUDGE"),temp,BODY);
		m_Xml->SetAttribute(_T("PTL"),_T("REASON"),temp,BODY);
		m_Xml->SetAttribute(_T("GCL"),_T("JUDGE"),temp,BODY);
		m_Xml->SetAttribute(_T("GCL"),_T("REASON"),temp,BODY);
		m_Xml->SetAttribute(_T("TOTAL"),_T("CUM"),temp2,BODY);
		m_Xml->SetAttribute(_T("TOTAL"),_T("STEP"),temp2,BODY);
		m_Xml->SetAttribute(_T("TOTAL"),_T("NEW"),temp2,BODY);
		m_Xml->SetAttribute(_T("STEP_S"),_T("RB"),temp2,BODY);
		m_Xml->SetAttribute(_T("STEP_M"),_T("RB"),temp2,BODY);
		m_Xml->SetAttribute(_T("STEP_L"),_T("RB"),temp2,BODY);
		m_Xml->SetAttribute(_T("STEP_O"),_T("RB"),temp2,BODY);
		m_Xml->SetAttribute(_T("STEP_S"),_T("RW"),temp2,BODY);
		m_Xml->SetAttribute(_T("STEP_M"),_T("RW"),temp2,BODY);
		m_Xml->SetAttribute(_T("STEP_L"),_T("RW"),temp2,BODY);
		m_Xml->SetAttribute(_T("STEP_O"),_T("RW"),temp2,BODY);
		m_Xml->SetAttribute(_T("CUM_S"),_T("RB"),temp2,BODY);
		m_Xml->SetAttribute(_T("CUM_M"),_T("RB"),temp2,BODY);
		m_Xml->SetAttribute(_T("CUM_L"),_T("RB"),temp2,BODY);
		m_Xml->SetAttribute(_T("CUM_O"),_T("RB"),temp2,BODY);
		m_Xml->SetAttribute(_T("CUM_S"),_T("RW"),temp2,BODY);
		m_Xml->SetAttribute(_T("CUM_M"),_T("RW"),temp2,BODY);
		m_Xml->SetAttribute(_T("CUM_L"),_T("RW"),temp2,BODY);
		m_Xml->SetAttribute(_T("CUM_O"),_T("RW"),temp2,BODY);

		return TRUE;
	}
	else
	{
		strlog.Format(_T("CreateXmlfile() faild !!"));
		return FALSE;
	}

}
// m_panel_e.main_sub_info.Format(_T("%s"),P_H_Main_sub_info());
BOOL CDialogDFServer::WriteXmlPanel(int idx)
{
	CString temp;
	int nTemp = 0;
	temp.Format(_T(""));
	m_Xml->PutText(_T("KEY_ID"),m_header_e.key_id,PANEL + idx);
	m_Xml->PutText(_T("PANEL_ID"),m_panel_e.panel_id,DEFAULT + idx);
//	m_panel_e.main_sub_info.Format(_T("%s"),P_H_Main_sub_info());
//	m_Xml->PutText(_T("MAIN_SUB_INFO"),m_panel_e.main_sub_info,PANEL + idx);

	m_Xml->SetAttribute(_T("LATEST"),_T("JUDGE"),temp,PANEL + idx);
	m_Xml->SetAttribute(_T("LATEST"),_T("REASON"),temp,PANEL + idx);
	m_Xml->SetAttribute(_T("FDI"),_T("JUDGE"),temp,PANEL + idx);
	m_Xml->SetAttribute(_T("FDI"),_T("REASON"),temp,PANEL + idx);
	m_Xml->SetAttribute(_T("FRT"),_T("JUDGE"),temp,PANEL + idx);
	m_Xml->SetAttribute(_T("FRT"),_T("REASON"),temp,PANEL + idx);
	m_Xml->SetAttribute(_T("PTL"),_T("JUDGE"),temp,PANEL + idx);
	m_Xml->SetAttribute(_T("PTL"),_T("REASON"),temp,PANEL + idx);
	m_Xml->SetAttribute(_T("GCL"),_T("JUDGE"),temp,PANEL + idx);
	m_Xml->SetAttribute(_T("GCL"),_T("REASON"),temp,PANEL + idx);
	m_Xml->SetAttribute(_T("TOTAL"),_T("CUM"),m_panel_a.cum,PANEL + idx);
	m_Xml->SetAttribute(_T("TOTAL"),_T("STEP"),m_panel_a.step,PANEL + idx);
	m_Xml->SetAttribute(_T("STEP_S"),_T("RB"),m_panel_a.step_s_rb,PANEL + idx);
	m_Xml->SetAttribute(_T("STEP_M"),_T("RB"),m_panel_a.step_m_rb,PANEL + idx);
	m_Xml->SetAttribute(_T("STEP_L"),_T("RB"),m_panel_a.step_l_rb,PANEL + idx);
	m_Xml->SetAttribute(_T("STEP_O"),_T("RB"),m_panel_a.step_o_rb,PANEL + idx);
	m_Xml->SetAttribute(_T("STEP_S"),_T("RW"),m_panel_a.step_s_rw,PANEL + idx);
	m_Xml->SetAttribute(_T("STEP_M"),_T("RW"),m_panel_a.step_m_rw,PANEL + idx);
	m_Xml->SetAttribute(_T("STEP_L"),_T("RW"),m_panel_a.step_l_rw,PANEL + idx);
	m_Xml->SetAttribute(_T("STEP_O"),_T("RW"),m_panel_a.step_o_rw,PANEL + idx);
	m_Xml->SetAttribute(_T("CUM_S"),_T("RB"),m_panel_a.cum_s_rb,PANEL + idx);
	m_Xml->SetAttribute(_T("CUM_M"),_T("RB"),m_panel_a.cum_m_rb,PANEL + idx);
	m_Xml->SetAttribute(_T("CUM_L"),_T("RB"),m_panel_a.cum_l_rb,PANEL + idx);
	m_Xml->SetAttribute(_T("CUM_O"),_T("RB"),m_panel_a.cum_o_rb,PANEL + idx);
	m_Xml->SetAttribute(_T("CUM_S"),_T("RW"),m_panel_a.cum_s_rw,PANEL + idx);
	m_Xml->SetAttribute(_T("CUM_M"),_T("RW"),m_panel_a.cum_m_rw,PANEL + idx);
	m_Xml->SetAttribute(_T("CUM_L"),_T("RW"),m_panel_a.cum_l_rw,PANEL + idx);
	m_Xml->SetAttribute(_T("CUM_O"),_T("RW"),m_panel_a.cum_o_rw,PANEL + idx);
	

	return TRUE;
}


void CDialogDFServer::WriteDefectInfo(int idx1,int idx2)
{
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("SHOP"),m_panel_a.shop,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("DEF_NO"),m_panel_a.def_no,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("GLS_START_TIME"),m_panel_a.gls_start_time,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("POINT_X"),m_panel_a.point_x,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("POINT_Y"),m_panel_a.point_y,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("ORIGIN_POINT_X"),m_panel_a.origin_point_x,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("ORIGIN_POINT_Y"),m_panel_a.origin_point_y,DEFAULT + idx1,idx2);
//	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("POINT_G"),m_panel_a.point_g,DEFAULT + idx1,idx2);
//	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("POINT_D"),m_panel_a.point_d,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("ACTIVE_AREA"),m_panel_a.active_area,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("XPIXEL"),m_panel_a.xpixel,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("YPIXEL"),m_panel_a.ypixel,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("PIXEL"),m_panel_a.pixel,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("XSIZE"),m_panel_a.xsize,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("YSIZE"),m_panel_a.ysize,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("SIZE"),m_panel_a.size,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("RANK"),m_panel_a.rank,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("MODE"),m_panel_a.mode,DEFAULT + idx1,idx2);
//	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("BRIGHT"),m_panel_a.bright,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("PROC_ID"),m_header_e.proc_id,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("UNIT_ID"),m_header_e.unit_id,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("PTN_CODE"),m_panel_a.ptn_code,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("OPERATOR_ID"),m_panel_a.operator_id,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("PTN_CODE"),m_panel_a.ptn_code_df,DEFAULT + idx1,idx2);
//	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("JUDGE_FINAL"),m_panel_a.judge_final,DEFAULT + idx1,idx2);
//	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("AUTO_JUDGE_CHECK"),m_panel_a.auto_juge_check,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("REVIEW_X_SIZE"),m_panel_a.review_x_size,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("REVIEW_Y_SIZE"),m_panel_a.review_y_size,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("REVIEW_LAYER"),m_panel_a.review_layer,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("PXL_ZONE"),m_panel_a.pxl_zone,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("ENCAP_TYPE"),m_panel_a.encap_type,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("ZONE"),m_panel_a.zone,DEFAULT + idx1,idx2);
//	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("SIZE_H"),m_panel_a.size_h,DEFAULT + idx1,idx2);
//	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("RPC_AUTO_CAPTURE"),m_panel_a.rpc_auto_capture,DEFAULT + idx1,idx2);
//	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("JUDGE_EI_RPC"),m_panel_a.judge_ei_rpc,DEFAULT + idx1,idx2);
//	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("REASON_EI_RPC"),m_panel_a.reason_ei_rpc,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("EI_RPC_OP"),m_panel_a.ei_rpc_op,DEFAULT + idx1,idx2);
	m_Xml->SetDfAttribute(_T("DEFECT_INFO"),_T("DEFECT"),_T("IMAGE_FILE_NO"),m_panel_a.image_file_no,DEFAULT + idx1,idx2);


//	m_Xml->GetDoc()->save((_variant_t)m_Xml->GetFilePath());
	
//	m_Xml->SetAttribute(_T("IMG"),_T("SEQ"),m_panel_a.seq,DEFAULT + idx1);
//	m_Xml->SetAttribute(_T("IMG"),_T("NAME"),m_panel_a.name,DEFAULT + idx1);
//	m_Xml->SetAttribute(_T("IMG"),_T("PATH"),m_panel_a.path,DEFAULT + idx1);
}

void CDialogDFServer::SetDefectInfo(typeDefectInfo defInfo)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strlog;
	CString imageidxfilename,imagefilename,temp;
	BOOL bXml = FALSE,bRet = FALSE,rslt = FALSE;

	int idx = 0;
	int def_idx = 0;
	

	CTime time;
	time = CTime::GetCurrentTime();

	CString month,day;
	month.Format(_T("%02d"),time.GetMonth());
	day.Format(_T("%02d"),time.GetDay());

	m_dfinfo.glsid.Format(_T("%S"), defInfo.glsid);
	m_dfinfo.time.Format(_T("%S"), defInfo.time);
	m_dfinfo.index.Format(_T("%S"), defInfo.index);
	m_dfinfo.cellid.Format(_T("%S"), defInfo.cellid);
	m_dfinfo.width = defInfo.width;
	m_dfinfo.height = defInfo.height;
	m_dfinfo.measure_x_e2 = defInfo.measure_x_e2;
	m_dfinfo.measure_y_e2 = defInfo.measure_y_e2;
	m_dfinfo.measure_x_ap2 = defInfo.measure_x_ap2;
	m_dfinfo.measure_y_ap2 = defInfo.measure_y_ap2;
	m_dfinfo.size_long = defInfo.size_long;
	m_dfinfo.area = defInfo.area;
	//m_dfinfo.areaper;
	m_dfinfo.size_range.Format(_T("%S"), defInfo.size_range);
	m_dfinfo.rank.Format(_T("%S"), defInfo.rank);
	m_dfinfo.in_out.Format(_T("%S"), defInfo.in_out);
	m_dfinfo.gray_min = defInfo.gray_min;
	m_dfinfo.gray_25 = defInfo.gray_25;
	m_dfinfo.gray_mediean = defInfo.gray_mediean;
	m_dfinfo.gray_75 = defInfo.gray_75;
	m_dfinfo.gray_max = defInfo.gray_max;
	m_dfinfo.gray_mean = defInfo.gray_mean;
	m_dfinfo.gray_deviation = defInfo.gray_deviation;
	m_dfinfo.black_white.Format(_T("%S"), defInfo.black_white);
	m_dfinfo.image_path.Format(_T("%S"), defInfo.image_path);
	m_dfinfo.bright = pdlg->m_RecipeScan->GetScanBright();
	m_dfinfo.width_pix = defInfo.width_pix;
	m_dfinfo.height_pix = defInfo.height_pix;
	m_dfinfo.area_pix = defInfo.area_pix;
	m_idf_count_ = 0;
	def_idx = pdlg->m_Glass->GetDfPanelidx(m_dfinfo.cellid,DEFECT_FILE_CREATE);

	int aaa = m_dfinfo.cellid.GetLength();
	if(0 < aaa)
	{
		

		int size_type = 0;
		if(!m_dfinfo.rank.Compare(_T("S")))
			size_type = DF_SMALL;
		else if(!m_dfinfo.rank.Compare(_T("M")))
			size_type = DF_MEDIUM;
		else if(!m_dfinfo.rank.Compare(_T("L")))
			size_type = DF_LARGE;
		else if(!m_dfinfo.rank.Compare(_T("O")))
			size_type = DF_OVERFLOW;
		else
		{
			strlog.Format(_T("DFPanelUpdate() rank : size error "));
		}


		switch (size_type)
		{
		case DF_SMALL:
			if(!m_dfinfo.black_white.Compare(_T("B")))
			{
				m_sizecnt[m_ii][m_ij].step_sb += 1;
				m_sizecnt[m_ii][m_ij].cum_sb += 1;
				m_Xml->TotalCountSetAttribute(DF_SMALL,0);
			}
			else
			{
				m_sizecnt[m_ii][m_ij].step_sw += 1;
				m_sizecnt[m_ii][m_ij].cum_sw += 1;
				m_Xml->TotalCountSetAttribute(DF_SMALL,1);
			}
			break;
		case DF_MEDIUM:
			if(!m_dfinfo.black_white.Compare(_T("B")))
			{
				m_sizecnt[m_ii][m_ij].step_mb += 1;
				m_sizecnt[m_ii][m_ij].cum_mb += 1;
				m_Xml->TotalCountSetAttribute(DF_MEDIUM,0);
			}
			else
			{
				m_sizecnt[m_ii][m_ij].step_mw += 1;
				m_sizecnt[m_ii][m_ij].cum_mw += 1;
				m_Xml->TotalCountSetAttribute(DF_MEDIUM,1);
			}
			break;
		case DF_LARGE:
			if(!m_dfinfo.black_white.Compare(_T("B")))
			{
				m_sizecnt[m_ii][m_ij].step_lb += 1;
				m_sizecnt[m_ii][m_ij].cum_lb += 1;
				m_Xml->TotalCountSetAttribute(DF_LARGE,0);
			}
			else
			{
				m_sizecnt[m_ii][m_ij].step_lw += 1;
				m_sizecnt[m_ii][m_ij].cum_lw += 1;
				m_Xml->TotalCountSetAttribute(DF_LARGE,1);
			}
			break;
		case DF_OVERFLOW:
			if(!m_dfinfo.black_white.Compare(_T("B")))
			{
				m_sizecnt[m_ii][m_ij].step_ob += 1;
				m_sizecnt[m_ii][m_ij].cum_ob += 1;
				m_Xml->TotalCountSetAttribute(DF_OVERFLOW,0);
			}
			else
			{
				m_sizecnt[m_ii][m_ij].step_ow += 1;
				m_sizecnt[m_ii][m_ij].cum_ow += 1;
				m_Xml->TotalCountSetAttribute(DF_OVERFLOW,1);
			}
			break;
		}
		DFPanelUpdate(m_idf_count_,def_idx);

		///*if(!m_cFPTCilent.m_bServerConnect)
		//{
		//	rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
		//}
		//else
		//{
		//	m_cFPTCilent.Close();
		//	rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
		//}*/
		imagefilename.Format(_T("%s_%s_%s_%s_%s_%s.JPG"),m_header_e.lot_id,m_header_e.gls_id,m_panel_e.panel_id,m_header_e.unit_id,m_header_e.proc_id,m_dfinfo.index);
		imageidxfilename.Format(_T("%s,%.3f,%.3f,%s_%s_%s_%s_%s.JPG"),m_dfinfo.index,m_dfinfo.measure_x_e2,m_dfinfo.measure_y_e2,m_header_e.lot_id,m_header_e.gls_id,m_header_e.unit_id,m_header_e.proc_id,m_dfinfo.index);
		//속도 범인
		ImageFileCopy(m_dfinfo.image_path,DEFECT_IMAGE2,m_header_e.gls_id,m_header_e.recipe_id,m_header_e.lot_id,m_header_e.proc_id,m_header_e.unit_id,m_header_e.eqp_id,m_panel_e.panel_id,m_dfinfo.measure_x_e2,m_dfinfo.measure_y_e2,m_dfinfo.index);
		if(m_dfinfo.index == '1')
		{
			ImageIdxFile(m_stream, m_header_e.gls_id,m_header_e.recipe_id,m_header_e.lot_id,m_header_e.proc_id,m_header_e.unit_id,m_defect_file_a.name,TRUE,m_imagemappath);
			CString imagemaptemp;
			imagemaptemp.Format(_T("%s"),m_imagemappath.Mid(2,m_imagemappath.GetLength() - 2));
			
			m_Xml->PutText(_T("PTN"),imagemaptemp,BODY);
		}
		ImageIdxFile(m_stream, m_header_e.gls_id,m_header_e.recipe_id,m_header_e.lot_id,m_header_e.proc_id,m_header_e.unit_id,m_defect_file_a.name,TRUE,imageidxfilename);
		
		//
		WriteXmlPanel(def_idx);
		DFInfoUpdate(def_idx);
		m_Xml->Xml_Format_Defect(def_idx);
		WriteDefectInfo(def_idx, m_idf_count_);
		
	}
	//strlog.Format(_T("SetDefectInfo() End!"));
	//DFWriteLog(strlog,DEFECT_SERVER_LOG);
}

BOOL CDialogDFServer::GetWriteOnCheck()
{
	//UpdateData(TRUE);
	return m_bWriteFile;
}

void CDialogDFServer::DFPanelUpdate(int count,int def_idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	int idx = 0,ret = 0;
	
	CString strlog;

	m_panel_e.key_id.Format(_T("%d"),m_header_e.key_id);
	m_panel_e.panel_id.Format(_T("%s%s"),m_dfinfo.glsid,m_dfinfo.cellid);
//	m_panel_e.main_sub_info.Format(_T("%s"),P_H_Main_sub_info());
	m_panel_e.judge.Format(_T(""));
	
	ret = m_Xml->GetElementCount(_T("PANEL"),_T("DEFECT_INFO"),idx);
//	ret = m_Xml->GetElementCountTest(_T("PANEL"),_T("DEFECT_INFO"),idx);
	m_panel_e.defect_no.Format(_T("%d"),count);
	m_panel_a.cum.Format(_T("%d"),count);
	m_panel_a.step.Format(_T("%d"),count);
	m_panel_a.step_s_rb.Format(_T("%d"),m_sizecnt[m_ii][m_ij].step_sb);
	m_panel_a.step_m_rb.Format(_T("%d"),m_sizecnt[m_ii][m_ij].step_mb);
	m_panel_a.step_l_rb.Format(_T("%d"),m_sizecnt[m_ii][m_ij].step_lb);
	m_panel_a.step_o_rb.Format(_T("%d"),m_sizecnt[m_ii][m_ij].step_ob);
	m_panel_a.step_s_rw.Format(_T("%d"),m_sizecnt[m_ii][m_ij].step_sw);
	m_panel_a.step_m_rw.Format(_T("%d"),m_sizecnt[m_ii][m_ij].step_mw);
	m_panel_a.step_l_rw.Format(_T("%d"),m_sizecnt[m_ii][m_ij].step_lw);
	m_panel_a.step_o_rw.Format(_T("%d"),m_sizecnt[m_ii][m_ij].step_ow);

	m_panel_a.cum_s_rb.Format(_T("%d"),m_sizecnt[m_ii][m_ij].cum_sb);
	m_panel_a.cum_m_rb.Format(_T("%d"),m_sizecnt[m_ii][m_ij].cum_mb);
	m_panel_a.cum_l_rb.Format(_T("%d"),m_sizecnt[m_ii][m_ij].cum_lb);
	m_panel_a.cum_o_rb.Format(_T("%d"),m_sizecnt[m_ii][m_ij].cum_ob);
	m_panel_a.cum_s_rw.Format(_T("%d"),m_sizecnt[m_ii][m_ij].cum_sw);
	m_panel_a.cum_m_rw.Format(_T("%d"),m_sizecnt[m_ii][m_ij].cum_mw);
	m_panel_a.cum_l_rw.Format(_T("%d"),m_sizecnt[m_ii][m_ij].cum_lw);
	m_panel_a.cum_o_rw.Format(_T("%d"),m_sizecnt[m_ii][m_ij].cum_ow);
	
	

}

void CDialogDFServer::DFInfoUpdate(int def_idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	int idx = 0,ret = 0;
	int size_type = 0;
	CString strlog;

//	idx = pdlg->m_Glass->GetDfPanelidx(m_panel_e.panel_id);
	ret = m_Xml->GetElementCount(_T("PANEL"),_T("DEFECT_INFO"),idx);
	m_panel_a.shop.Format(_T("CT"));
	m_panel_a.def_no.Format(_T("%d"),m_idf_count_);
	m_panel_a.gls_start_time.Format(_T("%s"),m_header_a.insp_time_start);
	m_panel_a.point_x.Format(_T("%.3f"),m_dfinfo.measure_x_e2);
	m_panel_a.point_y.Format(_T("%.3f"),m_dfinfo.measure_y_e2);
	m_panel_a.origin_point_x.Format(_T("%.3f"),m_dfinfo.measure_x_ap2);
	m_panel_a.origin_point_y.Format(_T("%.3f"),m_dfinfo.measure_y_ap2);
	m_panel_a.point_g.Format(_T(""));
	m_panel_a.point_d.Format(_T(""));
	m_panel_a.active_area.Format(_T("0"));
	m_panel_a.xpixel.Format(_T("%d"),m_dfinfo.width_pix);
	m_panel_a.ypixel.Format(_T("%d"),m_dfinfo.height_pix);
	m_panel_a.pixel.Format(_T("%d"),m_dfinfo.area_pix);
	m_panel_a.xsize.Format(_T("%.2f"),m_dfinfo.width * 1000);
	m_panel_a.ysize.Format(_T("%.2f"),m_dfinfo.height * 1000);
	m_panel_a.size.Format(_T("%.2f"),m_dfinfo.area);
	m_panel_a.rank.Format(_T("%s"),m_dfinfo.rank);
	m_panel_a.mode.Format(_T(""));
	m_panel_a.bright.Format(_T("%d"),m_dfinfo.bright);
	m_panel_a.proc_id.Format(_T(""));
	m_panel_a.unit_id.Format(_T(""));
	m_panel_a.ptn_code.Format(_T(""));
	m_panel_a.operator_id.Format(_T(""));
	m_panel_a.ptn_code_df.Format(_T(""));
	m_panel_a.judge_final.Format(_T(""));
	m_panel_a.auto_juge_check.Format(_T(""));
	m_panel_a.review_x_size.Format(_T(""));
	m_panel_a.review_y_size.Format(_T(""));
	m_panel_a.review_layer.Format(_T(""));
	m_panel_a.pxl_zone.Format(_T(""));
	m_panel_a.encap_type.Format(_T(""));
	m_panel_a.zone.Format(_T(""));
	m_panel_a.size_h.Format(_T(""));
	m_panel_a.rpc_auto_capture.Format(_T(""));
	m_panel_a.judge_ei_rpc.Format(_T(""));
	m_panel_a.reason_ei_rpc.Format(_T(""));
	m_panel_a.ei_rpc_op.Format(_T(""));
	m_panel_a.image_file_no.Format(_T(""));
}



void CDialogDFServer::SetDefectInfo(typedefectinfo defInfo, int idx)
{
	//m_DefectInfo[idx] = defInfo;
	if(!defInfo.index.Compare(_T("1")))
	{
		m_nDefectCnt = 0;
	}

	sprintf(m_DefectInfo[idx].glsid, "%S",  defInfo.glsid);
	sprintf(m_DefectInfo[idx].time, "%S",  defInfo.time);
	sprintf(m_DefectInfo[idx].index, "%S",  defInfo.index);
	sprintf(m_DefectInfo[idx].cellid, "%S",  defInfo.cellid);
	m_DefectInfo[idx].width = defInfo.width; 
	m_DefectInfo[idx].height = defInfo.height;
	m_DefectInfo[idx].measure_x_e2 = defInfo.measure_x_e2;
	m_DefectInfo[idx].measure_y_e2 = defInfo.measure_y_e2;
	m_DefectInfo[idx].measure_x_ap2 = defInfo.measure_x_ap2;
	m_DefectInfo[idx].measure_y_ap2 = defInfo.measure_y_ap2;
	m_DefectInfo[idx].size_long = defInfo.size_long;
	m_DefectInfo[idx].area = defInfo.area;
	//sprintf(m_DefectInfo[idx].areaper;
	sprintf(m_DefectInfo[idx].size_range, "%S",  defInfo.size_range);
	sprintf(m_DefectInfo[idx].rank, "%S",  defInfo.rank);
	sprintf(m_DefectInfo[idx].in_out, "%S",  defInfo.in_out);
	m_DefectInfo[idx].gray_min = defInfo.gray_min;
	m_DefectInfo[idx].gray_25 = defInfo.gray_25;
	m_DefectInfo[idx].gray_mediean = defInfo.gray_mediean;
	m_DefectInfo[idx].gray_75 = defInfo.gray_75;
	m_DefectInfo[idx].gray_max = defInfo.gray_max;
	m_DefectInfo[idx].gray_mean = defInfo.gray_mean;
	m_DefectInfo[idx].gray_deviation = defInfo.gray_deviation;
	sprintf(m_DefectInfo[idx].black_white, "%S",  defInfo.black_white);
	sprintf(m_DefectInfo[idx].image_path, "%S",  defInfo.image_path);
	m_DefectInfo[idx].width_pix = defInfo.width_pix;
	m_DefectInfo[idx].height_pix = defInfo.height_pix;
	m_DefectInfo[idx].area_pix = defInfo.area_pix;
	m_nDefectCnt++;
}

CString CDialogDFServer::GetGlassIDHash(CString glassid,int hashidx)
{
	CString temp,hash1;

	char cIndexName[MAX_PATH],cHashPath[MAX_PATH];
	CHash hash;
	sprintf(cIndexName, "%S", glassid);
	hash.GetHashPathName(cIndexName, cHashPath);
	hash1.Format(_T("%S"),cHashPath);
	if(hashidx == 0)
		AfxExtractSubString(temp,hash1,0,'\\');
	else
		AfxExtractSubString(temp,hash1,1,'\\');
	return temp;
}



BOOL CDialogDFServer::GlassIdxFile(CString glassid,CString recipe,CString lotid,CString procid,CString unitid,CString DFfilename)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString idxfilename,filepath,data,hash1,strIdxPath,strDfFilePath,strHostPath;
	CString strlog;

	strlog.Format(_T("Defect File Create : GlassIdxFile(glassid : %s,recipe : %s,lotid : %s,procid : %s,unitid : %s,DFfilename : %s) Start!"),glassid, recipe, lotid, procid, unitid, DFfilename); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);
	
	char strFile[512];
	char strTemp[512];
	long	result;
	FILE *fp, *stream;
	CTime time;
	time = CTime::GetCurrentTime();

	char cIndexName[MAX_PATH],cHashPath[MAX_PATH];
	CHash hash;
	sprintf(cIndexName, "%S", glassid);
	hash.GetHashPathName(cIndexName, cHashPath);
	hash1.Format(_T("%S"),cHashPath);
	strlog.Format(_T("Defect File Create : GlassIdxFile() HashPath : %s "),hash1); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	idxfilename.Format(_T("%s.IDX"),glassid);
	strlog.Format(_T("Defect File Create : GlassIdxFile() IDX file name : %s "),idxfilename); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	filepath.Format(_T("C:\\DEFECT\\INDEX\\%s\\%s"),hash1,idxfilename);
	data.Format(_T("C:\\DEFECT\\INSPECTOR\\%02d\\%02d\\%s\\%s\\%s\\%s"),time.GetMonth(),time.GetDay(),recipe,unitid,lotid,DFfilename);
	
	strDfFilePath.Format(_T("%02d\\%02d\\%s\\%s\\%s"),time.GetMonth(),time.GetDay(),recipe,unitid,lotid);
	strHostPath.Format(_T("C:\\DEFECT\\HOST\\%s\\%s"),recipe,DFfilename);

	DefectFilepath(data);
	strIdxPath.Format(_T("%s"),hash1);
	sprintf(strFile, "%S", filepath);

	_MakeDirectories(_T("C:\\DEFECT\\HOST\\") + recipe);
	_MakeDirectories(_T("C:\\DEFECT\\INDEX\\") + strIdxPath);
	_MakeDirectories(_T("C:\\DEFECT\\INSPECTOR\\") + strDfFilePath);
	

	if((stream = _fsopen(strFile, "a+", _SH_DENYNO)) == NULL)
	{
		HANDLE fd = CreateFile( (LPCWSTR)(LPCSTR)strFile,
							GENERIC_READ|GENERIC_WRITE,
							//GENERIC_WRITE,
							FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);

		stream = _fsopen(strFile, "a+", _SH_DENYNO);
		result = fseek(stream, 0L, SEEK_END);

		if(stream == NULL)
		{
			CloseHandle(fd);
			return FALSE;
		}

		CloseHandle(fd);
	}
	result = fseek(stream, 0L, SEEK_END);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}
	int iret = 0;
	sprintf_s(strTemp, data.GetLength() + 1, "%S", data);
    iret = fputs(strTemp, stream);
	iret = fputs("\n", stream);
	fclose(stream);
	BOOL rslt = FALSE;
	if(!m_cFPTCilent.m_bServerConnect)
	{
		rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
	}
	else
	{
		m_cFPTCilent.Close();
		rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
	}
	if(m_cFPTCilent.IDXUpload(filepath,hash1,glassid))
	{
		strlog.Format(_T("Defect File Create : IDXUpload(); return TRUE End! "),idxfilename); 
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
		return TRUE;
	}
	else
	{
		strlog.Format(_T("Defect File Create : IDXUpload(); return FALSE End! "),idxfilename); 
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
		return FALSE;
	}
}

CString CDialogDFServer::GetImageIdxFile(){return m_ImageIdxFile;}
BOOL CDialogDFServer::ImageIdxFile(FILE *stream, CString glassid,CString recipe,CString lotid,CString procid,CString unitid,CString DFfilename,BOOL filetype,CString data)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString idxfilename,idxtemppath,ref_filepath,ref_data,ptnmnt_data,filepath,strMakeFilePath;
	CString strlog;

	//strlog.Format(_T("Defect File Create : ImageIdxFile(glassid : %s,recipe : %s,lotid : %s,procid : %s,unitid : %s,DFfilename : %s,filetype : %d,data : %s); Start! "), glassid, recipe, lotid, procid, unitid, DFfilename, filetype, data); 
	//DFWriteLog(strlog,DEFECT_SERVER_LOG);
	
	BOOL result;

	char strTemp[512];

	result = fseek(stream, 0L, SEEK_END);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}

	sprintf_s(strTemp, data.GetLength() + 1, "%S", data);
    fputs(strTemp, stream);
	fputs("\n", stream);
	//fclose(stream);

	return TRUE;
	
}

BOOL CDialogDFServer::GetWriteDefectFile()
{
	return m_bWriteDefectFile;
}

BOOL CDialogDFServer::CopyFile_Df(CString sourcepath,CString targetpath)
{
	CString strlog;
	if(!CopyFile(sourcepath, targetpath, FALSE))
	{
		strlog.Format(_T("Error : Copy faild !! : %s : %s"),sourcepath,targetpath);
		return FALSE;
	}
	return TRUE;
}

CString CDialogDFServer::GetImageFilePath(int idx){return m_imagefilepath[idx];}

void CDialogDFServer::ImageFileCopy(CString sourcepath,int type,CString glassid,CString recipe,CString lotid,CString procid,CString unitid,CString eqp,CString panelid,double x,double y,CString no)
{
	CTime time;
	time = CTime::GetCurrentTime();
	CString imagepath,strMakeFilePath,strlog,imagemappath,temp,imagemappath2;
	BOOL bRet = FALSE;
	int count = 0;
	count = _wtoi(no);
	::AfxExtractSubString(temp,sourcepath,2,'ww');
	imagemappath.Format(_T("D:\\Result\\%s\\%s\\%s\\GlassWindowMap.bmp"),temp,lotid,glassid);
	switch(type)
	{
	case DEFECT_IMAGE:
		imagepath.Format(_T("C:\\IMAGE\\DEFECT_IMAGE\\%02d\\%02d\\%s\\%s\\%s\\%s\\%s_%s_%s_%s_%s_%.3f_%.3f_%s.JPG"),time.GetMonth(),time.GetDay(),recipe,unitid,lotid,glassid,lotid,glassid,panelid,unitid,procid,x,y,no);
		m_imagefilepath[count - 1].Format(_T("%s"),imagepath);
		m_strImgFilePath.Format(_T("IMAGE\\DEFECT_IMAGE\\%02d\\%02d\\%s\\%s\\%s\\%s\\"),time.GetMonth(),time.GetDay(), recipe, unitid, lotid,glassid);
		break;
	case DEFECT_IMAGE2:
		imagepath.Format(_T("C:\\IMAGE\\DEFECT_IMAGE2\\%02d\\%02d\\%s\\%s\\%s\\%s\\%s_%s_%s_%s_%s_%.3f_%.3f_%s.JPG"),time.GetMonth(),time.GetDay(),recipe,unitid,lotid,glassid,lotid,glassid,panelid,unitid,procid,x,y,no);
		m_imagefilepath[count - 1].Format(_T("%s"),imagepath);
		m_strImgFilePath.Format(_T("IMAGE\\DEFECT_IMAGE2\\%02d\\%02d\\%s\\%s\\%s\\%s\\"),time.GetMonth(),time.GetDay(), recipe, unitid, lotid,glassid);
		break;
	case REF_IMAGE:
		imagepath.Format(_T("C:\\IMAGE\\REF_IMAGE\\%02d\\%02d\\%s\\%s\\%s\\%s\\%s_%s_%s_%s_%s_%.3f_%.3f_%s.JPG"),time.GetMonth(),time.GetDay(),recipe,unitid,lotid,glassid,lotid,glassid,panelid,unitid,procid,x,y,no);
		m_imagefilepath[count - 1].Format(_T("%s"),imagepath);
		m_strImgFilePath.Format(_T("IMAGE\\REF_IMAGE\\%02d\\%02d\\%s\\%s\\%s\\%s\\"),time.GetMonth(),time.GetDay(), recipe, unitid, lotid,glassid);
		break;
	case PTNMNT_IMAGE:
		imagepath.Format(_T("C:\\IMAGE\\PTNMNT_IMAGE\\%02d\\%02d\\%s\\%s\\%s\\%s\\%s_%s_%s_%s_%s_%.3f_%.3f_%s.JPG"),time.GetMonth(),time.GetDay(),recipe,unitid,lotid,glassid,lotid,glassid,panelid,unitid,procid,x,y,no);
		m_imagefilepath[count - 1].Format(_T("%s"),imagepath);
		m_strImgFilePath.Format(_T("IMAGE\\PTNMNT_IMAGE\\%02d\\%02d\\%s\\%s\\%s\\%s\\"),time.GetMonth(),time.GetDay(), recipe, unitid, lotid,glassid);
		break;
	}
	/*if(rslt)
		imagepath.Format(_T("C:\\IMAGE\\DEFECT_IMAGE\\%02d\\%02d\\%s\\%s\\%s\\%s_%s_%s_%s_%s.JPG"),time.GetMonth(),time.GetDay(),recipe,eqp,lotid,lotid,glassid,eqp,unitid,m_dfinfo.index); // RFN
	else
		imagepath.Format(_T("C:\\IMAGE\\DEFECT_IMAGE\\%02d\\%02d\\%s\\%s\\%s\\%s_%s_%s_%s_%s.JPG"),time.GetMonth(),time.GetDay(),recipe,eqp,lotid,lotid,glassid,eqp,unitid,m_dfinfo.index);  //PTN  임시
	*/
	

	strMakeFilePath.Format(_T("%02d\\%02d\\%s\\%s\\%s\\%s"),time.GetMonth(),time.GetDay(),recipe,unitid,lotid,glassid);
	
	//m_strImgFilePath.Format(_T("IMAGE\\DEFECT_IMAGE\\%02d\\%02d\\%s\\%s\\%s\\%s\\"),m_timedata.month,m_timedata.day, recipe, unitid, lotid,glassid);

	_MakeDirectories(_T("C:\\IMAGE\\DEFECT_IMAGE\\") + strMakeFilePath);
	_MakeDirectories(_T("C:\\IMAGE\\DEFECT_IMAGE2\\") + strMakeFilePath);
	_MakeDirectories(_T("C:\\IMAGE\\REF_IMAGE\\") + strMakeFilePath);
	_MakeDirectories(_T("C:\\IMAGE\\PTNMNT_IMAGE\\") + strMakeFilePath);
	if(no == '1')
	{
		
		temp.Format(_T(""));
		bRet = AfxExtractSubString(temp,sourcepath,2,'\\');
		imagemappath.Format(_T("D:\\Result\\%s\\%s\\%s\\GlassWindowMap.bmp"),temp,lotid,glassid);
		m_imagemappath.Format(_T("C:\\IMAGE\\DEFECT_IMAGE2\\%02d\\%02d\\%s\\%s\\%s\\%s\\%s_%s_%s_%s_%s.JPG"),time.GetMonth(),time.GetDay(),recipe,unitid,lotid,glassid,lotid,glassid,unitid,procid,no);
		bRet = CopyFile_Df(imagemappath,m_imagemappath);
	}
	bRet = CopyFile_Df(sourcepath,imagepath);
	if(!bRet)
	{
		strlog.Format(_T("Error : copy faild!! : %s , %s"),sourcepath,imagepath);
	}

}

void CDialogDFServer::ClickBtnenhctrlDfConnect()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString ip;
	m_IPAddress_Server.GetWindowTextW(ip);
	if(m_btn_DFServerConnect.GetValue())
	{
		DFServerConnect(ip, m_edit_strID, m_edit_strPW, m_edit_strLocalDir, m_edit_strRemoteDir, 0);
		m_btn_DFServerConnect.SetWindowTextW(_T("Close"));
	}
	else
	{
		DFServerClose();
		m_btn_DFServerConnect.SetWindowTextW(_T("Connect"));
	}
}

void CDialogDFServer::ClickBtnenhctrlDfUpLoad()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString ip;
	m_IPAddress_Server.GetWindowTextW(ip);
	DFServerConnect(ip, m_edit_strID, m_edit_strPW, m_edit_strLocalDir, m_edit_strRemoteDir, 1);
}

void CDialogDFServer::ClickBtnenhctrlDfDownLoad()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString ip;
	m_IPAddress_Server.GetWindowTextW(ip);
	DFServerConnect(ip, m_edit_strID, m_edit_strPW, m_edit_strLocalDir, m_edit_strRemoteDir, 2);
}

void CDialogDFServer::ClickBtnenhctrlDfApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_bWriteFile = m_check_write_on_check.GetCheck();
}

void CDialogDFServer::ClickBtnenhctrlDfSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SaveDFServerPara(_T(IPS_PATH));
}

BOOL CDialogDFServer::LoadDFServerPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\DFServerCtrl.par");

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
		
		key.Empty();
		key.Format(_T("m_IPAddress_Server"));
		GetPrivateProfileString(_T("DFServer CTRL"), key, _T("127.0.0.1"), temp, sizeof(temp), file_path);
		dat = temp;
		m_IPAddress_Server.SetWindowTextW(dat);

		key.Empty();
		key.Format(_T("m_edit_strID"));
		GetPrivateProfileString(_T("DFServer CTRL"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		m_edit_strID.Format(_T("%s"), dat);

		key.Empty();
		key.Format(_T("m_edit_strPW"));
		GetPrivateProfileString(_T("DFServer CTRL"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		m_edit_strPW.Format(_T("%s"), dat);

		key.Empty();
		key.Format(_T("m_edit_strLocalDir"));
		GetPrivateProfileString(_T("DFServer CTRL"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		m_edit_strLocalDir.Format(_T("%s"), dat);

		key.Empty();
		key.Format(_T("m_edit_strRemoteDir"));
		GetPrivateProfileString(_T("DFServer CTRL"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		m_edit_strRemoteDir.Format(_T("%s"), dat);

		key.Empty();
		key.Format(_T("m_check_write_on_check"));
		GetPrivateProfileString(_T("DFServer CTRL"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		m_check_write_on_check.SetCheck(_wtoi(dat));
		m_bWriteFile = _wtoi(dat);

		key.Empty();
		key.Format(_T("m_check_up_load_file_del"));
		GetPrivateProfileString(_T("DFServer CTRL"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		m_check_up_load_file_del.SetCheck(_wtoi(dat));

		key.Empty();
		key.Format(_T("m_edit_nWriteTimeOut"));
		GetPrivateProfileString(_T("DFServer CTRL"), key, _T("0"), temp, sizeof(temp), file_path);
		dat = temp;
		m_edit_nWriteTimeOut = _wtoi(dat);

		
	}
	fd=0;
	
	return rslt;
}

BOOL CDialogDFServer::SaveDFServerPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	wchar_t temp[DAT_STR_LENG];
	char ctemp[DAT_STR_LENG];
	int i;
	CString file_path, pat_path, extention, master_path;
	Herror Herr;
	size_t getval = 0;
	
	file_path = path + _T("\\DFServerCtrl.par");

	//file 존재 확인 
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
		key.Format(_T("m_IPAddress_Server"));
		dat.Empty();
		m_IPAddress_Server.GetWindowTextW((LPTSTR)ctemp, 300);
		dat.Format(_T("%s"), ctemp);
		WritePrivateProfileString(_T("DFServer CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_strID"));
		dat.Empty();
		dat.Format(_T("%s"), m_edit_strID);
		WritePrivateProfileString(_T("DFServer CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_strPW"));
		dat.Empty();
		dat.Format(_T("%s"), m_edit_strPW);
		WritePrivateProfileString(_T("DFServer CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_strLocalDir"));
		dat.Empty();
		dat.Format(_T("%s"), m_edit_strLocalDir);
		WritePrivateProfileString(_T("DFServer CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_strRemoteDir"));
		dat.Empty();
		dat.Format(_T("%s"), m_edit_strRemoteDir);
		WritePrivateProfileString(_T("DFServer CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_check_write_on_check"));
		dat.Empty();
		dat.Format(_T("%d"), m_check_write_on_check.GetCheck());
		WritePrivateProfileString(_T("DFServer CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_check_up_load_file_del"));
		dat.Empty();
		dat.Format(_T("%d"), m_check_up_load_file_del.GetCheck());
		WritePrivateProfileString(_T("DFServer CTRL"), key, dat, file_path);

		key.Empty();
		key.Format(_T("m_edit_nWriteTimeOut"));
		dat.Empty();
		dat.Format(_T("%d"), m_edit_nWriteTimeOut);
		WritePrivateProfileString(_T("DFServer CTRL"), key, dat, file_path);

		
	}
	fd = 0;

	return rslt;
}

BOOL CDialogDFServer::DFServerConnect(CString IPAddress, CString ID, CString PW, CString LocalPath, CString RemotePath, int nUpDown)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString strLog;
	CString strCurDir;
	BOOL rslt = TRUE;
	m_FTPdata.m_cAddress = IPAddress;
	m_FTPdata.m_cName = ID;
	m_FTPdata.m_cPass = PW;

	if((nUpDown == 1) || (nUpDown == 2))
	{
		if(!m_cFPTCilent.m_bServerConnect)
		{
			strLog.Format(_T("Defect Server Connect Error!!"));
			pdlg->WriteLog(MAIN_LOG, strLog);
			return FALSE;
		}
	}

	CString temp, strDefectFilePath;
	AfxExtractSubString(temp, GetDFFilePath(), 8, '\\');
	strDefectFilePath.Format(_T("%s\\"), GetDFFilePath().Mid(10, LocalPath.GetLength() - 10));

	m_FTPdata.m_cLocalDir.Format(_T("%s"), LocalPath);
	m_FTPdata.m_cRemoteDir.Format(_T("%s\\%s"), GetDFServerRemotePath(), strDefectFilePath);

	m_FTPdata.m_cLocalImageDir.Format(_T("C:\\%s"), GetImgFilePath());
	m_FTPdata.m_cRemoteImageDir.Format(_T("%s\\%s"), GetImgFilePath());

	switch(nUpDown)
	{
	case DF_SERVER_CONNECT_TEST://create file
		if(!m_cFPTCilent.m_bServerConnect)
		{
			rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
		}
		else
		{
			m_cFPTCilent.Close();
			rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
		}

		m_btn_DFServerConnect.SetWindowTextW(_T("Close"));

		if(rslt)
		{
			strLog.Format(_T("Defect Server Connect!!"));
			
			m_cFPTCilent.m_pConnection->GetCurrentDirectory(strCurDir);
			m_cFPTCilent.m_pConnection->SetCurrentDirectory(GetDFServerRemotePath());
		}
		else
		{
			strLog.Format(_T("Defect Server Connect Error!!"));
		}
		break;
	case DF_SERVER_CONNECT_UP_LOAD://upload
		rslt = DFServerUpLoad(m_check_up_load_file_del.GetCheck());
		if(rslt)
		{
			strLog.Format(_T("Defect File Create : DFServerUpLoad(); return %d  End!"),rslt);
			DFWriteLog(strLog,DEFECT_SERVER_LOG);
			strLog.Format(_T("Defect Server Up Load Comp!!"));
			pdlg->WriteLog(MAIN_LOG, strLog);
		}
		else
		{
			strLog.Format(_T("Defect File Create : DFServerUpLoad(); return %d  End!"),rslt);
			DFWriteLog(strLog,DEFECT_SERVER_LOG);
			strLog.Format(_T("Defect Server Up Load Fail!!"));
			pdlg->WriteLog(MAIN_LOG, strLog);
		}
		rslt = RemoveDirectoryFile(_T("C:\\DEFECT_TEMP"));
		if(rslt)
		{
			strLog.Format(_T("Defect File Create : DEFECT_TEMP Directory Remove Successful!"));
			DFWriteLog(strLog,DEFECT_SERVER_LOG);
		}
		else
		{
			strLog.Format(_T("Defect File Create : DEFECT_TEMP Directory Remove fail!"));
			DFWriteLog(strLog,DEFECT_SERVER_LOG);
		}
		rslt = RemoveDirectoryFile(_T("C:\\DEFECT_FILE_TEMP"));
		if(rslt)
		{
			strLog.Format(_T("Defect File Create : DEFECT_FILE_TEMP Directory Remove Successful!"));
			DFWriteLog(strLog,DEFECT_SERVER_LOG);
		}
		else
		{
			strLog.Format(_T("Defect File Create : DEFECT_FILE_TEMP Directory Remove fail!"));
			DFWriteLog(strLog,DEFECT_SERVER_LOG);
		}
	
		break;
	case DF_SERVER_CONNECT_DOWN_LOAD://download
		rslt = DFServerDownLoad();
		if(rslt)
		{
			strLog.Format(_T("Defect Server Down Load Comp!!"));
		}
		else
		{
			strLog.Format(_T("Defect Server Down Load Fail!!"));
		}
		break;
	}

	pdlg->WriteLog(MAIN_LOG, strLog);
	
	return rslt;
}

BOOL CDialogDFServer::DFServerClose()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	BOOL rslt = TRUE;
	CString strLog;

	rslt = m_cFPTCilent.Close();
	strLog.Format(_T("Defect Server Close!!"));

	if(!rslt)
	{
		strLog.Format(_T("Defect Server Close Error!!"));
		rslt = FALSE;
	}

	pdlg->WriteLog(MAIN_LOG, strLog);

	m_btn_DFServerConnect.SetWindowTextW(_T("Connect"));

	return rslt;
}

BOOL CDialogDFServer::DFServerUpLoad(BOOL isUpLoadFileDel)
{
	BOOL rslt = TRUE;
	CString strLog,month,day,strlog;
	CTime time;
	time = CTime::GetCurrentTime();
	month.Format(_T("%02d"),time.GetMonth());
	day.Format(_T("%02d"),time.GetDay());

	strlog.Format(_T("Defect File Create : DFServerUpLoad(isUpLoadFileDel : %d); Start!"),isUpLoadFileDel);
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	if(!m_cFPTCilent.m_bServerConnect)
	{
		rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
	}
	else
	{
		m_cFPTCilent.Close();
		rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
	}
	//xml
	//rslt = m_cFPTCilent.UpLoadDir(m_FTPdata.m_cLocalDir,m_FTPdata.m_cRemoteDir,TRUE, isUpLoadFileDel);

	rslt = m_cFPTCilent.DefectFileUpload(month,day,m_header_e.recipe_id,m_header_e.unit_id,m_header_e.lot_id,m_defect_file_a.name,DEFECT_INSPECTOR);
	if(rslt)
	{
		strlog.Format(_T("Defect File Create : DefectFileUpload(); return %d End!"),rslt);
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	else
	{
		strlog.Format(_T("Defect File Create : DefectFileUpload(); return %d End!"),rslt);
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
		return FALSE;
	}
/*
	if(!m_cFPTCilent.m_bServerConnect)
	{
		rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
	}
	else
	{
		m_cFPTCilent.Close();
		rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
	}
	//xml
	//rslt = m_cFPTCilent.UpLoadDir(m_FTPdata.m_cLocalDir,m_FTPdata.m_cRemoteDir,TRUE, isUpLoadFileDel);

	rslt = m_cFPTCilent.DefectFileUpload(month,day,m_header_e.recipe_id,m_header_e.unit_id,m_header_e.lot_id,m_defect_file_a.name,DEFECT_HOST);
	if(rslt)
	{
		strlog.Format(_T("Defect File Create : DefectFileUpload(); return %d End!"),rslt);
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
		return TRUE;
	}
	else
	{
		strlog.Format(_T("Defect File Create : DefectFileUpload(); return %d End!"),rslt);
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
		return FALSE;
	}*/
	//image
	//rslt = m_cFPTCilent.UpLoadDir(m_FTPdata.m_cLocalImageDir,m_FTPdata.m_cRemoteImageDir,TRUE, isUpLoadFileDel);

}

BOOL CDialogDFServer::DFServerDownLoad()
{
	BOOL rslt = TRUE;
	CString strLog;

	rslt = m_cFPTCilent.DownLoadDir(m_FTPdata.m_cRemoteDir, m_FTPdata.m_cLocalDir);

	return rslt;
}

CString CDialogDFServer::GetDFServerIP()
{
	CString ip;
	m_IPAddress_Server.GetWindowTextW(ip); 
	return ip;
}

CString CDialogDFServer::GetDFServerID(){ return m_edit_strID; }
CString CDialogDFServer::GetDFServerPW(){ return m_edit_strPW; }
CString CDialogDFServer::GetDFServerLocalPath(){ return m_edit_strLocalDir; }
CString CDialogDFServer::GetDFServerRemotePath(){ return m_edit_strRemoteDir; }
CString CDialogDFServer::GetDFFilePath(){ return m_strDfFilePath; }
CString CDialogDFServer::GetImgFilePath(){ return m_strImgFilePath; }

void CDialogDFServer::WriteDefectInfo(int idx)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	typedfcount defectcount;
	CString temp,strlog;
	temp.Format(_T("%S"),m_DefectInfo[idx].cellid);
//	SetDefectInfo(m_DefectInfo[idx]);
	if(idx == 0)
	{
		strlog.Format(_T("SetDefectInfo() Start!"));
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	if(pdlg->m_bDfIdxFile)
	{
		SendMessage(USER_MSG_DEFECT_SERVER, XML_UPDATA_DATA, idx);
		/*m_Xml->Xml_Load_Update(GetBeforeDfFilePath(),pdlg->m_Glass->GetDfPanelidx(temp,DEFECT_FILE_UPDATE),m_Xml->SetDefectData(m_DefectInfo[idx].glsid,m_DefectInfo[idx].time,m_DefectInfo[idx].index,m_DefectInfo[idx].cellid,m_DefectInfo[idx].measure_x_e2,m_DefectInfo[idx].measure_y_e2,m_DefectInfo[idx].measure_x_ap2
			,m_DefectInfo[idx].measure_y_ap2,m_DefectInfo[idx].size_long,m_DefectInfo[idx].width,m_DefectInfo[idx].height,m_DefectInfo[idx].area,m_DefectInfo[idx].size_range,m_DefectInfo[idx].rank
			,m_DefectInfo[idx].in_out,m_DefectInfo[idx].gray_min,m_DefectInfo[idx].gray_25,m_DefectInfo[idx].gray_mediean,m_DefectInfo[idx].gray_75,m_DefectInfo[idx].gray_max,m_DefectInfo[idx].gray_mean
			,m_DefectInfo[idx].gray_deviation,m_DefectInfo[idx].black_white,m_DefectInfo[idx].image_path));*/
		if(idx + 1 == m_nDefectCnt)
		{
			m_Xml->m_pDoc2->save((_variant_t)m_cFPTCilent.m_temppath);
			//m_Xml->GetDoc()->save((_variant_t)m_Xml->GetFilePath());
			CopyFile_Df(m_cFPTCilent.m_temppath, m_Xml->GetFilePath());
			//m_cFPTCilent.m_temppath;
			m_Xml->m_pDoc2 = NULL;
			m_bWriteDefectFile = TRUE;
			m_bWriteComp = TRUE;
		}

		if(pdlg->GetDefectFileWriteBreak())
		{
			//m_Xml->GetDoc()->save((_variant_t)m_Xml->GetFilePath());

			m_bWriteDefectFile = TRUE;
			m_bWriteComp = TRUE;
		}
	}
	else
	{
		SetDefectInfo(m_DefectInfo[idx]);
		if(idx + 1 == m_nDefectCnt)
		{
			fclose(m_stream);
			strlog.Format(_T("SetDefectInfo() End!"));
			DFWriteLog(strlog,DEFECT_SERVER_LOG);
			strlog.Format(_T("defect server save start!"));
			DFWriteLog(strlog,DEFECT_SERVER_LOG);
			m_Xml->GetDoc()->save((_variant_t)m_Xml->GetFilePath());
			strlog.Format(_T("defect server save end!"));
			DFWriteLog(strlog,DEFECT_SERVER_LOG);

			m_bWriteDefectFile = TRUE;
			m_bWriteComp = TRUE;
		}

		if(pdlg->GetDefectFileWriteBreak())
		{
			strlog.Format(_T("defect server save start!"));
			DFWriteLog(strlog,DEFECT_SERVER_LOG);
			m_Xml->GetDoc()->save((_variant_t)m_Xml->GetFilePath());
			strlog.Format(_T("defect server save end!"));
			DFWriteLog(strlog,DEFECT_SERVER_LOG);

			m_bWriteDefectFile = TRUE;
			m_bWriteComp = TRUE;
		}
	}
}

//20160617 ngh
int CDialogDFServer::GetWriteTimeOut()
{
	return m_edit_nWriteTimeOut;
}

BOOL CDialogDFServer::GetWriteComp()
{
	return m_bWriteComp;
}

void CDialogDFServer::InitDefectCnt()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;

	for(int i = 0;i < pdlg->m_RecipePanel->GetCellCntY_coordXY();i++)
	{
		for(int j = 0;j < pdlg->m_RecipePanel->GetCellCntX_coordXY(); j++)
		{
			m_sizecnt[i][j].step_sb = 0;
			m_sizecnt[i][j].step_mb = 0;
			m_sizecnt[i][j].step_lb = 0;
			m_sizecnt[i][j].step_ob = 0;
			m_sizecnt[i][j].step_sw = 0;
			m_sizecnt[i][j].step_mw = 0;
			m_sizecnt[i][j].step_lw = 0;
			m_sizecnt[i][j].step_ow = 0;

			m_sizecnt[i][j].cum_sb = 0;
			m_sizecnt[i][j].cum_mb = 0;
			m_sizecnt[i][j].cum_lb = 0;
			m_sizecnt[i][j].cum_ob = 0;
			m_sizecnt[i][j].cum_sw = 0;
			m_sizecnt[i][j].cum_mw = 0;
			m_sizecnt[i][j].cum_lw = 0;
			m_sizecnt[i][j].cum_ow = 0;
		}
	}
}

int CDialogDFServer::GetDefectCnt()
{
	return m_nDefectCnt;
}

BOOL CDialogDFServer::GetDFWriteFile()
{
	return m_check_write_on_check.GetCheck();
}

BOOL CDialogDFServer::CreatDefectFile()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	BOOL bXml;
	int idx = 0;
	m_Xml = new CXml;
	CString strlog;
	strlog.Format(_T("Defect File Create : CreatDefectFile() Start!")); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	CString idxfilename,idxtemppath,ref_filepath,ref_data,ptnmnt_data,filepath,strMakeFilePath;
	BOOL bRet = TRUE;
	char strFile[512];
	char strTemp[512];
	long	result;
		
	CTime time;
	time = CTime::GetCurrentTime();

	char cIndexName[MAX_PATH],cHashPath[MAX_PATH];
	
		
	DfHeaderUpdate();
	strlog.Format(_T("Defect File Create : CreatDefectFile(); DfHeaderUpdate() End!")); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	DFBodyUpdate();
	strlog.Format(_T("Defect File Create : CreatDefectFile(); DFBodyUpdate() End!")); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	if(GlassIdxFile(m_header_e.gls_id,m_header_e.recipe_id,m_header_e.lot_id,m_header_e.proc_id,m_header_e.unit_id,m_defect_file_a.name))
	{
		strlog.Format(_T("Defect File Create : CreatDefectFile(); GlassIdxFile() return TRUE End!")); 
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	else
	{
		strlog.Format(_T("Defect File Create : CreatDefectFile(); GlassIdxFile() return FALSE End!")); 
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	sprintf(cIndexName, "%S", m_header_e.gls_id);
	
//	bRet = filetype;
	idxtemppath.Format(_T("C:\\IMAGE_IDX_TEMP"));
	idxfilename.Format(_T("%s_%s_%s.TXT"),m_header_e.lot_id,m_header_e.gls_id,m_header_e.recipe_id);
	m_ImageIdxFile.Format(_T("%s"),idxfilename);
	ref_data.Format(_T("C:\\IMAGE\\REF_IMAGE\\%02d\\%02d\\%s\\%s\\%s\\%s\\%s"),time.GetMonth(),time.GetDay(),m_header_e.recipe_id,m_header_e.unit_id,m_header_e.lot_id,m_header_e.gls_id,idxfilename);
	ptnmnt_data.Format(_T("C:\\IMAGE\\PTNMNT_IMAGE\\%02d\\%02d\\%s\\%s\\%s\\%s\\%s"),time.GetMonth(),time.GetDay(),m_header_e.recipe_id,m_header_e.unit_id,m_header_e.lot_id,m_header_e.gls_id,idxfilename);
	strMakeFilePath.Format(_T("%02d\\%02d\\%s\\%s\\%s\\%s"),time.GetMonth(),time.GetDay(),m_header_e.recipe_id,m_header_e.unit_id,m_header_e.lot_id,m_header_e.gls_id);
	
	_MakeDirectories(idxtemppath);
	_MakeDirectories(_T("C:\\IMAGE_FILE_TEMP"));
	_MakeDirectories(_T("C:\\IMAGE\\REF_IMAGE\\") + strMakeFilePath);
	_MakeDirectories(_T("C:\\IMAGE\\PTNMNT_IMAGE\\") + strMakeFilePath);
	
//	_MakeDirectories(_T("C:\\IMAGE\\DEFECT_IMAGE\\") + strMakeFilePath);
//	_MakeDirectories(_T("C:\\IMAGE\\DEFECT_IMAGE2\\") + strMakeFilePath);

	/*if(bRet)
	{
		filepath.Format(_T("%s"),ref_data);
	}
	else
	{
		filepath.Format(_T("%s"),ptnmnt_data);
	}*/

	filepath.Format(_T("%s\\%s"),idxtemppath,idxfilename);

	sprintf(strFile, "%S", filepath);

	
	if((m_stream = _fsopen(strFile, "a+", _SH_DENYNO)) == NULL)
	{
		HANDLE fd = CreateFile( (LPCWSTR)(LPCSTR)strFile,
							GENERIC_READ|GENERIC_WRITE,
							//GENERIC_WRITE,
							FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);

		
		m_stream = _fsopen(strFile, "a+", _SH_DENYNO);
		if(m_stream == NULL)
		{
			CloseHandle(fd);
			return FALSE;
		}

		CloseHandle(fd);
	}

	CString imagedata;
	imagedata.Format(_T("LOT_INFO=%s,%s,%s,%s"),m_header_e.lot_id,m_header_e.unit_id,m_header_e.recipe_id,m_header_e.proc_id);
	ImageIdxFile(m_stream, m_header_e.gls_id,m_header_e.recipe_id,m_header_e.lot_id,m_header_e.proc_id,m_header_e.unit_id,m_defect_file_a.name,TRUE,imagedata);
	strlog.Format(_T("Defect File Create : CreatDefectFile(); ImageIdxFile() End!")); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	imagedata.Format(_T("GLS_INFO=%s,%s"),m_header_e.gls_id,m_header_a.insp_time_start);
	ImageIdxFile(m_stream, m_header_e.gls_id,m_header_e.recipe_id,m_header_e.lot_id,m_header_e.proc_id,m_header_e.unit_id,m_defect_file_a.name,TRUE,imagedata);
	strlog.Format(_T("Defect File Create : CreatDefectFile(); ImageIdxFile() End!")); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	bXml = CreateXmlfile(m_strDfFilePath, 1);
	if(bXml)
	{
		strlog.Format(_T("Defect File Create : CreatDefectFile(); CreateXmlfile() return TRUE End!")); 
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	else
	{
		strlog.Format(_T("Defect File Create : CreatDefectFile(); CreateXmlfile() return FALSE End!")); 
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	WriteXmlHeader(TRUE);
	strlog.Format(_T("Defect File Create : CreatDefectFile(); WriteXmlHeader() End!")); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);
	WriteXmlBody(bXml);
	strlog.Format(_T("Defect File Create : CreatDefectFile(); WriteXmlBody() End!")); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);

	//m_panel_e.main_sub_info.Format(_T("%s"),P_H_Main_sub_info());	
//	m_Xml->m_main_sub.Format(_T("%s"),m_panel_e.main_sub_info);
	for(int i = 0;i < pdlg->m_RecipePanel->GetCellCntY_coordXY();i++)
	{
		for(int j = 0;j < pdlg->m_RecipePanel->GetCellCntX_coordXY(); j++)
		{
			m_panel_e.panel_id.Format(_T("%s%s"),m_header_e.gls_id,pdlg->m_Glass->m_strCell_table[i][j]);

			//typepanel_a			m_panel_a;

			//memset(&m_panel_a, NULL,sizeof(typepanel_a));
			Init_typepanel_a();
			m_Xml->Xml_Format_Panel(idx);
			WriteXmlPanel(idx);
			
			pdlg->m_Glass->m_DfCount[i][j].count = 0;
			
			idx++;
		}
	}

	
	return TRUE;
}

void CDialogDFServer::Init_typepanel_a()
{
	m_panel_a.latest_judge.Format(_T(""));
	m_panel_a.fdi_judge.Format(_T(""));
	m_panel_a.frt_judge.Format(_T(""));
	m_panel_a.ptl_judge.Format(_T(""));
	m_panel_a.gcl_judge.Format(_T(""));
	m_panel_a.latest_reason.Format(_T(""));
	m_panel_a.fdi_reason.Format(_T(""));
	m_panel_a.frt_reason.Format(_T(""));
	m_panel_a.ptl_reason.Format(_T(""));
	m_panel_a.gcl_reason.Format(_T(""));
	m_panel_a.cum.Format(_T("0"));
	m_panel_a.step.Format(_T("0"));
	m_panel_a.step_s_rb.Format(_T("0"));
	m_panel_a.step_m_rb.Format(_T("0"));
	m_panel_a.step_l_rb.Format(_T("0"));
	m_panel_a.step_o_rb.Format(_T("0"));
	m_panel_a.cum_s_rb.Format(_T("0"));
	m_panel_a.cum_m_rb.Format(_T("0"));
	m_panel_a.cum_l_rb.Format(_T("0"));
	m_panel_a.cum_o_rb.Format(_T("0"));
	m_panel_a.step_s_rw.Format(_T("0"));
	m_panel_a.step_m_rw.Format(_T("0"));
	m_panel_a.step_l_rw.Format(_T("0"));
	m_panel_a.step_o_rw.Format(_T("0"));
	m_panel_a.cum_s_rw.Format(_T("0"));
	m_panel_a.cum_m_rw.Format(_T("0"));
	m_panel_a.cum_l_rw.Format(_T("0"));
	m_panel_a.cum_o_rw.Format(_T("0"));
	m_panel_a.shop.Format(_T(""));
	m_panel_a.def_no.Format(_T(""));
	m_panel_a.gls_start_time.Format(_T(""));
	m_panel_a.point_x.Format(_T(""));
	m_panel_a.point_y.Format(_T(""));
	m_panel_a.origin_point_x.Format(_T(""));
	m_panel_a.origin_point_y.Format(_T(""));
	m_panel_a.point_g.Format(_T(""));
	m_panel_a.point_d.Format(_T(""));
	m_panel_a.active_area.Format(_T(""));
	m_panel_a.xpixel.Format(_T(""));
	m_panel_a.ypixel.Format(_T(""));
	m_panel_a.pixel.Format(_T(""));
	m_panel_a.xsize.Format(_T(""));
	m_panel_a.ysize.Format(_T(""));
	m_panel_a.size.Format(_T(""));
	m_panel_a.rank.Format(_T(""));
	m_panel_a.mode.Format(_T(""));
	m_panel_a.bright.Format(_T(""));
	m_panel_a.proc_id.Format(_T(""));
	m_panel_a.unit_id.Format(_T(""));
	m_panel_a.ptn_code.Format(_T(""));
	m_panel_a.operator_id.Format(_T(""));
	m_panel_a.ptn_code_df.Format(_T(""));
	m_panel_a.judge_final.Format(_T(""));
	m_panel_a.auto_juge_check.Format(_T(""));
	m_panel_a.review_x_size.Format(_T(""));
	m_panel_a.review_y_size.Format(_T(""));
	m_panel_a.review_layer.Format(_T(""));
	m_panel_a.pxl_zone.Format(_T(""));
	m_panel_a.encap_type.Format(_T(""));
	m_panel_a.zone.Format(_T(""));
	m_panel_a.size_h.Format(_T(""));
	m_panel_a.rpc_auto_capture.Format(_T(""));
	m_panel_a.judge_ei_rpc.Format(_T(""));
	m_panel_a.reason_ei_rpc.Format(_T(""));
	m_panel_a.ei_rpc_op.Format(_T(""));
	m_panel_a.image_file_no.Format(_T(""));
	m_panel_a.seq.Format(_T(""));
	m_panel_a.name.Format(_T(""));
	m_panel_a.path.Format(_T(""));
}

LRESULT CDialogDFServer::OnDefectMessage(WPARAM para0, LPARAM para1)
{
	int i, j;
	CString strlog;
	BOOL rslt = FALSE;
	int idx = (int)para1;
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	typedfcount defectcount;
	CString temp;
	HRESULT hr = S_FALSE;

	Getlotintime(pdlg->m_Inline->GetLotInTime()); //20160725 sc
	
	strlog.Format(_T("OnDefectMessage() Start!")); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);
	char message[500];

	switch(para0)
	{
	case CREATE_DEFECT_XML_FILE:
		m_bWriteDefectFile = FALSE;
		//WriteDefectFile();
		CreatDefectFile();
		strlog.Format(_T("Defect File Create : CreatDefectFile() End!")); 
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
		
		break;
	case XML_FILE_TRANSFORM:
		m_Xml->Transform((_bstr_t)m_Xml->GetFilePath(),(_bstr_t)FileNameChange(m_defect_file_a.name));// (_bstr_t)m_Xml->GetFilePath());
		DeleteFile(m_Xml->GetFilePath());
		if(!m_cFPTCilent.m_bServerConnect)
		{
			rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
		}
		else
		{
			m_cFPTCilent.Close();
			rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
		}
		m_cFPTCilent.DefectImageIdxUpload(m_timedata.month,m_timedata.day,m_header_e.recipe_id,m_header_e.unit_id,m_header_e.lot_id,m_header_e.gls_id,GetImageIdxFile());
		ImageUpload();
		delete m_Xml;		
		break;
	case XML_FILE_UPDATE:
		//m_Xml->Xml_Load_Update(GetBeforeDfFilePath());//m_Xml->Create_Xml_File(_T("A"),0,DEFECT_FILE_UPDATE,temp);//"C:\\DEFECT_FILE_TEMP\\111111A_0_   0 0 0_ 0 0 0.XML"));
		m_Xml->Update_Defect_CountInit(GetBeforeDfFilePath());
		break;
	case XML_UPDATA_DATA:
		temp.Format(_T("%S"),m_DefectInfo[idx].cellid);

		if(idx == 0)
		{
			
			hr=m_Xml->m_pDoc2.CreateInstance(__uuidof(DOMDocument)); // 문서불러오기
			m_Xml->m_pDoc2->put_async(VARIANT_FALSE);
			m_Xml->m_pDoc2->load((_variant_t)GetBeforeDfFilePath());
			m_Xml->GlassInfoUpdate(m_defect_file_a.name,m_header_e.key_id,m_header_e.lot_id,m_header_e.cst_id,m_header_e.mo_gls_id_e2,m_header_e.half_gls_id_e2,m_header_e.cf_id,m_header_e.gls_cut_e2,m_header_e.slot_no,
				m_header_e.eqp_id,m_header_e.machine_id,m_header_e.unit_id,m_header_e.oper_id,m_header_e.proc_id,m_header_e.recipe_id,m_header_a.insp_time_start,m_header_a.insp_time_end);
			
		}

		if(m_DefectInfo[idx].cellid == "MT")
		{
			int a;
			a = 9;
		}
//CString defectfilename,CString keyid,CString lotid,CString cstid,CString mo_gls_id,CString half_gls_id,CString cf_id,CString cut,CString slot,CString eqp,CString machine,CString unit,CString oper,CString proc,CString recipe,CString ins_time)	
		m_Xml->Xml_Load_Update(GetBeforeDfFilePath(),pdlg->m_Glass->GetDfPanelidx(temp,DEFECT_FILE_UPDATE),m_Xml->SetDefectData(m_DefectInfo[idx].glsid,m_DefectInfo[idx].time,m_DefectInfo[idx].index,m_DefectInfo[idx].cellid,m_DefectInfo[idx].measure_x_e2,m_DefectInfo[idx].measure_y_e2,m_DefectInfo[idx].measure_x_ap2
			,m_DefectInfo[idx].measure_y_ap2,m_DefectInfo[idx].size_long,m_DefectInfo[idx].width,m_DefectInfo[idx].height,m_DefectInfo[idx].area,m_DefectInfo[idx].size_range,m_DefectInfo[idx].rank
			,m_DefectInfo[idx].in_out,m_DefectInfo[idx].gray_min,m_DefectInfo[idx].gray_25,m_DefectInfo[idx].gray_mediean,m_DefectInfo[idx].gray_75,m_DefectInfo[idx].gray_max,m_DefectInfo[idx].gray_mean
			,m_DefectInfo[idx].gray_deviation,m_DefectInfo[idx].black_white,m_DefectInfo[idx].image_path));
		break;
	case IDX_FILE_UPDATE:
		IDXUpdate();
		break;
	default:
		AfxMessageBox(_T("Error : invalid index!(CDialogDFServer::OnDefectMessage)"));
		break;
	}
	strlog.Format(_T("OnDefectMessage() End!")); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);
	return 0;
}

BOOL CDialogDFServer::IDXUpdate()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString ip,hashtemp1,hashtemp2;
	m_IPAddress_Server.GetWindowTextW(ip);
	BOOL rslt = TRUE;
	char strFile[512];
	char strTemp[512];
	long	result;
	FILE *fp, *stream;
	int iret = 0;

	CString glassid,strlog,data;
	m_FTPdata.m_cAddress = ip;
	m_FTPdata.m_cName =  m_edit_strID;
	m_FTPdata.m_cPass = m_edit_strPW;
	CString str,rstr,filepath;

	CTime time;
	time = CTime::GetCurrentTime();
	//rslt = DFServerConnect(ip, m_edit_strID, m_edit_strPW, m_edit_strLocalDir, m_edit_strRemoteDir, 0);
	if(!m_cFPTCilent.m_bServerConnect)
	{
		rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
	}
	else
	{
		m_cFPTCilent.Close();
		rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
	}

	glassid.Format(_T("%S"),pdlg->m_Inline->GetLoaderGlassData().strGlassID);
	hashtemp1 = GetGlassIDHash(glassid,0);
	hashtemp2 = GetGlassIDHash(glassid,1);

	//m_header_e.gls_id,m_header_e.recipe_id,m_header_e.lot_id,m_header_e.proc_id,m_header_e.unit_id,m_defect_file_a.name
	filepath.Format(_T("C:\\DEFECT_TEMP\\%s.IDX"),glassid);
	sprintf(strFile,"%S",filepath);
	data.Format(_T("C:\\DEFECT\\INSPECTOR\\%02d\\%02d\\%s\\%s\\%s\\%s"),time.GetMonth(),time.GetDay(),m_header_e.recipe_id,m_header_e.unit_id,m_header_e.lot_id,m_defect_file_a.name);
	
	//rslt = m_cFPTCilent.m_pConnection->SetCurrentDirectory(rstr);
	//m_cFPTCilent.m_pConnection->GetCurrentDirectory(str);
	//m_cFPTCilent.m_pConnection->GetCurrentDirectoryAsURL(str);
	//m_cFPTCilent.m_pConnection->GetCurrentDirectoryW(str);
	//m_Xml->Defect_Update_New(20,_T("m"));

	//CString strLog;
	if((stream = _fsopen(strFile, "a+", _SH_DENYNO)) == NULL)
	{
		HANDLE fd = CreateFile( (LPCWSTR)(LPCSTR)strFile,
							GENERIC_READ|GENERIC_WRITE,
							//GENERIC_WRITE,
							FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);

		stream = _fsopen(strFile, "a+", _SH_DENYNO);
		result = fseek(stream, 0L, SEEK_END);

		if(stream == NULL)
		{
			CloseHandle(fd);
			return FALSE;
		}

		CloseHandle(fd);
	}
	result = fseek(stream, 0L, SEEK_END);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}

	sprintf_s(strTemp, data.GetLength() + 1, "%S", data);
    iret = fputs(strTemp, stream);
	iret = fputs("\n", stream);
	fclose(stream);

	//xml
	rslt = m_cFPTCilent.IDXFileUpdate(hashtemp1,hashtemp2,filepath,glassid);//,glassid);

	//RemoveDirectoryFile(_T("C:\\DEFECT_TEMP"));
	//RemoveDirectoryFile(_T("C:\\DEFECT_FILE_TEMP"));

	return TRUE;

}


void CDialogDFServer::ImageUpload()
{
	int count = 0;
	int i;
	CString strlog;

	count = _wtoi(m_dfinfo.index);
	strlog.Format(_T("image Upload Start!"));
	DFWriteLog(strlog,DEFECT_SERVER_LOG);
	for(i = 0;i < count;i++)
	{
		if(i == 0)
		{
			m_cFPTCilent.DefectImageUpload(REF_IMAGE,m_timedata.month,m_timedata.day,m_header_e.recipe_id,m_header_e.unit_id,m_header_e.lot_id,m_imagemappath);
		}
		m_cFPTCilent.DefectImageUpload(REF_IMAGE,m_timedata.month,m_timedata.day,m_header_e.recipe_id,m_header_e.unit_id,m_header_e.lot_id,GetImageFilePath(i));
	}
	strlog.Format(_T("image Upload End!"));
	DFWriteLog(strlog,DEFECT_SERVER_LOG);
}

BOOL CDialogDFServer::GetServerConnectStatus()
{
	return m_cFPTCilent.m_bServerConnect;
}

CString CDialogDFServer::FileNameChange(CString filename)
{
	CString temp,month,day;
	CTime time;
	time = CTime::GetCurrentTime();
	month.Format(_T("%02d"),time.GetMonth());
	day.Format(_T("%02d"),time.GetDay());
	temp.Format(_T("C:\\DEFECT\\INSPECTOR\\%s\\%s\\%s\\%s\\%s\\%s"),month,day,m_header_e.recipe_id,m_header_e.unit_id,m_header_e.lot_id,filename);
	return temp;
}
void CDialogDFServer::BeforeProcessDFload(CString dffilename)
{
	CString filepath,filename,temp;
	filepath.Format(_T("C:\\DEFECT\\"));
	AfxExtractSubString(temp,dffilename,0,'.');
	filename.Format(_T("%s%s.xml"),filepath,temp);

	//m_Xml->Xml_Load(filename);
}

void CDialogDFServer::BeforeProcessDFupdate()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	
}


void CDialogDFServer::ClickBtnenhctrlDfFind()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString ip,hashtemp1,hashtemp2;
	m_IPAddress_Server.GetWindowTextW(ip);
	BOOL rslt = TRUE;

	CString glassid;
	m_FTPdata.m_cAddress = ip;
	m_FTPdata.m_cName =  m_edit_strID;
	m_FTPdata.m_cPass = m_edit_strPW;
	//rslt = DFServerConnect(ip, m_edit_strID, m_edit_strPW, m_edit_strLocalDir, m_edit_strRemoteDir, 0);
	rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
	
	CString str,rstr;
	
	//rstr.Format(_T("\\\\127.0.0.1\\Working\\DEFECT_TEST"));//D:\Working\DEFECT_TEST
	//rstr.Format(_T("00000011"));
	glassid.Format(_T("%S"),pdlg->m_Inline->GetLoaderGlassData().strGlassID);
	hashtemp1 = GetGlassIDHash(glassid,0);
	hashtemp2 = GetGlassIDHash(glassid,1);
	//rslt = m_cFPTCilent.m_pConnection->SetCurrentDirectory(rstr);
	//m_cFPTCilent.m_pConnection->GetCurrentDirectory(str);
	//m_cFPTCilent.m_pConnection->GetCurrentDirectoryAsURL(str);
	//m_cFPTCilent.m_pConnection->GetCurrentDirectoryW(str);
	//m_Xml->Defect_Update_New(20,_T("m"));

	//CString strLog;

	//xml
	rslt = m_cFPTCilent.IDXFileFind(hashtemp1,hashtemp2,rstr,glassid);//,glassid);
	if(rslt)
	{
		ReadIDXFile(m_cFPTCilent.Getidxtemp());
	}
	//m_Xml->Xml_Load_Update(GetBeforeDfFilePath());
	//rslt = m_cFPTCilent.DownLoadDir(rstr,_T("C:\\DEFECT\\INSPECTOR\\05\\25\\RRRRRRR\\AAAAAAA"));
}

BOOL CDialogDFServer::DefectFileUpdate()
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	CString glassid,strlog;
	strlog.Format(_T("IDX file search : DefectFileUpdate(); start!")); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG); 
	CString ip,hashtemp1,hashtemp2;
	m_IPAddress_Server.GetWindowTextW(ip);
	BOOL rslt = TRUE;

	
	
	m_FTPdata.m_cAddress = ip;
	m_FTPdata.m_cName =  m_edit_strID;
	m_FTPdata.m_cPass = m_edit_strPW;
	//rslt = DFServerConnect(ip, m_edit_strID, m_edit_strPW, m_edit_strLocalDir, m_edit_strRemoteDir, 0);
	if(!m_cFPTCilent.m_bServerConnect)
	{
		rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
		strlog.Format(_T("IDX file search : DefectFileUpdate() ;m_cFPTCilent.Open(ip:%s,id:%s,pass:%s,port:%d);"),m_FTPdata.m_cAddress,m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT); 
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	else
	{
		m_cFPTCilent.Close();
		strlog.Format(_T("IDX file search : DefectFileUpdate(); m_cFPTCilent.Close()")); 
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
		rslt = m_cFPTCilent.Open(m_FTPdata.m_cAddress, m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT);
		strlog.Format(_T("IDX file search : DefectFileUpdate(); m_cFPTCilent.Open(ip:%s,id:%s,pass:%s,port:%d);"),m_FTPdata.m_cAddress,m_FTPdata.m_cName, m_FTPdata.m_cPass, INTERNET_DEFAULT_FTP_PORT); 
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	
	CString str,rstr;
	
	//rstr.Format(_T("\\\\127.0.0.1\\Working\\DEFECT_TEST"));//D:\Working\DEFECT_TEST
	//rstr.Format(_T("00000011"));
	glassid.Format(_T("%S"),pdlg->m_Inline->GetLoaderGlassData().strGlassID);
	strlog.Format(_T("IDX file search : DefectFileUpdate(); glass_id : %s search"),glassid); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);
	hashtemp1 = GetGlassIDHash(glassid,0);
	strlog.Format(_T("IDX file search : DefectFileUpdate(); glass_id : %s , hash1 : %s"),glassid,hashtemp1); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);
	hashtemp2 = GetGlassIDHash(glassid,1);
	strlog.Format(_T("IDX file search : DefectFileUpdate(); glass_id : %s , hash2 : %s"),glassid,hashtemp2); 
	DFWriteLog(strlog,DEFECT_SERVER_LOG);
	//rslt = m_cFPTCilent.m_pConnection->SetCurrentDirectory(rstr);
	//m_cFPTCilent.m_pConnection->GetCurrentDirectory(str);
	//m_cFPTCilent.m_pConnection->GetCurrentDirectoryAsURL(str);
	//m_cFPTCilent.m_pConnection->GetCurrentDirectoryW(str);

	//CString strLog;
	//xml
	rslt = m_cFPTCilent.IDXFileFind(hashtemp1,hashtemp2,rstr,glassid);//,glassid);
	strlog.Format(_T("IDX file search : IDXFileFind(hash1 : %s,hash2 : %s,idxpath : %s,glassid : %s); Return %d End!"),hashtemp1,hashtemp2,rstr,glassid,rslt);
	DFWriteLog(strlog,DEFECT_SERVER_LOG);
	if(rslt)
	{
		strlog.Format(_T("IDX file search : DefectFileUpdate();  %s IDX file search Successful!"),glassid);
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
		if(ReadIDXFile(m_cFPTCilent.Getidxtemp()))
		{
			strlog.Format(_T("IDX file search : DefectFileUpdate(); return TRUE End!"),glassid);
			DFWriteLog(strlog,DEFECT_SERVER_LOG);
			return TRUE;
		}
		else
		{
			strlog.Format(_T("IDX file search : DefectFileUpdate(); return FALSE End!"),glassid);
			DFWriteLog(strlog,DEFECT_SERVER_LOG);
			return FALSE;
		}		
	}
	else
	{
		strlog.Format(_T("IDX file search : DefectFileUpdate(); return FALSE End!"),glassid);
		DFWriteLog(strlog,DEFECT_SERVER_LOG);
		return FALSE;
	}
}

BOOL CDialogDFServer::DFWriteLog(CString text,int log_type)
{
	FILE*	stream;
	char	strFile[MAX_PATH], strToFile[MAX_PATH];
    long	result, line;
	char	szTime[DAT_STR_LENG]; 
	CString bak_file;
	int limit_line = 500000;
	CString path;
	CString strTime;
	CString log,strLogType;
	char buffer[DAT_STR_LENG];

	

	switch(log_type)
	{
	case DEFECT_SERVER_LOG:
		strLogType = DEFECT_SERVER_LOG_FILE_NAME;
		break;
	}

	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);
	strTime.Format(_T("%04d%02d%02d"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
	log.Format(_T("%04d-%02d-%02d-%02d:%02d:%02d:%03d->%s"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay, lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond, lpSystemTime.wMilliseconds, text);

	sprintf(strFile, "D:\\DATABASE\\LOG\\%S\\%S_%S", strTime, strTime, strLogType);

	_MakeDirectories(_T("D:\\DATABASE\\LOG\\") + strTime);

	if((stream = _fsopen(strFile, "a+", _SH_DENYNO)) == NULL)
	{
		HANDLE fd = CreateFile( (LPCWSTR)(LPCSTR)strFile,
							GENERIC_READ|GENERIC_WRITE,
							//GENERIC_WRITE,
							FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);

		stream = _fsopen(strFile, "a+", _SH_DENYNO);
		if(stream == NULL)
		{
			CloseHandle(fd);
			return FALSE;
		}

		CloseHandle(fd);
	}

	//Check first time
    result = fseek(stream, 0L, SEEK_SET);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}
	line = 0;
	while(fgets(buffer, DAT_STR_LENG, stream)) 
	{
		line++;
		if(1 < line)
		{
			break;
		}
	}

    result = fseek(stream, 0L, SEEK_END);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}

	memset(szTime, NULL, sizeof(szTime));
	sprintf(szTime,"%S",log);// log.GetLength() + 1, "%S", log);
    fputs(szTime, stream);
	fputs("\n", stream);

    result = fseek(stream, 0L, SEEK_SET);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}

	//Check limit line
	line = 0;
	if(limit_line)
	{
		while(fgets(buffer, DAT_STR_LENG, stream)) line++;
		if(line > limit_line)
		{
			sprintf_s(strToFile, path.GetLength() + bak_file.GetLength() + 2, "%S\\%S", path, bak_file); 
			CopyFile((LPCWSTR)(LPCSTR)strFile, (LPCWSTR)(LPCSTR)strToFile, FALSE);
			fclose(stream);
			DeleteFile((LPCWSTR)(LPCSTR)strFile);
		}
		else
		{
			fclose(stream);
		}
	}
	else
	{
		fclose(stream);
	}
	return TRUE;
}

void CDialogDFServer::Getlotintime(CString timedata)
{
	m_lotintime.year.Format(_T("%s"),timedata.Left(4));
	m_lotintime.month.Format(_T("%s"),timedata.Mid(4,2));
	m_lotintime.day.Format(_T("%s"),timedata.Mid(6,2));
	m_lotintime.hour.Format(_T("%s"),timedata.Mid(8,2));
	m_lotintime.min.Format(_T("%s"),timedata.Mid(10,2));
	m_lotintime.second.Format(_T("%s"),timedata.Right(2));
}



