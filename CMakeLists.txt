cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(Vito_ISOLDE)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

add_executable(ISOLDE ISOLDE.cc ${sources} ${headers})
target_link_libraries(ISOLDE ${Geant4_LIBRARIES})

add_custom_target(Vito_ISOLDE DEPENDS ISOLDE)