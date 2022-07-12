/*--------------------------------------------------------------------*/
/* main.c                                                             */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "reading.h"
#include "vec_newton.h"


/*--------------------------------------------------------------------*/

int main(int argc, char *argv[]) {

    if(argc != 2){
        fprintf(stderr, "Usage: %s critical conversion\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    double crit_conversion = strtod(argv[1], NULL);
    
    Polynomial_t poly = readPoly();
    double* roots = vec_guess(poly, crit_conversion);

    if (roots[0] == DBL_MAX) {
        printf("Your polynomial has no roots.\n");
    }
    else {
        for (int i = 0; i < poly.degree; i++) {
            if (roots[i] == DBL_MAX) {
                break;
            }
            printf("The root approximation is: %lf \n", roots[i]);
        }
    }
    freePoly(&poly);
    free(roots);
    
    // for (int i = 0; i <= polyd.degree; i++) {
    //     printf("%lf \n", polyd.coefficients[i]);
    // }

    return 0;
}
