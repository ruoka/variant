#include <variant>
#include <cassert>

using std::variant;

struct foo{};

int main()
{
    variant<float,unsigned,foo,char> v1, v2;
    v1 = 123456u;
    v2 = 'A';
    assert(std::get<1>(v1) == 123456u);
    assert(std::get<3>(v2) == 'A');
    v1.swap(v2);
    assert(std::get<1>(v2) == 123456u);
    assert(std::get<3>(v1) == 'A');
}
