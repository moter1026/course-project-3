#pragma once
#include <logger.h>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "afxpriv.h"
#include <time.h>
#include <cwchar>    // Для функции mbstowcs
#include <cstring>   // Для strlen
#include <string>
#include <codecvt>
//#include <Filecons.h>


//список номеров конфигурационных блоков
#define		CFG_HARDWARE		1       // hardware configuration block - самый важный блок настройки аппаратной части
#define		CFG_GAINWND			2       // GAINWND - конфигурационный блок настроек положения окна "усиление/время" (для DDM-2 особого смысла не имеет, так как усиление неизменно для этого типа систем)
#define		CFG_MAINFRAME		3       // MAINFRAME - конфигурационный блок настроек положения главного окна программы
#define		CFG_AMPWND			4       // AMPWND - конфигурационный блок настроек положения окна "средняя амплитуда/время"
#define		CFG_AMPTOTALWND		5       // AMPTOTALWND - конфигурационный блок настроек положения окна "средняя суммарная амплитуда/время"
#define		CFG_EVWND			6       // EVWND - конфигурационный блок настроек положения окна "активность АЭ /время"
#define		CFG_EVSUMWND		7       // EVSUMWND - конфигурационный блок настроек положения окна "суммарный счет импульсов АЭ /канал"
#define		CFG_EVRUNWND		8       // EVRUNWND - конфигурационный блок настроек положения окна "суммарный счет импульсов АЭ /время"
#define		CFG_EVTOTALWND		9		// EVTOTALWND - конфигурационный блок настроек положения окна "суммарная активность / время"
#define		CFG_RISEWND			10      // RISETIMEWND - конфигурационный блок настроек положения окна "среднее время нарастания / время"
#define		CFG_DURWND			11      // Duration Wnd - конфигурационный блок настроек положения окна "средняя длительность / время"
#define		CFG_ALARMS			12      // alarms settings
#define		CFG_PRINT			13      // printer settings - не используется
#define		CFG_ENERGYWND		14      // energy window - конфигурационный блок настроек положения окна "средняя энергия /время"
#define		CFG_DUMMY			15      // DUMMY - не используется
#define		CFG_TIMER			16      // timers - не используется
#define		CFG_NOTOTALWND		17      // NOISETOTALWND - конфигурационный блок настроек положения окна "средний уровень шума / канал"
#define		CFG_TIGWND			18      // Tig window - конфигурационный блок настроек положения окна "среднее число выбросов в АЭ импульсе/время"
#define		CFG_EVERYAMPWND		19		// EveryAmp real-time window - не используется
#define		CFG_MAXAMPWND		20		// MaxAmp real-time window - конфигурационный блок настроек положения окна "максимальная амплитуда/время"	
#define		CFG_GENERAL			21		// General setup configuration
#define		CFG_AUTOSETUP		22		// Autothreshold setup configuration
#define		CFG_POSTWNDSOLD		23		// PostWnds	(старая версия)	- не используется
#define		CFG_HISTWNDSOLD		24		// HistWnds	(старая версия)	- не используется
#define		CFG_MEASDATA		25		// MeasData	- настройки параметров нового измерения (см. структура MeasData выше)
#define		CFG_ENRSUMWND		26		// Energy summary window - конфигурационный блок настроек положения окна "сумма энергии / время"
#define		CFG_SYSTEMINFO		27		// Информация об аппаратной части комплекса и версии ПО для записываемого файла
#define		CFG_TIGSUMWND		28      // Tig summary window - конфигурационный блок настроек положения окна "суммарный счет выбросов АЭ / время"
#define		CFG_NOWND			29      // Noise window - конфигурационный блок настроек положения окна "средний уровень шума / время"
#define		CFG_CNTSUMWND		30      // Count Summ window - конфигурационный блок настроек положения окна "суммарный счет выбросов АЭ / канал"
#define		CFG_HOTKEYS			31      // Hot Key data configuration block
#define		CFG_MAXAMPCOUNTWND	32		// Amplitude/Count window - конфигурационный блок настроек положения окна "максимальная амплитуда / максимальное число выбросов"
#define		CFG_LOCPICT2D		33		// конфигурационный блок настроек положения окна локации типа "Рисунок" 2D
#define		CFG_LOCPICT3D		34		// конфигурационный блок настроек положения окна локации типа "Рисунок" 3D
#define		CFG_POSTWNDS		35		// PostWnds	(новая версия) - конфигурационный блок настроек корреляционных окон
#define		CFG_HISTWNDS		36		// HistWnds	(новая версия) - конфигурационный блок настроек гистограммных окон
#define		CFG_IPDEVICE		37		// IP-Addresses configuration block	
#define		CFG_EXPORT			38		// Export configuration block
#define		CFG_SERVWND			39      // ServWnd - конфигурационный блок настроек положения окна "служебные параметры / время"
#define		CFG_AMPCRIT			40		// Amplitude criterion block
#define		CFG_COUNTWND		41      // CountWnd - конфигурационный блок настроек положения окна "скорость счета / время"
#define		CFG_LEVRCVRWND		42		// Level of transmitter signal window - конфигурационный блок настроек положения окна "уровень сигнала передатчика ППС / время" (только для систем с радиоканалом)
#define		CFG_LEVPPSWND		43		// Level of receiver signal window - конфигурационный блок настроек положения окна "уровень сигнала приемника ППС / время" (только для систем с радиоканалом)
#define		CFG_NUMINTWND		44		// Intensity of data reception window - конфигурационный блок настроек положения окна "интенсивность приема / время" (только для систем с радиоканалом)
#define		CFG_ERRINTWND		45		// Error rates window - конфигурационный блок настроек положения окна "интенсивность ошибок приема / время" (только для систем с радиоканалом)
#define		CFG_POWERWND		46		// Battery life	- конфигурационный блок настроек положения окна "заряд АКБ / время" (только для систем с радиоканалом)
#define		CFG_FONT			47		// Настройки текстовых фонтов
#define		CFG_EVTOTALSUMWND	48		// Sum of all impulses Wnd - конфигурационный блок настроек положения окна "суммарная число импульсов АЭ / время"	
#define		CFG_RMSWND			49		// RMS window - конфигурационный блок настроек положения окна "RMS / время"
#define		CFG_ASLWND			50		// ASL window - конфигурационный блок настроек положения окна "ASL / время"

#define		CFG_SDOWND			100     // SDOWND configuration block - конфигурационный блок настроек положения окна "Осциллограмма / время"
#define		CFG_SPECTRWND		150		// spectrum window configuration block	- конфигурационный блок настроек положения окна "Частотный спектр / время"
#define		CFG_PARAMETWND		200		// Paramet windows configuration block	- конфигурационный блок настроек положения окна "<Параметрический канал №> / время"
#define		CFG_PAGE			250		// Page configuration block
//CFG_ID окон на разных страницах формируется как
//CFG_XXX в младшем байте младшего слова +
// 'номер страницы'в младшем байте старшего слова	//24.07.00S
#define		CFG_PROFILE			251		// Profile configuration block	
#define		CFG_IMPLINES		252		// Import Line configuration block	
#define		CFG_SERVICE			253		// Controller settings block	
#define		CFG_SYNCVIEW		254		// SyncView window settings block - конфигурационный блок настроек положения окна "Синхронный текстовый просмотр импульсов АЭ"

#define NUM_PAR_PER_MOD	4

#define MAX_PAGENAMELENGTH	256

//	где PreFilterData - информация об использованном в ходе сбора данных префильтре
#define MAX_FDEFS 4

//конфигурационный блок CFG_SERVICE содержит информацию о настройках системы отображения служебных диагностических данных аппаратуры
//размер блока CFG_SERVICE = NUM_DEVICES * NUM_TYPE * NUM_SERV * sizeof( ServDefMod )
#define NUM_TYPE	2
#define NUM_SERV	12
//NUM_DEVICES - число контроллеров линии (входов DDM-2)

