#pragma once
#include <vector>
#if 0 // UNUSED
template <typename T>
struct RAccBase
{
    uint32_t m_max_values_count;
    std::vector<T> m_values;
    uint32_t m_current_index;
    T m_pop_value;

    RAccBase(uint32_t max_size = 8)
        : m_max_values_count(max_size), m_values(max_size, 0.0f), m_current_index(0), m_pop_value(0.0f)
    {
    }

    bool addValueBase(T val)
    {
        const bool pop = m_current_index >= m_max_values_count;
        const uint32_t i = getIndex();
        m_pop_value = m_values[i];
        m_values[i] = val;
        ++m_current_index;
        return pop;
    }

    uint32_t getCount() const
    {
        return std::min(m_current_index + 1, m_max_values_count);
    }

    virtual T get() const = 0;

    operator T() const
    {
        return get();
    }

protected:
    uint32_t getIndex(int32_t offset = 0) const
    {
        return (m_current_index + offset) % m_max_values_count;
    }
};

template <typename T>
struct RMean : public RAccBase<T>
{
    T m_sum;

    RMean(uint32_t max_size = 8)
        : RAccBase<T>(max_size), m_sum(0.0f)
    {
    }

    void addValue(T v)
    {
        m_sum += v - float(RAccBase<T>::addValueBase(v)) * RAccBase<T>::m_pop_value;
    }

    T get() const override
    {
        return sum / float(RAccBase<T>::getCount());
    }
};

template <typename T>
struct RDiff : public RAccBase<T>
{
    RDiff(uint32_t max_size = 8)
        : RAccBase<T>(max_size)
    {
    }

    void addValue(T v)
    {
        RAccBase<T>::addValueBase(v);
    }

    T get() const override
    {
        return RAccBase<T>::m_values[RAccBase<T>::getIndex(-1)] - RAccBase<T>::m_values[RAccBase<T>::getIndex()];
    }
};
#endif
