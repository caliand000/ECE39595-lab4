#ifndef PARSER_H
#define PARSER_H
#include "poly.h"

#define ANSI_RED "\x1b[1;31m"
#define ANSI_RESET "\x1b[0m"

polynomial build_poly_from_file(char const *filename);


#endif