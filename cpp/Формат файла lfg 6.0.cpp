// ������ ����� ������� ��� �� ������ A-Line DDM, DDM-2, PCI-N � PCI8 ��� ���� ������ //created 08.05.2020

// ������� ������� ��������� ����� ������� TAGFileHdr
////////////////////////////////////////////////////////////////
typedef struct TAGFileHdr
{
    UINT signature;         // signature = 0xAA551313
    UINT ver_maj;           // version major = 33 ��� ������� ������ ������
    UINT ver_min;           // version minor = 8 ��� ������� ������ ������
    UINT ftype;             // type of file	= 0xAA03 ��� ����� ������� lfg-����
	UINT hard_version;		// version of hardware controller //������� ������ DDM-2 ����������� = 0x102071f (����������� �� ���������� �����)
	UINT soft_version;		// version of sort options //������� ������ �� A-Line = 0x20600
    UINT reserved[18];      // reserved bytes for future use
}FileHdr;

////////////////////////////////////////////////////////////////
// ����� ������� ���� ����� ���������� �������
// ���� 1 ����� ����� �������, ��������� ��� ������ � ������� m_SensorTotal � ���� ���������� ������ ExFlag
// ����� ����� ����� new_sens blk_size = sizeof ( UINT )
UNIT new_sens = m_SensorTotal + UINT ( ExFlag << 16 );

// ���� 2 ��������� ������������� clusterization info settings (����� ��� ���� ����� �������)
// ����� ����� ����� clusterization info settings = sizeof ( COLORREF ) * 5 + sizeof ( COLORREF ) * 5 + sizeof ( UINT ) * 4 + sizeof ( UINT ) * 4 + sizeof ( UINT ) * 3 + sizeof ( UINT )
// ���� 2.1 - ������ �������� ������� ������������� �� ���������� ������� � �������� - ����� 5 �������� ���� COLORREF
// ���� 2.2 - ������ �������� ������� ������������� �� ����������� ��������� � �������� - ����� 5 �������� ���� COLORREF
// ���� 2.3 - ������ �������� ������ ������������� �� ���������� ������� � �������� - ����� 4 �������� ���� UINT
// ���� 2.4 - ������ �������� ������ ������������� �� ����������� ��������� � �������� - ����� 4 �������� ���� UINT
// ���� 2.5 - �����, ������ � ������ �������� � �� - ����� 3 �������� ���� UINT
// ���� 2.6 - ��� �������������: �� ���������� ������� � ��������, �� �������� ���������� ������� � �������� �� ������ �� �����, �� �������� ���������� ������� � �������� �� ��������� - ����� 1 �������� ���� UINT

// ���� 3 ����������� ������
// ���� 3.1 - ����� ����������� ����������� ����� m_GroupsNum - ����� 1 �������� ���� UINT
// ����� �� ����� ����������� ����� m_GroupsNum �������� ���������, i - ����� ������� ������
// Groups[i].Enable, sizeof(BOOL) - ������� �������� ������, ������ TRUE
// Groups[i].Name, sizeof ( char ) * 16 - ��������� �������� ������, ������������ ����� ����������� � UNICODE � ������� CONVERT_2T (��. ����)
// Groups[i].Velocity, sizeof(UINT) - �������� �������� ����� � �/� ��� ������� ���������
// Groups[i].Bound, sizeof(DWORD) - �������� ������� �������, � ��
// Groups[i].ImageDiameter, sizeof(int) - ������� ����������� ������� �� ����� �������, � ���. ��
// Groups[i].Length, sizeof(DWORD) - ����� ������� �������, � ��
// Groups[i].Width, sizeof(DWORD) - ������ ������� �������, � ��
// Groups[i].Height, sizeof(DWORD) - ������ ������� �������, � ��
// Groups[i].Type, sizeof(int) - ��� ������� �������� - 0, ��������� - 1, ������� - 2, ����� - 3, ����� - 4, ���������� - 5, ������ - 6, ������� - 7, ����� - 8, ������ ��� - 9
// Groups[i].DisplayAmpLocWindow, sizeof(BOOL) - ���� ��������� ���� ����������� ���������
// Groups[i].Average, sizeof(BOOL) - ����� ���� ����������� ���������: 0 - �������� ������� �� ��������, 1 - ������������ �� �������� ���������
// Groups[i].Dbonm, sizeof(double) - �������� ��������� ���������, ��/�
// Groups[i].Display3DLocWindow, sizeof(BOOL) - ���� ��������� ���� 3D-������ �������
// Groups[i].DisplayLocEvWindow, sizeof(BOOL) - ���� ��������� ���� ���������� ������������� ������� �� �������
// Groups[i].fpackdef, sizeof ( FPackDef ) - ��������� ���������� ��������� �������� ����� (�������)
typedef struct TAGFPackDef		
{
    UINT amp_min;				// ����������� ����. 1-�� ������� � �����, ��
	int  all_chan;				// 0 - ����� �����, 1 - �� ������.
	char chan_list[200];		// ������ �������
	int  all_AES;				// 0-��� �������, 1-������ �� ����.������, 2-����. �� ����. ������, 3-������ ��������� ������� ��
	UINT pack_time;				// ����. �����, ���
	UINT pack_dead_time;		// ������� ����� ����� �����.
}FPackDef;

