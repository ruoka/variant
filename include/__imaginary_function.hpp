#pragma once

namespace std::__helper {

    template <typename T>
    struct __argument {using type = T;};

    template <typename... Ts>
    struct __imaginary_function; // undefined

    template <>
    struct __imaginary_function<> {constexpr void operator()() const noexcept;};

    template <typename... Ts>
    struct __imaginary_function<void, Ts...> : __imaginary_function<Ts...>
    {
        using __imaginary_function<Ts...>::operator();
        constexpr __argument<void> operator()() const noexcept;
    };

    template <typename T, typename... Ts>
    struct __imaginary_function<T, Ts...> : __imaginary_function<Ts...>
    {
        using __imaginary_function<Ts...>::operator();
        constexpr __argument<T> operator()(T) const noexcept;
    };


    template <class T, class... Types>
    struct __count; // undefined

    template <class T, class... Types>
    constexpr size_t __count_v = __count<T, Types...>::value;

    template <class T,class U>
    struct __count<T,U> : integral_constant<size_t, is_same_v<T,U >> {};

    template <class T, class U, class... Types>
    struct __count<T, U, Types...> : integral_constant<size_t, is_same_v<T,U > + __count_v<T, Types...>> {};


    template <class T, class... Types>
    using __imaginary_function_argument_t = typename result_of_t<__helper::__imaginary_function<Types...>(T)>::type;


    template <class T, class... Types>
    constexpr bool __imaginary_function_well_formed_v = __count<T, Types...>::value == 1;

} // namespace std::__helper
