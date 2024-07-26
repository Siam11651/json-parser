#include <string.hpp>
#include <list>
#include <array>
#include <cstdint>

json::string::string() : value(value::type::string)
{
    
}

json::string::string(const string &other) : value(value::type::string)
{
    m_valid = other.m_valid;
    m_value = other.m_value;
}

json::string::string(const string &&other) : value(value::type::string)
{
    m_valid = other.m_valid;
    m_value = std::move(other.m_value);
}

const json::string* json::string::parse_json_view(std::string_view &json)
{
    size_t start = 0;

    if(json.front() == '\"')
    {
        start = 1;
    }

    std::string value;
    bool valid = false;
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

const std::string &json::string::get_value() const
{
    return m_value;
}