#include <parser.hpp>
#include <string.hpp>
#include <number.hpp>

int main()
{
    json::parser parser("\"1234.567\"");
    const json::value* const json_value = parser.make_value();
    const json::string * const json_string = (const json::string *)json_value;

    return 0;
}