// stdafx.cpp : source file that includes just the standard includes
//	ALINE16D.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "constants.h"
#include "filecons.h"


double Distance(POINT* pt1, POINT* pt2)	//23.03.99L+A  //18.09.01 CH
{
	double	z1,z2;
	//return pow(double(pt1->x - pt2->x), 2.) + pow(double(pt1->y - pt2->y), 2.);
	z1=(double(pt1->x) - pt2->x);
	z2=(double(pt1->y) - pt2->y);
	return (z1*z1 + z2*z2);
}

float Round_dec (float a, UINT n)	//27.02
{
	double b;
	double mult = 1.0;

	for (UINT i=0; i<n; i++)
	{
		mult *= 10.0;
	}

	b = (round(a * mult)) / mult; 
	return (float(b));
}

POINT Convert ( POINT pt, BOOL direct )	//28.05.03S
{
	POINT tmp;
	if ( direct )
	{
		tmp.x = pt.y;
		tmp.y = -pt.x;
	}
	else
	{
		tmp.x = -pt.y;
		tmp.y = pt.x;
	}

	return tmp;
}

void NormalizePolygon ( POINT* pt )	//07.10.03S	// 22.11.04 CH	//26.11.04 CH
{
	POINT p;
	int i, j, k, r1,r2;
	for(i=0; i<4; i++)	//e. на стороне 3-хугольника
	{
		for(j=0; j<4; j++)
		{
			if( i==j ) continue;
			for(k=0; k<4; k++)
			{
				if( k==i || k==j ) continue;
				if( Sign2Line(pt[i], pt[j], pt[k])==0 && PtInInterval(pt[i], pt[j], pt[k]) )	//i-й на линии между j и k
				{
					if( ((i==0||i==2) && j==1 && k==3) || ((i==1||i==3) && j==0 && k==2) ) return;	//OK!
					if( ((i==0 || i==1) && j==2) || ((i==2 || i==3) && k==1) ) { r1=1; r2=2; } else { r1=2; r2=3; }
					p  = pt[r1]; pt[r1] = pt[r2]; pt[r2] = p; 
					return;
				}
			}
		}
	}

	if( IntersectLine(pt[0], pt[3], pt[1], pt[2]) || IntersectLine(pt[0], pt[1], pt[3], pt[2]) )
	{ // переставляем 2<=>3
		p  = pt[3]; pt[3] = pt[2]; pt[2] = p;
		if( IntersectLine(pt[0], pt[3], pt[1], pt[2]) || IntersectLine(pt[0], pt[1], pt[3], pt[2]) )
		{ //плохо переставили, надо по-другому: 1<=>2
			p  = pt[1]; pt[1] = pt[2]; pt[2] = p;
		}
	}
/*	int	j, counter;
	POINT	tmp;

beg:counter=0;

    for (j=0; j<3; j++)
	{
		if ( pt[j].x > pt[j+1].x)
		{
			tmp = pt[j];
			pt[j] = pt[j+1];
			pt[j+1] = tmp;
			counter++;
		}
	}

	if (counter) goto beg;

	if ((( pt[0].y < pt[1].y ) && ( pt[2].y < pt[3].y )) ||
		(( pt[0].y > pt[1].y ) && ( pt[2].y > pt[3].y )))
	{
		tmp = pt[2]; pt[2] = pt[3]; pt[3] = tmp;
	}*/
}

//вычисление длины пути по эллипсоиду высоты h	//09.06.08S
double MeridianLength ( double h, int n )
{
	double s = 0.;
    double dx = 1.0 / n, dx1000 = dx / 1000;
    double x1 = -1.0, x2 = -1.0 + dx;
    double y1 = 0, y2 = h * sqrt(1.0 - x2 * x2);

    for ( ; x2 < dx1000; ) {
        s += hypot(x2 - x1, y2 - y1);
        x1 = x2;
        y1 = y2;
        x2 = x1 + dx;
        y2 = h * sqrt(1.0 - x2 * x2);
    }

	return s;
}

