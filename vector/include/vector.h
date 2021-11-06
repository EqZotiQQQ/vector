#pragma once

#include <iostream>
#include <algorithm>

#define i32 int
#define i64 long long
#define u32 unsigned
#define u64 unsigned long long
#define f32 float

#define dbg std::cout << __PRETTY_FUNCTION__ << '\n';
namespace no_std {

    template<class T, typename Alloc = std::allocator<T>>
    class Vector {
    public:
    protected:
        T* data_ptr {nullptr};
        std::size_t size {0};
        std::size_t capacity {0};

        Alloc allocator;

        using AllocTraits = std::allocator_traits<Alloc>;

        static constexpr f32 capacity_multiplier {2.0}; // 2.0 std, fibonaccy for apple
        static constexpr std::size_t default_capacity {4};

    public:
        constexpr Vector(const T& new_entry = T(), const Alloc& alloc = Alloc()):
                allocator(alloc) {
            reserve(default_capacity);
        }

        constexpr explicit Vector(std::size_t new_size,
                        const T& new_entry = T(),
                        const Alloc& alloc = Alloc()):
                allocator(alloc)
        {
//            default_capacity > capacity && reserve(default_capacity) || reserve((new_size / 256) * 512);// make reserve ret ->bool

            if (default_capacity >= new_size) {
                reserve(default_capacity);
            } else {
                reserve(new_size);
            }

            size = new_size;

            for (std::size_t i = 0; i < size; i++) {
                AllocTraits::construct(allocator, data_ptr + i, new_entry);
            }
        }

        constexpr Vector(Vector<T>&& old) noexcept {
            if (&old == data_ptr) {
                return;
            }
            AllocTraits::deallocate(allocator, data_ptr, size);
            std::swap(data_ptr, old.data_ptr);
            std::swap(size, old.size);
            std::swap(capacity, old.capacity);
        }

        constexpr Vector(const Vector<T>& old) {
            if (&old == this) {
                return;
            }
            AllocTraits::deallocate(allocator, data_ptr, size);
            data_ptr = allocator.allocate(old.capacity);
            size = old.size;
            capacity = old.capacity;
            std::copy(old.data_ptr, old.data_ptr + old.size, data_ptr);
        }

        constexpr Vector<T>& operator=(Vector<T>&& old) noexcept {
            if (&old == this) {
                return *this;
            }
            AllocTraits::deallocate(allocator, data_ptr, size);

            data_ptr = nullptr;
            capacity = 0;
            size = 0;

            std::exchange(data_ptr, old.data_ptr);
            std::exchange(size, old.size);
            std::exchange(capacity, old.capacity);
            return *this;
        }

        constexpr Vector<T>& operator=(const Vector<T>& old) {
            if (&old == this) {
                return *this;
            }
            AllocTraits::deallocate(allocator, data_ptr, size);

            data_ptr = new T[old.size];
            size = old.size;
            for (unsigned i = 0; i < size; i++) {
                data_ptr[i] = old.data_ptr[i];
            }
            return *this;
        }

        ~Vector() {
            for (std::size_t i = 0; i < size; i++) {
                AllocTraits::destroy(allocator, data_ptr + i);
            }
            AllocTraits::deallocate(allocator, data_ptr, size);
        }

        constexpr T* operator&() {
            return data_ptr;
        }

        constexpr T& operator*() {
            return *data_ptr;
        }

        template <class... Args>
        constexpr void push_back(Args... a) {
            (push_back_impl(a), ...);
        }

        template <class... Args>
        constexpr void emplace_back(Args&&... args) {
            if (size == capacity) {
                reserve(capacity * capacity_multiplier);
            }
            try {
                allocator.construct(std::forward<Args>(args)...);
            } catch (...) {
                for (std::size_t j = 0; j < size; j++) {
                    AllocTraits::destroy(allocator, data_ptr + size);
                }
                allocator.deallocate(data_ptr);
                throw;
            }
            size += 1;
        }

        constexpr void reserve(std::size_t new_capacity) {
            if (capacity >= new_capacity) {
                return;
            }
            T* new_storage = AllocTraits::allocate(allocator, new_capacity);

            try {
                std::uninitialized_move(data_ptr, data_ptr + size, new_storage);
            } catch (...) {
                AllocTraits::deallocate(allocator, new_storage, new_capacity);
                throw;
            }

            for (std::size_t i = 0; i < size; i++) {
                AllocTraits::destroy(allocator, data_ptr + i);
            }

            AllocTraits::deallocate(allocator, data_ptr, size);
            data_ptr = new_storage;
            capacity = new_capacity;
        }

        constexpr void resize(std::size_t new_size, const T& value = T()) {
            if (new_size > size) {
                reserve(new_size);
                std::size_t i = 0;
                try {
                    for (; i < size; i++) {
                        AllocTraits::construct(allocator, data_ptr, data_ptr[i]);
//                        new(data_ptr + i) T(data_ptr[i]);
                    }
                } catch (...) {
                    for (std::size_t j = 0; j < i; j++) {
                        AllocTraits::destroy(allocator, data_ptr + i);
//                        (data_ptr + i)->~T();
                    }
                    AllocTraits::deallocate(allocator, data_ptr, size);
                    throw;
                }
            } else if (new_size < size) {
                size -= new_size;
                for (std::size_t i = new_size; i < size; i++) {
                    AllocTraits::destroy(allocator, data_ptr + i);
                }
            }
        }

        constexpr T& operator[](std::size_t pos) noexcept {
            return data_ptr[pos];
        }

        constexpr const T& operator[](std::size_t pos) const noexcept {
            return data_ptr[pos];
        }

        constexpr T& at(std::size_t pos) {
            if (pos >= size) {
                throw std::out_of_range("");
            }
            return data_ptr[pos];
        }

        std::size_t len() const noexcept {
            return size;
        }

        std::size_t cap() const noexcept {
            return capacity;
        }

        constexpr void pop_back() noexcept {
            (data_ptr + size)->~T();
            size -= 1;
        }

        //void srink_to_fit();

        constexpr friend std::ostream& operator<<(std::ostream& os, no_std::Vector<T> v) noexcept {
            os << "addr = " << &v;
            os << "\tsize = " << v.size;
            os << "\tcapacity = " << v.capacity;
            os << "\tdata = [ ";
            for (std::size_t i = 0; i < v.size; i++) {
                os << v.data_ptr[i] << " ";
            }
            os << "]\n";
            return os;
        }

    protected:
        void push_back_impl(const T& new_entry) {
            if (size == capacity) {
                reserve(capacity * capacity_multiplier);
            }
            try {
                new(data_ptr + size) T(new_entry);
            } catch (...) {
                for (std::size_t j = 0; j < size; j++) {
                    (data_ptr + size)->~T();
                }
                delete[] reinterpret_cast<char*>(data_ptr);
                throw;
            }
            size += 1;
        }

    };
}