#include <iostream>
#include <chrono>
#include <optional>
#include <vector>
#include <utility>

#include "poly.h"

// std::optional<double> poly_test(polynomial& p1,
//                                 polynomial& p2,
//                                 std::vector<std::pair<power, coeff>> solution)

// {
//     std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

//     polynomial p3 = p1 * p2;

//     auto p3_can_form = p3.canonical_form();

//     std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

//     p3.print();

//     if (p3_can_form != solution)
//     {
//         return std::nullopt;
//     }

//     return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
// }

int main()
{
    /** We're doing (x+1)^2, so solution is x^2 + 2x + 1*/
    std::vector<std::pair<power, coeff>> solution = {{2,1}, {1,2}, {0,1}};

    /** This holds (x+1), which we'll pass to each polynomial */
    std::vector<std::pair<power, coeff>> poly_input = {{1,1}, {0,1}};

    polynomial p1(poly_input.begin(), poly_input.end());
    polynomial p2(poly_input.begin(), poly_input.end());

    polynomial p3 = p1 + p2;

    p1.print();
    p2.print();
    std::cout << "poly1 + poly2: ";
    p3.print();

    // Addition of a polynomial and an integer
    polynomial poly_plus_int = p1 + 10;
    std::cout << "poly1 + 10: ";
    poly_plus_int.print();

    // Addition of an integer and a polynomial
    polynomial int_plus_poly = 20 + p2;
    std::cout << "20 + poly2: ";
    int_plus_poly.print();

    // std::optional<double> result = poly_test(p1, p2, solution);

    // if (result.has_value())
    // {
    //     std::cout << "Passed test, took " << result.value()/1000 << " seconds" << std::endl;
    // } 
    // else 
    // {
    //     std::cout << "Failed test" << std::endl;
    // }
}