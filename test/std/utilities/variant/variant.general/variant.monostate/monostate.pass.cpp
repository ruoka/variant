#include <variant>
#include <string>
#include <vector>

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
    std::variant<std::monostate,foo> v;
    assert(std::holds_alternative<std::monostate>(v));
    v.emplace<foo>(true);
    assert(std::holds_alternative<foo>(v));
}
