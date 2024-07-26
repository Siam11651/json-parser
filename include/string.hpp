#ifndef STRING_H
#define STRING_H

#include <string>

namespace json
{
    class string
    {
    private:
        std::string m_value;
    
    public:
        string(const std::string_view &json);
    };
}

#endif