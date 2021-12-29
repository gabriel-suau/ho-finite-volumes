cmake_minimum_required(VERSION 3.10)
project(HOFV)
enable_language(C)

# set the project name
set(HOFV_VERSION_MAJOR "1")
set(HOFV_VERSION_MINOR "0")
set(HOFV_VERSION_PATCH "0")
set(HOFV_VERSION "${HOFV_VERSION_MAJOR}.${HOFV_VERSION_MINOR}.${HOFV_VERSION_PATCH}")

option(HOFV_DOC "Build the doxygen documentation" OFF)

# set the main cmake compilation variables
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(HOFV_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

# include files
include_directories(${HOFV_SOURCE_DIR})

# glob source files into one variable
file(GLOB HOFV_SOURCE_FILES ${HOFV_SOURCE_DIR}/*.c)

# include the math library
find_library(M_LIB m)
set(LIBRARIES ${M_LIB} ${LIBRARIES})
link_libraries(${LIBRARIES})

# add the executable
add_executable(hofv ${HOFV_SOURCE_FILES})

# # documentation
# if (DMG_DOC)
#   add_subdirectory(doc)
# endif()
