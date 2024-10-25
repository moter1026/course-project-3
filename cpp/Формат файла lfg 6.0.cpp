// Формат файла локации для АЭ систем A-Line DDM, DDM-2, PCI-N и PCI8 для всех версий //created 08.05.2020

// сначала пишется заголовок файла локации TAGFileHdr
////////////////////////////////////////////////////////////////
typedef struct TAGFileHdr
{
    UINT signature;         // signature = 0xAA551313
    UINT ver_maj;           // version major = 33 для текущей версии файлов
    UINT ver_min;           // version minor = 8 для текущей версии файлов
    UINT ftype;             // type of file	= 0xAA03 для файла локации lfg-типа
	UINT hard_version;		// version of hardware controller //текущая версия DDM-2 контроллера = 0x102071f (считывается из аппаратной части)
	UINT soft_version;		// version of sort options //текущая версия ПО A-Line = 0x20600
    UINT reserved[18];      // reserved bytes for future use
}FileHdr;

////////////////////////////////////////////////////////////////
// далее записан блок общих параметров локации
// блок 1 общее число каналов, доступных для выбора в локации m_SensorTotal и флаг расширения версии ExFlag
// общая длина блока new_sens blk_size = sizeof ( UINT )
UNIT new_sens = m_SensorTotal + UINT ( ExFlag << 16 );

// блок 2 параметры кластеризации clusterization info settings (общие для всех групп локации)
// общая длина блока clusterization info settings = sizeof ( COLORREF ) * 5 + sizeof ( COLORREF ) * 5 + sizeof ( UINT ) * 4 + sizeof ( UINT ) * 4 + sizeof ( UINT ) * 3 + sizeof ( UINT )
// поле 2.1 - массив цветовой палитры кластеризации по количеству событий в кластере - всего 5 значений типа COLORREF
// поле 2.2 - массив цветовой палитры кластеризации по локационной амплитуде в кластере - всего 5 значений типа COLORREF
// поле 2.3 - массив значений границ кластеризации по количеству событий в кластере - всего 4 значения типа UINT
// поле 2.4 - массив значений границ кластеризации по локационной амплитуде в кластере - всего 4 значения типа UINT
// поле 2.5 - длина, ширина и высота кластера в мм - всего 3 значения типа UINT
// поле 2.6 - тип кластеризации: по количеству событий в кластере, по проценту количества событий в кластере от общего их числа, по проценту количества событий в кластере от максимума - всего 1 значение типа UINT

// блок 3 локационные группы
// поле 3.1 - число сохраненных локационных групп m_GroupsNum - всего 1 значение типа UINT
// далее по числу сохраненных групп m_GroupsNum читаются параметры, i - номер текущей группы
// Groups[i].Enable, sizeof(BOOL) - парметр активной группы, всегда TRUE
// Groups[i].Name, sizeof ( char ) * 16 - текстовое название группы, используется после конвертации в UNICODE с помощью CONVERT_2T (см. ниже)
// Groups[i].Velocity, sizeof(UINT) - параметр скорости волны в м/с для расчета координат
// Groups[i].Bound, sizeof(DWORD) - параметр региона отсечки, в мм
// Groups[i].ImageDiameter, sizeof(int) - диаметр изображения датчика на карте локации, в отн. ед
// Groups[i].Length, sizeof(DWORD) - длина области локации, в мм
// Groups[i].Width, sizeof(DWORD) - ширина области локации, в мм
// Groups[i].Height, sizeof(DWORD) - высота области локации, в мм
// Groups[i].Type, sizeof(int) - тип локации линейная - 0, планарная - 1, цилиндр - 2, сфера - 3, днище - 4, трехмерная - 5, зонная - 6, рисунок - 7, конус - 8, стенка РВС - 9
// Groups[i].DisplayAmpLocWindow, sizeof(BOOL) - флаг активации окна локационной амплитуды
// Groups[i].Average, sizeof(BOOL) - опции окна локационной амплитуды: 0 - показать среднюю по кластеру, 1 - максимальную по кластеру амплитуду
// Groups[i].Dbonm, sizeof(double) - величина параметра затухания, дБ/м
// Groups[i].Display3DLocWindow, sizeof(BOOL) - флаг активации окна 3D-модели объекта
// Groups[i].DisplayLocEvWindow, sizeof(BOOL) - флаг активации окна накопления слоцированных событий во времени
// Groups[i].fpackdef, sizeof ( FPackDef ) - структура параметров настройки селекции пачек (событий)
typedef struct TAGFPackDef		
{
    UINT amp_min;				// минимальная ампл. 1-го события в пачке, дБ
	int  all_chan;				// 0 - любой канал, 1 - из списка.
	char chan_list[200];		// список каналов
	int  all_AES;				// 0-все события, 1-первое по кажд.каналу, 2-макс. по кажд. каналу, 3-только стартовый импульс АЭ
	UINT pack_time;				// длит. пачки, мкс
	UINT pack_dead_time;		// мертвое время после пачки.
}FPackDef;

