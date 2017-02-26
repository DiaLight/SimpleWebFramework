//
// Created by dialight on 16.01.17.
//

#ifndef OPENCVCLIENT_POINTERGUARD_HPP
#define OPENCVCLIENT_POINTERGUARD_HPP

template<typename T>
class PointerGuard {
public:

    PointerGuard() {}
    PointerGuard(T *pointer) : pointer(pointer) {}

    ~PointerGuard() {
        if(pointer != nullptr) delete pointer;
    }

    PointerGuard(const T&) = delete;
    PointerGuard& operator=(const T&) = delete;

    T* operator->() {
        return pointer;
    }

    void set(T *pointer) {
        this->pointer = pointer;
    }

private:
    T *pointer;
};

#endif //OPENCVCLIENT_POINTERGUARD_HPP
