add_executable(unit_test
    test.cpp
    bf_test.cpp
    difference_map_test.cpp
)

target_link_libraries(unit_test
    PRIVATE
        ddt
)

add_custom_target(test unit_test)
