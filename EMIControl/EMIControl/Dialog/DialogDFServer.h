#pragma once
#include "Include\Hash.h"
#include "FTP\FPTClient.h"
#include "xml\Xml.h"
#include "afxwin.h"
#include "btnenhctrl.h"

#define ELEMENT	0
#define ATTRIBUTE_IN	1
#define	ATTRIBUTE_G		2
#define	ATTRIBUTE_N		3
#define	ATTRIBUTE_R		4
#define ATTRIBUTE_CUM	1
#define	ATTRIBUTE_STEP	2
#define	ATTRIBUTE_NEW	3
#define ATTRIBUTE_RB	1
#define ATTRIBUTE_RW	2
#define	DF_SMALL		0
#define	DF_MEDIUM		1
#define	DF_LARGE		2
#define	DF_OVERFLOW		3
#define	DF_TOTAL		4

#define DEFECT_MAX_COUNT 30000

#define E_HEADER	_T("HEADER")
#define	E_BODY		_T("BODY")
#define E_PANEL		_T("PANEL")


//20160428 ngh
#define USER_MSG_DEFECT_SERVER						WM_USER+30

#define DF_SERVER_CONNECT_TEST			0
#define DF_SERVER_CONNECT_UP_LOAD		1
#define DF_SERVER_CONNECT_DOWN_LOAD		2
#define DF_MAIN_PATH					_T("C:\\DEFECT\\INSPECTOR\\")



#define CREATE_DEFECT_XML_FILE				0
#define XML_FILE_TRANSFORM					1
#define	XML_FILE_UPDATE						2
#define XML_UPDATA_DATA						3
#define	IDX_FILE_UPDATE						4

#define DEFECT_FILE_CREATE			0
#define DEFECT_FILE_UPDATE			1

#define DEFECT_HOST					0
#define DEFECT_INSPECTOR			1

// CDialogDFServer 대화 상자입니다.
typedef struct defect_file_a
{
	CString name;
}typedefect_file_a;
typedef struct header_e
{
	CString key_id;
	CString	lot_id;
	CString	cst_id;
	CString	gls_id;
	CString	mo_gls_id_e2;
	CString	mo_gls_id_m1;
	CString	half_gls_id_e2;
	CString	half_gls_id_m1;
	CString	cf_id;
	CString	gls_cut_e2;
	CString	gls_cut_m1;
	CString	slot_no;
	CString	eqp_id;
	CString	machine_id;
	CString	unit_id;
	CString	oper_id;
	CString	proc_id;
	CString	recipe_id;
	CString	insp_time;
}typeheader_e;
typedef struct header_a
{
	CString insp_time_start;
	CString insp_time_end;
}typeheader_a;

typedef struct body_e
{
	CString	panel_qty;
	CString	main;
	CString	sub;
	CString	judge_info;
	CString	latest;
	CString	fdi;
	CString	frt;
	CString	ptl;
	CString	gcl;
	CString	defect_no;
	CString	total;
	CString	step_s;
	CString	step_m;
	CString step_l;
	CString step_o;
	CString cum_s;
	CString cum_m;
	CString cum_l;
	CString cum_o;
	CString image_file;
	CString	ref;
	CString ptn;
}typebody_e;

typedef struct body_a
{
	CString	main_IN;
	CString	main_G;
	CString	main_N;
	CString	main_R;
	CString sub_IN;
	CString sub_G;
	CString sub_N;
	CString sub_R;
	CString latest_judge;
	CString	fdi_judge;
	CString frt_judge;
	CString ptl_judge;
	CString gcl_judge;
	CString latest_reason;
	CString	fdi_reason;
	CString frt_reason;
	CString ptl_reason;
	CString gcl_reason;
	CString	cum;
	CString	step;
	CString	new_;
	CString	step_s_rb;
	CString	step_m_rb;
	CString step_l_rb;
	CString step_o_rb;
	CString cum_s_rb;
	CString cum_m_rb;
	CString cum_l_rb;
	CString cum_o_rb;
	CString	step_s_rw;
	CString	step_m_rw;
	CString step_l_rw;
	CString step_o_rw;
	CString cum_s_rw;
	CString cum_m_rw;
	CString cum_l_rw;
	CString cum_o_rw;
}typebody_a;


