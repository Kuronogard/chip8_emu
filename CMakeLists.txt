cmake_minimum_required(VERSION 2.6)
project(chip8_emu)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${chip8_emu_SOURCE_DIR}/CMake")

set(BIN_DIR ${chip8_emu_SOURCE_DIR}/bin)
set(SRC_DIR ${chip8_emu_SOURCE_DIR}/src)

# Bump up warning levels appropriately for clang, gcc & msvc
# Also set debug/optimization flags depending on the build type. IDE users choose this when
# selecting the build mode in their IDE
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
endif()


INCLUDE (FindPkgConfig)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SRC_DIR})


set(SOURCES ${SRC_DIR}/instructions.c ${SRC_DIR}/chip8.c ${SRC_DIR}/instructions.h ${SRC_DIR}/chip8.h ${SRC_DIR}/types.h)
add_executable(${PROJECT_NAME} src/main.c ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})

install(TARGETS chip8_emu RUNTIME DESTINATION ${BIN_DIR})
