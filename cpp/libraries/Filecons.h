/* filecons.h
This file contains all definitions for managing ALINE16D output, input and
intermidiart files if any.
VERSION CHECKING SHOULD BE DONE ALWAYS USING VERSION CONSTANTS
  */

#ifndef FILECONST_DEFINED
#define FILECONST_DEFINED

#include <time.h>
#include "mytypes.h"
#include "hard_constants.h"

// version definition constanst	for output files

// data event output file
#define		EV_DATA_VER_MAJ			1
#define		EV_DATA_VER_MIN			0

// system event output file
#define		SYS_DATA_VER_MAJ		1
#define		SYS_DATA_VER_MIN		0

#define		NUM_TYPE				2					//01.06.11Kir
#define		NUM_SERV				12					//01.06.11Kir
#define		MAX_DATA_SIZE			28					//25.05.16S
#define		MAX_FILTER_FDEFS		4
#define		FLOAT_PRECISION			1000.

//FILTER_CONST
#define		MEASSUMM_DLG_TYPE		0
#define		MEASHDRSHOW_DLG_TYPE	1
#define		FILTERVIEW_DLG_TYPE		2

enum ComboBoxID_T {
	CMB_ID_NUMBER				= 0,
	CMB_ID_TIME					= 1,
	CMB_ID_CHANNEL				= 2,
	CMB_ID_AMP					= 3,
	CMB_ID_AMP_MKV				= 4,
	CMB_ID_AMP_DB				= 5,
	CMB_ID_AMP_RMS				= 6,
	CMB_ID_AMP_RATIO			= 7,
	CMB_ID_AMP_TIG				= 8,
	CMB_ID_ENER_ADC				= 9,
	CMB_ID_ENERG				= 10,
	CMB_ID_EN_DB				= 11,
	CMB_ID_EN_MARSE				= 12,
	CMB_ID_DUR					= 13,
	CMB_ID_RISE					= 14,
	CMB_ID_COUNT				= 15,
	CMB_ID_FREQ1				= 16,
	CMB_ID_FLAG_A				= 17,
	CMB_ID_FLAG_D				= 18,
	CMB_ID_FLAG_O				= 19,
	CMB_ID_X					= 20,
	CMB_ID_Y					= 21,
	CMB_ID_Z					= 22,
	CMB_ID_LA					= 23,
	CMB_ID_TAD					= 24,
	CMB_ID_LSTAR				= 25,
	CMB_ID_CLAST				= 26,
	CMB_ID_EVENT				= 27,
	CMB_ID_EV_TOT				= 28,
	CMB_ID_NOISE_DB				= 29,
	CMB_ID_CONTEV				= 30,
	CMB_ID_POWER				= 31,
	CMB_ID_CNTRATE				= 32,
	CMB_ID_PARAMDUR				= 33,
	CMB_ID_NT_START				= 34,
	CMB_ID_NT_SUCCESS			= 35,
	CMB_ID_NT_FAILED			= 36,
	CMB_ID_NT_BREAK				= 37,
	CMB_ID_CT_START				= 38,
	CMB_ID_CT_SUCCESS			= 39,
	CMB_ID_CT_FAILED			= 40,
	CMB_ID_CT_BREAK				= 41,
	CMB_ID_CLB_START			= 42,
	CMB_ID_CLB_SUCCESS			= 43,
	CMB_ID_CLB_FAILED			= 44,
	CMB_ID_CLB_BREAK			= 45,
	CMB_ID_CLB_PAUSED			= 46,
	CMB_ID_CLB_RESUME			= 47,
	CMB_ID_CT_GAIN_START		= 48,		//24.09.09 CH
	CMB_ID_CT_GAIN_CONT			= 49,
	CMB_ID_CT_GAIN_SUCCESS		= 50,
	CMB_ID_CT_GAIN_FAILED		= 51,
	CMB_ID_CT_GAIN_BREAK		= 52,
	CMB_ID_CLB_CHANNUM			= 53,		//29.03.10 CH
	CMB_ID_PLATOFIXED			= 54,
	CMB_ID_PLATOBREAK			= 55,
	CMB_ID_PLATOYES				= 56,
	CMB_ID_PLATONO				= 57,
	CMB_ID_NETFAILED			= 58,		//22.03.12S
	CMB_ID_SEVENT				= 59,
	CMB_ID_LEVENT				= 60,
	CMB_ID_DFTI1				= 61,
	CMB_ID_DUR_RISE				= 62,
	CMB_ID_NUM_PACK				= 63,
	CMB_ID_DTIME				= 64,
	CMB_ID_AMP_SQV				= 65,
	CMB_ID_FUNC_MAX				= 66,
	CMB_ID_FUNC_MODE			= 67,
	CMB_ID_FUNC_MEAN			= 68,
	CMB_ID_ODDS_EQUAL			= 69,
	CMB_ID_ODDS_MORE			= 70,
	CMB_ID_ODDS_EQUAL_MORE		= 71,
	CMB_ID_ODDS_LESS			= 72,
	CMB_ID_ODDS_EQUAL_LESS		= 73,
	CMB_ID_ACT_MULT				= 74,
	CMB_ID_ACT_DIV				= 75,
	CMB_ID_ACT_LG				= 76,
	CMB_ID_ACT_LN				= 77,
	CMB_ID_ACT_EXP				= 78,
	CMB_ID_SUMM_SUM				= 79,
	CMB_ID_SUMM_DED				= 80,
	CMB_ID_AMP_NOISE			= 81,
	CMB_ID_THIKNESS				= 82,

