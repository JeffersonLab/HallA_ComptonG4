# Require at least cmake 2.6.0
cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

# Name of this project
project(ComptonG4_Project)

# Add the project include directory and put all source files in my_project_sources
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/detectors/include)
file(GLOB my_project_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/detectors/src/*.cc)
file(GLOB my_project_headers ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/detectors/include/*.hh)

# Local path for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Load ROOT and setup include directory
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})
#set(ROOT_LIBRARIES ${ROOT_LIBRARIES} -lGui)

# Find Boost libraries
# There is a bug in CMAke 2.8's internal find boost process
if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} EQUAL 2.8)
  SET( Boost_NO_BOOST_CMAKE ON)
endif(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} EQUAL 2.8)

find_package(Geant4 REQUIRED COMPONENTS gdml qt ui_all vis_all)                       # Find Geant4
include_directories(${Geant4_INCLUDE_DIRS})         # Add -I type paths
add_definitions(${Geant4_DEFINITIONS})              # Add -D type defs
set(CMAKE_CXX_FLAGS ${Geant4_CXX_FLAGS})            # Optional

# Add the target executables
add_executable(ComptonG4 ComptonG4.cc ${my_project_sources} ${my_project_headers})
add_executable(ComptonG4_batch ComptonG4.cc ${my_project_sources}  ${my_project_headers})
set_target_properties(ComptonG4_batch PROPERTIES COMPILE_DEFINITIONS "COMPTONG4_BATCH_MODE=1")

# Find Boost libraries
# Though, there seems to be a bug in version 2.8 of CMake's boost
# finding process. As such, we'll disable it for version 2.8
if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} EQUAL 2.8)
  SET( Boost_NO_BOOST_CMAKE ON )
endif(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} EQUAL 2.8)

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIR})


# Link the ROOT and Geant4 libraries
target_link_libraries(ComptonG4 ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(ComptonG4_batch ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} ${Boost_LIBRARIES})

# Now install the executables
install (TARGETS ComptonG4 DESTINATION bin)
install (TARGETS ComptonG4_batch DESTINATION bin)
