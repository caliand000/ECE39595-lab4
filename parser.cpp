#include <iostream>
#include "parser.h"

polynomial build_poly_from_file(char const *filename) {
    FILE *fptr = fopen(filename, "r");
    if(fptr ==  NULL) {
        std::cout << ANSI_RED << __func__ << " failed due to invalid filename" << ANSI_RESET << "\n";
        return polynomial();
    }

    power first;
    coeff second;
    std::vector<std::pair<power,coeff>> params;
    //stop at the line that is not formatted well
    while(fscanf(fptr, "%dx^%zu\n", &second, &first) == 2) {
        params.push_back(std::pair<power, coeff> {first, second});
    }

    if(fgetc(fptr) != ';') {
        std::cout << ANSI_RED << "encounter a line with wrong format" << ANSI_RESET << "\n";
    }

    fclose(fptr);

    return polynomial(params.begin(), params.end());
}