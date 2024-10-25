// ������ ����� ������ �� ������ A-Line DDM, DDM-2, PCI-N � PCI8 ��� ���� ������ //created 08.05.2020

// ������� ������� ��������� ����� ������ TAGFileHdr � ��������� ��������� TAGMeasData
////////////////////////////////////////////////////////////////
typedef struct TAGFileHdr
{
    UINT signature;         // signature = 0xAA551313
    UINT ver_maj;           // version major = 43 ��� ������� ������ ������
    UINT ver_min;           // version minor = 0 ��� ������� ������ ������
    UINT ftype;             // type of file	= 0xAA01 ��� ����� ������ ald-����
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
// ����� ������� ���� ������������ � ������� ��� ������ ��������������� ���� ������ (��. "������ ����� cfg 6.0.cpp")
////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
// ����� ���������� ������

enum	DataTypes { ev_data_old = 0, sys_data = 1, noise_data_old = 2, param_data = 3, noise_data = 4, ev_data = 5, marker = 6, text = 7, reserv = 8, service_data = 9,	u_param_data = 34, radio_data = 11, u_avg_data = 33 };

WORD	type; //��� ������ �������� DataTypes
WORD	size; //������ ��������� ��������������� ��������� ���� type
//				sizeof(EV_DATA_OUT_OLD) - ev_data_old, ���������� ��� ������� � ����������� ��-���������, �� ������������
//				sizeof(CH_INFO_OUT)		- sys_data, ������� ��������-���������� ����
//				sizeof(EV_DATA_OUT)		- noise_data_old, ���������� ��� �������������� �������, �� ������������
//				sizeof(EV_PARAM_OUT)	- param_data, ������� � ������� ��������������� ���������
//				sizeof(EV_AVG_OUT)		- noise_data, ������� � ������������ ������� ��-���������
//				sizeof(EV_DATA_OUT)		- ev_data, �������� ��� ������� � ����������� ��-���������
//				sizeof(MARKER_OUT)		- marker, ������� � �������� ���������� �������
//				sizeof(TEXT_OUT)		- text, ������� � ������� ���������� �������
//				1,						- reserved, ������
//				sizeof(SERVICE_OUT)		- service_data, ������� �� ���������� ���������������� �������
//				sizeof(UNI_PARAM_OUT)	- u_param_data,	������� � ������� ��������������� ��������� �� ������� �������
//				sizeof(RADIO_OUT)		- radio_data, ������� �� ���������� ���������������� ������� ��������� � ������ �����
//				sizeof(UNI_AVG_OUT)		- u_avg_data, ������� � ������������ ������� ��-��������� �� ������� �������


typedef struct TAG_AETIME
{
	__time32_t	seconds;	// absolute seconds value suitable for C time functions
	ulong	usec;			// microseconds since last second
}AE_TIME;

typedef struct TAG_EV_DATA_OUT_OLD	//ev_data_old - �� ������������
{
	AE_TIME		ev_time;	 	// start time of the event
    WORD		chan_num;		// channel number
	WORD		max_ampl;		// maximum amplitude
	UINT		duration;		// event duration in microseconds since start time
	UINT		rise_time;	    // event rise time in microseconds since start time	
    UINT        tig;			// tig or count
	WORD		energy_low;		// energy low word
	WORD		energy_medium;	// energy medium word
	WORD		energy_high;	// energy high word
	WORD		flags;			// validity flags
    UINT		loc_event;		// flag of localized event: 0-not, 1-sucsess, 5-must be deleted	//04.09.03S
    LONG        locX;			// location X-coordinate [mm] (NO_LOC) - no location by this event, (SEVERAL_LOC) - used for several locations,
    LONG        locY;			// location Y-coordinate [mm]
}EV_DATA_OUT_OLD;	

typedef struct TAG_CH_INFO_OUT	// sys_data
{
	AE_TIME		chan_info_time;	// ����� ������� �������
	WORD		chan_num;		// ����� ������
	WORD		type;			// ��� �������
	WORD		data[8];		// ������
}CH_INFO_OUT;
// �� ������� ������� ���� ����� ��������� �������� �������������� ������ � ��������
// ����� ���� �������� ���� type:
// ��� type = 18 � type = 3 � ���� data[0] � ����� �������� ������ �������������� ������ � ��. ���
// ��� type = 14 � ���� data[0] � ����� �������� ������ �������������� ��� � ��


#define NO_LOC		-1000000001
#define SEVERAL_LOC	-1000000002
typedef struct TAG_EV_DATA_OUT	// ev_data
{
	AE_TIME		ev_time;	 	// ����� ������� ��������
    WORD		chan_num;		// ����� ������
	WORD		max_ampl;		// ������� 16-������ ����� ��������� ��������
	WORD		duration;		// ������� 16-������ ����� ������������ ��������
	WORD		expnd;			// ����� �������������� �������� � ������� ��������� ��� 18-������ ������ (DDM-2)
	WORD		rise_time;	    // ������� 16-������ ����� ����� ���������� ��������� ������
	WORD		flags_add;		// ���������� ���� ������ ��� 18-������ ������ (DDM-2)
    WORD        tig;			// ������� 16-������ ����� ����� �������� (����������)
	WORD		attenuation;    // �����. ��������� ��������� (������� �� 10) ��� ���������� �������� ����������� ���������
	WORD		energy_low;		// ������� �������� (������� �����)
	WORD		energy_medium;	// ������� �������� (������� �����)
	WORD		energy_high;	// ������� �������� (������� �����)
	WORD		flags;			// �����
	WORD		tad;			// ���, ���
    BYTE		loc_event;		// ���� �������������� ��������������� �������: 0-���, 1-��, 5-�������
	BYTE		nsec;			// ������� ���������� ����� ��������� ������ ������������ (DDM-2), ���������� �� ������� ������� ev_time
    LONG        locX;			// ����������� ���������� X [��], (NO_LOC) - ��� �������, (SEVERAL_LOC) - �������� ��������� �������
    LONG        locY;			// ����������� ���������� Y [��]
    LONG        locZ;			// ����������� ���������� Z [��]
	LONG		gain;			// ������� �������� �������� [dB]
}EV_DATA_OUT;

//������ ���������� 18-������ ������� ���������� ��-��������� � ������ ����� �������������� �������� ��������� ���:
//	max_ampl_full	= ( DWORD ( EV_DATA_OUT.expnd & 0x08 ) << 13 ) + EV_DATA_OUT.max_ampl;
//  tig_full		= ( DWORD ( EV_DATA_OUT.expnd & 0x80 ) << 9 ) + EV_DATA_OUT.tig;
//	duration_full	= ExtractTimeQuant ( EV_DATA_OUT.expnd, 10 ) * (( DWORD ( EV_DATA_OUT.expnd & 0x07 ) << 16 ) + EV_DATA_OUT.duration );
//	rise_time_full	= ExtractTimeQuant ( EV_DATA_OUT.expnd, 10 ) * (( DWORD ( EV_DATA_OUT.expnd & 0x70 ) << 12 ) + EV_DATA_OUT.rise_time );
double ExtractTimeQuant ( WORD expnd, int shift )
{
	switch (( expnd >> shift ) & 0x07 )
	{
	case 2:	return 2.;
	case 4:	return 0.5;
	case 6:	return 0.25;
	}
	return 1.;
}
//	energy_full		= ConvertEnergy ( &EV_DATA_OUT );
double ConvertEnergy ( EV_DATA_OUT* data )
{
	DWORD tmp;
	if ( data->expnd )
	{
		tmp = MAKELONG ( data->energy_medium, data->energy_high );
		return double ( _int64 ( tmp >> 5 ) << WORD ( tmp & 0x1F ));
	}
	tmp = MAKELONG ( data->energy_low, data->energy_medium );	
	return double ( tmp ) + ( data->energy_high ) * pow ( 2., 32 );
}
// flags-����� �� ������������ ��� (�), ����. ������������ (D) �� ������� ��������
// EV_DATA_OUT.flags & 0x01?"A":" - ";
// EV_DATA_OUT.flags & 0x04?"D":" - ";
// ����������� ��������� LA = int(( EV_DATA_OUT.flags & 0xFFF0 ) >> 4);	
// ���� ���������� ������� ���������� ("+" - ����� �������� � ������������� ��������� ���������, "-" - � �������������) - ������ ��� expnd != 0
// EV_DATA_OUT.flags & 0x08? "-":"+"
// ���� ���������� ��������� ("+" - �������� ��������� � ������������� ��������� ���������, "-" - � �������������) - ������ ��� expnd != 0
// EV_DATA_OUT.flags_add & 0x01? "-":"+"

typedef struct TAG_EV_PARAM_OUT	// param_data (��� DDM-2 �� ������������)
{
	AE_TIME		param_time;						// ����� ������� ��������������� �������
	WORD		chan_num;						// ����� ������
	WORD		num_summ;						// ����� ������������ �������� ����������
	WORD		par_summ [ NUM_PAR_PER_MOD ];	// ��������� �������� ���������� �� �������
}EV_PARAM_OUT;
// �������� �������� ��������������� ��������� ��������� ����� ��������� �� �������
// V[i] = ( par_summ[i] / num_summ ) * K1[i] + K2[i], ��� i-����� ���������������� �����, �1[i]-�����������������, �2[i]-���������� ������������ ��������� �� ParDefMod (��. ������ ����� ������������)

typedef struct TAG_EV_AVG_OUT	// noise_data - ������� ��������������� ����, �������� ���������� ���������� ������� �������� �� ������ ���������� (1 ���)
{
	AE_TIME		avg_time;				// ����� �������
	WORD		chan_num;				// ����� ������
	WORD		num_summ;				// ����� ������������ ��� ���������� �������� ������ ���� (=1)
	DWORD		noise_summ_ampl;		// ��������� ������������ ��� ���������� �������� ������ ����
	WORD		intensity;				// ���������� ��
	WORD		lost;					// ����� ��������� �� ��������� (��-�� ����������� ������������ ��������)
	double		summ_energy;			// ������� �� ���������
	DWORD		summ_dur;				// ������������ �� ���������
	DWORD		summ_ampl;				// ��������� �� ���������
	DWORD		summ_rise;				// ����� ���������� ��������� ������
	DWORD		summ_tig;				// ����� �������� (����������)
	WORD		max_ampl;				// ������������ ��������� �� �������� �� ������ ����������
	WORD		expndw;					// ����� �������������� �������� � ������� ��������� ��� 18-������ ������ (DDM-2)
	_int8		cur_gain_dB;			// ������� �������� �������� [dB]
	BYTE		expndb;					// ���� �������������� �������� � ������� ��������� ��� 18-������ ������ (DDM-2)
	WORD		reserved[7];
}EV_AVG_OUT;

//������ ���������� 18-������ ������� ����������� ���������� ��-��������� � ������ ����� �������������� �������� ��������� ���:
//	noise_summ_ampl_full = ( DWORD ( EV_AVG_OUT.expndb & 0x08 ) << 13 ) + EV_AVG_OUT.noise_summ_ampl;
//	intensity_full	= ( DWORD ( EV_AVG_OUT.expndb & 0x30 ) << 12 ) + EV_AVG_OUT.intensity;
//	energy_full		= EV_AVG_OUT.summ_energy / double ( intensity_full );

//����� ������ ������ ��� EV_AVG_OUT.expndb != 0 � EV_AVG_OUT.expndw != 0 (��� 18-������ ������, � ��� ����� DDM-2):
//	max_ampl_full	= ( DWORD ( EV_AVG_OUT.expndw & 0x08 ) << 13 ) + HIWORD ( EV_AVG_OUT.summ_dur );
//  tig_full		= ( DWORD ( EV_AVG_OUT.expnd & 0x80 ) << 9 ) + HIWORD ( EV_AVG_OUT.summ_ampl );
//	duration_full	= (( DWORD ( EV_AVG_OUT.expndw & 0x07 ) << 16 ) + LOWORD ( EV_AVG_OUT.summ_dur )) * AE_Kernel->ExtractTimeQuant ( EV_AVG_OUT.expndb );
//	rise_time_full	= (( DWORD ( EV_AVG_OUT.expndw & 0x70 ) << 12 ) + LOWORD ( EV_AVG_OUT.summ_ampl )) * AE_Kernel->ExtractTimeQuant ( EV_AVG_OUT.expndb );
//	ASL_value		= (( DWORD ( HIWORD ( EV_AVG_OUT.summ_rise )) & 0x100 ) << 8 ) + HIWORD ( EV_AVG_OUT.summ_tig );
//	RMS_value		= (( DWORD ( HIWORD ( EV_AVG_OUT.summ_rise )) & 0xFF ) << 16 ) + EV_AVG_OUT.max_ampl;

//	� ��������� ������ ��� EV_AVG_OUT.expndb == 0 (��� 16-������ ������) ������� ���:
//	max_ampl_full	= round ( EV_AVG_OUT.summ_ampl / double ( intensity_full ));
//	duration_full	= round ( EV_AVG_OUT.summ_dur / double ( intensity_full ));
//	rise_time_full	= round ( EV_AVG_OUT.summ_rise / double ( intensity_full ));
//	tig_full		= round ( EV_AVG_OUT.summ_tig / double ( intensity_full ));
//	ASL � RMS � ���� ������ �� �����������

typedef struct TAG_MARKER_OUT		// marker
{
	AE_TIME		marker_time;		// ����� ������� �������
	DWORD		markerID;			// ������������� ������� ��� �������� � ������ �������
}MARKER_OUT;

typedef struct TAG_TEXT_OUT			// text
{
	DWORD		markerID;			// ������������� ������� ��� �������� �� ������� �������
	char		marker_text[68];	// ����� �������
}TEXT_OUT;

// ����������� ��������� � [��] ����������� ��� �[��] = 20.*log10 ( 1000. * (max_ampl_full, ASL_value)) * d_ADC / Din - gain, ���
// max_ampl_full ��� ASL_value - ��������� � ��. ���, ������� �� EV_DATA_OUT � EV_AVG_OUT
// gain - �������� ( ������ ��� � �� ) 
// ������� ���� �� EV_DATA_OUT ��� FullDefMod
// Dadc - ADC step value in mkV, ������� �� d_ADC - FullDefMod ������� ������ (��. ������ ����� cfg),
// Din - scale base value in nV, ������� ���������� �� ��������� ����������� 1 ��� (��. ������ ����� cfg), 

// ������� [��] = 10.* log10 ( energy_full * 1000./ F) + 20. * log10(K_mkV), ���
// energy_full - ������� � ��. ���, ������� �� EV_DATA_OUT,
// F - ������������ ������� ��������� ������ - freq_max �� OSCDefMod;
// K_mkV = d_ADC / pow ( 10., gain / 20. ) 

////////////////////////////////////////////////////////////////////////

//Struct member alignment ����������� �� ��������� � MSVisualC++, �.�. ������ 8 ����

///////////////THE END//////////THE END////////THE END/////////////////
