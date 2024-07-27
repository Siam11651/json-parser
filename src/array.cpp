#include <json/array.hpp>

json::array::array() : value(type::array)
{

}

const json::value *json::array::operator[] (const size_t &index) const
{
    if(index < m_value.size())
    {
        return m_value[index];
    }
    else
    {
        return nullptr;
    }
}

size_t json::array::get_size() const
{
    return m_value.size();
}

json::array::~array()
{
    for(const value *const &item : m_value)
    {
        delete item;
    }
}