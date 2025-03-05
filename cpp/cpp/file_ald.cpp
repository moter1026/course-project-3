#include <file_ald.h>

File_ald::File_ald(std::wstring fileName, const std::shared_ptr<Logger>& logger)
    : fileHdr(nullptr), measData(nullptr)
	, cfg(nullptr), file(nullptr)
{
	setlocale(LC_ALL, "Russian");
	logger->logging(LogLevel::_INFO_, "In constructor!");
	// <= 5, т.к. ".ald" в названии файла точно должно быть
	/*if (fileName.length() <= 5 || !logger.get()) {
		this->m_logger->logging(LogLevel::_INFO_, "Wrong logger or file name!");
		throw std::runtime_error("Wrong logger or file name!");
	}*/
    this->m_logger = logger;
	this->fileName = fileName;
    this->openReadFile();
	this->readAld();
    this->m_logger->logging(LogLevel::_INFO_, "Creating a File_ald class for working with ald files");
}
File_ald::~File_ald()
{
	if (this->fileHdr) delete this->fileHdr;
	if (this->measData) delete this->measData;
	if (this->cfg) delete this->cfg;
	if (this->file.get() &&
		this->file.use_count() == 1) this->cls();
}

File_ald::File_ald(const File_ald& objAld): fileName(objAld.fileName)
	, fileHdr(new FileHdr(*objAld.fileHdr))
	, measData(new MeasData(*objAld.measData))
	, cfg(new Cfg_in_ald (*objAld.cfg))
	, file(objAld.file)
	, m_logger(objAld.m_logger)
{
	
}

