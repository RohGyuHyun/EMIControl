#include "stdafx.h"
#include "string_mp.h"

#include <string.h>
#include <assert.h>
#include <windows.h>
#include <io.h>

#pragma warning (disable:4786)
#include <string>

int parse_cfg_items( char *items[], int nr_items, char *buf, const char *separators ) 
{
	char *token, *buf_inc;
	int token_pos, item_count = 0; 

	buf_inc = buf;
	items[item_count++] = buf_inc;

	while(nr_items-->0) {
		token_pos = strcspn( buf_inc, separators );
		token = buf_inc+token_pos;
		if ( token[0] == NULL ) {
			break;
		} else if ( strchr( separators, token[1]) != NULL){// || !isalnum(token[1]) ) {
			items[ (item_count++) ] = NULL;
		} else {
			items[ (item_count++) ] = token+1;
		}

		buf_inc = token+1;
		*token = NULL;
	}

	if ( items[item_count-1] != NULL ) {
		if ( (token = strchr(items[item_count-1], '\r' )) != NULL ) {
			*token=NULL;
		}
		if ( (token = strchr(items[item_count-1], '\n' )) != NULL ) {
			*token=NULL;
		}
	}

	return item_count;
}

int parse_items( char *items[], int nr_items, char *string, const char *separators )
{
	char *token;
	int item_count=0;
	
	token = strtok( string, separators );
	while ( token != NULL ) {
		items[item_count++]=token;	
		token = strtok( NULL, separators );
	}
	
	assert( item_count>0 );
	if ( (token = strchr(items[item_count-1], '\r' )) != NULL ) {
		*token='\0';
	}
	if ( (token = strchr(items[item_count-1], '\n' )) != NULL ) {
		*token='\0';
	}

	return item_count;
}

int	make_path( const char *path )
{
	std::string search_p, org_p;
	int last, dc_pos;

	org_p = path;
	last = 0;

	while( (dc_pos = org_p.find_first_of( '\\', last )) > 0 ) {

		search_p = org_p.substr( 0, dc_pos );
	
		if ( _access( search_p.data(), 0 ) < 0 ) {
			if ( CreateDirectory( search_p.data(), NULL ) == FALSE ) {
				return -1;
			}
		}

		last = search_p.size() + 2;
	}

	return 0;		
}

int strtolower( char* strDest, const char* strSrc )
{
	int nLength = strlen( strSrc );
	char nCh;

	for ( int i=0 ; i<nLength ; i++ )
	{
		nCh = strSrc[i];
		tolower( nCh );
		strDest[i] = nCh;
	}

	strDest[i] = 0;

	return nLength;
}

int strtoupper( char* strDest, const char* strSrc )
{
	int nLength = strlen( strSrc );
	char nCh;

	for ( int i=0 ; i<nLength ; i++ )
	{
		nCh = strSrc[i];
		toupper( nCh );
		strDest[i] = nCh;
	}

	strDest[i] = 0;

	return nLength;
}

/////////////////////////////////////////////////////////////////////////////

CLog::CLog()
{
	m_log_cnt = 10;
}

CLog::~CLog()
{
	clear();
}

int CLog::add( const char *str )
{
	char * new_str = new char[strlen(str)+1];
	strcpy( new_str, str );
	m_log.push_back( new_str );

	char * old_str;
	while( m_log.size() >= m_log_cnt ) {
		old_str = m_log.front();
		m_log.pop_front();
		delete old_str;
	}
	return 1;
}

int CLog::clear()
{
	char * old_str;
	while( m_log.size() > 0 ) {
		old_str = m_log.front();
		m_log.pop_front();
		delete old_str;
	}
	
	return 1;
}

int CLog::print( HWND hwnd )
{
	char msg[2048];
	msg[0] = 0;

	STRING_LIST::const_iterator str;

	for ( str=m_log.begin(); str!=m_log.end(); str++ ){
		strcat( msg, *str );
	}	

	::SetWindowText( hwnd, msg );

	return 1;
}
/////////////////////////////////////////////////////////////////////////////
