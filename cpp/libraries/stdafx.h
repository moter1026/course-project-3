// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once
#define WIN32_LEAN_AND_MEAN  // Убирает редко используемые части Windows.h
// Явно определяем версию Windows
#define WINVER 0x0A00  // Windows 10
#define _WIN32_WINNT 0x0A00 // для Windows 10
//#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxtempl.h>       // Array Specifiers
#include <math.h>			//	Math support
#include <oaidl.h>
#include <basetyps.h>
#include <olectl.h>			// for ole support
#include <afxdisp.h>        // MFC OLE automation classes //05.04.01MPt** 
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows 95 Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdhtml.h>
#include <afxdisp.h>
#include "LocalPoint.h"		// 22.11.04 CH for NormalizePolygon
#include <gl/gl.h>
#include "GLAux.h"

typedef struct TAG_NEW_PARAM	//29.11.00L //10.10.18S
{
	long		num_col;
	long		param;			//27.07.10S
	CListCtrl *	list;
	BOOL		asc_sort;		//TRUE - сортировка в порядке возрастания; //05.01.01L
}NEW_PARAM;

double	Distance ( POINT*, POINT* );
float	Round_dec ( float, UINT );		//27.02
POINT	Convert ( POINT, BOOL );		//28.05.03S
void	NormalizePolygon ( POINT* );	//07.10.03S
void	CorrectTime ( void *, void *, WORD, UINT, int );	//28.04.04S
int		CALLBACK SortFunc( LPARAM, LPARAM, LPARAM );		//10.10.18S

#define RATIO			.82843			//23.11.01S
#define MKS_MUL			1000000			//08.04.02S
#define ERR_VALUE		50				//значение погрешности постановки датчиков по умолчанию [мм]	//29.03.06S
#define D_VALUE			10				//величина размера ячейки решётки по умолчанию [мм]				//29.03.06S
#define MAXZONE			10000			//максимальный размер пятна в локации по умолчанию				//29.03.06S
#define MINTOTNUM		3				//минимально допустимый размер пачки для локации по умолчанию	//29.03.06S
#define NUM_MINOR		10				//количество делений в логарифмической шкале					//07.12.07S
#define	HIST_NORM_MAX	1000L			//normalization maximum for histogram	//10.04.02S	//26.05.08S
#define LANGUAGE_VERSION 000000			//версия языка (000000-русский, 100000 - английский, 200000 - польский) //19.10.15S

#define GET_STRING(prm) \
	CString(MAKEINTRESOURCE(prm))

#define SET_DLG_STRING(idc, ids) \
	SetDlgItemText((idc), GET_STRING(ids))

#ifdef UNICODE

#include <wchar.h>

#define __tscmp( a, b ) wcscmp( a, b )

#define _GETCSTR( prm ) CStringA( CStringW( prm ) )
#define _GETWSTR( prm ) (LPCTSTR)( prm )

#else

#define __tscmp( a, b ) strcmp( a, b )

#define _GETCSTR( prm ) (LPCTSTR)( prm )
#define _GETWSTR( prm ) CStringW( CStringA( prm ) )

#endif

#define _tscmp( a, b ) __tscmp( a, b )

#define GETCSTR( prm ) _GETCSTR( prm )
#define GETWSTR( prm ) _GETWSTR( prm )

// Convert from TCHAR sequence to char sequence
void convert_2C( const TCHAR * const source, char * const destination, const int sized );
// Convert from char sequence to TCHAR sequence
void convert_2T( const char * const source, TCHAR * const destination, const int sizes, const int sized );

#define CONVERT_2C( src, dest ) \
		convert_2C( (src), (dest), ( sizeof( dest ) / sizeof( dest[0] ) ) )
#define CONVERT_2T( src, dest ) \
		convert_2T( (src), (dest), ( sizeof( src ) / sizeof( src[0] ) ), ( sizeof( dest ) / sizeof( dest[0] ) ) )

// compare char sequence and TCHAR sequence
int compare_CT( const char * const csource, const TCHAR * const tsource, const int sizec );

#define COMPARE_CT( csrc, tsrc ) \
		compare_CT( (csrc), (tsrc), ( sizeof( csrc ) / sizeof( csrc[0] ) ) )

// reads from FILE* to delim or \n
int readline_delim( FILE * const file, TCHAR * const buffer, int length, const TCHAR delim );
// reads from FILE* to \n
int readline( FILE * const file, TCHAR * const buffer, int length );

// sets BOM mark in the beginning
size_t prepare_unicode( FILE * const file );
