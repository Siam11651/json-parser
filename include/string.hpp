#ifndef STRING_H
#define STRING_H

#include <parser.hpp>
#include <value.hpp>
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
        string(const string &other);
        const std::string &get_value() const;
    };
}

#endif