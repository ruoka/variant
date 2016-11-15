#include <variant>
#include <cassert>

int main()
{
    try
    {
        std::variant<unsigned,double,char> v{9u};
        std::get<char>(v);
        assert(false);
    }
    catch(std::bad_variant_access& bad)
    {}

    try
    {
        std::variant<unsigned,double,char> v{9u};
        std::get<2>(v);
        assert(false);
    }
    catch(std::bad_variant_access& bad)
    {}

    try
    {
        std::variant<unsigned,double,char> v{9u};
        std::get<unsigned>(v);
    }
    catch(std::bad_variant_access& bad)
    {
        assert(false);
    }

    try
    {
        std::variant<unsigned,double,char> v{9u};
        std::get<0>(v);
    }
    catch(std::bad_variant_access& bad)
    {
        assert(false);
    }
}
