// 
// Hash.h :
//
#ifndef __HASHCALC_H__
#define __HASHCALC_H__

#define SEED    					7919

class CHash
{
private:

public:
	CHash(void){};
	virtual ~CHash(void){};

	void GetHashPathName(char *sFileName, char *sHashPath); 
	long GetHashValue(char *pKeyStr) ;
	long TranHashValue2NumberInLayer(long HashValue, long LayerNumber);
};

#endif __HASHCALC_H__

