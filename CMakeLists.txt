cmake_minimum_required (VERSION 3.21)
project(cstrutils
    LANGUAGES   C
    DESCRIPTION "String handling function for C."
)


enable_testing()

add_subdirectory(src) 
add_subdirectory(tests)

