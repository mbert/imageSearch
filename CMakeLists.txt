# The name of our project is "IMAGESEARCH". CMakeLists files in this project
# can refer to the root source directory of the project as
# ${IMAGESEARCH_SOURCE_DIR} and to the root binary directory of the project
# as ${IMAGESEARCH_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
project (IMAGESEARCH)

add_subdirectory (backend) 
add_subdirectory (webapp) 
