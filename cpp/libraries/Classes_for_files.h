#include "afxpriv.h"
#include <time.h>
//#include "PFilter.h"

#pragma once

#define MAX_FDEFS 4

// ������ ����� ������������ �� ������ A-Line DDM, DDM-2, PCI-N � PCI8 ��� ���� ������ //created 08.05.2020

// ������� ������� ��������� ����� ������ TAGFileHdr � ��������� ��������� TAGMeasData
////////////////////////////////////////////////////////////////
struct FileHdr
{
	UINT signature;         // signature = 0xAA551313
	UINT ver_maj;           // version major = 22 ��� ������� ������ ������
	UINT ver_min;           // version minor = 0 ��� ������� ������ ������
	UINT ftype;             // type of file	= 0xAA02 ��� ����� ������������ osc-����
	UINT hard_version;		// version of hardware controller //������� ������ DDM-2 ����������� = 0x102071f (����������� �� ���������� �����)
	UINT soft_version;		// version of sort options //������� ������ �� A-Line = 0x20600
	UINT reserved[18];      // reserved bytes for future use
};
struct FilterDef
{
	UINT	param;					// parameter for filtration
	double	more_than[3];			// 3 minimal value interval		
	double	less_than[3];			// 3 maximum value interval		
	char	p_name[100];			// name of parameter
	byte	valid;					// validiti of filter			
	byte	interv_valid[3];		// 3 value validiti of interval	
};
struct PreFilterData
{
	byte		DontSave;			// don't save events not passed throuth filter
	byte		ver_PFilter;		// 0-old, 1-new 
	byte		reserved[2];
	BOOL		DontProcess;		// don't process events not passed throuth filter
	BOOL		IsKeep;				// keep selected events or delete them
	BOOL		AndOr[MAX_FDEFS];	// massiv or/and switches for conditions
	FilterDef	FltDef[MAX_FDEFS];	// set of filters
};
////////////////////////////////////////////////////////////////
struct MeasData
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
};

//	��� PreFilterData - ���������� �� �������������� � ���� ����� ������ ����������
#define MAX_FDEFS 4





////////////////////////////////////////////////////////////////
// ����� ������� ������������ ���� ����� ������������
////////////////////////////////////////////////////////////////
struct SDOHdr
{
	UINT NumOSC;			// ����� ����� ���������� ������������
	UINT InfoPointer;		// ��������� �� ��������� ���� ������ ������������ � ����� (���� ����� ���������� ������������ ���������)
	UINT DefSize;			// ������ ������ ������������, ������������� �� ��������� (����������� �� ���������� � � ��������� ������� �� �� ������������)
	UINT BaseSize;			// ������ ��������� OSCDefMod (��. ����)
	UINT Din;				// �������� ������� ��� � ��
	UINT reserved[16];      // reserved bytes for future use
};

////////////////////////////////////////////////////////////////
// ����� ������� ���� ������������ � ������� ��� ������ ��������������� ���� ������ (��. "������ ����� cfg 6.0.cpp")
////////////////////////////////////////////////////////////////


typedef unsigned long ulong;
struct AE_TIME
{
	__time32_t	seconds;	// absolute seconds value suitable for C time functions
	ulong	usec;			// microseconds since last second
};
///////////////////////////////////////////////////////////////
// ����� ���������� ������
// ������� ��� ���������-��������� �������������, ����� ������
// ����� �� �� ����� ������ ������������ SDOHdr.NumOSC
// ���� seek_wave � seek_wave_high - ��������� ��� ��������� ������ ������������ ������������� � ����� osc
struct OSCDefMod
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
};




// ������ ����� ������������ �� ������ A-Line DDM, DDM-2, PCI-N � PCI8 ��� ���� ������ //created 08.05.2020

////////////////////////////////////////////////////////////////
//��������� ����� ������������
struct CfgFileHdr
{
	UINT magic_word;            // magic word = 0x1515CFCF - ������ ��� ������ cfg
	UINT ver_maj;               // major version number = 25- ��� ������� ������ �����
	UINT ver_min;               // minor version number = 0	- ��� ������� ������ �����
	UINT info_size;             // ����� ������ ������ ���������������� ������
	UINT reserved[20];          // reserved bytes for future                                     
};