// Groups[i].Shift, sizeof(POINT) - ����� ������ ���������
// ����� ������������� ��������� ��� ������� � ������������ �������
// Groups[i].AmpCrit, sizeof(BOOL) - ���� ��������� ������������ ��������
// Groups[i].LocAmpCrit, sizeof(BOOL) - ���� ��������� �������� �� ����������� ���������
// Groups[i].LocAmpDiff, sizeof(UINT) - �������� ������������ ������� � ����������� ����������
// Groups[i].EvTimeCrit, sizeof(BOOL) - ���� ��������� ������ ��������� ������ (������� ������ ������� �������)
// Groups[i].EvTimeDiff, sizeof(UINT) - ���������� ���������� �������� ������ ������� ������� ��� ������ ������� ��������� ������
// Groups[i].DeadTimeCrit, sizeof(BOOL) - ���� ��������� �������� ������� � �������
// Groups[i].DeadTime, sizeof(UINT) - �������� �������� ������� ��� �������
// Groups[i].bPeakTime, sizeof(BOOL) - ���� �������� ������� �� ������� ��������� ���������	(�� ��������� - FALSE)
// Groups[i].AdoptedAtt, sizeof(int) - ���� ������� ��������� �� ����������� �������
// Groups[i].ParGateCrit1, sizeof(WORD) - ���� ��������� ���������������� ������ � �������
// Groups[i].ParGateMode1, sizeof(WORD) - ������������� ������ ������ ������ � �������: �������� ������ = 0, ������ �� ������� = 1, ������ �� ����� = 2
// Groups[i].ParLow1, sizeof(long) - ������ ������� ���������������� ������
// Groups[i].ParHigh1, sizeof(long) - ������� ������� ���������������� ������
// Groups[i].NumChan1, sizeof(WORD) - ����� ������������� ���������������� �����
// Groups[i].NumMod1, sizeof(WORD) - ����� ������������� ��������� ������ (������)
// Groups[i].AmpDiff, sizeof (int) - �������� ������� ��������� ��� ������������ ��������
// Groups[i].ReservedForCrit, 2 * sizeof( UINT ) - reserved for future criterium

