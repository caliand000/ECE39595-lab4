#ifndef POLY_H
#define POLY_H

#include <map>
#include <vector>
#include <utility>
#include <cstddef>
#include <iostream>

using power = size_t;
using coeff = int;

class polynomial
{    

public:
    /*
    * The polynomial is represented as a vector of pairs. Each pair contains a
    * coefficient and a power. The polynomial is the sum of all the terms in the
    * vector. The power is the exponent of the term, and the coefficient is the
    * coefficient of the term. For example, the polynomial 3x^2 + 2x + 1 would
    * be represented as the vector [(2,3),(1,2),(0,1)].
    */

    std::vector<std::pair<power, coeff>> terms;

    /**
     * @brief Construct a new polynomial object that is the number 0 (ie. 0x^0)
     *
     */
    polynomial();

    /**
     * @brief Construct a new polynomial object from an iterator to pairs of <power,coeff>
     *
     * @tparam Iter
     *  An iterator that points to a std::pair<power, coeff>
     * @param begin
     *  The start of the container to copy elements from
     * @param end
     *  The end of the container to copy elements from
     */
    template <typename Iter>
    polynomial(Iter begin, Iter end){
        for(Iter it = begin; it!=end; it++) {
            terms.push_back(*it);  // assign key value pairs
        }

    };

    /**
     * @brief Construct a new polynomial object from an existing polynomial object
     *
     * @param other
     *  The polynomial to copy
     */
    polynomial(const polynomial &other);

     std::map<power, coeff, std::greater<power>> sortMap() const;

    /**
     * @brief Prints the polynomial.
     *
     * Only used for debugging, isn't graded.
     *
     */
    void print() const;

    /**
     * @brief Turn the current polynomial instance into a deep copy of another
     * polynomial
     *
     * @param other
     * The polynomial to copy
     * @return
     * A reference to the copied polynomial
     */
    polynomial &operator=(const polynomial &other);


    /**
     * Overload the +, * and % operators. The function prototypes are not
     * provided.
     * 
     * Addition (+) should support
     * 1. polynomial + polynomial
     * 2. polynomial + int
     * 3. int + polynomial
     * 
     * Multiplication (*) should support
     * 1. polynomial * polynomial
     * 2. polynomial * int
     * 3. int * polynomial
     * 
     * Modulo (%) should support
     * 1. polynomial % polynomial
     */

    // Addition of two polynomials
    polynomial operator+(const polynomial &other) const;

    // Addition of a polynomial and an integer
    polynomial operator+(int value) const;

    // Friend function for addition of an integer and a polynomial
    friend polynomial operator+(int value, const polynomial &poly);

    // Multiplication of two polynomials
    polynomial operator*(const polynomial &other) const;

    // Multiplication of a polynomial and an integer
    polynomial operator*(int value) const;

    // Friend function for multiplication of an integer and a polynomial
    friend polynomial operator*(int value, const polynomial &poly);

    // Modulo of a polynomial with another polynomial
    polynomial operator%(polynomial &p2);

    /**
     * @brief Returns the degree of the polynomial
     *
     * @return size_t
     *  The degree of the polynomial
     */
    size_t find_degree_of();

    /**
     * @brief Returns a vector that contains the polynomial is canonical form. This
     *        means that the power at index 0 is the largest power in the polynomial,
     *        the power at index 1 is the second largest power, etc.
     *
     *        ie. x^2 + 7x^4 + 1 would be returned as [(4,7),(2,1),(0,1)]
     *
     *        Note: any terms that have a coefficient of zero aren't returned in
     *        in the canonical form, with one exception.
     *        See the above example (there's no x^3 term, so
     *        there's no entry in the vector for x^3)
     *
     *        The only exception to this is the polynomial 0. If your polynomial is
     *        just the constant 0 then you would return a single entry of [(0,0)]
     *
     *        ie. y = 0 would be returned as [(0,0)]
     *
     * @return std::vector<std::pair<power, coeff>>
     *  A vector of pairs representing the canonical form of the polynomial
     */
    std::vector<std::pair<power, coeff>> canonical_form() const;

};

#endif
