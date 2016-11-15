#include<variant>
#include<string>

int main()
{
    struct foo {};
    static_assert(std::uses_allocator_v<std::variant<bool>,std::allocator<char>>);
    static_assert(std::uses_allocator_v<std::variant<std::string,bool>,std::allocator<char>>);
    static_assert(std::uses_allocator_v<std::variant<foo>,std::allocator<char>>);
    static_assert(std::uses_allocator_v<std::variant<foo,bool>,std::allocator<char>>);
    static_assert(std::uses_allocator_v<std::variant<foo,std::string,bool>,std::allocator<char>>);
}
