#include <parser.hpp>

int main()
{
    json::parser parser("\"\\u0053\"");
    std::unique_ptr<json::value> json_value = parser.make_value();

    return 0;
}