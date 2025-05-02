#!/bin/bash

rm -r build-windows
mkdir build-windows
cd build-windows
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
mingw32-make -j$(nproc)
cd ..

echo "Copying data files..."
mkdir -p windows-bin
mkdir -p windows-bin/data
cp -ru data/* windows-bin/data
cp build-windows/TileRummy2D.exe windows-bin

echo "Done. The game is in the windows-bin directory."
