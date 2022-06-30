#pragma once
/*------------------------------------------------------------*/
/* reading.h                                                  */
/*------------------------------------------------------------*/

#include "polynomial.h"

/*------------------------------------------------------------*/
// frees memory
void freePoly(Polynomial_t* poly);

// Reads n and list of a_n from STDIN to create and return Polynoimial_t
// struct.
Polynomial_t readPoly(void);