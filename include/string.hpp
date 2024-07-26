#ifndef STRING_H
#define STRING_H

#include <value.hpp>
#include <string>

namespace json
{
    class string : public value
    {
    private:
        std::string m_value;
    
    public:
        string();
        string(const string &other);
        string(const string &&other);
        const std::string &get_value() const;
        static const string* parse_json_view(std::string_view &json);
    };
}

#endif