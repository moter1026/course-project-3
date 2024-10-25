#include "stdafx.h"
#include "Classes_for_files.h"

#include <string>
#include <string>
#include <fstream>
#include <iostream>
//#include "Cfgman.h"
//#include "DataMan.h"

#define MAX_FDEFS 4

#pragma once


class File_osc {
public:
	File_osc(): m_fileHdr(nullptr), m_measData(nullptr)
	, m_filterDef(nullptr), m_preFilterDef(nullptr)
	, m_sdoHdr(nullptr), m_oscDefMod(nullptr)
	, cf_memory(nullptr), m_cfgFileHdr(nullptr)
	, m_cfgFileInfo(nullptr){}

	FileHdr* m_fileHdr;
	MeasData* m_measData;
	SDOHdr* m_sdoHdr;
	CfgFileHdr* m_cfgFileHdr;
	CfgFileInfo* m_cfgFileInfo;
	void* cf_memory;
	OSCDefMod* m_oscDefMod;
	FilterDef* m_filterDef;
	PreFilterData* m_preFilterDef;


	bool readOsc(std::string fileName) {

		std::ifstream file(fileName, std::ios::binary);
		if (file)
		{
			m_fileHdr = new FileHdr();
			if (m_fileHdr) AfxMessageBox("Ошибка при выделении памяти.");
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
			return false;
		}

		if (file)	file.close();
		return true;
	}
	~File_osc() {
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
};