#define NO_LOC		-1000000001
#define SEVERAL_LOC	-1000000002

#define		CFG_VER_MIN			0
#define		CFG_MAGIC			0x1515CFCF
#define		CFG_VER_MAJ			25	//16.02.06S //16.06.17S //18.12.18S

#define		CFG_VER_MAJ_2015	22	//06.02.15Kir

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
	CMB_ID_NUMBER = 0,
	CMB_ID_TIME = 1,
	CMB_ID_CHANNEL = 2,
	CMB_ID_AMP = 3,
	CMB_ID_AMP_MKV = 4,
	CMB_ID_AMP_DB = 5,
	CMB_ID_AMP_RMS = 6,
	CMB_ID_AMP_RATIO = 7,
	CMB_ID_AMP_TIG = 8,
	CMB_ID_ENER_ADC = 9,
	CMB_ID_ENERG = 10,
	CMB_ID_EN_DB = 11,
	CMB_ID_EN_MARSE = 12,
	CMB_ID_DUR = 13,
	CMB_ID_RISE = 14,
	CMB_ID_COUNT = 15,
	CMB_ID_FREQ1 = 16,
	CMB_ID_FLAG_A = 17,
	CMB_ID_FLAG_D = 18,
	CMB_ID_FLAG_O = 19,
	CMB_ID_X = 20,
	CMB_ID_Y = 21,
	CMB_ID_Z = 22,
	CMB_ID_LA = 23,
	CMB_ID_TAD = 24,
	CMB_ID_LSTAR = 25,
	CMB_ID_CLAST = 26,
	CMB_ID_EVENT = 27,
	CMB_ID_EV_TOT = 28,
	CMB_ID_NOISE_DB = 29,
	CMB_ID_CONTEV = 30,
	CMB_ID_POWER = 31,
	CMB_ID_CNTRATE = 32,
	CMB_ID_PARAMDUR = 33,
	CMB_ID_NT_START = 34,
	CMB_ID_NT_SUCCESS = 35,
	CMB_ID_NT_FAILED = 36,
	CMB_ID_NT_BREAK = 37,
	CMB_ID_CT_START = 38,
	CMB_ID_CT_SUCCESS = 39,
	CMB_ID_CT_FAILED = 40,
	CMB_ID_CT_BREAK = 41,
	CMB_ID_CLB_START = 42,
	CMB_ID_CLB_SUCCESS = 43,
	CMB_ID_CLB_FAILED = 44,
	CMB_ID_CLB_BREAK = 45,
	CMB_ID_CLB_PAUSED = 46,
	CMB_ID_CLB_RESUME = 47,
	CMB_ID_CT_GAIN_START = 48,		//24.09.09 CH
	CMB_ID_CT_GAIN_CONT = 49,
	CMB_ID_CT_GAIN_SUCCESS = 50,
	CMB_ID_CT_GAIN_FAILED = 51,
	CMB_ID_CT_GAIN_BREAK = 52,
	CMB_ID_CLB_CHANNUM = 53,		//29.03.10 CH
	CMB_ID_PLATOFIXED = 54,
	CMB_ID_PLATOBREAK = 55,
	CMB_ID_PLATOYES = 56,
	CMB_ID_PLATONO = 57,
	CMB_ID_NETFAILED = 58,		//22.03.12S
	CMB_ID_SEVENT = 59,
	CMB_ID_LEVENT = 60,
	CMB_ID_DFTI1 = 61,
	CMB_ID_DUR_RISE = 62,
	CMB_ID_NUM_PACK = 63,
	CMB_ID_DTIME = 64,
	CMB_ID_AMP_SQV = 65,
	CMB_ID_FUNC_MAX = 66,
	CMB_ID_FUNC_MODE = 67,
	CMB_ID_FUNC_MEAN = 68,
	CMB_ID_ODDS_EQUAL = 69,
	CMB_ID_ODDS_MORE = 70,
	CMB_ID_ODDS_EQUAL_MORE = 71,
	CMB_ID_ODDS_LESS = 72,
	CMB_ID_ODDS_EQUAL_LESS = 73,
	CMB_ID_ACT_MULT = 74,
	CMB_ID_ACT_DIV = 75,
	CMB_ID_ACT_LG = 76,
	CMB_ID_ACT_LN = 77,
	CMB_ID_ACT_EXP = 78,
	CMB_ID_SUMM_SUM = 79,
	CMB_ID_SUMM_DED = 80,
	CMB_ID_AMP_NOISE = 81,
	CMB_ID_THIKNESS = 82,

	//None
	CMB_ID_NONE = 1000,

	//Parametric CMB_ID > 1000!!!
	CMB_ID_PARAMETRIC = 1001,
	CMB_ID_PARAMETRIC2 = 1002,
	CMB_ID_PARAMETRIC3 = 1003,
	CMB_ID_PARAMETRIC_END = 1004

	//
};

enum IsPackID {
	IS_PACK_NONE = 0,		// нет пачки
	IS_PACK_TRUE = 1,		// пачка
	IS_PACK_DEAD_TIME = 2			// мёртвое время
};

enum AeEventID {
	AE_EVENT_ALL = 0,		// все события
	AE_EVENT_FIRST = 1,		// первое по кажд.каналу
	AE_EVENT_MAX = 2,		// макс. по кажд.каналу
	AE_EVENT_START = 3			// только стартовый импульс АЭ
};

enum ExportDataID {
	EXPORT_DATA_AE = 0,		// инфа об АЭ импульсах
	EXPORT_DATA_PARAM = 1,		// показания парам.датчиков
	EXPORT_DATA_NOISE = 2,		// инфа о шумах и средних
	EXPORT_DATA_SYSTEM = 3,		// инфа о системных командах
	EXPORT_DATA_SERVICE = 4,		// инфа служебная
	PARAM_DATA_FILTER = 5,		// перечень параметров в окне фильтрации
	PARAM_DATA_SYNCHRO = 6			// перечень параметров в окне синхропросмотра
};

enum ParamDataID {							//новые параметры добавлять в конец списка!
	ID_NUMBER = 0,		//
	ID_DATE = 1,		//
	ID_TIME_ABS = 2,		//
	ID_TIME = 3,		//
	ID_CHANNEL = 4,		//Внимательно! Это номер канала!
	ID_AMP = 5,		//
	ID_AMP_MKV = 6,		//
	ID_AMP_DB = 7,		//
	ID_AMP_RMS = 8,		//
	ID_AMP_RATIO = 9,		//
	ID_AMP_TIG = 10,		//
	ID_ENER_ADC = 11,		//
	ID_ENERG = 12,		//
	ID_EN_DB = 13,		//
	ID_EN_MARSE = 14,		//
	ID_DUR = 15,		//
	ID_RISE = 16,		//
	ID_COUNT = 17,		//Внимательно! Это выбросы!
	ID_FREQ1 = 18,		//

	ID_FLAGS = 19,		//Внимательно! Это флаги!
	ID_NUM_PACK1 = 20,		//
	ID_Y1 = 21,		//
	ID_Z1 = 22,		//
	ID_LA1 = 23,		//
	ID_TAD = 24,		//
	ID_AMP_NOISE = 25,		//
	ID_THRESH_DB = 26,		//
	ID_SIGNAL_TYPE = 27		//
};

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


#define MAX_TEXT_LENGTH	68
#define NUM_RPARAM	5			//27.05.15S


// SERVICE_OUT possible types	//30.01.11S
#define	AEC_ERROR_TYPE	9		// Action_SysMonitor
#define	TIMER_TYPE		6		// Action_Timer


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


namespace py = pybind11;