////////////////////////////////////////////////////////////////
//���������������� ����-�����
struct CfgBlockDescr
{
	UINT block_type;            // block type;
	UINT block_offset;          // offset to the block in bytes from 
	// cfg file info structure
	UINT block_size;            // cfg block size
};
struct CfgFileInfo
{
	UINT num_cfg_blocks;        // number of configuration blocks in file
	UINT mem_size;              // size of configuration data
	CfgBlockDescr cfg_blocks[];  // unsized array of descriptors;
};

//����� ����� ����� CfgFileInfo ����� CfgFileHdr.info_size ����

////////////////////////////////////////////////////////////////
//����� ������� ����� "configuration data" ����� ������ ������ CfgFileInfo.mem_size ����,
//����� ������� �� num_cfg_blocks ������, ������ �� ������� ��������������� ����� ������������ CfgBlockDescr.
//� ����������� � block_type = X �� ������� �������� block_offset - �������� ������ ����� X ������������ ������ ������ "configuration data"
//� �������� block_size - ������ ����� hardware configuration.

//������ ������� ���������������� ������
#define		CFG_HARDWARE		1       // hardware configuration block - ����� ������ ���� ��������� ���������� �����
#define		CFG_GAINWND			2       // GAINWND - ���������������� ���� �������� ��������� ���� "��������/�����" (��� DDM-2 ������� ������ �� �����, ��� ��� �������� ��������� ��� ����� ���� ������)
#define		CFG_MAINFRAME		3       // MAINFRAME - ���������������� ���� �������� ��������� �������� ���� ���������
#define		CFG_AMPWND			4       // AMPWND - ���������������� ���� �������� ��������� ���� "������� ���������/�����"
#define		CFG_AMPTOTALWND		5       // AMPTOTALWND - ���������������� ���� �������� ��������� ���� "������� ��������� ���������/�����"
#define		CFG_EVWND			6       // EVWND - ���������������� ���� �������� ��������� ���� "���������� �� /�����"
#define		CFG_EVSUMWND		7       // EVSUMWND - ���������������� ���� �������� ��������� ���� "��������� ���� ��������� �� /�����"
#define		CFG_EVRUNWND		8       // EVRUNWND - ���������������� ���� �������� ��������� ���� "��������� ���� ��������� �� /�����"
#define		CFG_EVTOTALWND		9		// EVTOTALWND - ���������������� ���� �������� ��������� ���� "��������� ���������� / �����"
#define		CFG_RISEWND			10      // RISETIMEWND - ���������������� ���� �������� ��������� ���� "������� ����� ���������� / �����"
#define		CFG_DURWND			11      // Duration Wnd - ���������������� ���� �������� ��������� ���� "������� ������������ / �����"
#define		CFG_ALARMS			12      // alarms settings
#define		CFG_PRINT			13      // printer settings - �� ������������
#define		CFG_ENERGYWND		14      // energy window - ���������������� ���� �������� ��������� ���� "������� ������� /�����"
#define		CFG_DUMMY			15      // DUMMY - �� ������������
#define		CFG_TIMER			16      // timers - �� ������������
#define		CFG_NOTOTALWND		17      // NOISETOTALWND - ���������������� ���� �������� ��������� ���� "������� ������� ���� / �����"
#define		CFG_TIGWND			18      // Tig window - ���������������� ���� �������� ��������� ���� "������� ����� �������� � �� ��������/�����"
#define		CFG_EVERYAMPWND		19		// EveryAmp real-time window - �� ������������
#define		CFG_MAXAMPWND		20		// MaxAmp real-time window - ���������������� ���� �������� ��������� ���� "������������ ���������/�����"	
#define		CFG_GENERAL			21		// General setup configuration
#define		CFG_AUTOSETUP		22		// Autothreshold setup configuration
#define		CFG_POSTWNDSOLD		23		// PostWnds	(������ ������)	- �� ������������
#define		CFG_HISTWNDSOLD		24		// HistWnds	(������ ������)	- �� ������������
#define		CFG_MEASDATA		25		// MeasData	- ��������� ���������� ������ ��������� (��. ��������� MeasData ����)
#define		CFG_ENRSUMWND		26		// Energy summary window - ���������������� ���� �������� ��������� ���� "����� ������� / �����"
#define		CFG_SYSTEMINFO		27		// ���������� �� ���������� ����� ��������� � ������ �� ��� ������������� �����
#define		CFG_TIGSUMWND		28      // Tig summary window - ���������������� ���� �������� ��������� ���� "��������� ���� �������� �� / �����"
#define		CFG_NOWND			29      // Noise window - ���������������� ���� �������� ��������� ���� "������� ������� ���� / �����"
#define		CFG_CNTSUMWND		30      // Count Summ window - ���������������� ���� �������� ��������� ���� "��������� ���� �������� �� / �����"
#define		CFG_HOTKEYS			31      // Hot Key data configuration block
#define		CFG_MAXAMPCOUNTWND	32		// Amplitude/Count window - ���������������� ���� �������� ��������� ���� "������������ ��������� / ������������ ����� ��������"
#define		CFG_LOCPICT2D		33		// ���������������� ���� �������� ��������� ���� ������� ���� "�������" 2D
#define		CFG_LOCPICT3D		34		// ���������������� ���� �������� ��������� ���� ������� ���� "�������" 3D
#define		CFG_POSTWNDS		35		// PostWnds	(����� ������) - ���������������� ���� �������� �������������� ����
#define		CFG_HISTWNDS		36		// HistWnds	(����� ������) - ���������������� ���� �������� ������������� ����
#define		CFG_IPDEVICE		37		// IP-Addresses configuration block	
#define		CFG_EXPORT			38		// Export configuration block
#define		CFG_SERVWND			39      // ServWnd - ���������������� ���� �������� ��������� ���� "��������� ��������� / �����"
#define		CFG_AMPCRIT			40		// Amplitude criterion block
#define		CFG_COUNTWND		41      // CountWnd - ���������������� ���� �������� ��������� ���� "�������� ����� / �����"
#define		CFG_LEVRCVRWND		42		// Level of transmitter signal window - ���������������� ���� �������� ��������� ���� "������� ������� ����������� ��� / �����" (������ ��� ������ � ������������)
#define		CFG_LEVPPSWND		43		// Level of receiver signal window - ���������������� ���� �������� ��������� ���� "������� ������� ��������� ��� / �����" (������ ��� ������ � ������������)
#define		CFG_NUMINTWND		44		// Intensity of data reception window - ���������������� ���� �������� ��������� ���� "������������� ������ / �����" (������ ��� ������ � ������������)
#define		CFG_ERRINTWND		45		// Error rates window - ���������������� ���� �������� ��������� ���� "������������� ������ ������ / �����" (������ ��� ������ � ������������)
#define		CFG_POWERWND		46		// Battery life	- ���������������� ���� �������� ��������� ���� "����� ��� / �����" (������ ��� ������ � ������������)
#define		CFG_FONT			47		// ��������� ��������� ������
#define		CFG_EVTOTALSUMWND	48		// Sum of all impulses Wnd - ���������������� ���� �������� ��������� ���� "��������� ����� ��������� �� / �����"	
#define		CFG_RMSWND			49		// RMS window - ���������������� ���� �������� ��������� ���� "RMS / �����"
#define		CFG_ASLWND			50		// ASL window - ���������������� ���� �������� ��������� ���� "ASL / �����"

