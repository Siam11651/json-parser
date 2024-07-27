#ifndef ARRAY_H
#define ARRAY_H

#include <json/parser.hpp>
#include <json/value.hpp>
#include <vector>

namespace json
{
    class array : public value
    {
        friend class parser;

    private:
        std::vector<const value *> m_value;

    public:
        array();
        const value *operator[] (const size_t &index) const;
        size_t get_size() const;
        ~array();
    };
}

#endif