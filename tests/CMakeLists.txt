include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

include_directories(../include)

add_executable(cpu_speed cpu_speed.cpp ../src/mos6502.cpp)

target_link_libraries(cpu_speed
PRIVATE GTest::GTest)

add_test(cpu_speed_gtest cpu_speed)