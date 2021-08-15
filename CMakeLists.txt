cmake_minimum_required(VERSION 3.16)
project(learnboost)

set (CMAKE_CXX_STANDARD 14)

find_package(Boost)

if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(version version.cc)
add_executable(daytime_client daytime_client.cc)
add_executable(daytime_server daytime_server.cc)