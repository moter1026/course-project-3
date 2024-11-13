#include "libraries/file_osc.h"

#include <iostream>

int main() {
	setlocale(LC_ALL, "ru");
	File_osc osc;

	TCHAR szDirectory[MAX_PATH] = "";
	::GetCurrentDirectory(sizeof(szDirectory) - 1, szDirectory);

	osc.readOsc("D:/University/course project 3/cpp/aem19_06_23#01.osc");

	for (size_t i = 0; i < osc.m_sdoHdr->NumOSC; i++)
	{
		std::cout << "����� ������: " << osc.m_oscDefMod[i].chan << "\n";
		std::cout << "����� ����������� osc: " << osc.m_oscDefMod[i].osc_color << "\n";
		std::cout << "�-�� �������������, ���: " << osc.m_oscDefMod[i].freq << "\n";
		std::cout << "���������� �����: " << osc.m_oscDefMod[i].buf_size << "\n";
		std::cout << "������������ ������, ��� (���������): " << osc.m_oscDefMod[i].buf_time << "\n";
		std::cout << "�������� ������ ������������� � ����� (������� 32-������ �����): " << osc.m_oscDefMod[i].seek_wave << "\n";
		std::cout << "�������� ������ ������������� � ����� (������� 32-������ �����): " << osc.m_oscDefMod[i].seek_wave_high << "\n";
		std::cout << "����� ������� ����� ������� ����� (������������ ������ � ALine OSC Processing): " << osc.m_oscDefMod[i].tFastest << "\n";
		std::cout << "_____________________________________________________________\n\n\n";
	}
	

	return 0;
}
