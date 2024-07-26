#include <parser.hpp>
#include <utils.hpp>
#include <string.hpp>
#include <number.hpp>

json::parser::parser(const std::string_view &json_view)
{
    m_json_view = json_view;
}

const json::value *json::parser::make_value()
{
    const std::string_view ws_view(WS_CHARS);
    const std::string_view number_first_view(SIGNED_DECIMAL_CHARS);

    for(size_t i = 0; i < m_json_view.size(); ++i)
    {
        const char &c = m_json_view[i];

        if(c == '\"')
        {
            return string::parse_json_view(m_json_view);
        }
        else if(number_first_view.find(c) != std::string_view::npos)
        {
            return number::parse_json_view(m_json_view);
        }
        else if(ws_view.find(c) == std::string_view::npos)
        {
            return nullptr;
        }
    }

    return nullptr;
}