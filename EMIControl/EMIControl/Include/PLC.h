#ifndef __PLC_H__
#define __PLC_H__

typedef struct {
	int nDevType;
	int nStartAddress;
	int nEndAddress;
	WORD *pData;
} ADDRESS_MAP;

typedef struct {
	int nAddress;
	int nSize;
} ADDRESS_TYPE;

class CPLC 
{
public:
	virtual ~CPLC() {};
	virtual BOOL	Init(int nChannelNo, int nStationNo, int nPlcStation) = 0;
	virtual void	Exit() = 0;

	virtual void	AddAddress(int nDevType, int nStartAddress, int nEndAddress) = 0;
	virtual void	AddAddressSize(int nDevType, int nAddress, int nSize) = 0;
	virtual int		GetAddressSize(int nDevType, int nAddress) = 0;

	virtual BOOL	Read(int nDevType, int nStartAddress, int nEndAddress = -1, WORD *pData = NULL, int nDataSize = 0) = 0;
	virtual BOOL	Write(int nDevType, int nStartAddress, int nEndAddress = -1, WORD *pData = NULL, int nDataSize = 0) = 0;

	virtual WORD*	GetPointer(int nDevType, int nAddress) = 0;

	virtual BOOL	SetBit(int nAddress, BOOL bValue, BOOL bSync=TRUE) = 0;
	virtual BOOL	SetWORD(int nAddress, WORD wValue, BOOL bSync=TRUE) = 0;
	virtual BOOL	SetWORDS(int nAddress, int nSize, WORD *wValue, BOOL bSync=TRUE) = 0;
	virtual BOOL	SetDWORDS(int nAddress, int nSize, DWORD *wValue, BOOL bSync=TRUE) = 0;

	virtual BOOL	GetBit(int nAddress, BOOL bSync=TRUE) = 0;
	virtual WORD	GetWORD(int nAddress, BOOL bSync=TRUE) = 0;

};

#endif //__PLC_H__