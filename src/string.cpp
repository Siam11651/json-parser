#include <json/string.hpp>

json::string::string() : value(type::string)
{
    
}

const std::string &json::string::get_value() const
{
    return m_value;
}