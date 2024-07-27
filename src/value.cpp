#include <json/value.hpp>

json::value::value(const type &_type)
{
    m_type = _type;
}

const bool &json::value::is_valid() const
{
    return m_valid;
}

const json::value::type &json::value::get_type() const
{
    return m_type;
}

json::value::~value()
{
    
}