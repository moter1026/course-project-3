#include "stdafx.h"

BOOL Divide	(const __int64& Numer, const __int64& Denom, __int64& Trunc, __int64& Rem)
{ // �������� �� ��������� ������ Numer �� Denom, ����� ������� �� ������� - Trunc, ������� - Rem
	return( Trunc = Numer/Denom, (Rem = Numer%Denom) == 0); 
}

int Sign2Line(const __int64& X, const __int64& Y, const POINT& PLi1, const POINT& PLi2, const __int64 div /*=1*/) //18.09.01 CH	//29.11.01 CH
{	//����������� � ��������� ������ { F(x,y) = A*x + B*y + C } ��������� ����� (X/div, Y/div). 
	//������ �������� ����� ����� PLi1 � PLi2. ���� ����� ����� �� ������, �� F(x,y)=0.
	//��������� �������� ��������� (X/div, Y/div) ����� ���� �� �����, ��������� ��������� ���������
	//���� X=xn*div+x0, Y=yn*div+y0,			��	F(X,Y)   = A*xn+B*yn+C+ (A*x0+B*y0   )/div;
	//e��� X=xn*l+  x0, Y=yn*l+  y0, C=cn*l+c0, ��	F(X,Y)/l = A*xn+B*yn+cn+(A*x0+B*y0+c0)/l
	__int64 kn,k0,
		A = ( __int64(PLi1.y) - PLi2.y ), 
		B = ( __int64(PLi2.x) - PLi1.x ),
		C = ( __int64(PLi2.y) * PLi1.x - __int64(PLi1.y) * PLi2.x ),
		xn,x0, yn,y0, cn,c0, 
		Div=div;

	if(div == 1) Div = 1 << 30; //2_147_483_648
		//��� ���������� ����������� �� ��������� ������������
	Divide(X, Div, xn, x0);
	Divide(Y, Div, yn, y0);
	if(div == 1) Divide(C, Div, cn, c0);
	else { cn = C; c0 = 0; }

	Divide(A*x0 + B*y0 + c0, Div, kn, k0);

	double k = double(k0)/Div + A*xn + B*yn + cn + kn;

	return (k>0 ? +1 : (k<0 ? -1 : 0));
}
int Sign2Line(const POINT& P, const POINT& PLi1, const POINT& PLi2) //18.09.01 CH //29.11.01 CH
{	return Sign2Line(__int64(P.x), __int64(P.y), PLi1, PLi2);	}

int PtOutReg(const __int64& X, const __int64& Y, POINT* arP, int N/*=3*/, const __int64 div/*=1*/) //29.11.01 CH
{	//�������� �� ��������� ������� ����� � ������������ (X/div, Y/div)��� 
	//��������������, ��������� ������� arP[0:N-1]. ��� N<2 ���������� FALSE, �.�. ���������� ����-�� 2 �����
	// 3 - �������, 2 - �� �������, 1 - �� �������, 0 - ������ ��������������
	if(N<2) return FALSE;

	int iL=0, iE=0, iG=0;
	int f;

	for(int i=0; i<N; i++)
	{
		if(div*arP[i].x == X && div*arP[i].y == Y) return 2; //������� ��������������
		//
		f = Sign2Line(X, Y, arP[i], arP[i==N-1 ? 0 : i+1], div);

		iL += (f <  0);
		iE += (f == 0);
		iG += (f >  0);
	}

	if(N==2) //�������������� ����� � �������...
	{
		if(iE==2) //�������������� ����� ����� �� �����...
		{
			if( PtInInterval(X, Y, arP, 2, div) ) return 0;  //�������������� ����� ����� ������ �������
			return 3; //�������������� ����� ����� �� �����, �� �� ������ �������
		}
		else return 3; //�������������� ����� �� ����� ���� �� �����
	}
	//�������������� ����� � �������������...
	if( iL == N || iG == N )
		return 0; //������ ��������������
	if( iE != 0 && ( (iL != 0 && iG == 0) || (iL == 0 && iG != 0) ) )
		return 1; //������� ��������������
	return 3; //������� ��������������
}

BOOL PtInInterval(const __int64& X, const __int64& Y, POINT* PLi, int N/*=2*/, const __int64 div/*=1*/) //29.11.01 CH
{	//�������� ��������� ����� � ������������ (X/div, Y/div) ������ ���������������� (����������� �������),
	//������������� N-����� �� PLi. ��� N<2 ���������� FALSE, �.�. ���������� ����-�� 2 �����
	if(N<2) return FALSE;

	LONG	maxx = max(PLi[0].x, PLi[1].x),	maxy = max(PLi[0].y, PLi[1].y),	
			minx = min(PLi[0].x, PLi[1].x),	miny = min(PLi[0].y, PLi[1].y);

	for(int i=2; i<N; i++)
	{
		maxx = max(maxx, PLi[i].x);	maxy = max(maxy, PLi[i].y);
		minx = min(minx, PLi[i].x);	miny = min(miny, PLi[i].y);
	}
	__int64	maxX = div*(div<0 ? minx : maxx),	maxY = div*(div<0 ? miny : maxy),
			minX = div*(div<0 ? maxx : minx),	minY = div*(div<0 ? maxy : miny);
	return(	
			(minX == maxX && Y > minY && Y < maxY) || //����� �����������
			(minY == maxY && X > minX && X < maxX) || //����� �������������
			(X > minX && X < maxX && Y > minY && Y < maxY)
			);
}