// Groups[i].Shift, sizeof(POINT) - сдвиг начала координат
// Далее критериальные параметры для локации и локационного фильтра
// Groups[i].AmpCrit, sizeof(BOOL) - флаг активации амплитудного критерия
// Groups[i].LocAmpCrit, sizeof(BOOL) - флаг активации критерия по локационной амплитуде
// Groups[i].LocAmpDiff, sizeof(UINT) - параметр максимальной разницы в локационных амплитудах
// Groups[i].EvTimeCrit, sizeof(BOOL) - флаг активации отсева симфазной помехи (разница времен прихода событий)
// Groups[i].EvTimeDiff, sizeof(UINT) - минимально допустимая разность времен прихода событий для работы фильтра симфазной помехи
// Groups[i].DeadTimeCrit, sizeof(BOOL) - флаг установки мертвого времени в локации
// Groups[i].DeadTime, sizeof(UINT) - величина мертвого времени для локации
// Groups[i].bPeakTime, sizeof(BOOL) - флаг подсчета локации по времени максимума амплитуды	(по умолчанию - FALSE)
// Groups[i].AdoptedAtt, sizeof(int) - флаг подбора затухания по результатам локации
// Groups[i].ParGateCrit1, sizeof(WORD) - флаг активации параметрического строба в локации
// Groups[i].ParGateMode1, sizeof(WORD) - переключатель режима работы строба в локации: работает всегда = 0, только на подъеме = 1, только на спаде = 2
// Groups[i].ParLow1, sizeof(long) - нижняя граница параметрического строба
// Groups[i].ParHigh1, sizeof(long) - верхняя граница параметрического строба
// Groups[i].NumChan1, sizeof(WORD) - номер используемого параметрического входа
// Groups[i].NumMod1, sizeof(WORD) - номер используемого основного канала (модуля)
// Groups[i].AmpDiff, sizeof (int) - величина невязки амплитуды для амплитудного критерия
// Groups[i].ReservedForCrit, 2 * sizeof( UINT ) - reserved for future criterium

// Далее настройки локационной антенны, зон и др. параметров локационной схемы 
// Groups[i].bTrian, sizeof(int) - тип антенны, 1 - треугольная, 0 - четырехугольная, 2 - произвольная
// Groups[i].bCheckTrue, sizeof(BOOL) - флаг активации режима верификации результата для локации 4-х угольниками
// Groups[i].Delta, sizeof(UINT) - значение допустимой погрешности при верификации результата для локации 4-х угольниками
// Groups[i].bVelocityCheck, sizeof(BOOL) - флаг активации режима подбора оптимальной скорости для локации 4-х угольниками
// Groups[i].MaxVelocity, sizeof(UINT) - верхняя граница интервала подбора скорости
// Groups[i].MinVelocity, sizeof(UINT) - нижняя граница интервала подбора скорости
// Groups[i].DSize, sizeof(UINT) - параметр ячейки решётки для произвольной локации по умолчанию, в мм
// Groups[i].ZoneSize, sizeof(UINT) - максимальный размер пятна в произвольной локации по умолчанию
// Groups[i].MinTotalNum, sizeof(WORD) - минимальный размер пачки, подходящей для произвольной локации
// Groups[i].MaxUsedNum, sizeof(WORD) - максимальное число импульсов используемых в вычислениях произвольной локации
// Groups[i].bSilentCheck, sizeof(WORD) - флаг использования факта промолчавших датчиков в вычислениях произвольной локации
// Groups[i].MaxSilentNum, sizeof(WORD) - максимальное допустимое число промолчавших датчиков
// Groups[i].Save2DForm, sizeof(BOOL) - флаг сохранения форм 2D-объектов при отображении результатов
// Groups[i].LocInside, sizeof(BOOL) - флаг сохранения результатов локации вне локационной зоны
// Groups[i].ReservedCheck,  2 * sizeof( UINT ) - reserved for future use

