#pragma once
#include <cstdint>
#include <SFML/System.hpp>
#if 0 // UNUSED
class Profiler
{
public:
	class Element
	{
	public:
		Element()
			: m_start(0), m_total(0)
		{
		}

		void reset()
		{
			m_start = 0;
			m_total = 0;
		}

		float asMilliseconds() const
		{
			return m_total * 0.001f;
		}

		auto start() const
		{
			return m_start;
		}

		void setStart(sf::Int64 time)
		{
			m_start = time;
		}

		auto total() const
		{
			return m_total;
		}

		void setStop(sf::Int64 time)
		{
			m_total += time - m_start;
		}

	private:
		uint32_t m_start, m_total;
	};

	Profiler()
	{
		m_clock.restart();
	}

	void start(Element &elem)
	{
		elem.setStart(m_clock.getElapsedTime().asMicroseconds());
	}

	void stop(Element &elem)
	{
		elem.setStop(m_clock.getElapsedTime().asMicroseconds());
	}

private:
	sf::Clock m_clock;
};
#endif