#define		CFG_SDOWND			100     // SDOWND configuration block - ���������������� ���� �������� ��������� ���� "������������� / �����"
#define		CFG_SPECTRWND		150		// spectrum window configuration block	- ���������������� ���� �������� ��������� ���� "��������� ������ / �����"
#define		CFG_PARAMETWND		200		// Paramet windows configuration block	- ���������������� ���� �������� ��������� ���� "<��������������� ����� �> / �����"
#define		CFG_PAGE			250		// Page configuration block
//CFG_ID ���� �� ������ ��������� ����������� ���
//CFG_XXX � ������� ����� �������� ����� +
// '����� ��������'� ������� ����� �������� �����	//24.07.00S
#define		CFG_PROFILE			251		// Profile configuration block	
#define		CFG_IMPLINES		252		// Import Line configuration block	
#define		CFG_SERVICE			253		// Controller settings block	
#define		CFG_SYNCVIEW		254		// SyncView window settings block - ���������������� ���� �������� ��������� ���� "���������� ��������� �������� ��������� ��"


struct ColorMod
{
	COLORREF	ch_color;
	BOOL		changed;
	UINT		reserved[16];
};
struct ChanDefMod
{
	BOOL		enabled;		//��������� (0-��������)
	BOOL		show;			//����������� (0-�� ������������)
	UINT		high_filter;	//���, ���
	UINT		low_filter;		//�H�, ���
	long		gain_db;		//��������� �������� �������� ������
	float		d_ADC;			//����������� �� ��� ������ �������������� ���	
	float		tresh_db;		//���������
	UINT		tresh_ADC;		//��������������� �������� ������ ������ (� ��. ���)
	BOOL		changed;		//���� ����� ������������� ���������
	BOOL		changed_ON;		//���� ����� ������������� ��������� ��������� enabled
	BOOL		autotresh;		//���� ��������� ����������
	UINT		reserved[14];
};
struct TimeDefMod
{
	UINT		sceto;			//��� (SCETO)
	UINT		rtto;			//RTTO - �� ������������ (������ ����� max_dur)
	UINT		d_time;			//������� �����
	UINT		max_dur;		//������������ ������������
	BOOL		changed;		//���� ����� ������������� ���������
	UINT		reserved[16];
};
struct CalibrDefMod
{
	BOOL		enabled;		//������� ����������
	UINT		K_calibr;		//�����-� ����. (���) - �� ������������
	float		d_DAC;			//����������� �� ���, �.�. =((float)REF_DAC / (float)DAC_BIT)
	UINT		Amp_v;			//���������, B
	UINT		Amp_DAC;		//��������������� �������� ��������� �������������� �������
	UINT		Freq_Calibr;	//�� ������������
	BOOL		changed;		//���� ����� ������������� ���������
	BOOL		changed_ON;		//���� ����� ������������� ��������� enabled
	UINT		mode;			//������������� ����� ������ ����������� ��� DDM-2 (0�passive,not sync osc / 1�active,radiate / 2- passive,sync osc / 3 - AST)
	UINT		Dur_DAC;		//������������ �������� ������ �������������� AST
	float		F1;				//�������� ����� ������� ������ ������� ��� ������� ����������
	float		F2;				//�������� ������ ������� ������ ������� ��� ������� ����������
	float		Amin;			//����������� �������� ��������� ����������������� ��������
	float		Smin;			//����������� �������� ������������� ��������� ����������� ������� �������
	float		Smax;			//������������ �������� ������������� ��������� ����������� ������� �������
	UINT		PAEtype;		//��� ��������������� � ������ �������										
	UINT		reserved[7];
};
struct ParDefMod		//��� DDM-2 ���� �� ������������
{
	BOOL		enabled;		//������� ���. �����
	float		K_par;			//�����-� ����. (���)
	float		d_ADC_par;		//����������� �� ���, �.�. =((float)REF_ADC_PAR / (float)ADC_BIT_PAR)
	WORD		Freq_par;		//������� ������ ������, Hz
	_int16		Freq_multi;		//��������� ��������� ������� ������ ����������
	double		K1;				//����������������� ����������� ��������� ������� ��������������� ���������
	double		K2;				//���������� ����������� ��������� ������� ��������������� ���������
	BOOL		changed;		//���� ����� ������������� ���������
	BOOL		changed_ON;		//���� ����� ������������� ��������� enabled
	_int16		lMin;			//����������� �� ������� �������� ������ ������� ��������������� �����
	_int16		lMax;			//����������� �� ������� �������� ������� ������� ��������������� �����
	UINT		reserved[14];
};
union ald_drv_addr_t {
	DWORD	FAddr;	/* Full address */
	struct {
		WORD	Channel;					// ����� ������
		WORD	Board;						// ����� �����������
	} bc;
};
//��������� hardware configuration block:
//�� ��������� CFG_HARDWARE = 1 ��������� hardware configuration block, ������� ������� �� ���������� ����� ��������-���������� �������:
//FullDefMod, ����� ���� �������� ��������� � ������ ��������������� � ������� ������� � ������������ ������������
#define NUM_PAR_PER_MOD	4
struct TAGFullDefMod
{
	UINT		 chan;
	ColorMod	 color_def;					//��������� �����
	ChanDefMod	 chan_def;					//�������� ��������� ������
	TimeDefMod	 time_def;					//��������� ��������� ������
	OSCDefMod	 osc_def[2];				//��������� ������������������ ������� (������ A � B)
	CalibrDefMod calibr_def;				//���������� - � �������� PCI-N �� ������������
	ParDefMod	 par_def[NUM_PAR_PER_MOD];	//��������������� ������ (� ������ PCI-N ������������ ������ ���� ������ ����� ������ �����)
	ald_drv_addr_t addr;					//����� ������
	WORD		 DeviceName;				//��� ����� (��� DDM-2 ��� �����), � ������� ����������� �����
	WORD		 SubDeviceName;				//��� ����� ��� DDM-2, � ������� ����������� �����
	UINT		 num_Rmod;					//����� ���������� ��� ������ (������ ��� ������ � ������������)
	WORD		 gpre;						//����������� �������� ������������� (��� DDM-2 �� ������������)
	_int16		 shift0;					//�������� ���� ��� (��� DDM-2 �� ������������)
	WORD		 DeviceType;				//��� ������� (0,1 - DDM, 2 - PCI4, 3 - PCI8, 4 - PCI8E, 5 - PCI8S)
	WORD		 FilterType;				//��� ������� 1-������������ �������, 2-������� ������������, 4-������ �������, 8-��������� ���������, 16-������� DDM-2
	WORD		 VersionModule;				//������ ������ (������)
	WORD		 Capability;				//capabilities (�����������) ������ (������)
	WORD		 reserved[22];				//reserved
}FullDefMod;


