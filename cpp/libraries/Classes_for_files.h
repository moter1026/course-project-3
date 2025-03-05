#pragma once
#include <logger.h>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "afxpriv.h"
#include <time.h>
#include <cwchar>    // ��� ������� mbstowcs
#include <cstring>   // ��� strlen
#include <string>
#include <codecvt>
//#include <Filecons.h>


//������ ������� ���������������� ������
#define		CFG_HARDWARE		1       // hardware configuration block - ����� ������ ���� ��������� ���������� �����
#define		CFG_GAINWND			2       // GAINWND - ���������������� ���� �������� ��������� ���� "��������/�����" (��� DDM-2 ������� ������ �� �����, ��� ��� �������� ��������� ��� ����� ���� ������)
#define		CFG_MAINFRAME		3       // MAINFRAME - ���������������� ���� �������� ��������� �������� ���� ���������
#define		CFG_AMPWND			4       // AMPWND - ���������������� ���� �������� ��������� ���� "������� ���������/�����"
#define		CFG_AMPTOTALWND		5       // AMPTOTALWND - ���������������� ���� �������� ��������� ���� "������� ��������� ���������/�����"
#define		CFG_EVWND			6       // EVWND - ���������������� ���� �������� ��������� ���� "���������� �� /�����"
#define		CFG_EVSUMWND		7       // EVSUMWND - ���������������� ���� �������� ��������� ���� "��������� ���� ��������� �� /�����"
#define		CFG_EVRUNWND		8       // EVRUNWND - ���������������� ���� �������� ��������� ���� "��������� ���� ��������� �� /�����"
#define		CFG_EVTOTALWND		9		// EVTOTALWND - ���������������� ���� �������� ��������� ���� "��������� ���������� / �����"
#define		CFG_RISEWND			10      // RISETIMEWND - ���������������� ���� �������� ��������� ���� "������� ����� ���������� / �����"
#define		CFG_DURWND			11      // Duration Wnd - ���������������� ���� �������� ��������� ���� "������� ������������ / �����"
#define		CFG_ALARMS			12      // alarms settings
#define		CFG_PRINT			13      // printer settings - �� ������������
#define		CFG_ENERGYWND		14      // energy window - ���������������� ���� �������� ��������� ���� "������� ������� /�����"
#define		CFG_DUMMY			15      // DUMMY - �� ������������
#define		CFG_TIMER			16      // timers - �� ������������
#define		CFG_NOTOTALWND		17      // NOISETOTALWND - ���������������� ���� �������� ��������� ���� "������� ������� ���� / �����"
#define		CFG_TIGWND			18      // Tig window - ���������������� ���� �������� ��������� ���� "������� ����� �������� � �� ��������/�����"
#define		CFG_EVERYAMPWND		19		// EveryAmp real-time window - �� ������������
#define		CFG_MAXAMPWND		20		// MaxAmp real-time window - ���������������� ���� �������� ��������� ���� "������������ ���������/�����"	
#define		CFG_GENERAL			21		// General setup configuration
#define		CFG_AUTOSETUP		22		// Autothreshold setup configuration
#define		CFG_POSTWNDSOLD		23		// PostWnds	(������ ������)	- �� ������������
#define		CFG_HISTWNDSOLD		24		// HistWnds	(������ ������)	- �� ������������
#define		CFG_MEASDATA		25		// MeasData	- ��������� ���������� ������ ��������� (��. ��������� MeasData ����)
#define		CFG_ENRSUMWND		26		// Energy summary window - ���������������� ���� �������� ��������� ���� "����� ������� / �����"
#define		CFG_SYSTEMINFO		27		// ���������� �� ���������� ����� ��������� � ������ �� ��� ������������� �����
#define		CFG_TIGSUMWND		28      // Tig summary window - ���������������� ���� �������� ��������� ���� "��������� ���� �������� �� / �����"
#define		CFG_NOWND			29      // Noise window - ���������������� ���� �������� ��������� ���� "������� ������� ���� / �����"
#define		CFG_CNTSUMWND		30      // Count Summ window - ���������������� ���� �������� ��������� ���� "��������� ���� �������� �� / �����"
#define		CFG_HOTKEYS			31      // Hot Key data configuration block
#define		CFG_MAXAMPCOUNTWND	32		// Amplitude/Count window - ���������������� ���� �������� ��������� ���� "������������ ��������� / ������������ ����� ��������"
#define		CFG_LOCPICT2D		33		// ���������������� ���� �������� ��������� ���� ������� ���� "�������" 2D
#define		CFG_LOCPICT3D		34		// ���������������� ���� �������� ��������� ���� ������� ���� "�������" 3D
#define		CFG_POSTWNDS		35		// PostWnds	(����� ������) - ���������������� ���� �������� �������������� ����
#define		CFG_HISTWNDS		36		// HistWnds	(����� ������) - ���������������� ���� �������� ������������� ����
#define		CFG_IPDEVICE		37		// IP-Addresses configuration block	
#define		CFG_EXPORT			38		// Export configuration block
#define		CFG_SERVWND			39      // ServWnd - ���������������� ���� �������� ��������� ���� "��������� ��������� / �����"
#define		CFG_AMPCRIT			40		// Amplitude criterion block
#define		CFG_COUNTWND		41      // CountWnd - ���������������� ���� �������� ��������� ���� "�������� ����� / �����"
#define		CFG_LEVRCVRWND		42		// Level of transmitter signal window - ���������������� ���� �������� ��������� ���� "������� ������� ����������� ��� / �����" (������ ��� ������ � ������������)
#define		CFG_LEVPPSWND		43		// Level of receiver signal window - ���������������� ���� �������� ��������� ���� "������� ������� ��������� ��� / �����" (������ ��� ������ � ������������)
#define		CFG_NUMINTWND		44		// Intensity of data reception window - ���������������� ���� �������� ��������� ���� "������������� ������ / �����" (������ ��� ������ � ������������)
#define		CFG_ERRINTWND		45		// Error rates window - ���������������� ���� �������� ��������� ���� "������������� ������ ������ / �����" (������ ��� ������ � ������������)
#define		CFG_POWERWND		46		// Battery life	- ���������������� ���� �������� ��������� ���� "����� ��� / �����" (������ ��� ������ � ������������)
#define		CFG_FONT			47		// ��������� ��������� ������
#define		CFG_EVTOTALSUMWND	48		// Sum of all impulses Wnd - ���������������� ���� �������� ��������� ���� "��������� ����� ��������� �� / �����"	
#define		CFG_RMSWND			49		// RMS window - ���������������� ���� �������� ��������� ���� "RMS / �����"
#define		CFG_ASLWND			50		// ASL window - ���������������� ���� �������� ��������� ���� "ASL / �����"

#define		CFG_SDOWND			100     // SDOWND configuration block - ���������������� ���� �������� ��������� ���� "������������� / �����"
#define		CFG_SPECTRWND		150		// spectrum window configuration block	- ���������������� ���� �������� ��������� ���� "��������� ������ / �����"
#define		CFG_PARAMETWND		200		// Paramet windows configuration block	- ���������������� ���� �������� ��������� ���� "<��������������� ����� �> / �����"
#define		CFG_PAGE			250		// Page configuration block
//CFG_ID ���� �� ������ ��������� ����������� ���
//CFG_XXX � ������� ����� �������� ����� +
// '����� ��������'� ������� ����� �������� �����	//24.07.00S
#define		CFG_PROFILE			251		// Profile configuration block	
#define		CFG_IMPLINES		252		// Import Line configuration block	
#define		CFG_SERVICE			253		// Controller settings block	
#define		CFG_SYNCVIEW		254		// SyncView window settings block - ���������������� ���� �������� ��������� ���� "���������� ��������� �������� ��������� ��"

#define NUM_PAR_PER_MOD	4

#define MAX_PAGENAMELENGTH	256

//	��� PreFilterData - ���������� �� �������������� � ���� ����� ������ ����������
#define MAX_FDEFS 4

//���������������� ���� CFG_SERVICE �������� ���������� � ���������� ������� ����������� ��������� ��������������� ������ ����������
//������ ����� CFG_SERVICE = NUM_DEVICES * NUM_TYPE * NUM_SERV * sizeof( ServDefMod )
#define NUM_TYPE	2
#define NUM_SERV	12
//NUM_DEVICES - ����� ������������ ����� (������ DDM-2)

#define NO_LOC		-1000000001
#define SEVERAL_LOC	-1000000002

#define		CFG_VER_MIN			0
#define		CFG_MAGIC			0x1515CFCF
#define		CFG_VER_MAJ			25	//16.02.06S //16.06.17S //18.12.18S

#define		CFG_VER_MAJ_2015	22	//06.02.15Kir

// data event output file
#define		EV_DATA_VER_MAJ			1
#define		EV_DATA_VER_MIN			0

// system event output file
#define		SYS_DATA_VER_MAJ		1
#define		SYS_DATA_VER_MIN		0

#define		NUM_TYPE				2					//01.06.11Kir
#define		NUM_SERV				12					//01.06.11Kir
#define		MAX_DATA_SIZE			28					//25.05.16S
#define		MAX_FILTER_FDEFS		4
#define		FLOAT_PRECISION			1000.

