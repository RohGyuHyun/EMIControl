#include "StdAfx.h"

#include "Xml.h"




CXml::CXml(void)
{
	
}


CXml::~CXml(void)
{
}

BOOL CXml::Create_Xml_File(CString xmlpath,int idx)
{
	CString strProcessingInstruction;
	strProcessingInstruction.Format( _T("version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\""));//lpszEncoding);
	MSXML2::IXMLDOMProcessingInstructionPtr pPI = NULL;
	MSXML2::IXMLDOMElementPtr	pRootElement = NULL;
	MSXML2::IXMLDOMElementPtr	pGlassElement = NULL;
	HRESULT hr = S_FALSE;
	VARIANT_BOOL vbSuccessful = VARIANT_TRUE;
	CString Root,Glass;
	
	
	
	m_strFilePath.Format(_T("%s"),xmlpath);   // XML_PATH
	Root.Format(_T("DEFECT_FILE"));
	Glass.Format(_T("GLASS"));
	m_emVersion = MSXML50;
	if(m_pDoc == NULL)
	{
		hr = m_pDoc.CreateInstance(__uuidof(DOMDocument));
		pPI = m_pDoc->createProcessingInstruction( _bstr_t(_T("xml")), _bstr_t(strProcessingInstruction));
		if( pPI == NULL )
		{
			return FALSE;
		}
		m_pDoc->appendChild(pPI);
		pRootElement = m_pDoc->createElement((_bstr_t)Root);
		pGlassElement = m_pDoc->createElement((_bstr_t)Glass);

		pRootElement->appendChild(pGlassElement);
		m_pDoc->appendChild(pRootElement);
		
		//m_pDoc->save((_variant_t)m_strFilePath);
	}
	return TRUE;
}

BOOL CXml::Xml_Format()
{
	
	try{
		SetAttribute(_T("DEFECT_FILE"),_T("NAME"),_T(""),DEFAULT);
		AppendChild(_T("GLASS"),_T("HEADER"),DEFAULT);
		AppendChild(_T("GLASS"),_T("BODY"),DEFAULT);
		
		Xml_Format_Header();
		Xml_Format_Body();
	//	Xml_Format_Panel();
		return TRUE;
	}
	catch( _com_error e )
	{
		TRACE( _T("CXml::Xml_Format() failed:%s\n"), e.ErrorMessage());
		return FALSE;
	}

}

BOOL CXml::Xml_Format_Header()
{
	try
	{
		AppendChild(_T("HEADER"),_T("KEY_ID"),HEADER);
		AppendChild(_T("HEADER"),_T("LOT_ID"),HEADER);
		AppendChild(_T("HEADER"),_T("CST_ID"),HEADER);
		AppendChild(_T("HEADER"),_T("GLS_ID"),HEADER);
		AppendChild(_T("HEADER"),_T("MO_GLS_ID"),HEADER);
		AppendChild(_T("HEADER"),_T("HALF_GLS_ID"),HEADER);
		AppendChild(_T("HEADER"),_T("CF_ID"),HEADER);
		AppendChild(_T("HEADER"),_T("GLS_CUT"),HEADER);
		AppendChild(_T("HEADER"),_T("SLOT_NO"),HEADER);
		AppendChild(_T("HEADER"),_T("EQP_ID"),HEADER);
		AppendChild(_T("HEADER"),_T("MACHINE_ID"),HEADER);
		AppendChild(_T("HEADER"),_T("UNIT_ID"),HEADER);
		AppendChild(_T("HEADER"),_T("OPER_ID"),HEADER);
		AppendChild(_T("HEADER"),_T("PROC_ID"),HEADER);
		AppendChild(_T("HEADER"),_T("RECIPE_ID"),HEADER);
		AppendChild(_T("HEADER"),_T("INSP_TIME"),HEADER);
		

		return TRUE;
	}
	catch( _com_error e )
	{
		TRACE( _T("CXml::Xml_Format_Header() failed:%s\n"), e.ErrorMessage());
		return FALSE;
	}
}

BOOL CXml::Xml_Format_Body()
{
	try{
		AppendChild(_T("BODY"),_T("PANEL_QTY"),BODY);
		AppendChild(_T("PANEL_QTY"),_T("MAIN"),BODY);
		AppendChild(_T("PANEL_QTY"),_T("SUB"),BODY);
		AppendChild(_T("BODY"),_T("JUDGE_INFO"),BODY);
		AppendChild(_T("JUDGE_INFO"),_T("LATEST"),BODY);
		AppendChild(_T("JUDGE_INFO"),_T("FDI"),BODY);
		AppendChild(_T("JUDGE_INFO"),_T("FRT"),BODY);
		AppendChild(_T("JUDGE_INFO"),_T("PTL"),BODY);
		AppendChild(_T("JUDGE_INFO"),_T("GCL"),BODY);
		AppendChild(_T("BODY"),_T("DEFECT_NO"),BODY);
		AppendChild(_T("DEFECT_NO"),_T("TOTAL"),BODY);
		AppendChild(_T("DEFECT_NO"),_T("STEP_S"),BODY);
		AppendChild(_T("DEFECT_NO"),_T("STEP_M"),BODY);
		AppendChild(_T("DEFECT_NO"),_T("STEP_L"),BODY);
		AppendChild(_T("DEFECT_NO"),_T("STEP_O"),BODY);
		AppendChild(_T("DEFECT_NO"),_T("CUM_S"),BODY);
		AppendChild(_T("DEFECT_NO"),_T("CUM_M"),BODY);
		AppendChild(_T("DEFECT_NO"),_T("CUM_L"),BODY);
		AppendChild(_T("DEFECT_NO"),_T("CUM_O"),BODY);
		AppendChild(_T("BODY"),_T("IMAGE_FILE"),BODY);
		AppendChild(_T("IMAGE_FILE"),_T("REF"),BODY);
		AppendChild(_T("IMAGE_FILE"),_T("PTN"),BODY);
		
		
		return TRUE;
	}
	catch( _com_error e )
	{
		TRACE( _T("CXml::Xml_Format_Body( ) failed:%s\n"), e.ErrorMessage());
		return FALSE;
	}

}

