#include "stdafx.h"
//#include "../../ACPIMCSFunctions.h"
#include "PLCComm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define BIT_COUNT_PER_WORD	16

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPLCComm::CPLCComm()
{
	InitializeCriticalSection(&m_csPLC);
	InitializeCriticalSection(&m_csDevB);
	InitializeCriticalSection(&m_csDevM);
	InitializeCriticalSection(&m_csDevL);
	InitializeCriticalSection(&m_csDevY);
	InitializeCriticalSection(&m_csDevW);
	InitializeCriticalSection(&m_csDevX);
	InitializeCriticalSection(&m_csDevR);


	
	m_ChannelNo = 151;			// 51(MELSECNET-10/H)
	m_StationNo	= 2;			// -1
	m_PlcStation = 255;			// 255
	m_NetworkNo = 0;
	
	m_bOpened = FALSE;

	m_MelsecErr = 0;
}

CPLCComm::~CPLCComm()
{
	std::list<ADDRESS_MAP>::iterator I;
	for(I=m_AddressMap.begin(); I!=m_AddressMap.end(); I++)
		delete (*I).pData;

	m_AddressMap.clear();

	DeleteCriticalSection(&m_csPLC);
	DeleteCriticalSection(&m_csDevB);
	DeleteCriticalSection(&m_csDevM);
	DeleteCriticalSection(&m_csDevL);
	DeleteCriticalSection(&m_csDevY);
	DeleteCriticalSection(&m_csDevW);
	DeleteCriticalSection(&m_csDevX);
	DeleteCriticalSection(&m_csDevR);
}

BOOL CPLCComm::Init(int nChannelNo, int nStationNo, int nPlcStation)
{
	//logmsg(LT_DEVICE,"CPLCComm::Init 시작");
	if(m_bOpened) return TRUE;

	m_ChannelNo = nChannelNo;//151;			// 51(MELSECNET-10/H)
	m_StationNo	= nStationNo;//2;			// 장비별 반드시 확인할것!!
	m_PlcStation = nPlcStation;//255;			// 255

	int ret = mdOpen(m_ChannelNo, m_StationNo, &m_Path);
	if(ret) 
	{
//		logmsg(LT_DEVICE,"PLC Device Open:%d Ch,%d Station\n",m_ChannelNo,m_StationNo);
//		logmsg(LT_ERROR,"PLC Device Open:%d Ch,%d Station\n",m_ChannelNo,m_StationNo);
		ret = mdOpen(m_ChannelNo, m_StationNo, &m_Path);
		if(ret) 
		{
			m_bOpened = FALSE;
			return FALSE;
		}
	}
	mdBdRst(m_Path);
	m_bOpened = TRUE;

//	logmsg(LT_DEVICE,"CPLCComm::Init 끝");
	return TRUE;
}

void CPLCComm::Exit()
{
//	logmsg(LT_DEVICE,"CPLCComm::Exit 시작");
	if (m_bOpened) {
		int ret = mdClose(m_Path);		//	close channel	
		if(ret) {
			mdClose(m_Path);			//	close channel	
		}
	}

	m_bOpened = FALSE;
//	logmsg(LT_DEVICE,"CPLCComm::Exit 끝");
}


void CPLCComm::AddAddress(int nDevType, int nStartAddress, int nEndAddress)
{
	int nDataSize = nEndAddress - nStartAddress + 1;
	
	switch ( nDevType ) 
	{
	case DevM:
	case DevB:
	case DevX:
	case DevY:
	case DevL:
		{
//			Q_ASSERT( !(nDataSize<=0 && nDataSize%BIT_COUNT_PER_WORD) );
			nDataSize /= BIT_COUNT_PER_WORD;
			nDataSize += 1;
			break;
		}

	case DevR:
	case DevD:
	case DevW:
		{
//			Q_ASSERT( !(nDataSize<=0) );
			nDataSize = nDataSize;
			break;
		}

	default:
		return;
	}
	
	WORD* pData = new WORD[nDataSize];

	memset(pData, 0, sizeof(WORD)*nDataSize);
	
	ADDRESS_MAP PushData;
	PushData.nDevType		= nDevType;
	PushData.nStartAddress	= nStartAddress;
	PushData.nEndAddress	= nEndAddress;
	PushData.pData			= pData;
	
	m_AddressMap.push_back( PushData );
}

