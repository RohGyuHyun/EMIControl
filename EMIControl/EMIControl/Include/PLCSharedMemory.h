#ifndef __PLC_SHARED_MEMORY_H__
#define __PLC_SHARED_MEMORY_H__

#include "PLC.h"

#include "Memmap.h"
#include "MDFUNC.H"

#pragma warning (disable:4786)
#include <list>
#include <map>
using namespace std;

class CPLCSharedMemory : public CPLC
{
public:
	CPLCSharedMemory();
	virtual ~CPLCSharedMemory();

	BOOL	Init();
	void	Exit();
	
	void	AddAddress(int nDevType, int nStartAddress, int nEndAddress);
	void	AddAddressSize(int nDevType, int nAddress, int nSize);
	int		GetAddressSize(int nDevType, int nAddress);
	
	BOOL	Read(int nDevType, int nStartAddress, int nEndAddress = -1, WORD *pData = NULL, int nDataSize = 0);
	BOOL	Write(int nDevType, int nStartAddress, int nEndAddress = -1, WORD *pWriteData = NULL, int nDataSize = 0);

	WORD*	GetPointer(int nDevType, int nAddress);
	
	BOOL	SetBit(int nAddress, BOOL bValue, BOOL bSync=TRUE);
	BOOL	SetWORD(int nAddress, WORD wValue, BOOL bSync=TRUE);
	BOOL	SetWORDS(int nAddress, int nSize, WORD *wValue, BOOL bSync=TRUE);
	BOOL	SetDWORDS(int nAddress, int nSize, DWORD *wValue, BOOL bSync=TRUE);

	BOOL	GetBit(int nAddress, BOOL bSync=TRUE);
	WORD	GetWORD(int nAddress, BOOL bSync=TRUE);
	int		GetMelsecErr() { return m_MelsecErr; }	
	
private:	
	SHORT mdSend(LONG nPath, SHORT nStationNo, SHORT nDeviceType, SHORT nDeviceNo, 
					PSHORT pnWrittenByteSize, LPVOID pwWriteData);
	SHORT mdReceive(LONG nPath, SHORT nStationNo, SHORT nDeviceType, SHORT nDeviceNo, 
					PSHORT pReadByteSize, LPVOID pwReadData);
	
private:
	BOOL m_bOpened;
	int  m_MelsecErr;
	CRITICAL_SECTION m_csPLC;

	std::list<ADDRESS_MAP> m_AddressMap;
	std::map< int, std::map<int, int> > m_AddressSize;

	typedef struct {
		CMemMapFile *pMemMap;
		int nStartAddress, nMemSize, nBitPerData;
	} MEMMAP;
	std::map< int, std::map<int,MEMMAP> > m_MemMaps;
};

#endif