/////////////////////////////////////////////////////
//���������������� ����� ��������� ��������� ���� XXX ������ ��� ���������� ��������� � ������� ���� XXX �� ��������� �����������

//��� �������� ���� ��������� CFG_MAINFRAME �������� ��������� ���� MFData 
//������ ����� CFG_MAINFRAME = sizeof ( MFData )
struct MFData
{
	RECT rc;                // screen coordinates
	WINDOWPLACEMENT wp;     // window placement structure
};

//��� ��������� ���� ���������� �������� ������� �� ���������� ������� ����������� �� ����� CFG_PAGE
//������ ���� �� ����������� 2-� ����������������� ����� ������������� �� ����� ����� ������� ���������� � ����� ������� � ���������
//������ ����� = QueryCfgSize * NumPages
// NumPages - ����� ������� ����������� ����������� ���� (��. �������� CFG_PAGE)

struct  ScaleData
{
	long		lMin;			// scale min
	long		lMax;			// scale max
	BOOL		bMinAuto;		// set min automatically
	BOOL		bMaxAuto;		// set max automatically
	BOOL		bIsShown;		// Is scale visible
	BOOL		bIsStPos;		// Is in standard position
	COLORREF	cBackColor;		// Background color
	COLORREF	cTextColor;		// Text color
	DWORD		dwMajor;		// Major ticks
	DWORD		dwMinor;		// Minor ticks
	BOOL		bMajorAuto;		// Major automatic
	BOOL		bMinorAuto;		// Minor automatic
	char		label[30];		// label text	//29.06
	UINT		unit;			// 1-ADC, 2-mkV, (3-dB.), 4-sek., 5-min., 6-hour, 7-channel number;
	float		lMin_mkV;		// ������ ��� SDO (Y, unit = 2)
	float		lMax_mkV;		// ������ ��� SDO (Y, unit = 2)
	BOOL		bShowThresh;	// ������ ��� SDO (Y, unit = 2)
	int			IsLog;			// 0-�������� ���, 1-���������������
	WORD		bNormal;		// ���� ���������� ��� �����������, FALSE - ���, TRUE - ����
	WORD		bMarker;		// ���� ������� ����, FALSE - ���, TRUE - ����
	BOOL		bScroll;		// ���� ���������� ��� �������, FALSE - ���, TRUE - ����
	UINT		uScrollSize;	// ������ �������������� �������
	UINT		reserved[2];
	BOOL		bShowLabel;		// Show/Hide label
};
struct GraphData
{
	COLORREF	cBackColor;		//���� ����
	BOOL		cIsLines;		//���� ����������� �����/�����
	BOOL		cIsGrid_x;		//����������� ��� X
	BOOL		cIsGrid_y;		//����������� ��� Y
	COLORREF    cColor_x;		//���� �������.
	COLORREF    cColor_y;		//���� �������.
	BOOL		cIsGrid_clast;	//����������� �����
	BYTE		reserved[24];
};


