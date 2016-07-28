# Require at least cmake 2.6.0
cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

# Name of this project
project(ComptonG4_Project)

# Add all the project source files and header files to the respective variables
file(GLOB_RECURSE my_project_sources ${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cc)
file(GLOB_RECURSE my_project_headers ${CMAKE_CURRENT_SOURCE_DIR}/sources/*.hh)
file(GLOB_RECURSE my_project_linkdefs ${CMAKE_CURRENT_SOURCE_DIR}/sources/*LinkDef.h)

# Now add the proper include directories
set(my_project_headers_dirlist "")
foreach( _file ${my_project_headers})
  GET_FILENAME_COMPONENT(_dir ${_file} PATH)
  set(my_project_headers_dirlist ${my_project_headers_dirlist} ${_dir})
endforeach()
list(REMOVE_DUPLICATES my_project_headers_dirlist)
include_directories(${my_project_headers_dirlist})

# Local path for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

## Check if compiler supports C++11 standard if avialable
## (Got this snipped from Wouter)
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has C++11 support.")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has C++0x support.")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
endif()

# Load ROOT and setup include directory
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})
# Create dictionary for headers for ROOT
message("Generating dictionaries for ROOT ${ROOT_VERSION}...")
# Loop over all found LinkDef.h header files
set(my_project_linkdefs_files "")
set(my_project_linkdefs_header_files "")
foreach( _linkdef ${my_project_linkdefs})
  get_filename_component( _file ${_linkdef} NAME)
  set(my_project_linkdefs_files ${my_project_linkdefs_files} ${_file})
  string(REGEX REPLACE "LinkDef.h" ".hh" _header ${_file})
  string(REGEX REPLACE "LinkDef.h" "" _prefix ${_file})
  set(ROOTPWA_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/${_prefix}Dictionary)
ROOT_GENERATE_DICTIONARY(
  "${ROOTPWA_DICTIONARY}"
  "${_header}"
  LINKDEF "${_file}"
  OPTIONS "-p" ## -p to use compilers preprocessor instead of CINTs preprocessor
  )
set(my_project_sources_dictionary ${my_project_sources_dictionary} ${ROOTPWA_DICTIONARY})
endforeach()
#set(ROOT_LIBRARIES ${ROOT_LIBRARIES} -lGui)
message("All project sources: ${my_project_sources_dictionary}")

# Find Boost libraries
# There is a bug in CMAke 2.8's internal find boost process
# (jc2 2016-05-09: seems to also apply to version 3.2, so new
# check will just use greater than 2.7)
if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.7)
  SET( Boost_NO_BOOST_CMAKE ON)
endif(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.7)
find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIR})

find_package(Geant4 REQUIRED COMPONENTS gdml qt ui_all vis_all)                       # Find Geant4
include_directories(${Geant4_INCLUDE_DIRS})         # Add -I type paths
add_definitions(${Geant4_DEFINITIONS})              # Add -D type defs
set(CMAKE_CXX_FLAGS ${Geant4_CXX_FLAGS})            # Optional

# Create a shared library to make compilations faster
add_library(ComptonG4Lib SHARED ${my_project_sources} ${my_project_headers}
  ${my_project_sources_dictionary})
target_link_libraries(ComptonG4Lib ${Geant4_LIBRARIES} ${ROOT_LIBRARIES}
  ${Boost_LIBRARIES})
# We want the output library to have the same name as the main exec
set_target_properties(ComptonG4Lib PROPERTIES OUTPUT_NAME ComptonG4)

# Add the target executables
add_executable(ComptonG4 ComptonG4.cc ${my_project_headers})
add_executable(ComptonG4_batch ComptonG4.cc ${my_project_headers})
set_target_properties(ComptonG4_batch PROPERTIES COMPILE_DEFINITIONS "COMPTONG4_BATCH_MODE=1")
add_executable(ComptonG4Root ComptonG4Root.cc)
set_target_properties(ComptonG4Root PROPERTIES OUTPUT_NAME cg4root)

# Find Boost libraries
# Though, there seems to be a bug in version 2.8 of CMake's boost
## finding process. As such, we'll disable it for version 2.8
#if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} EQUAL 2.8)
#  SET( Boost_NO_BOOST_CMAKE ON )
#endif(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} EQUAL 2.8)

# Link the ROOT and Geant4 libraries
target_link_libraries(ComptonG4 ComptonG4Lib ${Geant4_LIBRARIES}
  ${ROOT_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(ComptonG4_batch ComptonG4Lib ${Geant4_LIBRARIES}
  ${ROOT_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(ComptonG4Root ${ROOT_LIBRARIES})

# Now install the executables
install (TARGETS ComptonG4 DESTINATION bin)
install (TARGETS ComptonG4_batch DESTINATION bin)
install (TARGETS ComptonG4Root DESTINATION bin)
install (TARGETS ComptonG4Lib DESTINATION lib)
install( FILES ${my_project_headers} DESTINATION include )
