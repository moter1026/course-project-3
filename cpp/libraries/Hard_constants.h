// include files for defining various hardare related structs and constants //12.09.00S //03.10.00S

#ifndef HARDWARE_CONSTANTS_DEFINED
#define HARDWARE_CONSTANTS_DEFINED

#include "mytypes.h"
#include <minwindef.h>

// possible buffer size
#define BUF0    1024
#define BUF1    2048
#define BUF2    4096
#define BUF3    8192

// for command register
#define C_SYS_START         0x01    // start system
#define C_SYS_STOP          0x00    // stop system
#define C_SYS_PAUSE         0x02    // pause
#define C_SYS_CONTINUE      0x00    // continue
#define C_SYS_RESET_FIFOE   0x10    // reset fifo events
#define C_SYS_RESET_PROM    0x20    // reset prom counter
#define C_SYS_RESET_TIMER   0x40    // reset timers
#define C_SYS_RESET         0x80    // reset system at all

// module capabilities constants	//19.04.18S //23.08.18S
#define ALM_CAPS_CAPS				0x0001	//есть капабилити собственно
#define	ALM_CAPS_FLT_RELOAD			( ALM_CAPS_CAPS | 0x0800 )	//есть прогружаемые из файла коэфф. фильтры
#define	ALM_CAPS_FLT_CM_INFO		( ALM_CAPS_CAPS | 0x1000 )	//в инфо номер 8 (base area) лежат номера (указатели) и количество: фильтровых инфо и инфо текущего состояния настроек модуля
#define	ALM_CAPS_AUTOCLB			( ALM_CAPS_CAPS | 0x2000 )	//есть режим автокалибровки
#define	ALM_CAPS_ALMC_NOT_NUB_IS	( ALM_CAPS_CAPS | 0x4000 )	//для DDM2 — признак хаба (не модуль)
#define	ALM_CAPS_PRM				( ALM_CAPS_CAPS | 0x0002 )	//есть базовые параметры (температура, например)
#define	ALM_CAPS_CLB				( ALM_CAPS_CAPS | 0x0004 )	//есть калибратор

#define	ALC_CAPS_HASSYNC			0x0001	//есть синхронизация?
#define	ALС_CAPS_HASFAST			0x0002	//есть возможность переключения скалера для усреднения менее 1 с
#define	ALC_CAPS_HASSYNCSTART		0x0008	//есть поддержка синхронного старта по кветированию
#define	ALC_CAPS_HASALMC			0x0010	//признак расширителя ALMC (DDM-2)



// some programming constants
#define PRE_PIXELS_DEFAULT	100

#define MAX_OSC_WND			50		// maximum 8 oscilloscope windows //09.07.07S

// time definition
//#define DEAD_TIME_DEFAULT	32
#define DEAD_TIME_DEFAULT	600		// Mot 08.10.24
//#define SCET_TIME_DEFAULT	500  
#define SCET_TIME_DEFAULT	300		// Mot 08.10.24
//#define RTTO_TIME_DEFAULT	500
#define RTTO_TIME_DEFAULT	500		// Mot 08.10.24
//#define MAX_DUR_DEFAULT     32000
#define MAX_DUR_DEFAULT     64000	// Mot 08.10.24
#define GAIN_DB_DEFAULT_DDM2	36		//20.09.19S
#define GAIN_DB_DEFAULT_DDM		34		//20.09.19S
#define GPRE_DB_DEFAULT		34		//30.04.02S
#define GAIN_DB_DEFAULT_PCI8		26	//30.09.04S
#define GPRE_DB_DEFAULT_PCI8		26	//30.09.04S
#define	TRESH_ADC_DEFAULT	33		//10		//27.11.00L
#define	TRESH_DB_DEFAULT	46.06f  //19.69	//27.11.00L
#define	SHIFT0_DEFAULT		0		//30.04.02S
#define	HIGH_FILTER_DEFAULT	30		//27.09.00L
#define	LOW_FILTER_DEFAULT	500		//27.09.00L
#define K_CALIBR_DEFAULT	40		//27.09.00L
#define AMP_MV_DEFAULT		8000	//mV =50*4*40
#define FREQ_CALIBR_DEFAULT	1		//Hz
#define AMP_V_DEFAULT		10		//V = 63*4*40 = 10080 mV = 10 V	//19.10.00L 
#define AMP_DAC_DEFAULT		63										//27.11.00L
#define FREQ_CALIBR_DEFAULT	1		//Hz

