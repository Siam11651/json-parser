#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <string_view>

namespace json
{
    class value;
    class string;
    class number;
    class object;

    class parser
    {
    private:
        std::string_view m_json_view;

        const value *parse_value(std::string_view &json);
        const string *parse_string(std::string_view &json);
        const number *parse_number(std::string_view &json);
        const object *parse_object(std::string_view &json);

    public:
        parser(const std::string_view &json_view);
        const json::value *make_value();
    };
}

#endif