BOOL PtInInterval(const POINT& P, POINT* PLi, int N/*=2*/) //29.11.01 CH
{	return PtInInterval(__int64(P.x), __int64(P.y), PLi, N);	}

BOOL PtInInterval(const __int64& X, const __int64& Y, const POINT& PLi1, const POINT& PLi2,	const __int64 div/*=1*/) //18.09.01 CH  //29.11.01 CH
{
	POINT PLi[2]; PLi[0] = PLi1; PLi[1] = PLi2; 
	return(PtInInterval(X, Y, PLi, 2, div));
}
BOOL PtInInterval(const POINT& P, const POINT& PLi1, const POINT& PLi2) //18.09.01 CH //29.11.01 CH
{	
	POINT PLi[2]; PLi[0] = PLi1; PLi[1] = PLi2; 
	return(PtInInterval(__int64(P.x), __int64(P.y), PLi));
}

BOOL PtInRect(const __int64& X, const __int64& Y, POINT* PLi, int N/*=3*/, const __int64 div/*=1*/) //29.11.01 CH
{	//�������� ��������� ����� � ������������ (X/div, Y/div) � ��������������� (����������� �� �������),
	//������������� N-����� �� PLi. ��� N<2 ���������� FALSE, �.�. ���������� ����-�� 2 �����
	if(N<2) return FALSE;

	LONG	maxx = max(PLi[0].x, PLi[1].x),	maxy = max(PLi[0].y, PLi[1].y),	
			minx = min(PLi[0].x, PLi[1].x),	miny = min(PLi[0].y, PLi[1].y);

	for(int i=2; i<N; i++)
	{
		maxx = max(maxx, PLi[i].x);	maxy = max(maxy, PLi[i].y);
		minx = min(minx, PLi[i].x);	miny = min(miny, PLi[i].y);
	}
	__int64	maxX = div*(div<0 ? minx : maxx),	maxY = div*(div<0 ? miny : maxy),
			minX = div*(div<0 ? maxx : minx),	minY = div*(div<0 ? maxy : miny);
	return(	(X >= minX && X <= maxX && Y >= minY && Y <= maxY) );
}
BOOL PtInRect(const POINT& P, POINT* PLi, int N/*=3*/) //29.11.01 CH
{	return (__int64(P.x), __int64(P.y), PLi, N);	}

BOOL IntersectLine(const POINT& PLi11, const POINT& PLi12, const POINT& PLi21, const POINT& PLi22) //29.11.01 CH
{	//������������ �� �����, ���������� ����� ����� {PLi11, PLi12} � {PLi21, PLi22}
	__int64 t1, t4, t6, t9, t12, t14, t17, X, Y;

	t1  = ((__int64)-PLi12.y) + PLi11.y;
	t14 = ((__int64) PLi12.x) - PLi11.x;
	t9  = ((__int64) PLi12.y)*PLi11.x - ((__int64) PLi11.y)*PLi12.x;

	t6  = ((__int64)-PLi22.y) + PLi21.y;
	t12 = ((__int64) PLi22.x) - PLi21.x;
	t4  = ((__int64) PLi22.y)*PLi21.x - ((__int64) PLi21.y)*PLi22.x;

	t17 = (-t1*t12+t6*t14);
	if(t17 == 0) //������ ����������� 
	{
		if(Sign2Line(PLi11, PLi21, PLi22) == 0) //����� ����� ����� ����� �� ������������ �� �����
		{
			POINT ARP[4]; ARP[0]=PLi11; ARP[1]=PLi12; ARP[2]=PLi21; ARP[3]=PLi22;
			UINT Imax, Imin;
			if( Distance(&ARP[0], &ARP[1]) < Distance(&ARP[2], &ARP[3]) ) 
			{ Imax = 2; Imin = 0; }
			else { Imax = 0; Imin = 2; }

			if( PtInInterval(ARP[Imin+0], ARP[Imax+0], ARP[Imax+1]) || 
				PtInInterval(ARP[Imin+1], ARP[Imax+0], ARP[Imax+1]) )
				return TRUE; //������������ ������� ������������� ���� �� �����
		}
		return FALSE;
	}

	Y = t1 *t4 - t6 *t9;
	X = t12*t9 - t14*t4;

	if( PtInInterval(X, Y, PLi11, PLi12, t17) ) return TRUE; // �����������
	return FALSE; // ��� �����������
}