std::string wstring_to_ansi(const std::wstring& wstr);
std::string wstring_to_string(const std::wstring& wstr);
std::wstring string_to_wstring(const std::string& str);
template<typename ... Args>
std::string string_format(const std::string& format, Args ... args);
std::wstring get_wstr(char* l_str);
template <typename T>
std::vector<T> get_vector_from_arr(T* arr, int len);
double ExtractTimeQuant(WORD expnd, int shift);
// Функция для биндинга FileHdr
void bind_FileHdr(py::module_& m);
// Функция для биндинга FilterDef
void bind_FilterDef(py::module_& m);
// Функция для биндинга FilterDef
void bind_PreFilterData(py::module_& m);
// Функция для биндинга MeasData
void bind_MeasData(py::module_& m);
// Функция для биндинга SDOHdr
void bind_SDOHdr(py::module_& m);
// Функция для биндинга AE_TIME
void bind_AE_TIME(py::module_& m);
// Функция для биндинга OSCDefMod
void bind_OSCDefMod(py::module_& m);
// Функция для биндинга CfgFileHdr
void bind_CfgFileHdr(py::module_& m);
// Функция для биндинга CfgBlockDescr
void bind_CfgBlockDescr(py::module_& m);
// Функция для биндинга CfgFileInfo
void bind_CfgFileInfo(py::module_& m);
// Функция для биндинга ColorMod
void bind_ColorMod(py::module_& m);
// Функция для биндинга ChanDefMod
void bind_ChanDefMod(py::module_& m);
// Функция для биндинга TimeDefMod
void bind_TimeDefMod(py::module_& m);
// Функция для биндинга CalibrDefMod
void bind_CalibrDefMod(py::module_& m);
// Функция для биндинга ParDefMod
void bind_ParDefMod(py::module_& m);
// Функция для биндинга ald_drv_addr_wrapper
void bind_ald_drv_addr_t(py::module_& m);
// Функция для биндинга FullDefMod
void bind_FullDefMod(py::module_& m);
// Функция для биндинга MFData
void bind_MFData(py::module_& m);
// Функция для биндинга ScaleData
void bind_ScaleData(py::module_& m);
// Функция для биндинга GraphData
void bind_GraphData(py::module_& m);
// Функция для биндинга AlarmData
void bind_AlarmData(py::module_& m);
// Функция для биндинга GeneralSetupMod
void bind_GeneralSetupMod(py::module_& m);
// Функция для биндинга AutoThresholdSetup
void bind_AutoThresholdSetup(py::module_& m);
// Функция для биндинга HotKeyData
void bind_HotKeyData(py::module_& m);
// Функция для биндинга CPostWin
void bind_CPostWin(py::module_& m);
// Функция для биндинга Graph3DData
void bind_Graph3DData(py::module_& m);
// Функция для биндинга CHistWin
void bind_CHistWin(py::module_& m);
// Функция для биндинга NetIPInfo
void bind_NetIPInfo(py::module_& m);
// Функция для биндинга ClassAmpDef
void bind_ClassAmpDef(py::module_& m);
// Функция для биндинга PageName
void bind_PageName(py::module_& m);
// Функция для биндинга PageData
void bind_PageData(py::module_& m);
// Функция для биндинга ILData
void bind_ILData(py::module_& m);
// Функция для биндинга ServDefMod
void bind_ServDefMod(py::module_& m);
// Функция для биндинга EV_DATA_OUT_OLD
void bind_EV_DATA_OUT_OLD(py::module_& m);
// Функция для биндинга CH_INFO_OUT
void bind_CH_INFO_OUT(py::module_& m);
// Функция для биндинга EV_DATA_OUT
void bind_EV_DATA_OUT(py::module_& m);
// Функция для биндинга EV_PARAM_OUT
void bind_EV_PARAM_OUT(py::module_& m);
// Функция для биндинга EV_AVG_OUT
void bind_EV_AVG_OUT(py::module_& m);
// Функция для биндинга MARKER_OUT
void bind_MARKER_OUT(py::module_& m);
// Функция для биндинга TEXT_OUT
void bind_TEXT_OUT(py::module_& m);


// Формат файла осциллограмм АЭ систем A-Line DDM, DDM-2, PCI-N и PCI8 для всех версий //created 08.05.2020

// сначала пишутся заголовки файла данных TAGFileHdr и параметры измерения TAGMeasData
////////////////////////////////////////////////////////////////
struct FileHdr
{
	UINT signature;         // signature = 0xAA551313
	UINT ver_maj;           // version major = 22 для текущей версии файлов
	UINT ver_min;           // version minor = 0 для текущей версии файлов
	UINT ftype;             // type of file	= 0xAA02 для файла осциллограмм osc-типа
	UINT hard_version;		// version of hardware controller //текущая версия DDM-2 контроллера = 0x102071f (считывается из аппаратной части)
	UINT soft_version;		// version of sort options //текущая версия ПО A-Line = 0x20600
	UINT reserved[18];      // reserved bytes for future use

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 18 }, reserved);
	}
};


struct FilterDef
{
	UINT	param;					// parameter for filtration
	double	more_than[3];			// 3 minimal value interval
	double	less_than[3];			// 3 maximum value interval
	char	p_name[100];			// name of parameter
	byte	valid;					// validiti of filter			
	byte	interv_valid[3];		// 3 value validiti of interval	

	py::array_t<double> get_less_than() {
		return py::array_t<double>({ 3 }, less_than);
	};
	py::array_t<double> get_more_than() {
		return py::array_t<double>({ 3 }, more_than);
	};
	std::wstring get_p_name() {
		return get_wstr(p_name);
	};
	py::array_t<byte> get_interv_valid() {
		return py::array_t<byte>({ 3 }, interv_valid);
	};
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

	py::array_t<byte> get_reserved() {
		return py::array_t<byte>({ 2 }, reserved);
	};
	py::array_t<BOOL> get_AndOr() {
		return py::array_t<BOOL>({ MAX_FDEFS }, AndOr);
	};
	py::array_t<FilterDef> get_FltDef() {
		return py::array_t<FilterDef>({ MAX_FDEFS }, FltDef);
	};
};



////////////////////////////////////////////////////////////////
struct MeasData
{
	UINT ver_maj;                   // version number major //резерв
	UINT ver_min;                   // version number minor //резерв
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
	__time32_t start_time;          // start time
	__time32_t stop_time;           // stop time
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
	int  SumEvents;					// (0 -> нет информации; -1 -> 0 событий)
	UINT MeasType[4];				// граничные значения окончания измерения: (0 - MaxEvents, 1 - MaxTigs, 2 - MaxFileSize, 3 - MaxOscNum)
	short IsMeasTime;				// останов измерения по окончанию времени измерения
	short IsEvents;					// останов измерения по признаку достижения максимального числа импульсов АЭ - MaxEvents
	short IsTig;					// останов измерения по признаку достижения максимального числа выбросов АЭ - MaxTigs
	short IsSize;					// останов измерения по признаку достижения максимального размера файла данный - MaxFileSize
	short IsOSC;					// останов измерения по признаку достижения максимального числа осциллограмм - MaxOscNum
	short IsRepeat;					// флаг автоматического перезапуска измерения после окончания придидущего
	UINT  Type;						// тип реализованного останова (0 - manual stop (default), 1 - max events, 2 - max tig, 3 - max file size, 4 - max osc num, 5 - max time expired)
	UINT Reserved[2];				// reserved

