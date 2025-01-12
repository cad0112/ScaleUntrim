#!/bin/bash

# model_name=surface_cut_conner
# model_name=cube
# model_name=cube_with_hole
model_name=face_with_hole

./build/example/untrim -i ./example/models/${model_name}.stp -o ./build/example/${model_name}_untrim.stp -c ./build/example/setting.config 
