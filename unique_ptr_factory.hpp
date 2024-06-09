/******************************************************************************
    
    Copyright 2024 Kelvin Wu

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

*******************************************************************************/

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