//FILTER_CONST
#define		MEASSUMM_DLG_TYPE		0
#define		MEASHDRSHOW_DLG_TYPE	1
#define		FILTERVIEW_DLG_TYPE		2

enum ComboBoxID_T {
	CMB_ID_NUMBER = 0,
	CMB_ID_TIME = 1,
	CMB_ID_CHANNEL = 2,
	CMB_ID_AMP = 3,
	CMB_ID_AMP_MKV = 4,
	CMB_ID_AMP_DB = 5,
	CMB_ID_AMP_RMS = 6,
	CMB_ID_AMP_RATIO = 7,
	CMB_ID_AMP_TIG = 8,
	CMB_ID_ENER_ADC = 9,
	CMB_ID_ENERG = 10,
	CMB_ID_EN_DB = 11,
	CMB_ID_EN_MARSE = 12,
	CMB_ID_DUR = 13,
	CMB_ID_RISE = 14,
	CMB_ID_COUNT = 15,
	CMB_ID_FREQ1 = 16,
	CMB_ID_FLAG_A = 17,
	CMB_ID_FLAG_D = 18,
	CMB_ID_FLAG_O = 19,
	CMB_ID_X = 20,
	CMB_ID_Y = 21,
	CMB_ID_Z = 22,
	CMB_ID_LA = 23,
	CMB_ID_TAD = 24,
	CMB_ID_LSTAR = 25,
	CMB_ID_CLAST = 26,
	CMB_ID_EVENT = 27,
	CMB_ID_EV_TOT = 28,
	CMB_ID_NOISE_DB = 29,
	CMB_ID_CONTEV = 30,
	CMB_ID_POWER = 31,
	CMB_ID_CNTRATE = 32,
	CMB_ID_PARAMDUR = 33,
	CMB_ID_NT_START = 34,
	CMB_ID_NT_SUCCESS = 35,
	CMB_ID_NT_FAILED = 36,
	CMB_ID_NT_BREAK = 37,
	CMB_ID_CT_START = 38,
	CMB_ID_CT_SUCCESS = 39,
	CMB_ID_CT_FAILED = 40,
	CMB_ID_CT_BREAK = 41,
	CMB_ID_CLB_START = 42,
	CMB_ID_CLB_SUCCESS = 43,
	CMB_ID_CLB_FAILED = 44,
	CMB_ID_CLB_BREAK = 45,
	CMB_ID_CLB_PAUSED = 46,
	CMB_ID_CLB_RESUME = 47,
	CMB_ID_CT_GAIN_START = 48,		//24.09.09 CH
	CMB_ID_CT_GAIN_CONT = 49,
	CMB_ID_CT_GAIN_SUCCESS = 50,
	CMB_ID_CT_GAIN_FAILED = 51,
	CMB_ID_CT_GAIN_BREAK = 52,
	CMB_ID_CLB_CHANNUM = 53,		//29.03.10 CH
	CMB_ID_PLATOFIXED = 54,
	CMB_ID_PLATOBREAK = 55,
	CMB_ID_PLATOYES = 56,
	CMB_ID_PLATONO = 57,
	CMB_ID_NETFAILED = 58,		//22.03.12S
	CMB_ID_SEVENT = 59,
	CMB_ID_LEVENT = 60,
	CMB_ID_DFTI1 = 61,
	CMB_ID_DUR_RISE = 62,
	CMB_ID_NUM_PACK = 63,
	CMB_ID_DTIME = 64,
	CMB_ID_AMP_SQV = 65,
	CMB_ID_FUNC_MAX = 66,
	CMB_ID_FUNC_MODE = 67,
	CMB_ID_FUNC_MEAN = 68,
	CMB_ID_ODDS_EQUAL = 69,
	CMB_ID_ODDS_MORE = 70,
	CMB_ID_ODDS_EQUAL_MORE = 71,
	CMB_ID_ODDS_LESS = 72,
	CMB_ID_ODDS_EQUAL_LESS = 73,
	CMB_ID_ACT_MULT = 74,
	CMB_ID_ACT_DIV = 75,
	CMB_ID_ACT_LG = 76,
	CMB_ID_ACT_LN = 77,
	CMB_ID_ACT_EXP = 78,
	CMB_ID_SUMM_SUM = 79,
	CMB_ID_SUMM_DED = 80,
	CMB_ID_AMP_NOISE = 81,
	CMB_ID_THIKNESS = 82,

	//None
	CMB_ID_NONE = 1000,

	//Parametric CMB_ID > 1000!!!
	CMB_ID_PARAMETRIC = 1001,
	CMB_ID_PARAMETRIC2 = 1002,
	CMB_ID_PARAMETRIC3 = 1003,
	CMB_ID_PARAMETRIC_END = 1004

	//
};

enum IsPackID {
	IS_PACK_NONE = 0,		// ��� �����
	IS_PACK_TRUE = 1,		// �����
	IS_PACK_DEAD_TIME = 2			// ������ �����
};

enum AeEventID {
	AE_EVENT_ALL = 0,		// ��� �������
	AE_EVENT_FIRST = 1,		// ������ �� ����.������
	AE_EVENT_MAX = 2,		// ����. �� ����.������
	AE_EVENT_START = 3			// ������ ��������� ������� ��
};

enum ExportDataID {
	EXPORT_DATA_AE = 0,		// ���� �� �� ���������
	EXPORT_DATA_PARAM = 1,		// ��������� �����.��������
	EXPORT_DATA_NOISE = 2,		// ���� � ����� � �������
	EXPORT_DATA_SYSTEM = 3,		// ���� � ��������� ��������
	EXPORT_DATA_SERVICE = 4,		// ���� ���������
	PARAM_DATA_FILTER = 5,		// �������� ���������� � ���� ����������
	PARAM_DATA_SYNCHRO = 6			// �������� ���������� � ���� ���������������
};

enum ParamDataID {							//����� ��������� ��������� � ����� ������!
	ID_NUMBER = 0,		//
	ID_DATE = 1,		//
	ID_TIME_ABS = 2,		//
	ID_TIME = 3,		//
	ID_CHANNEL = 4,		//�����������! ��� ����� ������!
	ID_AMP = 5,		//
	ID_AMP_MKV = 6,		//
	ID_AMP_DB = 7,		//
	ID_AMP_RMS = 8,		//
	ID_AMP_RATIO = 9,		//
	ID_AMP_TIG = 10,		//
	ID_ENER_ADC = 11,		//
	ID_ENERG = 12,		//
	ID_EN_DB = 13,		//
	ID_EN_MARSE = 14,		//
	ID_DUR = 15,		//
	ID_RISE = 16,		//
	ID_COUNT = 17,		//�����������! ��� �������!
	ID_FREQ1 = 18,		//

	ID_FLAGS = 19,		//�����������! ��� �����!
	ID_NUM_PACK1 = 20,		//
	ID_Y1 = 21,		//
	ID_Z1 = 22,		//
	ID_LA1 = 23,		//
	ID_TAD = 24,		//
	ID_AMP_NOISE = 25,		//
	ID_THRESH_DB = 26,		//
	ID_SIGNAL_TYPE = 27		//
};

//����� ����������� ���� ������� (loc_event)	//21.11.03S //18.06.19S
#define LOC_IDLE	0		//������ ��� �������/��� �������
#define LOC_DONE	1		//�������� �������
#define EV_PARAM	2		//��������������� �������
#define EV_SYS		3		//��������� �������
#define EV_NOISE	4		//������� �������
#define EV_DEL		5		//�� ��������� ��� ������� (������)
#define EV_MARKER	6		//��������� �������	//22.04.04S
#define UNI_PARAM	7		//��������������� ������� �� UNISCOPE //12.05.17S
#define UNI_AVG		8		//�������������� ������� �� UNISCOPE //18.06.19S
#define EV_PASS		16		//���������� ��� ����������� �������
#define EV_INCLUDE	0xEF	//�������� ��� ����������� �������


// CH_INFO_OUT possible types
#define	CHAN_INFO		3			// start channel
#define	CHAN_STOP		4			// stop channel
#define OSC1_INFO		5			// start of 1 OSC
#define OSC2_INFO		6			// start of 2 OSC
#define PF_INFO			9			// filters
#define PAR_INFO		10			// start parameters
#define PAR_STOP		11			// stop parameters
#define GAIN_INFO		14			// channel gain
#define CHANGAIN_INFO	18			// channel start&gain	//13.04.01S
#define OSC1_STOP		24			// stop of 1 osc
#define OSC2_STOP		25			// stop of 2 osc
#define CTRL_PAR_INFO	5			// ��������� ��������������� ������� PCI-8/PCI-N	//10.01.05S
#define CTRL_PAR_STOP	6			// ���������� ��������������� ������� PCI-8/PCI-N	//10.01.05S

#define SYNK_CHECK_CONST	429		// ����������� ��������� ���������� ������			//05.06.18S
#define SYNK_CHECK_NCONST	65106	// ����������� ��������� ���������� ������			//05.06.18S


