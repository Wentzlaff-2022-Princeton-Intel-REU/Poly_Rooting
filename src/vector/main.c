/*--------------------------------------------------------------------*/
/* main.c                                                             */
/*--------------------------------------------------------------------*/

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include "freePoly.h"
#include "newton.h"
#include "reading.h"

/*--------------------------------------------------------------------*/

int main(int argc, char *argv[]) {

    if(argc != 2){
        fprintf(stderr, "Usage: %s critical conversion\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    double crit_conversion = strtod(argv[1], NULL);
    
    Polynomial_t poly = reading();
    double* roots = newton(poly, crit_conversion);

    if (roots[0] == DBL_MAX) {
        printf("Your polynomial has no real roots.\n");
    }
    else {
        for (int i = 0; i < poly.degree; i++) {
            if (roots[i] == DBL_MAX) {
                break;
            }
            printf("The root approximation is: %.18lg \n", roots[i]);
        }
    }
    freePoly(&poly);
    free(roots);

    return 0;
}
