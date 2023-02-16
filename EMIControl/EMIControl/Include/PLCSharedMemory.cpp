#include "stdafx.h"
#include "PLCSharedMemory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define BIT_COUNT_PER_WORD	16
#define MEMMAP_OPEN_TIMEOUT	1000

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPLCSharedMemory::CPLCSharedMemory()
{
	InitializeCriticalSection(&m_csPLC);
	m_MelsecErr = 0;
}

CPLCSharedMemory::~CPLCSharedMemory()
{
	DeleteCriticalSection(&m_csPLC);
}

BOOL CPLCSharedMemory::Init()
{
	return TRUE;
}

void CPLCSharedMemory::Exit()
{
	std::map< int, std::map<int,MEMMAP> >::iterator M1;
	std::map<int,MEMMAP>::iterator M2;
	for(M1=m_MemMaps.begin(); M1!=m_MemMaps.end(); M1++) {
		for(M2=M1->second.begin(); M2!=M1->second.end(); M2++) {
			MEMMAP& map = M2->second;
			map.pMemMap->UnMap();
			delete map.pMemMap;
		}
	}
	m_MemMaps.clear();

	std::list<ADDRESS_MAP>::iterator I;
	for(I=m_AddressMap.begin(); I!=m_AddressMap.end(); I++)
		delete (*I).pData;

	m_AddressMap.clear();
}


void CPLCSharedMemory::AddAddress(int nDevType, int nStartAddress, int nEndAddress)
{
	int nDataSize = nEndAddress-nStartAddress+1;
	int nBitPerData = 1;
	
	switch (nDevType) {
	case DevM:
	case DevB:
	case DevX:
	case DevY:
	case DevL:
		nDataSize /= BIT_COUNT_PER_WORD;
		nDataSize += 1;
		nBitPerData = 1;
		break;

	case DevR:
	case DevD:
	case DevW:
		nBitPerData = BIT_COUNT_PER_WORD;
		break;

	default:
		return;
	}
	
	WORD* pData = new WORD[nDataSize];
	memset(pData, 0, nDataSize);
	
	ADDRESS_MAP addr_map;
	addr_map.nDevType		= nDevType;
	addr_map.nStartAddress	= nStartAddress;
	addr_map.nEndAddress	= nEndAddress;
	addr_map.pData			= pData;
	m_AddressMap.push_back(addr_map);

	int nMemSize = nDataSize*sizeof(WORD);
	MEMMAP &mem_map = m_MemMaps[0][nDevType];
	mem_map.pMemMap = new CMemMapFile;
	CString strMapName;
	strMapName.Format(_T("PLC.MEMORY.%d"), nDevType);
	if(mem_map.pMemMap->MapMemory(strMapName, nMemSize) == FALSE) {
		if(mem_map.pMemMap->MapExistingMemory(strMapName, nMemSize) == FALSE ) {
			return;
		}
	}
	
	mem_map.nStartAddress = nStartAddress;
	mem_map.nMemSize = nMemSize;
	mem_map.nBitPerData = nBitPerData;
}

void CPLCSharedMemory::AddAddressSize(int nDevType, int nAddress, int nSize)
{
	m_AddressSize[nDevType][nAddress] = nSize;
}

int CPLCSharedMemory::GetAddressSize(int nDevType, int nAddress)
{
	return m_AddressSize[nDevType][nAddress];
}

BOOL CPLCSharedMemory::Write(int nDevType, int nStartAddress, int nEndAddress, WORD *pWriteData, int nDataSize)
{
	std::list<ADDRESS_MAP>::iterator I;
	
	short size;

	for(I=m_AddressMap.begin(); I!=m_AddressMap.end(); I++)	{
		ADDRESS_MAP *pAM = &(*I);
		if(pAM->nDevType==nDevType && (pAM->nStartAddress<=nStartAddress) && (nEndAddress<=pAM->nEndAddress)) {
			WORD *pData = pAM->pData;
			switch (pAM->nDevType) {
			case DevM:
			case DevB:
			case DevX:
			case DevY:
			case DevL:
				{
					size = nEndAddress-nStartAddress+1;
					if ( size <= BIT_COUNT_PER_WORD-1 ) {
						size = 1;
					} else {
						size = size/BIT_COUNT_PER_WORD + 1;
					}
					pData = pData + (nStartAddress-pAM->nStartAddress)/BIT_COUNT_PER_WORD;
					nStartAddress = (int(nStartAddress/BIT_COUNT_PER_WORD))*BIT_COUNT_PER_WORD;
					mdSend( 0, 0, nDevType, nStartAddress, &size, pData );
					break;
				}
			case DevR:
			case DevD:
			case DevW:
				{
					pData = pData + nStartAddress-pAM->nStartAddress;
					size = nEndAddress-nStartAddress + 1;
					if (pWriteData)
						memcpy(pData, pWriteData, sizeof(WORD)*nDataSize);
					mdSend( 0, 0, nDevType, nStartAddress, &size, pData );
					break;
				}
		    }
		}
	}
	
	return TRUE;
}

