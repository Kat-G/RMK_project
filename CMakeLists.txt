cmake_minimum_required(VERSION 3.10)

project(RMK_PROJECT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(RMK_PROJECT
    main.cpp
    Shift.cpp
    Receipt.cpp
    ProductDataBase.cpp
    MainApp.cpp
)

target_include_directories(RMK_PROJECT PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
configure_file(${CMAKE_SOURCE_DIR}/products.csv ${CMAKE_BINARY_DIR}/products.csv COPYONLY)