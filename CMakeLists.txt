cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)

project(Vito_ISOLDE)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)


# set(sources
#     ISOLDE.cc
#     src/actionInitialization.cc
#     src/construction.cc
#     src/eventAction.cc
#     src/physicsList.cc
#     src/primaryGeneratorAction.cc
#     src/runAction.cc
#     src/sensitiveDetector.cc
#     src/VITOMagneticField.cc
# )

add_executable(ISOLDE ISOLDE.cc ${sources} ${headers})

target_include_directories(ISOLDE PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(ISOLDE ${Geant4_LIBRARIES})

find_package(ROOT REQUIRED)
target_link_libraries(ISOLDE ${Geant4_LIBRARIES} ROOT::Core ROOT::RIO)

add_custom_target(Vito_ISOLDE DEPENDS ISOLDE)
