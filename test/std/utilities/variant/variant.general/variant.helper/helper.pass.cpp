#include <variant>
#include <string>
#include <vector>

int main()
{
    using V = std::variant<unsigned, double, char, float, std::string, std::vector<int>>;
    static_assert(std::variant_size_v<V> == 6);
    static_assert(std::is_same_v<std::variant_alternative_t<5, V>, std::vector<int>>);
}
