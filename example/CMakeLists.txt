
# generate config file
set(test_temp_dir ${CMAKE_CURRENT_BINARY_DIR})
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/setting.config.in
    ${CMAKE_CURRENT_BINARY_DIR}/setting.config
)

# generate test exe
set(test_src test.cpp)
add_executable(test ${test_src})
target_link_libraries(test
                      ${ScaleUntrim_LIBRARIES})
