#!/bin/sh

mkdir src/

mv *.cpp src/ && mv *.hpp /src

mkdir build/
cd build/
cmake ..
make
cd ..
./build/main "$@"
