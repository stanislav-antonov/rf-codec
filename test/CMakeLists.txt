set(TEST_EXE_NAME tests)

add_executable(${TEST_EXE_NAME} tests.c)

target_link_libraries(${TEST_EXE_NAME}
    rf_codec
    Unity
)

add_test(NAME tests_suite COMMAND ${TEST_EXE_NAME})