	std::wstring get_obj_name() {
		return get_wstr(obj_name);
	};
	std::wstring get_oper_name() {
		return get_wstr(oper_name);
	};
	std::wstring get_reference() {
		return get_wstr(reference);
	};
	std::wstring get_comments() {
		return get_wstr(comments);
	};
	std::wstring get_data_name() {
		return get_wstr(data_name);
	};
	std::wstring get_cfg_name() {
		return get_wstr(cfg_name);
	};
	std::wstring get_sdo_name() {
		return get_wstr(sdo_name);
	};
	std::wstring get_source_name() {
		return get_wstr(source_name);
	};
	std::wstring get_local_name() {
		return get_wstr(local_name);
	};
	py::array_t<UINT> get_MeasType() {
		return py::array_t<UINT>({ 4 }, MeasType);
	};
	py::array_t<UINT> get_Reserved() {
		return py::array_t<UINT>({ 2 }, Reserved);
	};
};

//	где PreFilterData - информация об использованном в ходе сбора данных префильтре



////////////////////////////////////////////////////////////////
// затем пишется заголовочный блок файла осциллограмм
////////////////////////////////////////////////////////////////
struct SDOHdr
{
	UINT NumOSC;			// общее число записанных осциллограмм
	UINT InfoPointer;		// указатель на стартовый блок записи осциллограмм в файле (если число записанных осциллограмм ненулевое)
	UINT DefSize;			// размер буфера осциллограмм, установленный по умолчанию (считывается из аппаратуры и в настоящих версиях ПО не используется)
	UINT BaseSize;			// размер структуры OSCDefMod (см. ниже)
	UINT Din;				// величина единицы АЦП в нВ
	UINT reserved[16];      // reserved bytes for future use

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 16 }, reserved);
	};
};



////////////////////////////////////////////////////////////////
// затем пишется файл конфигурации с которым был собран рассматриваемый файл данных (см. "Формат файла cfg 6.0.cpp")
////////////////////////////////////////////////////////////////


typedef unsigned long ulong;
struct AE_TIME
{
	__time32_t	seconds;	// absolute seconds value suitable for C time functions
	ulong	usec;			// microseconds since last second
};


///////////////////////////////////////////////////////////////
// далее собственно данные
// сначала идёт структура-заголовок осциллограммы, затем данные
// всего их по числу снятых осциллограмм SDOHdr.NumOSC
// поля seek_wave и seek_wave_high - заполнены для ускорения поиска определенной осциллограммы в файле osc
struct OSCDefMod
{
	UINT		chan;			//номер канала
	UINT		num_osc;		//0-A, 1-B 	
	BOOL		enabled;		//включен OSC (A или B)
	COLORREF	osc_color;		//цвета отображения osc
	BOOL		is_color_chan;	//флаг отображения цветом канала
	BOOL		save;			//флаг сохранения
	UINT		num_wind;		//номер окна для отображения
	UINT		freq;			//ч-та дискретизации, кГц
	UINT		buf_size;		//количество точек
	UINT		buf_time;		//длительность буфера, мкс (расчетное)
	UINT		pre_mod;		//кол-во точек претриггеринга
	UINT		sync;			//0 - асинхронный, 1 - синхронный мастер, 2 - синхронный слейв, 3 - синхронный мастер/слейв
	BOOL		is_th_chan;		//флаг использования порога канала
	float		tresh_db_osc;	//расчетное
	UINT		tresh_ADC_osc;	//устанавливаемое значение индивидуального порога осциллограммы
	double		K_mkV;			//для вычисления амплитуды в мкВ и дБ		
	UINT		seek_wave;		//смещение данной осциллограммы в файле (младшее 32-битное слово)		
	AE_TIME		osc_time;		//время прихода осциллограммы абсолютное	
	UINT		ADC_bit;		//разрядность АЦП							
	BOOL		is_spect;		//флаг включение\выключение спектра
	BOOL		changed;		//флаг факта произведенных изменений
	UINT		freq_max;		//максимальная частота оцифровки канала, кГц
	BOOL		notuse_pre_mod;	//флаг использования режима претриггеринга
	UINT		buf_size_max;	//максимальный размер буфера
	UINT		seek_wave_high;	//смещение данной осциллограммы в файле (старшее 32-битное слово)
	long		tFastest;		//время прихода самой быстрой волны (используется только в ALine OSC Processing)
	long		tR;				//время прихода волны Рэлея (подобранное, используется только в ALine OSC Processing)
	int			calibration;	//флаг активного режима калибрации
	int			to;				//время излучения (используется только в ALine OSC Processing для расчета дисперсионных кривых)
	int			to_ald;			//время излучения из ald (используется только в ALine OSC Processing для расчета дисперсионных кривых)
	float		L;				//расстояние до источника излучения	(используется только в ALine OSC Processing для расчета дисперсионных кривых)
	float		L_ald;			//расстояние до источника излучения из ald (используется только в ALine OSC Processing для расчета дисперсионных кривых)
	float		d;				//толщина объекта (используется только в ALine OSC Processing для расчета дисперсионных кривых)
	WORD		bPowerMethod;	//флаг активации степенного пересчета (только для 18-битных систем, в том числе DDM-2)
	WORD		nsec;			//число десятков наносекунд прошедших с момента конца предидущей микросекунды (DDM-2), прибавлять ко времени прихода osc_time
	BOOL		bSyncAEO;		//флаг синхронизации AE-OSC — «1» включение синхронизации (только для DDM-2)
	UINT		reserved[2];

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 2 }, this->reserved);
	};
};





// Формат файла конфигурации АЭ систем A-Line DDM, DDM-2, PCI-N и PCI8 для всех версий //created 08.05.2020

////////////////////////////////////////////////////////////////
//заголовок файла конфигурации
struct CfgFileHdr
{
	UINT magic_word;            // magic word = 0x1515CFCF - только для файлов cfg
	UINT ver_maj;               // major version number = 25- для текущей версии файла
	UINT ver_min;               // minor version number = 0	- для текущей версии файла
	UINT info_size;             // общий размер секции конфигурационных блоков
	UINT reserved[20];          // reserved bytes for future   

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 2 }, this->reserved);
	};
};



////////////////////////////////////////////////////////////////
//конфигурационные инфо-блоки
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
	CfgBlockDescr* cfg_blocks; // unsized array of descriptors;
};


//общая длина блока CfgFileInfo равна CfgFileHdr.info_size байт

////////////////////////////////////////////////////////////////
//далее записан буфер "configuration data" общей длиной равной CfgFileInfo.mem_size байт,
//буфер состоит из num_cfg_blocks блоков, каждый из которых характеризуется своим дескриптором CfgBlockDescr.
//в дескрипторе с block_type = X вы найдете значение block_offset - смещение начала блока X относительно начала буфера "configuration data"
//и значение block_size - размер блока hardware configuration.




struct ColorMod
{
	COLORREF	ch_color;
	BOOL		changed;
	UINT		reserved[16];

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 16 }, this->reserved);
	};
};

struct ChanDefMod
{
	BOOL		enabled;		//включение (0-выключен)
	BOOL		show;			//отображение (0-не отображается)
	UINT		high_filter;	//ФВЧ, кГц
	UINT		low_filter;		//ФHЧ, кГц
	long		gain_db;		//суммарное значение усиления канала
	float		d_ADC;			//прочитанное из ПЗУ канала характеристика АЦП	
	float		tresh_db;		//расчетное
	UINT		tresh_ADC;		//устанавливаемое значение порога канала (в ед. АЦП)
	BOOL		changed;		//флаг факта произведенных изменений
	BOOL		changed_ON;		//флаг факта произведенных изменений параметра enabled
	BOOL		autotresh;		//флаг включение автопорога
	UINT		reserved[14];

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 14 }, this->reserved);
	};
};

struct TimeDefMod
{
	UINT		sceto;			//ИКИ (SCETO)
	UINT		rtto;			//RTTO - не используется (всегда равно max_dur)
	UINT		d_time;			//Мертвое время
	UINT		max_dur;		//максимальная длительность
	BOOL		changed;		//флаг факта произведенных изменений
	UINT		reserved[16];

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 16 }, this->reserved);
	};
};