typedef struct panel_e
{
	CString	key_id;
	CString	panel_id;
	CString	main_sub_info;
	CString	judge;
	CString	judge_info;
	CString	latest;
	CString	fdi;
	CString	frt;
	CString	ptl;
	CString	gcl;
	CString	defect_no;
	CString	total;
	CString	step_s;
	CString	step_m;
	CString step_l;
	CString step_o;
	CString cum_s;
	CString cum_m;
	CString cum_l;
	CString cum_o;
	CString defect;
	CString img;

}typepanel_e;


typedef struct panel_a
{
	CString latest_judge;
	CString	fdi_judge;
	CString frt_judge;
	CString ptl_judge;
	CString gcl_judge;
	CString latest_reason;
	CString	fdi_reason;
	CString frt_reason;
	CString ptl_reason;
	CString gcl_reason;
	CString	cum;
	CString	step;
	CString	step_s_rb;
	CString	step_m_rb;
	CString step_l_rb;
	CString step_o_rb;
	CString cum_s_rb;
	CString cum_m_rb;
	CString cum_l_rb;
	CString cum_o_rb;
	CString	step_s_rw;
	CString	step_m_rw;
	CString step_l_rw;
	CString step_o_rw;
	CString cum_s_rw;
	CString cum_m_rw;
	CString cum_l_rw;
	CString cum_o_rw;
	CString shop;
	CString def_no;
	CString gls_start_time;
	CString point_x;
	CString point_y;
	CString origin_point_x;
	CString origin_point_y;
	CString point_g;
	CString point_d;
	CString active_area;
	CString xpixel;
	CString ypixel;
	CString pixel;
	CString xsize;
	CString ysize;
	CString size;
	CString rank;
	CString mode;
	CString bright;
	CString proc_id;
	CString unit_id;
	CString ptn_code;
	CString operator_id;
	CString ptn_code_df;
	CString judge_final;
	CString auto_juge_check;
	CString review_x_size;
	CString review_y_size;
	CString review_layer;
	CString pxl_zone;
	CString encap_type;
	CString zone;
	CString size_h;
	CString rpc_auto_capture;
	CString judge_ei_rpc;
	CString reason_ei_rpc;
	CString ei_rpc_op;
	CString image_file_no;
	CString	seq;
	CString	name;
	CString	path;

}typepanel_a;

typedef struct defectinfo
{
	CString glsid;
	CString time;
	CString index;
	CString cellid;
	double measure_x_e2;
	double measure_y_e2;
	double measure_x_ap2;
	double measure_y_ap2;
	double size_long;
	double width;
	double height;
	double area;
	CString size_range;
	CString rank;
	CString in_out;
	double gray_min;
	double gray_25;
	double gray_mediean;
	double gray_75;
	double gray_max;
	double gray_mean;
	double gray_deviation;
	CString black_white;
	CString image_path;
	int width_pix;  //20160728 sc
	int height_pix; //20160728 sc
	int area_pix; //20160728 sc
	int bright; 

}typedefectinfo;

typedef struct DefectInfo
{
	char glsid[16];
	char time[30];
	char index[6];
	char cellid[4];
	double measure_x_e2;
	double measure_y_e2;
	double measure_x_ap2;
	double measure_y_ap2;
	double size_long;
	double width;
	double height;
	double area;
	char size_range[30];
	char rank[10];
	char in_out[4];
	double gray_min;
	double gray_25;
	double gray_mediean;
	double gray_75;
	double gray_max;
	double gray_mean;
	double gray_deviation;
	char black_white[4];
	char image_path[256];
	int width_pix; //20160728 sc
	int height_pix; //20160728 sc
	int area_pix; //20160728 sc


}typeDefectInfo;

typedef struct sizecnt
{
	int step_sw;
	int step_mw;
	int	step_lw;
	int	step_ow;
	int	step_sb;
	int	step_mb;
	int	step_lb;
	int	step_ob;
	int cum_sw;
	int cum_mw;
	int	cum_lw;
	int	cum_ow;
	int	cum_sb;
	int	cum_mb;
	int	cum_lb;
	int	cum_ob;
}typesizecnt;

typedef struct FTPdata
{
	CString m_cLocalDirText;
	CString	m_cRemoteDirText;
	CString	m_cRemoteDir;
	CString m_cLocalDir;
	CString	m_cAddress;
	CString m_cName;
	CString m_cPass;
	CString	m_cRemoteImageDir;
	CString m_cLocalImageDir;
}typeFTPdata;

