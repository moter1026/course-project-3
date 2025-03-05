#include "Classes_for_files.h"
#include <logger.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>

#pragma once

class Cfg_in_ald {
public:
	Cfg_in_ald() = default;
	Cfg_in_ald(std::shared_ptr<std::ifstream>& file, std::shared_ptr<Logger>& logger);
	Cfg_in_ald(const Cfg_in_ald& objCfg);
	~Cfg_in_ald();

	void checkOpen();
	BOOL readCfg();
	void cls();

	CfgFileHdr* cfgFileHdr;		// structure for holding file header
	std::shared_ptr<std::ifstream> file;
	// ƒалее не используютс€ умные указателели,
	// т.к. размер выделени€ пам€ти
	// каждый раз может быть различным
	CfgFileInfo* cfgFileInfo;					// structure for holding info block
	void* cf_memory;							// configuration data.


protected:
	std::shared_ptr <Logger> m_logger;			// system logging 
	UINT m_alddm_cfg_ver_maj;
};