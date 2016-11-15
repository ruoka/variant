#pragma once

#include <functional>
#include <array>
#include <cassert>

#include "__variant_storage.hpp"

namespace std {

    // 20.7.2, variant of value types
    template <class... Types>
    class variant;

    // 20.7.3, variant helper classes
    template <class T> struct variant_size; // undefined
    template <class T> struct variant_size<const T>;
    template <class T> struct variant_size<volatile T>;
    template <class T> struct variant_size<const volatile T>;

    template <class T> constexpr size_t variant_size_v = variant_size<T>::value;

    template <class... Types>
    struct variant_size<variant<Types...>> : integral_constant<size_t, sizeof...(Types)> {};

    template <size_t I, class T> struct variant_alternative; // undefined
    template <size_t I, class T> struct variant_alternative<I, const T>;
    template <size_t I, class T> struct variant_alternative<I, volatile T>;
    template <size_t I, class T> struct variant_alternative<I, const volatile T>;

    template <size_t I, class T>
    using variant_alternative_t = typename variant_alternative<I, T>::type;

    template <size_t I, class... Types>
    struct variant_alternative<I, variant<Types...>>
    {
        static_assert(I < sizeof...(Types), "variant_alternative index out of range");
        using type = tuple_element_t<I, tuple<Types...>>;
    };

    constexpr size_t variant_npos = -1;

    // helper templates

    namespace __helper {

        template <class... Types>
        class __variant_base;

        template <class T, class... Types>
        struct __index; // undefined

        template <class T>
        struct __index<T> : integral_constant<size_t, 1> {};

        template <class T, class... Types>
        struct __index<T, T, Types...> : integral_constant<size_t, 0> {};

        template <class T, class U, class... Types>
        struct __index<T, U, Types...> : integral_constant<size_t, 1 + __index<T, Types...>::value> {};

        template <class T, class... Types>
        constexpr size_t __index_v = __index<T, Types...>::value;

    } // namespace __helper

    // 20.7.10, class bad_variant_access
    class bad_variant_access : logic_error {
    public:
        bad_variant_access() : logic_error{"bad variant access"} {};
    };

    // 20.7.4, value access
    template <class T, class... Types>
    constexpr bool holds_alternative(const variant<Types...>& v) noexcept
    {
        return __helper::__index_v<T, Types...> == v.index();
    };

    template <size_t I, class... Types>
    constexpr variant_alternative_t<I, variant<Types...>>&
    get(variant<Types...>& v)
    {
        using T = variant_alternative_t<I,variant<Types...>>;
        return get<T>(v);
    };

    template <size_t I, class... Types>
    constexpr variant_alternative_t<I, variant<Types...>>&&
    get(variant<Types...>&& v)
    {
        using T = variant_alternative_t<I,variant<Types...>>;
        return move(get<T>(v));
    };

    template <class T, class... Types>
    constexpr T&
    get(variant<Types...>& v)
    {
        if(holds_alternative<T>(v))
        return v.m_storage.template get<T>();
        throw bad_variant_access{};
    };

    template <class T, class... Types>
    constexpr T&&
    get(variant<Types...>&& v)
    {
        if(holds_alternative<T>(v))
        return v.m_storage.template get<T>();
        throw bad_variant_access{};
    };

    template <class T, class... Types>
    constexpr const T&
    get(const variant<Types...>& v)
    {
        if(holds_alternative<T>(v))
        return v.m_storage.template get<T>();
        throw bad_variant_access{};
    };

    template <class T, class... Types>
    constexpr const T&&
    get(const variant<Types...>&& v)
    {
        if(holds_alternative<T>(v))
        return v.m_storage.template get<T>();
        throw bad_variant_access{};
    };

    template <size_t I, class... Types>
    constexpr variant_alternative_t<I, variant<Types...>> const&
    get(const variant<Types...>& v)
    {
        using T = variant_alternative_t<I,variant<Types...>>;
        return get<T>(v);
    };

    template <size_t I, class... Types>
    constexpr variant_alternative_t<I, variant<Types...>> const&&
    get(const variant<Types...>&& v)
    {
        using T = variant_alternative_t<I,variant<Types...>>;
        return move(get<T>(v));
    };

