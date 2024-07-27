#include <string.hpp>

json::string::string() : value(type::string)
{
    
}

json::string::string(const string &other) : value(value::type::string)
{
    m_valid = other.m_valid;
    m_value = other.m_value;
}

const std::string &json::string::get_value() const
{
    return m_value;
}