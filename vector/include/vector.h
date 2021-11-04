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
        Vector(const T& new_entry = T(), const Alloc& alloc = Alloc()):
                    capacity(default_capacity),
                    allocator(alloc) {
//            reserve(capacity);
            data_ptr = allocator.allocate(capacity);
            for (int i = 0; i < capacity; i++) {
                new(data_ptr + i) T(new_entry);
            }
        }

        Vector(std::size_t count,
               const T& value,
               const Alloc& alloc = Alloc()): allocator(alloc) {
            reserve(count);
            for (std::size_t i = 0; i < count; i++) {
                alloc.construct(data_ptr + i, value);
            }
        }

        explicit Vector(u32 capacity,
                        const T& new_entry = T(),
                        const Alloc& alloc = Alloc()):
            capacity(capacity),
            allocator(alloc)
        {
            data_ptr = allocator.allocate(capacity);
            for (int i = 0; i < capacity; i++) {
                new(data_ptr + i) T(new_entry);
            }
        }

        Vector(Vector<T>&& old)  noexcept {
            if (&old == data_ptr) {
                return;
            }
            delete[] data_ptr;
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
            delete[] data_ptr;
            data_ptr = new T[old.size];
            size = old.size;
            capacity = old.capacity;
            for (unsigned i = 0; i < size; i++) {
                data_ptr[i] = old.data_ptr[i];
            }
        }
        Vector<T>& operator=(Vector<T>&& old)  noexcept {
            if (&old == this) {
                return *this;
            }
            delete[] data_ptr;
            data_ptr = nullptr;
            capacity = 0;
            size = 0;

            std::exchange(data_ptr, old.data_ptr);
            std::exchange(size, old.size);
            std::exchange(capacity, old.capacity);
            return *this;
        }
        Vector<T>& operator=(const Vector<T>& old) {
            if (&old == this) {
                return *this;
            }
            delete[] data_ptr;

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

        void ring_shift(std::size_t shift) {
            for (int i = size-1; i >= 0; i--) {
                data_ptr[i + shift] = data_ptr[i];
            }
            for (int i = 0; i < shift; i++) {
                data_ptr[i] = data_ptr[i + size];
            }
        }

        //todo: if move noexcept then move othervice copy and clear old storage
        // or i can create subarray and copy it in front of data_ptr...
        // std::addressof(*iter) вызывается в uni copy потому что он может не иметь перегруженного оператора *
        constexpr void reserve(std::size_t new_capacity) noexcept {
            if (capacity >= new_capacity) {
                return;
            }
            std::size_t i = 0;
            T* new_storage = allocator.allocate(new_capacity);

//            T *new_storage = reinterpret_cast<T*>(new char[ new_capacity * sizeof(T)]); // create new array with [n bytes X size of T] //   T *new_storage = new T[capacity];

            try {
                std::uninitialized_copy(data_ptr, data_ptr + size, new_storage);
            } catch (...) {
                allocator.deallocate(new_storage, new_capacity);
//                delete[] reinterpret_cast<char*>(data_ptr);
                throw;
            }
            //try {
            //    for (; i < size; i++) {
            //         По идее плейсмент нью вызывает констр по адресу, не выделяя память.
            //        new(new_storage + i) T(data_ptr[i]); //placement new syntax // Означает, что нужно по адресу [new_storage + i] положить такой объект [data_ptr[i]] // new_storage[i] = data_ptr[i]; Так лучше не делать, т.к. это подразумевает, что объект слева уже существует и будет вызван его деструктор. Это будет UB.
            //    }
            //} catch (...) { // Защищаемся от эксепшена при копировании
            //    for (std::size_t j = 0; j < i; j++) {
            //        (new_storage + i)->~T();
            //    }
            //    delete[] reinterpret_cast<char*>(data_ptr);
            //    throw;
            //}
            for (std::size_t i = 0; i < size; i++) {// delete[] data_ptr;  Тоже UB. Вызывает деструктор объекта, который мог быть не проинициализирован.
//                data_ptr[i].~T();
                allocator.destroy(data_ptr + i);
            }

            allocator.deallocate(data_ptr, size);
//            delete[] reinterpret_cast<char*>(data_ptr); // Надо удалить выделенную память как char, чтобы он не вызывал деструкторы, а просто вызвал очищение.
            data_ptr = new_storage;
            capacity = new_capacity;
        }

        // second param is to create base object. But it works only for objects with default constructor
        //void resize(u32 new_size, const T& value = T()) {
        // this is bad case, cus when we do reserve we construct a lot of objects... there might be a problem
        // Внутри addressof делает reinterpred_cast<T*>(static_cast<char&>(reinterpted_cast<const volatile char&>())
        void resize(u32 new_size, const T& value = T()) {
            if (new_size > size) {
                reserve(new_size);
                std::size_t i = 0;
                try {
                    for (; i < size; i++) {
                        new(data_ptr + i) T(data_ptr[i]); //placement new syntax // Означает, что нужно по адресу [new_storage + i] положить такой объект [data_ptr[i]] // new_storage[i] = data_ptr[i]; Так лучше не делать, т.к. это подразумевает, что объект слева уже существует и будет вызван его деструктор. Это будет UB.
                    }
                } catch (...) { // Защищаемся от эксепшена при копировании
                    for (std::size_t j = 0; j < i; j++) {
                        (data_ptr + i)->~T();
                    }
                    delete[] reinterpret_cast<char*>(data_ptr);
                    throw;
                }
            } else if (new_size < size) {
                size -= new_size;
            }
        }

        // todo: prob need to use noexcept for []noexcept when << and [] with exceptions for deref operation.
        friend std::ostream& operator<<(std::ostream& os, no_std::Vector<T> v) noexcept {
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

        // todo: prob need to throw exception to avoid data access after ring shifting
        T& operator[](u32 pos) noexcept {
            return data_ptr[pos];
        }
        // todo: prob need to throw exception to avoid data access after ring shifting
        const T& operator[](u32 pos) const noexcept {
            return data_ptr[pos];
        }

        T& at(u32 pos) {
            if (pos >= size) {
                throw std::out_of_range("");
            }
            return data_ptr[pos];
        }

        u32 len() const noexcept {
            return size;
        }

        u32 cap() const noexcept {
            return capacity;
        }

        constexpr void pop_back() noexcept {
            (data_ptr + size)->~T();
            size -= 1;
        }

        constexpr void pop_front(const T& value = T()) {
            for (std::size_t i = 0; i < size - 1; i++) {
                data_ptr[i] = data_ptr[i + 1];
            }
            size -= 1;
            data_ptr[size] = value;
        }

        //void srink_to_fit();

    protected:
        // базовая гарантия - если кто-то бросил исключение, то контейнер останется в валидном состоянии
        // строгая - контейнер в неизменном состоянии, если кто-то бросил исключение
        // в деструкторе не надо кидать исключения ни при каких условиях
        // Если в конструкторе копирования вывалось исключение, то мы идём в цикле по всем скопированным объектам
        // и вызываем их деструкторы.
        void push_back_impl(const T& new_entry) {
            if (size == capacity) {
                reserve(capacity * capacity_multiplier);
            }

            std::size_t i = size;
            try {
                new(data_ptr + size) T(new_entry);
                size += 1;
//                for (; i < size; i++) {
//                    new(data_ptr + size) T(new_entry); //placement new syntax // Означает, что нужно по адресу [new_storage + i] положить такой объект [data_ptr[i]] // new_storage[i] = data_ptr[i]; Так лучше не делать, т.к. это подразумевает, что объект слева уже существует и будет вызван его деструктор. Это будет UB.
//                }
            } catch (...) { // Защищаемся от эксепшена при копировании
                size -= 1;
                for (std::size_t j = 0; j < i; j++) {
                    (data_ptr + i)->~T();
                }
                delete[] reinterpret_cast<char*>(data_ptr);
                throw;
            }
        }

    protected:
        T* data_ptr {nullptr};
        std::size_t size {0};
        std::size_t capacity {4};

        Alloc allocator;

        static constexpr f32 capacity_multiplier {2.0}; // 2.0 std, fibonaccy for apple
        static constexpr std::size_t default_capacity {4}; // 2.0 std, fibonaccy for apple
    };

    // Вектор из чаров будет занимать 1 байт на клетку, вектор булей будет занимать 4 элемента на байт
    // operator[] у vector<bool> возвращает bit_reference
    /*template<>
    class Vector<bool> {
    private:
        uint8_t* arr;
        struct BitReference {
            uint8_t* bucket_ptr;
            uint8_t pos;

            BitReference operator=(bool b) {
                if (b) {
                    *bucket_ptr |= ((uint8_t)1 << pos);
                } else {
                    *bucket_ptr &= ~((uint8_t)1 << pos);
                }
                return *this;
            }

            explicit operator bool() const {
                return *bucket_ptr & (1 << pos);
            }
        };
    public:
        BitReference operator[](std::size_t i) {
            uint8_t pos = i % 8;
            uint8_t* ptr = arr + i / 8;
            return BitReference(ptr, pos);
        }
    };*/
}