// ����� ��������� ����������� �������, ��� � ��. ���������� ����������� ����� 
// Groups[i].bTrian, sizeof(int) - ��� �������, 1 - �����������, 0 - ���������������, 2 - ������������
// Groups[i].bCheckTrue, sizeof(BOOL) - ���� ��������� ������ ����������� ���������� ��� ������� 4-� �����������
// Groups[i].Delta, sizeof(UINT) - �������� ���������� ����������� ��� ����������� ���������� ��� ������� 4-� �����������
// Groups[i].bVelocityCheck, sizeof(BOOL) - ���� ��������� ������ ������� ����������� �������� ��� ������� 4-� �����������
// Groups[i].MaxVelocity, sizeof(UINT) - ������� ������� ��������� ������� ��������
// Groups[i].MinVelocity, sizeof(UINT) - ������ ������� ��������� ������� ��������
// Groups[i].DSize, sizeof(UINT) - �������� ������ ������� ��� ������������ ������� �� ���������, � ��
// Groups[i].ZoneSize, sizeof(UINT) - ������������ ������ ����� � ������������ ������� �� ���������
// Groups[i].MinTotalNum, sizeof(WORD) - ����������� ������ �����, ���������� ��� ������������ �������
// Groups[i].MaxUsedNum, sizeof(WORD) - ������������ ����� ��������� ������������ � ����������� ������������ �������
// Groups[i].bSilentCheck, sizeof(WORD) - ���� ������������� ����� ������������ �������� � ����������� ������������ �������
// Groups[i].MaxSilentNum, sizeof(WORD) - ������������ ���������� ����� ������������ ��������
// Groups[i].Save2DForm, sizeof(BOOL) - ���� ���������� ���� 2D-�������� ��� ����������� �����������
// Groups[i].LocInside, sizeof(BOOL) - ���� ���������� ����������� ������� ��� ����������� ����
// Groups[i].ReservedCheck,  2 * sizeof( UINT ) - reserved for future use

// ����� ��������� ��������� �������� � ���������� ���������� ����������� ����
// Groups[i].Sensor, sizeof(UINT) - ����� ��������, �������������� � �������
// UINT num_channels - ���� ��������� �������� ����� ��������, ������������ � ������� � ������ ����������, ����������� ��� �������� ����;

	switch (Groups[i].Type)
{
//	����� ������� �����
case cone:	
// Groups[i].NumBelts, sizeof(UINT) - ����� ������ ��������
// Groups[i].LeftWidth, sizeof(UINT) - ������ ����� (�������) ������
// Groups[i].IncludeLeft, sizeof(BOOL) - ���� ��������� ������� ����� (�������) ������
// Groups[i].RightWidth, sizeof(UINT) - ������ ������ (������) ������
// Groups[i].IncludeRight, sizeof(BOOL) - ���� ��������� ������� ������ (������) ������
// Groups[i].PlaceBoard, sizeof(BOOL) - ���� ��������������� ���������� �������� �� ������� ��������
// Groups[i].bVertical, sizeof(BOOL) - ���� ����������� �������: 0 - ��������������, 1 - ������������
// num_channels, sizeof(UINT) - �������� ����� ������� � ������
break;

//	����� ������� �������
case volume:	
// Groups[i].NumBelts, sizeof(UINT) - ����� ������ ��������
// Groups[i].LeftWidth, sizeof(UINT) - ������ ����� (�������) ������
// Groups[i].IncludeLeft, sizeof(BOOL) - ���� ��������� ������� ����� (�������) ������
// Groups[i].RightWidth, sizeof(UINT) - ������ ������ (������) ������
// Groups[i].IncludeRight, sizeof(BOOL) - ���� ��������� ������� ������ (������) ������
// Groups[i].PlaceBoard, sizeof(BOOL) - ���� ��������������� ���������� �������� �� ������� ��������
// Groups[i].bVertical, sizeof(BOOL) - ���� ���������� �������: 0 - ��������������, 1 - ������������
// num_channels = Groups[i].Sensor + Groups[i].NumBelts - �������������� ����� ������� � ������
break;

//	����� ������� �����
case sphere: 
// Groups[i].bVertical, sizeof(BOOL) - ���� ���� ������� �����: 0 - ������������, 1 - ����������
if ( Groups[i].bVertical )
{
//	Groups[i].NumAdd, sizeof(UINT) - ����� �������������� ������� � �������;
//	num_channels = Groups[i].Sensor + Groups[i].NumAdd - �������������� ����� ������� � ������
//	Groups[i].NumBelts = 3;
}
else
{
//	Groups[i].NumBelts, sizeof(UINT) - ����� ������ ��������
//	j = (Groups[i].Sensor-2)/Groups[i].NumBelts;
//	num_channels = Groups[i].Sensor + 2*(j-1) + j*Groups[i].NumBelts - �������������� ����� ������� � ������
}
break;

//	������ ����������� �����
 default:
//	num_channels = Groups[i].Sensor - �������������� ����� ������� � ������
//	Groups[i].LeftWidth = Groups[i].RightWidth = 0;
//	Groups[i].NumBelts = 2;
break;
}

