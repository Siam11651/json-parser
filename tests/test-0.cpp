#include <parser.hpp>
#include <string.hpp>

int main()
{
    json::parser parser("\"\\u0053\"");
    const json::value* const json_value = parser.make_value();

    return 0;
}