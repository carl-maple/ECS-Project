#pragma once
#include <math.h>

struct CVector2i;

struct CVector2f
{
	constexpr CVector2f()
		:x(0.f), y(0.f)
	{
	}

	constexpr CVector2f(const float in_x, const float in_y)
		:x(in_x), y(in_y)
	{
	}

	void operator=(const CVector2f& in)
	{
		x = in.x;
		y = in.y;
	}

	void operator+=(const CVector2f& in)
	{
		x += in.x;
		y += in.y;
	}

	const CVector2f operator+(const CVector2f& in) const
	{
		return CVector2f(x + in.x, y + in.y);
	}

	const CVector2f operator+(const CVector2i& in) const;

	void operator-=(const CVector2f& in)
	{
		x -= in.x;
		y -= in.y;
	}

	const CVector2f operator-(const CVector2f& in) const
	{
		return CVector2f(x - in.x, y - in.y);
	}

	const CVector2f operator*(const float in) const
	{
		return CVector2f(x * in, y * in);
	}

	void operator*=(const float in)
	{
		x *= in;
		y *= in;
	}

	const CVector2f operator-() const
	{
		return CVector2f(-x, -y);
	}

	const CVector2f operator/(const float in) const
	{
		return CVector2f(x / in, y / in);
	}

	float CVector2f::Length() const
	{
		return static_cast<float>(sqrt(LengthSq()));
	}

	float CVector2f::LengthSq() const
	{
		return static_cast<float>((x * x + y * y));
	}

	float CVector2f::Dot(const CVector2f& in) const
	{
		return x * in.x + y * in.y;
	}

	float CVector2f::CrossProduct(const CVector2f& in) const
	{
		return x * in.y - y * in.x;
	}

	void CVector2f::Normalize()
	{
		const float length = Length();

		if (length != 0) 
		{
			x /= length;
			y /= length;
		}
	}

	bool CVector2f::SameDirection(const CVector2f& in)
	{
		return this->Dot(in) > 0;
	}

	CVector2f CVector2f::Left()
	{
		return CVector2f(y, -x);
	}

	CVector2f CVector2f::Right()
	{
		return CVector2f(-y, x);
	}

	static CVector2f TripleProduct(const CVector2f& a, const CVector2f& b, const CVector2f& c)
	{
		CVector2f result;
		const float cross = a.CrossProduct(b);
		result.x = -c.y * cross;
		result.y = c.x * cross;

		return result;
	}



	static CVector2f Zero() { return CVector2f(0, 0); }
	static CVector2f Up() { return CVector2f(1, 0); }

	float x;
	float y;
};

inline const CVector2f operator*(const float scale, const CVector2f& vector)
{
	return vector * scale;
}

struct CVector3f
{

	constexpr CVector3f()
		:x(0), y(0), z(0)
	{
	}

	constexpr CVector3f(const float in_x, const float in_y, const float in_z)
		:x(in_x), y(in_y), z(in_z)
	{
	}

	const CVector3f operator-() const
	{
		return CVector3f(-x, -y, -z);
	}

	const CVector3f operator-(const CVector3f& in) const
	{
		return CVector3f(x - in.x, y - in.y, z - in.z);
	}

	bool CVector3f::SameDirection(const CVector3f& in) const
	{
		return this->Dot(in) > 0;
	}

	float CVector3f::Dot(const CVector3f& in) const
	{
		return x * in.x + y * in.y + z * in.z;
	}



	static CVector3f Cross3D(const CVector3f& lhs, const CVector3f& rhs)
	{
		CVector3f result;

		result.x = lhs.y * rhs.z - lhs.z * rhs.y;
		result.y = lhs.z * rhs.x - lhs.x * rhs.z;
		result.z = lhs.x * rhs.y - lhs.y * rhs.x;

		return result;
	}

	float x;
	float y;
	float z;
};

