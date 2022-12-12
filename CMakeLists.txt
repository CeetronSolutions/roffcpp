cmake_minimum_required(VERSION 3.15)

project(roffcpp
  VERSION 0.1
  DESCRIPTION "Roff reader"
  LANGUAGES CXX)

add_subdirectory(src)
enable_testing()
add_subdirectory(tests)
