/////////////////////////////////////////////////////////////////////////////////
//Clientsok.cpp
//클라이언트용 소켓 클래스함수
#include "stdafx.h"
#include "Clientsok.h"



//생성자는 아무일도 안하고 모든생성을 CAsyncSocket
//에 넘겨줍니다.
CClientSock::CClientSock()
{
	CAsyncSocket::CAsyncSocket();
	connect = FALSE;
}
//데이타가 들어왔을때 현재 hwnd에 새로운 데이타가 들어
//왔다는 메세지를 날려주기위한 hwnd를 설정합니다.
void CClientSock::SetWnd(HWND hwnd)
{
	m_pHwnd = hwnd;
}

//새로운 데이타가 들어왔을때 현재의 hwnd에
//USER+2인 메세지 WM_RECEIVE를 보냅니다.
void CClientSock::OnConnect( int nErrorCode )
{
	TRACE("Errocode = %d", nErrorCode);

	if(nErrorCode == 0)
	{
		connect = TRUE;
		SendMessage(m_pHwnd, WM_ONCONNECT, 0, 0);
	}
}


void CClientSock::OnReceive( int nErrorCode )
{
	 TRACE("Errocode = %d",nErrorCode);
	 SendMessage(m_pHwnd, WM_RECEIVE_DATA, 0, 0);
}

void CClientSock::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	TRACE("Errocode = %d",nErrorCode);

	connect = FALSE;
	SendMessage(m_pHwnd, WM_CLOSE_SOCK, 0, 0);

	CAsyncSocket::OnClose(nErrorCode);
}

/////////////////////////////////////////////////////////////////////////////////
