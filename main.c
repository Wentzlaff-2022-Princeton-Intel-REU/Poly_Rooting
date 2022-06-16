/*--------------------------------------------------------------------*/
/* main.c.                                                            */
/* Author: Manya Zhu.                                                 */
/*--------------------------------------------------------------------*/

#include <stdlib.h>

int main(int argc, char *argv[]) {

    if(argc != 2){
        fprintf("stderr, "Usage: %s critical conversion\n", argv[0]")
        exit(EXIT_FAILURE);
    }
    Polynomial_t poly = readPoly();
    int crit_conversion = argv[1];
    
    

}