//////////////////////////////////////////////////
//���������������� ���� ��������� ��������� ��������� CFG_ALARMS �������� ����������� ��������� ������ � ���������� ���������� � ����������� �������������
//������ ����� CFG_ALARMS = NUM_ALARMS * sizeof ( AlarmData )
struct AlarmData
{
	char		AlarmName[80];		//��� ���������� ��������� ��� ������������� � ������� ������
	char		WaveName[MAX_PATH];	//���� ��� ��������������� ��������� wav-�����
	FilterDef	FltDef[MAX_FDEFS];	//��������� ��� ����������� ����� ��������� �������
	BOOL		AndOr[MAX_FDEFS];	//���������� ����� ��� ��������� ������� � ��������� �����
	LPVOID		buf;				//��������� �� ����� ��� ������������ �������� ����������� wav-�����
	char		bMarkerSet;			//���� ��������� ������� �� ��������� �����
	char		bActivated;			//���� ��������� ������ �������
	char		bSound;				//���� ������������� ��������� �������������
	char		bPicture;			//���� ������������� 2D-3D �������� ��� ��������� ������������ �������
	char		bPicOff;			//���� �������� �� ��������� ���. �������
	char		bPrevMarkerON;		//���� ��������� ������� �� ��������� ����� �� ������� � ������� ������� 
	//(�������������� ������������� ������ ������� ��� �������������� ������� ����������� ������)
	WORD		NumLocGroup;		//����� ���. ������, ���������� ������ ����������� �������� ��� ���� ���� ����������� ��
	UINT		NumLocDist;			//����� ������������� ������������ ������� � ���.������ NumLocGroup
	UINT		Reserved[16];
};