BOOL CPLCSharedMemory::Read(int nDevType, int nStartAddress, int nEndAddress, WORD *pReadData, int nDataSize)
{
	std::list<ADDRESS_MAP>::iterator I;
	
	short size;

	for(I=m_AddressMap.begin(); I!=m_AddressMap.end(); I++)
	{
		ADDRESS_MAP *pAM = &(*I);

		if( pAM->nDevType==nDevType && (pAM->nStartAddress<=nStartAddress) &&  (nEndAddress<=pAM->nEndAddress) )
		{
			WORD *pData = pAM->pData;
			short ret=0;

			switch ( pAM->nDevType ) {
			case DevM:
			case DevB:
			case DevX:
			case DevY:
			case DevL:
				{
					size = nEndAddress-nStartAddress+1;
					if ( size <= BIT_COUNT_PER_WORD-1 ) {
						size = 1;
					} else {
						size = size/BIT_COUNT_PER_WORD + 1;
					}
					pData = pData + (nStartAddress-pAM->nStartAddress)/BIT_COUNT_PER_WORD;
					nStartAddress = (int(nStartAddress/BIT_COUNT_PER_WORD))*BIT_COUNT_PER_WORD;
					ret = mdReceive( 0, 0, nDevType, nStartAddress, &size, pData );
					//Q_ASSERT( ret == 0 );
					break;
				}

			case DevR:
			case DevD:
			case DevW:
				{
					pData = pData + nStartAddress-pAM->nStartAddress;
					size = nEndAddress-nStartAddress + 1;
					ret = mdReceive( 0, 0, nDevType, nStartAddress, &size, pData );
					//Q_ASSERT( ret == 0 );
					if ( pReadData!=NULL ) {
						memcpy( pReadData, pData, sizeof(WORD)*min(nDataSize, size) );
					}
					break;
				}
			}

			return TRUE;
		}
	}

	return FALSE;
}


WORD* CPLCSharedMemory::GetPointer( int nDevType, int nAddress )
{
	int nBytePos = nAddress/BIT_COUNT_PER_WORD;
	
	std::list<ADDRESS_MAP>::iterator I;
	for(I=m_AddressMap.begin(); I!=m_AddressMap.end(); I++) {
		ADDRESS_MAP *pAM = &(*I);
		if( (pAM->nStartAddress <= nAddress) && (nAddress <= pAM->nEndAddress) && ( nDevType == pAM->nDevType) ) {
			WORD *pData = pAM->pData;
			switch( pAM->nDevType ) {
			case DevM:
			case DevB:
			case DevX:
			case DevY:
			case DevL:
				return pData + (nAddress - pAM->nStartAddress)/BIT_COUNT_PER_WORD;

			case DevR:		
			case DevD:
			case DevW:
				return pData + nAddress - pAM->nStartAddress;
			}
		}
	}
	
	return NULL;
}
	
BOOL CPLCSharedMemory::SetBit(int nAddress, BOOL bValue, BOOL bSync)
{
	if(bSync) Read(DevB, nAddress, nAddress);

	EnterCriticalSection(&m_csPLC);
		int nBitPos = nAddress%BIT_COUNT_PER_WORD;
		WORD *pData = GetPointer(DevB, nAddress);
		
		if(bValue)	*pData = *pData | (1<<nBitPos);
		else		*pData = *pData & ~(1<<nBitPos);
	LeaveCriticalSection(&m_csPLC);

	if(bSync) Write(DevB, nAddress, nAddress);

	return TRUE;
}

