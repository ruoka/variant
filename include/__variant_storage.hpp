#pragma once

namespace std::__helper {

template <class Type, class... Types>
union __storage; // undefined

struct __trivial{};

struct __non_trivial{};

template <class Type>
union __storage<Type,__trivial>
{
    Type m_type;

    constexpr __storage(): m_type{}
    {}

    template<class... Args>
    constexpr __storage(in_place_type_t<Type>, Args&&... args) : m_type{forward<Args>(args)...}
    {}
};

template <class Type>
union __storage<Type,__non_trivial>
{
    aligned_storage_t<sizeof(Type), alignof(Type)> m_storage;

    constexpr __storage(): m_storage{}
    {}

    template<class... Args>
    __storage(in_place_type_t<Type>, Args&&... args) : m_storage{}
    {
        new(&m_storage) Type(forward<Args>(args)...);
    }
};

template <class Type>
union __storage<Type>
{
    static constexpr bool trivial = is_trivially_destructible_v<Type>;

    __storage<Type, conditional_t<trivial, __trivial, __non_trivial>> m_head;

    constexpr __storage(): m_head{}
    {}

    template <class T,
              class... Args,
              enable_if_t<is_same_v<T,Type>, bool> = true
              >
    constexpr __storage(in_place_type_t<T>, Args&&... args) : m_head{in_place<T>, forward<Args>(args)...}
    {}

    template <class T, enable_if_t<is_same_v<T,Type> && trivial, bool> = true>
    constexpr T& get()
    {
        return m_head.m_type;
    }

    template <class T, enable_if_t<is_same_v<T,Type> && trivial, bool> = true>
    constexpr const T& get() const
    {
        return m_head.m_type;
    }

    template <class T, enable_if_t<is_same_v<T,Type> && !trivial, bool> = true>
    constexpr T& get()
    {
        return *reinterpret_cast<T*>(&m_head.m_storage);
    }

    template <class T, enable_if_t<is_same_v<T,Type> && !trivial, bool> = true>
    constexpr const T& get() const
    {
        return *reinterpret_cast<const T*>(&m_head.m_storage);
    }
};

template <class Type, class... Types>
union __storage
{
    static constexpr bool trivial = is_trivially_destructible_v<Type>;

    __storage<Type, conditional_t<trivial, __trivial, __non_trivial>> m_head;

    __storage<Types...> m_tail;

    __storage()
    {}

    template <class T,
              class... Args,
              enable_if_t<is_same_v<T,Type>, bool> = true
              >
    constexpr __storage(in_place_type_t<T>, Args&&... args) : m_head{in_place<T>, forward<Args>(args)...}
    {}

    template <class T,
              class... Args,
              enable_if_t<!is_same_v<T,Type>, bool> = true
              >
    constexpr __storage(in_place_type_t<T>, Args&&... args) : m_tail{in_place<T>, forward<Args>(args)...}
    {}

    template <class T, enable_if_t<is_same_v<T,Type> && trivial, bool> = true>
    constexpr T& get()
    {
        return m_head.m_type;
    }

    template <class T, enable_if_t<is_same_v<T,Type> && trivial, bool> = true>
    constexpr const T& get() const
    {
        return m_head.m_type;
    }

    template <class T, enable_if_t<is_same_v<T,Type> && !trivial, bool> = true>
    constexpr T& get()
    {
        return *reinterpret_cast<T*>(&m_head.m_storage);
    }

    template <class T, enable_if_t<is_same_v<T,Type> && !trivial, bool> = true>
    constexpr const T& get() const
    {
        return *reinterpret_cast<const T*>(&m_head.m_storage);
    }

    template <class T, enable_if_t<!is_same_v<T,Type>, bool> = true>
    constexpr T& get()
    {
        return m_tail.template get<T>();
    }

    template <class T, enable_if_t<!is_same_v<T,Type>, bool> = true>
    constexpr const T& get() const
    {
        return m_tail.template get<T>();
    }
};

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

template <class... Types>
struct __trivially_destructible {

    __storage<Types...> m_storage;

    ptrdiff_t m_index = -1;

    __trivially_destructible() :
        m_storage{},
        m_index{-1}
    {}

    __trivially_destructible(const __trivially_destructible& v) :
        m_storage{},
        m_index{-1}
    {
        __copy(v);
    }

    __trivially_destructible(__trivially_destructible&& v) :
        m_storage{},
        m_index{-1}
    {
        __move(v);
        v.m_index = -1;
    }

    template <class T, class... Args>
    constexpr __trivially_destructible(in_place_type_t<T>, Args&&... args) :
        m_storage{in_place<T>, forward<Args>(args) ...},
        m_index{__index_v<T, Types...>}
    {}