#define MAX_TEXT_LENGTH	68
#define NUM_RPARAM	5			//27.05.15S


// SERVICE_OUT possible types	//30.01.11S
#define	AEC_ERROR_TYPE	9		// Action_SysMonitor
#define	TIMER_TYPE		6		// Action_Timer


#define LOC_STEP			1024	//256	//07.12.99S

#define	GETDATA_BUF_SIZE	65536	//14.12.06S //25.11.11S	
#define WRITE_BUF_SIZE		5120	//4096		//20.05
#define READ_BUF_SIZE		65536	//07.12.99S	//01.10.14S
#define MESSAGE_BUF			61		//18.12.00LS + �-�� ���������� ������., ����������� ����� SendMessage
#define BOUND_LEVEL			4096	//2048

#define NO_LOC		-1000000001	//17.06
#define SEVERAL_LOC	-1000000002	//17.06

#define XCOOR		0x01		//17.03.99S � ����� ������������ � ����������
#define XYCOOR		0x02		//17.03.99S � ����� ������������ � � Y ����������
#define XYZCOOR		0x03		//24.04.01S � ����� ������������ X, Y � Z ����������
#define LAMP		0x04		//17.03.99S � ����� ������������ ����������� ���������


namespace py = pybind11;

std::string wstring_to_ansi(const std::wstring& wstr);
std::string wstring_to_string(const std::wstring& wstr);
std::wstring string_to_wstring(const std::string& str);
template<typename ... Args>
std::string string_format(const std::string& format, Args ... args);
std::wstring get_wstr(char* l_str);
template <typename T>
std::vector<T> get_vector_from_arr(T* arr, int len);
double ExtractTimeQuant(WORD expnd, int shift);
// ������� ��� �������� FileHdr
void bind_FileHdr(py::module_& m);
// ������� ��� �������� FilterDef
void bind_FilterDef(py::module_& m);
// ������� ��� �������� FilterDef
void bind_PreFilterData(py::module_& m);
// ������� ��� �������� MeasData
void bind_MeasData(py::module_& m);
// ������� ��� �������� SDOHdr
void bind_SDOHdr(py::module_& m);
// ������� ��� �������� AE_TIME
void bind_AE_TIME(py::module_& m);
// ������� ��� �������� OSCDefMod
void bind_OSCDefMod(py::module_& m);
// ������� ��� �������� CfgFileHdr
void bind_CfgFileHdr(py::module_& m);
// ������� ��� �������� CfgBlockDescr
void bind_CfgBlockDescr(py::module_& m);
// ������� ��� �������� CfgFileInfo
void bind_CfgFileInfo(py::module_& m);
// ������� ��� �������� ColorMod
void bind_ColorMod(py::module_& m);
// ������� ��� �������� ChanDefMod
void bind_ChanDefMod(py::module_& m);
// ������� ��� �������� TimeDefMod
void bind_TimeDefMod(py::module_& m);
// ������� ��� �������� CalibrDefMod
void bind_CalibrDefMod(py::module_& m);
// ������� ��� �������� ParDefMod
void bind_ParDefMod(py::module_& m);
// ������� ��� �������� ald_drv_addr_wrapper
void bind_ald_drv_addr_t(py::module_& m);
// ������� ��� �������� FullDefMod
void bind_FullDefMod(py::module_& m);
// ������� ��� �������� MFData
void bind_MFData(py::module_& m);
// ������� ��� �������� ScaleData
void bind_ScaleData(py::module_& m);
// ������� ��� �������� GraphData
void bind_GraphData(py::module_& m);
// ������� ��� �������� AlarmData
void bind_AlarmData(py::module_& m);
// ������� ��� �������� GeneralSetupMod
void bind_GeneralSetupMod(py::module_& m);
// ������� ��� �������� AutoThresholdSetup
void bind_AutoThresholdSetup(py::module_& m);
// ������� ��� �������� HotKeyData
void bind_HotKeyData(py::module_& m);
// ������� ��� �������� CPostWin
void bind_CPostWin(py::module_& m);
// ������� ��� �������� Graph3DData
void bind_Graph3DData(py::module_& m);
// ������� ��� �������� CHistWin
void bind_CHistWin(py::module_& m);
// ������� ��� �������� NetIPInfo
void bind_NetIPInfo(py::module_& m);
// ������� ��� �������� ClassAmpDef
void bind_ClassAmpDef(py::module_& m);
// ������� ��� �������� PageName
void bind_PageName(py::module_& m);
// ������� ��� �������� PageData
void bind_PageData(py::module_& m);
// ������� ��� �������� ILData
void bind_ILData(py::module_& m);
// ������� ��� �������� ServDefMod
void bind_ServDefMod(py::module_& m);
// ������� ��� �������� EV_DATA_OUT_OLD
void bind_EV_DATA_OUT_OLD(py::module_& m);
// ������� ��� �������� CH_INFO_OUT
void bind_CH_INFO_OUT(py::module_& m);
// ������� ��� �������� EV_DATA_OUT
void bind_EV_DATA_OUT(py::module_& m);
// ������� ��� �������� EV_PARAM_OUT
void bind_EV_PARAM_OUT(py::module_& m);
// ������� ��� �������� EV_AVG_OUT
void bind_EV_AVG_OUT(py::module_& m);
// ������� ��� �������� MARKER_OUT
void bind_MARKER_OUT(py::module_& m);
// ������� ��� �������� TEXT_OUT
void bind_TEXT_OUT(py::module_& m);


// ������ ����� ������������ �� ������ A-Line DDM, DDM-2, PCI-N � PCI8 ��� ���� ������ //created 08.05.2020

// ������� ������� ��������� ����� ������ TAGFileHdr � ��������� ��������� TAGMeasData
////////////////////////////////////////////////////////////////
struct FileHdr
{
	UINT signature;         // signature = 0xAA551313
	UINT ver_maj;           // version major = 22 ��� ������� ������ ������
	UINT ver_min;           // version minor = 0 ��� ������� ������ ������
	UINT ftype;             // type of file	= 0xAA02 ��� ����� ������������ osc-����
	UINT hard_version;		// version of hardware controller //������� ������ DDM-2 ����������� = 0x102071f (����������� �� ���������� �����)
	UINT soft_version;		// version of sort options //������� ������ �� A-Line = 0x20600
	UINT reserved[18];      // reserved bytes for future use

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 18 }, reserved);
	}
};


struct FilterDef
{
	UINT	param;					// parameter for filtration
	double	more_than[3];			// 3 minimal value interval
	double	less_than[3];			// 3 maximum value interval
	char	p_name[100];			// name of parameter
	byte	valid;					// validiti of filter			
	byte	interv_valid[3];		// 3 value validiti of interval	

	py::array_t<double> get_less_than() {
		return py::array_t<double>({ 3 }, less_than);
	};
	py::array_t<double> get_more_than() {
		return py::array_t<double>({ 3 }, more_than);
	};
	std::wstring get_p_name() {
		return get_wstr(p_name);
	};
	py::array_t<byte> get_interv_valid() {
		return py::array_t<byte>({ 3 }, interv_valid);
	};
};



struct PreFilterData
{
	byte		DontSave;			// don't save events not passed throuth filter
	byte		ver_PFilter;		// 0-old, 1-new 
	byte		reserved[2];
	BOOL		DontProcess;		// don't process events not passed throuth filter
	BOOL		IsKeep;				// keep selected events or delete them
	BOOL		AndOr[MAX_FDEFS];	// massiv or/and switches for conditions
	FilterDef	FltDef[MAX_FDEFS];	// set of filters

	py::array_t<byte> get_reserved() {
		return py::array_t<byte>({ 2 }, reserved);
	};
	py::array_t<BOOL> get_AndOr() {
		return py::array_t<BOOL>({ MAX_FDEFS }, AndOr);
	};
	py::array_t<FilterDef> get_FltDef() {
		return py::array_t<FilterDef>({ MAX_FDEFS }, FltDef);
	};
};



