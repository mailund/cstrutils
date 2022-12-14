add_library(${PROJECT_NAME} STATIC)
add_library(${PROJECT_NAME}::framework ALIAS ${PROJECT_NAME})

file(GLOB SOURCES ./*.h ./*.c)
target_sources(${PROJECT_NAME}
    PRIVATE 
        ${SOURCES}
)
# FIXME: make this portable (but right now, the variable I used to switch on
# doesn't seem to be set, so that didn't work).
target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wexit-time-destructors
        -Wglobal-constructors
        -Wmissing-noreturn
        -Wmissing-prototypes
        -Wno-missing-braces
        -Wold-style-cast
        -Wshadow
        -Wweak-vtables
        -Werror
        -Wall
        #-O3
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(${PROJECT_NAME}_PUBLIC_HEADERS
        cstrutils.h
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES 
        C_STANDARD          23
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS        OFF
        PUBLIC_HEADER       "${${PROJECT_NAME}_PUBLIC_HEADERS}"
        EXPORT_NAME         framework
)

install(TARGETS   ${PROJECT_NAME}
    EXPORT        ${PROJECT_NAME}Targets
    ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT     library
)

