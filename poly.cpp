#include "poly.h"
#include <utility>
#include <algorithm>
#include <thread>
#include <unordered_map>


// Default constructor: creates a polynomial representing the number 0
polynomial::polynomial() {
    terms.push_back({0,0});
}

// Copy constructor
polynomial::polynomial(const polynomial &other) {
    terms = other.terms;
}

// std::vector<std::pair<power, coeff>> polynomial::canonical_form() const{
//     std::map<power, coeff, std::greater<power>> sorted;
//     std::vector<std::pair<power, coeff>> output0;
//     // only one term in there


//     for (auto it : terms)
//     {
//         if (it.second != 0)
//         {
//             sorted[it.first] = it.second;
//         }
//     }

//     //std::cout<<sorted.size();
//     if (sorted.size() == 0)  
//     {
//         output0.push_back({0, 0});
//         return output0;
//     }
//     std::vector<std::pair<power, coeff>> output(sorted.begin(), sorted.end());
//     return output;

// }

std::vector<std::pair<power, coeff>> polynomial::canonical_form() const
{
    std::vector<std::pair<power, coeff>> output;

    // Process non-zero terms from the unordered_map
    for (const auto& term : this->terms)
    {
        if (term.second != 0) // Include only non-zero coefficients
        {
            output.emplace_back(term.first, term.second);
        }
    }

    // Sort the terms by power in descending order
    std::sort(output.begin(), output.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

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

bool compareDescendingPower(const std::pair<size_t, int>& a, const std::pair<size_t, int>& b) {
    return a.first > b.first;
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
    
   
   
//    polynomial result;

//    // Combine terms from both polynomials
//     std::vector<std::pair<power, coeff>> combined_terms = this->terms;
//     combined_terms.insert(combined_terms.end(), other.terms.begin(), other.terms.end());

//     // Sort the terms in descending order of power
//     std::sort(combined_terms.begin(), combined_terms.end(), compareDescendingPower);

//     //now set up multi threading and break up work in combined terms
//     std::vector<std::vector<std::pair<power, coeff>>> thread_results(5);
//     size_t chunk_size = combined_terms.size() / 5;

//     auto intermediate_add = [&](size_t start, size_t end, size_t thread_index) {
//         for (size_t i = start; i < end; ++i) {
//             const auto &term = combined_terms[i];
//             bool found = false;

//             for (auto &res_term : thread_results[thread_index]) {
//                 if (res_term.first == term.first) {
//                     res_term.second += term.second; // Add coefficients if powers are the same
//                     found = true;
//                     break;
//                 }
//             }
//             if (!found) {
//                 thread_results[thread_index].push_back(term); // Add new term if power is not found
//             }
//         }
//     };

//     std::thread t1(intermediate_add, 0, chunk_size, 0);
//     std::thread t2(intermediate_add, chunk_size, 2 * chunk_size, 1);
//     std::thread t3(intermediate_add, 2 * chunk_size, 3 * chunk_size, 2);
//     std::thread t4(intermediate_add, 3 * chunk_size, 4 * chunk_size, 3);
//     std::thread t5(intermediate_add, 4 * chunk_size, combined_terms.size(), 4);

//     t1.join();
//     t2.join();
//     t3.join();
//     t4.join();
//     t5.join();

//     // Combine results from all threads
//     for (const auto &thread_result : thread_results) {
//         result.terms.insert(result.terms.end(), thread_result.begin(), thread_result.end());
//     }

//     // Sort the result terms by power and combine terms with the same power
//     std::sort(result.terms.begin(), result.terms.end(), compareDescendingPower);
//     // std::sort(result.terms.begin(), result.terms.end());
//     auto it = result.terms.begin();
//     while (it != result.terms.end() - 1) {
//         if (it->first == (it + 1)->first) {
//             it->second += (it + 1)->second;
//             result.terms.erase(it + 1);
//         } else {
//             ++it;
//         }
//     }

//     return result; // Return the result polynomial
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
   
   polynomial result; // Start with an empty polynomial
    
    // Determine the number of threads to use
    size_t num_threads = 8; // Fixed number of threads
    size_t chunk_size = this->terms.size() / num_threads + (this->terms.size() % num_threads != 0);
    std::vector<std::unordered_map<power, coeff>> thread_results(num_threads);
    std::vector<std::thread> threads(num_threads);

    // Lambda function to add terms to the thread-specific result vector
    auto intermediate_multiply = [&](size_t start, size_t end, size_t thread_index) {
        for (size_t i = start; i < end && i < this->terms.size(); ++i) {
            for (auto &res_term : other.terms) {
                power new_power = this->terms[i].first + res_term.first;
                coeff new_coeff = this->terms[i].second * res_term.second;
                thread_results[thread_index][new_power] += new_coeff;

                    // Use unordered_map to store and update terms
                // if (thread_results[thread_index].find(new_power) != thread_results[thread_index].end()) {
                //     thread_results[thread_index][new_power] += new_coeff;
                // } else {
                //     thread_results[thread_index][new_power] = new_coeff;
                // }
                
            }
        }
    };

    //create threads
    for(size_t i = 0; i < num_threads; i++) {
        threads[i] = std::thread(intermediate_multiply, i * chunk_size, (i + 1) * chunk_size, i);
    }

    //join threads
    for(size_t i = 0; i < num_threads; i++) {
        threads[i].join();
    }


    // Define the result unordered_map
    std::unordered_map<power, coeff> combined_results;

    // Combine results from all threads
    for (const auto &thread_result : thread_results) {
        for (const auto &term : thread_result) {
            combined_results[term.first] += term.second;
        }
    }

    // Convert the unordered_map to a vector if needed
    for (const auto &term : combined_results) {
        result.terms.push_back(std::make_pair(term.first, term.second));
    }

    return result; // Return the result polynomial
    
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


polynomial polynomial::operator%(const polynomial &p2) const{
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
    if (this->find_degree_of() < p2.find_degree_of())
    {
        return *this;
    }
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

        subtracts.terms.clear(); // Reset subtraction terms for next iteration
    } while (!subset.terms.empty() && subset.find_degree_of() >= p2.terms.front().first);

    if (subset.terms.size() == 0)
    {
        subset.terms.push_back({0,0});
    }
    return subset;
}

size_t polynomial::find_degree_of() const {
    // std::sort(terms.begin(), terms.end(), std::greater<std::pair<power, coeff>>());
    // return terms.begin() -> first;

    // Create a copy of the terms vector
    std::vector<std::pair<power, coeff>> terms_copy = terms;

    // Sort the copy
    std::sort(terms_copy.begin(), terms_copy.end(), std::greater<std::pair<power, coeff>>());

    // Return the degree of the highest term
    return terms_copy.begin()->first;
}



// You can add more member function implementations here
// Example of a member function implementation
// void polynomial::someFunction() {
//     // Implementation of someFunction
// }