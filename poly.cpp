#include "poly.h"

// Default constructor: creates a polynomial representing the number 0
polynomial::polynomial() {
    terms.push_back(std::make_pair(0, 0));
}


polynomial &polynomial::operator=(const polynomial &other) {
    if(this == &other) {
        return *this;
    }
    else {
        this->terms = other.terms;
    }
    return *this;   
}

void polynomial::print() const {
    for(auto it = terms.begin(); it!=terms.end(); it++) {
        std::cout << it->second << "x^" << it->first << " ";
        if(next(it, 1) != terms.end()){
            std::cout << "+ ";
        }
    }
    std::cout << std::endl;
}

// You can add more member function implementations here

// Example of a member function implementation
// void polynomial::someFunction() {
//     // Implementation of someFunction
// }