BOOL CXml::Xml_Format_Panel(int idx)
{
	try{
		AppendChild(_T("GLASS"),_T("PANEL"),DEFAULT);
		AppendChild(_T("PANEL"),_T("HEADER"),DEFAULT + idx);
		AppendChild(_T("PANEL"),_T("BODY"),DEFAULT + idx);
		AppendChild(_T("PANEL"),_T("DEFECT_INFO"),DEFAULT + idx);
		AppendChild(_T("HEADER"),_T("KEY_ID"),PANEL + idx);
		AppendChild(_T("HEADER"),_T("PANEL_ID"),PANEL + idx);
		AppendChild(_T("HEADER"),_T("MAIN_SUB_INFO"),PANEL + idx);
		AppendChild(_T("HEADER"),_T("JUDGE"),PANEL + idx);
		AppendChild(_T("BODY"),_T("JUDGE_INFO"),PANEL + idx);
		AppendChild(_T("BODY"),_T("DEFECT_NO"),PANEL + idx);
		AppendChild(_T("JUDGE_INFO"),_T("LATEST"),PANEL + idx);
		AppendChild(_T("JUDGE_INFO"),_T("FDI"),PANEL + idx);
		AppendChild(_T("JUDGE_INFO"),_T("FRT"),PANEL + idx);
		AppendChild(_T("JUDGE_INFO"),_T("PTL"),PANEL + idx);
		AppendChild(_T("JUDGE_INFO"),_T("GCL"),PANEL + idx);
		AppendChild(_T("DEFECT_NO"),_T("TOTAL"),PANEL + idx);
		AppendChild(_T("DEFECT_NO"),_T("STEP_S"),PANEL + idx);
		AppendChild(_T("DEFECT_NO"),_T("STEP_M"),PANEL + idx);
		AppendChild(_T("DEFECT_NO"),_T("STEP_L"),PANEL + idx);
		AppendChild(_T("DEFECT_NO"),_T("STEP_O"),PANEL + idx);
		AppendChild(_T("DEFECT_NO"),_T("CUM_S"),PANEL + idx);
		AppendChild(_T("DEFECT_NO"),_T("CUM_M"),PANEL + idx);
		AppendChild(_T("DEFECT_NO"),_T("CUM_L"),PANEL + idx);
		AppendChild(_T("DEFECT_NO"),_T("CUM_O"),PANEL + idx);
//		AppendChild(_T("DEFECT_INFO"),_T("DEFECT"),DEFAULT + idx);
//		AppendChild(_T("DEFECT_INFO"),_T("IMG"),DEFAULT + idx);
		
		
		return TRUE;
	}
	catch( _com_error e )
	{
		TRACE( _T("CXml::Xml_Format_Panel() failed:%s\n"), e.ErrorMessage());
		return FALSE;
	}
}



BOOL CXml::Xml_Format_Defect(int idx)
{
	try{
		AppendChild(_T("DEFECT_INFO"),_T("DEFECT"),DEFAULT + idx);
	//	AppendChild(_T("DEFECT_INFO"),_T("IMG"),DEFAULT + idx);
	}
	catch( _com_error e)
	{
		TRACE( _T("CXml::Xml_Format_Defect() failed:%s\n"), e.ErrorMessage());
		return FALSE;
	}
	return TRUE;
}

BOOL CXml::PutText(CString searchelement,CString value,int type)
{
	CString temp;
	temp.Format(_T("//%s"),searchelement);
	try
	{
		MSXML2::IXMLDOMElementPtr	pSearchElement = NULL;
		
		//m_pDoc.CreateInstance(__uuidof(DOMDocument));
		//m_pDoc->load((_variant_t)m_strFilePath);

		pSearchElement = m_pDoc->selectNodes((_bstr_t)temp)->Getitem(type);
		pSearchElement->Puttext((_bstr_t)value);
		//m_pDoc->save((_variant_t)m_strFilePath);
		return TRUE;
	}
	catch( _com_error e )
	{
		TRACE( _T("CXml::AppendChild( ) failed:%s\n"), e.ErrorMessage());
		return FALSE;
	}
}

BOOL CXml::AppendChild(CString searchelement,CString addelement,int type)
{
	CString temp;
	temp.Format(_T("//%s"),searchelement);
	try
	{
		MSXML2::IXMLDOMElementPtr	pSearchElement = NULL;
		MSXML2::IXMLDOMElementPtr	pAddElement = NULL;
		
		//m_pDoc.CreateInstance(__uuidof(DOMDocument));
		//m_pDoc->load((_variant_t)m_strFilePath);

		pAddElement = m_pDoc->createElement((_bstr_t)addelement);
		pSearchElement = m_pDoc->selectNodes((_bstr_t)temp)->Getitem(type)->appendChild(pAddElement);

		//m_pDoc->save((_variant_t)m_strFilePath);
		return TRUE;
	}
	catch( _com_error e )
	{
		TRACE( _T("CXml::AppendChild( ) failed:%s\n"), e.ErrorMessage());
		return FALSE;
	}
}


BOOL CXml::SetAttribute(CString searchelement,CString addattribute,CString addvalue,int type)
{
	CString temp,temp2;
	temp.Format(_T(""));
	temp2.Format(_T(""));
	temp.Format(_T("//%s"),searchelement);
	temp2.Format(_T("%s"),addvalue);
	try{
		MSXML2::IXMLDOMElementPtr	pSearchElement = NULL;
		MSXML2::IXMLDOMAttributePtr pAttribute = NULL;

		//m_pDoc.CreateInstance(__uuidof(DOMDocument));
		//m_pDoc->load((_variant_t)m_strFilePath);

		pAttribute = m_pDoc->createAttribute((_bstr_t)addattribute);
		pAttribute->Putvalue((_variant_t)addvalue);
		pSearchElement = m_pDoc->selectNodes((_bstr_t)temp)->Getitem(type);
		pSearchElement->setAttributeNode(pAttribute);
		//m_pDoc->save((_variant_t)m_strFilePath);
		return TRUE;
	}
	catch( _com_error e )
	{
		TRACE( _T("CXml::SetAttribute( ) failed:%s\n"), e.ErrorMessage());
		return FALSE;
	}
}

BOOL CXml::TotalCountSetAttribute(int type,int bw)
{
	try{
		MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
		MSXML2::IXMLDOMNodeListPtr pNodeList2 = NULL;
		MSXML2::IXMLDOMNodeListPtr pNodeListTotal = NULL;

		MSXML2::IXMLDOMNodePtr		pNode = NULL;	
		
		
		MSXML2::IXMLDOMNamedNodeMapPtr pNodeMap = NULL;

		CString count_temp;
		int dcount_temp = 0;

		switch(type)
		{
		case 0:
			pNodeList = m_pDoc->selectNodes(L"//STEP_S");
			pNodeList2 = m_pDoc->selectNodes(L"//CUM_S");
			pNodeListTotal = m_pDoc->selectNodes(L"//TOTAL");

			if(bw == 0)
			{
				pNode = pNodeList->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(0)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeList2->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(0)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(0)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(1)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(2)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(2)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;
			}
			else if(bw == 1)
			{
				pNode = pNodeList->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(1)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeList2->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(1)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(0)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(1)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(2)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(2)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;
			}
			else
			{
			}
			break;
		case 1:
			pNodeList = m_pDoc->selectNodes(L"//STEP_M");
			pNodeList2 = m_pDoc->selectNodes(L"//CUM_M");
			pNodeListTotal = m_pDoc->selectNodes(L"//TOTAL");
			
			if(bw == 0)
			{
				pNode = pNodeList->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(0)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeList2->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(0)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(0)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(1)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(2)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(2)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;
			}
			else if(bw == 1)
			{
				pNode = pNodeList->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(1)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeList2->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(1)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(0)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(1)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(2)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(2)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;
			}
			else
			{
			}
			break;
		case 2:
			pNodeList = m_pDoc->selectNodes(L"//STEP_L");
			pNodeList2 = m_pDoc->selectNodes(L"//CUM_L");
			pNodeListTotal = m_pDoc->selectNodes(L"//TOTAL");

			if(bw == 0)
			{
				pNode = pNodeList->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(0)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeList2->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(0)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(0)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(1)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(2)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(2)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;
			}
			else if(bw == 1)
			{
				pNode = pNodeList->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(1)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeList2->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(1)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(0)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(1)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(2)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(2)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;
			}
			else
			{
			}
			break;
		case 3:
			pNodeList = m_pDoc->selectNodes(L"//STEP_O");
			pNodeList2 = m_pDoc->selectNodes(L"//CUM_O");
			pNodeListTotal = m_pDoc->selectNodes(L"//TOTAL");

			if(bw == 0)
			{
				pNode = pNodeList->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(0)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeList2->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(0)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(0)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(1)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(2)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(2)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;
			}
			else if(bw == 1)
			{
				pNode = pNodeList->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(1)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeList2->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(1)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(0)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(1)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;

				pNode = pNodeListTotal->Getitem(0);
				pNodeMap = pNode->Getattributes();
				count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(2)->GetnodeValue().bstrVal);
				dcount_temp = _wtoi(count_temp);
				dcount_temp += 1;
				pNodeMap->Getitem(2)->PutnodeValue((_variant_t)dcount_temp);
				dcount_temp = 0;
			}
			else
			{
			}
			break;
		}
		
		return TRUE;
	}
	catch( _com_error e )
	{
		TRACE( _T("CXml::SetAttribute( ) failed:%s\n"), e.ErrorMessage());
		return FALSE;
	}
}

