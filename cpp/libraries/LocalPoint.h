//
// Local Point Class Header file
//
///////////////////////////////////

#ifndef CLOCALPOINT_CLASS_INCLUDED
#define CLOCALPOINT_CLASS_INCLUDED

#define MAX_PARSTR_LENGTH	256		//19.02.08 CH

class CLocalPoint
{

 public: 

     CLocalPoint() {};
     CLocalPoint(int num, int rep, POINT c)
     {
         num_chan = num;
         num_repeat = rep;
         coor.x = c.x;
         coor.y = c.y;
     };

 public:

    UINT    num_chan;   // number of channel
    int     num_repeat; // number of mirror image 0-real for simple schemes, 1-mirror
	//	and sensor position type for complicated: 0-medium cylinder, 1-left side cover
	//	2-right side cover, 3-top of left side cover, 4-top of right side cover, 5-mutual cover;
    POINT   coor;       // coordinates

    COLORREF color;// color of channel
};

typedef CArray<CLocalPoint, CLocalPoint> PointArray;

enum ChannelTypes {medium = 0, lside = 1, rside = 2, ltop = 3, rtop = 4, mutual = 5, top = 6, bottom = 7};//16.06S
enum SchemeTypes {velocity = -1, linear = 0, plane = 1, volume = 2, sphere = 3, cover =  4, threeD = 5, zonary = 6, picture = 7, cone = 8, rvs_wall = 9, calibr = 10 };//17.07S //03.04.01S //25.09.01S //29.11.06S //10.10.07S //10.11.08 CH
enum ZoneTypes {triad = 1, quad = 0, option = 2};		//29.03.06S
enum ParGateTypes {always = 0, growup = 1, godown = 2};	//27.12.06S

BOOL Divide	(const __int64& Numer, const __int64& Denom, __int64& Trunc, __int64& Rem);	//29.11.01 CH
int Sign2Line(const __int64& X, const __int64& Y, const POINT& PLi1, const POINT& PLi2, const __int64 div=1);	 //18.09.01 CH	//29.11.01 CH
int Sign2Line(const POINT& P, const POINT& PLi1, const POINT& PLi2); //18.09.01 CH //29.11.01 CH
BOOL PtInInterval(const __int64& X, const __int64& Y, POINT* PLi, int N=2, const __int64 div=1); //29.11.01 CH
BOOL PtInInterval(const __int64& X, const __int64& Y, const POINT& PLi1, const POINT& PLi2, const __int64 div=1); //18.09.01 CH  //29.11.01 CH
BOOL PtInInterval(const POINT& P, POINT* PLi, int N=2); //29.11.01 CH
BOOL PtInInterval(const POINT& P, const POINT& PLi1, const POINT& PLi2); //18.09.01 CH //29.11.01 CH
BOOL PtInRect(const __int64& X, const __int64& Y, POINT* PLi, int N=3, const __int64 div=1); //29.11.01 CH
BOOL PtInRect(const POINT& P, POINT* PLi, int N=3); //29.11.01 CH
int  PtOutReg(const __int64& X, const __int64& Y, POINT* arP, int N=3, const __int64 div=1); //29.11.01 CH
BOOL IntersectLine(const POINT& PLi11, const POINT& PLi12, const POINT& PLi21, const POINT& PLi22); //29.11.01 CH

//Structure Graph Filter		//14.05.01S //29.03.19S
typedef struct TAGGraphFilter
{
	BOOL	ForX;
	BOOL	ForY;
	BOOL	ForZ;
	long	XMore, XLess;
	long	YMore, YLess;
	long	ZMore, ZLess;
}GraphFilter;

#endif