////////////////////////////////////////////////////////////////
struct MeasData
{
	UINT ver_maj;                   // version number major //������
	UINT ver_min;                   // version number minor //������
	char obj_name[255];             // object name
	char oper_name[255];            // operator name
	char reference[255];            // references
	UINT comsize;                   // comments size
	char comments[1000];            // comments text
	UINT filtered_flag;				// flag of filtered file: 0-source, 1-filtered, 2-location filter, 3-concatenation result, 4-clb_location filter
	UINT meas_hour;                 // measurement maximal hours
	UINT meas_min;                  // and minutes
	UINT init_hour;                 // initial scale hour
	UINT init_min;                  // initial scale min
	__time32_t start_time;          // start time
	__time32_t stop_time;           // stop time
	UINT avg_time;                  // averaging time in microseconds
	char data_name[255];            // file name to store data
	char cfg_name[255];             // file name to store configuration
	char sdo_name[255];             // file name to store SDO wave
	char source_name[255];          // file name of source file for secondary generation
	char local_name[255];           // file name of localization file
	UINT num_SDO;                   // number of SDO wave to write
	BOOL DontWriteDisk;             // prevent writing data to disk in real-time
	BOOL DontWritePar;              // prevent writing parametric to disk in real-time
	BOOL DontWriteNoise;            // prevent writing noise data to disk in real-time
	BOOL DontWriteSDO;              // prevent writing SDO wave to disk in real-time
	BOOL ActivatePreFilter;			// use pre-filter during on-line
	PreFilterData	PFData;			// struct of prefilter data used
	UINT IsCoorPresent;				// flag of localization coordinates presence: 1-X, 2-X,Y
	UINT TypeLoc;					// type of local.scheme (linear = 0, plane = 1, volume = 2, sphere = 3, cover =  4, threeD = 5, zonary = 6, picture = 7, cone = 8, rvs_wall = 9, calibr = 10)
	LONG LENGTH;					// size of local.sheme: x_max;
	LONG HEIGHT;					// y_max;
	LONG SHIFT_HEIGHT;				// y_min;
	LONG SHIFT_LEFT;				// x_min;
	UINT ClusterLength;				// length of the cluster
	UINT ClusterWidth;				// width of the cluster
	UINT BOUND;						// location Region Bound
	int  SumEvents;					// (0 -> ��� ����������; -1 -> 0 �������)
	UINT MeasType[4];				// ��������� �������� ��������� ���������: (0 - MaxEvents, 1 - MaxTigs, 2 - MaxFileSize, 3 - MaxOscNum)
	short IsMeasTime;				// ������� ��������� �� ��������� ������� ���������
	short IsEvents;					// ������� ��������� �� �������� ���������� ������������� ����� ��������� �� - MaxEvents
	short IsTig;					// ������� ��������� �� �������� ���������� ������������� ����� �������� �� - MaxTigs
	short IsSize;					// ������� ��������� �� �������� ���������� ������������� ������� ����� ������ - MaxFileSize
	short IsOSC;					// ������� ��������� �� �������� ���������� ������������� ����� ������������ - MaxOscNum
	short IsRepeat;					// ���� ��������������� ����������� ��������� ����� ��������� �����������
	UINT  Type;						// ��� �������������� �������� (0 - manual stop (default), 1 - max events, 2 - max tig, 3 - max file size, 4 - max osc num, 5 - max time expired)
	UINT Reserved[2];				// reserved

	std::wstring get_obj_name() {
		return get_wstr(obj_name);
	};
	std::wstring get_oper_name() {
		return get_wstr(oper_name);
	};
	std::wstring get_reference() {
		return get_wstr(reference);
	};
	std::wstring get_comments() {
		return get_wstr(comments);
	};
	std::wstring get_data_name() {
		return get_wstr(data_name);
	};
	std::wstring get_cfg_name() {
		return get_wstr(cfg_name);
	};
	std::wstring get_sdo_name() {
		return get_wstr(sdo_name);
	};
	std::wstring get_source_name() {
		return get_wstr(source_name);
	};
	std::wstring get_local_name() {
		return get_wstr(local_name);
	};
	py::array_t<UINT> get_MeasType() {
		return py::array_t<UINT>({ 4 }, MeasType);
	};
	py::array_t<UINT> get_Reserved() {
		return py::array_t<UINT>({ 2 }, Reserved);
	};
};

//	��� PreFilterData - ���������� �� �������������� � ���� ����� ������ ����������



////////////////////////////////////////////////////////////////
// ����� ������� ������������ ���� ����� ������������
////////////////////////////////////////////////////////////////
struct SDOHdr
{
	UINT NumOSC;			// ����� ����� ���������� ������������
	UINT InfoPointer;		// ��������� �� ��������� ���� ������ ������������ � ����� (���� ����� ���������� ������������ ���������)
	UINT DefSize;			// ������ ������ ������������, ������������� �� ��������� (����������� �� ���������� � � ��������� ������� �� �� ������������)
	UINT BaseSize;			// ������ ��������� OSCDefMod (��. ����)
	UINT Din;				// �������� ������� ��� � ��
	UINT reserved[16];      // reserved bytes for future use

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 16 }, reserved);
	};
};



////////////////////////////////////////////////////////////////
// ����� ������� ���� ������������ � ������� ��� ������ ��������������� ���� ������ (��. "������ ����� cfg 6.0.cpp")
////////////////////////////////////////////////////////////////


typedef unsigned long ulong;
struct AE_TIME
{
	__time32_t	seconds;	// absolute seconds value suitable for C time functions
	ulong	usec;			// microseconds since last second
};


///////////////////////////////////////////////////////////////
// ����� ���������� ������
// ������� ��� ���������-��������� �������������, ����� ������
// ����� �� �� ����� ������ ������������ SDOHdr.NumOSC
// ���� seek_wave � seek_wave_high - ��������� ��� ��������� ������ ������������ ������������� � ����� osc
struct OSCDefMod
{
	UINT		chan;			//����� ������
	UINT		num_osc;		//0-A, 1-B 	
	BOOL		enabled;		//������� OSC (A ��� B)
	COLORREF	osc_color;		//����� ����������� osc
	BOOL		is_color_chan;	//���� ����������� ������ ������
	BOOL		save;			//���� ����������
	UINT		num_wind;		//����� ���� ��� �����������
	UINT		freq;			//�-�� �������������, ���
	UINT		buf_size;		//���������� �����
	UINT		buf_time;		//������������ ������, ��� (���������)
	UINT		pre_mod;		//���-�� ����� ��������������
	UINT		sync;			//0 - �����������, 1 - ���������� ������, 2 - ���������� �����, 3 - ���������� ������/�����
	BOOL		is_th_chan;		//���� ������������� ������ ������
	float		tresh_db_osc;	//���������
	UINT		tresh_ADC_osc;	//��������������� �������� ��������������� ������ �������������
	double		K_mkV;			//��� ���������� ��������� � ��� � ��		
	UINT		seek_wave;		//�������� ������ ������������� � ����� (������� 32-������ �����)		
	AE_TIME		osc_time;		//����� ������� ������������� ����������	
	UINT		ADC_bit;		//����������� ���							
	BOOL		is_spect;		//���� ���������\���������� �������
	BOOL		changed;		//���� ����� ������������� ���������
	UINT		freq_max;		//������������ ������� ��������� ������, ���
	BOOL		notuse_pre_mod;	//���� ������������� ������ ��������������
	UINT		buf_size_max;	//������������ ������ ������
	UINT		seek_wave_high;	//�������� ������ ������������� � ����� (������� 32-������ �����)
	long		tFastest;		//����� ������� ����� ������� ����� (������������ ������ � ALine OSC Processing)
	long		tR;				//����� ������� ����� ����� (�����������, ������������ ������ � ALine OSC Processing)
	int			calibration;	//���� ��������� ������ ����������
	int			to;				//����� ��������� (������������ ������ � ALine OSC Processing ��� ������� ������������� ������)
	int			to_ald;			//����� ��������� �� ald (������������ ������ � ALine OSC Processing ��� ������� ������������� ������)
	float		L;				//���������� �� ��������� ���������	(������������ ������ � ALine OSC Processing ��� ������� ������������� ������)
	float		L_ald;			//���������� �� ��������� ��������� �� ald (������������ ������ � ALine OSC Processing ��� ������� ������������� ������)
	float		d;				//������� ������� (������������ ������ � ALine OSC Processing ��� ������� ������������� ������)
	WORD		bPowerMethod;	//���� ��������� ���������� ��������� (������ ��� 18-������ ������, � ��� ����� DDM-2)
	WORD		nsec;			//����� �������� ���������� ��������� � ������� ����� ���������� ������������ (DDM-2), ���������� �� ������� ������� osc_time
	BOOL		bSyncAEO;		//���� ������������� AE-OSC � �1� ��������� ������������� (������ ��� DDM-2)
	UINT		reserved[2];

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 2 }, this->reserved);
	};
};





// ������ ����� ������������ �� ������ A-Line DDM, DDM-2, PCI-N � PCI8 ��� ���� ������ //created 08.05.2020

////////////////////////////////////////////////////////////////
//��������� ����� ������������
struct CfgFileHdr
{
	UINT magic_word;            // magic word = 0x1515CFCF - ������ ��� ������ cfg
	UINT ver_maj;               // major version number = 25- ��� ������� ������ �����
	UINT ver_min;               // minor version number = 0	- ��� ������� ������ �����
	UINT info_size;             // ����� ������ ������ ���������������� ������
	UINT reserved[20];          // reserved bytes for future   

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 2 }, this->reserved);
	};
};



////////////////////////////////////////////////////////////////
//���������������� ����-�����
struct CfgBlockDescr
{
	UINT block_type;            // block type;
	UINT block_offset;          // offset to the block in bytes from 
	// cfg file info structure
	UINT block_size;            // cfg block size
};

struct CfgFileInfo
{
	UINT num_cfg_blocks;        // number of configuration blocks in file
	UINT mem_size;              // size of configuration data
	CfgBlockDescr* cfg_blocks; // unsized array of descriptors;
};


//����� ����� ����� CfgFileInfo ����� CfgFileHdr.info_size ����

