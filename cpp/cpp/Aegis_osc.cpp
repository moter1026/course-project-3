#pragma once
#include <file_ald.h>
#include <file_cfg.h>
#include <file_osc.h>
#include <logger.h>

PYBIND11_MODULE(Aegis_osc, m) {
	py::class_<File_osc>(m, "File_osc")
		.def(py::init<std::wstring, std::shared_ptr<Logger>>())
		.def("readOsc", &File_osc::readOsc, "����� ������ �� ����� fileName ������ �� ��������������")
		.def("getDotOSC", &File_osc::getDotOSC, "����� ���������� ����� �������������", py::arg("numOSC"))
		.def("getDotOSC_NOC", &File_osc::getDotOSC_NOC, "����� ���������� ����� �������������,\
			 ��������� ��� �������� �� ����� ����", py::arg("numOSC"))
		.def("getDotsOSC", &File_osc::getDotsOSC, "����� ���������� ������ ����� ������������", py::arg("startOSC"), py::arg("endOSC"))
		.def("get_K_mkV", &File_osc::get_K_mkV, "����� ���������� ������ �������� K_mkV", py::arg("start"), py::arg("end"))
		.def("saveOSC", &File_osc::saveOSC, "����� ��������� ����� ���� �������������", py::arg("fileName"), py::arg("arr_osc"), py::arg("indexes"))
		.def("openReadFile", &File_osc::openReadFile, "��������� ��� ������ ����, ������� ��� ������ ��� �������� �������,\
			� �� ��������� ���� �� ����� ������ close()")
		.def("cls", &File_osc::cls, "��������� ����, ������� ��� ������ ��� �������� �������")
		.def_readwrite("fileHdr", &File_osc::fileHdr)
		.def_readwrite("measData", &File_osc::measData)
		.def_readwrite("sdoHdr", &File_osc::sdoHdr)
		.def_readwrite("cfgFileHdr", &File_osc::cfgFileHdr)
		.def_readwrite("cfgFileInfo", &File_osc::cfgFileInfo)
		.def_readwrite("cf_memory", &File_osc::cf_memory)
		.def_readwrite("oscDefMod", &File_osc::oscDefMod);

	py::class_<File_ald>(m, "File_ald")
		.def(py::init<std::wstring, std::shared_ptr<Logger>>())
		.def(py::init<File_ald&>())
		.def("openReadFile", &File_ald::openReadFile, "��������� ��� ������ ����, ������� ��� ������ ��� �������� �������,\
			� �� ��������� ���� �� ����� ������ close()")
		.def("saveNewAld", &File_ald::saveNewAld
			, "����� ��������� ����� ���� ald, � ������� � �������� ���������� � ��� ����� osc ������ ��������� � �������� ��������� ����."
			, py::arg("newNameOSC"))
		.def("readAld", &File_ald::readAld, "����� ������ �� ����� ald ������.")
		.def("cls", &File_ald::cls, "��������� ����, ������� ��� ������ ��� �������� �������")
		.def_readwrite("fileName", &File_ald::fileName)
		.def_readwrite("fileHdr", &File_ald::fileHdr)
		.def_readwrite("measData", &File_ald::measData)
		.def_readwrite("cfg", &File_ald::cfg)
		.def_readwrite("evData", &File_ald::evData)
		.def_readwrite("noiseData", &File_ald::noiseData)
		.def_readwrite("unisAvData", &File_ald::unisAvData)
		.def_readwrite("paramData", &File_ald::paramData)
		.def_readwrite("unisParamData", &File_ald::unisParamData)
		.def_readwrite("sysData", &File_ald::sysData)
		.def_readwrite("markerData", &File_ald::markerData)
		.def_readwrite("textData", &File_ald::textData)
		.def_readwrite("servData", &File_ald::servData)
		.def_readwrite("radioData", &File_ald::radioData);

	py::class_<Cfg_in_ald>(m, "Cfg_in_ald")
		.def(py::init<std::shared_ptr<std::ifstream>, std::shared_ptr<Logger>>())
		.def(py::init<Cfg_in_ald&>())
		.def("checkOpen", &Cfg_in_ald::checkOpen, "��������� ��� ������ ����, ������� ��� ������ ��� �������� �������,\
			� �� ��������� ���� �� ����� ������ close()")
		.def("readCfg", &Cfg_in_ald::readCfg, "����� ������ �� ����� cfg ������.")
		.def("cls", &Cfg_in_ald::cls, "��������� ����, ������� ��� ������ ��� �������� �������");


	py::class_<Logger, std::shared_ptr<Logger>>(m, "Logger")
		.def(py::init<std::wstring>())
		.def("logg", static_cast<void (Logger::*)(LogLevel, const string&, const char*, int, const char*)>(&Logger::logg)
			, "������ ���.\n level: ������� ���������\n message: ��������� ����\n file: ����, � ������� ��������� ��������� ����\n line: ������ � �����\n func: �������, � ������� ��������� ��������� ����"
			, py::arg("level"), py::arg("message")
			, py::arg("file"), py::arg("line"), py::arg("func"))
		.def("logg", static_cast<void (Logger::*)(LogLevel, const wstring&, const wstring&, int, const wstring&)>(&Logger::logg)
			, "������ ���.\n level: ������� ���������\n message: ��������� ����\n file: ����, � ������� ��������� ��������� ����\n line: ������ � �����\n func: �������, � ������� ��������� ��������� ����"
			, py::arg("level"), py::arg("message")
			, py::arg("file"), py::arg("line"), py::arg("func"))
		.def("logg", static_cast<void (Logger::*)(LogLevel, const char*, const char*, int, const char*)>(&Logger::logg)
			, "������ ���.\n level: ������� ���������\n message: ��������� ����\n file: ����, � ������� ��������� ��������� ����\n line: ������ � �����\n func: �������, � ������� ��������� ��������� ����"
			, py::arg("level"), py::arg("message")
			, py::arg("file"), py::arg("line"), py::arg("func"));

	py::enum_<LogLevel>(m, "LogLevel")
		.value("_DEBUG_", LogLevel::_DEBUG_)
		.value("_INFO_", LogLevel::_INFO_)
		.value("_WARNING_", LogLevel::_WARNING_)
		.value("_ERROR_", LogLevel::_ERROR_)
		.value("_CRITICAL_", LogLevel::_CRITICAL_)
		.export_values();


	bind_FileHdr(m);
	bind_FilterDef(m);
	bind_PreFilterData(m);
	bind_MeasData(m);
	bind_SDOHdr(m);
	bind_AE_TIME(m);
	bind_OSCDefMod(m);
	bind_CfgFileHdr(m);
	bind_CfgFileInfo(m);
	bind_ColorMod(m);
	bind_ChanDefMod(m);
	bind_TimeDefMod(m);
	bind_CalibrDefMod(m);
	bind_ParDefMod(m);
	bind_ald_drv_addr_t(m);
	bind_FullDefMod(m);
	bind_MFData(m);
	bind_ScaleData(m);
	bind_GraphData(m);
	bind_AlarmData(m);
	bind_GeneralSetupMod(m);
	bind_AutoThresholdSetup(m);
	bind_HotKeyData(m);
	bind_CPostWin(m);
	bind_Graph3DData(m);
	bind_CHistWin(m);
	bind_NetIPInfo(m);
	bind_ClassAmpDef(m);
	bind_PageData(m);
	bind_ILData(m);
	bind_ServDefMod(m);
	bind_EV_DATA_OUT_OLD(m);
	bind_CH_INFO_OUT(m);
	bind_EV_DATA_OUT(m);
	bind_EV_PARAM_OUT(m);
	bind_EV_AVG_OUT(m);
	bind_MARKER_OUT(m);
	bind_TEXT_OUT(m);
}