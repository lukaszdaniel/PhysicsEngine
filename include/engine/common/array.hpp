#pragma once
#if 0 // UNUSED
template <typename T, uint64_t N>
class Array
{
public:
	T &operator[](const uint64_t index)
	{
		return m_data[index];
	}

	T operator[](const uint64_t index) const
	{
		return m_data[index];
	}

private:
	T m_data[N];
};
#endif