    template <size_t I, class... Types>
    constexpr add_pointer_t<variant_alternative_t<I, variant<Types...>>>
    get_if(variant<Types...>* v) noexcept
    {
        using T = variant_alternative_t<I,variant<Types...>>;
        return get_if<T>(v);
    };

    template <size_t I, class... Types>
    constexpr add_pointer_t<const variant_alternative_t<I, variant<Types...>>>
    get_if(const variant<Types...>* v) noexcept
    {
        using T = variant_alternative_t<I,variant<Types...>>;
        return get_if<T>(v);
    };

    template <class T, class... Types>
    constexpr add_pointer_t<T>
    get_if(variant<Types...>* v) noexcept
    {
        if(holds_alternative<T>(*v))
        return &(v->m_storage.template get<T>());
        return nullptr;
    };

    template <class T, class... Types>
    constexpr add_pointer_t<const T>
    get_if(const variant<Types...>* v) noexcept
    {
        if(holds_alternative<T>(*v))
        return &(v->m_storage.template get<T>());
        return nullptr;
    };

    // helper templates

    namespace __helper {

        template <class... Types>
        using __comparison_function_type = bool(*)(const variant<Types...>&, const variant<Types...>&);

        template <size_t I, class... Types>
        constexpr bool __equal_to(const variant<Types...>& v, const variant<Types...>& w)
        {
            return get<I>(v) == get<I>(w);
        }

        template <class... Types, size_t... I>
        constexpr array<__comparison_function_type<Types...>, sizeof...(I)> __make_equal_to_array(index_sequence<I...>)
        {
            return { __equal_to<I, Types...> ... };
        }

        template <size_t I, class... Types>
        constexpr bool __less(const variant<Types...>& v, const variant<Types...>& w)
        {
            return get<I>(v) < get<I>(w);
        }

        template <class... Types, size_t... I>
        constexpr array<__comparison_function_type<Types...>, sizeof...(I)> __make_less_array(index_sequence<I...>)
        {
            return { __less<I, Types...> ... };
        }

    } // namespace __helper

    // 20.7.5, relational operators
    template <class... Types>
    constexpr bool operator==(const variant<Types...>& v, const variant<Types...>& w)
    {
        constexpr auto __equal_to = __helper::__make_equal_to_array<Types...>(index_sequence_for<Types...>{});
        return (v.valueless_by_exception() && w.valueless_by_exception()) || (v.index() == w.index() && __equal_to[v.index()](v,w));
    };

    template <class... Types>
    constexpr bool operator!=(const variant<Types...>& v, const variant<Types...>& w)
    {
        return !(v == w);
    };

    template <class... Types>
    constexpr bool operator<(const variant<Types...>& v, const variant<Types...>& w)
    {
        constexpr auto __less = __helper::__make_less_array<Types...>(index_sequence_for<Types...>{});
        return (v.index() < w.index()) || (v.index() == w.index() && !v.valueless_by_exception() && __less[v.index()](v,w));
    };

    template <class... Types>
    constexpr bool operator>(const variant<Types...>& v, const variant<Types...>& w)
    {
        return w < v;
    }

    template <class... Types>
    constexpr bool operator<=(const variant<Types...>& v, const variant<Types...>& w)
    {
        return !(v > w);
    }

    template <class... Types>
    constexpr bool operator>=(const variant<Types...>& v, const variant<Types...>& w)
    {
        return !(v < w);
    }

    // helper templates

    namespace __helper {

        template <class Visitor, class... Variants>
        using __invoker_function_type = auto(*)(Visitor&&, Variants&&...) -> result_of_t<Visitor(variant_alternative_t<0, decay_t<Variants>> ...)>;

        template <size_t I, class Visitor, class... Variants>
        auto __invoker(Visitor&& vis, Variants&&... vars)
        {
            return invoke(forward<Visitor>(vis), get<I>(forward<Variants>(vars)) ...);
        }

        template <class Visitor, class... Variants, size_t... I>
        constexpr array<__invoker_function_type<Visitor, Variants...>, sizeof...(I)> __make_invoker_array(index_sequence<I...>)
        {
            return { __invoker<I, Visitor, Variants...> ... };
        }

