cmake_minimum_required (VERSION 2.6)
project (defend)

aux_source_directory (src SOURCES)

add_definitions(-D_GNU_SOURCE)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_link_libraries(${PROJECT_NAME}  dl)
