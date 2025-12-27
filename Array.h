#pragma once

#include <cstdlib>
#include <cassert>
#include <new>
#include <type_traits>
#include <utility>

template <typename T>
class Array final {
public:
    class Iterator {
    private:
        Array<T>& container;
        size_t index;
        bool rev;

    public:
        Iterator(Array<T>& arr, size_t index = 0, bool rev = false) 
            : container(arr), index(index), rev(rev) {}

        T& get() {
            return container[index];
        }

        const T& get() const {
            return container[index];
        }

        void set(const T& value) {
            container[index] = value;
        }

        void next() {
            if (hasNext()) {
                if (rev == false) {
                    index++;
                }
                else {
                    index--;
                }
            }
        }

        bool hasNext() const {
            if (container.size() == 0) {
                return false;
            }
            if (rev == false) {
                return index < static_cast<size_t>(container.size()) - 1;
            }
            else {
                return index > 0;
            }
        }
    };

    class ConstIterator {
    private:
        const Array<T>& container;
        size_t index;
        bool rev;

    public:
        ConstIterator(const Array<T>& arr, size_t index = 0, bool rev = false) 
            : container(arr), index(index), rev(rev) {}

        const T& get() const {
            return container[index];
        }

        void next() {
            if (hasNext()) {
                if (rev == false) {
                    index++;
                }
                else {
                    index--;
                }
            }
        }

        bool hasNext() const {
            if (container.size() == 0) {
                return false;
            }
            if (rev == false) {
                return index < static_cast<size_t>(container.size()) - 1;
            }
            else {
                return index > 0;
            }
        }
    };

private:
    static constexpr int default_initial_capacity = 8;
    static constexpr double growth_factor = 1.6;

    T* container;
    int length;
    int current_capacity;

    void swap(Array& other) noexcept {
        std::swap(container, other.container);
        std::swap(length, other.length);
        std::swap(current_capacity, other.current_capacity);
    }

    void EnsureCapacity() {
        if (length < current_capacity) {
            return;
        }

        int new_capacity = static_cast<int>(current_capacity * growth_factor) + 1;
        T* new_buf = (T*)malloc(new_capacity * sizeof(T));
        if (!new_buf) {
            throw std::bad_alloc();
        }

        T* old_buf = container;
        container = new_buf;

        for (int i = 0; i < length; i++) {
            if constexpr (std::is_nothrow_move_constructible<T>::value) {
                new (container + i) T(std::move(old_buf[i]));
            }
            else {
                new (container + i) T(old_buf[i]);
            }
            old_buf[i].~T();
        }

        free(old_buf);
        current_capacity = new_capacity;
    }

public:
    Array() : Array(default_initial_capacity) {}

    explicit Array(int capacity) {
        container = (T*)malloc(capacity * sizeof(T));
        if (!container) {
            throw std::bad_alloc();
        }
        current_capacity = capacity;
        length = 0;
    }

    ~Array() {
        for (int i = 0; i < length; i++) {
            container[i].~T();
        }
        free(container);
    }

    Array(const Array& arr) : Array(arr.current_capacity) {
        for (int i = 0; i < arr.length; i++) {
            new (container + i) T(arr[i]);
            length++;
        }
    }

    Array(Array&& arr) noexcept 
        : container(arr.container), length(arr.length), current_capacity(arr.current_capacity) {
        arr.container = nullptr;
        arr.length = 0;
        arr.current_capacity = 0;
    }

    Array& operator=(Array other) noexcept {
        swap(other);
        return *this;
    }

    int insert(const T& value) {
        if (length == current_capacity) {
            EnsureCapacity();
        }
        new (container + length) T(value);
        length++;
        return length - 1;
    }

    int insert(T&& value) {
        if (length == current_capacity) {
            EnsureCapacity();
        }
        new (container + length) T(std::move(value));
        length++;
        return length - 1;
    }

    int insert(int index, const T& value) {
        assert(index >= 0 && index <= length);

        if (length == current_capacity) {
            EnsureCapacity();
        }

        for (int i = length; i > index; i--) {
            if constexpr (std::is_nothrow_move_constructible<T>::value) {
                new (container + i) T(std::move(container[i - 1]));
            }
            else {
                new (container + i) T(container[i - 1]);
            }
            container[i - 1].~T();
        }

        new (container + index) T(value);
        length++;
        return index;
    }

    int insert(int index, T&& value) {
        assert(index >= 0 && index <= length);

        if (length == current_capacity) {
            EnsureCapacity();
        }

        for (int i = length; i > index; i--) {
            if constexpr (std::is_nothrow_move_constructible<T>::value) {
                new (container + i) T(std::move(container[i - 1]));
            }
            else {
                new (container + i) T(container[i - 1]);
            }
            container[i - 1].~T();
        }

        new (container + index) T(std::move(value));
        length++;
        return index;
    }

    void remove(int index) {
        assert(index >= 0 && index < length);

        if (length > 0) {
            container[index].~T();

            for (int i = index; i < length - 1; i++) {
                if constexpr (std::is_nothrow_move_constructible<T>::value) {
                    new (container + i) T(std::move(container[i + 1]));
                }
                else {
                    new (container + i) T(container[i + 1]);
                }
                container[i + 1].~T();
            }

            length--;
        }
    }

    int size() const {
        return length;
    }

    const T& operator[](int index) const {
        assert(index >= 0 && index < length);
        return container[index];
    }

    T& operator[](int index) {
        assert(index >= 0 && index < length);
        return container[index];
    }

    Iterator iterator() {
        return Iterator(*this);
    }

    ConstIterator iterator() const {
        return ConstIterator(*this);
    }

    Iterator reverseIterator() {
        return Iterator(*this, length > 0 ? length - 1 : 0, true);
    }

    ConstIterator reverseIterator() const {
        return ConstIterator(*this, length > 0 ? length - 1 : 0, true);
    }
};
