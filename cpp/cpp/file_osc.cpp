#include "file_osc.h"

File_osc::File_osc(std::wstring fileName, std::shared_ptr<Logger>& logger):
	file(nullptr) {
	setlocale(LC_ALL, "Russian");
	//if (!logger) throw std::runtime_error("Wrong logger!");
	this->m_logger = logger;
	this->m_logger->logging(LogLevel::_INFO_, "Creating a File_osc class for working with osc files");
	
	this->fileName = fileName;
	this->openReadFile();
	this->readOsc();
}

File_osc::File_osc(const File_osc& obgOsc): fileName(obgOsc.fileName)
	, fileHdr(obgOsc.fileHdr), measData(obgOsc.measData)
	, sdoHdr(obgOsc.sdoHdr), cfgFileHdr(obgOsc.cfgFileHdr)
	, oscDefMod(obgOsc.oscDefMod), file(obgOsc.file)
	, m_logger(obgOsc.m_logger)
{
	this->cfgFileInfo = (CfgFileInfo*)malloc(this->cfgFileHdr->info_size);
	if (!this->cfgFileInfo) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	std::copy(obgOsc.cfgFileInfo, obgOsc.cfgFileInfo + obgOsc.cfgFileHdr->info_size, this->cfgFileInfo);

	this->cf_memory = malloc(this->cfgFileInfo->mem_size);
	if (!this->cf_memory) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	std::memcpy(this->cf_memory, obgOsc.cf_memory, obgOsc.cfgFileInfo->mem_size);
}

bool File_osc::readOsc() 
{
	this->m_logger->logging(LogLevel::_INFO_, "read FileHdr info");
	this->fileHdr = new FileHdr;
	if (!this->fileHdr) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	file->read(reinterpret_cast<char*>(this->fileHdr), sizeof(FileHdr));

	this->m_logger->logging(LogLevel::_INFO_, "read MeasData info");
	this->measData = new MeasData;
	if (!this->measData) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	file->read(reinterpret_cast<char*>(this->measData), sizeof(MeasData));
	
	this->m_logger->logging(LogLevel::_INFO_, "read SDOHdr info");
	this->sdoHdr = new SDOHdr;
	if (!this->sdoHdr) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	file->read(reinterpret_cast<char*>(this->sdoHdr), sizeof(SDOHdr));

	this->m_logger->logging(LogLevel::_INFO_, "read CfgFileHdr info");
	this->cfgFileHdr = new CfgFileHdr;
	if (!this->cfgFileHdr) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	file->read(reinterpret_cast<char*>(this->cfgFileHdr), sizeof(CfgFileHdr));

	this->m_logger->logging(LogLevel::_INFO_, "read CfgFileInfo info");
	this->cfgFileInfo = (CfgFileInfo*)malloc(this->cfgFileHdr->info_size);
	if (!this->cfgFileInfo) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	file->read(reinterpret_cast<char*>(this->cfgFileInfo), this->cfgFileHdr->info_size);

	this->m_logger->logging(LogLevel::_INFO_, "read mem_size");
	this->cf_memory = malloc(cfgFileInfo->mem_size);
	if (!this->cf_memory) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	file->read(reinterpret_cast<char*>(this->cf_memory), this->cfgFileInfo->mem_size);
	
	std::string str = "start read data of osc. NumOSC = " + std::to_string(this->sdoHdr->NumOSC);
	this->m_logger->logging(LogLevel::_INFO_, str);
	for (size_t i = 0; i < this->sdoHdr->NumOSC; i++)
	{
		OSCDefMod values;
		file->read((char*)(&values), sizeof(OSCDefMod));
		file->seekg((values.buf_size << 1), std::ios::cur);
		this->oscDefMod.push_back(values);
	}
	this->m_logger->logging(LogLevel::_INFO_, "end read data of osc");

	return true;
}

