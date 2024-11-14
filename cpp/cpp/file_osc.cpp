#include "file_osc.h"

File_osc::File_osc(std::wstring fileName) {
	std::string logFileName = std::string("log_") + wstring_to_string(fileName);
	logFileName.end() = logFileName.end() - 4;
	this->m_logger = new Logger(logFileName);
	this->m_logger->logging(LogLevel::_INFO_, "Creating a File_osc class for working with osc files");
	this->readOsc(fileName);
}

bool File_osc::readOsc(std::wstring fileName) {
	this->m_logger->logging(LogLevel::_INFO_, "open file osc for reading");
	std::ifstream file(fileName, std::ios::binary);
	if (!file) {
		this->m_logger->logging(LogLevel::_CRITICAL_, "Can`t open file for reading");
		throw std::domain_error(_T("не удалось открыть файл для чтения."));
	}
	this->m_fileName = fileName;


	this->m_logger->logging(LogLevel::_INFO_, "read FileHdr info");
	this->m_fileHdr = new FileHdr();
	if (!this->m_fileHdr) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	file.read(reinterpret_cast<char*>(this->m_fileHdr), sizeof(FileHdr));

	this->m_logger->logging(LogLevel::_INFO_, "read MeasData info");
	this->m_measData = new MeasData();
	if (!this->m_measData) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	file.read(reinterpret_cast<char*>(this->m_measData), sizeof(MeasData));

	this->m_logger->logging(LogLevel::_INFO_, "read SDOHdr info");
	this->m_sdoHdr = new SDOHdr();
	if (!this->m_sdoHdr) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	file.read(reinterpret_cast<char*>(this->m_sdoHdr), sizeof(SDOHdr));

	this->m_logger->logging(LogLevel::_INFO_, "read CfgFileHdr info");
	this->m_cfgFileHdr = new CfgFileHdr();
	if (!this->m_cfgFileHdr) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	file.read(reinterpret_cast<char*>(this->m_cfgFileHdr), sizeof(CfgFileHdr));

	this->m_logger->logging(LogLevel::_INFO_, "read CfgFileInfo info");
	this->m_cfgFileInfo = (CfgFileInfo*)malloc(this->m_cfgFileHdr->info_size);
	if (!this->m_cfgFileInfo) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	file.read(reinterpret_cast<char*>(this->m_cfgFileInfo), this->m_cfgFileHdr->info_size);

	this->m_logger->logging(LogLevel::_INFO_, "read mem_size");
	this->cf_memory = malloc(m_cfgFileInfo->mem_size);
	if (!this->cf_memory) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	file.read(reinterpret_cast<char*>(this->cf_memory), this->m_cfgFileInfo->mem_size);

	this->m_logger->logging(LogLevel::_INFO_, "start read data of osc");
	for (size_t i = 0; i < this->m_sdoHdr->NumOSC; i++)
	{
		OSCDefMod values;
		file.read((char*)(&values), sizeof(OSCDefMod));
		file.seekg((values.buf_size << 1), std::ios::cur);
		this->m_oscDefMod.push_back(values);
	}
	this->m_logger->logging(LogLevel::_INFO_, "end read data of osc");


	close(file);
	return true;
}

std::vector<short> getDotOSCInFile(std::ifstream& file, int numOSC, OSCDefMod& oscDefMod)
{
	std::vector<short> dots;
	short dot;
	short dot_out;

	ULONGLONG f_pointer = oscDefMod.seek_wave;
	// Получаем длину файла
	file.seekg(0, std::ios::end);
	std::streamsize file_length = file.tellg();
	file.seekg(0, std::ios::beg);

	if (file_length > 0xFFFFFFFF) f_pointer += ULONGLONG(oscDefMod.seek_wave_high) << 32;

	file.seekg(f_pointer, std::ios::beg);
	file.seekg(sizeof(OSCDefMod), std::ios::cur);

	if (oscDefMod.bPowerMethod & 0x100) {
		long value;
		long base;
		for (size_t j = 0; j < oscDefMod.buf_size; j++)
		{
			file.read((char*)&dot, sizeof(dot));
			value = dot;
			base = value >> 2;
			switch (labs(value & 0x03))
			{
			case 0:	dot_out = base; break;
			case 1: dot_out = base * 2; break;
			case 2: dot_out = base * 4; break;
			case 3: dot_out = base * 16; break;
			}
			dots.push_back(dot_out);
		}
	}
	else
	{
		for (size_t j = 0; j < oscDefMod.buf_size; j++)
		{
			file.read((char*)&dot, sizeof(dot));
			dots.push_back(dot);
		}
	}

	return dots;
}