void CPLCComm::AddAddressSize(int nDevType, int nAddress, int nSize)
{
	m_AddressSize[nDevType][nAddress] = nSize;
}

int CPLCComm::GetAddressSize(int nDevType, int nAddress)
{
	return m_AddressSize[nDevType][nAddress];
}

BOOL CPLCComm::Write(int nDevType, int nStartAddress, int nEndAddress, WORD *pWriteData, int nDataSize)
{
	if(!m_bOpened) return FALSE;

	m_MelsecErr = 0;
	short size;
	long lsize;
	std::list<ADDRESS_MAP>::iterator I;

	for(I=m_AddressMap.begin(); I!=m_AddressMap.end(); I++) 
	{
		ADDRESS_MAP *pAM = &(*I);
		if(pAM->nDevType==nDevType && (pAM->nStartAddress<=nStartAddress) && (nEndAddress<=pAM->nEndAddress)) 
		{
			WORD *pData = pAM->pData;
			short ret=0;
			switch (pAM->nDevType) 
			{
			case DevM:
				{
					EnterCriticalSection(&m_csDevM);
					size = nEndAddress-nStartAddress+1;
					if(size <= BIT_COUNT_PER_WORD-1) size = 1;
					else size = size/BIT_COUNT_PER_WORD + 1;
					size*=2;
					pData = pData + (nStartAddress-pAM->nStartAddress)/BIT_COUNT_PER_WORD;
					nStartAddress = (int(nStartAddress/BIT_COUNT_PER_WORD))*BIT_COUNT_PER_WORD;
					
					ret = mdSend(m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData);
					LeaveCriticalSection(&m_csDevM);
					break;
				}
			case DevB:
				{
					EnterCriticalSection(&m_csDevB);
					size = nEndAddress-nStartAddress+1;
					if(size <= BIT_COUNT_PER_WORD-1) size = 1;
					else size = size/BIT_COUNT_PER_WORD + 1;
					size*=2;
					pData = pData + (nStartAddress-pAM->nStartAddress)/BIT_COUNT_PER_WORD;
					nStartAddress = (int(nStartAddress/BIT_COUNT_PER_WORD))*BIT_COUNT_PER_WORD;
					
					ret = mdSend(m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData);
					LeaveCriticalSection(&m_csDevB);
					break;
				}
			case DevX:
				{
					EnterCriticalSection(&m_csDevX);
					size = nEndAddress-nStartAddress+1;
					if(size <= BIT_COUNT_PER_WORD-1) size = 1;
					else size = size/BIT_COUNT_PER_WORD + 1;
					size*=2;
					pData = pData + (nStartAddress-pAM->nStartAddress)/BIT_COUNT_PER_WORD;
					nStartAddress = (int(nStartAddress/BIT_COUNT_PER_WORD))*BIT_COUNT_PER_WORD;
					
					ret = mdSend(m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData);
					LeaveCriticalSection(&m_csDevX);
					break;
				}
			case DevY:
				{
					EnterCriticalSection(&m_csDevY);
					size = nEndAddress-nStartAddress+1;
					if(size <= BIT_COUNT_PER_WORD-1) size = 1;
					else size = size/BIT_COUNT_PER_WORD + 1;
					size*=2;
					pData = pData + (nStartAddress-pAM->nStartAddress)/BIT_COUNT_PER_WORD;
					nStartAddress = (int(nStartAddress/BIT_COUNT_PER_WORD))*BIT_COUNT_PER_WORD;
					
					ret = mdSend(m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData);
					LeaveCriticalSection(&m_csDevY);
					break;
				}
			case DevL:
				{
					EnterCriticalSection(&m_csDevL);
					size = nEndAddress-nStartAddress+1;
					if(size <= BIT_COUNT_PER_WORD-1) size = 1;
					else size = size/BIT_COUNT_PER_WORD + 1;
					size*=2;
					pData = pData + (nStartAddress-pAM->nStartAddress)/BIT_COUNT_PER_WORD;
					nStartAddress = (int(nStartAddress/BIT_COUNT_PER_WORD))*BIT_COUNT_PER_WORD;
					
					ret = mdSend(m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData);
					LeaveCriticalSection(&m_csDevL);
					break;
				}

			case DevR:
				{
					EnterCriticalSection(&m_csDevR);
					pData = pData + nStartAddress-pAM->nStartAddress;
					size = nEndAddress-nStartAddress + 1;
					size *= 2;
					if (pWriteData != NULL) memcpy(pData, pWriteData, sizeof(WORD)*nDataSize);
					ret = mdSend(m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData);
					LeaveCriticalSection(&m_csDevR);
					break;
				}
			case DevD:
				{
					EnterCriticalSection(&m_csDevD);
					pData = pData + nStartAddress-pAM->nStartAddress;
					size = nEndAddress-nStartAddress + 1;
					size *= 2;
					if (pWriteData != NULL) 
					{
						memcpy(pData, pWriteData, sizeof(WORD)*nDataSize);
					}
					ret = mdSend(m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData);
					LeaveCriticalSection(&m_csDevD);
					break;
				}
			case DevW:
				{
					EnterCriticalSection(&m_csDevW);
					pData = pData + nStartAddress-pAM->nStartAddress;
					lsize = nEndAddress-nStartAddress + 1;
					lsize *= 2;
					//lsize = nDataSize;
					if (pWriteData != NULL) 
					{
						memcpy(pData, pWriteData, sizeof(WORD)*nDataSize);
					}
					//size = nDataSize - 1;
					//ret = mdSend(m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData);
					ret = mdSendEx(m_Path, m_NetworkNo, m_PlcStation, nDevType, nStartAddress, &lsize, pData);
					LeaveCriticalSection(&m_csDevW);
					break;
				}
		    }
			
			if(ret)
			{
				//logmsg(LT_DEVICE,"CPLCComm::Write::mdsend Error code :%d",ret);
//				logmsg(LT_DEVICE,"CPLCComm::Write::mdsend Error code :%d, Station:%d,Dev:%d,StartAddress:%d,Size:%d",ret,m_PlcStation,nDevType,nStartAddress,size);
				m_MelsecErr = ret;
				return FALSE;
			}
				
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CPLCComm::Read(int nDevType, int nStartAddress, int nEndAddress, WORD *pReadData, int nDataSize)
{
	if(!m_bOpened) return FALSE;
	
	m_MelsecErr = 0;
	short size;
	long lsize;
	std::list<ADDRESS_MAP>::iterator I;
	for(I=m_AddressMap.begin(); I!=m_AddressMap.end(); I++) 
	{
		ADDRESS_MAP *pAM = &(*I);
		
		if(pAM->nDevType == nDevType && (pAM->nStartAddress<=nStartAddress) && (nEndAddress<=pAM->nEndAddress)) 
		{
			WORD *pData = pAM->pData;
			short ret=0;
			switch (pAM->nDevType) 
			{
			case DevM:
				{
					EnterCriticalSection(&m_csDevM);
					size = nEndAddress-nStartAddress+1;
					if(size <= BIT_COUNT_PER_WORD-1) size = 1;
					else size = size/BIT_COUNT_PER_WORD + 1;
					size *= 2;
					pData = pData + (nStartAddress-pAM->nStartAddress)/BIT_COUNT_PER_WORD;
					nStartAddress = (int(nStartAddress/BIT_COUNT_PER_WORD))*BIT_COUNT_PER_WORD;
					ret = mdReceive( m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData );				
					if(ret)
						ret = mdReceive( m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData );

					LeaveCriticalSection(&m_csDevM);
					break;
				}
			case DevB:
				{
					EnterCriticalSection(&m_csDevB);
					size = nEndAddress-nStartAddress+1;
					if(size <= BIT_COUNT_PER_WORD-1) size = 1;
					else size = size/BIT_COUNT_PER_WORD + 1;
					size *= 2;
					pData = pData + (nStartAddress-pAM->nStartAddress)/BIT_COUNT_PER_WORD;
					nStartAddress = (int(nStartAddress/BIT_COUNT_PER_WORD))*BIT_COUNT_PER_WORD;
					ret = mdReceive( m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData );				
					if(ret)
						ret = mdReceive( m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData );
				
					LeaveCriticalSection(&m_csDevB);
					break;
				}
			case DevX:
				{
					EnterCriticalSection(&m_csDevX);
					size = nEndAddress-nStartAddress+1;
					if(size <= BIT_COUNT_PER_WORD-1) size = 1;
					else size = size/BIT_COUNT_PER_WORD + 1;
					size *= 2;
					pData = pData + (nStartAddress-pAM->nStartAddress)/BIT_COUNT_PER_WORD;
					nStartAddress = (int(nStartAddress/BIT_COUNT_PER_WORD))*BIT_COUNT_PER_WORD;
					ret = mdReceive( m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData );				
					if(ret)
						ret = mdReceive( m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData );
					
					LeaveCriticalSection(&m_csDevX);
					break;
				}
			case DevY:
				{
					EnterCriticalSection(&m_csDevY);
					size = nEndAddress-nStartAddress+1;
					if(size <= BIT_COUNT_PER_WORD-1) size = 1;
					else size = size/BIT_COUNT_PER_WORD + 1;
					size *= 2;
					pData = pData + (nStartAddress-pAM->nStartAddress)/BIT_COUNT_PER_WORD;
					nStartAddress = (int(nStartAddress/BIT_COUNT_PER_WORD))*BIT_COUNT_PER_WORD;
					ret = mdReceive( m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData );				
					if(ret)
						ret = mdReceive( m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData );
					
					LeaveCriticalSection(&m_csDevY);
					break;
				}
			case DevL:
				{
					EnterCriticalSection(&m_csDevL);
					size = nEndAddress-nStartAddress+1;
					if(size <= BIT_COUNT_PER_WORD-1) size = 1;
					else size = size/BIT_COUNT_PER_WORD + 1;
					size *= 2;
					pData = pData + (nStartAddress-pAM->nStartAddress)/BIT_COUNT_PER_WORD;
					nStartAddress = (int(nStartAddress/BIT_COUNT_PER_WORD))*BIT_COUNT_PER_WORD;
					ret = mdReceive( m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData );				
					if(ret)
						ret = mdReceive( m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData );
					
					LeaveCriticalSection(&m_csDevL);
					break;
				}
			case DevR:
				{
					EnterCriticalSection(&m_csDevR);
					pData = pData + nStartAddress-pAM->nStartAddress;
					size = nEndAddress-nStartAddress + 1;
					size *= 2;
					ret = mdReceive( m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData );
					if(ret) 
						ret = mdReceive( m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData );
					if(pReadData!=NULL) {
						memcpy( pReadData, pData, sizeof(WORD)*min(nDataSize, size) );
					}
					LeaveCriticalSection(&m_csDevR);
					break;
				}
			case DevD:
				{
					EnterCriticalSection(&m_csDevD);
					pData = pData + nStartAddress-pAM->nStartAddress;
					size = nEndAddress-nStartAddress + 1;
					size *= 2;
					ret = mdReceive( m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData );
					if(ret) 
						ret = mdReceive( m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData );
					if(pReadData!=NULL) {
						memcpy( pReadData, pData, sizeof(WORD)*min(nDataSize, size) );
					}
					LeaveCriticalSection(&m_csDevD);
					break;
				}
			case DevW:
				{
					EnterCriticalSection(&m_csDevW);
					pData = pData + nStartAddress-pAM->nStartAddress;
					lsize = nEndAddress-nStartAddress + 1;
					lsize *= 2;
					//ret = mdReceive( m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData );
					ret = mdReceiveEx( m_Path, m_NetworkNo, m_PlcStation, nDevType, nStartAddress, &lsize, pData );
					//ret = mdSendEx(m_Path, , m_PlcStation, nDevType, nStartAddress, &lsize, pData);
					if(ret) 
						//ret = mdReceive( m_Path, m_PlcStation, nDevType, nStartAddress, &size, pData );
						ret = mdReceiveEx( m_Path, m_NetworkNo, m_PlcStation, nDevType, nStartAddress, &lsize, pData );
					if(pReadData!=NULL) {
						memcpy( pReadData, pData, sizeof(WORD)*min(nDataSize, lsize) );
					}
					LeaveCriticalSection(&m_csDevW);
					break;
				}
			}

			if(ret)
			{
				//logmsg(LT_DEVICE,"CPLCComm::Read::mdReceive Error code :%d",ret);
//				logmsg(LT_DEVICE,"CPLCComm::Read::mdReceive Error code :%d, Station:%d,Dev:%d,StartAddress:%d,Size:%d",ret,m_PlcStation,nDevType,nStartAddress,size);
				m_MelsecErr = ret;
				return FALSE;
			}
			return TRUE;
		}
	}
	return FALSE;
}

WORD* CPLCComm::GetPointer(int nDevType, int nAddress)
{
	if(!m_bOpened) return FALSE;

	std::list<ADDRESS_MAP>::iterator I;
	WORD* pPointer = NULL;
	for(I=m_AddressMap.begin(); I!=m_AddressMap.end(); I++) 
	{
		ADDRESS_MAP *pAM = &(*I);
		
		if((pAM->nStartAddress <= nAddress) && (nAddress <= pAM->nEndAddress) && ( nDevType == pAM->nDevType)) 
		{
			WORD *pData = pAM->pData;
			switch(pAM->nDevType) 
			{
			case DevM:
				{
					EnterCriticalSection(&m_csDevM);
					pPointer = pData + (nAddress - pAM->nStartAddress)/BIT_COUNT_PER_WORD;
					LeaveCriticalSection(&m_csDevM);
					return pPointer;
				}
			case DevB:
				{
					EnterCriticalSection(&m_csDevB);
					pPointer = pData + (nAddress - pAM->nStartAddress)/BIT_COUNT_PER_WORD;
					LeaveCriticalSection(&m_csDevB);
					return pPointer;
				}
				
			case DevX:
				{
					EnterCriticalSection(&m_csDevX);
					pPointer = pData + (nAddress - pAM->nStartAddress)/BIT_COUNT_PER_WORD;
					LeaveCriticalSection(&m_csDevX);
					return pPointer;
				}
			case DevY:
				{
					EnterCriticalSection(&m_csDevY);
					pPointer = pData + (nAddress - pAM->nStartAddress)/BIT_COUNT_PER_WORD;
					LeaveCriticalSection(&m_csDevY);
					return pPointer;
				}
			case DevL:
				{
					EnterCriticalSection(&m_csDevL);
					pPointer = pData + (nAddress - pAM->nStartAddress)/BIT_COUNT_PER_WORD;
					LeaveCriticalSection(&m_csDevL);
					return pPointer;
				}
			case DevR:				
				{
					EnterCriticalSection(&m_csDevR);
					pPointer = pData + nAddress - pAM->nStartAddress;
					LeaveCriticalSection(&m_csDevR);
					return pPointer;
				} 
			case DevD:
				{
					EnterCriticalSection(&m_csDevD);
					pPointer = pData + nAddress - pAM->nStartAddress;
					LeaveCriticalSection(&m_csDevD);
					return pPointer;
				} 
			case DevW:
				{
					EnterCriticalSection(&m_csDevW);
					pPointer = pData + nAddress - pAM->nStartAddress;
					LeaveCriticalSection(&m_csDevW);
					return pPointer;
				}
			}
		}
	}
	
	return NULL;
}

// BOOL CPLCComm::SetBit(int nAddress, BOOL bValue, BOOL bSync)
// {
// 	int nStep = 0;
// 	volatile BOOL bRun = 0;
// 	int nRetry = 0;
// 
// 	while(bRun)
// 	{
// 		switch(nStep)
// 		{
// 		case 0 :	
// 			{				
// 				nStep = 100;
// 				
// 				if(nRetry > 2)
// 					break;
// 
// 				if(bSync) 
// 				{
// 					if(!Read(DevB, nAddress, nAddress))
// 					{
// 						nStep =100;
// 						nRetry+=1;
// 					}
// 				}
// 				break;
// 			}
// 	
// 		case 100:
// 			{
// 	EnterCriticalSection(&m_csPLC);
// 		int nBitPos = nAddress%BIT_COUNT_PER_WORD;
// 		WORD *pData = GetPointer(DevB, nAddress);
// 				if(pData==NULL) 
// 				{
// 			LeaveCriticalSection(&m_csPLC);
// 			return FALSE;
// 		}
// 
// 		if(bValue)	*pData = *pData | (1<<nBitPos);
// 		else		*pData = *pData & ~(1<<nBitPos);
// 	LeaveCriticalSection(&m_csPLC);
// 				nRetry = 0;
// 
// 				break;
// 			}
// 		case 200:
// 			{	
// 				
// 				if(bSync) 
// 				{
// 					if(nRetry > 2)
// 						return FALSE;
// 
// 					if(!Write(DevB, nAddress, nAddress))
// 					{
// 						nStep = 200;
// 						nRetry+=1;
// 					}
// 					else
// 					{
// 						bRun  = FALSE;
// 						nStep = 0;
// 					}
// 				}
// 				
// 			}
// 			break;
// 		}
// 	}
// 
// 	return TRUE;
// }

BOOL CPLCComm::SetBit(int nAddress, BOOL bValue, BOOL bSync)
{
	if(!m_bOpened) return FALSE;

	if(bSync) Read(DevB, nAddress, nAddress);
	
	EnterCriticalSection(&m_csPLC);
	int nBitPos = nAddress%BIT_COUNT_PER_WORD;
	WORD *pData = GetPointer(DevB, nAddress);
	if(pData==NULL) {
		LeaveCriticalSection(&m_csPLC);
		return FALSE;
	}
	
	if(bValue)	*pData = *pData | (1<<nBitPos);
	else		*pData = *pData & ~(1<<nBitPos);
	LeaveCriticalSection(&m_csPLC);
	
	if(bSync) Write(DevB, nAddress, nAddress);
	
	return TRUE;
}

BOOL CPLCComm::SetWORD(int nAddress, WORD wValue, BOOL bSync)
{
	if(!m_bOpened) return FALSE;

	if(bSync) Read(DevW, nAddress, nAddress);
	
	EnterCriticalSection(&m_csPLC);
		WORD* pData = (WORD*) GetPointer(DevW, nAddress);
		*pData = wValue;
	LeaveCriticalSection(&m_csPLC);
	
	if(bSync) Write(DevW, nAddress, nAddress);
	
	return TRUE;
}

BOOL CPLCComm::SetWORDS(int nAddress, int nSize, WORD *wValue, BOOL bSync)
{
	if(!m_bOpened) return FALSE;

	if(bSync) Read(DevW, nAddress, nAddress+nSize-1);
	
	EnterCriticalSection(&m_csPLC);
		WORD* pData = (WORD*) GetPointer(DevW, nAddress);
		memcpy(pData, wValue, sizeof(WORD)*nSize);
	LeaveCriticalSection(&m_csPLC);
	
	if(bSync) Write(DevW, nAddress, nAddress+nSize-1);
	
	return TRUE;
}

BOOL CPLCComm::SetDWORDS(int nAddress, int nSize, DWORD *wValue, BOOL bSync)
{
	if(!m_bOpened) return FALSE;

	if(bSync) Read(DevW, nAddress, nAddress+nSize-1);
	
	EnterCriticalSection(&m_csPLC);
		WORD* pData = (WORD*) GetPointer(DevW, nAddress);
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

BOOL CPLCComm::GetBit(int nAddress, BOOL bSync)
{
	if(!m_bOpened) return FALSE;
	// JWY
	BOOL a;

	if(bSync) 
	{
		a = Read(DevB, nAddress, nAddress);
	}
	int nBitPos = nAddress%BIT_COUNT_PER_WORD;
	WORD *pData = GetPointer(DevB, nAddress);
	if(pData==NULL) return FALSE;
	BOOL bRet = (*pData & (1<<nBitPos)) > 0 ? TRUE : FALSE;

	return bRet;
}

WORD CPLCComm::GetWORD(int nAddress, BOOL bSync)
{
	if(!m_bOpened) return FALSE;

	if(bSync) Read(DevW, nAddress, nAddress);

	WORD* pData = GetPointer(DevW, nAddress);
	if(pData==NULL) return 0;
	WORD wRet = *pData;

	return wRet;
}
