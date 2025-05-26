cmake_minimum_required(VERSION 3.18)

project(Sidequest CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_BINARY_DIR ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})
set(CMAKE_USE_RELATIVE_PATHS FALSE)

set(SIDEQUEST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/sidequest")
set(SIDEQUEST_SERVER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/server")
set(SIDEQUEST_DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")
include_directories(${SIDEQUEST_INCLUDE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(GetSourceFiles)

set(FILE_TYPES *.cpp *.h )
find_directories(LIBRARY_SOURCE_DIRS "src/sidequest" "${FILE_TYPES}")
generate_source_tree(LIBRARY_SOURCES "${LIBRARY_SOURCE_DIRS}" "${FILE_TYPES}")

find_directories(SERVER_SOURCE_DIRS "src/server" "${FILE_TYPES}")
generate_source_tree(SERVER_SOURCES "${SERVER_SOURCE_DIRS}" "${FILE_TYPES}")

#########################################
#message(STATUS "LIBRARY_SOURCES: ${LIBRARY_SOURCES}")
#message(STATUS "SERVER_SOURCES: ${SERVER_SOURCES}")
#########################################

link_directories(${SIDEQUEST_DEPENDENCIES_DIR})
add_library(sidequest_lib STATIC ${LIBRARY_SOURCES} )

add_library(server_lib ${SERVER_SOURCES}
		src/server/storage/database_exceptions.h
		src/server/storage/database_exceptions.cpp
		src/server/storage/Query.cpp
		src/server/storage/Query.h
		src/server/model/server_quest.cpp
		src/server/model/server_quest.h
		src/server/storage/statement_cache.cpp
		src/server/storage/statement_cache.h
		src/server/storage/column_cache.cpp
		src/server/storage/column_cache.h
		src/server/storage/prepared_statement.h
)
# Ensures that server_lib compiles before sidequest_lib, no effect on linking -> target_link_libraries
add_dependencies(server_lib sidequest_lib)

add_executable(server src/server/main.cpp)

# server_lib depends on sidequest_lib
target_link_libraries(server_lib sidequest_lib)
# server depends on server_lib and sqlite3
target_link_libraries(server server_lib sqlite3)
# Redundant
#target_link_libraries(server sidequest_lib)
#target_link_libraries(server server_lib)
#target_link_libraries(server sqlite3)

include_directories(${SIDEQUEST_INCLUDE_DIR})
include_directories(${SIDEQUEST_SERVER_INCLUDE_DIR})
include_directories(${SIDEQUEST_DEPENDENCIES_DIR})

# set the application working directory
set(VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/application_root")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

find_directories(TEST_SOURCE_DIRS "src/unit_tests" "${FILE_TYPES}")
generate_source_tree(TEST_SOURCES "${TEST_SOURCE_DIRS}" "${FILE_TYPES}")

add_executable(
  unit_tests
  ${TEST_SOURCES}
		src/unit_tests/quest_threadding_test.cpp
)
target_link_libraries(
  unit_tests
  GTest::gtest_main
  sidequest_lib
  server_lib
  sqlite3
)

include(GoogleTest)
gtest_discover_tests(unit_tests
    #WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/application_root
)



# copy necessary dlls to target directory
if(MSVC)
	add_custom_command(
		TARGET server
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
		"${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sqlite3.dll"
		"$<TARGET_FILE_DIR:server>"
	)
	add_custom_command(
		TARGET unit_tests
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
		"${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sqlite3.dll"
		"$<TARGET_FILE_DIR:server>"
	)
endif(MSVC)

# Custom run rule to test the server from applikation_root
add_custom_target(run_server
    COMMAND ${CMAKE_BINARY_DIR}/bin/server
    DEPENDS server
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    USES_TERMINAL
    VERBATIM
)

