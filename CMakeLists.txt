cmake_minimum_required(VERSION 3.22)


project(stellar_enigma
  VERSION 0.0.1
  DESCRIPTION "A simple 2D Game Engine"
  LANGUAGES CXX)

set(CXX g++)



set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type (Debug or Release)")
set(MAP_WIN_SIZE "16;12" STRING "Sets the size of the playable window")

#boolean options
option(RAND_TIME "Use time to seed RNG" OFF)
option(RAW_IN "If true, use raw keyboard state. Otherwise, use OS Events" ON)
option(STATIC_BUILD "If true, build stellar as a static library" ON)

# grab files. If necessary, add other subdirectories here
file(GLOB AUDIO_SRC "src/audio/*.cpp")
file(GLOB CUTSCENE_SRC "src/cutscene/*.cpp")
file(GLOB INTERFACE_SRC "src/interface/*.cpp")
file(GLOB LEVEL_SRC "src/level/*.cpp")
file(GLOB MAPDATA_SRC "src/mapdata/*.cpp")
file(GLOB MISC_SRC "src/misc/*.cpp")
file(GLOB MUTABLE_SRC "src/mutable/*.cpp")
file(GLOB UTILITY_SRC "src/utility/*.cpp")
file(GLOB PHYSICS_SRC "src/physics/*.cpp")

set(SRC ${AUDIO_SRC} ${CUTSCENE_SRC} ${INTERFACE_SRC} ${LEVEL_SRC} ${MAPDATA_SRC} ${MISC_SRC} ${MUTABLE_SRC} ${UTILITY_SRC} ${PHYSICS_SRC})

file(GLOB AUDIO_INC "include/audio/*.h")
file(GLOB CUTSCENE_INC "include/cutscene/*.h")
file(GLOB INTERFACE_INC "include/interface/*.h")
file(GLOB LEVEL_INC "include/level/*.h")
file(GLOB MAPDATA_INC "include/mapdata/*.h")
file(GLOB MISC_INC "include/misc/*.h")
file(GLOB MUTABLE_INC "include/mutable/*.h")
file(GLOB UTILITY_INC "include/utility/*.h")
file(GLOB PHYSICS_INC "include/physics/*.h")

set(INC ${AUDIO_INC} ${CUTSCENE_INC} ${INTERFACE_INC} ${LEVEL_INC} ${MAPDATA_INC} ${MISC_INC} ${MUTABLE_INC} ${UTILITY_INC} ${PHYSICS_INC} include/stellar-enigma.hpp)

add_library(stellar ${SRC} ${INC})
add_executable(stellar-test src/main.cpp)

target_link_libraries(stellar sfml-graphics sfml-window sfml-audio sfml-system json11)
target_link_libraries(stellar-test stellar)


set_property(TARGET stellar-test PROPERTY CXX_STANDARD 17)


target_include_directories(stellar PRIVATE "./include/")
target_include_directories(stellar-test PRIVATE "./include/")
target_compile_definitions(stellar PRIVATE WINDOW_WIDTH=16 WINDOW_HEIGHT=12 FRAMERATE=30)
target_compile_definitions(stellar-test PRIVATE WINDOW_WIDTH=16 WINDOW_HEIGHT=12 FRAMERATE=30)


if(RAND_TIME)
  target_compile_definitions(stellar PRIVATE RAND_USE_TIME)
endif()

if(RAW_IN)
  target_compile_definitions(stellar PRIVATE SE_RAW_IN)
endif()

target_compile_options(stellar PRIVATE -Wall -Wextra -Wpedantic)

# Doxygen

find_package(Doxygen)
option(BUILD_DOC "Generate Documentation with Doxygen" ON)

if(BUILD_DOC)
    if(NOT DOXYGEN_FOUND)
         message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(doc
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM)

    #    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION     share/doc)
endif()
