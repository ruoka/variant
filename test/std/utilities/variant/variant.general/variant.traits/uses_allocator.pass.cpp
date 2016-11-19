#include<variant>
#include<string>

using std::variant;
using std::uses_allocator_v;

int main()
{
    struct foo {};
    static_assert(uses_allocator_v<variant<bool>,std::allocator<char>>);
    static_assert(uses_allocator_v<variant<std::string,bool>,std::allocator<char>>);
    static_assert(uses_allocator_v<variant<foo>,std::allocator<char>>);
    static_assert(uses_allocator_v<variant<foo,bool>,std::allocator<char>>);
    static_assert(uses_allocator_v<variant<foo,std::string,bool>,std::allocator<char>>);
}
