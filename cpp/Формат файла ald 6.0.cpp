// Формат файла данных АЭ систем A-Line DDM, DDM-2, PCI-N и PCI8 для всех версий //created 08.05.2020

// сначала пишутся заголовки файла данных TAGFileHdr и параметры измерения TAGMeasData
////////////////////////////////////////////////////////////////
typedef struct TAGFileHdr
{
    UINT signature;         // signature = 0xAA551313
    UINT ver_maj;           // version major = 43 для текущей версии файлов
    UINT ver_min;           // version minor = 0 для текущей версии файлов
    UINT ftype;             // type of file	= 0xAA01 для файла данных ald-типа
	UINT hard_version;		// version of hardware controller //текущая версия DDM-2 контроллера = 0x102071f (считывается из аппаратной части)
	UINT soft_version;		// version of sort options //текущая версия ПО A-Line = 0x20600
    UINT reserved[18];      // reserved bytes for future use
}FileHdr;

////////////////////////////////////////////////////////////////
typedef struct TAGMeasData
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
}MeasData;

//	где PreFilterData - информация об использованном в ходе сбора данных префильтре
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
// затем пишется файл конфигурации с которым был собран рассматриваемый файл данных (см. "Формат файла cfg 6.0.cpp")
////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
// далее собственно данные

enum	DataTypes { ev_data_old = 0, sys_data = 1, noise_data_old = 2, param_data = 3, noise_data = 4, ev_data = 5, marker = 6, text = 7, reserv = 8, service_data = 9,	u_param_data = 34, radio_data = 11, u_avg_data = 33 };

WORD	type; //тип данных согласно DataTypes
WORD	size; //размер структуры соответствующей структуры типа type
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


typedef struct TAG_AETIME
{
	__time32_t	seconds;	// absolute seconds value suitable for C time functions
	ulong	usec;			// microseconds since last second
}AE_TIME;

typedef struct TAG_EV_DATA_OUT_OLD	//ev_data_old - не используется
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
	AE_TIME		chan_info_time;	// время прихода посылки
	WORD		chan_num;		// номер канала
	WORD		type;			// тип посылки
	WORD		data[8];		// данные
}CH_INFO_OUT;
// Из посылок данного типа можно извлекать значение установленного порога и усиления
// Здесь есть значащее поле type:
// для type = 18 и type = 3 в поле data[0] – лежит значение нового установленного порога в ед. АЦП
// для type = 14 в поле data[0] – лежит значение нового установленного Кус в дБ


#define NO_LOC		-1000000001
#define SEVERAL_LOC	-1000000002
typedef struct TAG_EV_DATA_OUT	// ev_data
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
}EV_DATA_OUT;

//расчет правильных 18-битных величин параметров АЭ-импульсов с учетом слова дополнительных разрядов выполнять так:
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
// flags-флаги по переполнению АЦП (А), макс. длительности (D) по данному импульсу
// EV_DATA_OUT.flags & 0x01?"A":" - ";
// EV_DATA_OUT.flags & 0x04?"D":" - ";
// локационная амплитуда LA = int(( EV_DATA_OUT.flags & 0xFFF0 ) >> 4);	
// флаг полярности первого вступления ("+" - порог превышен в положительную полуволну колебаний, "-" - в отрицательную) - только для expnd != 0
// EV_DATA_OUT.flags & 0x08? "-":"+"
// флаг полярности максимума ("+" - максимум достигнут в положительную полуволну колебаний, "-" - в отрицательную) - только для expnd != 0
// EV_DATA_OUT.flags_add & 0x01? "-":"+"

typedef struct TAG_EV_PARAM_OUT	// param_data (для DDM-2 не используется)
{
	AE_TIME		param_time;						// время прихода параметрической посылки
	WORD		chan_num;						// номер канала
	WORD		num_summ;						// число суммирований значений параметров
	WORD		par_summ [ NUM_PAR_PER_MOD ];	// суммарное значение параметров по каналам
}EV_PARAM_OUT;
// значение величины параметрических измерений выводится после пересчета по формуле
// V[i] = ( par_summ[i] / num_summ ) * K1[i] + K2[i], где i-номер параметрического входа, К1[i]-мультипликативный, К2[i]-аддитивный коэффициенты пересчета из ParDefMod (см. формат файла конфигурации)

