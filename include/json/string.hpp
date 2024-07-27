#ifndef STRING_H
#define STRING_H

#include <json/parser.hpp>
#include <json/value.hpp>
#include <string>

namespace json
{
    class string : public value
    {
        friend class parser;

    private:
        std::string m_value;
    
    public:
        string();
        const std::string &get_value() const;
    };
}

#endif