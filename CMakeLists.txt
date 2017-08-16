cmake_minimum_required (VERSION 3.0)
project (csv-writer CXX)

# enable C and C++14 language
enable_language (C CXX)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wno-sign-compare")

# set project version
set (CSV_WRITER_MAJOR_VERSION 0)
set (CSV_WRITER_MINOR_VERSION 2)
set (CSV_WRITER_PATCH_VERSION 0)
set (CSV_WRITER_VERSION
    "${CSV_WRITER_MAJOR_VERSION}.\
     ${CSV_WRITER_MINOR_VERSION}.\
     ${CSV_WRITER_PATCH_VERSION}"
     )

message(STATUS "Start of execution CMake for Csv Writer in version "
        "${CSV_WRITER_MAJOR_VERSION}.${CSV_WRITER_MINOR_VERSION}."
        "${CSV_WRITER_PATCH_VERSION}"
       )

# loaction of logger configuration file
set (EASYLOGGING_CONFIG_FILE_PATH
    "${CMAKE_SOURCE_DIR}/easylogging++.config"
    )

# location of additional cmake modules
set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    )

# get git commit hash
include(git-commit-hash)

get_directory_property(HAS_CSV_WRITER_PARENT PARENT_DIRECTORY)

# if csv-writer is standalone project or is sub-project
if(NOT HAS_CSV_WRITER_PARENT)
  # detect operating system
  message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
  if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
      add_definitions(-DSYSTEM_LINUX)
  endif()
  if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
      add_definitions(-DSYSTEM_WINDOWS)
  endif()

  # detect host processor
  message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")

  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

#-------------------------------------------------------------------------------
# BOOST
#-------------------------------------------------------------------------------

find_package( Boost REQUIRED COMPONENTS filesystem )
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(ERROR
        "The Boost library headers were not found; targets depending "
        "on Boost won't be available.")
endif()

#-------------------------------------------------------------------------------
# LIBRARY SOURCES SETTING
#-------------------------------------------------------------------------------

set (CSV_WRITER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (CSV_WRITER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set (CSV_WRITER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/csv-writer.cc)

add_library (csvWriter SHARED ${CSV_WRITER_SOURCES})

target_include_directories (csvWriter PUBLIC ${CSV_WRITER_INCLUDE_DIR})

# target to update git submodules
add_custom_target(
    update_submodules
    COMMAND git submodule init
    COMMAND git submodule update
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

#-------------------------------------------------------------------------------
# EXTERNAL MODULES
#-------------------------------------------------------------------------------

# set external modules path
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/modules)

#-------------------------------------------------------------------------------
# GOOGLETEST MODULE AND ADD TESTS
#-------------------------------------------------------------------------------

add_subdirectory(${EXT_PROJECTS_DIR}/googletest)
# set googletest module and add tests
set(GOOGLETEST_ROOT ${EXT_PROJECTS_DIR}/googletest
    CACHE STRING "Google Test source root")

message(STATUS "GOOGLETEST_ROOT set to ${GOOGLETEST_ROOT}")

set(PROJECT_TEST_NAME ${PROJECT_NAME}-test)

add_definitions(${MSVC_COMPILER_DEFS})

# google test includes
include_directories(${GTEST_INCLUDE_DIRS})

# unit test executable
add_executable(
    ${PROJECT_TEST_NAME}
    test/main.cc
    )

set(TEST_CSV_WRITER_FILES "${CMAKE_SOURCE_DIR}/test/test-csv-writer")

# unit test executable depends on google test
add_dependencies(${PROJECT_TEST_NAME} googletest)

# link unit test executable against google test

if(NOT WIN32 OR MINGW)
    target_link_libraries(${PROJECT_TEST_NAME}
        ${GTEST_LIBS_DIR}/libgtest.a
        ${GTEST_LIBS_DIR}/libgtest_main.a
        )
else()
    target_link_libraries(${PROJECT_TEST_NAME}
        debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
        optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
        )
    target_link_libraries(${PROJECT_TEST_NAME}
        debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
        optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
        )
endif()

target_link_libraries(
    ${PROJECT_TEST_NAME}
    csvWriter
    pthread
    ${Boost_LIBRARIES}
    )

# enable tests
enable_testing()

# include Dart that generate default DartConfiguration.tcl
include(Dart)

# add tests
add_test(csvWriterTests ${PROJECT_TEST_NAME})

#-------------------------------------------------------------------------------
# GENERATE CONFIGURE FILE
#-------------------------------------------------------------------------------

# configure header file
configure_file(
    ${PROJECT_SOURCE_DIR}/cmake/config.h.in
    ${PROJECT_BINARY_DIR}/config.h
    )

# add the binary tree to the search path for include files
# so that we will find config.h
include_directories("${PROJECT_BINARY_DIR}")