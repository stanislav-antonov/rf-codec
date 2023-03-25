add_executable(tests_executable
    tests.c
)

target_link_libraries(tests_executable
    hamming_codes
    Unity
)

add_test(tests_suite tests_executable)
