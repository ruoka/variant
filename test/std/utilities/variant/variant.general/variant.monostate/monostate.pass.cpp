#include <variant>
#include <string>
#include <vector>

using std::variant;
using std::monostate;

struct foo
{
    foo() = delete;
    foo(const foo&) = delete;
    foo(foo&&) = delete;
    foo(bool){}
    virtual ~foo(){}
};

int main()
{
    variant<monostate,foo> v;
    assert(std::holds_alternative<monostate>(v));
    v.emplace<foo>(true);
    assert(std::holds_alternative<foo>(v));
}