BOOL CXml::SetDfAttribute(CString element1,CString element2,CString addattribute,CString addvalue,int type1,int type2)
{
	CString temp,temp2,temp3;
	temp.Format(_T("//%s"),element1);
	temp3.Format(_T("//%s"),element2);
	temp2.Format(_T("%s"),addvalue);
	try{
		MSXML2::IXMLDOMElementPtr	pSearchElement = NULL;
	
		MSXML2::IXMLDOMElementPtr	pElement = NULL;
		MSXML2::IXMLDOMAttributePtr pAttribute = NULL;
		MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;

		//m_pDoc.CreateInstance(__uuidof(DOMDocument));
		//m_pDoc->load((_variant_t)m_strFilePath);

		
		pAttribute = m_pDoc->createAttribute((_bstr_t)addattribute);
		pAttribute->Putvalue((_variant_t)temp2);
		pSearchElement = m_pDoc->selectNodes((_bstr_t)temp)->Getitem(type1)->GetlastChild();
//		pSearchElement = m_pDoc->selectNodes((_bstr_t)temp)->Getitem(type1);
		
	
		pSearchElement->setAttributeNode(pAttribute);
		//m_pDoc->save((_variant_t)m_strFilePath);
		return TRUE;
	}
	catch( _com_error e )
	{
		TRACE( _T("CXml::SetAttribute( ) failed:%s\n"), e.ErrorMessage());
		return FALSE;
	}
}

BOOL CXml::ChangeValue(CString element,CString attribute,CString value,int type)
{
	CString temp,temp2;
	try{
		/*
		MSXML2::IXMLDOMElementPtr	pSearchElement = NULL;
		MSXML2::IXMLDOMAttributePtr pAttribute = NULL;

		m_pDoc.CreateInstance(__uuidof(DOMDocument));
		m_pDoc->load((_variant_t)m_strFilePath);

		pAttribute = m_pDoc->createAttribute((_bstr_t)addattribute);
		pAttribute->Putvalue((_variant_t)temp2);
		pSearchElement = m_pDoc->selectNodes((_bstr_t)temp)->Getitem(type);
		pSearchElement->setAttributeNode(pAttribute);
		m_pDoc->save((_variant_t)m_strFilePath);
		return TRUE;
		*/
	//	temp.Format(_T("//*[@%s='%s']"),attribute,value);
		MSXML2::IXMLDOMDocument2Ptr pDoc = NULL;
		MSXML2::IXMLDOMElementPtr pElement = NULL;
		pDoc.CreateInstance(__uuidof(DOMDocument));
		pDoc->load((_variant_t)m_strFilePath);
		temp.Format(_T("//%s"),element);
		temp2.Format(_T("//*[@%s]"),attribute);
		pElement = pDoc->selectNodes(_bstr_t(temp))->Getitem(type);
		if(pElement)
		{
			//pElement//getAttributeNode((_bstr_t)temp2)->Putvalue((_variant_t)value);
		}
		return TRUE;
	}
	catch( _com_error e )
	{
		TRACE( _T("CXml::ChangeValue( ) failed:%s\n"), e.ErrorMessage());
		return FALSE;
	}
	
}
	 

BOOL CXml::GetAttribute(CString attribute,CString value,int type)
{
	CString temp,temp2;
	temp.Format(_T("//%s"));
	return TRUE;
}

int CXml::GetElementCount(CString element1,CString element2,int idx)
{
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	MSXML2::IXMLDOMElementPtr	pSearchElement = NULL;

	long count = 0;
	CString temp1,temp2;
	temp1.Format(_T("//%s"),element1);
	temp2.Format(_T("//%s"),element2);
	//m_pDoc.CreateInstance(__uuidof(DOMDocument));
	//m_pDoc->load((_variant_t)m_strFilePath);

	pNodeList = m_pDoc->selectNodes((_bstr_t)temp1)->Getitem((long)idx)->selectSingleNode((_bstr_t)temp2)->GetchildNodes();
	count = pNodeList->Getlength();
	
	return count;
}

int CXml::GetElementCountTest(CString element1,CString element2,int idx)
{
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	MSXML2::IXMLDOMElementPtr	pSearchElement = NULL;

	long count = 0;
	CString temp1,temp2;
	temp1.Format(_T("//%s"),element1);
	temp2.Format(_T("//%s"),element2);
	//m_pDoc.CreateInstance(__uuidof(DOMDocument));
	//m_pDoc->load((_variant_t)m_strFilePath);

	pNodeList = m_pDoc->selectNodes((_bstr_t)temp1);//->Getitem((long)idx)->selectSingleNode((_bstr_t)temp2)->GetchildNodes();
	count = pNodeList->Getlength();
	
	return count;
}

int CXml::GetAttributeValue(CString element,CString attribute,int idx)
{
	MSXML2::IXMLDOMElementPtr	pSearchElement = NULL;
	CString temp,value;
	int rslt = 0;
	temp.Format(_T("//%s"),element);

	//m_pDoc.CreateInstance(__uuidof(DOMDocument));
	//m_pDoc->load((_variant_t)m_strFilePath);

	pSearchElement = m_pDoc->selectNodes((_bstr_t)temp)->Getitem((long)idx + 1);
	value.Format(_T("%S"),pSearchElement->getAttribute((_bstr_t)attribute).bstrVal);
	rslt = _wtoi(value);
	return rslt;
}

MSXML2::IXMLDOMDocument2Ptr CXml::GetDoc()
{
	return m_pDoc;
}

CString CXml::GetFilePath()
{
	return m_strFilePath;
}

void CXml::InitDoc()
{
	m_pDoc = NULL;
}

