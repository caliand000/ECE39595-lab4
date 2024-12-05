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
         polynomial result;

        // Determine number of threads to use
        size_t total_work = this->terms.size() * other.terms.size();    // total number of multiplications
        size_t num_threads = 8; // fixed number of threads
        size_t chunk_size = total_work / num_threads + (total_work % num_threads != 0); // determine how many terms go in a chunk

        // thread stuffs
        std::vector<std::unordered_map<power, coeff>> thread_results(num_threads);
        std::vector<std::thread> threads(num_threads);

        // lambda function to add terms to the thread-specific result vector
        auto intermediate_multiply = [&](size_t start, size_t end, size_t thread_index) {
            for (size_t index = start; index < end; ++index) {
                size_t i = index / other.terms.size(); // taken from first polynomial
                size_t j = index % other.terms.size(); // taken from second polynomial, all elements up to chunk size

                power new_power = this->terms[i].first + other.terms[j].first;
                coeff new_coeff = this->terms[i].second * other.terms[j].second;

                thread_results[thread_index][new_power] += new_coeff;
            }
        };

        // create threads
        for (size_t i = 0; i < num_threads; ++i) {
            size_t start = i * chunk_size;
            size_t end = (start + chunk_size) > total_work ? total_work : start + chunk_size;   // makes sure the end does not go out of bounds
            threads[i] = std::thread(intermediate_multiply, start, end, i);
        }

        // Join threads
        for (size_t i = 0; i < num_threads; ++i) {
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

        return result;  // Return the result polynomial
    } 

