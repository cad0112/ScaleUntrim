
# include 
list(APPEND ScaleUntrim_INC_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/MergeFit ${CMAKE_CURRENT_SOURCE_DIR}/QuadriFlow ${CMAKE_CURRENT_SOURCE_DIR}/Commands)
set(ScaleUntrim_INC_DIRS ${ScaleUntrim_INC_DIRS} PARENT_SCOPE)

#
include_directories(${ScaleUntrim_INC_DIRS})
file(GLOB project_sources "MergeFit/*.cpp" "QuadriFlow/*.cpp" "Commands/*.cpp")
add_library(${CMAKE_PROJECT_NAME} STATIC ${project_sources})

# lib
list(APPEND ScaleUntrim_LIBRARIES ${CMAKE_PROJECT_NAME})
set(ScaleUntrim_LIBRARIES ${ScaleUntrim_LIBRARIES} PARENT_SCOPE)
