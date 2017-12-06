cmake_minimum_required(VERSION 2.6)
project(Crystal)

# Set the C++ standard used by the RmbRT Engine.
set(CMAKE_CXX_FLAGS "-std=c++17 -Wfatal-errors -g")

# Add Lock library include path.
include_directories(depend/Lock/include)

# Select all source files.
file(GLOB_RECURSE crystal_sources ./src/*.cpp)
# Select all header files.
file(GLOB_RECURSE crystal_headers ./src/*.hpp ./src/*.inl)

# Add them to the RmbRT Engine library.
add_library(crystal ${crystal_sources} ${crystal_headers})

# Link the RmbRT Engine with GLFW.
target_link_libraries(crystal pthread)

# Creates an include directory containing all header files used in the RmbRT Engine.
# Add re/include/ to your include directories and access the files via #include <re/*>
file(COPY "src/" DESTINATION "include/crystal/" FILES_MATCHING PATTERN "*.hpp" PATTERN "*.inl")
file(COPY "depend/Lock/include/" DESTINATION "include/")