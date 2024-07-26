#include <parser.hpp>
#include <string.hpp>

json::parser::parser(const std::string_view &json_view)
{
    m_json_view = json_view;
}

const json::value *json::parser::make_value()
{
    const constexpr char ws[] = " \r\n\t";
    const constexpr char decimals[] = "0123456789";
    const std::string_view ws_view(ws);
    const std::string_view decimals_view(decimals);

    for(size_t i = 0; i < m_json_view.size(); ++i)
    {
        const char &c = m_json_view[i];

        if(c == '\"')
        {
            return string::parse_json_view(m_json_view);
        }
        else if(decimals_view.find(c) != std::string_view::npos)
        {

        }
        else if(ws_view.find(c) == std::string_view::npos)
        {
            return nullptr;
        }
    }

    return nullptr;
}