#define NUM_PAR_PER_MOD	4	//07.11.00S
#define NUM_PAR_PER_DEV	2	//26.10.02S
#define K_PAR_DEFAULT		1.f		//параметрические каналы //19.10.00L
//#define D_ADC_PAR_DEFAULT	0.001221
#define FREQ_PAR_MULTI_DEFAULT	0		//десятичный множитель	//10.01.05S
#define FREQ_PAR_DEFAULT	5		//Hz
#define K1_DEFAULT			1.563	//27.03.02S
#define K2_DEFAULT			-3200.	//27.03.02S
#define K1_DEFAULT_T		.02		//27.03.02S
#define K2_DEFAULT_T		0.		//27.03.02S
#define PAR_MIN_DEFAULT		-20000	//30.07.03S
#define PAR_MAX_DEFAULT		20000	//30.07.03S

#define D_IN				1000
#define PAGE_SIZE			1000

#define USEC_PER_SEC		1000000	// microseconds per second
#define TIME_QUANTUM		148		// частота опроса железа (цена младшего слова таймера, мкс)
#define TIME_DELTA			2.f		// временная дельта запаздывания между соседними модулями (мкс)

// for EV_EVENTDATA.chan_info
#define F_ERR_FRAME			0x0
#define F_ERR_COMM			0x1
#define F_ERR_ASK_COMM		0x2
#define F_FIFO_FULL			0x4
#define F_FIFO_EMPTY		0x10
#define F_STARTNEGATIVE		0x100			// знак первого вступления (фазы)(AEF 8): «1» - вступление «отрицательное», «0» вступление «положительное»	//22.10.18S
#define F_MAXDUR_STOP		0x200			// АЭ - измерение завершено по MAX_DUR
#define F_SCETO_STOP		0x400			// АЭ - измерение завершено по SCETO
#define F_OSC1START			0x800			// идет измерение по первому каналу осциллографа
#define F_EN_OVERFLOW		0x1000			// флаг переполнения энергии одиночного АЭ события (AEF 12)													//22.10.18S 
#define F_MAXNEGATIVE		0x2000			// знак максимума импульса (AEF 13): «1» - максимум «отрицательный», «0» максимум «положительный»			//22.10.18S
#define F_ADC_OVERFLOW		0x4000			// adc overflowed within events

#define	NPINGA				1000			// число тестовых посылок для экспресс тестаA	//30.10.18S
#define	NPINGB				10000			// число тестовых посылок для полного тестаB	//30.10.18S

	// for EV_CHANSTATE.chan_state
#define CS_COMM_START		0x01
#define CS_COMM_PAUSE		0x02
#define CS_CHAN_ON			0x04
#define CS_FLAG_OSC			0x08

#define	DATA_SIZE_DEFAULT		24		 // 03.10.00S
#define OSC_SIZE_DEFAULT		16384	 // 03.10.00S

//controller version block		//18.04.01S
#define SINCHRO_CONTROLLER		0x01006202	//версия контроллера, начиная с которой поддерживается
//a) временная синхронизация линий;
//б) номер (имя платы)
#define ERRORNUM_CONTROLLER		0x010107046	//версия контроллера, начиная с которой поддерживается запрос информационных посылок к аппаратной части //16.04.10S
#define NEWNAME_CONTROLLER		0x01010708	//версия контроллера, начиная с которой поддерживается новый механизм запроса имени контроллера			//19.12.12S
#define INFORMABLE_CONTROLLER	0x01020000	//версия контроллера, начиная с которой поддерживается запрос информационных посылок к аппаратной части //16.04.10S
#define ZERO_RADIO_PLAN			0x500		//версия контроллера, начиная с которой поддерживается нулевой радиоплан	//09.12.03S
#define	ALINE_HARD_VER_UNISCOPE	0xA201		//файл с UNISCOPE

#define DEFAULT_MODULE_VERSION	20020	//начальный номер версии модулей	//05.12.01S
#define NEW_TIME_MODULE_VERSION	20030	//первое изменение версии модулей - новый пересчет времени	//05.12.01S
#define MODULE_VERSION_37		20037	//версия модулей хх37				//09.11.17S
#define MODULE_VERSION_411		20411	//версия модулей х411				//09.11.17S
#define MODULE_VERSION_41		20041	//версия модулей хх41				//09.11.17S
#define MODULE_VERSION_451		20451	//версия модулей х411				//09.11.17S
#define MODULE_VERSION_45		20045	//версия модулей хх41				//09.11.17S
#define MODULE_VERSION_198		10198	//версия модулей DDM-2 хх198		//23.09.21S
#define MODULE_VERSION_199		10199	//версия модулей DDM-2 хх199		//23.06.20S
#define MODULE_VERSION_201		10201	//версия модулей DDM-2 хх201		//14.08.20S
#define DEFAULT_TVDELTA			5		//коррекционная временная задержка	//09.11.17S

