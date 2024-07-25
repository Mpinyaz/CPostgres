cmake_minimum_required(VERSION 3.10)

# Project name
project(PostgresCProject C)

# Set the C standard
set(CMAKE_C_STANDARD 99)

# Find the PostgreSQL package
find_package(PostgreSQL REQUIRED)

# Include the PostgreSQL headers
include_directories(${PostgreSQL_INCLUDE_DIRS})

# Add your source files here
# For example, if your source file is main.c
add_executable(CPostgres postgres.c)

# Link the PostgreSQL libraries
target_link_libraries(CPostgres ${PostgreSQL_LIBRARIES})
