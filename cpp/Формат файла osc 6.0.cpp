// ������ ����� ������������ �� ������ A-Line DDM, DDM-2, PCI-N � PCI8 ��� ���� ������ //created 08.05.2020

// ������� ������� ��������� ����� ������ TAGFileHdr � ��������� ��������� TAGMeasData
////////////////////////////////////////////////////////////////
typedef struct TAGFileHdr
{
    UINT signature;         // signature = 0xAA551313
    UINT ver_maj;           // version major = 22 ��� ������� ������ ������
    UINT ver_min;           // version minor = 0 ��� ������� ������ ������
    UINT ftype;             // type of file	= 0xAA02 ��� ����� ������������ osc-����
	UINT hard_version;		// version of hardware controller //������� ������ DDM-2 ����������� = 0x102071f (����������� �� ���������� �����)
	UINT soft_version;		// version of sort options //������� ������ �� A-Line = 0x20600
    UINT reserved[18];      // reserved bytes for future use
}FileHdr;

////////////////////////////////////////////////////////////////
typedef struct TAGMeasData
{
    UINT ver_maj;                   // version number major //������
    UINT ver_min;                   // version number minor //������
    char obj_name[255];             // object name
    char oper_name[255];            // operator name
    char reference[255];            // references
    UINT comsize;                   // comments size
    char comments[1000];            // comments text
	UINT filtered_flag;				// flag of filtered file: 0-source, 1-filtered, 2-location filter, 3-concatenation result, 4-clb_location filter
    UINT meas_hour;                 // measurement maximal hours
    UINT meas_min;                  // and minutes
    UINT init_hour;                 // initial scale hour
    UINT init_min;                  // initial scale min
    __time32_t start_time;              // start time
    __time32_t stop_time;               // stop time
    UINT avg_time;                  // averaging time in microseconds
    char data_name[255];            // file name to store data
    char cfg_name[255];             // file name to store configuration
    char sdo_name[255];             // file name to store SDO wave
    char source_name[255];          // file name of source file for secondary generation
    char local_name[255];           // file name of localization file
    UINT num_SDO;                   // number of SDO wave to write
    BOOL DontWriteDisk;             // prevent writing data to disk in real-time
    BOOL DontWritePar;              // prevent writing parametric to disk in real-time
    BOOL DontWriteNoise;            // prevent writing noise data to disk in real-time
    BOOL DontWriteSDO;              // prevent writing SDO wave to disk in real-time
	BOOL ActivatePreFilter;			// use pre-filter during on-line
	PreFilterData	PFData;			// struct of prefilter data used
	UINT IsCoorPresent;				// flag of localization coordinates presence: 1-X, 2-X,Y
	UINT TypeLoc;					// type of local.scheme (linear = 0, plane = 1, volume = 2, sphere = 3, cover =  4, threeD = 5, zonary = 6, picture = 7, cone = 8, rvs_wall = 9, calibr = 10)
    LONG LENGTH;					// size of local.sheme: x_max;
    LONG HEIGHT;					// y_max;
    LONG SHIFT_HEIGHT;				// y_min;
	LONG SHIFT_LEFT;				// x_min;
	UINT ClusterLength;				// length of the cluster
	UINT ClusterWidth;				// width of the cluster
	UINT BOUND;						// location Region Bound
	int  SumEvents;					// (0 -> ��� ����������; -1 -> 0 �������)
	UINT MeasType[4];				// ��������� �������� ��������� ���������: (0 - MaxEvents, 1 - MaxTigs, 2 - MaxFileSize, 3 - MaxOscNum)
	short IsMeasTime;				// ������� ��������� �� ��������� ������� ���������
	short IsEvents;					// ������� ��������� �� �������� ���������� ������������� ����� ��������� �� - MaxEvents
	short IsTig;					// ������� ��������� �� �������� ���������� ������������� ����� �������� �� - MaxTigs
	short IsSize;					// ������� ��������� �� �������� ���������� ������������� ������� ����� ������ - MaxFileSize
	short IsOSC;					// ������� ��������� �� �������� ���������� ������������� ����� ������������ - MaxOscNum
	short IsRepeat;					// ���� ��������������� ����������� ��������� ����� ��������� �����������
	UINT  Type;						// ��� �������������� �������� (0 - manual stop (default), 1 - max events, 2 - max tig, 3 - max file size, 4 - max osc num, 5 - max time expired)
	UINT Reserved[2];				// reserved
}MeasData;

//	��� PreFilterData - ���������� �� �������������� � ���� ����� ������ ����������
#define MAX_FDEFS 4

typedef struct TAGFilterDef
{
    UINT	param;					// parameter for filtration
    double	more_than[3];			// 3 minimal value interval		
    double	less_than[3];			// 3 maximum value interval		
    char	p_name[100];			// name of parameter
    byte	valid;					// validiti of filter			
	byte	interv_valid[3];		// 3 value validiti of interval	
}FilterDef;

typedef struct TAGPreFilterData
{
	byte		DontSave;			// don't save events not passed throuth filter
	byte		ver_PFilter;		// 0-old, 1-new 
	byte		reserved[2];		
	BOOL		DontProcess;		// don't process events not passed throuth filter
	BOOL		IsKeep;				// keep selected events or delete them
	BOOL		AndOr[MAX_FDEFS];	// massiv or/and switches for conditions
	FilterDef	FltDef[MAX_FDEFS];	// set of filters
}PreFilterData;

