#ifndef OBJECT_H
#define OBJECT_H

#include <value.hpp>
#include <map>
#include <string>

namespace json
{
    class object : public value
    {
    private:
        std::map<std::string, const value *> m_fields;

    public:
        object();
        const value *get_field(const std::string &name) const;
        static const object* parse_json_view(std::string_view &json);
    };
}

#endif