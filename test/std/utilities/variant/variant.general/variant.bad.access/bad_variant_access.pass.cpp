#include <variant>
#include <cassert>

using std::variant;
using std::get;

int main()
{
    try
    {
        variant<unsigned,double,char> v{9u};
        get<char>(v);
        assert(false);
    }
    catch(std::bad_variant_access& bad)
    {}

    try
    {
        variant<unsigned,double,char> v{9u};
        get<2>(v);
        assert(false);
    }
    catch(std::bad_variant_access& bad)
    {}

    try
    {
        variant<unsigned,double,char> v{9u};
        get<unsigned>(v);
    }
    catch(std::bad_variant_access& bad)
    {
        assert(false);
    }

    try
    {
        variant<unsigned,double,char> v{9u};
        get<0>(v);
    }
    catch(std::bad_variant_access& bad)
    {
        assert(false);
    }
}
