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

    __storage(): m_storage{}
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

} // namespace std::__helper
