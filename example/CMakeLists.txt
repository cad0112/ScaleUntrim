
# generate config file
set(test_temp_dir ${CMAKE_CURRENT_BINARY_DIR})
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/setting.config.in
    ${CMAKE_CURRENT_BINARY_DIR}/setting.config
)

# build example
include_directories(${ScaleUntrim_INC_DIRS})
## untrim
set(untrim_src untrim.cpp)
add_executable(untrim ${untrim_src})
target_link_libraries(untrim
                      ${ScaleUntrim_LIBRARIES})
## stl2cad
set(stl2cad_src stl2cad.cpp)
add_executable(stl2cad ${stl2cad_src})
target_link_libraries(stl2cad
                      ${ScaleUntrim_LIBRARIES})
## cad2stl
set(cad2stl_src cad2stl.cpp)
add_executable(cad2stl ${cad2stl_src})
target_link_libraries(cad2stl
                      ${ScaleUntrim_LIBRARIES})
