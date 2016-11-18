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

        constexpr __storage() : m_type{}
        {}

        template<class... Args>
        constexpr __storage(in_place_type_t<Type>, Args&&... args) : m_type{forward<Args>(args)...}
        {}
    };

    template <class Type>
    union __storage<Type,__non_trivial>
    {
        aligned_storage_t<sizeof(Type), alignof(Type)> m_storage;

        constexpr __storage() : m_storage{}
        {}

        template<class... Args>
        __storage(in_place_type_t<Type>, Args&&... args) : m_storage{}
        {
            new(&m_storage) Type(forward<Args>(args)...);
        }
    };

    template <>
    union __storage<void,__trivial>
    {
        constexpr __storage()
        {}

        template <class T>
        constexpr __storage(in_place_type_t<T>)
        {}
    };

    template <class Type>
    union __storage<Type>
    {
        static constexpr bool trivial = is_trivially_destructible_v<Type> || is_same_v<Type,void>;

        __storage<Type, conditional_t<trivial, __trivial, __non_trivial>> m_head;

        constexpr __storage() : m_head{}
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
        static constexpr bool trivial = is_trivially_destructible_v<Type> || is_same_v<Type,void>;

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

    template <class T, class... Types>
    constexpr size_t __index_v = __index<T, Types...>::value;

    template <class T>
    struct __index<T> : integral_constant<size_t, 1> {};

    template <class T, class... Types>
    struct __index<T, T, Types...> : integral_constant<size_t, 0> {};

    template <class T, class U, class... Types>
    struct __index<T, U, Types...> : integral_constant<size_t, 1 + __index_v<T, Types...>> {};

    template <bool CopyConstructible,
              bool MoveConstructible,
              bool CopyAssignable,
              bool MoveAssignable,
              bool TiviallyDestructible,
              class... Types>
    class __variant_storage; // undefined

    template <class... Types>
    class __variant_storage<false,false,false,false,false,Types...>
    {
    public:

        __storage<Types...> m_storage;

    protected:

        ptrdiff_t m_index = -1;

        __variant_storage(const __variant_storage&) = delete;
        __variant_storage(__variant_storage&&) = delete;
        __variant_storage& operator=(const __variant_storage&) = delete;
        __variant_storage& operator=(__variant_storage&&) = delete;

        __variant_storage() :
            m_storage{},
            m_index{-1}
        {}

        template <class T, class... Args>
        __variant_storage(in_place_type_t<T>, Args&&... args) :
            m_storage{},
            m_index{-1}
        {
            __construct(in_place<T>, forward<Args>(args) ...);
        }

        template <class Alloc, class T, class... Args>
        constexpr __variant_storage(allocator_arg_t, const Alloc& a, in_place_type_t<T>, Args&&... args) :
            m_storage{},
            m_index{-1}
        {
            __construct(in_place<T>, forward<Args>(args) ...);
        }

        ~__variant_storage()
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
        void __copy(const __variant_storage& v)
        {
            assert(v.m_index >= 0 && v.m_index < sizeof...(Types));
            assert(m_index < 0);
            __construct(in_place<T>,v.m_storage.template get<T>());
            m_index = v.m_index;
        }

        void __copy(const __variant_storage& v)
        {
            assert(v.m_index >= 0 && v.m_index < sizeof...(Types));
            assert(m_index < 0);
            using F = void(__variant_storage::*)(const __variant_storage&);
            constexpr F __array[sizeof...(Types)] = {&__variant_storage::__copy<Types> ...};
            (this->*__array[v.m_index])(v);
        }

        template<class T>
        void __move(__variant_storage&& v)
        {
            assert(v.m_index >= 0 && v.m_index < sizeof...(Types));
            assert(m_index < 0);
            __construct(in_place<T>,v.m_storage.template get<T>());
            m_index = v.m_index;
        }

        void __move(__variant_storage&& v)
        {
            assert(v.m_index >= 0 && v.m_index < sizeof...(Types));
            assert(m_index < 0);
            using F = void(__variant_storage::*)(__variant_storage&&);
            constexpr F __array[sizeof...(Types)] = {&__variant_storage::__move<Types> ...};
            (this->*__array[v.m_index])(move(v));
        }

        template<class T, enable_if_t<is_class_v<decay_t<T>>,bool> = true>
        void __destroy()
        {
            assert(m_index >= 0);
            reinterpret_cast<add_pointer_t<T>>(&m_storage)->~T();
            m_index = -1;
        };

        template<class T, enable_if_t<!is_class_v<decay_t<T>>,bool> = true>
        void __destroy()
        {
            assert(m_index >= 0);
            m_index = -1;
        };

        void __destroy()
        {
            assert(m_index >= 0 && m_index < sizeof...(Types));
            using F = void(__variant_storage::*)();
            constexpr F __array[sizeof...(Types)] = {&__variant_storage::__destroy<Types> ...};
            (this->*__array[m_index])();
        };
    };

    template <class... Types>
    class __variant_storage<false,false,false,false,true,Types...>
    {
    public:

        __storage<Types...> m_storage;

    protected:

        ptrdiff_t m_index = -1;

        __variant_storage(const __variant_storage&) = delete;
        __variant_storage(__variant_storage&&) = delete;
        __variant_storage& operator=(const __variant_storage&) = delete;
        __variant_storage& operator=(__variant_storage&&) = delete;
        ~__variant_storage() = default;

        __variant_storage() :
            m_storage{},
            m_index{-1}
        {}

        template <class T, class... Args>
        constexpr __variant_storage(in_place_type_t<T>, Args&&... args) :
            m_storage{in_place<T>, forward<Args>(args) ...},
            m_index{__index_v<T, Types...>}
        {}

        template <class Alloc, class T, class... Args>
        constexpr __variant_storage(allocator_arg_t, const Alloc& a, in_place_type_t<T>, Args&&... args) :
            m_storage{in_place<T>, forward<Args>(args) ...},
            m_index{__index_v<T, Types...>}
        {}

        template <class T, class... Args>
        void __construct(in_place_type_t<T>, Args&&... args)
        {
            assert(m_index < 0);
            new(&m_storage) remove_reference_t<T>{forward<Args>(args) ...};
            m_index = __index_v<T, Types...>;
        };

        template<class T>
        void __copy(const __variant_storage& v)
        {
            assert(v.m_index >= 0 && v.m_index < sizeof...(Types));
            assert(m_index < 0);
            __construct(in_place<T>,v.m_storage.template get<T>());
            m_index = v.m_index;
        }

        void __copy(const __variant_storage& v)
        {
            assert(v.m_index >= 0 && v.m_index < sizeof...(Types));
            assert(m_index < 0);
            using F = void(__variant_storage::*)(const __variant_storage&);
            constexpr F __array[sizeof...(Types)] = {&__variant_storage::__copy<Types> ...};
            (this->*__array[v.m_index])(v);
        }

        void __move(__variant_storage&& v)
        {
            __copy(v);
        }

        void __destroy()
        {
            assert(m_index >= 0);
            m_index = -1;
        };
    };

    template <bool TiviallyDestructible,
              class... Types>
    class __variant_storage<false,false,false,true,TiviallyDestructible,Types...> :
    public __variant_storage<false,false,false,false,TiviallyDestructible,Types...>
    {
        using __base = __variant_storage<false,false,false,false,TiviallyDestructible,Types...>;
        using __base::__base;

    protected:

        using __base::__move;
        using __base::__destroy;
        using __base::m_index;

        __variant_storage& operator=(__variant_storage&& rhs) noexcept(conjunction_v<is_nothrow_move_constructible<Types> ...> &&
                                                             conjunction_v<is_nothrow_move_assignable<Types> ...>)
        {
            static_assert(conjunction_v<is_move_constructible<Types> ...> &&
                          conjunction_v<is_move_assignable   <Types> ...> ,
            R"(This function shall not participate in overload resolution unless is_move_constructible_- v<Ti> &&
                                                                                 is_move_assignable_v<Ti> is true for all i.)");
            if(!(m_index < 0))
                __destroy();
            if(!(rhs.m_index < 0))
                __move(move(rhs));
            return *this;
        }
    };

    template <bool MoveAssignable,
              bool TiviallyDestructible,
              class... Types>
    class __variant_storage<false,false,true,MoveAssignable,TiviallyDestructible,Types...> :
    public __variant_storage<false,false,false,MoveAssignable,TiviallyDestructible,Types...>
    {
        using __base = __variant_storage<false,false,false,MoveAssignable,TiviallyDestructible,Types...>;
        using __base::__base;

    protected:

        using __base::__copy;
        using __base::__destroy;
        using __base::m_index;

        __variant_storage& operator=(const __variant_storage& rhs)
        {
            static_assert(conjunction_v<is_copy_constructible<Types> ...> &&
                          conjunction_v<is_move_constructible<Types> ...> &&
                          conjunction_v<is_copy_assignable   <Types> ...> ,
            R"(This function shall not participate in overload resolution unless is_copy_constructible_v<Ti> &&
                                                                                 is_move_constructible_v<Ti> &&
                                                                                 is_copy_assignable_v<Ti> is true for all i.)");
            if(!(m_index < 0))
                __destroy();
            if(!(rhs.m_index < 0))
                __copy(rhs);
            return *this;
        }

        __variant_storage& operator=(__variant_storage&&) = default;
    };

    template <bool CopyAssignable,
              bool MoveAssignable,
              bool TiviallyDestructible,
              class... Types>
    class __variant_storage<false,true,CopyAssignable,MoveAssignable,TiviallyDestructible,Types...> :
    public __variant_storage<false,false,CopyAssignable,MoveAssignable,TiviallyDestructible,Types...>
    {
        using __base = __variant_storage<false,false,CopyAssignable,MoveAssignable,TiviallyDestructible,Types...>;
        using __base::__base;

    protected:

        using __base::__move;

        __variant_storage(__variant_storage&& v) :
            __base{}
        {
            static_assert(conjunction_v<is_copy_constructible<Types> ...>,
                "This function shall not participate in overload resolution unless is_copy_constructible_v<Ti> is true for all i.");
            __move(v);
            v.m_index = -1;
        }

        __variant_storage& operator=(const __variant_storage&) = default;
        __variant_storage& operator=(__variant_storage&&) = default;
    };

    template <bool MoveConstructible,
              bool CopyAssignable,
              bool MoveAssignable,
              bool TiviallyDestructible,
              class... Types>
    class __variant_storage<true,MoveConstructible,CopyAssignable,MoveAssignable,TiviallyDestructible,Types...> :
    public __variant_storage<false,MoveConstructible,CopyAssignable,MoveAssignable,TiviallyDestructible,Types...>
    {
        using __base = __variant_storage<false,MoveConstructible,CopyAssignable,MoveAssignable,TiviallyDestructible,Types...>;
        using __base::__base;

    protected:

        using __base::__copy;

        __variant_storage(const __variant_storage& v) :
            __base{}
        {
            static_assert(conjunction_v<is_move_constructible<Types> ...>,
                "This function shall not participate in overload resolution unless is_move_constructible_v<Ti> is true for all i.");
            __copy(v);
        }

        __variant_storage& operator=(const __variant_storage&) = default;
        __variant_storage& operator=(__variant_storage&&) = default;
    };

} // namespace std::__helper