	//None
	CMB_ID_NONE					= 1000,

	//Parametric CMB_ID > 1000!!!
	CMB_ID_PARAMETRIC			= 1001,
	CMB_ID_PARAMETRIC2			= 1002,
	CMB_ID_PARAMETRIC3			= 1003,
	CMB_ID_PARAMETRIC_END		= 1004

	//
};

enum IsPackID {
	IS_PACK_NONE				= 0,		// нет пачки
	IS_PACK_TRUE				= 1,		// пачка
	IS_PACK_DEAD_TIME			= 2			// мёртвое время
};

enum AeEventID {
	AE_EVENT_ALL				= 0,		// все события
	AE_EVENT_FIRST				= 1,		// первое по кажд.каналу
	AE_EVENT_MAX				= 2,		// макс. по кажд.каналу
	AE_EVENT_START				= 3			// только стартовый импульс АЭ
};

enum ExportDataID {
	EXPORT_DATA_AE				= 0,		// инфа об АЭ импульсах
	EXPORT_DATA_PARAM			= 1,		// показания парам.датчиков
	EXPORT_DATA_NOISE			= 2,		// инфа о шумах и средних
	EXPORT_DATA_SYSTEM			= 3,		// инфа о системных командах
	EXPORT_DATA_SERVICE			= 4,		// инфа служебная
	PARAM_DATA_FILTER			= 5,		// перечень параметров в окне фильтрации
	PARAM_DATA_SYNCHRO			= 6			// перечень параметров в окне синхропросмотра
};

enum ParamDataID {							//новые параметры добавлять в конец списка!
	ID_NUMBER					= 0,		//
	ID_DATE						= 1,		//
	ID_TIME_ABS					= 2,		//
	ID_TIME						= 3,		//
	ID_CHANNEL					= 4,		//Внимательно! Это номер канала!
	ID_AMP						= 5,		//
	ID_AMP_MKV					= 6,		//
	ID_AMP_DB					= 7,		//
	ID_AMP_RMS					= 8,		//
	ID_AMP_RATIO				= 9,		//
	ID_AMP_TIG					= 10,		//
	ID_ENER_ADC					= 11,		//
	ID_ENERG					= 12,		//
	ID_EN_DB					= 13,		//
	ID_EN_MARSE					= 14,		//
	ID_DUR						= 15,		//
	ID_RISE						= 16,		//
	ID_COUNT					= 17,		//Внимательно! Это выбросы!
	ID_FREQ1					= 18,		//

	ID_FLAGS					= 19,		//Внимательно! Это флаги!
	ID_NUM_PACK1				= 20,		//
	ID_Y1						= 21,		//
	ID_Z1						= 22,		//
	ID_LA1						= 23,		//
	ID_TAD						= 24,		//
	ID_AMP_NOISE				= 25,		//
	ID_THRESH_DB				= 26,		//
	ID_SIGNAL_TYPE				= 27		//
};

typedef struct TAG_CRC_CNT		//28.04.04S
{
	UINT	old_time;
	UINT	overflow;
	BOOL	bNextSecond;
}CRC_CNT;

typedef struct TAG_CRC_STRUCT	//28.04.04S
{
	CRC_CNT	crc[4];
}CRC_STRUCT;

// data event output structure
typedef struct TAG_AETIME
{
	__time32_t	seconds;	// absolute seconds value suitable for C time functions
	ulong	usec;		// microseconds since last second
}AE_TIME;

