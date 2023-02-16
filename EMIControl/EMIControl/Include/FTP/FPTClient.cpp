#include "StdAfx.h"
#include "FPTClient.h"
#include "EMIControlDlg.h"



CFTPFileFind::CFTPFileFind(CFtpConnection* pConnection)
{
	m_pRemoteFindFile = new CFtpFileFind(pConnection);
	pConnection->GetCurrentDirectoryAsURL(m_strAddress);
}

CFTPFileFind::~CFTPFileFind()
{
	m_pRemoteFindFile->Close();
	delete m_pRemoteFindFile;
}

BOOL CFTPFileFind::FindFile(LPCTSTR RemoteFile, DWORD dwFlags)
{
	try
	{
		BOOL brc;
		brc = m_pRemoteFindFile->FindFile(RemoteFile, dwFlags);
		if(brc == FALSE){
			TRACE("RFindFile ERROR : %s -> %d \n", m_strAddress, ::GetLastError());
			GetErrorMessage();
			return FALSE;
		}
	}
	catch(CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RFindFile ERROR : %s -> %s \n", m_strAddress, buff);
		ee->Delete();
		return FALSE;
	}

	return TRUE;
}

BOOL CFTPFileFind::FindNextFile()
{
	try
	{
		BOOL brc;
		brc = m_pRemoteFindFile->FindNextFile();
		if(brc == FALSE){
			TRACE("RFindNextFile ERROR : %s -> %d \n", m_strAddress, ::GetLastError());
			GetErrorMessage();
			return FALSE;
		}
	}
	catch(CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RFindNextFile ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return FALSE;
	}

	return TRUE;
}

CString CFTPFileFind::GetFileURL()
{
	try
	{
		return m_pRemoteFindFile->GetFileURL();
	}
	catch(CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RFindNextFile ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return _T("");
	}
}

BOOL CFTPFileFind::GetCreationTime(CTime &Time)
{
	try
	{
		return m_pRemoteFindFile->GetCreationTime(Time);
	}
	catch(CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RGetCreationTime ERROR : %s -> %S \n", m_strAddress, buff);
		
		ee->Delete();
		return FALSE;
	}
}

CString CFTPFileFind::GetFileName()
{
	try
	{
		return m_pRemoteFindFile->GetFileName();
	}
	catch(CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RGetFimeName ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return _T("");
	}
}

CString CFTPFileFind::GetFilePath()
{
	try
	{
		return m_pRemoteFindFile->GetFilePath();
	}
	catch(CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RGetFilePath ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return _T("");
	}
}

CString CFTPFileFind::GetFileTitle()
{
	try
	{
		return m_pRemoteFindFile->GetFileTitle();
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RGetFileTitle ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return _T("");
	}
}

BOOL CFTPFileFind::GetLastAccessTime(CTime &Time)
{
	try
	{
		return m_pRemoteFindFile->GetLastAccessTime(Time);
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RGetLastAccessTime ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return FALSE;
	}
}

BOOL CFTPFileFind::GetLastWriteTime(CTime &Time)
{
	try
	{
		return m_pRemoteFindFile->GetLastWriteTime(Time);
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RGetLastWriteTime ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return FALSE;
	}
}

DWORD CFTPFileFind::GetLength()
{
	try
	{
		return m_pRemoteFindFile->GetLength();
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RGetLength ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return 0;
	}
}

BOOL CFTPFileFind::IsArchived()
{
	try
	{
		return m_pRemoteFindFile->IsArchived();
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RIsArchived ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return FALSE;
	}
}

BOOL CFTPFileFind::IsCompressed()
{
	try
	{
		return m_pRemoteFindFile->IsCompressed();
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RIsCompressed ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return FALSE;
	}
}

BOOL CFTPFileFind::IsDirectory()
{
	try
	{
		return m_pRemoteFindFile->IsDirectory();
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RIsDirectory ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return FALSE;
	}
}

BOOL CFTPFileFind::IsDots()
{
	try
	{
		return m_pRemoteFindFile->IsDots();
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RIsDots ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return FALSE;
	}
}

BOOL CFTPFileFind::IsHidden()
{
	try
	{
		return m_pRemoteFindFile->IsHidden();
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RIsHidden ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return FALSE;
	}
}