// Далее настройки координат датчиков и параметров конкретных локационных схем
// Groups[i].Sensor, sizeof(UINT) - число датчиков, использованных в локации
// UINT num_channels - есть расчетный параметр числа датчиков, используемых в локации с учетом зеркальных, вычисляется как показано ниже;

	switch (Groups[i].Type)
{
//	схема локации конус
case cone:	
// Groups[i].NumBelts, sizeof(UINT) - число поясов датчиков
// Groups[i].LeftWidth, sizeof(UINT) - высота левой (верхней) крышки
// Groups[i].IncludeLeft, sizeof(BOOL) - флаг включения локации левой (верхней) крышки
// Groups[i].RightWidth, sizeof(UINT) - высота правой (нижней) крышки
// Groups[i].IncludeRight, sizeof(BOOL) - флаг включения локации правой (нижней) крышки
// Groups[i].PlaceBoard, sizeof(BOOL) - флаг автоматического размещения датчиков по границе обечайки
// Groups[i].bVertical, sizeof(BOOL) - флаг ориаентации объекта: 0 - горизонтальный, 1 - вертикальный
// num_channels, sizeof(UINT) - читается число каналов в группе
break;

//	схема локации цилиндр
case volume:	
// Groups[i].NumBelts, sizeof(UINT) - число поясов датчиков
// Groups[i].LeftWidth, sizeof(UINT) - высота левой (верхней) крышки
// Groups[i].IncludeLeft, sizeof(BOOL) - флаг включения локации левой (верхней) крышки
// Groups[i].RightWidth, sizeof(UINT) - высота правой (нижней) крышки
// Groups[i].IncludeRight, sizeof(BOOL) - флаг включения локации правой (нижней) крышки
// Groups[i].PlaceBoard, sizeof(BOOL) - флаг автоматического размещения датчиков по границе обечайки
// Groups[i].bVertical, sizeof(BOOL) - флаг ориентации объекта: 0 - горизонтальный, 1 - вертикальный
// num_channels = Groups[i].Sensor + Groups[i].NumBelts - рассчитывается число каналов в группе
break;

//	схема локации сфера
case sphere: 
// Groups[i].bVertical, sizeof(BOOL) - флаг типа локации сферы: 0 - секторальный, 1 - кубический
if ( Groups[i].bVertical )
{
//	Groups[i].NumAdd, sizeof(UINT) - число дополнительных каналов в локации;
//	num_channels = Groups[i].Sensor + Groups[i].NumAdd - рассчитывается число каналов в группе
//	Groups[i].NumBelts = 3;
}
else
{
//	Groups[i].NumBelts, sizeof(UINT) - число поясов датчиков
//	j = (Groups[i].Sensor-2)/Groups[i].NumBelts;
//	num_channels = Groups[i].Sensor + 2*(j-1) + j*Groups[i].NumBelts - рассчитывается число каналов в группе
}
break;

//	другие локационные схемы
 default:
//	num_channels = Groups[i].Sensor - рассчитывается число каналов в группе
//	Groups[i].LeftWidth = Groups[i].RightWidth = 0;
//	Groups[i].NumBelts = 2;
break;
}

