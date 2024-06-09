/*
 * Created on Sun Jun 09 2024
 *
 * Copyright (c) 2024 Kelvin Wu
 */

#pragma once

#include <memory>

namespace unique_ptr_factory {
template<typename T, typename Alloc>
class UniquePtrFactory {
    private:
    struct Deleter {
        constexpr void operator()(T* ptr) {
            alloc_.delete_object(ptr);
        }

        Alloc& alloc_;
    };

    public:
        using PointerType = std::unique_ptr<T, Deleter>;

        template<class... Args>
        static constexpr PointerType allocate_unique(Alloc& alloc, Args&&... args) noexcept {
            T* ptr = alloc.template new_object<T>(std::forward<Args>(args)...);
            return PointerType{ptr, Deleter{alloc}};
        };
};

template<typename T, typename Alloc>
class UniquePtrFactory<T[], Alloc> {
    private:
    struct Deleter {
        constexpr void operator()(T* ptr) {
            for(std::size_t i = 0; i < n; i++) {
                ptr[i].~T();
            }

            alloc_.deallocate_object(ptr, n);
        }

        Alloc& alloc_;
        const std::size_t n;
    };

    public:
        using PointerType = std::unique_ptr<T[], Deleter>;

        template<class... Args>
        static constexpr PointerType allocate_unique(Alloc& alloc, std::size_t n, Args&& ... args) noexcept {
            T* ptr = alloc.template allocate_object<T>(n);
            for(std::size_t i = 0; i < n; i++) {
               alloc.construct(ptr + i, std::forward<Args>(args)...);
            }

            return PointerType{ptr, Deleter{alloc, n}};
        };
};
}





