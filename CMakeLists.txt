cmake_minimum_required (VERSION 2.8.12)
project (sam2pseudo)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

## Determine build type and set flags accordingly
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release) ## Build Release by default
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -Wall -Wextra -Wpedantic")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g -Wall -Wextra -Wpedantic")
endif()

## Set C++11 support depending on cmake version
if (${CMAKE_MAJOR_VERSION} GREATER 2 AND ${CMAKE_MINOR_VERSION} GREATER 0)
  set (CMAKE_CXX_STANDARD 11)
  set (CMAKE_CXX_STANDARD_REQUIRED ON)
else()
  add_compile_options(-std=c++11)
endif()

## sam2pseudo executable
add_executable(sam2pseudo ${CMAKE_CURRENT_SOURCE_DIR}/src/sam2pseudo.cpp)

## Check supported compression types
find_package(BZip2)
if (BZIP2_FOUND)
  include_directories(${BZIP2_INCLUDE_DIRS})
  target_link_libraries(sam2pseudo ${BZIP2_LIBRARIES})
endif()    
find_package(LibLZMA)
if (LIBLZMA_FOUND)
  include_directories(${LIBLZMA_INCLUDE_DIRS})
  target_link_libraries(sam2pseudo ${LIBLZMA_LIBRARIES})
endif()
find_package(ZLIB)
if (ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIRS})
  target_link_libraries(sam2pseudo ${ZLIB_LIBRARIES})
endif()

## Check dependencies and download them if not given
## bxzstr
if (DEFINED CMAKE_BXZSTR_HEADERS)
  include_directories("${CMAKE_BXZSTR_HEADERS}")
else()
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/CMakeLists-bxzstr.txt.in ${CMAKE_BINARY_DIR}/external/bxzstr-download/CMakeLists.txt)
  execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/bxzstr-download )
  if(result)
    message(FATAL_ERROR "CMake step for bxzstr failed: ${result}")
  endif()
  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/bxzstr-download )
  if(result)
    message(FATAL_ERROR "Build step for bxzstr failed: ${result}")
  endif()
  include_directories(${CMAKE_CURRENT_BINARY_DIR}/external/bxzstr/include)
endif()