struct CalibrDefMod
{
	BOOL		enabled;		//включен калибратор
	UINT		K_calibr;		//коэфф-т усил. (раз) - не используется
	float		d_DAC;			//прочитанное из ПЗУ, т.е. =((float)REF_DAC / (float)DAC_BIT)
	UINT		Amp_v;			//расчетное, B
	UINT		Amp_DAC;		//устанавливаемое значение амплитуды калибровочного сигнала
	UINT		Freq_Calibr;	//не используется
	BOOL		changed;		//флаг факта произведенных изменений
	BOOL		changed_ON;		//флаг факта произведенных изменений enabled
	UINT		mode;			//установленный режим работы излучателей для DDM-2 (0—passive,not sync osc / 1—active,radiate / 2- passive,sync osc / 3 - AST)
	UINT		Dur_DAC;		//длительность импульса режима автокалибровки AST
	float		F1;				//значение левой границы полосы спектра для анализа калибровки
	float		F2;				//значение правой границы полосы спектра для анализа калибровки
	float		Amin;			//минимальное значение амплитуды самокалиброчоного импульса
	float		Smin;			//минимальное значение определяемого отношения парциальных энергий спектра
	float		Smax;			//максимальное значение определяемого отношения парциальных энергий спектра
	UINT		PAEtype;		//тип подсоединенного к каналу датчика										
	UINT		reserved[7];

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 7 }, this->reserved);
	};
};

struct ParDefMod		//для DDM-2 пока не используется
{
	BOOL		enabled;		//включен пар. канал
	float		K_par;			//коэфф-т усил. (раз)
	float		d_ADC_par;		//прочитанное из ПЗУ, т.е. =((float)REF_ADC_PAR / (float)ADC_BIT_PAR)
	WORD		Freq_par;		//частота опроса канала, Hz
	_int16		Freq_multi;		//множитель диапазона частоты опроса параметров
	double		K1;				//мультипликативный коэффициент пересчета величин параметрических измерений
	double		K2;				//аддитивный коэффициент пересчета величин параметрических измерений
	BOOL		changed;		//флаг факта произведенных изменений
	BOOL		changed_ON;		//флаг факта произведенных изменений enabled
	_int16		lMin;			//прочитанное из модулей значение нижней границы параметрической шкалы
	_int16		lMax;			//прочитанное из модулей значение верхней границы параметрической шкалы
	UINT		reserved[14];

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 14 }, this->reserved);
	};
};

union ald_drv_addr_t {
	DWORD	FAddr;	/* Full address */
	struct {
		WORD	Channel;					// адрес канала
		WORD	Board;						// адрес контроллера
	} bc;
};
// Обертка для union
class ald_drv_addr_wrapper {
public:
	ald_drv_addr_t addr;

	ald_drv_addr_wrapper() : addr() {}

	DWORD get_FAddr() const { return addr.FAddr; }
	void set_FAddr(DWORD value) { addr.FAddr = value; }

	WORD get_Channel() const { return addr.bc.Channel; }
	void set_Channel(WORD value) { addr.bc.Channel = value; }

	WORD get_Board() const { return addr.bc.Board; }
	void set_Board(WORD value) { addr.bc.Board = value; }
};


//Структура hardware configuration block:
//со значением CFG_HARDWARE = 1 выступает hardware configuration block, который состоит из некоторого числа структур-описателей каналов:
//FullDefMod, число этих структур совпадает с числом инталлированных в системе каналов и теоретически неограничено
struct FullDefMod
{
	UINT		 chan;
	ColorMod	 color_def;					//описатели цвета
	ChanDefMod	 chan_def;					//основные параметры канала
	TimeDefMod	 time_def;					//временные параметры канала
	OSCDefMod	 osc_def[2];				//параметры осциллографических каналов (каналы A и B)
	CalibrDefMod calibr_def;				//калибратор - в системах PCI-N не используется
	ParDefMod	 par_def[NUM_PAR_PER_MOD];	//параметрические каналы (в случае PCI-N используется только один первый канал каждой платы)
	ald_drv_addr_t addr;					//адрес канала
	WORD		 DeviceName;				//имя линии (для DDM-2 имя входа), к которой присоединен канал
	WORD		 SubDeviceName;				//имя линии для DDM-2, к которой присоединен канал
	UINT		 num_Rmod;					//номер радиовхода для модуля (только для систем с радиоканалом)
	WORD		 gpre;						//коэффициент усиления предусилителя (для DDM-2 не используется)
	_int16		 shift0;					//смещение нуля АЦП (для DDM-2 не используется)
	WORD		 DeviceType;				//тип системы (0,1 - DDM, 2 - PCI4, 3 - PCI8, 4 - PCI8E, 5 - PCI8S)
	WORD		 FilterType;				//тип фильтра 1-подгружаемые фильтры, 2-быстрое переключение, 4-парные фильтры, 8-подгрузка разрешена, 16-фильтры DDM-2
	WORD		 VersionModule;				//версия модуля (канала)
	WORD		 Capability;				//capabilities (возможности) модуля (канала)
	WORD		 reserved[22];				//reserved

	std::vector<OSCDefMod> get_osc_def() {
		return get_vector_from_arr<OSCDefMod>(this->osc_def, 2);
	};
	std::vector<ParDefMod> get_par_def() {
		return get_vector_from_arr<ParDefMod>(this->par_def, NUM_PAR_PER_MOD);
	};
	py::array_t<WORD> get_reserved() {
		return py::array_t<WORD>({ 22 }, this->reserved);
	};
};



/////////////////////////////////////////////////////
//конфигурационные блоки настройки положения окна XXX служит для сохранения положения и размера окна XXX на страницах отображения

//для главного окна программы CFG_MAINFRAME содержит структуру типа MFData 
//размер блока CFG_MAINFRAME = sizeof ( MFData )
struct MFData
{
	RECT rc;                // screen coordinates
	WINDOWPLACEMENT wp;     // window placement structure
};


//для остальных окон количество структур зависит от количества страниц отображения из блока CFG_PAGE
//каждое окно за исключением 2-х вышеперечисленных может располагаться на любом числе страниц независимо в своем размере и положении
//размер блока = QueryCfgSize * NumPages
// NumPages - число страниц отображения добавленных окон (см. описание CFG_PAGE)

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
	float		lMin_mkV;		// только для SDO (Y, unit = 2)
	float		lMax_mkV;		// только для SDO (Y, unit = 2)
	BOOL		bShowThresh;	// только для SDO (Y, unit = 2)
	int			IsLog;			// 0-линейная ось, 1-логарифмическая
	WORD		bNormal;		// флаг нормировки для гистограммы, FALSE - нет, TRUE - есть
	WORD		bMarker;		// флаг маркера окна, FALSE - нет, TRUE - есть
	BOOL		bScroll;		// флаг скроллинга оси времени, FALSE - нет, TRUE - есть
	UINT		uScrollSize;	// размер скроллируемого участка
	UINT		reserved[2];
	BOOL		bShowLabel;		// Show/Hide label

	std::wstring get_label() {
		return get_wstr(this->label);
	}
	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 2 }, this->reserved);
	};
};

struct GraphData
{
	COLORREF	cBackColor;		//цвет фона
	BOOL		cIsLines;		//флаг отображения линии/точки
	BOOL		cIsGrid_x;		//отображение оси X
	BOOL		cIsGrid_y;		//отображение оси Y
	COLORREF    cColor_x;		//пока резервн.
	COLORREF    cColor_y;		//пока резервн.
	BOOL		cIsGrid_clast;	//отображение сетки
	BYTE		reserved[24];