BOOL CFTPFileFind::IsNormal()
{
	try
	{
		return m_pRemoteFindFile->IsNormal();
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("LIsNormal ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return FALSE;
	}
}

BOOL CFTPFileFind::IsReadOnly()
{
	try
	{
		return m_pRemoteFindFile->IsReadOnly();
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RIsReadOnly ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return FALSE;
	}
}

BOOL CFTPFileFind::IsSystem()
{
	try
	{
		return m_pRemoteFindFile->IsSystem();
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RIsSystem ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return FALSE;
	}
}

BOOL CFTPFileFind::IsTemporary()
{
	try
	{
		return m_pRemoteFindFile->IsTemporary();
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RIsTemporary ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return FALSE;
	}
}

BOOL CFTPFileFind::MatchsMask(DWORD dwMask)
{
	try
	{
		return m_pRemoteFindFile->MatchesMask(dwMask);
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RMatchsMask ERROR : %s -> %S \n", m_strAddress, buff);
		ee->Delete();
		return FALSE;
	}
}

void CFTPFileFind::GetErrorMessage()
{
	char *szRcode;
	int rCode;
	CString listlog;

	rCode = ::GetLastError();
	switch(rCode) {
		 case ERROR_ACCESS_DENIED          :    szRcode = "ERROR_ACCESS_DENIED          ";    break;
		 case ERROR_ADAP_HDW_ERR           :    szRcode = "ERROR_ADAP_HDW_ERR           ";    break;
		 case ERROR_ALREADY_ASSIGNED       :    szRcode = "ERROR_ALREADY_ASSIGNED       ";    break;
		 case ERROR_ARENA_TRASHED          :    szRcode = "ERROR_ARENA_TRASHED          ";    break;
		 case ERROR_BAD_COMMAND            :    szRcode = "ERROR_BAD_COMMAND            ";    break;
		 case ERROR_BAD_DEV_TYPE           :    szRcode = "ERROR_BAD_DEV_TYPE           ";    break;
		 case ERROR_BAD_ENVIRONMENT        :    szRcode = "ERROR_BAD_ENVIRONMENT        ";    break;
		 case ERROR_BAD_FORMAT             :    szRcode = "ERROR_BAD_FORMAT             ";    break;
		 case ERROR_BAD_LENGTH             :    szRcode = "ERROR_BAD_LENGTH             ";    break;
		 case ERROR_BAD_NET_NAME           :    szRcode = "ERROR_BAD_NET_NAME           ";    break;
		 case ERROR_BAD_NET_RESP           :    szRcode = "ERROR_BAD_NET_RESP           ";    break;
		 case ERROR_BAD_NETPATH            :    szRcode = "ERROR_BAD_NETPATH            ";    break;
		 case ERROR_BAD_REM_ADAP           :    szRcode = "ERROR_BAD_REM_ADAP           ";    break;
		 case ERROR_BAD_UNIT               :    szRcode = "ERROR_BAD_UNIT               ";    break;
		 case ERROR_CANNOT_MAKE            :    szRcode = "ERROR_CANNOT_MAKE            ";    break;
		 case ERROR_CRC                    :    szRcode = "ERROR_CRC                    ";    break;
		 case ERROR_CURRENT_DIRECTORY      :    szRcode = "ERROR_CURRENT_DIRECTORY      ";    break;
		 case ERROR_DEV_NOT_EXIST          :    szRcode = "ERROR_DEV_NOT_EXIST          ";    break;
		 case ERROR_DUP_NAME               :    szRcode = "ERROR_DUP_NAME               ";    break;
		 case ERROR_FAIL_I24               :    szRcode = "ERROR_FAIL_I24               ";    break;
		 case ERROR_FILE_EXISTS            :    szRcode = "ERROR_FILE_EXISTS            ";    break;
		 case ERROR_FILE_NOT_FOUND         :    szRcode = "ERROR_FILE_NOT_FOUND         ";    break;
		 case ERROR_GEN_FAILURE            :    szRcode = "ERROR_GEN_FAILURE            ";    break;
		 case ERROR_HANDLE_DISK_FULL       :    szRcode = "ERROR_HANDLE_DISK_FULL       ";    break;
		 case ERROR_HANDLE_EOF             :    szRcode = "ERROR_HANDLE_EOF             ";    break;
		 case ERROR_INVALID_ACCESS         :    szRcode = "ERROR_INVALID_ACCESS         ";    break;
		 case ERROR_INVALID_BLOCK          :    szRcode = "ERROR_INVALID_BLOCK          ";    break;
		 case ERROR_INVALID_DATA           :    szRcode = "ERROR_INVALID_DATA           ";    break;
		 case ERROR_INVALID_DRIVE          :    szRcode = "ERROR_INVALID_DRIVE          ";    break;
		 case ERROR_INVALID_FUNCTION       :    szRcode = "ERROR_INVALID_FUNCTION       ";    break;
		 case ERROR_INVALID_HANDLE         :    szRcode = "ERROR_INVALID_HANDLE         ";    break;
		 case ERROR_INVALID_PARAMETER      :    szRcode = "ERROR_INVALID_PARAMETER      ";    break;
		 case ERROR_INVALID_PASSWORD       :    szRcode = "ERROR_INVALID_PASSWORD       ";    break;
		 case ERROR_LOCK_VIOLATION         :    szRcode = "ERROR_LOCK_VIOLATION         ";    break;
		 case ERROR_NET_WRITE_FAULT        :    szRcode = "ERROR_NET_WRITE_FAULT        ";    break;
		 case ERROR_NETNAME_DELETED        :    szRcode = "ERROR_NETNAME_DELETED        ";    break;
		 case ERROR_NETWORK_ACCESS_DENIED  :    szRcode = "ERROR_NETWORK_ACCESS_DENIED  ";    break;
		 case ERROR_NETWORK_BUSY           :    szRcode = "ERROR_NETWORK_BUSY           ";    break;
		 case ERROR_NO_MORE_FILES          :    szRcode = "ERROR_NO_MORE_FILES          ";    break;
		 case ERROR_NO_PROC_SLOTS          :    szRcode = "ERROR_NO_PROC_SLOTS          ";    break;
		 case ERROR_NO_SPOOL_SPACE         :    szRcode = "ERROR_NO_SPOOL_SPACE         ";    break;
		 case ERROR_NOT_DOS_DISK           :    szRcode = "ERROR_NOT_DOS_DISK           ";    break;
		 case ERROR_NOT_ENOUGH_MEMORY      :    szRcode = "ERROR_NOT_ENOUGH_MEMORY      ";    break;
		 case ERROR_NOT_READY              :    szRcode = "ERROR_NOT_READY              ";    break;
		 case ERROR_NOT_SAME_DEVICE        :    szRcode = "ERROR_NOT_SAME_DEVICE        ";    break;
		 case ERROR_NOT_SUPPORTED          :    szRcode = "ERROR_NOT_SUPPORTED          ";    break;
		 case ERROR_OUT_OF_PAPER           :    szRcode = "ERROR_OUT_OF_PAPER           ";    break;
		 case ERROR_OUT_OF_STRUCTURES      :    szRcode = "ERROR_OUT_OF_STRUCTURES      ";    break;
		 case ERROR_OUTOFMEMORY            :    szRcode = "ERROR_OUTOFMEMORY            ";    break;
		 case ERROR_PATH_NOT_FOUND         :    szRcode = "ERROR_PATH_NOT_FOUND         ";    break;
		 case ERROR_PRINT_CANCELLED        :    szRcode = "ERROR_PRINT_CANCELLED        ";    break;
		 case ERROR_PRINTQ_FULL            :    szRcode = "ERROR_PRINTQ_FULL            ";    break;
		 case ERROR_READ_FAULT             :    szRcode = "ERROR_READ_FAULT             ";    break;
		 case ERROR_REDIR_PAUSED           :    szRcode = "ERROR_REDIR_PAUSED           ";    break;
		 case ERROR_REM_NOT_LIST           :    szRcode = "ERROR_REM_NOT_LIST           ";    break;
		 case ERROR_REQ_NOT_ACCEP          :    szRcode = "ERROR_REQ_NOT_ACCEP          ";    break;
		 case ERROR_SECTOR_NOT_FOUND       :    szRcode = "ERROR_SECTOR_NOT_FOUND       ";    break;
		 case ERROR_SEEK                   :    szRcode = "ERROR_SEEK                   ";    break;
		 case ERROR_SHARING_BUFFER_EXCEEDED:    szRcode = "ERROR_SHARING_BUFFER_EXCEEDED";    break;
		 case ERROR_SHARING_PAUSED         :    szRcode = "ERROR_SHARING_PAUSED         ";    break;
		 case ERROR_SHARING_VIOLATION      :    szRcode = "ERROR_SHARING_VIOLATION      ";    break;
		 case ERROR_TOO_MANY_CMDS          :    szRcode = "ERROR_TOO_MANY_CMDS          ";    break;
		 case ERROR_TOO_MANY_NAMES         :    szRcode = "ERROR_TOO_MANY_NAMES         ";    break;
		 case ERROR_TOO_MANY_OPEN_FILES    :    szRcode = "ERROR_TOO_MANY_OPEN_FILES    ";    break;
		 case ERROR_TOO_MANY_SESS          :    szRcode = "ERROR_TOO_MANY_SESS          ";    break;
		 case ERROR_UNEXP_NET_ERR          :    szRcode = "ERROR_UNEXP_NET_ERR          ";    break;
		 case ERROR_WRITE_FAULT            :    szRcode = "ERROR_WRITE_FAULT            ";    break;
		 case ERROR_WRITE_PROTECT          :    szRcode = "ERROR_WRITE_PROTECT          ";    break;
		 case ERROR_WRONG_DISK             :    szRcode = "ERROR_WRONG_DISK             ";    break;
          
		 default 			               :    szRcode = "DEFAULT                      ";    break;
	}           
	TRACE("CFTPFileFind GetLastError() Errocode = %s [%d] \n", szRcode, rCode);
	listlog.Format(_T("CFTPFileFind GetLastError() Errocode = %S [%d]"),szRcode,rCode);
	return;
}

CFPTClient::CFPTClient()
{
	m_pConnection = NULL;

	//m_pConnection = new CFtpConnection(*m_pConnection);

	m_strLocalDirText.Empty();
	m_strRemoteDirText.Empty();
	m_strAddress.Empty();
	m_bServerConnect = FALSE;
}

CFPTClient::~CFPTClient()
{
	if(m_pConnection != NULL){
		m_pConnection->Close();
		delete m_pConnection;
	}
	m_Session.Close();
}

BOOL CFPTClient::Open(CString strAddress, CString strName, CString strPassword, INTERNET_PORT nPort)
{
	char buff[256];
	wchar_t temp[512];
//	CString buffer;
	CString log;
	m_bServerConnect = FALSE;
	if (m_pConnection != NULL) { // 이미 연결된 것이 있으면 해제
		m_pConnection->Close();
		delete m_pConnection;
		m_pConnection = NULL;
	}

	try {
		m_pConnection = m_Session.GetFtpConnection((LPCTSTR)strAddress, (LPCTSTR)strName, (LPCTSTR)strPassword, nPort);
		if(m_pConnection)
		{
			log.Format(_T("GetFtpConnection %s %s %s %d  successful!"),strAddress,strName,strPassword,nPort);
		}
		m_strAddress.Format(_T("%s"),strAddress);
		// 현재 Local 디렉토리
		GetCurrentDirectory(255, temp);
		m_strLocalDirText.Format(_T("%s"), temp);
		// 현재 Remote 디렉토리
		m_pConnection->GetCurrentDirectory(m_strRemoteDirText);
		m_bServerConnect = TRUE;
		
	}
	catch(CInternetException* ee)
	{
		//ee->GetErrorMessage((LPTSTR)buff, 255);
		//TRACE("(%s  ==>FTP Connection Error<==  %S)", strAddress, buff);
		//log.Format(_T("(%s  ==>FTP Connection Error<==  %S)"), strAddress, buff);
		m_pConnection = NULL;
		ee->Delete();
		
		return FALSE;
	}
	return TRUE;
}

CString CFPTClient::GetRootPath()
{
	CString temp;
	temp.Format(_T("%s"),DF_ROOT_PATH);
	return temp;
}

CString CFPTClient::GetDefectPath()
{
	CString temp;
	temp.Format(_T("%s"),DF_DEFECT_PATH);
	return temp;
}

CString CFPTClient::GetImagePath()
{
	CString temp;
	temp.Format(_T("%s"),DF_IMAGE_PATH);
	return temp;
}
	
BOOL CFPTClient::Open(CString strAddress, INTERNET_PORT nPort)
{
	CString log;

	char buff[256];

	if (m_pConnection != NULL){ // 이미 연결된 것이 있으면 해제
		m_pConnection->Close();
		delete m_pConnection;
		m_pConnection = NULL;
	}

	try {
		m_pConnection = m_Session.GetFtpConnection((LPCTSTR)strAddress, NULL, NULL, nPort);
		m_strAddress = strAddress;

		// 현재 Local 디렉토리
		GetCurrentDirectory(255, (LPWSTR)buff);
		m_strLocalDirText = buff;
		// 현재 Remote 디렉토리
		m_pConnection->GetCurrentDirectory(m_strRemoteDirText);
		log.Format(_T("GetCurrentDirectory(m_strRemoteDirText) %s", m_strRemoteDirText));
	} catch(CInternetException* ee) {
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("Connection ERROR : %s  ==><==  %S \n", strAddress, buff);
		log.Format(_T("Connection ERROR : %s  ==><==  %S \n", strAddress, buff));
		m_pConnection = NULL;
		ee->Delete();
		return FALSE;
	}
	return TRUE;
}

BOOL CFPTClient::Close()
{
	CString log;
	if (m_pConnection == NULL) return FALSE;

	try {
		m_pConnection->Close();
		delete m_pConnection;
		m_pConnection = NULL;
		m_bServerConnect = FALSE;
	} catch(CInternetException* ee) {
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("Close ERROR : %s (%s)", m_strAddress, buff);
		log.Format(_T("Close ERROR : %s (%S)"), m_strAddress, buff);
		m_pConnection = NULL;
		ee->Delete();
		return FALSE;
	}

	m_strLocalDirText.Empty();
	m_strRemoteDirText.Empty();
	m_strAddress.Empty();
		
	return TRUE;
}

BOOL CFPTClient::UpLoad(CString csLocalFile, CString csFTPdir)
{
	CString strPathName;

	CString log;

	if (m_pConnection == NULL) {
//		logmsg(LT_ERROR, "[DEFECT FILE] UpLoad - (m_pConnection == NULL)");
		log.Format(_T("[DEFECT FILE] UpLoad - (m_pConnection == NULL)"));
		return FALSE;
	}

	try {
		BOOL brc;
		/*csLocalFile.Replace( _T("\\"), _T("/"));
		csLocalFile.Replace( _T("//"), _T("/"));
		csFTPdir.Replace( _T("\\"), _T("/"));
		csFTPdir.Replace( _T("//"), _T("/"));*/
		brc = m_pConnection->PutFile(csLocalFile, csFTPdir); // 파일 업로드

		
		if (brc == FALSE) {
			TRACE("UpLoad ERROR : %s (%s)<==(%s) %d \n", m_strAddress, csFTPdir, csLocalFile, ::GetLastError());
			GetErrorMessage();
//			logmsg(LT_ERROR, "[DEFECT FILE] UpLoad - try");
			log.Format(_T("upload error"));
			return FALSE;
		}
	} catch(CInternetException* ee) {
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("UpLoad ERROR : %s (%s)<==(%s) %s \n", m_strAddress, csFTPdir, csLocalFile, buff);
		log.Format(_T("UpLoad ERROR : %s (%s)<==(%s) %S \n"), m_strAddress, csFTPdir, csLocalFile, buff);
//		testdlg->ListLog(log);
		ee->Delete();
//		logmsg(LT_ERROR, "[DEFECT FILE] UpLoad - catch");
		log.Format(_T("[DEFECT FILE] UpLoad - catch"));
	//	testdlg->ListLog(log);
		return FALSE;
	}

	return TRUE;
}

BOOL CFPTClient::DownLoad(CString csFtpFilePath, CString csLocalFilePath)
{
	CString log;
	if (m_pConnection == NULL) return FALSE;

	try 
	{
		BOOL brc = m_pConnection->GetFile(csFtpFilePath, csLocalFilePath);//,FALSE,FILE_ATTRIBUTE_NORMAL,FTP_TRANSFER_TYPE_ASCII | INTERNET_FLAG_RELOAD,NULL); // 파일 다운로드
		if (brc == FALSE) 
		{
			TRACE("DownLoad ERROR : %s (%s)==>(%s) %d \n", m_strAddress, csFtpFilePath, csLocalFilePath, ::GetLastError());
			log.Format(_T("DownLoad ERROR : %s (%s)==>(%s) %d \n"), m_strAddress, csFtpFilePath, csLocalFilePath, ::GetLastError());
	//		testdlg->ListLog(log);
			GetErrorMessage();
			return FALSE;
		}
	} 
	catch (CInternetException* ee) 
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("DownLoad ERROR : %s (%s)==>(%s) %s \n", m_strAddress, csFtpFilePath, csLocalFilePath, buff);
	//	log.Format(_T("DownLoad ERROR : %s (%s)==>(%s) %S \n"), m_strAddress, csFtpFilePath, csLocalFilePath, buff);
	//	testdlg->ListLog(log);
		ee->Delete();
		return FALSE;
	}

	return TRUE;
}