void File_ald::openReadFile()
{
	//if (this->file && this->file->is_open()) this->cls();

    //this->m_logger->logging(LogLevel::_INFO_, "open file \"" + wstring_to_string(this->fileName) + "\" ald for reading");
	//this->file = std::make_shared<std::ifstream>(this->fileName, std::ios::binary);
    //std::ifstream* file = new std::ifstream(fileName, std::ios::binary);
    /*if (!this->file->is_open()) {
        this->m_logger->logging(LogLevel::_CRITICAL_, "Can`t open file for reading");
        throw std::domain_error(_T("не удалось открыть файл для чтения."));
    }*/

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
			this->m_logger->logging(LogLevel::_CRITICAL_,
				"Failed to open file: " + wstring_to_string(this->fileName) + " | Error: " + ec.message());
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

void File_ald::saveNewAld(std::wstring newNameOSC) {
	try {
		setlocale(LC_ALL, "Russian");
		this->m_logger->logging(LogLevel::_INFO_, "start save new ald");

		this->openReadFile();

		// Проверка длины имени
		if (newNameOSC.size() >= 255 || newNameOSC.size() <= 5) {
			throw std::runtime_error("Wrong size of string \"newNameOSC\"");
		}

		// Сохранение старого имени
		char oldOSCName[255];
		strncpy(oldOSCName, this->measData->sdo_name, 254);
		oldOSCName[254] = '\0';

		// Конвертация wstring в string
		std::string new_name = wstring_to_ansi(newNameOSC);
		if (new_name.size() >= 255) {
			throw std::runtime_error("Имя файла слишком длинное");
		}

		new_name.erase(new_name.begin(), new_name.begin() + new_name.rfind('/') + 1);
		strncpy(this->measData->sdo_name, new_name.c_str(), 254);
		this->measData->sdo_name[254] = '\0';

		// Формирование имени .ald файла
		std::wstring newFileNameAld = newNameOSC;
		size_t dot_pos = newFileNameAld.rfind('.');
		if (dot_pos == std::string::npos) {
			this->m_logger->logging(LogLevel::_ERROR_, "Некорректное имя файла (нет расширения)");
			return;
		}
		newFileNameAld.replace(dot_pos + 1, 3, L"ald");

		// Создание файла
		this->m_logger->logging(LogLevel::_INFO_, "start create new file");
		std::ofstream newAld(wstring_to_ansi(newFileNameAld), std::ios_base::binary | std::ios_base::trunc);
		if (!newAld.is_open()) {
			this->m_logger->logging(LogLevel::_ERROR_, "Не удалось создать файл: " + wstring_to_ansi(newFileNameAld));
			return;
		}

		// Копирование данных
		if (!this->file->is_open()) {
			this->m_logger->logging(LogLevel::_ERROR_, "Исходный файл не открыт");
			newAld.close();
			return;
		}

		newAld.write(reinterpret_cast<char*>(this->fileHdr), sizeof(FileHdr));
		newAld.write(reinterpret_cast<char*>(this->measData), sizeof(MeasData));
		this->m_logger->logging(LogLevel::_INFO_, "after write FileHdr and MeasData.");

		size_t now_p = this->file->tellg();
		this->file->seekg(0, std::ios::end);
		size_t max_size_file = this->file->tellg();
		this->file->seekg(now_p, std::ios::beg);

		size_t seek = sizeof(FileHdr) + sizeof(MeasData);
		this->m_logger->logging(LogLevel::_INFO_, "find seek. seek = " + std::to_string(seek) + "; max = " + std::to_string(max_size_file));

		// Проверяем, что позиция установлена корректно
		if (max_size_file < seek) {
			this->m_logger->logging(LogLevel::_ERROR_, "Ошибка позиционирования в файле");
			return;
		}

		this->m_logger->logging(LogLevel::_INFO_, "try seekg.");
		// Перемещаем указатель чтения в исходном файле на нужное смещение
		this->file->clear();  // Сброс ошибок перед seekg()
		this->file->seekg(seek, std::ios::beg);  // seek - ваше вычисленное смещение
		if (this->file->tellg() != seek) this->m_logger->logging(LogLevel::_CRITICAL_, "this->file->tellg() != seek");

		this->m_logger->logging(LogLevel::_INFO_, "start copy data");

		size_t memory_size = 1000;
		char* data = new char[memory_size];
		while (true)
		{
			this->file->read(data, memory_size);
			size_t bytes_read = this->file->gcount();
			if (bytes_read != memory_size)
			{
				newAld.write(data, bytes_read);
				newAld.flush();
				break;
			}
			newAld.write(data, bytes_read);
			newAld.flush();
		}
		delete[] data;
		this->m_logger->logging(LogLevel::_INFO_, "end copy data");

		// Восстановление старого имени
		strncpy(this->measData->sdo_name, oldOSCName, 254);
		this->measData->sdo_name[254] = '\0';

		newAld.close();
		this->m_logger->logging(LogLevel::_INFO_, "end create new file");
	}
	catch (const std::exception& err)
	{
		this->m_logger->logging(LogLevel::_CRITICAL_, err.what());
		return;
	}
}


BOOL File_ald::readAld()
{
    try
    {
		if (!this->fileHdr) this->fileHdr = new FileHdr();
		this->file->read(reinterpret_cast<char*>(this->fileHdr), sizeof(FileHdr));
		size_t bytes_read = this->file->gcount();
        if (bytes_read != sizeof(FileHdr))
        {
            this->m_logger->logging(LogLevel::_CRITICAL_, "Wrong read bytes FileHdr");
            throw std::runtime_error("Wrong read bytes FileHdr");
        }

		if (!this->measData) this->measData = new MeasData();
		this->file->read(reinterpret_cast<char*>(this->measData), sizeof(MeasData));
        bytes_read = this->file->gcount();
        if (bytes_read != sizeof(MeasData))
        {
            this->m_logger->logging(LogLevel::_CRITICAL_, "Wrong read bytes MeasData");
            throw std::runtime_error("Wrong read bytes MeasData");
        }
        std::streampos now = file->tellg();
        this->file->seekg(0, std::ios::end);
        std::streampos fileSize = file->tellg();
        this->file->seekg(now, std::ios::beg);

		if (!this->cfg) this->cfg = new Cfg_in_ald(this->file, this->m_logger);
        this->cfg->readCfg();
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//	creating file reading buffer
		EV_DATA_OUT*	data;
		EV_AVG_OUT*		avg_data;
		EV_PARAM_OUT*	p_data;
		CH_INFO_OUT*	s_data;
		MARKER_OUT*		marker_data;	//22.04.04S
		TEXT_OUT*		text_data;		//22.04.04S
		SERVICE_OUT*	serv_data;		//01.06.11Kir
		RADIO_OUT*		r_data;			//27.05.15S
		UNI_PARAM_OUT*	unis_p_data;	//12.05.17S
		UNI_AVG_OUT*	unis_av_data;	//18.06.19S

		AE_TIME			cur_time;
		size_t	Usize;

		size_t	stay_count = 0;
		long	cur_file_pos;	//03.06.04S
		size_t	pointer, position, words_read, type, def_size, otschet;	//13.04.01S
		
		size_t*	readBuffer = new size_t[READ_BUF_SIZE + MAX_DATA_SIZE];
		if (!readBuffer) {
			return FALSE;
		}
		std::fill(readBuffer, readBuffer + (READ_BUF_SIZE + MAX_DATA_SIZE), 0);
		
		cur_file_pos = long(this->file->tellg());	//03.06.04S
		//	reading file data to the buffer
		while (true) {
			words_read = ReadDataIntoBuffer(readBuffer, stay_count);
			if (!words_read) break;

			pointer = 0;
			if (words_read >= READ_BUF_SIZE) {
				otschet = MAX_DATA_SIZE;	//16.02.01S
			}
			else {
				otschet = 0;
			}
			while (words_read + stay_count > otschet + pointer) {	//06.04.05S
				type = LOWORD(readBuffer[pointer]);
				def_size = HIWORD(readBuffer[pointer]);
				Usize = def_size / sizeof(UINT);
				position = pointer;	//03.06.04S
				pointer++;
				switch (type) {
				case ev_data_old:	//24.04.01S
					//					reading old type ordinary events from the file
					data = (EV_DATA_OUT*)(readBuffer + pointer);
					//memcpy(&data, (void*)(m_ReadBuffer + pointer), DataSizes[ev_data_old]);
					data->locZ = NO_LOC;	//24.04.01S
					data->gain = 0L;		//24.04.01S
					type = ev_data;		//24.04.01S
					pointer += DataSizes[ev_data_old] / sizeof(unsigned int);
					this->evData.push_back(*data);
					break;
					//					goto ordi_data;
				case ev_data:	//24.04.01S
					//					reading ordinary events from the file
					data = (EV_DATA_OUT*)(readBuffer + pointer);
					pointer += DataSizes[ev_data] / sizeof(unsigned int);
					this->evData.push_back(*data);
					break;
				case noise_data:	//17.09.02S+	//20.09.02S
					//					reading noise data from the file
					avg_data = (EV_AVG_OUT*)(readBuffer + pointer);
					pointer += DataSizes[noise_data] / sizeof(UINT);
					this->noiseData.push_back(*avg_data);
					break;
				case unis_avg_data:	//18.06.19S
					//					reading averaged data from the file
					unis_av_data = (UNI_AVG_OUT*)(readBuffer + pointer);
					pointer += DataSizes[unis_avg_data] / sizeof(UINT);
					this->unisAvData.push_back(*unis_av_data);
					break;
				case param_data:
					//					reading parameter events from the file
					p_data = (EV_PARAM_OUT*)(readBuffer + pointer);
					pointer += DataSizes[param_data] / sizeof(UINT);
					this->paramData.push_back(*p_data);
					break;
				case unis_param_data:
					//					reading uniscope parameter events from the file //12.05.17S
					unis_p_data = (UNI_PARAM_OUT*)(readBuffer + pointer);
					pointer += DataSizes[unis_param_data] / sizeof(UINT);
					this->unisParamData.push_back(*unis_p_data);
					break;
				case sys_data:
					//					reading parameter events from the file
					s_data = (CH_INFO_OUT*)(readBuffer + pointer);
					pointer += DataSizes[sys_data] / sizeof(UINT);
					this->sysData.push_back(*s_data);
					break;
				case marker:
					//					reading time marker from the file	//22.04.04S
					marker_data = (MARKER_OUT*)(readBuffer + pointer);
					pointer += Usize;
					this->markerData.push_back(*marker_data);
					break;
				case text:
					//					reading time marker text from the file	//22.04.04S
					text_data = (TEXT_OUT*)(readBuffer + pointer);
					pointer += Usize;
					this->textData.push_back(*text_data);
					break;
				case service_data:						//01.06.11Kir
					//					reading service data from the file
					serv_data = (SERVICE_OUT*)(readBuffer + pointer);
					pointer += DataSizes[service_data] / sizeof(UINT);
					this->servData.push_back(*serv_data);
					break;
					//					reading radio-channel service data from the file
				case radio_data:						//27.05.15S
					r_data = (RADIO_OUT*)(readBuffer + pointer);
					pointer += DataSizes[radio_data] / sizeof(UINT);
					this->radioData.push_back(*r_data);
					break;
				default:
					//					processing unknown type
					pointer += Usize;
				}
			}

			// Если остались необработанные данные, сохраняем их для следующей итерации
			if (words_read + stay_count >= pointer) {
				stay_count = words_read + stay_count - pointer;
				std::memcpy((void*)readBuffer, (void*)(readBuffer + pointer), stay_count * sizeof(UINT));
			}
			cur_file_pos = long(this->file->tellg());	//03.06.04S
		}
		return true;
    }
    catch (const std::exception& err)
    {
		this->m_logger->logging(LogLevel::_CRITICAL_, err.what());
        return false;
    }
}

void File_ald::cls()
{
    this->m_logger->logging(LogLevel::_INFO_, "close file ald");
    if (this->file->is_open()) this->file->close();
}

bool File_ald::ReadDataIntoBuffer(size_t* readBuffer, const size_t stayСount)
{
	// Проверка валидности
	if (!this->file->is_open() || !readBuffer) {
		return false; 
	}

	// Вычисляем количество байт для чтения
	const size_t bytesToRead = READ_BUF_SIZE * sizeof(UINT);

	// Читаем данные из файла
	this->file->read(reinterpret_cast<char*>(readBuffer + stayСount), bytesToRead);

	// Проверяем, удалось ли прочитать данные
	if (!this->file->good()) {
		return false; // Ошибка чтения
	}
	// Вычисляем количество прочитанных элементов типа UINT
	// и возвращаем это значение
	size_t bytesRead = this->file->gcount();
	return bytesRead / sizeof(UINT);;
}
