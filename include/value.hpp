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
        value(const value &) = delete;      // only use pointers which are generated >:(
        value(const value &&) = delete;
        const bool &is_valid() const;
        const type &get_type() const;
        virtual ~value();
    };
}

#endif