#ifndef __PMAC_COMM_H__
#define __PMAC_COMM_H__

#include "PMAC.h"

class CPMACComm : public CPMAC
{
public:
	CPMACComm();
	virtual ~CPMACComm();

	BOOL	Init();
	void	Exit();
	int		GetError();
	BOOL	GetErrorStr(CString &strError);
	BOOL	SetPmac(CString strCommand);
	BOOL	GetPmac(CString& strResponse, CString strCommand);
	BOOL	SetPmacDataInt(CString strAddress, int nData);
	BOOL	GetPmacDataInt(CString strAddress, int *nData);
	BOOL	GetPmacDataInt(const char *szAddress[], int *nData, int nSize);
	BOOL	GetPmacDataInt(CMD_SET_LIST *lstCMDSET);

public:
	int		ParseItems(char *items[], int nr_items, char *string, const char *separators);

public:
	CRITICAL_SECTION m_csPMAC;
	
	DWORD		m_dwDevice;
	BOOL		m_bDriverOpen;
	HINSTANCE	m_hPmacLib;
};

#endif // __PMAC_COMM_H__