        template <class Visitor, class... Types, class... Variants>
        auto __invoke(Visitor&& vis, variant<Types...>& var, Variants&&... vars)
        {
            constexpr auto __array = __make_invoker_array<Visitor, variant<Types...>, Variants...>(index_sequence_for<Types...>{});
            return __array[var.index()](forward<Visitor>(vis), forward<variant<Types...>>(var), forward<Variants>(vars) ...);
        }

    } // namespace __helper

    // 20.7.6, visitation
    template <class Visitor, class... Variants>
    constexpr auto visit(Visitor&& vis, Variants&&... vars)
    {
        return __helper::__invoke(forward<Visitor>(vis), forward<Variants>(vars) ...);
    };

    // 20.7.7, class monostate
    struct monostate {};

    // 20.7.8, monostate relational operators
    constexpr bool operator<(monostate, monostate) noexcept {return false;};
    constexpr bool operator>(monostate, monostate) noexcept {return false;};
    constexpr bool operator<=(monostate, monostate) noexcept {return true;};
    constexpr bool operator>=(monostate, monostate) noexcept {return true;};
    constexpr bool operator==(monostate, monostate) noexcept {return true;};
    constexpr bool operator!=(monostate, monostate) noexcept {return false;};

    // 20.7.9, specialized algorithms
    template <class... Types>
    void swap(variant<Types...>& lhs, variant<Types...>& rhs) noexcept(conjunction_v<is_nothrow_move_constructible<Types> ...> &&
                                                                       conjunction_v<is_nothrow_swappable<Types> ...>)
    {
        lhs.swap(rhs);
    };

    // 20.7.11, hash support
    // template <class T> struct hash;

    namespace __helper {

        template <class... Types>
        using __hash_function_type = size_t(*)(const variant<Types...>&);

        template <size_t I, class... Types>
        constexpr size_t __hash(const variant<Types...>& v)
        {
            using T = variant_alternative_t<I,variant<Types...>>;
            return hash<T>{}(get<I>(v));
        }

        template <class... Types, size_t... I>
        constexpr array<__hash_function_type<Types...>, sizeof...(I)> __make_hash_array(index_sequence<I...>)
        {
            return { __hash<I, Types...> ... };
        }

    } // namespace __helper

    template <class... Types>
    struct hash<variant<Types...>>
    {
        size_t operator()(const variant<Types...>& v) noexcept
        {
            constexpr auto hash = __helper::__make_hash_array<Types...>(index_sequence_for<Types...>{});
            return hash[v.index()](v);
        }
    };

    template <>
    struct hash<monostate>
    {
        size_t operator()(monostate) noexcept
        {
            return 42;
        }
    };

    // 20.7.12, allocator-related traits
    // template <class T, class Alloc> struct uses_allocator;
    template <class... Types, class Alloc>
    struct uses_allocator<variant<Types...>, Alloc> : true_type {};

} // namespace std

namespace std {

    // helper templates

    namespace __helper {

        template <class... Types>
        using __swap_function_type = void(*)(variant<Types...>&, variant<Types...>&);

        template <size_t I, class... Types>
        constexpr void __swap(variant<Types...>& v, variant<Types...>& w) noexcept
        {
            swap(get<I>(v), get<I>(w));
        }

        template <class... Types, size_t... I>
        constexpr array<__swap_function_type<Types...>, sizeof...(I)> __make_swap_array(index_sequence<I...>)
        {
            return { __swap<I, Types...> ... };
        }

        template <class... Types>
        using __constructor_function_type = void(*)(__variant_base<Types...>&, const __variant_base<Types...>&);

        template <size_t I, class... Types>
        constexpr void __construct(__variant_base<Types...>& v, const __variant_base<Types...>& w) noexcept
        {
            using T = variant_alternative_t<I,variant<Types...>>;
            v.template __construct<T>(get<T>(w));
        }

        template <class... Types, size_t... I>
        constexpr array<__constructor_function_type<Types...>, sizeof...(I)> __make_construct_array(index_sequence<I...>)
        {
            return { __construct<I, Types...> ... };
        }

        template <class Alloc, class... Types>
        using __allocator_function_type = void(*)(allocator_arg_t, Alloc, __variant_base<Types...>&, const __variant_base<Types...>&);