// ������ ��������� �������� � ������ ����������� ��� ��� ��� �����, ��� ��� ��������� ���������
// ��������� ��������� ������� 2D
class CLocalPoint
{
    UINT		num_chan;   // ����� ������ ��� ����� � ���������� ������
    int			num_repeat; // ��� ����������� ������� �� ������� ������: 0 - ��������, 1 - ����� ������, 2 - ������ ������, 3 - ���� ����� ������, 4 - ����� ������ ������, 5 - ����������, 6 - �������, 7 - ������
    POINT		coor;       // 2D- ���������� �������
    COLORREF	color;		// ���� ����������� �������
}
// ��������� ��������� ������� 3D
typedef struct TAGPoint3DL
{
	long	x;
	long	y;
	long	z;
}Point3DL;
class CLocalPoint3D
{
    UINT		num_chan;   // ����� ������ ��� ����� � ���������� ������
    int			num_repeat; // ��� ����������� ������� �� ������� ������: 0 - ��������, 1 - ����� ������, 2 - ������ ������, 3 - ���� ����� ������, 4 - ����� ������ ������, 5 - ����������, 6 - �������, 7 - ������
    Point3DL	coor;       // 3D- ���������� �������
    COLORREF	color;		// ���� ����������� �������
};

// ��������� ����������� ����������� ����
class CTriad
{
    UINT n[3];				// ����� ������� ���� � ����� ������� ������� DataArray
    UINT num_chan[3];		// ������ ������� ���� �������� �� ���������� ����� 
}
// ��������� ��������������� ����������� ����
class CQuad
{
    UINT n[4];				// ����� ������� ���� � ����� ������� ������� DataArray
    UINT num_chan[4];		// ������ ������� ���� �������� �� ���������� ����� 
	int NumChIn;			// ����� ������ �� DataArray, ��������� �������� ������ ��� �������������� �����
}

if (( Groups[i].Type != threeD ) && ( Groups[i].Type != zonary ) && ( Groups[i].bTrian != option ) && (( Groups[i].Type != picture ) || ( Groups[i].bTrian != quad )))
// ��� ����������� �������, �������������� ������� �� �����
// �� ����� num_channels �������� ��������������� ����� �������� CLocalPoint
// Groups[i] - > Add (CLocalPoint);

// ������ ����������� ���
// Groups[i].NumZone, sizeof(int) - ����� ����������� ���
// �� ����� Groups[i].NumZone �������� ��������������� ����� �������� CTriad ��� CQuad
if ( Groups[i].bTrian )
{
// Groups[i] - > Add (CTriad)
}
else
{
// Groups[i] - > Add (CQuad)
}

// ��� ���������������� �������
// �� ����� num_channels �������� ��������������� ����� �������� CLocalPoint3D
// Groups[i] - > Add (CLocalPoint3D);
else
{
// Groups[i] -> Add ( CLocalPoint3D )
}

// ������ �������� ������� �������
// Groups[i].DelFlag, sizeof(int) - ���� ��������/���������� ���������� �������
// Groups[i].gFilter.ForX, sizeof(BOOL) - ���� ���������� �� ��������� ���������� X
// Groups[i].gFilter.ForY, sizeof(BOOL) - ���� ���������� �� ��������� ���������� Y
// Groups[i].gFilter.ForZ, sizeof(BOOL) - ���� ���������� �� ��������� ���������� Z

// ������ ������ ���������� �� �����������
// Groups[i].gFilter.XMore, sizeof(LONG) - ����� ������� ������� �� ���������� X;
// Groups[i].gFilter.XLess, sizeof(LONG) - ������ ������� ������� �� ���������� X;
// Groups[i].gFilter.YMore, sizeof(LONG) - ����� ������� ������� �� ���������� Y;
// Groups[i].gFilter.YLess, sizeof(LONG) - ������ ������� ������� �� ���������� Y;
// Groups[i].gFilter.ZMore, sizeof(LONG) - ����� ������� ������� �� ���������� Z;
// Groups[i].gFilter.ZLess, sizeof(LONG) - ������ ������� ������� �� ���������� Z;