    template <class Alloc, class T, class... Args>
    constexpr __trivially_destructible(allocator_arg_t, const Alloc& a, in_place_type_t<T>, Args&&... args) :
        m_storage{in_place<T>, forward<Args>(args) ...},
        m_index{__index_v<T, Types...>}
    {}

    ~__trivially_destructible() = default;

    template <class T, class... Args>
    void __construct(in_place_type_t<T>, Args&&... args)
    {
        assert(m_index < 0);
        new(&m_storage) remove_reference_t<T>{forward<Args>(args) ...};
        m_index = __index_v<T, Types...>;
    };

    template<class T>
    void __copy(const __trivially_destructible& v)
    {
        assert(v.m_index >= 0 && v.m_index < sizeof...(Types));
        assert(m_index < 0);
        __construct(in_place<T>,v.m_storage.template get<T>());
        m_index = v.m_index;
    }

    void __copy(const __trivially_destructible& v)
    {
        assert(v.m_index >= 0 && v.m_index < sizeof...(Types));
        assert(m_index < 0);
        using F = void(__trivially_destructible::*)(const __trivially_destructible&);
        constexpr F __array[sizeof...(Types)] = {&__trivially_destructible::__copy<Types> ...};
        (this->*__array[v.m_index])(v);
    }

    void __move(__trivially_destructible&& v)
    {
        __copy(v);
    }

    void __destroy()
    {
        assert(m_index >= 0);
        m_index = -1;
    };
};

template <class... Types>
struct __not_trivially_destructible {

    __storage<Types...> m_storage;

    ptrdiff_t m_index = -1;

    __not_trivially_destructible() :
        m_storage{},
        m_index{-1}
    {}

    __not_trivially_destructible(const __not_trivially_destructible& v) :
        m_storage{},
        m_index{-1}
    {
        __copy(v);
    }

    __not_trivially_destructible(__not_trivially_destructible&& v) :
        m_storage{},
        m_index{-1}
    {
        __movet(v);
        v.m_index = -1;
    }

    template <class T, class... Args>
    __not_trivially_destructible(in_place_type_t<T>, Args&&... args) :
        m_storage{},
        m_index{-1}
    {
        __construct(in_place<T>, forward<Args>(args) ...);
    }

    template <class Alloc, class T, class... Args>
    constexpr __not_trivially_destructible(allocator_arg_t, const Alloc& a, in_place_type_t<T>, Args&&... args) :
        m_storage{},
        m_index{-1}
    {
        __construct(in_place<T>, forward<Args>(args) ...);
    }

    ~__not_trivially_destructible()
    {
        if(m_index >= 0)
            __destroy();
    }

    template <class T, class... Args>
    void __construct(in_place_type_t<T>, Args&&... args)
    {
        assert(m_index < 0);
        new(&m_storage) remove_reference_t<T>{forward<Args>(args) ...};
        m_index = __index_v<T, Types...>;
    };

    template<class T>
    void __copy(const __not_trivially_destructible& v)
    {
        assert(v.m_index >= 0 && v.m_index < sizeof...(Types));
        assert(m_index < 0);
        __construct(in_place<T>,v.m_storage.template get<T>());
        m_index = v.m_index;
    }

    void __copy(const __not_trivially_destructible& v)
    {
        assert(v.m_index >= 0 && v.m_index < sizeof...(Types));
        assert(m_index < 0);
        using F = void(__not_trivially_destructible::*)(const __not_trivially_destructible&);
        constexpr F __array[sizeof...(Types)] = {&__not_trivially_destructible::__copy<Types> ...};
        (this->*__array[v.m_index])(v);
    }

    template<class T>
    void __move(__not_trivially_destructible&& v)
    {
        assert(v.m_index >= 0 && v.m_index < sizeof...(Types));
        assert(m_index < 0);
        __construct(in_place<T>,v.m_storage.template get<T>());
        m_index = v.m_index;
    }

    void __move(__not_trivially_destructible&& v)
    {
        assert(v.m_index >= 0 && v.m_index < sizeof...(Types));
        assert(m_index < 0);
        using F = void(__not_trivially_destructible::*)(__not_trivially_destructible&&);
        constexpr F __array[sizeof...(Types)] = {&__not_trivially_destructible::__move<Types> ...};
        (this->*__array[v.m_index])(move(v));
    }

    template<class T>
    void __destroy()
    {
        assert(m_index >= 0);
        if(is_class_v<decay_t<T>>)
            reinterpret_cast<add_pointer_t<T>>(&m_storage)->~T();
        m_index = -1;
    };

    void __destroy()
    {
        assert(m_index >= 0 && m_index < sizeof...(Types));
        using F = void(__not_trivially_destructible::*)();
        constexpr F __array[sizeof...(Types)] = {&__not_trivially_destructible::__destroy<Types> ...};
        (this->*__array[m_index])();
    };
};

} // namespace std::__helper