        template <size_t I, class Alloc, class... Types>
        constexpr void __construct(allocator_arg_t, Alloc a, __variant_base<Types...>& v, const __variant_base<Types...>& w) noexcept
        {
            using T = variant_alternative_t<I,variant<Types...>>;
            // v.template __construct<T>(allocator_arg_t{}, a, get<T>(w));
            v.template __construct<T, Alloc>(allocator_arg_t{}, a, get<T>(w));
        }

        template <class Alloc, class... Types, size_t... I>
        constexpr array<__allocator_function_type<Alloc, Types...>, sizeof...(I)> __make_allocator_construct_array(index_sequence<I...>)
        {
            return { __construct<I, Alloc, Types...> ... };
        }

        template <class... Types>
        using __member_function_type = void(*)(__variant_base<Types...>&);

        template <size_t I, class... Types>
        constexpr void __destroy(__variant_base<Types...>& v) noexcept
        {
            using T = variant_alternative_t<I,variant<Types...>>;
            v.template __destroy<T>();
        }

        template <class... Types, size_t... I>
        constexpr array<__member_function_type<Types...>, sizeof...(I)> __make_destroy_array(index_sequence<I...>)
        {
            return { __destroy<I, Types...> ... };
        }

        template <class... Types>
        class __variant_base {

            using __T0 = variant_alternative_t<0,variant<Types...>>;

        public:

            __storage<Types...> m_storage;

            // 20.7.2.1, constructors
            constexpr __variant_base() noexcept(is_nothrow_default_constructible_v<__T0>) :
                m_storage{in_place<__T0>},
                m_index{0}
            {
                static_assert(is_default_constructible_v<__T0>,
                    "This function shall not participate in overload resolution unless is_default_constructible_v<T0> is true.");
            };

            __variant_base(const variant<Types...>& v) :
                m_storage{},
                m_index{v.index()}
            {
                static_assert(conjunction_v<is_copy_constructible<Types> ...>,
                    "This function shall not participate in overload resolution unless is_copy_constructible_v<Ti> is true for all i.");
                __construct(v);
            };

            __variant_base(variant<Types...>&& v) noexcept(conjunction_v<is_nothrow_move_constructible<Types> ...>) :
                m_storage{},
                m_index{v.index()}
            {
                static_assert(conjunction_v<is_move_constructible<Types> ...>,
                    "This function shall not participate in overload resolution unless is_move_constructible_v<Ti> is true for all i.");
                __construct(forward<variant<Types...>>(v));
            };

            template <class T,
                      enable_if_t<conjunction_v<
                                 negation< is_same< decay_t<T> , variant<Types...> > >,
                                 is_constructible< T , T >
                                 >,
                                 bool> = true
                      >
            constexpr __variant_base(T&& t) noexcept(conjunction_v<is_nothrow_constructible<Types> ...>) :
                m_storage{in_place<T>, forward<T>(t)},
                m_index{__helper::__index_v<T, Types...>}
            {
                static_assert(conjunction_v<is_constructible<Types,Types> ...>,
                    "This function shall not participate in overload resolution unless is_constructible_v<Ti> is true for all i.");
            };

            template <class T,
                      class... Args,
                      enable_if_t<is_constructible_v<T, Args...>, bool> = true
                      >
            constexpr explicit __variant_base(in_place_type_t<T>, Args&&... args) :
                m_storage{in_place<T>, forward<Args>(args)...},
                m_index{__helper::__index_v<T, Types...>}
            {};

            template <class T,
                      class U,
                      class... Args,
                      enable_if_t<is_constructible_v<T, initializer_list<U>&, Args...>, bool> = true
                      >
            constexpr explicit __variant_base(in_place_type_t<T>, initializer_list<U> il, Args&&... args) :
                m_storage{in_place<T>, forward<initializer_list<U>>(il), forward<Args>(args)...},
                m_index{__helper::__index_v<T, Types...>}
            {};

            template <size_t I,
                      class... Args,
                      class T = variant_alternative_t<I,variant<Types...>>,
                      enable_if_t<is_constructible_v<T, Args...>, bool> = true
                      >
            constexpr explicit __variant_base(in_place_index_t<I>, Args&&... args) :
                m_storage{in_place<T>, forward<Args>(args)...},
                m_index{I}
            {};

