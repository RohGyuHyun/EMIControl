#pragma once
//#include "DialogDFServer.h"
#include <map>
#define	DEFAULT			0
#define	HEADER			0
#define	PANEL			1
#define	BODY			0

#define	DF_SMALL		0
#define	DF_MEDIUM		1
#define	DF_LARGE		2
#define	DF_OVERFLOW		3
#define	DF_TOTAL		4
//#define	PANEL_BODY		1

#ifndef RELEASE_PTR
#define RELEASE_PTR(x)	\
	if( x != NULL )		\
		{				\
		x.Release();	\
		x = NULL;		\
		}
#endif
 


#define XML_PATH	_T("D:\\Working\\EMI\\FTP_test_xml\\")
typedef enum
{
	MSXML_UNKNOWN	= 00,
	MSXML26 		= 26,
	MSXML30 		= 30,
	MSXML40 		= 40,
	MSXML50 		= 50,
	MSXML60 		= 60,
} MSXML_VERSION;

typedef struct defectdata
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
	
}typedefectdata;

/*
CString defectfilename;
	CString keyid;
	CString lotid;
	CString cstid;
	CString mo_gls_id;
	CString half_gls_id;
	CString cf_id;
	CString cut;
	CString slot;
	CString eqp;
	CString machine;
	CString unit;
	CString oper;
	CString proc;
	CString recipe;
	CString ins_time;
	*/

typedef struct defect_info
{
	CString shop;
	CString def_no;
	CString gls_start_time;
	double point_x;
	double point_y;
	double origin_point_x;
	double origin_point_y;
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
	CString review_x_size;
	CString review_y_size;
	CString review_layer;
	CString pxl_zone;
	CString encap_type;
	CString zone;
	CString ei_rpc_op;
	CString image_file_no;
	CString black_white;
	CString cell_id;
	int idx;
}typedefect_info;

typedef struct dfcount
{
	int total_step;
	int total_cum;
	int old_step_s_rb;
	int old_step_s_rw;
	int old_step_m_rb;
	int old_step_m_rw;
	int old_step_l_rb;
	int old_step_l_rw;
	int old_step_o_rb;
	int old_step_o_rw;
	int old_cum_s_rb;
	int old_cum_s_rw;
	int old_cum_m_rb;
	int old_cum_m_rw;
	int old_cum_l_rb;
	int old_cum_l_rw;
	int old_cum_o_rb;
	int old_cum_o_rw;
	int new_step_s_rb;
	int new_step_s_rw;
	int new_step_m_rb;
	int new_step_m_rw;
	int new_step_l_rb;
	int new_step_l_rw;
	int new_step_o_rb;
	int new_step_o_rw;
	int new_cum_s_rb;
	int new_cum_s_rw;
	int new_cum_m_rb;
	int new_cum_m_rw;
	int new_cum_l_rb;
	int new_cum_l_rw;
	int new_cum_o_rb;
	int new_cum_o_rw;

}typedfcount;

class CXml
{	
public:
	CXml(void);
	~CXml(void);
	MSXML2::IXMLDOMDocument2Ptr m_pDoc2;
protected:
	CString						m_strFilePath;
	MSXML_VERSION				m_emVersion;
	std::map< CString, CString>	m_mpNamespace;

	MSXML2::IXMLDOMDocument2Ptr m_pDoc;
	
	
	

public:
	BOOL Create_Xml_File(CString xmlpath,int idx);
	BOOL AppendChild(CString searchelement,CString addelement,int type);
	BOOL SetAttribute(CString searchelement,CString addattribute,CString addvalue,int type);
	BOOL Xml_Format();
	BOOL Xml_Format_Header();
	BOOL Xml_Format_Body();
	BOOL Xml_Format_Panel(int idx);
	BOOL Xml_Format_Defect(int idx);
	BOOL PutText(CString searchelement,CString value,int type);
	BOOL GetAttribute(CString attribute,CString value,int type);
	BOOL ChangeValue(CString element,CString attribute,CString value,int type);
	int GetElementCount(CString element1,CString element2,int idx);
	int GetAttributeValue(CString element,CString attribute,int idx);
	BOOL SetDfAttribute(CString element1,CString element2,CString addattribute,CString addvalue,int type1,int type2);
	int GetElementCountTest(CString element1,CString element2,int idx);
	MSXML2::IXMLDOMDocument2Ptr GetDoc();
	CString GetFilePath();
	void InitDoc();
	BOOL Xml_Load_Update(CString file,int idx,typedefectdata defectdata);
	HRESULT Transform(BSTR bsInputXMLFile,BSTR bsOutputXMLFile);
	typedefectdata m_defectdata;
	typedefectdata SetDefectData(char *glsid,char *time,char *index,char *cellid,double measure_x_e2,double measure_y_e2,double measure_x_ap2,double measure_y_ap2,double size_long,double width,
		double height,double area,char *size_range,char *rank,char *in_out,double gray_min,double gray_25,double gray_mediean,double gray_75,double gray_max,double gray_mean,double gray_deviation,char *black_white,char *image_path);
	BOOL Defect_Update_New(int idx,CString filepath,typedefectdata data); //юс╫ц

	BOOL GlassInfoUpdate(CString defectfilename,CString keyid,CString lotid,CString cstid,CString mo_gls_id,CString half_gls_id,CString cf_id,CString cut,CString slot,
	CString eqp,CString machine,CString unit,CString oper,CString proc,CString recipe,CString ins_time_start,CString ins_time_end);
	
	void Update_Defect_CountInit(CString filepath);

	BOOL TotalCountSetAttribute(int type,int bw);

	
	//
};