// Groups[i].Save, sizeof(BYTE) - ���� ���������� ����������� ���������� � �����
// Groups[i].DontDel, sizeof(BYTE) - ���� �� ������� �� ����� ��������������� ��������
// Groups[i].CurPage, sizeof(WORD) - ����� �������� ��� ������������ ���� �������
// Groups[i].FileName, sizeof ( char ) * 260 - ���� ��� ���������� ����������� ���������� � ���� ����� ������ ���������������� �� �������, ������������ ����� ����������� � UNICODE � ������� CONVERT_2T (��. ����)

// ��������� ���� ����������� �������, �������� ������� � �.�. ��� ����������
// Groups[i].cfg_loc_size, sizeof ( UINT ) - ������ ����������������� ����� ��� ���������� ���������� ��������� ������������ ����, ��. C2DView::QueryCfgSize ( void ) �� ����� cfg
// Groups[i].cfg_loc_wnd, Groups[i].cfg_loc_size - ������ ������ ����������������� ����� ���������� ��������� ������������ ����
// Groups[i].imp_loc_size, sizeof ( UINT ) - ������ ����������������� ����� ��� ���������� ���������� ��������� ������������� ������� ��������� ������������ ���� ����� sizeof ( IPData )
// Groups[i].imp_loc_wnd, Groups[i].imp_loc_size - ������ ������ ����������������� ����� ���������� ��������� ������������� ������� ��������� ������������ ����, ��������� IPData
typedef struct TAGImportPictureData	
{
	BOOL	m_bEnable;					// ��������� ��������� ������������� �������
    RECTL	rc;							// ���������� ���� ���������
	char	m_FileName[MAX_PATH];		// ����-�������� ������� (��������� �������: *.emf, *.wmf)
	BOOL	m_bXY;						// ���� ���������� �������� ���������
	BOOL	m_bEnh;						// ���� ����������� enhanced metafile format	
}IPData;
// Groups[i].cfg_3Dloc_size, sizeof ( UINT ) - ������ ����������������� ����� ��� ���������� ���������� ���� 3D-������ �������, ��. C3DView::QueryCfgSize ( void ) �� ����� cfg
// Groups[i].cfg_3Dloc_wnd, Groups[i].cfg_3Dloc_size - ������ ������ ����������������� ����� ���������� ��������� ������������ ����

UINT CTableWnd::QueryCfgSize ( void ) //������ ����������������� ����� ��� ���� ������ �������
{
    UINT cfg_size = 0;
    cfg_size += sizeof ( RECT );			// ���������� ����
	cfg_size += sizeof ( byte );			// is window minimized, maximized or hide
    return ( cfg_size );
}
// Groups[i].cfg_tloc_size, sizeof ( UINT ) - ������ ����������������� ����� ��� ���������� ���������� ���� ������ �������
// Groups[i].cfg_tloc_wnd, Groups[i].cfg_tloc_size - ������ ������ ����������������� ����� ���������� ���� ������ �������
// Groups[i].cfg_locev_size, sizeof ( UINT ) - ������ ����������������� ����� ��� ���������� ���������� ���� ���������� ������������� ��������� �� �������, ��. C2DView::QueryCfgSize ( void ) �� ����� cfg
// Groups[i].cfg_locev_wnd, Groups[i].cfg_locev_size - ������ ������ ����������������� ����� ���������� ���� ���������� ������������� ��������� �� �������
// Groups[i].cfg_amp_size, sizeof ( UINT ) - ������ ����������������� ����� ��� ���������� ���������� ���� ����������� ���������, ��. C2DView::QueryCfgSize ( void ) �� ����� cfg
// Groups[i].cfg_amp_wnd, Groups[i].cfg_amp_size - ������ ������ ����������������� ����� ���������� ���� ����������� ���������
// Groups[i].imp_amp_size, sizeof ( UINT ) - ������ ����������������� ����� ��� ���������� ���������� ��������� ������������� ������� ���� ����������� ��������� ����� sizeof ( IPData )
// Groups[i].imp_amp_wnd, Groups[i].imp_amp_size - ������ ������ ����������������� ����� ���������� ��������� ������������� ������� ���� ����������� ���������, ��������� IPData
// Groups[i].imp_3Dflag, sizeof ( BOOL ) - ���� ��������� ������������� ������� �� ���� �������� ������ �������
if ( Groups[i].imp_3Dflag )	
// Groups[i].imp_3Dname, sizeof ( chan ) * 260 - ����-�������� 3D-������� (��������� �������: *.ase)

