cmake_minimum_required(VERSION 3.16)

project(toaster_protocol LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(SpellCheck.Core STATIC
    SpellCheck.Core/SpellCheck.cpp
)
target_include_directories(SpellCheck.Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/SpellCheck.Core
)

add_executable(SpellCheck
    SpellCheck/Source.cpp
)
target_link_libraries(SpellCheck PRIVATE SpellCheck.Core)

add_library(Cipher.Core STATIC
    Cipher.Core/Cipher.cpp
)
target_include_directories(Cipher.Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Cipher.Core
)

add_executable(Cipher
    Cipher/Source.cpp
)
target_link_libraries(Cipher PRIVATE Cipher.Core)