// чтение координат датчиков и списка локационных зон для тех групп, где эти параметры актуальны
// структура координат датчика 2D
class CLocalPoint
{
    UINT		num_chan;   // номер канала для связи с аппаратной частью
    int			num_repeat; // тип отображения датчика на сложных схемах: 0 - обечайка, 1 - левая крышка, 2 - правая крышка, 3 - цент левой крышки, 4 - центр правой крышки, 5 - совместный, 6 - верхний, 7 - нижний
    POINT		coor;       // 2D- координаты датчика
    COLORREF	color;		// цвет отображения датчика
}
// структура координат датчика 3D
typedef struct TAGPoint3DL
{
	long	x;
	long	y;
	long	z;
}Point3DL;
class CLocalPoint3D
{
    UINT		num_chan;   // номер канала для связи с аппаратной частью
    int			num_repeat; // тип отображения датчика на сложных схемах: 0 - обечайка, 1 - левая крышка, 2 - правая крышка, 3 - цент левой крышки, 4 - центр правой крышки, 5 - совместный, 6 - верхний, 7 - нижний
    Point3DL	coor;       // 3D- координаты датчика
    COLORREF	color;		// цвет отображения датчика
};

// структура треугольной локационной зоны
class CTriad
{
    UINT n[3];				// номер каналов зоны в общем массиве каналов DataArray
    UINT num_chan[3];		// номера каналов зоны сквозные из аппаратной части 
}
// структура четырехугольной локационной зоны
class CQuad
{
    UINT n[4];				// номер каналов зоны в общем массиве каналов DataArray
    UINT num_chan[4];		// номера каналов зоны сквозные из аппаратной части 
	int NumChIn;			// номер канала из DataArray, попавшего вовнутрь триады для центрированной схемы
}

if (( Groups[i].Type != threeD ) && ( Groups[i].Type != zonary ) && ( Groups[i].bTrian != option ) && (( Groups[i].Type != picture ) || ( Groups[i].bTrian != quad )))
// для плоскостных локаций, осуществляющих локацию по зонам
// по числу num_channels записано соответствующее число структур CLocalPoint
// Groups[i] - > Add (CLocalPoint);

// чтение локационных зон
// Groups[i].NumZone, sizeof(int) - число локационных зон
// по числу Groups[i].NumZone записано соответствующее число структур CTriad или CQuad
if ( Groups[i].bTrian )
{
// Groups[i] - > Add (CTriad)
}
else
{
// Groups[i] - > Add (CQuad)
}

// для пространственных локаций
// по числу num_channels записано соответствующее число структур CLocalPoint3D
// Groups[i] - > Add (CLocalPoint3D);
else
{
// Groups[i] -> Add ( CLocalPoint3D )
}

// чтение настроек раздела фильтра
// Groups[i].DelFlag, sizeof(int) - флаг удаления/сохранения выделенных событий
// Groups[i].gFilter.ForX, sizeof(BOOL) - флаг фильтрации по параметру координата X
// Groups[i].gFilter.ForY, sizeof(BOOL) - флаг фильтрации по параметру координата Y
// Groups[i].gFilter.ForZ, sizeof(BOOL) - флаг фильтрации по параметру координата Z

// чтение границ фильтрации по координатам
// Groups[i].gFilter.XMore, sizeof(LONG) - левая граница условий по координате X;
// Groups[i].gFilter.XLess, sizeof(LONG) - правая граница условий по координате X;
// Groups[i].gFilter.YMore, sizeof(LONG) - левая граница условий по координате Y;
// Groups[i].gFilter.YLess, sizeof(LONG) - правая граница условий по координате Y;
// Groups[i].gFilter.ZMore, sizeof(LONG) - левая граница условий по координате Z;
// Groups[i].gFilter.ZLess, sizeof(LONG) - правая граница условий по координате Z;

// Groups[i].Save, sizeof(BYTE) - флаг сохранения результатов фильтрации в файле
// Groups[i].DontDel, sizeof(BYTE) - флаг не удалять из файла отфильтрованные импульсы
// Groups[i].CurPage, sizeof(WORD) - номер страницы для расположения окон локации
// Groups[i].FileName, sizeof ( char ) * 260 - путь для сохранения результатов фильтрации в виде файла данных отфильтрованного по локации, используется после конвертации в UNICODE с помощью CONVERT_2T (см. ниже)