// ��������� ��������������� ������������ ��������
#define MAX_ALARM_LEVEL		3
typedef struct TAGLocDistDef
{
	char		Name [ 100 ];	//��� �������
	UINT		Thickness;		//������� ����������� � 3D
	long		Xmax;			//������� ������� �� �
	long		Xmin;			//������� ������� �� �
	long		Ymax;			//������� ������� �� Y
	long		Ymin;			//������� ������� �� Y
	long		Zmax;			//������� ������� �� Z
	long		Zmin;			//������� ������� �� Z
	long		X[4];			//������� ���������
	long		Y[4];
	long		Z[4];
	long		X1[4];			//����� ���������
	long		Y1[4];
	long		Z1[4];
	long		X2[4];			//������ ���������
	long		Y2[4];
	long		Z2[4];
	UINT		num_lgroup;		//����� ������ (����) ����������� �����������
	BOOL		IUse[MAX_ALARM_LEVEL];		//���� ������������� ������ �� ������������� ����������
	UINT		ILevel[MAX_ALARM_LEVEL];	//������� ���������� ������ �� ������������� ����������
	UINT		IValue;						//������� �������� �������� ���������� �������
	COLORREF	IColor[MAX_ALARM_LEVEL];	//���� ����������� ������ �� ������������� ����������
	BOOL		DUse[MAX_ALARM_LEVEL];		//���� ������������� ������ �� �������� ����������
	UINT		DLevel[MAX_ALARM_LEVEL];	//������� ���������� ������ �� �������� ����������
	UINT		DValue;						//������� �������� �������� �������� ����������
	COLORREF	DColor[MAX_ALARM_LEVEL];	//���� ����������� ������ �� �������� ����������
	int			Divider;					//�������� �������� ���������� (�������-�����)
	char		flagsNamePrint;				//����� ��������� ����� � � ��� ����� �� ����������
	char		flagsValPrint;				//����� ��������� �������� � �������� ����������
	char		NameValPos;					//������� ����� � �����, ��.-Name, ��.-Val, �� 4 ����
	char		bMustRedraw;				//���� ������������� ��������� ��� ������� ��������
	UINT		reserved [49];				//reserved
}LocDistDef;
// Groups[i].bEnableLocCrit, sizeof ( BOOL ) - ���� ��������� ������������ ��������
// Groups[i].num_dist, sizeof ( UINT ) - ����� ��������-��������� ������� ������� ��� ������������� ������������ ��������
if ( Groups[i].num_dist ) 
// Groups[i].list_dist, sizeof ( LocDistDef ) * Groups[i].num_dist - ������ ������������� ������������ ��������

// ������ ���������� ������� ���������
//Structure GraphBuf
typedef struct TAGGraphBuf
{
	float distance;			//����������, �
	float amplitude;		//���������, ��
}GraphBuf;
// Groups[i].attenuation_buf_count, sizeof( int ) - ����� ����� � ������� ���������
// �� ����� ���������� � ����������� ����� � ������� ��������� Groups[i].attenuation_buf_count, ������ ������ ��������� sizeof (GraphBuf)
// Groups[i].attenuation_buf, sizeof( GraphBuf ) * Groups[i].attenuation_buf_count - ������ ����������� ������� ���������

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

////////////////////////////////////////////////////////////////////////

//Struct member alignment ����������� �� ��������� � MSVisualC++, �.�. ������ 8 ����

///////////////THE END//////////THE END////////THE END/////////////////
