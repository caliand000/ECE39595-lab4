#include "poly.h"

// Default constructor: creates a polynomial representing the number 0
polynomial::polynomial() {
    terms.push_back(std::make_pair(0, 0));
}

// Copy constructor
polynomial::polynomial(const polynomial &other) {
    terms = other.terms;
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

// Addition of two polynomials
polynomial polynomial::operator+(const polynomial &other) const {
    polynomial result = *this;
    for (const auto &term : other.terms) {
        bool found = false;
        for (auto &res_term : result.terms) {
            if (res_term.first == term.first) {
                res_term.second += term.second;
                found = true;
                break;
            }
        }
        if (!found) {
            result.terms.push_back(term);
        }
    }
    return result;
}

// Addition of a polynomial and an integer
polynomial polynomial::operator+(int value) const {
    polynomial result = *this;
    bool found = false;
    for (auto &term : result.terms) {
        if (term.first == 0) {
            term.second += value;
            found = true;
            break;
        }
    }
    if (!found) {
        result.terms.push_back(std::make_pair(0, value));
    }
    return result;
}

// Addition of an integer and a polynomial
polynomial operator+(int value, const polynomial &poly) {
    return poly + value;
}

// Multiplication of two polynomials
polynomial polynomial::operator*(const polynomial &other) const {
    polynomial result;
    result.terms.clear(); // Clear the default term (0, 0)

    for (const auto &term1 : terms) {
        for (const auto &term2 : other.terms) {
            power new_power = term1.first + term2.first;
            coeff new_coeff = term1.second * term2.second;

            bool found = false;
            for (auto &res_term : result.terms) {
                if (res_term.first == new_power) {
                    res_term.second += new_coeff;
                    found = true;
                    break;
                }
            }
            if (!found) {
                result.terms.push_back(std::make_pair(new_power, new_coeff));
            }
        }
    }
    return result;
}

// Multiplication of a polynomial and an integer
polynomial polynomial::operator*(int value) const {
    polynomial result = *this;
    for (auto &term : result.terms) {
        term.second *= value;
    }
    return result;
}

// Multiplication of an integer and a polynomial
polynomial operator*(int value, const polynomial &poly) {
    return poly * value;
}

size_t polynomial::find_degree_of() {
    size_t highest_degree = 0;
    for (const auto &term : terms) {
        if (term.first > highest_degree) {
            highest_degree = term.first;
        }
    }
    if (highest_degree == 0 && terms.begin()->second == 0) {
        return 0;
    }
    return highest_degree;
}

// You can add more member function implementations here

// Example of a member function implementation
// void polynomial::someFunction() {
//     // Implementation of someFunction
// }