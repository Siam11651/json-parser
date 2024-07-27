#include <parser.hpp>
#include <utils.hpp>
#include <string.hpp>
#include <number.hpp>
#include <array>
#include <sstream>

const json::value *json::parser::parse_value(std::string_view &json)
{
    const std::string_view ws_view(WS_CHARS);
    const std::string_view number_first_view(SIGNED_DECIMAL_CHARS);

    while(!m_json_view.empty())
    {
        const char &c = m_json_view.front();

        if(c == '\"')
        {
            return parse_string(m_json_view);
        }
        else if(number_first_view.find(c) != std::string_view::npos)
        {
            return parse_number(m_json_view);
        }
        else if(ws_view.find(c) != std::string_view::npos)
        {
            m_json_view.remove_prefix(1);
        }
        else
        {
            return nullptr;
        }
    }

    return nullptr;
}

const json::string *json::parser::parse_string(std::string_view &json)
{
    size_t start = 0;

    if(json.front() == '\"')
    {
        start = 1;
    }

    std::string value;
    bool valid = false;
    /**
     * @brief
     * escaped 4 -> non escaped
     * escaped 5 -> escaped
     * escaped 0-3 -> hex
     */
    uint8_t escaped = 4;
    std::array<uint8_t, 4> hex_values;
    size_t trim_size = json.size();

    for(size_t i = start; i < json.size(); ++i)
    {
        const char &c = json[i];

        if(escaped == 4)
        {
            if(c == '\\')
            {
                escaped = 5;
            }
            else if(c == '\"')
            {
                valid = true;
                trim_size = i + 1;

                break;
            }
            else if(0x20 <= c && c <= 0x7f)
            {
                value.push_back(c);
            }
            else
            {
                break;
            }
        }
        else if(escaped > 4)
        {
            if(c == 'b')
            {
                value.push_back('\b');

                escaped = 4;
            }
            else if(c == 'f')
            {
                value.push_back('\f');

                escaped = 4;
            }
            else if(c == 'n')
            {
                value.push_back('\n');

                escaped = 4;
            }
            else if(c == 'r')
            {
                value.push_back('\r');

                escaped = 4;
            }
            else if(c == 't')
            {
                value.push_back('\t');

                escaped = 4;
            }
            else if(c == 'u')
            {
                escaped = 0;
            }
            else
            {
                value.push_back(c);

                const constexpr char esc_chars[] = "\\\"/";
                const std::string_view esc_chars_view(esc_chars);

                if(esc_chars_view.find(c) == std::string_view::npos)
                {
                    valid = false;
                }

                escaped = 4;
            }
        }
        else
        {
            if('0' <= c && c <= '9')
            {
                hex_values[escaped] = c - '0';
            }
            else if('A' <= c && c <= 'F')
            {
                hex_values[escaped] = c - 'A' + 10;
            }
            else if('a' <= c && c <= 'f')
            {
                hex_values[escaped] = c - 'a' + 10;
            }
            else
            {
                escaped = 5;
                valid = false;

                continue;
            }

            ++escaped;

            if(escaped == 4)
            {
                value.push_back(hex_values[0] << 4 | hex_values[1]);
                value.push_back(hex_values[2] << 4 | hex_values[3]);
            }
        }
    }

    json.remove_prefix(trim_size);

    string* const to_return = new string();
    to_return->m_valid = valid;
    to_return->m_value = std::move(value);

    return to_return;
}

const json::number *json::parser::parse_number(std::string_view &json)
{
    /**
     * @brief 
     * state 0 integer part, 
     * state 1 fraction part and
     * state 2 exponent part
     */
    uint8_t state = 0;
    std::string value;
    bool valid = true;

    for(size_t i = 0; i < json.size(); ++i)
    {
        const char &c  = json[i];

        if(state == 0)
        {
            if(c == '.')
            {
                state = 1;
                valid = false;
            }
            else
            {
                if(i == 0 && c == '-')
                {
                    valid = false;
                }
                else
                {
                    const std::string_view char_view(json::SIGNED_DECIMAL_CHARS + 1);

                    if(char_view.find(c) == std::string_view::npos)
                    {
                        break;
                    }
                    else
                    {
                        valid = true;
                    }
                }
            }

            value.push_back(c);
        }
        else if(state == 1)
        {
            if(c == 'e' || c == 'E')
            {
                if(valid)
                {
                    state = 2;
                    valid = false;
                }
                else
                {
                    break;
                }
            }
            else
            {
                const std::string_view char_view(json::SIGNED_DECIMAL_CHARS + 1);

                if(char_view.find(c) == std::string_view::npos)
                {
                    break;
                }
                else
                {
                    valid = true;
                }
            }

            value.push_back(c);
        }
        else
        {
            const std::string_view char_view(i == 0 ? json::SIGNED_DECIMAL_CHARS : json::SIGNED_DECIMAL_CHARS + 1);

            if(char_view.find(c) == std::string_view::npos)
            {
                break;
            }
            else
            {
                valid = true;
            }
        }
    }

    json.remove_prefix(value.size());

    number *to_return = new number();
    to_return->m_valid = valid;

    {
        std::stringstream ss(value);

        ss >> to_return->m_value;
    }

    return to_return;
}

const json::object *json::parser::parse_object(std::string_view &json)
{
    return nullptr;
}

json::parser::parser(const std::string_view &json_view)
{
    m_json_view = json_view;
}

const json::value *json::parser::make_value()
{
    return parse_value(m_json_view);
}