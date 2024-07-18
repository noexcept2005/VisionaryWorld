//VisionaryWorld ר��ͷ�ļ�
//�κ��˲��õ��á�
//Written by Rico
#pragma once

//�����ڶ���������ѧ�ĺ�������

typedef BYTE	DIR, DIR4, DIR8, *PDIR4, *PDIR8;	//����
#define DIR_UP 0
#define DIR_RIGHT 1
#define DIR_DOWN 2
#define DIR_LEFT 3
#define DIR_RIGHTUP 4
#define DIR_RIGHTDOWN 5
#define DIR_LEFTDOWN 6
#define DIR_LEFTUP 7

bool IsVertDir(DIR dir)
{
	if (dir == DIR_UP || dir == DIR_DOWN)	
		return true;
	return false;
}
bool IsHorizDir(DIR dir)
{
	if (dir == DIR_RIGHT || dir == DIR_LEFT)
		return true;
	return false;
}
bool IsBentDir(DIR dir)
{
	if (dir >= DIR_RIGHTUP && dir <= DIR_LEFTUP)
		return true;
	return false;
}

bool InRect(const RECT& rt, int x, int y)
{
	if (x >= rt.left && x <= rt.right
		&& y <= rt.bottom && y >= rt.top)
		return true;
	return false;
}
float Distance(float x1, float y1, float x2, float y2)
{	//���������
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
bool InDistance(float x0, float y0, float x, float y, float dist)
{	//�����ڣ�
	float fdist = the Distance(x0, y0, x, y);
	return (fdist < dist);
}

//Maths
int CJZAPI RandomRange(int Min = 0, int Max = 32767, bool rchMin = true, bool rchMax = true)
{ //�����ֵ���� 
	UINT a;
	a = rand();

	if (rchMin && rchMax)	//[a,b]
		return (a % (Max - Min + 1)) + Min;
	else if (rchMin && !rchMax)		//[a,b)
		return (a % (Max - Min)) + Min;
	else if (!rchMin && rchMax)		//(a,b]
		return (a % (Max - Min)) + Min + 1;
	else							//(a,b)
		return (a % (Max - Min - 1)) + Min + 1;
}
double CJZAPI RandomRangeDouble(double _min, double _max,	//min,max
	bool rchMin = true, bool rchMax = true,	//��/�� 
	UINT uPrec = 2	//���ȣ�λ���� 
	)
{	//���С������ 
	double p_small = pow(10.0f, -int(uPrec));
	int p_big = pow(10, uPrec);
	int l = int(p_big*_min);
	int r = int(p_big*_max);
	int res;
	res = the RandomRange(l, r, rchMin, rchMax);
	if (res != res)
		MessageBoxA(NULL, "������������⣡", "stdcjz.h RandomRangeDouble()", MB_ICONEXCLAMATION);
	return (p_small*(double)res);
}
template <typename _T1, typename _T2>
_T1 CJZAPI Varience(_T1 val, _T2 single_offset)	//���� 
{
	//srand(time(NULL));
	val += RandomRange(0 - single_offset, single_offset, true, true, true);
	return val;
}
template <typename _T>
_T CJZAPI Clamp(_T val, _T min = 0, _T max = 2147483647)	//�޶���Χ 
{
	if (val < min) val = min;
	else if (val > max) val = max;
	return val;
}
//���������ḳֵ���ײ���Ϊ���á� 
template <typename _T1, typename _T2>
_T1 CJZAPI VarienceA(_T1& val, _T2 single_offset)	//���� 
{
	val += RandomRange(0 - single_offset, single_offset, true, true, true);
	return val;
}
template <typename _T>
_T CJZAPI ClampA(_T& val, _T min = 0, _T max = 2147483647)	//�޶���Χ 
{
	if (val < min) val = min;
	else if (val > max) val = max;
	return val;
}

class Vector {	//Vector2D
public:
	float x;
	float y;

	Vector(float _x,float _y) : x{_x} , y{_y} {}
	Vector(const Vector& src)
	{
		x = src.x;
		y = src.y;
	}
	Vector()
	{
		x = y = 0.0f;
	}
	Vector operator+ (Vector& second)
	{	//�����ӷ� 
		return Vector(x + second.x, y + second.y);
	}
	Vector operator- (Vector &second)
	{	//�������� 
		return Vector(x - second.x, y - second.y);
	}
	double operator*(const Vector &second)
	{	//������ 
		return (x*second.x + y * second.y);
	}
	Vector operator*(float a)
	{	//���� 
		return Vector(x*a, y*a);
	}
	Vector operator*(int a)
	{	//���� 
		return Vector(x*a, y*a);
	}
	Vector operator/(float a)
	{
		return Vector(x / a, y / a);
	}
	Vector operator/(int a)
	{
		return Vector(x / a, y / a);
	}
	Vector& operator=(const Vector& src)
	{	//��ֵ 
		x = src.x;
		y = src.y;
		return *this;
	}
	float len()	//ģ 
	{
		return float(sqrt(x*x + y * y));
	}
	float operator()()
	{	//ģ 
		return float(sqrt(x*x + y*y));
	}
	operator float()
	{	//ģ 
		return float(sqrt(x*x + y * y));
	}
	bool operator== (const Vector& sec) const
	{	//��� 
		return (x == sec.x&&y == sec.y);
	}
	bool operator!= (const Vector& sec) const
	{	//���� 
		return (x != sec.x || y != sec.y);
	}
	bool operator > (Vector sec)
	{
		return len() > sec.len();
	}
	bool operator >= (Vector sec)
	{
		return len() >= sec.len();
	}
	bool operator < (Vector sec)
	{
		return len() < sec.len();
	}
	bool operator <= (Vector sec)
	{
		return len() <= sec.len();
	}

	Vector& operator+= (const Vector &sec) {
		x += sec.x;
		y += sec.y;
		return *this;
	}
	Vector& operator-= (const Vector &sec) {
		x -= sec.x;
		y -= sec.y;
		return *this;
	}
	Vector& operator *= (const double t) {
		x *= t;
		y *= t;
		return *this;
	}
	Vector& operator /= (const double t) {
		x /= t;
		y /= t;
		return *this;
	}
	bool IsZero()
	{	//�Ƿ��������� 
		return (x == 0.0&&y == 0.0);
	}
	Vector ReversedVector()	//���� �෴���� 
	{	
		return Vector(-x, -y);
	}
	void Reverse()	//�෴���� ��ֵ
	{	
		x = -x, y = -y;
	}
	Vector Unit()
	{	//�����������ϵĵ�λ����
		Vector ret;
		float len = operator()();
		ret.x = x * (1.0f / len);
		ret.y = y * (1.0f / len);
		return ret;
	}
	Vector ClampMagnitude(float maxLength)
	{	//������������
		Vector ret = *this;
		float len = ret.operator()();
		if (len > maxLength)
		{
			ret.x = ret.x * (maxLength / len);
			ret.y = ret.y * (maxLength / len);
		}
		return ret;
	}
	void ClampMagnitudeA(float maxLength)
	{
		float len = operator()();
		if (len > maxLength)
		{
			x = x * (maxLength / len);
			y = y * (maxLength / len);
		}
	}
	void Clear()
	{
		x = y = 0.0f;
	}
	DIR4 Dir4()	//���ൽ�ĸ������е�һ��
	{
		float dx = 0.0f - x;
		float dy = 0.0f - y;
		DIR4 adir = DIR_RIGHT;
		float k = abs(dy) / (float)abs(dx);
		if (dx > 0)
		{
			if (k <= 1.0)
			{
				adir = DIR_RIGHT;
			}
			else {
				adir = (dy < 0 ? DIR_UP : DIR_DOWN);
			}
		}
		else {
			if (k <= 1.0)
			{
				adir = DIR_LEFT;
			}
			else {
				adir = (dy < 0 ? DIR_UP : DIR_DOWN);
			}
		}
		return adir;
	}
	DIR8 Dir8()	//���ൽ�˸������е�һ��
	{
		float dx = 0.0f - x;
		float dy = 0.0f - y;
		DIR8 adir = DIR_RIGHT;
		float k = abs(dy) / (float)abs(dx);
		if (dx > 0)
		{	//��

			if (k <= tan(PI / 8.0))
			{
				adir = DIR_RIGHT;
			}
			else if (k <= tan(PI * 3.0 / 8.0))
			{
				adir = (dy < 0 ? DIR_RIGHTUP : DIR_RIGHTDOWN);
			}
			else {
				adir = (dy < 0 ? DIR_UP : DIR_DOWN);
			}
		}
		else {	//��
			if (k <= tan(PI / 8.0))
			{
				adir = DIR_LEFT;
			}
			else if (k <= tan(PI * 3.0 / 8.0))
			{
				adir = (dy < 0 ? DIR_LEFTUP : DIR_LEFTDOWN);
			}
			else {
				adir = (dy < 0 ? DIR_UP : DIR_DOWN);
			}
		}
		return adir;
	}
};

class Physics {
public:
	float x;		//m
	float y;		//m
	the Vector v;	//m/s
	float m;		//kg

	Physics()
	{
		x = 1.0f, y = 1.0f;
		v.x = v.y = 0.0f;
	}
	void Move()
	{
		x += v.x * (g_deltaTime / 1000.0);
		y += v.y * (g_deltaTime / 1000.0);
	}
#define GROUND_FRICTION_V 15.0f	//����Ħ��ϵ��
	void GroundFrict()	//����Ħ������
	{
		v.x -= (v.x > 0.0f ? 1 : -1) * (GROUND_FRICTION_V * g_deltaTime / 1000.0);
		v.y -= (v.y > 0.0f ? 1 : -1) * (GROUND_FRICTION_V * g_deltaTime / 1000.0);

		if (abs(v.x) <= 1.0f) v.x = 0.0f;
		if (abs(v.y) <= 1.0f) v.y = 0.0f;
	}
#define WALL_FRICTION_V 16.0f
	void WallStop()	//��ǽ��������ͣס
	{
		v.x -= (v.x > 0.0f ? 1 : -1) * (WALL_FRICTION_V * g_deltaTime / 1000.0);
		v.y -= (v.y > 0.0f ? 1 : -1) * (WALL_FRICTION_V * g_deltaTime / 1000.0);

		if (abs(v.x) <= 0.9f) v.x = 0.0f;
		if (abs(v.y) <= 0.9f) v.y = 0.0f;
	}
	void Stop()
	{
		v.Clear();
	}
	float GetSpeed()
	{
		return float(v);
	}
	bool SideTouch(the Vector side);	//BLOCK�Ƿ�����
};