HRESULT CXml::Transform(BSTR bsInputXMLFile,BSTR bsOutputXMLFile)
{
	MSXML2::IXMLDOMDocumentPtr pXML=NULL; // 원래문서
	MSXML2::IXMLDOMDocumentPtr pXSL=NULL; // 스타일쉬트~
	MSXML2::IXMLDOMDocumentPtr pResult=NULL;
	HRESULT hr=S_OK;

	try
	{
		hr=pXML.CreateInstance(__uuidof(DOMDocument)); // 문서불러오기
		pXML->put_async(VARIANT_FALSE);
		pXML->load((_variant_t)bsInputXMLFile);
		hr=pXSL.CreateInstance(__uuidof(DOMDocument));

		// 쉿~타일시트불러오기
		pXSL->put_async(VARIANT_FALSE);
		pXSL->load((_variant_t)"D:\\Working\\EMI\\EMIControl\\EMIControl\\EMIControl\\Include\\xls\\indent.xsl"); // 파일경로에주의하세요!!!
		hr=pResult.CreateInstance(__uuidof(DOMDocument));
		if(FAILED(hr)) {
		return E_FAIL;
		}
		VARIANT vObject;
		VariantInit(&vObject);
		vObject.vt=VT_DISPATCH;
		vObject.pdispVal=pResult;
		hr=pXML->transformNodeToObject(pXSL,vObject); // 변환하기~
		if(FAILED(hr)) { // 에러처리
			return E_FAIL;
		}
		// 변환완성저장하기
		pResult->save((_variant_t)bsOutputXMLFile);
	}
	catch(_com_error &e) {
		return E_FAIL;
	}
	return S_OK;
}

BOOL CXml::Xml_Load_Update(CString file,int idx,typedefectdata defectdata)
{
	//MSXML2::IXMLDOMDocumentPtr pXML=NULL; // 원래문서
	MSXML2::IXMLDOMDocument2Ptr pDoc = NULL;
	//m_pDoc = NULL;
	HRESULT hr = S_FALSE;
	VARIANT_BOOL vbSuccessful = VARIANT_TRUE;
	typedefect_info Defect_info;
	BOOL bDefect_update_new = FALSE;
	
	//m_strFilePath.Format(_T("%s"),xmlpath);   // XML_PATH
	
	//  20160630 검사 시퀀스에 Glass ID 가져와서 서버 index 파일 있는지 검사 하고 m_bDfIdxFile 변수에 TRUE,FALSE 작업 해야함 ( 7월1일부터 작업 예정)
	//  20160630 Get으로 Defect 정보 가져 와서 비교, Defect 정보 쓰고 ,New로 Defect 정보 새로 생성 - Get,New 함수 만들어야함  (7월1일부터 작업 예정) 
		//Defect_info = GetDefectInfo(idx,file,defectdata); 
		//Defect_Update_New(idx,file,defectdata);	//임시
		
		// 20160701 구조체 변수 이름,함수 이름 수정 필요 헷갈림
		//  20160701 검사 시퀀스에 Glass ID 가져와서 서버 index 파일 있는지 검사 하고 m_bDfIdxFile 변수에 TRUE,FALSE 작업 해야함 (7월1일부터 작업 예정)(미완료 작업 못함)
	
	return Defect_Update_New(idx,file,defectdata);	//임시;

}