#define	ADD_SEC		343597	//28.04.04S
#define ADD_USEC	383600	//28.04.04S
void CorrectTime ( void * ptr, void * buf, WORD chan_num, UINT start_time_s, int type )	//28.04.04S
{
	AE_TIME * ev_time	= ( AE_TIME * ) ptr;
	CRC_STRUCT * crtime	= ( CRC_STRUCT * ) buf;

	UINT time_s = UINT ( ev_time->seconds - start_time_s );
	if ( time_s < crtime[chan_num].crc[type].old_time )	
	{
		crtime[chan_num].crc[type].overflow ++;
		crtime[chan_num].crc[type].bNextSecond = TRUE;
	}
	else 
	{
		if ( crtime[chan_num].crc[type].bNextSecond && ( type == 1 ))
		{
			crtime[chan_num].crc[type].bNextSecond = FALSE;
			crtime[chan_num].crc[0].overflow = crtime[chan_num].crc[2].overflow = crtime[chan_num].crc[3].overflow = crtime[chan_num].crc[1].overflow;
			crtime[chan_num].crc[0].old_time = crtime[chan_num].crc[2].old_time = crtime[chan_num].crc[3].old_time = time_s;
		}
	}
	crtime[chan_num].crc[type].old_time = time_s;
	ev_time->seconds += crtime[chan_num].crc[type].overflow * ADD_SEC;
	ev_time->usec += crtime[chan_num].crc[type].overflow * ADD_USEC;
	while ( ev_time->usec >= USEC_PER_SEC )
	{
		ev_time->seconds ++;
		ev_time->usec -= USEC_PER_SEC;
	}
}

//	цвета планарной локации //05.04.99S
COLORREF				loc_color[MAX_LOC_COLOR]=
											{	
												RGB(255,128,0),
												RGB(255,0,0),
												RGB(216,0,0),
												RGB(160,0,0),
												RGB(128,0,64)
											};  

COLORREF				loc_amp_color[MAX_LOC_COLOR]=
											{	
												RGB(255,128,0),
												RGB(255,0,0),
												RGB(216,0,0),
												RGB(160,0,0),
												RGB(128,0,64)
											};  

UINT				loc_color_boundary[MAX_LOC_COLOR-1] = {2,10,25,100};
UINT				loc_amp_color_boundary[MAX_LOC_COLOR-1] = {40,50,60,70};	//17.03.99S
UINT				ClusterLength = 200;	//length of the cluster rect in mm //05.04.99S
UINT				ClusterWidth  = 200;	//width of the cluster rect in mm //05.04.99S
UINT				ClusterHeight = 200;	//height of the cluster rect in mm //14.05.01S
int					IsClusterProc = 0;		//флаг кластеризации в процентах от максимума	//26.04.06S

UINT				NumberofDataFile;		//number of data files already saved by this copy of program //20.04.99S
UINT				NumberofSDOFile;		//number of sdo files already saved by this copy of program //20.04.99S
CString				base_data_name;			//data file filename without number and extension	//20.04.99S
CString				base_sdo_name;			//sdo file filename without number and extension	//20.04.99S

void convert_2C( const TCHAR * const source, char * const destination, const int sized )
{
	CStringA	temp_str;
	int	symbol_size;
	int	i;

	if ( !source ) return;
	if ( !destination ) return;
	if ( sized < 2 ) return;

	temp_str = CStringA( source );
	if ( CString( temp_str ) == CString( source ) ) {
		strncpy( destination, temp_str, sized );
		destination[ sized - 1 ] = 0;
	} else {
		i = 0;
		symbol_size = sizeof( TCHAR ) / sizeof( char );

		while ( ( source[ i ] != 0 ) && ( i * symbol_size < sized - 1 ) ) {
			i++;
		} /* count chars */
		if ( i > 0 ) {
			memcpy( &( destination[ 0 ] ), &( source[ 0 ] ), i * sizeof( TCHAR ) );
		} /* copy chars */
		((TCHAR *)(&destination[ i * symbol_size ]))[0] = 0;
		destination[ sized - 2 ] = 1;
		destination[ sized - 1 ] = 1;
	} /* can be printed as char? */
}

void convert_2T( const char * const source, TCHAR * const destination, const int sizes, const int sized )
{
	int	symbol_size;
	CString	str;
	int	i;

	if ( !source ) return;
	if ( !destination ) return;
	if ( sizes < 2 ) return;
	if ( sized < 2 ) return;

	if ( ( source[ sizes - 2 ] == 1 ) && ( source[ sizes - 1 ]  == 1 ) ) {
		i = 0;
		symbol_size = sizeof( TCHAR ) / sizeof( char );

		while (
			!(	( source[ i * symbol_size ] == 0 ) &&
				( source[ i * symbol_size + ( 1 % symbol_size ) ] == 0 ) ) &&	/* if UNICODE - check both char, else check only first */
			( i < ( sized / symbol_size ) - 1 ) &&
			( i * symbol_size < sizes )
			) {
			i++;
		} /* count chars */
		if ( i > 0 ) {
			memcpy( &( destination[ 0 ] ), &( source[ 0 ] ), i * sizeof( TCHAR ) );
		} /* copy chars */
		destination[ i ] = 0;
	} else {
		str = CString( source );
		_tcsncpy( destination, str, sized );
		destination[ sized - 1 ] = 0;
	} /* check marker */
}