typedef struct TAG_EV_DATA_OUT_OLD	//12.11.00S
{
	AE_TIME		ev_time;	 	// start time of the event
    WORD		chan_num;		// channel number
	WORD		max_ampl;		// maximum amplitude
	UINT		duration;		// event duration in microseconds since start time
	UINT		rise_time;	    // event rise time in microseconds since start time	
    WORD        tig;			// tig or count
	WORD		attenuation;    // amplitude attenuation (multiplied by 10) to calculate location amplitude	//24.01.06S
	WORD		energy_low;		// energy low word
	WORD		energy_medium;	// energy medium word
	WORD		energy_high;	// energy high word
	WORD		flags;			// validity flags
    UINT		loc_event;		// flag of localized event: 0-not, 1-sucsess, 5-must be deleted //30.06.00L //04.09.03S + везде
    LONG        locX;			// location X-coordinate [mm] (NO_LOC) - no location by this event, (SEVERAL_LOC) - used for several locations, //17.06
    LONG        locY;			// location Y-coordinate [mm]
}EV_DATA_OUT_OLD;	//24.04.01S

//флаги локализации типа события (loc_event)	//21.11.03S //18.06.19S
#define LOC_IDLE	0		//готово для локации/нет локации
#define LOC_DONE	1		//успешная локация
#define EV_PARAM	2		//параметрическое событие
#define EV_SYS		3		//системное событие
#define EV_NOISE	4		//шумовая посылка
#define EV_DEL		5		//не применять для локации (помеха)
#define EV_MARKER	6		//маркерная посылка	//22.04.04S
#define UNI_PARAM	7		//параметрическая посылка от UNISCOPE //12.05.17S
#define UNI_AVG		8		//усреднительная посылка от UNISCOPE //18.06.19S
#define EV_PASS		16		//пропустить для последующей локации
#define EV_INCLUDE	0xEF	//включить для последующей локации

typedef struct TAG_EV_DATA_OUT
{
	AE_TIME		ev_time;	 	// start time of the event
    WORD		chan_num;		// channel number
	WORD		max_ampl;		// maximum amplitude
	WORD		duration;		// event duration in microseconds since start time
	WORD		expnd;			// слово дополнительных разрядов и частоты оцифровки (DDM-2)	//22.03.17S
	WORD		rise_time;	    // event rise time in microseconds since start time	
	WORD		flags_add;		// расширение поля флагов										//22.10.18S
    WORD        tig;			// tig or count
	WORD		attenuation;    // amplitude attenuation (multiplied by 10) to calculate location amplitude	//24.01.06S
	WORD		energy_low;		// energy low word
	WORD		energy_medium;	// energy medium word
	WORD		energy_high;	// energy high word
	WORD		flags;			// validity flags
	WORD		tad;			// time arrival difference, mcs									//15.07.13S
    BYTE		loc_event;		// flag of localized event: 0-not, 1-sucsess, 5-must be deleted //30.06.00L //04.09.03S + везде //15.07.13S
	BYTE		nsec;			// tens of nanoseconds since last microseconds					//22.03.17S
    LONG        locX;			// location X-coordinate [mm] (NO_LOC) - no location by this event, (SEVERAL_LOC) - used for several locations, //17.06
    LONG        locY;			// location Y-coordinate [mm]
    LONG        locZ;			// location Z-coordinate [mm]
	LONG		gain;			// current gain value [dB]
}EV_DATA_OUT;	//24.04.01S

// noise and averaged info event output structure
typedef struct TAG_EV_AVG_OUT	//07.12.00S
{
	AE_TIME		avg_time;				// arrival time of average message
	WORD		chan_num;				// channel number
	WORD		num_summ;				// number of noise amplitude addition
	DWORD		noise_summ_ampl;		// total noise amplitude in ADC bit
	WORD		intensity;				// real intensity of AE events
	WORD		lost;					// number of lost AE events
	double		summ_energy;			// energy of AE events
	DWORD		summ_dur;				// duration summary of AE events
	DWORD		summ_ampl;				// amplitude in ADC bit summary of AE events
	DWORD		summ_rise;				// risetime summary of AE events
	DWORD		summ_tig;				// tig summary of AE events
	WORD		max_ampl;				// maximum amplitude of AE events
	WORD		expndw;					// слово дополнительных разрядов и частоты оцифровки (DDM-2)//22.03.17S
	_int8		cur_gain_dB;			// current gain vlaue			//04.05.01S	//21.05.02S
	BYTE		expndb;					// байт дополнительных разрядов и частоты оцифровки (DDM-2)	//22.03.17S
	WORD		reserved[7];
}EV_AVG_OUT;

