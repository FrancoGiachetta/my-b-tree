
cmake_minimum_required(VERSION 4.2.1)

project("btree" LANGUAGES CXX)

# Configure clangd

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Create symlink immediately during configuration
execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json
)

add_subdirectory("src")