// compare char sequence and TCHAR sequence
int compare_CT( const char * const csource, const TCHAR * const tsource, const int sizec )
{
	TCHAR	convert[ MAX_PATH ];
	int	res;

	convert_2T( csource, convert, sizec, MAX_PATH );

	res = _tcscmp( convert, tsource );

	return res;
}

static
int
read_file_bom(
	FILE * const file )
{
	WORD	bom_mark;
	long	pos;
	int	res;

	if ( !file ) return 0;

	res = 0;
	pos = ftell( file );
	if ( !pos ) {
		res = fread( &bom_mark, sizeof( WORD ), 1, file );
		if ( res ) {
			if ( bom_mark != 0xfeff ) {
				res = 0;
				fseek( file, 0, SEEK_SET );
			} /* not BOM mark */
		} /* read smth? */
	} /* beginning of the file? */

	return res;
}

// check if there is printed code 0x00
static
int
check_string_character_size(
	const TCHAR * const buffer,
	const int length )
{
	char	*cbuf;
	int	res;
	int	j;
	int	i;

	if ( !buffer ) return 0;

	cbuf = ( char * )buffer;
	i = 0;
	while ( buffer[ i ] ) {
		i++;
	} /* count length of tchar */

	j = 0;
	while ( cbuf[ j ] ) {
		j++;
	} /* search first 0 */

	if ( j / sizeof( TCHAR ) != i ) {
		res = sizeof( TCHAR );
	} else {
		res = sizeof( char );
	} /* if met 0 before real end */

	return res;
}

// check first string of file
static
int
check_file_character_size(
	FILE * const file )
{
	TCHAR	buffer[256];
	int	current_pos;
	TCHAR	*ptr;
	int	bom;
	int	res;
	
	if ( !file ) return 0;

	current_pos = ftell( file );

	fseek( file, 0, SEEK_SET );

	bom = read_file_bom( file );
	if ( !bom ) {
		ptr = _fgetts( buffer, 256, file );
		if ( ptr ) {
			res = check_string_character_size( buffer, 256 );
		} else {
			res = 0;
		} /* can read? */
	} else {
		res = sizeof( TCHAR );
	} /* have bom? */

	fseek( file, current_pos, SEEK_SET );

	return res;
}

// read only one line
static
int
get_line_func(
	FILE * const file,
	TCHAR * const buffer,
	const int length,
	size_t * const char_size )
{
	char	*cptr;
	char	*buf;
	TCHAR	*ptr;
	int	check;
	int	pos;
	int	res;

	if ( !file ) return 0;
	if ( !buffer ) return 0;

	pos = ftell( file );
	if ( !pos ) {
		read_file_bom( file );
	} /* read bom symbol */
	ptr = _fgetts( buffer, length, file );

	res = 0;

	if ( ptr ) {
		check = check_string_character_size( buffer, length );
		if ( !check ) {
			check = check_file_character_size( file );
		} /* if no zeros in cur string - look in file */

		if ( check ) {
			if ( char_size ) {
				*char_size = check;
			} /* if have value? */
			if ( check != sizeof( TCHAR ) ) {
				buf = ( char * )malloc( length * sizeof( char ) );
				if ( buf ) {
					fseek( file, pos, SEEK_SET );
					cptr = fgets( buf, length, file );
					if ( cptr ) {
						_sntprintf( buffer, length, CString( buf ) );
						res = 1;
					} /* read smth char */
					free( buf );
				} /* malloc buf? */
			} else {
				res = 1;
			} /* if realized char */
		} /* if decided */
	} else {
		res = 0;
	} /* can read? */

	return res;
}