	py::array_t<BYTE> get_reserved() {
		return py::array_t<BYTE>({ 24 }, this->reserved);
	};
};


//////////////////////////////////////////////////
//конфигурационный блок настройки тревожных сообщений CFG_ALARMS содержит настроечные структуры тревог в количестве заведенных и сохраненных пользователем
//размер блока CFG_ALARMS = NUM_ALARMS * sizeof ( AlarmData )
struct AlarmData
{
	char		AlarmName[80];		//имя тревожного сообщения для идентификации в системе тревог
	char		WaveName[MAX_PATH];	//путь для воспроизведения звукового wav-файла
	FilterDef	FltDef[MAX_FDEFS];	//структура для определения факта активации тревоги
	BOOL		AndOr[MAX_FDEFS];	//логические ключи для активации условия в несколько строк
	LPVOID		buf;				//указатель на буфер для оперативного хранения содержимого wav-файла
	char		bMarkerSet;			//флаг установки маркера во временных окнах
	char		bActivated;			//флаг включения данной тревоги
	char		bSound;				//флаг использования звукового сопровождения
	char		bPicture;			//флаг использования 2D-3D картинки для отрисовки локационного участка
	char		bPicOff;			//флаг удаления из отрисовки лок. участка
	char		bPrevMarkerON;		//флаг установки маркера во временных окнах по тревоге в прошлую секунду 
	//(предотвращение ежесекундного вывода маркера для одномоментного события усредненных данных)
	WORD		NumLocGroup;		//номер лок. группы, содержащей список локационных участков или само окно отображения их
	UINT		NumLocDist;			//номер активируемого локационного участка в лок.группе NumLocGroup
	UINT		Reserved[16];

	std::wstring get_AlarmName() {
		return get_wstr(this->AlarmName);
	};
	std::wstring get_WaveName() {
		return get_wstr(this->WaveName);
	};
	std::vector<FilterDef> get_FltDef() {
		return get_vector_from_arr<FilterDef>(this->FltDef, MAX_FDEFS);
	};
	std::vector<BOOL> get_AndOr() {
		return get_vector_from_arr<BOOL>(this->AndOr, MAX_FDEFS);
	};
	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 16 }, this->Reserved);
	};
};


//////////////////////////////////////////////////
//конфигурационный блок CFG_GENERAL содержит структуру для хранения дополнительных настроечных параметров системы
//размер блока CFG_GENERAL = sizeof ( GeneralSetupMod )
struct GeneralSetupMod
{
	char		name_par[NUM_PAR_PER_MOD][256];	// названия окон отображения данных с параметрических каналов
	UINT		PageSize;						// размер страницы фильтра
	UINT		Din;							// опорное напряжение в нВ
	int			SingleNumber;					// флаг сквозной/двойной нумерации каналов
	char		ChanSep[2];						// разделитель при двойной нумерации
	char		LocSep[2];						// разделитель при нумерации зеркальных каналов в локации
	BOOL		AutoSaveCfg;					// автосохранение файла конфигурации
	BOOL		OSCAutoScale;					// масштабирование OSC по максимуму осциллограммы
	BOOL		SaveService;					// сохранение служебных диагностических данных
	BOOL		OnLineSpect;					// вычисление спектра в On-Line
	BOOL		UpgradeSpect;					// вычислять уточненный спектр в постобработке
	COLORREF	GenColor;						// цвет каналов для рисования общих графиков
	BOOL		ParReadCfg;						// читать коэффициенты параметрических каналов из модулей
	BOOL		NoPauseOSC;						// не останавливать работу осциллографа в режиме паузы
	BOOL		RMSOff;							// не вычислять и показывать RMS в окне осциллографа
	BOOL		ProtOn;							// флаг ведения протокола
	int			LineThickness;					// толщина линии данных на графиках (0 - min)
	BOOL		AnalizInfoMod;					// флаг вывода предупреждения для разных прошивок модулей
	BOOL		WriteServiceInfo;				// флаг сохранения служебной сервисной информации в файле
	WORD		InfoTimeout;					// таймаут ожидания информационных посылок
	WORD		StartTimeout;					// таймаут ожидания выполнения старта
	BOOL		TempMonitor;					// флаг мониторинга температуры аппаратной части
	BOOL		UseComAlarm;					// флаг посылки тревожных сообщений по COM-порту
	UINT		AlarmComPort;					// номер COM-порта для посылки тревожных сообщений
	BOOL		SyncStatic;						// флаг статического режима для синхронного просмотра
	BOOL		ManualCmd;						// флаг разрешения режим ручной подачи команд
	BOOL		ClbSyncOsc;						// флаг синхронного включения осциллограмм при калибровке
	BOOL		GetAllVer;						// флаг запроса полного комплекта версий аппаратуры
	UINT		reserved[240];
	int			Unit;							//радио выбор единиц изменения [мм, см, дм, м]
	char		ProtFileName[256];				// имя файла-протокола
	char		name_reserv[NUM_PAR_PER_MOD - 1][256];	//reserved

	std::vector<std::wstring> get_name_par() {
		std::wstring str;
		std::vector<std::wstring> vect;
		for (size_t i = 0; i < NUM_PAR_PER_MOD; i++)
		{
			str = get_wstr(this->name_par[i]);
			vect.push_back(str);
		}
		return vect;
	};
	std::wstring get_ChanSep() { return get_wstr(this->ChanSep); };
	std::wstring get_LocSep() { return get_wstr(this->LocSep); };
	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 240 }, this->reserved);
	};
	std::wstring get_ProtFileName() { return get_wstr(this->ProtFileName); };
	std::vector<std::wstring> get_name_reserv() {
		std::wstring str;
		std::vector<std::wstring> vect;
		for (size_t i = 0; i < NUM_PAR_PER_MOD - 1; i++)
		{
			str = get_wstr(this->name_par[i]);
			vect.push_back(str);
		}
		return vect;
	};
};


//////////////////////////////////////////////////
//конфигурационный CFG_AUTOSETUP содержит структуру для хранения настроечных параметров автоматического порога
//размер блока CFG_AUTOSETUP = sizeof ( AutoThresholdSetup )
// structure for update auto threshold setup parameters
struct AutoThresholdSetup
{
	BOOL	m_bAuto;					//on/off auto threshold
	int		m_normal_delta;				//delta of normal threshold change
	int		m_alarm_delta;				//delta of alarm threshold increase
	int		m_length;					//length of storage buffer to apply
	int		m_th_delta;					//если предыдущий порог отличается от нового меньше, чем это значение, то ничего не менять.
	int		m_th_min_ADC;				//min threshold, ADC
	UINT	reserved[16];

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 16 }, this->reserved);
	};
};

//////////////////////////////////////////////////
//конфигурационный блок CFG_MEASDATA содержит структуру для хранения настроечных параметров нового измерения
//размер блока CFG_MEASDATA = sizeof ( MeasData ) см. описание выше

//////////////////////////////////////////////////
//конфигурационный блок CFG_SYSTEMINFO содержит текстовый массив с информацией об аппаратной части комплекса и версии ПО для записываемого файла

//////////////////////////////////////////////////
//конфигурационный блок CFG_HOTKEYS содержит структуру для хранения настроечных параметров горячих клавиш ПО
//размер блока CFG_HOTKEYS = num_hotkeys * sizeof ( HotKeyData )
struct HotKeyData
{
	WORD	m_wVK;
	WORD	m_wMod;
	DWORD	m_nCmd;
};