//////////////////////////////////////////////////
//���������������� ���� CFG_GENERAL �������� ��������� ��� �������� �������������� ����������� ���������� �������
//������ ����� CFG_GENERAL = sizeof ( GeneralSetupMod )
struct GeneralSetupMod
{
	char		name_par[NUM_PAR_PER_MOD][256];	// �������� ���� ����������� ������ � ��������������� �������
	UINT		PageSize;						// ������ �������� �������
	UINT		Din;							// ������� ���������� � ��
	int			SingleNumber;					// ���� ��������/������� ��������� �������
	char		ChanSep[2];						// ����������� ��� ������� ���������
	char		LocSep[2];						// ����������� ��� ��������� ���������� ������� � �������
	BOOL		AutoSaveCfg;					// �������������� ����� ������������
	BOOL		OSCAutoScale;					// ��������������� OSC �� ��������� �������������
	BOOL		SaveService;					// ���������� ��������� ��������������� ������
	BOOL		OnLineSpect;					// ���������� ������� � On-Line
	BOOL		UpgradeSpect;					// ��������� ���������� ������ � �������������
	COLORREF	GenColor;						// ���� ������� ��� ��������� ����� ��������
	BOOL		ParReadCfg;						// ������ ������������ ��������������� ������� �� �������
	BOOL		NoPauseOSC;						// �� ������������� ������ ������������ � ������ �����
	BOOL		RMSOff;							// �� ��������� � ���������� RMS � ���� ������������
	BOOL		ProtOn;							// ���� ������� ���������
	int			LineThickness;					// ������� ����� ������ �� �������� (0 - min)
	BOOL		AnalizInfoMod;					// ���� ������ �������������� ��� ������ �������� �������
	BOOL		WriteServiceInfo;				// ���� ���������� ��������� ��������� ���������� � �����
	WORD		InfoTimeout;					// ������� �������� �������������� �������
	WORD		StartTimeout;					// ������� �������� ���������� ������
	BOOL		TempMonitor;					// ���� ����������� ����������� ���������� �����
	BOOL		UseComAlarm;					// ���� ������� ��������� ��������� �� COM-�����
	UINT		AlarmComPort;					// ����� COM-����� ��� ������� ��������� ���������
	BOOL		SyncStatic;						// ���� ������������ ������ ��� ����������� ���������
	BOOL		ManualCmd;						// ���� ���������� ����� ������ ������ ������
	BOOL		ClbSyncOsc;						// ���� ����������� ��������� ������������ ��� ����������
	BOOL		GetAllVer;						// ���� ������� ������� ��������� ������ ����������
	UINT		reserved[240];
	int			Unit;							//����� ����� ������ ��������� [��, ��, ��, �]
	char		ProtFileName[256];				// ��� �����-���������
	char		name_reserv[NUM_PAR_PER_MOD - 1][256];	//reserved
};