// reads from FILE* to delim or \n
int
readline_delim(
	FILE * const file,
	TCHAR * const buffer,
	int	length,
	const TCHAR delim )
{
	CString	strbuf;
	size_t	size;
	long	pos;
	int	dpos;
	int	res;

	if ( !file ) return 0;
	if ( !buffer ) return 0;

	pos = ftell( file );
	res = get_line_func( file, buffer, length, &size );

	if ( res ) {
		strbuf = CString( buffer );
		dpos = strbuf.Find( delim );
		if ( dpos != -1 ) {
			strbuf = strbuf.Left( dpos );
			_sntprintf( buffer, length, _T("%s"), strbuf );
			fseek( file, pos + ( dpos + 1 ) * (long)size, SEEK_SET );
		} /* if found delim */
	} /* read smth? */
	
	return res;
}

// reads from FILE* to \n
int
readline(
	FILE * const file,
	TCHAR * const buffer,
	int	length )
{
	int	res;

	if ( !file ) return 0;
	if ( !buffer ) return 0;

	res = get_line_func( file, buffer, length, NULL );

	return res;
}

int CALLBACK SortFunc( LPARAM lParam1, LPARAM lParam2, LPARAM dwType ) //29.11.00L //27.07.10S //10.10.18S
{
	CString			str1, str2;
	int				ind1, ind2;
	double			a1, a2;
	NEW_PARAM *		n_par = ( NEW_PARAM * ) dwType;
	LV_FINDINFO		pFindInfo;

	pFindInfo.flags  = LVFI_PARAM;
	pFindInfo.lParam = lParam1;
	ind1 = n_par->list->FindItem( &pFindInfo, -1 );
	pFindInfo.lParam = lParam2;
	ind2 = n_par->list->FindItem( &pFindInfo, -1 );

	if ( ( ind1 < 0 ) || ( ind2 < 0 ) ) return 0;

	str1 = n_par->list->GetItemText( ind1, n_par->num_col );
	str2 = n_par->list->GetItemText( ind2, n_par->num_col );
	a1 = _ttof( str1 );
	a2 = _ttof( str2 );

	switch ( n_par->param ) {
		case ID_NUMBER:
			if ( n_par->asc_sort ) {		//05.01.01L
				return ( ( lParam1 > lParam2 ) ? 1 : ( lParam1 == lParam2 ) ? 0 : -1 );
			} else {
				return ( ( lParam1 < lParam2 ) ? 1 : ( lParam1 == lParam2 ) ? 0 : -1 );
			}
		case ID_DATE:
		case ID_TIME_ABS:
		case ID_CHANNEL:
		case ID_FLAGS:						//17.08.15S
			if ( n_par->asc_sort ) {
				return _tcscmp( str1, str2 );
			} else {
				return ( -_tcscmp( str1, str2 ) );
			}
		case ID_NUM_PACK1:
		case ID_Y1:							//15.07.13S
		case ID_Z1:
		case ID_LA1:
		case ID_TAD:
			if ( n_par->asc_sort ) {
				if ( ( !str1.IsEmpty() ) && ( str1.Find( _T('*') ) == -1 ) && ( !str2.IsEmpty() ) && ( str2.Find( _T('*') ) == -1 ) ) {
					return ( ( a1 > a2 ) ? 1 : ( a1 == a2 ) ? 0 : -1 );
				} else {
					return _tcscmp( str1, str2 );
				}
			} else {
				if ( ( !str1.IsEmpty() ) && ( str1.Find( _T('*') ) == -1 ) && ( !str2.IsEmpty() ) && ( str2.Find( _T('*') ) == -1 ) ) {
					return ( ( a1 < a2 ) ? 1 : ( a1 == a2 ) ? 0 : -1 );
				} else {
					return ( -_tcscmp( str1, str2 ) );
				}
			}
		default:
			if ( n_par->asc_sort ) {
				return ( ( a1 > a2 ) ? 1 : ( a1 == a2 ) ? 0 : -1 );
			} else {
				return ( ( a1 < a2 ) ? 1 : ( a1 == a2 ) ? 0 : -1 );
			}
	}
  
	return 0;
}


size_t
prepare_unicode(
	FILE * const file )
{
	WORD	bom_mark;
	long	pos;
	size_t	ret;
	
	if ( !file ) return 0;

#ifndef _UNICODE
	return 1;
#endif

	ret = 0;
	pos = ftell( file );
	if ( !pos ) {
		bom_mark = 0xfeff;
		ret = fwrite( &bom_mark, sizeof( WORD ), 1, file );
	} /* if not the beginning */

	return ret;
}
