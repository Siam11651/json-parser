#include <object.hpp>

json::object::object() : value(type::object)
{

}

const json::value *json::object::operator[](const std::string &name) const
{
    const std::map<std::string, const value *>::const_iterator found = m_fields.find(name);

    if(found == m_fields.cend())
    {
        return nullptr;
    }
    else
    {
        return found->second;
    }
}

json::object::~object()
{
    for(const std::pair<std::string, const value *> &field : m_fields)
    {
        delete field.second;
    }
}