std::vector<short> getDotOSCInFile(std::ifstream& file, int numOSC, OSCDefMod& oscDefMod)
{
	std::vector<short> dots;
	short dot;
	short dot_out;

	size_t f_pointer = oscDefMod.seek_wave;
	// Получаем длину файла
	file.seekg(0, std::ios::end);
	std::streamsize file_length = file.tellg();
	file.seekg(0, std::ios::beg);

	if (file_length > 0xFFFFFFFF) f_pointer += size_t(oscDefMod.seek_wave_high) << 32;

	file.seekg(f_pointer, std::ios::beg);
	file.seekg(sizeof(OSCDefMod), std::ios::cur);

	if (oscDefMod.bPowerMethod & 0x100) {
		size_t value;
		size_t base;
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

void setDotOSCInFile(std::ofstream& file, WORD bPowerMethod, std::vector<short> dots)
{
	if (bPowerMethod & 0x100) {
		std::vector<int> delitel;
		delitel.push_back(1);
		delitel.push_back(2);
		delitel.push_back(4);
		delitel.push_back(16);
		for (size_t j = 0; j < dots.size(); j++)
		{
			int base = 0;
			for (size_t i = 0; i < delitel.size(); i++)
			{
				base = dots[j] / delitel[i];
				if (labs((base * 4) & 0x03) == i)
					dots[j] = base * 4;
				break;
			}
			file.write((char*)&dots[j], sizeof(dots[j]));
		}
		return;
	}
	
	for (size_t j = 0; j < dots.size(); j++)
	{	
		file.write((char*)&dots[j], sizeof(dots[j]));
	}
}

std::vector<short> File_osc::getDotOSC(int numOSC)
{
	this->openReadFile();

	this->m_logger->logging(LogLevel::_INFO_, "start read dots of osc");
	std::vector<short> dots = getDotOSCInFile(*this->file, numOSC, this->oscDefMod[numOSC]);
	this->m_logger->logging(LogLevel::_INFO_, "end read dots of osc");

	this->file->close();
	return dots;
}

// NOC - no open, no close
std::vector<short> File_osc::getDotOSC_NOC(int numOSC)
{
	if (!this->file->is_open()) 
	{
		this->m_logger->logging(LogLevel::_CRITICAL_, "file for read closed!");
		throw std::runtime_error("file for read closed!");
	}

	std::vector<short> dots = getDotOSCInFile(*this->file, numOSC, this->oscDefMod[numOSC]);

	return dots;
}

std::vector<std::vector<short>> File_osc::getDotsOSC(int startOSC, int endOSC) {
	this->openReadFile();

	std::vector<std::vector<short>> dots;

	this->m_logger->logging(LogLevel::_INFO_, "start read dots of osc");
	for (size_t i = startOSC; i < endOSC; i++)
	{
		dots.push_back(getDotOSCInFile(*this->file, i, this->oscDefMod[i]));
	}
	this->m_logger->logging(LogLevel::_INFO_, "end read dots of osc");

	this->file->close();
	return dots;
}

std::vector<double> File_osc::get_K_mkV(size_t start, size_t end)
{
	std::vector<double> K_mkV;
	for (size_t i = start; i < end; i++)
	{
		K_mkV.push_back(this->oscDefMod[i].K_mkV);
	}
	return K_mkV;
}

bool File_osc::saveOSC(std::wstring fileName
	, std::vector<std::vector<short>> arr_osc
	, std::vector<int> indexes)
{
	std::ofstream file(fileName, std::ios::binary);
	if (!file) {
		this->m_logger->logging(LogLevel::_CRITICAL_, "Can`t open file for writing");
		throw std::domain_error(_T("не удалось открыть файл для записи."));
	}

	this->m_logger->logging(LogLevel::_INFO_, "write FileHdr info");
	if (!this->fileHdr) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	file.write(reinterpret_cast<char*>(this->fileHdr), sizeof(FileHdr));

	this->m_logger->logging(LogLevel::_INFO_, "write MeasData info");
	if (!this->measData) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	file.write(reinterpret_cast<char*>(this->measData), sizeof(MeasData));

	this->m_logger->logging(LogLevel::_INFO_, "write SDOHdr info");
	if (!this->sdoHdr) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	this->sdoHdr->NumOSC = arr_osc.size();
	file.write(reinterpret_cast<char*>(this->sdoHdr), sizeof(SDOHdr));

	this->m_logger->logging(LogLevel::_INFO_, "write CfgFileHdr info");
	if (!this->cfgFileHdr) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	file.write(reinterpret_cast<char*>(this->cfgFileHdr), sizeof(CfgFileHdr));

	this->m_logger->logging(LogLevel::_INFO_, "write CfgFileInfo info");
	if (!this->cfgFileInfo) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	file.write(reinterpret_cast<char*>(this->cfgFileInfo), this->cfgFileHdr->info_size);

	this->m_logger->logging(LogLevel::_INFO_, "write mem_size");
	if (!this->cf_memory) this->m_logger->logging(LogLevel::_ERROR_, "Memory allocation error");
	file.write(reinterpret_cast<char*>(this->cf_memory), this->cfgFileInfo->mem_size);

	this->m_logger->logging(LogLevel::_INFO_, "start write data of osc");
	for (size_t i = 0; i < arr_osc.size(); i++)
	{
		OSCDefMod* now_oscDefMod = &(this->oscDefMod[indexes[i]]);
		now_oscDefMod->seek_wave = file.tellp();
		file.write(reinterpret_cast<char*>(now_oscDefMod), sizeof(OSCDefMod));
		setDotOSCInFile(file, now_oscDefMod->bPowerMethod, arr_osc[i]);
	}
	this->m_logger->logging(LogLevel::_INFO_, "end write data of osc");


	close(file);
	return true;
}

void File_osc::openReadFile()
{
	try {
		// Создаем новый поток, если нужно
		if (!this->file) {
			this->file = std::make_shared<std::ifstream>();
		}
		else if (this->file.get() && this->file->is_open()) {
			this->m_logger->logging(LogLevel::_INFO_, "File is open.");
			return;
		}
		// Открываем файл с проверкой режимов
		this->file->open(this->fileName, std::ios::binary | std::ios::ate); // Открываем в конце для проверки размера

		// Проверка успешности открытия
		if (!this->file->is_open()) {
			std::error_code ec(errno, std::system_category());
			this->m_logger->logging(LogLevel::_CRITICAL_ ,
				L"Failed to open file: " + this->fileName + L" | Error: " + string_to_wstring(ec.message()));
			throw std::runtime_error(
				"File open error " + wstring_to_string(this->fileName));
		}

		// Дополнительные проверки
		const auto file_size = this->file->tellg();
		if (file_size == 0) {
			this->m_logger->logging(LogLevel::_WARNING_, "Opened empty file: " + wstring_to_string(this->fileName));
		}

		// Возвращаем указатель в начало
		this->file->seekg(0, std::ios::beg);

	}
	catch (const std::ios_base::failure& e) {
		this->m_logger->logging(LogLevel::_CRITICAL_,
			"I/O exception: " + std::string(e.what()) + " | Code: " + std::to_string(e.code().value()));
		throw;
	}
}

void File_osc::cls()
{
	this->m_logger->logging(LogLevel::_INFO_, "close file osc");
	if (this->file) this->file->close();
}

void File_osc::close(std::ifstream& file)
{
	this->m_logger->logging(LogLevel::_INFO_, "close file osc");
	if (file)	file.close();
}
void File_osc::close(std::ofstream& file)
{
	this->m_logger->logging(LogLevel::_INFO_, "close file osc");
	if (file)	file.close();
}

File_osc::~File_osc() {
	this->m_logger->logging(LogLevel::_INFO_, "clear File_osc data");
	if (this->fileHdr) free(this->fileHdr);
	if (this->measData) free(this->measData);
	if (this->sdoHdr) free(this->sdoHdr);
	if (this->cfgFileHdr) free(this->cfgFileHdr);
	if (this->cfgFileInfo) free(this->cfgFileInfo);
	if (this->cf_memory) free(this->cf_memory);
	if (this->file->is_open() &&
		this->file.use_count() == 1) this->cls();
}
