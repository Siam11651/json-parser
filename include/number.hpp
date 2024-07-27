#ifndef NUMBER_H
#define NUMBER_H

#include <value.hpp>
#include <string_view>

namespace json
{
    class number : public value
    {
        friend class parser;

    private:
        double m_value;

    public:
        number();
        const double &get_value() const;
    };
}

#endif