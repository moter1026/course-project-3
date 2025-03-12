#pragma once
#include "Classes_for_files.h"
#include <logger.h>
#include <file_cfg.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>


class File_ald {
public:
	File_ald() = default;
	File_ald(std::wstring fileName, const std::shared_ptr<Logger>& logger);
	~File_ald();
	File_ald(const File_ald& objAld);

	void openReadFile();
	void saveNewAld(std::wstring newNameOSC);
	BOOL readAld();
	void cls();

	std::wstring fileName;
	FileHdr*	fileHdr;
	MeasData*	measData;
	Cfg_in_ald*	cfg;
	std::shared_ptr<std::ifstream>	file;

	std::vector <EV_DATA_OUT>	evData;
	std::vector <EV_AVG_OUT>	noiseData;	// localisation noise data storages //07.12.99S //05.09.00S
	std::vector <UNI_AVG_OUT>	unisAvData;	// UNISCOPE averaged data storages //18.06.19S
	std::vector <EV_PARAM_OUT>	paramData;	// localisation parameters data storages //07.12.99S //05.09.00S
	std::vector <UNI_PARAM_OUT> unisParamData;	// UNISCOPE parametric data storages //12.05.17S
	std::vector <CH_INFO_OUT>	sysData;		// localisation system data storages //07.12.99S //05.09.00S
	std::vector <MARKER_OUT>	markerData;	// localisation makrer data storages //22.04.04S
	std::vector <TEXT_OUT>		textData;
	std::vector <SERVICE_OUT>	servData;
	std::vector <RADIO_OUT>		radioData;

protected:
	std::shared_ptr<Logger> m_logger;
	// Читает данные из файла блоками.
	// Возвращает количество прочитанных элементов типа UINT.
	// readBuffer - буфер, из которого будут читаться данные.
	// stay_count - количество элементов, оставшихся с предыдущей итерации.
	size_t ReadDataIntoBuffer(UINT* readBuffer, const size_t stayСount);
};