typedef struct TAG_EV_AVG_OUT	// noise_data - посылка усреднительного типа, содержит результаты вычисления средних значений за период усреднения (1 сек)
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
}EV_AVG_OUT;

//расчет правильных 18-битных величин усредненных параметров АЭ-импульсов с учетом слова дополнительных разрядов выполнять так:
//	noise_summ_ampl_full = ( DWORD ( EV_AVG_OUT.expndb & 0x08 ) << 13 ) + EV_AVG_OUT.noise_summ_ampl;
//	intensity_full	= ( DWORD ( EV_AVG_OUT.expndb & 0x30 ) << 12 ) + EV_AVG_OUT.intensity;
//	energy_full		= EV_AVG_OUT.summ_energy / double ( intensity_full );

//далее расчет только для EV_AVG_OUT.expndb != 0 и EV_AVG_OUT.expndw != 0 (для 18-битных систем, в том числе DDM-2):
//	max_ampl_full	= ( DWORD ( EV_AVG_OUT.expndw & 0x08 ) << 13 ) + HIWORD ( EV_AVG_OUT.summ_dur );
//  tig_full		= ( DWORD ( EV_AVG_OUT.expnd & 0x80 ) << 9 ) + HIWORD ( EV_AVG_OUT.summ_ampl );
//	duration_full	= (( DWORD ( EV_AVG_OUT.expndw & 0x07 ) << 16 ) + LOWORD ( EV_AVG_OUT.summ_dur )) * AE_Kernel->ExtractTimeQuant ( EV_AVG_OUT.expndb );
//	rise_time_full	= (( DWORD ( EV_AVG_OUT.expndw & 0x70 ) << 12 ) + LOWORD ( EV_AVG_OUT.summ_ampl )) * AE_Kernel->ExtractTimeQuant ( EV_AVG_OUT.expndb );
//	ASL_value		= (( DWORD ( HIWORD ( EV_AVG_OUT.summ_rise )) & 0x100 ) << 8 ) + HIWORD ( EV_AVG_OUT.summ_tig );
//	RMS_value		= (( DWORD ( HIWORD ( EV_AVG_OUT.summ_rise )) & 0xFF ) << 16 ) + EV_AVG_OUT.max_ampl;

//	в противном случае для EV_AVG_OUT.expndb == 0 (для 16-битных систем) считать так:
//	max_ampl_full	= round ( EV_AVG_OUT.summ_ampl / double ( intensity_full ));
//	duration_full	= round ( EV_AVG_OUT.summ_dur / double ( intensity_full ));
//	rise_time_full	= round ( EV_AVG_OUT.summ_rise / double ( intensity_full ));
//	tig_full		= round ( EV_AVG_OUT.summ_tig / double ( intensity_full ));
//	ASL и RMS в этом случае не вычисляются

typedef struct TAG_MARKER_OUT		// marker
{
	AE_TIME		marker_time;		// время прихода маркета
	DWORD		markerID;			// идентификатор маркера для привязки к тексту маркера
}MARKER_OUT;

typedef struct TAG_TEXT_OUT			// text
{
	DWORD		markerID;			// идентификатор маркера для привязки ко времени маркера
	char		marker_text[68];	// текст маркера
}TEXT_OUT;

// амплитудные параметры в [дБ] вычисляются так А[дБ] = 20.*log10 ( 1000. * (max_ampl_full, ASL_value)) * d_ADC / Din - gain, где
// max_ampl_full или ASL_value - амплитуды в ед. АЦП, берется из EV_DATA_OUT и EV_AVG_OUT
// gain - усиление ( теперь оно в дБ ) 
// берется тоже из EV_DATA_OUT или FullDefMod
// Dadc - ADC step value in mkV, берется из d_ADC - FullDefMod данного канала (см. формат файла cfg),
// Din - scale base value in nV, опорное напряжение по умолчанию установлено 1 мкВ (см. формат файла cfg), 

// энергия [дБ] = 10.* log10 ( energy_full * 1000./ F) + 20. * log10(K_mkV), где
// energy_full - энергия в ед. АЦП, берется из EV_DATA_OUT,
// F - максимальная частота оцифровки канала - freq_max из OSCDefMod;
// K_mkV = d_ADC / pow ( 10., gain / 20. ) 

////////////////////////////////////////////////////////////////////////

//Struct member alignment установлено по умолчанию в MSVisualC++, т.е. равным 8 байт

///////////////THE END//////////THE END////////THE END/////////////////
