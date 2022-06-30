/*--------------------------------------------------------------------*/
/* main.c                                                             */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "reading.h"
#include "newton.h"
#include "derivative.h"

/*--------------------------------------------------------------------*/

int main(int argc, char *argv[]) {

    if(argc != 2){
        fprintf(stderr, "Usage: %s critical conversion\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    double crit_conversion = strtod(argv[1], NULL);
    
    Polynomial_t poly = readPoly();
    double* guesses = guess(poly, crit_conversion);
    for (int i = 0; i < poly.degree; i++) {
        printf("The root approximation is: %lf \n", guesses[i]);
    }
    freePoly(&poly);
    free(guesses);
    
    // for (int i = 0; i <= polyd.degree; i++) {
    //     printf("%lf \n", polyd.coefficients[i]);
    // }

    return 0;
}
