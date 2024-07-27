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
    class array;

    class parser
    {
    private:
        std::string_view m_json_view;

        const value *parse_value();
        const string *parse_string();
        const number *parse_number();
        const object *parse_object();
        const array *parse_array();

    public:
        parser(const std::string_view &_json_view);
        const json::value *make_value();
    };
}

#endif