typedef struct TAG_CH_INFO_OUT
{
	AE_TIME		chan_info_time;	// time of the channel settings
	WORD		chan_num;		// number of channel to set
	WORD		type;			// type of settings
	WORD		data[8];		// info data set
}CH_INFO_OUT;

// CH_INFO_OUT possible types
#define	CHAN_INFO		3			// start channel
#define	CHAN_STOP		4			// stop channel
#define OSC1_INFO		5			// start of 1 OSC
#define OSC2_INFO		6			// start of 2 OSC
#define PF_INFO			9			// filters
#define PAR_INFO		10			// start parameters
#define PAR_STOP		11			// stop parameters
#define GAIN_INFO		14			// channel gain
#define CHANGAIN_INFO	18			// channel start&gain	//13.04.01S
#define OSC1_STOP		24			// stop of 1 osc
#define OSC2_STOP		25			// stop of 2 osc
#define CTRL_PAR_INFO	5			// включение параметрических каналов PCI-8/PCI-N	//10.01.05S
#define CTRL_PAR_STOP	6			// выключение параметрических каналов PCI-8/PCI-N	//10.01.05S

#define SYNK_CHECK_CONST	429		// проверочная константа готовности старта			//05.06.18S
#define SYNK_CHECK_NCONST	65106	// проверочная константа готовности старта			//05.06.18S

typedef struct TAG_EV_PARAM_OUT	//07.11.00S
{
	AE_TIME		param_time;						// parameter absolute time
	WORD		chan_num;						// parameter owner module number
	WORD		num_summ;						// число суммирований параметров
	WORD		par_summ[ NUM_PAR_PER_MOD ];	// суммарное значение параметров по каналам
}EV_PARAM_OUT;

typedef struct TAG_UNI_PARAM_OUT	//12.05.17S
{
	AE_TIME			time;
	WORD			chan_num;
	float			value;
	unsigned char	prm_chan;
	unsigned char	prm_type;
	unsigned char	flags;
	unsigned char	res;
}UNI_PARAM_OUT;

typedef struct TAG_UNI_AVG_OUT	//18.06.19S
{
    AE_TIME		time;
    WORD		chan_num;
    DWORD		sec;
    DWORD		impulse_total;
    DWORD		impulse_sceto;
    DWORD		impulse_drop;
    DWORD		impulse_replace;
    DWORD		ampl_h;
    DWORD		ampl_l;
    DWORD		dur_h;
    DWORD		dur_l;
    DWORD		rise_h;
    DWORD		rise_l;
    DWORD		counts_h;
    DWORD		counts_l;
    DWORD		energy_h;
    DWORD		energy_l;
    DWORD		thresh;
    DWORD		sceto;
    DWORD		maxdur;
    DWORD		noise_energy_h;
    DWORD		noise_energy_l;
    DWORD		noise_ampl;
    DWORD		noise_amplsum_h;
    DWORD		noise_amplsum_l;
    DWORD		noise_num;
    DWORD		flags;
}UNI_AVG_OUT;

typedef struct TAG_MARKER_OUT	//22.04.04S
{
	AE_TIME		marker_time;					// marker absolute time
	DWORD		markerID;						// идентификатор маркера
}MARKER_OUT;

#define MAX_TEXT_LENGTH	68
typedef struct TAG_TEXT_OUT		//22.04.04S
{
	DWORD		markerID;						// идентификатор маркера
	char		marker_text[MAX_TEXT_LENGTH];	// текст маркера
}TEXT_OUT;

#define NUM_RPARAM	5			//27.05.15S
// structure for handling average data	
typedef struct TAGAvgData	//07.12.00S
{
	UINT	NumNoiseCome;
	UINT	NumAvgCome;
	UINT	NoiseTime;		//15.12.00S
	UINT	AvgTime;		//15.12.00S
    double	energy;			//06.11.00S
	ULONG	max_max_amp;	//24.05.00L //22.03.17S
	ULONG	max_ampl;
	ULONG	intensity;
	ULONG	duration;
	ULONG	rise_time;
	ULONG	tig;
	ULONG	noise_ampl;
	ULONG	noise_summ;
	ULONG	asl;			//19.04.18S
	ULONG	rms;			//19.04.18S
	ULONG	num_osc_wave;	// number of OSC waves during this seconds	//24.11.00S
	ULONG	lost;			// number of the lost AE events				//24.11.00S
	POINT	max_amp;		//06.06.03S
	POINT	max_count;		//06.06.03S
	WORD	expndb;			//22.03.17S
	WORD	expndw;			//22.03.17S
	WORD	NumFailed;		//23.12.21S
	WORD	rData[NUM_RPARAM];		// radio parameters //27.05.15S
} AvgData;