//////////////////////////////////////////////////
//���������������� CFG_AUTOSETUP �������� ��������� ��� �������� ����������� ���������� ��������������� ������
//������ ����� CFG_AUTOSETUP = sizeof ( AutoThresholdSetup )
// structure for update auto threshold setup parameters
struct AutoThresholdSetup
{
	BOOL	m_bAuto;					//on/off auto threshold
	int		m_normal_delta;				//delta of normal threshold change
	int		m_alarm_delta;				//delta of alarm threshold increase
	int		m_length;					//length of storage buffer to apply
	int		m_th_delta;					//���� ���������� ����� ���������� �� ������ ������, ��� ��� ��������, �� ������ �� ������.
	int		m_th_min_ADC;				//min threshold, ADC
	UINT	reserved[16];
};

//////////////////////////////////////////////////
//���������������� ���� CFG_MEASDATA �������� ��������� ��� �������� ����������� ���������� ������ ���������
//������ ����� CFG_MEASDATA = sizeof ( MeasData ) ��. �������� ����

//////////////////////////////////////////////////
//���������������� ���� CFG_SYSTEMINFO �������� ��������� ������ � ����������� �� ���������� ����� ��������� � ������ �� ��� ������������� �����

//////////////////////////////////////////////////
//���������������� ���� CFG_HOTKEYS �������� ��������� ��� �������� ����������� ���������� ������� ������ ��
//������ ����� CFG_HOTKEYS = num_hotkeys * sizeof ( HotKeyData )
struct HotKeyData
{
	WORD	m_wVK;
	WORD	m_wMod;
	DWORD	m_nCmd;
};

// NumPostWins - ����� ����������� ����
// NumPages - ����� ������� ����������� ����������� ���� (��. �������� CFG_PAGE)
class CPostWin
{
	int		axis_x;			//����� ��������� �� �� ��� X
	int		axis_y;			//����� ��������� �� �� ��� Y
	int		axis_z;			//����� ��������� �� �� ��� Z
	DWORD	par_chan;		//����� ���������������� ������
	UINT	reserved[8];	//reserved for future need
};

struct Graph3DData
{
	COLORREF	cBackColor;		//���� ����
	BOOL		cIsGrid_x;		//����������� ��� X
	BOOL		cIsGrid_y;		//����������� ��� Y
	COLORREF    cColor_x;		//���� ������
	COLORREF    cColor_y;		//���� ������
	BOOL		cIsGrid_clast;	//����������� �����
	COLORREF	cObjectColor;	//���� ����	
	BOOL		cIsGrid_z;		//����������� ��� Z
	BYTE		reserved[20];
};



// NumHistWins - ����� ����������� ����
// NumPages - ����� ������� ����������� ����������� ���� (��. �������� CFG_PAGE)
class CHistWin
{
	int		axis_x;			//����� ��������� �� �� ��� X
	int		axis_y;			//����� ��������� �� �� ��� Y
	float	step_x;			//������ �������������� ��������� �� ��� X
	float	step_y;			//������ �������������� ��������� �� ��� Y
	int		type;			//��� �������������: 0 - ����������������, 1 - ������������
	BOOL	bSummed;		//���� ����������� �� �������
	UINT	reserved[6];	//reserved for future need
};

//////////////////////////////////////////////////
//���������������� ���� CFG_IPDEVICE �������� ���������� � ���������� ������� ����������� ������ ����� � ��������� ������ ���� EtnernetBox (����� num_ipdevices �� ����������)
//������ ����� CFG_IPDEVICE = num_ipdevices * sizeof ( NetIPInfo )
struct NetIPInfo	//��������� ���������� ��������� �������� �������������
{
	BYTE		IPAddress[4];	//IP-����� ����������
	DWORD		pID;			//����������������� ����� ����������
	int			bStatus;		//-1 - ������������, 0 - connected, 1 - discinnected
	WORD		MinLineAddr;	//��������� ����� ����� � ������ ����������
	WORD		TimeElapsed;	//����� ���������� �� ����������
	WORD		LineNums[8];	//������ ������������ � ���������� �����
	WORD		LineAddr[8];	//������ ������������ � ���������� ����� ��� ������������
	DWORD		reserved[10];
};

