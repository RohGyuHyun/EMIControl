#pragma once

#define DF_ROOT_PATH					_T("C:\\data2u5\\OLED\\")
#define DF_DEFECT_PATH					_T("DEFECT")
#define	DF_IMAGE_PATH					_T("IMAGE")

#define DEFECT_IMAGE	0
#define DEFECT_IMAGE2	1
#define	REF_IMAGE		2
#define PTNMNT_IMAGE	3

class CFTPFileFind  
{
public:
	void GetErrorMessage();
	
	//{{AFX_FindFile ÇÔ¼ö
	BOOL MatchsMask(DWORD dwMask);
	BOOL IsTemporary();
	BOOL IsSystem();
	BOOL IsReadOnly();
	BOOL IsNormal();
	BOOL IsHidden();
	BOOL IsDots();
	BOOL IsDirectory();
	BOOL IsCompressed();
	BOOL IsArchived();
	DWORD GetLength();
	BOOL GetLastWriteTime(CTime& Time);
	BOOL GetLastAccessTime(CTime& Time);
	CString GetFileTitle();
	CString GetFilePath();
	CString GetFileName();
	BOOL GetCreationTime(CTime& Time);
	CString GetFileURL();
	BOOL FindNextFile();
	BOOL FindFile(LPCTSTR RemoteFile = NULL, DWORD dwFlags = INTERNET_FLAG_RELOAD);
	//}}AFX_FindFile

	CFTPFileFind(CFtpConnection* pConnection);
	virtual ~CFTPFileFind();

	CFtpFileFind *m_pRemoteFindFile;
	CString	m_strAddress;
	
	
};

class CFPTClient  
{
public:
	BOOL Open(CString strAddress, CString strName, CString strPassword, INTERNET_PORT nPort = INTERNET_DEFAULT_FTP_PORT);
	BOOL Open(CString strAddress, INTERNET_PORT nPort);
	BOOL Close();

	CFPTClient();
	virtual ~CFPTClient();

	BOOL UpLoad(CString csLocalFile, CString csFTPdir);
	BOOL DownLoad(CString csFtpFilePath, CString csLocalFilePath);

	BOOL DownLoadDir(CString rDir, CString lDir);
	BOOL UpLoadDir(CString LocalDir, CString RemoteDir, BOOL bUpdatedata, BOOL isUpLoadFIleDel);
	BOOL Make_FtpPath(const char *path);
	BOOL GetURL(CString& RemoteURL);
	BOOL LPWD();
	BOOL RPWD();
	BOOL LRen(CString LocalOldFile, CString LocalNewFile);
	BOOL RRen(CString RemoteOldFile, CString RemoteNewFile);
	BOOL LDel(CString LocalFile);
	BOOL RDel(CString RemoteFile);
	BOOL LRD(CString LocalDir);
	BOOL RRD(CString RemoteDir);
	BOOL LMD(CString LocalDir);
	BOOL RMD(CString RemoteDir);
	BOOL LCD(CString LocalDir);
	BOOL RCD(CString RemoteDir);
	CString RCD();
	CString LCD();


	void GetErrorMessage();

	int make_path(const char *path);
	//virtual ~CFPTClient();

	//{{AFX_DATA(FTP)
	CInternetSession	m_Session;
	CFtpConnection		*m_pConnection;
	//CFtpConnection		m_pConnection1;

	CString m_strLocalDirText;
	CString	m_strRemoteDirText;
	CString	m_strRemoteDir;
	CString	m_strAddress;

	BOOL m_bServerConnect;
//	BOOL IDXFindFile(CString hash1,CString hash2,CString idxpath);
	BOOL IDXFileFind(CString hash1,CString hash2,CString idxpath,CString glassid);
	BOOL DefectFileFind(CString month,CString day,CString recipe,CString unitid,CString lotid,CString defectfilename);
	CString m_idxtemp;
	CString Getidxtemp();
	CString m_temppath;

	BOOL IDXUpload(CString path,CString hash,CString glassid);
	BOOL DefectFileUpload(CString month,CString day,CString recipe,CString unitid,CString lotid,CString defectfilename,int type);
	BOOL RMC(CString searchdir);

	BOOL DefectImageIdxUpload(CString month,CString day,CString recipe,CString unitid,CString lotid,CString glassid,CString filename);
	BOOL DefectImageUpload(int type,CString month,CString day,CString recipe,CString unitid,CString lotid,CString Lfilepath);
	BOOL IDXFileUpdate(CString hash1,CString hash2,CString pathdata,CString glassid);

	//20160622	sc
	CString GetRootPath();
	CString	GetDefectPath();
	CString GetImagePath();

	BOOL FTPWriteLog(CString text,int log_type);
	
	//}}AFX_DATA

};