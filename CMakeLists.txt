cmake_minimum_required(VERSION 3.1)
project(ScaleUntrim LANGUAGES CXX)

option(BUILD_PERFORMANCE_TEST "More subdivisition for performance test" OFF)
option(BUILD_LOG "Enable verbose log" OFF)
option(BUILD_GUROBI "Enable GUROBI for comparison ONLY" OFF)
option(BUILD_OPENMP "Enable support for OpenMP" OFF)
option(BUILD_TBB "Enable support for TBB" OFF)
option(BUILD_FREE_LICENSE "Only use libraries with permissive licenses" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(Boost_USE_STATIC_LIBS ON)
find_package(Eigen REQUIRED)
find_package(Boost REQUIRED)
find_package(OpenCASCADE REQUIRED)

if (BUILD_GUROBI)
    find_package(GUROBI REQUIRED)
endif(BUILD_GUROBI)

if (BUILD_TBB)
    find_package(TBB REQUIRED)
endif(BUILD_TBB)

set(LEMON_3RD_PATH 3rd/lemon-1.3.1)

find_path(
    LEMON_SOURCE_ROOT_DIR
    CMakeLists.txt
    PATHS ${CMAKE_SOURCE_DIR}/${LEMON_3RD_PATH} NO_DEFAULT_PATH
    DOC "Location of LEMON source as a CMAKE subproject"
)

if (EXISTS ${LEMON_SOURCE_ROOT_DIR})
    add_subdirectory(${LEMON_SOURCE_ROOT_DIR})
    set(LEMON_INCLUDE_DIRS ${LEMON_SOURCE_ROOT_DIR} ${PROJECT_BINARY_DIR}/${LEMON_3RD_PATH})
    set(LEMON_LIBRARIES lemon)
    unset(LEMON_ROOT_DIR CACHE)
    unset(LEMON_DIR CACHE)
    unset(LEMON_INCLUDE_DIR CACHE)
    unset(LEMON_LIBRARY CACHE)
else()
    find_package(LEMON QUIET NO_MODULE)
    find_package(LEMON REQUIRED)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")  # enable assert
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")  # enable assert
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS}")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG}")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fopenmp -Wno-int-in-bool-context -Wno-sign-compare")
    # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS}")
    # set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")
endif()

if(MSVC)
    # Enable M_PI and disable fopen() etc. warnings
    ADD_DEFINITIONS(-D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS) 
    # Disable the following warnings : C4267 and 4244 (conversions) C4018 (sign compare) and C4800 (bool to int)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /wd4267 /wd4244 /wd4018 /wd4800") 
endif()

if (BUILD_PERFORMANCE_TEST)
    add_definitions(-DPERFORMANCE_TEST)
endif(BUILD_PERFORMANCE_TEST)

if (BUILD_OPENMP)
    add_definitions(-DWITH_OMP)
endif(BUILD_OPENMP)

if (BUILD_LOG)
    add_definitions(-DLOG_OUTPUT)
endif(BUILD_LOG)

if (BUILD_GUROBI)
    add_definitions(-DWITH_GUROBI)
endif(BUILD_GUROBI)

if (BUILD_TBB)
    add_definitions(-DWITH_TBB)
endif(BUILD_TBB)

if (BUILD_FREE_LICENSE)
    add_definitions(-DEIGEN_MPL2_ONLY)
endif(BUILD_FREE_LICENSE)

set(ScaleUntrim_LIBRARIES
    ${CMAKE_PROJECT_NAME}
    ${TBB_LIBRARIES}
    ${LEMON_LIBRARIES}
    ${GUROBI_LIBRARIES}
    ${OpenCASCADE_LIBRARIES}
)

set(ScaleUntrim_INC_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd/pcg32
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd/pss
    ${Boost_INCLUDE_DIRS}
    ${EIGEN_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
    ${LEMON_INCLUDE_DIRS}
    ${TBB_INCLUDE_DIRS}
    ${GUROBI_INCLUDE_DIRS}
    ${OpenCASCADE_INCLUDE_DIR}
)

# build source
add_subdirectory(source)

# build example
add_subdirectory(example)
