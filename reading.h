#pragma once
/*--------------------------------------------------------------------*/
/* reading.h                                                          */
/* Author: Dara Oseyemi                                               */
/*--------------------------------------------------------------------*/

#include "polynomial.h"

/*--------------------------------------------------------------------*/

// Reads n and list of a_n from STDIN to create and return Polynoimial_t
// struct.
Polynomial_t readPoly(void);