// structure for handling averaged parameter data
typedef struct TAGAvgParamet
{
	UINT	counter [NUM_PAR_PER_MOD];	// counter of recieved parameters portions
    long	param [NUM_PAR_PER_MOD];	// parameter values
} AvgParam;

typedef struct TAGSDOHdr	//16.10.00S
{
	UINT NumOSC;			// total osc number
	UINT InfoPointer;		// pointer to the info osc block in the file (if this block exists)
	UINT DefSize;			// maximum indefinite size of sdo buffer (from hardware)
	UINT BaseSize;			// sizeof base OSC definition structure
	UINT Din;				// Scale base value in nV
    UINT reserved[16];      // reserved bytes for future use
}SDOHdr;

typedef struct TAG_SERVICE_OUT	//30.01.11S
{
	AE_TIME		s_time;		// service info absolute time
	WORD		board;		// number of device to inform
	BYTE		type;		// type of info					//01.06.11Kir
	BYTE		line;		// name of line (in IP setup)	//01.06.11Kir
	WORD		data[12];	// info data set
}SERVICE_OUT;

// radioplan status struct	//27.05.15S
typedef struct TAG_RADIO_OUT
{
	AE_TIME		r_time;
	WORD		chan_num;
	WORD		data[NUM_RPARAM];
}RADIO_OUT;

// SERVICE_OUT possible types	//30.01.11S
#define	AEC_ERROR_TYPE	9		// Action_SysMonitor
#define	TIMER_TYPE		6		// Action_Timer

#define	u_param_data	34
#define	u_avg_data		33	//25.05.16S

enum	DataTypes { ev_data_old = 0,
					sys_data = 1,
					noise_data_old = 2,
					param_data = 3,
					noise_data = 4,
					ev_data = 5,
					marker = 6,
					text = 7,
					reserv = 8,
					service_data = 9,	//07.12.00S //24.04.01S //22.04.04S //30.01.11S
					unis_param_data = 10,
					radio_data = 11,
					unis_avg_data = 12 };	//27.05.15S //25.05.16S

#define NUM_FILE_STRUCT	13		//24.04.01S //22.04.04S //30.01.11S //25.05.16S

const   UINT	DataSizes [NUM_FILE_STRUCT] = {
				sizeof(EV_DATA_OUT_OLD),
				sizeof(CH_INFO_OUT),
				sizeof(EV_DATA_OUT),
				sizeof(EV_PARAM_OUT),
				sizeof(EV_AVG_OUT),
				sizeof(EV_DATA_OUT),
				sizeof(MARKER_OUT),
				sizeof(TEXT_OUT),
				1,						//15.12.11Kir
				sizeof(SERVICE_OUT), 	//24.04.01S //22.04.04S //30.01.11S
				sizeof(UNI_PARAM_OUT),	//12.05.17S
				sizeof(RADIO_OUT),
				sizeof(UNI_AVG_OUT)};	//27.05.15S //25.05.16S //18.06.19S

#define LOC_STEP			1024	//256	//07.12.99S

#define	GETDATA_BUF_SIZE	65536	//14.12.06S //25.11.11S	
#define WRITE_BUF_SIZE		5120	//4096		//20.05
#define READ_BUF_SIZE		65536	//07.12.99S	//01.10.14S
#define MESSAGE_BUF			61		//18.12.00LS + к-во интервалов усредн., накопленных перед SendMessage
#define BOUND_LEVEL			4096	//2048

#define NO_LOC		-1000000001	//17.06
#define SEVERAL_LOC	-1000000002	//17.06

#define XCOOR		0x01		//17.03.99S в файле представлены Х координаты
#define XYCOOR		0x02		//17.03.99S в файле представлены Х и Y координаты
#define XYZCOOR		0x03		//24.04.01S в файле представлены X, Y и Z координаты
#define LAMP		0x04		//17.03.99S в файле представлена Локационная амплитуда

#endif