BOOL CXml::Defect_Update_New(int idx,CString filepath,typedefectdata data)
{
	typedefectdata defectdata;
	typedefect_info defect_info;
//	typedfcount defect_count;
	MSXML2::IXMLDOMDocumentPtr pDoc=NULL;
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	MSXML2::IXMLDOMNodeListPtr pNodeList2 = NULL;
	MSXML2::IXMLDOMNodeListPtr pNodeListTotal = NULL;
	MSXML2::IXMLDOMNodeListPtr pNodeListStep_s = NULL;
	MSXML2::IXMLDOMNodeListPtr pNodeListStep_m = NULL;
	MSXML2::IXMLDOMNodeListPtr pNodeListStep_l = NULL;
	MSXML2::IXMLDOMNodeListPtr pNodeListStep_o = NULL;

	MSXML2::IXMLDOMNodeListPtr pNodeListCum_s = NULL;
	MSXML2::IXMLDOMNodeListPtr pNodeListCum_m = NULL;
	MSXML2::IXMLDOMNodeListPtr pNodeListCum_l = NULL;
	MSXML2::IXMLDOMNodeListPtr pNodeListCum_o = NULL;

	
	
	MSXML2::IXMLDOMNodePtr		pNode = NULL;	
	MSXML2::IXMLDOMNodePtr		pNode2 = NULL;
	MSXML2::IXMLDOMNodePtr		pNodeTotal = NULL;
	MSXML2::IXMLDOMNamedNodeMapPtr pNodeMap = NULL;
	MSXML2::IXMLDOMNamedNodeMapPtr pNodeMap2 = NULL;
	MSXML2::IXMLDOMNamedNodeMapPtr pNodeMapTotal = NULL;

	CString strProcessingInstruction;
	strProcessingInstruction.Format( _T("version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\""));//lpszEncoding);

	HRESULT hr = S_FALSE;
	VARIANT_BOOL vbSuccessful = VARIANT_TRUE;
	long count = 0,length = 0,countatti = 0,totalatti = 0,updatecountatti = 0;
	int dcount_temp = 0;
	CString defectattrname,defectnodevalue,data_temp,strlog,count_temp,test;
	CString time; // 임시
	BOOL bUpdatebreak = FALSE;
	BOOL bDefect_update_new = FALSE;	// 0 - update , 1 - new

	time.Format(_T("11112233445566"));
	double rslt_x = 0,rslt_y = 0,data_x = 0,data_y = 0; //임시 

	//hr=m_pDoc2.CreateInstance(__uuidof(DOMDocument)); // 문서불러오기
	//m_pDoc2->put_async(VARIANT_FALSE);
	//m_pDoc2->load((_variant_t)filepath);

	////hr = m_pDoc2.CreateInstance(__uuidof(DOMDocument)); 
	////hr = m_pDoc2->createProcessingInstruction( _bstr_t(_T("xml")), _bstr_t(strProcessingInstruction));
	//vbSuccessful = m_pDoc2->load((_variant_t)filepath);

	pNodeList = m_pDoc2->selectNodes(L"//DEFECT_INFO");
	pNodeListStep_s = m_pDoc2->selectNodes(L"//STEP_S");
	pNodeListStep_m = m_pDoc2->selectNodes(L"//STEP_M");
	pNodeListStep_l = m_pDoc2->selectNodes(L"//STEP_L");
	pNodeListStep_o = m_pDoc2->selectNodes(L"//STEP_O");

	pNodeList2 = pNodeList->Getitem(idx)->GetchildNodes();
	pNodeListTotal = m_pDoc2->selectNodes(L"//TOTAL");
	length = pNodeList2->Getlength();

	defect_info.gls_start_time.Format(_T("%s"),time);
	defect_info.point_x = data.measure_x_e2; //임시
	defect_info.point_y = data.measure_y_e2;
	defect_info.origin_point_x = data.measure_x_e2; //임시
	defect_info.origin_point_y = data.measure_y_e2;	//임시
	defect_info.active_area.Format(_T("%.3f"),data.area);  //임시
	defect_info.xpixel.Format(_T("%s"),data_temp);
	defect_info.ypixel.Format(_T("%s"),data_temp);
	defect_info.pixel.Format(_T("%s"),data_temp);
	defect_info.xsize.Format(_T("%.3f"),data.width * 1000);
	defect_info.ysize.Format(_T("%.3f"),data.height * 1000);
	defect_info.size.Format(_T("%s"),data_temp);
	defect_info.rank.Format(_T("%s"),data.rank);
	defect_info.mode.Format(_T("%s"),data_temp);
	defect_info.proc_id.Format(_T("%s"),data_temp);
	defect_info.unit_id.Format(_T("%s"),data_temp);
	defect_info.operator_id.Format(_T("%s"),data_temp);
	defect_info.ptn_code.Format(_T("%s"),data_temp);
	defect_info.review_x_size.Format(_T("%s"),data_temp);
	defect_info.review_y_size.Format(_T("%s"),data_temp);
	defect_info.review_layer.Format(_T("%s"),data_temp);
	defect_info.pxl_zone.Format(_T("%s"),data_temp);
	defect_info.encap_type.Format(_T("%s"),data_temp);
	defect_info.zone.Format(_T("%s"),data_temp);
	defect_info.ei_rpc_op.Format(_T("%s"),data_temp);
	defect_info.image_file_no.Format(_T("%s"),data_temp);
	defect_info.black_white.Format(_T("%s"),data.black_white);
	defect_info.cell_id.Format(_T("%s"),data.cellid);
	defect_info.idx = count;

	while(count<length)
	{
		pNode = pNodeList2->Getitem(count);
		pNodeMap = pNode->Getattributes();
		totalatti = pNodeMap->Getlength();
		while(countatti < totalatti)
		{
			defectattrname.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(countatti)->GetbaseName());
			if(!defectattrname.Compare(_T("POINT_X")))
			{
				defectnodevalue.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(countatti)->GetnodeValue().bstrVal);
				data_x = _wtof(defectnodevalue);
			}
			if(!defectattrname.Compare(_T("POINT_Y")))
			{
				defectnodevalue.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(countatti)->GetnodeValue().bstrVal);
				data_y = _wtof(defectnodevalue);
				rslt_x = abs(defect_info.point_x) - abs(data_x);
				rslt_y = abs(defect_info.point_y) - abs(data_y);
				if((abs(rslt_x) < 0.5) && (abs(rslt_y) < 0.5))  //같은 디펙일경우 EMI defect 정보로 업데이트 
				{
					defect_info.shop.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(0)->GetnodeValue().bstrVal);
					defect_info.def_no.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(1)->GetnodeValue().bstrVal);
					

					pNodeMap->Getitem(2)->PutnodeValue((_variant_t)defect_info.gls_start_time);
					pNodeMap->Getitem(3)->PutnodeValue((_variant_t)defect_info.point_x);
					pNodeMap->Getitem(4)->PutnodeValue((_variant_t)defect_info.point_y);
					pNodeMap->Getitem(5)->PutnodeValue((_variant_t)defect_info.origin_point_x);
					pNodeMap->Getitem(6)->PutnodeValue((_variant_t)defect_info.origin_point_y);
					pNodeMap->Getitem(7)->PutnodeValue((_variant_t)defect_info.active_area);
					pNodeMap->Getitem(8)->PutnodeValue((_variant_t)defect_info.xpixel);
					pNodeMap->Getitem(9)->PutnodeValue((_variant_t)defect_info.ypixel);
					pNodeMap->Getitem(10)->PutnodeValue((_variant_t)defect_info.pixel);
					pNodeMap->Getitem(11)->PutnodeValue((_variant_t)defect_info.xsize);
					pNodeMap->Getitem(12)->PutnodeValue((_variant_t)defect_info.ysize);
					pNodeMap->Getitem(13)->PutnodeValue((_variant_t)defect_info.size);
					pNodeMap->Getitem(14)->PutnodeValue((_variant_t)defect_info.rank);
					pNodeMap->Getitem(15)->PutnodeValue((_variant_t)defect_info.mode);
					pNodeMap->Getitem(16)->PutnodeValue((_variant_t)defect_info.proc_id);
					pNodeMap->Getitem(17)->PutnodeValue((_variant_t)defect_info.unit_id);
					pNodeMap->Getitem(18)->PutnodeValue((_variant_t)defect_info.operator_id);
					pNodeMap->Getitem(19)->PutnodeValue((_variant_t)defect_info.ptn_code);
					pNodeMap->Getitem(20)->PutnodeValue((_variant_t)defect_info.review_x_size);
					pNodeMap->Getitem(21)->PutnodeValue((_variant_t)defect_info.review_y_size);
					pNodeMap->Getitem(22)->PutnodeValue((_variant_t)defect_info.review_layer);
					pNodeMap->Getitem(23)->PutnodeValue((_variant_t)defect_info.pxl_zone);
					pNodeMap->Getitem(24)->PutnodeValue((_variant_t)defect_info.encap_type);
					pNodeMap->Getitem(25)->PutnodeValue((_variant_t)defect_info.zone);
					pNodeMap->Getitem(26)->PutnodeValue((_variant_t)defect_info.ei_rpc_op);
					pNodeMap->Getitem(27)->PutnodeValue((_variant_t)defect_info.image_file_no);
					
					int size_type = 0;
					if(!defect_info.rank.Compare(_T("S")))
						size_type = DF_SMALL;
					else if(!defect_info.rank.Compare(_T("M")))
						size_type = DF_MEDIUM;
					else if(!defect_info.rank.Compare(_T("L")))
						size_type = DF_LARGE;
					else if(!defect_info.rank.Compare(_T("OL")))
						size_type = DF_OVERFLOW;
					else
					{
						strlog.Format(_T("DFPanelUpdate() rank : size error "));
					}


					switch (size_type)
					{
					case DF_SMALL:
						if(!defect_info.black_white.Compare(_T("B")))
						{
							pNode2 = pNodeListStep_s->Getitem(idx + 1);
							pNodeMap2 = pNode2->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap2->Getitem(0)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMap2->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNode2 = pNodeListStep_s->Getitem(0);
							pNodeMap2 = pNode2->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap2->Getitem(0)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMap2->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNodeTotal = pNodeListTotal->Getitem(idx + 1);
							pNodeMapTotal = pNodeTotal->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNodeTotal = pNodeListTotal->Getitem(0);
							pNodeMapTotal = pNodeTotal->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
						}
						else
						{
							pNode2 = pNodeListStep_s->Getitem(idx + 1);
							pNodeMap2 = pNode2->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap2->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMap2->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNode2 = pNodeListStep_s->Getitem(0);
							pNodeMap2 = pNode2->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap2->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMap2->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNodeTotal = pNodeListTotal->Getitem(idx + 1);
							pNodeMapTotal = pNodeTotal->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNodeTotal = pNodeListTotal->Getitem(0);
							pNodeMapTotal = pNodeTotal->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
						
						}
						break;
					case DF_MEDIUM:
						if(!defect_info.black_white.Compare(_T("B")))
						{
							pNode2 = pNodeListStep_m->Getitem(idx + 1);
							pNodeMap2 = pNode2->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap2->Getitem(0)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMap2->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;
							
							pNode2 = pNodeListStep_m->Getitem(0);
							pNodeMap2 = pNode2->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap2->Getitem(0)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMap2->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNodeTotal = pNodeListTotal->Getitem(idx + 1);
							pNodeMapTotal = pNodeTotal->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNodeTotal = pNodeListTotal->Getitem(0);
							pNodeMapTotal = pNodeTotal->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
						}
						else
						{
							pNode2 = pNodeListStep_m->Getitem(idx + 1);
							pNodeMap2 = pNode2->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap2->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMap2->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;
							
							pNode2 = pNodeListStep_m->Getitem(0);
							pNodeMap2 = pNode2->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap2->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMap2->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNodeTotal = pNodeListTotal->Getitem(idx + 1);
							pNodeMapTotal = pNodeTotal->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNodeTotal = pNodeListTotal->Getitem(0);
							pNodeMapTotal = pNodeTotal->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
						
						}
						break;
					case DF_LARGE:
						if(!defect_info.black_white.Compare(_T("B")))
						{
							pNode2 = pNodeListStep_l->Getitem(idx + 1);
							pNodeMap2 = pNode2->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap2->Getitem(0)->GetnodeValue().bstrVal);
							test.Format(_T("%s"),(LPCTSTR)pNodeMap2->Getitem(0)->GetbaseName());
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMap2->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;
							
							pNode2 = pNodeListStep_l->Getitem(0);
							pNodeMap2 = pNode2->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap2->Getitem(0)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMap2->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNodeTotal = pNodeListTotal->Getitem(idx + 1);
							pNodeMapTotal = pNodeTotal->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNodeTotal = pNodeListTotal->Getitem(0);
							pNodeMapTotal = pNodeTotal->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
						}
						else
						{
							pNode2 = pNodeListStep_l->Getitem(idx + 1);
							pNodeMap2 = pNode2->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap2->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMap2->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;
							
							pNode2 = pNodeListStep_l->Getitem(0);
							pNodeMap2 = pNode2->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap2->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMap2->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNodeTotal = pNodeListTotal->Getitem(idx + 1);
							pNodeMapTotal = pNodeTotal->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNodeTotal = pNodeListTotal->Getitem(0);
							pNodeMapTotal = pNodeTotal->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
						
						}
						break;
					case DF_OVERFLOW:
						if(!defect_info.black_white.Compare(_T("B")))
						{
							pNode2 = pNodeListStep_o->Getitem(idx + 1);
							pNodeMap2 = pNode2->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap2->Getitem(0)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMap2->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;
							
							pNode2 = pNodeListStep_o->Getitem(0);
							pNodeMap2 = pNode2->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap2->Getitem(0)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMap2->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNodeTotal = pNodeListTotal->Getitem(idx + 1);
							pNodeMapTotal = pNodeTotal->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNodeTotal = pNodeListTotal->Getitem(0);
							pNodeMapTotal = pNodeTotal->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
						}
						else
						{
							pNode2 = pNodeListStep_o->Getitem(idx + 1);
							pNodeMap2 = pNode2->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap2->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMap2->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;
							
							pNode2 = pNodeListStep_o->Getitem(0);
							pNodeMap2 = pNode2->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMap2->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMap2->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNodeTotal = pNodeListTotal->Getitem(idx + 1);
							pNodeMapTotal = pNodeTotal->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
							dcount_temp = 0;

							pNodeTotal = pNodeListTotal->Getitem(0);
							pNodeMapTotal = pNodeTotal->Getattributes();
							count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
							dcount_temp = _wtoi(count_temp);
							dcount_temp += 1;
							pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
						
						}
						break;
					}
					//vbSuccessful = m_pDoc2->save((_variant_t)filepath);

					bDefect_update_new = TRUE;
					bUpdatebreak = TRUE;
					break;
				}
			}
			countatti++;
		}
		countatti = 0;
		count++;
		if(count == length)
		{

		}
		if(bUpdatebreak)
			break;
	}
	if(!bDefect_update_new)			//새로 검출된 디펙일 경우
	{
		MSXML2::IXMLDOMNodeListPtr pNodeListNew = NULL;
		MSXML2::IXMLDOMNodeListPtr pNodeListNew2 = NULL;
		MSXML2::IXMLDOMElementPtr	pAddElement = NULL;
		MSXML2::IXMLDOMElementPtr	pSearchElement = NULL;
		MSXML2::IXMLDOMElementPtr	pSearchElement2 = NULL;
		MSXML2::IXMLDOMElementPtr	pSearchElement3 = NULL;
		MSXML2::IXMLDOMAttributePtr pAttribute = NULL;
		MSXML2::IXMLDOMNodePtr		pNodeNew = NULL;

		MSXML2::IXMLDOMNodePtr		pNodeCum = NULL;
		MSXML2::IXMLDOMNamedNodeMapPtr pNodeMapCum = NULL;

		MSXML2::IXMLDOMNamedNodeMapPtr pNodeMapNew = NULL;

		CString newdefect,searchnewdefect,defectinfo,searchdefectinfo,strtemp,nodetemp;
		long temp = 0;
		defectinfo.Format(_T("//DEFECT_INFO"));
	//	searchdefectinfo.Format(_T("DEFECT_INFO"));
		searchnewdefect.Format(_T("//DEFECT"));
		newdefect.Format(_T("DEFECT"));
		
		if(defect_info.cell_id.GetLength() > 0)
		{

			pNodeListCum_s = m_pDoc2->selectNodes(L"//CUM_S");
			pNodeListCum_m = m_pDoc2->selectNodes(L"//CUM_M");
			pNodeListCum_l = m_pDoc2->selectNodes(L"//CUM_L");
			pNodeListCum_o = m_pDoc2->selectNodes(L"//CUM_O");

			

			pAddElement = m_pDoc2->createElement((_bstr_t)newdefect);
			pNodeListNew2 = m_pDoc2->selectNodes((_bstr_t)defectinfo);//->Getitem(idx)->appendChild(pAddElement);
			pSearchElement3 = pNodeListNew2->Getitem(idx);//->appendChild(pAddElement);
			pSearchElement3->appendChild(pAddElement);

			pSearchElement = m_pDoc2->selectNodes((_bstr_t)defectinfo)->Getitem(idx);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("SHOP"));
			pAttribute->Putvalue((_variant_t)_T(""));
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("DEF_NO"));
	//		pNodeNew = pSearchElement->GetlastChild();
	//		pNodeMapNew = pNodeNew->Getattributes();
	//		nodetemp.Format(_T("%s"),(LPCTSTR)pNodeMapNew->Getitem(1)->GetnodeValue().bstrVal);
			pNodeListNew = pSearchElement->selectNodes((_bstr_t)newdefect);
			temp = pNodeListNew->Getlength();
			//temp = temp + 1;
			pAttribute->Putvalue((_variant_t)temp);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("GLS_START_TIME"));
			pAttribute->Putvalue((_variant_t)time);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("POINT_X"));
			pAttribute->Putvalue((_variant_t)defect_info.point_x);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("POINT_Y"));
			pAttribute->Putvalue((_variant_t)defect_info.point_y);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("ORIGIN_POINT_X"));
			pAttribute->Putvalue((_variant_t)defect_info.origin_point_x);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("ORIGIN_POINT_Y"));
			pAttribute->Putvalue((_variant_t)defect_info.origin_point_y);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("ACTIVE_AREA"));
			pAttribute->Putvalue((_variant_t)defect_info.active_area);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("XPIXEL"));
			pAttribute->Putvalue((_variant_t)defect_info.xpixel);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("YPIXEL"));
			pAttribute->Putvalue((_variant_t)defect_info.ypixel);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("PIXEL"));
			pAttribute->Putvalue((_variant_t)defect_info.pixel);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("XSIZE"));
			pAttribute->Putvalue((_variant_t)defect_info.xsize);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("YSIZE"));
			pAttribute->Putvalue((_variant_t)defect_info.ysize);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("SIZE"));
			pAttribute->Putvalue((_variant_t)defect_info.size);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("RANK"));
			pAttribute->Putvalue((_variant_t)defect_info.rank);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("MODE"));
			pAttribute->Putvalue((_variant_t)defect_info.mode);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("PROC_ID"));
			pAttribute->Putvalue((_variant_t)defect_info.proc_id);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("UNIT_ID"));
			pAttribute->Putvalue((_variant_t)defect_info.unit_id);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("PTN_CODE"));
			pAttribute->Putvalue((_variant_t)defect_info.ptn_code);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("OPERATOR_ID"));
			pAttribute->Putvalue((_variant_t)defect_info.operator_id);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("REVIEW_X_SIZE"));
			pAttribute->Putvalue((_variant_t)defect_info.review_x_size);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("REVIEW_Y_SIZE"));
			pAttribute->Putvalue((_variant_t)defect_info.review_y_size);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("REVIEW_LAYER"));
			pAttribute->Putvalue((_variant_t)defect_info.review_layer);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("PXL_ZONE"));
			pAttribute->Putvalue((_variant_t)defect_info.pxl_zone);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("ENCAP_TYPE"));
			pAttribute->Putvalue((_variant_t)defect_info.encap_type);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("ZONE"));
			pAttribute->Putvalue((_variant_t)defect_info.zone);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("EI_RPC_OP"));
			pAttribute->Putvalue((_variant_t)defect_info.ei_rpc_op);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			pAttribute = m_pDoc2->createAttribute((_bstr_t)_T("IMAGE_FILE_NO"));
			pAttribute->Putvalue((_variant_t)defect_info.image_file_no);
			pSearchElement2 = pSearchElement->GetlastChild();
			pSearchElement2->setAttributeNode(pAttribute);

			int size_type = 0;
			if(!defect_info.rank.Compare(_T("S")))
				size_type = DF_SMALL;
			else if(!defect_info.rank.Compare(_T("M")))
				size_type = DF_MEDIUM;
			else if(!defect_info.rank.Compare(_T("L")))
				size_type = DF_LARGE;
			else if(!defect_info.rank.Compare(_T("OL")))
				size_type = DF_OVERFLOW;
			else
			{
				strlog.Format(_T("DFPanelUpdate() rank : size error "));
			}


			switch (size_type)
			{
			case DF_SMALL:
				if(!defect_info.black_white.Compare(_T("B")))
				{
					pNodeCum = pNodeListCum_s->Getitem(idx + 1);
					pNodeMapCum = pNodeCum->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapCum->Getitem(0)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapCum->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeCum = pNodeListCum_s->Getitem(0);
					pNodeMapCum = pNodeCum->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapCum->Getitem(0)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapCum->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(idx + 1);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(0);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(0)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(0);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(2)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(2)->PutnodeValue((_variant_t)dcount_temp);
				}
				else
				{
					pNodeCum = pNodeListCum_s->Getitem(idx + 1);
					pNodeMapCum = pNodeCum->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapCum->Getitem(1)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapCum->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeCum = pNodeListCum_s->Getitem(0);
					pNodeMapCum = pNodeCum->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapCum->Getitem(1)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapCum->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(idx + 1);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(0);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(0)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(0);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(2)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(2)->PutnodeValue((_variant_t)dcount_temp);
						
				}
				break;
			case DF_MEDIUM:
				if(!defect_info.black_white.Compare(_T("B")))
				{
					pNodeCum = pNodeListCum_m->Getitem(idx + 1);
					pNodeMapCum = pNodeCum->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapCum->Getitem(0)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapCum->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;
							
					pNodeCum = pNodeListCum_m->Getitem(0);
					pNodeMapCum = pNodeCum->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapCum->Getitem(0)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapCum->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(idx + 1);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(0);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(0)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(0);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(2)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(2)->PutnodeValue((_variant_t)dcount_temp);
				}
				else
				{
					pNodeCum = pNodeListCum_m->Getitem(idx + 1);
					pNodeMapCum = pNodeCum->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapCum->Getitem(1)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapCum->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;
							
					pNodeCum = pNodeListCum_m->Getitem(0);
					pNodeMapCum = pNodeCum->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapCum->Getitem(1)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapCum->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(idx + 1);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(0);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(0)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(0);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(2)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(2)->PutnodeValue((_variant_t)dcount_temp);
						
				}
				break;
			case DF_LARGE:
				if(!defect_info.black_white.Compare(_T("B")))
				{
					pNodeCum = pNodeListCum_l->Getitem(idx + 1);
					pNodeMapCum = pNodeCum->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapCum->Getitem(0)->GetnodeValue().bstrVal);
					test.Format(_T("%s"),(LPCTSTR)pNodeMapCum->Getitem(0)->GetbaseName());
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapCum->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;
							
					pNodeCum = pNodeListCum_l->Getitem(0);
					pNodeMapCum = pNodeCum->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapCum->Getitem(0)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapCum->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(idx + 1);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(0);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(0)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(0);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(2)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(2)->PutnodeValue((_variant_t)dcount_temp);
				}
				else
				{
					pNodeCum = pNodeListCum_l->Getitem(idx + 1);
					pNodeMapCum = pNodeCum->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapCum->Getitem(1)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapCum->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;
							
					pNodeCum = pNodeListCum_l->Getitem(0);
					pNodeMapCum = pNodeCum->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapCum->Getitem(1)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapCum->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(idx + 1);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(0);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(0)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(0);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(2)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(2)->PutnodeValue((_variant_t)dcount_temp);
						
				}
				break;
			case DF_OVERFLOW:
				if(!defect_info.black_white.Compare(_T("B")))
				{
					pNodeCum = pNodeListCum_o->Getitem(idx + 1);
					pNodeMapCum = pNodeCum->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapCum->Getitem(0)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapCum->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;
							
					pNodeCum = pNodeListCum_o->Getitem(0);
					pNodeMapCum = pNodeCum->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapCum->Getitem(0)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapCum->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(idx + 1);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(0);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(0)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(0);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(2)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(2)->PutnodeValue((_variant_t)dcount_temp);
				}
				else
				{
					pNodeCum = pNodeListCum_o->Getitem(idx + 1);
					pNodeMapCum = pNodeCum->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapCum->Getitem(1)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapCum->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;
							
					pNodeCum = pNodeListCum_o->Getitem(0);
					pNodeMapCum = pNodeCum->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapCum->Getitem(1)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapCum->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(idx + 1);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(1)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(1)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(0);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(0)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(0)->PutnodeValue((_variant_t)dcount_temp);
					dcount_temp = 0;

					pNodeTotal = pNodeListTotal->Getitem(0);
					pNodeMapTotal = pNodeTotal->Getattributes();
					count_temp.Format(_T("%s"),(LPCTSTR)pNodeMapTotal->Getitem(2)->GetnodeValue().bstrVal);
					dcount_temp = _wtoi(count_temp);
					dcount_temp += 1;
					pNodeMapTotal->Getitem(2)->PutnodeValue((_variant_t)dcount_temp);
						
				}
				break;
			}

			//vbSuccessful = m_pDoc2->save((_variant_t)filepath);
		}
		
	}

	return bDefect_update_new;
}

