cmake_minimum_required (VERSION 2.8.11)
project (WormHole)
# The version number.
execute_process(
    COMMAND git describe --always --tags
    OUTPUT_VARIABLE wh_VERSION)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# C_STANDARD
set (CMAKE_C_STANDARD 11)
set (CMAKE_CXX_STANDARD 11)

include_directories("include")
include_directories("include/libworm")
add_definitions(-O3 -g -pipe -flto -ffat-lto-objects) # -Wall -Wextra -Werror eliminado por problema con libressl

set(WARNINGFLAGS "-Wall -Wextra -Werror")

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto -ldl -O3 -g -pipe")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -flto -ldl -O3 -g -pipe")

if (NOT NO_REDEFINEFUNTIONS)
    #Function to make output a bit more quiet
    function(message)
        if (NOT MESSAGE_QUIET)
            _message(${ARGN})
        endif()
    endfunction()

    #Function to remove imported executables
    function(add_executable)
        if (NOT NO_COMPILE_EXECUTABLES)
            _add_executable(${ARGN})
        else()
            _add_executable(${ARGN})
            set_target_properties(${ARGV0} PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
        endif()
    endfunction()

    #Function to remove custom targets
    function(add_custom_target)
        if (NOT NO_ADD_CUSTOM)
            _add_custom_target(${ARGN})
        else()
        endif()
    endfunction()

    function(INSTALL)
        if (NOT NO_COMPILE_EXECUTABLES)
            _INSTALL(${ARGN})
        else()
        endif()
    endfunction()
endif()

#Import all .hpp
file(GLOB libworm_INCLUDE
    "include/libworm/*.h"
    "include/libworm/*.hpp"
)

#Import all .cpp
file(GLOB libworm_SRC
    "src/libworm/*.c"
    "src/libworm/*.cpp"
)

include(Config.cmake)

# Dependencies
set(NO_REDEFINEFUNTIONS ON)

#Netlib
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/repos/netlib" ${CMAKE_MODULE_PATH})

set(DESTDIR "/")
set(OPENSSLDIR "/etc/ssl")

set(NO_COMPILE_EXECUTABLES ON CACHE BOOL "FORCE TO RECURSIVELY unset this flag")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/repos/netlib/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/repos/netlib/src)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/dependencies/repos/netlib dependencies/repos/netlib/build)
unset(NO_COMPILE_EXECUTABLES CACHE)

#HPTL
set(NO_COMPILE_EXECUTABLES ON CACHE BOOL "FORCE TO RECURSIVELY unset this flag")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/repos/hptimelib/include)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/dependencies/repos/hptimelib dependencies/repos/hptimelib/build)
unset(NO_COMPILE_EXECUTABLES CACHE)

#LibWorm: Binary-Headers
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/structures.o/structures.o
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/structures.o/"
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/tools/parseFile.bash ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/structures.o/parseFile.bash
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/structures.o/include
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/structures.o/" && ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/structures.o/parseFile.bash > ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/structures.o/structures.h
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/structures.o/" && objcopy --input binary --output elf64-x86-64 --binary-architecture i386 structures.h structures.o
    DEPENDS ${libworm_INCLUDE}
    VERBATIM
)
add_library(worm-structures STATIC ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/structures.o/structures.o ${libworm_INCLUDE})
SET_SOURCE_FILES_PROPERTIES(
  ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/structures.o/structures.o
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
)
SET_TARGET_PROPERTIES(
  worm-structures
  PROPERTIES
  LINKER_LANGUAGE C 
)

#LibWorm
set(libworm_libdeps dl pthread)

add_library(libworm_obj OBJECT ${libworm_SRC} ${libworm_INCLUDE})
# shared libraries need PIC
set_property(TARGET ${libworm_obj} PROPERTY POSITION_INDEPENDENT_CODE 1)
# actual shared and static libraries built from the same object files
add_library(libworm-shared SHARED $<TARGET_OBJECTS:libworm_obj>)
add_library(libworm STATIC $<TARGET_OBJECTS:libworm_obj>)

#Library wall messages
set_target_properties(libworm_obj PROPERTIES COMPILE_FLAGS "${WARNINGFLAGS} -fPIC")

#Renombrar libworms
set_target_properties(libworm PROPERTIES
        OUTPUT_NAME worm
        ARCHIVE_OUTPUT_NAME worm)
set_target_properties(libworm-shared PROPERTIES
        OUTPUT_NAME worm
        ARCHIVE_OUTPUT_NAME worm)

#Library Dependencies
#target_link_libraries(libworm rt pthread ssl crypto)
#target_link_libraries(libworm-shared rt pthread ssl-shared crypto-shared)
target_link_libraries(libworm        worm-structures netlib          hptl           ${libworm_libdeps})
target_link_libraries(libworm-shared worm-structures netlib-shared   hptl-shared    ${libworm_libdeps})

# Aplicactions & Examples
unset(files CACHE)
file(GLOB files "mk/*.mk")
foreach(file ${files})
    MESSAGE( STATUS "Including file: " ${file})
    include(${file})
endforeach()