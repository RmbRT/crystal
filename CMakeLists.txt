cmake_minimum_required(VERSION 2.6)
project(Crystal)

# Ensure all dependencies are existent.
execute_process(
	COMMAND git submodule init
	COMMAND git submodule update
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(depend/netlib)

# Set the C++ standard used by the Crystal library.
set(CMAKE_CXX_FLAGS "-std=c++17 -Wfatal-errors -g")

# Add Lock library include path.
include_directories(include)

# Select all source files.
file(GLOB_RECURSE crystal_sources ./src/*.cpp)
# Select all header files.
file(GLOB_RECURSE crystal_headers ./src/*.hpp ./src/*.inl)

# Add them to the Crystal library.
add_library(crystal STATIC ${crystal_sources} ${crystal_headers})

# Link the Crystal library with pthreads and netlib.
target_link_libraries(crystal pthread netlib)

# Creates an include directory containing all header files used in the Crystal library
# Add crystal/include/ to your include directories and access the files via #include <crystal/*>
file(COPY "src/" DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/crystal/ FILES_MATCHING PATTERN "*.hpp" PATTERN "*.inl")

# Add includes from dependencies into the include directory.
file(COPY "depend/Lock/include/" DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/)
file(COPY "depend/netlib/include/" DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/)
file(COPY "depend/cppeie/include/" DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/)
