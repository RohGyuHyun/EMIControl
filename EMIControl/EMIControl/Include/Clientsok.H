/////////////////////////////////////////////////////////////////////////////////
//clientsok.h
//Ŭ���̾�Ʈ�� ���� ���α׷� ���
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
		//�޼����� ������ HWND�� �����ϴ� �Լ�
		void SetWnd(HWND hwnd);
		//���ο� ����Ÿ�� �������� ����Ǵ� �Լ�
		virtual void OnConnect( int nErrorCode );
		virtual void OnReceive( int nErrorCode );
		virtual void OnClose(int nErrorCode);
public:
		//�޼����� ������ HWND
		HWND m_pHwnd;
		BOOL connect;
};
///////////////////////////////////////////////////////////////////////////////