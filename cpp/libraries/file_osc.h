#pragma once
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

namespace py = pybind11;

class File_osc {
public:
	File_osc() = default;
	File_osc(std::wstring fileName, std::shared_ptr<Logger>& logger);
	File_osc(const File_osc& obgOsc);
	~File_osc();

	std::wstring fileName;

	// Ќе использую unique_ptr, т.к. его проблематично
	// использовать с pybind11

	FileHdr* fileHdr;
	MeasData* measData;
	SDOHdr* sdoHdr;
	CfgFileHdr* cfgFileHdr;
	CfgFileInfo* cfgFileInfo;
	void* cf_memory;
	std::vector<OSCDefMod> oscDefMod;
	std::shared_ptr<std::ifstream> file;

private:
	std::shared_ptr<Logger> m_logger;

public:
	bool readOsc();
	std::vector<short> getDotOSC(int numOSC);
	std::vector<short> File_osc::getDotOSC_NOC(int numOSC);
	std::vector<std::vector<short>> getDotsOSC(int startOSC, int endOSC);
	std::vector<double> get_K_mkV(size_t start, size_t end);
	bool saveOSC(std::wstring fileName
		, std::vector<std::vector<short>> arr_osc
		, std::vector<int> indexes);
	void openReadFile();
	void cls();
	void close(std::ifstream& file);
	void close(std::ofstream& file);
};

