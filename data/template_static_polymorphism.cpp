#include <iostream>

template <class derived>
struct Base {
    void interface() {
        static_cast<derived*>(this)->implementation();
    }
};

struct Derived: Base<Derived> {
    void implementation() {
        std::cout << "derived struct" << std::endl;
    }
};


int main() {
    Derived d;
    d.implementation(); 
    d.interface();
    return 0;
}

