

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)


set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

set(THREADS_PREFER_PTHREAD_FLAG ON)


# Tests need to be added as executables first
add_executable(roffcpp-tests TokenTests.cpp AsciiTokenizerTests.cpp BinaryTokenizerTests.cpp ReaderTests.cpp  roffcpptestmain.cpp)


CONFIGURE_FILE( ${CMAKE_CURRENT_LIST_DIR}/RoffTestDataDirectory.hpp.cmake
                ${CMAKE_BINARY_DIR}/Generated/RoffTestDataDirectory.hpp
)

target_include_directories(roffcpp-tests PUBLIC ../src/ ${CMAKE_BINARY_DIR}/Generated)

target_compile_features(roffcpp-tests PRIVATE cxx_std_17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(roffcpp-tests PRIVATE roffcpp gtest gtest_main)
add_test(NAME roffcpp-tests COMMAND roffcpp-tests)
