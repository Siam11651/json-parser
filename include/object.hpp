#ifndef OBJECT_H
#define OBJECT_H

#include <value.hpp>
#include <map>
#include <string>

namespace json
{
    class object : public value
    {
        friend class parser;

    private:
        std::map<std::string, const value *> m_fields;

    public:
        object();
        const value *operator[](const std::string &name) const;
        ~object();
    };
}

#endif