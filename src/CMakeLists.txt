add_library(ddt
    bf.cpp
    difference_map.cpp
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

add_executable(ddt-exec
    main.cpp)

target_link_libraries(ddt-exec
    PRIVATE ddt)
