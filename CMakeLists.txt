project("RF Codec" C)
cmake_minimum_required(VERSION 3.0)

set(TARGET_GROUP production CACHE STRING "Group to build")

add_subdirectory(rf_codec)

if(TARGET_GROUP STREQUAL production)
elseif(TARGET_GROUP STREQUAL test)
    include(CTest)
    add_subdirectory(external)
    add_subdirectory(test)
else()
    message(FATAL_ERROR "Given TARGET_GROUP unknown")
endif()