#define		UNDEFINED						0	//31.05.02S
#define		ALINE_DEVICE_TYPE_DDM			1	//31.05.02S
#define		ALINE_DEVICE_TYPE_PCI4			2	//31.05.02S //26.11.03S
#define		ALINE_DEVICE_TYPE_PCI8			3	//31.05.02S //26.11.03S
#define		ALINE_DEVICE_TYPE_PCI8E			4	//05.05.06S
#define		ALINE_DEVICE_TYPE_DS			5	//17.02.13S
#define		ALINE_DEVICE_TYPE_DDM2			6	//30.03.21S
#define		ALINE_DEVICE_TYPE_PCI2			7	//11.01.24S
#define		ALINE_DEVICE_TYPE_UNISCOPE		10

#define FLT_FILE_SIGN		0x464E4C41	//27.11.03S
#define FLT_SUBDIR			_T("FILTERS")	//27.11.03S

#define FILE_REPORT_NAME	_T("\\report.txt")	//04.02.05S //19.10.06 CH	//19.08.09 CH
// structs

// channel information event
struct TAG_EV_CHANSTATUS
{
	WORD		time_low;		// time mkS counter
	WORD		time_medium;	// time TIME_QUANTUM mkS counter low word
	WORD		time_high;		// time TIME_QUANTUM mkS counter high word
	WORD		type;			// type of the chan status info
	WORD		data[8];		// channel settings
}EV_CHANSTATUS;

// real event
typedef struct TAG_EV_EVENTDATA
{
	WORD		event_start_low;		// event start mkS counter
	WORD		event_start_medium;		// event start TIME_QUANTUM mkS counter low word
	WORD		event_start_high;		// event start TIME_QUANTUM mkS counter high word
	WORD		duration;				// event duration
	WORD		max_ampl;				// max signal value for give event
	WORD		rise_time;				// event rise time
	WORD		tig;					// counter threshold cross
	WORD		energy_low;				// energy low word
	WORD		energy_medium;			// energy low word
	WORD		energy_high;			// energy low word
	WORD		flags;					// additional information (flags and current gain)	//04.05.01S
	WORD		expnd;					// слово дополнительных разрядов и частоты оцифровки (DDM-2) //22.03.17S								
}EV_EVENTDATA;

// noise event
typedef struct TAG_EV_NOISEDATA
{
	WORD		cur_medium;				// second TIME_QUANTUM mkS counter low word
	WORD		cur_high;				// second TIME_QUANTUM mkS counter high word
	WORD		num_summ;				// number of addition always equals 1	//04.05.01S
	WORD		summ_noise_ampl;		// total noise amplitude in ADC bit		//04.05.01S
	_int8		cur_gain_dB;			// current gain dB value				//04.05.01S //13.01.10S
	BYTE		expnd;					// байт дополнительных разрядов и частоты оцифровки (DDM-2)	//22.03.17S
	WORD		intensity;				// real intensity of AE events
	WORD		lost;					// number of lost AE events
	WORD		energy_low;				// energy low word
	WORD		energy_medium;			// energy medium word
	WORD		energy_high;			// energy high word
	WORD		energy_high_high;		// energy high-high word
	WORD		flags;					// additional information
}EV_NOISEDATA;

// noise event
typedef struct TAG_EV_AVGDATA
{
	WORD		cur_medium;				// second TIME_QUANTUM mkS counter low word
	WORD		cur_high;				// second TIME_QUANTUM mkS counter high word
	DWORD		summ_dur;				// duration summary
	DWORD		summ_ampl;				// amplitude in ADC bit summary
	DWORD		summ_rise;				// risetime summary
	DWORD		summ_tig;				// tig summary
	WORD		max_ampl;				// maximum amplitude
	WORD		expnd;					// слово дополнительных разрядов и частоты оцифровки (DDM-2)	//22.03.17S
}EV_AVGDATA;

// parameter event
typedef struct TAG_EV_PARAMETDATA
{
	WORD		event_start_low;			// param start timer low word
	WORD		event_start_medium;			// param start timer medium word
	WORD		event_start_high;			// param start timer high word
	WORD		num_param;					// number of parameter
	WORD		paramet_low;				// paramet value low word
	WORD		paramet_high;				// paramet value high word
	WORD		num_summ;					// количество суммирований
	WORD		par_summ [NUM_PAR_PER_MOD];	// суммарное значение параметров по каналам
	WORD		flags;						// additional information
}EV_PARAMETDATA;

#endif
