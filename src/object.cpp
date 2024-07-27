#include <object.hpp>

json::object::object() : value(type::object)
{

}

const json::value *json::object::get_field(const std::string &name) const
{
    return m_fields.at(name);
}