BOOL CXml::GlassInfoUpdate(CString defectfilename,CString keyid,CString lotid,CString cstid,CString mo_gls_id,CString half_gls_id,CString cf_id,CString cut,CString slot,
	CString eqp,CString machine,CString unit,CString oper,CString proc,CString recipe,CString ins_time_start,CString ins_time_end)
{
	try{
		MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
		MSXML2::IXMLDOMNodePtr		pNode = NULL;
		MSXML2::IXMLDOMElementPtr	pElement = NULL;
		MSXML2::IXMLDOMAttributePtr pAttribute = NULL;
		MSXML2::IXMLDOMNamedNodeMapPtr pNodeMap = NULL;

		pNode = m_pDoc2->selectSingleNode(L"//DEFECT_FILE");	
		pNodeMap = pNode->Getattributes();
		pNodeMap->Getitem(0)->PutnodeValue((_variant_t)defectfilename);

		pElement = m_pDoc2->selectNodes(L"//KEY_ID")->Getitem(0);	
		pElement->Puttext((_bstr_t)keyid);

		pElement = m_pDoc2->selectNodes(L"//LOT_ID")->Getitem(0);	
		pElement->Puttext((_bstr_t)lotid);

		pElement = m_pDoc2->selectNodes(L"//CST_ID")->Getitem(0);	
		pElement->Puttext((_bstr_t)cstid);

		pElement = m_pDoc2->selectNodes(L"//MO_GLS_ID")->Getitem(0);	
		pElement->Puttext((_bstr_t)mo_gls_id);

		pElement = m_pDoc2->selectNodes(L"//HALF_GLS_ID")->Getitem(0);	
		pElement->Puttext((_bstr_t)half_gls_id);

		pElement = m_pDoc2->selectNodes(L"//CF_ID")->Getitem(0);	
		pElement->Puttext((_bstr_t)cf_id);

		pElement = m_pDoc2->selectNodes(L"//GLS_CUT")->Getitem(0);	
		pElement->Puttext((_bstr_t)cut);

		pElement = m_pDoc2->selectNodes(L"//SLOT_NO")->Getitem(0);	
		pElement->Puttext((_bstr_t)slot);

		pElement = m_pDoc2->selectNodes(L"//EQP_ID")->Getitem(0);	
		pElement->Puttext((_bstr_t)eqp);

		pElement = m_pDoc2->selectNodes(L"//MACHINE_ID")->Getitem(0);	
		pElement->Puttext((_bstr_t)machine);

		pElement = m_pDoc2->selectNodes(L"//UNIT_ID")->Getitem(0);	
		pElement->Puttext((_bstr_t)unit);

		pElement = m_pDoc2->selectNodes(L"//OPER_ID")->Getitem(0);	
		pElement->Puttext((_bstr_t)oper);

		pElement = m_pDoc2->selectNodes(L"//PROC_ID")->Getitem(0);	
		pElement->Puttext((_bstr_t)proc);

		pElement = m_pDoc2->selectNodes(L"//RECIPE_ID")->Getitem(0);	
		pElement->Puttext((_bstr_t)recipe);

		pNode = m_pDoc2->selectSingleNode(L"//INSP_TIME");	
		pNodeMap = pNode->Getattributes();
		pNodeMap->Getitem(0)->PutnodeValue((_variant_t)ins_time_start);

		pNode = m_pDoc2->selectSingleNode(L"//INSP_TIME");	
		pNodeMap = pNode->Getattributes();
		pNodeMap->Getitem(1)->PutnodeValue((_variant_t)ins_time_end);
		return TRUE;
	}
	catch( _com_error e )
	{
		TRACE( _T("CXml::Xml_Format_Body( ) fail:%s\n"), e.ErrorMessage());
		return FALSE;
	}
	
}


