#pragma once
#include <array>
#include <SFML/System/Vector2.hpp>
#if 0 // UNUSED
template <typename T>
class Grid
{
public:
	class HitPoint
	{
	public:
		HitPoint()
			: m_cell(nullptr), m_dist(0.0f)
		{
		}

	private:
		T *m_cell;
		float m_dist;
	};

	Grid()
		: m_width(0), m_height(0)
	{
	}

	Grid(int32_t width_, int32_t height_)
		: m_width(width_), m_height(height_)
	{
		m_data.resize(m_width * m_height);
	}

	int32_t mod(int32_t dividend, int32_t divisor) const
	{
		return (dividend % divisor + divisor) % divisor;
	}

	template <typename Vec2Type>
	bool checkCoords(const Vec2Type &v) const
	{
		return checkCoords(v.x, v.y);
	}

	bool checkCoords(int32_t x, int32_t y) const
	{
		return x > 0 && x < (m_width - 1) && y > 0 && y < (m_height - 1);
	}

	const T &get(int32_t x, int32_t y) const
	{
		return m_data[y * m_width + x];
	}

	template <typename Vec2Type>
	T &get(const Vec2Type &v)
	{
		return get(v.x, v.y);
	}

	template <typename Vec2Type>
	const T &get(const Vec2Type &v) const
	{
		return get(v.x, v.y);
	}

	template <typename Vec2Type>
	const T &getWrap(Vec2Type v) const
	{
		return getWrap(v.x, v.y);
	}

	const T &getWrap(int32_t x, int32_t y) const
	{
		return get(mod(x, m_width), mod(y, m_height));
	}

	T &get(int32_t x, int32_t y)
	{
		return m_data[y * m_width + x];
	}

	template <typename Vec2Type>
	void set(const Vec2Type &v, const T &obj)
	{
		set(v.x, v.y, obj);
	}

	void set(int32_t x, int32_t y, const T &obj)
	{
		m_data[y * m_width + x] = obj;
	}

	template <typename Vec2Type>
	HitPoint getFirstHit(Vec2Type p, Vec2Type d, float max_dist)
	{
		HitPoint intersection;
		sf::Vector2i cell_p(p.x, p.y);
		const sf::Vector2i step(d.x < 0.0f ? -1 : 1, d.y < 0.0f ? -1 : 1);
		const sf::Vector2f inv_d(1.0f / d.x, 1.0f / d.y);
		const float t_dx = std::abs(inv_d.x);
		const float t_dy = std::abs(inv_d.y);
		float t_max_x = ((cell_p.x + (step.x > 0)) - p.x) * inv_d.x;
		float t_max_y = ((cell_p.y + (step.y > 0)) - p.y) * inv_d.y;
		while (intersection.m_dist < max_dist)
		{
			const uint32_t b = t_max_x < t_max_y;
			// Advance in grid
			intersection.m_dist = b * t_max_x + (!b) * t_max_y;
			t_max_x += t_dx * b;
			t_max_y += t_dy * (!b);
			cell_p.x += step.x * b;
			cell_p.y += step.y * (!b);
			if (!checkCoords(cell_p))
			{
				return intersection;
			}
			else
			{
				T &cell = get(cell_p);
				if (cell.atoms_count)
				{
					intersection.cell = &cell;
					// intersection.normal = sf::Vector2f(to<float>(b), to<float>(!b));
					return intersection;
				}
			}
		}
		return intersection;
	}

private:
	int32_t m_width, m_height;
	std::vector<T> m_data;
};
#endif