// NumPostWins - число добавленных окон
// NumPages - число страниц отображения добавленных окон (см. описание CFG_PAGE)
class CPostWin
{
	int		axis_x;			//номер параметра АЭ по оси X
	int		axis_y;			//номер параметра АЭ по оси Y
	int		axis_z;			//номер параметра АЭ по оси Z
	DWORD	par_chan;		//номер параметрического канала
	UINT	reserved[8];	//reserved for future need

public:
	int get_axis_x() { return this->axis_x; };
	int get_axis_y() { return this->axis_y; };
	int get_axis_z() { return this->axis_z; };
	DWORD get_par_chan() { return this->par_chan; };
	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 8 }, this->reserved);
	};
};


struct Graph3DData
{
	COLORREF	cBackColor;		//цвет фона
	BOOL		cIsGrid_x;		//отображение оси X
	BOOL		cIsGrid_y;		//отображение оси Y
	COLORREF    cColor_x;		//пока резерв
	COLORREF    cColor_y;		//пока резерв
	BOOL		cIsGrid_clast;	//отображение сетки
	COLORREF	cObjectColor;	//цвет осей	
	BOOL		cIsGrid_z;		//отображение оси Z
	BYTE		reserved[20];

	py::array_t<BYTE> get_reserved() {
		return py::array_t<BYTE>({ 20 }, this->reserved);
	};
};



// NumHistWins - число добавленных окон
// NumPages - число страниц отображения добавленных окон (см. описание CFG_PAGE)
class CHistWin
{
	int		axis_x;			//номер параметра АЭ по оси X
	int		axis_y;			//номер параметра АЭ по оси Y
	float	step_x;			//ширина гистограммного интервала по оси X
	float	step_y;			//ширина гистограммного интервала по оси Y
	int		type;			//тип распределения: 0 - дифференциальное, 1 - кумулятивное
	BOOL	bSummed;		//флаг суммарности по каналам
	UINT	reserved[6];	//reserved for future need

public:
	int get_axis_x() { return this->axis_x; };
	int get_axis_y() { return this->axis_y; };
	float get_step_x() { return this->step_x; };
	float get_step_y() { return this->step_y; };
	int get_type() { return this->type; };
	BOOL get_bSummed() { return this->bSummed; };
	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 6 }, this->reserved);
	};
};


//////////////////////////////////////////////////
//конфигурационный блок CFG_IPDEVICE содержит информацию о настройках сетевых подключений блоков сбора и обработки данных типа EtnernetBox (число num_ipdevices не ограничено)
//размер блока CFG_IPDEVICE = num_ipdevices * sizeof ( NetIPInfo )
struct NetIPInfo	//структура параметров настройки сетевого концентратора
{
	BYTE		IPAddress[4];	//IP-адрес устройства
	DWORD		pID;			//идентификационный номер устройства
	int			bStatus;		//-1 - опрашивается, 0 - connected, 1 - discinnected
	WORD		MinLineAddr;	//начальный номер линии в данном устройстве
	WORD		TimeElapsed;	//время отведенное на соединение
	WORD		LineNums[8];	//номера подключенных к устройству линий
	WORD		LineAddr[8];	//адреса подключенных к устройству линий для коммуникации
	DWORD		reserved[10];

	std::vector<BYTE> get_IPAddress() {
		return get_vector_from_arr<BYTE>(this->IPAddress, 4);
	};
	std::vector<WORD> get_LineNums() {
		return get_vector_from_arr<WORD>(this->LineNums, 8);
	};
	std::vector<WORD> get_LineAddr() {
		return get_vector_from_arr<WORD>(this->LineAddr, 8);
	};
	std::vector<DWORD> get_reserved() {
		return get_vector_from_arr<DWORD>(this->reserved, 10);
	};
};


//////////////////////////////////////////////////
//конфигурационный блок CFG_AMPCRIT содержит информацию о настройках амплитудного критерия оценки опасности источников АЭ
//размер блока CFG_AMPCRIT = sizeof ( ClassAmpDef )
struct ClassAmpDef
{
	BOOL	show_amp_crit;		// 1 - открыть окно амп. крит.
	int		is_auto;			// 0 - автом. выбор пар-ров, 1 - ручной
	int		is_EN;				// 0 - критерий в терминах ПБ, 1 - критерий в терминах EN
	int		material;			// не используется
	int		object;				// не используется
	UINT	A1;
	UINT	A2;
	UINT	N1;
	UINT	N2;
	UINT	reserved[10];

	std::vector<UINT> get_reserved() {
		return get_vector_from_arr<UINT>(this->reserved, 10);
	};
};


//////////////////////////////////////////////////
//конфигурационный блок CFG_FONT содержит информацию о настройках текстовых шрифтов
//размер блока CFG_FONT = 5 * sizeof ( LOGFONT )
//LOGFONT	mFont[5];
//OSCTextFont.GetLogFont(&mFont[0]);		//шрифт отображения легенды в окне осциллографа
//MarkerTextFont.GetLogFont(&mFont[1]);	//шрифт отображения подписи на маркерах
//LDTextFont.GetLogFont(&mFont[2]);		//шрифт отображения шкал и надписей на экране
//PrinterFont.GetLogFont(&mFont[3]);		//шрифт отображения шкал и надписей на печати
//TabCtrlFont.GetLogFont(&mFont[4]);		//шрифт отображения подписи на страницах отображения

//////////////////////////////////////////////////
//конфигурационный блок CFG_PAGE содержит информацию о настройках системы многостраничного отображения окон
//размер блока CFG_PAGE = sizeof ( PageData ) + sizeof ( PageName ) * NumPages;
//NumPages - число страниц (вкладок) главного окна программы
struct PageName
{
	char page_name[MAX_PAGENAMELENGTH];

	std::wstring get_page_name() {
		return get_wstr(this->page_name);
	}
};

struct PageData
{
	UINT mem_size;		// size of page configuration
	UINT num_pages;		// number of pages in file
	UINT cur_page;		// currently selectde page
	PageName* name;		// unsized array of page names;

	std::vector<PageName> get_name() {
		return get_vector_from_arr<PageName>(this->name, 0);
	}
};


//////////////////////////////////////////////////
//конфигурационный блок CFG_PROFILE содержит информацию о настройках системы мультипрофильных настроек системы (см. CFG_PAGE)
//размер блока CFG_PROFILE = sizeof ( PageData ) + sizeof ( PageName ) * NumProfiles;
//NumProfiles - число профилей (настроек) аппаратуры

//////////////////////////////////////////////////
//конфигурационный блок CFG_IMPLINES содержит информацию о настройках внешней линии нагружения для накладывания на временные окна отображения
//размер блока CFG_IMPLINES = sizeof ( ILData );
struct ILData	//27.12.99S //03.05.06S
{
	BOOL		enable_import;		//флаг включения наложения линии
	int			ParInput;			//флаг наложения линии из данных параметрического канала
	int			ParChanNum;			//номер параметрического входа
	int			ParModNum;			//номер основного канала
	char		ParName[MAX_PATH];	//имя параметра
	char		FileName[MAX_PATH];	//имя файла для наложения параметра из текстового файла
	int			thickness;			//толщина линии отображения
	COLORREF	color;				//цвет отображения
	double		NumFraction;		//точность (число десятичных знаков)

	std::wstring get_ParName() { return get_wstr(this->ParName); };
	std::wstring get_FileName() { return get_wstr(this->FileName); };
};



//////////////////////////////////////////////////
struct ServDefMod
{
	BOOL		show;				//флаг отображения
	COLORREF	color;				//цвет отображения
	int			K1;					//мультипликативный коэффициент
	int			K2;					//аддитивный коэффициент
	int			show_text;			//флаг отображения текстового значения
	UINT		reserved[5];

	std::vector<UINT> get_reserved() {
		return get_vector_from_arr<UINT>(this->reserved, 5);
	};
};



