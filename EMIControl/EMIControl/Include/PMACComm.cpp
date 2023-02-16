#include "stdafx.h"
#include "PMACComm.h"

#include "Runtime.h"
#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPMACComm::CPMACComm()
{
	InitializeCriticalSection(&m_csPMAC);
	m_dwDevice		= 0;
	m_bDriverOpen	= FALSE;
	m_hPmacLib		= NULL;
}

CPMACComm::~CPMACComm()
{
	DeleteCriticalSection(&m_csPMAC);
}

int CPMACComm::ParseItems(char *items[], int nr_items, char *string, const char *separators)
{
	char *token;
	int item_count=0;
	token = strtok(string, separators);
	while (token != NULL) {
		items[item_count++]=token;	
		token = strtok(NULL, separators);
	}
	if(item_count<=0) return 0;
	if ((token = strchr(items[item_count-1], '\r')) != NULL) {
		*token='\0';
	}
	if ((token = strchr(items[item_count-1], '\n')) != NULL) {
		*token='\0';
	}

	return item_count;
}

BOOL CPMACComm::Init()
{
	if (!m_hPmacLib) m_hPmacLib = OpenRuntimeLink();

	if (!m_hPmacLib) return FALSE;

	m_bDriverOpen = DeviceOpen(m_dwDevice);

	if (!m_bDriverOpen)	return FALSE;

	return TRUE;
}

void CPMACComm::Exit()
{
	CloseRuntimeLink();
}

int CPMACComm::GetError()
{
	return DeviceGetError(m_dwDevice);
}

BOOL CPMACComm::GetErrorStr(CString &strError)
{
	char szBuffer[MAX_PATH];
	int nLength = DeviceGetErrorStr(m_dwDevice, szBuffer, MAX_PATH);
	if (nLength > 0) {
		strError = szBuffer;
		return TRUE;
	}

	m_bDriverOpen = 0;
	return FALSE;
}

BOOL CPMACComm::SetPmac(CString strCommand)
{
	char temp[2000];

	if (!m_bDriverOpen) return FALSE;
	
	char szBuffer[2000];
	//long lResponse = DeviceGetResponseEx(m_dwDevice, szBuffer, MAX_PATH, (char*)(LPCTSTR)strCommand); /*Sleep(5);*/
	size_t getval = 0;
	wcstombs_s(&getval, temp, strCommand.GetLength() + 1, strCommand, _TRUNCATE);
	long lResponse = DeviceGetResponseEx(m_dwDevice, szBuffer, 2000, temp); /*Sleep(5);*/

	if (IS_COMM_EOT(lResponse)) return TRUE;
	return FALSE;	
}

BOOL CPMACComm::GetPmac(CString &strResponse, CString strCommand)
{
	char temp[2000];

	if (!m_bDriverOpen) return FALSE;

	char szBuffer[2000];
	//long lResponse = DeviceGetResponseEx(m_dwDevice, szBuffer, MAX_PATH, (char*)(LPCTSTR)strCommand); Sleep(5);
	size_t getval = 0;
	wcstombs_s(&getval, temp, strCommand.GetLength() + 1, strCommand, _TRUNCATE);
	long lResponse = DeviceGetResponseEx(m_dwDevice, szBuffer, 2000, temp); Sleep(5);

	if ( IS_COMM_ANYERROR(lResponse) == 0) {
		strResponse = szBuffer;
		return TRUE;	
	}
	
	m_bDriverOpen = 0;
	return FALSE;
}

/*
BOOL CPMACComm::GetPmac(CString &strResponse, CString strCommand)
{
	if (!m_bDriverOpen) 
	{
		if(!RetryConnect())
		{
			logmsg(LT_ERROR,"GetPmac::!m_bDriverOpen");
			return FALSE;
		}
	}

	char szBuffer[MAX_PATH] = {0,};
	long lResponse = DeviceGetResponseEx(m_dwDevice, szBuffer, MAX_PATH, (char*)(LPCTSTR)strCommand); Sleep(5);

	if ( IS_COMM_ANYERROR(lResponse) == 0) {
		strResponse = szBuffer;
		return TRUE;	
	}

	if(RetryConnect())
	{
		lResponse = DeviceGetResponseEx(m_dwDevice, szBuffer, MAX_PATH, (char*)(LPCTSTR)strCommand); Sleep(5);
		
		if ( IS_COMM_ANYERROR(lResponse) == 0) {
			strResponse = szBuffer;
			return TRUE;	
		}
	}
	
	logmsg(LT_ERROR,"GetPmac::DeviceGetResponseEx %s:%s,Errorcode :%d",strCommand,szBuffer,lResponse);
	m_bDriverOpen = 0;
	return FALSE;
}
*/
BOOL CPMACComm::SetPmacDataInt(CString strAddress, int nData)
{
	CString strCommand;
	strCommand.Format(_T("%s=%d"), strAddress, nData);
	if(!SetPmac(strCommand)) return FALSE;
	return TRUE;
}