////////////////////////////////////////////////////////////////
//����� ������� ����� "configuration data" ����� ������ ������ CfgFileInfo.mem_size ����,
//����� ������� �� num_cfg_blocks ������, ������ �� ������� ��������������� ����� ������������ CfgBlockDescr.
//� ����������� � block_type = X �� ������� �������� block_offset - �������� ������ ����� X ������������ ������ ������ "configuration data"
//� �������� block_size - ������ ����� hardware configuration.




struct ColorMod
{
	COLORREF	ch_color;
	BOOL		changed;
	UINT		reserved[16];

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 16 }, this->reserved);
	};
};

struct ChanDefMod
{
	BOOL		enabled;		//��������� (0-��������)
	BOOL		show;			//����������� (0-�� ������������)
	UINT		high_filter;	//���, ���
	UINT		low_filter;		//�H�, ���
	long		gain_db;		//��������� �������� �������� ������
	float		d_ADC;			//����������� �� ��� ������ �������������� ���	
	float		tresh_db;		//���������
	UINT		tresh_ADC;		//��������������� �������� ������ ������ (� ��. ���)
	BOOL		changed;		//���� ����� ������������� ���������
	BOOL		changed_ON;		//���� ����� ������������� ��������� ��������� enabled
	BOOL		autotresh;		//���� ��������� ����������
	UINT		reserved[14];

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 14 }, this->reserved);
	};
};

struct TimeDefMod
{
	UINT		sceto;			//��� (SCETO)
	UINT		rtto;			//RTTO - �� ������������ (������ ����� max_dur)
	UINT		d_time;			//������� �����
	UINT		max_dur;		//������������ ������������
	BOOL		changed;		//���� ����� ������������� ���������
	UINT		reserved[16];

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 16 }, this->reserved);
	};
};

struct CalibrDefMod
{
	BOOL		enabled;		//������� ����������
	UINT		K_calibr;		//�����-� ����. (���) - �� ������������
	float		d_DAC;			//����������� �� ���, �.�. =((float)REF_DAC / (float)DAC_BIT)
	UINT		Amp_v;			//���������, B
	UINT		Amp_DAC;		//��������������� �������� ��������� �������������� �������
	UINT		Freq_Calibr;	//�� ������������
	BOOL		changed;		//���� ����� ������������� ���������
	BOOL		changed_ON;		//���� ����� ������������� ��������� enabled
	UINT		mode;			//������������� ����� ������ ����������� ��� DDM-2 (0�passive,not sync osc / 1�active,radiate / 2- passive,sync osc / 3 - AST)
	UINT		Dur_DAC;		//������������ �������� ������ �������������� AST
	float		F1;				//�������� ����� ������� ������ ������� ��� ������� ����������
	float		F2;				//�������� ������ ������� ������ ������� ��� ������� ����������
	float		Amin;			//����������� �������� ��������� ����������������� ��������
	float		Smin;			//����������� �������� ������������� ��������� ����������� ������� �������
	float		Smax;			//������������ �������� ������������� ��������� ����������� ������� �������
	UINT		PAEtype;		//��� ��������������� � ������ �������										
	UINT		reserved[7];

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 7 }, this->reserved);
	};
};

struct ParDefMod		//��� DDM-2 ���� �� ������������
{
	BOOL		enabled;		//������� ���. �����
	float		K_par;			//�����-� ����. (���)
	float		d_ADC_par;		//����������� �� ���, �.�. =((float)REF_ADC_PAR / (float)ADC_BIT_PAR)
	WORD		Freq_par;		//������� ������ ������, Hz
	_int16		Freq_multi;		//��������� ��������� ������� ������ ����������
	double		K1;				//����������������� ����������� ��������� ������� ��������������� ���������
	double		K2;				//���������� ����������� ��������� ������� ��������������� ���������
	BOOL		changed;		//���� ����� ������������� ���������
	BOOL		changed_ON;		//���� ����� ������������� ��������� enabled
	_int16		lMin;			//����������� �� ������� �������� ������ ������� ��������������� �����
	_int16		lMax;			//����������� �� ������� �������� ������� ������� ��������������� �����
	UINT		reserved[14];

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 14 }, this->reserved);
	};
};

union ald_drv_addr_t {
	DWORD	FAddr;	/* Full address */
	struct {
		WORD	Channel;					// ����� ������
		WORD	Board;						// ����� �����������
	} bc;
};
// ������� ��� union
class ald_drv_addr_wrapper {
public:
	ald_drv_addr_t addr;

	ald_drv_addr_wrapper() : addr() {}

	DWORD get_FAddr() const { return addr.FAddr; }
	void set_FAddr(DWORD value) { addr.FAddr = value; }

	WORD get_Channel() const { return addr.bc.Channel; }
	void set_Channel(WORD value) { addr.bc.Channel = value; }

	WORD get_Board() const { return addr.bc.Board; }
	void set_Board(WORD value) { addr.bc.Board = value; }
};


//��������� hardware configuration block:
//�� ��������� CFG_HARDWARE = 1 ��������� hardware configuration block, ������� ������� �� ���������� ����� ��������-���������� �������:
//FullDefMod, ����� ���� �������� ��������� � ������ ��������������� � ������� ������� � ������������ ������������
struct FullDefMod
{
	UINT		 chan;
	ColorMod	 color_def;					//��������� �����
	ChanDefMod	 chan_def;					//�������� ��������� ������
	TimeDefMod	 time_def;					//��������� ��������� ������
	OSCDefMod	 osc_def[2];				//��������� ������������������ ������� (������ A � B)
	CalibrDefMod calibr_def;				//���������� - � �������� PCI-N �� ������������
	ParDefMod	 par_def[NUM_PAR_PER_MOD];	//��������������� ������ (� ������ PCI-N ������������ ������ ���� ������ ����� ������ �����)
	ald_drv_addr_t addr;					//����� ������
	WORD		 DeviceName;				//��� ����� (��� DDM-2 ��� �����), � ������� ����������� �����
	WORD		 SubDeviceName;				//��� ����� ��� DDM-2, � ������� ����������� �����
	UINT		 num_Rmod;					//����� ���������� ��� ������ (������ ��� ������ � ������������)
	WORD		 gpre;						//����������� �������� ������������� (��� DDM-2 �� ������������)
	_int16		 shift0;					//�������� ���� ��� (��� DDM-2 �� ������������)
	WORD		 DeviceType;				//��� ������� (0,1 - DDM, 2 - PCI4, 3 - PCI8, 4 - PCI8E, 5 - PCI8S)
	WORD		 FilterType;				//��� ������� 1-������������ �������, 2-������� ������������, 4-������ �������, 8-��������� ���������, 16-������� DDM-2
	WORD		 VersionModule;				//������ ������ (������)
	WORD		 Capability;				//capabilities (�����������) ������ (������)
	WORD		 reserved[22];				//reserved

	std::vector<OSCDefMod> get_osc_def() {
		return get_vector_from_arr<OSCDefMod>(this->osc_def, 2);
	};
	std::vector<ParDefMod> get_par_def() {
		return get_vector_from_arr<ParDefMod>(this->par_def, NUM_PAR_PER_MOD);
	};
	py::array_t<WORD> get_reserved() {
		return py::array_t<WORD>({ 22 }, this->reserved);
	};
};



/////////////////////////////////////////////////////
//���������������� ����� ��������� ��������� ���� XXX ������ ��� ���������� ��������� � ������� ���� XXX �� ��������� �����������

//��� �������� ���� ��������� CFG_MAINFRAME �������� ��������� ���� MFData 
//������ ����� CFG_MAINFRAME = sizeof ( MFData )
struct MFData
{
	RECT rc;                // screen coordinates
	WINDOWPLACEMENT wp;     // window placement structure
};


//��� ��������� ���� ���������� �������� ������� �� ���������� ������� ����������� �� ����� CFG_PAGE
//������ ���� �� ����������� 2-� ����������������� ����� ������������� �� ����� ����� ������� ���������� � ����� ������� � ���������
//������ ����� = QueryCfgSize * NumPages
// NumPages - ����� ������� ����������� ����������� ���� (��. �������� CFG_PAGE)

struct  ScaleData
{
	long		lMin;			// scale min
	long		lMax;			// scale max
	BOOL		bMinAuto;		// set min automatically
	BOOL		bMaxAuto;		// set max automatically
	BOOL		bIsShown;		// Is scale visible
	BOOL		bIsStPos;		// Is in standard position
	COLORREF	cBackColor;		// Background color
	COLORREF	cTextColor;		// Text color
	DWORD		dwMajor;		// Major ticks
	DWORD		dwMinor;		// Minor ticks
	BOOL		bMajorAuto;		// Major automatic
	BOOL		bMinorAuto;		// Minor automatic
	char		label[30];		// label text	//29.06
	UINT		unit;			// 1-ADC, 2-mkV, (3-dB.), 4-sek., 5-min., 6-hour, 7-channel number;
	float		lMin_mkV;		// ������ ��� SDO (Y, unit = 2)
	float		lMax_mkV;		// ������ ��� SDO (Y, unit = 2)
	BOOL		bShowThresh;	// ������ ��� SDO (Y, unit = 2)
	int			IsLog;			// 0-�������� ���, 1-���������������
	WORD		bNormal;		// ���� ���������� ��� �����������, FALSE - ���, TRUE - ����
	WORD		bMarker;		// ���� ������� ����, FALSE - ���, TRUE - ����
	BOOL		bScroll;		// ���� ���������� ��� �������, FALSE - ���, TRUE - ����
	UINT		uScrollSize;	// ������ �������������� �������
	UINT		reserved[2];
	BOOL		bShowLabel;		// Show/Hide label