BOOL CPLCSharedMemory::SetWORD(int nAddress, WORD wValue, BOOL bSync)
{
	if(bSync) Read(DevW, nAddress, nAddress);

	EnterCriticalSection(&m_csPLC);
		WORD* pData = (WORD*) GetPointer(DevW, nAddress);
		*pData = wValue;
	LeaveCriticalSection(&m_csPLC);

	if(bSync) Write(DevW, nAddress, nAddress);
	
	return TRUE;
}

BOOL CPLCSharedMemory::SetWORDS(int nAddress, int nSize, WORD *wValue, BOOL bSync)
{
	if(bSync) Read(DevW, nAddress, nAddress+nSize-1);
	
	EnterCriticalSection(&m_csPLC);
		WORD* pData = (WORD*) GetPointer(DevW, nAddress);
		memcpy(pData, wValue, sizeof(WORD)*nSize);
	LeaveCriticalSection(&m_csPLC);
	
	if(bSync) Write(DevW, nAddress, nAddress+nSize-1);
	
	return TRUE;
}

BOOL CPLCSharedMemory::SetDWORDS(int nAddress, int nSize, DWORD *wValue, BOOL bSync)
{
	if(bSync) Read(DevW, nAddress, nAddress+nSize-1);
	
	EnterCriticalSection(&m_csPLC);
		WORD* pData = (WORD*)GetPointer(DevW, nAddress);
		WORD *wwValue[2];
		WORD dwTemp;
		dwTemp = (*wValue)>>16;
		wwValue[0] = (WORD *)wValue;
		wwValue[1] = (WORD *)&dwTemp;
		memcpy(pData, wwValue, sizeof(DWORD)*nSize);
	LeaveCriticalSection(&m_csPLC);

	if(bSync) Write(DevW, nAddress, nAddress+nSize-1);
	
	return TRUE;
}

BOOL CPLCSharedMemory::GetBit(int nAddress, BOOL bSync)
{
	if(bSync) Read(DevB, nAddress, nAddress);	

	int nBitPos = nAddress%BIT_COUNT_PER_WORD;
	WORD *pData = GetPointer(DevB, nAddress);
	BOOL bRet = ( *pData & (1<<nBitPos) ) > 0 ? TRUE : FALSE;

	return bRet;
}

WORD CPLCSharedMemory::GetWORD(int nAddress, BOOL bSync)
{
	if(bSync) Read(DevW, nAddress, nAddress);

	WORD* pData = GetPointer(DevW, nAddress);
	WORD wRet = *pData;

	return wRet;
}

SHORT CPLCSharedMemory::mdSend( LONG nPath, SHORT nStationNo, SHORT nDeviceType, SHORT nDeviceNo, 
		PSHORT pnWrittenByteSize, LPVOID pwWriteData ) 
{ 
	EnterCriticalSection(&m_csPLC);
	MEMMAP &map = m_MemMaps[0][nDeviceType];
	BYTE* pMem = (BYTE*) map.pMemMap->Open(MEMMAP_OPEN_TIMEOUT);
	if ( pMem != NULL ) {
		int nStartPos = (int)( (float)(nDeviceNo-map.nStartAddress) / ( 16.f / (float)map.nBitPerData ) );
		memcpy( pMem+nStartPos*sizeof(WORD), pwWriteData, sizeof(WORD)*(*pnWrittenByteSize) );
		
		map.pMemMap->Close();
	}

	LeaveCriticalSection(&m_csPLC);
	return 0; 
}

SHORT CPLCSharedMemory::mdReceive( LONG nPath, SHORT nStationNo, SHORT nDeviceType, SHORT nDeviceNo, 
									PSHORT pnReadByteSize, LPVOID pwReadData ) 
{ 
	EnterCriticalSection(&m_csPLC);
	MEMMAP &map = m_MemMaps[0][nDeviceType];
	BYTE* pMem = (BYTE*) map.pMemMap->Open(MEMMAP_OPEN_TIMEOUT);
	if ( pMem != NULL ) {
		int nStartPos = (int)( (float)(nDeviceNo-map.nStartAddress) / ( 16.f / (float)map.nBitPerData ) );
		memcpy( pwReadData, pMem+nStartPos*sizeof(WORD), sizeof(WORD)*(*pnReadByteSize) );
		
		map.pMemMap->Close();
	}

	LeaveCriticalSection(&m_csPLC);
	return 0; 
}