// параметры окон отображения локации, объёмных моделей и т.д. для сохранения
// Groups[i].cfg_loc_size, sizeof ( UINT ) - размер конфигурационного блока для сохранения параметров основного локационного окна, см. C2DView::QueryCfgSize ( void ) из файла cfg
// Groups[i].cfg_loc_wnd, Groups[i].cfg_loc_size - чтение самого конфигурационного блока параметров основного локационного окна
// Groups[i].imp_loc_size, sizeof ( UINT ) - размер конфигурационного блока для сохранения параметров наложения импортируемой графики основного локационного окна равен sizeof ( IPData )
// Groups[i].imp_loc_wnd, Groups[i].imp_loc_size - чтение самого конфигурационного блока параметров наложения импортируемой графики основного локационного окна, структуры IPData
typedef struct TAGImportPictureData	
{
	BOOL	m_bEnable;					// активация наложения импортируемой графики
    RECTL	rc;							// координаты поля наложения
	char	m_FileName[MAX_PATH];		// файл-источник графики (доступные форматы: *.emf, *.wmf)
	BOOL	m_bXY;						// флаг сохранения исходных пропорций
	BOOL	m_bEnh;						// флаг доступнойти enhanced metafile format	
}IPData;
// Groups[i].cfg_3Dloc_size, sizeof ( UINT ) - размер конфигурационного блока для сохранения параметров окна 3D-модели локации, см. C3DView::QueryCfgSize ( void ) из файла cfg
// Groups[i].cfg_3Dloc_wnd, Groups[i].cfg_3Dloc_size - чтение самого конфигурационного блока параметров основного локационного окна

UINT CTableWnd::QueryCfgSize ( void ) //размер конфигурационного блока для окон зонной локации
{
    UINT cfg_size = 0;
    cfg_size += sizeof ( RECT );			// координаты окна
	cfg_size += sizeof ( byte );			// is window minimized, maximized or hide
    return ( cfg_size );
}
// Groups[i].cfg_tloc_size, sizeof ( UINT ) - размер конфигурационного блока для сохранения параметров окна зонной локации
// Groups[i].cfg_tloc_wnd, Groups[i].cfg_tloc_size - чтение самого конфигурационного блока параметров окна зонной локации
// Groups[i].cfg_locev_size, sizeof ( UINT ) - размер конфигурационного блока для сохранения параметров окна накопления слоцированных импульсов от времени, см. C2DView::QueryCfgSize ( void ) из файла cfg
// Groups[i].cfg_locev_wnd, Groups[i].cfg_locev_size - чтение самого конфигурационного блока параметров окна накопления слоцированных импульсов от времени
// Groups[i].cfg_amp_size, sizeof ( UINT ) - размер конфигурационного блока для сохранения параметров окна локационной амплитуды, см. C2DView::QueryCfgSize ( void ) из файла cfg
// Groups[i].cfg_amp_wnd, Groups[i].cfg_amp_size - чтение самого конфигурационного блока параметров окна локационной амплитуды
// Groups[i].imp_amp_size, sizeof ( UINT ) - размер конфигурационного блока для сохранения параметров наложения импортируемой графики окна локационной амплитуды равен sizeof ( IPData )
// Groups[i].imp_amp_wnd, Groups[i].imp_amp_size - чтение самого конфигурационного блока параметров наложения импортируемой графики окна локационной амплитуды, структуры IPData
// Groups[i].imp_3Dflag, sizeof ( BOOL ) - флаг наложения импортируемой графики на окно объемной модели локации
if ( Groups[i].imp_3Dflag )	
// Groups[i].imp_3Dname, sizeof ( chan ) * 260 - файл-источник 3D-графики (доступные форматы: *.ase)

