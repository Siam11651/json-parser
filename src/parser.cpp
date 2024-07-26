#include <parser.hpp>
#include <string.hpp>

json::parser::parser(const std::string_view &json_view)
{
    m_json_view = json_view;
}

const const json::value *json::parser::make_value()
{
    const constexpr char *ws = " \r\n\t";
    const std::string_view ws_view(ws);

    while(!m_json_view.empty())
    {
        const char &c = m_json_view.front();

        if(ws_view.find(c) != std::string_view::npos)
        {
            m_json_view.remove_prefix(1);
        }
        else if(c == '\"')
        {
            return new string(std::move(string::parse_json_view(m_json_view)));
        }
    }

    return nullptr;
}