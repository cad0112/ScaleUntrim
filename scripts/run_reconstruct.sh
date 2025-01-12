#!/bin/bash

model_name=surface_cut_conner
# model_name=MyTop3d1205
# model_name=cube_with_hole

./build/example/stl2cad -i ./example/models/${model_name}.stl -o ./build/example/${model_name}.stp -c ./build/example/setting.config
