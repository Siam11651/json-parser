#include <number.hpp>
#include <utils.hpp>
#include <cstdint>
#include <string>
#include <sstream>

json::number::number() : value(type::number)
{

}

const double &json::number::get_value() const
{
    return m_value;
}

const json::number* json::number::parse_json_view(std::string_view &json)
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
                const std::string_view char_view(i == 0 ? json::SIGNED_DECIMAL_CHARS : json::SIGNED_DECIMAL_CHARS + 1);

                if(char_view.find(c) == std::string_view::npos)
                {
                    break;
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