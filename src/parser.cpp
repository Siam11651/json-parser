#include <string.hpp>
#include <number.hpp>
#include <object.hpp>
#include <array.hpp>
#include <utils.hpp>
#include <array>
#include <sstream>
#include <map>

const json::value *json::parser::parse_value()
{
    const std::string_view ws_view(WS_CHARS);
    const std::string_view number_first_view(SIGNED_DECIMAL_CHARS);

    while(!m_json_view.empty())
    {
        const char &c = m_json_view.front();

        if(ws_view.find(c) == std::string_view::npos)
        {
            if(c == '\"')
            {
                return parse_string();
            }
            else if(c == '{')
            {
                return parse_object();
            }
            else if(c == '[')
            {
                return parse_array();
            }
            else if(number_first_view.find(c) != std::string_view::npos)
            {
                return parse_number();
            }
            else
            {
                return nullptr;
            }
        }
        else
        {
            m_json_view.remove_prefix(1);
        }
    }

    return nullptr;
}

const json::string *json::parser::parse_string()
{
    if(m_json_view.front() == '\"')
    {
        m_json_view.remove_prefix(1);
    }
    else
    {
        return nullptr;
    }

    if(m_json_view.empty())
    {
        return nullptr;
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
    size_t trim_size = m_json_view.size();

    for(size_t i = 0; i < m_json_view.size(); ++i)
    {
        const char &c = m_json_view[i];

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

    m_json_view.remove_prefix(trim_size);

    string* const to_return = new string();

    if(valid)
    {
        to_return->m_value = std::move(value);
    }
    else
    {
        to_return->m_valid = false;
    }

    return to_return;
}

const json::number *json::parser::parse_number()
{
    /**
     * @brief 
     * state 0 integer part, 
     * state 1 fraction part and
     * state 2 exponent part
     */
    uint8_t state = 0;
    std::string value;
    size_t trim_size = m_json_view.size();
    bool valid = true;

    for(size_t i = 0; i < m_json_view.size(); ++i)
    {
        const char &c  = m_json_view[i];

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
                        trim_size = i;

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

    m_json_view.remove_prefix(trim_size);

    number *to_return = new number();

    if(valid)
    {
        std::stringstream ss(value);

        ss >> to_return->m_value;
    }
    else
    {
        to_return->m_valid = false;
    }

    return to_return;
}

const json::object *json::parser::parse_object()
{
    if(m_json_view.front() == '{')
    {
        m_json_view.remove_prefix(1);
    }
    else
    {
        return nullptr;
    }

    if(m_json_view.empty())
    {
        return nullptr;
    }

    /**
     * @brief
     * state 0 -> expect new member or '}'
     * state 1 -> expect ':'
     * state 2 -> expect value
     * state 3 -> expect ',' or '}'
     * state 4 -> expect new member
     */
    uint8_t state = 0;
    bool valid = true;
    std::string field_name;
    std::map<std::string, const value *> fields;
    const std::string_view ws_view(WS_CHARS);

    while(!m_json_view.empty())
    {
        const char &c = m_json_view.front();

        if(state == 1)
        {
            if(ws_view.find(c) == std::string_view::npos)
            {
                if(c == ':')
                {
                    state = 2;

                    m_json_view.remove_prefix(1);
                }
                else
                {
                    valid = false;

                    break;
                }
            }
            else
            {
                m_json_view.remove_prefix(1);
            }
        }
        else if(state == 2)
        {
            const value *new_value = parse_value();

            if(new_value && new_value->is_valid())
            {
                state = 3;

                fields.insert({field_name, new_value});
            }
            else
            {
                valid = false;

                break;
            }
        }
        else if(state == 3)
        {
            if(ws_view.find(c) == std::string_view::npos)
            {
                if(c == ',')
                {
                    state = 4;

                    m_json_view.remove_prefix(1);
                }
                else if(c == '}')
                {
                    m_json_view.remove_prefix(1);

                    break;
                }
                else
                {
                    valid = false;

                    break;
                }
            }
            else
            {
                m_json_view.remove_prefix(1);
            }
        }
        else if(state == 4)
        {
            if(ws_view.find(c) == std::string_view::npos)
            {
                const string *new_string = parse_string();

                if(new_string && new_string->is_valid())
                {
                    state = 1;
                    field_name = new_string->get_value();

                    delete new_string;
                }
                else
                {
                    valid = false;

                    break;
                }
            }
            else
            {
                m_json_view.remove_prefix(1);
            }
        }
        else    // put state 0 in the end cuz it is supposed to be used only once
        {
            if(ws_view.find(c) == std::string_view::npos)
            {
                if(c == '}')
                {
                    m_json_view.remove_prefix(1);

                    break;
                }
                else
                {
                    const string *new_string = parse_string();
                
                    if(new_string && new_string->is_valid())
                    {
                        state = 1;
                        field_name = new_string->get_value();

                        delete new_string;
                    }
                    else
                    {
                        valid = false;

                        break;
                    }
                }
            }
            else
            {
                m_json_view.remove_prefix(1);
            }
        }
    }

    object *to_return = new object();

    if(valid)
    {
        to_return->m_fields = std::move(fields);
    }
    else
    {
        to_return->m_valid = false;
    }

    return to_return;
}

const json::array *json::parser::parse_array()
{
    if(m_json_view.front() == '[')
    {
        m_json_view.remove_prefix(1);
    }
    else
    {
        return nullptr;
    }

    if(m_json_view.empty())
    {
        return nullptr;
    }

    /**
     * @brief
     * state 0 -> expect new element or ']'
     * state 1 -> expect ',' or ']'
     * state 2 -> expect new element
     */
    uint8_t state = 0;
    bool valid = true;
    std::vector<const value *> values;
    const std::string_view ws_view(WS_CHARS);

    while(!m_json_view.empty())
    {
        const char &c = m_json_view.front();

        if(state == 1)
        {
            if(ws_view.find(c) == std::string_view::npos)
            {
                if(c == ',')
                {
                    state = 2;

                    m_json_view.remove_prefix(1);
                }
                else if(c == ']')
                {
                    m_json_view.remove_prefix(1);

                    break;
                }
                else
                {
                    valid = false;

                    break;
                }
            }
            else
            {
                m_json_view.remove_prefix(1);
            }
        }
        else if(state == 2)
        {
            if(ws_view.find(c) == std::string_view::npos)
            {
                const value *new_value = parse_value();

                if(new_value && new_value->is_valid())
                {
                    state = 1;

                    values.push_back(new_value);
                }
                else
                {
                    valid = false;

                    break;
                }
            }
            else
            {
                m_json_view.remove_prefix(1);
            }
        }
        else    // put state 0 in the end cuz it is supposed to be used only once
        {
            if(ws_view.find(c) == std::string_view::npos)
            {
                if(c == '}')
                {
                    m_json_view.remove_prefix(1);

                    break;
                }
                else
                {
                    const value *new_value = parse_value();
                
                    if(new_value && new_value->is_valid())
                    {
                        state = 1;
                        
                        values.push_back(new_value);
                    }
                    else
                    {
                        valid = false;

                        break;
                    }
                }
            }
            else
            {
                m_json_view.remove_prefix(1);
            }
        }
    }

    array *to_return = new array();

    if(valid)
    {
        to_return->m_value = std::move(values);
    }
    else
    {
        to_return->m_valid = false;
    }

    return to_return;
}

json::parser::parser(const std::string_view &_json_view)
{
    m_json_view = _json_view;
}

const json::value *json::parser::make_value()
{
    return parse_value();
}