	std::wstring get_label() {
		return get_wstr(this->label);
	}
	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 2 }, this->reserved);
	};
};

struct GraphData
{
	COLORREF	cBackColor;		//���� ����
	BOOL		cIsLines;		//���� ����������� �����/�����
	BOOL		cIsGrid_x;		//����������� ��� X
	BOOL		cIsGrid_y;		//����������� ��� Y
	COLORREF    cColor_x;		//���� �������.
	COLORREF    cColor_y;		//���� �������.
	BOOL		cIsGrid_clast;	//����������� �����
	BYTE		reserved[24];

	py::array_t<BYTE> get_reserved() {
		return py::array_t<BYTE>({ 24 }, this->reserved);
	};
};


//////////////////////////////////////////////////
//���������������� ���� ��������� ��������� ��������� CFG_ALARMS �������� ����������� ��������� ������ � ���������� ���������� � ����������� �������������
//������ ����� CFG_ALARMS = NUM_ALARMS * sizeof ( AlarmData )
struct AlarmData
{
	char		AlarmName[80];		//��� ���������� ��������� ��� ������������� � ������� ������
	char		WaveName[MAX_PATH];	//���� ��� ��������������� ��������� wav-�����
	FilterDef	FltDef[MAX_FDEFS];	//��������� ��� ����������� ����� ��������� �������
	BOOL		AndOr[MAX_FDEFS];	//���������� ����� ��� ��������� ������� � ��������� �����
	LPVOID		buf;				//��������� �� ����� ��� ������������ �������� ����������� wav-�����
	char		bMarkerSet;			//���� ��������� ������� �� ��������� �����
	char		bActivated;			//���� ��������� ������ �������
	char		bSound;				//���� ������������� ��������� �������������
	char		bPicture;			//���� ������������� 2D-3D �������� ��� ��������� ������������ �������
	char		bPicOff;			//���� �������� �� ��������� ���. �������
	char		bPrevMarkerON;		//���� ��������� ������� �� ��������� ����� �� ������� � ������� ������� 
	//(�������������� ������������� ������ ������� ��� �������������� ������� ����������� ������)
	WORD		NumLocGroup;		//����� ���. ������, ���������� ������ ����������� �������� ��� ���� ���� ����������� ��
	UINT		NumLocDist;			//����� ������������� ������������ ������� � ���.������ NumLocGroup
	UINT		Reserved[16];

	std::wstring get_AlarmName() {
		return get_wstr(this->AlarmName);
	};
	std::wstring get_WaveName() {
		return get_wstr(this->WaveName);
	};
	std::vector<FilterDef> get_FltDef() {
		return get_vector_from_arr<FilterDef>(this->FltDef, MAX_FDEFS);
	};
	std::vector<BOOL> get_AndOr() {
		return get_vector_from_arr<BOOL>(this->AndOr, MAX_FDEFS);
	};
	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 16 }, this->Reserved);
	};
};


//////////////////////////////////////////////////
//���������������� ���� CFG_GENERAL �������� ��������� ��� �������� �������������� ����������� ���������� �������
//������ ����� CFG_GENERAL = sizeof ( GeneralSetupMod )
struct GeneralSetupMod
{
	char		name_par[NUM_PAR_PER_MOD][256];	// �������� ���� ����������� ������ � ��������������� �������
	UINT		PageSize;						// ������ �������� �������
	UINT		Din;							// ������� ���������� � ��
	int			SingleNumber;					// ���� ��������/������� ��������� �������
	char		ChanSep[2];						// ����������� ��� ������� ���������
	char		LocSep[2];						// ����������� ��� ��������� ���������� ������� � �������
	BOOL		AutoSaveCfg;					// �������������� ����� ������������
	BOOL		OSCAutoScale;					// ��������������� OSC �� ��������� �������������
	BOOL		SaveService;					// ���������� ��������� ��������������� ������
	BOOL		OnLineSpect;					// ���������� ������� � On-Line
	BOOL		UpgradeSpect;					// ��������� ���������� ������ � �������������
	COLORREF	GenColor;						// ���� ������� ��� ��������� ����� ��������
	BOOL		ParReadCfg;						// ������ ������������ ��������������� ������� �� �������
	BOOL		NoPauseOSC;						// �� ������������� ������ ������������ � ������ �����
	BOOL		RMSOff;							// �� ��������� � ���������� RMS � ���� ������������
	BOOL		ProtOn;							// ���� ������� ���������
	int			LineThickness;					// ������� ����� ������ �� �������� (0 - min)
	BOOL		AnalizInfoMod;					// ���� ������ �������������� ��� ������ �������� �������
	BOOL		WriteServiceInfo;				// ���� ���������� ��������� ��������� ���������� � �����
	WORD		InfoTimeout;					// ������� �������� �������������� �������
	WORD		StartTimeout;					// ������� �������� ���������� ������
	BOOL		TempMonitor;					// ���� ����������� ����������� ���������� �����
	BOOL		UseComAlarm;					// ���� ������� ��������� ��������� �� COM-�����
	UINT		AlarmComPort;					// ����� COM-����� ��� ������� ��������� ���������
	BOOL		SyncStatic;						// ���� ������������ ������ ��� ����������� ���������
	BOOL		ManualCmd;						// ���� ���������� ����� ������ ������ ������
	BOOL		ClbSyncOsc;						// ���� ����������� ��������� ������������ ��� ����������
	BOOL		GetAllVer;						// ���� ������� ������� ��������� ������ ����������
	UINT		reserved[240];
	int			Unit;							//����� ����� ������ ��������� [��, ��, ��, �]
	char		ProtFileName[256];				// ��� �����-���������
	char		name_reserv[NUM_PAR_PER_MOD - 1][256];	//reserved

	std::vector<std::wstring> get_name_par() {
		std::wstring str;
		std::vector<std::wstring> vect;
		for (size_t i = 0; i < NUM_PAR_PER_MOD; i++)
		{
			str = get_wstr(this->name_par[i]);
			vect.push_back(str);
		}
		return vect;
	};
	std::wstring get_ChanSep() { return get_wstr(this->ChanSep); };
	std::wstring get_LocSep() { return get_wstr(this->LocSep); };
	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 240 }, this->reserved);
	};
	std::wstring get_ProtFileName() { return get_wstr(this->ProtFileName); };
	std::vector<std::wstring> get_name_reserv() {
		std::wstring str;
		std::vector<std::wstring> vect;
		for (size_t i = 0; i < NUM_PAR_PER_MOD - 1; i++)
		{
			str = get_wstr(this->name_par[i]);
			vect.push_back(str);
		}
		return vect;
	};
};


//////////////////////////////////////////////////
//���������������� CFG_AUTOSETUP �������� ��������� ��� �������� ����������� ���������� ��������������� ������
//������ ����� CFG_AUTOSETUP = sizeof ( AutoThresholdSetup )
// structure for update auto threshold setup parameters
struct AutoThresholdSetup
{
	BOOL	m_bAuto;					//on/off auto threshold
	int		m_normal_delta;				//delta of normal threshold change
	int		m_alarm_delta;				//delta of alarm threshold increase
	int		m_length;					//length of storage buffer to apply
	int		m_th_delta;					//���� ���������� ����� ���������� �� ������ ������, ��� ��� ��������, �� ������ �� ������.
	int		m_th_min_ADC;				//min threshold, ADC
	UINT	reserved[16];

	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 16 }, this->reserved);
	};
};

//////////////////////////////////////////////////
//���������������� ���� CFG_MEASDATA �������� ��������� ��� �������� ����������� ���������� ������ ���������
//������ ����� CFG_MEASDATA = sizeof ( MeasData ) ��. �������� ����

//////////////////////////////////////////////////
//���������������� ���� CFG_SYSTEMINFO �������� ��������� ������ � ����������� �� ���������� ����� ��������� � ������ �� ��� ������������� �����

//////////////////////////////////////////////////
//���������������� ���� CFG_HOTKEYS �������� ��������� ��� �������� ����������� ���������� ������� ������ ��
//������ ����� CFG_HOTKEYS = num_hotkeys * sizeof ( HotKeyData )
struct HotKeyData
{
	WORD	m_wVK;
	WORD	m_wMod;
	DWORD	m_nCmd;
};


// NumPostWins - ����� ����������� ����
// NumPages - ����� ������� ����������� ����������� ���� (��. �������� CFG_PAGE)
class CPostWin
{
	int		axis_x;			//����� ��������� �� �� ��� X
	int		axis_y;			//����� ��������� �� �� ��� Y
	int		axis_z;			//����� ��������� �� �� ��� Z
	DWORD	par_chan;		//����� ���������������� ������
	UINT	reserved[8];	//reserved for future need

public:
	int get_axis_x() { return this->axis_x; };
	int get_axis_y() { return this->axis_y; };
	int get_axis_z() { return this->axis_z; };
	DWORD get_par_chan() { return this->par_chan; };
	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 8 }, this->reserved);
	};
};


