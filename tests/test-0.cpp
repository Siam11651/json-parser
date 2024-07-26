#include <parser.hpp>
#include <string.hpp>
#include <number.hpp>

int main()
{
    json::parser parser("   1234.567   {}");
    const json::value* const json_value = parser.make_value();
    const json::number * const json_number = (const json::number *)json_value;

    return 0;
}