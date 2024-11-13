// Формат файла осциллограмм АЭ систем A-Line DDM, DDM-2, PCI-N и PCI8 для всех версий //created 08.05.2020

// сначала пишутся заголовки файла данных TAGFileHdr и параметры измерения TAGMeasData
////////////////////////////////////////////////////////////////
typedef struct TAGFileHdr
{
    UINT signature;         // signature = 0xAA551313
    UINT ver_maj;           // version major = 22 для текущей версии файлов
    UINT ver_min;           // version minor = 0 для текущей версии файлов
    UINT ftype;             // type of file	= 0xAA02 для файла осциллограмм osc-типа
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
// затем пишется заголовочный блок файла осциллограмм
////////////////////////////////////////////////////////////////
typedef struct TAGSDOHdr
{
	UINT NumOSC;			// общее число записанных осциллограмм
	UINT InfoPointer;		// указатель на стартовый блок записи осциллограмм в файле (если число записанных осциллограмм ненулевое)
	UINT DefSize;			// размер буфера осциллограмм, установленный по умолчанию (считывается из аппаратуры и в настоящих версиях ПО не используется)
	UINT BaseSize;			// размер структуры OSCDefMod (см. ниже)
	UINT Din;				// величина единицы АЦП в нВ
    UINT reserved[16];      // reserved bytes for future use
}SDOHdr;

////////////////////////////////////////////////////////////////
// затем пишется файл конфигурации с которым был собран рассматриваемый файл данных (см. "Формат файла cfg 6.0.cpp")
////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
// далее собственно данные
// сначала идёт структура-заголовок осциллограммы, затем данные
// всего их по числу снятых осциллограмм SDOHdr.NumOSC
// поля seek_wave и seek_wave_high - заполнены для ускорения поиска определенной осциллограммы в файле osc
typedef struct TAGOSCDefMod
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
}OSCDefMod;

typedef struct TAG_AETIME
{
	__time32_t	seconds;	// absolute seconds value suitable for C time functions
	ulong	usec;			// microseconds since last second
}AE_TIME;

///////////////////////////////////////////////////////////////
// далее сразу отсчёты АЦП (т.е. сама осциллограмма) - простой массив из buf_size слов, 
// одно слово - 2 байта, это одна точка ampl (отсчёт АЦП)
// для 18-битных систем используется степенной пересчет для получения 18-битного значений ampl_full из 16-битного слова ampl (флаг активации bPowerMethod при этом установлен в |= 0x100):
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

// для справки:
// амплитуда [дБ] = 20.*log10 ( 1000. * ampl_full ) * d_ADC / Din - gain, где
// ampl - амплитуда в ед. АЦП, берется из данных
// gain - усиление ( теперь оно в дБ ) 
// берется из FullDefMod
// Dadc - ADC step value in mkV, берется из d_ADC - FullDefMod данного канала,
// Din - scale base value in nV, опорное напряжение 1 мкВ,

////////////////////////////////////////////////////////////////////////

//Struct member alignment установлено по умолчанию в MSVisualC++, т.е. равным 8 байт

///////////////THE END//////////THE END////////THE END/////////////////