            template <size_t I,
                      class U,
                      class... Args,
                      class T = variant_alternative_t<I,variant<Types...>>,
                      enable_if_t<is_constructible_v<T, initializer_list<U>&, Args...>, bool> = true
                      >
            constexpr explicit __variant_base(in_place_index_t<I>, initializer_list<U> il, Args&&... args) :
                m_storage{in_place<variant_alternative_t<I,variant<Types...>>>, forward<initializer_list<U>>(il), forward<Args>(args)...},
                m_index{I}
            {};

            // allocator-extended constructors
            template <class Alloc>
            __variant_base(allocator_arg_t, const Alloc& a) :
                m_storage{}
            {
                static_assert(is_default_constructible_v<__T0>,
                    "This function shall not participate in overload resolution unless is_default_constructible_v<T0> is true.");
                __construct<__T0>(allocator_arg_t{}, a);
            };

            template <class Alloc>
            __variant_base(allocator_arg_t, const Alloc& a, const variant<Types...>& v) :
                m_storage{},
                m_index{v.index()}
            {
                static_assert(conjunction_v<is_copy_constructible<Types> ...>,
                    "This function shall not participate in overload resolution unless is_copy_constructible_v<Ti> is true for all i.");
                __construct(allocator_arg_t{}, a, v);
            }

            template <class Alloc>
            __variant_base(allocator_arg_t, const Alloc& a, variant<Types...>&& v) :
                m_storage{},
                m_index{v.index()}
            {
                static_assert(conjunction_v<is_move_constructible<Types> ...>,
                    "This function shall not participate in overload resolution unless is_move_constructible_v<Ti> is true for all i.");
                __construct(allocator_arg_t{}, a, forward<__variant_base>(v));
            }

            template <class Alloc,
                      class T,
                      enable_if_t<conjunction_v<
                                 negation< is_same< decay_t<T> , variant<Types...> > >,
                                 is_constructible< T , T >
                                 >,
                                 bool> = true
                      >
            __variant_base(allocator_arg_t, const Alloc& a, T&& t) :
                m_storage{}
            {
                static_assert(conjunction_v<is_constructible<Types,Types> ...>,
                    "This function shall not participate in overload resolution unless is_constructible_v<Ti> is true for all i.");
                __construct<T>(allocator_arg_t{}, a, t);
            };

            template <class Alloc,
                      class T,
                      class... Args,
                      enable_if_t<is_constructible_v<T, Args...>, bool> = true
                      >
            __variant_base(allocator_arg_t, const Alloc& a, in_place_type_t<T>, Args&&... args) :
                m_storage{}
            {
                __construct<T>(allocator_arg_t{}, a, forward<Args>(args) ...);
            };

            template <class Alloc,
                      class T,
                      class U,
                      class... Args,
                      enable_if_t<is_constructible_v<T, initializer_list<U>&, Args...>, bool> = true
                      >
            __variant_base(allocator_arg_t, const Alloc& a, in_place_type_t<T>, initializer_list<U> il, Args&&... args) :
                m_storage{}
            {
                __construct<T>(allocator_arg_t{}, a, forward<initializer_list<U>>(il), forward<Args>(args) ...);
            };

            template <class Alloc,
                      size_t I,
                      class... Args,
                      class T = variant_alternative_t<I,variant<Types...>>,
                      enable_if_t<is_constructible_v<T, Args...>, bool> = true
                      >
            __variant_base(allocator_arg_t, const Alloc& a, in_place_index_t<I>, Args&&... args) :
                m_storage{}
            {
                __construct<T>(allocator_arg_t{}, a, forward<Args>(args) ...);
            };

            template <class Alloc,
                      size_t I,
                      class U,
                      class... Args,
                      class T = variant_alternative_t<I,variant<Types...>>,
                      enable_if_t<is_constructible_v<T, initializer_list<U>&, Args...>, bool> = true
                      >
            __variant_base(allocator_arg_t, const Alloc& a, in_place_index_t<I>, initializer_list<U> il, Args&&... args) :
                m_storage{}
            {
                __construct<T>(allocator_arg_t{}, a, forward<initializer_list<U>>(il), forward<Args>(args) ...);
            };

