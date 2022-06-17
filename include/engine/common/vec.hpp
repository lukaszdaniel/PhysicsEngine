#pragma once
#include <cmath>
#if 0 // UNUSED
class Vec2
{
public:
	Vec2()
		: m_x(0.0f), m_y(0.0f)
	{
	}

	Vec2(float x_, float y_)
		: m_x(x_), m_y(y_)
	{
	}

	float getLength2() const
	{
		return m_x * m_x + m_y * m_y;
	}

	float getLength() const
	{
		return sqrt(getLength2());
	}

	Vec2 operator/(float f) const
	{
		const float inv = 1.0f / f;
		return Vec2(m_x * inv, m_y * inv);
	}

	Vec2 operator*(float f) const
	{
		return Vec2(m_x * f, m_y * f);
	}

	Vec2 operator-(const Vec2 &other) const
	{
		return Vec2(m_x - other.m_x, m_y - other.m_y);
	}

	Vec2 operator-() const
	{
		return Vec2(-m_x, -m_y);
	}

	void operator+=(const Vec2 &other)
	{
		m_x += other.m_x;
		m_y += other.m_y;
	}

	void operator+=(float f)
	{
		m_x += f;
		m_y += f;
	}

	void operator/=(float f)
	{
		m_x /= f;
		m_y /= f;
	}

	Vec2 plus(const Vec2 &other) const
	{
		return Vec2(m_x + other.m_x, m_y + other.m_y);
	}

	Vec2 minus(const Vec2 &other) const
	{
		return Vec2(m_x - other.m_x, m_y - other.m_y);
	}

	void operator-=(const Vec2 &other)
	{
		m_x -= other.m_x;
		m_y -= other.m_y;
	}

	void rotate(const Vec2 &origin, float angle)
	{
		const Vec2 v = *this - origin;

		// This should be precomputed
		const float ca = cos(angle);
		const float sa = sin(angle);

		const float new_x = v.m_x * ca - v.m_y * sa;
		const float new_y = v.m_x * sa + v.m_y * ca;

		m_x = new_x + origin.m_x;
		m_y = new_y + origin.m_y;
	}

	Vec2 getNormal() const
	{
		return Vec2(-m_y, m_x);
	}

	float dot(const Vec2 &other) const
	{
		return m_x * other.m_x + m_y * other.m_y;
	}

	float cross(const Vec2 &other) const
	{
		return m_x * other.m_y - m_y * other.m_x;
	}

	Vec2 getNormalized() const
	{
		return (*this) / getLength();
	}

	static Vec2 rotate(Vec2 v, const Vec2 &origin, float angle)
	{
		v.rotate(origin, angle);
		return v;
	}

private:
	float m_x, m_y;
};

class IVec2
{
public:
	IVec2()
		: m_x(0), m_y(0)
	{
	}

	IVec2(int32_t x_, int32_t y_)
		: m_x(x_), m_y(y_)
	{
	}

private:
	int32_t m_x, m_y;
};
#endif