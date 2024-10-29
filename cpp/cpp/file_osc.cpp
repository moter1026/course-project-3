#include "file_osc.h"

bool File_osc::readOsc(std::wstring fileName) {
	std::setlocale(LC_ALL, "Russian_Russia.1251");

	std::ifstream file(fileName, std::ios::binary);
	if (file)
	{
		m_fileHdr = new FileHdr();
		if (!m_fileHdr) AfxMessageBox("Ошибка при выделении памяти.");
		file.read(reinterpret_cast<char*>(m_fileHdr), sizeof(FileHdr));

		m_measData = new MeasData();
		if (!m_measData) AfxMessageBox("Ошибка при выделении памяти.");
		file.read(reinterpret_cast<char*>(m_measData), sizeof(MeasData));

		m_sdoHdr = new SDOHdr();
		if (!m_sdoHdr) AfxMessageBox("Ошибка при выделении памяти.");
		file.read(reinterpret_cast<char*>(m_sdoHdr), sizeof(SDOHdr));

		m_cfgFileHdr = new CfgFileHdr();
		if (!m_cfgFileHdr) AfxMessageBox("Ошибка при выделении памяти.");
		file.read(reinterpret_cast<char*>(m_cfgFileHdr), sizeof(CfgFileHdr));

		m_cfgFileInfo = (CfgFileInfo*)malloc(m_cfgFileHdr->info_size);
		if (!m_cfgFileInfo) AfxMessageBox("Ошибка при выделении памяти.");
		file.read(reinterpret_cast<char*>(m_cfgFileInfo), m_cfgFileHdr->info_size);

		cf_memory = malloc(m_cfgFileInfo->mem_size);
		if (!cf_memory) AfxMessageBox("Ошибка при выделении памяти.");
		file.read(reinterpret_cast<char*>(cf_memory), m_cfgFileInfo->mem_size);

		m_oscDefMod = new OSCDefMod[m_sdoHdr->NumOSC]();
		if (!m_oscDefMod) return false;
		for (size_t i = 0; i < m_sdoHdr->NumOSC; i++)
		{
			file.read((char*)(&m_oscDefMod[i]), sizeof(OSCDefMod));
			file.seekg((m_oscDefMod[i].buf_size << 1), std::ios::cur);
		}

	}
	else
	{
		throw std::domain_error(_T("не удалось открыть файл для чтения."));
		return false;
	}

	if (file)	file.close();
	return true;
}

File_osc::~File_osc() {
	if (m_fileHdr) delete m_fileHdr;
	if (m_measData) delete m_measData;
	if (m_filterDef) delete m_filterDef;
	if (m_preFilterDef) delete m_preFilterDef;
	if (m_sdoHdr) delete m_sdoHdr;
	if (m_oscDefMod) delete[] m_oscDefMod;
	if (cf_memory) free(cf_memory);
	if (m_cfgFileHdr) delete m_cfgFileHdr;
	if (m_cfgFileInfo) delete m_cfgFileInfo;
}


PYBIND11_MODULE(Aegis_osc, m) {
	py::class_<File_osc>(m, "File_osc")
		.def(py::init<>())
		.def("readOsc", &File_osc::readOsc, "Метод читает из файла fileName данные об осциллограммах",
			py::arg("fileName"))
		.def_readwrite("m_fileHdr", &File_osc::m_fileHdr)
		.def_readwrite("m_measData", &File_osc::m_measData)
	.def_readwrite("m_sdoHdr", &File_osc::m_sdoHdr)
	.def_readwrite("m_cfgFileHdr", &File_osc::m_cfgFileHdr)
	.def_readwrite("m_cfgFileInfo", &File_osc::m_cfgFileInfo)
	.def_readwrite("cf_memory", &File_osc::cf_memory)
	.def_readwrite("m_oscDefMod", &File_osc::m_oscDefMod)
	.def_readwrite("m_filterDef", &File_osc::m_filterDef)
	.def_readwrite("m_preFilterDef", &File_osc::m_preFilterDef);

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
}