void CXml::Update_Defect_CountInit(CString filepath)
{
	MSXML2::IXMLDOMDocument2Ptr pDoc = NULL;
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	MSXML2::IXMLDOMNodeListPtr pNodeList2 = NULL;
	MSXML2::IXMLDOMNodePtr		pNode = NULL;	
	MSXML2::IXMLDOMNamedNodeMapPtr pNodeMap = NULL;
	MSXML2::IXMLDOMElementPtr	pElement = NULL;
	MSXML2::IXMLDOMAttributePtr pAttribute = NULL;

	HRESULT hr = S_FALSE;
	VARIANT_BOOL vbSuccessful = VARIANT_TRUE;
	long count = 0,length = 0,attrcount = 0,attrlength = 0;
	int init = 0,strcount = 0;
	CString nodename,basename,fullstr,substr;

	fullstr.Format(_T("//TOTAL,//STEP_S,//STEP_M,//STEP_L,//STEP_O"));
	hr = pDoc.CreateInstance(__uuidof(DOMDocument)); 
	vbSuccessful = pDoc->load((_variant_t)filepath);

	while(AfxExtractSubString(substr, fullstr, strcount, ','))
	{
		pNodeList = pDoc->selectNodes((_bstr_t)substr);
		length = pNodeList->Getlength();
		while(count < length)
		{
			pNode = pNodeList->Getitem(count);
			pNodeMap = pNode->Getattributes();
			attrlength = pNodeMap->Getlength();
			while(attrcount < attrlength)
			{
				basename.Format(_T("%s"),(LPCTSTR)pNodeMap->Getitem(attrcount)->GetbaseName());
				if(!basename.Compare(_T("STEP"))){
					pNodeMap->Getitem(attrcount)->PutnodeValue((_variant_t)init);
				}
				if(!basename.Compare(_T("NEW"))){
					pNodeMap->Getitem(attrcount)->PutnodeValue((_variant_t)init);
				}
				if(!basename.Compare(_T("RB"))){
					pNodeMap->Getitem(attrcount)->PutnodeValue((_variant_t)init);
				}
				if(!basename.Compare(_T("RW"))){
					pNodeMap->Getitem(attrcount)->PutnodeValue((_variant_t)init);
				}
				attrcount++;
			}
			attrcount = 0;
			count++;
		}
		count = 0;
		strcount++;
	}

	vbSuccessful = pDoc->save((_variant_t)filepath);
}
//typeheader_e		m_header_e

