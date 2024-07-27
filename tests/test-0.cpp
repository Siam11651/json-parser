#include <parser.hpp>
#include <string.hpp>
#include <number.hpp>
#include <object.hpp>
#include <array.hpp>

int main()
{
    json::parser parser(R"({
        "abc":
        {
            "def": 123,
            "ghi":
            [
                "hehe",
                "hoho"
            ]
        }
    })");
    const json::value *const json_value = parser.make_value();
    const json::object *const json_object = (const json::object *)json_value;
    const json::object *const abc = (const json::object *)json_object->get_field("abc");
    const json::number *const def = (const json::number *)abc->get_field("def");
    const json::array *const ghi = (const json::array *)abc->get_field("ghi");
    const json::string *const ghi0 = (const json::string *)(*ghi)[0];
    const json::string *const ghi1 = (const json::string *)(*ghi)[1];

    return 0;
}