#pragma once
#include <vector>

namespace civ
{

    using ID = uint64_t;

    template <typename T>
    struct Ref;

    struct Slot
    {
        ID m_id;
        ID m_data_id;
    };

    template <typename T>
    class ObjectSlot
    {
    public:
        ObjectSlot(ID id_, T *object_)
            : m_id(id_), m_object(object_)
        {
        }

    private:
        ID m_id;
        T *m_object;
    };

    template <typename T>
    struct ObjectSlotConst
    {
        ObjectSlotConst(ID id_, const T *object_)
            : m_id(id_), m_object(object_)
        {
        }

        ID m_id;
        const T *m_object;
    };

    struct SlotMetadata
    {
        ID m_rid;
        ID m_op_id;
    };

    template <typename T>
    class Vector
    {
    public:
        Vector()
            : m_data_size(0), m_op_count(0)
        {
        }
        // Data ADD / REMOVE
        template <typename... Args>
        ID emplace_back(Args &&...args);
        ID push_back(const T &obj);
        void erase(uint64_t id);
        void remove_if(const std::function<bool(const T &)> &f);
        // Data access by ID
        T &operator[](ID id);
        const T &operator[](ID id) const;
        // Returns a standalone object allowing access to the underlying data
        Ref<T> getRef(ID id);
        // Returns the data at a specific place in the data vector (not an ID)
        T &getDataAt(uint64_t i);
        // Check if the data behind the pointer is the same
        bool isValid(ID id, ID validity) const;
        // Returns the ith object and global_id
        ObjectSlot<T> getSlotAt(uint64_t i);
        ObjectSlotConst<T> getSlotAt(uint64_t i) const;
        // Iterators
        typename std::vector<T>::iterator begin();
        typename std::vector<T>::iterator end();
        typename std::vector<T>::const_iterator begin() const;
        typename std::vector<T>::const_iterator end() const;
        // Number of objects in the array
        uint64_t size() const;

        bool isFull() const;
        // Returns the ID of the ith element of the data array
        ID getID(uint64_t i) const;
        // Returns the data emplacement of an ID
        uint64_t getDataID(ID id) const;
        Slot createNewSlot();
        Slot getFreeSlot();
        Slot getSlot();
        SlotMetadata &getMetadataAt(ID id);
        const T &getAt(uint64_t id) const;

    private:
        std::vector<T> m_data;
        std::vector<uint64_t> m_ids;
        std::vector<SlotMetadata> m_metadata;
        uint64_t m_data_size;
        uint64_t m_op_count;
    };

    template <typename T>
    template <typename... Args>
    inline uint64_t Vector<T>::emplace_back(Args &&...args)
    {
        const Slot slot = getSlot();
        new (&m_data[slot.m_data_id]) T(args...);
        return slot.m_id;
    }

    template <typename T>
    inline uint64_t Vector<T>::push_back(const T &obj)
    {
        const Slot slot = getSlot();
        m_data[slot.m_data_id] = obj;
        return slot.m_id;
    }

    template <typename T>
    inline void Vector<T>::erase(ID id)
    {
        // Retrieve the object position in data
        const uint64_t data_index = m_ids[id];
        // Check if the object has been already erased
        if (data_index >= m_data_size)
        {
            return;
        }
        // Swap the object at the end
        --m_data_size;
        const uint64_t last_id = m_metadata[m_data_size].m_rid;
        std::swap(m_data[m_data_size], m_data[data_index]);
        std::swap(m_metadata[m_data_size], m_metadata[data_index]);
        std::swap(m_ids[last_id], m_ids[id]);
        // Invalidate the operation ID
        m_metadata[m_data_size].m_op_id = ++m_op_count;
    }

    template <typename T>
    inline T &Vector<T>::operator[](ID id)
    {
        return const_cast<T &>(getAt(id));
    }

    template <typename T>
    inline const T &Vector<T>::operator[](ID id) const
    {
        return getAt(id);
    }

    template <typename T>
    inline ObjectSlot<T> Vector<T>::getSlotAt(uint64_t i)
    {
        return ObjectSlot<T>(m_metadata[i].m_rid, &m_data[i]);
    }