            // 20.7.2.2, destructor
            // ~__variant_base();

            // 20.7.2.3, assignment
            __variant_base& operator=(const variant<Types...>& rhs)
            {
                static_assert(conjunction_v<is_copy_constructible<Types> ...> &&
                              conjunction_v<is_move_constructible<Types> ...> &&
                              conjunction_v<is_copy_assignable   <Types> ...> ,
                R"(This function shall not participate in overload resolution unless is_copy_constructible_v<Ti> &&
                                                                                     is_move_constructible_v<Ti> &&
                                                                                     is_copy_assignable_v<Ti> is true for all i.)");
                if(!valueless_by_exception())
                    __destroy();
                if(!rhs.valueless_by_exception())
                    __construct(rhs);
                return *this;
            };

            __variant_base& operator=(variant<Types...>&& rhs) noexcept(conjunction_v<is_nothrow_move_constructible<Types> ...> &&
                                                                        conjunction_v<is_nothrow_move_assignable   <Types> ...>)
            {
                static_assert(conjunction_v<is_move_constructible<Types> ...> &&
                              conjunction_v<is_move_assignable   <Types> ...> ,
                R"(This function shall not participate in overload resolution unless is_move_constructible_- v<Ti> &&
                                                                                     is_move_assignable_v<Ti> is true for all i.)");
                if(!valueless_by_exception())
                    __destroy();
                if(!rhs.valueless_by_exception())
                    swap(rhs);
                return *this;
            };

            template <class T> __variant_base& operator=(T&& rhs) noexcept(conjunction_v<is_nothrow_assignable<Types&,Types>    ...>  &&
                                                                           conjunction_v<is_nothrow_constructible<Types&,Types> ...>)
            {
                static_assert(is_same_v<decay_t<T>, variant> && is_assignable_v<T&, T> && is_constructible_v<T, T>,
                R"(This function shall not participate in overload resolution unless is_same_v<decay_t<T>, variant> is false,
                                                                              unless is_assignable_v<Tj&, T> && is_constructible_v<Tj, T> is true)");
                if(!valueless_by_exception())
                    __destroy();
                __construct<T>(forward<T>(rhs));
                return *this;
            };

            // 20.7.2.4, modifiers

            template <class T, class... Args>
            void emplace(Args&&... args)
            {
                if(!valueless_by_exception())
                    __destroy();
                __construct<T>(forward<Args>(args)...);
            };

            template <class T, class U, class... Args>
            void emplace(initializer_list<U> il, Args&&... args)
            {
                if(!valueless_by_exception())
                    __destroy();
                __construct<T>(il, forward<Args>(args)...);
            };

            template <size_t I, class... Args>
            void emplace(Args&&... args)
            {
                using T = variant_alternative_t<I,variant<Types...>>;
                emplace<T>(forward<Args>(args)...);
            };

            template <size_t I, class U, class... Args>
            void emplace(initializer_list<U> il, Args&&... args)
            {
                using T = variant_alternative_t<I,variant<Types...>>;
                emplace<T>(il, forward<Args>(args)...);
            };

            // 20.7.2.5, value status
            constexpr bool valueless_by_exception() const noexcept
            {
                return m_index == variant_npos;
            }

            constexpr size_t index() const noexcept
            {
                return m_index;
            };

            template <class T, class... Args>
            void constexpr __construct(Args&&... args)
            {
                assert(valueless_by_exception());
                new(&m_storage) remove_reference_t<T>{forward<Args>(args) ...};
                m_index = __helper::__index_v<T, Types...>;
            };

            template <class T,
                      class Alloc,
                      class... Args,
                      enable_if_t<!uses_allocator_v<decay_t<T>, Alloc>,bool> = true
                      >
            void __construct(allocator_arg_t, const Alloc& a, Args&&... args)
            {
                __construct<T>(forward<Args>(args)...);
            };

