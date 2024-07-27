#include <array.hpp>

json::array::array() : value(type::array)
{

}

const json::value *json::array::operator[] (const size_t &index) const
{
    return m_value[index];
}