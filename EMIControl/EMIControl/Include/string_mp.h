/////////////////////////////////////////////////////////////////////////////
//	string_mp.h
//	link user32.lib 
/////////////////////////////////////////////////////////////////////////////

#ifndef __STRING_MP_H__
#define __STRING_MP_H__

#include <windows.h>

#if defined(__cplusplus)
extern "C"
{
#endif

int parse_cfg_items( char *items[], int nr_items, char *buf, const char *separators );
int parse_items( char *items[], int nr_items, char *string, const char *separators );

int	make_path( const char *path );
int strtolower( char* strDest, const char* strSrc );
int strtoupper( char* strDest, const char* strSrc );

#if defined(__cplusplus)
}
#endif 

#pragma warning (disable:4786)
#include <list>
#include <string>
typedef std::list<char*> STRING_LIST;

class CLog {
private:
	STRING_LIST m_log;
	int			m_log_cnt;

public:
	CLog();
	~CLog();

	int add( const char *str );
	int clear();
	int print( HWND hwnd );
	void set_count( int count ) { m_log_cnt = count; }
};

#endif