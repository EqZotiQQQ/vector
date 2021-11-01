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

    template<class T>
    class Vector {
    public:
        Vector():
            data_ptr(new T[4]) {
        }

        Vector(u32 capacity):
            data_ptr(new T[capacity]),
            capacity(capacity) {
        }

        Vector(Vector<T>&& old) {
            data_ptr = old.data_ptr;
            size = old.size;
            capacity = old.capacity;
            old.data_ptr = nullptr;
            old.size = 0;
            old.capacity = 0;
        }

        Vector(const Vector<T>& old) {
            if (&old == this) {
                return;
            }
            if (data_ptr) {
                delete[] data_ptr;
                data_ptr = nullptr;
            }
            data_ptr = new T[old.size];
            size = old.size;
            capacity = old.capacity;
            for (unsigned i = 0; i < size; i++) {
                data_ptr[i] = old.data_ptr[i];
            }
        }

        Vector<T>& operator=(Vector<T>&& old) {
            if (&old == this) {
                return *this;
            }
            if (data_ptr) {
                delete[] data_ptr;
                capacity = 0;
                size = 0;
            }
            std::exchange(data_ptr, old.data_ptr);
            std::exchange(size, old.size);
            std::exchange(capacity, old.capacity);
            return *this;
        }

        Vector<T>& operator=(const Vector<T>& old) {
            if (&old == this) {
                return *this;
            }
            if (data_ptr) {
                delete[] data_ptr;
                data_ptr = nullptr;
            }
            data_ptr = new T[old.size];
            size = old.size;
            for (unsigned i = 0; i < size; i++) {
                data_ptr[i] = old.data_ptr[i];
            }
            return *this;
        }

        ~Vector() {
            delete[] data_ptr;
        }

        T* operator&() {
            return data_ptr;
        }

        T& operator*() {
            return *data_ptr;
        }

        template <class... Args>
        void push_back(Args... a) {
            (push_back_impl(a), ...);
        }

        template <class... Args>
        void push_front(Args... args) {
            u32 shift = sizeof ... (args);
            if (size + shift > capacity) {
                reserve(capacity + shift + size);
            }
            (push_back_impl(args),...);
            ring_shift(shift);
        }

        void ring_shift(u32 shift) {
            for (int i = size-1; i >= 0; i--) {
                data_ptr[i + shift] = data_ptr[i];
            }
            for (int i = 0; i < shift; i++) {
                data_ptr[i] = data_ptr[i + size];
            }
        }

        void push_front_impl(const T& new_entry) {
            std::cout << new_entry << "\n";
        }

        void shift_right(u32 shift) {
            for (int i = size - 1; i >= 0; i--) {
                std::exchange(data_ptr[i + shift], 0);
            }
        }

        //todo: if move noexcept then move othervice copy and clear old storage
        // or i can create subarray and copy it in front of data_ptr...
        constexpr void reserve(u32 new_capacity) noexcept {
            if (capacity >= new_capacity) {
                return;
            }
            capacity = new_capacity;
            T *new_storage = new T[capacity];
            for (int i = 0; i < size; i++) {
                new_storage[i] = data_ptr[i];
            }
            delete[] data_ptr;
            data_ptr = new_storage;
        }

        // todo: prob need to use noexcept for []noexcept when << and [] with exceptions for deref operation.
        friend std::ostream& operator<<(std::ostream& os, no_std::Vector<T> v) noexcept {
            os << "addr = " << &v;
            os << "\tsize = " << v.size;
            os << "\tcapacity = " << v.capacity;
            os << "\tdata = [ ";
            for (int i = 0; i < v.size; i++) {
                os << v.data_ptr[i] << " ";
            }
            os << "]\n";
            return os;
        }

        // todo: prob need to throw exception to avoid data access after ring shifting
        T& operator[](u32 pos) noexcept {
            return data_ptr[pos];
        }

        u32 len() const noexcept {
            return size;
        }

        u32 cap() const noexcept {
            return capacity;
        }

        void pop_back() {

        }

        void pop_front() {
            
        }

    protected:
        void push_back_impl(const T& new_entry) {
            if (size == capacity) {
                capacity *= capacity_multiplier;
                T* new_storage = new T[capacity];
                for (int i = 0; i < size; i++) {
                    new_storage[i] = data_ptr[i];
                }
                delete[] data_ptr;
                data_ptr = new_storage;
            }
            data_ptr[size] = new_entry;
            size += 1;
        }

    protected:
        T* data_ptr {nullptr};
        u32 size {0};
        u32 capacity {4};

        static constexpr f32 capacity_multiplier {2.0}; // 2.0 std, fibonaccy for apple
    };
}