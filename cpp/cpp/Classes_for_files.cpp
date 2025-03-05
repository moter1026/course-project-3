#include <Classes_for_files.h>


std::string wstring_to_ansi(const std::wstring& wstr) {
	setlocale(LC_ALL, "Russian");
	try {
		std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>, wchar_t> converter(new std::codecvt<wchar_t, char, std::mbstate_t>(""));
		return converter.to_bytes(wstr);
	}
	catch (const std::range_error& e) {
		std::cerr << "Ошибка конвертации в ANSI: " << e.what() << std::endl;
		return "";
	}
}

std::string wstring_to_string(const std::wstring& wstr) {
	setlocale(LC_ALL, "Russian");
	try {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(wstr);
	}
	catch (const std::range_error& e) {
		std::cerr << "Ошибка конвертации в string: " << e.what() << std::endl;
		return "";
	}
}

std::wstring string_to_wstring(const std::string& str) {
	setlocale(LC_ALL, "Russian");
	try {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes(str);
	}
	catch (const std::range_error& e) {
		std::cerr << "Ошибка конвертации в wstring: " << e.what() << std::endl;
		return L"";
	}
}

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
	auto size = static_cast<size_t>(size_s);
	std::unique_ptr<char[]> buf(new char[size]);
	std::snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

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
// Функция для биндинга AE_TIME
void bind_AE_TIME(py::module_& m) {
	py::class_<AE_TIME>(m, "AE_TIME")
		.def(py::init<>())
		.def_readwrite("seconds", &AE_TIME::seconds)
		.def_readwrite("usec", &AE_TIME::usec);
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
// Функция для биндинга CfgBlockDescr
void bind_CfgBlockDescr(py::module_& m) {
	py::class_<CfgBlockDescr>(m, "CfgBlockDescr")
		.def(py::init<>())
		.def_readwrite("magic_word", &CfgBlockDescr::block_type)
		.def_readwrite("ver_maj", &CfgBlockDescr::block_offset)
		.def_readwrite("ver_min", &CfgBlockDescr::block_size);
};
// Функция для биндинга CfgFileInfo
void bind_CfgFileInfo(py::module_& m) {
	py::class_<CfgFileInfo>(m, "CfgFileInfo")
		.def(py::init<>())
		.def_readwrite("magic_word", &CfgFileInfo::num_cfg_blocks)
		.def_readwrite("ver_maj", &CfgFileInfo::mem_size);
};
// Функция для биндинга ColorMod
void bind_ColorMod(py::module_& m) {
	py::class_<ColorMod>(m, "ColorMod")
		.def(py::init<>())
		.def_readwrite("ch_color", &ColorMod::ch_color)
		.def_readwrite("changed", &ColorMod::changed)
		.def("get_reserved", &ColorMod::get_reserved);
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
// Функция для биндинга ald_drv_addr_wrapper
void bind_ald_drv_addr_t(py::module_& m) {
	py::class_<ald_drv_addr_wrapper>(m, "ald_drv_addr_t")
		.def(py::init<>())
		.def_property("FAddr", &ald_drv_addr_wrapper::get_FAddr, &ald_drv_addr_wrapper::set_FAddr)
		.def_property("Channel", &ald_drv_addr_wrapper::get_Channel, &ald_drv_addr_wrapper::set_Channel)
		.def_property("Board", &ald_drv_addr_wrapper::get_Board, &ald_drv_addr_wrapper::set_Board);
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
// Функция для биндинга MFData
void bind_MFData(py::module_& m) {
	py::class_<MFData>(m, "MFData")
		.def(py::init<>())
		.def_readwrite("rc", &MFData::rc)
		.def_readonly("chan", &MFData::wp);
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
// Функция для биндинга HotKeyData
void bind_HotKeyData(py::module_& m) {
	py::class_<HotKeyData>(m, "HotKeyData")
		.def(py::init<>())
		.def_readwrite("m_wVK", &HotKeyData::m_wVK)
		.def_readwrite("m_wMod", &HotKeyData::m_wMod)
		.def_readwrite("m_nCmd", &HotKeyData::m_nCmd);
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
// Функция для биндинга PageName
void bind_PageName(py::module_& m) {
	py::class_<PageName>(m, "PageName")
		.def(py::init<>())
		.def("get_page_name", &PageName::get_page_name);
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
// Функция для биндинга EV_DATA_OUT_OLD
void bind_EV_DATA_OUT_OLD(py::module_& m) {
	py::class_<EV_DATA_OUT_OLD>(m, "EV_DATA_OUT_OLD")
		.def(py::init<>())
		.def_readwrite("ev_time", &EV_DATA_OUT_OLD::ev_time)
		.def_readwrite("chan_num", &EV_DATA_OUT_OLD::chan_num)
		.def_readwrite("max_ampl", &EV_DATA_OUT_OLD::max_ampl)
		.def_readwrite("duration", &EV_DATA_OUT_OLD::duration)
		.def_readwrite("rise_time", &EV_DATA_OUT_OLD::rise_time)
		.def_readwrite("tig", &EV_DATA_OUT_OLD::tig)
		.def_readwrite("energy_low", &EV_DATA_OUT_OLD::energy_low)
		.def_readwrite("energy_medium", &EV_DATA_OUT_OLD::energy_medium)
		.def_readwrite("energy_high", &EV_DATA_OUT_OLD::energy_high)
		.def_readwrite("flags", &EV_DATA_OUT_OLD::flags)
		.def_readwrite("loc_event", &EV_DATA_OUT_OLD::loc_event)
		.def_readwrite("locX", &EV_DATA_OUT_OLD::locX)
		.def_readwrite("locY", &EV_DATA_OUT_OLD::locY);
};
// Функция для биндинга CH_INFO_OUT
void bind_CH_INFO_OUT(py::module_& m) {
	py::class_<CH_INFO_OUT>(m, "CH_INFO_OUT")
		.def(py::init<>())
		.def_readwrite("chan_info_time", &CH_INFO_OUT::chan_info_time)
		.def_readwrite("chan_num", &CH_INFO_OUT::chan_num)
		.def_readwrite("type", &CH_INFO_OUT::type)
		.def("get_data", &CH_INFO_OUT::get_data);
};
// Функция для биндинга EV_DATA_OUT
void bind_EV_DATA_OUT(py::module_& m) {
	py::class_<EV_DATA_OUT>(m, "EV_DATA_OUT")
		.def(py::init<>())
		.def_readwrite("ev_time", &EV_DATA_OUT::ev_time)
		.def_readwrite("chan_num", &EV_DATA_OUT::chan_num)
		.def_readwrite("max_ampl", &EV_DATA_OUT::max_ampl)
		.def_readwrite("duration", &EV_DATA_OUT::duration)
		.def_readwrite("expnd", &EV_DATA_OUT::expnd)
		.def_readwrite("rise_time", &EV_DATA_OUT::rise_time)
		.def_readwrite("flags_add", &EV_DATA_OUT::flags_add)
		.def_readwrite("tig", &EV_DATA_OUT::tig)
		.def_readwrite("attenuation", &EV_DATA_OUT::attenuation)
		.def_readwrite("energy_low", &EV_DATA_OUT::energy_low)
		.def_readwrite("energy_medium", &EV_DATA_OUT::energy_medium)
		.def_readwrite("energy_high", &EV_DATA_OUT::energy_high)
		.def_readwrite("flags", &EV_DATA_OUT::flags)
		.def_readwrite("tad", &EV_DATA_OUT::tad)
		.def_readwrite("loc_event", &EV_DATA_OUT::loc_event)
		.def_readwrite("nsec", &EV_DATA_OUT::nsec)
		.def_readwrite("locX", &EV_DATA_OUT::locX)
		.def_readwrite("locY", &EV_DATA_OUT::locY)
		.def_readwrite("locZ", &EV_DATA_OUT::locZ)
		.def_readwrite("gain", &EV_DATA_OUT::gain);
};
//расчет правильных 18-битных величин параметров АЭ-импульсов с учетом слова дополнительных разрядов выполнять так:
//	max_ampl_full	= ( DWORD ( EV_DATA_OUT.expnd & 0x08 ) << 13 ) + EV_DATA_OUT.max_ampl;
//  tig_full		= ( DWORD ( EV_DATA_OUT.expnd & 0x80 ) << 9 ) + EV_DATA_OUT.tig;
//	duration_full	= ExtractTimeQuant ( EV_DATA_OUT.expnd, 10 ) * (( DWORD ( EV_DATA_OUT.expnd & 0x07 ) << 16 ) + EV_DATA_OUT.duration );
//	rise_time_full	= ExtractTimeQuant ( EV_DATA_OUT.expnd, 10 ) * (( DWORD ( EV_DATA_OUT.expnd & 0x70 ) << 12 ) + EV_DATA_OUT.rise_time );
double ExtractTimeQuant(WORD expnd, int shift)
{
	switch ((expnd >> shift) & 0x07)
	{
	case 2:	return 2.;
	case 4:	return 0.5;
	case 6:	return 0.25;
	}
	return 1.;
}
//	energy_full		= ConvertEnergy ( &EV_DATA_OUT );
double ConvertEnergy(EV_DATA_OUT* data)
{
	DWORD tmp;
	if (data->expnd)
	{
		tmp = MAKELONG(data->energy_medium, data->energy_high);
		return double(_int64(tmp >> 5) << WORD(tmp & 0x1F));
	}
	tmp = MAKELONG(data->energy_low, data->energy_medium);
	return double(tmp) + (data->energy_high) * pow(2., 32);
}
// flags-флаги по переполнению АЦП (А), макс. длительности (D) по данному импульсу
// EV_DATA_OUT.flags & 0x01?"A":" - ";
// EV_DATA_OUT.flags & 0x04?"D":" - ";
// локационная амплитуда LA = int(( EV_DATA_OUT.flags & 0xFFF0 ) >> 4);	
// флаг полярности первого вступления ("+" - порог превышен в положительную полуволну колебаний, "-" - в отрицательную) - только для expnd != 0
// EV_DATA_OUT.flags & 0x08? "-":"+"
// флаг полярности максимума ("+" - максимум достигнут в положительную полуволну колебаний, "-" - в отрицательную) - только для expnd != 0
// EV_DATA_OUT.flags_add & 0x01? "-":"+"
// Функция для биндинга EV_PARAM_OUT
void bind_EV_PARAM_OUT(py::module_& m) {
	py::class_<EV_PARAM_OUT>(m, "EV_PARAM_OUT")
		.def(py::init<>())
		.def_readwrite("param_time", &EV_PARAM_OUT::param_time)
		.def_readwrite("chan_num", &EV_PARAM_OUT::chan_num)
		.def_readwrite("num_summ", &EV_PARAM_OUT::num_summ)
		.def("get_par_summ", &EV_PARAM_OUT::get_par_summ);
};
// значение величины параметрических измерений выводится после пересчета по формуле
// V[i] = ( par_summ[i] / num_summ ) * K1[i] + K2[i], где i-номер параметрического входа, К1[i]-мультипликативный, К2[i]-аддитивный коэффициенты пересчета из ParDefMod (см. формат файла конфигурации)

// Функция для биндинга EV_AVG_OUT
void bind_EV_AVG_OUT(py::module_& m) {
	py::class_<EV_AVG_OUT>(m, "EV_AVG_OUT")
		.def(py::init<>())
		.def_readwrite("avg_time", &EV_AVG_OUT::avg_time)
		.def_readwrite("chan_num", &EV_AVG_OUT::chan_num)
		.def_readwrite("num_summ", &EV_AVG_OUT::num_summ)
		.def_readwrite("noise_summ_ampl", &EV_AVG_OUT::noise_summ_ampl)
		.def_readwrite("intensity", &EV_AVG_OUT::intensity)
		.def_readwrite("lost", &EV_AVG_OUT::lost)
		.def_readwrite("summ_energy", &EV_AVG_OUT::summ_energy)
		.def_readwrite("summ_dur", &EV_AVG_OUT::summ_dur)
		.def_readwrite("summ_ampl", &EV_AVG_OUT::summ_ampl)
		.def_readwrite("summ_rise", &EV_AVG_OUT::summ_rise)
		.def_readwrite("summ_tig", &EV_AVG_OUT::summ_tig)
		.def_readwrite("max_ampl", &EV_AVG_OUT::max_ampl)
		.def_readwrite("expndw", &EV_AVG_OUT::expndw)
		.def_readwrite("cur_gain_dB", &EV_AVG_OUT::cur_gain_dB)
		.def_readwrite("expndb", &EV_AVG_OUT::expndb)
		.def("get_reserved", &EV_AVG_OUT::get_reserved);
};

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
// Функция для биндинга MARKER_OUT
void bind_MARKER_OUT(py::module_& m) {
	py::class_<MARKER_OUT>(m, "MARKER_OUT")
		.def(py::init<>())
		.def_readwrite("marker_time", &MARKER_OUT::marker_time)
		.def_readwrite("markerID", &MARKER_OUT::markerID);
};
// Функция для биндинга TEXT_OUT
void bind_TEXT_OUT(py::module_& m) {
	py::class_<TEXT_OUT>(m, "TEXT_OUT")
		.def(py::init<>())
		.def_readwrite("markerID", &TEXT_OUT::markerID)
		.def("get_p_name", &TEXT_OUT::get_p_name);
};
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

