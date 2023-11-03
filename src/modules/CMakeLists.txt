cmake_minimum_required(VERSION 3.15)

file(GLOB SUBMODULES_DIRS LIST_DIRECTORIES true RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)

message(STATUS "Submodules dirs: ${SUBMODULES_DIRS}")

foreach (SUBMODULE_DIR ${SUBMODULES_DIRS})
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${SUBMODULE_DIR}/CMakeLists.txt)
        add_subdirectory(${SUBMODULE_DIR})
    endif ()
endforeach ()