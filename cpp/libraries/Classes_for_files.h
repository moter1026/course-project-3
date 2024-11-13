#include <logger.h>

#include "afxpriv.h"
#include <time.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <cwchar>    // ��� ������� mbstowcs
#include <cstring>   // ��� strlen
#include <string>

#pragma once


namespace py = pybind11;

#define MAX_FDEFS 4

std::wstring get_wstr(char* l_str) {
	std::setlocale(LC_ALL, "Russian_Russia.1251");
	// ���������� ����� ����� ������ � �������� ������ ��� wchar_t ������
	size_t len = std::strlen(l_str) + 1;
	wchar_t* wchar_str = new wchar_t[len];
	// ����������� �� ������������� ������ � ������� ������
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

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 18 }, reserved);
	}
};

// ������� ��� �������� FileHdr
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

// ������� ��� �������� FilterDef
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

// ������� ��� �������� FilterDef
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

// ������� ��� �������� MeasData
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

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 16 }, reserved);
	};
};

// ������� ��� �������� SDOHdr
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
// ����� ������� ���� ������������ � ������� ��� ������ ��������������� ���� ������ (��. "������ ����� cfg 6.0.cpp")
////////////////////////////////////////////////////////////////


typedef unsigned long ulong;
struct AE_TIME
{
	__time32_t	seconds;	// absolute seconds value suitable for C time functions
	ulong	usec;			// microseconds since last second
};

// ������� ��� �������� AE_TIME
void bind_AE_TIME(py::module_& m) {
	py::class_<AE_TIME>(m, "AE_TIME")
		.def(py::init<>())
		.def_readwrite("seconds", &AE_TIME::seconds)
		.def_readwrite("usec", &AE_TIME::usec);
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

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 2 }, this->reserved);
	};
};

// ������� ��� �������� OSCDefMod
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

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 2 }, this->reserved);
	};
};

// ������� ��� �������� CfgFileHdr
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
//���������������� ����-�����
struct CfgBlockDescr
{
	UINT block_type;            // block type;
	UINT block_offset;          // offset to the block in bytes from 
	// cfg file info structure
	UINT block_size;            // cfg block size
};
// ������� ��� �������� CfgBlockDescr
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
// ������� ��� �������� CfgFileInfo
void bind_CfgFileInfo(py::module_& m) {
	py::class_<CfgFileInfo>(m, "CfgFileInfo")
		.def(py::init<>())
		.def_readwrite("magic_word", &CfgFileInfo::num_cfg_blocks)
		.def_readwrite("ver_maj", &CfgFileInfo::mem_size);
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

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 16 }, this->reserved);
	};
};
// ������� ��� �������� ColorMod
void bind_ColorMod(py::module_& m) {
	py::class_<ColorMod>(m, "ColorMod")
		.def(py::init<>())
		.def_readwrite("ch_color", &ColorMod::ch_color)
		.def_readwrite("changed", &ColorMod::changed)
		.def("get_reserved", &ColorMod::get_reserved);
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

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 14 }, this->reserved);
	};
};
// ������� ��� �������� ChanDefMod
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
	UINT		sceto;			//��� (SCETO)
	UINT		rtto;			//RTTO - �� ������������ (������ ����� max_dur)
	UINT		d_time;			//������� �����
	UINT		max_dur;		//������������ ������������
	BOOL		changed;		//���� ����� ������������� ���������
	UINT		reserved[16];

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 16 }, this->reserved);
	};
};
// ������� ��� �������� TimeDefMod
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

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 7 }, this->reserved);
	};
};
// ������� ��� �������� CalibrDefMod
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

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 14 }, this->reserved);
	};
};
// ������� ��� �������� ParDefMod
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
		WORD	Channel;					// ����� ������
		WORD	Board;						// ����� �����������
	} bc;
};
// ������� ��� union
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

// ������� ��� �������� ald_drv_addr_wrapper
void bind_ald_drv_addr_t(py::module_& m) {
	py::class_<ald_drv_addr_wrapper>(m, "ald_drv_addr_t")
		.def(py::init<>())
		.def_property("FAddr", &ald_drv_addr_wrapper::get_FAddr, &ald_drv_addr_wrapper::set_FAddr)
		.def_property("Channel", &ald_drv_addr_wrapper::get_Channel, &ald_drv_addr_wrapper::set_Channel)
		.def_property("Board", &ald_drv_addr_wrapper::get_Board, &ald_drv_addr_wrapper::set_Board);
};
//��������� hardware configuration block:
//�� ��������� CFG_HARDWARE = 1 ��������� hardware configuration block, ������� ������� �� ���������� ����� ��������-���������� �������:
//FullDefMod, ����� ���� �������� ��������� � ������ ��������������� � ������� ������� � ������������ ������������
#define NUM_PAR_PER_MOD	4
struct FullDefMod
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