std::vector<short> File_osc::getDotOSC(int numOSC)
{
	this->m_logger->logging(LogLevel::_INFO_, "open file osc for reading");
	std::ifstream file(this->m_fileName, std::ios::binary);
	if (!file) {
		this->m_logger->logging(LogLevel::_CRITICAL_, "Can`t open file for reading");
		throw std::domain_error(_T("не удалось открыть файл для чтения."));
	}

	this->m_logger->logging(LogLevel::_INFO_, "start read dots of osc");
	std::vector<short> dots = getDotOSCInFile(file, numOSC, this->m_oscDefMod[numOSC]);
	this->m_logger->logging(LogLevel::_INFO_, "end read dots of osc");

	close(file);
	return dots;
}

std::vector<std::vector<short>> File_osc::getDotsOSC(int startOSC, int endOSC) {
	this->m_logger->logging(LogLevel::_INFO_, "open file osc for reading");
	std::ifstream file(this->m_fileName, std::ios::binary);
	if (!file) {
		this->m_logger->logging(LogLevel::_CRITICAL_, "Can`t open file for reading");
		throw std::domain_error(_T("не удалось открыть файл для чтения."));
	}

	std::vector<std::vector<short>> dots;

	this->m_logger->logging(LogLevel::_INFO_, "start read dots of osc");
	for (size_t i = startOSC; i < endOSC; i++)
	{
		dots.push_back(getDotOSCInFile(file, i, this->m_oscDefMod[i]));
	}
	this->m_logger->logging(LogLevel::_INFO_, "end read dots of osc");

	close(file);
	return dots;
}


void File_osc::close(std::ifstream& file)
{
	this->m_logger->logging(LogLevel::_INFO_, "close file osc");
	if (file)	file.close();
}

File_osc::~File_osc() {
	this->m_logger->logging(LogLevel::_INFO_, "clear File_osc data");
	if (m_fileHdr) delete m_fileHdr;
	if (m_measData) delete m_measData;
	if (m_sdoHdr) delete m_sdoHdr;
	if (m_cfgFileHdr) delete m_cfgFileHdr;
	if (m_cfgFileInfo) delete m_cfgFileInfo;
	if (cf_memory) free(cf_memory);
	if (m_logger) delete m_logger;
}


PYBIND11_MODULE(Aegis_osc, m) {
	py::class_<File_osc>(m, "File_osc")
		.def(py::init<std::wstring>())
		.def("readOsc", &File_osc::readOsc, "Метод читает из файла fileName данные об осциллограммах",
			py::arg("fileName"))
		.def("getDotOSC", &File_osc::getDotOSC, "Метод возвращает точки осциллограммы", py::arg("numOSC"))
		.def("getDotsOSC", &File_osc::getDotsOSC, "Метод возвращает список точек осциллограмм", py::arg("startOSC"), py::arg("endOSC"))
		.def_readwrite("m_fileHdr", &File_osc::m_fileHdr)
		.def_readwrite("m_measData", &File_osc::m_measData)
		.def_readwrite("m_sdoHdr", &File_osc::m_sdoHdr)
		.def_readwrite("m_cfgFileHdr", &File_osc::m_cfgFileHdr)
		.def_readwrite("m_cfgFileInfo", &File_osc::m_cfgFileInfo)
		.def_readwrite("cf_memory", &File_osc::cf_memory)
		.def_readwrite("m_oscDefMod", &File_osc::m_oscDefMod);

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