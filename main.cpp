#include <iostream>
#include <chrono>
#include <optional>
#include <vector>
#include <utility>
#include <chrono>
#include <fstream>

#include "poly.h"
#include "parser.h"

std::optional<double> poly_test(polynomial& p1,
                                polynomial& p2,
                                std::vector<std::pair<power, coeff>> solution)

{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::cout << "Starting test" << std::endl;
    polynomial p3 = p1 * p2;
    std::cout << "Finished multiplication" << std::endl;

    auto p3_can_form = p3.canonical_form();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    // p3.print();

    if (p3_can_form != solution)
    {
        return std::nullopt;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

int main()
{
    /** We're doing (x+1)^2, so solution is x^2 + 2x + 1*/
    //std::vector<std::pair<power, coeff>> solution = {{3,1}, {2,-2}, {1,0}, {0,-4}};

    /** This holds (x+1), which we'll pass to each polynomial */
    // std::vector<std::pair<power, coeff>> poly_input = {{3,1}, {2,-2}, {1,0}, {0,-4}};
    // std::vector<std::pair<power, coeff>> poly_input2 = {{1,1}, {0, -3}};
    // 3x4−5x2+3 by x+2.
    // std::vector<std::pair<power, coeff>> poly_input = {{2,1}, {1,5}, {0,6}};
    // std::vector<std::pair<power, coeff>> poly_input2 = { {3,1}, {1, 1}};

    // std::vector<std::pair<power, coeff>> poly_input = {{3,1}, {2,1},{1,1}, {0,1}};
    // std::vector<std::pair<power, coeff>> poly_input2 = {{1,1}, {0, 9}};
    // polynomial p1(poly_input.begin(), poly_input.end());
    // polynomial p2(poly_input2.begin(), poly_input2.end());

    // p1.print();
    // p2.print();

    // polynomial p3 = p1 % p2;
    // p3.print();
    
    // polynomial p4 = -1 * p2;
    // polynomial p5 = p2 * -1;
    // p4.print();
    // p5.print();

    //CANONICAL TESTING
    // std::vector<std::pair<power, coeff>> poly_input2 = {{2,7}, {4,-1}, {0,1}};
    // polynomial p4 = polynomial(poly_input2.begin(), poly_input2.end());
    // std::vector<std::pair<power, coeff>> canon;
    // canon = p4.canonical_form();

    // for (auto it: canon)
    // {
    //     std::cout << it.first << ", "<< it.second << "  ";
    // }
    
    
    // std::optional<double> result = poly_test(p1, p2, solution);

    // if (result.has_value())
    // {
    //     std::cout << "Passed test, took " << result.value()/1000 << " seconds" << std::endl;
    // } 
    // else 
    // {
    //     std::cout << "Failed test" << std::endl;
    // }

    

    
    std::vector<std::pair<power, coeff>> solution = build_poly_from_file("result.txt").canonical_form();
    
    polynomial p1(build_poly_from_file("poly1.txt"));
    polynomial p2(build_poly_from_file("poly2.txt"));
    
    // p2.print();
    // p1.print();

    std::optional<double> result = poly_test(p1, p2, solution);


    if (result.has_value())
    {
        std::cout << "Passed test, took " << result.value()/1000 << " seconds" << std::endl;
    } 
    else 
    {
        std::cout << "Failed test" << std::endl;
    }
    


    
    
    // Create two polynomials
    // std::vector<std::pair<power, coeff>> poly_input1 = {{3, 1}, {2, 1}, {1, 1}, {0, 1}}; // x^3 + x^2 + x + 1
    // std::vector<std::pair<power, coeff>> poly_input2 = {{1, 1}, {0, 9}}; // x + 9

    // polynomial p1(poly_input1.begin(), poly_input1.end());
    // polynomial p2(poly_input2.begin(), poly_input2.end());

    // // Print the input polynomials
    // std::cout << "Polynomial 1: ";
    // p1.print();
    // std::cout << "Polynomial 2: ";
    // p2.print();

    // // Add the polynomials
    // polynomial p3 = p1 * p2;

    // // Print the result
    // std::cout << "Result of multiplication: ";
    // p3.print();
    
    
    
}