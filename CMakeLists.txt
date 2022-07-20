# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)
project (Poly_Rooting)

include_directories(include)
add_library(poly src/freePoly.c src/longDiv.c src/reading.c)

add_executable (poly_rooting src/scalar/derivative.c src/scalar/horner.c src/scalar/main.c src/scalar/newton.c)
target_include_directories(poly_rooting PRIVATE include/scalar)
target_link_libraries(poly_rooting PRIVATE poly)

add_executable (multi_poly_rooting src/multi/derivative.c src/multi/horner.c src/multi/main.c src/multi/newton.c)
target_include_directories(multi_poly_rooting PRIVATE include/multi)
target_link_libraries(multi_poly_rooting PRIVATE poly)

add_executable (vec_poly_rooting src/vector/derivative.c src/vector/horner.c src/vector/main.c src/vector/newton.c)
target_include_directories(vec_poly_rooting PRIVATE include/vector)
target_link_libraries(vec_poly_rooting PRIVATE poly)

# add_executable (ara_poly_rooting src/ara/reading.c src/ara/scalar/derivative.c src/ara/scalar/horner.c src/ara/scalar/main.c src/ara/scalar/newton.c src/freePoly.c src/longDiv.c)
# add_executable (ara_vec_poly_rooting src/ara/reading.c src/ara/vector/derivative.c src/ara/vector/horner.c src/ara/vector/main.c src/ara/vector/newton.c src/freePoly.c src/longDiv.c)
