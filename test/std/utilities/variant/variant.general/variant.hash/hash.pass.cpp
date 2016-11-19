#include <variant>
#include <cassert>

using std::variant;

struct foo{};

int main()
{
    std::hash<variant<unsigned,double,char>> hv;
    std::hash<unsigned> hu;
    std::hash<double> hd;
    std::hash<char> hc;

    variant<unsigned,double,char> v;

    v = 123u;
    assert(hv(v) == hu(123u));

    v = 123.456;
    assert(hv(v) == hd(123.456));

    v = 'R';
    assert(hv(v) == hc('R'));
}
