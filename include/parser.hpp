#ifndef PARSER_H
#define PARSER_H

#include <value.hpp>
#include <memory>
#include <string_view>

namespace json
{
    class parser
    {
    private:
        std::string_view m_json_view;

    public:
        parser(const std::string_view &json_view);
        const const value *make_value();
    };
}

#endif