// параметры индивидуального локационного критерия
#define MAX_ALARM_LEVEL		3
typedef struct TAGLocDistDef
{
	char		Name [ 100 ];	//имя участка
	UINT		Thickness;		//толщина отображения в 3D
	long		Xmax;			//границы участка по Х
	long		Xmin;			//границы участка по Х
	long		Ymax;			//границы участка по Y
	long		Ymin;			//границы участка по Y
	long		Zmax;			//границы участка по Z
	long		Zmin;			//границы участка по Z
	long		X[4];			//средняя плоскость
	long		Y[4];
	long		Z[4];
	long		X1[4];			//левая плоскость
	long		Y1[4];
	long		Z1[4];
	long		X2[4];			//правая плоскость
	long		Y2[4];
	long		Z2[4];
	UINT		num_lgroup;		//номер группы (окна) отображения результатов
	BOOL		IUse[MAX_ALARM_LEVEL];		//флаг использования тревог по интегральному накоплению
	UINT		ILevel[MAX_ALARM_LEVEL];	//границы интервалов тревог по интегральному накоплению
	UINT		IValue;						//текущее значение величины накоплнных событий
	COLORREF	IColor[MAX_ALARM_LEVEL];	//цвет отображения тревог по интегральному накоплению
	BOOL		DUse[MAX_ALARM_LEVEL];		//флаг использования тревог по скорости накопления
	UINT		DLevel[MAX_ALARM_LEVEL];	//границы интервалов тревог по скорости накопления
	UINT		DValue;						//текущее значение величины скорости накопления
	COLORREF	DColor[MAX_ALARM_LEVEL];	//цвет отображения тревог по скорости накопления
	int			Divider;					//делитель скорости накопления (секунда-месяц)
	char		flagsNamePrint;				//флаги отрисовки имени и с ним чисел от накопления
	char		flagsValPrint;				//флаги отрисовки значения и скорости накопления
	char		NameValPos;					//позиция имени и числа, мл.-Name, ст.-Val, по 4 бита
	char		bMustRedraw;				//флаг необходимости отрисовки для данного элемента
	UINT		reserved [49];				//reserved
}LocDistDef;
// Groups[i].bEnableLocCrit, sizeof ( BOOL ) - флаг активации локационного критерия
// Groups[i].num_dist, sizeof ( UINT ) - число участков-разбиений объекта локации для использования локационного критерия
if ( Groups[i].num_dist ) 
// Groups[i].list_dist, sizeof ( LocDistDef ) * Groups[i].num_dist - чтение характеристик локационного критерия

// чтение параметров таблицы затухания
//Structure GraphBuf
typedef struct TAGGraphBuf
{
	float distance;			//расстояние, м
	float amplitude;		//амплитуда, дБ
}GraphBuf;
// Groups[i].attenuation_buf_count, sizeof( int ) - число строк в таблице затухания
// по числу заведенных и сохраненных строк в таблице затухания Groups[i].attenuation_buf_count, размер каждой структуры sizeof (GraphBuf)
// Groups[i].attenuation_buf, sizeof( GraphBuf ) * Groups[i].attenuation_buf_count - чтение содержимого таблицы затухания

void convert_2T( const char * const source, TCHAR * const destination, const int sizes, const int sized )
{
	int	symbol_size;
	CString	str;
	int	i;

	if ( !source ) return;
	if ( !destination ) return;
	if ( sizes < 2 ) return;
	if ( sized < 2 ) return;

	if ( ( source[ sizes - 2 ] == 1 ) && ( source[ sizes - 1 ]  == 1 ) ) {
		i = 0;
		symbol_size = sizeof( TCHAR ) / sizeof( char );

		while (
			!(	( source[ i * symbol_size ] == 0 ) &&
				( source[ i * symbol_size + ( 1 % symbol_size ) ] == 0 ) ) &&	/* if UNICODE - check both char, else check only first */
			( i < ( sized / symbol_size ) - 1 ) &&
			( i * symbol_size < sizes )
			) {
			i++;
		} /* count chars */
		if ( i > 0 ) {
			memcpy( &( destination[ 0 ] ), &( source[ 0 ] ), i * sizeof( TCHAR ) );
		} /* copy chars */
		destination[ i ] = 0;
	} else {
		str = CString( source );
		_tcsncpy( destination, str, sized );
		destination[ sized - 1 ] = 0;
	} /* check marker */
}

////////////////////////////////////////////////////////////////////////

//Struct member alignment установлено по умолчанию в MSVisualC++, т.е. равным 8 байт

///////////////THE END//////////THE END////////THE END/////////////////
