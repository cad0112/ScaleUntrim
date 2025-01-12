
# generate config file
set(test_temp_dir ${CMAKE_CURRENT_BINARY_DIR})
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/setting.config.in
    ${CMAKE_CURRENT_BINARY_DIR}/setting.config
)

# build example
include_directories(${ScaleUntrim_INC_DIRS})
set(untrim_src untrim.cpp)
add_executable(untrim ${untrim_src})
target_link_libraries(untrim
                      ${ScaleUntrim_LIBRARIES})