typedefectdata CXml::SetDefectData(char *glsid,char *time,char *index,char *cellid,double measure_x_e2,double measure_y_e2,double measure_x_ap2,double measure_y_ap2,double size_long,double width,
		double height,double area,char *size_range,char *rank,char *in_out,double gray_min,double gray_25,double gray_mediean,double gray_75,double gray_max,double gray_mean,double gray_deviation,char *black_white,char *image_path)
{
	typedefectdata defectdata;

	defectdata.glsid.Format(_T("%S"),glsid);
	defectdata.time.Format(_T("%S"),time);
	defectdata.index.Format(_T("%S"),index);
	defectdata.cellid.Format(_T("%S"),cellid);
	defectdata.measure_x_e2 = measure_x_e2;
	defectdata.measure_y_e2 = measure_y_e2;
	defectdata.measure_x_ap2 = measure_x_ap2;
	defectdata.measure_y_ap2 = measure_y_ap2;
	defectdata.size_long = size_long;
	defectdata.width = width;
	defectdata.height = height;
	defectdata.area = area;
	defectdata.size_range.Format(_T("%S"),size_range);
	defectdata.rank.Format(_T("%S"),rank);
	defectdata.in_out.Format(_T("%S"),in_out);
	defectdata.gray_min = gray_min;
	defectdata.gray_25 = gray_25;
	defectdata.gray_mediean = gray_mediean;
	defectdata.gray_75 = gray_75;
	defectdata.gray_max = gray_max;
	defectdata.gray_deviation = gray_deviation;
	defectdata.black_white.Format(_T("%S"),black_white);
	defectdata.image_path.Format(_T("%S"),image_path);
	return defectdata;
}