            template <class T,
                      class Alloc,
                      class... Args,
                      enable_if_t<is_constructible_v<T, allocator_arg_t, Alloc, Args...>,bool> = true
                      >
            void __construct(allocator_arg_t, const Alloc& a, Args&&... args)
            {
                assert(valueless_by_exception());
                new(&m_storage) remove_reference_t<T>{allocator_arg_t{}, a, forward<Args>(args)...};
                m_index = __helper::__index_v<T, Types...>;
            };

            template <class T,
                      class Alloc,
                      class... Args,
                      enable_if_t<is_constructible_v<T, Args..., Alloc>,bool> = true
                      >
            void __construct(allocator_arg_t, const Alloc& a, Args&&... args)
            {
                assert(valueless_by_exception());
                new(&m_storage) remove_reference_t<T>{forward<Args>(args)..., a};
                m_index = __helper::__index_v<T, Types...>;
            };

            // template <class T, class U, class... Args>
            // void __construct(initializer_list<U> il, Args&&... args)
            // {
            //     assert(valueless_by_exception());
            //     new(&m_storage) remove_reference_t<T>{il, forward<Args>(args)...};
            //     m_index = __helper::__index_v<T, Types...>;
            // };
            //
            // template <class T, class Alloc, class U, class... Args>
            // void __construct(allocator_arg_t, const Alloc& a, initializer_list<U> il, Args&&... args)
            // {
            //     assert(valueless_by_exception());
            //     new(&m_storage) remove_reference_t<T>{allocator_arg_t{}, a, forward(il), forward<Args>(args) ...};
            //     m_index = __helper::__index_v<T, Types...>;
            // };

            template <class T>
            void __destroy()
            {
                assert(!valueless_by_exception());
                if(is_class_v<T>)
                    reinterpret_cast<add_pointer_t<T>>(&m_storage)->~T();
                m_index = variant_npos;
            }

        private:

            void __construct(const __variant_base& v)
            {
                constexpr auto construct = __make_construct_array<Types...>(index_sequence_for<Types...>{});
                construct[v.index()](*this, v);
            }

            template <class Alloc>
            void __construct(allocator_arg_t, const Alloc& a, const __variant_base& v)
            {
                constexpr auto __array = __make_allocator_construct_array<Alloc, Types...>(index_sequence_for<Types...>{});
                __array[v.index()](allocator_arg_t{}, a, *this, v);
            }

        protected:

            void __destroy()
            {
                constexpr auto __array = __make_destroy_array<Types...>(index_sequence_for<Types...>{});
                __array[m_index](*this);
            }

            size_t m_index = variant_npos;
        };

        template <class... Types>
        struct __trivially_destructible : public __variant_base<Types...> {
            using __base = __helper::__variant_base<Types...>;
            using __base::__base;
            ~__trivially_destructible() = default;
        };

        template <class... Types>
        struct __non_trivially_destructible : public __variant_base<Types...> {
            using __base = __helper::__variant_base<Types...>;
            using __base::__base;
            ~__non_trivially_destructible()
            {
                __base::__destroy();
            }
        };

    } // namespace __helper

    template <class... Types>
    class variant : public conditional_t<conjunction_v<is_trivially_destructible<Types>...>,
                                        __helper::__trivially_destructible<Types...>,
                                        __helper::__non_trivially_destructible<Types...>>
    {
        using __base = conditional_t<conjunction_v<is_trivially_destructible<Types>...>,
                                  __helper::__trivially_destructible<Types...>,
                                  __helper::__non_trivially_destructible<Types...>>;
        public:

        using __base::__base;

        // 20.7.2.6, swap
        void swap(variant& rhs) noexcept(conjunction_v<is_nothrow_move_constructible<Types> ...> &&
                                         conjunction_v<is_nothrow_swappable<Types> ...>)
        {
            constexpr auto __swap = __helper::__make_swap_array<Types...>(index_sequence_for<Types...>{});
            variant& lhs = *this;
            if(lhs.valueless_by_exception() && rhs.valueless_by_exception())
                return;
            if(lhs.m_index == rhs.m_index)
                __swap[lhs.m_index](lhs,rhs);
            else
            {
                variant tmp{rhs};
                rhs.m_index = lhs.m_index;
                __swap[lhs.m_index](rhs,lhs);
                lhs.m_index = tmp.m_index;
                __swap[lhs.m_index](lhs,tmp);
            }
        }
    };

} // namespace std