//enum	DataTypes { ev_data_old = 0, sys_data = 1, noise_data_old = 2, param_data = 3, noise_data = 4, ev_data = 5, marker = 6, text = 7, reserv = 8, service_data = 9, u_param_data = 34, radio_data = 11, u_avg_data = 33 };
enum	DataTypes {
	ev_data_old = 0,
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
	unis_avg_data = 12
};	//27.05.15S //25.05.16S

//WORD	type; //тип данных согласно DataTypes
//WORD	size; //размер структуры соответствующей структуры типа type
//				sizeof(EV_DATA_OUT_OLD) - ev_data_old, устаревший тип посылок с параметрами АЭ-импульсов, не используется
//				sizeof(CH_INFO_OUT)		- sys_data, посылки командно-системного типа
//				sizeof(EV_DATA_OUT)		- noise_data_old, устаревший тип усреднительных посылок, не используется
//				sizeof(EV_PARAM_OUT)	- param_data, посылки с данными параметрических измерений
//				sizeof(EV_AVG_OUT)		- noise_data, посылки с усредненными данными АЭ-измерений
//				sizeof(EV_DATA_OUT)		- ev_data, основной тип посылок с параметрами АЭ-импульсов
//				sizeof(MARKER_OUT)		- marker, посылка с временем временного маркера
//				sizeof(TEXT_OUT)		- text, посылки с текстом временного маркера
//				1,						- reserved, резерв
//				sizeof(SERVICE_OUT)		- service_data, посылка со служебными диагностическими данными
//				sizeof(UNI_PARAM_OUT)	- u_param_data,	посылки с данными параметрических измерений от прибора ЮНИСКОП
//				sizeof(RADIO_OUT)		- radio_data, посылка со служебными диагностическими данными измерений в режиме радио
//				sizeof(UNI_AVG_OUT)		- u_avg_data, посылки с усредненными данными АЭ-измерений от прибора ЮНИСКОП

struct EV_DATA_OUT_OLD	//ev_data_old - не используется
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
};


struct CH_INFO_OUT	// sys_data
{
	AE_TIME		chan_info_time;	// время прихода посылки
	WORD		chan_num;		// номер канала
	WORD		type;			// тип посылки
	WORD		data[8];		// данные

	std::vector<WORD> get_data() {
		return get_vector_from_arr<WORD>(this->data, 8);
	};
};

// Из посылок данного типа можно извлекать значение установленного порога и усиления
// Здесь есть значащее поле type:
// для type = 18 и type = 3 в поле data[0] – лежит значение нового установленного порога в ед. АЦП
// для type = 14 в поле data[0] – лежит значение нового установленного Кус в дБ


struct EV_DATA_OUT	// ev_data
{
	AE_TIME		ev_time;	 	// время прихода импульса
	WORD		chan_num;		// номер канала
	WORD		max_ampl;		// младшее 16-битное слово амплитуды импульса
	WORD		duration;		// младшее 16-битное слово длительности импульса
	WORD		expnd;			// слово дополнительных разрядов и частоты оцифровки для 18-битных систем (DDM-2)
	WORD		rise_time;	    // младшее 16-битное слово времи нарастания переднего фронта
	WORD		flags_add;		// расширение поля флагов для 18-битных систем (DDM-2)
	WORD        tig;			// младшее 16-битное слово числа выбросов (осцилляций)
	WORD		attenuation;    // коэфф. затухания амплитуды (умножен на 10) для вычисления значения локационной амплитуды
	WORD		energy_low;		// энергия импульса (младшее слово)
	WORD		energy_medium;	// энергия импульса (среднее слово)
	WORD		energy_high;	// энергия импульса (старшее слово)
	WORD		flags;			// флаги
	WORD		tad;			// РВП, мкс
	BYTE		loc_event;		// флаг принадлежности локализованному событию: 0-нет, 1-да, 5-удалить
	BYTE		nsec;			// десятки наносекунд после окончания текщей микросекунды (DDM-2), прибавлять ко времени прихода ev_time
	LONG        locX;			// локационная координата X [мм], (NO_LOC) - нет локации, (SEVERAL_LOC) - возможно несколько локаций
	LONG        locY;			// локационная координата Y [мм]
	LONG        locZ;			// локационная координата Z [мм]
	LONG		gain;			// текущее значение усиления [dB]
};

double ConvertEnergy(EV_DATA_OUT* data);



struct EV_PARAM_OUT	// param_data (для DDM-2 не используется)
{
	AE_TIME		param_time;						// время прихода параметрической посылки
	WORD		chan_num;						// номер канала
	WORD		num_summ;						// число суммирований значений параметров
	WORD		par_summ[NUM_PAR_PER_MOD];		// суммарное значение параметров по каналам
	
	std::vector<WORD> get_par_summ() {
		return get_vector_from_arr<WORD>(this->par_summ, NUM_PAR_PER_MOD);
	};
};


struct EV_AVG_OUT	// noise_data - посылка усреднительного типа, содержит результаты вычисления средних значений за период усреднения (1 сек)
{
	AE_TIME		avg_time;				// время прихода
	WORD		chan_num;				// номер канала
	WORD		num_summ;				// число суммирований для вычисления среднего уровня шума (=1)
	DWORD		noise_summ_ampl;		// результат суммирований для вычисления среднего уровня шума
	WORD		intensity;				// активность АЭ
	WORD		lost;					// число потеряных АЭ импульсов (из-за ограничений возможностей передачи)
	double		summ_energy;			// энергия АЭ импульсов
	DWORD		summ_dur;				// длительность АЭ импульсов
	DWORD		summ_ampl;				// амплитуда АЭ импульсов
	DWORD		summ_rise;				// время нарастания переднего фронта
	DWORD		summ_tig;				// число выбросов (осцилляций)
	WORD		max_ampl;				// максимальная амплитуда АЭ импульса за период усреднения
	WORD		expndw;					// слово дополнительных разрядов и частоты оцифровки для 18-битных систем (DDM-2)
	_int8		cur_gain_dB;			// текущее значение усиления [dB]
	BYTE		expndb;					// байт дополнительных разрядов и частоты оцифровки для 18-битных систем (DDM-2)
	WORD		reserved[7];

	std::vector<WORD> get_reserved() {
		return get_vector_from_arr<WORD>(this->reserved, 7);
	};
};

struct MARKER_OUT		// marker
{
	AE_TIME		marker_time;		// время прихода маркета
	DWORD		markerID;			// идентификатор маркера для привязки к тексту маркера
};


struct TEXT_OUT			// text
{
	DWORD		markerID;			// идентификатор маркера для привязки ко времени маркера
	char		marker_text[68];	// текст маркера

	std::wstring get_p_name() {
		return get_wstr(marker_text);
	};
};


// доп структуры 

struct UNI_AVG_OUT
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
};

struct UNI_PARAM_OUT	
{
	AE_TIME			time;
	WORD			chan_num;
	float			value;
	unsigned char	prm_chan;
	unsigned char	prm_type;
	unsigned char	flags;
	unsigned char	res;
};

struct SERVICE_OUT	
{
	AE_TIME		s_time;		// service info absolute time
	WORD		board;		// number of device to inform
	BYTE		type;		// type of info					
	BYTE		line;		// name of line (in IP setup)	
	WORD		data[12];	// info data set
};

#define NUM_RPARAM	5			
// radioplan status struct	
struct RADIO_OUT
{
	AE_TIME		r_time;
	WORD		chan_num;
	WORD		data[NUM_RPARAM];
};

#define NUM_FILE_STRUCT	13		//24.04.01S //22.04.04S //30.01.11S //25.05.16S
const   UINT	DataSizes[NUM_FILE_STRUCT] = {
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
				sizeof(UNI_AVG_OUT) };

////////////////////////////////////////////////////////////////////////
//Struct member alignment установлено по умолчанию в MSVisualC++, т.е. равным 8 байт

///////////////THE END//////////THE END////////THE END/////////////////