// Multiplication of a polynomial and an integer
polynomial polynomial::operator*(int value) const {
        polynomial result;

        // similar to poly * poly, used to break up the main polynomial into chunks and multiply those chunks and reconstruct
        // Determine number of threads to use
        size_t num_threads = 8; // fixed number of threads
        size_t chunk_size = terms.size() / num_threads + (terms.size() % num_threads != 0); // determine how many terms go in a chunk

        // thread stuffs
        std::vector<std::unordered_map<power, coeff>> thread_results(num_threads);
        std::vector<std::thread> threads(num_threads);

        // lambda function to add terms to the thread-specific result vector
        auto intermediate_multiply = [&](size_t start, size_t end, size_t thread_index) {
            for (size_t index = start; index < end; ++index) {
                power new_power = terms[index].first;
                coeff new_coeff = terms[index].second * value;

                thread_results[thread_index][new_power] += new_coeff;
            }
        };

        // create threads
        for (size_t i = 0; i < num_threads; ++i) {
            size_t start = i * chunk_size;
            size_t end = (start + chunk_size) > terms.size() ? terms.size() : start + chunk_size;   // makes sure the end does not go out of bounds
            threads[i] = std::thread(intermediate_multiply, start, end, i);
        }

        // Join threads
        for (size_t i = 0; i < num_threads; ++i) {
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

        // Convert the unordered_map to a vector
        for (const auto &term : combined_results) {
            result.terms.push_back(std::make_pair(term.first, term.second));
        }

        return result;  // Return the result polynomial
    
    // polynomial result = *this;
    // for (auto &term : result.terms) {
    //     term.second *= value;
    // }
    // return result;
    
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

    
    // */
    polynomial divisor = p2;
    polynomial newDividend = *this;
    polynomial quotient;
    polynomial subtracts;

    std::pair<int, power> divid = newDividend.findDegAndInd();
    std::pair<int, power> divis = divisor.findDegAndInd();

    size_t dividendInd = divid.first;
    power dividendPower = divid.second;

    size_t divisorInd = divis.first;
    power divisorPower = divis.second;
    
    // auto [dividendInd, dividendPower] = newDividend.findDegAndInd();
    // auto [divisorInd, divisorPower] = divisor.findDegAndInd();

    if (divisorPower > dividendPower)
    {
        return *this;
    }

    // while dividend power is higher than divisor power
    while (dividendPower >= divisorPower)
    {
        // calculate new coefficient and power
        auto multCoeff = newDividend.terms[dividendInd].second / divisor.terms[divisorInd].second;
        auto multPower = dividendPower - divisorPower;
        
        // add new single term polynomial to quotient
        quotient.terms.push_back({multPower, multCoeff});
        
        // perform operations
        subtracts = quotient * divisor * -1;
        newDividend = newDividend + subtracts;
        
        // update dividend degree
        auto dividUpdate = newDividend.findDegAndInd();
        dividendInd = dividUpdate.first;
        dividendPower = dividUpdate.second;
        
        quotient.terms.clear(); // remove the term to make way for another single term
    }

    
    return newDividend;
    // get new term


    


    


    // return *this;

    // int numTermsP2 = p2.terms.size();

    // polynomial subset;
    // polynomial subtracts;
    // int powers;
    // int coeffs;
    // auto origIt = terms.begin();
    // if (this->find_degree_of() < p2.find_degree_of())
    // {
    //     return *this;
    // }
    // do {
    //     // make dividend subset with as many terms as the divisor (p2), or if there are no more terms left
    //     while ((int)subset.terms.size() <= numTermsP2 && origIt != terms.end()) {
    //         subset.terms.push_back(*origIt); // Add term to the subset polynomial
    //         ++origIt;
    //     }

    //     // sort subset in descending order
    //     std::sort(subset.terms.begin(), subset.terms.end(), std::greater<std::pair<power, coeff>>());

    //     // find power and coefficient to cancel out subset when subtracting
    //     powers = subset.terms.front().first - p2.terms.front().first;
    //     coeffs = subset.terms.front().second / p2.terms.front().second;

    //     // store into the subtracting polynomial
    //     subtracts.terms.push_back({powers, coeffs});    // add cancel term to subtracts polynomial
    //     std::sort(subtracts.terms.begin(), subtracts.terms.end(), std::greater<std::pair<power, coeff>>()); // sort in descending order of powers

    //     subset = subset + (subtracts * p2 * -1); // perform the subtraction to get whats left and put in subset poly

    //     // get rid of zeros
    //     subset.terms.erase(
    //         std::remove_if(subset.terms.begin(), subset.terms.end(), [](const std::pair<power, coeff> &term) {
    //             return term.second == 0;
    //         }),
    //         subset.terms.end()
    //     );

    //     subtracts.terms.clear(); // Reset subtraction terms for next iteration
    // } while (!subset.terms.empty() && subset.find_degree_of() >= p2.terms.front().first);

    // if (subset.terms.size() == 0)
    // {
    //     subset.terms.push_back({0,0});
    // }
    // return subset;
}

size_t polynomial::find_degree_of() const {
    size_t deg = 0;
    for (const auto & pair : terms) // go through and find the highest degree
    {
        deg = (pair.first > deg) ? pair.first : deg;
    }
    return deg;
}

void polynomial::sortTerms() {
    std::sort(terms.begin(), terms.end(), std::greater<std::pair<power, coeff>>());
}

// index, power
std::pair<int, power> polynomial::findDegAndInd() const{

        // similar to poly * poly, used to break up the main polynomial into chunks and multiply those chunks and reconstruct
        // Determine number of threads to use
        size_t num_threads = 8; // fixed number of threads
        size_t chunk_size = terms.size() / num_threads + (terms.size() % num_threads != 0); // determine how many terms go in a chunk

        // thread stuffs
        // std::vector<std::unordered_map<power, coeff>> thread_results(num_threads);
        std::vector<std::pair<int, power>> degIndResult((num_threads > terms.size()) ? terms.size() : num_threads);
        std::vector<std::thread> threads((num_threads > terms.size()) ? terms.size() : num_threads);

        // lambda function to add terms to the thread-specific result vector
        auto intermediate_multiply = [&](size_t start, size_t end, size_t thread_index) {
            int maxInd = start;
            power max_power = (terms[start].second != 0) ? terms[start].first : 0;
            for (size_t i = start + 1; i < end; ++i) {
                if ((terms[i].second != 0) && (terms[i].first > max_power))
                {
                    max_power = terms[i].first;
                    maxInd = i;
                }
            }

            degIndResult[thread_index] = {maxInd, max_power};
        };

        // create threads   
        for (size_t i = 0; i < num_threads && i < terms.size(); ++i) {
            size_t start = i * chunk_size;
            size_t end = (start + chunk_size) > terms.size() ? terms.size() : start + chunk_size;   // makes sure the end does not go out of bounds
            threads[i] = std::thread(intermediate_multiply, start, end, i);
        }

        // Join threads
        for (auto& t : threads) {
            t.join();
        }

        // find the highest index and power
        int maxInd = 0;
        power maxPower = 0;
        for (const auto &thread_result : degIndResult) {
            if ((terms[thread_result.first].second != 0) && thread_result.second > maxPower)
            {
                maxInd = thread_result.first;
                maxPower = thread_result.second;
            }
        }
        
        return {maxInd, maxPower};

}



// You can add more member function implementations here
// Example of a member function implementation
// void polynomial::someFunction() {
//     // Implementation of someFunction
// }