    template <typename T>
    inline ObjectSlotConst<T> Vector<T>::getSlotAt(uint64_t i) const
    {
        return ObjectSlotConst<T>(m_metadata[i].m_rid, &m_data[i]);
    }

    template <typename T>
    inline Ref<T> Vector<T>::getRef(ID id)
    {
        return Ref<T>(id, this, m_metadata[m_ids[id]].m_op_id);
    }

    template <typename T>
    inline T &Vector<T>::getDataAt(uint64_t i)
    {
        return m_data[i];
    }

    template <typename T>
    inline uint64_t Vector<T>::getID(uint64_t i) const
    {
        return m_metadata[i].m_rid;
    }

    template <typename T>
    inline uint64_t Vector<T>::size() const
    {
        return m_data_size;
    }

    template <typename T>
    inline typename std::vector<T>::iterator Vector<T>::begin()
    {
        return m_data.begin();
    }

    template <typename T>
    inline typename std::vector<T>::iterator Vector<T>::end()
    {
        return m_data.begin() + m_data_size;
    }

    template <typename T>
    inline typename std::vector<T>::const_iterator Vector<T>::begin() const
    {
        return m_data.begin();
    }

    template <typename T>
    inline typename std::vector<T>::const_iterator Vector<T>::end() const
    {
        return m_data.begin() + m_data_size;
    }

    template <typename T>
    inline bool Vector<T>::isFull() const
    {
        return m_data_size == m_data.size();
    }

    template <typename T>
    inline Slot Vector<T>::createNewSlot()
    {
        m_data.emplace_back();
        m_ids.push_back(m_data_size);
        m_metadata.push_back({m_data_size, m_op_count++});
        return {m_data_size, m_data_size};
    }

    template <typename T>
    inline Slot Vector<T>::getFreeSlot()
    {
        const uint64_t reuse_id = m_metadata[m_data_size].m_rid;
        m_metadata[m_data_size].m_op_id = m_op_count++;
        return {reuse_id, m_data_size};
    }

    template <typename T>
    inline Slot Vector<T>::getSlot()
    {
        const Slot slot = isFull() ? createNewSlot() : getFreeSlot();
        ++m_data_size;
        return slot;
    }

    template <typename T>
    inline SlotMetadata &Vector<T>::getMetadataAt(ID id)
    {
        return m_metadata[getDataID(id)];
    }

    template <typename T>
    inline uint64_t Vector<T>::getDataID(ID id) const
    {
        return m_ids[id];
    }

    template <typename T>
    inline const T &Vector<T>::getAt(ID id) const
    {
        return m_data[getDataID(id)];
    }

    template <typename T>
    inline bool Vector<T>::isValid(ID id, uint64_t validity) const
    {
        return validity == m_metadata[getDataID(id)].m_op_id;
    }

    template <typename T>
    void Vector<T>::remove_if(const std::function<bool(const T &)> &f)
    {
        uint64_t data_index = 0;
        for (auto it = m_data.begin(); it != this->end(); ++it)
        {
            if (f(*it))
            {
                this->erase(m_metadata[data_index].m_rid);
                --it;
            }
            else
            {
                ++data_index;
            }
        }
    }

    template <typename T>
    class Ref
    {
        public:
        Ref()
            : m_id(0), m_array(nullptr), m_validity_id(0)
        {
        }

        Ref(ID id_, Vector<T> *a, ID vid)
            : m_id(id_), m_array(a), m_validity_id(vid)
        {
        }

        T *operator->()
        {
            return &(*m_array)[m_id];
        }

        T &operator*()
        {
            return (*m_array)[m_id];
        }

        const T &operator*() const
        {
            return (*m_array)[m_id];
        }

        civ::ID getID() const
        {
            return m_id;
        }

        explicit
        operator bool() const
        {
            return m_array && m_array->isValid(m_id, m_validity_id);
        }

    private:
        ID m_id;
        Vector<T> *m_array;
        ID m_validity_id;
    };

}