//////////////////////////////////////////////////
//���������������� ���� CFG_AMPCRIT �������� ���������� � ���������� ������������ �������� ������ ��������� ���������� ��
//������ ����� CFG_AMPCRIT = sizeof ( ClassAmpDef )
struct ClassAmpDef
{
	BOOL	show_amp_crit;		// 1 - ������� ���� ���. ����.
	int		is_auto;			// 0 - �����. ����� ���-���, 1 - ������
	int		is_EN;				// 0 - �������� � �������� ��, 1 - �������� � �������� EN
	int		material;			// �� ������������
	int		object;				// �� ������������
	UINT	A1;
	UINT	A2;
	UINT	N1;
	UINT	N2;
	UINT	reserved[10];
};

//////////////////////////////////////////////////
//���������������� ���� CFG_FONT �������� ���������� � ���������� ��������� �������
//������ ����� CFG_FONT = 5 * sizeof ( LOGFONT )
//LOGFONT	mFont[5];
//OSCTextFont.GetLogFont(&mFont[0]);		//����� ����������� ������� � ���� ������������
//MarkerTextFont.GetLogFont(&mFont[1]);	//����� ����������� ������� �� ��������
//LDTextFont.GetLogFont(&mFont[2]);		//����� ����������� ���� � �������� �� ������
//PrinterFont.GetLogFont(&mFont[3]);		//����� ����������� ���� � �������� �� ������
//TabCtrlFont.GetLogFont(&mFont[4]);		//����� ����������� ������� �� ��������� �����������

//////////////////////////////////////////////////
//���������������� ���� CFG_PAGE �������� ���������� � ���������� ������� ���������������� ����������� ����
//������ ����� CFG_PAGE = sizeof ( PageData ) + sizeof ( PageName ) * NumPages;
//NumPages - ����� ������� (�������) �������� ���� ���������
#define MAX_PAGENAMELENGTH	256
struct PageName
{
	char page_name[MAX_PAGENAMELENGTH];
};
struct PageData
{
	UINT mem_size;		// size of page configuration
	UINT num_pages;		// number of pages in file
	UINT cur_page;		// currently selectde page
	PageName name[];	// unsized array of page names;
};

//////////////////////////////////////////////////
//���������������� ���� CFG_PROFILE �������� ���������� � ���������� ������� ���������������� �������� ������� (��. CFG_PAGE)
//������ ����� CFG_PROFILE = sizeof ( PageData ) + sizeof ( PageName ) * NumProfiles;
//NumProfiles - ����� �������� (��������) ����������

//////////////////////////////////////////////////
//���������������� ���� CFG_IMPLINES �������� ���������� � ���������� ������� ����� ���������� ��� ������������ �� ��������� ���� �����������
//������ ����� CFG_IMPLINES = sizeof ( ILData );
struct ILData	//27.12.99S //03.05.06S
{
	BOOL		enable_import;		//���� ��������� ��������� �����
	int			ParInput;			//���� ��������� ����� �� ������ ���������������� ������
	int			ParChanNum;			//����� ���������������� �����
	int			ParModNum;			//����� ��������� ������
	char		ParName[MAX_PATH];	//��� ���������
	char		FileName[MAX_PATH];	//��� ����� ��� ��������� ��������� �� ���������� �����
	int			thickness;			//������� ����� �����������
	COLORREF	color;				//���� �����������
	double		NumFraction;		//�������� (����� ���������� ������)
};


//////////////////////////////////////////////////
//���������������� ���� CFG_SERVICE �������� ���������� � ���������� ������� ����������� ��������� ��������������� ������ ����������
//������ ����� CFG_SERVICE = NUM_DEVICES * NUM_TYPE * NUM_SERV * sizeof( ServDefMod )
#define NUM_TYPE	2
#define NUM_SERV	12
//NUM_DEVICES - ����� ������������ ����� (������ DDM-2)
struct ServDefMod
{
	BOOL		show;				//���� �����������
	COLORREF	color;				//���� �����������
	int			K1;					//����������������� �����������
	int			K2;					//���������� �����������
	int			show_text;			//���� ����������� ���������� ��������
	UINT		reserved[5];
};



////////////////////////////////////////////////////////////////////////
//Struct member alignment ����������� �� ��������� � MSVisualC++, �.�. ������ 8 ����

///////////////THE END//////////THE END////////THE END/////////////////
