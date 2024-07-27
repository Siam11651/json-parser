#include <json/number.hpp>
#include <json/utils.hpp>

json::number::number() : value(type::number)
{

}

const double &json::number::get_value() const
{
    return m_value;
}