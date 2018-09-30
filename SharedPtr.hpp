#include <atomic>
#include <iostream>

class Control_Block {
    std::atomic_uint counter;
public:
    Control_Block() :counter(0) {}
    void add() {
        counter++;
    }
    void del() {
        counter--;
    }
    bool empty() {
        if (counter == 1) return true;
        return false;
    }
    int value_counter() {
        return counter;
    }
    ~Control_Block() {};
};

template <typename T>
class SharedPtr {
public:
    T* ptr;
    Control_Block* control_block;
    SharedPtr() :ptr(nullptr), control_block(nullptr) {};
    SharedPtr(const SharedPtr&);
    SharedPtr<T>& operator = (const SharedPtr<T>&);
    SharedPtr(const T);

    void reset();
    void reset(T*);
    void swap(SharedPtr&);
    T* get() const;
    T& operator*() const;
    T* operator->() const;
    size_t use_count() const;
    operator bool() const;
    ~SharedPtr();
};

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& shared_ptr) {
    ptr = shared_ptr.get();
    control_block = shared_ptr.control_block;
    control_block->add();
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator = (const SharedPtr<T>& shared_ptr) {
    control_block->del();
    if (control_block->empty()) {
        delete ptr;
        delete control_block;
        ptr = shared_ptr.get();
        control_block = shared_ptr.control_block;
        control_block->add();
        return *this;
    }
    ptr = shared_ptr.get();
    control_block = shared_ptr.control_block;
    control_block->add();
    return *this;

}

template <typename T>
SharedPtr<T>::SharedPtr(const T value) {
    ptr = new T(value);
    control_block = new Control_Block();
    control_block->add();
}

template <typename T>
void SharedPtr<T>::reset() {
    if (control_block->empty()) {
        delete ptr;
        delete control_block;
        ptr = nullptr;
        control_block = nullptr;
    }
    else {
        ptr = nullptr;
        control_block = nullptr;
    }
}

template <typename T>
void SharedPtr<T>::reset(T* curr_ptr) {
    if (control_block->empty()) {
        delete ptr;
        delete control_block;
        ptr = curr_ptr;
        control_block = new Control_Block();
        control_block->add();
    }
    else {
        ptr = curr_ptr;
        control_block = new Control_Block();
        control_block->add();
    }
}

template <typename T>
void SharedPtr<T>::swap(SharedPtr& shared_ptr) {
    std::swap(ptr, shared_ptr.ptr);
    std::swap(control_block, shared_ptr.control_block);
}

template <typename T>
T* SharedPtr<T>::get() const {
    if (ptr == nullptr) return 0;
    return ptr;
}

template <typename T>
T& SharedPtr<T>::operator*() const {
    return *ptr;
}

template <typename T>
T* SharedPtr<T>::operator->() const {
    return ptr;
}

template <typename T>
size_t SharedPtr<T>::use_count() const {
    return control_block->value_counter();
}

template <typename T>
SharedPtr<T>::operator bool() const {
    if (ptr == nullptr) return false;
    return true;
}

template <typename T>
SharedPtr<T>::~SharedPtr() {
    if (control_block != nullptr) {
        if (!control_block->empty()) {
            control_block->del();
        }
        else {
            delete ptr;
            delete control_block;
            ptr = nullptr;
            control_block = nullptr;
        }
    }
}