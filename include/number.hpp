#ifndef NUMBER_H
#define NUMBER_H

#include <value.hpp>
#include <string_view>

namespace json
{
    class number : public value
    {
    private:
        double m_value;

    public:
        number();
        const double &get_value() const;
        static const number* parse_json_view(std::string_view &json);
    };
}

#endif