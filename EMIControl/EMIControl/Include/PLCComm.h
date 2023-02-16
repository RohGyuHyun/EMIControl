#ifndef __PLC_COMM_H__
#define __PLC_COMM_H__

#include "PLC.h"
#include "MDFUNC.H"

#pragma warning (disable:4786)
#include <list>
#include <map>
using namespace std;

class CPLCComm : public CPLC
{
public:
	CPLCComm();
	virtual ~CPLCComm();

	BOOL	Init(int nChannelNo, int nStationNo, int nPlcStation);
	void	Exit();

	void	AddAddress(int nDevType, int nStartAddress, int nEndAddress);
	void	AddAddressSize(int nDevType, int nAddress, int nSize);
	int		GetAddressSize(int nDevType, int nAddress);
	
	BOOL	Read(int nDevType, int nStartAddress, int nEndAddress = -1, WORD *pData = NULL, int nDataSize = 0);
	BOOL	Write(int nDevType, int nStartAddress, int nEndAddress = -1, WORD *pData = NULL, int nDataSize = 0);

	WORD*	GetPointer(int nDevType, int nAddress);

	BOOL	SetBit(int nAddress, BOOL bValue, BOOL bSync=TRUE);
	BOOL	SetWORD(int nAddress, WORD wValue, BOOL bSync=TRUE);
	BOOL	SetWORDS(int nAddress, int nSize, WORD *wValue, BOOL bSync=TRUE);
	BOOL	SetDWORDS(int nAddress, int nSize, DWORD *wValue, BOOL bSync=TRUE);

	BOOL	GetBit(int nAddress, BOOL bSync=TRUE);
	WORD	GetWORD(int nAddress, BOOL bSync=TRUE);
	int		GetMelsecErr() { return m_MelsecErr; }

private:
	int		m_MelsecErr;

// 	BOOL	mdReceive(long nPath, short nStation, short nDevType, short nStartAddress, PSHORT size, LPVOID pData);
// 	BOOL	mdSend(long nPath, short nStation, short nDevType, short nStartAddress, PSHORT size, LPVOID pData);

private:
	short	m_ChannelNo;			// 51(MELSECNET-10/H)
	short	m_StationNo;			// -1
	long 	m_Path;					// Channel No
	short	m_PlcStation;			// 255
	long	m_NetworkNo;			// 0

	BOOL m_bOpened;
	CRITICAL_SECTION m_csPLC;
	
	CRITICAL_SECTION	m_csDevM;
	CRITICAL_SECTION	m_csDevB;
	CRITICAL_SECTION	m_csDevX;
	CRITICAL_SECTION	m_csDevY;
	CRITICAL_SECTION	m_csDevL;
	CRITICAL_SECTION	m_csDevR;
	CRITICAL_SECTION	m_csDevD;
	CRITICAL_SECTION	m_csDevW;

	std::list<ADDRESS_MAP> m_AddressMap;
	std::map< int, std::map<int, int> > m_AddressSize;
};

#endif // __PLC_COMM_H__
