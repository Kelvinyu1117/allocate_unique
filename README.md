# allocate_unique
This repository contains a simple factory class for creating `std::unique_ptr` from custom allocator.


## Intention
Since using custom allocator with smart pointers requires custom deleter in order to release the resources properly. For `std::shared_ptr`, that is easy that you can simply use `std::allocate_shared` by passing the allocator reference and the arguments used for object construction.

However, for `std::unique_ptr`, the type of the deleter is embedded into the `std::unique_ptr`, which is `std::unique_ptr<T, D>` where `T` is the data type and `D` is the type of the deleter. 

The STL doesn't provide the corresponding utility functions and it is a bit inconvenient to use it with other containers like `std::vector` as well. 

This repository contains a simple factory class for you to deal this use case easily.


## Example
```cpp
#include "unique_ptr_factory.hpp"
#include <vector>

struct TestObject {
    int a;
};

int main() {
    std::array<std::byte, 1024> buffer; 
    std::pmr::monotonic_buffer_resource mbr{buffer.data(), buffer.size()};
    std::pmr::polymorphic_allocator<TestObject> pa{&mbr};

    using UniquePtrFactory = unique_ptr_factory::UniquePtrFactory<TestObject, std::pmr::polymorphic_allocator<TestObject>>;

    std::vector<UniquePtrFactory::PointerType> test_objects;

    for(int i = 0; i < 10; i++>)
    {
        test_objects.emplace_back(UniquePtrFactory::allocate_unique(pa, i));
    }

    return 0;
}

```

