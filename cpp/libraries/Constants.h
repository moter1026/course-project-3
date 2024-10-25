/*
	constants.h
	Some constants that will be used across all the AE software
*/
// initial scale maximum
#define INI_MAX				10L
#define AUTO_SAVE_INT		67L		// интервал автосохранени€, с
#define INI_POINT			1000	// начальный размер массивов точек в коррел€ционных окнах	//17.12.09S
#define MAX_LOC_COLOR		5		// 21.12.98 ("количество интервалов разбиени€ кластеризации")
#define METAFILEHEADERSIZE	22		// metafile header size	//16.12.99S
#define MAX_POLYPOINTSNUM	16384	// максимальный размер буфера дл€ polyline функции	//07.04.09S

// logger related constants

#define LOG_INFO	1
#define LOG_ERR		2
#define LOG_WARNING 3
#define LOG_CRASH	4
#define LOG_EMPTY   0xffff


// message handlers related constants

#define H_EVENT		1
#define H_SDO		2
#define H_ERR		3
#define H_SYSTEM    4
#define H_TIMER     5
#define H_AVERAGE   6
#define H_GDI	    7	//10.09.03S
#define H_THRESH    8	//03.07.08S


// internal messages
#define SYNC_MSG    0x4576	
#define SDO_MSG     0x4577
#define EVENT_MSG   0x4578
#define AVG_MSG     0x4580
#define GDI_MSG     0x4581	//10.09.03S