typedef struct idxcmp
{
	CString month;
	CString day;
	CString unitid;
	CString lotid;
	CString recipe;
	CString proid;
	CString path;
	CString defectfilename;	
}typeidxcmp;


typedef struct timedata
{
	CString month;
	CString day;
}typetimedata;

//20160725 sc
typedef struct lotintime
{
	CString year;
	CString month;
	CString day;
	CString hour;
	CString min;
	CString second;
}typelotintime;

class CDialogDFServer : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogDFServer)

public:
	CDialogDFServer(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogDFServer();
	BOOL ReadIDXFile(CString idxfilepath);
	CString R_DefectFileName_a(CString glassid,CString H_procid,CString time,CString unitid);
	CString H_Key_id_e(CString glassid,CString time);
	CString H_Lot_id_e(char * lotid);
	CString H_Cst_id_e(char * cstid);
	CString H_Gls_id_e(CString glassid);
	CString H_Mo_gls_id_e2_e(CString glassid);
	CString H_Mo_gls_id_m1_e(CString glassid);
	CString H_Half_gls_id_e2_e(CString glassid);
	CString H_Half_gls_id_m1_e(CString glassid);
	CString H_Cf_id_e(char * cdif);
	CString H_Gls_cut_e2_e(CString glassid);
	CString H_Gls_cut_m1_e(CString glassid);
	CString H_Slot_no_e(CString glassid);
	CString H_Eqp_id_e(char * eqpid);
	CString H_Machine_id_e(char * machineid);
	CString H_Unit_id_e(char * unitid);
	CString H_Oper_id_e(char * operid);
	CString H_Proc_id_e(char * procid);
	CString H_Recipe_id_e(int recipeid);
	
	void DfHeaderUpdate();
	void DFBodyUpdate();
	void DFPanelUpdate(int count,int def_idx);
	void DFInfoUpdate(int def_idx);
	CString B_Panel_qty_e(int count);
	CString B_Main_e(int count,int type);
	CString B_Sub_e(int count,int type);

	CString B_Defect_no_e(int count);
	CString	B_Defect_no_total_e(int count,int type);
	CString B_Defect_no_step_s_e(int count,int type);
	CString B_Defect_no_step_m_e(int count,int type);
	CString B_Defect_no_step_l_e(int count,int type);
	CString B_Defect_no_step_o_e(int count,int type);
	CString B_Defect_no_cum_s_e(int count,int type);
	CString B_Defect_no_cum_m_e(int count,int type);
	CString B_Defect_no_cum_l_e(int count,int type);
	CString B_Defect_no_cum_o_e(int count,int type);

	CString B_Image_file_e(CString path);
	CString B_Image_file_ref_e(CString path);
	CString B_Image_file_ptn_e(CString path);

	CString P_H_Key_id_e(char *glassid,CString procid,CString time);
	CString P_H_Panel_id_e(char *glassid,CString cellid);
	CString P_H_Main_sub_info();

	CString P_B_Defect_no_e(int count);
	CString P_B_Defect_no_total_e(int count,int type);
	CString P_B_Defect_no_step_s_e(int count,int type);
	CString P_B_Defect_no_step_m_e(int count,int type);
	CString P_B_Defect_no_step_o_e(int count,int type);
	CString P_B_Defect_no_step_l_e(int count,int type);
	CString P_B_Defect_no_cum_s_e(int count,int type);
	CString P_B_Defect_no_cum_m_e(int count,int type);
	CString P_B_Defect_no_cum_l_e(int count,int type);
	CString P_B_Defect_no_cum_o_e(int count,int type);

	void P_B_Defect_info_defect_e();
	//CString m_strDefectInfo[DEFECT_MAX_COUNT];
	BOOL m_bDefectInfo;
	BOOL m_bXml;
	BOOL CreateXmlfile(CString xmlpath,int cnt);
	CString DefectFilepath(CString def_path);
	BOOL WriteXmlHeader(BOOL bRet);
	BOOL WriteXmlBody(BOOL bRet);
	BOOL WriteXmlPanel(int idx);
	BOOL WriteXmlPanel2(int idx);
	void WriteDefectInfo(int idx1,int idx2);
	
	BOOL GlassIdxFile(CString glassid,CString recipe,CString lotid,CString procid,CString unitid,CString DFfilename);
	BOOL ImageIdxFile(FILE *stream, CString glassid,CString recipe,CString lotid,CString procid,CString unitid,CString DFfilename,BOOL filetype,CString data);

	//20160418 ngh
	void SetDefectInfo(typeDefectInfo defInfo);
	void SetDefectInfo(typedefectinfo defInfo, int idx);
	BOOL GetWriteDefectFile();
	BOOL CopyFile_Df(CString sourcepath,CString targetpath);
	BOOL GetWriteOnCheck();
	void ImageFileCopy(CString sourcepath,int type,CString glassid,CString recipe,CString lotid,CString procid,CString unitid,CString eqp,CString panelid,double x,double y,CString no);

	int m_idf_count_;
	int m_ii;
	int m_ij;
	CString m_strDfFilePath;

	void InitDefectCnt();
	void WriteDefectInfo(int idx);
	int GetDefectCnt();

	//20160428 ngh
	BOOL DFServerConnect(CString IPAddress, CString ID, CString PW, CString LocalPath, CString RemotePath, int isUpDown);
	BOOL DFServerClose();
	BOOL LoadDFServerPara(CString path);
	BOOL SaveDFServerPara(CString path);
	BOOL DFServerUpLoad(BOOL isUpLoadFileDel);
	BOOL DFServerDownLoad();
	CString GetDFServerIP();
	CString GetDFServerID();
	CString GetDFServerPW();
	CString GetDFServerLocalPath();
	CString GetDFServerRemotePath();
	CString GetDFFilePath();
	BOOL GetDFWriteFile();
	CString FileNameChange(CString filename);
	CString GetImageIdxFile();
	BOOL IDXUpdate();
	
	void BeforeProcessDFload(CString dffilename);
	void BeforeProcessDFupdate();
	CString GetGlassIDHash(CString glassid,int hashidx);
	
	//20160429 ngh
	BOOL GetServerConnectStatus();
	BOOL CreatDefectFile();
	LRESULT OnDefectMessage(WPARAM para0, LPARAM para1);

	//20160502 ngh
	CString m_strImgFilePath;
	CString GetImgFilePath();
	BOOL DefectFileDownload(typeidxcmp idxfileinfo);
	BOOL DefectFileUpdate();

	//20160615 sc
	CString GetBeforeDfFilePath();

	//20160617 ngh
	int GetWriteTimeOut();
	BOOL m_bWriteComp;
	BOOL GetWriteComp();

	//20160623 sc
	CString m_imagefilepath[DEFECT_MAX_COUNT];
	CString GetImageFilePath(int idx);

	BOOL DFWriteLog(CString text,int logtype);

	typelotintime m_lotintime;  //20160725 sc
	void Getlotintime(CString timedata);
	
	
	//20160728 ngh
	FILE *m_stream;
	
	CString m_imagemappath;
protected:
	CFPTClient m_cFPTCilent;
	typeFTPdata m_FTPdata;
	typeidxcmp m_idxfilecmp[100];
	typetimedata m_timedata;
	
	CString m_ImageIdxFile;
	

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DEFECT_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CXml *m_Xml;
	typedefect_file_a	m_defect_file_a;
	typeheader_e		m_header_e;
	typeheader_a		m_header_a;
	typebody_e			m_body_e;
	typebody_a			m_body_a;
	typepanel_e			m_panel_e;
	typepanel_a			m_panel_a;
	typedefectinfo		m_dfinfo;
	typesizecnt			m_sizecnt[30][30];
	BOOL m_bWriteDefectFile;
	typeDefectInfo *m_DefectInfo;
	int m_nDefectCnt;
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlIpsSave();

	//20160728 ngh
	void Init_typepanel_a();
	
	CIPAddressCtrl m_IPAddress_Server;
	CString m_edit_strID;
	CString m_edit_strPW;
	CString m_edit_strLocalDir;
	CString m_edit_strRemoteDir;
	void ClickBtnenhctrlDfConnect();
	void ClickBtnenhctrlDfUpLoad();
	void ClickBtnenhctrlDfDownLoad();
	void ClickBtnenhctrlDfApply();
	void ClickBtnenhctrlDfSave();
	CBtnenhctrl m_btn_DFServerConnect;
	CButton m_check_write_on_check;
	CButton m_check_up_load_file_del;
	CString m_strBeforeDfFilePath;
	void ImageUpload();
	
	
	void ClickBtnenhctrlDfFind();
	int m_edit_nWriteTimeOut;
	BOOL m_bWriteFile;
	
};