//LSS 20151021
int CPMACComm::GetPmacDataInt(CString strAddress, int *nData)
{
	CString strCommand, strReponse;
	char temp[DAT_STR_LENG];

	strCommand.Format(_T("%s"), strAddress);
	if(!GetPmac(strReponse, strCommand)) return FALSE;

	size_t getval = 0;
	wcstombs_s(&getval, temp, strReponse.GetLength() + 1, strReponse, _TRUNCATE);

	*nData = atoi(temp);
	return TRUE;
}

//LSS 20151021
BOOL CPMACComm::GetPmacDataInt(const char *szAddress[], int *nData, int nSize)
{
	char temp[DAT_STR_LENG];
	int i=0;
	const int MAX_ITEMS = 33;
	if(nSize>MAX_ITEMS) nSize = MAX_ITEMS;

	CString strCommand=_T(""), strReponse=_T("");
	for(i=0; i<nSize; i++) {
		strCommand += szAddress[i];
		strCommand += " ";
	}
	if(!GetPmac(strReponse, strCommand)) return FALSE;

	char *pItem[MAX_ITEMS];
	CString strTemp = _T("");
	int nItems;

	size_t getval = 0;
	wcstombs_s(&getval, temp, strReponse.GetLength() + 1, strReponse, _TRUNCATE);

	nItems = ParseItems(pItem, MAX_ITEMS, temp, "\r");
	for(i=0;i<nItems;i++) {
		nData[i] = atoi(pItem[i]);
	}

	return TRUE;
}

BOOL CPMACComm::GetPmacDataInt(CMD_SET_LIST *lstCMDSET)
{
	int i=0;
	const int MAX_ITEMS = 100;
	char temp[2000];

	CString strCommand=_T("");
	CMD_SET_LIST::iterator CSL;
	for(CSL=lstCMDSET->begin(); CSL!=lstCMDSET->end(); CSL++) {
		CMD_SET& cmdset = CSL->second;
		strCommand += cmdset.strAddress;
		strCommand += " ";
	}

	CString strReponse;
	if(!GetPmac(strReponse, strCommand)) return FALSE;

	char *pItem[MAX_ITEMS];
	int nItems;

	size_t getval = 0;
	wcstombs_s(&getval, temp, strReponse.GetLength() + 1, strReponse, _TRUNCATE);

	nItems = ParseItems(pItem, MAX_ITEMS, temp, "\r");
	for(CSL=lstCMDSET->begin(), i=0; CSL!=lstCMDSET->end(); CSL++, i++) {
		if(i>=nItems) break;
		CMD_SET& cmdset = CSL->second;
		cmdset.nValue = atoi(pItem[i]);
	}

	return TRUE;
}

/*
BOOL CPMACComm::GetPmacDataInt(CMD_SET_LIST *lstCMDSET)
{
	int i=0;
	const int MAX_ITEMS = 72;

	CString strCommand="";
	CMD_SET_LIST::iterator CSL;
	for(CSL=lstCMDSET->begin(); CSL!=lstCMDSET->end(); CSL++) {
		CMD_SET& cmdset = CSL->second;
		strCommand += cmdset.strAddress;
		strCommand += " ";
	}

	CString strReponse;
	if(!GetPmac(strReponse, strCommand)) return FALSE;

	char *pItem[MAX_ITEMS];
	int nItems = ParseItems(pItem, MAX_ITEMS, strReponse.GetBuffer(0), "\r");
	for(CSL=lstCMDSET->begin(), i=0; CSL!=lstCMDSET->end(); CSL++, i++) {
		if(i>=nItems) break;
		CMD_SET& cmdset = CSL->second;
		cmdset.nValue = atoi(pItem[i]);
	}

	return TRUE;
}
*/