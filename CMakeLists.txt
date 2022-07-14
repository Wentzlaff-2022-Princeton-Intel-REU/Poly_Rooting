# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)
project (Poly_Rooting)
add_executable (poly_rooting derivative.c freePoly.c horner.c longDiv.c main.c multi_horner.c multi_newton.c newton.c reading.c)
add_executable (vec_poly_rooting freePoly.c longDiv.c reading.c vec_derivative.c vec_horner.c vec_main.c vec_newton.c)
