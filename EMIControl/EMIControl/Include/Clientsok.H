/////////////////////////////////////////////////////////////////////////////////
//clientsok.h
//클라이언트용 소켓 프로그램 헤더
#define WM_ONCONNECT		WM_USER+5
#define WM_RECEIVE_DATA		WM_USER+6
#define WM_CLOSE_SOCK		WM_USER+7
#define MAX_PACKET			256
//20151130 ngh
#define PACKET_CHAR_STX					0x02
#define PACKET_CHAR_ETX					0x03

class CClientSock : public CAsyncSocket
{

public:
		CClientSock();
		//메세지를 전달할 HWND를 설정하는 함수
		void SetWnd(HWND hwnd);
		//새로운 데이타가 들어왔을대 실행되는 함수
		virtual void OnConnect( int nErrorCode );
		virtual void OnReceive( int nErrorCode );
		virtual void OnClose(int nErrorCode);
public:
		//메세지를 전달할 HWND
		HWND m_pHwnd;
		BOOL connect;
};
///////////////////////////////////////////////////////////////////////////////
