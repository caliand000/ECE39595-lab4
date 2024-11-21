#include "poly.h"
#include <utility>
#include <algorithm>
// Default constructor: creates a polynomial representing the number 0
polynomial::polynomial() {
    terms.push_back({0,0});
}

// Copy constructor
polynomial::polynomial(const polynomial &other) {
    terms = other.terms;
}

std::vector<std::pair<power, coeff>> polynomial::canonical_form() const{
    std::map<power, coeff, std::greater<power>> sorted;
    std::vector<std::pair<power, coeff>> output0;
    // only one term in there


    for (auto it : terms)
    {
        if (it.second != 0)
        {
            sorted[it.first] = it.second;
        }
    }

    //std::cout<<sorted.size();
    if (sorted.size() == 0)  
    {
        output0.push_back({0, 0});
        return output0;
    }
    std::vector<std::pair<power, coeff>> output(sorted.begin(), sorted.end());
    return output;

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
    polynomial result;

    // go thru all elements and multiply scalar
    for (const auto &term : poly.terms) {
        result.terms.push_back({term.first, term.second * value});
    }

    return result;
}

polynomial polynomial::operator%(const polynomial &p2) {
        /*
        assuming sorted vecs
        we need to perform division as long as there is an exponent that is of the same power
        as the highest exponent of poly2, which we know is this:

        poly2.at(0).first   // gives the degree of the highest term

        so we will keep on divisioning while poly1.at(i).first is greater than this
        poly2.at(0).first value

        while (poly1.at(i).first >= poly2.at(0).first) 
         // let's take x^3 -2x^2 + 0x - 4 / x-3 
         //              poly1              poly2
        
        To perform division, we need to do the following:
        Grab the subset (need as many terms from poly1 as in poly2)
        1. Match the exponents together
            - Make poly2.at(0).first equal poly1.at(0).first
                - subtract poly1 power by poly2 power (3-1 from our example to get x^2)
            - make poly2.at(0).second equal poly2.at(0).second
                - divide poly1 coeff by poly2 coeff (1/1) = 1
            - Make new pair with the new power and coeff (quotient) (x^2)
        2. Multiply quotient by poly2 and -1  
            - Take a subset of poly1 to only include as many terms as
              poly2 has (x^3 - 2x^2)
                - Quotient is currently -x^3+ 3x^2
        3. Add the poly1 subset with the quotient
            - Should get x^2
        4. Add the next pairs from poly1 into the subset so the number of terms are equal again
            - x^2 + 0x

    
    */
    int numTermsP2 = p2.terms.size();

    polynomial subset;
    polynomial subtracts;
    int powers;
    int coeffs;
    auto origIt = terms.begin();
    
    do {
        // make dividend subset with as many terms as the divisor (p2), or if there are no more terms left
        while ((int)subset.terms.size() <= numTermsP2 && origIt != terms.end()) {
            subset.terms.push_back(*origIt); // Add term to the subset polynomial
            ++origIt;
        }

        // sort subset in descending order
        std::sort(subset.terms.begin(), subset.terms.end(), std::greater<std::pair<power, coeff>>());

        // find power and coefficient to cancel out subset when subtracting
        powers = subset.terms.front().first - p2.terms.front().first;
        coeffs = subset.terms.front().second / p2.terms.front().second;

        // store into the subtracting polynomial
        subtracts.terms.push_back({powers, coeffs});    // add cancel term to subtracts polynomial
        std::sort(subtracts.terms.begin(), subtracts.terms.end(), std::greater<std::pair<power, coeff>>()); // sort in descending order of powers
        
        subset = subset + (subtracts * p2 * -1); // perform the subtraction to get whats left and put in subset poly
        
        // get rid of zeros
        subset.terms.erase(
            std::remove_if(subset.terms.begin(), subset.terms.end(), [](const std::pair<power, coeff> &term) {
                return term.second == 0;
            }),
            subset.terms.end()
        );

        //sort again
        std::sort(subset.terms.begin(), subset.terms.end(), std::greater<std::pair<power, coeff>>());
        subtracts.terms.clear(); // Reset subtraction terms for next iteration
    } while (!subset.terms.empty() && subset.find_degree_of() >= p2.terms.front().first);

    return subset;
}

size_t polynomial::find_degree_of() {
    return terms.begin() -> first;
}



// You can add more member function implementations here
// Example of a member function implementation
// void polynomial::someFunction() {
//     // Implementation of someFunction
// }