# The name of our project is "IMAGESEARCH". CMakeLists files in this project
# can refer to the root source directory of the project as
# ${IMAGESEARCH_SOURCE_DIR} and to the root binary directory of the project
# as ${IMAGESEARCH_BINARY_DIR}.

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(IMAGESEARCH)

ADD_SUBDIRECTORY(backend) 
ADD_SUBDIRECTORY(webapp) 
ADD_SUBDIRECTORY(importer) 
