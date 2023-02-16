#ifndef __PMAC_H__
#define __PMAC_H__

#pragma warning (disable:4786)
#include <map>
using namespace std;

typedef struct {
	CString strAddress;
	BOOL bValue;
	int nValue;
} CMD_SET;
typedef map<UINT, CMD_SET> CMD_SET_LIST;

class CPMAC 
{
public:
	virtual ~CPMAC() { };
	virtual BOOL	Init() = 0;
	virtual void	Exit() = 0;
	virtual int		GetError() = 0;
	virtual BOOL	GetErrorStr(CString &strError) = 0;
	virtual BOOL	SetPmac(CString strCommand) = 0;
	virtual BOOL	GetPmac(CString& strResponse, CString strCommand) = 0;
	virtual BOOL	SetPmacDataInt(CString strName, int nData) = 0;
	virtual BOOL	GetPmacDataInt(CString strName, int *nData) = 0;
	virtual BOOL	GetPmacDataInt(const char *szAddress[], int *nData, int nSize) = 0;
	virtual BOOL	GetPmacDataInt(CMD_SET_LIST *lstCMDSET) = 0;
};

#endif //__PMAC_H__