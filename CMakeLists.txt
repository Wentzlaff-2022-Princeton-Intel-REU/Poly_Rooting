# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)
project (Poly_Rooting)
add_executable (poly_rooting main.c reading.c horner.c newton.c derivative.c multiHorner.c multiNewton.c polynomial.c longDiv.c)
add_executable (vec_poly_rooting vec_main.c reading.c vec_horner.c vec_derivative.c vec_newton.c polynomial.c longDiv.c)
