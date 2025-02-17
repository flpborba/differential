add_library(ddt
    bf.cpp
    differential.cpp
)

target_include_directories(ddt
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(ddt
    PUBLIC
        ntl
        pthread
        tbb
)