////////////////////////////////////////////////////////////////
// ����� ������� ������������ ���� ����� ������������
////////////////////////////////////////////////////////////////
typedef struct TAGSDOHdr
{
	UINT NumOSC;			// ����� ����� ���������� ������������
	UINT InfoPointer;		// ��������� �� ��������� ���� ������ ������������ � ����� (���� ����� ���������� ������������ ���������)
	UINT DefSize;			// ������ ������ ������������, ������������� �� ��������� (����������� �� ���������� � � ��������� ������� �� �� ������������)
	UINT BaseSize;			// ������ ��������� OSCDefMod (��. ����)
	UINT Din;				// �������� ������� ��� � ��
    UINT reserved[16];      // reserved bytes for future use
}SDOHdr;

////////////////////////////////////////////////////////////////
// ����� ������� ���� ������������ � ������� ��� ������ ��������������� ���� ������ (��. "������ ����� cfg 6.0.cpp")
////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
// ����� ���������� ������
// ������� ��� ���������-��������� �������������, ����� ������
// ����� �� �� ����� ������ ������������ SDOHdr.NumOSC
// ���� seek_wave � seek_wave_high - ��������� ��� ��������� ������ ������������ ������������� � ����� osc
typedef struct TAGOSCDefMod
{
	UINT		chan;			//����� ������
	UINT		num_osc;		//0-A, 1-B 	
	BOOL		enabled;		//������� OSC (A ��� B)
	COLORREF	osc_color;		//����� ����������� osc
	BOOL		is_color_chan;	//���� ����������� ������ ������
	BOOL		save;			//���� ����������
	UINT		num_wind;		//����� ���� ��� �����������
	UINT		freq;			//�-�� �������������, ���
	UINT		buf_size;		//���������� �����
	UINT		buf_time;		//������������ ������, ��� (���������)
	UINT		pre_mod;		//���-�� ����� ��������������
	UINT		sync;			//0 - �����������, 1 - ���������� ������, 2 - ���������� �����, 3 - ���������� ������/�����
	BOOL		is_th_chan;		//���� ������������� ������ ������
	float		tresh_db_osc;	//���������
	UINT		tresh_ADC_osc;	//��������������� �������� ��������������� ������ �������������
	double		K_mkV;			//��� ���������� ��������� � ��� � ��		
	UINT		seek_wave;		//�������� ������ ������������� � ����� (������� 32-������ �����)		
	AE_TIME		osc_time;		//����� ������� ������������� ����������	
	UINT		ADC_bit;		//����������� ���							
	BOOL		is_spect;		//���� ���������\���������� �������
	BOOL		changed;		//���� ����� ������������� ���������
	UINT		freq_max;		//������������ ������� ��������� ������, ���
	BOOL		notuse_pre_mod;	//���� ������������� ������ ��������������
	UINT		buf_size_max;	//������������ ������ ������
	UINT		seek_wave_high;	//�������� ������ ������������� � ����� (������� 32-������ �����)
	long		tFastest;		//����� ������� ����� ������� ����� (������������ ������ � ALine OSC Processing)
	long		tR;				//����� ������� ����� ����� (�����������, ������������ ������ � ALine OSC Processing)
	int			calibration;	//���� ��������� ������ ����������
	int			to;				//����� ��������� (������������ ������ � ALine OSC Processing ��� ������� ������������� ������)
	int			to_ald;			//����� ��������� �� ald (������������ ������ � ALine OSC Processing ��� ������� ������������� ������)
	float		L;				//���������� �� ��������� ���������	(������������ ������ � ALine OSC Processing ��� ������� ������������� ������)
	float		L_ald;			//���������� �� ��������� ��������� �� ald (������������ ������ � ALine OSC Processing ��� ������� ������������� ������)
	float		d;				//������� ������� (������������ ������ � ALine OSC Processing ��� ������� ������������� ������)
	WORD		bPowerMethod;	//���� ��������� ���������� ��������� (������ ��� 18-������ ������, � ��� ����� DDM-2)
	WORD		nsec;			//����� �������� ���������� ��������� � ������� ����� ���������� ������������ (DDM-2), ���������� �� ������� ������� osc_time
	BOOL		bSyncAEO;		//���� ������������� AE-OSC � �1� ��������� ������������� (������ ��� DDM-2)
	UINT		reserved[2];	
}OSCDefMod;

typedef struct TAG_AETIME
{
	__time32_t	seconds;	// absolute seconds value suitable for C time functions
	ulong	usec;			// microseconds since last second
}AE_TIME;

///////////////////////////////////////////////////////////////
// ����� ����� ������� ��� (�.�. ���� �������������) - ������� ������ �� buf_size ����, 
// ���� ����� - 2 �����, ��� ���� ����� ampl (������ ���)
// ��� 18-������ ������ ������������ ��������� �������� ��� ��������� 18-������� �������� ampl_full �� 16-������� ����� ampl (���� ��������� bPowerMethod ��� ���� ���������� � |= 0x100):
if ( bPowerMethod & 0x100 )
{
	long value = _int16 ( ampl );
	long base = value >> 2;
	switch ( labs ( value & 0x03 ) )
	{
case 0:	ampl_full = base; break;
case 1: ampl_full = base * 2; break;
case 2: ampl_full = base * 4; break;
case 3: ampl_full = base * 16; break;
	}
}
else	ampl_full = _int16 ( ampl );

// ��� �������:
// ��������� [��] = 20.*log10 ( 1000. * ampl_full ) * d_ADC / Din - gain, ���
// ampl - ��������� � ��. ���, ������� �� ������
// gain - �������� ( ������ ��� � �� ) 
// ������� �� FullDefMod
// Dadc - ADC step value in mkV, ������� �� d_ADC - FullDefMod ������� ������,
// Din - scale base value in nV, ������� ���������� 1 ���,

////////////////////////////////////////////////////////////////////////

//Struct member alignment ����������� �� ��������� � MSVisualC++, �.�. ������ 8 ����

///////////////THE END//////////THE END////////THE END/////////////////
