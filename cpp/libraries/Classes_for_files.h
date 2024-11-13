#include <logger.h>

#include "afxpriv.h"
#include <time.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <cwchar>    // Для функции mbstowcs
#include <cstring>   // Для strlen
#include <string>

#pragma once


namespace py = pybind11;

#define MAX_FDEFS 4

std::wstring get_wstr(char* l_str) {
	std::setlocale(LC_ALL, "Russian_Russia.1251");
	// Определяем длину новой строки и выделяем память для wchar_t строки
	size_t len = std::strlen(l_str) + 1;
	wchar_t* wchar_str = new wchar_t[len];
	// Преобразуем из многобайтовой строки в широкую строку
	std::mbstowcs(wchar_str, l_str, len);
	std::wstring w_str = std::wstring(wchar_str);

	delete[] wchar_str;
	return w_str;
};
template <typename T>
std::vector<T> get_vector_from_arr(T* arr, int len) {
	std::vector<T> local_vector;
	for (size_t i = 0; i < len; i++) { local_vector.push_back(arr[i]); }
	return local_vector;
};
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

// Функция для биндинга FileHdr
void bind_FileHdr(py::module_& m) {
	py::class_<FileHdr>(m, "FileHdr")
		.def(py::init<>())
		.def_readwrite("signature", &FileHdr::signature)
		.def_readwrite("ver_maj", &FileHdr::ver_maj)
		.def_readwrite("ver_min", &FileHdr::ver_min)
		.def_readwrite("ftype", &FileHdr::ftype)
		.def_readwrite("hard_version", &FileHdr::hard_version)
		.def_readwrite("soft_version", &FileHdr::soft_version)
		.def("reserved", &FileHdr::get_reserved);
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

// Функция для биндинга FilterDef
void bind_FilterDef(py::module_& m) {
	py::class_<FilterDef>(m, "FilterDef")
		.def(py::init<>())
		.def_readwrite("param", &FilterDef::param)
		.def_readwrite("valid", &FilterDef::valid)
		.def("get_more_than", &FilterDef::get_more_than)
		.def("get_less_than", &FilterDef::get_less_than)
		.def("get_p_name", &FilterDef::get_p_name)
		.def("get_interv_valid", &FilterDef::get_interv_valid);
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

// Функция для биндинга FilterDef
void bind_PreFilterData(py::module_& m) {
	py::class_<PreFilterData>(m, "PreFilterData")
		.def(py::init<>())
		.def_readwrite("DontSave", &PreFilterData::DontSave)
		.def_readwrite("ver_PFilter", &PreFilterData::ver_PFilter)
		.def_readwrite("DontProcess", &PreFilterData::DontProcess)
		.def_readwrite("IsKeep", &PreFilterData::IsKeep)
		.def("get_reserved", &PreFilterData::get_reserved)
		.def("get_AndOr", &PreFilterData::get_AndOr)
		.def("get_FltDef", &PreFilterData::get_FltDef);
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

// Функция для биндинга MeasData
void bind_MeasData(py::module_& m) {
	py::class_<MeasData>(m, "MeasData")
		.def(py::init<>())
		.def_readwrite("ver_maj", &MeasData::ver_maj)
		.def_readwrite("ver_min", &MeasData::ver_min)
		.def_readwrite("comsize", &MeasData::comsize)
		.def_readwrite("filtered_flag", &MeasData::filtered_flag)
		.def_readwrite("meas_hour", &MeasData::meas_hour)
		.def_readwrite("meas_min", &MeasData::meas_min)
		.def_readwrite("init_hour", &MeasData::init_hour)
		.def_readwrite("init_min", &MeasData::init_min)
		.def_readwrite("start_time", &MeasData::start_time)
		.def_readwrite("stop_time", &MeasData::stop_time)
		.def_readwrite("avg_time", &MeasData::avg_time)
		.def_readwrite("num_SDO", &MeasData::num_SDO)
		.def_readwrite("DontWriteDisk", &MeasData::DontWriteDisk)
		.def_readwrite("DontWritePar", &MeasData::DontWritePar)
		.def_readwrite("DontWriteNoise", &MeasData::DontWriteNoise)
		.def_readwrite("DontWriteSDO", &MeasData::DontWriteSDO)
		.def_readwrite("ActivatePreFilter", &MeasData::ActivatePreFilter)
		.def_readwrite("PFData", &MeasData::PFData)
		.def_readwrite("IsCoorPresent", &MeasData::IsCoorPresent)
		.def_readwrite("TypeLoc", &MeasData::TypeLoc)
		.def_readwrite("LENGTH", &MeasData::LENGTH)
		.def_readwrite("HEIGHT", &MeasData::HEIGHT)
		.def_readwrite("SHIFT_HEIGHT", &MeasData::SHIFT_HEIGHT)
		.def_readwrite("SHIFT_LEFT", &MeasData::SHIFT_LEFT)
		.def_readwrite("ClusterLength", &MeasData::ClusterLength)
		.def_readwrite("ClusterWidth", &MeasData::ClusterWidth)
		.def_readwrite("BOUND", &MeasData::BOUND)
		.def_readwrite("SumEvents", &MeasData::SumEvents)
		.def_readwrite("IsMeasTime", &MeasData::IsMeasTime)
		.def_readwrite("IsEvents", &MeasData::IsEvents)
		.def_readwrite("IsTig", &MeasData::IsTig)
		.def_readwrite("IsSize", &MeasData::IsSize)
		.def_readwrite("IsOSC", &MeasData::IsOSC)
		.def_readwrite("IsRepeat", &MeasData::IsRepeat)
		.def_readwrite("Type", &MeasData::Type)
		.def("get_obj_name", &MeasData::get_obj_name)
		.def("get_oper_name", &MeasData::get_oper_name)
		.def("get_reference", &MeasData::get_reference)
		.def("get_comments", &MeasData::get_comments)
		.def("get_data_name", &MeasData::get_data_name)
		.def("get_cfg_name", &MeasData::get_cfg_name)
		.def("get_sdo_name", &MeasData::get_sdo_name)
		.def("get_source_name", &MeasData::get_source_name)
		.def("get_local_name", &MeasData::get_local_name)
		.def("get_MeasType", &MeasData::get_MeasType)
		.def("get_Reserved", &MeasData::get_Reserved);
};

//	где PreFilterData - информация об использованном в ходе сбора данных префильтре
#define MAX_FDEFS 4





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

// Функция для биндинга SDOHdr
void bind_SDOHdr(py::module_& m) {
	py::class_<SDOHdr>(m, "SDOHdr")
		.def(py::init<>())
		.def_readwrite("NumOSC", &SDOHdr::NumOSC)
		.def_readwrite("InfoPointer", &SDOHdr::InfoPointer)
		.def_readwrite("DefSize", &SDOHdr::DefSize)
		.def_readwrite("BaseSize", &SDOHdr::BaseSize)
		.def_readwrite("Din", &SDOHdr::Din)
		.def("get_reserved", &SDOHdr::get_reserved);
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

// Функция для биндинга AE_TIME
void bind_AE_TIME(py::module_& m) {
	py::class_<AE_TIME>(m, "AE_TIME")
		.def(py::init<>())
		.def_readwrite("seconds", &AE_TIME::seconds)
		.def_readwrite("usec", &AE_TIME::usec);
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

// Функция для биндинга OSCDefMod
void bind_OSCDefMod(py::module_& m) {
	py::class_<OSCDefMod>(m, "OSCDefMod")
		.def(py::init<>())
		.def_readwrite("chan", &OSCDefMod::chan)
		.def_readwrite("num_osc", &OSCDefMod::num_osc)
		.def_readwrite("enabled", &OSCDefMod::enabled)
		.def_readwrite("osc_color", &OSCDefMod::osc_color)
		.def_readwrite("is_color_chan", &OSCDefMod::is_color_chan)
		.def_readwrite("save", &OSCDefMod::save)
		.def_readwrite("num_wind", &OSCDefMod::num_wind)
		.def_readwrite("freq", &OSCDefMod::freq)
		.def_readwrite("buf_size", &OSCDefMod::buf_size)
		.def_readwrite("buf_time", &OSCDefMod::buf_time)
		.def_readwrite("pre_mod", &OSCDefMod::pre_mod)
		.def_readwrite("is_th_chan", &OSCDefMod::is_th_chan)
		.def_readwrite("tresh_db_osc", &OSCDefMod::tresh_db_osc)
		.def_readwrite("tresh_ADC_osc", &OSCDefMod::tresh_ADC_osc)
		.def_readwrite("K_mkV", &OSCDefMod::K_mkV)
		.def_readwrite("seek_wave", &OSCDefMod::seek_wave)
		.def_readwrite("osc_time", &OSCDefMod::osc_time)
		.def_readwrite("ADC_bit", &OSCDefMod::ADC_bit)
		.def_readwrite("is_spect", &OSCDefMod::is_spect)
		.def_readwrite("changed", &OSCDefMod::changed)
		.def_readwrite("freq_max", &OSCDefMod::freq_max)
		.def_readwrite("notuse_pre_mod", &OSCDefMod::notuse_pre_mod)
		.def_readwrite("buf_size_max", &OSCDefMod::buf_size_max)
		.def_readwrite("seek_wave_high", &OSCDefMod::seek_wave_high)
		.def_readwrite("tFastest", &OSCDefMod::tFastest)
		.def_readwrite("tR", &OSCDefMod::tR)
		.def_readwrite("calibration", &OSCDefMod::calibration)
		.def_readwrite("to", &OSCDefMod::to)
		.def_readwrite("to_ald", &OSCDefMod::to_ald)
		.def_readwrite("L", &OSCDefMod::L)
		.def_readwrite("L_ald", &OSCDefMod::L_ald)
		.def_readwrite("d", &OSCDefMod::d)
		.def_readwrite("bPowerMethod", &OSCDefMod::bPowerMethod)
		.def_readwrite("nsec", &OSCDefMod::nsec)
		.def_readwrite("bSyncAEO", &OSCDefMod::bSyncAEO)
		.def("get_reserved", &OSCDefMod::get_reserved);
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

// Функция для биндинга CfgFileHdr
void bind_CfgFileHdr(py::module_& m) {
	py::class_<CfgFileHdr>(m, "CfgFileHdr")
		.def(py::init<>())
		.def_readwrite("magic_word", &CfgFileHdr::magic_word)
		.def_readwrite("ver_maj", &CfgFileHdr::ver_maj)
		.def_readwrite("ver_min", &CfgFileHdr::ver_min)
		.def_readwrite("info_size", &CfgFileHdr::info_size)
		.def("get_reserved", &CfgFileHdr::get_reserved);
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
// Функция для биндинга CfgBlockDescr
void bind_CfgBlockDescr(py::module_& m) {
	py::class_<CfgBlockDescr>(m, "CfgBlockDescr")
		.def(py::init<>())
		.def_readwrite("magic_word", &CfgBlockDescr::block_type)
		.def_readwrite("ver_maj", &CfgBlockDescr::block_offset)
		.def_readwrite("ver_min", &CfgBlockDescr::block_size);
};
struct CfgFileInfo
{
	UINT num_cfg_blocks;        // number of configuration blocks in file
	UINT mem_size;              // size of configuration data
	CfgBlockDescr cfg_blocks[];  // unsized array of descriptors;
};
// Функция для биндинга CfgFileInfo
void bind_CfgFileInfo(py::module_& m) {
	py::class_<CfgFileInfo>(m, "CfgFileInfo")
		.def(py::init<>())
		.def_readwrite("magic_word", &CfgFileInfo::num_cfg_blocks)
		.def_readwrite("ver_maj", &CfgFileInfo::mem_size);
};

//общая длина блока CfgFileInfo равна CfgFileHdr.info_size байт

////////////////////////////////////////////////////////////////
//далее записан буфер "configuration data" общей длиной равной CfgFileInfo.mem_size байт,
//буфер состоит из num_cfg_blocks блоков, каждый из которых характеризуется своим дескриптором CfgBlockDescr.
//в дескрипторе с block_type = X вы найдете значение block_offset - смещение начала блока X относительно начала буфера "configuration data"
//и значение block_size - размер блока hardware configuration.

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


struct ColorMod
{
	COLORREF	ch_color;
	BOOL		changed;
	UINT		reserved[16];

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 16 }, this->reserved);
	};
};
// Функция для биндинга ColorMod
void bind_ColorMod(py::module_& m) {
	py::class_<ColorMod>(m, "ColorMod")
		.def(py::init<>())
		.def_readwrite("ch_color", &ColorMod::ch_color)
		.def_readwrite("changed", &ColorMod::changed)
		.def("get_reserved", &ColorMod::get_reserved);
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
// Функция для биндинга ChanDefMod
void bind_ChanDefMod(py::module_& m) {
	py::class_<ChanDefMod>(m, "ChanDefMod")
		.def(py::init<>())
		.def_readwrite("enabled", &ChanDefMod::enabled)
		.def_readwrite("show", &ChanDefMod::show)
		.def_readwrite("high_filter", &ChanDefMod::high_filter)
		.def_readwrite("low_filter", &ChanDefMod::low_filter)
		.def_readwrite("gain_db", &ChanDefMod::gain_db)
		.def_readwrite("d_ADC", &ChanDefMod::d_ADC)
		.def_readwrite("tresh_db", &ChanDefMod::tresh_db)
		.def_readwrite("tresh_ADC", &ChanDefMod::tresh_ADC)
		.def_readwrite("changed", &ChanDefMod::changed)
		.def_readwrite("changed_ON", &ChanDefMod::changed_ON)
		.def_readwrite("autotresh", &ChanDefMod::autotresh)
		.def("get_reserved", &ChanDefMod::get_reserved);
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
// Функция для биндинга TimeDefMod
void bind_TimeDefMod(py::module_& m) {
	py::class_<TimeDefMod>(m, "TimeDefMod")
		.def(py::init<>())
		.def_readwrite("sceto", &TimeDefMod::sceto)
		.def_readwrite("rtto", &TimeDefMod::rtto)
		.def_readwrite("d_time", &TimeDefMod::d_time)
		.def_readwrite("max_dur", &TimeDefMod::max_dur)
		.def_readwrite("changed", &TimeDefMod::changed)
		.def("get_reserved", &TimeDefMod::get_reserved);
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
// Функция для биндинга CalibrDefMod
void bind_CalibrDefMod(py::module_& m) {
	py::class_<CalibrDefMod>(m, "CalibrDefMod")
		.def(py::init<>())
		.def_readwrite("enabled", &CalibrDefMod::enabled)
		.def_readwrite("K_calibr", &CalibrDefMod::K_calibr)
		.def_readwrite("d_DAC", &CalibrDefMod::d_DAC)
		.def_readwrite("Amp_v", &CalibrDefMod::Amp_v)
		.def_readwrite("Amp_DAC", &CalibrDefMod::Amp_DAC)
		.def_readwrite("Freq_Calibr", &CalibrDefMod::Freq_Calibr)
		.def_readwrite("changed", &CalibrDefMod::changed)
		.def_readwrite("changed_ON", &CalibrDefMod::changed_ON)
		.def_readwrite("mode", &CalibrDefMod::mode)
		.def_readwrite("Dur_DAC", &CalibrDefMod::Dur_DAC)
		.def_readwrite("F1", &CalibrDefMod::F1)
		.def_readwrite("F2", &CalibrDefMod::F2)
		.def_readwrite("Amin", &CalibrDefMod::Amin)
		.def_readwrite("Smin", &CalibrDefMod::Smin)
		.def_readwrite("Smax", &CalibrDefMod::Smax)
		.def_readwrite("PAEtype", &CalibrDefMod::PAEtype)
		.def("get_reserved", &CalibrDefMod::get_reserved);
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
// Функция для биндинга ParDefMod
void bind_ParDefMod(py::module_& m) {
	py::class_<ParDefMod>(m, "ParDefMod")
		.def(py::init<>())
		.def_readwrite("enabled", &ParDefMod::enabled)
		.def_readwrite("K_par", &ParDefMod::K_par)
		.def_readwrite("d_ADC_par", &ParDefMod::d_ADC_par)
		.def_readwrite("Freq_par", &ParDefMod::Freq_par)
		.def_readwrite("Freq_multi", &ParDefMod::Freq_multi)
		.def_readwrite("K1", &ParDefMod::K1)
		.def_readwrite("K2", &ParDefMod::K2)
		.def_readwrite("changed", &ParDefMod::changed)
		.def_readwrite("changed_ON", &ParDefMod::changed_ON)
		.def_readwrite("lMin", &ParDefMod::lMin)
		.def_readwrite("lMax", &ParDefMod::lMax)
		.def("get_reserved", &ParDefMod::get_reserved);
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

// Функция для биндинга ald_drv_addr_wrapper
void bind_ald_drv_addr_t(py::module_& m) {
	py::class_<ald_drv_addr_wrapper>(m, "ald_drv_addr_t")
		.def(py::init<>())
		.def_property("FAddr", &ald_drv_addr_wrapper::get_FAddr, &ald_drv_addr_wrapper::set_FAddr)
		.def_property("Channel", &ald_drv_addr_wrapper::get_Channel, &ald_drv_addr_wrapper::set_Channel)
		.def_property("Board", &ald_drv_addr_wrapper::get_Board, &ald_drv_addr_wrapper::set_Board);
};
//Структура hardware configuration block:
//со значением CFG_HARDWARE = 1 выступает hardware configuration block, который состоит из некоторого числа структур-описателей каналов:
//FullDefMod, число этих структур совпадает с числом инталлированных в системе каналов и теоретически неограничено
#define NUM_PAR_PER_MOD	4
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

// Функция для биндинга FullDefMod
void bind_FullDefMod(py::module_& m) {
	py::class_<FullDefMod>(m, "FullDefMod")
		.def(py::init<>())
		.def_readwrite("chan", &FullDefMod::chan)
		.def_readwrite("color_def", &FullDefMod::color_def)
		.def_readwrite("chan_def", &FullDefMod::chan_def)
		.def_readwrite("time_def", &FullDefMod::time_def)
		.def("get_osc_def", &FullDefMod::get_osc_def)
		.def_readwrite("calibr_def", &FullDefMod::calibr_def)
		.def("get_par_def", &FullDefMod::get_par_def)
		.def_readwrite("addr", &FullDefMod::addr)
		.def_readwrite("DeviceName", &FullDefMod::DeviceName)
		.def_readwrite("SubDeviceName", &FullDefMod::SubDeviceName)
		.def_readwrite("num_Rmod", &FullDefMod::num_Rmod)
		.def_readwrite("gpre", &FullDefMod::gpre)
		.def_readwrite("shift0", &FullDefMod::shift0)
		.def_readwrite("DeviceType", &FullDefMod::DeviceType)
		.def_readwrite("FilterType", &FullDefMod::FilterType)
		.def_readwrite("VersionModule", &FullDefMod::VersionModule)
		.def_readwrite("Capability", &FullDefMod::Capability)
		.def("get_reserved", &FullDefMod::get_reserved);
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
// Функция для биндинга MFData
void bind_MFData(py::module_& m) {
	py::class_<MFData>(m, "MFData")
		.def(py::init<>())
		.def_readwrite("rc", &MFData::rc)
		.def_readonly("chan", &MFData::wp);
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
// Функция для биндинга ScaleData
void bind_ScaleData(py::module_& m) {
	py::class_<ScaleData>(m, "ScaleData")
		.def(py::init<>())
		.def_readwrite("lMin", &ScaleData::lMin)
		.def_readwrite("lMax", &ScaleData::lMax)
		.def_readwrite("bMinAuto", &ScaleData::bMinAuto)
		.def_readwrite("bMaxAuto", &ScaleData::bMaxAuto)
		.def_readwrite("bIsShown", &ScaleData::bIsShown)
		.def_readwrite("bIsStPos", &ScaleData::bIsStPos)
		.def_readwrite("cBackColor", &ScaleData::cBackColor)
		.def_readwrite("cTextColor", &ScaleData::cTextColor)
		.def_readwrite("dwMajor", &ScaleData::dwMajor)
		.def_readwrite("dwMinor", &ScaleData::dwMinor)
		.def_readwrite("bMajorAuto", &ScaleData::bMajorAuto)
		.def_readwrite("bMinorAuto", &ScaleData::bMinorAuto)
		.def("get_label", &ScaleData::get_label)
		.def_readwrite("unit", &ScaleData::unit)
		.def_readwrite("lMin_mkV", &ScaleData::lMin_mkV)
		.def_readwrite("lMax_mkV", &ScaleData::lMax_mkV)
		.def_readwrite("bShowThresh", &ScaleData::bShowThresh)
		.def_readwrite("IsLog", &ScaleData::IsLog)
		.def_readwrite("bNormal", &ScaleData::bNormal)
		.def_readwrite("bMarker", &ScaleData::bMarker)
		.def_readwrite("bScroll", &ScaleData::bScroll)
		.def_readwrite("uScrollSize", &ScaleData::uScrollSize)
		.def("get_reserved", &ScaleData::get_reserved)
		.def_readwrite("bShowLabel", &ScaleData::bShowLabel);
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
// Функция для биндинга GraphData
void bind_GraphData(py::module_& m) {
	py::class_<GraphData>(m, "GraphData")
		.def(py::init<>())
		.def_readwrite("cBackColor", &GraphData::cBackColor)
		.def_readwrite("cIsLines", &GraphData::cIsLines)
		.def_readwrite("cIsGrid_x", &GraphData::cIsGrid_x)
		.def_readwrite("cIsGrid_y", &GraphData::cIsGrid_y)
		.def_readwrite("cColor_x", &GraphData::cColor_x)
		.def_readwrite("cColor_y", &GraphData::cColor_y)
		.def_readwrite("cIsGrid_clast", &GraphData::cIsGrid_clast)
		.def("get_reserved", &GraphData::get_reserved);
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
// Функция для биндинга AlarmData
void bind_AlarmData(py::module_& m) {
	py::class_<AlarmData>(m, "AlarmData")
		.def(py::init<>())
		.def("get_AlarmName", &AlarmData::get_AlarmName)
		.def("get_WaveName", &AlarmData::get_WaveName)
		.def("get_FltDef", &AlarmData::get_FltDef)
		.def("get_AndOr", &AlarmData::get_AndOr)
		.def_readwrite("buf", &AlarmData::buf)
		.def_readwrite("bMarkerSet", &AlarmData::bMarkerSet)
		.def_readwrite("bActivated", &AlarmData::bActivated)
		.def_readwrite("bSound", &AlarmData::bSound)
		.def_readwrite("bPicture", &AlarmData::bPicture)
		.def_readwrite("bPicOff", &AlarmData::bPicOff)
		.def_readwrite("bPrevMarkerON", &AlarmData::bPrevMarkerON)
		.def_readwrite("NumLocGroup", &AlarmData::NumLocGroup)
		.def_readwrite("NumLocDist", &AlarmData::NumLocDist)
		.def("get_reserved", &AlarmData::get_reserved);
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
// Функция для биндинга GeneralSetupMod
void bind_GeneralSetupMod(py::module_& m) {
	py::class_<GeneralSetupMod>(m, "GeneralSetupMod")
		.def(py::init<>())
		.def("get_name_par", &GeneralSetupMod::get_name_par)
		.def_readwrite("PageSize", &GeneralSetupMod::PageSize)
		.def_readwrite("Din", &GeneralSetupMod::Din)
		.def_readwrite("SingleNumber", &GeneralSetupMod::SingleNumber)
		.def("get_ChanSep", &GeneralSetupMod::get_ChanSep)
		.def("get_LocSep", &GeneralSetupMod::get_LocSep)
		.def_readwrite("AutoSaveCfg", &GeneralSetupMod::AutoSaveCfg)
		.def_readwrite("OSCAutoScale", &GeneralSetupMod::OSCAutoScale)
		.def_readwrite("SaveService", &GeneralSetupMod::SaveService)
		.def_readwrite("OnLineSpect", &GeneralSetupMod::OnLineSpect)
		.def_readwrite("UpgradeSpect", &GeneralSetupMod::UpgradeSpect)
		.def_readwrite("GenColor", &GeneralSetupMod::GenColor)
		.def_readwrite("ParReadCfg", &GeneralSetupMod::ParReadCfg)
		.def_readwrite("NoPauseOSC", &GeneralSetupMod::NoPauseOSC)
		.def_readwrite("RMSOff", &GeneralSetupMod::RMSOff)
		.def_readwrite("ProtOn", &GeneralSetupMod::ProtOn)
		.def_readwrite("LineThickness", &GeneralSetupMod::LineThickness)
		.def_readwrite("AnalizInfoMod", &GeneralSetupMod::AnalizInfoMod)
		.def_readwrite("WriteServiceInfo", &GeneralSetupMod::WriteServiceInfo)
		.def_readwrite("InfoTimeout", &GeneralSetupMod::InfoTimeout)
		.def_readwrite("StartTimeout", &GeneralSetupMod::StartTimeout)
		.def_readwrite("TempMonitor", &GeneralSetupMod::TempMonitor)
		.def_readwrite("UseComAlarm", &GeneralSetupMod::UseComAlarm)
		.def_readwrite("AlarmComPort", &GeneralSetupMod::AlarmComPort)
		.def_readwrite("SyncStatic", &GeneralSetupMod::SyncStatic)
		.def_readwrite("ManualCmd", &GeneralSetupMod::ManualCmd)
		.def_readwrite("ClbSyncOsc", &GeneralSetupMod::ClbSyncOsc)
		.def_readwrite("GetAllVer", &GeneralSetupMod::GetAllVer)
		.def("get_reserved", &GeneralSetupMod::get_reserved)
		.def_readwrite("Unit", &GeneralSetupMod::Unit)
		.def("get_ProtFileName", &GeneralSetupMod::get_ProtFileName)
		.def("get_name_reserv", &GeneralSetupMod::get_name_reserv);
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
// Функция для биндинга AutoThresholdSetup
void bind_AutoThresholdSetup(py::module_& m) {
	py::class_<AutoThresholdSetup>(m, "AutoThresholdSetup")
		.def(py::init<>())
		.def_readwrite("m_bAuto", &AutoThresholdSetup::m_bAuto)
		.def_readwrite("m_normal_delta", &AutoThresholdSetup::m_normal_delta)
		.def_readwrite("m_alarm_delta", &AutoThresholdSetup::m_alarm_delta)
		.def_readwrite("m_length", &AutoThresholdSetup::m_length)
		.def_readwrite("m_th_delta", &AutoThresholdSetup::m_th_delta)
		.def_readwrite("m_th_min_ADC", &AutoThresholdSetup::m_th_min_ADC)
		.def("get_reserved", &AutoThresholdSetup::get_reserved);
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
// Функция для биндинга HotKeyData
void bind_HotKeyData(py::module_& m) {
	py::class_<HotKeyData>(m, "HotKeyData")
		.def(py::init<>())
		.def_readwrite("m_wVK", &HotKeyData::m_wVK)
		.def_readwrite("m_wMod", &HotKeyData::m_wMod)
		.def_readwrite("m_nCmd", &HotKeyData::m_nCmd);
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
// Функция для биндинга CPostWin
void bind_CPostWin(py::module_& m) {
	py::class_<CPostWin>(m, "CPostWin")
		.def(py::init<>())
		.def("axis_x", &CPostWin::get_axis_x)
		.def("axis_y", &CPostWin::get_axis_y)
		.def("axis_z", &CPostWin::get_axis_z)
		.def("par_chan", &CPostWin::get_par_chan)
		.def("get_reserved", &CPostWin::get_reserved);
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
// Функция для биндинга Graph3DData
void bind_Graph3DData(py::module_& m) {
	py::class_<Graph3DData>(m, "Graph3DData")
		.def(py::init<>())
		.def_readwrite("cBackColor", &Graph3DData::cBackColor)
		.def_readwrite("cIsGrid_x", &Graph3DData::cIsGrid_x)
		.def_readwrite("cIsGrid_y", &Graph3DData::cIsGrid_y)
		.def_readwrite("cColor_x", &Graph3DData::cColor_x)
		.def_readwrite("cColor_y", &Graph3DData::cColor_y)
		.def_readwrite("cIsGrid_clast", &Graph3DData::cIsGrid_clast)
		.def_readwrite("cObjectColor", &Graph3DData::cObjectColor)
		.def_readwrite("cIsGrid_z", &Graph3DData::cIsGrid_z)
		.def("get_reserved", &Graph3DData::get_reserved);
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
// Функция для биндинга CHistWin
void bind_CHistWin(py::module_& m) {
	py::class_<CHistWin>(m, "CHistWin")
		.def(py::init<>())
		.def("axis_x", &CHistWin::get_axis_x)
		.def("axis_y", &CHistWin::get_axis_y)
		.def("step_x", &CHistWin::get_step_x)
		.def("step_y", &CHistWin::get_step_y)
		.def("type", &CHistWin::get_type)
		.def("bSummed", &CHistWin::get_bSummed)
		.def("get_reserved", &CHistWin::get_reserved);
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
// Функция для биндинга NetIPInfo
void bind_NetIPInfo(py::module_& m) {
	py::class_<NetIPInfo>(m, "NetIPInfo")
		.def(py::init<>())
		.def("get_IPAddress", &NetIPInfo::get_IPAddress)
		.def_readwrite("pID", &NetIPInfo::pID)
		.def_readwrite("bStatus", &NetIPInfo::bStatus)
		.def_readwrite("MinLineAddr", &NetIPInfo::MinLineAddr)
		.def_readwrite("TimeElapsed", &NetIPInfo::TimeElapsed)
		.def("get_LineNums", &NetIPInfo::get_LineNums)
		.def("get_LineAddr", &NetIPInfo::get_LineAddr)
		.def("get_reserved", &NetIPInfo::get_reserved);
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
// Функция для биндинга ClassAmpDef
void bind_ClassAmpDef(py::module_& m) {
	py::class_<ClassAmpDef>(m, "ClassAmpDef")
		.def(py::init<>())
		.def_readwrite("show_amp_crit", &ClassAmpDef::show_amp_crit)
		.def_readwrite("is_auto", &ClassAmpDef::is_auto)
		.def_readwrite("is_EN", &ClassAmpDef::is_EN)
		.def_readwrite("material", &ClassAmpDef::material)
		.def_readwrite("object", &ClassAmpDef::object)
		.def_readwrite("A1", &ClassAmpDef::A1)
		.def_readwrite("A2", &ClassAmpDef::A2)
		.def_readwrite("N1", &ClassAmpDef::N1)
		.def_readwrite("N2", &ClassAmpDef::N2)
		.def("get_reserved", &ClassAmpDef::get_reserved);
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
#define MAX_PAGENAMELENGTH	256
struct PageName
{
	char page_name[MAX_PAGENAMELENGTH];

	std::wstring get_page_name() {
		return get_wstr(this->page_name);
	}
};
// Функция для биндинга PageName
void bind_PageName(py::module_& m) {
	py::class_<PageName>(m, "PageName")
		.def(py::init<>())
		.def("get_page_name", &PageName::get_page_name);
};
struct PageData
{
	UINT mem_size;		// size of page configuration
	UINT num_pages;		// number of pages in file
	UINT cur_page;		// currently selectde page
	PageName name[];	// unsized array of page names;

	std::vector<PageName> get_name() {
		return get_vector_from_arr<PageName>(this->name, 0);
	}
};
// Функция для биндинга PageData
void bind_PageData(py::module_& m) {
	py::class_<PageData>(m, "PageData")
		.def(py::init<>())
		.def_readwrite("mem_size", &PageData::mem_size)
		.def_readwrite("num_pages", &PageData::num_pages)
		.def_readwrite("cur_page", &PageData::cur_page)
		.def("get_name", &PageData::get_name);
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
// Функция для биндинга ILData
void bind_ILData(py::module_& m) {
	py::class_<ILData>(m, "ILData")
		.def(py::init<>())
		.def_readwrite("enable_import", &ILData::enable_import)
		.def_readwrite("ParInput", &ILData::ParInput)
		.def_readwrite("ParChanNum", &ILData::ParChanNum)
		.def_readwrite("ParModNum", &ILData::ParModNum)
		.def("get_ParName", &ILData::get_ParName)
		.def("get_FileName", &ILData::get_FileName)
		.def_readwrite("thickness", &ILData::thickness)
		.def_readwrite("color", &ILData::color)
		.def_readwrite("NumFraction", &ILData::NumFraction);
};


//////////////////////////////////////////////////
//конфигурационный блок CFG_SERVICE содержит информацию о настройках системы отображения служебных диагностических данных аппаратуры
//размер блока CFG_SERVICE = NUM_DEVICES * NUM_TYPE * NUM_SERV * sizeof( ServDefMod )
#define NUM_TYPE	2
#define NUM_SERV	12
//NUM_DEVICES - число контроллеров линии (входов DDM-2)
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
// Функция для биндинга ServDefMod
void bind_ServDefMod(py::module_& m) {
	py::class_<ServDefMod>(m, "ServDefMod")
		.def(py::init<>())
		.def_readwrite("show", &ServDefMod::show)
		.def_readwrite("color", &ServDefMod::color)
		.def_readwrite("K1", &ServDefMod::K1)
		.def_readwrite("K2", &ServDefMod::K2)
		.def_readwrite("show_text", &ServDefMod::show_text)
		.def("get_reserved", &ServDefMod::get_reserved);
};



////////////////////////////////////////////////////////////////////////
//Struct member alignment установлено по умолчанию в MSVisualC++, т.е. равным 8 байт

///////////////THE END//////////THE END////////THE END/////////////////
