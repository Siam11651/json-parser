#ifndef VALUE_H
#define VALUE_H

namespace json
{
    class value
    {
    public:
        enum class type
        {
            string,
            number,
            object,
            array
        };

    protected:
        bool m_valid = true;
        type m_type;

        value(const type &_type);
        
    public:
        const bool &is_valid() const;
        const type &get_type() const;
        virtual ~value();
    };
}

#endif