BOOL CFPTClient::IDXUpload(CString path,CString hash,CString glassid)
{
	CFTPFileFind* finder = new CFTPFileFind(m_pConnection);

	CString	cFileName,cFilePath, cFileDate, cLDIR, cRDIR, cJDIR,temp,Rfilepath,temp2,hashtemp;
	CString strlog;
	strlog.Format(_T("Defect File Create : IDXUpload(path : %s,hash : %s,glassid : %s) Start! "), path, hash, glassid); 
	FTPWriteLog(strlog,DEFECT_SERVER_LOG);

	BOOL bWorking,bUpload,ret;

	bWorking  = m_pConnection->SetCurrentDirectory(GetDefectPath());
	m_pConnection->GetCurrentDirectoryW(temp);
	
//	m_pConnection->SetCurrentDirectory(_T("INDEX"));
	m_pConnection->SetCurrentDirectory(_T("INDEX"));
	m_pConnection->GetCurrentDirectoryW(temp);
	if(bWorking)
	{
		strlog.Format(_T("Defect File Create : IDXUpload(); Current Directory INDEX Set Successful ")); 
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	else
	{
		strlog.Format(_T("Defect File Create : IDXUpload(); Current Directory INDEX Set fail ")); 
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
		return FALSE;
	}
	AfxExtractSubString(hashtemp,hash,0,'\\');

	cJDIR = hashtemp;
	if (cJDIR.Right(1) != _T("\\")) cJDIR += _T("\\*.*");
	else cJDIR += _T("*.*");
	bWorking = finder->FindFile(cJDIR);

	if(!bWorking)
	{
		ret = RMD(hashtemp); // Remote Make Dir : FTP서버에 Dir 만들기.
		if (ret == FALSE)
		{
			strlog.Format(_T("Defect File Create : IDXUpload(); %s Directory Create fail! "),hashtemp); 
			FTPWriteLog(strlog,DEFECT_SERVER_LOG);
//			delete finder;
			return FALSE;
		}
	}
	
	bWorking = m_pConnection->SetCurrentDirectory(hashtemp);
	m_pConnection->GetCurrentDirectoryW(temp);
	if(bWorking)
	{
		strlog.Format(_T("Defect File Create : IDXUpload(); Current Directory %s Set Successful "),hashtemp); 
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	else
	{
		strlog.Format(_T("Defect File Create : IDXUpload(); Current Directory %s Set fail! "),hashtemp); 
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
		return FALSE;
	}

	AfxExtractSubString(hashtemp,hash,1,'\\');

	cJDIR = hashtemp;
	if (cJDIR.Right(1) != _T("\\")) cJDIR += _T("\\*.*");
	else cJDIR += _T("*.*");
	bWorking = finder->FindFile(cJDIR);

	if(!bWorking)
	{
		ret = RMD(hashtemp); // Remote Make Dir : FTP서버에 Dir 만들기.
		if (ret == FALSE)
		{
			strlog.Format(_T("%s 디렉토리 생성 실패"),hashtemp);
//			delete finder;
			return FALSE;
		}
	}
	
	bWorking = m_pConnection->SetCurrentDirectory(hashtemp);
	m_pConnection->GetCurrentDirectoryW(temp);
	if(bWorking)
	{
		strlog.Format(_T("Defect File Create : IDXUpload(); Current Directory %s Set Successful "),hashtemp); 
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	else
	{
		strlog.Format(_T("Defect File Create : IDXUpload(); Current Directory %s Set fail! "),hashtemp); 
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
		return FALSE;
	}
	temp.Format(_T("%s.IDX"),glassid);
	bUpload = UpLoad(path,temp);
	if(bUpload)
	{
		strlog.Format(_T("Defect File Create : IDXUpload(); %s file Download Successful! "),temp); 
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	//bUpload = UpLoad(_T("C:\\DEFECT_TEMP\\111111A.IDX"),_T("111111A.IDX"));
	else
	{
		strlog.Format(_T("Defect File Create : IDXUpload(); %s file Download fail! "),temp); 
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
//		delete finder;
		return FALSE;
	}
//	delete finder;
	return TRUE;

}

BOOL CFPTClient::DefectFileUpload(CString month,CString day,CString recipe,CString unitid,CString lotid,CString defectfilename,int type)
{
	CFTPFileFind* finder = new CFTPFileFind(m_pConnection);

	CString	cFileName,cFilePath, cFileDate, cLDIR, cRDIR, cJDIR,temp,data;
	CString strlog;
	BOOL bWorking,bUpload,ret;
	strlog.Format(_T("Defect File Create : DefectFileUpload(month : %s,day : %s,recipe : %s,unitid : %s,lotid : %s,defectfilename : %s,type : %d); Start!"), month, day, recipe, unitid, lotid, defectfilename, type); 
	FTPWriteLog(strlog,DEFECT_SERVER_LOG);

	data.Format(_T("%s,%s,%s,%s,%s"),month,day,recipe,unitid,lotid);
	cFilePath.Format(_T("C:\\DEFECT\\INSPECTOR\\%s\\%s\\%s\\%s\\%s\\%s"),month,day,recipe,unitid,lotid,defectfilename);
//	RCD(_T("\\DEFECT"));

	bWorking = m_pConnection->SetCurrentDirectory(_T("DEFECT"));
	m_pConnection->GetCurrentDirectoryW(temp);
	if(bWorking)
	{
		strlog.Format(_T("Defect File Create : DefectFileUpload(); Current Directory Set DEFECT Successful!")); 
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	else
	{
		strlog.Format(_T("Defect File Create : DefectFileUpload(); Current Directory Set DEFECT fail!")); 
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	int i = 0;
	switch(type)
	{
	case DEFECT_INSPECTOR:
		m_pConnection->SetCurrentDirectory(_T("INSPECTOR"));
		m_pConnection->GetCurrentDirectoryW(temp);
		if(bWorking)
		{
			strlog.Format(_T("Defect File Create : DefectFileUpload(); Current Directory Set INSPECTOR Successful!")); 
			FTPWriteLog(strlog,DEFECT_SERVER_LOG);
		}
		else
		{
			strlog.Format(_T("Defect File Create : DefectFileUpload(); Current Directory Set INSPECTOR fail!")); 
			FTPWriteLog(strlog,DEFECT_SERVER_LOG);
		}
		
		while(1)
		{
			if(AfxExtractSubString(temp,data,i,','))
			{
				RMC(temp);
			}
			else
			{
				//bUpload = UpLoad(_T("C:\\DEFECT_FILE_TEMP\\111111D_0_20160617_190900.AAAAAAA"),defectfilename);
				bUpload = UpLoad(cFilePath,defectfilename);
				if(!bUpload)
				{				
					strlog.Format(_T("Defect File Create : DefectFileUpload(); %s Upload fail!"),defectfilename); 
					FTPWriteLog(strlog,DEFECT_SERVER_LOG);
					return FALSE;
				}
				else
				{
					strlog.Format(_T("Defect File Create : DefectFileUpload(); %s Upload Successful!"),defectfilename); 
					FTPWriteLog(strlog,DEFECT_SERVER_LOG);
					return TRUE;
				}			
			}
			i++;
		}
		break;
	case DEFECT_HOST:
		m_pConnection->SetCurrentDirectory(_T("HOST"));
		m_pConnection->GetCurrentDirectoryW(temp);
		if(bWorking)
		{
			strlog.Format(_T("Defect File Create : DefectFileUpload(); Current Directory Set HOST Successful!")); 
			FTPWriteLog(strlog,DEFECT_SERVER_LOG);
		}
		else
		{
			strlog.Format(_T("Defect File Create : DefectFileUpload(); Current Directory Set HOST fail!")); 
			FTPWriteLog(strlog,DEFECT_SERVER_LOG);
		}
		while(1)
		{
			if(AfxExtractSubString(temp,data,i,','))
			{
				RMC(temp);
			}
			else
			{
				//bUpload = UpLoad(_T("C:\\DEFECT_FILE_TEMP\\111111D_0_20160617_190900.AAAAAAA"),defectfilename);
				bUpload = UpLoad(cFilePath,defectfilename);
				if(!bUpload)
				{				
					strlog.Format(_T("Defect File Create : DefectFileUpload(); %s Upload fail!"),defectfilename); 
					FTPWriteLog(strlog,DEFECT_SERVER_LOG);
					return FALSE;
				}
				else
				{
					strlog.Format(_T("Defect File Create : DefectFileUpload(); %s Upload Successful!"),defectfilename); 
					FTPWriteLog(strlog,DEFECT_SERVER_LOG);
					return TRUE;
				}			
			}
			i++;
		}
		break;
	}
	
//	delete finder;
	return FALSE;
}
BOOL CFPTClient::DefectImageIdxUpload(CString month,CString day,CString recipe,CString unitid,CString lotid,CString glassid,CString filename)
{
	CFTPFileFind* finder = new CFTPFileFind(m_pConnection);

	CString	cFileName,cFilePath, cFileDate, cLDIR, cRDIR, cJDIR,temp,data,cLimageidxpath;
	CString strlog;
	BOOL bWorking,bUpload,ret;

	m_pConnection->SetCurrentDirectory(GetImagePath());
	m_pConnection->GetCurrentDirectoryW(temp);

	m_pConnection->SetCurrentDirectory(_T("DEFECT_IMAGE2"));
	m_pConnection->GetCurrentDirectoryW(temp);

	cLDIR.Format(_T("C:\\IMAGE_IDX_TEMP\\%s"),filename);
//	LOTID_GLASSID_RECIPE.TXT
//	cLimageidxpath.Format(_T("C:\\IMAGE\\DEFECT_IMAGE2\\%s\\%s\\%s\\%s\\%s\\%s\\%s_%s_%s.TXT"),month,day,recipe,unitid,lotid,glassid,lotid,glassid,recipe);

	data.Format(_T("%s,%s,%s,%s,%s,%s"),month,day,recipe,unitid,lotid,glassid);

	int i = 0;
	while(1)
	{
		if(AfxExtractSubString(temp,data,i,','))
		{
			RMC(temp);
		}
		else
		{
			bUpload = UpLoad(cLDIR,filename);
			if(!bUpload)
			{
				strlog.Format(_T("%s 업로드 실패!"),filename);
		//		delete finder;
				return FALSE;
			}
	//		delete finder;
			return TRUE;
		}
		i++;
	}
	return TRUE;
}
BOOL CFPTClient::DefectImageUpload(int type,CString month,CString day,CString recipe,CString unitid,CString lotid,CString Lfilepath)
{
	CFTPFileFind* finder = new CFTPFileFind(m_pConnection);

	BOOL rslt = FALSE;

	
	BOOL ret,bUpload;
	CString filename,temp,strlog;

	//filename.Format(_T("06\\22\\RRRRRRR\\AAAAAAA\\11111111\\111111A\\%s_%s_%s.JPG"),recipe,unitid,lotid);

//	m_pConnection->SetCurrentDirectory(_T("IMAGE"));
	m_pConnection->GetCurrentDirectoryW(temp);

	switch(type)
	{
	case DEFECT_IMAGE:
		break;
	case DEFECT_IMAGE2:
		break;
	case REF_IMAGE:
//		m_pConnection->SetCurrentDirectory(_T("REF_IMAGE"));
//		m_pConnection->GetCurrentDirectoryW(temp);
		break;
	case PTNMNT_IMAGE:
		break;
	}
	AfxExtractSubString(filename, Lfilepath, 9, '\\');
	
	if(bUpload = UpLoad(Lfilepath,filename))
	{
		strlog.Format(_T("%s 이미지 파일 업로드 성공!"),filename);
		return TRUE;
	}
	else
	{
		strlog.Format(_T("%s 이미지 파일 업로드 실패!"),filename);
		return FALSE;
	}
}

BOOL CFPTClient::RMC(CString searchdir)//hsc
{
	CFTPFileFind* finder = new CFTPFileFind(m_pConnection);

	CString cJDIR,strlog,temp;
	BOOL bWorking,ret,btest;

//	cJDIR = searchdir;
//	if (cJDIR.Right(1) != _T("\\")) cJDIR += _T("\\*.*");
//	else cJDIR += _T("*.*");
//	bWorking = finder->FindFile(cJDIR);
	bWorking = m_pConnection->SetCurrentDirectory(searchdir);
		
	btest = m_pConnection->GetCurrentDirectoryW(temp);
	if(!bWorking)
	{
		ret = RMD(searchdir); // Remote Make Dir : FTP서버에 Dir 만들기.
		if (ret == FALSE)
		{
			strlog.Format(_T("IDX file search : RMC(); %s Directory Create fail!"),searchdir);
			FTPWriteLog(strlog,DEFECT_SERVER_LOG);
			delete finder;
			return FALSE;
		}
		bWorking = m_pConnection->SetCurrentDirectory(searchdir);
		btest = m_pConnection->GetCurrentDirectoryW(temp);
		if (bWorking == FALSE)
		{
			strlog.Format(_T("IDX file search  : RMC(); %s Directory Set fail!"),searchdir);
			FTPWriteLog(strlog,DEFECT_SERVER_LOG);
			delete finder;
			return FALSE;
		}
	}
	
	strlog.Format(_T("IDX file search : RMC(); %s Directory Set Successful!"),searchdir);
	FTPWriteLog(strlog,DEFECT_SERVER_LOG);

	delete finder;
	return TRUE;
}

BOOL CFPTClient::IDXFileUpdate(CString hash1,CString hash2,CString pathdata,CString glassid)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	char filedir[512];
	CFTPFileFind* finder = new CFTPFileFind(m_pConnection);
	

	CString	cFileName,cFilePath, cFileDate, cLDIR, cRDIR, cJDIR,temp,Rfilepath,temp2,path;
	CString strlog,idxtemp;
	BOOL bWorking,bDownload;

	strlog.Format(_T("IDX file Update : IDXFileUpdate(hash1 : %s,hash2 : %s,pathdata : %s,glassid : %s) Start! "), hash1, hash2, pathdata, glassid);
	FTPWriteLog(strlog,DEFECT_SERVER_LOG);

	idxtemp.Format(_T("%s.IDX"),glassid);

	m_pConnection->SetCurrentDirectory(_T("DEFECT"));
	m_pConnection->GetCurrentDirectoryW(temp);

	m_pConnection->SetCurrentDirectory(_T("INDEX"));
	m_pConnection->GetCurrentDirectoryW(temp);

	bWorking = m_pConnection->SetCurrentDirectory(hash1);
	if(bWorking)
	{
		strlog.Format(_T("IDX file Update : IDXFileUpdate() Directory Set %s Successful!"),hash1);
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	else
	{
		strlog.Format(_T("IDX file Update : IDXFileUpdate() Directory Set %s fail!"),hash1);
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
		return FALSE;
	}

	bWorking = m_pConnection->SetCurrentDirectory(hash2);
	if(bWorking)
	{
		strlog.Format(_T("IDX file Update : IDXFileUpdate() Directory Set %s Successful!"),hash2);
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
		//m_pConnection->OpenFile(idxtemp,GENERIC_WRITE,FTP_TRANSFER_TYPE_BINARY ,1)->Write(pathdata,pathdata.GetLength() + 1);
		//CString strTemp;
		//::AfxExtractSubString(strTemp,idxtemp,0,'.');
		bWorking = m_pConnection->Remove((LPCTSTR)idxtemp);
		//bWorking = FtpDeleteFile(m_pConnection->, idxtemp); 

		if(bWorking)
		{
			
			strlog.Format(_T("IDX file Update : IDXFileUpdate() IDX file %s Delete Successful!"),idxtemp);
			FTPWriteLog(strlog,DEFECT_SERVER_LOG);
			bWorking = UpLoad(pathdata,idxtemp);
			if(bWorking)
			{
				strlog.Format(_T("IDX file Update : IDXFileUpdate() IDX file %s Upload Successful!"),pathdata);
				FTPWriteLog(strlog,DEFECT_SERVER_LOG);
				return TRUE;
			}
			else
			{
				strlog.Format(_T("IDX file Update : IDXFileUpdate() IDX file %s Upload fail!"),pathdata);
				FTPWriteLog(strlog,DEFECT_SERVER_LOG);
				return FALSE;
			}			
		}
		else
		{
			strlog.Format(_T("IDX file Update : IDXFileUpdate() IDX file %s Delete fail!"),idxtemp);
			FTPWriteLog(strlog,DEFECT_SERVER_LOG);
			return FALSE;
		}
	}
	else
	{
		strlog.Format(_T("IDX file Update : IDXFileUpdate() Directory Set %s fail!"),hash2);
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
		return FALSE;
	}
}


BOOL CFPTClient::IDXFileFind(CString hash1,CString hash2,CString idxpath,CString glassid)
{
	CEMIControlDlg * pdlg = (CEMIControlDlg *)AfxGetApp()->m_pMainWnd;
	char filedir[512];
	CFTPFileFind* finder = new CFTPFileFind(m_pConnection);

	CString	cFileName,cFilePath, cFileDate, cLDIR, cRDIR, cJDIR,temp,Rfilepath,temp2,path;
	CString strlog,idxtemp;

	strlog.Format(_T("IDX file search : IDXFileFind(hash1 : %s,hash2 : %s,idxpath : %s,glassid : %s); Start!"),hash1,hash2,idxpath,glassid);
	FTPWriteLog(strlog,DEFECT_SERVER_LOG);
	BOOL bWorking,bDownload;

	bWorking = m_pConnection->SetCurrentDirectory(_T("DEFECT"));
	if(bWorking)
	{
		strlog.Format(_T("IDX file search : Current Directory Set DEFECT Successful!"));
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	else
	{
		strlog.Format(_T("IDX file search : Current Directory Set DEFECT faild!"));
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
		return FALSE;
	}
	m_pConnection->GetCurrentDirectoryW(temp);
	
	

	bWorking = m_pConnection->SetCurrentDirectory(_T("INDEX"));
	if(bWorking)
	{
		strlog.Format(_T("IDX file search : Current Directory Set INDEX Successful!"));
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	else
	{
		strlog.Format(_T("IDX file search : Current Directory Set INDEX faild!"));
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
		return FALSE;
	}
	m_pConnection->GetCurrentDirectoryW(temp);
	

	cJDIR = hash1;
	if (cJDIR.Right(1) != _T("\\")) cJDIR += _T("\\*.*");
	else cJDIR += _T("*.*");
	bWorking = finder->FindFile(cJDIR);
	if(!bWorking)
	{
		strlog.Format(_T("IDX file search : %s Directory Search faild!"),hash1);
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
		return FALSE;
	}
	else
	{
		while (bWorking == TRUE) {
			bWorking = finder->FindNextFile();	// 화일하나를 찾음
			if (finder->IsDots()) continue;	// "."이나  ".." 이면 다음으로 넘어감

			bDownload = m_pConnection->SetCurrentDirectory(hash1);
			if(!bDownload)
			{
				strlog.Format(_T("IDX file search : Current Directory Set %s Successful!"),hash1);
				FTPWriteLog(strlog,DEFECT_SERVER_LOG);
			}
			else
			{
				bDownload = finder->FindFile(_T("*.*"));
				while(bDownload == TRUE)
				{
					bDownload = finder->FindNextFile();	// 화일하나를 찾음
					if (finder->IsDots()) continue;	// "."이나  ".." 이면 다음으로 넘어감
					cFileName = finder->GetFileName();	// 화일이름 얻기
					if(!cFileName.Compare(hash2))
					{
						bDownload = m_pConnection->SetCurrentDirectory(hash2);
						if(bDownload)
						{
							strlog.Format(_T("IDX file search : Current Directory Set %s Successful!"),hash2);
							FTPWriteLog(strlog,DEFECT_SERVER_LOG);
						}
						else
						{
							strlog.Format(_T("IDX file search : Current Directory Set %s fail!"),hash2);
							FTPWriteLog(strlog,DEFECT_SERVER_LOG);
							return FALSE;
						}

						sprintf(filedir, "C:\\DEFECT_TEMP");
						temp.Format(_T("%S"), filedir);

						::CreateDirectoryA(filedir, NULL);
						temp.Format(_T("%s.IDX"),glassid);
						m_idxtemp.Format(_T("C:\\DEFECT_TEMP\\%s.IDX"),glassid);
						
						bDownload = DownLoad(temp,m_idxtemp);
						if(bDownload)
						{
							strlog.Format(_T("IDX file search : %s file Download Successful!"),temp);
							FTPWriteLog(strlog,DEFECT_SERVER_LOG);
							return TRUE;
						}
						else
						{
							strlog.Format(_T("IDX file search : %s.IDX file Download fail!"),temp);
							FTPWriteLog(strlog,DEFECT_SERVER_LOG);				
							return FALSE;
						}
					}
				}
				
			}
			
		}
		return FALSE;
	}
}

BOOL CFPTClient::DefectFileFind(CString month,CString day,CString recipe,CString unitid,CString lotid,CString defectfilename)
{
	CFTPFileFind* finder = new CFTPFileFind(m_pConnection);

	CString	cFileName,cFilePath, cFileDate, cLDIR, cRDIR, cJDIR,temp,Rfilepath,temp2,path,cLtemppath;
	CString cdfname0,cdfname1,cdfname2,cdfname3,cdfname4;
	
	CString strlog,idxtemp,strTemp,strTemp2;
	BOOL bWorking,bDownload,bRet;
	strlog.Format(_T("IDX file search : DefectFileFind(month : %s,day : %s,recipe : %s,unitid : %s,lotid : %s,defectfilename : %s); Start!"),month,day,recipe,unitid,lotid,defectfilename);
	FTPWriteLog(strlog,DEFECT_SERVER_LOG);

	bWorking = m_pConnection->SetCurrentDirectory(_T("DEFECT"));
	if(bWorking)
	{
		strlog.Format(_T("IDX file search : DefectFileFind(); Current Directory Set DEFECT Successful!"));
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	else
	{
		strlog.Format(_T("IDX file search : DefectFileFind();  Current Directory Set DEFECT fail!"));
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
		return FALSE;
	}
	
	bWorking = m_pConnection->GetCurrentDirectoryW(temp);

	bWorking = m_pConnection->SetCurrentDirectory(_T("INSPECTOR"));
	if(bWorking)
	{
		strlog.Format(_T("IDX file search : DefectFileFind();  Current Directory Set INSPECTOR Successful!"));
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
	}
	else
	{
		strlog.Format(_T("IDX file search : DefectFileFind();  Current Directory Set INSPECTOR fail!"));
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
		return FALSE;
	}

	bWorking = m_pConnection->GetCurrentDirectoryW(temp);
	AfxExtractSubString(cdfname0,defectfilename,0,'_');
	AfxExtractSubString(cdfname1,defectfilename,1,'_');
	AfxExtractSubString(cdfname2,defectfilename,2,'_');
	AfxExtractSubString(temp,defectfilename,3,'_');
	AfxExtractSubString(cdfname3,temp,0,'.');
	AfxExtractSubString(cdfname4,defectfilename,1,'.');
//	cLtemppath.Format(_T("C:\\DEFECT_FILE_TEMP\\%s"),defectfilename);
//	cFileName.Format(_T("%s/%s/%s/%s/%s/%s"),month,day,recipe,unitid,lotid,defectfilename);
//	bDownload = DownLoad(cFileName,cLtemppath);
	
	cJDIR = month;
	if (cJDIR.Right(1) != _T("\\")) cJDIR += _T("\\*.*");
	else cJDIR += _T("*.*");
	bWorking = finder->FindFile(cJDIR);
	if(!bWorking)
	{
		strlog.Format(_T("IDX file search : DefectFileFind();  %s Directory Search fail!"),month);
		FTPWriteLog(strlog,DEFECT_SERVER_LOG);
		return FALSE;
	}
	else
	{
		bRet = m_pConnection->SetCurrentDirectory(month);
		if(!bRet)
		{
			if(bRet = RMC(month))
			{
				strlog.Format(_T("IDX file search : DefectFileFind();  Current Directory Set %s Successful!"),month);
				FTPWriteLog(strlog,DEFECT_SERVER_LOG);
			}
			else
			{
				strlog.Format(_T("IDX file search : DefectFileFind();  Current Directory Set %s fail!"),month);
				FTPWriteLog(strlog,DEFECT_SERVER_LOG);
				return FALSE;
			}
		}
		bRet = m_pConnection->SetCurrentDirectory(day);
		if(!bRet)
		{
			if(bRet = RMC(day))
			{
				strlog.Format(_T("IDX file search : DefectFileFind();  Current Directory Set %s Successful!"),day);
				FTPWriteLog(strlog,DEFECT_SERVER_LOG);
			}
			else
			{
				strlog.Format(_T("IDX file search : DefectFileFind();  Current Directory Set %s fail!"),day);
				FTPWriteLog(strlog,DEFECT_SERVER_LOG);
				return FALSE;
			}
		}
	//	bDownload = m_pConnection->GetCurrentDirectoryW(temp);
		while (bRet == TRUE) {
			cJDIR = recipe;
			if(cJDIR.Right(1) != _T("\\")) cJDIR += _T("\\*.*");
			else cJDIR += _T("*.*");
			if(bDownload = finder->FindFile(cJDIR))
			{
				if(bDownload = m_pConnection->SetCurrentDirectory(recipe))
				{
					strlog.Format(_T("IDX file search : DefectFileFind();  Current Directory Set %s Successful!"),recipe);
					FTPWriteLog(strlog,DEFECT_SERVER_LOG);
				}
				else
				{
					strlog.Format(_T("IDX file search : DefectFileFind();  Current Directory Set %s fail!"),recipe);
					FTPWriteLog(strlog,DEFECT_SERVER_LOG);
					return FALSE;
				}
				
//				bDownload = m_pConnection->GetCurrentDirectory(temp);
				cJDIR = unitid;
				if(cJDIR.Right(1) != _T("\\")) cJDIR += _T("\\*.*");
				else cJDIR += _T("*.*");
				if(bDownload = finder->FindFile(cJDIR))				
				{
					bDownload = m_pConnection->SetCurrentDirectory(unitid);
					cJDIR = lotid;
					if(cJDIR.Right(1) != _T("\\")) cJDIR += _T("\\*.*");
					else cJDIR += _T("*.*");
					
					if(bDownload = m_pConnection->SetCurrentDirectory(lotid))
					{
						strlog.Format(_T("IDX file search : DefectFileFind();  Current Directory Set %s Successful!"),lotid);
						FTPWriteLog(strlog,DEFECT_SERVER_LOG);
					}
					else
					{
						strlog.Format(_T("IDX file search : DefectFileFind();  Current Directory Set %s fail!"),lotid);
						FTPWriteLog(strlog,DEFECT_SERVER_LOG);
						return FALSE;
					}
					bDownload = m_pConnection->GetCurrentDirectoryW(temp);
					if(bDownload = finder->FindFile(defectfilename))//defectfilename))
					{
						m_temppath.Format(_T("C:\\DEFECT_FILE_TEMP\\%s_%s_%s_%s.XML"),cdfname0,cdfname1,cdfname2,cdfname3);//_%s_%s_%s.XML"),cdfname0,cdfname1,cdfname2,cdfname3);//%s_%s_%s_%s.%s"),cdfname0,cdfname1,cdfname2,cdfname3,cdfname4);
						::CreateDirectoryW(_T("C:\\DEFECT_FILE_TEMP"), NULL);
						bDownload = DownLoad(defectfilename,m_temppath);
						if(bDownload)
						{
							strlog.Format(_T("IDX file search : DefectFileFind();  DEFECT FILE %s Download Successful!"),defectfilename);
							FTPWriteLog(strlog,DEFECT_SERVER_LOG);
							return TRUE;
						}
						else
						{
							strlog.Format(_T("IDX file search : DefectFileFind();  DEFECT FILE %s Download fail!"),defectfilename);
							FTPWriteLog(strlog,DEFECT_SERVER_LOG);
							return FALSE;
						}
					}
					else
					{
						strlog.Format(_T("IDX file search : DefectFileFind();  DEFECT FILE %s Search fail!"),defectfilename);
						FTPWriteLog(strlog,DEFECT_SERVER_LOG);
						return FALSE;
					}


					/*while(TRUE)
					{
						
						bDownload = finder->FindNextFile();	// 화일하나를 찾음
						if (finder->IsDots()) continue;	// "."이나  ".." 이면 다음으로 넘어감
						if(bDownload)
						{
						}
						else
						{
							strlog.Format(_T("FindNextFile() 실패 : %s 파일 검색 실패"),defectfilename);
							return FALSE;
						}
							
						cFileName = finder->GetFileName();
						AfxExtractSubString(strTemp,cFileName,0,'.');
		//				strTemp.Format(_T("%s"),cFileName);
						AfxExtractSubString(strTemp2,defectfilename,0,'.');
						if(strTemp == strTemp2)
						{
							
							
							
						}
														
					}*/
				}

			}
				
		}
		return FALSE;
	}
}

CString CFPTClient::Getidxtemp()
{
	return m_idxtemp;
}
/*
BOOL CFPTClient::IDXFindFile(CString hash1,CString hash2,CString idxpath)
{
	CFTPFileFind* finder = new CFTPFileFind(m_pConnection);

	CString	cFileName,cFilePath, cFileDate, cLDIR, cRDIR, cJDIR,temp;
	CString cJobDirName;
	BOOL	SubDirJob = FALSE;
	BOOL	bRet = FALSE;
	CTime time;
	unsigned long size;
	BOOL bWorking,bDownload = FALSE;
	CString Lfilepath,Rfilepath;

	cJDIR = hash1;
	if (cJDIR.Right(1) != _T("\\")) cJDIR += _T("\\*.*");
	else cJDIR += _T("*.*");

	bWorking = finder->FindFile(cJDIR);	// 모든화일
	m_pConnection->GetCurrentDirectoryW(temp);
	bRet = m_pConnection->SetCurrentDirectory(temp);
	while (bRet == TRUE) {
		if (temp.Right(1) != _T("/")) temp += _T("/*.*");
		else temp += _T("*.*");
		bWorking = finder->FindFile(temp);
		bWorking = finder->FindNextFile();	// 화일하나를 찾음
		if (finder->IsDots()) continue;	// "."이나  ".." 이면 다음으로 넘어감

		// 화일이름
		cFileName = finder->GetFileName();	// 화일이름 얻기

		Rfilepath = finder->GetFilePath();
		
		bDownload = DownLoad(cFileName,_T("C:\\DEFECT_TEMP\\111111A.IDX"));
		if (SubDirJob == TRUE) {
			if (cJobDirName != cFileName) continue;
			else {
				SubDirJob = FALSE;
				continue;
			}
		}
		if(!cFileName.Compare(hash2))
		{
			
			cFilePath = finder->GetFilePath();
			if (idxpath.Right(1) != _T("\\")) idxpath += _T("\\");
			//else cJDIR += _T("*.*");
			idxpath += cFilePath;
		//	idxpath.Replace(_T("/"),_T("\\"));
			if (idxpath.Right(1) != _T("\\")) idxpath += _T("\\*.*");
			else idxpath += _T("*.*");
			idxpath.Replace(_T("\\"),_T("/"));
			bDownload = DownLoad(_T("/INDEX/00000017/00000011/111111A.IDX"),_T("C:\\DEFECT_TEMP\\111111A.IDX"));
			delete finder;
			return TRUE;
		}
		
	}
	delete finder;
	return FALSE;
}
*/
BOOL CFPTClient::DownLoadDir(CString rDir, CString lDir)
{
	CFTPFileFind* finder = new CFTPFileFind(m_pConnection);

	CString	cFileName, cFileDate, cLDIR, cRDIR, cJDIR;
	CString cJobDirName;
	BOOL	SubDirJob = FALSE;
	CTime time;
	unsigned long size;
	BOOL bWorking;

	cJDIR = rDir;
	if (cJDIR.Right(1) != _T("\\")) cJDIR += _T("\\*.*");
	else cJDIR += _T("*.*");

	bWorking = finder->FindFile(cJDIR);	// 모든화일

	while (bWorking == TRUE) {
		bWorking = finder->FindNextFile();	// 화일하나를 찾음
		if (finder->IsDots()) continue;	// "."이나  ".." 이면 다음으로 넘어감

		// 화일이름
		cFileName = finder->GetFileName();	// 화일이름 얻기

		if (SubDirJob == TRUE) {
			if (cJobDirName != cFileName) continue;
			else {
				SubDirJob = FALSE;
				continue;
			}
		}

		cLDIR = lDir;
		if(cLDIR.Right(1) != _T("\\"))
			cLDIR += _T("\\");
		cLDIR += cFileName;

		cRDIR = rDir;
		if(cRDIR.Right(1) != _T("/"))
			cRDIR += _T("/");
		cRDIR += cFileName;

		if (finder->IsDirectory()) {
			LMD(cLDIR);
			cJobDirName = cFileName;
			SubDirJob = TRUE;
			delete finder;
			finder = NULL;
			DownLoadDir(cRDIR, cLDIR);
			finder = new CFTPFileFind(m_pConnection);
			bWorking = finder->FindFile(cJDIR);//, INTERNET_FLAG_EXISTING_CONNECT);	// 모든화일
			if (bWorking == FALSE) break;
		} else {
			size = finder->GetLength();		// 화일크기
			finder->GetLastWriteTime(time); // 화일날짜
			cFileDate = time.Format(_T("%y%m%d %H%M"));

			if (!DownLoad(cRDIR, cLDIR)) {
				delete finder;
				return FALSE;
			}
		}
	}

	delete finder;
	return TRUE;
}


BOOL CFPTClient::UpLoadDir(CString LocalDir, CString RemoteDir, BOOL bUpdatedata, BOOL isUpLoadFIleDel)
{
	CFileFind finder;
	CString	cFileName, cFileDate, csLocalDir, csRemoteDir;
	CTime time;
	unsigned long size;
	BOOL bFind;
	char cFilePath[1024], cDelLocalDir[1024];
	
	CString log;

	csLocalDir = LocalDir;
	if (csLocalDir.Right(1) != _T("\\")) csLocalDir += _T("\\*.*");
	else csLocalDir += _T("*.*");

	bFind = finder.FindFile(csLocalDir); // 모든화일

	if(!finder.FindFile(csLocalDir)) 
	{
		finder.Close();
		return 0;
	}

	while (bFind == TRUE) {
		bFind = finder.FindNextFile();		// 화일하나를 찾음
		if (finder.IsDots()) continue;		// "."이나  ".." 이면 다음으로 넘어감
		cFileName = finder.GetFileName();	// 화일이름 얻기

		csLocalDir = LocalDir;
		if (csLocalDir.Right(1) != _T("\\")) csLocalDir += _T("\\");
		csLocalDir += cFileName;

		csRemoteDir = RemoteDir;
		if (csRemoteDir.Right(1) != _T("\\")) csRemoteDir += _T("\\");
		csRemoteDir += cFileName;
		//csRemoteDir.Replace(_T("\\"), _T("/"));
 
		sprintf(cFilePath, "%S", RemoteDir);

		if (finder.IsDirectory()) {
			
			if (!Make_FtpPath(cFilePath)) {
//				logmsg(LT_ERROR, "[DEFECT FILE] UpLoadDir >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> !Make_FtpPath(csRemoteDir) 1");
		//		log.Format(_T("[DEFECT FILE] UpLoadDir >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> !Make_FtpPath(csRemoteDir) 1"));
				return FALSE;
			}
			if (!UpLoadDir(csLocalDir, csRemoteDir, bUpdatedata, isUpLoadFIleDel)) {
//				logmsg(LT_ERROR, "[DEFECT FILE] UpLoadDir >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> !UpLoadDir(csLocalDir, csRemoteDir, bUpdatedata) 1");
		//		log.Format(_T("[DEFECT FILE] UpLoadDir >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> !UpLoadDir(csLocalDir, csRemoteDir, bUpdatedata) 1"));
				return FALSE;
			}
			::RemoveDirectory(csLocalDir);
		} else {
			if (!Make_FtpPath(cFilePath)) {
//				logmsg(LT_ERROR, "[DEFECT FILE] UpLoadDir >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> !Make_FtpPath(csRemoteDir) 2");
				
			//	log.Format(_T("[DEFECT FILE] UpLoadDir >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> !Make_FtpPath(csRemoteDir) 2"));
				return FALSE;
			}
			size = finder.GetLength();	   // 화일크기
			finder.GetLastWriteTime(time); // 화일날짜
			cFileDate = time.Format(_T("%y%m%d %H%M"));

			//////////////////////////////////////////////////////////
			if (bUpdatedata) { // DownLoad 후 File-Update 한다.
				CString csFTPfile, csTempFile;
	//			csTempFile.Format(_T("%s%s"), LOACL_DEFECT_PATH, csRemoteDir);
				//csTempFile.Replace(_T("/"), _T("\\"));
				make_path((LPSTR)(LPCTSTR)csTempFile);
				::remove((LPSTR)(LPCTSTR)csTempFile);
				::RemoveDirectory(csTempFile);
				csFTPfile = csRemoteDir;
				/*csFTPfile.Replace( _T("\\"),_T("/") );
				csFTPfile.Replace( _T("//"), _T("/") );*/

				if (DownLoad(csFTPfile, csTempFile)) 
				{
					DownLoad(csFTPfile, csTempFile);
					FILE *fp_Ftp, *fp_Add;

					if ((fp_Ftp = fopen((LPSTR)(LPCTSTR)csTempFile,"at")) != NULL) {
						if ((fp_Add=fopen((LPSTR)(LPCTSTR)csLocalDir, "rt")) != NULL) {
							char *buff;
							fseek(fp_Add,0,SEEK_END);	// 파일의 크기를 알고.
							size=ftell(fp_Add);
							buff=new char[size];	// 해당크기만큼 buff를 메모리에 설정.
							memset(buff, 0, size);
							fseek(fp_Add, 0, SEEK_SET);	// 데이터를 읽는다.
							fread(buff, size, 1, fp_Add);
							fclose(fp_Add);

							fprintf(fp_Ftp, buff);
							fclose(fp_Ftp);	

							remove((LPSTR)(LPCTSTR)csLocalDir);
							MoveFile(csTempFile , csLocalDir);
							delete buff;
						} else fclose(fp_Ftp);	
					}								
				}
			}
			//////////////////////////////////////////////////////////
			
			if (UpLoad(csLocalDir, csRemoteDir)) {
				sprintf(cDelLocalDir, "%S", csLocalDir);
				if(isUpLoadFIleDel)
				{
					remove(cDelLocalDir);
					::RemoveDirectory(csLocalDir);
				}
			} else {
//				logmsg(LT_ERROR, "[DEFECT FILE] UpLoadDir >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> !UpLoad(csLocalDir, csRemoteDir)");
				log.Format(_T("[DEFECT FILE] UpLoadDir >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> !UpLoad(csLocalDir, csRemoteDir)"));
			//	testdlg->ListLog(log);
				return FALSE;
			}
		}
	}

	finder.Close();

	return TRUE;
}

BOOL CFPTClient::Make_FtpPath(const char *path)
{
	BOOL ret;
	std::string search_p, org_p;
	CString temp;
	
	int last, dc_pos;
	org_p = path;
	last = 0;

	while ((dc_pos = org_p.find_first_of( '\\', last)) > 0) {
		search_p = org_p.substr(0, dc_pos);
		temp.Format(_T("%S"), search_p.c_str());
		//ret = RCD(temp); // Remote Change Dir : FTP서버 현재-Dir 변경.

		//if (ret == FALSE) { // 만약 Dir를 변경할 수 없다면
			ret = RMD(temp); // Remote Make Dir : FTP서버에 Dir 만들기.
			if (ret == FALSE) return FALSE;
		//}

		last = search_p.size() + 2;
		RCD(m_strRemoteDir);
	}

	RCD(m_strRemoteDir); // Remote Change Dir
	return TRUE;
}

// Remote Change Dir
BOOL CFPTClient::RCD(CString RemoteDir)
{
	if (m_pConnection == NULL) return FALSE;

	try {
		BOOL brc; CString a;
		brc = m_pConnection->GetCurrentDirectory(a);
		brc = m_pConnection->SetCurrentDirectory(RemoteDir.GetBuffer(0));
		m_pConnection->GetCurrentDirectory(a);
		if (brc == FALSE) {
			TRACE("RCD ERROR : %s (%s) >< %d \n", m_strAddress, RemoteDir, ::GetLastError());
			GetErrorMessage();
			return FALSE;
		} else
			m_strRemoteDirText.Format(_T("%s"),RemoteDir);
	} catch(CInternetException* ee) {
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RCD ERROR : %s (%s) >< %s \n", m_strAddress, RemoteDir, buff);
		ee->Delete();
		return FALSE;
	}

	return TRUE;
}

// Local Change Dir
BOOL CFPTClient::LCD(CString LocalDir)
{
	if (::SetCurrentDirectory(LocalDir) == FALSE) {
		GetErrorMessage();
		return FALSE;
	} else m_strLocalDirText = LocalDir;
	return TRUE;
}

// Remote Make Dir
BOOL CFPTClient::RMD(CString RemoteDir)
{
	if (m_pConnection == NULL) return FALSE;

	try {
		BOOL brc; CString a;
		brc = m_pConnection->GetCurrentDirectory(a);
		brc = m_pConnection->CreateDirectory(RemoteDir.GetBuffer(0));

		if (brc == FALSE) {
			TRACE("RMD ERROR : %s (%s) >< %d \n", m_strAddress, RemoteDir, ::GetLastError());
			GetErrorMessage();
			return FALSE;
		} else
			m_strRemoteDirText = RemoteDir;
	} catch(CInternetException* ee) {
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RMD ERROR : %s (%s) >< %s \n", m_strAddress, RemoteDir, buff);
		ee->Delete();
		return FALSE;
	}

	return TRUE;
}

// Local Make Dir
BOOL CFPTClient::LMD(CString LocalDir)
{
	return ::CreateDirectory(LocalDir, NULL);
}

// Remove Remote Dir
BOOL CFPTClient::RRD(CString RemoteDir)
{
	if (m_pConnection == NULL) return FALSE;

	try {
		BOOL brc;
		brc = m_pConnection->RemoveDirectory((LPCTSTR)RemoteDir);
		if(brc == FALSE){
			TRACE("RrD ERROR : %s (%s) >< %d \n", m_strAddress, RemoteDir, ::GetLastError());
			GetErrorMessage();
			return FALSE;
		}
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RrD ERROR : %s (%s) >< %s \n", m_strAddress, RemoteDir, buff);
		ee->Delete();
		return FALSE;
	}
	return TRUE;
}

// Local Remove Dir
BOOL CFPTClient::LRD(CString LocalDir)
{
	return ::RemoveDirectory(LocalDir);
}

BOOL CFPTClient::RDel(CString RemoteFile)
{
	if(m_pConnection == NULL) return FALSE;
	try
	{
		BOOL brc;
		brc = m_pConnection->Remove((LPCTSTR)RemoteFile);
		if(brc == FALSE){
			TRACE("RDel ERROR : %s (%s) >< %d \n", m_strAddress, RemoteFile, ::GetLastError());
			GetErrorMessage();
			return FALSE;
		}
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RDel ERROR : %s (%s) >< %s \n", m_strAddress, RemoteFile, buff);
		ee->Delete();
		return FALSE;
	}
	return TRUE;
}

BOOL CFPTClient::LDel(CString LocalFile)
{
	return ::DeleteFile(LocalFile);
}

BOOL CFPTClient::RRen(CString RemoteOldFile, CString RemoteNewFile)
{
	if(m_pConnection == NULL) return FALSE;
	try
	{
		BOOL brc;
		brc = m_pConnection->Rename((LPCTSTR)RemoteOldFile,(LPCTSTR)RemoteNewFile);
		if(brc == FALSE){
			TRACE("RRen ERROR : %s (%s) %s -> %d \n", m_strAddress, RemoteOldFile, RemoteNewFile, ::GetLastError());
			GetErrorMessage();
			return FALSE;
		}
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RRen ERROR : %s (%s) %s -> %s \n", m_strAddress, RemoteOldFile, RemoteNewFile, buff);
		ee->Delete();
		return FALSE;
	}
	return TRUE;
}

BOOL CFPTClient::LRen(CString LocalOldFile, CString LocalNewFile)
{
	remove((LPSTR)(LPCTSTR)LocalNewFile);
	return ::MoveFile(LocalOldFile, LocalNewFile);
}

CString CFPTClient::RCD()
{
	RPWD();
	return m_strRemoteDirText;
}

CString CFPTClient::LCD()
{
	LPWD();
	return m_strLocalDirText;
}

BOOL CFPTClient::RPWD()
{
	if (m_pConnection == NULL) return FALSE;

	try {
		BOOL brc;
		brc = m_pConnection->GetCurrentDirectory(m_strRemoteDirText);
		if (brc == FALSE) {
			TRACE("RPWD ERROR : %s -> %d \n", m_strAddress, ::GetLastError());
			GetErrorMessage();
			return FALSE;
		}
	} catch(CInternetException* ee) {
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("RPWD ERROR : %s -> %s \n", m_strAddress, buff);
		ee->Delete();
		return FALSE;
	}
	return TRUE;
}

BOOL CFPTClient::LPWD()
{
	char buff[256];
	::GetCurrentDirectory(255, (LPTSTR)buff);
	m_strLocalDirText = buff;
	return TRUE;
}

BOOL CFPTClient::GetURL(CString &RemoteURL)
{
	if(m_pConnection == NULL) return FALSE;
	try
	{
		BOOL brc;
		brc = m_pConnection->GetCurrentDirectoryAsURL(RemoteURL);
		if(brc == FALSE){
			TRACE("GetURL ERROR : %s -> %d \n", m_strAddress, ::GetLastError());
			GetErrorMessage();
			return FALSE;
		}
	}
	catch(	CInternetException* ee)
	{
		char buff[256];
		ee->GetErrorMessage((LPTSTR)buff, 255);
		TRACE("GetURL ERROR : %s -> %s \n", m_strAddress, buff);
		ee->Delete();
		return FALSE;
	}

	return TRUE;
}

void CFPTClient::GetErrorMessage()
{
	char *szRcode;
	int rCode;

	CString listlog;

	rCode = ::GetLastError();
	switch(rCode) {
		 case ERROR_ACCESS_DENIED          :    szRcode = "ERROR_ACCESS_DENIED          ";    break;
		 case ERROR_ADAP_HDW_ERR           :    szRcode = "ERROR_ADAP_HDW_ERR           ";    break;
		 case ERROR_ALREADY_ASSIGNED       :    szRcode = "ERROR_ALREADY_ASSIGNED       ";    break;
		 case ERROR_ARENA_TRASHED          :    szRcode = "ERROR_ARENA_TRASHED          ";    break;
		 case ERROR_BAD_COMMAND            :    szRcode = "ERROR_BAD_COMMAND            ";    break;
		 case ERROR_BAD_DEV_TYPE           :    szRcode = "ERROR_BAD_DEV_TYPE           ";    break;
		 case ERROR_BAD_ENVIRONMENT        :    szRcode = "ERROR_BAD_ENVIRONMENT        ";    break;
		 case ERROR_BAD_FORMAT             :    szRcode = "ERROR_BAD_FORMAT             ";    break;
		 case ERROR_BAD_LENGTH             :    szRcode = "ERROR_BAD_LENGTH             ";    break;
		 case ERROR_BAD_NET_NAME           :    szRcode = "ERROR_BAD_NET_NAME           ";    break;
		 case ERROR_BAD_NET_RESP           :    szRcode = "ERROR_BAD_NET_RESP           ";    break;
		 case ERROR_BAD_NETPATH            :    szRcode = "ERROR_BAD_NETPATH            ";    break;
		 case ERROR_BAD_REM_ADAP           :    szRcode = "ERROR_BAD_REM_ADAP           ";    break;
		 case ERROR_BAD_UNIT               :    szRcode = "ERROR_BAD_UNIT               ";    break;
		 case ERROR_CANNOT_MAKE            :    szRcode = "ERROR_CANNOT_MAKE            ";    break;
		 case ERROR_CRC                    :    szRcode = "ERROR_CRC                    ";    break;
		 case ERROR_CURRENT_DIRECTORY      :    szRcode = "ERROR_CURRENT_DIRECTORY      ";    break;
		 case ERROR_DEV_NOT_EXIST          :    szRcode = "ERROR_DEV_NOT_EXIST          ";    break;
		 case ERROR_DUP_NAME               :    szRcode = "ERROR_DUP_NAME               ";    break;
		 case ERROR_FAIL_I24               :    szRcode = "ERROR_FAIL_I24               ";    break;
		 case ERROR_FILE_EXISTS            :    szRcode = "ERROR_FILE_EXISTS            ";    break;
		 case ERROR_FILE_NOT_FOUND         :    szRcode = "ERROR_FILE_NOT_FOUND         ";    break;
		 case ERROR_GEN_FAILURE            :    szRcode = "ERROR_GEN_FAILURE            ";    break;
		 case ERROR_HANDLE_DISK_FULL       :    szRcode = "ERROR_HANDLE_DISK_FULL       ";    break;
		 case ERROR_HANDLE_EOF             :    szRcode = "ERROR_HANDLE_EOF             ";    break;
		 case ERROR_INVALID_ACCESS         :    szRcode = "ERROR_INVALID_ACCESS         ";    break;
		 case ERROR_INVALID_BLOCK          :    szRcode = "ERROR_INVALID_BLOCK          ";    break;
		 case ERROR_INVALID_DATA           :    szRcode = "ERROR_INVALID_DATA           ";    break;
		 case ERROR_INVALID_DRIVE          :    szRcode = "ERROR_INVALID_DRIVE          ";    break;
		 case ERROR_INVALID_FUNCTION       :    szRcode = "ERROR_INVALID_FUNCTION       ";    break;
		 case ERROR_INVALID_HANDLE         :    szRcode = "ERROR_INVALID_HANDLE         ";    break;
		 case ERROR_INVALID_PARAMETER      :    szRcode = "ERROR_INVALID_PARAMETER      ";    break;
		 case ERROR_INVALID_PASSWORD       :    szRcode = "ERROR_INVALID_PASSWORD       ";    break;
		 case ERROR_LOCK_VIOLATION         :    szRcode = "ERROR_LOCK_VIOLATION         ";    break;
		 case ERROR_NET_WRITE_FAULT        :    szRcode = "ERROR_NET_WRITE_FAULT        ";    break;
		 case ERROR_NETNAME_DELETED        :    szRcode = "ERROR_NETNAME_DELETED        ";    break;
		 case ERROR_NETWORK_ACCESS_DENIED  :    szRcode = "ERROR_NETWORK_ACCESS_DENIED  ";    break;
		 case ERROR_NETWORK_BUSY           :    szRcode = "ERROR_NETWORK_BUSY           ";    break;
		 case ERROR_NO_MORE_FILES          :    szRcode = "ERROR_NO_MORE_FILES          ";    break;
		 case ERROR_NO_PROC_SLOTS          :    szRcode = "ERROR_NO_PROC_SLOTS          ";    break;
		 case ERROR_NO_SPOOL_SPACE         :    szRcode = "ERROR_NO_SPOOL_SPACE         ";    break;
		 case ERROR_NOT_DOS_DISK           :    szRcode = "ERROR_NOT_DOS_DISK           ";    break;
		 case ERROR_NOT_ENOUGH_MEMORY      :    szRcode = "ERROR_NOT_ENOUGH_MEMORY      ";    break;
		 case ERROR_NOT_READY              :    szRcode = "ERROR_NOT_READY              ";    break;
		 case ERROR_NOT_SAME_DEVICE        :    szRcode = "ERROR_NOT_SAME_DEVICE        ";    break;
		 case ERROR_NOT_SUPPORTED          :    szRcode = "ERROR_NOT_SUPPORTED          ";    break;
		 case ERROR_OUT_OF_PAPER           :    szRcode = "ERROR_OUT_OF_PAPER           ";    break;
		 case ERROR_OUT_OF_STRUCTURES      :    szRcode = "ERROR_OUT_OF_STRUCTURES      ";    break;
		 case ERROR_OUTOFMEMORY            :    szRcode = "ERROR_OUTOFMEMORY            ";    break;
		 case ERROR_PATH_NOT_FOUND         :    szRcode = "ERROR_PATH_NOT_FOUND         ";    break;
		 case ERROR_PRINT_CANCELLED        :    szRcode = "ERROR_PRINT_CANCELLED        ";    break;
		 case ERROR_PRINTQ_FULL            :    szRcode = "ERROR_PRINTQ_FULL            ";    break;
		 case ERROR_READ_FAULT             :    szRcode = "ERROR_READ_FAULT             ";    break;
		 case ERROR_REDIR_PAUSED           :    szRcode = "ERROR_REDIR_PAUSED           ";    break;
		 case ERROR_REM_NOT_LIST           :    szRcode = "ERROR_REM_NOT_LIST           ";    break;
		 case ERROR_REQ_NOT_ACCEP          :    szRcode = "ERROR_REQ_NOT_ACCEP          ";    break;
		 case ERROR_SECTOR_NOT_FOUND       :    szRcode = "ERROR_SECTOR_NOT_FOUND       ";    break;
		 case ERROR_SEEK                   :    szRcode = "ERROR_SEEK                   ";    break;
		 case ERROR_SHARING_BUFFER_EXCEEDED:    szRcode = "ERROR_SHARING_BUFFER_EXCEEDED";    break;
		 case ERROR_SHARING_PAUSED         :    szRcode = "ERROR_SHARING_PAUSED         ";    break;
		 case ERROR_SHARING_VIOLATION      :    szRcode = "ERROR_SHARING_VIOLATION      ";    break;
		 case ERROR_TOO_MANY_CMDS          :    szRcode = "ERROR_TOO_MANY_CMDS          ";    break;
		 case ERROR_TOO_MANY_NAMES         :    szRcode = "ERROR_TOO_MANY_NAMES         ";    break;
		 case ERROR_TOO_MANY_OPEN_FILES    :    szRcode = "ERROR_TOO_MANY_OPEN_FILES    ";    break;
		 case ERROR_TOO_MANY_SESS          :    szRcode = "ERROR_TOO_MANY_SESS          ";    break;
		 case ERROR_UNEXP_NET_ERR          :    szRcode = "ERROR_UNEXP_NET_ERR          ";    break;
		 case ERROR_WRITE_FAULT            :    szRcode = "ERROR_WRITE_FAULT            ";    break;
		 case ERROR_WRITE_PROTECT          :    szRcode = "ERROR_WRITE_PROTECT          ";    break;
		 case ERROR_WRONG_DISK             :    szRcode = "ERROR_WRONG_DISK             ";    break;
          
		 default 			               :    szRcode = "DEFAULT                      ";    break;
	}           
	TRACE("CFPTClient GetLastError() Errocode = %s [%d] \n", szRcode, rCode);
	listlog.Format(_T("CFTPFileFind GetLastError() Errocode = %S [%d]"),szRcode,rCode);
	//testdlg->ListLog(listlog);
	return;
}

int CFPTClient::make_path(const char *path)
{
	std::string search_p, org_p;
	int last, dc_pos;

	org_p = path;
	last = 0;

	while ((dc_pos = org_p.find_first_of('\\', last)) > 0) {
		search_p = org_p.substr(0, dc_pos);
	
		if (_access(search_p.data(), 0) < 0) {
			if (CreateDirectory((LPCTSTR)(LPSTR)search_p.data(), NULL) == FALSE) return -1;
		}

		last = search_p.size() + 2;
	}

	return 0;		
}

BOOL CFPTClient::FTPWriteLog(CString text,int log_type)
{
	FILE*	stream;
	char	strFile[MAX_PATH], strToFile[MAX_PATH];
    long	result, line;
	char	szTime[DAT_STR_LENG]; 
	CString bak_file;
	int limit_line = 500000;
	CString path;
	CString strTime;
	CString log,strLogType;
	char buffer[DAT_STR_LENG];

	

	switch(log_type)
	{
	case DEFECT_SERVER_LOG:
		strLogType = DEFECT_SERVER_LOG_FILE_NAME;
		break;
	}

	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);
	strTime.Format(_T("%04d%02d%02d"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
	log.Format(_T("%04d-%02d-%02d-%02d:%02d:%02d:%03d->%s"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay, lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond, lpSystemTime.wMilliseconds, text);

	sprintf(strFile, "D:\\DATABASE\\LOG\\%S\\%S_%S", strTime, strTime, strLogType);

	_MakeDirectories(_T("D:\\DATABASE\\LOG\\") + strTime);

	if((stream = _fsopen(strFile, "a+", _SH_DENYNO)) == NULL)
	{
		HANDLE fd = CreateFile( (LPCWSTR)(LPCSTR)strFile,
							GENERIC_READ|GENERIC_WRITE,
							//GENERIC_WRITE,
							FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);

		stream = _fsopen(strFile, "a+", _SH_DENYNO);
		if(stream == NULL)
		{
			CloseHandle(fd);
			return FALSE;
		}

		CloseHandle(fd);
	}

	//Check first time
    result = fseek(stream, 0L, SEEK_SET);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}
	line = 0;
	while(fgets(buffer, DAT_STR_LENG, stream)) 
	{
		line++;
		if(1 < line)
		{
			break;
		}
	}

    result = fseek(stream, 0L, SEEK_END);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}

	memset(szTime, NULL, sizeof(szTime));
	sprintf(szTime,"%S",log);// log.GetLength() + 1, "%S", log);
    fputs(szTime, stream);
	fputs("\n", stream);

    result = fseek(stream, 0L, SEEK_SET);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}

	//Check limit line
	line = 0;
	if(limit_line)
	{
		while(fgets(buffer, DAT_STR_LENG, stream)) line++;
		if(line > limit_line)
		{
			sprintf_s(strToFile, path.GetLength() + bak_file.GetLength() + 2, "%S\\%S", path, bak_file); 
			CopyFile((LPCWSTR)(LPCSTR)strFile, (LPCWSTR)(LPCSTR)strToFile, FALSE);
			fclose(stream);
			DeleteFile((LPCWSTR)(LPCSTR)strFile);
		}
		else
		{
			fclose(stream);
		}
	}
	else
	{
		fclose(stream);
	}
	return TRUE;
}