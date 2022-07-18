# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)
project (Poly_Rooting)

include_directories(include)
add_library(poly scr/freePoly.c scr/longDiv.c scr/reading.c)

add_executable (poly_rooting scr/scalar/derivative.c scr/scalar/horner.c scr/scalar/main.c scr/scalar/newton.c)
target_include_directories(poly_rooting PRIVATE include/scalar)
target_link_libraries(poly_rooting PRIVATE poly)

add_executable (multi_poly_rooting scr/multi/derivative.c scr/multi/horner.c scr/multi/main.c scr/multi/newton.c)
target_include_directories(multi_poly_rooting PRIVATE include/multi)
target_link_libraries(multi_poly_rooting PRIVATE poly)

add_executable (vec_poly_rooting scr/vector/derivative.c scr/vector/horner.c scr/vector/main.c scr/vector/newton.c)
target_include_directories(vec_poly_rooting PRIVATE include/vector)
target_link_libraries(vec_poly_rooting PRIVATE poly)

# add_executable (ara_poly_rooting scr/ara/reading.c scr/ara/scalar/derivative.c scr/ara/scalar/horner.c scr/ara/scalar/main.c scr/ara/scalar/newton.c scr/freePoly.c scr/longDiv.c)
# add_executable (ara_vec_poly_rooting scr/ara/reading.c scr/ara/vector/derivative.c scr/ara/vector/horner.c scr/ara/vector/main.c scr/ara/vector/newton.c scr/freePoly.c scr/longDiv.c)
