#!/bin/bash
set -e

echo "Extracting Boost..."
tar -xzf libs/boost.tar.gz -C libs/
echo "Extracting SFML..."
tar -xzf libs/sfml.tar.gz -C libs/


echo "Building TileRummy2D..."
mkdir -p build
cd build
cmake ..
make -j$(nproc)

echo "Copying data files..."
cd ..
mkdir -p bin
cp data/* -r bin
cp build/TileRummy2D bin

echo "Done. The game is in the bin directory."