struct Graph3DData
{
	COLORREF	cBackColor;		//���� ����
	BOOL		cIsGrid_x;		//����������� ��� X
	BOOL		cIsGrid_y;		//����������� ��� Y
	COLORREF    cColor_x;		//���� ������
	COLORREF    cColor_y;		//���� ������
	BOOL		cIsGrid_clast;	//����������� �����
	COLORREF	cObjectColor;	//���� ����	
	BOOL		cIsGrid_z;		//����������� ��� Z
	BYTE		reserved[20];

	py::array_t<BYTE> get_reserved() {
		return py::array_t<BYTE>({ 20 }, this->reserved);
	};
};



// NumHistWins - ����� ����������� ����
// NumPages - ����� ������� ����������� ����������� ���� (��. �������� CFG_PAGE)
class CHistWin
{
	int		axis_x;			//����� ��������� �� �� ��� X
	int		axis_y;			//����� ��������� �� �� ��� Y
	float	step_x;			//������ �������������� ��������� �� ��� X
	float	step_y;			//������ �������������� ��������� �� ��� Y
	int		type;			//��� �������������: 0 - ����������������, 1 - ������������
	BOOL	bSummed;		//���� ����������� �� �������
	UINT	reserved[6];	//reserved for future need

public:
	int get_axis_x() { return this->axis_x; };
	int get_axis_y() { return this->axis_y; };
	float get_step_x() { return this->step_x; };
	float get_step_y() { return this->step_y; };
	int get_type() { return this->type; };
	BOOL get_bSummed() { return this->bSummed; };
	py::array_t<UINT> get_reserved() {
		return py::array_t<UINT>({ 6 }, this->reserved);
	};
};


//////////////////////////////////////////////////
//���������������� ���� CFG_IPDEVICE �������� ���������� � ���������� ������� ����������� ������ ����� � ��������� ������ ���� EtnernetBox (����� num_ipdevices �� ����������)
//������ ����� CFG_IPDEVICE = num_ipdevices * sizeof ( NetIPInfo )
struct NetIPInfo	//��������� ���������� ��������� �������� �������������
{
	BYTE		IPAddress[4];	//IP-����� ����������
	DWORD		pID;			//����������������� ����� ����������
	int			bStatus;		//-1 - ������������, 0 - connected, 1 - discinnected
	WORD		MinLineAddr;	//��������� ����� ����� � ������ ����������
	WORD		TimeElapsed;	//����� ���������� �� ����������
	WORD		LineNums[8];	//������ ������������ � ���������� �����
	WORD		LineAddr[8];	//������ ������������ � ���������� ����� ��� ������������
	DWORD		reserved[10];

	std::vector<BYTE> get_IPAddress() {
		return get_vector_from_arr<BYTE>(this->IPAddress, 4);
	};
	std::vector<WORD> get_LineNums() {
		return get_vector_from_arr<WORD>(this->LineNums, 8);
	};
	std::vector<WORD> get_LineAddr() {
		return get_vector_from_arr<WORD>(this->LineAddr, 8);
	};
	std::vector<DWORD> get_reserved() {
		return get_vector_from_arr<DWORD>(this->reserved, 10);
	};
};


//////////////////////////////////////////////////
//���������������� ���� CFG_AMPCRIT �������� ���������� � ���������� ������������ �������� ������ ��������� ���������� ��
//������ ����� CFG_AMPCRIT = sizeof ( ClassAmpDef )
struct ClassAmpDef
{
	BOOL	show_amp_crit;		// 1 - ������� ���� ���. ����.
	int		is_auto;			// 0 - �����. ����� ���-���, 1 - ������
	int		is_EN;				// 0 - �������� � �������� ��, 1 - �������� � �������� EN
	int		material;			// �� ������������
	int		object;				// �� ������������
	UINT	A1;
	UINT	A2;
	UINT	N1;
	UINT	N2;
	UINT	reserved[10];

	std::vector<UINT> get_reserved() {
		return get_vector_from_arr<UINT>(this->reserved, 10);
	};
};


//////////////////////////////////////////////////
//���������������� ���� CFG_FONT �������� ���������� � ���������� ��������� �������
//������ ����� CFG_FONT = 5 * sizeof ( LOGFONT )
//LOGFONT	mFont[5];
//OSCTextFont.GetLogFont(&mFont[0]);		//����� ����������� ������� � ���� ������������
//MarkerTextFont.GetLogFont(&mFont[1]);	//����� ����������� ������� �� ��������
//LDTextFont.GetLogFont(&mFont[2]);		//����� ����������� ���� � �������� �� ������
//PrinterFont.GetLogFont(&mFont[3]);		//����� ����������� ���� � �������� �� ������
//TabCtrlFont.GetLogFont(&mFont[4]);		//����� ����������� ������� �� ��������� �����������

//////////////////////////////////////////////////
//���������������� ���� CFG_PAGE �������� ���������� � ���������� ������� ���������������� ����������� ����
//������ ����� CFG_PAGE = sizeof ( PageData ) + sizeof ( PageName ) * NumPages;
//NumPages - ����� ������� (�������) �������� ���� ���������
struct PageName
{
	char page_name[MAX_PAGENAMELENGTH];

	std::wstring get_page_name() {
		return get_wstr(this->page_name);
	}
};

struct PageData
{
	UINT mem_size;		// size of page configuration
	UINT num_pages;		// number of pages in file
	UINT cur_page;		// currently selectde page
	PageName* name;		// unsized array of page names;

	std::vector<PageName> get_name() {
		return get_vector_from_arr<PageName>(this->name, 0);
	}
};


//////////////////////////////////////////////////
//���������������� ���� CFG_PROFILE �������� ���������� � ���������� ������� ���������������� �������� ������� (��. CFG_PAGE)
//������ ����� CFG_PROFILE = sizeof ( PageData ) + sizeof ( PageName ) * NumProfiles;
//NumProfiles - ����� �������� (��������) ����������

//////////////////////////////////////////////////
//���������������� ���� CFG_IMPLINES �������� ���������� � ���������� ������� ����� ���������� ��� ������������ �� ��������� ���� �����������
//������ ����� CFG_IMPLINES = sizeof ( ILData );
struct ILData	//27.12.99S //03.05.06S
{
	BOOL		enable_import;		//���� ��������� ��������� �����
	int			ParInput;			//���� ��������� ����� �� ������ ���������������� ������
	int			ParChanNum;			//����� ���������������� �����
	int			ParModNum;			//����� ��������� ������
	char		ParName[MAX_PATH];	//��� ���������
	char		FileName[MAX_PATH];	//��� ����� ��� ��������� ��������� �� ���������� �����
	int			thickness;			//������� ����� �����������
	COLORREF	color;				//���� �����������
	double		NumFraction;		//�������� (����� ���������� ������)

	std::wstring get_ParName() { return get_wstr(this->ParName); };
	std::wstring get_FileName() { return get_wstr(this->FileName); };
};



//////////////////////////////////////////////////
struct ServDefMod
{
	BOOL		show;				//���� �����������
	COLORREF	color;				//���� �����������
	int			K1;					//����������������� �����������
	int			K2;					//���������� �����������
	int			show_text;			//���� ����������� ���������� ��������
	UINT		reserved[5];

	std::vector<UINT> get_reserved() {
		return get_vector_from_arr<UINT>(this->reserved, 5);
	};
};



//enum	DataTypes { ev_data_old = 0, sys_data = 1, noise_data_old = 2, param_data = 3, noise_data = 4, ev_data = 5, marker = 6, text = 7, reserv = 8, service_data = 9, u_param_data = 34, radio_data = 11, u_avg_data = 33 };
enum	DataTypes {
	ev_data_old = 0,
	sys_data = 1,
	noise_data_old = 2,
	param_data = 3,
	noise_data = 4,
	ev_data = 5,
	marker = 6,
	text = 7,
	reserv = 8,
	service_data = 9,	//07.12.00S //24.04.01S //22.04.04S //30.01.11S
	unis_param_data = 10,
	radio_data = 11,
	unis_avg_data = 12
};	//27.05.15S //25.05.16S

//WORD	type; //��� ������ �������� DataTypes
//WORD	size; //������ ��������� ��������������� ��������� ���� type
//				sizeof(EV_DATA_OUT_OLD) - ev_data_old, ���������� ��� ������� � ����������� ��-���������, �� ������������
//				sizeof(CH_INFO_OUT)		- sys_data, ������� ��������-���������� ����
//				sizeof(EV_DATA_OUT)		- noise_data_old, ���������� ��� �������������� �������, �� ������������
//				sizeof(EV_PARAM_OUT)	- param_data, ������� � ������� ��������������� ���������
//				sizeof(EV_AVG_OUT)		- noise_data, ������� � ������������ ������� ��-���������
//				sizeof(EV_DATA_OUT)		- ev_data, �������� ��� ������� � ����������� ��-���������
//				sizeof(MARKER_OUT)		- marker, ������� � �������� ���������� �������
//				sizeof(TEXT_OUT)		- text, ������� � ������� ���������� �������
//				1,						- reserved, ������
//				sizeof(SERVICE_OUT)		- service_data, ������� �� ���������� ���������������� �������
//				sizeof(UNI_PARAM_OUT)	- u_param_data,	������� � ������� ��������������� ��������� �� ������� �������
//				sizeof(RADIO_OUT)		- radio_data, ������� �� ���������� ���������������� ������� ��������� � ������ �����
//				sizeof(UNI_AVG_OUT)		- u_avg_data, ������� � ������������ ������� ��-��������� �� ������� �������

struct EV_DATA_OUT_OLD	//ev_data_old - �� ������������
{
	AE_TIME		ev_time;	 	// start time of the event
	WORD		chan_num;		// channel number
	WORD		max_ampl;		// maximum amplitude
	UINT		duration;		// event duration in microseconds since start time
	UINT		rise_time;	    // event rise time in microseconds since start time	
	UINT        tig;			// tig or count
	WORD		energy_low;		// energy low word
	WORD		energy_medium;	// energy medium word
	WORD		energy_high;	// energy high word
	WORD		flags;			// validity flags
	UINT		loc_event;		// flag of localized event: 0-not, 1-sucsess, 5-must be deleted	//04.09.03S
	LONG        locX;			// location X-coordinate [mm] (NO_LOC) - no location by this event, (SEVERAL_LOC) - used for several locations,
	LONG        locY;			// location Y-coordinate [mm]
};


struct CH_INFO_OUT	// sys_data
{
	AE_TIME		chan_info_time;	// ����� ������� �������
	WORD		chan_num;		// ����� ������
	WORD		type;			// ��� �������
	WORD		data[8];		// ������

	std::vector<WORD> get_data() {
		return get_vector_from_arr<WORD>(this->data, 8);
	};
};

// �� ������� ������� ���� ����� ��������� �������� �������������� ������ � ��������
// ����� ���� �������� ���� type:
// ��� type = 18 � type = 3 � ���� data[0] � ����� �������� ������ �������������� ������ � ��. ���
// ��� type = 14 � ���� data[0] � ����� �������� ������ �������������� ��� � ��


struct EV_DATA_OUT	// ev_data
{
	AE_TIME		ev_time;	 	// ����� ������� ��������
	WORD		chan_num;		// ����� ������
	WORD		max_ampl;		// ������� 16-������ ����� ��������� ��������
	WORD		duration;		// ������� 16-������ ����� ������������ ��������
	WORD		expnd;			// ����� �������������� �������� � ������� ��������� ��� 18-������ ������ (DDM-2)
	WORD		rise_time;	    // ������� 16-������ ����� ����� ���������� ��������� ������
	WORD		flags_add;		// ���������� ���� ������ ��� 18-������ ������ (DDM-2)
	WORD        tig;			// ������� 16-������ ����� ����� �������� (����������)
	WORD		attenuation;    // �����. ��������� ��������� (������� �� 10) ��� ���������� �������� ����������� ���������
	WORD		energy_low;		// ������� �������� (������� �����)
	WORD		energy_medium;	// ������� �������� (������� �����)
	WORD		energy_high;	// ������� �������� (������� �����)
	WORD		flags;			// �����
	WORD		tad;			// ���, ���
	BYTE		loc_event;		// ���� �������������� ��������������� �������: 0-���, 1-��, 5-�������
	BYTE		nsec;			// ������� ���������� ����� ��������� ������ ������������ (DDM-2), ���������� �� ������� ������� ev_time
	LONG        locX;			// ����������� ���������� X [��], (NO_LOC) - ��� �������, (SEVERAL_LOC) - �������� ��������� �������
	LONG        locY;			// ����������� ���������� Y [��]
	LONG        locZ;			// ����������� ���������� Z [��]
	LONG		gain;			// ������� �������� �������� [dB]
};

double ConvertEnergy(EV_DATA_OUT* data);



struct EV_PARAM_OUT	// param_data (��� DDM-2 �� ������������)
{
	AE_TIME		param_time;						// ����� ������� ��������������� �������
	WORD		chan_num;						// ����� ������
	WORD		num_summ;						// ����� ������������ �������� ����������
	WORD		par_summ[NUM_PAR_PER_MOD];		// ��������� �������� ���������� �� �������
	
	std::vector<WORD> get_par_summ() {
		return get_vector_from_arr<WORD>(this->par_summ, NUM_PAR_PER_MOD);
	};
};


struct EV_AVG_OUT	// noise_data - ������� ��������������� ����, �������� ���������� ���������� ������� �������� �� ������ ���������� (1 ���)
{
	AE_TIME		avg_time;				// ����� �������
	WORD		chan_num;				// ����� ������
	WORD		num_summ;				// ����� ������������ ��� ���������� �������� ������ ���� (=1)
	DWORD		noise_summ_ampl;		// ��������� ������������ ��� ���������� �������� ������ ����
	WORD		intensity;				// ���������� ��
	WORD		lost;					// ����� ��������� �� ��������� (��-�� ����������� ������������ ��������)
	double		summ_energy;			// ������� �� ���������
	DWORD		summ_dur;				// ������������ �� ���������
	DWORD		summ_ampl;				// ��������� �� ���������
	DWORD		summ_rise;				// ����� ���������� ��������� ������
	DWORD		summ_tig;				// ����� �������� (����������)
	WORD		max_ampl;				// ������������ ��������� �� �������� �� ������ ����������
	WORD		expndw;					// ����� �������������� �������� � ������� ��������� ��� 18-������ ������ (DDM-2)
	_int8		cur_gain_dB;			// ������� �������� �������� [dB]
	BYTE		expndb;					// ���� �������������� �������� � ������� ��������� ��� 18-������ ������ (DDM-2)
	WORD		reserved[7];

	std::vector<WORD> get_reserved() {
		return get_vector_from_arr<WORD>(this->reserved, 7);
	};
};

struct MARKER_OUT		// marker
{
	AE_TIME		marker_time;		// ����� ������� �������
	DWORD		markerID;			// ������������� ������� ��� �������� � ������ �������
};


struct TEXT_OUT			// text
{
	DWORD		markerID;			// ������������� ������� ��� �������� �� ������� �������
	char		marker_text[68];	// ����� �������

	std::wstring get_p_name() {
		return get_wstr(marker_text);
	};
};


// ��� ��������� 

struct UNI_AVG_OUT
{
	AE_TIME		time;
	WORD		chan_num;
	DWORD		sec;
	DWORD		impulse_total;
	DWORD		impulse_sceto;
	DWORD		impulse_drop;
	DWORD		impulse_replace;
	DWORD		ampl_h;
	DWORD		ampl_l;
	DWORD		dur_h;
	DWORD		dur_l;
	DWORD		rise_h;
	DWORD		rise_l;
	DWORD		counts_h;
	DWORD		counts_l;
	DWORD		energy_h;
	DWORD		energy_l;
	DWORD		thresh;
	DWORD		sceto;
	DWORD		maxdur;
	DWORD		noise_energy_h;
	DWORD		noise_energy_l;
	DWORD		noise_ampl;
	DWORD		noise_amplsum_h;
	DWORD		noise_amplsum_l;
	DWORD		noise_num;
	DWORD		flags;
};

struct UNI_PARAM_OUT	
{
	AE_TIME			time;
	WORD			chan_num;
	float			value;
	unsigned char	prm_chan;
	unsigned char	prm_type;
	unsigned char	flags;
	unsigned char	res;
};

struct SERVICE_OUT	
{
	AE_TIME		s_time;		// service info absolute time
	WORD		board;		// number of device to inform
	BYTE		type;		// type of info					
	BYTE		line;		// name of line (in IP setup)	
	WORD		data[12];	// info data set
};

#define NUM_RPARAM	5			
// radioplan status struct	
struct RADIO_OUT
{
	AE_TIME		r_time;
	WORD		chan_num;
	WORD		data[NUM_RPARAM];
};

#define NUM_FILE_STRUCT	13		//24.04.01S //22.04.04S //30.01.11S //25.05.16S
const   UINT	DataSizes[NUM_FILE_STRUCT] = {
				sizeof(EV_DATA_OUT_OLD),
				sizeof(CH_INFO_OUT),
				sizeof(EV_DATA_OUT),
				sizeof(EV_PARAM_OUT),
				sizeof(EV_AVG_OUT),
				sizeof(EV_DATA_OUT),
				sizeof(MARKER_OUT),
				sizeof(TEXT_OUT),
				1,						//15.12.11Kir
				sizeof(SERVICE_OUT), 	//24.04.01S //22.04.04S //30.01.11S
				sizeof(UNI_PARAM_OUT),	//12.05.17S
				sizeof(RADIO_OUT),
				sizeof(UNI_AVG_OUT) };

////////////////////////////////////////////////////////////////////////
//Struct member alignment ����������� �� ��������� � MSVisualC++, �.�. ������ 8 ����

///////////////THE END//////////THE END////////THE END/////////////////
