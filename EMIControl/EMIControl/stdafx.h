
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원

#include <winsock2.h>
#include <afxsock.h>
#include <WinInet.h>

#include <afxinet.h>
#include <xstring>

#include <cassert>
#include <io.h>

#include <afxsock.h>
#include "halconcpp.h"
using namespace Halcon;

#include "Mil.h"
#include "TeliCamApi.h"
#include "atf_lib_exp.h"
#include "Ezi/FAS_EziMOTIONPlusR.h"
using namespace Teli;

#include <vector>
#include "stdio.h"
#include "share.h"

#include "Mdfunc.h"

#include "Define.h"

//#ifdef DEFECT
#import "msxml6.dll" // msxml3.dll or msxml4.dll or msxml5.dll or msxml6.dll
#include <memory>

namespace MSXML2
{
	struct __declspec(uuid("f6d90f11-9c73-11d3-b32e-00c04f990bb4"))
		DOMDocument;

	struct __declspec(uuid("f5078f1b-c551-11d3-89b9-0000f81fe221"))
		DOMDocument26;

	struct __declspec(uuid("f5078f32-c551-11d3-89b9-0000f81fe221"))
		DOMDocument30;

	struct __declspec(uuid("88d969c0-f192-11d4-a65f-0040963251e5"))
		DOMDocument40;

	struct __declspec(uuid("88d969e5-f192-11d4-a65f-0040963251e5"))
		DOMDocument50;

	struct __declspec(uuid("88d96a05-f192-11d4-a65f-0040963251e5"))
		DOMDocument60;
}
//#endif


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


