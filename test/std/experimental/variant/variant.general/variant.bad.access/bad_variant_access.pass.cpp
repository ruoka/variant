#include <experimental/variant>
#include <cassert>

using std::experimental::variant;
using std::experimental::get;
using std::experimental::bad_variant_access;

int main()
{
    try
    {
        variant<unsigned,double,char> v{9u};
        get<char>(v);
        assert(false);
    }
    catch(bad_variant_access& bad)
    {}

    try
    {
        variant<unsigned,double,char> v{9u};
        get<2>(v);
        assert(false);
    }
    catch(bad_variant_access& bad)
    {}

    try
    {
        variant<unsigned,double,char> v{9u};
        get<unsigned>(v);
    }
    catch(bad_variant_access& bad)
    {
        assert(false);
    }

    try
    {
        variant<unsigned,double,char> v{9u};
        get<0>(v);
    }
    catch(bad_variant_access& bad)
    {
        assert(false);
    }
}