// ������� ��� �������� FullDefMod
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
//���������������� ����� ��������� ��������� ���� XXX ������ ��� ���������� ��������� � ������� ���� XXX �� ��������� �����������

//��� �������� ���� ��������� CFG_MAINFRAME �������� ��������� ���� MFData 
//������ ����� CFG_MAINFRAME = sizeof ( MFData )
struct MFData
{
	RECT rc;                // screen coordinates
	WINDOWPLACEMENT wp;     // window placement structure
};
// ������� ��� �������� MFData
void bind_MFData(py::module_& m) {
	py::class_<MFData>(m, "MFData")
		.def(py::init<>())
		.def_readwrite("rc", &MFData::rc)
		.def_readonly("chan", &MFData::wp);
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

	std::wstring get_label() {
		return get_wstr(this->label);
	}
	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 2 }, this->reserved);
	};
};
// ������� ��� �������� ScaleData
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
	COLORREF	cBackColor;		//���� ����
	BOOL		cIsLines;		//���� ����������� �����/�����
	BOOL		cIsGrid_x;		//����������� ��� X
	BOOL		cIsGrid_y;		//����������� ��� Y
	COLORREF    cColor_x;		//���� �������.
	COLORREF    cColor_y;		//���� �������.
	BOOL		cIsGrid_clast;	//����������� �����
	BYTE		reserved[24];

	py::array_t<BYTE> get_reserved() {
		return py::array_t<BYTE>({ 24 }, this->reserved);
	};
};
// ������� ��� �������� GraphData
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
// ������� ��� �������� AlarmData
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
// ������� ��� �������� GeneralSetupMod
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

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 16 }, this->reserved);
	};
};
// ������� ��� �������� AutoThresholdSetup
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
// ������� ��� �������� HotKeyData
void bind_HotKeyData(py::module_& m) {
	py::class_<HotKeyData>(m, "HotKeyData")
		.def(py::init<>())
		.def_readwrite("m_wVK", &HotKeyData::m_wVK)
		.def_readwrite("m_wMod", &HotKeyData::m_wMod)
		.def_readwrite("m_nCmd", &HotKeyData::m_nCmd);
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

public:
	int get_axis_x() { return this->axis_x; };
	int get_axis_y() { return this->axis_y; };
	int get_axis_z() { return this->axis_z; };
	DWORD get_par_chan() { return this->par_chan; };
	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 8 }, this->reserved);
	};
};
// ������� ��� �������� CPostWin
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
	COLORREF	cBackColor;		//���� ����
	BOOL		cIsGrid_x;		//����������� ��� X
	BOOL		cIsGrid_y;		//����������� ��� Y
	COLORREF    cColor_x;		//���� ������
	COLORREF    cColor_y;		//���� ������
	BOOL		cIsGrid_clast;	//����������� �����
	COLORREF	cObjectColor;	//���� ����	
	BOOL		cIsGrid_z;		//����������� ��� Z
	BYTE		reserved[20];

	py::array_t<BYTE> get_reserved() {
		return py::array_t<BYTE>({ 20 }, this->reserved);
	};
};
// ������� ��� �������� Graph3DData
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
// ������� ��� �������� CHistWin
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
// ������� ��� �������� NetIPInfo
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

	std::vector<UINT> get_reserved() {
		return get_vector_from_arr<UINT>(this->reserved, 10);
	};
};
// ������� ��� �������� ClassAmpDef
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

	std::wstring get_page_name() {
		return get_wstr(this->page_name);
	}
};
// ������� ��� �������� PageName
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
// ������� ��� �������� PageData
void bind_PageData(py::module_& m) {
	py::class_<PageData>(m, "PageData")
		.def(py::init<>())
		.def_readwrite("mem_size", &PageData::mem_size)
		.def_readwrite("num_pages", &PageData::num_pages)
		.def_readwrite("cur_page", &PageData::cur_page)
		.def("get_name", &PageData::get_name);
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

	std::wstring get_ParName() { return get_wstr(this->ParName); };
	std::wstring get_FileName() { return get_wstr(this->FileName); };
};
// ������� ��� �������� ILData
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

	std::vector<UINT> get_reserved() {
		return get_vector_from_arr<UINT>(this->reserved, 5);
	};
};
// ������� ��� �������� ServDefMod
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
//Struct member alignment ����������� �� ��������� � MSVisualC++, �.�. ������ 8 ����

///////////////THE END//////////THE END////////THE END/////////////////
