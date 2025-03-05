#include <file_cfg.h>

Cfg_in_ald::Cfg_in_ald(std::shared_ptr<std::ifstream>& file, std::shared_ptr<Logger>& logger):
    cfgFileHdr(nullptr), cfgFileInfo(nullptr), cf_memory(nullptr)
{
    setlocale(LC_ALL, "Russian");

    this->m_logger = logger;
    this->file = file;

    this->checkOpen();

    this->m_logger->logging(LogLevel::_INFO_, "Creating a Cfg_in_ald class for working with cfg files");
}


Cfg_in_ald::Cfg_in_ald(const Cfg_in_ald& objCfg):
    cfgFileHdr(new CfgFileHdr(*objCfg.cfgFileHdr))
    , file(objCfg.file)
    , m_logger(objCfg.m_logger)
    , m_alddm_cfg_ver_maj(objCfg.m_alddm_cfg_ver_maj)
{
    this->cfgFileInfo = (CfgFileInfo*)malloc(this->cfgFileHdr->info_size);
    if (!this->cfgFileInfo && this->file.use_count() == 1)
        this->cls();
    this->file->read(reinterpret_cast<char*>(this->cfgFileInfo), this->cfgFileHdr->info_size);


    this->cf_memory = malloc(this->cfgFileInfo->mem_size);
    if (!this->cf_memory && this->file.use_count() == 1)
        this->cls();
    this->file->read(reinterpret_cast<char*>(this->cf_memory), this->cfgFileInfo->mem_size);
}

Cfg_in_ald::~Cfg_in_ald()
{
    this->m_logger->logging(LogLevel::_INFO_, "clear File_osc data");
    if (this->cfgFileInfo) free(cfgFileInfo);
	if (this->cf_memory) free(cf_memory);
}

void Cfg_in_ald::checkOpen()
{
   /* if (this->file && this->file->is_open()) this->cls();

    this->m_logger->logging(LogLevel::_INFO_, "open file cfg for reading");
    this->file = std::make_shared<std::ifstream>(wstring_to_string(this->fileName), std::ios::binary);
    if (!this->file->is_open()) {
        this->m_logger->logging(LogLevel::_CRITICAL_, "Can`t open file for reading");
        throw std::domain_error(_T("не удалось открыть файл для чтения."));
    }*/

    try {
        // Создаем новый поток, если нужно
        if (!this->file || !this->file.get()) {
            this->m_logger->logging(LogLevel::_CRITICAL_, "NO FILE ALD for read cfg");
            throw std::runtime_error("NO FILE ALD for read cfg");
        }

        // Проверка открыт ли файл
        if (!this->file->is_open()) {
            std::error_code ec(errno, std::system_category());
            this->m_logger->logging(LogLevel::_CRITICAL_, "FILE ALD NO OPEN! Can't read cfg");
            throw std::runtime_error("FILE ALD NO OPEN! Can't read cfg");
        }
    }
    catch (const std::ios_base::failure& e) {
        this->m_logger->logging(LogLevel::_CRITICAL_,
            "I/O exception: " + std::string(e.what()) + " | Code: " + std::to_string(e.code().value()));
        throw;
    }
}

void Cfg_in_ald::cls()
{
    this->m_logger->logging(LogLevel::_INFO_, "close file osc");
    if (this->file->is_open() && this->file.use_count() == 1)
        this->file->close();
}

BOOL Cfg_in_ald::readCfg()
{
    try { 	//15.07.02 CHс
        if (!this->file) throw std::runtime_error("No File");

        this->m_logger->logging(LogLevel::_INFO_, "start read cfg info.");
        if (!this->cfgFileHdr) this->cfgFileHdr = new CfgFileHdr();
        else
        {
            delete this->cfgFileHdr;
            this->cfgFileHdr = new CfgFileHdr();
        }
        this->file->read(reinterpret_cast<char*>(this->cfgFileHdr), sizeof(CfgFileHdr));

        if (this->cfgFileHdr->magic_word != CFG_MAGIC) {
            // +++ insert file open dialog here to restart reading CFG
            this->m_logger->logging(LogLevel::_CRITICAL_, "Неправильный файл конфигурации.");
            if (this->file.use_count() == 1) 
                this->file->close();
            return FALSE;
        }

        if ((this->cfgFileHdr->ver_maj > CFG_VER_MAJ) || (this->cfgFileHdr->ver_min != CFG_VER_MIN)) {	//16.02.06S
            // ++ may be we should try to handle old file versions later.
            this->m_logger->logging(LogLevel::_CRITICAL_, "Старый файл конфигурации.");
            if (this->file.use_count() == 1) 
                this->file->close();
            return FALSE;
        }

        // очищаем, т.к. размер новой выделенной памяти может быть другим
        if (this->cfgFileInfo) {
            free(this->cfgFileInfo);
        }

        this->cfgFileInfo = (CfgFileInfo*)malloc(this->cfgFileHdr->info_size);
        if (!this->cfgFileInfo && this->file.use_count() == 1) {
            this->file->close();
            return FALSE;
        }

        file->read(reinterpret_cast<char*>(this->cfgFileInfo), this->cfgFileHdr->info_size);

        // очищаем, т.к. размер новой выделенной памяти может быть другим
        if (this->cf_memory) {
            free(this->cf_memory);
        }
        this->cf_memory = malloc(this->cfgFileInfo->mem_size);
        if (!this->cf_memory && this->file.use_count() == 1) {
            this->file->close();
            return FALSE;
        }

        file->read(reinterpret_cast<char*>(this->cf_memory), this->cfgFileInfo->mem_size);
        if (this->file.use_count() == 1)
            this->file->close();
        this->m_logger->logging(LogLevel::_INFO_, "End read cfg info.");

    }
    catch (std::runtime_error e) {
        this->m_logger->logging(LogLevel::_ERROR_, e.what());
        return FALSE;
    }

    return TRUE;
}
