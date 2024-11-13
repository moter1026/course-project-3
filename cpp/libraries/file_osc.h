#include "stdafx.h"
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
#define MAX_FDEFS 4

namespace py = pybind11;

#pragma once


class File_osc {
public:
	File_osc() = default;
	File_osc(std::wstring fileName);
	~File_osc();

	std::wstring m_fileName;
	FileHdr* m_fileHdr;
	MeasData* m_measData;
	SDOHdr* m_sdoHdr;
	CfgFileHdr* m_cfgFileHdr;
	CfgFileInfo* m_cfgFileInfo;
	void* cf_memory;
	std::vector<OSCDefMod> m_oscDefMod;

private:
	Logger* m_logger;

public:
	bool readOsc(std::wstring fileName);
	std::vector<short> getDotsOSC(int numOSC);
	void close(std::ifstream& file);
};

std::string wstring_to_string(const std::wstring& wstr) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(wstr);
}

std::wstring string_to_wstring(const std::string& str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(str);
}
