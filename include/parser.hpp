#ifndef PARSER_H
#define PARSER_H


#include <string_view>

namespace json
{
    class parser
    {
    private:
        std::string_view m_json_view;

    public:
        parser(const std::string_view &json_view);
        void parse();
    };
}

#endif