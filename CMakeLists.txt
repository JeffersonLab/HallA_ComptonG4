# Require at least cmake 2.6.0
cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

# Name of this project
project(ComptonG4)

# Add the project include directory and put all source files in my_project_sources
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB my_project_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

# Local path for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Load ROOT and setup include directory
#find_package(ROOT REQUIRED)
#include_directories(${ROOT_INCLUDE_DIR})

# Create ROOT dictionary before messing up include dirs with Geant4 stuff
# (-p option to "use of the compiler's preprocessor instead of CINT's preprocessor.")
#ROOT_GENERATE_DICTIONARY(${CMAKE_CURRENT_BINARY_DIR}/QweakSimUserMainEvent_Dict
#	${CMAKE_CURRENT_SOURCE_DIR}/include/QweakSimUser*Event.hh
#	${CMAKE_CURRENT_SOURCE_DIR}/include/QweakSimUser*Config.hh
#	LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/QweakSimUserMainEvent_LinkDef.hh
#	OPTIONS -p)
#set(my_project_sources ${my_project_sources} ${CMAKE_CURRENT_BINARY_DIR}/QweakSimUserMainEvent_Dict.cxx)
set(my_project_sources ${my_project_sources})

# Load Geant4 through Find and Use files (the Geant4_DIR directory is where
# the Geant4 cmake Find and Use files can be found)
set(Geant4_DIR $ENV{G4INSTALL}/build)
find_package(Geant4 REQUIRED COMPONENTS gdml qt ui_all vis_all)
include(${Geant4_USE_FILE})

# Add the target executables
add_executable(ComptonG4 ComptonG4.cc ${my_project_sources})
add_executable(ComptonG4_batch ComptonG4.cc ${my_project_sources})
set_target_properties(ComptonG4_batch PROPERTIES COMPILE_DEFINITIONS "COMPTONG4_BATCH_MODE=1")

# Link the ROOT and Geant4 libraries
target_link_libraries(ComptonG4 ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
target_link_libraries(ComptonG4_batch ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
